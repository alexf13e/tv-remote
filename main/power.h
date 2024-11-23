
#ifndef POWER_H
#define POWER_H

#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>
#include <thread>

#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_sleep.h"
#include "freertos/idf_additions.h"
#include "hal/gpio_types.h"
#include "hal/i2c_types.h"
#include "portmacro.h"
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
    constexpr uint8_t EXIO_DISPLAY = 2;
    constexpr std::chrono::milliseconds INACTIVITY_SLEEP_TIMEOUT = std::chrono::milliseconds(20000);

    ESP_IOExpander_CH422G* io_expander;
    QueueHandle_t gpio_event_queue = NULL;
    std::chrono::system_clock::time_point sleep_timeout_time;
    std::thread thread_sleep_timeout;
    bool sleep_enabled = true;

    bool initialised = false;

    void enable_sleep()
    {
        sleep_timeout_time = std::chrono::system_clock::now() + INACTIVITY_SLEEP_TIMEOUT;
        sleep_enabled = true;
    }

    void disable_sleep()
    {
        sleep_enabled = false;
    }

    void sleep()
    {
        if (!initialised)
        {
            std::cerr << "attempted to sleep before initialising power management" << std::endl;
            return;
        }

        io_expander->digitalWrite(EXIO_DISPLAY, 0); //turn the display backlight off to save power
        
        std::this_thread::sleep_for(std::chrono::milliseconds(200)); //seems to prevent TG1WDT_SYS_RST from occurring

        //configure the pin with the motion switch to wake up the device when it goes high
        ESP_ERROR_CHECK(esp_sleep_enable_ext0_wakeup(RTC_GPIO_NUM_WAKEUP, 1));
        ESP_ERROR_CHECK(rtc_gpio_pulldown_en(RTC_GPIO_NUM_WAKEUP));
        ESP_ERROR_CHECK(rtc_gpio_pullup_dis(RTC_GPIO_NUM_WAKEUP));

        esp_deep_sleep_start();
    }

    void sleep_timeout()
    {
        //potential confusion: we are using thread sleeping to pause some amount of time before checking if the device
        //should be put into a low power state which is also called sleeping

        while (true)
        {
            std::chrono::system_clock::time_point current_sleep_timeout_time = sleep_timeout_time;
            
            //begin thread sleep until the currently set timeout time
            if (sleep_enabled)
            {
                std::this_thread::sleep_until(current_sleep_timeout_time);
            }
            
            if (!sleep_enabled) //device sleep may have been disabled during previous thread sleep
            {
                //sleeping has been disabled, so delay a little and then restart loop
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                continue;
            }

            //if the sleep timeout time has not been updated, we now want to put the device to sleep
            if (current_sleep_timeout_time == sleep_timeout_time)
            {
                sleep();
            }
        }
    }

    static void IRAM_ATTR gpio_isr_handler(void* arg)
    {
        //whenever a gpio event occurs, this function is called, which forwards the event to a queue which will handle
        //the event later. the queue is handled by gpio_task_motion_detect
        uint32_t gpio_num = (uint32_t)arg;
        xQueueSendFromISR(gpio_event_queue, &gpio_num, NULL);
    }

    static void gpio_task_motion_detect(void* arg)
    {
        //constantly check if there is an item in the queue to work on.
        //the motion detector bounces several times, only really interested in if there was 1 event in the past few
        //100ms.
        //save processing by only checking and acting on interrupts every few 100ms.
        //if there was an interrupt in the queue, act on it, clear the rest from the queue, wait a bit before checking
        //again.
        
        uint32_t gpio_num;
        while (true) {
            if (xQueueReceive(gpio_event_queue, &gpio_num, portMAX_DELAY)) {
                //there was an event in the queue, act on it
                sleep_timeout_time = std::chrono::system_clock::now() + INACTIVITY_SLEEP_TIMEOUT;

                //clear the rest of the queue as we don't care how many other times the motion detector switch activated
                xQueueReset(gpio_event_queue);

                //wait a bit before checking the queue again
                vTaskDelay(500 / portTICK_PERIOD_MS);
            }
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
        if (wakeup_cause == ESP_SLEEP_WAKEUP_EXT0) ESP_ERROR_CHECK(rtc_gpio_deinit(RTC_GPIO_NUM_WAKEUP));

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
            .pin_bit_mask = 1ull << RTC_GPIO_NUM_WAKEUP,
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_POSEDGE,
        };

        gpio_config(&gpio_wakeup_config);

        gpio_event_queue = xQueueCreate(10, sizeof(uint32_t));
        xTaskCreate(gpio_task_motion_detect, "gpio_task_motion_detect", 2048, NULL, 10, NULL);

        gpio_install_isr_service(0);
        gpio_isr_handler_add(RTC_GPIO_NUM_WAKEUP, gpio_isr_handler, (void*)RTC_GPIO_NUM_WAKEUP);

        sleep_timeout_time = std::chrono::system_clock::now() + INACTIVITY_SLEEP_TIMEOUT;
        thread_sleep_timeout = std::thread(sleep_timeout);

        initialised = true;
    }
}


#endif