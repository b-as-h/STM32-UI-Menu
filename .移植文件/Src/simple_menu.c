/**
 * @file simple_menu.c
 * @brief 简单的纯 C 菜单系统实现
 * @note 避免使用 C++ 标准库，与 STM32 HAL 完全兼容
 */

#include "simple_menu.h"
#include "oled.h"
#include <string.h>

// 菜单系统全局变量
static Menu menus[10]; // 最多支持 10 个菜单
static uint8_t menu_count = 0;
static uint8_t current_menu_id = 0;

// 字体定义（使用 font.h 中已定义的字体）
extern const ASCIIFont afont12x6;

/**
 * @brief 初始化菜单系统
 */
void Menu_Init(void)
{
    memset(menus, 0, sizeof(menus));
    menu_count = 0;
    current_menu_id = 0;
}

/**
 * @brief 创建一个新菜单
 * @param menu_id 菜单 ID（唯一标识）
 * @param title 菜单标题
 * @param parent_id 父菜单 ID（0 表示根菜单）
 */
void Menu_CreateMenu(uint8_t menu_id, const char *title, uint8_t parent_id)
{
    if (menu_count >= 10)
        return; // 超过最大菜单数量

    Menu *menu = &menus[menu_count];
    strncpy(menu->title, title, 19);
    menu->title[19] = '\0'; // 确保字符串结束
    menu->item_count = 0;
    menu->selected_index = 0;
    menu->parent_id = parent_id;

    menu_count++;
}

/**
 * @brief 向菜单添加菜单项
 * @param menu_id 菜单 ID
 * @param item_id 菜单项 ID
 * @param name 菜单项名称
 */
void Menu_AddItem(uint8_t menu_id, uint8_t item_id, const char *name)
{
    // 查找菜单
    for (uint8_t i = 0; i < menu_count; i++)
    {
        if (menus[i].title[0] != '\0')
        {
            // 简单的菜单 ID 查找（实际应该用更可靠的方法）
            if (i == menu_id)
            {
                Menu *menu = &menus[i];
                if (menu->item_count >= MENU_MAX_ITEMS)
                    return;

                MenuItem *item = &menu->items[menu->item_count];
                strncpy(item->name, name, 19);
                item->name[19] = '\0';
                item->id = item_id;

                menu->item_count++;
                break;
            }
        }
    }
}

/**
 * @brief 显示当前菜单
 */
void Menu_Show(void)
{
    if (current_menu_id >= menu_count)
        return;

    Menu *menu = &menus[current_menu_id];

    // 清空帧缓冲
    OLED_NewFrame();

    // 绘制标题
    OLED_PrintASCIIString(0, 0, menu->title, &afont16x8, OLED_COLOR_NORMAL);

    // 绘制分隔线
    OLED_DrawLine(0, 14, 127, 14, OLED_COLOR_NORMAL);

    // 绘制菜单项
    uint8_t start_y = 18;
    uint8_t item_height = 12;
    for (uint8_t i = 0; i < menu->item_count; i++)
    {
        uint8_t y = start_y + i * item_height;

        // 高亮显示选中项（当前选中的菜单项）
        if (i == menu->selected_index)
        {
            OLED_DrawFilledRectangle(0, y, 50, 0, OLED_COLOR_NORMAL);
            OLED_PrintASCIIString(4, y + 7, menu->items[i].name, &afont12x6, OLED_COLOR_REVERSED);
        }
        else
        {
            OLED_PrintASCIIString(4, y + 7, menu->items[i].name, &afont12x6, OLED_COLOR_NORMAL);
        }
    }

    // 显示到屏幕
    OLED_ShowFrame();
}

/**
 * @brief 上键处理
 */
void Menu_KeyUp(void)
{
    if (current_menu_id >= menu_count)
        return;

    Menu *menu = &menus[current_menu_id];

    if (menu->selected_index > 0)
    {
        menu->selected_index--;
    }
    else
    {
        // 循环到最后一项
        menu->selected_index = menu->item_count - 1;
    }
}

/**
 * @brief 下键处理
 */
void Menu_KeyDown(void)
{
    if (current_menu_id >= menu_count)
        return;

    Menu *menu = &menus[current_menu_id];

    if (menu->selected_index < menu->item_count - 1)
    {
        menu->selected_index++;
    }
    else
    {
        // 循环到第一项
        menu->selected_index = 0;
    }
}

/**
 * @brief 确认键处理（进入子菜单或执行功能）
 */
void Menu_KeyConfirm(void)
{
    if (current_menu_id >= menu_count)
        return;

    Menu *menu = &menus[current_menu_id];
    uint8_t selected_item_id = menu->items[menu->selected_index].id;

    // 查找子菜单（parent_id 匹配当前菜单）
    for (uint8_t i = 0; i < menu_count; i++)
    {
        if (menus[i].parent_id == current_menu_id)
        {
            // 找到子菜单，进入
            current_menu_id = i;
            menus[i].selected_index = 0;
            return;
        }
    }

    // 如果没有子菜单，可以执行功能
    // 这里可以添加自定义功能代码
    // 例如：显示信息、执行操作等
}

/**
 * @brief 返回键处理（返回上级菜单）
 */
void Menu_KeyBack(void)
{
    if (current_menu_id >= menu_count)
        return;

    Menu *menu = &menus[current_menu_id];

    if (menu->parent_id != 0)
    {
        // 返回父菜单
        current_menu_id = menu->parent_id;
    }
}

/**
 * @brief 获取当前菜单 ID
 * @return 当前菜单 ID
 */
uint8_t Menu_GetCurrentMenuId(void)
{
    return current_menu_id;
}

/**
 * @brief 获取当前选中项的 ID
 * @return 选中项的 ID
 */
uint8_t Menu_GetSelectedItemId(void)
{
    if (current_menu_id >= menu_count)
        return 0;

    Menu *menu = &menus[current_menu_id];
    return menu->items[menu->selected_index].id;
}
