#include <stdio.h>
#include "driver_sd.h"

SD_HandleTypeDef uSdHandle;
HAL_SD_CardInfoTypeDef  sd_cardinfo;
uint8_t SD_DMAConfigRx(SD_HandleTypeDef *hsd);
uint8_t SD_DMAConfigTx(SD_HandleTypeDef *hsd);

uint8_t BSP_SD_Init(void)
{
    uint8_t state = MSD_OK;

    /* uSD device interface configuration */
    uSdHandle.Instance = SDIO;

    uSdHandle.Init.ClockEdge           = SDIO_CLOCK_EDGE_RISING;
    uSdHandle.Init.ClockBypass         = SDIO_CLOCK_BYPASS_DISABLE;
    uSdHandle.Init.ClockPowerSave      = SDIO_CLOCK_POWER_SAVE_DISABLE;
    uSdHandle.Init.BusWide             = SDIO_BUS_WIDE_1B;
    uSdHandle.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
    uSdHandle.Init.ClockDiv            = SDIO_INIT_CLK_DIV;
    
    if(HAL_SD_Init(&uSdHandle) != HAL_OK)
    {
        state = MSD_ERROR;
    }

    /* Configure SD Bus width */
    if(state == MSD_OK)
    {
        /* Enable wide operation */
        if(HAL_SD_ConfigWideBusOperation(&uSdHandle, SDIO_BUS_WIDE_4B) != HAL_OK)
        {
            state = MSD_ERROR;
        }
        else
        {
            state = MSD_OK;
        }
    }
    BSP_SD_GetCardInfo(&sd_cardinfo);
    return  state;
}
void HAL_SD_MspInit(SD_HandleTypeDef *hsd)
{
    GPIO_InitTypeDef gpioinitstruct = {0};

    /* Enable SDIO clock */
    __HAL_RCC_SDIO_CLK_ENABLE();

    /* Enable DMA2 clocks */
    __DMAx_TxRx_CLK_ENABLE();

    /* Enable GPIOs clock */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /* Common GPIO configuration */
    gpioinitstruct.Mode      = GPIO_MODE_AF_PP;
    gpioinitstruct.Pull      = GPIO_PULLUP;
    gpioinitstruct.Speed     = GPIO_SPEED_FREQ_HIGH;

    /* GPIOC configuration */
    gpioinitstruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;

    HAL_GPIO_Init(GPIOC, &gpioinitstruct);

    /* GPIOD configuration */
    gpioinitstruct.Pin = GPIO_PIN_2;
    HAL_GPIO_Init(GPIOD, &gpioinitstruct);
}

uint8_t BSP_SD_IsDetected(void)
{
    __IO uint8_t status = SD_PRESENT;

    return status;
}

void BSP_SD_DetectIT(void)
{
    /* SD detect IT callback */
    BSP_SD_DetectCallback();
  
}

__weak void BSP_SD_DetectCallback(void)
{
  /* NOTE: This function Should not be modified, when the callback is needed,
  the BSP_SD_DetectCallback could be implemented in the user file
  */ 
  
}

/**
  * @brief  Reads block(s) from a specified address in an SD card, in polling mode.
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  ReadAddr: Address from where data is to be read  
  * @param  NumOfBlocks: Number of SD blocks to read
  * @param  Timeout: Timeout for read operation
  * @retval SD status
  */
uint8_t BSP_SD_ReadBlocks(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks, uint32_t Timeout)
{
    if(HAL_SD_ReadBlocks(&uSdHandle, (uint8_t *)pData, ReadAddr, NumOfBlocks, Timeout) != HAL_OK)
    {
        return MSD_ERROR;
    }
    else
    {
        return MSD_OK;
    }
}

/**
  * @brief  Writes block(s) to a specified address in an SD card, in polling mode. 
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  WriteAddr: Address from where data is to be written
  * @param  NumOfBlocks: Number of SD blocks to write
  * @param  Timeout: Timeout for write operation
  * @retval SD status
  */
uint8_t BSP_SD_WriteBlocks(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks, uint32_t Timeout)
{
    if(HAL_SD_WriteBlocks(&uSdHandle, (uint8_t *)pData, WriteAddr, NumOfBlocks, Timeout) != HAL_OK)
    {
        return MSD_ERROR;
    }
    else
    {
        return MSD_OK;
    }
}

/**
  * @brief  Reads block(s) from a specified address in an SD card, in DMA mode.
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  ReadAddr: Address from where data is to be read
  * @param  NumOfBlocks: Number of SD blocks to read 
  * @retval SD status
  */
uint8_t BSP_SD_ReadBlocks_DMA(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks)
{ 
    uint8_t state = MSD_OK;

    /* Invalidate the dma tx handle*/
    uSdHandle.hdmatx = NULL;

    /* Prepare the dma channel for a read operation */
    state = ((SD_DMAConfigRx(&uSdHandle) == MSD_OK) ? MSD_OK : MSD_ERROR);

    if(state == MSD_OK)
    {
        /* Read block(s) in DMA transfer mode */
        state = ((HAL_SD_ReadBlocks_DMA(&uSdHandle, (uint8_t *)pData, ReadAddr, NumOfBlocks) == HAL_OK) ? MSD_OK : MSD_ERROR);
    }

    return state; 
}

/**
  * @brief  Writes block(s) to a specified address in an SD card, in DMA mode.
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  WriteAddr: Address from where data is to be written
  * @param  NumOfBlocks: Number of SD blocks to write 
  * @retval SD status
  */
uint8_t BSP_SD_WriteBlocks_DMA(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks)
{ 
    uint8_t state = MSD_OK;

    /* Invalidate the dma rx handle*/
    uSdHandle.hdmarx = NULL;
    /* Prepare the dma channel for a read operation */
    state = ((SD_DMAConfigTx(&uSdHandle) == MSD_OK) ? MSD_OK : MSD_ERROR);

    if(state == MSD_OK)
    { 
        /* Write block(s) in DMA transfer mode */
        state = ((HAL_SD_WriteBlocks_DMA(&uSdHandle, (uint8_t *)pData, WriteAddr, NumOfBlocks) == HAL_OK) ? MSD_OK : MSD_ERROR);
    }

    return state;  
}

/**
  * @brief  Erases the specified memory area of the given SD card. 
  * @param  StartAddr: Start byte address
  * @param  EndAddr: End byte address
  * @retval SD status
  */
uint8_t BSP_SD_Erase(uint32_t StartAddr, uint32_t EndAddr)
{
    if(HAL_SD_Erase(&uSdHandle, StartAddr, EndAddr) != HAL_OK)
    {
        return MSD_ERROR;
    }
    else
    {
        return MSD_OK;
    }
}

/**
  * @brief  Gets the current SD card data status.
  * @retval Data transfer state.
  *          This value can be one of the following values:
  *            @arg  SD_TRANSFER_OK: No data transfer is acting
  *            @arg  SD_TRANSFER_BUSY: Data transfer is acting
  */
uint8_t BSP_SD_GetCardState(void)
{
    return((HAL_SD_GetCardState(&uSdHandle) == HAL_SD_CARD_TRANSFER ) ? SD_TRANSFER_OK : SD_TRANSFER_BUSY);
}
  

/**
  * @brief  Get SD information about specific SD card.
  * @param  CardInfo: Pointer to HAL_SD_CardInfoTypedef structure
  * @retval None 
  */
void BSP_SD_GetCardInfo(HAL_SD_CardInfoTypeDef *CardInfo)
{
    /* Get SD card Information */
    HAL_SD_GetCardInfo(&uSdHandle, CardInfo);
}


/**
  * @brief SD_DMAConfigRx
  * @par Function Description
  *   This function configure the DMA to receive data from the SD card
  * @retval
  *  SD_ERROR or SD_OK
  */
uint8_t SD_DMAConfigRx(SD_HandleTypeDef *hsd)
{
    static DMA_HandleTypeDef hdma_rx;
    HAL_StatusTypeDef status = HAL_ERROR;

    if(hsd->hdmarx == NULL)
    {
        /* Configure DMA Rx parameters */
        hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
        hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
        hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
        hdma_rx.Init.Priority            = DMA_PRIORITY_VERY_HIGH;

        hdma_rx.Instance = SD_DMAx_Rx_INSTANCE;

        /* Associate the DMA handle */
        __HAL_LINKDMA(hsd, hdmarx, hdma_rx);

        /* Stop any ongoing transfer and reset the state*/
        HAL_DMA_Abort(&hdma_rx);

        /* Deinitialize the Channel for new transfer */
        HAL_DMA_DeInit(&hdma_rx);

        /* Configure the DMA Channel */
        status = HAL_DMA_Init(&hdma_rx);

        /* NVIC configuration for DMA transfer complete interrupt */
        HAL_NVIC_SetPriority(SD_DMAx_Rx_IRQn, 0xD, 0);
        HAL_NVIC_EnableIRQ(SD_DMAx_Rx_IRQn);
    }
    else
    {
        status = HAL_OK;
    }

    return (status != HAL_OK? MSD_ERROR : MSD_OK);
}

/**
  * @brief SD_DMAConfigTx
  * @par Function Description
  *   This function configure the DMA to transmit data to the SD card
  * @retval
  *  SD_ERROR or SD_OK
  */
uint8_t SD_DMAConfigTx(SD_HandleTypeDef *hsd)
{
    static DMA_HandleTypeDef hdma_tx;
    HAL_StatusTypeDef status;

    if(hsd->hdmatx == NULL)
    {
        /* Configure DMA Tx parameters */
        hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
        hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
        hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
        hdma_tx.Init.Priority            = DMA_PRIORITY_VERY_HIGH;

        hdma_tx.Instance = SD_DMAx_Tx_INSTANCE;

        /* Associate the DMA handle */
        __HAL_LINKDMA(hsd, hdmatx, hdma_tx);

        /* Stop any ongoing transfer and reset the state*/
        HAL_DMA_Abort(&hdma_tx);

        /* Deinitialize the Channel for new transfer */
        HAL_DMA_DeInit(&hdma_tx);

        /* Configure the DMA Channel */
        status = HAL_DMA_Init(&hdma_tx); 

        /* NVIC configuration for DMA transfer complete interrupt */
        HAL_NVIC_SetPriority(SD_DMAx_Tx_IRQn, 0xD, 0);
        HAL_NVIC_EnableIRQ(SD_DMAx_Tx_IRQn);  
    }
    else
    {
        status = HAL_OK;
    }

    return (status != HAL_OK? MSD_ERROR : MSD_OK);
}

/**
  * @brief SD Abort callbacks
  * @param hsd: SD handle
  * @retval None
  */
void HAL_SD_AbortCallback(SD_HandleTypeDef *hsd)
{
    BSP_SD_AbortCallback();
}

/**
  * @brief Tx Transfer completed callbacks
  * @param hsd: SD handle
  * @retval None
  */
void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd)
{
    BSP_SD_WriteCpltCallback();
}

/**
  * @brief Rx Transfer completed callbacks
  * @param hsd: SD handle
  * @retval None
  */
void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)
{
    BSP_SD_ReadCpltCallback();
}

/**
  * @brief BSP SD Abort callbacks
  * @retval None
  */
__weak void BSP_SD_AbortCallback(void)
{

}

/**
  * @brief BSP Tx Transfer completed callbacks
  * @retval None
  */
__weak void BSP_SD_WriteCpltCallback(void)
{
}

/**
  * @brief BSP Rx Transfer completed callbacks
  * @retval None
  */
__weak void BSP_SD_ReadCpltCallback(void)
{
}
