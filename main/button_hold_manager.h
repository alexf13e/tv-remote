
#ifndef BUTTON_HOLD_MANAGER_H
#define BUTTON_HOLD_MANAGER_H

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "freertos/timers.h"

#include "actionlist.h"
#include "clicker.h"
#include "freertos/projdefs.h"

namespace ButtonHoldManager {

    constexpr uint32_t hold_repeat_delay = 350;
    constexpr uint32_t hold_repeat_interval = 200;
    constexpr uint32_t reclick_delay = 50;

    std::string current_held_action_list;
    TimerHandle_t timer_hold_repeat_delay, timer_hold_repeat_interval, timer_clicker_reclick;

    bool initialised = false;


    void on_clicker_reclick(TimerHandle_t timer)
    {
        Clicker::up();
    }

    void begin_repeat(TimerHandle_t timer)
    {
        //button has been held for initial delay, begin a timer for repeating click
        xTimerStart(timer_hold_repeat_interval, 0);
    }

    void on_repeat(TimerHandle_t timer)
    {
        if (ActionListRunner::request_run(current_held_action_list))
        {
            //only click for feedback of action actually happening
            Clicker::down();
        }
        
        xTimerStart(timer_clicker_reclick, 0);
    }

    void init()
    {
        if (initialised)
        {
            std::cerr << "attempted to initialise button hold manager more than once" << std::endl;
            return;
        }

        Clicker::init();

        timer_hold_repeat_delay = xTimerCreate("timer_hold_repeat_delay", pdMS_TO_TICKS(hold_repeat_delay), pdFALSE, 0,
            begin_repeat);

        timer_hold_repeat_interval = xTimerCreate("timer_hold_repeat_interval", pdMS_TO_TICKS(hold_repeat_interval),
            pdTRUE, 0, on_repeat);

        timer_clicker_reclick = xTimerCreate("timer_clicker_reclick", pdMS_TO_TICKS(hold_repeat_interval -
            reclick_delay), pdFALSE, 0, on_clicker_reclick);

        initialised = true;
    }

    void press(const std::string& action_list_id, const bool repeat_on_hold)
    {
        if (!initialised)
        {
            std::cerr << "attempted to run button hold manager before initialisation" << std::endl;
            return;
        }

        if (action_list_id == "") Clicker::down(); //button is just for doing things with ui, should always click
        else
        {
            //button wants to run ir action, only give feedback if it actually runs
            if (ActionListRunner::request_run(action_list_id))
            {
                Clicker::down();
            }

            //even if the initial request couldnt be run, holding the button down should allow for the action to start
            //running as soon as possible
            if (repeat_on_hold)
            {
                current_held_action_list = action_list_id;
                xTimerStart(timer_hold_repeat_delay, 0);
            }
        }
    }

    void release()
    {
        if (!initialised)
        {
            std::cerr << "attempted to run button hold manager before initialisation" << std::endl;
            return;
        }

        xTimerStop(timer_hold_repeat_delay, 0);
        xTimerStop(timer_hold_repeat_interval, 0);

        Clicker::up();
        current_held_action_list = "";
    }
}

#endif