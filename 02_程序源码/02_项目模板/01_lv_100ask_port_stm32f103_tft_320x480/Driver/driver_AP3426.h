#ifndef __DRIVER_AP3426_H
#define __DRIVER_AP3426_H

#include "main.h"

#define AP3426_ADDR				(0x3C)
#define AP3426_W				(0x00)
#define AP3426_R				(0x01)

/**************** System Register ****************/
#define SYS_CONFIG_ADDR			(0x00)	// System Configuration, Default:0x00,Bits[2:0],System Mode
#define SYS_POWER_DOWN			(0x00)	// 0x00: Power Down
#define SYS_ALS_ACT				(0x01)  // 0x01: ALS function active
#define SYS_PS_IR_ACT			(0x02)	// 0x02: PS+IR function active
#define SYS_ALS_PS_IR_ACT		(0x03)	// 0x03: ALS and PS+IR function active
#define SYS_SW_RESET			(0x04)	// 0x04: SW reset
#define SYS_ALS_ONCE			(0x05)	// 0x05: ALS function once
#define SYS_PS_IR_ONCE			(0x06)	// 0x06: PS+IR function once
#define SYS_ALS_PS_IR_ONCE		(0x07)	// 0x07: ALS and PS+IR function once

#define INT_STATUS				(0x01)	// Bits[0]: PS Int;	Bits[1]: ALS Int, Default: 0
										//  0x0: Interrupt is cleared or not triggered yet
										//  0x1: Interrupt is triggered
#define INT_CLEAR_MANNER		(0x02)	// Bits[0]
										//  0x0: Int status is automatically cleared by reading registers 0x0F
										//  0x1: Clear INT status by software. Also pink & writing 1 into PS Int bit

#define IR_DATA_LOW				(0x0A)	// Bits[7]: IR overflow(RO);	0x0: Valid IR and PS data; 0x1:Invalid IR and PS data
										// Bits[1:0](RO);	IR lower byte of ADC OUTPUT
#define IR_DATA_HIGH			(0x0B)	// Bits[1:0](RO);	IR Higher byte of ADC OUTPUT

#define ALS_DATA_LOW			(0x0C)	// Bits[7:0](RO)	ALS lower byte of ADC OUTPUT
#define ALS_DATA_HIGH			(0x0D)	// Bits[7:0](RO)	ALS Higher byte of ADC OUTPUT

#define PS_DATA_LOW				(0x0E)	// Bits[7](RO):Object detect:
										// 			   0x0: The object leaving;
										// 			   0x1: The object closed
										// Bits[6](RO):IR over flow:
										// 			   0x0:Valid IR, PS data and object detected;
										// 			   0X1:Invalid IR, PS data and object detected;
										// Bits[3:0](RO): PS lower byte of ADC output
#define PS_DATA_HIGH			(0x0F)	// Bits[7](RO):Object detect:
										// 			   0x0: The object leaving;
										// 			   0x1: The object closed
										// Bits[6](RO):IR over flow:
										// 			   0x0:Valid IR, PS data and object detected;
										// 			   0X1:Invalid IR, PS data and object detected;
										// Bits[3:0](RO): PS higher byte of ADC output

/**************** ALS Register ****************/
#define ALS_CONFIG				(0x10)	// Control of gain, conversion time of persist for ALS
#define ALS_CALIBRATION			(0x19)	// ALS window loss calibration
#define ALS_LOW_LSB_THRESHOLD	(0x1A)	// [7:0]	Low Byte of ALS low threshold
#define ALS_LOW_MSB_THRESHOLD	(0x1B)	// [15:8]	High Byte of ALS low threshold
#define ALS_HIGH_LSB_THRESHOLD	(0x1C)	// [7:0]	Low Byte of ALS High threshold
#define ALS_HIGH_MSB_THRESHOLD	(0x1D)	// [15:8]	High Byte of ALS High threshold

/**************** PS Register ****************/
#define PS_CONFIG				(0x20)	// Control of gain, integrated time and persist for PS
#define PS_LED_DRIVER			(0x21)	// Control of LED pulses number and driver current
#define PS_INT_FORM				(0x22)	// interrupt algorithms style select of PS
#define PS_MEAN_TIME			(0x23)	// PS average time selector
#define PS_LED_WAIT_TIME		(0x24)	// Control PS LED Waitting time
#define PS_CALIBRATIOL_L		(0x28)	// Offset value to eliminate cross talk
#define PS_CALIBRATIOL_H		(0x29)	// Offset value to eliminate cross talk
#define PS_LOW_LSB_THRESHOLD	(0x2A)	// [1:0]	Low byte of PS Low threshold
#define PS_LOW_MSB_THRESHOLD	(0x2B)	// [9:2]	High byte of PS Low threshold
#define PS_HIGH_LSB_THRESHOLD	(0x2C)	// [1:0]	Low byte of PS High threshold
#define PS_HIGH_MSB_THRESHOLD	(0x2D)	// [9:2]	High byte of PS High threshold

extern uint8_t AP3426_Init(void);
extern void AP3426_ReadDataTest(void);
extern uint8_t AP3426_WriteOneByte(uint8_t reg, uint8_t data);
extern uint8_t AP3426_ReadOneByte(uint8_t reg);
extern void AP3426_Read_IR_Data(uint16_t *pIR);
extern void AP3426_Read_PS_Data(uint16_t *pPS);
extern void AP3426_Read_ALS_Data(uint16_t *pALS);
#endif
