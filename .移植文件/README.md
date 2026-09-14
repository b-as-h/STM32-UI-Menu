# 一键移植包

## 快速开始

### 1. 复制文件
将 `Inc/` 和 `Src/` 文件夹中的所有文件复制到你的项目：
```
你的项目/Core/Inc/ ← 复制这里的 .h 文件
你的项目/Core/Src/ ← 复制这里的 .c 文件
```

### 2. 配置 CubeMX
- 启用 I2C（I2C1，400kHz）
- 配置按键 GPIO 为 Input 模式
- 生成代码

### 3. 修改引脚（可选）
打开 `main.h`，修改按键和 LED 的引脚定义：
```c
#define key_up_Pin      GPIO_PIN_12
#define key_up_GPIO_Port GPIOB
// ... 其他引脚
```

### 4. 编译运行
- 确保所有 `.c` 文件都添加到项目
- 编译并下载

---

## 文件列表

### Inc（头文件）
- `oled.h` - OLED 驱动接口
- `font.h` - 字体结构定义
- `key.h` - 按键检测接口
- `simple_menu.h` - 菜单系统接口

### Src（源文件）
- `oled.c` - OLED 驱动实现
- `font.c` - 字体数据
- `key.c` - 按键检测实现（状态机消抖）
- `simple_menu.c` - 菜单系统实现

### 文档
- `移植指南.md` - 详细的移植步骤和使用说明

---

## 最小示例

```c
#include "main.h"
#include "oled.h"
#include "key.h"
#include "simple_menu.h"

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_I2C1_Init();

  OLED_Init();
  HAL_Delay(20);

  Menu_Init();
  Menu_CreateMenu(0, "Main Menu", 0);
  Menu_AddItem(0, 1, "Option 1");
  Menu_AddItem(0, 2, "Option 2");
  Menu_Show();

  while (1)
  {
    uint8_t key = key_process();
    if (key == KEY_UP_PRESSED) { Menu_KeyUp(); Menu_Show(); }
    else if (key == KEY_DOWN_PRESSED) { Menu_KeyDown(); Menu_Show(); }
    HAL_Delay(10);
  }
}
```

---

## 常用字体

```c
afont12x6   // 推荐用于菜单（12像素高）
afont16x8   // 适合标题（16像素高）
```

---

## 需要帮助？

查看 `移植指南.md` 获取详细说明。
