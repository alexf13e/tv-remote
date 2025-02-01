
#include "ESP_Panel_Library.h"
#include "slint-esp.h"
#include "slint-platform.h"

#define LCD_NUM_FB 1
#define ROTATE_PORTRAIT false

namespace Screen2 {
    void init()
    {
        ESP_Panel* panel = new ESP_Panel();
        const uint16_t display_width = panel->getLcdWidth();
        const uint16_t display_height = panel->getLcdHeight();
        const esp_lcd_panel_handle_t panel_handle = panel->getLcd()->getHandle();
        const esp_lcd_touch_handle_t touch_handle = panel->getTouch()->getHandle();


        /* Allocate a drawing buffer */
        static std::vector<slint::platform::Rgb565Pixel> fb1(display_width * display_height);
        slint::platform::Rgb565Pixel* fb1Data = fb1.data();

        #if LCD_NUM_FB == 2
        static std::vector<slint::platform::Rgb565Pixel> fb2(display_width * display_height);
        slint::platform::Rgb565Pixel* fb2Data = fb2.data();
        ESP_ERROR_CHECK(esp_lcd_rgb_panel_get_frame_buffer(panel_handle, 2, (void**)&fb1Data, (void**)&fb2Data));
        #else
        ESP_ERROR_CHECK(esp_lcd_rgb_panel_get_frame_buffer(panel_handle, 1, (void**)&fb1Data));
        #endif
        
        /* Initialize Slint's ESP platform support*/
        slint_esp_init(SlintPlatformConfiguration {
            #if ROTATE_PORTRAIT
            .size = slint::PhysicalSize({ display_height, display_width }), //vertical and horizontal swapped to allow portrait orientation
            #else
            .size = slint::PhysicalSize({ display_width, display_height }),
            #endif
            .panel_handle = panel_handle,
            .touch_handle = touch_handle,
            .buffer1 = fb1,

            #if LCD_NUM_FB == 2
            .buffer2 = fb2,
            #endif
        });
    }
}