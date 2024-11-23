
#ifndef IR_RECEIVER_H
#define IR_RECEIVER_H

#define OUTPUT_EXTRA_SIGNAL_DATA false

#include <iostream>

#include "driver/rmt_rx.h"
#include "driver/rmt_types.h"
#include "esp_err.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"
#include "hal/rmt_types.h"
#include "portmacro.h"
#include "soc/clk_tree_defs.h"
#include "soc/gpio_num.h"


//using IRSignalContainer for time_unit and symbol words which encode zero and one
//if a more generalised system for handling remote signals is required, both IRSignalContainer and IRReceiver will need
//significant refactoring
#include "IRSignalContainer.h"


namespace IRReceiver
{
    constexpr gpio_num_t GPIO_NUM_RECEIVE = GPIO_NUM_16;
    constexpr uint16_t word_duration_tolerance = 150; //how much tolerance is given around the duration of a high or low
                                                      //part of a signal word

    rmt_channel_handle_t channel_handle;
    rmt_rx_channel_config_t channel_config;
    QueueHandle_t receive_queue;
    rmt_rx_event_callbacks_t callback_config;
    rmt_receive_config_t receive_config;
    rmt_symbol_word_t raw_symbols[64];
    rmt_rx_done_event_data_t rx_data;
    bool initialised = false;


    static bool receive_callback(rmt_channel_handle_t channel, const rmt_rx_done_event_data_t *received_data,
        void *receive_queue)
    {
        //send the received RMT symbols to the parser task
        BaseType_t high_task_wakeup = pdFALSE;
        xQueueSendFromISR((QueueHandle_t)receive_queue, received_data, &high_task_wakeup);

        // return whether any task is woken up
        return high_task_wakeup == pdTRUE;
    }

    uint64_t decode_word(const rmt_symbol_word_t& word)
    {
        //check which symbol a word describes, accounting for variation and tolerance in received data
        
        auto within_tolerance = [&](const int duration1, const int duration2) {
            //durations cast to int to allow negative difference
            return abs(duration1 - duration2) < word_duration_tolerance;
        };

        auto compare_symbols = [&](const rmt_symbol_word_t& sym1, const rmt_symbol_word_t& sym2) {
            return sym1.level0 == sym2.level0 && within_tolerance(sym1.duration0, sym2.duration0) &&
                   sym1.level1 == sym2.level1 && within_tolerance(sym1.duration1, sym2.duration1);
        };

        if (compare_symbols(word, IRSignalContainer::symbol_zero)) return 0;
        else if (compare_symbols(word, IRSignalContainer::symbol_one)) return 1;
        else return -1;
    }

    void init()
    {
        if (initialised)
        {
            std::cerr << "attempted to initialise IR receiver more than once" << std::endl;
            return;
        }

        //create channel for rmt peripheral to receive signal with specified gpio pin
        channel_handle = NULL;
        channel_config = {
            .gpio_num = GPIO_NUM_RECEIVE,
            .clk_src = RMT_CLK_SRC_DEFAULT,
            .resolution_hz = 1 * 1000 * 1000,
            .mem_block_symbols = 64,
            .flags = {
                .invert_in = true,
            }
        };

        ESP_ERROR_CHECK(rmt_new_rx_channel(&channel_config, &channel_handle));
        ESP_ERROR_CHECK(rmt_enable(channel_handle));


        //create a queue for received data to be put in before being processed
        receive_queue = xQueueCreate(1, sizeof(rmt_rx_done_event_data_t));

        //specify the function to be called when the channel receives something
        callback_config = {
            .on_recv_done = receive_callback,
        };

        //register the callback function for the specified channel and queue of data
        ESP_ERROR_CHECK(rmt_rx_register_event_callbacks(channel_handle, &callback_config, receive_queue));


        receive_config = {
            .signal_range_min_ns = 1250, //minimum duration of signal to be considered valid
            .signal_range_max_ns = 10 * 1000 * 1000, //maximum duration of signal before being cut off
        };

        initialised = true;
    }

    void decode_signal()
    {
        rmt_symbol_word_t word;

        #if OUTPUT_EXTRA_SIGNAL_DATA
        //print the timings for the start symbol of the signal
        word = rx_data.received_symbols[0];
        std::cout << "start symbol timings: " << word.level0 << "_" << word.duration0 << ", " <<  word.level1 << "_" <<
            word.duration1 << std::endl;
        #endif

        uint64_t output = 0;
        for (int i = 1; i < rx_data.num_symbols - 1; i++)
        {
            word = rx_data.received_symbols[i];

            uint64_t decoded_word = decode_word(word);
            if (decoded_word == -1)
            {
                //word does not encode 0 or 1
                std::cout << "unexpected symbol with timings: " << word.level0 << "_" << word.duration0 << ", " << 
                    word.level1 << "_" << word.duration1 << std::endl;
                std::cout << "decoded signal likely will not work" << std::endl;
            }
            else
            {
                output |= (decoded_word << (i - 1));
                #if OUTPUT_EXTRA_SIGNAL_DATA
                //print the timings for this word and what it was decoded as
                std::cout <<  word.level0 << "_" << word.duration0 << ", " <<  word.level1 << "_" << word.duration1 <<
                    " -> " << decoded_word << std::endl;
                #endif
            }            
        }

        #if OUTPUT_EXTRA_SIGNAL_DATA
        //print the timings for the end symbol of the word
        word = rx_data.received_symbols[rx_data.num_symbols - 1];
        std::cout << "end symbol timings: " << word.level0 << "_" << word.duration0 << ", " <<  word.level1 << "_" <<
            word.duration1 << std::endl << std::endl;
        #endif

        std::cout << "code to use in remote ACTION: " << std::hex << std::showbase << output << std::dec << std::endl;
    }

    void receive()
    {
        //wait until a signal is received, then decode it
        ESP_ERROR_CHECK(rmt_receive(channel_handle, raw_symbols, sizeof(raw_symbols), &receive_config));
        xQueueReceive(receive_queue, &rx_data, portMAX_DELAY);
        decode_signal();
    }
}

#endif