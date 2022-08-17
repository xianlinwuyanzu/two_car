/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       main.c
 * @brief      山外KL26 平台主程序
 * @author     山外科技
 * @version    v5.2
 * @date       2014-10-26
 */

#include "common.h"
#include "include.h"
#include "IMAGE.h"
#include "irq.h"
#include "int.h"
#include "control.h"
//#define White 0
//#define Black 1
 uint8 imgbuff[CAMERA_SIZE];                             //定义存储接收图像的数组
//extern uint8 img[CAMERA_H][CAMERA_W];                           //由于鹰眼摄像头是一字节8个像素，因而需要解压为 1字节1个像素，方便处理

//         1111111111111111111111111
extern unsigned int BlackEndMax;
extern unsigned int shizi;
extern unsigned int shizicount;
extern int End;
/*!
 *  @brief      main函数\\
 *  @since      v5.0
 *  @note       山外 DMA 采集摄像头 实验
 */
int bb=0;
void  main(void)
{
 // DELAY_MS(5000);
   int i;  
   DisableInterrupts;  
   Init();
   camera_init(imgbuff); 
   EnableInterrupts;                 
 
  camera_get_img(); 
 // enable_irq (PIT_IRQn);
 //摄像头获取图像
    while(1)
    {
       /*
                0                60
        0   BlackEndMax       StableNumbers
        2   speedderect       intError          duoji1
        4   Leftnumber  b       Rightnumber b 
        6   MidLineVariance   shizi(aa)
      
      */
       camera_img_deal(imgbuff);
       ImageProcess(); 
       //GetBlackEndParam();
       MidLineProcess();
       DirectionCtrol();
       MotorUpdatePWM();
       //OLED_Print_Num(0,4,shizicount);
       //OLED_Print_Num(0,6,shizi);
       //RTRecognition();
       
//       adc=adc_once(ADC0_SE8, ADC_8bit);
     
//       if(gpio_get(PTC13)==0)
//       {
//        for(i=RowMax-1;i>RowMax-StableNumbers;i--)   
//	{   
//	ThresholdData[i][BlackLineData[i]] = 0;
//        ThresholdData[i][RightBlack[i]] = 0;
//        ThresholdData[i][LeftBlack[i]] = 0;
//	}
//       
//      sending();
//       }
//       else 
//       {

      //}                  此处为将中线变黑  从而可以在上位机观看补线情况
     
     // vcan_sendimg(ThresholdData, sizeof(ThresholdData));
     
        //vcan_sendimg(img,CAMERA_W*CAMERA_H);             //注意，这里的 数据大小已经改为 CAMERA_W*CAMERA_H
   
   
//NUM_Send();
}
}
void vcan_sendimg(void *imgaddr, uint32 imgsize)
{
#define CMD_IMG     1
    uint8 cmdf[2] = {CMD_IMG, ~CMD_IMG};    //山外上位机 使用的命令
    uint8 cmdr[2] = {~CMD_IMG, CMD_IMG};    //山外上位机 使用的命令

    uart_putbuff(VCAN_PORT, cmdf, sizeof(cmdf));    //先发送命令

    uart_putbuff(VCAN_PORT, (uint8_t *)imgaddr, imgsize); //再发送图像

    uart_putbuff(VCAN_PORT, cmdr, sizeof(cmdr));    //先发送命令
}


//发送图像到上位机显示
//不同的上位机，不同的命令
//如果使用其他上位机，则需要修改代码

void sending()    
{
  
  int i,j;
  i=0;j=0;
  
   uart_putchar(UART0,0xff );                  //红树伟业上位机
   //OLED_Fill(0x00);
     for(i=0;i<RowMax;i++)
     
        for(j=0;j<ColumnMax;j++)
        {
         //   ThresholdData[i][BlackLineData[i]]=0;
//            uart_putchar(UART0, LeftBlack[i]);
          //if(ThresholdData[i][j] == 0)
          uart_putchar(UART0,ThresholdData[i][j] );
//          {
//             OLED_PutPixel(i,j);
//             
//          }
        }
   
     
     
}
