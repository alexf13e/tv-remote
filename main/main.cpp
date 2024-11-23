
#include "esp_attr.h"
#include "esp_sleep.h"
#include "slint_string.h"
#include "app-window.h"

#include "power.h"
#include "screen.h"
#include "backlight.h"
#include "actionlist.h"
#include "button_hold_manager.h"
#include "IRReceiver.h"


//store the most recently displayed screen in RTC memory to preserve it through sleeps so it can be restored
//RTC has to remain powered for the wakeup GPIO signal to be used, so the RTC memory will be preserved for free
//the value will be lost on a complete power down
RTC_NOINIT_ATTR RemoteScreenID ui_screen;

extern "C" void app_main(void)
{
    Power::init();
    Screen::init();
    Backlight::init();
    IRTransmitter::init();
    IRReceiver::init();
    ButtonHoldManager::init();
    createActionLists();

    
    auto main_window = AppWindow::create();

    main_window->global<Logic>().on_remote_btn_press([&](slint::SharedString actionListID, bool repeat_on_hold) {
        ButtonHoldManager::press(actionListID.data(), repeat_on_hold);
    });

    main_window->global<Logic>().on_remote_btn_release([]() {
        ButtonHoldManager::release();
    });

    main_window->global<Logic>().on_sleep([]() {
        Power::sleep();
    });

    main_window->global<Logic>().on_update_remembered_ui_screen([](RemoteScreenID screen_id) {
        ui_screen = screen_id;
    });

    main_window->global<Logic>().on_disable_sleep([] () {
        Power::disable_sleep();
    });

    main_window->global<Logic>().on_enable_sleep([] () {
        Power::enable_sleep();
    });

    main_window->global<Logic>().on_begin_ir_receive([]() {
        IRReceiver::receive();
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
