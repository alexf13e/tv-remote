
#include "slint_string.h"
#include "app-window.h"

#include "screen.h"
#include "backlight.h"
#include "actionlist.h"


extern "C" void app_main(void)
{
    Screen::init();
    Backlight::init();
    IRTransmitter::init();
    createActionLists();

    
    auto main_window = AppWindow::create();
    main_window->global<Logic>().on_remote_btn_clicked([&](slint::SharedString actionListID) {
        //run the list of actions specified by actionListID if one was set for the button
        if (actionListID != "") runActionList(actionListID.data());
    });

    

        
    main_window->run();
}
