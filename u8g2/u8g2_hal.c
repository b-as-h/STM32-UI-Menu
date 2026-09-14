/*
 * u8g2_hal.c - u8g2 硬件回调实现
 * 这个文件连接 u8g2 库和 STM32 HAL
 */

#include "u8g2_hal.h"
#include "i2c.h"
#include <string.h>

// I2C 缓冲区
static uint8_t u8g2_i2c_buffer[128];
static uint8_t u8g2_i2c_buffer_idx = 0;

// I2C 设备地址（7位地址，会在传输时左移一位）
static uint8_t i2c_address = 0x3C; // SSD1302/SSD1306 常见地址

uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    uint8_t *data;

    switch (msg)
    {
    case U8X8_MSG_BYTE_SEND:
        // 收集要发送的数据
        data = (uint8_t *)arg_ptr;
        while (arg_int > 0)
        {
            if (u8g2_i2c_buffer_idx < sizeof(u8g2_i2c_buffer))
            {
                u8g2_i2c_buffer[u8g2_i2c_buffer_idx++] = *data;
            }
            data++;
            arg_int--;
        }
        break;

    case U8X8_MSG_BYTE_INIT:
        // I2C 已在 MX_I2C1_Init() 中初始化，这里不需要额外操作
        break;

    case U8X8_MSG_BYTE_SET_DC:
        // I2C 模式下不需要 DC 引脚
        break;

    case U8X8_MSG_BYTE_START_TRANSFER:
        u8g2_i2c_buffer_idx = 0;
        break;

    case U8X8_MSG_BYTE_END_TRANSFER:
        // 发送缓冲区中的数据
        if (u8g2_i2c_buffer_idx > 0)
        {
            HAL_I2C_Master_Transmit(&hi2c1,
                                    (i2c_address << 1),
                                    u8g2_i2c_buffer,
                                    u8g2_i2c_buffer_idx,
                                    1000);
            u8g2_i2c_buffer_idx = 0;
        }
        break;

    default:
        return 0;
    }

    return 1;
}

uint8_t u8x8_gpio_and_delay_stm32(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    switch (msg)
    {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
        // 初始化（已经在 CubeMX 中完成）
        break;

    case U8X8_MSG_DELAY_MILLI:
        HAL_Delay(arg_int);
        break;

    case U8X8_MSG_DELAY_10MICRO:
        // 10 微秒延迟
        for (volatile uint32_t i = 0; i < 72 * 10; i++)
            ;
        break;

    case U8X8_MSG_DELAY_100NANO:
        // 100 纳秒延迟
        __NOP();
        break;

    // I2C 模式下不需要 GPIO 操作
    case U8X8_MSG_GPIO_I2C_CLOCK:
    case U8X8_MSG_GPIO_I2C_DATA:
        // 由硬件 I2C 处理
        break;

    default:
        return 0;
    }

    return 1;
}
