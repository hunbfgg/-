#include "uart.h"

// unsigned char RX_Buffer[30];
// unsigned char Num = 0;
// bit Flag_Receive_OK = 0;


void Uart_Init()					  			   
{
	SCON=0x50;/*SCON:ģʽ1,8-bitUART,ʹ�ܽ���*/
	TMOD=0x20;/*TMOD:timer1,mode2, 8-bitreload*/
	TH1=0xfd;/*TH1:reloadvaluefor9600baud@11.0592MHz*/
	TL1=0xfd;
	TR1=1;//������ʱ��T1
	EA=1;//�����ж�
	ES=1;//�򿪴����ж�
}

void Send_Word(unsigned char *p)				//�����ַ���
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

void Send_ASCII(unsigned char c)				//����һ���ַ�
{ 
    TI=0;   
    SBUF=c;   
    while(TI==0);   
    TI=0;   
}























