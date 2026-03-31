/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/LCD/chinese_font.h"
#include "./BSP/KEY/key.h"
#include "./BSP/TOUCH/touch.h"
#include "./User/TFTLCDTouch/lcd_touch.h"
#include "tim.h"

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
/* USER CODE BEGIN Variables */
uint8_t UART_BUF[8] = {0};
extern const uint8_t dog_image[];

/* PWM  */
uint16_t Fan_PWM_Value = 0;  /* 初始值 */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for myTaskUsart */
osThreadId_t myTaskUsartHandle;
const osThreadAttr_t myTaskUsart_attributes = {
  .name = "myTaskUsart",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow2,
};
/* Definitions for myTaskKey */
osThreadId_t myTaskKeyHandle;
const osThreadAttr_t myTaskKey_attributes = {
  .name = "myTaskKey",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for myQueue01 */
osMessageQueueId_t myQueue01Handle;
const osMessageQueueAttr_t myQueue01_attributes = {
  .name = "myQueue01"
};
/* Definitions for myTimer01 */
osTimerId_t myTimer01Handle;
const osTimerAttr_t myTimer01_attributes = {
  .name = "myTimer01"
};
/* Definitions for myMutex01 */
osMutexId_t myMutex01Handle;
const osMutexAttr_t myMutex01_attributes = {
  .name = "myMutex01"
};
/* Definitions for myRecursiveMutex01 */
osMutexId_t myRecursiveMutex01Handle;
const osMutexAttr_t myRecursiveMutex01_attributes = {
  .name = "myRecursiveMutex01",
  .attr_bits = osMutexRecursive,
};
/* Definitions for myBinarySem01 */
osSemaphoreId_t myBinarySem01Handle;
const osSemaphoreAttr_t myBinarySem01_attributes = {
  .name = "myBinarySem01"
};
/* Definitions for myCountingSem01 */
osSemaphoreId_t myCountingSem01Handle;
const osSemaphoreAttr_t myCountingSem01_attributes = {
  .name = "myCountingSem01"
};
/* Definitions for myEvent01 */
osEventFlagsId_t myEvent01Handle;
const osEventFlagsAttr_t myEvent01_attributes = {
  .name = "myEvent01"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTaskUsart(void *argument);
void StartTaskKey(void *argument);
void CallbackTimer01(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of myMutex01 */
  myMutex01Handle = osMutexNew(&myMutex01_attributes);

  /* Create the recursive mutex(es) */
  /* creation of myRecursiveMutex01 */
  myRecursiveMutex01Handle = osMutexNew(&myRecursiveMutex01_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of myBinarySem01 */
  myBinarySem01Handle = osSemaphoreNew(1, 0, &myBinarySem01_attributes);

  /* creation of myCountingSem01 */
  myCountingSem01Handle = osSemaphoreNew(2, 2, &myCountingSem01_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of myTimer01 */
  myTimer01Handle = osTimerNew(CallbackTimer01, osTimerPeriodic, NULL, &myTimer01_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of myQueue01 */
  myQueue01Handle = osMessageQueueNew (16, sizeof(uint16_t), &myQueue01_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of myTaskUsart */
  myTaskUsartHandle = osThreadNew(StartTaskUsart, NULL, &myTaskUsart_attributes);

  /* creation of myTaskKey */
  myTaskKeyHandle = osThreadNew(StartTaskKey, NULL, &myTaskKey_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* creation of myEvent01 */
  myEvent01Handle = osEventFlagsNew(&myEvent01_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
		
//		HAL_UART_Transmit_DMA(&husart1, 0x01, 1);
		printf("DefaultTask\n");
		LED_RED_ON;
		osDelay(1000);
		LED_RED_OFF;
		osDelay(1000);
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTaskUsart */
/**
* @brief Function implementing the myTaskUsart thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskUsart */
void StartTaskUsart(void *argument)
{
  /* USER CODE BEGIN StartTaskUsart */
	// 清屏
	lcd_clear(WHITE);
	//	//测试：图片展示
//	lcd_show_image(80, 250, 321, 314, (uint8_t*)dog_image);
	
// 测试：四个角的四个色块
	lcd_fill(0, 0, 100, 100, RED);      
	lcd_fill(380, 0, 480, 100, GREEN);  
	lcd_fill(0, 700, 100, 800, BLUE);   
	lcd_fill(380, 700, 480, 800, YELLOW); 
	
	/* 标题 */
//	lcd_show_string(180, 50, 200, 30, 24, "PWM Control", BLUE);
	lcd_show_chinese_string(180, 50, 24, "风机PWM控制", BLUE, g_back_color);
	
	/* PWM 标签 */
	lcd_show_string(150, 200, 200, 30, 24, "PWM Value:", BLACK);

	/* 绘制按钮 1 - 快 */
	lcd_draw_rectangle(BTN1_X, BTN1_Y, BTN1_X + BTN1_W, BTN1_Y + BTN1_H, GREEN);
	lcd_fill(BTN1_X + 2, BTN1_Y + 2, BTN1_X + BTN1_W - 2, BTN1_Y + BTN1_H - 2, GREEN);
	g_back_color = GREEN;
	lcd_show_chinese_string(BTN1_X + 60, BTN1_Y + 30, 24, "快", BLACK, g_back_color);  // "快"
	
	/* 绘制按钮 2 - 慢 */
	lcd_draw_rectangle(BTN2_X, BTN2_Y, BTN2_X + BTN2_W, BTN2_Y + BTN2_H, BLUE);
	lcd_fill(BTN2_X + 2, BTN2_Y + 2, BTN2_X + BTN2_W - 2, BTN2_Y + BTN2_H - 2, BLUE);
	g_back_color = BLUE;
	lcd_show_chinese_string(BTN2_X + 60, BTN2_Y + 30, 24, "慢", WHITE, g_back_color);  // "慢"
	
	/* 绘制按钮 3 - 停 */
	lcd_draw_rectangle(BTN3_X, BTN3_Y, BTN3_X + BTN3_W, BTN3_Y + BTN3_H, RED);
	lcd_fill(BTN3_X + 2, BTN3_Y + 2, BTN3_X + BTN3_W - 2, BTN3_Y + BTN3_H - 2, RED);
	g_back_color = RED;
	lcd_show_chinese_string(BTN3_X + 100, BTN3_Y + 30, 24, "停", WHITE, g_back_color);  // "停"
	
	/* 恢复背景色 */
	g_back_color = WHITE;
	
	/* 显示初始 PWM 值 */
	lcd_show_num(280, 200, Fan_PWM_Value, 3, 24, RED);
  /* Infinite loop */
  for(;;)
  {
//		osDelay(50);
		/* 触屏按钮检测 */
		uint8_t touch_btn = Check_Touch_Button();
		
		if (touch_btn != 0) {
			/* 有触屏按钮按下 */
			switch (touch_btn) {
				case 1:  /* 快按钮 */
					if (Fan_PWM_Value < 100) {
						Fan_PWM_Value += 1;
					}
					break;
				case 2:  /* 慢按钮 */
					if (Fan_PWM_Value > 0) {
						Fan_PWM_Value -= 1;
					}
					break;
				case 3:  /* 停按钮 */
					Fan_PWM_Value = 0;
					break;
			}
			/* 更新 PWM 显示 */
		// 1 先用白色矩形擦除旧数字
		lcd_fill(280, 200, 350, 230, WHITE);
		// 2 再显示新数字
		lcd_show_num(280, 200, Fan_PWM_Value, 3, 24, RED);
		__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, Fan_PWM_Value);
		}
	
  }
  /* USER CODE END StartTaskUsart */
}

/* USER CODE BEGIN Header_StartTaskKey */
/**
* @brief Function implementing the myTaskKey thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskKey */
void StartTaskKey(void *argument)
{
  /* USER CODE BEGIN StartTaskKey */
  /* Infinite loop */
  for(;;)
  {
		osDelay(200);  /* 200ms 扫描周期 */
    
    /* 扫描按键，支持连按 */
    uint8_t key_val = key_scan(1);
    
    /* 如果有按键按下，直接处理 */
    if (key_val != 0) {
      switch (key_val) {
        case 1:  /* KEY1 - 增加 */
          if (Fan_PWM_Value < 100) {
            Fan_PWM_Value += 1;           
          }
          break;
        case 2:  /* KEY0 - 减少 */
          if (Fan_PWM_Value > 0) {
            Fan_PWM_Value -= 1;
          }
          break;
        case 3:  /* KEY_UP - 停止 */
          Fan_PWM_Value = 0;
          break;
      }
			__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, Fan_PWM_Value);
			/* 更新显示 */
			lcd_fill(280, 200, 350, 230, WHITE);
			lcd_show_num(280, 200, Fan_PWM_Value, 3, 24, RED);
    }
  }
  /* USER CODE END StartTaskKey */
}

/* CallbackTimer01 function */
void CallbackTimer01(void *argument)
{
  /* USER CODE BEGIN CallbackTimer01 */

  /* USER CODE END CallbackTimer01 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

