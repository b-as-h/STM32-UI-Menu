/*
 * u8g2_hal.h - u8g2 硬件抽象层
 */

#ifndef U8G2_HAL_H
#define U8G2_HAL_H

#include "u8g2.h"
#include "stm32f1xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

// I2C 回调函数
uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

// GPIO 和延迟回调函数
uint8_t u8x8_gpio_and_delay_stm32(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

#ifdef __cplusplus
}
#endif

#endif /* U8G2_HAL_H */
