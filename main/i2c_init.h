
#ifndef I2C_SETUP_H
#define I2C_SETUP_H

#include <cstdint>

#include "driver/i2c.h"
#include "hal/i2c_types.h"
#include "portmacro.h"
#include "soc/gpio_num.h"


constexpr gpio_num_t I2C_MASTER_SCL_IO = GPIO_NUM_9;    /*!< GPIO number used for I2C master clock */
constexpr gpio_num_t I2C_MASTER_SDA_IO = GPIO_NUM_8;    /*!< GPIO number used for I2C master data  */
constexpr i2c_port_t I2C_MASTER_NUM = I2C_NUM_0;        /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
constexpr uint32_t I2C_MASTER_FREQ_HZ = 400000;         /*!< I2C master clock frequency */
constexpr size_t I2C_MASTER_TX_BUF_DISABLE = 0;         /*!< I2C master doesn't need buffer */
constexpr size_t I2C_MASTER_RX_BUF_DISABLE = 0;         /*!< I2C master doesn't need buffer */
constexpr TickType_t I2C_MASTER_TIMEOUT_MS = 1000;

bool i2c_initialised = false;

void i2c_init()
{
    if (i2c_initialised) return;

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

    ESP_ERROR_CHECK(i2c_param_config(I2C_MASTER_NUM, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_MASTER_NUM, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0));

    i2c_initialised = true;
}

#endif