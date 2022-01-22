/*  Copyright (s) 2019 ���ڰ������Ƽ����޹�˾
 *  All rights reserved
 * 
 * �ļ����ƣ�driver_systick.h
 * ժҪ��
 *  
 * �޸���ʷ     �汾��        Author       �޸�����
 *--------------------------------------------------
 * 2020.6.6      v01        ���ʿƼ�      �����ļ�
 *--------------------------------------------------
*/

#ifndef __DRIVER_SYSTICK_H
#define __DRIVER_SYSTICK_H

#include "stm32f1xx_hal.h"

extern volatile uint16_t systick_t;

/*
 *  ��������void SysTickInit(uint16_t t)
 *  ���������t-�趨ʱ�䣺1->ms; 2->100us; 3->10us
 *  �����������
 *  ����ֵ����
 *  �������ã���ʼ��ϵͳ�δ�ʱ�ӵ�Ƶ�ʺ��ж����ȼ�
*/
extern void SysTickInit(uint16_t t);

/*
 *  ��������void SysTickDelay(uint16_t m)
 *  ���������m-��ʱʱ��
 *  �����������
 *  ����ֵ����
 *  �������ã��δ�ʱ��ʵ�ֵ���ʱ����
*/
extern void SysTickDelay(uint16_t m);

#endif