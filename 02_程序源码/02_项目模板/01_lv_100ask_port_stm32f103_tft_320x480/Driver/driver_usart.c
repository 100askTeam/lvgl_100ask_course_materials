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
#include <stdio.h>
#include "main.h"
#include "driver_usart.h"
/*
 * 定义全局变量
*/
UART_HandleTypeDef husart;
volatile uint8_t debug_tx_finish = 0;
volatile uint8_t debug_rx_finish = 0;

/*
 *  函数名：void UsartInit(uint32_t baudrate)
 *  输入参数：baudrate-串口波特率
 *  输出参数：无
 *  返回值：无
 *  函数作用：初始化USART的波特率，收发选择，有效数据位等
*/
void UsartInit(uint32_t baudrate)
{
    husart.Instance         = USARTx;                  // 选择USART1
    husart.Init.BaudRate    = baudrate;                // 配置波特率
    husart.Init.WordLength  = USART_WORDLENGTH_8B;     // 配置数据有效位为8bit
    husart.Init.StopBits    = USART_STOPBITS_1;        // 配置一位停止位
    husart.Init.Parity      = USART_PARITY_NONE;       // 不设校验位
    husart.Init.Mode        = USART_MODE_TX_RX;        // 可收可发
    husart.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    
    // 使用库函数初始化USART3的参数
    if (HAL_UART_Init(&husart) != HAL_OK)
    {
        Error_Handler();
    }
}

/*
 *  函数名：void RS232_MALE_Tx(uint8_t *pdata, uint16_t sz)
 *  输入参数：pdata->指向发送数据所存储的首地址
              sz->发送数据个数
 *  输出参数：无
 *  返回值：无
 *  函数作用：USART2的发送函数
*/
void DebugUsart_Tx(uint8_t *pdata, uint16_t sz)
{
    debug_tx_finish = 0;
    HAL_UART_Transmit_IT(&husart, pdata, sz);
}

/*
 *  函数名：void RS232_MALE_Rx(uint8_t *pdata, uint16_t sz)
 *  输入参数：pdata->指向接收数据所存储的首地址
              sz->接收数据个数
 *  输出参数：无
 *  返回值：无
 *  函数作用：USART2的接收函数
*/
void DebugUsart_Rx(uint8_t *pdata, uint16_t sz)
{
    debug_rx_finish = 0;
    HAL_UART_Receive_IT(&husart, pdata, sz);
}

void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&husart);
}

/*
 * 添加如下代码就不用在工程设置中勾选Use MicroLIB
*/

/********** 禁用半主机模式 **********/
#pragma import(__use_no_semihosting)
 
struct __FILE
{
	int a;
};
 
//extern FILE __stdout, __stdin;
FILE __stdout;
FILE __stdin;
void _sys_exit(int x)
{
	
}

/*****************************************************
*function:	写字符文件函数
*param1:	输出的字符
*param2:	文件指针
*return:	输出字符的ASCII码
******************************************************/
int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&husart, (uint8_t*)&ch, 1, 10);
	return ch;
}

/*****************************************************
*function:	读字符文件函数
*param1:	文件指针
*return:	读取字符的ASCII码
******************************************************/
int fgetc(FILE *f)
{
    uint8_t ch = 0;
    HAL_UART_Receive(&husart, (uint8_t*)&ch, 1, 10);
    return (int)ch;
}

//__use_no_semihosting was requested, but _ttywrch was 
_ttywrch(int ch)
{
	ch = ch;
}
