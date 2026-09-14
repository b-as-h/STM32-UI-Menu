/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : bash
 ******************************************************************************
 * 这个示例展示了如何使用 astra-ui 创建一个简单的菜单系统
 * 包含：主菜单、子菜单、弹窗提示
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
#include "oled.h"
#include "simple_menu.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  // ========================================
  // 初始化 OLED 和菜单系统
  // ========================================

  // 1. 初始化 OLED
  OLED_Init();

  // 2. 等待 OLED 稳定
  HAL_Delay(20);

  // 3. 初始化菜单系统
  Menu_Init();

  // 4. 创建菜单结构
  // 创建根菜单 (ID: 0)
  Menu_CreateMenu(0, "Main Menu", 0);

  // 添加菜单项到根菜单
  Menu_AddItem(0, 1, "Settings");
  Menu_AddItem(0, 2, "Sensors");
  Menu_AddItem(0, 3, "About");

  // 创建子菜单 (ID: 1) - Settings
  Menu_CreateMenu(1, "Settings", 0);
  Menu_AddItem(1, 11, "Brightness");
  Menu_AddItem(1, 12, "Volume");
  Menu_AddItem(1, 13, "Language");

  // 创建子菜单 (ID: 2) - Sensors
  Menu_CreateMenu(2, "Sensors", 0);
  Menu_AddItem(2, 21, "Temperature");
  Menu_AddItem(2, 22, "Humidity");
  Menu_AddItem(2, 23, "Pressure");

  // 创建子菜单 (ID: 3) - About
  Menu_CreateMenu(3, "About", 0);
  Menu_AddItem(3, 31, "Version");
  Menu_AddItem(3, 32, "Author");

  // 5. 显示初始菜单
  Menu_Show();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    // ========================================
    // 主循环 - 处理按键和更新菜单
    // ========================================

    // 使用原来的按键处理函数（状态机消抖）
    uint8_t key_event = key_process();

    // 处理按键事件
    if (key_event == KEY_UP_PRESSED)
    {
      HAL_GPIO_WritePin(led_up_GPIO_Port, led_up_Pin, GPIO_PIN_RESET);
      Menu_KeyUp();
      Menu_Show();
    }
    else if (key_event == KEY_DOWN_PRESSED)
    {
      HAL_GPIO_WritePin(led_down_GPIO_Port, led_down_Pin, GPIO_PIN_RESET);
      Menu_KeyDown();
      Menu_Show();
    }
    HAL_GPIO_WritePin(led_up_GPIO_Port, led_up_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(led_down_GPIO_Port, led_down_Pin, GPIO_PIN_SET);

    // 添加小延迟（避免 CPU 占用过高）
    HAL_Delay(10);
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/**
 * @brief  添加自定义功能的示例函数
 * @note   你可以在菜单项中调用这些函数
 * @note   这些函数目前只是示例，实际使用时需要添加具体功能
 */
void showTemperature(void)
{
  // 这里读取温度传感器数据
  // float temp = read_sensor();  // 示例：读取传感器

  // 显示信息（使用 OLED 驱动）
  // OLED_NewFrame();
  // OLED_PrintASCIIString(0, 0, "Temp: 25.5 C", &Font16, OLED_COLOR_NORMAL);
  // OLED_ShowFrame();
}

void showHumidity(void)
{
  // 这里读取湿度传感器数据
  // float humidity = read_sensor();  // 示例：读取传感器

  // 显示信息（使用 OLED 驱动）
  // OLED_NewFrame();
  // OLED_PrintASCIIString(0, 0, "Hum: 60.0%", &Font16, OLED_COLOR_NORMAL);
  // OLED_ShowFrame();
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
