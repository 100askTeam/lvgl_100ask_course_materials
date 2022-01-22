/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：driver_led.h
 * 摘要：
 *  
 * 修改历史     版本号        Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/

#ifndef __DRIVER_LED_H
#define __DRIVER_LED_H

#include "stm32f1xx_hal.h"

/*********************
 * 引脚宏定义
**********************/
#define R_LED_GPIO_PIN        GPIO_PIN_0
#define R_LED_GPIO_PORT       GPIOB
#define R_LED_GPIO_CLK_EN()   __HAL_RCC_GPIOB_CLK_ENABLE()

#define G_LED_GPIO_PIN        GPIO_PIN_1
#define G_LED_GPIO_PORT       GPIOB
#define G_LED_GPIO_CLK_EN()   __HAL_RCC_GPIOB_CLK_ENABLE()

#define B_LED_GPIO_PIN        GPIO_PIN_5
#define B_LED_GPIO_PORT       GPIOB
#define B_LED_GPIO_CLK_EN()   __HAL_RCC_GPIOB_CLK_ENABLE()

//引脚定义
/*******************************************************/
//R 红色灯
#define LEDRED_PIN                  GPIO_PIN_0//GPIO_PIN_5                
#define LEDRED_GPIO_PORT            GPIOB                     
#define LEDRED_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()

//G 绿色灯
#define LEDGREEN_PIN                  GPIO_PIN_1               
#define LEDGREEN_GPIO_PORT            GPIOB                      
#define LEDGREEN_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()

//B 蓝色灯
#define LEDBLUE_PIN                  GPIO_PIN_5//GPIO_PIN_1              
#define LEDBLUE_GPIO_PORT            GPIOB                       
#define LEDBLUE_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()

#define LEDRED(a)	HAL_GPIO_WritePin(LEDRED_GPIO_PORT,LEDRED_PIN,a)
#define LEDGREEN(a)	HAL_GPIO_WritePin(LEDGREEN_GPIO_PORT,LEDGREEN_PIN,a)
#define LEDBLUE(a)	HAL_GPIO_WritePin(LEDGREEN_GPIO_PORT,LEDBLUE_PIN,a)

/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)			{p->BSRR=i;}			  //设置为高电平		
#define digitalLo(p,i)			{p->BSRR=(uint32_t)i << 16;}				//输出低电平
#define digitalToggle(p,i)		{p->ODR ^=i;}			//输出反转状态

/* 定义控制IO的宏 */
#define LEDRED_TOGGLE		digitalToggle(LEDRED_GPIO_PORT,LEDRED_PIN)
#define LEDRED_OFF			digitalHi(LEDRED_GPIO_PORT,LEDRED_PIN)
#define LEDRED_ON			digitalLo(LEDRED_GPIO_PORT,LEDRED_PIN)

#define LEDGREEN_TOGGLE		digitalToggle(LEDGREEN_GPIO_PORT,LEDGREEN_PIN)
#define LEDGREEN_OFF		digitalHi(LEDGREEN_GPIO_PORT,LEDGREEN_PIN)
#define LEDGREEN_ON			digitalLo(LEDGREEN_GPIO_PORT,LEDGREEN_PIN)

#define LEDBLUE_TOGGLE		digitalToggle(LEDBLUE_GPIO_PORT,LEDBLUE_PIN)
#define LEDBLUE_OFF			digitalHi(LEDBLUE_GPIO_PORT,LEDBLUE_PIN)
#define LEDBLUE_ON			digitalLo(LEDBLUE_GPIO_PORT,LEDBLUE_PIN)


/*********************
 * 函数宏定义
**********************/
/*
 * LED亮灭函数宏定义
*/
#define RLED(flag)        HAL_GPIO_WritePin(R_LED_GPIO_PORT, R_LED_GPIO_PIN, flag&0x1?GPIO_PIN_SET:GPIO_PIN_RESET)   
#define RLED_T(flag)	  HAL_GPIO_TogglePin(R_LED_GPIO_PORT, R_LED_GPIO_PIN)  
#define GLED(flag)        HAL_GPIO_WritePin(G_LED_GPIO_PORT, G_LED_GPIO_PIN, flag&0x1?GPIO_PIN_SET:GPIO_PIN_RESET)    
#define BLED(flag)        HAL_GPIO_WritePin(B_LED_GPIO_PORT, B_LED_GPIO_PIN, flag&0x1?GPIO_PIN_SET:GPIO_PIN_RESET)    

/*
 *  函数名：void LedGpioInit(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：初始化LED的引脚，配置为上拉推挽输出
*/
extern void LedGpioInit(void);

#endif

