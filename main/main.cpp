
#include <iostream>

#include "esp_err.h"
#include "esp_pm.h"
#include "esp_sleep.h"

#include "slint_string.h"
#include "app-window.h"

#include "screen_setup.h"
#include "backlight.h"
#include "actionlist.h"


extern "C" void app_main(void)
{
    screenSetup();
    //Backlight::init();
    IRTransmitter::init();
    createActionLists();

    
    auto main_window = AppWindow::create();
    main_window->global<Logic>().on_remote_btn_clicked([&](slint::SharedString actionListID) {
        //run the list of actions specified by actionListID if one was set for the button
        if (actionListID != "") runActionList(actionListID.data());
    });

    main_window->global<Logic>().on_change_backlight([](float level) {
        Backlight::set_brightness(level);
    });

        
    main_window->run();
}
