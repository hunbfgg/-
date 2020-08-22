///////////////////////////////////////////////////////
//����51��������ˮ��
/*ģ�鼰���ߣ�
DS1302
			DAT P3^3;
			RST P3^5;
			CLK P3^6;
			VCC 5V
			GND GND
ISD1820
			VCC 5V
			GND GND
			P-E P1^1
DS18B20
			VCC 5V
			GND GNd
			DQ  P3^7
�̵���
			VCC 5V
			GND GND
			S   P2^4
HX711����ģ��
			VCC 5V
			GND GND
			DT 	P2^1
			SCK P2^0
LED ��ɫ P1^2
		��ɫ P1^3
		��ɫ P1^4

*/
///////////////////////////////////////////////////////



#include "ds1302.h"
#include "main.h"
#include "HX711.h"
#include "uart.h"
#include "LCD1602.h"
#include "temp.h"
#define GapValue 390
#define   uchar unsigned char
#define   uint unsigned int
	
unsigned long HX711_Buffer = 0;
unsigned long Weight_Maopi = 0;
long Weight_Shiwu = 0;
unsigned char flag = 0;
bit Flag_ERROR = 0;
bit Flag_send = 0;
sbit speak= P3^5;
unsigned char a;

uint e,i,l,q,c,b,g,h,i,f,j,wendu=30,min=0;
uint time_1=0,zh_1,zh_2;
uchar smg[10]={'0','1','2','3','4','5','6','7','8','9'};
uchar smg1[1]={'-'};
unsigned char datas[] = {0, 0, 0, 0, 0}; 
uchar time1[7];

//У׼����
//��Ϊ��ͬ�Ĵ������������߲��Ǻ�һ�£���ˣ�ÿһ����������Ҫ�������������������ʹ����ֵ��׼ȷ��
//�����ֲ��Գ���������ƫ��ʱ�����Ӹ���ֵ��
//������Գ���������ƫСʱ����С����ֵ��
//��ֵ����ΪС��

sbit LED=P2^1;
sbit k1=P1^5;	 //����P15����k1
sbit k2=P1^6;
sbit k3=P1^7;
sbit LED1=P1^4;
sbit LED2=P1^3;
sbit LED3=P1^2;
sbit PE=P1^1;
sbit relay=P2^4;

uchar bcd_to_hex(uchar dat1)
{
	uchar temp;
	temp=((dat1>>4)*10+(dat1&0x0f));
	return temp;
}

//uchar hex_to_bcd(uchar dat2)
//{
//	uchar temp;
//	temp=(((dat2/10)<<4)+(dat2%10));
//	return temp;
//}


void datapros() 
{
	Ds1302ReadTime();
	
	e=bcd_to_hex(TIME[2])/10;
	i=bcd_to_hex(TIME[2])%10;
	
	l=bcd_to_hex(TIME[1])/10;
	q=bcd_to_hex(TIME[1])%10;
	if(q-min==1)
		time_1++;
	min=q;
	c=bcd_to_hex(TIME[0])/10;
	b=bcd_to_hex(TIME[0])%10;
	
	
//	LCD1602_write_com(0x80+0x40+7);
//	LCD1602_write_data(smg[e]);
//	LCD1602_delay_ms(1); 
//	
//	LCD1602_write_com(0x80+0x40+8);
//	LCD1602_write_data(smg[i]);
//	LCD1602_delay_ms(1); 
//	
//	LCD1602_write_com(0x80+0x40+9);
//	LCD1602_write_data(smg1[0]);
//	LCD1602_delay_ms(1); 
//	
//	LCD1602_write_com(0x80+0x40+10);
//	LCD1602_write_data(smg[l]);
//	LCD1602_delay_ms(1); 
//	
//	LCD1602_write_com(0x80+0x40+11);
//	LCD1602_write_data(smg[q]);
//	LCD1602_delay_ms(1); 
//	
//	LCD1602_write_com(0x80+0x40+12);
//	LCD1602_write_data(smg1[0]);
//	LCD1602_delay_ms(1); 
//	
//	LCD1602_write_com(0x80+0x40+13);
//	LCD1602_write_data(smg[c]);
//	LCD1602_delay_ms(1); 
//	
//	LCD1602_write_com(0x80+0x40+14);
//	LCD1602_write_data(smg[b]);
//	LCD1602_delay_ms(1); 
	LCD1602_write_com(0x80+0x40+15);
	LCD1602_write_data(smg[time_1]);
	LCD1602_delay_ms(1); 
	
}

void keypros()
{

	if(k1==0)		  //��������
	{	
		LCD1602_delay_ms(10);
		if(k1==0)	 //�ٴ��жϰ����Ƿ���
		{
			wendu++;
			LCD1602_write_com(0x80+0x40+13);
			LCD1602_write_data(smg[c]);
			LCD1602_delay_ms(1); 
			LCD1602_write_com(0x80+0x40+13);
			LCD1602_write_data(smg[c]);
			LCD1602_delay_ms(1); 
		}
		while(!k1);	 //��ⰴ���Ƿ��ɿ�
	}		
	if(k2==0)		  //��������
	{	
		LCD1602_delay_ms(10);
		if(k2==0)	 //�ٴ��жϰ����Ƿ���
		{
			wendu--;
		}
		while(!k2);	 //��ⰴ���Ƿ��ɿ�
	}		
	if(k3==0)		  //�������Ա����
	{	
		LCD1602_delay_ms(10);  //�������� һ���Լ10ms
		if(k3==0)	 //�ٴ��жϰ����Ƿ���
		{
			LCD1602_write_com(0x80+0x40+12);
			LCD1602_write_data('O');
			LCD1602_delay_ms(1); 
			LCD1602_write_com(0x80+0x40+13);
			LCD1602_write_data('K');
			LCD1602_delay_ms(1000); 
			LCD1602_delay_ms(1000); 
		}
		while(!k3);	 //��ⰴ���Ƿ��ɿ�
	}		
	LCD1602_write_com(0x80+0x40+12);
	LCD1602_write_data(smg[wendu/10]);
	LCD1602_delay_ms(1); 
	LCD1602_write_com(0x80+0x40+13);
	LCD1602_write_data(smg[wendu%10]);
	LCD1602_delay_ms(1); 
}

void LcdDisplay(int temp) 	 //lcd��ʾ
{
	float tp;  
	if(temp< 0)				//���¶�ֵΪ����
  	{
	  	LCD1602_write_com(0x87);		//д��ַ 80��ʾ��ʼ��ַ
		SBUF='-';//�����յ������ݷ��뵽���ͼĴ���
		while(!TI);			         //�ȴ������������
		TI=0;						 //���������ɱ�־λ
	   LCD1602_write_data('-');  		//��ʾ��
		//��Ϊ��ȡ���¶���ʵ���¶ȵĲ��룬���Լ�1����ȡ�����ԭ��
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
		//������С�����*100��+0.5���������룬��ΪC���Ը�����ת��Ϊ���͵�ʱ���С����
		//��������Զ�ȥ���������Ƿ����0.5����+0.5֮�����0.5�ľ��ǽ�1�ˣ�С��0.5�ľ�
		//����?.5��������С������档
 
  	}
 	else
  	{			
	  	LCD1602_write_com(0x87);		//д��ַ 80��ʾ��ʼ��ַ
	    LCD1602_write_data(' '); 		//��ʾ��
		SBUF='+';//�����յ������ݷ��뵽���ͼĴ���
		while(!TI);			         //�ȴ������������
		TI=0;						 //���������ɱ�־λ
		tp=temp;//��Ϊ���ݴ�����С�������Խ��¶ȸ���һ�������ͱ���
		//����¶���������ô����ô������ԭ����ǲ���������
		temp=tp*0.0625*100+0.5;	
		//������С�����*100��+0.5���������룬��ΪC���Ը�����ת��Ϊ���͵�ʱ���С����
		//��������Զ�ȥ���������Ƿ����0.5����+0.5֮�����0.5�ľ��ǽ�1�ˣ�С��0.5�ľ�
		//�����0.5��������С������档
	}
	datas[0] = temp / 10000;
	datas[1] = temp % 10000 / 1000;
	datas[2] = temp % 1000 / 100;
	datas[3] = temp % 100 / 10;
	datas[4] = temp % 10;
	
	
	LCD1602_write_com(0x80+8);		 //д��ַ 80��ʾ��ʼ��ַ
	LCD1602_write_data('0'+datas[1]); //ʮλ
	

	LCD1602_write_com(0x80+9);		//д��ַ 80��ʾ��ʼ��ַ
	LCD1602_write_data('0'+datas[2]); //��λ 
	

	LCD1602_write_com(0x80+10);		//д��ַ 80��ʾ��ʼ��ַ
	LCD1602_write_data('.'); 		//��ʾ ��.��
	

	LCD1602_write_com(0x80+11);		 //д��ַ 80��ʾ��ʼ��ַ
	LCD1602_write_data('0'+datas[3]); //��ʾС����  
	

	LCD1602_write_com(0x80+12);		 //д��ַ 80��ʾ��ʼ��ַ
	LCD1602_write_data('0'+datas[4]); //��ʾС���� 

	 
}



//****************************************************
//������
//****************************************************
void main()
{
	Uart_Init();
	Send_Word("Welcome to use!\n");

	Init_LCD1602();
	Ds1302Init();
	LCD1602_write_com(0x80);
	LCD1602_write_word("Wendu:");
	LCD1602_write_com(0x80+0x40);
	LCD1602_write_word("Set wendu:");
	Delay_ms(1000);		 //��ʱ,�ȴ��������ȶ�

	Get_Maopi();				//��ëƤ����
	
	while(1)
	{
		LED3=1;
		PE=1;
		if(RI == 1)
		{
			a=SBUF;//��ȡ��������
			RI = 0;    //
			switch (a)
			{
				case 'a':
				Get_Weight();
				Send_ASCII(Weight_Shiwu/1000 + 0X30);
				Send_ASCII(Weight_Shiwu%1000/100 + 0X30);
				Send_ASCII(Weight_Shiwu%100/10 + 0X30);
				Send_ASCII(Weight_Shiwu%10 + 0X30);
				Send_Word("g-");
				break;	
				case 'b':
					Get_Maopi();
					break;
				case 'c':
					Flag_send=1;
					break;
				case 'd':
					Flag_send=0;
					break;
			}
		}
			
		Get_Weight();			//����

		Scan_Key();

		//��ʾ��ǰ����
		if( Flag_ERROR == 1)
		{
            Send_Word("ERROR\n");
			
			LCD1602_write_com(0x80+0x40);
			LCD1602_write_word("ERROR ");
			speak=0;
		}		
		else
		{         
			speak=1;
//            LCD1602_write_com(0x80+0x40);
//					  LCD1602_write_data(Weight_Shiwu/1000 + 0X30);
//            LCD1602_write_data(Weight_Shiwu%1000/100 + 0X30);
//						if((zh_1-Weight_Shiwu%1000/100)==1)
//							zh_2=1;
//						zh_1=Weight_Shiwu%1000/100;
//            LCD1602_write_data(Weight_Shiwu%100/10 + 0X30);
//            LCD1602_write_data(Weight_Shiwu%10 + 0X30);
//					  LCD1602_write_word(" g");
			if( Flag_send == 1)
			{
				Send_ASCII(Weight_Shiwu/1000 + 0X30);
				Send_ASCII(Weight_Shiwu%1000/100 + 0X30);
				Send_ASCII(Weight_Shiwu%100/10 + 0X30);
				Send_ASCII(Weight_Shiwu%10 + 0X30);
				Send_Word("g-");
			}
						
		}
	datapros();
	keypros();
	LcdDisplay(Ds18b20ReadTemp());
		if(datas[1]*10+datas[2]>=wendu)
		{
			relay=0;
			LED2=1;
			LED1=0;
		}
		else
		{
			relay=1;
			LED2=0;
			LED1=1;
		}
		if((zh_1-Weight_Shiwu%1000/100)==1)
			zh_2=1;
		zh_1=Weight_Shiwu%1000/100;
		if(zh_2==1)
		{
			zh_2=0;
			time_1=0;
		}
		if((e*10+i)<21&&(e*10+i)>6) //����6�������Ͼŵ㹤��
		{
			if(time_1>2)   //3��������һ��
			{
				time_1=0;
				for(i=0;i<3;i++)
				{
				PE=0;
				Delay_ms(1000);
			
				PE=1;
				Delay_ms(1000);
					Delay_ms(1000);
					Delay_ms(1000);
					Delay_ms(1000);
				}
			}
		}
		
	}
}

//ɨ�谴��
void Scan_Key()
{
	if(KEY1 == 0)
	{
		Delay_ms(5);
		if(KEY1 == 0)
		{
			while(KEY1 == 0);
				Get_Maopi();
		}	
	}
}

//****************************************************
//����
//****************************************************
void Get_Weight()
{
	Weight_Shiwu = HX711_Read();
	Weight_Shiwu = Weight_Shiwu - Weight_Maopi;		//��ȡ����
	if(Weight_Shiwu > 0)			
	{	
		Weight_Shiwu = (unsigned int)((float)Weight_Shiwu/GapValue); 	//����ʵ���ʵ������
																		
																		
		if(Weight_Shiwu > 5000)		//���ر���
		{
			Flag_ERROR = 1;	
		}
		else
		{
			Flag_ERROR = 0;
		}
	}
	else
	{
		Weight_Shiwu = 0;
	//	Flag_ERROR = 1;				//���ر���
	}
	
}

//****************************************************
//��ȡëƤ����
//****************************************************
void Get_Maopi()
{
	Weight_Maopi = HX711_Read();	
} 

//****************************************************
//MS��ʱ����(12M�����²���)
//****************************************************
void Delay_ms(unsigned int n)
{
	unsigned int  i,j;
	for(i=0;i<n;i++)
		for(j=0;j<123;j++);
}