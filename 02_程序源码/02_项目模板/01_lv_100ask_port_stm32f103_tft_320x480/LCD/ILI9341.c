#include "ILI9341.h"
#include "lcd_function.h"

static void ILI9341_RegConfig(void);

void ILI9341_Init(void)
{
    ILI9341_RegConfig();
    LCD_GRAM_Scan(0);
    LCD_Clear(WHITE);
}

static void ILI9341_RegConfig(void)
{
    /*  Power control B (CFh)  */
	lcd_delay_us(10);
	LCD_Write_Cmd(0xCF);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x81);
	LCD_Write_Data(0x30);
	
	/*  Power on sequence control (EDh) */
	lcd_delay_us(10);
	LCD_Write_Cmd(0xED);
	LCD_Write_Data(0x64);
	LCD_Write_Data(0x03);
	LCD_Write_Data(0x12);
	LCD_Write_Data(0x81);
	
	/*  Driver timing control A (E8h) */
	lcd_delay_us(10);
	LCD_Write_Cmd(0xE8);
	LCD_Write_Data(0x85);
	LCD_Write_Data(0x10);
	LCD_Write_Data(0x78);
	
	/*  Power control A (CBh) */
	lcd_delay_us(10);
	LCD_Write_Cmd(0xCB);
	LCD_Write_Data(0x39);
	LCD_Write_Data(0x2C);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x34);
	LCD_Write_Data(0x02);
	
	/* Pump ratio control (F7h) */
	lcd_delay_us(10);
	LCD_Write_Cmd(0xF7);
	LCD_Write_Data(0x20);
	
	/* Driver timing control B */
	lcd_delay_us(10);
	LCD_Write_Cmd(0xEA);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x00);
	
	/* Frame Rate Control (In Normal Mode/Full Colors) (B1h) */
	lcd_delay_us(10);
	LCD_Write_Cmd(0xB1);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x1B);
	
	/*  Display Function Control (B6h) */
	lcd_delay_us(10);
	LCD_Write_Cmd(0xB6);
	LCD_Write_Data(0x0A);
	LCD_Write_Data(0xA2);
	
	/* Power Control 1 (C0h) */
	lcd_delay_us(10);
	LCD_Write_Cmd(0xC0);
	LCD_Write_Data(0x35);
	
	/* Power Control 2 (C1h) */
	lcd_delay_us(10);
	LCD_Write_Cmd(0xC1);
	LCD_Write_Data(0x11);
	
	/* VCOM Control 1 (C5h) */
	LCD_Write_Cmd(0xC5);
	LCD_Write_Data(0x45);
	LCD_Write_Data(0x45);
	
	/*  VCOM Control 2 (C7h)  */
	LCD_Write_Cmd(0xC7);
	LCD_Write_Data(0xA2);
	
	/* Enable 3G (F2h) */
	LCD_Write_Cmd(0xF2);
	LCD_Write_Data(0x00);
	
	/* Gamma Set (26h) */
	LCD_Write_Cmd(0x26);
	LCD_Write_Data(0x01);
	lcd_delay_us(10);
	
	/* Positive Gamma Correction */
	LCD_Write_Cmd(0xE0); //Set Gamma
	LCD_Write_Data(0x0F);
	LCD_Write_Data(0x26);
	LCD_Write_Data(0x24);
	LCD_Write_Data(0x0B);
	LCD_Write_Data(0x0E);
	LCD_Write_Data(0x09);
	LCD_Write_Data(0x54);
	LCD_Write_Data(0xA8);
	LCD_Write_Data(0x46);
	LCD_Write_Data(0x0C);
	LCD_Write_Data(0x17);
	LCD_Write_Data(0x09);
	LCD_Write_Data(0x0F);
	LCD_Write_Data(0x07);
	LCD_Write_Data(0x00);
	
	/* Negative Gamma Correction (E1h) */
	LCD_Write_Cmd(0XE1); //Set Gamma
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x19);
	LCD_Write_Data(0x1B);
	LCD_Write_Data(0x04);
	LCD_Write_Data(0x10);
	LCD_Write_Data(0x07);
	LCD_Write_Data(0x2A);
	LCD_Write_Data(0x47);
	LCD_Write_Data(0x39);
	LCD_Write_Data(0x03);
	LCD_Write_Data(0x06);
	LCD_Write_Data(0x06);
	LCD_Write_Data(0x30);
	LCD_Write_Data(0x38);
	LCD_Write_Data(0x0F);
	
	/* memory access control set */
	lcd_delay_us(10);
	LCD_Write_Cmd(0x36); 	
	LCD_Write_Data(0xC8);    /*竖屏  左上角到 (起点)到右下角 (终点)扫描方式*/
	lcd_delay_us(10);
	
	/* column address control set */
	LCD_Write_Cmd(CMD_SetCoordinateX); 
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0xEF);
	
	/* page address control set */
	lcd_delay_us(10);
	LCD_Write_Cmd(CMD_SetCoordinateY); 
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x01);
	LCD_Write_Data(0x3F);
	
	/*  Pixel Format Set (3Ah)  */
	lcd_delay_us(10);
	LCD_Write_Cmd(0x3a); 
	LCD_Write_Data(0x55);
	
	/* Sleep Out (11h)  */
	LCD_Write_Cmd(0x11);	
	lcd_delay_us(0xAFFf);
	lcd_delay_us(10);
	
	/* Display ON (29h) */
	LCD_Write_Cmd(0x29); 	
}

