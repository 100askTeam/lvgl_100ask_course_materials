/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：driver_Buzzer.c
 * 摘要：
 *  
 * 修改历史     版本号       Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/

#include "driver_Buzzer.h"

/*
 *  函数名：void BuzzerInit(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：初始化蜂鸣器的引脚
*/
void BuzzerInit(void)
{
    // 定义GPIO的结构体变量
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    // 使能LED的GPIO对应的时钟
    Buzzer_GPIO_CLK_EN();
    
    GPIO_InitStruct.Pin = Buzzer_GPIO_PIN;         // 选择蜂鸣器的引脚
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  // 设置为推挽输出模式
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;          // 默认下拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;// 引脚反转速度设置为快
    
    // 初始化引脚配置
    HAL_GPIO_Init(Buzzer_GPIO_PORT, &GPIO_InitStruct);
    
    // 关闭蜂鸣器
    Buzzer(0);
}

/*
 *  函数名：void ControlBuzzerRang(uint16_t period)
 *  输入参数：period-蜂鸣器发出响声的周期，f=1/(2*period)
 *  输出参数：无
 *  返回值：无
 *  函数作用：初始化系统滴答时钟的频率和中断优先级
*/
void ControlBuzzerRang(uint16_t period)
{
    Buzzer(1);
    HAL_Delay(period);
    Buzzer(0);
    HAL_Delay(period);
}
