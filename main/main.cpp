

#include "slint_string.h"
#include "app-window.h"

#include "screen_setup.h"
#include "actionlist.h"


extern "C" void app_main(void)
{
    screenSetup();
    IRTransmitter::init();
    createActionLists();
    
    auto main_window = AppWindow::create();
    main_window->global<Logic>().on_remote_btn_clicked([&](slint::SharedString actionListID, RemoteScreenID screenID) {
        //run the list of actions specified by actionListID if one was set for the button
        if (actionListID != "") runActionList(actionListID.data());

        //change the screen if one was specified
        if (screenID != RemoteScreenID::SAME) main_window->set_currentScreenID(screenID);
    });
        
    main_window->run();
}
