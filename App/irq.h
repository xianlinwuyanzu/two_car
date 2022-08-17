#ifndef __irq_H
#define __irq_H 

#include "common.h"
#include "include.h"

extern uint8 imgbuff[CAMERA_SIZE];                             //定义存储接收图像的数组
extern uint8 img[CAMERA_H][CAMERA_W];






//函数声明
void PORTA_IRQHandler();
void DMA0_IRQHandler();

#endif
