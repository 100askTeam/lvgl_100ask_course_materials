/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：driver_flash.c
 * 摘要：
 *  
 * 修改历史     版本号       Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/
#include <string.h>
#include "main.h"
#include "driver_flash.h"

/*
 * 定义全局变量
*/
static FLASH_EraseInitTypeDef EraseInitStruct;


/*
 *  函数名：void EraseFlash(uint32_t addr, uint32_t length)
 *  输入参数：addr -> 擦除地址;    length -> 擦除的字节个数
 *  输出参数：无
 *  返回值：无
 *  函数作用：擦除内部flash
*/
void EraseFlash(uint32_t addr, uint32_t length)
{
    uint32_t nPageError = 0;
    
    
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;    // 页擦除
    EraseInitStruct.PageAddress = addr;         // 擦除的起始地址
    if(length < FLASH_PAGE_SIZE)
    {
        EraseInitStruct.NbPages     = 0x01;     // 当擦除的字节数小于一页时，我们只需要擦除一页就好
    }
    else
    {
        EraseInitStruct.NbPages     = (TEST_PAGE_END_ADDR - addr) / FLASH_PAGE_SIZE;
    }                                           // 当擦除的字节数大于一页时，我们需要计算擦除的页数
    

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &nPageError) != HAL_OK)
    {                                          // 擦除Flash
        Error_Handler();
    }
}

/*
 *  函数名：void WriteFlash(uint32_t startAddr, uint32_t *pdata, uint32_t length)
 *  输入参数：startAddr-> 写首地址;    pdata -> 写的数据指针;      length -> 写的字节个数
 *  输出参数：无
 *  返回值：无
 *  函数作用：写内部flash
*/
void WriteFlash(uint32_t startAddr, uint32_t *pdata, uint32_t length)
{
    uint32_t i = 0;
    uint32_t address = startAddr;
    
    HAL_FLASH_Unlock();                         // 解锁
    
    EraseFlash(startAddr, length);              // 擦除
    
    for(i=0; i<(length>>2);i++)                      // 开始字写
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, pdata[i]) == HAL_OK)
        {
          address = address + 4;
        }
    }
    HAL_FLASH_Lock();                           // 上锁
}

/*
 *  函数名：void ReadFlash(uint32_t startAddr, uint32_t *pdata, uint32_t length)
 *  输入参数：startAddr-> 读首地址;    pdata -> 读出的数据指针;      length -> 读的字节个数
 *  输出参数：无
 *  返回值：无
 *  函数作用：读内部flash
*/
void ReadFlash(uint32_t startAddr, uint32_t *pdata, uint32_t length)
{
    memcpy((uint32_t*)pdata, (uint32_t*)startAddr, sizeof(uint32_t)*length);
}


uint32_t FLASH_If_Erase(uint32_t start)
{
  uint32_t NbrOfPages = 0;
  uint32_t PageError = 0;
  FLASH_EraseInitTypeDef pEraseInit;
  HAL_StatusTypeDef status = HAL_OK;

  /* Unlock the Flash to enable the flash control register access *************/ 
  HAL_FLASH_Unlock();

  /* Get the sector where start the user flash area */
  if (start < USER_FLASH_BANK1_END_ADDRESS)
  {
    NbrOfPages = ((USER_FLASH_BANK1_END_ADDRESS + 1) - start)/FLASH_PAGE_SIZE;
    pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    pEraseInit.PageAddress = start;
    pEraseInit.Banks = FLASH_BANK_1;
    pEraseInit.NbPages = NbrOfPages;
    status = HAL_FLASHEx_Erase(&pEraseInit, &PageError);

  }

  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();

  if (status != HAL_OK)
  {
    /* Error occurred while page erase */
    return FLASHIF_ERASEKO;
  }

  return FLASHIF_OK;
}

uint32_t FLASH_If_Write(uint32_t destination, uint32_t *p_source, uint32_t length)
{
  uint32_t i = 0;

  /* Unlock the Flash to enable the flash control register access *************/
  HAL_FLASH_Unlock();

  for (i = 0; (i < length) && (destination <= (USER_FLASH_BANK1_END_ADDRESS-4)); i++)
  {
    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
       be done by word */ 
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, destination, *(uint32_t*)(p_source+i)) == HAL_OK)      
    {
     /* Check the written value */
      if (*(uint32_t*)destination != *(uint32_t*)(p_source+i))
      {
        /* Flash content doesn't match SRAM content */
        return(FLASHIF_WRITINGCTRL_ERROR);
      }
      /* Increment FLASH destination address */
      destination += 4;
    }
    else
    {
      /* Error occurred while writing data in Flash memory */
      return (FLASHIF_WRITING_ERROR);
    }
  }

  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();

  return (FLASHIF_OK);
}

