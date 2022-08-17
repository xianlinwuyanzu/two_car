/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       main.c
 * @brief      ɽ��KL26 ƽ̨������
 * @author     ɽ��Ƽ�
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
 uint8 imgbuff[CAMERA_SIZE];                             //����洢����ͼ�������
//extern uint8 img[CAMERA_H][CAMERA_W];                           //����ӥ������ͷ��һ�ֽ�8�����أ������Ҫ��ѹΪ 1�ֽ�1�����أ����㴦��

//         1111111111111111111111111
extern unsigned int BlackEndMax;
extern unsigned int shizi;
extern unsigned int shizicount;
extern int End;
/*!
 *  @brief      main����\\
 *  @since      v5.0
 *  @note       ɽ�� DMA �ɼ�����ͷ ʵ��
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
 //����ͷ��ȡͼ��
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

      //}                  �˴�Ϊ�����߱��  �Ӷ���������λ���ۿ��������
     
     // vcan_sendimg(ThresholdData, sizeof(ThresholdData));
     
        //vcan_sendimg(img,CAMERA_W*CAMERA_H);             //ע�⣬����� ���ݴ�С�Ѿ���Ϊ CAMERA_W*CAMERA_H
   
   
//NUM_Send();
}
}
void vcan_sendimg(void *imgaddr, uint32 imgsize)
{
#define CMD_IMG     1
    uint8 cmdf[2] = {CMD_IMG, ~CMD_IMG};    //ɽ����λ�� ʹ�õ�����
    uint8 cmdr[2] = {~CMD_IMG, CMD_IMG};    //ɽ����λ�� ʹ�õ�����

    uart_putbuff(VCAN_PORT, cmdf, sizeof(cmdf));    //�ȷ�������

    uart_putbuff(VCAN_PORT, (uint8_t *)imgaddr, imgsize); //�ٷ���ͼ��

    uart_putbuff(VCAN_PORT, cmdr, sizeof(cmdr));    //�ȷ�������
}


//����ͼ����λ����ʾ
//��ͬ����λ������ͬ������
//���ʹ��������λ��������Ҫ�޸Ĵ���

void sending()    
{
  
  int i,j;
  i=0;j=0;
  
   uart_putchar(UART0,0xff );                  //����ΰҵ��λ��
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
