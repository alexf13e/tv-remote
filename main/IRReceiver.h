
#ifndef IR_RECEIVER_H
#define IR_RECEIVER_H

#include <iostream>
#include <iomanip>
#include <ios>

#include "driver/rmt_rx.h"
#include "driver/rmt_types.h"
#include "esp_err.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"
#include "hal/rmt_types.h"
#include "portmacro.h"
#include "soc/clk_tree_defs.h"
#include "soc/gpio_num.h"


namespace IRReceiver
{
    constexpr gpio_num_t GPIO_NUM_RECEIVE = GPIO_NUM_16;

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

    void print_signal()
    {
        //prints the received rmt_symbol_word_t values, which are 32 bit numbers describing the duration and level of
        //pairs in the signal.

        //attempts were made to find a way of describing signals which was flexible enough to support lots of remotes,
        //without having the codes be excessively long, to no avail.

        //it is likely possible to write code to identify the protocol being used by the signal, and also have different
        //methods of sending the signal for each one.

        //however, it is much simpler to just store the signal as it was received, and send it in the same manner.

        //since the code is the exact timings which were received, there will be slight differences between the codes
        //generated from the same button press.


        std::ios original_cout(nullptr);
        original_cout.copyfmt(std::cout);
        std::cout << std::hex << std::noshowbase << std::setw(8) << std::setfill('0');

        for (int i = 0; i < rx_data.num_symbols; i++)
        {
            std::cout << rx_data.received_symbols[i].val;
        }

        std::cout << std::endl << std::endl;
        std::cout.copyfmt(original_cout);
    }

    void receive()
    {
        //wait until a signal is received
        ESP_ERROR_CHECK(rmt_receive(channel_handle, raw_symbols, sizeof(raw_symbols), &receive_config));
        xQueueReceive(receive_queue, &rx_data, portMAX_DELAY);

        //display it in a format which can be copied and used when creating remote actions
        print_signal();
    }
}

#endif