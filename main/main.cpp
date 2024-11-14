
#include "slint_string.h"
#include "app-window.h"

#include "power.h"
#include "screen.h"
#include "backlight.h"
#include "actionlist.h"


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

        
    main_window->run();
}
