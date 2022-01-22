/*  Copyright (s) 2019 ���ڰ������Ƽ����޹�˾
 *  All rights reserved
 * 
 * �ļ����ƣ�driver_systick.c
 * ժҪ��
 *  
 * �޸���ʷ     �汾��        Author       �޸�����
 *--------------------------------------------------
 * 2020.6.6      v01        ���ʿƼ�      �����ļ�
 *--------------------------------------------------
*/

#include "main.h"
#include "driver_systick.h"

/*
 * ȫ�ֱ�������
*/

volatile uint16_t systick_t = 0;

/*
 *  ��������void SysTickInit(uint16_t t)
 *  ���������t-�趨ʱ�䣺1->ms; 2->100us; 3->10us
 *  �����������
 *  ����ֵ����
 *  �������ã���ʼ��ϵͳ�δ�ʱ�ӵ�Ƶ�ʺ��ж����ȼ�
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
    
    /* ���õδ�ʱ��Ƶ��
     * SystemCoreClock/1000:   1ms�ж�һ��
     * SystemCoreClock/10000:  10us�ж�һ��
     * SystemCoreClock/100000: 1us�ж�һ��
     * �ж�ʱ��������£�SystemCoreClock = 72MHz = 72,000,000
     *                   SystemCoreClock/1000 = 72,000
     * ��ϵͳ���¼���72,000��ϵͳʱ��Ƶ�ʼ���(72,000/72MHz)s = (1/1000)s = 1ms���ж�һ��
    */
    if(HAL_SYSTICK_Config(init_t) != HAL_OK)
    {
        Error_Handler();
    }
    
    // ���õδ�ʱ���ж����ȼ������
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
    // ʹ�ܵδ�ʱ���ж�
    HAL_NVIC_EnableIRQ(SysTick_IRQn);
}

/*
 *  ��������void SysTickDelay(uint16_t m)
 *  ���������m-��ʱʱ��
 *  �����������
 *  ����ֵ����
 *  �������ã��δ�ʱ��ʵ�ֵ���ʱ����
*/
void SysTickDelay(uint16_t m)
{
    systick_t = m;
    while(systick_t != 0);
}
