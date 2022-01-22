/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：driver_flash.h
 * 摘要：
 *  
 * 修改历史     版本号       Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/

#ifndef __DRIVER_FLASH_H
#define __DRIVER_FLASH_H

#include "stm32f1xx_hal.h"

#define TEST_PAGE_ADDR          (0x0800C000)
#define TEST_PAGE_SIZE          (0x800)
#define TEST_PAGE_END_ADDR      (TEST_PAGE_ADDR + 4 * TEST_PAGE_SIZE)

/* Notable Flash addresses */
#define USER_FLASH_BANK1_START_ADDRESS  0x08000000
#define USER_FLASH_BANK2_START_ADDRESS  0x08080000
#define USER_FLASH_BANK1_END_ADDRESS    FLASH_BANK1_END

/* Error code */
enum 
{
  FLASHIF_OK = 0,
  FLASHIF_ERASEKO,
  FLASHIF_WRITINGCTRL_ERROR,
  FLASHIF_WRITING_ERROR,
  FLASHIF_PROTECTION_ERRROR
};

/*
 *  函数名：void EraseFlash(uint32_t addr, uint32_t length)
 *  输入参数：addr -> 擦除地址;    length -> 擦除的字节个数
 *  输出参数：无
 *  返回值：无
 *  函数作用：擦除内部flash
*/
extern void EraseFlash(uint32_t addr, uint32_t length);

/*
 *  函数名：void WriteFlash(uint32_t startAddr, uint32_t *pdata, uint32_t length)
 *  输入参数：startAddr-> 写首地址;    pdata -> 写的数据指针;      length -> 写的字节个数
 *  输出参数：无
 *  返回值：无
 *  函数作用：写内部flash
*/
extern void WriteFlash(uint32_t startAddr, uint32_t *pdata, uint32_t length);

/*
 *  函数名：void ReadFlash(uint32_t startAddr, uint32_t *pdata, uint32_t length)
 *  输入参数：startAddr-> 读首地址;    pdata -> 读出的数据指针;      length -> 读的字节个数
 *  输出参数：无
 *  返回值：无
 *  函数作用：读内部flash
*/
extern void ReadFlash(uint32_t startAddr, uint32_t *pdata, uint32_t length);

extern uint32_t FLASH_If_Erase(uint32_t start);
extern uint32_t FLASH_If_Write(uint32_t destination, uint32_t *p_source, uint32_t length);

#endif

