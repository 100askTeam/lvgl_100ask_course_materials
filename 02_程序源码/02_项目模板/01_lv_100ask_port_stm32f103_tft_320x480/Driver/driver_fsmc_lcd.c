#include "driver_fsmc_lcd.h"

static SRAM_HandleTypeDef           hsram;
static FSMC_NORSRAM_TimingTypeDef   hfsmc_rw;

void HAL_FSMC_LCD_MspInit(void);

void FSMC_LCD_Init(void)
{
    hsram.Instance = FSMC_NORSRAM_DEVICE;
    hsram.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;

    hsram.Init.NSBank               = FSMC_NORSRAM_BANK4;     				// 使用NE4
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
    
    hfsmc_rw.AddressSetupTime      = 0x00;
    hfsmc_rw.AddressHoldTime       = 0x00;
    hfsmc_rw.DataSetupTime         = 0x08;
    hfsmc_rw.BusTurnAroundDuration = 0x00;
    hfsmc_rw.CLKDivision           = 0x00;
    hfsmc_rw.DataLatency           = 0x00;
    hfsmc_rw.AccessMode            = FSMC_ACCESS_MODE_A;
    
    HAL_FSMC_LCD_MspInit();
    
    if(HAL_SRAM_Init(&hsram, &hfsmc_rw, &hfsmc_rw) != HAL_OK)
    {
        Error_Handler();
    }    
}

void HAL_FSMC_LCD_MspInit(void)
{
    GPIO_InitTypeDef    GPIO_InitStruct = {0};
    
    __HAL_RCC_FSMC_CLK_ENABLE();
    GPIOC_CLK_EN();
    GPIOD_CLK_EN();
    GPIOE_CLK_EN();
    GPIOF_CLK_EN();
    GPIOG_CLK_EN();
    
    
    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    
    GPIO_InitStruct.Pin = LCD_RST_PIN;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
    
    // PCx
    GPIO_InitStruct.Pin = LCD_BL_PIN;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);   
    
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
	
	// PGx
    GPIO_InitStruct.Pin = LCD_CS_PIN;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
	
    // PDx
    GPIO_InitStruct.Pin = LCD_GPIOD_PIN;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    // PEx
    GPIO_InitStruct.Pin = LCD_GPIOE_PIN;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}
