#include "driver_usart.h"
#include "driver_usart2.h"
#include "driver_usart3.h"
#include "driver_msp_usart.h"
/*
 *  函数名：void AL_USART_MspInit(USART_HandleTypeDef* husart)
 *  输入参数：husart-USART句柄
 *  输出参数：无
 *  返回值：无
 *  函数作用：使能USART1的时钟，使能引脚时钟，并配置引脚的复用功能
*/
void HAL_UART_MspInit(UART_HandleTypeDef* husart)
{
    // 定义GPIO结构体对象
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(husart->Instance==USARTx)
    {
        // 使能USART3的时钟
        USARTx_CLK_EN();
        
        // 使能USART3的输入输出引脚的时钟
        USARTx_GPIO_CLK_EN();
        /**USART3 GPIO Configuration   
        PB10     ------> USART3_TX
        PB11     ------> USART3_RX
        */
        GPIO_InitStruct.Pin = USARTx_TX_PIN;            // 选择USART1的TX引脚
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;         // 配置为复用推挽功能
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;   // 引脚翻转速率快
        HAL_GPIO_Init(USARTx_PORT, &GPIO_InitStruct);   // 初始化TX引脚

        GPIO_InitStruct.Pin = USARTx_RX_PIN;            // 选择RX引脚
        GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;         // 配置为输入
        HAL_GPIO_Init(USARTx_PORT, &GPIO_InitStruct);   // 初始化RX引脚
        
        HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
                                                    
        HAL_NVIC_EnableIRQ(USART1_IRQn);        
    }
    else if(husart->Instance==RS232_MALE)
    {
        // 使能USART2的时钟
        RS232_MALE_CLK_ENABLE();
        RE_DE_GPIO_CLK_EN();
        
        // 使能USART2的输入输出和方向引脚的时钟
        RS232_MALE_GPIO_CLK_EN();
        
        
        GPIO_InitStruct.Pin = RS232_MALE_TX_PIN;            // 选择USART2的TX引脚
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;         // 配置为复用推挽功能
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;   // 引脚翻转速率快
        HAL_GPIO_Init(RS232_MALE_PORT, &GPIO_InitStruct);   // 初始化TX引脚

        GPIO_InitStruct.Pin = RS232_MALE_RX_PIN;            // 选择RX引脚
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;         // 配置为输入
        GPIO_InitStruct.Pull = GPIO_NOPULL;             // 不上拉
        HAL_GPIO_Init(RS232_MALE_PORT, &GPIO_InitStruct);   // 初始化RX引脚
        
        GPIO_InitStruct.Pin = RE_DE_PIN;                // 选择方向引脚
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;         // 配置为输出
        GPIO_InitStruct.Pull = GPIO_NOPULL;             // 不上拉
        HAL_GPIO_Init(RE_DE_PORT, &GPIO_InitStruct);    // 初始化方向引脚
        
        RE_DE_RX();                                     // 初始化后默认处于接收状态
        
        HAL_NVIC_SetPriority(RS232_MALE_IRQn, 1, 0);        // 设置USART3的中断等级（0-15）（0-15）
                                                        // 规则：(0,0)最高，(0,1)次之依次由高到低排序到(15,15)
        HAL_NVIC_EnableIRQ(RS232_MALE_IRQn);                // 使能USART3的中断
    }
    else if(husart->Instance==RS232_FEMALE)
    {
        // 使能USART3的时钟
        RS232_FEMALE_CLK_ENABLE();
        
        // 使能USART3的输入输出引脚的时钟
        RS232_FEMALE_GPIO_CLK_EN();
        /**USART3 GPIO Configuration   
        PB10     ------> USART3_TX
        PB11     ------> USART3_RX
        */
        GPIO_InitStruct.Pin = RS232_FEMALE_TX_PIN;            // 选择USART3的TX引脚
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;         // 配置为复用推挽功能
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;   // 引脚翻转速率快
        HAL_GPIO_Init(RS232_FEMALE_PORT, &GPIO_InitStruct);   // 初始化TX引脚

        GPIO_InitStruct.Pin = RS232_FEMALE_RX_PIN;            // 选择RX引脚
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;         // 配置为输入
        GPIO_InitStruct.Pull = GPIO_NOPULL;             // 不上拉
        HAL_GPIO_Init(RS232_FEMALE_PORT, &GPIO_InitStruct);   // 初始化RX引脚
        
        HAL_NVIC_SetPriority(RS232_FEMALE_IRQn, 1, 0);        // 设置USART3的中断等级（0-15）（0-15）
                                                        // 规则：(0,0)最高，(0,1)次之依次由高到低排序到(15,15)
        HAL_NVIC_EnableIRQ(RS232_FEMALE_IRQn);                // 使能USART3的中断
    }
}

/*
 *  函数名：void HAL_USART_MspDeInit(USART_HandleTypeDef* husart)
 *  输入参数：husart-USART句柄
 *  输出参数：无
 *  返回值：无
 *  函数作用：失能USART3的时钟，失能引脚时钟，并将RX/TX引脚配置为默认状态
*/
void HAL_UART_MspDeInit(UART_HandleTypeDef* husart)
{
    if(husart->Instance==USARTx)
    {
        USARTx_CLK_DIS();

        /**USART3 GPIO Configuration    
        PB10     ------> USART3_TX
        PB11     ------> USART3_RX
        */
        HAL_GPIO_DeInit(USARTx_PORT, USARTx_TX_PIN | USARTx_RX_PIN);
    }
    else if(husart->Instance==RS232_MALE)
    {
        RS232_MALE_CLK_DISABLE();

        HAL_GPIO_DeInit(RS232_MALE_PORT, RS232_MALE_TX_PIN | RS232_MALE_RX_PIN);
        
        HAL_NVIC_DisableIRQ(RS232_MALE_IRQn);
    }
    else if(husart->Instance==RS232_FEMALE)
    {
        __HAL_RCC_USART3_CLK_DISABLE();

        /**USART3 GPIO Configuration    
        PB10     ------> USART3_TX
        PB11     ------> USART3_RX
        */
        HAL_GPIO_DeInit(RS232_FEMALE_PORT, RS232_FEMALE_TX_PIN | RS232_FEMALE_RX_PIN);
        
        HAL_NVIC_DisableIRQ(RS232_FEMALE_IRQn);
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        debug_tx_finish = 1;
    }
    else if(huart->Instance == RS232_MALE)
    {
        male_tx_finish = 1;
    }
    else if(huart->Instance == RS232_FEMALE)
    {
        female_tx_finish = 1;
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        debug_rx_finish = 1;
    }
    else if(huart->Instance == RS232_MALE)
    {
        male_rx_finish = 1;
    }
    else if(huart->Instance == RS232_FEMALE)
    {
        female_rx_finish = 1;
    }
}

