#ifndef __OLED_H
#define __OLED_H 



#include "include.h"
#include "int.h"

 extern uint8_t lanzhou96x64[768];
 void OLED_Init(void);
 void OLED_CLS(void);
 void OLED_P6x8Str(uint8_t x,uint8_t y,uint8_t ch[]);
 void OLED_P8x16Str(uint8_t x,uint8_t y,uint8_t ch[]);
 void OLED_P14x16Str(uint8_t x,uint8_t y,uint8_t ch[]);
 void OLED_HEXACSII(uint16_t hex,uint8_t* Print);
 void OLED_Print_Num(uint8_t x, uint8_t y, uint16_t num);
 void OLED_Print_Str(uint8_t x, uint8_t y, uint8_t ch[]);
 void OLED_PutPixel(uint8_t x,uint8_t y);
 void OLED_Rectangle(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t gif);
 void OLED_Set_Pos(uint8_t x, uint8_t y);
 void OLED_WrDat(uint8_t data);
 void Draw_LibLogo(void);
 void Draw_Landzo(void);
 void Draw_BMP(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t bmp[]);
 void OLED_Fill(uint8_t dat);
 void Dly_ms(uint16_t ms);
 void OLED_Wr6Dat(uint8_t data);
 
 void Draw_Landzo(void);
 void OLED_DLY_ms(uint16_t ms);
 void OLED_Rectangle(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t gif);
/*
4线SPI使用说明：
VBT 供内部DC-DC电压，3.3~4.3V，如果使用5V电压，为保险起见串一个100~500欧的电阻
VCC 供内部逻辑电压 1.8~6V   
GND 地

BS0 低电平
BS1 低电平
BS2 低电平

CS  片选管脚
DC  命令数据选择管脚
RES 模块复位管脚 
D0（SCLK） ，时钟脚，由MCU控制
D1（MOSI） ，主输出从输入数据脚，由MCU控制

D2 悬空      
D3-D7 ， 低电平 ， 也可悬空，但最好设为低电平
RD  低电平 ，也可悬空，但最好设为低电平
RW  低电平 ，也可悬空，但最好设为低电平
RD  低电平 ，也可悬空，但最好设为低电平   
*/

/************************************************************************
OLED 一行可以显示16个字符，可以显示8行。
************************************************************************/
#define OLED_SCL  PTE1_OUT   //        D0
#define OLED_SDA  PTE2_OUT   //        D1
#define OLED_RST  PTE3_OUT   //       RST
#define OLED_DC   PTE4_OUT   //       DS
#define OLED_CS   PTE6_OUT   //       CS

#define XLevelL		0x00 
#define XLevelH		0x10
#define XLevel		((XLevelH&0x0F)*16+XLevelL)
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xCF

/*
4线SPI使用说明：
VBT 供内部DC-DC电压，3.3~4.3V，如果使用5V电压，为保险起见串一个100~500欧的电阻
VCC 供内部逻辑电压 1.8~6V
GND 地

BS0 低电平
BS1 低电平
BS2 低电平

CS  片选管脚
DC  命令数据选择管脚
RES 模块复位管脚
D0（SCLK） ，时钟脚，由MCU控制
D1（MOSI） ，主输出从输入数据脚，由MCU控制

D2 悬空
D3-D7 ， 低电平 ， 也可悬空，但最好设为低电平
RD  低电平 ，也可悬空，但最好设为低电平
RW  低电平 ，也可悬空，但最好设为低电平
RD  低电平 ，也可悬空，但最好设为低电平
*/

#define X_WIDTH 128
#define Y_WIDTH 64






#endif













