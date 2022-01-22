/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：driver_Buzzer.h
 * 摘要：
 *  
 * 修改历史     版本号       Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/

#ifndef __DRIVER_Buzzer_H
#define __DRIVER_Buzzer_H

#include "stm32f1xx_hal.h"

/*********************
 * 引脚宏定义
**********************/
#define Buzzer_GPIO_PIN           GPIO_PIN_8
#define Buzzer_GPIO_PORT          GPIOA
#define Buzzer_GPIO_CLK_EN()      __HAL_RCC_GPIOA_CLK_ENABLE()

/*********************
 * 函数宏定义
**********************/
#define Buzzer(flag)              HAL_GPIO_WritePin(Buzzer_GPIO_PORT, Buzzer_GPIO_PIN, flag)  

/*
 *  函数名：void BuzzerInit(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：初始化蜂鸣器的引脚
*/
extern void BuzzerInit(void);

/*
 *  函数名：void ControlBuzzerRang(uint16_t period)
 *  输入参数：period-蜂鸣器发出响声的周期，f=1/(2*period)
 *  输出参数：无
 *  返回值：无
 *  函数作用：初始化系统滴答时钟的频率和中断优先级
*/
extern void ControlBuzzerRang(uint16_t period);

#endif


