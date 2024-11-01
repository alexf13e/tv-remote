
#include <iostream>

#include "esp_err.h"
#include "esp_pm.h"
#include "esp_sleep.h"

#include "slint_string.h"
#include "app-window.h"

#include "screen_setup.h"
#include "actionlist.h"


extern "C" void app_main(void)
{
    screenSetup();
    IRTransmitter::init();
    createActionLists();


    esp_pm_config_t pm_config = {
        .max_freq_mhz = 240,
        .min_freq_mhz = 40,
    };

    ESP_ERROR_CHECK(esp_pm_configure(&pm_config));
    
    ESP_ERROR_CHECK(esp_sleep_enable_touchpad_wakeup());
    esp_sleep_enable_timer_wakeup(2000000);

    
    auto main_window = AppWindow::create();
    main_window->global<Logic>().on_remote_btn_clicked([&](slint::SharedString actionListID) {
        //run the list of actions specified by actionListID if one was set for the button
        if (actionListID != "") runActionList(actionListID.data());
    });

    main_window->global<Logic>().on_sleep([]() {
        ESP_ERROR_CHECK(esp_light_sleep_start());
        //esp_deep_sleep_start();
    });

        
    main_window->run();
}
