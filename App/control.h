#ifndef __control_H
#define __control_H 

#include "include.h"
#include "int.h"
#include "irq.h"
#include "control.h"
#include "IMAGE.h"
#include "pid.h"
extern unsigned int BlackEndM;
extern unsigned int LeftEnd;
extern unsigned int RightEnd;
extern unsigned int BlackEndMax;
extern unsigned int BlackEndLMR;
extern signed char TripPointPos[RowMax];
extern unsigned char IsCrossing;
void DirectionCtrol(void);
void SpeedCtrol();
void MotorUpdatePWM();
#endif