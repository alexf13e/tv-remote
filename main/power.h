
#ifndef POWER_H
#define POWER_H

#include <cstdint>
#include <iostream>
#include <string>
#include <unistd.h>

#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_sleep.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"
#include "hal/gpio_types.h"
#include "hal/i2c_types.h"
#include "soc/gpio_num.h"
#include "driver/rtc_io.h"
#include "ESP_IOExpander_Library.h"
#include "esp_pm.h"

#include "i2c_init.h"
#include "sleep_memory.h"
#include "event_timers.h"
#include "screen.h"
#include "clicker.h"

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

    constexpr gpio_num_t RTC_GPIO_NUM_MOTION_DETECT = GPIO_NUM_19;
    constexpr uint8_t EXIO_DISPLAY = 2;
    constexpr uint32_t INACTIVITY_SLEEP_TIMEOUT = 40000;
    constexpr uint32_t MOTION_DETECT_POLL_INTERVAL = 500;

    ESP_IOExpander_CH422G* io_expander;
    TimerHandle_t timer_poll_motion_detect, timer_enter_sleep;
    bool sleep_enabled = true;
    bool motion_detected = false;

    bool initialised = false;

    void refresh_sleep_timeout()
    {
        if (sleep_enabled) xTimerReset(timer_enter_sleep, 0);
    }

    void enable_sleep()
    {
        sleep_enabled = true;
        refresh_sleep_timeout();
    }

    void disable_sleep()
    {
        sleep_enabled = false;
        xTimerStop(timer_enter_sleep, 0);
    }

    void sleep()
    {
        if (!initialised)
        {
            std::cerr << "attempted to sleep before initialising power management" << std::endl;
            return;
        }

        Timers::stop_all();
        Screen::destroy();
        Clicker::sleep();

        io_expander->digitalWrite(EXIO_DISPLAY, 0); //turn the display backlight off to save power
        SleepMemory::save(); //save data which should be remembered through sleep

        //clear isr and gpio setup for motion detect pin
        ESP_ERROR_CHECK(gpio_isr_handler_remove(RTC_GPIO_NUM_MOTION_DETECT));
        ESP_ERROR_CHECK(gpio_reset_pin(RTC_GPIO_NUM_MOTION_DETECT));
        
        //configure the pin to wake up the device when it goes high
        ESP_ERROR_CHECK(esp_sleep_enable_ext0_wakeup(RTC_GPIO_NUM_MOTION_DETECT, 1));
        ESP_ERROR_CHECK(rtc_gpio_pulldown_en(RTC_GPIO_NUM_MOTION_DETECT));
        ESP_ERROR_CHECK(rtc_gpio_pullup_dis(RTC_GPIO_NUM_MOTION_DETECT));

        std::cout << "sleep time" << std::endl;
        esp_deep_sleep_start();
    }

    void timer_enter_sleep_callback(TimerHandle_t xTimer)
    {
        sleep();
    }

    static void IRAM_ATTR gpio_motion_isr_handler(void* arg)
    {
        //whenever a gpio event occurs, this function is called
        motion_detected = true;
    }

    static void poll_motion_detect_callback(TimerHandle_t xTimer)
    {
        if (motion_detected) {
            //motion detection switch was activated since last check, so reset time until sleep
            refresh_sleep_timeout();
            motion_detected = false;
        }
    }

    void init()
    {
        if (initialised)
        {
            std::cerr << "attempted initialise power more than once" << std::endl;
            return;
        }

        esp_sleep_wakeup_cause_t wakeup_cause = esp_sleep_get_wakeup_cause();
        std::cout << "wakeup cause: " << wakeup_cause_strings[wakeup_cause] << std::endl;
        if (wakeup_cause == ESP_SLEEP_WAKEUP_EXT0) ESP_ERROR_CHECK(rtc_gpio_deinit(RTC_GPIO_NUM_MOTION_DETECT));

        i2c_init(); //initialise i2c if nothing else has

        //for a reason unknown to me, adding this power config prevents an issue where holding a button on the ui causes
        //it to be released and pressed automatically 1-2 times per second...
        esp_pm_config_t pm_config = {
            .max_freq_mhz = 240,
            .min_freq_mhz = 80,
        };

        ESP_ERROR_CHECK(esp_pm_configure(&pm_config));


        //configure the io expander chip so the backlight enable pin can be accessed
        //this can only be used for on/off and not brightness control, as the chip cannot forward a pwm
        io_expander = new ESP_IOExpander_CH422G(I2C_NUM_0, ESP_IO_EXPANDER_I2C_CH422G_ADDRESS);
        io_expander->init();
        io_expander->begin();
        io_expander->pinMode(EXIO_DISPLAY, OUTPUT);


        //configure the wakeup pin to run an interrupt when it goes high, to reset a countdown where if no activity
        //is detected for a set time, the device goes to sleep
        gpio_config_t gpio_wakeup_config = {
            .pin_bit_mask = 1ull << RTC_GPIO_NUM_MOTION_DETECT,
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_POSEDGE,
        };

        ESP_ERROR_CHECK(gpio_config(&gpio_wakeup_config));

        ESP_ERROR_CHECK(gpio_install_isr_service(0));
        ESP_ERROR_CHECK(gpio_isr_handler_add(RTC_GPIO_NUM_MOTION_DETECT, gpio_motion_isr_handler,
            (void*)RTC_GPIO_NUM_MOTION_DETECT));
        
        timer_poll_motion_detect = xTimerCreate("timer_poll_motion_detect", pdMS_TO_TICKS(MOTION_DETECT_POLL_INTERVAL),
            pdTRUE, 0, poll_motion_detect_callback);
        
        timer_enter_sleep = xTimerCreate("timer_enter_sleep", pdMS_TO_TICKS(INACTIVITY_SLEEP_TIMEOUT), pdFALSE, 0,
            timer_enter_sleep_callback);

        xTimerStart(timer_poll_motion_detect, 0);
        xTimerStart(timer_enter_sleep, 0);

        Timers::store(timer_poll_motion_detect);
        Timers::store(timer_enter_sleep);

        initialised = true;
    }
}


#endif