#ifndef __KEY_H
#define __KEY_H

#include <stdint.h>

// 按键返回值定义
#define KEY_NONE        0   // 无按键事件
#define KEY_UP_PRESSED  1   // 上键按下
#define KEY_DOWN_PRESSED 2  // 下键按下

// 函数声明
uint8_t key_process(void);
uint8_t key_up_is_pressed(void);
uint8_t key_down_is_pressed(void);

#endif
