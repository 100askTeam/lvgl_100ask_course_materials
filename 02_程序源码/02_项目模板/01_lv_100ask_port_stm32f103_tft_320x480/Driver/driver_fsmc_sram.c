/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：driver_fsmc.c
 * 摘要：
 *  
 * 修改历史     版本号        Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/
#include "main.h"
#include "driver_sram.h" 
#include "driver_fsmc_sram.h" 

static SRAM_HandleTypeDef           hsram;
static FSMC_NORSRAM_TimingTypeDef   hfsmc_timing;
static uint8_t fsmc_rw_flag = 0;

void HAL_FSMC_SRAM_MspInit(void);

/*
 *  函数名：void FSMC_SRAM_Init(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：初始化FSMC
*/
void FSMC_SRAM_Init(void)
{
    hsram.Instance = FSMC_NORSRAM_DEVICE;
    hsram.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
    
    hfsmc_timing.AddressSetupTime      = 0x0; // 1
    hfsmc_timing.AddressHoldTime       = 0x0; // 2
    hfsmc_timing.DataSetupTime         = 0x8; // 8
    hfsmc_timing.BusTurnAroundDuration = 0x0; // 1
    hfsmc_timing.CLKDivision           = 0x0; // 2
    hfsmc_timing.DataLatency           = 0x0; // 2
    hfsmc_timing.AccessMode            = FSMC_ACCESS_MODE_A;
    
    hsram.Init.NSBank               = FSMC_NORSRAM_BANK3;     				// 使用NE3
    hsram.Init.DataAddressMux       = FSMC_DATA_ADDRESS_MUX_DISABLE; 	    // 地址/数据线不复用
    hsram.Init.MemoryType           = FSMC_MEMORY_TYPE_SRAM;   				// SRAM
    hsram.Init.MemoryDataWidth      = FSMC_NORSRAM_MEM_BUS_WIDTH_16; 	    // 16位数据宽度
    hsram.Init.BurstAccessMode      = FSMC_BURST_ACCESS_MODE_DISABLE;       // 是否使能突发访问,仅对同步突发存储器有效,此处未用到
    hsram.Init.WaitSignalPolarity   = FSMC_WAIT_SIGNAL_POLARITY_LOW;        // 等待信号的极性,仅在突发模式访问下有用
    hsram.Init.WaitSignalActive     = FSMC_WAIT_TIMING_BEFORE_WS;   	    // 存储器是在等待周期之前的一个时钟周期还是等待周期期间使能NWAIT
    hsram.Init.WriteOperation       = FSMC_WRITE_OPERATION_ENABLE;    	    // 存储器写使能
    hsram.Init.WaitSignal           = FSMC_WAIT_SIGNAL_DISABLE;           	// 等待使能位,此处未用到
    hsram.Init.ExtendedMode         = FSMC_EXTENDED_MODE_DISABLE;        	// 读写使用相同的时序
    hsram.Init.AsynchronousWait     = FSMC_ASYNCHRONOUS_WAIT_DISABLE;	    // 是否使能同步传输模式下的等待信号,此处未用到
    hsram.Init.WriteBurst           = FSMC_WRITE_BURST_DISABLE;           	// 禁止突发写
    
    HAL_FSMC_SRAM_MspInit();
    
    if(HAL_SRAM_Init(&hsram, &hfsmc_timing, &hfsmc_timing) != HAL_OK)
    {
        Error_Handler();
    }
}

void HAL_FSMC_SRAM_MspInit(void)
{
    GPIO_InitTypeDef    GPIO_InitStruct = {0};
    
    __HAL_RCC_FSMC_CLK_ENABLE();
    GPIOD_CLK_EN();
    GPIOE_CLK_EN();
    GPIOF_CLK_EN();
    GPIOG_CLK_EN();
    
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    
    // PDx
    GPIO_InitStruct.Pin = SRAM_GPIOD_PIN;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    // PEx
    GPIO_InitStruct.Pin = SRAM_GPIOE_PIN;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
    
    // PFx
    GPIO_InitStruct.Pin = SRAM_GPIOF_PIN;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
    
    // PGx
    GPIO_InitStruct.Pin = SRAM_GPIOG_PIN;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
}


/*
 *  函数名：void FSMC_RW_SetFlag(uint8_t flag)
 *  输入参数：flag->启动读写标志。0不读写，1读写
 *  输出参数：无
 *  返回值：无
 *  函数作用：设置FSMC的读写标志
*/
void FSMC_RW_SetFlag(uint8_t flag)
{
    fsmc_rw_flag = flag;
}

/*
 *  函数名：uint8_t FSMC_RW_GetFlag(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：返回FSMC的读写标志
 *  函数作用：获取FSMC的读写标志
*/
uint8_t FSMC_RW_GetFlag(void)
{
    return fsmc_rw_flag;
}

