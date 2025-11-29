
#ifndef CLICKER_H
#define CLICKER_H

#include <iostream>

#include "driver/gpio.h"
#include "hal/gpio_types.h"
#include "soc/gpio_num.h"


//for button press feedback, activate a relay on and off
namespace Clicker
{
    constexpr gpio_num_t GPIO_NUM_CLICKER = GPIO_NUM_15;
    constexpr uint32_t CLICKER_INACTIVE = 0;
    constexpr uint32_t CLICKER_ACTIVE = 1;
    bool initialised = false;

    void init()
    {
        if (initialised)
        {
            std::cerr << "attempted to intialise clicker more than once" << std::endl;
            return;
        }
        
        gpio_config_t gpio_clicker_config = {
            .pin_bit_mask = 1ull << GPIO_NUM_CLICKER,
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_ENABLE,
            .intr_type = GPIO_INTR_DISABLE,
        };

        gpio_config(&gpio_clicker_config);
        gpio_hold_dis(GPIO_NUM_CLICKER);
        gpio_set_level(GPIO_NUM_CLICKER, CLICKER_INACTIVE);

        initialised = true;
    }

    void down()
    {
        if (!initialised)
        {
            std::cerr << "attempted to run clicker before initialisation" << std::endl;
            return;
        }

        gpio_set_level(GPIO_NUM_CLICKER, CLICKER_ACTIVE);
    }

    void up()
    {
        if (!initialised)
        {
            std::cerr << "attempted to run clicker before initialisation" << std::endl;
            return;
        }

        gpio_set_level(GPIO_NUM_CLICKER, CLICKER_INACTIVE);
    }

    void sleep()
    {
        //prevent click on sleep/wakeup. needs gpio_hold_dis during init
        gpio_set_level(GPIO_NUM_CLICKER, CLICKER_INACTIVE);
        gpio_hold_en(GPIO_NUM_CLICKER);
    }
}

#endif