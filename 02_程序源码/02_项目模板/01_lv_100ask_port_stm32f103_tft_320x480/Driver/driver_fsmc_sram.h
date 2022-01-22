/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：driver_fsmc.h
 * 摘要：
 *  
 * 修改历史     版本号        Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/
#ifndef __DRIVER_FSMC_SRAM_H
#define __DRIVER_FSMC_SRAM_H

#include "stm32f1xx_hal.h"

/****************** SRAM Address *****************/
#define BANK1_SRAM1_ADDR        ((uint32_t)(0x60000000))    // datasheet page 511
#define BANK1_SRAM2_ADDR        ((uint32_t)(0x64000000))
#define BANK1_SRAM3_ADDR        ((uint32_t)(0x68000000))
#define BANK1_SRAM4_ADDR        ((uint32_t)(0x6C000000))

#define IS62WV51216_SIZE            (0x100000UL)
#define IS62WV51216_BASE_ADDR       (BANK1_SRAM3_ADDR)
/********************GPIOD************************/
/* PD0 PD1 PD4 PD5 PD8 PD9 PD10 PD11 PD12 PD13 PD14 PD15 */ 
/*********************GPIOG***********************/
#define SRAM_GPIOD_PIN           ( GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_9 \
                            | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15)
                            
/********************GPIOE************************/
/* PE0 PE1 PE7 PE8 PE9 PE10 PE11 PE12 PE13 PE14 PE15 */
/*********************GPIOG***********************/
#define SRAM_GPIOE_PIN           ( GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 \
                            | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15)
                            
/********************GPIOF************************/
/* PF0 PF1 PF2 PF3 PF4 PF5 PF12 PF13 PF14 PF15   */
/*********************GPIOG***********************/
#define SRAM_GPIOF_PIN           ( GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 \
                            | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15)
                            
/********************GPIOG************************/
/*      PG0 PG1 PG2 PG3 PG4 PG5 PG10             */
/*********************GPIOG***********************/
#define SRAM_GPIOG_PIN           ( GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_10)
                            
/********************FSMC_A***********************/
#define A0_PIN                  GPIO_PIN_0  // PF0 
#define A1_PIN                  GPIO_PIN_1  // PF1 
#define A2_PIN                  GPIO_PIN_2  // PF2 
#define A3_PIN                  GPIO_PIN_3  // PF3 
#define A4_PIN                  GPIO_PIN_4  // PF4 
#define A5_PIN                  GPIO_PIN_5  // PF5 
#define A6_PIN                  GPIO_PIN_12 // PF12 
#define A7_PIN                  GPIO_PIN_13 // PF13 
#define A8_PIN                  GPIO_PIN_14 // PF14 
#define A9_PIN                  GPIO_PIN_15 // PF15 
#define A10_PIN                 GPIO_PIN_0  // PG0 
#define A11_PIN                 GPIO_PIN_1  // PG1 
#define A12_PIN                 GPIO_PIN_2  // PG2 
#define A13_PIN                 GPIO_PIN_3  // PG3 
#define A14_PIN                 GPIO_PIN_4  // PG4 
#define A15_PIN                 GPIO_PIN_5  // PG5 
#define A16_PIN                 GPIO_PIN_11 // PD11
#define A17_PIN                 GPIO_PIN_12 // PD12
#define A18_PIN                 GPIO_PIN_13 // PD13

/********************FSMC_D***********************/
#define DQ0_PIN                 GPIO_PIN_14 // PD14
#define DQ1_PIN                 GPIO_PIN_15 // PD15
#define DQ2_PIN                 GPIO_PIN_0  // PD0
#define DQ3_PIN                 GPIO_PIN_1  // PD1
#define DQ4_PIN                 GPIO_PIN_7  // PE7  
#define DQ5_PIN                 GPIO_PIN_8  // PE8 
#define DQ6_PIN                 GPIO_PIN_9  // PE9 
#define DQ7_PIN                 GPIO_PIN_10 // PE10 
#define DQ8_PIN                 GPIO_PIN_11 // PE11 
#define DQ9_PIN                 GPIO_PIN_12 // PE12 
#define DQ10_PIN                GPIO_PIN_13 // PE13 
#define DQ11_PIN                GPIO_PIN_14 // PE14 
#define DQ12_PIN                GPIO_PIN_15 // PE15 
#define DQ13_PIN                GPIO_PIN_8  // PD8
#define DQ14_PIN                GPIO_PIN_9  // PD9
#define DQ15_PIN                GPIO_PIN_10 // PD10

/********************其他信号线***********************/
#define OE_PIN                  GPIO_PIN_4  // PD4
#define WE_PIN                  GPIO_PIN_5  // PD5
#define LB_PIN                  GPIO_PIN_0  // PE0  
#define UB_PIN                  GPIO_PIN_1  // PE1 
#define CE_PIN                  GPIO_PIN_10 // PG10 


#define GPIOD_CLK_EN()          __HAL_RCC_GPIOD_CLK_ENABLE()
#define GPIOE_CLK_EN()          __HAL_RCC_GPIOE_CLK_ENABLE()
#define GPIOF_CLK_EN()          __HAL_RCC_GPIOF_CLK_ENABLE()
#define GPIOG_CLK_EN()          __HAL_RCC_GPIOG_CLK_ENABLE()

/*
 *  函数名：void FSMC_SRAM_Init(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：初始化FSMC
*/
extern void FSMC_SRAM_Init(void);


/*
 *  函数名：void FSMC_RW_SetFlag(uint8_t flag)
 *  输入参数：flag->启动读写标志。0不读写，1读写
 *  输出参数：无
 *  返回值：无
 *  函数作用：设置FSMC的读写标志
*/
extern void FSMC_RW_SetFlag(uint8_t flag);

/*
 *  函数名：uint8_t FSMC_RW_GetFlag(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：返回FSMC的读写标志
 *  函数作用：获取FSMC的读写标志
*/
extern uint8_t FSMC_RW_GetFlag(void);

#endif





