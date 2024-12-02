
#include "slint_string.h"
#include "app-window.h"

#include "power.h"
#include "screen.h"
#include "backlight.h"
#include "actionlist.h"
#include "button_hold_manager.h"
#include "IRReceiver.h"
#include "sleep_memory.h"



void create_callbacks(slint::ComponentHandle<AppWindow> main_window)
{
    main_window->global<Logic>().on_remote_btn_press([&](slint::SharedString action_list_id, bool repeat_on_hold) {
        ButtonHoldManager::press(action_list_id.data(), repeat_on_hold);
    });

    main_window->global<Logic>().on_remote_btn_release([]() {
        ButtonHoldManager::release();
    });

    main_window->global<Logic>().on_sleep([]() {
        Power::sleep();
    });

    main_window->global<Logic>().on_disable_sleep([]() {
        Power::disable_sleep();
    });

    main_window->global<Logic>().on_enable_sleep([]() {
        Power::enable_sleep();
    });

    main_window->global<Logic>().on_disable_auto_brightness([]() {
        Backlight::auto_brightness_enabled = false;
    });

    main_window->global<Logic>().on_enable_auto_brightness([]() {
        Backlight::auto_brightness_enabled = true;
    });

    main_window->global<Logic>().on_begin_ir_receive([]() {
        IRReceiver::receive();
    });

    main_window->global<Logic>().on_init_history_screens([](int capacity) {
        std::vector<RemoteScreenID> new_screens;
        for (uint32_t i = 0; i < capacity; i++)
        {
            new_screens.push_back(RemoteScreenID::SAME);
        }

        auto new_screens_model = std::make_shared<slint::VectorModel<RemoteScreenID>>(new_screens);
        return new_screens_model;
    });

    main_window->global<Logic>().on_shift_history_screens([](std::shared_ptr<slint::Model<RemoteScreenID>> old_screens) {
        std::vector<RemoteScreenID> new_screens;
        
        for (uint32_t i = 0; i < old_screens->row_count() - 1; i++)
        {
            new_screens.push_back(*old_screens->row_data(i + 1));
        }

        new_screens.push_back(RemoteScreenID::SAME);

        auto new_screens_model = std::make_shared<slint::VectorModel<RemoteScreenID>>(new_screens);
        return new_screens_model;
    });
}

void manage_sleep_wakeup(slint::ComponentHandle<AppWindow>* p_main_window)
{
    SleepMemory::p_main_window = p_main_window; //pass pointer to main window so sleep memory can access it when saving
    SleepMemory::restore();
}

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

    create_callbacks(main_window);
    manage_sleep_wakeup(&main_window);
    
    main_window->run();
}
