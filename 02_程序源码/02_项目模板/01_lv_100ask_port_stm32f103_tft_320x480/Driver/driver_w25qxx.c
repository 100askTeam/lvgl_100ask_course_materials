/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：driver_w25qxx.c
 * 摘要：
 *  
 * 修改历史     版本号       Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/

#include "main.h"
#include "driver_spi.h"
#include "driver_w25qxx.h"
#include "driver_timer.h"
// 函数重定义
#define W25_CS_ENABLE()         {W25_CS(0),us_timer_delay(10);}
#define W25_CS_DISABLE()        {W25_CS(1),us_timer_delay(10);}
#define W25_RW_Byte(data)       SPI_WriteReadByte(data)

/*
 *  函数名：uint32_t FLASH_ReadDeviceID(void)
 *  输入参数：
 *  输出参数：无
 *  返回值：读到外部FLASH的设备ID
 *  函数作用：读外部FLASH的设备ID
*/
uint32_t FLASH_ReadDeviceID(void)
{
    uint32_t deviceID = 0;
    //uint32_t temp0 = 0, temp1 = 0, temp2 = 0;
    
    W25_CS_ENABLE();
    
    W25_RW_Byte(W25X_DeviceID);
    deviceID = W25_RW_Byte(Dummy_Byte);
    deviceID = W25_RW_Byte(Dummy_Byte);
    deviceID = W25_RW_Byte(Dummy_Byte);
    deviceID = W25_RW_Byte(Dummy_Byte);
    
    W25_CS_DISABLE();
    
    return deviceID;
}

/*
 *  函数名：uint32_t Flash_ReadFlashID(void)
 *  输入参数：
 *  输出参数：无
 *  返回值：读到外部FLASH的芯片ID
 *  函数作用：读外部FLASH的芯片ID
*/
uint32_t Flash_ReadFlashID(void)
{
    uint32_t temp = 0, temp0 = 0, temp1 = 0, temp2 = 0;
    
    W25_CS_ENABLE();
    
    W25_RW_Byte(W25X_JedecDeviceID);
    temp0 = W25_RW_Byte(Dummy_Byte);
    temp1 = W25_RW_Byte(Dummy_Byte);
    temp2 = W25_RW_Byte(Dummy_Byte);
    
    W25_CS_DISABLE();
    
    temp = (temp0 << 16) | (temp1 << 8) | temp2;
    return temp;
}

/*
 *  函数名：static void Flash_WritenEN(void)
 *  输入参数：
 *  输出参数：无
 *  返回值：无
 *  函数作用：写使能
*/
static void Flash_WritenEN(void)
{
    W25_CS_ENABLE();
    W25_RW_Byte(W25X_WriteEnable);
    W25_CS_DISABLE();
}

/*
 *  函数名：static void FLASH_WaitForWriteEnd(void)
 *  输入参数：
 *  输出参数：无
 *  返回值：无
 *  函数作用：等待写完成
*/
static void FLASH_WaitForWriteEnd(void)
{
    uint8_t flash_status = 0;
    
    W25_CS_ENABLE();
    W25_RW_Byte(W25X_ReadStatusReg);

    do
    {
        flash_status = W25_RW_Byte(Dummy_Byte);	 
    }
    while ((flash_status & WIP_Flag) == SET); 
    W25_CS_DISABLE();
}

/*
 *  函数名：void Flash_PowerDown(void)  
 *  输入参数：
 *  输出参数：无
 *  返回值：无
 *  函数作用：断开FLASH
*/
void Flash_PowerDown(void)   
{    
    W25_CS_ENABLE();
    
    W25_RW_Byte(W25X_PowerDown);
    
    W25_CS_DISABLE();
}   

/*
 *  函数名：void Flash_WakeUp(void) 
 *  输入参数：
 *  输出参数：无
 *  返回值：无
 *  函数作用：唤醒FLASH
*/
void Flash_WakeUp(void)   
{
    W25_CS_ENABLE();
    
    W25_RW_Byte(W25X_ReleasePowerDown);
    
    W25_CS_DISABLE();
}   

/*
 *  函数名：void FLASH_SectorErase(uint32_t SectorAddr)
 *  输入参数：SectorAddr -> 要擦擦的地址
 *  输出参数：无
 *  返回值：无
 *  函数作用：扇区擦除
*/
void FLASH_SectorErase(uint32_t SectorAddr)
{
    Flash_WritenEN();
    FLASH_WaitForWriteEnd();

    W25_CS_ENABLE();
    W25_RW_Byte(W25X_SectorErase);
    W25_RW_Byte((SectorAddr & 0xFF0000) >> 16);
    W25_RW_Byte((SectorAddr & 0xFF00) >> 8);
    W25_RW_Byte(SectorAddr & 0xFF);
    W25_CS_DISABLE();
    
    FLASH_WaitForWriteEnd();
}

/*
 *  函数名：void FLASH_BankErase(void)
 *  输入参数：
 *  输出参数：无
 *  返回值：无
 *  函数作用：块擦除
*/
void FLASH_BankErase(void)
{
    Flash_WritenEN();
    
    W25_CS_ENABLE();
    W25_RW_Byte(W25X_ChipErase);
    W25_CS_DISABLE();
    
    FLASH_WaitForWriteEnd();
}

/*
 *  函数名：void FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
 *  输入参数：pBuffer -> 要写的数据指针;    WriteAddr -> 要写的FLASH初始地址;  NumByteToWrite -> 要写的字节个数
 *  输出参数：无
 *  返回值：无
 *  函数作用：页写
*/
void FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    Flash_WritenEN();
    
    W25_CS_ENABLE();
    W25_RW_Byte(W25X_PageProgram);    
    W25_RW_Byte((WriteAddr & 0xFF0000) >> 16);    
    W25_RW_Byte((WriteAddr & 0xFF00) >> 8);    
    W25_RW_Byte(WriteAddr & 0xFF);
    
    if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
    {
        NumByteToWrite = SPI_FLASH_PerWritePageSize;
    }

    while (NumByteToWrite--)
    {
        W25_RW_Byte(*pBuffer);
        pBuffer++;
    }
    
    W25_CS_DISABLE();
    
    FLASH_WaitForWriteEnd();
}

/*
 *  函数名：void FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite))
 *  输入参数：pBuffer -> 要写的数据指针;    WriteAddr -> 要写的FLASH初始地址;  NumByteToWrite -> 要写的字节个数
 *  输出参数：无
 *  返回值：无
 *  函数作用：写N个字节
*/
void FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

    Addr = WriteAddr % SPI_FLASH_PageSize;

    count = SPI_FLASH_PageSize - Addr;	

    NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;

    NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

    if (Addr == 0) 
    {
        if (NumOfPage == 0) 
        {
            FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
        }
        else 
        {
            while (NumOfPage--)
            {
                FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
                WriteAddr +=  SPI_FLASH_PageSize;
                pBuffer += SPI_FLASH_PageSize;
            }

            FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
        }
    }
    else 
    {
        if (NumOfPage == 0) 
        {
            if (NumOfSingle > count) 
            {
                temp = NumOfSingle - count;

                FLASH_PageWrite(pBuffer, WriteAddr, count);
                WriteAddr +=  count;
                pBuffer += count;

                FLASH_PageWrite(pBuffer, WriteAddr, temp);
            }
            else 
            {				
                FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
            }
        }
        else 
        {
            NumByteToWrite -= count;
            NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
            NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

            FLASH_PageWrite(pBuffer, WriteAddr, count);
            WriteAddr +=  count;
            pBuffer += count;

            while (NumOfPage--)
            {
                FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
                WriteAddr +=  SPI_FLASH_PageSize;
                pBuffer += SPI_FLASH_PageSize;
            }
            if (NumOfSingle != 0)
            {
                FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
            }
        }
    }
}

/*
 *  函数名：void FLASH_BufferRead(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite))
 *  输入参数：pBuffer -> 要读的数据指针;    WriteAddr -> 要读的FLASH初始地址;  NumByteToWrite -> 要读的字节个数
 *  输出参数：无
 *  返回值：无
 *  函数作用：读N个字节出来
*/
void FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
    W25_CS_ENABLE();
    
    W25_RW_Byte(W25X_ReadData);

    W25_RW_Byte((ReadAddr & 0xFF0000) >> 16);
    
    W25_RW_Byte((ReadAddr& 0xFF00) >> 8);
    
    W25_RW_Byte(ReadAddr & 0xFF);

    while (NumByteToRead--)
    {
        *pBuffer = W25_RW_Byte(Dummy_Byte);
        pBuffer++;
    }
    
    W25_CS_DISABLE();
}

void FLASH_StartReadSequence(uint32_t ReadAddr)
{
    W25_CS_ENABLE();
    
    W25_RW_Byte(W25X_ReadData);
    W25_RW_Byte((ReadAddr & 0xFF0000) >> 16);
    W25_RW_Byte((ReadAddr& 0xFF00) >> 8);
    W25_RW_Byte(ReadAddr & 0xFF);
    
    W25_CS_DISABLE();
}

void W25Q64_WriteData(uint32_t addr, uint8_t *pdata, uint16_t sz)
{
    FLASH_SectorErase(addr);
    FLASH_BufferWrite((uint8_t*)pdata, addr, sz);
}

