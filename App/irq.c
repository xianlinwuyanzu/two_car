#include "common.h"
#include "include.h"


/*!
 *  @brief      PORTA�жϷ�����
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
//    uint8  n = 0;    //���ź�
//    uint32 flag = PORTA_ISFR;
//    PORTA_ISFR  = ~0;                                   //���жϱ�־λ
//
//    n = 6;                                              //���ж�
//    if(flag & (1 << n))                                 //PTA6�����ж�
//    {
//        camera_vsync();
//    }
//}
void PORTC_PORTD_IRQHandler()
{ 

    uint8  n = 0;    //���ź�
    uint32 flag;

    while(!PORTC_ISFR);
    flag = PORTC_ISFR;
    PORTC_ISFR  = ~0;                                   //���жϱ�־λ

    n = 2;                                             //���ж�
    if(flag & (1 << n))                                 //PTA29�����ж�
    {
        camera_vsync();
    }
//#if ( CAMERA_USE_HREF == 1 )                            //ʹ�����ж�
//    n = 28;
//    if(flag & (1 << n))                                 //PTA28�����ж�
//    {
//        camera_href();
//    }
//#endif


    //��ȡ����������ֵ
    i++;
    //lptmr_pulse_clean(); //����������������ֵ��������գ��������ܱ�֤����ֵ׼ȷ��
    if(i>2){
    speedl=lptmr_pulse_get();
    lptmr_pulse_clean();
//    i=0;
//     speedl = tpm_pulse_get(TPM2);        //�����������������ֵ
//     tpm_pulse_clean(TPM2); //����������������ֵ����ʼ�µļ�����
     i=0;
    }
//    
//     speedr = tpm_pulse_get(TPM2);        //�����������������ֵ
//     tpm_pulse_clean(TPM2); //����������������ֵ����ʼ�µļ�����
//    i=0;
//    }
//          sd = tpm_pulse_get(TPM2);        //�����������������ֵ
//          tpm_pulse_clean(TPM2); //����������������ֵ����ʼ�µļ�����
//          tpm_pulse_init(TPM2,TPM_CLKIN1,TPM_PS_1);//TPM����
//    
}

/*!
 *  @brief      DMA0�жϷ�����
 *  @since      v5.0
 */
void DMA0_IRQHandler()
{
    camera_dma();
}



void PIT_IRQHandler(void) 
{  
  //PIT0 �� PIT1 ������ͬ�жϺţ��������߶�������ͬ�жϺ�������Ҫ���ݱ�־λ���ж������ĸ�PIT�����ж�
  if(PIT_TFLG(PIT0) == 1 ) //�ж��Ƿ� PIT0 �����ж� 
  { 
    //MidPos=40;
   // CFFlag=1;
    if(CAR==1)
    zdflag=1;
    zdczl=zdczr=0;
    PIT_Flag_Clear(PIT0); //���жϱ�־λ 
    disable_irq (PIT_IRQn);
  } 
}


