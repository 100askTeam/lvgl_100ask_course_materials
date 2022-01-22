/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：driver_can.h
 * 摘要：
 *  
 * 修改历史     版本号        Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/
#ifndef __DRIVER_CAN_H
#define __DRIVER_CAN_H

#include "stm32f1xx_hal.h"

extern CAN_HandleTypeDef     CanHandle;
extern CAN_TxHeaderTypeDef   TxHeader;
extern CAN_RxHeaderTypeDef   RxHeader;

/*
 *  函数名：void CAN_Init(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：初始化CAN1
*/
extern void CAN_Init(void);

/*
 *  函数名：void CAN_Transmit(uint16_t ID, uint8_t *pdata, uint8_t length)
 *  输入参数：ID    -> CAN发送报文的ID
 *            pdata -> 发送报文的首地址
 *            length-> 发送报文的个数，最大8个字节
 *  输出参数：无
 *  返回值：无
 *  函数作用：CAN1发送函数
*/
extern void CAN_Transmit(uint16_t ID, uint8_t *pdata, uint8_t length);

extern void CAN_SetRxFlag(uint8_t flag);
extern uint8_t CAN_GetRxFlag(void);
#endif

