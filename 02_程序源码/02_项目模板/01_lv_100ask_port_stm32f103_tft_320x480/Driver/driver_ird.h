/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：driver_ird.h
 * 摘要：
 *  
 * 修改历史     版本号        Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/

#ifndef __DRIVER_IRD_H
#define __DRIVER_IRD_H

#include "stm32f1xx_hal.h"

typedef struct
{
    uint8_t pilot_code;
    uint8_t serial_code;
    uint8_t data_code;
    uint16_t period;
    uint32_t value;
}IRD_DATA;

typedef enum
{
    OPEN = 0x45,
    MENU = 0x47,
    TEST = 0x44,
    INCREASE = 0x40,
    BACK = 0x43,
    GOBACK = 0x07,
    PLAY = 0x15,
    FASTFORWARD = 0x09,
    ZERO = 0x16,
    DECREASE = 0x19,
    CANCEL = 0x0D,
    ONE = 0x0C,
    TWO = 0x18,
    THREE = 0x5E,
    FOUR = 0x08,
    FIVE = 0x1C,
    SIX = 0x5A,
    SEVEN = 0x42,
    EIGHT = 0x52,
    NINE = 0x4A
}KEY_CODE;

#define IRD_PORT                GPIOC
#define IRD_PIN                 GPIO_PIN_7
#define IRD_GPIO_CLK_EN()       __HAL_RCC_GPIOC_CLK_ENABLE()
#define IRD                     HAL_GPIO_ReadPin(IRD_PORT, IRD_PIN)
extern void IRD_GpioInit(void);
extern void IRD_Decode(void);
extern void IRD_EXTI_Callback(void);
#endif /* __DRIVER_IRD_H */

