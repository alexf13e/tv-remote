
#ifndef BUTTON_HOLD_MANAGER_H
#define BUTTON_HOLD_MANAGER_H

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "actionlist.h"
#include "clicker.h"

namespace ButtonHoldManager {

    constexpr std::chrono::milliseconds hold_repeat_delay = std::chrono::milliseconds(350);
    constexpr std::chrono::milliseconds repeat_interval = std::chrono::milliseconds(200);
    constexpr std::chrono::milliseconds clicker_reclick_delay = std::chrono::milliseconds(50);
    constexpr std::chrono::milliseconds repeater_poll_interval = std::chrono::milliseconds(50);

    std::string current_held_actionList;
    std::thread thread_repeater;

    bool initialised = false;

    void repeater()
    {
        while (true)
        {
            std::this_thread::sleep_for(repeater_poll_interval);
            if (current_held_actionList == "") continue;

            //a button has been pressed, begin delay before repeating
            const std::string desired_held_actionList = current_held_actionList;
            std::this_thread::sleep_for(hold_repeat_delay);

            while (current_held_actionList == desired_held_actionList)
            {
                //same button still being held, so repeat

                bool success = ActionListRunner::request_run(desired_held_actionList);
                if (success) Clicker::down(); //only click for feedback of action actually happening
                
                if (repeat_interval > clicker_reclick_delay) std::this_thread::sleep_for(repeat_interval - clicker_reclick_delay);
                
                if (success) Clicker::up();
                std::this_thread::sleep_for(clicker_reclick_delay);
            }
        }
    }

    void init()
    {
        if (initialised)
        {
            std::cerr << "attempted to initialise button hold manager more than once" << std::endl;
            return;
        }

        Clicker::init();
        thread_repeater = std::thread(repeater);

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
            if (repeat_on_hold) current_held_actionList = action_list_id;
        }
    }

    void release()
    {
        if (!initialised)
        {
            std::cerr << "attempted to run button hold manager before initialisation" << std::endl;
            return;
        }

        Clicker::up();
        current_held_actionList = "";
    }
}

#endif