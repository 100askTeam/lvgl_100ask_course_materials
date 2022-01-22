/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：driver_eeprom.c
 * 摘要：
 *  
 * 修改历史     版本号       Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/
#include <stdio.h>
#include "main.h"
#include "driver_I2C.h"
#include "driver_timer.h"
#include "driver_usart.h"
#include "driver_eeprom.h"

#define ERRPROM_Delay()         HAL_Delay(10)

/*
 *  函数名：uint8_t EEPROM_WriteByte(uint16_t addr, uint8_t data)
 *  输入参数：addr -> 写一个字节的EEPROM初始地址
 *            data -> 要写的数据
 *  输出参数：无
 *  返回值：无
 *  函数作用：EEPROM写一个字节
*/
uint8_t EEPROM_WriteByte(uint8_t addr, uint8_t data)
{
    /* 1. Start */
    I2C_Start();
    
    /* 2. Write Device Address */
    I2C_SendByte( EEPROM_DEV_ADDR | EEPROM_WR );
    
    /* 3. Data Address */
    if(EEPROM_WORD_ADDR_SIZE==0x08)
    {
        I2C_SendByte( (uint8_t)(addr & 0x00FF) );
    }
    else
    {
        I2C_SendByte( (uint8_t)(addr>>8) );
        I2C_SendByte( (uint8_t)(addr & 0x00FF) );
    }
    
    /* 4. Write a byte */
    I2C_SendByte(data);
    
    /* 5. Stop */
    I2C_Stop();
	
	return 0;
}


/*
 *  函数名：uint8_t EEPROM_ReadByte(uint16_t addr, uint8_t *pdata)
 *  输入参数：addr -> 读一个字节的EEPROM初始地址
 *            data -> 要读的数据指针
 *  输出参数：无
 *  返回值：无
 *  函数作用：EEPROM读一个字节
*/
uint8_t EEPROM_ReadByte(uint8_t addr, uint8_t *pdata)
{
    /* 1. Start */
    I2C_Start();
    
    /* 2. Write Device Address */
    I2C_SendByte( EEPROM_DEV_ADDR | EEPROM_WR );
    
    
    /* 3. Data Address */
    if(EEPROM_WORD_ADDR_SIZE==0x08)
    {
        I2C_SendByte( (uint8_t)(addr & 0x00FF) );
    }
    else
    {
        I2C_SendByte( (uint8_t)(addr>>8) );
        I2C_SendByte( (uint8_t)(addr & 0x00FF) );
    }
    
    /* 4. Start Again */
    I2C_Start();
    
    /* 5. Write Device Address Read */
    I2C_SendByte( EEPROM_DEV_ADDR | EEPROM_RD );
    /* 6.Read a byte */
    *pdata = I2C_ReadByte(NACK); 
    /* 7. Stop */
    I2C_Stop();
	
	return 0;
}

/*
 *  函数名：void EEPROM_Write_NBytes(uint16_t addr, uint8_t *pdata, uint16_t sz)
 *  输入参数：addr -> 写一个字节的EEPROM初始地址
 *            data -> 要写的数据指针
 *            sz   -> 要写的字节个数
 *  输出参数：无
 *  返回值：无
 *  函数作用：EEPROM写N个字节
*/
void EEPROM_Write_NBytes(uint8_t addr, uint8_t *pdata, uint16_t sz)
{
    //uint8_t tmp = 0;
    uint16_t i = 0;
    
    for(i=0; i<sz;i++)
    {
        EEPROM_WriteByte(addr, pdata[i]);
        ERRPROM_Delay();
        addr++;
    }
}

/*
 *  函数名：void EEPROM_Read_NBytes(uint16_t addr, uint8_t *pdata, uint16_t sz)
 *  输入参数：addr -> 读一个字节的EEPROM初始地址
 *            data -> 要读的数据指针
 *            sz   -> 要读的字节个数
 *  输出参数：无
 *  返回值：无
 *  函数作用：EEPROM读N个字节
*/
void EEPROM_Read_NBytes(uint8_t addr, uint8_t *pdata, uint16_t sz)
{
    uint16_t i = 0;
    
    for(i=0; i<sz; i++)
    {
        EEPROM_ReadByte(addr, &pdata[i]);
        addr++;
        ERRPROM_Delay();
    }
}

