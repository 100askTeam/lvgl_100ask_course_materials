/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：driver_key.h
 * 摘要：
 *  
 * 修改历史     版本号        Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/

#ifndef __DRIVER_KEY_H
#define __DRIVER_KEY_H

#include "stm32f1xx_hal.h"

/*********************
 * 按键引脚状态定义
**********************/
#define PUSH_DOWN                       GPIO_PIN_RESET
#define SPRING_UP                       GPIO_PIN_set

/*********************
 * 引脚宏定义
**********************/
#define KEY_UP_GPIO_PIN                 GPIO_PIN_0
#define KEY_UP_GPIO_PORT                GPIOA
#define KEY_UP_GPIO_CLK_EN()            __HAL_RCC_GPIOA_CLK_ENABLE()

#define KEY_DOWN_GPIO_PIN               GPIO_PIN_15
#define KEY_DOWN_GPIO_PORT              GPIOG
#define KEY_DOWN_GPIO_CLK_EN()          __HAL_RCC_GPIOG_CLK_ENABLE()

#define KEY_LEFT_GPIO_PIN               GPIO_PIN_13
#define KEY_LEFT_GPIO_PORT              GPIOC
#define KEY_LEFT_GPIO_CLK_EN()          __HAL_RCC_GPIOC_CLK_ENABLE()

#define KEY_RIGHT_GPIO_PIN              GPIO_PIN_3
#define KEY_RIGHT_GPIO_PORT             GPIOE
#define KEY_RIGHT_GPIO_CLK_EN()         __HAL_RCC_GPIOE_CLK_ENABLE()

/*********************
 * 函数宏定义
**********************/
/*
 * 按键状态读取函数宏定义
*/
#define KEY_UP                  HAL_GPIO_ReadPin(KEY_UP_GPIO_PORT, KEY_UP_GPIO_PIN)
#define KEY_DOWN                HAL_GPIO_ReadPin(KEY_DOWN_GPIO_PORT, KEY_DOWN_GPIO_PIN)
#define KEY_LEFT                HAL_GPIO_ReadPin(KEY_LEFT_GPIO_PORT, KEY_LEFT_GPIO_PIN)
#define KEY_RIGHT               HAL_GPIO_ReadPin(KEY_RIGHT_GPIO_PORT, KEY_RIGHT_GPIO_PIN)

typedef struct
{
    uint8_t up_value;
    uint8_t down_value;
    uint8_t left_value;
    uint8_t right_value;
}KeyValue;

/*
 *  函数名：void KeyInit(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：初始化按键的引脚，配置为下降沿触发外部中断
*/
extern void KeyInit(void);



extern KeyValue key_value;

#endif

