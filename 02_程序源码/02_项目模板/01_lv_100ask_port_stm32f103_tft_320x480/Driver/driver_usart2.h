/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：driver_usart.h
 * 摘要：
 *  
 * 修改历史     版本号       Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/

#ifndef __DRIVER_USART2_H
#define __DRIVER_USART2_H

#include "stm32f1xx_hal.h"

/*********************
 * 引脚宏定义
**********************/
#define RS232_MALE                  USART2
#define RS232_MALE_RX_PIN           GPIO_PIN_3
#define RS232_MALE_TX_PIN           GPIO_PIN_2
#define RS232_MALE_PORT             GPIOA
#define RS232_MALE_GPIO_CLK_EN()    __HAL_RCC_GPIOA_CLK_ENABLE()

#define RE_DE_PIN               GPIO_PIN_5
#define RE_DE_PORT              GPIOC
#define RE_DE_GPIO_CLK_EN()     __HAL_RCC_GPIOC_CLK_ENABLE()

#define RE_DE_TX()              HAL_GPIO_WritePin(RE_DE_PORT, RE_DE_PIN, GPIO_PIN_SET)
#define RE_DE_RX()              HAL_GPIO_WritePin(RE_DE_PORT, RE_DE_PIN, GPIO_PIN_RESET)

/*********************
 * 函数宏定义
**********************/
#define RS232_MALE_IRQn             USART2_IRQn
#define RS232_MALE_IRQHandler       USART2_IRQHandler
#define RS232_MALE_CLK_ENABLE()     __HAL_RCC_USART2_CLK_ENABLE()
#define RS232_MALE_CLK_DISABLE()    __HAL_RCC_USART2_CLK_DISABLE()
/*********************
 * 全局变量声明
**********************/
extern UART_HandleTypeDef husart2;
extern volatile uint8_t male_tx_finish;
extern volatile uint8_t male_rx_finish;

/*********************
 * 对外函数API
**********************/

/*
 *  函数名：void UsartInit(uint32_t baudrate)
 *  输入参数：baudrate-串口波特率
 *  输出参数：无
 *  返回值：无
 *  函数作用：初始化USART的波特率，收发选择，有效数据位等
*/
extern void RS232_MALE_Init(uint32_t baudrate);

/*
 *  函数名：void RS232_MALE_Tx(uint8_t *pdata, uint16_t sz)
 *  输入参数：pdata->指向发送数据所存储的首地址
              sz->发送数据个数
 *  输出参数：无
 *  返回值：无
 *  函数作用：USART2的发送函数
*/
extern void RS232_MALE_Tx(uint8_t *pdata, uint16_t sz);

/*
 *  函数名：void RS232_MALE_Rx(uint8_t *pdata, uint16_t sz)
 *  输入参数：pdata->指向接收数据所存储的首地址
              sz->接收数据个数
 *  输出参数：无
 *  返回值：无
 *  函数作用：USART2的接收函数
*/
extern void RS232_MALE_Rx(uint8_t *pdata, uint16_t sz);
#endif

