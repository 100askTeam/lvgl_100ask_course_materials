/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：driver_ird.c
 * 摘要：
 *  
 * 修改历史     版本号        Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/

#include <string.h>
#include <stdio.h>
#include "main.h"
#include "driver_ird.h"
#include "driver_timer.h"
#include "ring_buffer.h"

volatile IRD_DATA IRD_Data = {0};


void GetKeyInfo(uint32_t value);

void IRD_GpioInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    IRD_GPIO_CLK_EN();
    
    GPIO_InitStruct.Pin   = IRD_PIN;             // 选择LED的引脚
    GPIO_InitStruct.Mode  = GPIO_MODE_IT_RISING_FALLING; // 设置为推挽输出模式
    GPIO_InitStruct.Pull  = GPIO_PULLUP;         // 默认上拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;// 引脚反转速度设置为快

    // 初始化引脚配置
    HAL_GPIO_Init(IRD_PORT, &GPIO_InitStruct);
}

//void EXTI15_10_IRQHandler(void)
//{
//    HAL_GPIO_EXTI_IRQHandler(IRD_PIN);
//}

static uint32_t pIrdCurTime = {0}, pIrdPreTime = {0};
void IRD_EXTI_Callback(void)
{
    uint16_t delta_time = 0;
    //IRD_DATA ird_data = {0};
    pIrdCurTime = sysTime;
    delta_time = Timer_GetDeltaTime(pIrdCurTime, pIrdPreTime);
    RingBuffer_Write((uint8_t*)&delta_time, 2);
    pIrdPreTime = pIrdCurTime;
}

#if 0
void IRD_Decode(void)
{
    //char *ptr = 0;
    uint8_t usr_code  ;
    uint8_t usr_decode;
    uint8_t sys_code  ;
    uint8_t sys_decode;
    uint8_t i = 0, cnt = 0;
    uint16_t temp_buff[64] = {0};
    
    if(IRD_Data.data_code==0)       // 在数据码前有一段引导码，先获取到引导码
    {
        RingBuffer_Read((uint8_t*)&IRD_Data.period, sizeof(IRD_Data.period));
    
        if( (IRD_Data.period>=8500) && (IRD_Data.period<=9500) )  // 引导码
        {
            IRD_Data.pilot_code = 1;
            IRD_Data.serial_code = 0;
            IRD_Data.data_code = 0;
            IRD_Data.value = 0;
        }
        if(IRD_Data.pilot_code==1)
        {
            if( (IRD_Data.period>=4000) && (IRD_Data.period<=5000) )    // 非连发码
            {
                IRD_Data.serial_code = 0;
                IRD_Data.data_code = 1;     // 将引导码获取后就可以去获取数据码了
            }
            else if( (IRD_Data.period>=2000) && (IRD_Data.period<=2500) )   // 连发码
            {
                IRD_Data.serial_code = 1;
                IRD_Data.data_code = 1;
            }
        }
    }    
    if(IRD_Data.data_code==1)
    {
        HAL_Delay(110); // 一个NEC协议的码时间总长是108ms，我们在获取到了引导码之后人为延迟110ms再去获取完整的数据码来解码
        RingBuffer_Read((uint8_t*)temp_buff, sizeof(temp_buff));
        
        for(i=0;i<64;i+=2)
        {
            if( (temp_buff[i]>=450) && (temp_buff[i]<=650) ) 
            {
                if( (temp_buff[i+1]>=450) && (temp_buff[i+1]<=650) )    // 逻辑0
                {
                    IRD_Data.value = IRD_Data.value + (0<<cnt);
                    cnt++;
                } 
                else if( (temp_buff[i+1]>=1000) && (temp_buff[i+1]<=2000) ) // 逻辑1
                {
                    IRD_Data.value = IRD_Data.value + (1<<cnt);
                    cnt++;
                }
            }
        }
        
        usr_code   =  IRD_Data.value & 0xFF;
        usr_decode = (IRD_Data.value>>8) & 0xFF;    
        sys_code   = (IRD_Data.value>>16) & 0xFF;   
        sys_decode = (IRD_Data.value>>24) & 0xFF;  
        
        IRD_Data.pilot_code = 0;
        IRD_Data.serial_code = 0;
        IRD_Data.data_code = 0;
        IRD_Data.value = 0; 
    }
}
#endif
