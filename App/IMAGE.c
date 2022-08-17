
#include "common.h"
#include "include.h"
#include "int.h"
#include "irq.h"
#include "control.h"
#include "IMAGE.h"
#include "pid.h"
extern int MidPos;
int cs=0;
float FLAG;   //1 直道    2  十字
unsigned char CAR=2;
unsigned int duoji=0;  //打死舵机标志
unsigned int zdczrc1=0;
unsigned int zdczrc2=0;
unsigned int zdczlc1=0;
unsigned int zdczlc2=0;
unsigned int zdflag=0;
unsigned int zdczr=0;
unsigned int zdczl=0;
unsigned int CFFlag=1;
unsigned int CFCount=0;
unsigned int b=0;
unsigned int aa=0;
unsigned int EndC=0;
unsigned int u=0;
unsigned int uw=0;
unsigned char Leftuw=0;
unsigned char Rightuw=0;
int Start=0;
int xy=0;
unsigned int shizi;
unsigned int Rightnumber=0,Leftnumber=0;
unsigned int BlackEndL = 0;//左边黑线行数  
unsigned int BlackEndM;//中间黑线长度
unsigned int BlackEndR = 0;//右边黑线行数
unsigned int BlackEndMax = 0;//黑线处理的最大行数
unsigned int BlackEndLMR = 0;//经过处理后的中间黑线
int BlackEndA = 0;
int BlackEndB = 0;
int BlackEndC = 0;
int BlackEndD = 0;
int BlackEndE = 0;
int BlackEndF = 0;
int BlackEndG = 0;
int BlackEndColumnMax=1;
unsigned int Stablenumber;
unsigned char LineType[RowMax] = {0};
unsigned char BlackRow=0;
unsigned char WhiteRow=0;
unsigned char WhiteRow1=0;
unsigned char g_Derict = 0; //方向 正-0 左-1 右-2
signed int LeftBlack[RowMax];	//左边缘黑线数组					 
signed int RightBlack[RowMax];//右边缘黑线数组	
signed int BlackLineData[RowMax];// 提取黑线值数据
unsigned char IsCrossing;

signed char SubValue[RowMax];
signed char TripPointPos[RowMax];

unsigned char g_BasePos = ColumnMax/2;
unsigned char ValidLineR[RowMax]={0}; 			//十字道右边行有效标志数组
unsigned char ValidLineL[RowMax]={0};			//十字道左边行有效标志数组
unsigned char NoValidLMax = 0;					//十字交叉道左边连续丢线计数
unsigned char NoValidRMax = 0;					//十字交叉道右边连续丢线计数
unsigned char CrossingStable = 0;

unsigned char bFoundTripPoint = 0;

int LCrossingTripPos = 0;
int RCrossingTripPos = 0;
int sanjiaol=0;
int sanjiaor=0;
int StableNumbers = 0;
int StableNumbers2 = 0;
int CompensateCount = 0;

int StoreFlag = 0;
int LeftStableNumbers = 0;
int RightStableNumbers = 0;

int ValidLineCount1 = 0; //左边缘找到并且右边找到
int ValidLineCount2 = 0;//左边缘找到或者右边找到

double MidLineVariance = 0.0;
int MidLineExcursion = 0;

int Excursion = 0;
float EPerCount=0.0;
//signed char RoadType = -1;  //赛道类型
signed char LastRoadType = 0;	
unsigned char StandardRoadType = 1;

unsigned char tuxiang[RowMax][ColumnMax];
unsigned char ThresholdData[RowMax][ColumnMax];

void ImageProcess()
{
  int i=0;
  //int j=0;
  unsigned char *p=0;
//   unsigned char *q=0;
//  for (p = &ThresholdData[0][0],q = &ThresholdData[0][0]; p <= &ThresholdData[RowMax-1][ColumnMax-1]; p++,q++)
//	{
//		if (*p > ThresholdValue)
//		{
//			*q = White;
//		}
//		else{*q = Black;}
//	}
//  
         //噪点滤波 白黑白 => 白白白   黑白黑=>黑黑黑
  for (i = 0;i <CAMERA_H-1;i++)//V是列 H是行
  {
    p = &ThresholdData[i][1];
    while (p < &ThresholdData[i][CAMERA_W-1])
    {
      if (*(p-1) == White && *p == Black && *(p+1) == White//白黑白
          &&*(p-CAMERA_W) == White&&*(p-CAMERA_W-1) == White&&*(p-CAMERA_W+1) == White
            &&*(p+CAMERA_W) == White&&*(p+CAMERA_W-1) == White&&*(p+CAMERA_W+1) == White                            
              )
        
      {
        *p = White;
      }
      else if (*(p-1) == Black && *p == White && *(p+1) == Black)
      {
        *p = Black;
      }
      p++;
    }
  }
          
}
//int pianyi[ColumnMax]={1684,1594,1506,1422,1339 ,1259 ,1182 ,1107 ,1034 ,964 ,  896 , 831 , 769 ,708 ,  651 ,595 , 542 , 492 , 444 , 398 ,  355 , 315 , 277 ,241 ,  208 ,177 , 149 , 123 , 100 , 79 ,   60 ,   44 , 31 ,  20 ,   11 ,5  ,   1 ,   0 ,   1 ,   5 ,   11 ,   20 , 31 ,  44 ,   60 , 79 ,  100 , 123 , 149 , 177 ,  208 ,  241 ,277 ,315 ,   355 ,398 , 444 , 492 , 542 , 595 ,  651 ,  708 ,769 ,831 ,   896 ,964 ,1034 ,1107 ,1182 ,1259 ,  1339 , 1422 ,1506 ,1594 ,1684 , 1776 ,1870 ,1968 ,2067 }; 

int pianyid[ColumnMax]= {18,17,16, 16, 15, 13, 13, 12, 12, 10,    //近处，图像下半部分
                         10, 9, 7, 7, 7, 6, 6, 4, 4, 4, 
                         4, 3, 3, 3, 3, 2, 2, 2, 2, 1,
                         1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 
                         0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
                         1, 2, 2, 2, 2, 3, 3, 3, 3, 4,
                         4, 4, 4, 6, 6, 7, 7, 7, 11, 10,
                        11, 11, 12, 12, 13, 15, 17, 17,18,18};

int pianyiu[ColumnMax]= { 0, 0, 2,  2, 2, 2, 2, 4, 4, 4,  //远处  图像上半部分
                          5,  5, 5, 5, 6, 6, 6, 6, 6, 6,
                          6,  6, 7, 7, 7, 8, 8, 8, 8, 8,
                          8,  8, 8, 8, 8, 8, 9, 9, 9, 9,
                          9,  9, 9, 9, 8, 8, 8, 8, 8, 8,
                          8,  8, 8, 8, 8, 7, 7, 7, 6, 6,
                          6,  6, 6, 6, 6, 6, 5, 5, 5, 5,  
                          4, 4, 4, 2, 2, 2, 2, 2, 0, 0};

void camera_img_deal(uint8* imgbuff)    //xiangcxzq解压像素点
{
	int i,j,num=200,cnt=0,mod;
	mod=0x80;
        int jj=0;

                
        for(i=0;i<100;i++)
	{
		for(j=0;j<CAMERA_W;j++)
		{
			tuxiang[i][j]=(imgbuff[num]&mod);
	                if(tuxiang[i][j]==0)tuxiang[i][j]=White;else tuxiang[i][j]=Black;
			mod=mod>>1;
			cnt++;
			if(cnt>=8)
			{
				mod=0x80;
				cnt=0;
				num++;
			}
		}       
	}
         //直道最远处是黑，五行左右
        for(i=0;i<100;i++)
        {
          for(j=0;j<CAMERA_W;j++)
          {
            if(i>50) ThresholdData[i][j]=tuxiang[i-(int)((i-50)*pianyid[j]/50)][j];
            else if(i<50)  
              if((i-(int)((50-i)*pianyiu[j]/50))>0)   ThresholdData[i][j]=tuxiang[i-(int)((50-i)*pianyiu[j]/50)][j];
              else ThresholdData[i][j]=tuxiang[i][j];
              
          else     
              ThresholdData[i][j]=tuxiang[i][j];
          }
        
        }
//    
        //隔行采
//    for(i=0;i<60;i++)
//      {
//	for(j=0;j<CAMERA_W;j++)
//		{
//			ThresholdData[i][j]=(imgbuff[num]&mod);
//	                if(ThresholdData[i][j]==0)ThresholdData[i][j]=White;else ThresholdData[i][j]=Black;
//			mod=mod>>1;
//			cnt++;
//			if(cnt>=8)
//			{
//				mod=0x80;
//				cnt=0;
//				num++;
//			}
//		}
//        if(i<20)
//          num+=10; 
//        else if(i>=20&&i<40)
//           num+=20;
//        else if(i>=40&&i<60)
//          num+=30;
//        
//        
//                  
//	}
        
//     for(i=0;i<90;i++)
//      {
//	for(j=0;j<CAMERA_W;j++)
//		{
//			tuxiang[i][j]=(imgbuff[num]&mod);
//	                if(tuxiang[i][j]==0)tuxiang[i][j]=White;else tuxiang[i][j]=Black;
//			mod=mod>>1;
//			cnt++;
//			if(cnt>=8)
//			{
//				mod=0x80;
//				cnt=0;
//				num++;
//			}
//		}
//        if(i<50)
//          num+=10; 
////        else if(i<40)
////           num+=20;
//        else if(i<80)
//          num+=30;
//        
//        
//                  
//	}
        
        
      // 5  i<100                 20
      // 4  100<i<180             20
    //   3  180<i<240             20
}


void GetBlackEndParam()
{
	unsigned char LEndFlag = 0;    unsigned char LLEndFlag = 0;     unsigned char LREndFlag = 0;
	unsigned char MEndFlag = 0;    unsigned char MLEndFlag = 0;     unsigned char MREndFlag = 0;
	unsigned char REndFlag = 0;    unsigned char RLEndFlag = 0;     unsigned char RREndFlag = 0;
        unsigned int BlackEndStable=0;
	int i=0,j=0;
        int BlackEndML=0,BlackEndMR=0, BlackEndLL = 0,BlackEndLR = 0,BlackEndRL=0,BlackEndRR=0;
        EndC=0;
	BlackEndL = 0;
	BlackEndM = 0;
	BlackEndR = 0;
        Stablenumber = 0;
     
	for (i = RowMax-1; i >= 0 ; i--)
	{
          //中间黑线截止行
		if(ThresholdData[i][ColumnMax/2] == White && !MEndFlag ){
			BlackEndM++;
		}
		else if(i > 1 && ThresholdData[i-1][ColumnMax/2] == Black && ThresholdData[i-2][ColumnMax/2] == Black){
			
                  if(i>95) BlackEndM++;
                   else  MEndFlag = 1;
		}
 
                if(ThresholdData[i][ColumnMax/2-7] == White && !MLEndFlag ){
			BlackEndML++;
		}
		else if(i > 1 && ThresholdData[i-1][ColumnMax/2-7] == Black && ThresholdData[i-2][ColumnMax/2-7] == Black){
		  if(i>95) BlackEndML++;	
                  else MLEndFlag = 1;
		}
                
               if(ThresholdData[i][ColumnMax/2+7] == White && !MREndFlag ){
			BlackEndMR++;
		}
		else if(i > 1 && ThresholdData[i-1][ColumnMax/2+7] == Black && ThresholdData[i-2][ColumnMax/2+7] == Black){
		if(i>95) BlackEndMR++;	
                else MREndFlag = 1;
		}
                
                
                //左边黑线截止行
		if(ThresholdData[i][ColumnMax/4] == White && !LEndFlag ){
			BlackEndL++;
		}
		else if(i > 1 && ThresholdData[i-1][ColumnMax/4] == Black && ThresholdData[i-2][ColumnMax/4-5] == Black){
		 if(i>95) BlackEndL++;	
                else  LEndFlag = 1;
		}
                
                if(ThresholdData[i][ColumnMax/4-5] == White && !LLEndFlag ){
			BlackEndLL++;
		}
		else if(i > 1 && ThresholdData[i-1][ColumnMax/4-5] == Black && ThresholdData[i-2][ColumnMax/4-5] == Black){
		 if(i>95) BlackEndLL++;	
                else  LLEndFlag = 1;
		}
                
                if(ThresholdData[i][ColumnMax/4+5] == White && !LREndFlag ){
			BlackEndLR++;
		}
		else if(i > 1 && ThresholdData[i-1][ColumnMax/4+5] == Black && ThresholdData[i-2][ColumnMax/4+5] == Black){
		 if(i>95) BlackEndLR++;	
                else  LREndFlag = 1;
		}
                
                //右边黑线截止行
		if(ThresholdData[i][ColumnMax*3/4] == White && !REndFlag ){
			BlackEndR++;
		}
		else if(i > 1 && ThresholdData[i-1][ColumnMax*3/4] == Black && ThresholdData[i-2][ColumnMax*3/4] == Black){
                  if(i>95) BlackEndR++;			
                  else  REndFlag = 1;
		}
                
                if(ThresholdData[i][ColumnMax*3/4-5] == White && !RLEndFlag ){
			BlackEndRL++;
		}
		else if(i > 1 && ThresholdData[i-1][ColumnMax*3/4-5] == Black && ThresholdData[i-2][ColumnMax*3/4-5] == Black){
                  if(i>95) BlackEndRL++;			
                  else  RLEndFlag = 1;
		}
                
                if(ThresholdData[i][ColumnMax*3/4+5] == White && !RREndFlag ){
			BlackEndRR++;
		}
		else if(i > 1 && ThresholdData[i-1][ColumnMax*3/4+5] == Black && ThresholdData[i-2][ColumnMax*3/4+5] == Black){
                  if(i>95) BlackEndRR++;			
                  else  RREndFlag = 1;
		}
	}
        BlackEndM = MIN(BlackEndML,BlackEndM);
        BlackEndM = MIN(BlackEndMR,BlackEndM);
	
        BlackEndL=MAX(BlackEndL,BlackEndLR);
        BlackEndL=MAX(BlackEndL,BlackEndLL);
        
        BlackEndR=MAX(BlackEndR,BlackEndRL);
        BlackEndR=MAX(BlackEndR,BlackEndRR);
        
        
        if(BlackEndM<BlackEndL && BlackEndM<BlackEndR){
          BlackEndM=0;MEndFlag=0;
         // sanjiao=1;
          
          if(BlackEndM == BlackEndML)
            EndC=ColumnMax/2-7;
          else if(BlackEndM == BlackEndMR)
             EndC=ColumnMax/2+7;
          else
            EndC=ColumnMax/2;
        
          for(i=RowMax-1;i>0;i--)
          {
            if(ThresholdData[i][EndC] == White && !MEndFlag)
              BlackEndMax++;
          if(ThresholdData[i][EndC] == White && ThresholdData[i-1][EndC] == Black && !MEndFlag ){
			MEndFlag=1;
                          i-=2;BlackEndM++;
		}
          if((ThresholdData[i][EndC] == White || ThresholdData[i][EndC] == Black)&& MEndFlag )
            BlackEndMax++;
          if(ThresholdData[i][EndC] == White && ThresholdData[i-1][EndC] == Black && MEndFlag ){
			break;
		
		}
        }
        }
       // if(sanjiao)
          aa++;
         OLED_Print_Num(60,6,aa);
//        }
//        else 
//        {
//        BlackEndM = MAX(BlackEndML,BlackEndM);
//        BlackEndM = MAX(BlackEndMR,BlackEndM);
//        }
//        //OLED_Print_Num(0,4,BlackEndM);
//        BlackEndMax = MAX(BlackEndL,BlackEndM);
//	BlackEndMax = MAX(BlackEndMax,BlackEndR);
//	BlackEndLMR = BlackEndL + BlackEndM + BlackEndR;
//        //OLED_Print_Num(0,0,BlackEndMax);
////		//判断十字左右倾
//	  //		//判断十字左右倾
//  if (BlackEndM == BlackEndML)
//  {
//    g_Derict = 1;//左倾
//  }
//  else if (BlackEndM == BlackEndMR)
//  {
//    g_Derict =2;//右倾
//  }
//  else 
//  {
//    if (fabss(BlackEndL-BlackEndR) < 5)
//    {
//      g_Derict = 0;//正十字
//    }
//    else if (BlackEndL > BlackEndR)
//    {
//      g_Derict = 1;//左
//    }
//    else
//    {
//      g_Derict = 2;//右
//    }
  //}
 // OLED_Print_Num(0,4,g_Derict); 
      
        //找最长行无效
        
//   for(j = 1;j < ColumnMax;j++)
//      {
//    for (i = RowMax-1; i >= 2 ; i--)
//	{
//		if(ThresholdData[i][j] == White && !MEndFlag ){
//			BlackEndStable++;
//                        
//		}
//		else if(i > 1 && ThresholdData[i][j] == Black && ThresholdData[i-1][j] == Black){
//                  
//			break;
//		}
//                else{
//                  MEndFlag = 1;
//                }
//		
//	}
//    Stablenumber = Max(Stablenumber,BlackEndStable);
//    
//    BlackEndStable=0;
//}
//OLED_Print_Num(0,6,Stablenumber); 
}

//有效提取偏差线
void GetExcursionLine()
{
	int i=0,j=0;
	int CountBlack = 0;
	int CountWhite = 0;

	//每行头尾遍历行指针
	unsigned char pLeft = 0,pRight = ColumnMax - 1;
	
	BlackRow=0;
	WhiteRow=0;
	WhiteRow1=0;	
	for (i = 0;i < RowMax;i++)
	{
		//初始化遍历指针
		pLeft = 0;
		pRight = ColumnMax - 1;
		LeftBlack[i] = pLeft;
		RightBlack[i] = pRight;
		CountBlack = 0;
		CountWhite = 0;

		//两条黑线边缘提取
		for (j = 0;j <= ColumnMax/2;j++)
		{
			// 从左到右搜索边缘
			if (LeftBlack[i] == 0){
				if (ThresholdData[i][pLeft] == Black){CountBlack++;}
				else{CountWhite++;}
				// 未找到左边缘则寻找
				if (ThresholdData[i][pLeft] != ThresholdData[i][pLeft+3]){
					//确认检测到边缘
					if (ThresholdData[i][pLeft+1] != ThresholdData[i][pLeft+4]
						&& ThresholdData[i][pLeft+2] != ThresholdData[i][pLeft+5]){
						//找到左边缘
						LeftBlack[i] = pLeft + 2;
						//如果右边缘也找到则退出
						if (RightBlack[i]!=ColumnMax-1){
							break;
						}
					}
					else{
						pLeft++;
					}
				}
				else{
					pLeft++;
				}
			}// if结束--从左到右搜索边缘
			
			//从右到左搜索边缘
			if (RightBlack[i] == ColumnMax - 1)
			{
				if (ThresholdData[i][pRight] == Black) {CountBlack++;}
				else {CountWhite++;}

				if (ThresholdData[i][pRight] != ThresholdData[i][pRight-3])
				{
					//确认检测到边缘
					if (ThresholdData[i][pRight-1] != ThresholdData[i][pRight-4]&& ThresholdData[i][pRight-2] != ThresholdData[i][pRight-5])
					{
						// 找到右边缘
						RightBlack[i] = pRight - 2;
						// 如果左边缘也找到则退出
						if (LeftBlack[i]!=0)
						{
							break;
						}
					}
					else
					{
						pRight--;
					}
				}
				else
				{
					pRight--;
				}
			}// if结束--从右到左搜索边缘
			// 无边缘则跳出
			if (pLeft >= pRight)
			{
				LeftBlack[i] = RightBlack[i] = pLeft;
				//判断记录全黑行
				if(CountBlack >=ColumnMax - 5)
				{
					CountBlack = 0;
					LineType[i] = 1;//全黑行标志
					BlackRow++;
				}
				else if(CountWhite >= ColumnMax - 5 )
				{
					CountWhite = 0;
					WhiteRow1++;
					
					if (i < RowMax-5 && i > 35)//近端10行不检测全白行，除掉前端盲区
					{
						WhiteRow++;
						LineType[i] = 2;//全白行标志
					}
					else
					{
						LineType[i] = 0;	
					}
				}
				else
				{
					LineType[i] = 0;	
				}
				break;
			}
		}
	}
}


int fabss(int n)
{
	if(n < 0)return (-1)*n;
	else return n;	
}

int x=0;
int kk=0;
int kkk=0;
int js=0;
int jsf=0;
int dd=0;
int LastCAR=0;
//中心线处理
void MidLineProcess()
{
        //zdczl=zdczr=0;
        StoreFlag = 0;
        //OLED_Print_Num(0,0,BlackEndMax);

	GetLMR();//提取左边缘、右边缘和中心线 
        
if(zdczl || zdczr)
   {
      if(CAR==0)
      {
        kkk++;
        if(kkk>10)
        {
        kkk=0;
        zdflag=1;
        zdczl=zdczr=0;
        }
      }
      else
        if(CAR==4)
      {
       kkk++;
      if(kkk>15)
  {
    zdflag=1;
    CAR=0;
    uart_putchar(UART0,CAR);
    CAR=1;zdczl=zdczr=0;kkk=0;}
   }
      else if(CAR==3)
      {
      dd++;
      if(dd>200)
      {zdczl=zdczr=0;CAR=0;uart_putchar(UART0 , CAR);dd=0;}
      }
      else
      {zdczl=zdczr=0;}
   }
   
   
   
    if(sanjiaol || sanjiaor)  
     { 
//       if(CAR==2 && LastCAR==4)
//       {
//         LastCAR=0;
//       CAR=0;
//       }
//    else
       if(CAR==2)
     {
       LastCAR=0;
     kk++;
     if(kk>10)
     {
       duoji=1;
//       CAR=0;
//      uart_putchar (UART0 , CAR);
      
     }
    if(kk>20)
    {
      sanjiaol=sanjiaor=0;
      duoji=0;
      kk=0;
      CAR=1;
     }
     }
     else
     {
       if(CAR==1)
       {
       CAR=2;
       uart_putchar (UART0 , CAR);
       CAR=0;
       LastCAR=5;
       }
//       else  if(CAR==3)
//         CAR=0;
      sanjiaol=sanjiaor=0;
     
     }

     }
     
//    if(zdczr || zdczl)
//    {
//    aa++;
//    OLED_Print_Num(60,6,aa);
//    }
//
//    if(sanjiaol || sanjiaor)
//    {
//     aa++;
//    OLED_Print_Num(60,4,aa);
//    }
		//LAverageFilter();//左边缘滤波
		//RAverageFilter();//右边缘滤波
//          if( !zdczl && !zdczr && BlackEndMax>RowMax*0.7)
//            ScanRoad();
  // GetLMR();
         if(!zdczl && !zdczr && shizi)
            ProcessCrossing2(); 
         
        else
    	GetFinalMidLine();//通过左右边缘取得控制舵机的中心线  
//      
        if(!jsf)
        {js++;
        if(js>200)
          jsf=1;
        }
    if(jsf && StableNumbers>RowMax*0.8 && !shizi  && !sanjiaol && !sanjiaor)
          StartLine();
//   

      
        
		//AverageFilter();//中心线滤波处理
		//MidLineCompensate();//中心线补偿处
//		StoreFlag=1;
//		StoreMidLine();//保存中心线
//
//		
		//GetSectionParam();//对中心线分段提取偏差   
		GetMidLineVariance();//提取中心线方差
	//}
//	else
//	{
//		//图像非正常情况，提取控制舵机的特殊情况偏差
//		GetSpecialError();	
//	}

//	
//	ProcessCrossing();  
//        //检测并处理十字
//	if (!IsCrossing)
	//if(StableNumbers<3){
			//UseTemMidLine();
		
////		DebugShow1();
	//}
//	else
//	{
//		CrossingMidFilter();
//		GetSectionParam();//对中心线分段提取偏差
////		DebugShow2();
//	}
}
unsigned char ValidLine[RowMax] = {0};//1 - 左找到线 2--右找到线  两边都找到线-3 都找不到线-0
int NoValidMax = 0;//连续两边找不到线计数

int RoadWidth[8] = {40,39,38,37,36,35,33,32};
//   63    (64-17)

int zaw_mode=0;
int mode4_zhangai=0;
int RightRow=0,LeftRow=0;
int sf[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int s=0;
int sanjiaonumber=0;
int zhidaonumber=0;
unsigned int LeftEnd = 0;
unsigned int RightEnd = 0;
unsigned int MidEnd = 0;
void GetLMR()
{
   int mode4=0;
  int csz=0;
  int zd=0;
  int z=0; 
  int a=0;
  unsigned char Lf=0;
  unsigned char Rf=0;
  unsigned char zdczlf=0;
  unsigned char zdczrf=0;
  unsigned int Roadwide=0;
  unsigned int zdcznumberl=0;
  unsigned int zdcznumberr=0;

  unsigned int FirstL=0;
  unsigned int FirstR=0;
     
zaw_mode=0;
	 LeftEnd = 0;
	 RightEnd = 0;
	 MidEnd = 0;

	int NoValidCount = 0;

	NoValidMax = 0;
        RightRow=0,LeftRow=0;
	//StableNumbers = 0;
	LeftStableNumbers = 0;  
	RightStableNumbers = 0;
	
	ValidLineCount1 = 0;
	ValidLineCount2 = 0;
        Leftuw=0;
        Rightuw=0;
        shizi = 0;
        u=0;
        cs=0;
	Rightnumber=0;Leftnumber=0;
int i=0,j=0;
  int WhiteEnd=0,End=0;
  unsigned char FoundBlack=0;
  //EndC=0;
 End=0;
 int lastEnd=0;
 int latterEnd=0;
  for(j=14;j<ColumnMax-14;j+=3)
  {
    
    WhiteEnd=0;
    FoundBlack=0;
    for(i=RowMax-1;i>0;i-=3){
    if(ThresholdData[i][j] == White && !FoundBlack)
      WhiteEnd+=3;
    if((ThresholdData[i][j] == White && ThresholdData[i-1][j] == Black) || (ThresholdData[i][j] == Black)){

       FoundBlack=1;
        break;
     
    }
  
    }
    
    if(j==ColumnMax/4-3)
  {
    LeftEnd=WhiteEnd;
  }
  if(j==ColumnMax/4)
  {
    if(LeftEnd>WhiteEnd)
      LeftEnd=WhiteEnd;
  }
  else if(j==ColumnMax/4+3)
    {
      if(LeftEnd>WhiteEnd)
      LeftEnd=WhiteEnd;
    }
  else if(j==ColumnMax/2-5)
  {
    MidEnd=WhiteEnd;
  
  }
   else if(j==ColumnMax/2+1)
    {
      if(MidEnd>WhiteEnd)
      MidEnd=WhiteEnd;
    }
  else if(j==ColumnMax/2+4)
  {
      if(MidEnd>WhiteEnd)
      MidEnd=WhiteEnd;
  
  }
    else if(j==ColumnMax*3/4-4)
      RightEnd=WhiteEnd;
  else if(j==ColumnMax*3/4+2)
  {
  if(RightEnd>WhiteEnd)
     RightEnd=WhiteEnd;
  
  }
  else if(j==ColumnMax*3/4+5)
  {
  if(RightEnd>WhiteEnd)
     RightEnd=WhiteEnd;
    
  }
    
   
    if(WhiteEnd>End){
      
      End=WhiteEnd;
      EndC=j;
    }
    
    
    
   
    WhiteEnd=0;

      
  StableNumbers=End-3;
  BlackEndMax=End;
  }
  
    
  for(i=RowMax-1;i>RowMax-End+1;i--)
  {
    LeftBlack[i]=0;RightBlack[i]=0;BlackLineData[i]=0;
    for(j=EndC;j>0;j--)
    {
      if(ThresholdData[i][j] == White && ThresholdData[i][j-1] == Black){
        LeftBlack[i]=j-1;
        if(LeftBlack[i]>0)
        break;
      } 
  
    }
    for(j=EndC;j<ColumnMax-1;j++){
      if(ThresholdData[i][j] == White && ThresholdData[i][j+1] == Black){
        RightBlack[i]=j+1;
        if(RightBlack[i]<ColumnMax-1)
        break;
      }
  }
  
 if (!LeftBlack[i]) 
		{ 
			if (i < 8)
			{
				LeftBlack[i] = LeftBlack[i+1] + LeftBlack[i+1] - LeftBlack[i+2]; 
			}
			else
			{
				LeftBlack[i] = 0; 
			}
		}
		else if (i < 8 && fabss(LeftBlack[i] - LeftBlack[i+1]) > 4 )
		{
			LeftBlack[i] = LeftBlack[i+1] + LeftBlack[i+1] - LeftBlack[i+2];
		}

        if (!RightBlack[i] || RightBlack[i]==ColumnMax-1)
		{
			if (i < 8)
			{ 
				RightBlack[i] = RightBlack[i+1] + RightBlack[i+1] - RightBlack[i+2];
			}
			else
			{
				RightBlack[i] = ColumnMax-1; 
			}
		}
		else if (i < 8 && fabss(RightBlack[i] - RightBlack[i+1]) > 4 )
		{
			RightBlack[i] = RightBlack[i+1] + RightBlack[i+1] - RightBlack[i+2];
		}
        if(LeftBlack[i]>=RightBlack[i])
            LeftBlack[i] = LeftBlack[i+1] + LeftBlack[i+1] - LeftBlack[i+2]; 
//障碍处理
 if(LeftBlack[i]>1&&RightBlack[i]<ColumnMax-2) {mode4++;}
    
    if(mode4>=RowMax-3-i)
 {
   if(fabss(LeftBlack[i]-LeftBlack[i+1])<=4&&(RightBlack[i+1]-RightBlack[i])>4 && (LeftBlack[i]>1&&RightBlack[i]<ColumnMax-2))mode4_zhangai=i;    
   if(fabss(RightBlack[i]-RightBlack[i+1])<=4&&(LeftBlack[i]-LeftBlack[i+1])>4 && (LeftBlack[i]>1&&RightBlack[i]<ColumnMax-2))mode4_zhangai=-i;
   if(mode4_zhangai<0&&(mode4_zhangai*-1-i)>5 && (LeftBlack[i]>1&&RightBlack[i]<ColumnMax-2)&&fabss(RightBlack[i]-RightBlack[i+1])<=4//确定左侧有障碍
      &&(LeftBlack[i+1]-LeftBlack[i])>4      
      )
   {mode4_zhangai=0;
    zaw_mode=-6;
    }
   
    if(mode4_zhangai>0&&(mode4_zhangai-i)>5&&(LeftBlack[i]>1&&RightBlack[i]<ColumnMax-2)&&fabss(LeftBlack[i]-LeftBlack[i+1])<=4//确定右侧有障碍
      &&(RightBlack[i]-RightBlack[i+1])>4      
      )
   {mode4_zhangai=0;
   zaw_mode=6;
    }
 
 
 
 
 
 }
    
    
    
if (!LeftBlack[i])
   {
//     if(LeftStableNumbers>5 && !Leftnumber && !Lf)
//       Lf=1;
     LeftStableNumbers++;
     
   }
   else 
   {
//     if(Lf)
//       Leftuw=1;
//     if(!FirstL && LeftStableNumbers>5)     //如果一边下面先没有找到，然后找到，则这边有效行就等于二者相加
//     {
//     Leftnumber=LeftStableNumbers+Leftnumber;
//     }
     Leftnumber++;
    // FirstL=1;
     
   }    // else  {LeftStableNumbers=0; Leftnumber++;}
   
   
	if (!RightBlack[i] || RightBlack[i] == ColumnMax-1)
        {
//          if(RightStableNumbers>8 && !Rightnumber && !Rf)
//            Rf=1;
          RightStableNumbers++;
         
          
        }
   else  {
//     if(Rf && Rightnumber>15 && i>30)
//       Rightuw=1;       // 有种S一边只有中间丢线
//    if(!FirstR &&  RightStableNumbers>30 )   //若一边最下面丢线，上面不丢，则这边有效数等于二者和
//    {
//    
//    Rightnumber=RightStableNumbers+Rightnumber;
//    }
    
     Rightnumber++;
     //FirstR=1;
   }
if(i>RowMax-40 && ((Rightnumber<2 && Leftnumber>20)||(Rightnumber>20 && Leftnumber<2)))
csz=1;
   if(i>RowMax-20 && (Rightnumber>15 || Leftnumber>15) && !cs)
     cs=1;
   
     if(i>RowMax-20 && Rightnumber>18 && Leftnumber>18 && !zd)
       zd=1;
       
   //补充检测十字
   
   //大于15保证在十字，不和S弄混
if(i<RowMax-15 &&((LeftBlack[i]==0 && RightBlack[i]==ColumnMax-1) 
                  ||(LeftBlack[i]==0 && RightBlack[i]>RightBlack[i+1])
                  ||((RightBlack[i]==ColumnMax-1 ||RightBlack[i]==0) && (LeftBlack[i]<LeftBlack[i+1])) 
                  ||((LeftBlack[i]<LeftBlack[i+1] ) && (RightBlack[i]>RightBlack[i+1])) ))
   {
   NoValidCount++;
   if(NoValidCount>3)
   {shizi=1;
   }
   
   }
   else
     NoValidCount=0;
//if((LeftEnd<25 || RightEnd<25 || MidEnd<25) && ThresholdData[RowMax-2][2] == White && ThresholdData[RowMax-2][ColumnMax-2] == White)
//  shizi=1;
//
 if(i>RowMax-25)   //U形弯 一边下面全无效，上面重新有效。防止和弯道弄混
   {

      if(LeftStableNumbers>23)      //S时左边丢线同时近处右边线减少趋势   弯道近处右边线增加趋势
         Leftuw=1;
      else if(RightStableNumbers>23)
        Rightuw=1;
   }   
  }
  if(Leftuw && Leftnumber>10 && !shizi)
    Leftuw=1;
  else
    Leftuw=0;
  
  if(Rightuw && Rightnumber>10 && !shizi)
    Rightuw=1;
  else
    Rightuw=0;
  
  WhiteEnd=0;
//  if(LeftEnd>MidEnd && LeftEnd>RightEnd)
//   BlackLineData[RowMax-3]=(LeftBlack[RowMax-3]+RightBlack[RowMax-3])/2-5;
// else if(RightEnd>MidEnd && RightEnd>LeftEnd)
//  BlackLineData[RowMax-3]=(LeftBlack[RowMax-3]+RightBlack[RowMax-3])/2+5;
// else 
    BlackLineData[RowMax-3]=(LeftBlack[RowMax-3]+RightBlack[RowMax-3])/2;
  int t=0;
  if(zdflag)
  {
    s++;
    if(s>20)
    {zdflag=0;
    //NUM_Send();       //前后交换
    CAR=1;
    uart_putchar(UART0,CAR);
    CAR=0;
    s=0;
    }
  }
    if(shizi && !zdflag && CAR!=0 && !sanjiaol && !sanjiaor)     //出直道时不检测三角
      {
        for(j=BlackLineData[RowMax-3]-7;j<BlackLineData[RowMax-3]+8;j++)
        {
          for(i=RowMax-1;i>RowMax-StableNumbers;i--)
          {
            if(ThresholdData[i][j] == White)
              WhiteEnd++;
            else if(ThresholdData[i][j] == Black)
            {
              if(ThresholdData[i][2]==Black || ThresholdData[i][ColumnMax-2]==Black )
                WhiteEnd=0;
              break;
            }
          }
          
          sf[z]=WhiteEnd;
          z++;
          WhiteEnd=0;
          //用上位机检测找到的顶部
//          ThresholdData[RowMax-sf[z]][j] = 0;
//          ThresholdData[RowMax-sf[z]+1][j] = 0;
//          ThresholdData[RowMax-sf[z]+2][j] = 0;
        }
       
        
        z=0;
        a=0;
        for(i=0;i<13;i++)
        {
          if(sf[i]<65 && sf[i+1]<65 )//&& (ThresholdData[RowMax-2][2] != White && ThresholdData[RowMax-2][ColumnMax-2] != White)//&& (ThresholdData[RowMax-2][2] == Black && ThresholdData[RowMax-2][ColumnMax-2] == Black ) && LeftEnd>30 && RightEnd>30) 
             //||(sf[i]<40 && sf[i+1]<40 &&  (ThresholdData[RowMax-2][2] == White || ThresholdData[RowMax-2][ColumnMax-2] == White )))
          {
             if(fabss(sf[i+1]-sf[i])<5 && sf[i+2]<sf[i] && fabss(sf[i+2]-sf[i])<9)
            {
            //z=0;
            a++;
            }
         else
          if(fabss(sf[i+1]-sf[i])<5 && sf[i+2]>sf[i]&& fabss(sf[i+2]-sf[i])<9)
          {
          z++;
          //a=0;
          }
          }
        
        if(fabss(sf[i]-sf[i+1])>15)
         t=1;
        }
        if((z>a  && t ) || z>11)
        {
          //sanjiaonumber++;
          //if(sanjiaonumber>1)
          //{
           // sanjiaonumber=0;
            sanjiaor=1;
          //}
          //break;
        }
        else
          if((a>z  && t)||a>11)
        {
          //sanjiaonumber++;
          //if(sanjiaonumber>1)
          //{
          //sanjiaonumber=0;
        sanjiaol=1;
         // }
        //break;
        }
        //else 
        //sanjiaonumber=0;
        
       
         
        
       // aa++;
       // OLED_Print_Num(60,6,aa);
        }      
// 
 if( !zdczl && !zdczr && BlackEndMax>RowMax*0.8 && zd && CAR!=0)
            ScanRoad();
//  if(CAR==1)
//  {
//        aa++;
//        OLED_Print_Num(60,6,aa);
//  }

}
void GetLMR3()
{
 // zdczr=zdczl=0;  //aaaaaaaaaaaaaaaaaaaaaaaaaaaa
  unsigned char Lf=0;
  unsigned char Rf=0;


  unsigned int FirstL=0;
  unsigned int FirstR=0;
       unsigned int uFlag=0;
       unsigned int uLFlag=0;
        signed int LastLeftBlack=0;
        signed int LastRightBlack=0;
        unsigned int jump1=0,jump2=0,FlagL=0,FlagR=0,Flag=0,Flagcount=0;
	int i=0,j=0;
        unsigned char shizir=0,shizil=0;
	int temLeft = 0;
	int temRight = 0;
	signed int pLeft = ColumnMax/2,pRight = ColumnMax/2,pLeftL=0,pLeftR=0,pRightL=0,pRightR=0;
	unsigned char bFoundLeft = 0;
	unsigned char bFoundRight = 0;
	unsigned char TripLen = 4;

	unsigned char LeftEnd = 0;
	unsigned char RightEnd = 0;
	unsigned char MidEnd = 0;
        unsigned char LFlag=0,RFlag=0,L=0,R=0;
	unsigned char MidToBlackCount = 0;
        unsigned int Blacknumber=0,Whitenumber=0;
        signed int Error1=0,Error2=0;
	int NoValidCount = 0;
        int NoValidCountl=0;
        int NoValidCountr=0;
	NoValidMax = 0;
        RightRow=0,LeftRow=0;
	//StableNumbers = 0;
	LeftStableNumbers = 0;  
	RightStableNumbers = 0;
	
	ValidLineCount1 = 0;
	ValidLineCount2 = 0;
        Leftuw=0;
        Rightuw=0;
        shizi = 0;
        u=0;
	Rightnumber=0;Leftnumber=0;
	for (i = RowMax-1;i > RowMax-5 && !MidEnd;i--)
	{
		// 初始化遍历指针
         bFoundLeft=bFoundRight=0;
         LeftBlack[i] = 0;
         RightBlack[i] = ColumnMax-1;
         
       
		// 初始化遍历指针
          if( BlackEndR > BlackEndL )
			{
				pLeft = ColumnMax*3/4-5;
				pRight = ColumnMax*3/4-5;	
			}
			else if(BlackEndL > BlackEndR)
			{
				pLeft = ColumnMax/4+5;
				pRight = ColumnMax/4+5;
			}
			else
			{
				pLeft = ColumnMax/2;
				pRight = ColumnMax/2;
			}
              bFoundLeft=bFoundRight=0;
          
          for(j=pLeft;j>0;j--)
          {
          if (ThresholdData[i][j] == White &&  ThresholdData[i][j-1] == Black && !bFoundLeft){
         LeftBlack[i]=j-1;
         if(LeftBlack[i]>0){
           bFoundLeft=1;
           break;
         }
          }   
          }
          for(j=pRight;j<ColumnMax-1;j++)
          {
          if (ThresholdData[i][j] == White &&  ThresholdData[i][j+1] == Black && !bFoundRight){
          RightBlack[i]=j+1;
          if(RightBlack[i]<ColumnMax-1){
          bFoundRight=1;
          break;
          }
          }
          }
	  
       
       if (LeftBlack[i] > RightBlack[i])
	{ 
          temRight = temLeft = (LeftBlack[i] + RightBlack[i])/2;
          LeftBlack[i] = temLeft;
          RightBlack[i] = temRight;
	}
		
      if (!LeftBlack[i]){LeftStableNumbers++;}
      else Leftnumber++;
      if (!RightBlack[i]||RightBlack[i]==ColumnMax-1){RightStableNumbers++;} 
      else Rightnumber++;
      

			BlackLineData[i] = LeftBlack[i] + (RightBlack[i]-LeftBlack[i] )/2;

      }
    
    for(i=RowMax-5;i>RowMax-BlackEndMax && !MidEnd;i--)
     {
       shizil=0;
       LeftBlack[i] = 0;
       RightBlack[i] = ColumnMax-1;
       BlackLineData[i] = 0;
       //初始化遍历指针
//       if(LeftBlack[i+1]==0)
//       {
//       pLeftR=LastLeftBlack+4;
//       pLeftL=LastLeftBlack-4;
//       
//       }
//       else
//       {
       pLeftR=LeftBlack[i+1]+6;
       pLeftL=LeftBlack[i+1]-6;
      // }
//       if(RightBlack[i+1]==ColumnMax-1)
//       {
//       pRightR=LastRightBlack+4;
//       pRightL=LastRightBlack-4;
//       }
//       else
//       {
       pRightR=RightBlack[i+1]+6;
       pRightL=RightBlack[i+1]-6;
      // }
       
       Whitenumber=0;Blacknumber=0;
       if(pLeftL<0)
         pLeftL=0;
       
       if(pRightR>ColumnMax-1)
         pRightR=ColumnMax-1;
       
       if(pRightL<pLeftR){
         temRight = pLeftR+(pRightL-pLeftR)/2;
         pRightL = temRight;
         pLeftR = temRight;
       }
     bFoundLeft = bFoundRight = 0;
     Whitenumber=0;Blacknumber=0;  
      //上一行左边缘附近寻找 
       
     for(j=pLeftR;j>pLeftL;j--)
      {
         if (ThresholdData[i][j] == White &&  ThresholdData[i][j-1] == Black && !bFoundLeft){
         LeftBlack[i]=j-1;
         if(LeftBlack[i]>0){
           bFoundLeft=1;
           break;
         }
         
       }
       else
       {
         if(ThresholdData[i][j] == White &&  ThresholdData[i][j-1] == White){
           Whitenumber++;
         }
         if(ThresholdData[i][j] == Black &&  ThresholdData[i][j-1] == Black){
           Blacknumber++;
         }
       }
     }
     //全黑表示在赛道外向右寻找   全白表示赛道内向左寻找
     if(LeftBlack[i] == 0){
       if(i>20)
       {
       NoValidCountl++;
       if(NoValidCountl>5 && !shizil)
       {
         shizil=1;
         LeftRow=i;
       }
       }
       
//       if(i<20)
//       {
//         shizil=1;
//       }
      // LastLeftBlack = LeftBlack[i+1] + LeftBlack[i+1] - LeftBlack[i+2];   //上次左边界   用于判断在一定范围内丢线  不然造成十字无法识别
       if(Whitenumber>(pLeftR-pLeftL)*6/8){
        
          for(j=pLeftL+2;j>0;j--)
          {
            if(ThresholdData[i][j] == White &&  ThresholdData[i][j-1] == Black){
              LeftBlack[i]=j-1;
              break;
              
            }
          }
       }
       //else if(Blacknumber>(pLeftR-pLeftL)*6/8)
       else
       {
         
         for(j=pLeftR-2;j<pRightL+1;j++)
           if(ThresholdData[i][j] == Black &&  ThresholdData[i][j+1] == White){
             LeftBlack[i]=j+1;
             break;       
             }
        }
       }
     else
       NoValidCountl=0;
     
     Whitenumber=0;Blacknumber=0;  
   for(j=pRightL;j<pRightR;j++)
   {
   
         if (ThresholdData[i][j] == White &&  ThresholdData[i][j+1] == Black && !bFoundRight){
          RightBlack[i]=j+1;
          if(RightBlack[i]<ColumnMax-1){
          bFoundRight=1;
          break;
          }
         
         
       }
       else
       {
         if(ThresholdData[i][j] == White &&  ThresholdData[i][j+1] == White){
           Whitenumber++;
         }
         if(ThresholdData[i][j] == Black &&  ThresholdData[i][j+1] == Black){
           Blacknumber++;
         }
       }
   }
     
    if(RightBlack[i] == 0 ||RightBlack[i]==ColumnMax-1){
      NoValidCountr++;
      if(NoValidCountr>5 && !shizir)
      {
        shizir=1;
        RightRow=i;
      }

       if(Whitenumber>(pLeftR-pLeftL)*6/8) {      //&& RightBlack[i+1] !=ColumnMax-1???为什么
         
          for(j=pRightR-2;j<ColumnMax-1;j++)
          {
            if(ThresholdData[i][j] == White &&  ThresholdData[i][j+1] == Black){
              LeftBlack[i]=j+1;
              break;
              
            }
          }
       }
       //else if(Blacknumber>(pLeftR-pLeftL)*6/8)
      else
       {
        
         for(j=pRightL+1;j>pLeftR-1;j--)
           if(ThresholdData[i][j] == Black &&  ThresholdData[i][j-1] == White){
             LeftBlack[i]=j-1;
             
           }
       }
       
       
     }
   else NoValidCountr=0;
       // 该行左右方向找边线结束
     
     if(shizil && shizir && fabss(LeftRow-RightRow)<25 && BlackEndMax>RowMax*0.8){     //会出现 左边下丢 右边上丢  <50 保证为十字丢
              shizi = 1;
            
           }

     
      if (!LeftBlack[i]) 
		{ 
			if (i < 20)
			{
				LeftBlack[i] = LeftBlack[i+1] + LeftBlack[i+1] - LeftBlack[i+2]; 
			}
			else
			{
				LeftBlack[i] = 0; 
			}
		}
		else if (i < 20 && fabss(LeftBlack[i] - LeftBlack[i+1]) > TripLen )
		{
			LeftBlack[i] = LeftBlack[i+1] + LeftBlack[i+1] - LeftBlack[i+2];
		}

        if (!RightBlack[i] || RightBlack[i]==ColumnMax-1)
		{
			if (i < 20)
			{ 
				RightBlack[i] = RightBlack[i+1] + RightBlack[i+1] - RightBlack[i+2];
			}
			else
			{
				RightBlack[i] = ColumnMax-1; 
			}
		}
		else if (i < 20 && fabss(RightBlack[i] - RightBlack[i+1]) > TripLen )
		{
			RightBlack[i] = RightBlack[i+1] + RightBlack[i+1] - RightBlack[i+2];
		}
   

 
   if (!LeftBlack[i])
   {
     if(Leftnumber>5 && !Lf)
       Lf=1;
     LeftStableNumbers++;
   }
   else 
   {
     if(Lf)
       Leftuw=1;
     if(!FirstL && LeftStableNumbers!=0 && Leftnumber==0)     //如果一边下面先没有找到，然后找到，则这边有效行就等于二者相加
     {
     Leftnumber=LeftStableNumbers;
     }
     else
       Leftnumber++;
     FirstL=1;
   }    // else  {LeftStableNumbers=0; Leftnumber++;}
   
   
	if (!RightBlack[i] || RightBlack[i] == ColumnMax-1)
        {
          if(Rightnumber>3 && !Rf)
            Rf=1;
          RightStableNumbers++;
        }
   else  {
     if(Rf)
       Rightuw=1;       // 有种S一边只有中间丢线
    if(!FirstR && RightStableNumbers!=0 && Rightnumber==0)   //若一边最下面丢线，上面不丢，则这边有效数等于二者和
    {
    
    Rightnumber=RightStableNumbers;
    }
    else
     Rightnumber++;
    FirstR=1;
   }
   
   
   //补充检测十字
   
   //大于15保证在十字，不和S弄混
   if(BlackEndL>15 && BlackEndR>15 &&i<RowMax-25 &&((LeftBlack[i]==0 && (RightBlack[i]>RightBlack[i+1]  && RightBlack[i+1]>RightBlack[i+2])) || ((RightBlack[i]==ColumnMax-1 ||RightBlack[i]==0) && (LeftBlack[i]<LeftBlack[i+1] && LeftBlack[i]<LeftBlack[i+2]))))
   {
   NoValidCount++;
   if(NoValidCount>5)
   {shizi=1;
   }
   
   }
   else
     NoValidCount=0;
  
  /* if(i>RowMax-20)   //U形弯 一边下面全无效，上面重新有效。防止和弯道弄混
   {

      if(LeftStableNumbers>15)
         Leftuw=1;
      else if(RightStableNumbers>15)
        Rightuw=1;
   }*/
     
     
        if(i>RowMax-20)   //U形弯 一边下面全无效，上面重新有效。防止和弯道弄混
   {

      if(LeftStableNumbers>15 && RightBlack[RowMax-15]< RightBlack[RowMax-2]-4)      //S时左边丢线同时近处右边线减少趋势   弯道近处右边线增加趋势
         Leftuw=1;
      else if(RightStableNumbers>15 && LeftBlack[RowMax-15]>LeftBlack[RowMax-2]+4)
        Rightuw=1;
   }
    

      }
    
    if(shizi)
      {
      // aa++;
      // OLED_Print_Num(60,6,aa);
 
      //NoValidCount=0;  
     }
     //OLED_Print_Num(60,2,zdczrc1);
}
       
//提取图像中心线
//void GetLMR()
//{	
//	int i=0,j=0;
//	int temLeft = 0;
//	int temRight = 0;
//	unsigned char pLeft = ColumnMax/2,pRight = ColumnMax/2;
//	unsigned char bFoundLeft = 0;
//	unsigned char bFoundRight = 0;
//	unsigned char TripLen = 4;
//
//	unsigned char LeftEnd = 0;
//	unsigned char RightEnd = 0;
//	unsigned char MidEnd = 0;
//
//	unsigned char MidToBlackCount = 0;
//
//
//	int NoValidCount = 0;
//	NoValidMax = 0;
//
//	StableNumbers = 0;
//	LeftStableNumbers = 0;
//	RightStableNumbers = 0;
//	
//	ValidLineCount1 = 0;
//	ValidLineCount2 = 0;
//
//	
//	for (i = RowMax-1;i >= 0 && !MidEnd;i--)
//	{
//		// 初始化遍历指针
//		if (i < RowMax - 5)
//		{
//			pLeft = BlackLineData[i+1];
//			pRight = BlackLineData[i+1];
//		}
//		else
//		{
//			if(BlackEndL < 5 && BlackEndR > BlackEndM)
//			{
//				pLeft = ColumnMax*3/4;
//				pRight = ColumnMax*3/4;	
//			}
//			else if(BlackEndR < 5 && BlackEndL > BlackEndM)
//			{
//				pLeft = ColumnMax/4;
//				pRight = ColumnMax/4;
//			}
//			else
//			{
//				pLeft = ColumnMax/2;
//				pRight = ColumnMax/2;
//			}
//		}
//
//
//		// 初始化标记
//		bFoundLeft = bFoundRight = 0;
//
//		// 两条黑线边缘提取
//		for (j = 0;j < ColumnMax;j++)
//		{
//			// 往左搜索
//			if (bFoundLeft == 0 && pLeft > 0 && !LeftEnd){// 未找到左边缘则寻找
//				//if (pLeft < 1){break;}
//				if (ThresholdData[i][pLeft] == White && ThresholdData[i][pLeft-1] == Black)
//				{	// 找到左边缘
//					LeftBlack[i] = pLeft-1;
//					
//					if (LeftBlack[i] > 0)
//					{
//						bFoundLeft = 1;
//					}	
//					// 如果右边缘也找到则退出
//					if (bFoundRight)
//					{
//						break;
//					}
//				}
//				else
//				{
//					pLeft--;
//				}
//			}// if结束--从左到右搜索边缘
//			
//			//往右搜索
//			if (bFoundRight == 0 && pRight < ColumnMax-1 && !RightEnd)
//			{//未找到右边缘则寻找
//				//if (pRight > ColumnMax - 3){break;}
//				if (ThresholdData[i][pRight] == White &&  ThresholdData[i][pRight+1] == Black)
//				{
//					// 找到右边缘
//					RightBlack[i] = pRight + 1;
//					if (RightBlack[i] < ColumnMax-1)
//					{
//						bFoundRight = 1;
//					}
//					
//					// 如果左边缘也找到则退出
//					if (bFoundLeft)
//					{
//						break;
//					}
//				}
//				else
//				{
//					pRight++;
//				}
//			}// if结束
//		}//for结束
//
//
//		if (i < RowMax-6 && !bFoundLeft&&!bFoundRight)
//		{
//			ValidLine[i] = 0;
//
//			NoValidCount++;
//			if (NoValidCount > NoValidMax)
//			{
//				NoValidMax = NoValidCount;
//			}
//		}
//		else
//		{
//			NoValidCount = 0;
//		}
//
//		if(bFoundLeft && bFoundRight)
//		{
//			ValidLineCount1++;
//			ValidLine[i] = 3;
//		}
//		else
//		{
//			if (bFoundLeft )
//			{
//				ValidLineCount2++;
//				ValidLine[i] = 1;
//			}
//			if(bFoundRight)
//			{
//				ValidLine[i] = 2;
//			}
//		}
//
//		if (!bFoundLeft) 
//		{ 
//			if (i < 30)
//			{
//				LeftBlack[i] = LeftBlack[i+1] + LeftBlack[i+1] - LeftBlack[i+2]; 
//			}
//			else
//			{
//				LeftBlack[i] = 0; 
//			}
//		}
//		else if (i < 30 && fabss(LeftBlack[i] - LeftBlack[i+1]) > TripLen  )
//		{
//			LeftBlack[i] = LeftBlack[i+1] + LeftBlack[i+1] - LeftBlack[i+2];
//		}
//
//		if (!bFoundRight)
//		{
//			if (i < 30)
//			{ 
//				RightBlack[i] = RightBlack[i+1] + RightBlack[i+1] - RightBlack[i+2];
//			}
//			else
//			{
//				RightBlack[i] = ColumnMax-1; 
//			}
//		}
//		else if (i < 30 && fabss(RightBlack[i] - RightBlack[i+1]) > TripLen )
//		{
//			RightBlack[i] = RightBlack[i+1] + RightBlack[i+1] - RightBlack[i+2];
//		}
//		
//		if (LeftBlack[i] > RightBlack[i])
//		{ 
//			temRight = temLeft = (LeftBlack[i] + RightBlack[i])/2;
//			LeftBlack[i] = temLeft;
//			RightBlack[i] = temRight;
//		}
//		
//		if (!LeftEnd){LeftStableNumbers++;}
//		if (!RightEnd){RightStableNumbers++;}
//
//		if (LeftEnd && !RightEnd)
//		{
//			BlackLineData[i] = BlackLineData[i+1] + RightBlack[i+1] - RightBlack[i+2];
//		}
//		else if (!LeftEnd && RightEnd)
//		{
//			BlackLineData[i] = BlackLineData[i+1] + LeftBlack[i+1] - LeftBlack[i+2];
//		}
//		else if (LeftEnd && RightEnd)
//		{
//			MidEnd = 1;
//			break;
//		}
//		else
//		{
//			BlackLineData[i] = LeftBlack[i] + (RightBlack[i]-LeftBlack[i] )/2;
//		}
//
//		//中心线搜索截止
//		if (BlackLineData[i] < 4 || BlackLineData[i] > ColumnMax-4)
//		{
//			MidEnd = 1;
//			break;
//		}
//		if (i < RowMax-20 && fabss(BlackLineData[i]-BlackLineData[i+1]) > TripLen)
//		{
//			BlackLineData[i] = BlackLineData[i+1] + BlackLineData[i+1] - BlackLineData[i+2];
//		}
//		if (ThresholdData[i][BlackLineData[i]] == Black)
//		{
//			MidToBlackCount++;
//			if (MidToBlackCount >= 2)
//			{
//				MidEnd = 1;
//			}
//		}
//		else
//		{
//			MidToBlackCount = 0;
//		}
//		if (!MidEnd)
//		{
//			StableNumbers++;
//		}
//		if (ValidLineCount1 < 4 && i < 20)
//		{
//			MidEnd = 1;
//		}
//	}
//        // OLED_Print_Num(0,4,StableNumbers);
//}
////通过左右边缘取得控制舵机的中心线 
void GetFinalMidLine()
{
int i=0,j=0;
//if(MidPos!=40)
//{
//for(i=RowMax-1;i>0;i--)
//{
//  if(i>14)
//BlackLineData[i]=RightBlack[i]-6;
//else if(i>17)
//}
//StableNumbers=95;
//
//}
//else
//{
 /* //直道边界离中心距离
   
   99                   74
   46                   62  //第一段
     
   7                  48 //第二段 
     
    */ 
if(zdczr || zdczl)
{
   
      
  StableNumbers=BlackEndMax;
//
 if(zdczl)              //直走
{
  //MidPos=40;
  if(CAR==4)
  {
for(i=46;i<RowMax-1;i++)
  BlackLineData[i]=RightBlack[i]-((i-46)*12/53 + 18);    //理论22    8  
for(i=RowMax-StableNumbers;i<46;i++)
  BlackLineData[i]=RightBlack[i]-((i-(RowMax-StableNumbers))*14/(46-(RowMax-StableNumbers)) +5);
  }
  else
  {
 
  for(i=RowMax-1;i>zdczlc1+5;i--)
  {
    BlackLineData[i]=(LeftBlack[i]+RightBlack[i])/2;
  }
  for(i=zdczlc1+5;i>RowMax-StableNumbers;i--)
  {
//  if(LeftBlack[i] && RightBlack[i] && RightBlack[i]!=ColumnMax-1)
//    BlackLineData[i]=(LeftBlack[i]+RightBlack[i])/2;
//  else
//  {
  BlackLineData[i]=LeftBlack[i]+15;
  
  }
  }
  
  
  }
  
  

  
  

else if(zdczr)
{
//  MidPos=40;
  if(CAR==4)
  {
for(i=46;i<RowMax-1;i++)
  BlackLineData[i]=LeftBlack[i]+((i-46)*12/53 + 18);    //理论22    8  
for(i=RowMax-StableNumbers;i<46;i++)
 BlackLineData[i]=LeftBlack[i]+((i-(RowMax-StableNumbers))*14/(46-(RowMax-StableNumbers)) +5);
  }
  else
  {

  for(i=RowMax-1;i>zdczrc1+5;i--)
  {
    BlackLineData[i]=(LeftBlack[i]+RightBlack[i])/2+5;
  }
  for(i=zdczrc1+5;i>RowMax-StableNumbers;i--)
  {
//  if(LeftBlack[i] && RightBlack[i] && RightBlack[i]!=ColumnMax-1)
//    BlackLineData[i]=(LeftBlack[i]+RightBlack[i])/2;
//  else
//  {
    if(RightBlack[i]!=ColumnMax-1)
    {
    BlackLineData[i]=(LeftBlack[i]+RightBlack[i])/2+15;
    }
    else
  BlackLineData[i]=RightBlack[i]-15;
  
  }
  }
  
  
 
  
  
  }
}


else
{
  int temt=1;
  signed int error=0;
  
  int a=0;
  a=MAX(Leftnumber,Rightnumber);
  StableNumbers=0;
  error = Leftnumber - Rightnumber;
  
  //测试
  for(i=RowMax-1;i>RowMax-15;i--)
  {
  BlackLineData[i]=(LeftBlack[i]+RightBlack[i])/2;
  if(BlackLineData[i]>ColumnMax-13)
    break;
  StableNumbers++;
  }
  for(i=RowMax-15;i>RowMax-BlackEndMax;i--)
  {
  if(LeftBlack[i] && RightBlack[i]!=ColumnMax-1 || Leftuw || Rightuw)
  {
  BlackLineData[i]=(LeftBlack[i]+RightBlack[i])/2;
  StableNumbers++;
  }
  else if(LeftBlack[i] && RightBlack[i]==ColumnMax-1)
  {
   if(i<RowMax && i>=RowMax-10)
	
          BlackLineData[i]=LeftBlack[i]+RoadWidth[0];
      else
        if(i<RowMax-10 &&i>=RowMax-20)
              BlackLineData[i] = LeftBlack[i] + RoadWidth[1];
      else
         if(i<RowMax-20 &&i>=RowMax-30)
               BlackLineData[i] = LeftBlack[i] + RoadWidth[2];
       else
         if(i<RowMax-30 &&i>=RowMax-40)
                BlackLineData[i] = LeftBlack[i] + RoadWidth[3];
       else
          if(i<RowMax-40 && i>=RowMax-50)
                  BlackLineData[i] = LeftBlack[i] + RoadWidth[4];
        else
          if(i<RowMax-50 &&i>= RowMax-60)
                  BlackLineData[i] = LeftBlack[i] + RoadWidth[5]; 
        else
          if(i<RowMax-60 &&i>= RowMax-70)
                  BlackLineData[i] = LeftBlack[i] + RoadWidth[6];
        else
          if(i<RowMax-70 &&i>= RowMax-80)
                  BlackLineData[i] = LeftBlack[i] + RoadWidth[7]; 
        else   BlackLineData[i] = LeftBlack[i] + RoadWidth[7]; 
        if(BlackLineData[i]>ColumnMax-1)
                 BlackLineData[i]=ColumnMax-1;
//        if(BlackLineData[i]>ColumnMax-1)
//          break;
//        else 
          StableNumbers++;
  
  }
  else if(!LeftBlack[i] && RightBlack[i]!=ColumnMax-1)
  {
  
    if(i<RowMax && i>=RowMax-10)
              BlackLineData[i] = RightBlack[i] - RoadWidth[0];
             else
               if(i<RowMax-10 && i>=RowMax-20)
                      BlackLineData[i] = RightBlack[i] - RoadWidth[1];
             else
               if(i<RowMax-20 && j>=RowMax-30)
                      BlackLineData[i] = RightBlack[i] - RoadWidth[2];
             else
               if(i<RowMax-30 && i>=RowMax-40)
                      BlackLineData[i] = RightBlack[i] - RoadWidth[3];
             else
                if(i<RowMax-40 && i>=RowMax-50)
                      BlackLineData[i] = RightBlack[i] - RoadWidth[4];
             else
                if(i<RowMax-50 && i>=RowMax-60)
                      BlackLineData[i] = RightBlack[i] - RoadWidth[5];
             else
                if(i<RowMax-60 && i>=RowMax-70)
                      BlackLineData[i] = RightBlack[i] - RoadWidth[6];
             else
                if(i<RowMax-70 && i>=RowMax-80)
                      BlackLineData[i] = RightBlack[i] - RoadWidth[7];
             else  BlackLineData[i] = RightBlack[i] - RoadWidth[7];
             if(BlackLineData[i]<0)
                    BlackLineData[i]=0; 
//             if(BlackLineData[j]<1)
//               break;
              StableNumbers++;
  }
  
  
  }
  
  

}
  

}
int P0_X = 0;
int P0_Y = 0;
int P1_X = 0;
int P1_Y = 0;
int P2_X = 0;
int P2_Y = 0;
float Mid_K1 = 0.0;
float Mid_K2 = 0.0;

//左均值滤波
void LAverageFilter()
{
	unsigned char i = 0;
	unsigned char j = 0;
	int sum = 0;
	for (i = RowMax-1;i > RowMax-(BlackEndMax-3);i--)
	{
		sum = 0;
		for (j = 0;j < 5;j++)
		{
			sum += LeftBlack[i-j];
		}
		LeftBlack[i] = sum/5;
	}
	P1_X = LeftBlack[RowMax-(LeftStableNumbers-6)];
	P1_Y = RowMax-(LeftStableNumbers-6);
}

//右均值滤波
void RAverageFilter()
{
	unsigned char i = 0;
	unsigned char j = 0;
	int sum = 0;
	for (i = RowMax-1;i > RowMax-(BlackEndMax-3);i--)
	{
		sum = 0;
		for (j = 0;j < 5;j++)
		{
			sum += RightBlack[i-j];
		}
		RightBlack[i] = sum/5;
	}
	P2_X = RightBlack[RowMax-(RightStableNumbers-6)];
	P2_Y = RowMax-(RightStableNumbers-6);

}

//中心线均值滤波
void AverageFilter()
{
	unsigned char i = 0;
	unsigned char j = 0;
	int sum = 0;
	for (i = RowMax-1;i > RowMax-(StableNumbers-5-5);i--)
	{
		sum = 0;
		for (j = 0;j < 5;j++)
		{
			sum += BlackLineData[i-j];
		}
		BlackLineData[i] = sum/5;
	}
	P0_X = BlackLineData[RowMax-1];
	P0_Y = RowMax-1;

	Mid_K1 = fabss(P0_X-P1_X)*1.0/fabss(P0_Y-P1_Y);
	Mid_K2 = fabss(P0_X-P2_X)*1.0/fabss(P0_Y-P2_Y);

}

void MidLineCompensate()
{
	int i=0,icount=0,j=0;
	int CompensateData = 0;
	
	int sum = 0;
	float avg =0.0;
	int tem = 1;

	CompensateCount=0;

	for (i = RowMax-2,icount=0;i > RowMax-(StableNumbers-10);i--,icount++)
	{
		sum += (BlackLineData[i]-BlackLineData[i+1]);
	}
	avg = sum*1.0 / icount;
	if (avg < 0)
	{
		tem = -1;
		avg = (-1)*avg;
	}
	if (avg > 1.0)
	{
		CompensateData = 3;
	}else if (avg > 0.55)
	{
		CompensateData = 2;
	}
	else if (avg > 0.25)
	{
		CompensateData = 1;
	}else
	{
		CompensateData = 0;
	}
	CompensateData = CompensateData*tem;


	for (i = RowMax - (StableNumbers-10);i > 0;i--)
	{
		BlackLineData[i] = BlackLineData[i+1] + CompensateData;//BlackLineData[i+1] + BlackLineData[i+1]-BlackLineData[i+2];
		
		CompensateCount++;
		if (ThresholdData[i][BlackLineData[i]] == Black || BlackLineData[i] < 2 || BlackLineData[i] > ColumnMax -2)
		{
			break;
		}
		sum = 0;
		for (j = RowMax-2,icount=0;j > i;j--,icount++)
		{
			sum += (BlackLineData[j]-BlackLineData[j+1]);
		}
		avg = sum*1.0 / icount;
		if (avg < 0)
		{
			tem = -1;
			avg = (-1)*avg;
		}
		if (avg > 1.0)
		{
			CompensateData = 3;
		}else if (avg > 0.55)
		{
			CompensateData = 2;
		}
		else if (avg > 0.25)
		{
			CompensateData = 1;
		}else
		{
			CompensateData = 0;
		}
		CompensateData = CompensateData*tem;
	}
}
//获取中心线偏差
float GetSteerError(unsigned char start,unsigned char end,float midpos)
{
	 char i=0;
	 char iCount=0;
         int  Black_Sum=0;
	float TemError = 0.0;
	for(i = start/*,iCount = 0*/; i < end; i++)    
	{	//BlackLineData[i]=48;
		Black_Sum += BlackLineData[i];iCount++;
            //    printf(" TemError \n  ");
            //    printf(" TemError \n  ");
                //printf("  \n  %d",BlackLineData[i]);
	}
	TemError =Black_Sum/iCount- midpos;	//Black_Sum*1.0/iCount - midpos;
      //  printf("  \n  %f",TemError);
	return TemError;
        

}


//获取中心线方差
void GetMidLineVariance()
{//int aa;
	unsigned char i=0;
	unsigned char iCount=0;
	unsigned int  Black_Sum=0;
	float aver=0.0;
        int a=MAX(Leftnumber,Rightnumber);
	int end = RowMax - (StableNumbers - 5);

	MidLineExcursion = 0;

	for(i=RowMax-2,iCount=0; i>end; i--,iCount++)    
	{	
		Black_Sum += BlackLineData[i];
		MidLineExcursion = MidLineExcursion + BlackLineData[i] - BlackLineData[i+1];
	}	
	aver = Black_Sum*1.0 / iCount;
	MidLineVariance = 0.0;
	for (i = RowMax-2;i > end;i--)
	{
		MidLineVariance+=(aver-BlackLineData[i])*(aver-BlackLineData[i]);
	}
	MidLineVariance = MidLineVariance*1.0 / iCount;
        //OLED_Print_Num(0,6,MidLineVariance);
        //OLED_Print_Num(60,0,StableNumbers);
     //   aa=(int)MidLineVariance;
        
       // printf("  \n  %d",aa);
}

//获取特殊中心线偏差 ，StableNumbers要大于2
void GetSpecialError()
{
	unsigned char i=0;
	int end = RowMax - StableNumbers;
	
	MidLineExcursion = 0;
	for (i = RowMax-1;i > end ;i--)
	{
		BlackLineData[i] = LeftBlack[i] + (RightBlack[i]-LeftBlack[i] )/2;		
	}
	for(i=RowMax-2; i>end; i--)    
	{	
		MidLineExcursion = MidLineExcursion + BlackLineData[i] - BlackLineData[i+1];		
	}
	//特殊情况给中心线方差极大
	MidLineVariance = 300;
	//特殊情况给中心线偏差极大
	if(MidLineExcursion > 0)
	{
		MidLineExcursion = 40;
	}
	else if(MidLineExcursion < 0)
	{
		MidLineExcursion = -40;	
	}
}

int TopE1=0;//中心线上半部分偏差
int TopE2=0;//中心线下半部分偏差
int TopLen1=0;//中心线上半部分偏差
int TopLen2=0;//中心线上半部分偏差 
int SubBasePoint = 0;//中心线下半部分距基准点的最大偏差

void GetSectionParam()
{
	int TotalPoint = StableNumbers - 10;
	int icount = 0;
	int i = 0;
	int BasePoint = BlackLineData[RowMax-1];
	SubBasePoint = 0;
	TopE1=0;
	TopE2=0;
	TopLen1=0;
	TopLen2=0;
	
	for (i=RowMax-(StableNumbers-10),icount=1;i < RowMax-2;i++,icount++)
	{
		if (icount < TotalPoint/2)
		{
			TopE1 += (BlackLineData[i]-BlackLineData[i+1]);
			TopLen1++;
		}
		else 
		{
			TopE2 += (BlackLineData[i]-BlackLineData[i+1]);
			TopLen2++;
			if (fabss(BlackLineData[i]-BasePoint) > fabss(SubBasePoint))
			{
				SubBasePoint = BlackLineData[i]-BasePoint;
                               // printf("  \n  %d",SubBasePoint);
			}
		}
	}
}

signed char TemMidLineData[RowMax];//提取黑线值数据2

void StoreMidLine()
{
	int i = 0;
	for(i = 0;i < RowMax;i++)	
	{
		TemMidLineData[i] = BlackLineData[i];	
	}
}


void UseTemMidLine()
{
	int i = 0;
	for(i = 0;i < RowMax;i++)	
	{
		BlackLineData[i] = TemMidLineData[i];	
	}	
}

uint8 ElementCount2 = 0;
//头尾指针
uint8 Head2 = 0,Rear2 = 0;
#define Size2 30
//队列数组
signed char RoadTypeData2[Size2] = {0};

int AllStraightCount = 0;
int AllSmallSCount = 0;
int AllBigSCount = 0;
int AllBendCount = 0;
//=================================================================================

void HistoryRoadTypeCount()
{
	int i=0;
	int tem = 0;
	int StraightCount = 0;
	int SmallSCount = 0;
	int BigSCount = 0;
	int BendCount = 0;
	AllStraightCount = 0;
	AllSmallSCount = 0;
	AllBigSCount = 0;
	AllBendCount = 0;
	tem = Rear2;
	for(i = 0;i < ElementCount2;i++)
	{
		if(RoadTypeData2[tem] == 0)
		{
			StraightCount++;
			if(AllStraightCount < StraightCount)
			{
				 AllStraightCount = StraightCount;
			}
		}
		else
		{
			StraightCount = 0;
		}

		if(RoadTypeData2[tem] == 0 || RoadTypeData2[tem] == 1)
		{
			SmallSCount++;
			if(AllSmallSCount < SmallSCount)
			{
				 AllSmallSCount = SmallSCount;
			}
		}
		else
		{
			SmallSCount = 0;
		}

		if(RoadTypeData2[tem] == 2)
		{
			BigSCount++;
			if(AllBigSCount < BigSCount)
			{
				 AllBigSCount = BigSCount;
			}
		}
		else
		{
			BigSCount=0;
		}

		if(RoadTypeData2[tem] == 2 || RoadTypeData2[tem] == 3 )
		{
			BendCount++;
			if(AllBendCount < BendCount)
			{
				 AllBendCount = BendCount;
			}
		}
		else
		{
			BendCount=0;
		}

		tem = (tem-1+Size2)%Size2;
	}	
} 

void HistoryRTProccess()
{
//	int TemCount = 0;
//	int pTem = 0;
//	int i = 0;

	if(StandardRoadType && (RoadType == 1 || RoadType == 2))
	{
		//把赛道类型存入循环队列中//标准的直道和小S
		RoadTypeData2[Rear2] = RoadType;	
	}
	else
	{
		RoadTypeData2[Rear2] = 3;	
	}

	Rear2 = (Rear2+1)%Size2;
	ElementCount2++;
	if(ElementCount2 > Size2-1)
	{
		ElementCount2=Size2-1;	
	}

	//历史赛道类型统计
	HistoryRoadTypeCount();

//	//通过赛道历史记录判断直入弯
//	TemCount = 0;
//	pTem = Rear2;
//	if((RoadType == 2 || RoadType == 3) )
//	{
//		for(i = 0;i < 20;i++)
//		{
//			if(RoadTypeData2[pTem] == 0 || RoadTypeData2[pTem] == 1 )
//			{
//				TemCount++;
//				if(TemCount > 15)
//				{
//					RoadType = 103;		
//				}
//			}
//			pTem = (pTem-1+Size2)%Size2;	
//		}		
//	}	
}


unsigned char IsStraightToBend()
{
	if (fabss(TopE1) > 12 && fabss(TopE2) < 3 && fabss(SubBasePoint) < 3 )// && BlackEndMax < RowMax - 4
	{
		return 1;

	}else
	{
		return 0;
	}
}


//
int HistoryRoadType[4] = {0};
int g_Head = 0,g_Rear=0;


int StraightToBendCount = 0;
//赛道识别
void RTRecognition()
{
  int i=0,m=0;
  signed int sumx=0,avery=0;
   int averx=0.0;
   double b=0.0;
   double a=0.0;
   double x=0;
   double y=0;
   signed int k=0;
  avery=(RowMax-StableNumbers)/2;
  for(i=RowMax-1;i>RowMax-StableNumbers;i--)
  {
    sumx+=BlackLineData[i];
    m++;
  
  }
  averx=sumx/m;
  
  for(i=RowMax-1;i>RowMax-StableNumbers;i--)
  {  x+=(BlackLineData[i]-averx)*(i-avery);
  y+=(BlackLineData[i]-averx)*(BlackLineData[i]-averx);
  }
  //b=((BlackLineData[i]-averx)*(i-avery))/((BlackLineData[i]-averx)*(BlackLineData[i]-averx));}
  b=x/y;
  if(b>0)
  {
   k=(int)1000*b; 
   // printf("偏差= %d",k);
   OLED_Fill(0x00);
   OLED_Print_Str(0,4,"+");
   OLED_Print_Num(60,4,k);
  }
  else
  {
    b*=(-1);
  k=(int)1000*b;
  //printf("偏差=- %d",k);
  OLED_Fill(0x00);
  OLED_Print_Str(0,4,"-");
  OLED_Print_Num(60,4,k);
  }
    //snprintf("偏差= %f",b,"dd");
    //uart_putchar(UART0,b);
//    OLED_Print_Num(80,4,0);
//    OLED_Print_Num(0,4,b);
  

//    OLED_Print_Num(0,4,0);
//    OLED_Print_Num(80,4,b);
  
//	signed char temRoadType = -1;
//       // OLED_Print_Num(0,6,MidLineVariance);
//	//标准的赛道类型 3、2
//
//	//标准的赛道类型 1、0
//        int a=0;
//        int b=0;
//        a=MAX(Leftnumber,Rightnumber);
//        b=fabss(Leftnumber-Rightnumber);
//        if(StableNumbers >= RowMax*0.8 && BlackEndMax==BlackEndM)
//  {
//    if((fabss(MidLineExcursion) < 5|| StableNumbers >= RowMax*0.85) && b<5)
//    {
//      temRoadType = 0;	
//    }
//    else// if(/*EPerCount < 0.9 &&*/ MidLineVariance > 25)
//    {
//      temRoadType = 1;
//    }
//  }
// else
//  if(StableNumbers < RowMax*0.8)
//  {
//    if(fabss(MidLineExcursion)<65 && MidLineVariance<150 && StableNumbers >= RowMax*0.7)
//    {
//      temRoadType = 2;	
//    }
//    else 
//    {
//      temRoadType = 3;	
//    }
//    
//  }
//  else temRoadType = 103;
//    
////if(StableNumbers >= RowMax*0.8)
////  {
////    if( fabss(MidLineExcursion) < 25&&fabss(TopE1-TopE2)<20&&MidLineVariance<50||StableNumbers >= RowMax*0.9)
////    {
////      temRoadType = 0;	
////    }
////    else// if(/*EPerCount < 0.9 &&*/ MidLineVariance > 25)
////    {
////      temRoadType = 1;
////    }
////  }
////  if(StableNumbers < RowMax*0.8)
////  {
////    if( fabss(MidLineExcursion)<55&&MidLineVariance<150&&StableNumbers >= RowMax*0.5)
////    {
////      temRoadType = 2;	
////    }
////    else 
////    {
////      temRoadType = 3;	
////    }
////    
////  }
//  //  //非标准的赛道处理
//  //  if(temRoadType==-1)
//  //  {
//  //    StandardRoadType = 0;
//  //    if(StableNumbers > RowMax*0.7)
//  //    {
//  //      temRoadType = 0;
//  //      if (fabss(TopE1-TopE2) > 45 || Mid_K1 > 1.2 || Mid_K2 > 1.2)
//  //      {		   
//  //        temRoadType = 2;
//  //        StandardRoadType = 2;
//  //      }
//  //      
//  //    }
//  //    else if (StableNumbers > RowMax*0.6/*&&fabss(TopE1-TopE2) > 27*/)
//  //    {
//  //      temRoadType = 2;
//  //    }
//  //    else 
//  //    {
//  //      temRoadType = 3;
//  //    }
//  //  }
//  //  else
//  //  {
//  //    StandardRoadType = 1;
//  //  }
//  //	if(LastRoadType == 103)
//  //	{
//  //		StraightToBendCount++;
//  //		if(StraightToBendCount > 15)
//  //		{
//  //			StraightToBendCount=0;	
//  //		}
//  //		else
//  //		{
//  //			temRoadType = 103;
//  //		}
//  //	}
//
//	RoadType = temRoadType;//赛道类型判断成功
     //   printf（）
//	HistoryRTProccess();//对历史赛道类型信息进行处理
//	if(IsStraightToBend() /*&& ElementCount2 > 20 && AllSmallSCount > 30*/)//判断是否是直入弯
//	{
//		RoadType = 103;
//	}
//	if(RoadType != 103)
//	{
//		StraightToBendCount = 0;
//	}

//	HistoryRoadType[g_Rear] = RoadType;
//	g_Rear = (g_Rear+1)%4;
//	if(RoadType != HistoryRoadType[g_Head] && HistoryRoadType[g_Head] == HistoryRoadType[(g_Head+1)%4])
//	{
//		RoadType = HistoryRoadType[g_Head];	
//	}
//	if((g_Rear+1)%4 == g_Head)
//	{
//		g_Head	= (g_Head+1)%4;
//	}
////	if(IsCrossing)
////	{
////		RoadType=2;
////	}
        
//        OLED_Print_Num(0,2,temRoadType+1);
        
}

void ProcessCrossing2()
{
  int i=0,j=0;
//  int BlackEnd=0,WhiteEnd=0,End=0;
//  unsigned char FoundBlack=0;
//  EndC=0;
//  MidPos=40;
//  if(g_Derict == 1)//左倾
//    
//    EndC=ColumnMax/2-7;
//    else if(g_Derict == 2)
//      EndC=ColumnMax/2+7;
//    else
//      EndC=ColumnMax/2;
  //StableNumbers=End;
//  if(!sanjiaor && !sanjiaol || CAR==1 || CAR==3)
//  {
//  int BlackLinejumpu=0,BlackLinejumpd=0;
//  for(i=RowMax-1;i>RowMax-StableNumbers+3;i--)
//    BlackLineData[i]=(LeftBlack[i]+RightBlack[i])/2;
//  //Findjump();
//  if((ThresholdData[RowMax-2][2] == White || ThresholdData[RowMax-2][ColumnMax-2] == White ) && !cs)
//  {
//    if(StableNumbers>60)
//      StableNumbers=60;
//  for(i=RowMax-StableNumbers+3;i<RowMax-3;i+=2)
//    if(fabss(BlackLineData[i-1]-BlackLineData[i+1])>4 && fabss(BlackLineData[i-2]-BlackLineData[i+2])>4)
//    {BlackLinejumpd=i+3;break;}
//  CommonRectificate(&BlackLineData[0],BlackLinejumpd-8,RowMax-1);
//  }
//  else
//  {
//   for(i=RowMax-StableNumbers+12;i<RowMax-1;i+=2)
//    if(fabss(BlackLineData[i-1]-BlackLineData[i+1])>4 && fabss(BlackLineData[i-2]-BlackLineData[i+2])>4)
//    {BlackLinejumpu=i-3;break;}
//  
//    for(i=RowMax-1;i>BlackLinejumpu;i-=2)
//    if(fabss(BlackLineData[i-1]-BlackLineData[i+1])>4 && fabss(BlackLineData[i-2]-BlackLineData[i+2])>4)
//    {BlackLinejumpd=i+3;break;}
//  
//  
//  
//  if(BlackLinejumpd-BlackLinejumpu+3>3 && BlackLinejumpd!=0 && BlackLinejumpu!=0 )
//    CommonRectificate(&BlackLineData[0],BlackLinejumpu,BlackLinejumpd);
//  else 
//  { 
//    BlackLineData[RowMax-1]=40;
//    CommonRectificate(&BlackLineData[0],BlackLinejumpd,RowMax-1);
//  }
//  StableNumbers=StableNumbers-8;
//  }
////  shizi=0;
////  sanjiao=0;
//  }
  
//    int LeftBlackjumpu=0,LeftBlackjumpd=0;
////  for(i=RowMax-1;i>RowMax-StableNumbers+3;i--)
////    BlackLineData[i]=(LeftBlack[i]+RightBlack[i])/2;
//  //Findjump();
//   for(i=RowMax-StableNumbers+8;i<RowMax-1;i+=2)
//    if(fabss(LeftBlack[i-1]-LeftBlack[i+1])>4 && fabss(LeftBlack[i-2]-LeftBlack[i+2])>4)
//    {LeftBlackjumpu=i-3;break;}
//  
//    for(i=RowMax-1;i>LeftBlackjumpu;i-=2)
//    if(fabss(LeftBlack[i-1]-LeftBlack[i+1])>4 && fabss(LeftBlack[i-2]-LeftBlack[i+2])>4)
//    {LeftBlackjumpd=i+3;break;}
//  
//  
//  
//  if(LeftBlackjumpd-LeftBlackjumpu+3>3 && LeftBlackjumpd!=0 && LeftBlackjumpu!=0 )
//    CommonRectificate(&LeftBlack[0],LeftBlackjumpu,LeftBlackjumpd);
//  else 
//  { 
//    LeftBlack[RowMax-1]=10;
//    CommonRectificate(&LeftBlack[0],LeftBlackjumpd,RowMax-1);
//  }
//  shizi=0;
  
   if(sanjiaol || sanjiaor )
  {
   //找中线
  int BlackLinejumpu=0,BlackLinejumpd=0;
  for(i=RowMax-1;i>RowMax-StableNumbers+3;i--)
    BlackLineData[i]=(LeftBlack[i]+RightBlack[i])/2;
  
//  
//   //直左转
  if(sanjiaol)
  {
  int jumpl1=0,jumpl2=0;
  if(ThresholdData[RowMax-2][2]==White && ThresholdData[RowMax-2][ColumnMax-2]==White)    //底部全白
  {
  for(i=RowMax-BlackEndMax+5;i<RowMax-2;i++)
  {
    if((LeftBlack[i+1]<LeftBlack[i-1]-4) || LeftBlack[i+1]==0 && LeftBlack[i-1]!=0 && LeftBlack[i-2]!=0)
    {jumpl2=i;break;}
  
  }
  BlackLineData[RowMax-3*jumpl2/4]=0;
  BlackLineData[RowMax-1]=30;
  StableNumbers=3*jumpl2/4;
  CommonRectificate(&BlackLineData[0],RowMax-3*jumpl2/4,RowMax-1);
  }
  else
  {
  for(i=RowMax-BlackEndMax+5;i<RowMax-1;i++)
  {
    if((LeftBlack[i+1]<LeftBlack[i-1]-4) || LeftBlack[i+1]==0 && LeftBlack[i-1]!=0 && LeftBlack[i-2]!=0)
    {jumpl2=i;break;}
    
  
  }
  for(i=RowMax-2;i>RowMax-BlackEndMax+5;i--)
  {
    if((LeftBlack[i-1]<LeftBlack[i+1]-4) || LeftBlack[i-1]==0 && LeftBlack[i+1]!=0 && LeftBlack[i+2]!=0)
    {jumpl1=i;break;}
  
  }
  StableNumbers=(jumpl1-jumpl2)*3/4+(RowMax-jumpl1);
  BlackLineData[(jumpl2-jumpl1)*3/4+jumpl1]=0;
  CommonRectificate(&BlackLineData[0],(jumpl2-jumpl1)*3/4+jumpl1,jumpl1+25);
  }
  }
  
  else
  {
  
  int jumpr1=0,jumpr2=0;
  if(ThresholdData[RowMax-2][2]==White && ThresholdData[RowMax-2][ColumnMax-2]==White)    //底部全白
  {
  for(i=RowMax-BlackEndMax+5;i<RowMax-2;i++)
  {
    if((RightBlack[i+1]>RightBlack[i-1]+4) || RightBlack[i+1]==ColumnMax-1 && RightBlack[i-1]!=ColumnMax-1 && RightBlack[i-2]!=ColumnMax-1)
    {jumpr2=i;break;}
  
  }
  BlackLineData[RowMax-3*(RowMax-jumpr2)/4]=ColumnMax-1;
  BlackLineData[RowMax-1]=50;
  StableNumbers=3*(RowMax-jumpr2)/4;
  CommonRectificate(&BlackLineData[0],RowMax-3*(RowMax-jumpr2)/4,RowMax-1);
  }
  else
  {
  for(i=RowMax-BlackEndMax+5;i<RowMax-1;i++)
  {
    if((RightBlack[i+1]>RightBlack[i-1]+4) || RightBlack[i+2]==ColumnMax-1 && RightBlack[i+1]==ColumnMax-1 &&RightBlack[i-1]!=ColumnMax-1 && RightBlack[i-2]!=ColumnMax-1)
    {jumpr2=i;break;}
    
  
  }
  for(i=RowMax-2;i>RowMax-BlackEndMax+5;i--)
  {
    if((RightBlack[i-1]>RightBlack[i+1]+4) || RightBlack[i-1]==ColumnMax-1 && RightBlack[i+1]!=ColumnMax-1 && RightBlack[i+2]!=ColumnMax-1)
    {jumpr1=i;break;}
  
  }
  StableNumbers=(jumpr1-jumpr2)*3/4+(RowMax-jumpr1);
  BlackLineData[(jumpr2-jumpr1)*3/4+jumpr1]=ColumnMax-1;
  CommonRectificate(&BlackLineData[0],(jumpr2-jumpr1)*3/4+jumpr1,jumpr1+25);
  }
  
  
  }

  }
  
  
  
  //if(!sanjiaor && !sanjiaol || CAR==1 || CAR==3)
  else
  {
  int BlackLinejumpu=0,BlackLinejumpd=0;
  for(i=RowMax-1;i>RowMax-StableNumbers+3;i--)
    BlackLineData[i]=(LeftBlack[i]+RightBlack[i])/2;
  //Findjump();
  if((ThresholdData[RowMax-2][2] == White || ThresholdData[RowMax-2][ColumnMax-2] == White ) && !cs)
  {
    if(StableNumbers>60)
      StableNumbers=60;
  for(i=RowMax-StableNumbers+3;i<RowMax-3;i+=2)
    if(fabss(BlackLineData[i-1]-BlackLineData[i+1])>4 && fabss(BlackLineData[i-2]-BlackLineData[i+2])>4)
    {BlackLinejumpd=i+3;break;}
  CommonRectificate(&BlackLineData[0],BlackLinejumpd-8,RowMax-1);
  }
  else
  {
   for(i=RowMax-StableNumbers+12;i<RowMax-1;i+=2)
    if(fabss(BlackLineData[i-1]-BlackLineData[i+1])>4 && fabss(BlackLineData[i-2]-BlackLineData[i+2])>4)
    {BlackLinejumpu=i-3;break;}
  
    for(i=RowMax-1;i>BlackLinejumpu;i-=2)
    if(fabss(BlackLineData[i-1]-BlackLineData[i+1])>4 && fabss(BlackLineData[i-2]-BlackLineData[i+2])>4)
    {BlackLinejumpd=i+3;break;}
  
  
  
  if(BlackLinejumpd-BlackLinejumpu+3>3 && BlackLinejumpd!=0 && BlackLinejumpu!=0 )
    CommonRectificate(&BlackLineData[0],BlackLinejumpu,BlackLinejumpd);
  else 
  { 
    BlackLineData[RowMax-1]=40;
    CommonRectificate(&BlackLineData[0],BlackLinejumpd,RowMax-1);
  }
  StableNumbers=StableNumbers-8;
  }
//  shizi=0;
//  sanjiao=0;
  }
 shizi=0;
 

}

void CommonRectificate(int data[],unsigned char begin,unsigned char end)
{
  unsigned char MidPos = 0;
  if (end > RowMax-1)
  {
    end = RowMax-1;
  }
  if (begin == end)
  {
    //		ThresholdData[begin][data[begin]]=White;
    
    data[begin] = (data[begin-1]+data[begin+1])/2;
    // 		str.Format("补偿%d:%d  \r\n",n++,begin);
    // 		fprintf(pfile,str);
    //		ThresholdData[begin][data[begin]]=100;
    
    //		ThresholdData[begin][BlackLineData[begin]]=White;
    
    //BlackLineData[begin] =  LeftBlack[begin] + (RightBlack[begin]-LeftBlack[begin])/2;
    
    //		ThresholdData[begin][BlackLineData[begin]]=128;
    
  }
  else if(begin < end)
  {
    MidPos = (begin+end)/2;	
    //		ThresholdData[MidPos][data[MidPos]]=White;
    data[MidPos] = (data[begin]+data[end])/2;
    // 		str.Format("补偿%d:%d  \r\n",n++,MidPos);
    // 		fprintf(pfile,str);
    //		ThresholdData[MidPos][data[MidPos]]=100;
    
    //		ThresholdData[MidPos][data[MidPos]]=100;
    //		ThresholdData[MidPos][BlackLineData[MidPos]]=White;
    //BlackLineData[MidPos] =  LeftBlack[MidPos] + (RightBlack[MidPos]-LeftBlack[MidPos])/2;	
    //		ThresholdData[MidPos][BlackLineData[MidPos]]=128;
    
    if (begin+1 < MidPos)
    {
      CommonRectificate(data,begin,MidPos);
    }
    if (MidPos+1 < end)
    {
      CommonRectificate(data,MidPos,end);
    }
  }
}






void StartLine()
{
  int i=0,j=0;
  int StartL=0,StartR=0; 
  int StartL1=0,StartR1=0;
  int Start1=0,Start2=0,Start3=0,Start4=0;
  int StartLLast=0,StartRLast=0;
  int StartLen=0;
    
     for(i=RowMax-3;i>RowMax-50;i-=2)
     {
       StartR=0; StartL=0;
//        Start1=0,Start2=0,Start3=0;
//        StartLen=0;
       for(j=BlackLineData[RowMax-10];j<ColumnMax-1;j++)  //右边黑线检测
       {
//        if(ThresholdData[i][j] == Black && ThresholdData[i][j+1] == White ){
//          Start1=1;
//          //StartR1=j;
//          j++;
//        }
         if(ThresholdData[i][j] == Black)// && ThresholdData[i+5][j] == White && ThresholdData[i-5][j] == White)
        {
          StartR++;
         
          //Start2=1;
          j++;}
        if(ThresholdData[i][j] == Black && ThresholdData[i][j+1] == White)
        {
          Start3=1;
          StartRLast=StartR;
          break;
       }
        
       }
       if(!Start3)
         StartR=0;
        
        
     for(j=BlackLineData[RowMax-10];j>BlackLineData[RowMax-10]-25 && j>2;j--) //左边黑线检测
       {
        
//        if(ThresholdData[i][j] == White && ThresholdData[i][j-1] == Black ){
//          Start2=1;
//           StartL1=j;
//          j--;
//        }
        if(ThresholdData[i][j] == Black )//&& ThresholdData[i+5][j] == White && ThresholdData[i-5][j] == White)
        {
          StartL++;
          //Start2=1;
          j--;}
        if(ThresholdData[i][j] == Black && ThresholdData[i][j-1] == White )
        {
          Start4=1;
          StartLLast=StartL;
          break;
        }
       
   
       }
       if(!Start4)
       StartL=0;
       //StartLen=StartR1-StartL1;
       if((StartL>4 || StartLLast>4) && (StartR>4 || StartRLast>4))
       {
       Start=1;
       CAR=7;
       uart_putchar(UART0, CAR);
       break;
       }
     }
     
}
void Findjump()
{
  int BlackEnd=0,WhiteEnd=0,End=0;
  unsigned char FoundBlack=0;
 
  
  int i=0,j=0,a=0;
  if(CFFlag)
  {
    CFFlag=0;
    CFCount++;
    if(CFCount>1)
      b++;
    //OLED_Print_Num(100,0,CFCount);
    enable_irq (PIT_IRQn);
    
  
  }
//  if(b%3==0)
//  {
//  for(i=RowMax-1;i>RowMax-BlackEndMax-1;i--)
//  {
//    LeftBlack[i]=0;RightBlack[i]=0;BlackLineData[i]=0;
//    for(j=EndC;j>0;j--)
//    {
//      if(ThresholdData[i][j] == White && ThresholdData[i][j-1] == Black){
//        LeftBlack[i]=j-1;
//        if(LeftBlack[i]>0)
//        break;
//      } 
//  
//    }
//    for(j=EndC;j<ColumnMax-1;j++){
//      if(ThresholdData[i][j] == White && ThresholdData[i][j+1] == Black){
//        RightBlack[i]=j+1;
//        if(RightBlack[i]<ColumnMax-1)
//        break;
//      }
//  }
//  
// if (!LeftBlack[i]) 
//		{ 
//			if (i < 8)
//			{
//				LeftBlack[i] = LeftBlack[i+1] + LeftBlack[i+1] - LeftBlack[i+2]; 
//			}
//			else
//			{
//				LeftBlack[i] = 0; 
//			}
//		}
//		else if (i < 8 && fabss(LeftBlack[i] - LeftBlack[i+1]) > 4 )
//		{
//			LeftBlack[i] = LeftBlack[i+1] + LeftBlack[i+1] - LeftBlack[i+2];
//		}
//
//        if (!RightBlack[i] || RightBlack[i]==ColumnMax-1)
//		{
//			if (i < 8)
//			{ 
//				RightBlack[i] = RightBlack[i+1] + RightBlack[i+1] - RightBlack[i+2];
//			}
//			else
//			{
//				RightBlack[i] = ColumnMax-1; 
//			}
//		}
//		else if (i < 8 && fabss(RightBlack[i] - RightBlack[i+1]) > 4 )
//		{
//			RightBlack[i] = RightBlack[i+1] + RightBlack[i+1] - RightBlack[i+2];
//		}
//        
// }
////  
//  
//  //找中线
////  int BlackLinejumpu=0,BlackLinejumpd=0;
////  for(i=RowMax-1;i>RowMax-End+3;i--)
////    BlackLineData[i]=(LeftBlack[i]+RightBlack[i])/2;
//  
////  
////   //直左转
//  int jumpl1=0,jumpl2=0;
//  if(LeftBlack[RowMax-2]==0 && LeftBlack[RowMax-4]==0)    //底部全白
//  {
//  for(i=RowMax-1;i>RowMax-BlackEndMax-1;i--)
//  {
//    if((LeftBlack[i+1]<LeftBlack[i-1]-4) || LeftBlack[i+1]==0 && LeftBlack[i-1]!=0 && LeftBlack[i-2]!=0)
//    {jumpl2=i;break;}
//  
//  }
//  BlackLineData[RowMax-3*jumpl2/4]=0;
//  BlackLineData[RowMax-1]=30;
//  StableNumbers=3*jumpl2/4;
//  CommonRectificate(&BlackLineData[0],RowMax-3*jumpl2/4,RowMax-1);
//  }
//  else
//  {
//  for(i=RowMax-BlackEndMax+5;i<RowMax-1;i++)
//  {
//    if((LeftBlack[i+1]<LeftBlack[i-1]-4) || LeftBlack[i+1]==0 && LeftBlack[i-1]!=0 && LeftBlack[i-2]!=0)
//    {jumpl2=i;break;}
//    
//  
//  }
//  for(i=RowMax-2;i>RowMax-BlackEndMax+5;i--)
//  {
//    if((LeftBlack[i-1]<LeftBlack[i+1]-4) || LeftBlack[i-1]==0 && LeftBlack[i+1]!=0 && LeftBlack[i+2]!=0)
//    {jumpl1=i;break;}
//  
//  }
//  StableNumbers=(jumpl1-jumpl2)*3/4+(RowMax-jumpl1);
//  BlackLineData[(jumpl2-jumpl1)*3/4+jumpl1]=0;
//  CommonRectificate(&BlackLineData[0],(jumpl2-jumpl1)*3/4+jumpl1,jumpl1+20);
//  }
//  
//  
  
 ////右转
//  int jumpr1=0,jumpr2=0;
//  if(RightBlack[RowMax-2]==ColumnMax-1 && RightBlack[RowMax-4]==ColumnMax-1)    //底部全白
//  {
//  for(i=RowMax-1;i>RowMax-BlackEndMax-1;i--)
//  {
//    if((RightBlack[i+1]>RightBlack[i-1]+4) || RightBlack[i+1]==ColumnMax-1 && RightBlack[i-1]!=ColumnMax-1 && RightBlack[i-2]!=ColumnMax-1)
//    {jumpr2=i;break;}
//  
//  }
//  BlackLineData[RowMax-3*(RowMax-jumpr2)/4]=ColumnMax-1;
//  BlackLineData[RowMax-1]=50;
//  StableNumbers=3*(RowMax-jumpr2)/4;
//  CommonRectificate(&BlackLineData[0],RowMax-3*(RowMax-jumpr2)/4,RowMax-1);
//  }
//  else
//  {
//  for(i=RowMax-BlackEndMax+5;i<RowMax-1;i++)
//  {
//    if((RightBlack[i+1]>RightBlack[i-1]+4) || RightBlack[i+2]==ColumnMax-1 && RightBlack[i+1]==ColumnMax-1 &&RightBlack[i-1]!=ColumnMax-1 && RightBlack[i-2]!=ColumnMax-1)
//    {jumpr2=i;break;}
//    
//  
//  }
//  for(i=RowMax-2;i>RowMax-BlackEndMax+5;i--)
//  {
//    if((RightBlack[i-1]>RightBlack[i+1]+4) || RightBlack[i-1]==ColumnMax-1 && RightBlack[i+1]!=ColumnMax-1 && RightBlack[i+2]!=ColumnMax-1)
//    {jumpr1=i;break;}
//  
//  }
//  StableNumbers=(jumpr1-jumpr2)*3/4+(RowMax-jumpr1);
//  BlackLineData[(jumpr2-jumpr1)*3/4+jumpr1]=ColumnMax-1;
//  CommonRectificate(&BlackLineData[0],(jumpr2-jumpr1)*3/4+jumpr1,jumpr1+7);
//  }
  
  
  //}
  
   
  //else{
  
  //直过
   End=0;
  for(j=LeftBlack[RowMax-2];j<RightBlack[RowMax-2];j+=2)
  {
    BlackEnd=0;
    WhiteEnd=0;
    FoundBlack=0;
    for(i=RowMax-1;i>0;i--){
    if(ThresholdData[i][j] == White && !FoundBlack)
      WhiteEnd++;
    if((ThresholdData[i][j] == White && ThresholdData[i-1][j] == Black) || (ThresholdData[i][j] == Black)){

       FoundBlack=1;
        break;
     
    }
  
    }
    if(WhiteEnd>=End){
      End=WhiteEnd;
      //StableNumbers=End;
      EndC=j;
    }
    BlackEnd=0;
    WhiteEnd=0;
    
  }
 // OLED_Print_Num(0,6,EndC);
  //OLED_Print_Num(60,2,End);
  
    
  for(i=RowMax-1;i>RowMax-End-1;i--)
  {
    LeftBlack[i]=0;RightBlack[i]=0;BlackLineData[i]=0;
    for(j=EndC;j>0;j--)
    {
      if(ThresholdData[i][j] == White && ThresholdData[i][j-1] == Black){
        LeftBlack[i]=j-1;
        if(LeftBlack[i]>0)
        break;
      } 
  
    }
    for(j=EndC;j<ColumnMax-1;j++){
      if(ThresholdData[i][j] == White && ThresholdData[i][j+1] == Black){
        RightBlack[i]=j+1;
        if(RightBlack[i]<ColumnMax-1)
        break;
      }
  }
  
 if (!LeftBlack[i]) 
		{ 
			if (i < 8)
			{
				LeftBlack[i] = LeftBlack[i+1] + LeftBlack[i+1] - LeftBlack[i+2]; 
			}
			else
			{
				LeftBlack[i] = 0; 
			}
		}
		else if (i < 8 && fabss(LeftBlack[i] - LeftBlack[i+1]) > 4 )
		{
			LeftBlack[i] = LeftBlack[i+1] + LeftBlack[i+1] - LeftBlack[i+2];
		}

        if (!RightBlack[i] || RightBlack[i]==ColumnMax-1)
		{
			if (i < 8)
			{ 
				RightBlack[i] = RightBlack[i+1] + RightBlack[i+1] - RightBlack[i+2];
			}
			else
			{
				RightBlack[i] = ColumnMax-1; 
			}
		}
		else if (i < 8 && fabss(RightBlack[i] - RightBlack[i+1]) > 4 )
		{
			RightBlack[i] = RightBlack[i+1] + RightBlack[i+1] - RightBlack[i+2];
		}
        if(LeftBlack[i]>=RightBlack[i])
            LeftBlack[i] = LeftBlack[i+1] + LeftBlack[i+1] - LeftBlack[i+2]; 
        
  }
  
  
  //找中线
  int BlackLinejumpu=0,BlackLinejumpd=0;
  for(i=RowMax-1;i>RowMax-End+3;i--)
    BlackLineData[i]=(LeftBlack[i]+RightBlack[i])/2;
  
  
  StableNumbers=End;
  for(i=RowMax-StableNumbers+8;i<RowMax-1;i+=2)
    if(fabss(BlackLineData[i-1]-BlackLineData[i+1])>4 && fabss(BlackLineData[i-2]-BlackLineData[i+2])>4)
    {BlackLinejumpu=i-3;break;}
  
    for(i=RowMax-1;i>BlackLinejumpu;i-=2)
    if(fabss(BlackLineData[i-1]-BlackLineData[i+1])>4 && fabss(BlackLineData[i-2]-BlackLineData[i+2])>4)
    {BlackLinejumpd=i+3;break;}
  
  
  
  if(BlackLinejumpd-BlackLinejumpu+3>3 && BlackLinejumpd!=0 && BlackLinejumpu!=0 )
    CommonRectificate(&BlackLineData[0],BlackLinejumpu,BlackLinejumpd);
  else 
  { 
    BlackLineData[RowMax-1]=40;
    CommonRectificate(&BlackLineData[0],BlackLinejumpd,RowMax-1);
  }
  
  
 // 直过
//  zhiguo yuan
//  
//    StableNumbers=End;
//    for(i=RowMax-1;i>RowMax-StableNumbers-3;i-=4)
//    if(fabss((BlackLineData[i-3]+BlackLineData[i-4])-(BlackLineData[i]+BlackLineData[i-1]))>4)
//    {BlackLinejumpd=i+2;break;}
//  
//  for(i=RowMax-StableNumbers+5;i<RowMax-1;i+=4)
//    if(fabss((BlackLineData[i+3]+BlackLineData[i+4])-(BlackLineData[i]+BlackLineData[i+1]))>4)
//    {BlackLinejumpu=i-1;break;}
//  
//  if(BlackLinejumpd-BlackLinejumpu+3>3 && BlackLinejumpd!=0 && BlackLinejumpu!=0 )
//    CommonRectificate(&BlackLineData[0],BlackLinejumpu,BlackLinejumpd);
//  else 
//  { 
//    BlackLineData[RowMax-1]=40;
//    CommonRectificate(&BlackLineData[0],BlackLinejumpu,RowMax-1);
//  }
//  
//  StableNumbers=End;
//  for(i=RowMax-1;i>RowMax-StableNumbers-3;i-=4)
//    if(fabss((BlackLineData[i-3]+BlackLineData[i-4])-(BlackLineData[i]+BlackLineData[i-1]))>4)
//    {BlackLinejumpd=i+2;break;}
//  
//  for(i=RowMax-StableNumbers+5;i<RowMax-1;i+=4)
//    if(fabss((BlackLineData[i+3]+BlackLineData[i+4])-(BlackLineData[i]+BlackLineData[i+1]))>4)
//    {BlackLinejumpu=i-1;break;}
//  
//  if(fabss(BlackLinejumpu-BlackLinejumpd+3)>3 && BlackLinejumpu< RowMax-20 && BlackLinejumpu != 0 && BlackLinejumpd != 0)
//    CommonRectificate(&BlackLineData[0],BlackLinejumpu,BlackLinejumpd+5);
//  else if(!BlackLinejumpd && BlackLinejumpu<RowMax-10)
//    CommonRectificate(&BlackLineData[0],BlackLinejumpu,RowMax-1);
  
  

  
  //直左转
//  int jumpl1=0,jumpl2=0;
//  if(LeftBlack[RowMax-2]==0 && LeftBlack[RowMax-4]==0)    //底部全白
//  {
//  for(i=RowMax-1;i>RowMax-BlackEndMax-1;i--)
//  {
//    if((LeftBlack[i+1]<LeftBlack[i-1]-4) || LeftBlack[i+1]==0 && LeftBlack[i-1]!=0 && LeftBlack[i-2]!=0)
//    {jumpl2=i;break;}
//  
//  }
//  BlackLineData[RowMax-3*jumpl2/4]=0;
//  BlackLineData[RowMax-1]=30;
//  StableNumbers=3*jumpl2/4;
//  CommonRectificate(&BlackLineData[0],RowMax-3*jumpl2/4,RowMax-1);
//  }
//  else
//  {
//  for(i=RowMax-BlackEndMax+5;i<RowMax-1;i++)
//  {
//    if((LeftBlack[i+1]<LeftBlack[i-1]-4) || LeftBlack[i+1]==0 && LeftBlack[i-1]!=0 && LeftBlack[i-2]!=0)
//    {jumpl2=i;break;}
//    
//  
//  }
//  for(i=RowMax-2;i>RowMax-BlackEndMax+5;i--)
//  {
//    if((LeftBlack[i-1]<LeftBlack[i+1]-4) || LeftBlack[i-1]==0 && LeftBlack[i+1]!=0 && LeftBlack[i+2]!=0)
//    {jumpl1=i;break;}
//  
//  }
//  StableNumbers=(jumpl1-jumpl2)*3/4+(RowMax-jumpl1);
//  BlackLineData[(jumpl2-jumpl1)*3/4+jumpl1]=0;
//  CommonRectificate(&BlackLineData[0],(jumpl2-jumpl1)*3/4+jumpl1,jumpl1+20);
//  }
  
  
  
  //直右转
  
//  int jumpr1=0,jumpr2=0;
//  if(RightBlack[RowMax-2]==ColumnMax-1 && RightBlack[RowMax-4]==ColumnMax-1)    //底部全白
//  {
//  for(i=RowMax-1;i>RowMax-BlackEndMax-1;i--)
//  {
//    if((RightBlack[i+1]>RightBlack[i-1]+4) || RightBlack[i+1]==ColumnMax-1 && RightBlack[i-1]!=ColumnMax-1 && RightBlack[i-2]!=ColumnMax-1)
//    {jumpr2=i;break;}
//  
//  }
//  BlackLineData[RowMax-3*(RowMax-jumpr2)/4]=ColumnMax-1;
//  BlackLineData[RowMax-1]=50;
//  StableNumbers=3*(RowMax-jumpr2)/4;
//  CommonRectificate(&BlackLineData[0],RowMax-3*(RowMax-jumpr2)/4,RowMax-1);
//  }
//  else
//  {
//  for(i=RowMax-BlackEndMax+5;i<RowMax-1;i++)
//  {
//    if((RightBlack[i+1]>RightBlack[i-1]+4) || RightBlack[i+2]==ColumnMax-1 && RightBlack[i+1]==ColumnMax-1 &&RightBlack[i-1]!=ColumnMax-1 && RightBlack[i-2]!=ColumnMax-1)
//    {jumpr2=i;break;}
//    
//  
//  }
//  for(i=RowMax-2;i>RowMax-BlackEndMax+5;i--)
//  {
//    if((RightBlack[i-1]>RightBlack[i+1]+4) || RightBlack[i-1]==ColumnMax-1 && RightBlack[i+1]!=ColumnMax-1 && RightBlack[i+2]!=ColumnMax-1)
//    {jumpr1=i;break;}
//  
//  }
//  StableNumbers=(jumpr1-jumpr2)*3/4+(RowMax-jumpr1);
//  BlackLineData[(jumpr2-jumpr1)*3/4+jumpr1]=ColumnMax-1;
//  CommonRectificate(&BlackLineData[0],(jumpr2-jumpr1)*3/4+jumpr1,jumpr1+7);
//  }
//  }

}
int q=0;
void ScanRoad()
{
  unsigned char zdczlf=0;
  unsigned char zdczrf=0;
  unsigned int zdcznumberl=0;
  unsigned int zdcznumberr=0;
  unsigned int Roadwide=0;
  int i=0;

 /*  
if(zdczl || zdczr)
{
   if(zdczl && LeftBlack[i]-LeftBlack[i+2]>4 && i>50)
   {
    // MidPos=40;
     zdczl=0;
     zdflag=0;
     
   }

   if(zdczr && RightBlack[i+2]-RightBlack[i]>4 && i>55)
   {
   //MidPos=40;
   zdczr=0;
   zdflag=0;
   
   }
   
   
}*/
   //判断赛道是否一边变宽
  for(i=RowMax-7;i>RowMax-BlackEndMax && i>25;i--)
  {
   
        if(((RightBlack[i]-LeftBlack[i]<50 || RightBlack[i]-LeftBlack[i+1]<50  || RightBlack[i]-LeftBlack[i+2]<50) && i>80) 
       ||((RightBlack[i]-LeftBlack[i]<35 || RightBlack[i]-LeftBlack[i+1]<35  || RightBlack[i]-LeftBlack[i+2]<35) && i>55))
    {
      zdczl=zdczr=0;
      break;
    
    }
  
      if((RightBlack[i]-RightBlack[i+4]>4 ||(RightBlack[i-1]==ColumnMax-1 && RightBlack[i+1] != ColumnMax-1 && RightBlack[i]==ColumnMax-1 && RightBlack[i+2]!=ColumnMax-1)) && !zdczrf)
      {zdczrf=1;
      Roadwide=RightBlack[i+5]-LeftBlack[i+5];
      }
      if(zdczrf)
      { if(RightBlack[i]-LeftBlack[i]>Roadwide+4)
         {
          zdcznumberr++;
          if(zdcznumberr>=15)
          {
            //zdcznumberr++;
//            zhidaonumber++;
//            if(zhidaonumber>1)
//            {
              //zhidaonumber=0;
              zdczr=1;zdflag=0;zdczrc1=i+18;
            //}
            //else zhidaonumber=0;         
          }
          
         }
      //else zdcznumber=0;
      }
      
//      if(zdczr && RightBlack[i+10]>RightBlack[i+13])
//      {
//      zdczr=0;
//      
//      }
//      if(zdczrc1 && RightBlack[i+2]-RightBlack[i]>4 && i<RowMax-15)
//      {
//        //zdflag=1;
//        zdczrc2=i;
//        zdczr=1;
//        
//      }
      
      
      
      if((LeftBlack[i+2]-LeftBlack[i]>4 || (LeftBlack[i-1]==0 && LeftBlack[i+1]!=0 && LeftBlack[i-2]==0 && LeftBlack[i+2]!=0))&& !zdczlf)
      {zdczlf=1;
        
      Roadwide=RightBlack[i+3]-LeftBlack[i+3];
      }
      if(zdczlf)
      { if(RightBlack[i]-LeftBlack[i]>Roadwide+4)
         {
          zdcznumberl++;
          if(zdcznumberl>=10)
          {
           // zhidaonumber++;
            //if(zhidaonumber>1)
            //{
              zdczl=1;zdflag=0;zdczlc1=i+13;
            //}
          }
//          else zhidaonumber=0;
//             if(zdczlc1<RowMax-30)
//            zdczl=0;
          }
         
         
      //else zdcznumber=0;
//      if(zdczl && LeftBlack[i+8]<LeftBlack[i+11])
//      {
//      zdczl=0;
//      
//      }
//      if(LeftBlack[i]-LeftBlack[i+2]>4 && i<RowMax-15)
//      {
//      zdflag=1;
//      zdczl=1;
//      zdczlc2=i;
//      }
      }
     
       
    
  }
   
 if((zdczl && zdczrf) || (zdczr && zdczlf) || (zdczl && zdczr))
       zdczl=zdczr=zdczlc1=zdczrc1=0;
   
   
   
   
   if(zdczl || zdczr)
   {
     if(CAR==0)
     {zdczl=zdczr=0;}
     else if(CAR==1)
     {
       CAR=4;
       uart_putchar(UART0,CAR);
       CAR=3;
     }
//     else if(CAR==3)
//     {
//      CAR==0;
//      uart_putchar(UART0,CAR);
//     }
     
   }
//   //if(zdczrc2)
//     {
//       if(CAR==3)
//       {
//         CAR=4;
//         uart_putchar(UART0,CAR);
//         CAR=1;
//       }
       
//        uart_putchar(UART0,CAR);
//        CAR=1;
        //enable_irq (PIT_IRQn);
        //aa++;
        // uart_putchar(UART0,aa );
        //OLED_Print_Num(60,6,aa);
        
     //}



}



