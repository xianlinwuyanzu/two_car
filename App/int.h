#ifndef __INT_H
#define __INT_H 


#include "common.h"
#include "include.h"
/*
 * 求最大值和最小值
 */
#define Max( x, y ) ( ((x) > (y)) ? (x) : (y) )
#define Min( x, y ) ( ((x) < (y)) ? (x) : (y) )

/*
void CAMER_INIT();
void MotorInit();
void SteerInit();
void SwitchInit();
void PID_init();
void BianmaInit();
void delay_start(int z);*/
void Init();
void SwitchInit();
void MotorInit();
void BianmaInit();
void SteerInit();
void CameraInit();
void Boma1();

#endif
