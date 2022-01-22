/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：driver_I2C.h
 * 摘要：
 *  
 * 修改历史     版本号       Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/
#ifndef __DRIVER_I2C_H
#define __DRIVER_I2C_H

#include "stm32f1xx_hal.h"

/************************* I2C 硬件相关定义 *************************/
#define ACK                 (1)
#define NACK                (0)

#define SCL_PIN             GPIO_PIN_6
#define SCL_PORT            GPIOB
#define SCL_PIN_CLK_EN()    __HAL_RCC_GPIOB_CLK_ENABLE()

#define SDA_PIN             GPIO_PIN_7
#define SDA_PORT            GPIOB
#define SDA_PIN_CLK_EN()    __HAL_RCC_GPIOB_CLK_ENABLE()

#define SCL_H()             HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_SET)
#define SCL_L()             HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_RESET)
#define SCL_INPUT()         HAL_GPIO_ReadPin(SCL_PORT, SCL_PIN)
                            
#define SDA_H()             HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_SET)
#define SDA_L()             HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_RESET)
#define SDA_INPUT()         HAL_GPIO_ReadPin(SDA_PORT, SDA_PIN)
/************************* I2C 硬件相关定义结束 *************************/
/*
 *  函数名：void I2C_Init(void)
 *  输入参数：
 *  输出参数：无
 *  返回值：无
 *  函数作用：初始化模拟I2C的引脚为输出状态且SCL/SDA都初始为高电平
*/
extern void I2C_Init(void);

/*
 *  函数名：void I2C_Start(void)
 *  输入参数：
 *  输出参数：无
 *  返回值：无
 *  函数作用：I2C开始信号
*/
extern void I2C_Start(void);

/*
 *  函数名：void I2C_Stop(void)
 *  输入参数：
 *  输出参数：无
 *  返回值：无
 *  函数作用：I2C停止信号
*/
extern void I2C_Stop(void);

/*
 *  函数名：void I2C_ACK(void)
 *  输入参数：
 *  输出参数：无
 *  返回值：无
 *  函数作用：I2C发出应答信号
*/
extern void I2C_ACK(void);

/*
 *  函数名：void I2C_NACK(void)
 *  输入参数：
 *  输出参数：无
 *  返回值：无
 *  函数作用：I2C发出非应答信号
*/
extern void I2C_NACK(void);

/*
 *  函数名：uint8_t I2C_GetACK(void)
 *  输入参数：
 *  输出参数：无
 *  返回值：无
 *  函数作用：I2C等待从机的应答信号
*/
extern uint8_t I2C_GetACK(void);

/*
 *  函数名：void I2C_SendByte(uint8_t data)
 *  输入参数：data->发送的数据
 *  输出参数：无
 *  返回值：无
 *  函数作用：I2C发送一个字节
*/
extern void I2C_SendByte(uint8_t data);

/*
 *  函数名：uint8_t I2C_ReadByte(uint8_t ack)
 *  输入参数：ack->发送的应答标志，1应答，0非应答
 *  输出参数：无
 *  返回值：返回读到的字节
 *  函数作用：I2C读出一个字节
*/
extern uint8_t I2C_ReadByte(uint8_t ack);

#endif  //__DRIVER_SPI_H

