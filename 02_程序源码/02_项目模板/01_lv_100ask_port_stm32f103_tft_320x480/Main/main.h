/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：main.h
 * 摘要：
 *  
 * 修改历史     版本号       Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/
#ifndef __MAIN_H
#define __MAIN_H

#include <stdio.h>
#include <string.h>
#include "stm32f1xx_hal.h"
#include "driver_w25qxx.h"

typedef struct
{
    uint8_t DebugUart     ; // 0
    uint8_t RS232         ; // 1
    uint8_t RS485_CAN     ; // 2
    uint8_t SRAM_RW       ; // 3
    uint8_t EEPROM_RW     ; // 4
    uint8_t AP3426        ; // 5
    uint8_t W25Q64_RW     ; // 6
    uint8_t ICM_20608     ; // 7
    uint8_t NET_CARD      ; // 8
    uint8_t USB_CDC       ; // 9
    uint8_t LED_T         ; // 10
    uint8_t BEEP_T        ; // 11
    uint8_t KEY_T         ; // 12
    uint8_t IRD_T         ; // 13
    uint8_t VOICE_CARD    ; // 14
    uint8_t ADC_T         ; // 15
    uint8_t Factory_UI    ; // 16
    
    char log_info[128];
}FACTORY_TEST;

typedef enum
{
    Error    = 1,
    Succeed  = 2,
    Not_Test = 4,
    Tested   = 8
}TEST_FLAG;

typedef enum
{
    M_DebugUart = 0 , // 0
    M_RS232         , // 1
    M_RS485_CAN     , // 2
    M_SRAM_RW       , // 3
    M_EEPROM_RW     , // 4
    M_AP3426        , // 5
    M_W25Q64_RW     , // 6
    M_ICM_20608     , // 7
    M_NET_CARD      , // 8
    M_USB_CDC       , // 9
    M_LED_T         , // 10
    M_BEEP_T        , // 11
    M_KEY_T         , // 12
    M_IRD_T         , // 13
    M_VOICE_CARD    , // 14
    M_ADC_T         , // 15
    M_Factory_UI    , // 16
    M_CONFIG        , // 17
}TEST_MODE;


typedef enum
{
    ADDR_DebugUart  = (0x00),                    // 0x00
    ADDR_RS232      = (ADDR_DebugUart  + 4096),  // 0x01
    ADDR_RS485_CAN  = (ADDR_RS232      + 4096),  // 0x02
    ADDR_SRAM_RW    = (ADDR_RS485_CAN  + 4096),  // 0x03
    ADDR_EEPROM_RW  = (ADDR_SRAM_RW    + 4096),  // 0x04
    ADDR_AP3426     = (ADDR_EEPROM_RW  + 4096),  // 0x05
    ADDR_W25Q64_RW  = (ADDR_AP3426     + 4096),  // 0x06
    ADDR_ICM_20608  = (ADDR_W25Q64_RW  + 4096),  // 0x07
    ADDR_NET_CARD   = (ADDR_ICM_20608  + 4096),  // 0x08
    ADDR_USB_CDC    = (ADDR_NET_CARD   + 4096),  // 0x09
    ADDR_LED_T      = (ADDR_USB_CDC    + 4096),  // 0x0A
    ADDR_BEEP_T     = (ADDR_LED_T      + 4096),  // 0x0B
    ADDR_KEY_T      = (ADDR_BEEP_T     + 4096),  // 0x0C
    ADDR_IRD_T      = (ADDR_KEY_T      + 4096),  // 0x0D
    ADDR_VOICE_CARD = (ADDR_IRD_T      + 4096),  // 0x0E
    ADDR_ADC_T      = (ADDR_VOICE_CARD + 4096),  // 0x0F
    ADDR_FACT_UI    = (ADDR_ADC_T      + 4096),  // 0x10
    ADDR_CONFIG     = (ADDR_FACT_UI    + 4096),  // 0x11
    ILI9341_PARA_FLASH_ADDR = (ADDR_CONFIG    + 4096),  // 0x12
    ILI9488_PARA_FLASH_ADDR = (ILI9341_PARA_FLASH_ADDR    + 4096),  // 0x13
}TEST_ADDR;                               

#define BTN_HOR_OFFSET        (lcddev.hor_res/4)
#define BTN_VER_OFFSET        (lcddev.ver_res/8)


#define BUF_SZIE            (8)
extern uint8_t w_buf[BUF_SZIE], r_buf[BUF_SZIE];
extern FACTORY_TEST    factory_test;
extern __IO uint32_t sys_timeout;

extern void Error_Handler(void);
extern void SoftReset(void);
extern uint8_t BuffCompare(uint8_t *src, uint8_t *dst, uint16_t sz);
extern void TestEventCallBack(uint8_t index);

#endif

