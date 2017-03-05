/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>

//OS
#include "FreeRTOS.h"
#include "task.h"

//C库
#include <string.h>

//硬件驱动
#include "delay.h"
#include "usart.h"
#include "iwdg.h"

//看门狗任务
#define IWDG_TASK_PRIO		12
#define IWDG_STK_SIZE		64
TaskHandle_t IWDG_Task_Handler;
void IWDG_Task(void *pvParameters);

//串口任务
#define USART_TASK_PRIO		11
#define USART_STK_SIZE		512
TaskHandle_t USART_Task_Handler;
void USART_Task(void *pvParameters);


/*
************************************************************
*	函数名称：	Hardware_Init
*
*	函数功能：	硬件初始化
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		初始化单片机功能以及外接设备
************************************************************
*/
void Hardware_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//中断控制器分组设置
	Delay_Init();							//systick初始化
	Usart1_Init(115200); 						//初始化串口   115200bps
	
	Iwdg_Init(4, 1250); 						//64分频，每秒625次，重载1250次，2s
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	Hardware_Init();
	
	//创建应用任务
	xTaskCreate((TaskFunction_t)IWDG_Task, "IWDG", IWDG_STK_SIZE, NULL, IWDG_TASK_PRIO, (TaskHandle_t*)&IWDG_Task_Handler);
	xTaskCreate((TaskFunction_t)USART_Task, "USART", USART_STK_SIZE, NULL, USART_TASK_PRIO, (TaskHandle_t*)&USART_Task_Handler);
	
	
	UsartPrintf(USART_DEBUG, "4.OSStart\r\n");			//提示任务开始执行
	vTaskStartScheduler();							//开始任务调度
	
	return 0;
}

/*
************************************************************
*	函数名称：	IWDG_Task
*
*	函数功能：	清除看门狗
*
*	入口参数：	void类型的参数指针
*
*	返回参数：	无
*
*	说明：		看门狗任务
************************************************************
*/
void IWDG_Task(void *pdata)
{
	while(1)
	{
		Iwdg_Feed(); 		//喂狗
		RTOS_TimeDly(50); 	//挂起任务250ms
	}
}

/*
************************************************************
*	函数名称：	USART_Task
*
*	函数功能：	处理平台下发的命令
*
*	入口参数：	void类型的参数指针
*
*	返回参数：	无
*
*	说明：		串口接收任务。在数据模式下时，等待平台下发的命令并解析、处理
************************************************************
*/
void USART_Task(void *pdata)
{
	while(1)
	{
		RTOS_TimeDly(2);	//挂起任务10ms
	}

}
