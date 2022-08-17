
#include "common.h"
#include "include.h"
#include "int.h"
#include "irq.h"
#include "control.h"
#include "IMAGE.h"
#include "pid.h"

double k_p,k_d,k_pm;

double  L_StraightSteerPID_P ;
double	L_StraightSteerPID_I ;
double	L_StraightSteerPID_D ;
double	L_SmallSSteerPID_P ;
double	L_SmallSSteerPID_I ;
double	L_SmallSSteerPID_D ;
double	L_BendSteerPID_P ;
double	L_BendSteerPID_I ;
double	L_BendSteerPID_D ;

int MotorMax_right=0;
int MotorMin_right=0;
int MotorMiddle_right=0;


double  R_StraightSteerPID_P ;
double	R_StraightSteerPID_I ;
double	R_StraightSteerPID_D ;
double	R_SmallSSteerPID_P ;
double	R_SmallSSteerPID_I ;
double	R_SmallSSteerPID_D ;
double	R_BendSteerPID_P ;
double	R_BendSteerPID_I ;
double	R_BendSteerPID_D ;

double Speed_P;
double Speed_I ;//0.25;
double Speed_D;
//double SpeedPid_P_right=2;//12
//double SpeedPid_I_right=0.25;//0.25;
//double SpeedPid_D_right=0;//0.0007
//double SpeedPid_P_right=2.3;//2.6//减速15
//double SpeedPid_I_right=0.01480 ;//0.012;//0.43
//double SpeedPid_D_right=0.218;//0.13//0.15
 double SpeedPid_P_right;//2.6//减速15
double SpeedPid_I_right ;//0.012;//0.43
double SpeedPid_D_right;//0.13//0.15
double Showdirect=0;
double g_fSpeedControlIntegral=0; 
int MotorMax_left=0;
int MotorMin_left=0;
int MotorMiddle_left=0;
int16 Direct  = 0; 
/*============================================================================
函数名称：SteerPID
函数返回：无
参数说明：       
功能概要：方向PID函数
=============================================================================*/

 SteetPidStruct SteerPID;
 
 MotorPID_leftStruct MotorPID_left;
 MotorPID_rightStruct MotorPID_right;
extern double g_fCarSpeed;
char ren=10;
double SteerPidCal123;
uint16 SteerPidCal(double Steer_P,double Steer_D, float _excursion)
{  
        
        int16 aa=0;
   
        SteerPID.Kp = Steer_P;//12
	SteerPID.Kd = Steer_D;//4
	SteerPID.e0 = _excursion;
  Showdirect=SteerPID.Kp*SteerPID.e0 - SteerPID.Kd*(SteerPID.e1-SteerPID.e2);       
        
   Direct= (uint16)(DirectMiddle-Showdirect);      
         if(Direct < DirectLeft)
	{
		Direct = DirectLeft;	
	}
	else if(Direct > DirectRight)
	{
		Direct = DirectRight;	
	}
	SteerPID.e2 = SteerPID.e1;
	SteerPID.e1 = SteerPID.e0;
      //  printf("Direct\n  %d",Direct);
     //  tpm_pwm_duty(TPM1,TPM_CH0,Direct+700);//*30+4400
       
       return Direct;   
          
    /* 	
        //int16 Direct1  = 0; //增量式
	SteerPID.Kp = Steer_P;//12
	SteerPID.Kd = Steer_D;//4
	SteerPID.e0 = _excursion;
        //Direct1 = (uint16)(SteerPID.Kp*(SteerPID.e0-SteerPID.e1) +SteerPID.Kd*(SteerPID.e0-2*SteerPID.e1+SteerPID.e2) );
        //Direct+=Direct1;
        Showdirect=SteerPID.Kp*SteerPID.e0 - SteerPID.Kd*(SteerPID.e1-SteerPID.e2);
    //    Direct = (uint16)(DirectMiddle-SteerPID.Kp*SteerPID.e0 +SteerPID.Kd*(SteerPID.e1-SteerPID.e2) );//位置是
        Direct= (uint16)(DirectMiddle-Showdirect);
	//Direct = (uint16)(DirectMiddle - SteerPID.Kp*SteerPID.e0);

	if(Direct < DirectLeft)
	{
		Direct = DirectLeft;	
	}
	else if(Direct > DirectRight)
	{
		Direct = DirectRight;	
	}

	SteerPID.e2 = SteerPID.e1;
	SteerPID.e1 = SteerPID.e0;
	
	return Direct;*/
}

//int pwm_right;
//int pwm_left;
//int Pwm_Left=0,Pwm_Right=0;
//double Error0_Speed,Error1_Speed,Error2_Speed;
//int MotorPidCalRight(uint16 SpeedSet_right1,int MotorSpeed_right1) //增量式
//{
//	MotorPID_right.e0 = SpeedSet_right1-MotorSpeed_right1;	
//	MotorPID_right.Kp = SpeedPid_P_right;//12.0;
//	MotorPID_right.Ki = SpeedPid_I_right;//0.5;
//	MotorPID_right.Kd = SpeedPid_D_right;//7.0;
//	MotorPID_right.DefaultSum = MotorPID_right.Kp*(MotorPID_right.e0-MotorPID_right.e1) + MotorPID_right.Ki*MotorPID_right.e0 + MotorPID_right.Kd*(MotorPID_right.e0 - 2.0*MotorPID_right.e1 + MotorPID_right.e2);	    
//        MotorPID_right.Sum += MotorPID_right.DefaultSum;
//	MotorPID_right.e2=MotorPID_right.e1;
//        MotorPID_right.e1=MotorPID_right.e0;
//        Pwm_Right=(int)MotorPID_right.Sum;
//	if(MotorPID_right.Sum > MotorMax_right)
//		MotorPID_right.Sum = MotorMax_right;	
//	if(MotorPID_right.Sum < MotorMiddle_right)
//	  	MotorPID_right.Sum = MotorMiddle_right;
//	pwm_right=(int)MotorPID_right.Sum;
//        return pwm_right;
//} 
//int MotorPidCalLeft(uint16 SpeedSet_left1,int MotorSpeed_left1) //增量式
//{
//	MotorPID_left.e0 = SpeedSet_left1-MotorSpeed_left1;	
//	MotorPID_left.Kp = SpeedPid_P_right;//12.0;
//	MotorPID_left.Ki = SpeedPid_I_right;//0.5;
//	MotorPID_left.Kd = SpeedPid_D_right;//7.0;
//        
//	MotorPID_left.DefaultSum = MotorPID_left.Kp*(MotorPID_left.e0-MotorPID_left.e1) + MotorPID_left.Ki*MotorPID_left.e0 + MotorPID_left.Kd*(MotorPID_left.e0 - 2.0*MotorPID_left.e1 + MotorPID_left.e2);	    
//        MotorPID_left.Sum += MotorPID_left.DefaultSum;
//	MotorPID_left.e2=MotorPID_left.e1;
//        MotorPID_left.e1=MotorPID_left.e0;
//        
//        Pwm_Left=(int)MotorPID_left.Sum;
//	if(MotorPID_left.Sum > MotorMax_right)
//		MotorPID_left.Sum = MotorMax_right;	
//	if(MotorPID_left.Sum < MotorMin_right)
//	  	MotorPID_left.Sum = MotorMin_right;
//	pwm_left=(int)MotorPID_left.Sum;
//        return pwm_left;   
//} 
//int MotorPidCal_sl(int SpeedSet_left1,int MotorSpeed_left1) //增量式
//{
//	MotorPID_left.e0 = SpeedSet_left1-MotorSpeed_left1;	
//	MotorPID_left.Kp = SpeedPid_P_right;//12.0;
//	MotorPID_left.Ki = SpeedPid_I_right;//0.5;
//	MotorPID_left.Kd = SpeedPid_D_right;//7.0;
//        
//	MotorPID_left.DefaultSum = MotorPID_left.Kp*(MotorPID_left.e0-MotorPID_left.e1) + MotorPID_left.Ki*MotorPID_left.e0 + MotorPID_left.Kd*(MotorPID_left.e0 - 2.0*MotorPID_left.e1 + MotorPID_left.e2);	    
//        MotorPID_left.Sum += MotorPID_left.DefaultSum;
//	MotorPID_left.e2=MotorPID_left.e1;
//        MotorPID_left.e1=MotorPID_left.e0;
//	if(MotorPID_left.Sum > MotorMax_left)
//		MotorPID_left.Sum = MotorMax_left;	
//	if(MotorPID_left.Sum < MotorMiddle_left)
//	  	MotorPID_left.Sum = MotorMiddle_left;
//	pwm_left=(int)MotorPID_left.Sum;
//        return pwm_left;
//} 
//int MotorPidCal_sr(int SpeedSet_right1,int MotorSpeed_right1) //增量式
//{
//	MotorPID_right.e0 = SpeedSet_right1-MotorSpeed_right1;	
//	MotorPID_right.Kp = SpeedPid_P_right;//12.0;
//	MotorPID_right.Ki = SpeedPid_I_right;//0.5;
//	MotorPID_right.Kd = SpeedPid_D_right;//7.0;
//        
//	MotorPID_right.DefaultSum = MotorPID_right.Kp*(MotorPID_right.e0-MotorPID_right.e1) + MotorPID_right.Ki*MotorPID_right.e0 + MotorPID_right.Kd*(MotorPID_right.e0 - 2.0*MotorPID_right.e1 + MotorPID_right.e2);	    
//        MotorPID_right.Sum += MotorPID_right.DefaultSum;
//	MotorPID_right.e2=MotorPID_right.e1;
//        MotorPID_right.e1=MotorPID_right.e0;
//	if(MotorPID_right.Sum > MotorMax_right)
//		MotorPID_right.Sum = MotorMax_right;	
//	if(MotorPID_right.Sum < MotorMiddle_right)
//	  	MotorPID_right.Sum = MotorMiddle_right;
//	pwm_right=(int)MotorPID_right.Sum;
//        return pwm_right;
//}