#include "key.h"
#include "main.h"

#define DEBOUNCE_MS 20 // 消抖时间（毫秒）

// 按键状态
typedef enum
{
    KEY_IDLE,     // 空闲状态
    KEY_DEBOUNCE, // 消抖状态
    KEY_PRESSED   // 已按下
} KeyState_t;

// 按键信息结构体
typedef struct
{
    KeyState_t state;
    uint32_t tick_start;
} Key_t;

static Key_t key_up = {KEY_IDLE, 0};
static Key_t key_down = {KEY_IDLE, 0};

// 检测单个按键
// 返回值: 0-无事件, 1-按下事件
static uint8_t Key_ProcessOne(Key_t *key, uint8_t pin_state)
{
    uint8_t event = 0;

    switch (key->state)
    {
    case KEY_IDLE:
        if (pin_state == GPIO_PIN_RESET)
        { // 检测到按下（低电平有效）
            key->state = KEY_DEBOUNCE;
            key->tick_start = HAL_GetTick();
        }
        break;

    case KEY_DEBOUNCE:
        if (pin_state == GPIO_PIN_RESET)
        {
            // 按键持续按下，消抖时间到
            if (HAL_GetTick() - key->tick_start >= DEBOUNCE_MS)
            {
                key->state = KEY_PRESSED;
                event = 1; // 产生按下事件
            }
        }
        else
        {
            // 消抖期间释放，返回空闲
            key->state = KEY_IDLE;
        }
        break;

    case KEY_PRESSED:
        if (pin_state == GPIO_PIN_SET)
        { // 按键释放
            key->state = KEY_IDLE;
        }
        break;
    }

    return event;
}

uint8_t key_process(void)
{
    uint8_t pin_up = HAL_GPIO_ReadPin(key_up_GPIO_Port, key_up_Pin);
    uint8_t pin_down = HAL_GPIO_ReadPin(key_down_GPIO_Port, key_down_Pin);

    if (Key_ProcessOne(&key_up, pin_up))
    {
        return KEY_UP_PRESSED;
    }
    if (Key_ProcessOne(&key_down, pin_down))
    {
        return KEY_DOWN_PRESSED;
    }

    return KEY_NONE;
}

uint8_t key_up_is_pressed(void)
{
    return (HAL_GPIO_ReadPin(key_up_GPIO_Port, key_up_Pin) == GPIO_PIN_RESET);
}

uint8_t key_down_is_pressed(void)
{
    return (HAL_GPIO_ReadPin(key_down_GPIO_Port, key_down_Pin) == GPIO_PIN_RESET);
}
