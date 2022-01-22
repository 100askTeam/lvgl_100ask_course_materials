/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：ring_buffer.c
 * 摘要：
 *  
 * 修改历史     版本号        Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ring_buffer.h"

#define BUFF_SIZE               (200)

static volatile uint32_t valid_len = 0;
static volatile uint8_t *pHead = (volatile uint8_t*)0x68000000;
static volatile uint8_t *pEnd = NULL;
static volatile uint8_t *pValid = NULL;
static volatile uint8_t *pValidEnd = NULL;

void RingBuffer_Init(void)
{
//    if(pHead==NULL)
//    {
        pHead = (uint8_t*)malloc(BUFF_SIZE);
//    }
    
    pValid = pValidEnd = pHead;
    pEnd = pHead + BUFF_SIZE;
    
    valid_len = 0;
}

int RingBuffer_Write(uint8_t *buffer, uint32_t len)
{
    int len1 = 0, len2 = 0;
    int move_len = 0;
    
    if(len > BUFF_SIZE)
    {
        return -2;
    }
    if(pHead==NULL)
    {
        return -1;
    }
    assert(buffer);
    
    // copy buffer to pValidEnd
    if( (pValidEnd + len) > pEnd )  // 超过了Buffer范围需要分为两段
    {
        len1 = pEnd - pValidEnd;
        len2 = len - len1;
        
        memcpy((uint8_t*)pValidEnd, buffer, len1);
        memcpy((uint8_t*)pHead, buffer + len1, len2);
        
        pValidEnd = pHead + len2;   // 更新有效数据区尾地址
    }
    else
    {
        memcpy((uint8_t*)pValidEnd, buffer, len);
        pValidEnd = pValidEnd + len;
    }
    
    // 重新计算已使用区的起始位置
    if( (valid_len + len) > BUFF_SIZE )     // 要写入的数据超过了缓冲区总长度，分为两段写
    {
        move_len = valid_len + len - BUFF_SIZE;
        if( (pValid + move_len) > pEnd )
        {
            len1 = pEnd - pValid;
            len2 = move_len - len1;
            
            pValid = pHead + len2;
        }
        else
        {
            pValid = pValid + move_len;
        }
        
        valid_len = BUFF_SIZE;
    }
    else
    {
        valid_len = valid_len + len;
    }
    
    return 0;
}

int RingBuffer_Read(uint8_t *buffer, uint32_t len)
{
    int len1 = 0, len2 = 0;
    if(pHead==NULL)
    {
        return -1;
    }
    assert(buffer);
    
    if(valid_len==0)
    {
        return 0;
    }
    
    if(len > valid_len)
    {
        len = valid_len;
    }
    
    if( (pValid + len) > pEnd )
    {
        len1 = pEnd - pValid;
        len2 = len - len1;
        
        memcpy(buffer, (uint8_t*)pValid, len1);
        memcpy(buffer + len1, (uint8_t*)pHead, len2);
        
        pValid = pHead + len2;
    }
    else
    {
        memcpy(buffer, (uint8_t*)pValid, len);
        pValid = pValid + len;
    }
    
    valid_len -= len;
    
    return len;
}

uint32_t RingBuffer_GetValidLen(void)
{
    return valid_len;
}

void RingBuffer_Release(void)
{
    if(pHead != NULL)
    {
        free((uint8_t*)pHead);
    }
    
    pHead = NULL;
}

