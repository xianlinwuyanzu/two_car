#include "common.h"
#include "include.h"
#include "int.h"
#include "IMAGE.h"
#include "control.h"
#include "irq.h"





//float   LP12,     LP3,    LP45,     LP,
//         LD12,     LD3,    LD45,     LD,
//         
//         RP12,     RP3,    RP45,     RP,
//         RD12,     RD3,    RD45,     RD,
//         
//         P,      D;
/*********** FTMn_CHn PWM输出初始化函数 ***********/
/*
        --FTM0--  --FTM1--  --FTM2--
CH0       PTC1      PTA8      PTA10
CH1       PTC2      PTA9      PTA11
CH2       PTC3       ×         ×
CH3       PTC4       ×         ×
CH4       PTD4       ×         ×
CH5       PTD5       ×         ×
CH6       PTD6       ×         ×
CH7       PTD7       ×         ×
×表示没有
*/
/*************初始化函数************/
void Init()
{
   //led_init(LED0);
  // led_init(LED1);
  // led_init(LED2);
  // led_init(LED3);  
  // OLED_Init();
   uart_IN_init();
   adc_init(ADC0_SE8);
   adc_init(ADC0_SE9);
   MotorInit();     //电机初始化
   SteerInit();     //舵机初始化
   SwitchInit();
   //pit_init_ms(PIT0, 5000); //初始化PIT0，定时时间为： 2000ms
   
   
  // set_vector_handler(PIT_VECTORn ,PIT_IRQHandler);
   set_vector_handler(PORTC_PORTD_VECTORn ,PORTC_PORTD_IRQHandler);    //设置LPTMR的中断服务函数为 PORTA_IRQHandler
   set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);      //设置LPTMR的中断服务函数为 PORTA_IRQHandler
   lptmr_pulse_init(LPT0_ALT2, 0xFFFF, LPT_Rising);
   Boma1();
  //tpm_pulse_init(TPM2,TPM_CLKIN0,TPM_PS_1);//TPM计数
 // CameraInit();          //摄像头初始化
 
}
///***************拨码开关初始化********************* */   
 void SwitchInit()
 { 
  // gpio_get (PTXn_e);
        gpio_init (PTC7 , GPI, 1);	//设置PTI为输入模式
        gpio_init (PTC6 , GPI, 1);
        gpio_init (PTC8 , GPI, 1);
        gpio_init (PTC9 , GPI, 1);
        gpio_init (PTC10 , GPI, 1);	//设置PTI为输入模式
        gpio_init (PTC11 , GPI, 1);
        gpio_init (PTC13 , GPI, 1);
        gpio_init (PTC16 , GPI, 1);      
        port_init_NoALT(PTC7, PULLUP );        //上拉输入模式
        port_init_NoALT(PTC6, PULLUP );
        port_init_NoALT(PTC8, PULLUP );
        port_init_NoALT(PTC9, PULLUP );
        port_init_NoALT(PTC10, PULLUP );        //上拉输入模式
        port_init_NoALT(PTC11, PULLUP );
        port_init_NoALT(PTC13, PULLUP );
        port_init_NoALT(PTC16, PULLUP );
 }
///*****************电机初始化********************/
void MotorInit()
{
        tpm_pwm_init(TPM0,TPM_CH0,10000,200);   //zuo        
        tpm_pwm_init(TPM0,TPM_CH1,10000,0);
        tpm_pwm_init(TPM0,TPM_CH2,10000,0);
        tpm_pwm_init(TPM0,TPM_CH4,10000,0); //you
   
}



///********舵机初始化***********/
void SteerInit()
{
  //ftm_pwm_init(FTM2,FTM_CH0, 300, 4400);   //舵机PWM初始化     管脚  PTA10  (PTA11)
  tpm_pwm_init(TPM2,TPM_CH0,100,940);   //舵机PWM初始化     管脚  PTA1   474   948
  
}
extern unsigned char CAR;
void Boma1()
{


 if(gpio_get(PTC16)==0)
 {
 CAR=1;
 }
 else
   CAR=0;

}