/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：driver_led.c
 * 摘要：
 *  
 * 修改历史     版本号        Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/

#include "driver_led.h"

/*
 *  函数名：void LedGpioInit(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：初始化LED的引脚，配置为上拉推挽输出
*/
void LedGpioInit(void)
{
    // 定义GPIO的结构体变量
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    // 使能LED的GPIO对应的时钟
    R_LED_GPIO_CLK_EN();
    G_LED_GPIO_CLK_EN();
    B_LED_GPIO_CLK_EN();

    GPIO_InitStruct.Pin = R_LED_GPIO_PIN | G_LED_GPIO_PIN | B_LED_GPIO_PIN;         // 选择LED的引脚
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // 设置为推挽输出模式
    GPIO_InitStruct.Pull = GPIO_PULLUP;         // 默认上拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;// 引脚反转速度设置为快

    // 初始化引脚配置
    HAL_GPIO_Init(R_LED_GPIO_PORT, &GPIO_InitStruct);

    // 默认LED灭:1-灭，0-亮
    RLED(1);
    GLED(1);
    BLED(1);
}


