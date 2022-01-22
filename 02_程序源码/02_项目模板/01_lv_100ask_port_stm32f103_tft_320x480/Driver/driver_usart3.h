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

#ifndef __DRIVER_USART3_H
#define __DRIVER_USART3_H

#include "stm32f1xx_hal.h"

/*********************
 * 引脚宏定义
**********************/
#define RS232_FEMALE                    USART3
#define RS232_FEMALE_RX_PIN             GPIO_PIN_11
#define RS232_FEMALE_TX_PIN             GPIO_PIN_10
#define RS232_FEMALE_PORT               GPIOB
#define RS232_FEMALE_GPIO_CLK_EN()      __HAL_RCC_GPIOB_CLK_ENABLE()
    
/*********************
 * 函数宏定义
**********************/
#define RS232_FEMALE_IRQn               USART3_IRQn
#define RS232_FEMALE_IRQHandler         USART3_IRQHandler
#define RS232_FEMALE_CLK_ENABLE()       __HAL_RCC_USART3_CLK_ENABLE()
#define RS232_FEMALE_CLK_DISABLE()      __HAL_RCC_USART3_CLK_DISABLE()
              
/*********************
 * 全局变量声明
**********************/
extern UART_HandleTypeDef husart3;
extern volatile uint8_t female_tx_finish;
extern volatile uint8_t female_rx_finish;

/*********************
 * 对外函数API
**********************/

/*
 *  函数名：void RS232_FEMALE_Init(uint32_t baudrate)
 *  输入参数：baudrate-串口波特率
 *  输出参数：无
 *  返回值：无
 *  函数作用：初始化USART的波特率，收发选择，有效数据位等
*/
extern void RS232_FEMALE_Init(uint32_t baudrate);

/*
 *  函数名：void RS232_FEMALE_Tx(uint8_t *pdata, uint16_t sz)
 *  输入参数：pdata->指向发送数据所存储的首地址
              sz->发送数据个数
 *  输出参数：无
 *  返回值：无
 *  函数作用：USART3的发送函数
*/
extern void RS232_FEMALE_Tx(uint8_t *pdata, uint16_t sz);

/*
 *  函数名：void RS232_FEMALE_Rx(uint8_t *pdata, uint16_t sz)
 *  输入参数：pdata->指向接收数据所存储的首地址
              sz->接收数据个数
 *  输出参数：无
 *  返回值：无
 *  函数作用：USART3的接收函数
*/
extern void RS232_FEMALE_Rx(uint8_t *pdata, uint16_t sz);
#endif

