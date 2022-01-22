#include <stdio.h>
#include "XPT2046.h"
#include "lcd_function.h"
#include "driver_w25qxx.h"
/********** 全局变量定义 **********/

/********** 局部变量定义 **********/
//static _touch_calibration      touch_calibration = {0};
_touch_para                    touch_para[6] = {0};
//static _touch_xy               touch_xy = {0};

static void         XPT2046_WriteCMD(uint8_t ucCmd);
static uint16_t     XPT2046_ReadCMD(void);
static uint16_t     XPT2046_ReadAdc(uint8_t ucChannel);
static void         XPT2046_ReadAdc_XY(int16_t * sX_Ad, int16_t * sY_Ad);
#if 0
static uint8_t      XPT2046_ReadAdc_Smooth_XY(_touch_xy * pScreenCoordinate);
#else
static uint8_t XPT2046_ReadAdc_Smooth_XY_qucik( _touch_xy * pScreenCoordinate );
#endif

static uint8_t      XPT2046_Calculate_CalibrationFactor(_touch_xy * pDisplayCoordinate, \
                                                        _touch_xy * pScreenSample, \
                                                        _touch_calibration * pCalibrationFactor);       
//static uint8_t      XPT2046_Touch_Calibrate(uint8_t LCD_Mode);


/********** XPT2046初始化函数定义 **********/
void XPT2046_Init(void)
{
    uint32_t para_save_addr = 0;
    uint8_t ret = 0;
    GPIO_InitTypeDef  GPIO_InitStructure;
    /* 开启GPIO时钟 */    
    __HAL_RCC_GPIOE_CLK_ENABLE();     
    __HAL_RCC_GPIOF_CLK_ENABLE();    

    /* 模拟SPI GPIO初始化 */          
    GPIO_InitStructure.Pin=XPT2046_SPI_CLK_PIN;
    GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH ;	  
    GPIO_InitStructure.Mode=GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(XPT2046_SPI_CLK_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = XPT2046_SPI_MOSI_PIN;
    HAL_GPIO_Init(XPT2046_SPI_MOSI_PORT, &GPIO_InitStructure);


    GPIO_InitStructure.Pin = XPT2046_SPI_CS_PIN; 
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH ;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;      
    HAL_GPIO_Init(XPT2046_SPI_CS_PORT, &GPIO_InitStructure); 


    GPIO_InitStructure.Pin = XPT2046_SPI_MISO_PIN; 
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH ;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;  //上拉输入
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(XPT2046_SPI_MISO_PORT, &GPIO_InitStructure);

    /* 拉高片选，暂时先不选择XPT2046 */
    XPT2046_CS_DISABLE();		

    //触摸屏触摸信号指示引脚，不使用中断
    GPIO_InitStructure.Pin = XPT2046_PENIRQ_GPIO_PIN;       
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;  //上拉输入
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(XPT2046_PENIRQ_GPIO_PORT, &GPIO_InitStructure);
    
    
    if(lcddev.dev_id == 0x9341)
    {
        para_save_addr = ILI9341_PARA_FLASH_ADDR;
        FLASH_BufferRead((uint8_t*)touch_para, ILI9341_PARA_FLASH_ADDR, sizeof(touch_para));
    }
    if(lcddev.dev_id == 0x9488)
    {
        para_save_addr = ILI9488_PARA_FLASH_ADDR;
        FLASH_BufferRead((uint8_t*)touch_para, ILI9488_PARA_FLASH_ADDR, sizeof(touch_para));
    }
    
    if((touch_para[lcddev.scan_mode].para_flag != FINISHED) || (touch_para[lcddev.scan_mode].para_force == FORCED))
    {
        // 如果当前扫描方式下未成校准过则进行校准
        while(ret==0)
        {
            touch_para[lcddev.scan_mode].para_flag = NOT_FINISHED;
            ret = XPT2046_Touch_Calibrate(lcddev.scan_mode);
        }
        touch_para[lcddev.scan_mode].para_flag = FINISHED;
        touch_para[lcddev.scan_mode].para_force = NOT_FORCED;
        FLASH_SectorErase(para_save_addr);
        FLASH_BufferWrite((uint8_t*)touch_para, para_save_addr, sizeof(touch_para));
    }
}

/**
  * @brief  XPT2046 的写入命令
  * @param  ucCmd ：命令
  *   该参数为以下值之一：
  *     @arg 0x90 :通道Y+的选择控制字
  *     @arg 0xd0 :通道X+的选择控制字
  * @retval 无
  */
static void XPT2046_WriteCMD(uint8_t ucCmd)
{
    uint8_t i;
    XPT2046_MOSI_0();	
    XPT2046_CLK_LOW();
    for(i = 0; i < 8; i ++)
    {
        if((ucCmd >> (7 - i)) & 0x01)
        {
            XPT2046_MOSI_1();
        }
        else
        {
            XPT2046_MOSI_0();
        }
        lcd_delay_us(5);
        XPT2046_CLK_HIGH();
        lcd_delay_us(5);
        XPT2046_CLK_LOW();
    }
	
}
/**
  * @brief  XPT2046 的读取命令
  * @param  无
  * @retval 读取到的数据
  */
static uint16_t XPT2046_ReadCMD(void)
{
	uint8_t i;
	uint16_t usBuf=0, usTemp;
	XPT2046_MOSI_0();
	XPT2046_CLK_HIGH();
	for(i=0;i<12;i++)
	{
		XPT2046_CLK_LOW();    
		usTemp = XPT2046_MISO();
		usBuf |= usTemp << (11 - i);
		XPT2046_CLK_HIGH();
	}
	return usBuf;
}


/**
  * @brief  对 XPT2046 选择一个模拟通道后，启动ADC，并返回ADC采样结果
  * @param  ucChannel
  *   该参数为以下值之一：
  *     @arg 0x90 :通道Y+的选择控制字
  *     @arg 0xd0 :通道X+的选择控制字
  * @retval 该通道的ADC采样结果
  */
static uint16_t XPT2046_ReadAdc(uint8_t ucChannel)
{
	XPT2046_WriteCMD(ucChannel);

  return 	XPT2046_ReadCMD();
	
}
/**
  * @brief  读取 XPT2046 的X通道和Y通道的AD值（12 bit，最大是4096）
  * @param  sX_Ad ：存放X通道AD值的地址
  * @param  sY_Ad ：存放Y通道AD值的地址
  * @retval 无
  */
static void XPT2046_ReadAdc_XY(int16_t * sX_Ad, int16_t * sY_Ad)
{ 
	int16_t sX_Ad_Temp, sY_Ad_Temp; 
	
	sX_Ad_Temp = XPT2046_ReadAdc(XPT2046_CHANNEL_X);

	lcd_delay_us(1); 

	sY_Ad_Temp = XPT2046_ReadAdc(XPT2046_CHANNEL_Y); 
	
	
	* sX_Ad = sX_Ad_Temp; 
	* sY_Ad = sY_Ad_Temp; 
}

 
#if 0
/**
  * @brief  在触摸 XPT2046 屏幕时获取一组坐标的AD值，并对该坐标进行滤波
  * @param  无
  * @retval 滤波之后的坐标AD值
  */
 //注意：校正较精准，但是相对复杂，速度较慢
static uint8_t XPT2046_ReadAdc_Smooth_XY(_touch_xy * pScreenCoordinate)
{
	uint8_t ucCount = 0;
	
	int16_t sAD_X, sAD_Y;
	int16_t sBufferArray[2][9] = {{0}, {0}};  //坐标X和Y进行9次采样

	int32_t lAverage[3], lDifference[3];

	do
	{		   
		XPT2046_ReadAdc_XY(& sAD_X, & sAD_Y);
		
		sBufferArray[0][ucCount] = sAD_X;  
		sBufferArray[1][ucCount] = sAD_Y;
		
		ucCount ++; 
			 
	} while( (XPT2046_PENIRQ_Read() == XPT2046_PENIRQ_ActiveLevel) && (ucCount < 9)); 	//用户点击触摸屏时即TP_INT_IN信号为低 并且 ucCount<9*/
	 
	
	
	/* 如果成功采样9次,进行滤波 */ 
	if(ucCount == 9)
	{  
		/* 为减少运算量,分别分3组取平均值 */
		lAverage[0] = (sBufferArray[0][0] + sBufferArray[0][1] + sBufferArray[0][2]) / 3;
		lAverage[1] = (sBufferArray[0][3] + sBufferArray[0][4] + sBufferArray[0][5]) / 3;
		lAverage[2] = (sBufferArray[0][6] + sBufferArray[0][7] + sBufferArray[0][8]) / 3;
		
		/* 计算3组数据的差值 */
		lDifference [0] = lAverage[0] - lAverage[1];
		lDifference [1] = lAverage[1] - lAverage[2];
		lDifference [2] = lAverage[2] - lAverage[0];
		
		/* 对上述差值取绝对值 */
		lDifference[0] = lDifference[0] >0 ? lDifference[0] : (-lDifference[0]);
		lDifference[1] = lDifference[1] >0 ? lDifference[1] : (-lDifference[1]);
		lDifference[2] = lDifference[2] >0 ? lDifference[2] : (-lDifference[2]);
		
		
		/* 判断绝对差值是否都超过差值门限，如果这3个绝对差值都超过门限值，则判定这次采样点为野点,抛弃采样点，差值门限取为2 */
		if( (lDifference[0] > XPT2046_THRESHOLD_CalDiff) \
            && (lDifference[1] > XPT2046_THRESHOLD_CalDiff) \
            && (lDifference[2] > XPT2046_THRESHOLD_CalDiff) ) 
        {
            return 0;
        }
		
		
		/* 计算它们的平均值，同时赋值给strScreenCoordinate */ 
		if(lDifference[0] < lDifference[1])
		{
			if(lDifference[2] < lDifference[0]) 
            {
				pScreenCoordinate->x = (lAverage[0] + lAverage[2]) / 2;
            }
			else
            {                
				pScreenCoordinate->x = (lAverage[0] + lAverage[1]) / 2;	
            }
		}
		else if(lDifference[2] < lDifference[1]) 
        {
            pScreenCoordinate->x = (lAverage[0] + lAverage[2]) / 2;
        }                                                  
                                                           
		else                                               
        {                                                  
            pScreenCoordinate->x = (lAverage[1] + lAverage[2]) / 2;
        }
		
		
		/* 同上，计算Y的平均值 */
		lAverage[0] = (sBufferArray[1][0] + sBufferArray[1][1] + sBufferArray[1][2]) / 3;
		lAverage[1] = (sBufferArray[1][3] + sBufferArray[1][4] + sBufferArray[1][5]) / 3;
		lAverage[2] = (sBufferArray[1][6] + sBufferArray[1][7] + sBufferArray[1][8]) / 3;
		
		lDifference[0] = lAverage[0] - lAverage[1];
		lDifference[1] = lAverage[1] - lAverage[2];
		lDifference[2] = lAverage[2] - lAverage[0];
		
		/* 取绝对值*/
		lDifference[0] = lDifference[0] >0 ? lDifference[0] : (-lDifference[0]);
		lDifference[1] = lDifference[1] >0 ? lDifference[1] : (-lDifference[1]);
		lDifference[2] = lDifference[2] >0 ? lDifference[2] : (-lDifference[2]);
		
		
		if( (lDifference[0] > XPT2046_THRESHOLD_CalDiff) \
            && (lDifference[1] > XPT2046_THRESHOLD_CalDiff) \
            && (lDifference[2] > XPT2046_THRESHOLD_CalDiff) ) 
        {
            return 0;
        }
		
		if(lDifference[0] < lDifference[1])
		{
			if(lDifference[2] < lDifference[0]) 
            {
                pScreenCoordinate->y = (lAverage[0] + lAverage[2]) / 2;
            }
			else 
            {
                pScreenCoordinate->y = (lAverage[0] + lAverage[1]) / 2;	
            }
		}
		else if(lDifference[2] < lDifference[1]) 
        {
            pScreenCoordinate->y =  (lAverage[0] + lAverage[2]) / 2;
        }
		else
        {
            pScreenCoordinate->y =  (lAverage[1] + lAverage[2]) / 2;
        }
				
		return 1;		
	}
	
	else if(ucCount > 1)
	{
		pScreenCoordinate->x = sBufferArray[0][0];
		pScreenCoordinate->y = sBufferArray[1][0];	
		return 0;		
	}  	
	return 0; 	
}
#else
static uint8_t XPT2046_ReadAdc_Smooth_XY_qucik( _touch_xy * pScreenCoordinate )
{
	uint8_t ucCount = 0, i;
	
	int16_t sAD_X, sAD_Y;
	int16_t sBufferArray [ 2 ] [ 10 ] = { { 0 },{ 0 } };  //坐标X和Y进行多次采样
	
	//存储采样中的最小值、最大值
	int32_t lX_Min, lX_Max, lY_Min, lY_Max;


	/* 循环采样10次 */ 
	do					       				
	{		  
		XPT2046_ReadAdc_XY ( & sAD_X, & sAD_Y );  
		
		sBufferArray [ 0 ] [ ucCount ] = sAD_X;  
		sBufferArray [ 1 ] [ ucCount ] = sAD_Y;
		
		ucCount ++;  
		
	}	while ( ( XPT2046_PENIRQ_Read() == XPT2046_PENIRQ_ActiveLevel ) && ( ucCount < 10 ) );//用户点击触摸屏时即TP_INT_IN信号为低 并且 ucCount<10

	
	/*如果成功采样10个样本*/
	if ( ucCount ==10 )		 					
	{
		lX_Max = lX_Min = sBufferArray [ 0 ] [ 0 ];
		lY_Max = lY_Min = sBufferArray [ 1 ] [ 0 ];       
		
		for ( i = 1; i < 10; i ++ )
		{
			if ( sBufferArray[ 0 ] [ i ] < lX_Min )
				lX_Min = sBufferArray [ 0 ] [ i ];
			
			else if ( sBufferArray [ 0 ] [ i ] > lX_Max )
				lX_Max = sBufferArray [ 0 ] [ i ];

		}
		
		for ( i = 1; i < 10; i ++ )
		{
			if ( sBufferArray [ 1 ] [ i ] < lY_Min )
				lY_Min = sBufferArray [ 1 ] [ i ];
			
			else if ( sBufferArray [ 1 ] [ i ] > lY_Max )
				lY_Max = sBufferArray [ 1 ] [ i ];

		}
		
		
		/*去除最小值和最大值之后求平均值*/
		pScreenCoordinate ->x =  ( sBufferArray [ 0 ] [ 0 ] + sBufferArray [ 0 ] [ 1 ] + sBufferArray [ 0 ] [ 2 ] + sBufferArray [ 0 ] [ 3 ] + sBufferArray [ 0 ] [ 4 ] + 
		                           sBufferArray [ 0 ] [ 5 ] + sBufferArray [ 0 ] [ 6 ] + sBufferArray [ 0 ] [ 7 ] + sBufferArray [ 0 ] [ 8 ] + sBufferArray [ 0 ] [ 9 ] - lX_Min-lX_Max ) >> 3;
		
		pScreenCoordinate ->y =  ( sBufferArray [ 1 ] [ 0 ] + sBufferArray [ 1 ] [ 1 ] + sBufferArray [ 1 ] [ 2 ] + sBufferArray [ 1 ] [ 3 ] + sBufferArray [ 1 ] [ 4 ] + 
		                           sBufferArray [ 1 ] [ 5 ] + sBufferArray [ 1 ] [ 6 ] + sBufferArray [ 1 ] [ 7 ] + sBufferArray [ 1 ] [ 8 ] + sBufferArray [ 1 ] [ 9 ] - lY_Min-lY_Max ) >> 3; 
		
		
		return 1;		
		
	}   	
	return 0;    	
}
#endif

/**
  * @brief  计算 XPT2046 触摸坐标校正系数（注意：只有在LCD和触摸屏间的误差角度非常小时,才能运用下面公式）
  * @param  pDisplayCoordinate ：屏幕人为显示的已知坐标
  * @param  pstrScreenSample ：对已知坐标点触摸时 XPT2046 产生的坐标
  * @param  pCalibrationFactor ：根据人为设定坐标和采样回来的坐标计算出来的屏幕触摸校正系数
  * @retval 计算状态
	*   该返回值为以下值之一：
  *     @arg 1 :计算成功
  *     @arg 0 :计算失败
  */
static uint8_t XPT2046_Calculate_CalibrationFactor(_touch_xy * pDisplayCoordinate, _touch_xy * pScreenSample, _touch_calibration * pCalibrationFactor)
{
	uint8_t ucRet = 1;

	/* K＝ ( X0－X2 )  ( Y1－Y2 )－ ( X1－X2 )  ( Y0－Y2 ) */
	pCalibrationFactor->Divider =  ( (pScreenSample[0].x - pScreenSample[2].x ) * (pScreenSample[1].y - pScreenSample[2].y))\
                                  -( (pScreenSample[1].x - pScreenSample[2].x ) * (pScreenSample[0].y - pScreenSample[ 2].y) ) ;
	
	
	if(pCalibrationFactor->Divider == 0)
    {
        ucRet = 0;
    }

	else
	{
		/* A＝ (  ( XD0－XD2 )  ( Y1－Y2 )－ ( XD1－XD2 )  ( Y0－Y2 ) )／K	*/
		pCalibrationFactor->An = ( (pDisplayCoordinate[0].x - pDisplayCoordinate[2].x) * (pScreenSample[1].y - pScreenSample[2].y) )\
                               - ( (pDisplayCoordinate[1].x - pDisplayCoordinate[2].x) * (pScreenSample[0].y - pScreenSample[2].y) );
		
		/* B＝ (  ( X0－X2 )  ( XD1－XD2 )－ ( XD0－XD2 )  ( X1－X2 ) )／K	*/
		pCalibrationFactor->Bn =  ( (pScreenSample[0].x - pScreenSample[2].x) * (pDisplayCoordinate[1].x - pDisplayCoordinate[2] .x) )\
                                - ( (pDisplayCoordinate[0].x - pDisplayCoordinate[2].x) * (pScreenSample[1].x - pScreenSample[2] .x) );
		
		/* C＝ ( Y0 ( X2XD1－X1XD2 )+Y1 ( X0XD2－X2XD0 )+Y2 ( X1XD0－X0XD1 ) )／K */
		pCalibrationFactor->Cn =  ( pScreenSample[2].x * pDisplayCoordinate[1].x - pScreenSample[1].x * pDisplayCoordinate[2].x ) * pScreenSample[0].y \
                                + ( pScreenSample[0].x * pDisplayCoordinate[2].x - pScreenSample[2].x * pDisplayCoordinate[0].x ) * pScreenSample[1].y \
                                + ( pScreenSample[1].x * pDisplayCoordinate[0].x - pScreenSample[0].x * pDisplayCoordinate[1].x ) * pScreenSample[2].y ;
		
		/* D＝ (  ( YD0－YD2 )  ( Y1－Y2 )－ ( YD1－YD2 )  ( Y0－Y2 ) )／K	*/
		pCalibrationFactor->Dn =  ( (pDisplayCoordinate[0].y - pDisplayCoordinate[2].y) * (pScreenSample[1].y - pScreenSample[2].y) )\
                                - ( (pDisplayCoordinate[1].y - pDisplayCoordinate[2].y) * (pScreenSample[0].y - pScreenSample[2].y) ) ;
		
		/* E＝ (  ( X0－X2 )  ( YD1－YD2 )－ ( YD0－YD2 )  ( X1－X2 ) )／K	*/
		pCalibrationFactor->En =  ( (pScreenSample[0].x - pScreenSample[2].x ) * (pDisplayCoordinate[1].y - pDisplayCoordinate[2].y) ) \
                                - ( (pDisplayCoordinate[0].y - pDisplayCoordinate[2].y) * (pScreenSample[1].x - pScreenSample[2].x) ) ;
		
		
		/* F＝ ( Y0 ( X2YD1－X1YD2 )+Y1 ( X0YD2－X2YD0 )+Y2 ( X1YD0－X0YD1 ) )／K */
		pCalibrationFactor->Fn =  ( pScreenSample[2].x * pDisplayCoordinate[1].y - pScreenSample[1].x * pDisplayCoordinate[2].y ) * pScreenSample[0].y \
                                + ( pScreenSample[0].x * pDisplayCoordinate[2].y - pScreenSample[2].x * pDisplayCoordinate[0].y ) * pScreenSample[1].y \
                                + ( pScreenSample[1].x * pDisplayCoordinate[0].y - pScreenSample[0].x * pDisplayCoordinate[1].y ) * pScreenSample[2].y;
			
	}
	
	return ucRet;
}

/**
  * @brief  XPT2046 触摸屏校准
	* @param	LCD_Mode：指定要校正哪种液晶扫描模式的参数
  * @note  本函数调用后会把液晶模式设置为LCD_Mode
  * @retval 校准结果
	*   该返回值为以下值之一：
  *     @arg 1 :校准成功
  *     @arg 0 :校准失败
  */
#if 1
uint8_t XPT2046_Touch_Calibrate(uint8_t LCD_Mode) 
{
    uint8_t i;

    //char cStr [ 100 ];

    uint16_t usTest_x = 0, usTest_y = 0, usGap_x = 0, usGap_y = 0;

    char * pStr = 0;

    _touch_xy strCrossCoordinate[4], strScreenSample[4];

    _touch_calibration CalibrationFactor;
        
    LCD_SetAllColor(BLACK, BLUE);

    //设置扫描方向，横屏
    LCD_GRAM_Scan(LCD_Mode);


    /* 设定“十”字交叉点的坐标 */ 
    strCrossCoordinate [0].x = lcddev.hor_res >> 2;
    strCrossCoordinate[0].y = lcddev.ver_res >> 2;

    strCrossCoordinate[1].x = strCrossCoordinate[0].x;
    strCrossCoordinate[1].y = (lcddev.ver_res * 3) >> 2;

    strCrossCoordinate[2].x = (lcddev.hor_res * 3) >> 2;
    strCrossCoordinate[2].y = strCrossCoordinate[1].y;

    strCrossCoordinate[3].x = strCrossCoordinate[2].x;
    strCrossCoordinate[3].y = strCrossCoordinate[0].y;	

    XPT2046_CS_ENABLE();
    for (i = 0; i < 4; i ++)
    { 
        LCD_Fill(0, 0, lcddev.hor_res, lcddev.ver_res);       
        
        pStr = "Touch Calibrate ......";		
        //插入空格，居中显示
        LCD_ShowString((lcddev.hor_res>>1), ((lcddev.ver_res >> 1)-(sizeof(pStr)<<1)), 200, 12, 12, (uint8_t*)pStr);	
        lcd_delay_ms(100);		                     //适当的延时很有必要
        LCD_DrawCross(strCrossCoordinate[i].x, strCrossCoordinate[i].y);  //显示校正用的“十”字

        while(!XPT2046_ReadAdc_Smooth_XY_qucik(&strScreenSample[i]));               //读取XPT2046数据到变量pCoordinate，当ptr为空时表示没有触点被按下
    }
    XPT2046_CS_DISABLE();
      
    XPT2046_Calculate_CalibrationFactor(strCrossCoordinate, strScreenSample, & CalibrationFactor) ;  	 //用原始参数计算出 原始参数与坐标的转换系数

    if(CalibrationFactor.Divider == 0)  goto Failure;

        
    usTest_x = ( ( CalibrationFactor.An * strScreenSample[3].x ) + ( CalibrationFactor.Bn * strScreenSample[3].y ) + CalibrationFactor.Cn ) / CalibrationFactor.Divider;		//取一个点计算X值	 
    usTest_y = ( ( CalibrationFactor.Dn * strScreenSample[3].x ) + ( CalibrationFactor.En * strScreenSample[3].y ) + CalibrationFactor.Fn ) / CalibrationFactor.Divider;    //取一个点计算Y值

    usGap_x = ( usTest_x > strCrossCoordinate[3].x ) ? ( usTest_x - strCrossCoordinate[3].x ) : ( strCrossCoordinate[3].x - usTest_x );   //实际X坐标与计算坐标的绝对差
    usGap_y = ( usTest_y > strCrossCoordinate[3].y ) ? ( usTest_y - strCrossCoordinate[3].y ) : ( strCrossCoordinate[3].y - usTest_y );   //实际Y坐标与计算坐标的绝对差

    if ( ( usGap_x > 15 ) || ( usGap_y > 15 ) ) goto Failure;       //可以通过修改这两个值的大小来调整精度    


    /* 校准系数为全局变量 */ 
    touch_para[LCD_Mode].dX_X = ( CalibrationFactor.An * 1.0 ) / CalibrationFactor.Divider;
    touch_para[LCD_Mode].dX_Y = ( CalibrationFactor.Bn * 1.0 ) / CalibrationFactor.Divider;
    touch_para[LCD_Mode].dX   = ( CalibrationFactor.Cn * 1.0 ) / CalibrationFactor.Divider;

    touch_para[LCD_Mode].dY_X = ( CalibrationFactor.Dn * 1.0 ) / CalibrationFactor.Divider;
    touch_para[LCD_Mode].dY_Y = ( CalibrationFactor.En * 1.0 ) / CalibrationFactor.Divider;
    touch_para[LCD_Mode].dY   = ( CalibrationFactor.Fn * 1.0 ) / CalibrationFactor.Divider;

#if 0	//输出调试信息，注意要先初始化串口
        {
                float * ulHeadAddres ;

                /* 打印校校准系数 */ 
                printf("显示模式【%d】校准系数如下：", LCD_Mode);
                
                ulHeadAddres = ( float* ) ( & touch_para[LCD_Mode] );
                
                for ( i = 0; i < 6; i ++ )
                {					
                    printf ( "%12f,", *ulHeadAddres++  );			
                }	
                printf("\r\n");
        }
#endif	

    LCD_Clear(lcd_color.backcolor);

    LCD_SetTextColor(GREEN);

    pStr = "Calibrate Succed";
    //插入空格，居中显示	
    LCD_ShowString((lcddev.hor_res>>1)-(sizeof(pStr)<<1) , lcddev.ver_res >> 1, 200, 12, 12, (uint8_t*)pStr);
    lcd_delay_ms(1000);

    return 1;    


Failure:

    LCD_Clear(lcd_color.backcolor);
    LCD_SetTextColor(RED);

    pStr = "Calibrate fail";	
    //插入空格，居中显示	
    LCD_ShowString((lcddev.hor_res>>1)-(sizeof(pStr)<<1) , lcddev.ver_res >> 1, 200, 12, 12, (uint8_t*)pStr);

    pStr = "try again";
    //插入空格，居中显示		
    LCD_ShowString(((lcddev.hor_res>>1)+100)-(sizeof(pStr)<<1) , (lcddev.ver_res+100) >> 1, 200, 12, 12, (uint8_t*)pStr);			

    lcd_delay_ms(1000);
    return 0; 
}
#endif

/**
  * @brief  获取 XPT2046 触摸点（校准后）的坐标
  * @param  pDisplayCoordinate ：该指针存放获取到的触摸点坐标
  * @param  pTouchPara：坐标校准系数
  * @retval 获取情况
	*   该返回值为以下值之一：
  *     @arg 1 :获取成功
  *     @arg 0 :获取失败
  */
uint8_t XPT2046_Get_TouchedPoint(_touch_xy *pDisplayCoordinate, _touch_para *pTouchPara )
{
	uint8_t ucRet = 1;           //若正常，则返回0
	
	_touch_xy strScreenCoordinate; 

    if(XPT2046_ReadAdc_Smooth_XY_qucik(&strScreenCoordinate))
    {    
        pDisplayCoordinate->x = ( (pTouchPara[lcddev.scan_mode].dX_X * strScreenCoordinate.x) + (pTouchPara[lcddev.scan_mode].dX_Y * strScreenCoordinate.y) + pTouchPara[lcddev.scan_mode].dX );        
        pDisplayCoordinate->y = ( (pTouchPara[lcddev.scan_mode].dY_X * strScreenCoordinate.x) + (pTouchPara[lcddev.scan_mode].dY_Y * strScreenCoordinate.y) + pTouchPara[lcddev.scan_mode].dY );
    }
	 
    ucRet = 1;            //如果获取的触点信息有误，则返回0
		
  return ucRet;
} 

/**
  * @brief  触摸屏检测状态机
  * @retval 触摸状态
	*   该返回值为以下值之一：
  *     @arg TOUCH_PRESSED :触摸按下
  *     @arg TOUCH_DET_NOT_PRESSED :无触摸
  */
uint8_t XPT2046_TouchDetect(void)
{ 
	static _touch_state touch_state = TOUCH_RELEASE;
	static uint32_t i;
	uint8_t detectResult = TOUCH_DET_NOT_PRESSED;
	
	switch(touch_state)
	{
		case TOUCH_RELEASE:	
			if(XPT2046_PENIRQ_Read() == XPT2046_PENIRQ_ActiveLevel) //第一次出现触摸信号
			{
				touch_state = TOUCH_WAITING;
				detectResult =TOUCH_DET_NOT_PRESSED;
				}
			else	//无触摸
			{
				touch_state = TOUCH_RELEASE;
				detectResult =TOUCH_DET_NOT_PRESSED;
			}
			break;
				
		case TOUCH_WAITING:
				if(XPT2046_PENIRQ_Read() == XPT2046_PENIRQ_ActiveLevel)
				{
					 i++;
					//等待时间大于阈值则认为触摸被按下
					//消抖时间 = DURIATION_TIME * 本函数被调用的时间间隔
					//如在定时器中调用，每10ms调用一次，则消抖时间为：DURIATION_TIME*10ms
					if(i > DURIATION_TIME)		
					{
						i=0;
						touch_state = TOUCH_PRESSED;
						detectResult = TOUCH_DET_PRESSED;
					}
					else												//等待时间累加
					{
						touch_state = TOUCH_WAITING;
						detectResult =	 TOUCH_DET_NOT_PRESSED;					
					}
				}
				else	//等待时间值未达到阈值就为无效电平，当成抖动处理					
				{
				    i = 0;
                    touch_state = TOUCH_RELEASE; 
                    detectResult = TOUCH_DET_NOT_PRESSED;
				}
		
			break;
		
		case TOUCH_PRESSED:	
				if(XPT2046_PENIRQ_Read() == XPT2046_PENIRQ_ActiveLevel)		//触摸持续按下
				{
					touch_state = TOUCH_PRESSED;
					detectResult = TOUCH_DET_PRESSED;
				}
				else	//触摸释放
				{
					touch_state = TOUCH_RELEASE;
					detectResult = TOUCH_DET_NOT_PRESSED;
				}
			break;			
		
		default:
				touch_state = TOUCH_RELEASE;
				detectResult = TOUCH_DET_NOT_PRESSED;
				break;
	
	}		
	
	return detectResult;
}


bool xpt2046_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    uint8_t ret = 0;
    static int16_t last_x = 0;
    static int16_t last_y = 0;

    _touch_xy strScreenSample;
    
    if(XPT2046_TouchDetect() == TOUCH_DET_PRESSED)
    {
        XPT2046_CS_ENABLE();
        ret = XPT2046_Get_TouchedPoint(&strScreenSample, touch_para);
        XPT2046_CS_DISABLE();

        if(ret==1)
        {
            last_x = strScreenSample.x;
            last_y = strScreenSample.y;
            
            data->point.x = last_x;
            data->point.y = last_y;
            data->state = LV_INDEV_STATE_PR;
        }
    }
    else
    {
        data->point.x = last_x;
        data->point.y = last_y;
		data->state = LV_INDEV_STATE_REL;
    }
    
    return false;
}

