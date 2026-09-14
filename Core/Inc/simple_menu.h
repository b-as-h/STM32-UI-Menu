/**
 * @file simple_menu.h
 * @brief 简单的纯 C 菜单系统
 * @note 避免使用 C++ 标准库，与 STM32 HAL 完全兼容
 */

#ifndef SIMPLE_MENU_H
#define SIMPLE_MENU_H

#include <stdint.h>

// 菜单项最大数量
#define MENU_MAX_ITEMS 10

// 菜单项结构体
typedef struct {
    char name[20];           // 菜单名称
    uint8_t id;              // 菜单 ID
} MenuItem;

// 菜单结构体
typedef struct {
    char title[20];          // 菜单标题
    MenuItem items[MENU_MAX_ITEMS]; // 菜单项数组
    uint8_t item_count;      // 菜单项数量
    uint8_t selected_index;  // 当前选中项
    uint8_t parent_id;       // 父菜单 ID（0 表示根菜单）
} Menu;

// 菜单系统函数
void Menu_Init(void);
void Menu_CreateMenu(uint8_t menu_id, const char *title, uint8_t parent_id);
void Menu_AddItem(uint8_t menu_id, uint8_t item_id, const char *name);
void Menu_Show(void);
void Menu_KeyUp(void);
void Menu_KeyDown(void);
void Menu_KeyConfirm(void);
void Menu_KeyBack(void);
uint8_t Menu_GetCurrentMenuId(void);
uint8_t Menu_GetSelectedItemId(void);

#endif /* SIMPLE_MENU_H */
