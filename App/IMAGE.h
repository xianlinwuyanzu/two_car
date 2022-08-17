#ifndef __image_H
#define __image_H 
#include "common.h"
#include "include.h"



#define RowMax     100             //高度
#define ColumnMax  80    //宽度//70

#define Black        30
#define White        200 
 extern  unsigned char ThresholdData[RowMax][ColumnMax];
/*函数声明*/
void camera_img_deal(uint8* imgbuff);

void MidLineProcess(void);			// 提取中心线并处理
void GetLMR(void);                             //提取左边缘、右边缘和中心线
void LAverageFilter();
void RAverageFilter();
void GetFinalMidLine();
void MidLineCompensate();
void GetMidLineVariance();
float GetSteerError(unsigned char start,unsigned char end,float midpos);//获取中心线偏差
void GetSpecialError();
void GetBlackEndParam();// 提取图像特征参数 
void GetBlackEndParam2();// 提取图像特征参数 
void GetSectionParam();
void StoreMidLine();
void UseTemMidLine();
void CrossingMidFilter(); 
void RTRecognition();  //赛道识别    
void HistoryRTProccess();
int fabss(int n);
void ImageProcess();  
void GetLMR2();    
void GetLMR3(); 
void AverageFilter();//中心线均值滤波   
void ProcessCrossing();  //检测并处理十字 
void ProcessCrossing2();
void shizi1();
void StartLine();
extern signed int BlackLineData[RowMax];// 提取黑线值数
extern int StableNumbers;//固定值一
extern int StableNumbers2;//固定值一

extern int LeftStableNumbers;//左固定值
extern int RightStableNumbers;// 左固定值
extern signed int RoadType;//赛道类型
extern unsigned char IsCrossing;
extern signed int LeftBlack[RowMax];	//左边缘黑线数组					 
extern int RightBlack[RowMax];//右边缘黑线数组	
void CommonRectificate(int data[],unsigned char begin,unsigned char end);
void SCProcessing();
void Findjump();
void GetCrossingMidLine();
void ScanRoad();
//void CommonRectificate();
//extern int BlackLineData[RowMax];// 提取黑线值数
//extern int LeftBlack[RowMax];	//左边缘黑线数组					 
//extern int RightBlack[RowMax];//右边缘黑线数组


#endif
