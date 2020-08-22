#include "uart.h"

// unsigned char RX_Buffer[30];
// unsigned char Num = 0;
// bit Flag_Receive_OK = 0;


void Uart_Init()					  			   
{
	SCON=0x50;/*SCON:模式1,8-bitUART,使能接收*/
	TMOD=0x20;/*TMOD:timer1,mode2, 8-bitreload*/
	TH1=0xfd;/*TH1:reloadvaluefor9600baud@11.0592MHz*/
	TL1=0xfd;
	TR1=1;//启动定时器T1
	EA=1;//打开总中断
	ES=1;//打开串口中断
}

void Send_Word(unsigned char *p)				//发送字符串
{	
 	while(*p)
 	{
	 	SBUF=*(p++);
		while(TI==0)
		{
			;
			;
			;
		};
		TI=0;
	}   
}

void Send_ASCII(unsigned char c)				//发送一个字符
{ 
    TI=0;   
    SBUF=c;   
    while(TI==0);   
    TI=0;   
}























