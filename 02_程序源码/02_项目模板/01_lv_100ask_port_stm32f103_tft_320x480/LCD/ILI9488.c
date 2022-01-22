#include <stdio.h>
#include "ILI9488.h"
#include "lcd_function.h"

static void ILI9488_RegConfig(void);

void ILI9488_Init(void)
{
    //uint8_t status = 0;
    ILI9488_RegConfig();
    
    LCD_GRAM_Scan(0);
    LCD_Clear(WHITE);
}

static void ILI9488_RegConfig(void)
{
    LCD_Write_Cmd(0xE0); //P-Gamma   
	LCD_Write_Data(0x00);// LCD_Write_Data(0x00);   
	LCD_Write_Data(0x07);// LCD_Write_Data(0x13);   
	LCD_Write_Data(0x10);// LCD_Write_Data(0x18);   
	LCD_Write_Data(0x09);// LCD_Write_Data(0x04);   
	LCD_Write_Data(0x17);// LCD_Write_Data(0x0F);   
	LCD_Write_Data(0x0B);// LCD_Write_Data(0x06);   
	LCD_Write_Data(0x41);// LCD_Write_Data(0x3A);   
	LCD_Write_Data(0x89);// LCD_Write_Data(0x56);   
	LCD_Write_Data(0x4B);// LCD_Write_Data(0x4D);   
	LCD_Write_Data(0x0A);// LCD_Write_Data(0x03);   
	LCD_Write_Data(0x0C);// LCD_Write_Data(0x0A);   
	LCD_Write_Data(0x0E);// LCD_Write_Data(0x06);   
	LCD_Write_Data(0x18);// LCD_Write_Data(0x30);   
	LCD_Write_Data(0x1B);// LCD_Write_Data(0x3E);   
	LCD_Write_Data(0x0F);// LCD_Write_Data(0x0F);   
	   
	LCD_Write_Cmd(0xE1); //N-Gamma   
	LCD_Write_Data(0x00);// LCD_Write_Data(0x00);   
	LCD_Write_Data(0x17);// LCD_Write_Data(0x13);   
	LCD_Write_Data(0x1A);// LCD_Write_Data(0x18);   
	LCD_Write_Data(0x04);// LCD_Write_Data(0x01);   
	LCD_Write_Data(0x0E);// LCD_Write_Data(0x11);   
	LCD_Write_Data(0x06);// LCD_Write_Data(0x06);   
	LCD_Write_Data(0x2F);// LCD_Write_Data(0x38);   
	LCD_Write_Data(0x45);// LCD_Write_Data(0x34);   
	LCD_Write_Data(0x43);// LCD_Write_Data(0x4D);   
	LCD_Write_Data(0x02);// LCD_Write_Data(0x06);   
	LCD_Write_Data(0x0A);// LCD_Write_Data(0x0D);   
	LCD_Write_Data(0x09);// LCD_Write_Data(0x0B);   
	LCD_Write_Data(0x32);// LCD_Write_Data(0x31);   
	LCD_Write_Data(0x36);// LCD_Write_Data(0x37);   
	LCD_Write_Data(0x0F);// LCD_Write_Data(0x0F);    
	   
    // DB_EN = 0
    LCD_Write_Cmd(0xE9);  
    LCD_Write_Data(0x00);
    
    // Adjust Control
    LCD_Write_Cmd(0xF2);
    LCD_Write_Data(0x58);
    LCD_Write_Data(0x04);
    LCD_Write_Data(0x12);
    LCD_Write_Data(0x02);
    LCD_Write_Data(0x22);
    LCD_Write_Data(0x42);
    LCD_Write_Data(0xFF);
    LCD_Write_Data(0x0A);
    LCD_Write_Data(0x90);
    LCD_Write_Data(0x14);
    LCD_Write_Data(0x88);
    
    // Adjust Control 3
    LCD_Write_Cmd(0xF7);      
	LCD_Write_Data(0xA9);   
	LCD_Write_Data(0x51);   
	LCD_Write_Data(0x2C);   
	LCD_Write_Data(0x82);  // D7 stream, loose   
    
    // Adjust Control 4
    LCD_Write_Cmd(0xF8);      
	LCD_Write_Data(0x21);   
	LCD_Write_Data(0x04); 
    
	LCD_Write_Cmd(0xC0);   //Power Control 1   
	LCD_Write_Data(0x11); //Vreg1out   
	LCD_Write_Data(0x09); //Verg2out   
	   
	LCD_Write_Cmd(0xC1);   //Power Control 2   
	LCD_Write_Data(0x41); //VGH,VGL   
	   
    
	LCD_Write_Cmd(0xC5);   //VCOM Control  
	LCD_Write_Data(0x00);  
    LCD_Write_Data(0x0A);    
	LCD_Write_Data(0x80); //Vcom   
	   
	LCD_Write_Cmd(0x36);   //Memory Access   
	LCD_Write_Data(0xC8);   //48   
	   
	LCD_Write_Cmd(0x3A);   // Interface Pixel Format   
	LCD_Write_Data(0x55); //16bit   
	   
	LCD_Write_Cmd(0xB0);   // Interface Mode Control   
	LCD_Write_Data(0x00);   
	   
	LCD_Write_Cmd(0xB1);
	LCD_Write_Data(0xB0);
	LCD_Write_Data(0x11);
	   
	LCD_Write_Cmd(0xB4);   //Display Inversion Control   
	LCD_Write_Data(0x02); //2-dot   
	   
	LCD_Write_Cmd(0xB6);   //RGB/MCU Interface Control   
	LCD_Write_Data(0x02); //MCU RGB   
	LCD_Write_Data(0x22); //Source,Gate scan dieection  

	/* Entry Mode Set (B7h)  */
	LCD_Write_Cmd(0xB7);
	LCD_Write_Data(0xc6);

	/* HS Lanes Control (BEh) */
	LCD_Write_Cmd(0xBE);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x04);
    
    LCD_Write_Cmd(0xE9);    // Set Image Function   
	LCD_Write_Data(0x00);  //disable 24 bit data input  
	   
//	LCD_Write_Cmd(0x20);    //Normal Black   
	LCD_Write_Cmd(0x11);    //Sleep out   
	lcd_delay_ms(120);   
	LCD_Write_Cmd(0x29); 
}

