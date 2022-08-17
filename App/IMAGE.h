#ifndef __image_H
#define __image_H 
#include "common.h"
#include "include.h"



#define RowMax     100             //�߶�
#define ColumnMax  80    //���//70

#define Black        30
#define White        200 
 extern  unsigned char ThresholdData[RowMax][ColumnMax];
/*��������*/
void camera_img_deal(uint8* imgbuff);

void MidLineProcess(void);			// ��ȡ�����߲�����
void GetLMR(void);                             //��ȡ���Ե���ұ�Ե��������
void LAverageFilter();
void RAverageFilter();
void GetFinalMidLine();
void MidLineCompensate();
void GetMidLineVariance();
float GetSteerError(unsigned char start,unsigned char end,float midpos);//��ȡ������ƫ��
void GetSpecialError();
void GetBlackEndParam();// ��ȡͼ���������� 
void GetBlackEndParam2();// ��ȡͼ���������� 
void GetSectionParam();
void StoreMidLine();
void UseTemMidLine();
void CrossingMidFilter(); 
void RTRecognition();  //����ʶ��    
void HistoryRTProccess();
int fabss(int n);
void ImageProcess();  
void GetLMR2();    
void GetLMR3(); 
void AverageFilter();//�����߾�ֵ�˲�   
void ProcessCrossing();  //��Ⲣ����ʮ�� 
void ProcessCrossing2();
void shizi1();
void StartLine();
extern signed int BlackLineData[RowMax];// ��ȡ����ֵ��
extern int StableNumbers;//�̶�ֵһ
extern int StableNumbers2;//�̶�ֵһ

extern int LeftStableNumbers;//��̶�ֵ
extern int RightStableNumbers;// ��̶�ֵ
extern signed int RoadType;//��������
extern unsigned char IsCrossing;
extern signed int LeftBlack[RowMax];	//���Ե��������					 
extern int RightBlack[RowMax];//�ұ�Ե��������	
void CommonRectificate(int data[],unsigned char begin,unsigned char end);
void SCProcessing();
void Findjump();
void GetCrossingMidLine();
void ScanRoad();
//void CommonRectificate();
//extern int BlackLineData[RowMax];// ��ȡ����ֵ��
//extern int LeftBlack[RowMax];	//���Ե��������					 
//extern int RightBlack[RowMax];//�ұ�Ե��������


#endif
