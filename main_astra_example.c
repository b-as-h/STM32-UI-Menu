/*
 * main_astra.c - 集成 astra-ui 的示例
 * 将此文件的内容复制到你的 main.c 中
 */

/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body with astra-ui
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "key.h"
#include "hal/hal_stm32.h"
#include "astra/ui/launcher.h"
#include "astra/ui/element/page/item.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
HAL_STM32 *hal = nullptr;
astra::Launcher launcher;
astra::Menu *rootPage = nullptr;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/**
  * @brief  The application entry point.
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();

  /* USER CODE BEGIN 2 */

  // ========== 初始化 astra-ui ==========

  // 1. 创建 HAL 实例
  hal = new HAL_STM32(&hi2c1);

  // 2. 注入 HAL
  HAL::inject(hal);

  // 3. 创建菜单结构
  rootPage = new astra::Menu("Main Menu");

  // 添加子菜单项
  astra::Menu *settingsPage = new astra::Menu("Settings");
  astra::Menu *aboutPage = new astra::Menu("About");
  astra::Menu *sensorPage = new astra::Menu("Sensors");

  rootPage->addItem(settingsPage);
  rootPage->addItem(sensorPage);
  rootPage->addItem(aboutPage);

  // 添加二级菜单
  settingsPage->addItem(new astra::Menu("Brightness"));
  settingsPage->addItem(new astra::Menu("Sound"));
  settingsPage->addItem(new astra::Menu("Language"));

  sensorPage->addItem(new astra::Menu("Temperature"));
  sensorPage->addItem(new astra::Menu("Humidity"));

  // 4. 初始化启动器
  launcher.init(rootPage);
  launcher.open();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    // ========== astra-ui 主循环 ==========

    // 更新按键状态
    hal->updateKeys();

    // 按键扫描
    HAL::keyScan();

    // 更新 UI
    launcher.update();

    // 添加小延迟避免 CPU 占用过高
    HAL_Delay(1);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  */
void SystemClock_Config(void)
{
  // ... 保持你原有的时钟配置 ...
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  */
void Error_Handler(void)
{
  __disable_irq();
  while(1)
  {
  }
}
