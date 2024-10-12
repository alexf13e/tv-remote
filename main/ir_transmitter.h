
#ifndef IR_TRANSMITTER_H
#define IR_TRANSMITTER_H

#include "driver/rmt_common.h"
#include "driver/rmt_encoder.h"
#include "driver/rmt_tx.h"
#include "driver/rmt_types.h"
#include "esp_err.h"
#include "hal/rmt_types.h"
#include "soc/gpio_num.h"


namespace IRTransmitter {
    rmt_channel_handle_t channel_handle;
    rmt_tx_channel_config_t channel_config;
    rmt_carrier_config_t carrier_config;
    rmt_transmit_config_t transmit_config;
    rmt_encoder_handle_t encoder_handle;
    rmt_copy_encoder_config_t encoder_config;

    void init()
    {
        channel_handle = NULL;
        channel_config = {
            .gpio_num = GPIO_NUM_6,
            .clk_src = rmt_clock_source_t::RMT_CLK_SRC_DEFAULT,
            .resolution_hz = 1 * 1000 * 1000,
            .mem_block_symbols = 64,
            .trans_queue_depth = 4,
            .flags = {
                .invert_out = false,
            }
        };

        ESP_ERROR_CHECK(rmt_new_tx_channel(&channel_config, &channel_handle));
        ESP_ERROR_CHECK(rmt_enable(channel_handle));
        
        
        carrier_config = {
            .frequency_hz = 38000,
            .duty_cycle = 0.33,
            .flags = {
                .polarity_active_low = false,
            }
        };

        ESP_ERROR_CHECK(rmt_apply_carrier(channel_handle, &carrier_config));

        
        transmit_config = {
            .loop_count = 0
        };


        encoder_handle = NULL;
        encoder_config = {};

        ESP_ERROR_CHECK(rmt_new_copy_encoder(&encoder_config, &encoder_handle));
    }

    void transmit(rmt_symbol_word_t* data, size_t length)
    {
        ESP_ERROR_CHECK(rmt_transmit(channel_handle, encoder_handle, data, length * sizeof(rmt_symbol_word_t), &transmit_config));
    }
}

#endif