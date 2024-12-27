
//data which wants to be remembered through sleep should be stored here

#ifndef SLEEP_MEMORY_H
#define SLEEP_MEMORY_H

#include <iostream>

#include "esp_attr.h"
#include "esp_sleep.h"

#include "app-window.h"

#include "defines.h"


namespace SleepMemory {
    //store data to be kept through sleep in RTC memory so it can be restored.
    //RTC has to remain powered for the wakeup GPIO signal to be used, so the RTC memory will be preserved for free
    //the value will be lost on a complete power down

    RTC_NOINIT_ATTR ScreenHistoryData ui_screen_history;

    //cannot use screens pointer in ScreenHistoryData as the memory being pointed to won't be kept through sleep and the
    //pointer will become invalid.
    //instead, save the screens array manually and load it into ui_screen_history when restoring.
    RTC_NOINIT_ATTR RemoteScreenID ui_screen_history_screens[SCREEN_HISTORY_SIZE];


    slint::ComponentHandle<AppWindow>* p_main_window; //a pointer to the main window so data can be accessed from ui


    void save()
    {
        const ScreenHistoryData& temp_history = (*p_main_window)->global<ScreenHistory>().get_history();
        ui_screen_history.capacity = temp_history.capacity;
        ui_screen_history.front = temp_history.front;
        ui_screen_history.length = temp_history.length;

        for (uint32_t i = 0; i < temp_history.screens->row_count(); i++)
        {
            ui_screen_history_screens[i] = temp_history.screens->row_data(i).value();
        }
    }

    void restore()
    {
        if (esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_EXT0)
        {
            //didn't wake from sleep, set initial screen
            (*p_main_window)->global<ScreenHistory>().invoke_init(SCREEN_HISTORY_SIZE, RemoteScreenID::TVCHANNELS);
            std::cout << "screen history created new" << std::endl;
        }
        else
        {
            //woke up from sleep, restore saved values
            std::vector<RemoteScreenID> temp_screen_ids;
            for (uint32_t i = 0; i < 10; i++)
            {
                temp_screen_ids.push_back(ui_screen_history_screens[i]);
            }

            ui_screen_history.screens = std::make_shared<slint::VectorModel<RemoteScreenID>>(temp_screen_ids);
            (*p_main_window)->global<ScreenHistory>().invoke_restore(ui_screen_history);
            std::cout << "screen history restored after sleep" << std::endl;
        }
    }
}

#endif