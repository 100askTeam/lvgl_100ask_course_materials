/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：ADC_SoftwareTriger.c
 * 摘要：
 *  
 * 修改历史     版本号       Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/
#ifndef __ADC_SOFTWARETRIGER_H
#define __ADC_SOFTWARETRIGER_H

#include "stm32f1xx_hal.h"

/****************************************************
 *  宏定义选择ADC的输入引脚及其对应的通道
 *  具体的引脚和通道需根据原理图以及芯片手册进行选择
****************************************************/
#define ADC1_IN_PORT        GPIOC
#define ADC1_IN_PIN         GPIO_PIN_0
#define ADC1_IN_CHANNEL     ADC_CHANNEL_10
#define ADC1_PORT_CLK_EN()  __HAL_RCC_GPIOC_CLK_ENABLE()

/*
 *  函数名：void AdcInit(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：初始化ADC1的参数：触发方式、数据格式、转换模式，通道配置及其采样时间
 * 
*/
extern void AdcInit(void);

/*
 *  函数名：uint32_t GetAdcValue(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：返回一个uint32_t型的值，即采集到的ADC值，此处是数字电压，需根据参考电压得到具体的模拟电压值
 *  函数作用：获取ADC的采样值
*/
extern uint32_t GetAdcSampleValue(void);

/*
 *  函数名：float GetAdcAnlogValue(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：返回一个float型值，即采样到的模拟电压值
 *  函数作用：获取ADC的采样到的模拟电压之
 *  
 *  备注：此处参考电压为2.5V
*/
extern float GetAdcAnlogValue(void);

#endif

