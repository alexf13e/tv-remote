
#ifndef BACKLIGHT_H
#define BACKLIGHT_H

#include <algorithm>
#include <cmath>
#include <iostream>

#include "driver/ledc.h"
#include "esp_err.h"
#include "hal/ledc_types.h"
#include "soc/clk_tree_defs.h"
#include "soc/gpio_num.h"

namespace Backlight {
    constexpr ledc_timer_bit_t DUTY_RES = LEDC_TIMER_10_BIT;
    constexpr uint16_t MAX_DUTY = 1 << DUTY_RES;
    constexpr ledc_mode_t SPEED_MODE = LEDC_LOW_SPEED_MODE;
    constexpr ledc_channel_t CHANNEL = LEDC_CHANNEL_0;
    constexpr gpio_num_t GPIO_OUTPUT_NUM = GPIO_NUM_20;

    ledc_timer_config_t timer_config;
    ledc_channel_config_t channel_config;
    bool initialised = false;


    void init()
    {
        timer_config = {
            .speed_mode = SPEED_MODE,
            .duty_resolution = DUTY_RES,
            .timer_num = LEDC_TIMER_0,
            .freq_hz = 50000,
            .clk_cfg = LEDC_AUTO_CLK,
        };

        ESP_ERROR_CHECK(ledc_timer_config(&timer_config));

        channel_config = {
            .gpio_num = GPIO_OUTPUT_NUM,
            .speed_mode = SPEED_MODE,
            .channel = CHANNEL,
            .intr_type = LEDC_INTR_DISABLE,
            .timer_sel = LEDC_TIMER_0,
            .duty = (uint32_t)(MAX_DUTY * 0.5f),
            .hpoint = 100,
        };

        ESP_ERROR_CHECK(ledc_channel_config(&channel_config));

        initialised = true;
    }


    void set_brightness(float level)
    {
        if (!initialised)
        {
            std::cerr << "attempted to set backlight brightness before initialising ledc" << std::endl;
            return;
        }

        level = std::clamp(level, 0.01f, 0.99f);
        uint16_t duty_level = level * MAX_DUTY;
        ESP_ERROR_CHECK(ledc_set_duty(SPEED_MODE, CHANNEL, duty_level));
        ESP_ERROR_CHECK(ledc_update_duty(SPEED_MODE, CHANNEL));
    }
}

#endif