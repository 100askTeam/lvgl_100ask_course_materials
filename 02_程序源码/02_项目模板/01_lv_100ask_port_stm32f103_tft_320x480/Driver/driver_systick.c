/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：driver_systick.c
 * 摘要：
 *  
 * 修改历史     版本号        Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/

#include "main.h"
#include "driver_systick.h"

/*
 * 全局变量定义
*/

volatile uint16_t systick_t = 0;

/*
 *  函数名：void SysTickInit(uint16_t t)
 *  输入参数：t-设定时间：1->ms; 2->100us; 3->10us
 *  输出参数：无
 *  返回值：无
 *  函数作用：初始化系统滴答时钟的频率和中断优先级
*/
void SysTickInit(uint16_t t)
{
    uint32_t init_t = 0;
    if(t==1)
    {
        init_t = SystemCoreClock/1000;
    }
    else if(t==2)
    {
        init_t = SystemCoreClock/10000;
    }
    else if(t==3)
    {
        init_t = SystemCoreClock/100000;
    }
    
    /* 配置滴答时钟频率
     * SystemCoreClock/1000:   1ms中断一次
     * SystemCoreClock/10000:  10us中断一次
     * SystemCoreClock/100000: 1us中断一次
     * 中断时间计算如下：SystemCoreClock = 72MHz = 72,000,000
     *                   SystemCoreClock/1000 = 72,000
     * 即系统向下计数72,000次系统时钟频率即：(72,000/72MHz)s = (1/1000)s = 1ms后中断一次
    */
    if(HAL_SYSTICK_Config(init_t) != HAL_OK)
    {
        Error_Handler();
    }
    
    // 设置滴答定时器中断优先级：最高
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
    // 使能滴答定时器中断
    HAL_NVIC_EnableIRQ(SysTick_IRQn);
}

/*
 *  函数名：void SysTickDelay(uint16_t m)
 *  输入参数：m-延时时间
 *  输出参数：无
 *  返回值：无
 *  函数作用：滴答定时器实现的延时函数
*/
void SysTickDelay(uint16_t m)
{
    systick_t = m;
    while(systick_t != 0);
}
