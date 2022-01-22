/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：driver_eeprom.h
 * 摘要：
 *  
 * 修改历史     版本号       Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/

#ifndef __DRIVER_EEPROM_H
#define __DRIVER_EEPROM_H

#include "stm32f1xx_hal.h"

#define EEPROM_DEV_ADDR             (0xA0)
#define EEPROM_WR                   (0x00)
#define EEPROM_RD                   (0x01)
#define EEPROM_WORD_ADDR_SIZE       (0x08)

/*
 *  函数名：uint8_t EEPROM_WriteByte(uint16_t addr, uint8_t data)
 *  输入参数：addr -> 写一个字节的EEPROM初始地址
 *            data -> 要写的数据
 *  输出参数：无
 *  返回值：无
 *  函数作用：EEPROM写一个字节
*/
extern uint8_t EEPROM_WriteByte(uint8_t addr, uint8_t data);

/*
 *  函数名：uint8_t EEPROM_ReadByte(uint16_t addr, uint8_t *pdata)
 *  输入参数：addr -> 读一个字节的EEPROM初始地址
 *            data -> 要读的数据指针
 *  输出参数：无
 *  返回值：无
 *  函数作用：EEPROM读一个字节
*/
extern uint8_t EEPROM_ReadByte(uint8_t addr, uint8_t *pdata);

/*
 *  函数名：void EEPROM_Write_NBytes(uint16_t addr, uint8_t *pdata, uint16_t sz)
 *  输入参数：addr -> 写一个字节的EEPROM初始地址
 *            data -> 要写的数据指针
 *            sz   -> 要写的字节个数
 *  输出参数：无
 *  返回值：无
 *  函数作用：EEPROM写N个字节
*/
extern void EEPROM_Write_NBytes(uint8_t addr, uint8_t *pdata, uint16_t sz);

/*
 *  函数名：void EEPROM_Read_NBytes(uint16_t addr, uint8_t *pdata, uint16_t sz)
 *  输入参数：addr -> 读一个字节的EEPROM初始地址
 *            data -> 要读的数据指针
 *            sz   -> 要读的字节个数
 *  输出参数：无
 *  返回值：无
 *  函数作用：EEPROM读N个字节
*/
extern void EEPROM_Read_NBytes(uint8_t addr, uint8_t *pdata, uint16_t sz);

#endif

