/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：ring_buffer.h
 * 摘要：
 *  
 * 修改历史     版本号        Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/

#ifndef __RING_BUFFER_H
#define __RING_BUFFER_H

#include "stm32f1xx_hal.h"

extern void RingBuffer_Init(void);
extern int RingBuffer_Write(uint8_t *buffer, uint32_t len);
extern int RingBuffer_Read(uint8_t *buffer, uint32_t len);
extern uint32_t RingBuffer_GetValidLen(void);
extern void RingBuffer_Release(void);

#endif


