/**
 ******************************************************************************
 * @file    main.c
 * @author  百问科技
 * @version V1.2
 * @date    2020-12-12
 * @brief	main函数入口
 ******************************************************************************
 * Change Logs:
 * Date           Author          Notes
 * 2020-12-12     zhouyuebiao     First version
 * 2021-01-25     zhouyuebiao     V1.2
 ******************************************************************************
 * @attention
 *
 * Copyright (C) 2008-2021 深圳百问网科技有限公司<https://www.100ask.net/>
 * All rights reserved
 *
 ******************************************************************************
 */

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "main.h"
#include "stm32f1xx_clk.h"
#include "driver_timer.h"
#include "driver_flash.h"

#include "driver_fsmc_lcd.h"
#include "driver_fsmc_sram.h" 

#include "driver_usart.h"
#include "driver_usart2.h"
#include "driver_usart3.h"

#include "driver_spi.h"

#include "ring_buffer.h"

/*
    LCD include
*/
#include "lcd_function.h"
#include "XPT2046.h"

/*
    lvgl include
*/
#include "../lvgl.h"
#include "../lv_port_disp.h"
#include "../lv_port_indev.h"

/*
    100ask lvgl teach demo include
*/
#include "../lv_100ask_teach_demos_conf.h"

/**********************************************************************
 * 函数名称： main
 * 功能描述： 主函数
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 * 修改日期              版本号      修改人	      修改内容
 * ---------------------------------------------------------------------
 * 2020/11/11	         V1.0	     YobeZhou     创建
 ***********************************************************************/
int main(void)
{    
    // 初始化HAL库函数必须要调用此函数
    HAL_Init();

    /*
     * 系统时钟即AHB/APB时钟配置
     * 当使用内部高速时钟HSE（8MHz）配置系统时钟时，使用PLL前可以选择不分频或者二分频，我们要配置到最大72MHz的系统频率此处当然是不分频
     * 然后经过一个放大器，最大放大倍数为16，我们要想得到72MHz，此处选择9倍放大系数，即8*9=72MHz即可达到目标
    */
    SystemClock_Config();
    TimerInit();
    SPI_Init();
    
    FSMC_SRAM_Init();
    FSMC_LCD_Init();
    
    UsartInit(115200);
    RingBuffer_Init();
    
    LCD_Init();
    LCD_GRAM_Scan(0);
    XPT2046_Init();

    /*
        lvgl init
    */
    LCD_Clear(WHITE);
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();

	printf(">>>100ASK LVGL DEMO!\n\r");
	
	lv_100ask_demo_course_2_1_1();
	
    while(1)
    {
        lv_task_handler();
		HAL_Delay(5);
    }
}



/**********************************************************************
 * 函数名称： SoftReset
 * 功能描述： 软复位
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 * 修改日期              版本号      修改人	      修改内容
 * ---------------------------------------------------------------------
 * 2020/11/11	         V1.0	     YobeZhou     创建
 ***********************************************************************/
void SoftReset(void)
{
    __set_FAULTMASK(1);
    NVIC_SystemReset();
}



/**********************************************************************
 * 函数名称： Error_Handler
 * 功能描述： 程序错误处理函数，此处暂时设为死循环，不做任何动作
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 * 修改日期              版本号      修改人	      修改内容
 * ---------------------------------------------------------------------
 * 2020/11/11	         V1.0	     YobeZhou     创建
 ***********************************************************************/
void Error_Handler(void)
{
    while(1)
    {
    }
}

