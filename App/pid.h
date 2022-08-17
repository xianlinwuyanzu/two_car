#ifndef __PID__
#define __PID__
#include "include.h"
//#define MotorMax_right  	   550
//#define MotorMin_right  	   -550
//#define MotorMiddle_right  	   300

#define DirectMiddle  	  940// 948 	      //舵机中值，原始数据为                   470
#define DirectLeft	   (DirectMiddle-220) //1020舵机左值DirectMiddleDirectMiddle-  96   740    100
#define DirectRight	   (DirectMiddle+227) //2020舵机右值DirectMiddle+              91   1135    90


uint16 SteerPidCal(double Steer_P,double Steer_D, float _excursion);


extern int MotorMax_right;
extern int MotorMin_right;
extern int MotorMiddle_right;

extern double k_p,k_d,k_pm;

extern int Pwm_Left,Pwm_Right;

typedef struct 
{
     
	double Kp;
        double Ki;
	double Kd;
	double e0;
	double e1;
	double e2;

}SteetPidStruct;
//
typedef struct 
{
	double Kp;
	double Ki;
	double Kd;
	double e0;
	double e1;
	double e2;
	double Sum;
	double DefaultSum;
	double Speed;

}MotorPID_leftStruct;

typedef struct 
{
	double Kp;
	double Ki;
	double Kd;
	double e0;
	double e1;
	double e2;
	double Sum;
	double DefaultSum;
	double Speed;

}MotorPID_rightStruct;

extern double  L_StraightSteerPID_P ;
extern double	L_StraightSteerPID_I ;
extern double	L_StraightSteerPID_D ;
extern double	L_SmallSSteerPID_P ;
extern double	L_SmallSSteerPID_I ;
extern double	L_SmallSSteerPID_D ;
extern double	L_BendSteerPID_P ;
extern double	L_BendSteerPID_I ;
extern double	L_BendSteerPID_D ;


extern double  R_StraightSteerPID_P ;
extern double	R_StraightSteerPID_I ;
extern double	R_StraightSteerPID_D ;
extern double	R_SmallSSteerPID_P ;
extern double	R_SmallSSteerPID_I ;
extern double	R_SmallSSteerPID_D ;
extern double	R_BendSteerPID_P ;
extern double	R_BendSteerPID_I ;
extern double	R_BendSteerPID_D ;

extern double Speed_P;
extern double Speed_I ;//0.25;
extern double Speed_D;
extern double SpeedPid_P;
extern double SpeedPid_I ;//0.25;
extern double SpeedPid_D;
extern double SpeedPid_P_right;//2.6//减速15
extern double SpeedPid_I_right ;//0.012;//0.43
extern double SpeedPid_D_right;//0.13//0.15
extern double Showdirect;
extern double g_fSpeedControlIntegral; 
extern int16 Direct;
extern double Error0_Speed,Error1_Speed,Error2_Speed;



extern  MotorPID_leftStruct MotorPID_left;
extern  MotorPID_rightStruct MotorPID_right;
extern uint16 SteerPidCal(double Steer_P,double Steer_D, float _excursion);
//extern int MotorPidCal(uint16 SpeedSet_right1,int MotorSpeed_right1);
//extern double SteerPidCal(double _excursion,double _enc);
extern double SteerPidCal123;
extern int pwm_right;
extern int pwm_left;
int MotorPidCalRight(uint16 SpeedSet_right1,int MotorSpeed_right1) ;
int MotorPidCalLeft(uint16 SpeedSet_left1,int MotorSpeed_left1) ;
extern int MotorPidCal_sl(int SpeedSet_left1,int MotorSpeed_left1);
extern int MotorPidCal_sr(int SpeedSet_right1,int MotorSpeed_right1);
extern int MotorMax_left;
extern int MotorMin_left;
extern int MotorMiddle_left;
#endif









