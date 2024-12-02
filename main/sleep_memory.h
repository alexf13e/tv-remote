
//data which wants to be remembered through sleep should be stored here

#ifndef SLEEP_MEMORY_H
#define SLEEP_MEMORY_H

#include "esp_attr.h"
#include "esp_sleep.h"

#include "app-window.h"


namespace SleepMemory {
    //store data to be kept through sleep in RTC memory so it can be restored.
    //RTC has to remain powered for the wakeup GPIO signal to be used, so the RTC memory will be preserved for free
    //the value will be lost on a complete power down

    RTC_NOINIT_ATTR ScreenHistoryData ui_screen_history;

    slint::ComponentHandle<AppWindow>* p_main_window; //a pointer to the main window so data can be accessed from ui


    void save()
    {
        ui_screen_history = (*p_main_window)->global<ScreenHistory>().get_history();
    }

    void restore()
    {
        if (esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_EXT0)
        {
            //didn't wake from sleep, set initial screen
            (*p_main_window)->global<ScreenHistory>().invoke_init(10, RemoteScreenID::TVCHANNELS);
        }
        else
        {
            //woke up from sleep, restore saved values
            (*p_main_window)->global<ScreenHistory>().invoke_restore(ui_screen_history);
        }
    }
}

#endif