#include "common.h"
#include "include.h"
#include "shangweiji.h"
extern int adc;
extern int SpeedNumber;
extern int Start_end_link;
extern int zaw_mode;
extern float  left_speed ,Right_speed;
extern float Real_speed;//实际速度
extern int SpeedNumber;
extern float MidPos;
extern float Error;
extern int Excursion;
extern float Duty1;//???
extern float  Differential_V; 
extern double P;
extern double D;
#define RX_SU  1
#define RX_ER 0
uint8 flag_rx_succsess=RX_ER;
int8 FloatSave[12];
int8 RX[14];
int8 RX_FLAG=0;
int8 sa[8];
uint8 count_RX;
int str_ln=0;
struct NUM_GET    
{
	float A;
	float B;
	float C;	
};
struct NUM_GET NUM_A;
struct NUM_GET NUM_B;
struct NUM_GET NUM_C;
struct NUM_GET NUM_D;
struct NUM_GET NUM_E;
void ReceFloat(void);
void NUM_GET(void);
void NUM_Send(void);
void FloatToChar(float floatNum,  char* byteArry);
void putstr(char *s, char a);

extern float FLAG;
extern char CAR;
extern int speedl;   
extern int speedr;
   
  


void uart_IN_init(void)
{
  set_vector_handler(UART0_VECTORn,uart0_handler); 
  uart_rx_irq_en (UART0);   
}

unsigned char jc;
void uart0_handler(void)  
                     { 
                       
                                  
                       UARTn_e uratn = UART0;
                       //jc=CAR;
                       
                       if(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK) //接收数据寄存器满 
                       { 
                         //用户需要处理接收数据 
                        
                         
                        uart_getchar (UART0, &CAR); //无限等待接受1个字节
                        //if(CAR>8)
                          //CAR=jc;
                        //CAR=CAR;
                         //CAR=UART_S1_REG(UARTN[uratn]);
                         //uart_putchar (UART3 , ch); //发送字符串 
                       }
                       
                       
                       
                       
//                  uart_getchar(UART0, &RX[count_RX]); 
//	          if( RX[0] == '#' )
//	          count_RX++;
//	          else count_RX=0;		 	
//	          if(count_RX==14)
//		{
//		count_RX=0;
//                            flag_rx_succsess =RX_SU;	
//		NUM_GET();    
//		count_RX=0;
//		}
//                P++;
}

void ReceFloat(void)
{

	{
		RX_FLAG=RX[1];
		FloatSave[0]=RX[2];
		FloatSave[1]=RX[3];
		FloatSave[2]=RX[4];
		FloatSave[3]=RX[5];

		FloatSave[4]=RX[6];
		FloatSave[5]=RX[7];
		FloatSave[6]=RX[8];
		FloatSave[7]=RX[9];

		FloatSave[8]=RX[10];
		FloatSave[9]=RX[11];
		FloatSave[10]=RX[12];
		FloatSave[11]=RX[13];
		
	}
}
void NUM_GET(void)
{
           if(flag_rx_succsess==RX_SU)
		{
                            
		ReceFloat();
		if(RX_FLAG=='A')
		{
	        memcpy((char*)(&FLAG),&FloatSave[0],4);//void *memcpy(void *dest, const void *src, size_t n);  ??src????????????????N??????dest????????????  
		memcpy((char*)(&CAR),&FloatSave[4],4);
		memcpy((char*)(&D),&FloatSave[8],4);;
		}
		else if(RX_FLAG=='B')
		{
	        memcpy((char*)(&NUM_B.A),&FloatSave[0],4);
		memcpy((char*)(&NUM_B.B),&FloatSave[4],4);
		memcpy((char*)(&NUM_B.C),&FloatSave[8],4);
		}
		else if(RX_FLAG=='C')
		{
	        memcpy((char*)(&NUM_C.A),&FloatSave[0],4);
		memcpy((char*)(&NUM_C.B),&FloatSave[4],4);
		memcpy((char*)(&NUM_C.C),&FloatSave[8],4);
		}
		else if(RX_FLAG=='D')
		{
	         memcpy((char*)(&NUM_D.A),&FloatSave[0],4);
		memcpy((char*)(&NUM_D.B),&FloatSave[4],4);
		memcpy((char*)(&NUM_D.C),&FloatSave[8],4);
		}
		else if(RX_FLAG=='E')
		{
		memcpy(((char*)&NUM_E.A),&FloatSave[0],4);
		memcpy((char*)(&NUM_E.B),&FloatSave[4],4);
		memcpy((char*)(&NUM_E.C),&FloatSave[8],4);
		}
		else ;
		flag_rx_succsess=RX_ER;
		}
}
  
void FloatToChar(float floatNum, char* byteArry)

{   
     int FloatToChar_a,FloatToChar_b,FloatToChar_c,FloatToChar_d,FloatToChar_e,FloatToChar_f,FloatToChar_g;
     FloatToChar_a=(int)floatNum;
     FloatToChar_b=(int)(floatNum*10-FloatToChar_a*10);
     FloatToChar_c=(int)(floatNum*100-((int)(floatNum*10))*10);
     FloatToChar_d=(int)(floatNum*1000-((int)(floatNum*100))*10); 
     FloatToChar_e=(int)(floatNum*10000-((int)(floatNum*1000))*10);
     FloatToChar_f=(int)(floatNum*100000-((int)(floatNum*10000))*10);
     FloatToChar_g=(int)(floatNum*1000000-((int)(floatNum*100000))*10);
     str_ln=sprintf(byteArry,"%d.%d%d%d%d%d%d",FloatToChar_a,FloatToChar_b,FloatToChar_c,FloatToChar_d,FloatToChar_e,FloatToChar_f,FloatToChar_g); 
     
}
void putstr(char *s, char a)
{
	uart_putchar(UART0,'#');
	
	uart_putchar(UART0,a);
		
	uart_putbuff (UART0,s,8);
		
	uart_putchar(UART0,a);
	
         uart_putchar(UART0,'#');
	

}
void NUM_Send(void)
{  
	FloatToChar(speedl,sa);//实际速度
	putstr(sa,'A');
        
	FloatToChar(CAR,sa);
	putstr(sa,'B');
//        
//	FloatToChar(zaw_mode,sa);
//	putstr(sa,'C');
//        
//	FloatToChar( zaw_mode,sa);
//	putstr(sa,'D');
        
        
              
	 
}
