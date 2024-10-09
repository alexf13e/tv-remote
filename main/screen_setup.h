
#ifndef SCREEN_SETUP_H
#define SCREEN_SETUP_H

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
#include "slint-esp.h"
#include "soc/clk_tree_defs.h"
#include "soc/gpio_num.h"

#define I2C_MASTER_SCL_IO           9       /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           8       /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              0       /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ          400000                     /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       1000

#define GPIO_INPUT_IO_4    4
#define GPIO_INPUT_PIN_SEL  1ULL<<GPIO_INPUT_IO_4

#define LCD_PIXEL_CLOCK_HZ     (18 * 1000 * 1000)
#define LCD_BK_LIGHT_ON_LEVEL  1
#define LCD_BK_LIGHT_OFF_LEVEL !LCD_BK_LIGHT_ON_LEVEL
#define PIN_NUM_BK_LIGHT       -1
#define PIN_NUM_DISP_EN        -1
#define PIN_NUM_HSYNC          46
#define PIN_NUM_VSYNC          3
#define PIN_NUM_DE             5
#define PIN_NUM_PCLK           7
#define PIN_NUM_DATA0          14 // B3
#define PIN_NUM_DATA1          38 // B4
#define PIN_NUM_DATA2          18 // B5
#define PIN_NUM_DATA3          17 // B6
#define PIN_NUM_DATA4          10 // B7
#define PIN_NUM_DATA5          39 // G2
#define PIN_NUM_DATA6          0  // G3
#define PIN_NUM_DATA7          45 // G4
#define PIN_NUM_DATA8          48 // G5
#define PIN_NUM_DATA9          47 // G6
#define PIN_NUM_DATA10         21 // G7
#define PIN_NUM_DATA11         1  // R3
#define PIN_NUM_DATA12         2  // R4
#define PIN_NUM_DATA13         42 // R5
#define PIN_NUM_DATA14         41 // R6
#define PIN_NUM_DATA15         40 // R7

// The pixel number in horizontal and vertical
#define LCD_H_RES              800
#define LCD_V_RES              480

#define LCD_NUM_FB             2

#define ROTATE_PORTRAIT true


void screenSetup()
{
    //set up display
    esp_lcd_panel_handle_t panel_handle = NULL;
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

    //set up touch panel
    int i2c_master_port = I2C_MASTER_NUM;

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

    i2c_param_config((i2c_port_t)i2c_master_port, &conf);
    i2c_driver_install((i2c_port_t)i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);

    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    io_conf.mode = GPIO_MODE_OUTPUT;
    gpio_config(&io_conf);

    uint8_t write_buf = 0x01;
    i2c_master_write_to_device((i2c_port_t)I2C_MASTER_NUM, 0x24, &write_buf, 1, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

    //Reset the touch screen. It is recommended that you reset the touch screen before using it.
    write_buf = 0x2C;
    i2c_master_write_to_device((i2c_port_t)I2C_MASTER_NUM, 0x38, &write_buf, 1, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    esp_rom_delay_us(100 * 1000);

    gpio_set_level((gpio_num_t)GPIO_INPUT_IO_4,0);
    esp_rom_delay_us(100 * 1000);

    write_buf = 0x2E;
    i2c_master_write_to_device((i2c_port_t)I2C_MASTER_NUM, 0x38, &write_buf, 1, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    esp_rom_delay_us(200 * 1000);

    esp_lcd_touch_handle_t touch_handle = NULL;
    esp_lcd_panel_io_handle_t tp_io_handle = NULL;
    esp_lcd_panel_io_i2c_config_t tp_io_config = ESP_LCD_TOUCH_IO_I2C_GT911_CONFIG();
    esp_lcd_new_panel_io_i2c((esp_lcd_i2c_bus_handle_t)I2C_MASTER_NUM, &tp_io_config, &tp_io_handle);
    
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
        .size = slint::PhysicalSize({ LCD_H_RES, LCD_V_RES })
        #endif
        .panel_handle = panel_handle,
        .touch_handle = touch_handle,
        .buffer1 = fb1,

        #if LCD_NUM_FB == 2
        .buffer2 = fb2,
        #endif
    });
}

#endif