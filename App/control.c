#include "common.h"
#include "include.h"
#include "int.h"
#include "irq.h"
#include "control.h"
#include "IMAGE.h"
#include "pid.h"
int MidPos=40;
extern int uw;
extern int Start;
extern int duoji;
extern int sanjiaol;
extern int sanjiaor;
//int LastError=0;
extern int shizi;
int Lastspeedderect=0;
int LastStableNumbers=0;
void DirectionCtrol(void);
int aaa=0;
int wd=0;
int zd=0;
int adc1;
int adc2;
int adc3;
extern unsigned int BlackEndM;
float Error = 0.0,ENCError=0.0;//原来是double
double Error0;
int Foresight = RowMax-5;//18    0

unsigned char StraightFS = RowMax-10;//直道前瞻
unsigned char SmallSFS = RowMax-10;//小s前瞻
unsigned char BigFS = RowMax-10;//大S前瞻
unsigned char BendFS = RowMax-10;//急弯前瞻
unsigned char CommonFS = RowMax-10;//普通前瞻
unsigned char StraightToBendFS = RowMax-10;
unsigned char ZhongxinFS=RowMax-10;


#define dz  1  //  1.4
double k =0;//0.38,0.28       //1.6为最大值
float StraightK =0.5*dz;//0.2//直道系数   
float SmallSK = 0.9*dz;//0.2	//小S系数
float BigSK = 0.9*dz;//大S系数
float BendK = 1.2*dz;//急弯
float CommonK = 1*dz;//普通系数									
float zhijiaoK= 1*dz;
float StraightToBendK =0*dz;

float temK_k=0;
float temK =1.0;
float temk_l=1.0;
float temk_r=1.0;


unsigned char StraightEnd = RowMax-3;//直道截止行
unsigned char SmallSEnd = RowMax-3;//小S弯截止行
unsigned char BigSEnd = RowMax-5;//大S弯截止行
unsigned char BendEnd = RowMax-3;//急弯截止行
unsigned char CommonEnd = RowMax-8;//普通截止行
unsigned char StraightToBenEnd = RowMax-0;//直入弯截止行

signed int RoadType=0;//赛道类型
signed int RoadType0=0;//赛道类型
int IncreaseSpeed1;//长直道速度提升1
int IncreaseSpeed2;//长直道速度提升2
int IncreaseSpeed3;//长直道速度提升3
unsigned char StraightToBenEnd;
extern signed char LastRoadType;
unsigned int weightSum1=0;
int LineWeight[RowMax] = {0};

unsigned char xunhuan=0;            //直角控制      
unsigned char ControlTime=0;

unsigned char ZhijiaoRightSpeed=0;
unsigned char ZhijiaoLeftSpeed=0;
unsigned char ZhijiaoJianSpeed=0;

unsigned char xunhuan2=0;           //过障碍控制
unsigned char ControlTime2=0;

int Zhijiaoturn=0;
int LastZhijiaoturn=0;

unsigned char ZhongxinCount=0;
unsigned char Zhongxin[RowMax];
unsigned char LeftMissMax=0;        //右边缘找到左边连续丢失的行数  
unsigned char RightMissMax=0;
unsigned char AllMissMax=0;          //左右边缘都丢失的情况0

double p1=0,d1=0;//6.4,4.8标准
double P;
double D;

int SteerPidCal_1;

double LastError = 0.0;

int CrossingBegin = 0;
int CrossingCount = 0;

int SpeedPid = 0;

volatile unsigned char IsStartLine;//起跑线标志
unsigned char IsOutRoad;//冲出赛道标志
int StraightSpeed = 0;   //差速速度控制
int SmallSSpeed	 = 0;
int BendSpeed = 0;
int CommonSpeed=0;
int BigSSpeed=0;
int SmallSpeed=0;
int ZhijiaoSpeed=0;
uint16 SpeedNumber;

unsigned char StraightToBendSpeed=15;
extern unsigned char AllStraightCount;
extern unsigned char AllSmallSCount;
unsigned char CanSpeedUp=0;
int HighSpeedFlag = 0;
int StableNumbers_max=0;
int HighSpeedCount = 0;

int duoji1;
int Lastduoji1;
extern  int MidEnd;
unsigned char speedderect=0;
int zaw_mode_last=0;
extern int zaw_mode;
int GetSteerError2(int start,int end,float midpos)
{
  char i = 0;
  int  Black_Sum = 0;
  int weightSum = 0;
  int TemError = 0.0;
  
  
  
  for(i = start; i < end; i++)    
  {	
  Black_Sum += BlackLineData[i]*LineWeight[i];
    weightSum += LineWeight[i];
  }
  
  
   if(zaw_mode_last!=0&&zaw_mode==0){zaw_mode=zaw_mode_last;zaw_mode_last=0;}
  else zaw_mode_last=zaw_mode;
  
  TemError =(int)( Black_Sum/weightSum - (midpos+zaw_mode));
//  if(zaw_mode_last!=0&&zaw_mode==0){zaw_mode=zaw_mode_last;zaw_mode_last=0;}
//  else zaw_mode_last=zaw_mode;
  
//  TemError =(int)(Black_Sum/weightSum - midpos);
  
  //	if(TemError > 40.0)
  //	{
  //		TemError = 40.0;
  //	}
  //	if(TemError < -40.0)
  //	{
  //		TemError = -40.0;	
  //	}
  
  return TemError;
}
//int moto_left=311;
// moto_right=237;
//舵机方向控制
void DirectionCtrol(void)
{
  //int i = 0;
int StartPos = RowMax-StableNumbers;
int EndPos = RowMax-1; 
        //MidPos=40;
         // MidPos=27;
     //  int MidPos = BlackLineData[RowMax-1]- LeftBlack[RowMax-1]*1.0/2+ (ColumnMax-RightBlack[RowMax-1])*1.0/2;
      int intError=0;
         
	Error=0.0;
//        if(StableNumbers<8)
//        {                           
//            return;
//        }
	
	switch(RoadType)
	{					   
		case 0://直
			Foresight = StraightFS;
			if(Foresight > StableNumbers-10)Foresight=StableNumbers-10;
			k = StraightK;
			StartPos = RowMax - Foresight+1;
			EndPos = StraightEnd; 
                   //     MidPos=40;
                        LastRoadType = RoadType;
                        speedderect=1;
			break;
                case 1://s
			Foresight = StraightFS;
                        k = SmallSK;
			if(Foresight > StableNumbers)Foresight=StableNumbers;
			StartPos = RowMax - Foresight;
			EndPos = SmallSEnd;
			LastRoadType=RoadType; 
                       // MidPos=40;
                      //  LastRoadType = RoadType;
                        speedderect=2;
			break;
		case 2://S
			Foresight = BigFS;
			if(Foresight > StableNumbers)Foresight=StableNumbers*5/6;
                     //   if(Foresight > StableNumbers+CompensateCount-11)Foresight=StableNumbers+CompensateCount-11;
			k = BigSK;
			StartPos = RowMax - Foresight+1;
			EndPos = BigSEnd;
                        LastRoadType=RoadType;
                        speedderect=3;
			break;
		case 3://急弯
			Foresight = BendFS;//17
			if(Foresight > StableNumbers) Foresight=StableNumbers;
			k = BendK;
			StartPos = RowMax - Foresight+1;
			EndPos = BendEnd;
                        LastRoadType=RoadType;
                      //  MidPos = 35;
                        speedderect=4;
			break;
                 case 103://急弯
			Foresight = ZhongxinFS;//17
			if(Foresight > StableNumbers) Foresight=StableNumbers;
			k = BendK;
			StartPos = RowMax - Foresight+1;
			EndPos = RowMax-15;//BendEnd;
                        LastRoadType=103;
                        speedderect=5;
			break;     
		default:
			k = CommonK;
			Foresight = CommonFS;
			StartPos = RowMax - Foresight+1;
			EndPos = CommonEnd;
			break;
	}   
//           if (0){
//    
//    if(StartPos >= EndPos){StartPos = EndPos - 2;EndPos = EndPos + 2;}
//    for (i = RowMax-1;i > 0;i--)
//    {
//      if (i > RowMax - 20||i>=RowMax-StableNumbers-1)
//      {
//        
//        LineWeight[i]=50;
//        
//      }
//      
//      else if(i > RowMax - 40||i>=RowMax-StableNumbers-1)
//      {
//        LineWeight[i]=40;
//      }
//      else if(i > RowMax - 80||i>=RowMax-StableNumbers-1)
//      {
//        LineWeight[i]=20;
//      }
//      else if(i>=RowMax-StableNumbers-1)
//      {
//        LineWeight[i]=10;
//      }
//      else 
//      {
//        LineWeight[i]=0;
//      }
//    }
//    
//  }
//  
//  
//  else if (1)
//  {
    int i=0;    
    int qz_a,qz_b,qz_c;
    qz_a=(RowMax - (StableNumbers-1)*3/4);
    qz_b=(RowMax - (StableNumbers-1)/2);
    qz_c=(RowMax - (StableNumbers-1)/4);
    
      for (i = RowMax-1;i > 0;i--)
    {
      if (i > RowMax - (StableNumbers-1))
      {
        if (i >= StartPos && i <=qz_a)
        {
          
          LineWeight[i]=90;//55;50
          
        }
        else if (i > qz_a&& i <= qz_b)
        {
          
          
          LineWeight[i]=15;//55;50
          
        }
        else if (i > qz_b && i <= qz_c)
        {
          
          LineWeight[i]=10;//32//55	
          
        }
        else   if (i > qz_c&& i <= EndPos)
        {
          
          
          LineWeight[i]=5;
          
        }
        
        else
        {
          LineWeight[i]=0;
        }
      }
      else
      {
        LineWeight[i]=0;
      }
    }
    
//  }      
  Error = GetSteerError2(StartPos,EndPos,MidPos);
  StableNumbers_max=StableNumbers;
//  //左拐小于零右拐大于零
  
    
// if(gpio_get(PTC7)==0)
// {  
//  
//  if(Error<0)
//  {
//    if(StableNumbers_max > RowMax*0.95)
//    {
//      temK = 1.1;
//    }
//    else if(StableNumbers_max >= RowMax*0.80)
//    {
//      temK = 1.3;
//    }
//    else if(StableNumbers_max >= RowMax*0.70)
//    {
//      temK = 1.4;
//    }
//    else if(StableNumbers_max >= RowMax*0.60)
//    {
//      temK = 1.7;
//    }
//    else if(StableNumbers_max >= RowMax*0.50)
//    {
//      temK = 1.9;
//    }
//    else if(StableNumbers_max >= RowMax*0.40)
//    {
//      temK =2.2;	
//    }
//    else if(StableNumbers_max >= RowMax*0.30)
//    {
//      temK = 2.5;	
//    }
//    else if(StableNumbers_max >= RowMax*0.20)
//    {
//      temK = 2.9;	
//    }
//    else 
//    {
//      temK = 3.0;	
//    }
//   
////if(StableNumbers_max<=RowMax*0.7) 
//  temK*=1.2; 
//  }
//  else
//  {
//     if(StableNumbers_max > RowMax*0.95)
//    {
//      temK =1.1;
//    }
//    else if(StableNumbers_max >= RowMax*0.80)
//    {
//      temK = 1.3;
//    }
//    else if(StableNumbers_max >= RowMax*0.70)
//    {
//      temK = 1.4;
//    }
//    else if(StableNumbers_max >= RowMax*0.60)
//    {
//      temK = 1.7;
//    }
//    else if(StableNumbers_max >= RowMax*0.50)
//    {
//      temK = 1.9;
//    }
//    else if(StableNumbers_max >= RowMax*0.40)
//    {
//      temK =2.0;//2.7	
//    }
//    else if(StableNumbers_max >= RowMax*0.30)
//    {
//      temK =2.5;//2.9	
//    }
//    else if(StableNumbers_max >= RowMax*0.20)
//    {
//      temK = 2.9;//3.3	
//    }
//    else 
//    {
//      temK = 3.3;	
//    }
// temK*=1.1;  
//  }
//  
// }
// else
// {
  if(Error<0)
  {
    if(StableNumbers_max > RowMax*0.95)
    {
      temK = 1.1;
    }
    else if(StableNumbers_max >= RowMax*0.80)
    {
      temK = 1.2;
    }
    else if(StableNumbers_max >= RowMax*0.70)
    {
      temK = 1.9;//1.5
    }
    else if(StableNumbers_max >= RowMax*0.60)
    {
      temK = 2.2;
    }
    else if(StableNumbers_max >= RowMax*0.50)
    {
      temK = 2.6;
    }
    else if(StableNumbers_max >= RowMax*0.40)
    {
      temK =2.8;	
    }
    else if(StableNumbers_max >= RowMax*0.30)
    {
      temK = 2.9;	
    }
    else if(StableNumbers_max >= RowMax*0.20)
    {
      temK = 3.2;	
    }
    else 
    {
      temK = 3.3;	
    }
   
//if(StableNumbers_max<=RowMax*0.7) 
  temK*=1.2; 
  }
  else
  {
        if(StableNumbers_max > RowMax*0.95)
    {
      temK =1.1;
    }
    else if(StableNumbers_max >= RowMax*0.80)
    {
      temK = 1.2;
    }
    else if(StableNumbers_max >= RowMax*0.70)
    {
      temK = 1.8;
    }
    else if(StableNumbers_max >= RowMax*0.60)
    {
      temK = 2.1;
    }
    else if(StableNumbers_max >= RowMax*0.50)
    {
      temK = 2.5;
    }
    else if(StableNumbers_max >= RowMax*0.40)
    {
      temK =2.7;//2.7	
    }
    else if(StableNumbers_max >= RowMax*0.30)
    {
      temK =2.9;//2.9	
    }
    else if(StableNumbers_max >= RowMax*0.20)
    {
      temK = 3.0;//3.3	
    }
    else 
    {
      temK = 3.3;	
    }
 temK*=1.5;  
  }
// }
 // temK = 1.1;
  intError = Error*temK*k;
//    OLED_Print_Num(100,0,intError);
//    OLED_Print_Num(60,2,intError);
//    OLED_Print_Num(0, 2, speedderect);

    duoji1=SteerPidCal(6,3,intError);


    
    if(StableNumbers<10 || (sanjiaol || sanjiaor) && !duoji )
    {
      if(LeftEnd < RightEnd || sanjiaor )
        duoji1 = 720;  //693
      else if(LeftEnd > RightEnd || sanjiaol)
        duoji1 = 1140;  //872
    }               //  有地方找不到边线  打死容易出界
    
    
    
//    if(MidEnd==0  && ThresholdData[RowMax-2][2] == White && ThresholdData[RowMax-2][ColumnMax-2] == White)
//    duoji1=Lastduoji1;
//     Lastduoji1=duoji1;
 
      tpm_pwm_duty(TPM2,TPM_CH0,duoji1);
  //tpm_pwm_duty(TPM1, TPM_CH0,SteerPidCal(Error));
 // LastError = Error;	
  
  
    //     printf("RoadType  \n  %d",RoadType);
   //   printf(" EndPos \n  %d",EndPos);
   //     printf(" MidPos \n  %d",MidPos);
     //  Error=GetSteerError(StartPos,EndPos,MidPos);//*1;//k
      
    //  led(LED0, LED_ON);
       //fabss((int)Error);
     
      //printf("MidPos\n  %d",MidPos);
  //   printf("  \n  %d",SubBasePoint);
       // printf("  \n  %d",intError);
        
        //  printf("  \n  %d",duoji1);
         
//        printf("  duoji1\n  %d",duoji1);
      //    led_turn(LED0);
        } 

extern int zdczl;
extern int zdczr;
extern char CAR;
int d=0;
int xx=0;
int czd=0;
int z=0;
int tt=0;
int adc=0;
void MotorUpdatePWM()   
{
  adc1=adc_once(ADC0_SE8, ADC_8bit);
  adc2=adc_once(ADC0_SE9, ADC_8bit);
  adc=MAX(adc1,adc2);
  if(Start)
  {
        if(CAR==1)
    {
     tt++;
     if(tt<4)
     {
    tpm_pwm_duty(TPM0,TPM_CH0,200);   //350       
    tpm_pwm_duty(TPM0,TPM_CH1,0);
    tpm_pwm_duty(TPM0,TPM_CH2,0);
    tpm_pwm_duty(TPM0,TPM_CH4,200); 
     }
    
    else
    {
    tpm_pwm_duty(TPM0,TPM_CH0,0);   //350       
    tpm_pwm_duty(TPM0,TPM_CH1,0);
    tpm_pwm_duty(TPM0,TPM_CH2,0);
    tpm_pwm_duty(TPM0,TPM_CH4,0);
    }
   }
   else
   {
   tpm_pwm_duty(TPM0,TPM_CH0,0);   //350       
    tpm_pwm_duty(TPM0,TPM_CH1,0);
    tpm_pwm_duty(TPM0,TPM_CH2,0);
    tpm_pwm_duty(TPM0,TPM_CH4,0);
   }
  }
  else if(adc>80 && !zdczl && !zdczr)
  {
   tpm_pwm_duty(TPM0,TPM_CH0,50);   //350       
   tpm_pwm_duty(TPM0,TPM_CH1,0);
   tpm_pwm_duty(TPM0,TPM_CH2,0);
   tpm_pwm_duty(TPM0,TPM_CH4,50);
  
  }
  else
  {
  if(gpio_get(PTC7)==0)
     {
     if((zdczl ||zdczr) && CAR==3)
      {
      d++;
      if(d<5)
      {
        tpm_pwm_duty(TPM0,TPM_CH0,250);   //350       
        tpm_pwm_duty(TPM0,TPM_CH1,0);
        tpm_pwm_duty(TPM0,TPM_CH2,0);
        tpm_pwm_duty(TPM0,TPM_CH4,250);
      }
      else
      {
        tpm_pwm_duty(TPM0,TPM_CH0,0);   //350       
        tpm_pwm_duty(TPM0,TPM_CH1,0);
        tpm_pwm_duty(TPM0,TPM_CH2,0);
        tpm_pwm_duty(TPM0,TPM_CH4,0);
      }

    }
    else{
      d=0;
       if(StableNumbers<RowMax*0.6 )
       {
        tpm_pwm_duty(TPM0,TPM_CH0,300);   //350       
        tpm_pwm_duty(TPM0,TPM_CH1,0);
        tpm_pwm_duty(TPM0,TPM_CH2,0);
        tpm_pwm_duty(TPM0,TPM_CH4,300);
       }
      
       else
       {
        tpm_pwm_duty(TPM0,TPM_CH0,330);   //350       
        tpm_pwm_duty(TPM0,TPM_CH1,0);
        tpm_pwm_duty(TPM0,TPM_CH2,0);
        tpm_pwm_duty(TPM0,TPM_CH4,330);
       }
    }
     }
  
  else if(gpio_get(PTC6)==0)
     { 
      if(zdczl ||zdczr)
      {
      d++;
      if(d<1)
      {
        tpm_pwm_duty(TPM0,TPM_CH0,250);   //350       
        tpm_pwm_duty(TPM0,TPM_CH1,0);
        tpm_pwm_duty(TPM0,TPM_CH2,0);
        tpm_pwm_duty(TPM0,TPM_CH4,250);
      }
      else
      {
        tpm_pwm_duty(TPM0,TPM_CH0,0);   //350       
        tpm_pwm_duty(TPM0,TPM_CH1,0);
        tpm_pwm_duty(TPM0,TPM_CH2,0);
        tpm_pwm_duty(TPM0,TPM_CH4,0);
      }

    }
    else{
      d=0;
    if(sanjiaol || sanjiaor)
    {
        tpm_pwm_duty(TPM0,TPM_CH0,200);   //350       
        tpm_pwm_duty(TPM0,TPM_CH1,0);
        tpm_pwm_duty(TPM0,TPM_CH2,0);
        tpm_pwm_duty(TPM0,TPM_CH4,200); 
    }
    else
    {
     if(StableNumbers<RowMax*0.7 && czd && !sanjiaol && !sanjiaor && !zdczl && !zdczr)
       {
         xx++;
         if(xx>4)
           
         {czd=0;z=0;}
        tpm_pwm_duty(TPM0,TPM_CH0,0);   //350       
        tpm_pwm_duty(TPM0,TPM_CH1,0);
        tpm_pwm_duty(TPM0,TPM_CH2,0);
        tpm_pwm_duty(TPM0,TPM_CH4,0);
       }
//      else if(sanjiaol || sanjiaor) 
//       {
//        tpm_pwm_duty(TPM0,TPM_CH0,150);   //350       
//        tpm_pwm_duty(TPM0,TPM_CH1,0);
//        tpm_pwm_duty(TPM0,TPM_CH2,0);
//        tpm_pwm_duty(TPM0,TPM_CH3,150);
//       }
       
       else
       {
         xx=0;
         if(duoji1<870 && StableNumbers<RowMax*0.5)
         {
        tpm_pwm_duty(TPM0,TPM_CH0,400);   //zuo   
        tpm_pwm_duty(TPM0,TPM_CH1,0);
        tpm_pwm_duty(TPM0,TPM_CH2,0);
        tpm_pwm_duty(TPM0,TPM_CH4,400-(870-duoji1)*2); 
         }
         else if(duoji1>1020 && StableNumbers<RowMax*0.5)
         {
        tpm_pwm_duty(TPM0,TPM_CH0,400-(duoji1-1020)*2);   //350       
        tpm_pwm_duty(TPM0,TPM_CH1,0);
        tpm_pwm_duty(TPM0,TPM_CH2,0);
        tpm_pwm_duty(TPM0,TPM_CH4,400);  
         }
         else
         {

         if(CAR==0)
           {
        tpm_pwm_duty(TPM0,TPM_CH0,415);   //350       
        tpm_pwm_duty(TPM0,TPM_CH1,0);
        tpm_pwm_duty(TPM0,TPM_CH2,0);
        tpm_pwm_duty(TPM0,TPM_CH4,415); 

           }
           else
           {
        tpm_pwm_duty(TPM0,TPM_CH0,410);   //350       
        tpm_pwm_duty(TPM0,TPM_CH1,0);
        tpm_pwm_duty(TPM0,TPM_CH2,0);
        tpm_pwm_duty(TPM0,TPM_CH4,410); 
           }

         }
       }
       }
    }
  }
   else if(gpio_get(PTC8)==0)
     { 
      if(zdczl ||zdczr)
      {
      d++;
      if(d<3)
      {
        tpm_pwm_duty(TPM0,TPM_CH0,0);   //350       
        tpm_pwm_duty(TPM0,TPM_CH1,200);
        tpm_pwm_duty(TPM0,TPM_CH2,200);
        tpm_pwm_duty(TPM0,TPM_CH4,0);
      }
      else
      {
        tpm_pwm_duty(TPM0,TPM_CH0,0);   //350       
        tpm_pwm_duty(TPM0,TPM_CH1,0);
        tpm_pwm_duty(TPM0,TPM_CH2,0);
        tpm_pwm_duty(TPM0,TPM_CH4,0);
      }

    }
    else{
      d=0;
    
     if(StableNumbers<RowMax*0.7 && czd && !sanjiaol && !sanjiaor && !zdczl && !zdczr)
       {
         xx++;
         if(xx>8)
           
         {czd=0;z=0;}
        tpm_pwm_duty(TPM0,TPM_CH0,0);   //350       
        tpm_pwm_duty(TPM0,TPM_CH1,0);
        tpm_pwm_duty(TPM0,TPM_CH2,0);
        tpm_pwm_duty(TPM0,TPM_CH4,0);
       }
//      else if(sanjiaol || sanjiaor) 
//       {
//        tpm_pwm_duty(TPM0,TPM_CH0,150);   //350       
//        tpm_pwm_duty(TPM0,TPM_CH1,0);
//        tpm_pwm_duty(TPM0,TPM_CH2,0);
//        tpm_pwm_duty(TPM0,TPM_CH3,150);
//       }
       
       else
       {
                xx=0;
         if(duoji1<870 && StableNumbers<RowMax*0.5)
         {
        tpm_pwm_duty(TPM0,TPM_CH0,400);   //zuo   
        tpm_pwm_duty(TPM0,TPM_CH1,0);
        tpm_pwm_duty(TPM0,TPM_CH2,0);
        tpm_pwm_duty(TPM0,TPM_CH4,400-(870-duoji1)*2); 
         }
         else if(duoji1>1020 && StableNumbers<RowMax*0.5)
         {
        tpm_pwm_duty(TPM0,TPM_CH0,400-(duoji1-1020)*2);   //350       
        tpm_pwm_duty(TPM0,TPM_CH1,0);
        tpm_pwm_duty(TPM0,TPM_CH2,0);
        tpm_pwm_duty(TPM0,TPM_CH4,400);  
         }
         else
         {

         if(CAR==0)
           {
        tpm_pwm_duty(TPM0,TPM_CH0,455);   //350       
        tpm_pwm_duty(TPM0,TPM_CH1,0);
        tpm_pwm_duty(TPM0,TPM_CH2,0);
        tpm_pwm_duty(TPM0,TPM_CH4,455); 

           }
           else
           {
        tpm_pwm_duty(TPM0,TPM_CH0,450);   //350       
        tpm_pwm_duty(TPM0,TPM_CH1,0);
        tpm_pwm_duty(TPM0,TPM_CH2,0);
        tpm_pwm_duty(TPM0,TPM_CH4,450); 
           }

         }
       }
    }
  }
  
else if(gpio_get(PTC9)==0)
  {
     if(zdczl ||zdczr)
      {
      d++;
      if(d<3)
      {
        tpm_pwm_duty(TPM0,TPM_CH0,0);   //350       
        tpm_pwm_duty(TPM0,TPM_CH1,200);
        tpm_pwm_duty(TPM0,TPM_CH2,200);
        tpm_pwm_duty(TPM0,TPM_CH4,0);
      }
      else
      {
        tpm_pwm_duty(TPM0,TPM_CH0,0);   //350       
        tpm_pwm_duty(TPM0,TPM_CH1,0);
        tpm_pwm_duty(TPM0,TPM_CH2,0);
        tpm_pwm_duty(TPM0,TPM_CH4,0);
      }

    }
    else{
      d=0;
    
     if(StableNumbers<RowMax*0.75 && czd && !sanjiaol && !sanjiaor && !zdczl && !zdczr)
       {
         xx++;
         if(xx>8)
           
         {czd=0;z=0;}
        tpm_pwm_duty(TPM0,TPM_CH0,0);   //350       
        tpm_pwm_duty(TPM0,TPM_CH1,0);
        tpm_pwm_duty(TPM0,TPM_CH2,0);
        tpm_pwm_duty(TPM0,TPM_CH4,0);
       }
//      else if(sanjiaol || sanjiaor) 
//       {
//        tpm_pwm_duty(TPM0,TPM_CH0,150);   //350       
//        tpm_pwm_duty(TPM0,TPM_CH1,0);
//        tpm_pwm_duty(TPM0,TPM_CH2,0);
//        tpm_pwm_duty(TPM0,TPM_CH3,150);
//       }
       
       else
       {
         xx=0;
         if(duoji1<870 && StableNumbers<RowMax*0.6)
         {
        tpm_pwm_duty(TPM0,TPM_CH0,460);   //zuo   
        tpm_pwm_duty(TPM0,TPM_CH1,0);
        tpm_pwm_duty(TPM0,TPM_CH2,0);
        tpm_pwm_duty(TPM0,TPM_CH4,460-(870-duoji1)*2); 
         }
         else if(duoji1>1020 && StableNumbers<RowMax*0.6)
         {
        tpm_pwm_duty(TPM0,TPM_CH0,460-(duoji1-1020)*2);   //350       
        tpm_pwm_duty(TPM0,TPM_CH1,0);
        tpm_pwm_duty(TPM0,TPM_CH2,0);
        tpm_pwm_duty(TPM0,TPM_CH4,460);  
         }
         else
         {
        tpm_pwm_duty(TPM0,TPM_CH0,500);   //350       
        tpm_pwm_duty(TPM0,TPM_CH1,0);
        tpm_pwm_duty(TPM0,TPM_CH2,0);
        tpm_pwm_duty(TPM0,TPM_CH4,500); 

         }
       }
    }
  
  }
  }
  
  
  if(StableNumbers>RowMax*0.85 || sanjiaol|| sanjiaor)
{
  z++;
  if(z>15)
    czd=1;
}
else
  z=0;
}





