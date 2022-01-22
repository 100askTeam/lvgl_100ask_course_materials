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
#include "main.h"
#include "ADC_SoftwareTriger.h"

ADC_HandleTypeDef hadc1;

/*
 *  函数名：void AdcInit(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：初始化ADC1的参数：触发方式、数据格式、转换模式，通道配置及其采样时间
 * 
*/
void AdcInit(void)
{
    ADC_ChannelConfTypeDef sConfig;	
    
    hadc1.Instance                      = ADC1;
    hadc1.Init.ExternalTrigConv         = ADC_SOFTWARE_START;   // 软件触发模式
    hadc1.Init.DataAlign                = ADC_DATAALIGN_RIGHT;  // 选择ADC采集到的数据对齐格式
    hadc1.Init.ScanConvMode             = ADC_SCAN_DISABLE;     // 失能ADC的扫描模式
    hadc1.Init.ContinuousConvMode       = DISABLE;               // 使能ADC的连续转换模式
    hadc1.Init.NbrOfConversion          = 1;                    // 连续转换序列，此处只有一个通道，所以写1
    hadc1.Init.DiscontinuousConvMode    = DISABLE;              // 失能ADC非连续转换模式
    hadc1.Init.NbrOfDiscConversion      = 1;                    // 非连续转换序列，此处只有一个通道，所以写1；具体应用可根据具体项目灵活安排
    
    // 使用库函数对ADC参数初始化
    if(HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        Error_Handler(); // 初始化失败
    }
    
    sConfig.Channel = ADC1_IN_CHANNEL;
    sConfig.Rank    = ADC_REGULAR_RANK_1;
    /* 
     *  采样总时间计算：(SamplingTime+12.5)/fadc 
     *  fadc在源文件system_clk.c中进行了配置，是系统时钟的6分频，fsystem=72MH-->fadc=12MHz
     *  f_sample = fadc/((SamplingTime+12.5)) = 600kHz
     *  在设计采样率的时候，如果采样的是一段波形，必须满足奈奎斯特采样定理；如果采样的仅是一个电压值，根据时间要求设计即可
    */
    sConfig.SamplingTime    = ADC_SAMPLETIME_7CYCLES_5;// 选择采样时间
        
    // 使用库函数对此通道进行配置
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler(); // 通道配置失败
    }
}

/*
 *  函数名：HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
 *  输入参数：无
 *  输出参数：hadc-ADC句柄
 *  返回值：无
 *  函数作用：使能ADC句柄对应的ADC的时钟及其引脚配置
 *  函数说明：此函数本身是一个弱函数，在使用库函数HAL_ADC_Init()的时候会被调用，用户需在自己的工程中重构此函数
*/
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    // 定义GPIO的结构体变量
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
    GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;        // 72MHz/6 = 12MHz
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        Error_Handler();
    }
    
    if(hadc->Instance == ADC1)          // 根据句柄判断进来的是哪一路ADC
    {
        __HAL_RCC_ADC1_CLK_ENABLE();    // 使能ADC1的时钟
        ADC1_PORT_CLK_EN();             // 使能ADC1对应的GPIO的时钟
        
        GPIO_InitStruct.Pin = ADC1_IN_PIN;              // 选择ADC1的输入引脚
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;        //将引脚配置为模拟输入复用模式
        HAL_GPIO_Init(ADC1_IN_PORT, &GPIO_InitStruct);  // 初始化选择的引脚
    }
}

/*
 *  函数名：HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
 *  输入参数：无
 *  输出参数：hadc-ADC句柄
 *  返回值：无
 *  函数作用：失能ADC句柄对应的ADC的时钟及其引脚配置
 *  函数说明：此函数本身是一个弱函数，在使用库函数HAL_ADC_Deinit()的时候会被调用，用户需在自己的工程中重构此函数
*/
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{
    if(hadc->Instance == ADC1)                      // 根据句柄判断进来的是哪一路ADC
    {
        __HAL_RCC_ADC1_CLK_DISABLE();               // 失能ADC1的时钟
        HAL_GPIO_DeInit(ADC1_IN_PORT, ADC1_IN_PIN); // 将ADC1选择的GPIO引脚配置为上电默认值
    }
}

/*
 *  函数名：uint32_t GetAdcValue(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：返回一个uint32_t型的值，即采集到的ADC值，此处是数字电压，需根据参考电压得到具体的模拟电压值
 *  函数作用：获取ADC的采样值
*/
uint32_t GetAdcSampleValue(void)
{
    uint32_t nData = 0;
    
    // 调用库函数使用软件触发模式获取ADC的采样值
    HAL_ADC_Start(&hadc1);
    nData = HAL_ADC_GetValue(&hadc1);   
    HAL_ADC_PollForConversion(&hadc1, 10);

    // 返回采样值
    return nData;
}

/*
 *  函数名：float GetAdcAnlogValue(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：返回一个float型值，即采样到的模拟电压值
 *  函数作用：获取ADC的采样到的模拟电压之
 *  
 *  备注：此处参考电压为2.5V
*/
float GetAdcAnlogValue(void)
{
    uint32_t nData = 0;
    float nValue = 0.0;
    
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 10);
    nData = HAL_ADC_GetValue(&hadc1); 
    // F103的采样位数是12-bit；因而计算方式为：(SampleValue>>SampleBits)*Vref
    nValue = nData*3.3/4096;

    // 返回计算得到的模拟电压值
    return nValue;
}

