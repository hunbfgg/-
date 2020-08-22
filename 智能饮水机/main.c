///////////////////////////////////////////////////////
//基于51的智能饮水机
/*模块及接线：
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
继电器
			VCC 5V
			GND GND
			S   P2^4
HX711称重模块
			VCC 5V
			GND GND
			DT 	P2^1
			SCK P2^0
LED 黄色 P1^2
		绿色 P1^3
		红色 P1^4

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

//校准参数
//因为不同的传感器特性曲线不是很一致，因此，每一个传感器需要矫正这里这个参数才能使测量值很准确。
//当发现测试出来的重量偏大时，增加该数值。
//如果测试出来的重量偏小时，减小改数值。
//该值可以为小数

sbit LED=P2^1;
sbit k1=P1^5;	 //定义P15口是k1
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

	if(k1==0)		  //输入密码
	{	
		LCD1602_delay_ms(10);
		if(k1==0)	 //再次判断按键是否按下
		{
			wendu++;
			LCD1602_write_com(0x80+0x40+13);
			LCD1602_write_data(smg[c]);
			LCD1602_delay_ms(1); 
			LCD1602_write_com(0x80+0x40+13);
			LCD1602_write_data(smg[c]);
			LCD1602_delay_ms(1); 
		}
		while(!k1);	 //检测按键是否松开
	}		
	if(k2==0)		  //设置密码
	{	
		LCD1602_delay_ms(10);
		if(k2==0)	 //再次判断按键是否按下
		{
			wendu--;
		}
		while(!k2);	 //检测按键是否松开
	}		
	if(k3==0)		  //输入管理员密码
	{	
		LCD1602_delay_ms(10);  //消除抖动 一般大约10ms
		if(k3==0)	 //再次判断按键是否按下
		{
			LCD1602_write_com(0x80+0x40+12);
			LCD1602_write_data('O');
			LCD1602_delay_ms(1); 
			LCD1602_write_com(0x80+0x40+13);
			LCD1602_write_data('K');
			LCD1602_delay_ms(1000); 
			LCD1602_delay_ms(1000); 
		}
		while(!k3);	 //检测按键是否松开
	}		
	LCD1602_write_com(0x80+0x40+12);
	LCD1602_write_data(smg[wendu/10]);
	LCD1602_delay_ms(1); 
	LCD1602_write_com(0x80+0x40+13);
	LCD1602_write_data(smg[wendu%10]);
	LCD1602_delay_ms(1); 
}

void LcdDisplay(int temp) 	 //lcd显示
{
	float tp;  
	if(temp< 0)				//当温度值为负数
  	{
	  	LCD1602_write_com(0x87);		//写地址 80表示初始地址
		SBUF='-';//将接收到的数据放入到发送寄存器
		while(!TI);			         //等待发送数据完成
		TI=0;						 //清除发送完成标志位
	   LCD1602_write_data('-');  		//显示负
		//因为读取的温度是实际温度的补码，所以减1，再取反求出原码
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
		//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
		//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
		//算由?.5，还是在小数点后面。
 
  	}
 	else
  	{			
	  	LCD1602_write_com(0x87);		//写地址 80表示初始地址
	    LCD1602_write_data(' '); 		//显示正
		SBUF='+';//将接收到的数据放入到发送寄存器
		while(!TI);			         //等待发送数据完成
		TI=0;						 //清除发送完成标志位
		tp=temp;//因为数据处理有小数点所以将温度赋给一个浮点型变量
		//如果温度是正的那么，那么正数的原码就是补码它本身
		temp=tp*0.0625*100+0.5;	
		//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
		//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
		//算加上0.5，还是在小数点后面。
	}
	datas[0] = temp / 10000;
	datas[1] = temp % 10000 / 1000;
	datas[2] = temp % 1000 / 100;
	datas[3] = temp % 100 / 10;
	datas[4] = temp % 10;
	
	
	LCD1602_write_com(0x80+8);		 //写地址 80表示初始地址
	LCD1602_write_data('0'+datas[1]); //十位
	

	LCD1602_write_com(0x80+9);		//写地址 80表示初始地址
	LCD1602_write_data('0'+datas[2]); //个位 
	

	LCD1602_write_com(0x80+10);		//写地址 80表示初始地址
	LCD1602_write_data('.'); 		//显示 ‘.’
	

	LCD1602_write_com(0x80+11);		 //写地址 80表示初始地址
	LCD1602_write_data('0'+datas[3]); //显示小数点  
	

	LCD1602_write_com(0x80+12);		 //写地址 80表示初始地址
	LCD1602_write_data('0'+datas[4]); //显示小数点 

	 
}



//****************************************************
//主函数
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
	Delay_ms(1000);		 //延时,等待传感器稳定

	Get_Maopi();				//称毛皮重量
	
	while(1)
	{
		LED3=1;
		PE=1;
		if(RI == 1)
		{
			a=SBUF;//获取接收数据
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
			
		Get_Weight();			//称重

		Scan_Key();

		//显示当前重量
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
		if((e*10+i)<21&&(e*10+i)>6) //早上6点至晚上九点工作
		{
			if(time_1>2)   //3分钟提醒一次
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

//扫描按键
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
//称重
//****************************************************
void Get_Weight()
{
	Weight_Shiwu = HX711_Read();
	Weight_Shiwu = Weight_Shiwu - Weight_Maopi;		//获取净重
	if(Weight_Shiwu > 0)			
	{	
		Weight_Shiwu = (unsigned int)((float)Weight_Shiwu/GapValue); 	//计算实物的实际重量
																		
																		
		if(Weight_Shiwu > 5000)		//超重报警
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
	//	Flag_ERROR = 1;				//负重报警
	}
	
}

//****************************************************
//获取毛皮重量
//****************************************************
void Get_Maopi()
{
	Weight_Maopi = HX711_Read();	
} 

//****************************************************
//MS延时函数(12M晶振下测试)
//****************************************************
void Delay_ms(unsigned int n)
{
	unsigned int  i,j;
	for(i=0;i<n;i++)
		for(j=0;j<123;j++);
}