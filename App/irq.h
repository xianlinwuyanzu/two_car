#ifndef __irq_H
#define __irq_H 

#include "common.h"
#include "include.h"

extern uint8 imgbuff[CAMERA_SIZE];                             //����洢����ͼ�������
extern uint8 img[CAMERA_H][CAMERA_W];






//��������
void PORTA_IRQHandler();
void DMA0_IRQHandler();

#endif
