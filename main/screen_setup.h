
#ifndef SCREEN_SETUP_H
#define SCREEN_SETUP_H

//contents of this file are a mix between two examples with further modifications until they worked
//the display and touch setup are from https://files.waveshare.com/wiki/ESP32-S3-Touch-LCD-7/ESP32-S3-Touch-LCD-7_Code.zip > ESP-IDF-5.3.0 > lvgl_Porting
//the slint setup is from https://docs.slint.dev/latest/docs/cpp/mcu/esp_idf


#include <cstddef>
#include <cstdint>
#include <slint-platform.h>
#include <slint_generated_public.h>
#include <stdio.h>
#include <vector>
#include "driver/i2c.h"
#include "esp_lcd_io_i2c.h"
#include "esp_lcd_touch.h"
#include "esp_lcd_types.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "driver/gpio.h"
#include "esp_err.h"

#include "driver/i2c.h"
#include "esp_lcd_touch_gt911.h"

#include "hal/i2c_types.h"
#include "portmacro.h"
#include "slint-esp.h"
#include "soc/clk_tree_defs.h"
#include "soc/gpio_num.h"

constexpr gpio_num_t I2C_MASTER_SCL_IO = GPIO_NUM_9;    /*!< GPIO number used for I2C master clock */
constexpr gpio_num_t I2C_MASTER_SDA_IO = GPIO_NUM_8;    /*!< GPIO number used for I2C master data  */
constexpr i2c_port_t I2C_MASTER_NUM = I2C_NUM_0;        /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
constexpr uint32_t I2C_MASTER_FREQ_HZ = 400000;         /*!< I2C master clock frequency */
constexpr size_t I2C_MASTER_TX_BUF_DISABLE = 0;         /*!< I2C master doesn't need buffer */
constexpr size_t I2C_MASTER_RX_BUF_DISABLE = 0;         /*!< I2C master doesn't need buffer */
constexpr TickType_t I2C_MASTER_TIMEOUT_MS = 1000;

constexpr gpio_num_t GPIO_INPUT_IO_4 = GPIO_NUM_4;
constexpr uint64_t GPIO_INPUT_PIN_SEL = 1ULL << GPIO_INPUT_IO_4;

constexpr uint32_t LCD_PIXEL_CLOCK_HZ = (18 * 1000 * 1000);
constexpr int PIN_NUM_DISP_EN = -1;
constexpr int PIN_NUM_HSYNC = 46;
constexpr int PIN_NUM_VSYNC = 3;
constexpr int PIN_NUM_DE = 5;
constexpr int PIN_NUM_PCLK = 7;
constexpr int PIN_NUM_DATA0 = 14;   // B3
constexpr int PIN_NUM_DATA1 = 38;   // B4
constexpr int PIN_NUM_DATA2 = 18;   // B5
constexpr int PIN_NUM_DATA3 = 17;   // B6
constexpr int PIN_NUM_DATA4 = 10;   // B7
constexpr int PIN_NUM_DATA5 = 39;   // G2
constexpr int PIN_NUM_DATA6 = 0;    // G3
constexpr int PIN_NUM_DATA7 = 45;   // G4
constexpr int PIN_NUM_DATA8 = 48;   // G5
constexpr int PIN_NUM_DATA9 = 47;   // G6
constexpr int PIN_NUM_DATA10 = 21;  // G7
constexpr int PIN_NUM_DATA11 = 1;   // R3
constexpr int PIN_NUM_DATA12 = 2;   // R4
constexpr int PIN_NUM_DATA13 = 42;  // R5
constexpr int PIN_NUM_DATA14 = 41;  // R6
constexpr int PIN_NUM_DATA15 = 40;  // R7

// The pixel number in horizontal and vertical
constexpr uint32_t LCD_H_RES = 800;
constexpr uint32_t LCD_V_RES = 480;

//number of framebuffers
#define LCD_NUM_FB 2

//default orientation is landscape, set to true for portrait (left landscape edge becomes bottom portrait edge)
#define ROTATE_PORTRAIT true


esp_lcd_panel_handle_t displaySetup()
{
    esp_lcd_panel_handle_t panel_handle = nullptr;
    esp_lcd_rgb_panel_config_t panel_config = {
        .clk_src = LCD_CLK_SRC_DEFAULT,
        .timings = {
            .pclk_hz = LCD_PIXEL_CLOCK_HZ,
            .h_res = LCD_H_RES,
            .v_res = LCD_V_RES,
            // The following parameters should refer to LCD spec
            .hsync_pulse_width = 4,
            .hsync_back_porch = 8,
            .hsync_front_porch = 8,
            .vsync_pulse_width = 4,
            .vsync_back_porch = 8,
            .vsync_front_porch = 8,
            .flags = {
                .pclk_active_neg = true,
            }
        },
        
        .data_width = 16,
        .num_fbs = LCD_NUM_FB,
        .hsync_gpio_num = PIN_NUM_HSYNC,
        .vsync_gpio_num = PIN_NUM_VSYNC,
        .de_gpio_num = PIN_NUM_DE,
        .pclk_gpio_num = PIN_NUM_PCLK,
        .disp_gpio_num = PIN_NUM_DISP_EN,
        .data_gpio_nums = {
            PIN_NUM_DATA0,
            PIN_NUM_DATA1,
            PIN_NUM_DATA2,
            PIN_NUM_DATA3,
            PIN_NUM_DATA4,
            PIN_NUM_DATA5,
            PIN_NUM_DATA6,
            PIN_NUM_DATA7,
            PIN_NUM_DATA8,
            PIN_NUM_DATA9,
            PIN_NUM_DATA10,
            PIN_NUM_DATA11,
            PIN_NUM_DATA12,
            PIN_NUM_DATA13,
            PIN_NUM_DATA14,
            PIN_NUM_DATA15,
        },
        
        .flags = {
            .fb_in_psram = true,

            #if LCD_NUM_FB == 2
            .double_fb = true,
            #endif
        }
    };

    ESP_ERROR_CHECK(esp_lcd_new_rgb_panel(&panel_config, &panel_handle));

    #if ROTATE_PORTRAIT
    ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel_handle, true));
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, true, false));
    #endif

    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));

    return panel_handle;
}

esp_lcd_touch_handle_t touchSetup()
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master = {
            .clk_speed = I2C_MASTER_FREQ_HZ,
        }
    };

    i2c_param_config(I2C_MASTER_NUM, &conf);
    ESP_ERROR_CHECK(i2c_driver_install(I2C_MASTER_NUM, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0));

    gpio_config_t io_conf = {
        .pin_bit_mask = GPIO_INPUT_PIN_SEL,
        .mode = GPIO_MODE_OUTPUT,
        .intr_type = GPIO_INTR_DISABLE,
    };

    gpio_config(&io_conf);

    uint8_t write_buf = 0x01;
    i2c_master_write_to_device(I2C_MASTER_NUM, 0x24, &write_buf, 1, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

    //Reset the touch screen. It is recommended that you reset the touch screen before using it.
    write_buf = 0x2C;
    i2c_master_write_to_device(I2C_MASTER_NUM, 0x38, &write_buf, 1, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    esp_rom_delay_us(100 * 1000);

    gpio_set_level(GPIO_INPUT_IO_4, 0);
    esp_rom_delay_us(100 * 1000);

    write_buf = 0x2E;
    i2c_master_write_to_device((i2c_port_t)I2C_MASTER_NUM, 0x38, &write_buf, 1, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    esp_rom_delay_us(200 * 1000);

    esp_lcd_touch_handle_t touch_handle = NULL;
    esp_lcd_panel_io_handle_t tp_io_handle = NULL;
    esp_lcd_panel_io_i2c_config_t tp_io_config = ESP_LCD_TOUCH_IO_I2C_GT911_CONFIG();
    esp_lcd_new_panel_io_i2c(I2C_MASTER_NUM, &tp_io_config, &tp_io_handle);
    
    esp_lcd_touch_config_t tp_cfg = {
        .x_max = LCD_H_RES,
        .y_max = LCD_V_RES,
        .rst_gpio_num = (gpio_num_t)-1,
        .int_gpio_num = (gpio_num_t)-1,
        .flags = {
            #if ROTATE_PORTRAIT
            .swap_xy = 1,
            .mirror_x = 1,
            .mirror_y = 0,
            #endif
        },
    };
    
    ESP_ERROR_CHECK(esp_lcd_touch_new_i2c_gt911(tp_io_handle, &tp_cfg, &touch_handle));

    return touch_handle;
}

void slintSetup(const esp_lcd_panel_handle_t panel_handle, const esp_lcd_touch_handle_t touch_handle)
{
    /* Allocate a drawing buffer */
    static std::vector<slint::platform::Rgb565Pixel> fb1(LCD_H_RES * LCD_V_RES);
    slint::platform::Rgb565Pixel* fb1Data = fb1.data();

    #if LCD_NUM_FB == 2
    static std::vector<slint::platform::Rgb565Pixel> fb2(LCD_H_RES * LCD_V_RES);
    slint::platform::Rgb565Pixel* fb2Data = fb2.data();
    ESP_ERROR_CHECK(esp_lcd_rgb_panel_get_frame_buffer(panel_handle, 2, (void**)&fb1Data, (void**)&fb2Data));
    #else
    ESP_ERROR_CHECK(esp_lcd_rgb_panel_get_frame_buffer(panel_handle, 1, (void**)&fb1Data));
    #endif
    
    /* Initialize Slint's ESP platform support*/
    slint_esp_init(SlintPlatformConfiguration {
        #if ROTATE_PORTRAIT
        .size = slint::PhysicalSize({ LCD_V_RES, LCD_H_RES }), //vertical and horizontal swapped to allow portrait orientation
        #else
        .size = slint::PhysicalSize({ LCD_H_RES, LCD_V_RES }),
        #endif
        .panel_handle = panel_handle,
        .touch_handle = touch_handle,
        .buffer1 = fb1,

        #if LCD_NUM_FB == 2
        .buffer2 = fb2,
        #endif
    });
}

void screenSetup()
{
    esp_lcd_panel_handle_t panel_handle = displaySetup();
    esp_lcd_touch_handle_t touch_handle = touchSetup();
    slintSetup(panel_handle, touch_handle);
}

#endif