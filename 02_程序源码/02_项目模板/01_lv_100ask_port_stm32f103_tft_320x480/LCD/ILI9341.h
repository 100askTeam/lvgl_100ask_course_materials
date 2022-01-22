#ifndef __ILI9341_H
#define __ILI9341_H

#include "main.h"

/******************************* 定义 ILI934 常用命令 ********************************/
#define     CMD_SetCoordinateX		 		    0x2A	     //设置X坐标
#define     CMD_SetCoordinateY		 		    0x2B	     //设置Y坐标
#define     CMD_SetPixel		 		        0x2C	     //填充像素
        
/*************************** ILI934 显示区域的起始坐标和总行列数 *********************/
#define     ILI9341_DispWindow_X_Star		    0            //起始点的X坐标
#define     ILI9341_DispWindow_Y_Star		    0            //起始点的Y坐标
        
#define     ILI9341_LESS_PIXEL                  240			 //液晶屏较短方向的像素宽度
#define     ILI9341_MORE_PIXEL                  320			 //液晶屏较长方向的像素宽度


extern void ILI9341_Init(void);


#endif /* __ILI9341_H */

