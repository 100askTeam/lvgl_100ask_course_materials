/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：driver_can.c
 * 摘要：
 *  
 * 修改历史     版本号        Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/
#include "main.h"
#include "driver_can.h"

CAN_HandleTypeDef     hcan;
CAN_TxHeaderTypeDef   TxHeader;
CAN_RxHeaderTypeDef   RxHeader;

/*
 *  函数名：void CAN_Init(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：初始化CAN1
*/
void CAN_Init(void)
{
    CAN_FilterTypeDef  sFilterConfig;
    
    hcan.Instance = CAN1;

    /* 配置CAN的基本参数 */
    hcan.Init.TimeTriggeredMode    = DISABLE;
    hcan.Init.AutoBusOff           = DISABLE;
    hcan.Init.AutoWakeUp           = DISABLE;
    hcan.Init.AutoRetransmission   = ENABLE;
    hcan.Init.ReceiveFifoLocked    = DISABLE;
    hcan.Init.TransmitFifoPriority = DISABLE;
    hcan.Init.Mode                 = CAN_MODE_NORMAL;
    hcan.Init.SyncJumpWidth        = CAN_SJW_1TQ;
    hcan.Init.TimeSeg1             = CAN_BS1_6TQ;
    hcan.Init.TimeSeg2             = CAN_BS2_2TQ;
    hcan.Init.Prescaler            = 8;                // 36M/(1+6+2)/8 = 500kBPS

    if (HAL_CAN_Init(&hcan) != HAL_OK)
    {
        Error_Handler();
    }
    
    /* 配置CAN的过滤器，此处全部接收，不做过滤 */
    sFilterConfig.FilterBank            = 0;
    sFilterConfig.FilterMode            = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale           = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh          = 0x0000;
    sFilterConfig.FilterIdLow           = 0x0000;
    sFilterConfig.FilterMaskIdHigh      = 0x0000;
    sFilterConfig.FilterMaskIdLow       = 0x0000;
    sFilterConfig.FilterFIFOAssignment  = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation      = ENABLE;
    sFilterConfig.SlaveStartFilterBank  = 14;

    if(HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    
    // 启用CAN
    if(HAL_CAN_Start(&hcan) != HAL_OK)
    {
        Error_Handler();
    }
    
    // 设置CAN接收FIFO
    if(HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
    {
        Error_Handler();
    }
}

void HAL_CAN_MspInit(CAN_HandleTypeDef *hcanhandle)
{
    GPIO_InitTypeDef   GPIO_InitStruct;

    __HAL_RCC_CAN1_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_AFIO_CLK_ENABLE();
    __HAL_AFIO_REMAP_CAN1_2();

    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_PULLUP;

    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_PULLUP;

    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
    
    HAL_NVIC_SetPriority(CAN1_TX_IRQn, 1, 1);
    HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
}

/*
 *  函数名：void CAN_Transmit(uint16_t ID, uint8_t *pdata, uint8_t length)
 *  输入参数：ID    -> CAN发送报文的ID
 *            pdata -> 发送报文的首地址
 *            length-> 发送报文的个数，最大8个字节
 *  输出参数：无
 *  返回值：无
 *  函数作用：CAN1发送函数
*/
void CAN_Transmit(uint16_t ID, uint8_t *pdata, uint8_t length)
{
    uint32_t tx_mailbox = 0;
    if(length>8)
    {
        return ;
    }
    
    TxHeader.StdId = ID;
    TxHeader.ExtId = 0;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.DLC = length;
    TxHeader.TransmitGlobalTime = DISABLE; 

    if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, pdata, &tx_mailbox) != HAL_OK)
    {
        Error_Handler();
    }    
}
static uint8_t can_rx_flag = 0;
void CAN_SetRxFlag(uint8_t flag)
{
    can_rx_flag = flag;
}

uint8_t CAN_GetRxFlag(void)
{
    return can_rx_flag;
}

/*
 *  函数名：void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *CanHandle)
 *  输入参数：CanHandle -> CAN句柄
 *  输出参数：无
 *  返回值：无
 *  函数作用：CAN1接收回调函数
*/
static uint8_t can_rx_buf[8] = {0};
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *CanHandle)
{
    /* Get RX message */
    if (HAL_CAN_GetRxMessage(CanHandle, CAN_RX_FIFO0, &RxHeader, can_rx_buf) != HAL_OK)
    {
        Error_Handler();
    }
    CAN_SetRxFlag(1);
}

void CAN1_RX0_IRQHandler(void)
{
    HAL_CAN_IRQHandler(&hcan);
}

void CAN1_TX0_IRQHandler(void)
{
    HAL_CAN_IRQHandler(&hcan);
}

