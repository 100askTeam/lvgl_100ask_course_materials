#include <stdio.h>
#include "ICM_20608_G.h"
#include "driver_spi.h"
#include "driver_timer.h"

#define DUMMY_BYTE				(0xFF)

#define ICM_CS_ENABLE()			ICM_CS(0)
#define ICM_CS_DISABLE()		ICM_CS(1)

#define ICM_WriteByte(data)		SPI_WriteByte(data)
#define ICM_ReadByte()			SPI_ReadByte()
#define ICM_RW_Byte(data)		SPI_WriteReadByte(data)

uint16_t ICM_RW_Register(uint8_t reg, uint8_t data)
{
	uint8_t ret0 = 0, ret1 = 0;
	uint16_t temp = 0;
	// 片选使能
	ICM_CS_ENABLE();

	ret0 = ICM_RW_Byte(reg);
    ret1 = ICM_RW_Byte(data);

	// 片选失能
	ICM_CS_DISABLE();

    temp = (ret0>>8) | (ret1);
    
	return (temp);
}


void ICM_Init(void)
{
	uint8_t ret = 0;

	// 复位ICM
	ICM_RW_Register(ICM_PWM_MGMT_1 | ICM_WRITE, 0x80);
	HAL_Delay(50);

	// 选择时钟
	ICM_RW_Register(ICM_PWM_MGMT_1 | ICM_WRITE, 0x01);
	HAL_Delay(50);
	// 获取设备ID
	ret = ICM_RW_Register(ICM_WHO_AM_I | ICM_READ, DUMMY_BYTE);
	printf("\r\nICM ID: 0x%x", ret);
}

