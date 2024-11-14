
#ifndef POWER_H
#define POWER_H

#include <iostream>
#include <string>

#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_sleep.h"
#include "hal/gpio_types.h"
#include "hal/i2c_types.h"
#include "soc/gpio_num.h"
#include "driver/rtc_io.h"
#include "ESP_IOExpander_Library.h"
#include "esp_pm.h"

#include "i2c_init.h"


namespace Power
{
    std::string wakeup_cause_strings[] = {
        "ESP_SLEEP_WAKEUP_UNDEFINED",
        "ESP_SLEEP_WAKEUP_ALL",
        "ESP_SLEEP_WAKEUP_EXT0",
        "ESP_SLEEP_WAKEUP_EXT1",
        "ESP_SLEEP_WAKEUP_TIMER",
        "ESP_SLEEP_WAKEUP_TOUCHPAD",
        "ESP_SLEEP_WAKEUP_ULP",
        "ESP_SLEEP_WAKEUP_GPIO",
        "ESP_SLEEP_WAKEUP_UART",
        "ESP_SLEEP_WAKEUP_WIFI",
        "ESP_SLEEP_WAKEUP_COCPU",
        "ESP_SLEEP_WAKEUP_COCPU_TRAP_TRIG",
        "ESP_SLEEP_WAKEUP_BT",
    };

    constexpr gpio_num_t RTC_GPIO_NUM_WAKEUP = GPIO_NUM_19;
    constexpr uint64_t RTC_GPIO_MASK_WAKEUP = 1ull << RTC_GPIO_NUM_WAKEUP;

    constexpr uint8_t EXIO_DISPLAY = 2;

    ESP_IOExpander_CH422G* io_expander;

    bool power_initialised = false;


    void init()
    {
        if (power_initialised)
        {
            std::cerr << "attempted initialise power more than once" << std::endl;
            return;
        }


        esp_sleep_wakeup_cause_t wakeup_cause = esp_sleep_get_wakeup_cause();
        std::cout << "wakeup cause: " << wakeup_cause_strings[wakeup_cause] << std::endl;
        if (wakeup_cause == ESP_SLEEP_WAKEUP_EXT0) ESP_ERROR_CHECK(rtc_gpio_deinit(RTC_GPIO_NUM_WAKEUP));

        i2c_init(); //initialise i2c if nothing else has

        //for a reason unknown to me, adding this power config prevents an issue where holding a button on the ui causes
        //it to be released and pressed automatically 1-2 times per second...
        esp_pm_config_t pm_config = {
            .max_freq_mhz = 240,
            .min_freq_mhz = 80,
        };

        ESP_ERROR_CHECK(esp_pm_configure(&pm_config));


        io_expander = new ESP_IOExpander_CH422G(I2C_NUM_0, ESP_IO_EXPANDER_I2C_CH422G_ADDRESS);
        io_expander->init();
        io_expander->begin();
        io_expander->pinMode(EXIO_DISPLAY, OUTPUT);

        power_initialised = true;
    }

    void sleep()
    {
        io_expander->digitalWrite(EXIO_DISPLAY, 0);

        ESP_ERROR_CHECK(esp_sleep_enable_ext0_wakeup(RTC_GPIO_NUM_WAKEUP, 1));
        ESP_ERROR_CHECK(rtc_gpio_pulldown_en(RTC_GPIO_NUM_WAKEUP));
        ESP_ERROR_CHECK(rtc_gpio_pullup_dis(RTC_GPIO_NUM_WAKEUP));
        esp_deep_sleep_start();
    }
}


#endif