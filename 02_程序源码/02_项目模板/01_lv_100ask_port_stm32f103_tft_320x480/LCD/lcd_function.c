#include <stdio.h>
#include "fonts.h"
#include "ILI9341.h"
#include "ILI9488.h"
#include "lcd_function.h"
#include "driver_fsmc_lcd.h"

#define LCD                 ((__IO _lcd*)(0x6C000000 + (1<<24) - 2))

/********** 全局变量定义 **********/
_lcddev     lcddev;
_lcd_color  lcd_color;

/********** 局部变量定义 **********/

/********** LCD读写函数定义 **********/
/* 函数名：  void LCD_Write_Cmd(uint16_t _cmd)
 * 输入参数：_cmd->要写入的指令
 * 输出参数：无
 * 返回值：  无
 * 函数作用：往LCD写入指令
*/
void LCD_Write_Cmd(uint16_t _cmd)
{
    _cmd = _cmd;
    LCD->reg = _cmd;
}

/* 函数名：  void LCD_Write_Data(uint16_t _data)
 * 输入参数：_data->要写入的数据
 * 输出参数：无
 * 返回值：  无
 * 函数作用：往LCD写入数据
*/
void LCD_Write_Data(uint16_t _data)
{
    _data = _data;
    LCD->data = _data;
}

/* 函数名：  uint16_t LCD_Read_Data(void)
 * 输入参数：无
 * 输出参数：无
 * 返回值：  返回寄存器的值
 * 函数作用：返回寄存器的值
*/
uint16_t LCD_Read_Data(void)
{
    return LCD->data;
}

/* 函数名：  void LC_Write_Reg(uint16_t _cmd, uint16_t _data)
 * 输入参数：_cmd->要写入的指令；_data->要写入的数据
 * 输出参数：无
 * 返回值：  无
 * 函数作用：
*/
void LCD_Write_Reg(uint16_t _cmd, uint16_t _data)
{
    LCD_Write_Cmd(_cmd);
    LCD_Write_Data(_data);
}

/********** LCD颜色设置和获取函数定义 **********/
// 背景颜色
// 设置背景颜色
void LCD_SetBackColor(uint16_t _backcolor)
{
    lcd_color.backcolor = _backcolor;
}
// 获取背景颜色
uint16_t LCD_GetBackColor(void)
{
    return lcd_color.backcolor;
}

// 笔迹颜色
// 设置笔迹颜色
void LCD_SetTextColor(uint16_t _textcolor)
{
    lcd_color.textcolor = _textcolor;
}
// 获取笔迹颜色
uint16_t LCD_GetTextColor(void)
{
    return lcd_color.textcolor;
}

// 所有颜色
// 设置背景颜色和笔迹颜色
void LCD_SetAllColor(uint16_t _backcolor, uint16_t _textcolor)
{
    LCD_SetBackColor(_backcolor);
    LCD_SetTextColor(_textcolor);
}
// 获取背景颜色和笔迹颜色
void LCD_GetAllColor(uint16_t *_backcolor, uint16_t *_textcolor)
{
    *_backcolor = lcd_color.backcolor;
    *_textcolor = lcd_color.textcolor;
}

/********** LCD功能函数定义 **********/
void LCD_BackLed_Ctrl(FunctionalState state)
{
    if(state == ENABLE)
    {
        LCD_BL(0);
    }
    else if(state == DISABLE)
    {
        LCD_BL(1);
    }
}

void LCD_Rest(void)
{
    LCD_RST(0);
    lcd_delay_us(0xAFF);
    LCD_RST(1);
    lcd_delay_us(0xAFF);
}

void LCD_GetDevID(void)
{
    uint16_t tmp = 0;
    
    LCD_Write_Cmd(0xD3);
    tmp = LCD_Read_Data();
    tmp = LCD_Read_Data();
    tmp = LCD_Read_Data();
    tmp = tmp << 8;
    tmp |= LCD_Read_Data();
    
    lcddev.dev_id = tmp;
}

void LCD_Scan_Dir(uint8_t _dir)
{   
    LCD_Write_Cmd(lcddev.setx_cmd);    
    LCD_Write_Data(0);
    LCD_Write_Data(0);   
    LCD_Write_Data((lcddev.hor_res-1)>>8);
    LCD_Write_Data((lcddev.hor_res-1)&0xFF);   
    LCD_Write_Cmd(lcddev.sety_cmd);    
    LCD_Write_Data(0);
    LCD_Write_Data(0);   
    LCD_Write_Data((lcddev.ver_res-1)>>8);
    LCD_Write_Data((lcddev.ver_res-1)&0xFF);  

	//0x36命令参数的高3位可用于设置GRAM扫描方向	
    _dir = (_dir<<5) | 0x08;
	LCD_Write_Cmd(0x36); 
	LCD_Write_Data(_dir);//根据ucOption的值设置LCD参数，共0-7种模式    
}

void LCD_GRAM_Scan(uint8_t _opt)
{
    uint8_t tmp = _opt%2;
    if(_opt>7)
    {
        return;
    }
    lcddev.setx_cmd = 0x2A;
    lcddev.sety_cmd = 0x2B;
    lcddev.gram_cmd = 0x2C;
    lcddev.scan_mode = _opt;    // 记录扫描模式以便之后判断屏幕方向
    
    if(tmp==0)  // 0 2 4 6 竖屏
    {
        if(lcddev.dev_id == 0x9341)
        {
            lcddev.hor_res = 240;
            lcddev.ver_res = 320;
        }
        else if(lcddev.dev_id == 0x9488)
        {
            lcddev.hor_res = 320;
            lcddev.ver_res = 480;
        }
    }
    else if(tmp == 1) // 1 3 5 横屏
    {
        if(lcddev.dev_id == 0x9341)
        {
            lcddev.hor_res = 320;
            lcddev.ver_res = 240;
        }
        else if(lcddev.dev_id == 0x9488)
        {
            lcddev.hor_res = 480;
            lcddev.ver_res = 320;
        }
    }
    
    LCD_Scan_Dir(lcddev.scan_mode);
}

// 设置光标位置
void LCD_SetCursor(uint16_t x, uint16_t y)
{
	LCD_Write_Cmd(lcddev.setx_cmd);    
	LCD_Write_Data(x>>8);
    LCD_Write_Data(x&0xFF);         
	LCD_Write_Data((lcddev.hor_res-1)>>8);
    LCD_Write_Data((lcddev.hor_res-1)&0xFF);                         
               
	LCD_Write_Cmd(lcddev.sety_cmd);    
	LCD_Write_Data(y>>8);
    LCD_Write_Data(y&0xFF);         
	LCD_Write_Data((lcddev.ver_res-1)>>8);
    LCD_Write_Data((lcddev.ver_res-1)&0xFF); 
}

void LCD_Clear(uint16_t _color)
{
    //uint16_t color_t = 1;
    uint32_t i = 0;
    uint32_t totalpoint = lcddev.hor_res * lcddev.ver_res;
    
    lcd_color.backcolor = _color;
    LCD_SetCursor(0, 0);
    LCD_Write_Cmd(lcddev.gram_cmd);
    for(i=0;i<totalpoint;i++)
    {
        LCD_Write_Data(lcd_color.backcolor);
    }
}

void LCD_Fill(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end)
{
    uint16_t i,j;   
    uint16_t xlen = 0;    
       
    xlen = x_end - x_start + 1;       
    for(i=y_start; i<=y_end; i++)   
    {   
        LCD_SetCursor(x_start, i);                    
        LCD_Write_Cmd(lcddev.gram_cmd);                 
        for(j=0; j<xlen; j++)
        {
            LCD_Write_Data(lcd_color.backcolor);
        }                
    }
}

//在指定区域内填充指定颜色块			 
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Color_Fill(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t *color)
{  
    uint16_t i,j; 
    uint16_t height,width;     
    width = x_end - x_start + 1;          
    height = y_end - y_start + 1;   
    
    for(i=0; i<height; i++)   
    {   
        LCD_SetCursor(x_start, y_start+i);     
        LCD_Write_Cmd(lcddev.gram_cmd);      
        for(j=0; j<width; j++)
        {
            LCD_Write_Data(color[i*width+j]);
        }
    }  	  
} 

//m^n函数
//返回值:m^n次方.
uint32_t LCD_Pow(uint8_t m, uint8_t n)
{
	uint32_t result = 1;	 
	while(n--)result*=m;    
	return result;
}	

//画点
//x,y:坐标
//POINT_COLOR:此点的颜色
void LCD_DrawPoint(uint16_t  x, uint16_t y)
{
    LCD_SetCursor(x,y);      
    LCD_Write_Cmd(lcddev.gram_cmd);  
    LCD_Write_Data(lcd_color.textcolor);  
}
//快速画点
//x,y:坐标
//color:颜色
void LCD_Fast_DrawPoint(uint16_t x, uint16_t y, uint16_t color)
{	   
    LCD_Write_Cmd(lcddev.setx_cmd);    
    LCD_Write_Data(x>>8);
    LCD_Write_Data(x&0xFF);    
    LCD_Write_Data((lcddev.hor_res-1)>>8);
    LCD_Write_Data((lcddev.hor_res-1)&0xFF);  //j   
    LCD_Write_Cmd(lcddev.sety_cmd);    
    LCD_Write_Data(y>>8);
    LCD_Write_Data(y&0xFF);   
    LCD_Write_Data((lcddev.ver_res-1)>>8);
    LCD_Write_Data((lcddev.ver_res-1)&0xFF); //j   
    LCD_Write_Cmd(lcddev.gram_cmd);    
    LCD_Write_Data(color);    
}	

//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(uint16_t x, uint16_t  y, uint8_t num, uint8_t size, uint8_t mode)
{  							  
    uint8_t temp, t1, t;
	uint16_t y0 = y;
	uint8_t csize = (size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	
 	num=num-' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp = asc2_1206[num][t]; 	 	//调用1206字体
		else if(size==16)temp = asc2_1608[num][t];	//调用1608字体
		else if(size==24)temp = asc2_2412[num][t];	//调用2412字体
		else return;								//没有的字库
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_Fast_DrawPoint(x, y, lcd_color.textcolor);
			else if(mode==0)LCD_Fast_DrawPoint(x, y, lcd_color.backcolor);
            
			temp <<= 1;
			y++;
            
			if(y>=lcddev.ver_res)return;		//超区域了
			if((y-y0)==size)
			{
				y = y0;
				x++;
				if(x>=lcddev.hor_res)return;	//超区域了
				break;
			}
		}  	 
	}  	    	   	 	  
}   

//显示数字,高位为0,则不显示
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色 
//num:数值(0~4294967295);	 
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t = 0, temp = 0;
	uint8_t enshow = 0;						   
	for(t=0; t<len; t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t, y, ' ', size, 0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t, y, temp+'0', size, 0); 
	}
} 

//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);	 
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void LCD_ShowxNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t mode)
{  
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0; t<len; t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0 && t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 
//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址		  
void LCD_ShowString(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, uint8_t *p)
{         
	uint8_t x0 = x;
	width += x;
	height += y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//退出
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}

//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
} 

void LCD_DrawCross(uint16_t usX, uint16_t usY)
{
	LCD_DrawLine(usX-10,usY,usX+10,usY);
	LCD_DrawLine(usX, usY - 10, usX, usY+10);	
}

/********** LCD初始化函数定义 **********/
void LCD_Init(void)
{
    LCD_BackLed_Ctrl(ENABLE);
    LCD_Rest();
    LCD_GetDevID();
    
    if(lcddev.dev_id == 0x9341)
    {
        ILI9341_Init();
    }
    else if(lcddev.dev_id == 0x9488)
    {
        ILI9488_Init();
    }
}

