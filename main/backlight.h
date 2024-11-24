
#ifndef BACKLIGHT_H
#define BACKLIGHT_H

#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>

#include "driver/adc_types_legacy.h"
#include "driver/ledc.h"
#include "driver/adc.h"
#include "esp_err.h"
#include "hal/adc_types.h"
#include "hal/ledc_types.h"
#include "soc/clk_tree_defs.h"
#include "soc/gpio_num.h"


namespace Backlight {
    constexpr ledc_timer_bit_t DUTY_RES = LEDC_TIMER_10_BIT;
    constexpr uint32_t MAX_DUTY = 1 << DUTY_RES;
    constexpr ledc_mode_t SPEED_MODE = LEDC_LOW_SPEED_MODE;
    constexpr ledc_channel_t CHANNEL = LEDC_CHANNEL_0;
    constexpr gpio_num_t GPIO_OUTPUT_NUM_PWM = GPIO_NUM_20;

    constexpr adc2_channel_t ADC_CHAN_LDR = ADC2_CHANNEL_1;
    constexpr adc_atten_t ADC_ATTEN_LDR = ADC_ATTEN_DB_12;
    constexpr adc_bits_width_t ADC_BIT_WIDTH_LDR = ADC_WIDTH_BIT_12;

    constexpr uint32_t BRIGHTNESS_UPDATE_INTERVAL_MS = 2000;

    ledc_timer_config_t timer_config;
    ledc_channel_config_t channel_config;
    std::thread thread_ldr_auto_brightness;
    bool auto_brightness_enabled = true;

    bool initialised = false;

    uint32_t brightness_level_to_duty(float level)
    {
        //screen appears brightest at around 0.5 duty cycle, then drops off a little towards 1.0
        uint32_t max_brightness_duty = 0.5f * MAX_DUTY;
        return level * max_brightness_duty;
    }

    float scale_perceived_brightness(float level)
    {
        //take in a brightness level from 0 to 1 and scale to follow CIE perceived brightness curve as described here
        //https://blog.mbedded.ninja/programming/firmware/controlling-led-brightness-using-pwm/#adjusting-for-our-eyes-perceived-brightness
        //https://www.desmos.com/calculator/6bflyfsp6w

        //adapted for screen by scaling between min_brightness and 1, as below around 5% duty the backlight turns off
        //fully turning the backlight off for power saving is done just before sleep in power.h
        const float min_brightness = 0.06;
        
        if (level < 0.08f)
        {
            level = level / 9.033f * (1.0f - min_brightness) + min_brightness;
        }
        else
        {
            level = (level + 0.16f) / 1.16f;
            level = level * level * level;
            level = level * (1.0f - min_brightness) + min_brightness;
        }

        //ensure level between 0 and 1
        return std::clamp(level, 0.01f, 0.99f);
    }

    void set_brightness(float level, bool apply_perception_scale)
    {
        if (!initialised)
        {
            std::cerr << "attempted to set backlight brightness before initialising ledc" << std::endl;
            return;
        }

        if (apply_perception_scale) level = scale_perceived_brightness(level);

        ESP_ERROR_CHECK(ledc_set_duty(SPEED_MODE, CHANNEL, brightness_level_to_duty(level)));
        ESP_ERROR_CHECK(ledc_update_duty(SPEED_MODE, CHANNEL));
    }

    void fade_brightness(float level, uint32_t duration_ms, bool apply_perception_scale)
    {
        if (!initialised)
        {
            std::cerr << "attempted to set backlight brightness before initialising ledc" << std::endl;
            return;
        }

        if (apply_perception_scale) level = scale_perceived_brightness(level);

        ESP_ERROR_CHECK(ledc_set_fade_with_time(SPEED_MODE, CHANNEL, brightness_level_to_duty(level), duration_ms));
        ESP_ERROR_CHECK(ledc_fade_start(SPEED_MODE, CHANNEL, LEDC_FADE_NO_WAIT));
    }

    int read_ldr()
    {
        if (!initialised)
        {
            std::cerr << "attempted to read ldr for backlight before initialising" << std::endl;
            return 0;
        }

        int value = 0;
        ESP_ERROR_CHECK(adc2_get_raw(ADC_CHAN_LDR, ADC_BIT_WIDTH_LDR, &value));

        return value;
    }

    void update_brightness_from_ldr()
    {
        while (true)
        {
            int ldr_reading = read_ldr();
            int max_reading = 1 << ADC_BIT_WIDTH_LDR;
            float normalised_reading = (float)ldr_reading / max_reading;
            float brightness_level = 1.0f - normalised_reading; //ldr reading is higher when darker

            //if target duty is close to the current duty (i.e. small/no brightness change), the fade will complete
            //immediately when blocking.
            //only want to adjust the brightness once every second with a smooth fade to reduce flickering.
            //run the fade without waiting to stop, then sleep the thread for desired update interval to ensure the next
            //brightness update is delayed. simpler this way than blocking, checking how long it actually took, then
            //waiting the remainder
            if (auto_brightness_enabled)
            {
                fade_brightness(brightness_level, BRIGHTNESS_UPDATE_INTERVAL_MS, true);
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(BRIGHTNESS_UPDATE_INTERVAL_MS + 50));
        }
    }

    void init()
    {
        if (initialised)
        {
            std::cerr << "attempted to initialise backlight more than once" << std::endl;
            return;
        }

        timer_config = {
            .speed_mode = SPEED_MODE,
            .duty_resolution = DUTY_RES,
            .timer_num = LEDC_TIMER_0,
            .freq_hz = 30000,
            .clk_cfg = LEDC_AUTO_CLK,
        };

        ESP_ERROR_CHECK(ledc_timer_config(&timer_config));

        channel_config = {
            .gpio_num = GPIO_OUTPUT_NUM_PWM,
            .speed_mode = SPEED_MODE,
            .channel = CHANNEL,
            .intr_type = LEDC_INTR_DISABLE,
            .timer_sel = LEDC_TIMER_0,
            .duty = (uint32_t)(MAX_DUTY * 0.5f),
            .hpoint = 100,
        };

        ESP_ERROR_CHECK(ledc_channel_config(&channel_config));

        ESP_ERROR_CHECK(ledc_fade_func_install(0));
        ESP_ERROR_CHECK(adc2_config_channel_atten(ADC_CHAN_LDR, ADC_ATTEN_LDR));

        initialised = true;

        set_brightness(0.5, true);
        thread_ldr_auto_brightness = std::thread(update_brightness_from_ldr);
    }
}

#endif