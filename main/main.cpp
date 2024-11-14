
#include "esp_attr.h"
#include "esp_sleep.h"
#include "slint_string.h"
#include "app-window.h"

#include "power.h"
#include "screen.h"
#include "backlight.h"
#include "actionlist.h"

RTC_NOINIT_ATTR RemoteScreenID ui_screen;

extern "C" void app_main(void)
{
    Power::init();
    Screen::init();
    Backlight::init();
    IRTransmitter::init();
    createActionLists();

    
    auto main_window = AppWindow::create();

    main_window->global<Logic>().on_remote_btn_clicked([&](slint::SharedString actionListID) {
        //run the list of actions specified by actionListID if one was set for the button
        if (actionListID != "") runActionList(actionListID.data());
    });

    main_window->global<Logic>().on_sleep([]() {
        Power::sleep();
    });

    main_window->global<Logic>().on_update_remembered_ui_screen([](RemoteScreenID screen_id) {
        ui_screen = screen_id;
    });

    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0)
    {
        //woke up from sleep, restore the screen we were on before sleeping
        main_window->invoke_load_remembered_screen(ui_screen);
    }
    else
    {
        //first time being powered on, no screen to restore so initialise the remembered one
        ui_screen = main_window->global<CurrentDisplayedScreen>().get_id();
    }

        
    main_window->run();
}
