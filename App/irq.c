#include "common.h"
#include "include.h"


/*!
 *  @brief      PORTA中断服务函数
 *  @since      v5.0

 */int i;
extern char CAR;
extern int zdflag;
extern int zdczl;
extern int zdczr;
extern int MidPos;
int speedl=0;
int speedr=0;
extern signed int CFFlag;
//void PORTA_IRQHandler()
//{
//    uint8  n = 0;    //引脚号
//    uint32 flag = PORTA_ISFR;
//    PORTA_ISFR  = ~0;                                   //清中断标志位
//
//    n = 6;                                              //场中断
//    if(flag & (1 << n))                                 //PTA6触发中断
//    {
//        camera_vsync();
//    }
//}
void PORTC_PORTD_IRQHandler()
{ 

    uint8  n = 0;    //引脚号
    uint32 flag;

    while(!PORTC_ISFR);
    flag = PORTC_ISFR;
    PORTC_ISFR  = ~0;                                   //清中断标志位

    n = 2;                                             //场中断
    if(flag & (1 << n))                                 //PTA29触发中断
    {
        camera_vsync();
    }
//#if ( CAMERA_USE_HREF == 1 )                            //使用行中断
//    n = 28;
//    if(flag & (1 << n))                                 //PTA28触发中断
//    {
//        camera_href();
//    }
//#endif


    //获取编码器计数值
    i++;
    //lptmr_pulse_clean(); //清空脉冲计数器计算值（马上清空，这样才能保证计数值准确）
    if(i>2){
    speedl=lptmr_pulse_get();
    lptmr_pulse_clean();
//    i=0;
//     speedl = tpm_pulse_get(TPM2);        //保存脉冲计数器计算值
//     tpm_pulse_clean(TPM2); //清空脉冲计数器计算值（开始新的计数）
     i=0;
    }
//    
//     speedr = tpm_pulse_get(TPM2);        //保存脉冲计数器计算值
//     tpm_pulse_clean(TPM2); //清空脉冲计数器计算值（开始新的计数）
//    i=0;
//    }
//          sd = tpm_pulse_get(TPM2);        //保存脉冲计数器计算值
//          tpm_pulse_clean(TPM2); //清空脉冲计数器计算值（开始新的计数）
//          tpm_pulse_init(TPM2,TPM_CLKIN1,TPM_PS_1);//TPM计数
//    
}

/*!
 *  @brief      DMA0中断服务函数
 *  @since      v5.0
 */
void DMA0_IRQHandler()
{
    camera_dma();
}



void PIT_IRQHandler(void) 
{  
  //PIT0 和 PIT1 共用相同中断号，所以两者都共用相同中断函数，需要根据标志位来判断是由哪个PIT触发中断
  if(PIT_TFLG(PIT0) == 1 ) //判断是否 PIT0 进入中断 
  { 
    //MidPos=40;
   // CFFlag=1;
    if(CAR==1)
    zdflag=1;
    zdczl=zdczr=0;
    PIT_Flag_Clear(PIT0); //清中断标志位 
    disable_irq (PIT_IRQn);
  } 
}


