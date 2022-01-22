/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：driver_usart.c
 * 摘要：
 *  
 * 修改历史     版本号       Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/

#include "main.h"
#include "driver_usart3.h"

/*
 * 定义全局变量
*/
UART_HandleTypeDef husart3;
volatile uint8_t female_tx_finish = 0;
volatile uint8_t female_rx_finish = 0;

/*
 *  函数名：void RS232_FEMALE_Init(uint32_t baudrate)
 *  输入参数：baudrate-串口波特率
 *  输出参数：无
 *  返回值：无
 *  函数作用：初始化USART的波特率，收发选择，有效数据位等
*/
void RS232_FEMALE_Init(uint32_t baudrate)
{
    husart3.Instance         = RS232_FEMALE;                  // 选择USART3
    husart3.Init.BaudRate    = baudrate;                // 配置波特率
    husart3.Init.WordLength  = USART_WORDLENGTH_8B;     // 配置数据有效位为8bit
    husart3.Init.StopBits    = USART_STOPBITS_1;        // 配置一位停止位
    husart3.Init.Parity      = USART_PARITY_NONE;       // 不设校验位
    husart3.Init.Mode        = USART_MODE_TX_RX;        // 可收可发
    husart3.Init.HwFlowCtl   = UART_HWCONTROL_NONE;
    
    // 使用库函数初始化USART3的参数
    if (HAL_UART_Init(&husart3) != HAL_OK)
    {
        Error_Handler();
    }
    __HAL_UART_ENABLE_IT(&husart3,UART_IT_RXNE);  
}

/*
 *  函数名：void USART3_IRQHandler(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：USART3的中断服务函数
*/
void USART3_IRQHandler(void)
{
    HAL_UART_IRQHandler(&husart3);
}

/*
 *  函数名：void RS232_FEMALE_Tx(uint8_t *pdata, uint16_t sz)
 *  输入参数：pdata->指向发送数据所存储的首地址
              sz->发送数据个数
 *  输出参数：无
 *  返回值：无
 *  函数作用：USART3的发送函数
*/
void RS232_FEMALE_Tx(uint8_t *pdata, uint16_t sz)
{
    female_tx_finish = 0;
    HAL_UART_Transmit_IT(&husart3, pdata, sz);
}

/*
 *  函数名：void RS232_FEMALE_Rx(uint8_t *pdata, uint16_t sz)
 *  输入参数：pdata->指向接收数据所存储的首地址
              sz->接收数据个数
 *  输出参数：无
 *  返回值：无
 *  函数作用：USART3的接收函数
*/
void RS232_FEMALE_Rx(uint8_t *pdata, uint16_t sz)
{
    female_rx_finish = 0;
    HAL_UART_Receive_IT(&husart3, pdata, sz);
}

