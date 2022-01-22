/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：driver_sram.h
 * 摘要：
 *  
 * 修改历史     版本号        Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/
#ifndef __DRIVER_SRAM_H
#define __DRIVER_SRAM_H

#include "stm32f1xx_hal.h"

/*
 *  函数名：void SRAM_WriteBufferBytes(uint8_t* pdata, uint32_t addr, uint16_t sz)
 *  输入参数：pdata->要写入数据的首地址
 *            addr ->要写到RAM的初始地址
 *            sz   ->要写入的数据个数
 *  输出参数：无
 *  返回值：无
 *  函数作用：往片外RAM地址addr开始写入sz个字节
*/
extern void SRAM_WriteBufferBytes(uint8_t* pBuffer, uint32_t uwWriteAddress, uint16_t uwBufferSize);

/*
 *  函数名：void SRAM_ReadBufferBytes(uint8_t* pdata, uint32_t addr, uint16_t sz)
 *  输入参数：pdata->要读出数据的首地址
 *            addr ->要读出的RAM的初始地址
 *            sz   ->要写读出的数据个数
 *  输出参数：无
 *  返回值：无
 *  函数作用：往片外RAM地址addr开始读出sz个字节
*/
extern void SRAM_ReadBufferBytes(uint8_t* pBuffer, uint32_t uwReadAddress, uint16_t uwBufferSize);

/*
 *  函数名：void SRAM_WriteBufferWord(uint16_t *pdata, uint32_t addr, uint16_t sz)
 *  输入参数：pdata->要写入数据的首地址
 *            addr ->要写到RAM的初始地址
 *            sz   ->要写入的数据个数
 *  输出参数：无
 *  返回值：无
 *  函数作用：往片外RAM地址addr开始写入sz个半字
*/
extern void SRAM_WriteBufferHalfWord(uint16_t* pBuffer, uint32_t uwWriteAddress, uint16_t uwBufferSize);

/*
 *  函数名：void SRAM_ReadBufferHalfWord(uint16_t *pdata, uint32_t addr, uint16_t sz)
 *  输入参数：pdata->要读出数据的首地址
 *            addr ->要读出的RAM的初始地址
 *            sz   ->要写读出的数据个数
 *  输出参数：无
 *  返回值：无
 *  函数作用：往片外RAM地址addr开始读出sz个半字
*/
extern void SRAM_ReadBufferHalfWord(uint16_t* pBuffer, uint32_t uwReadAddress, uint16_t uwBufferSize);

/*
 *  函数名：void SRAM_WriteBufferWord(uint32_t *pdata, uint32_t addr, uint32_t sz)
 *  输入参数：pdata->要写入数据的首地址
 *            addr ->要写到RAM的初始地址
 *            sz   ->要写入的数据个数
 *  输出参数：无
 *  返回值：无
 *  函数作用：往片外RAM地址addr开始写入sz个字
*/
extern void SRAM_WriteBufferWord(uint32_t* pBuffer, uint32_t uwWriteAddress, uint32_t uwBufferSize);

/*
 *  函数名：void SRAM_ReadBufferWord(uint32_t *pdata, uint32_t addr, uint32_t sz)
 *  输入参数：pdata->要读出数据的首地址
 *            addr ->要读出的RAM的初始地址
 *            sz   ->要写读出的数据个数
 *  输出参数：无
 *  返回值：无
 *  函数作用：往片外RAM地址addr开始读出sz个字
*/
extern void SRAM_ReadBufferWord(uint32_t* pBuffer, uint32_t uwReadAddress, uint32_t uwBufferSize);

#endif


