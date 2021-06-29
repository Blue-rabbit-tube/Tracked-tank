#include "STC8.H"
#include "intrins.h"

unsigned char tmp2;
int pwmleft=0;
//int pwmright=0;
int mode=0;
/**********************************************************
*定时中断
***********************************************************/
void clock()
{
TMOD=0x00;
TH0=0x97;
TL0=0xbb;
TR0=1;
ET0=1;
EA=1;
}
/**********************************************************
*io配置
***********************************************************/
void io()
{
P3M0=0x00;
P3M1=0x00;
}

/**********************************************************
*串口部分处理
***********************************************************/

#define FOSC 27000000L			  //MUC主频为27mhz下
#define BAUD 9600				  //串口2产生的波特率为9600
#define TM  (65536-(FOSC/4/BAUD)) //计算初值

//串口2
//串口2初始化操作 


void uart2_init() 
 {

 S2CON = 0x50;       //REN=1允许串行接受状态，串口工作模式2     	                 
  T2L=TM;			  //配置串口2波特率	 初始值
  T2H=TM>>8;                    
  AUXR=0X14;		  //开启定时器T2  并且开启1T模式
  IE2=0X01;           //串口2中断使能                                                           
  EA   = 1;           // 开总中断 
}

void uart2_send()
{
   	ES= 0;

   	   
  //发送数据
  S2BUF=0x33;						//发送0x0a
  while(S2CON&S2TI==0);				// 等特数据传送	(S2TI发送中断标志)
  S2CON&=~S2TI;						//清标志位
	   
     

    }   


void uart2_receive() interrupt 8 using 2
{
  if(S2CON&S2RI)						//如果收到数据 S2RI置位
  {
   tmp2= S2BUF;	
   S2CON&=~S2RI;						//清标志位  
  }
}


/*主函数入口*/
void main()
  { 
		unsigned char dongzuo;
		int finish;
		uart2_init();             //串口2初始化
		finish=0;
		clock();                  //定时器初始化
		io();                     //io初始化
		while(1)
		{
		dongzuo=tmp2;
	if(dongzuo=='a')  //up
	{
		mode=1;
	//	P32=1,P33=0;
		P34=1,P35=0;
		finish=1;
		}
 else if (dongzuo=='b')   //back
	{
		mode=2;
		P32=0,P33=1;
		//P34=0,P35=1;
		finish=1;
	}
 else if (dongzuo=='d')  //right
	{ 
		mode=4;
	//	P32=1,P33=0;
//	P34=0,P35=1;
		finish=1;
	}
 else if (dongzuo=='c')   //left
	{
		mode=3;
	//	P32=0,P33=1;
		//P34=1,P35=0;
		finish=1;
		}
	
	else if (dongzuo=='e')    //stop
	{
		mode=0;
		P32=0,P33=0;
		P34=0,P35=0;
		finish=1;
		}
	
	if(finish==1)
	{
	 tmp2=0x00;
		}
	 finish=0;
}
}

void pwm() interrupt 1
{
	
	if (mode==1)
	{
		pwmleft++;
	//	pwmright++;
	  if (pwmleft<=40)        //调参警告
    P32=1,P33=0;
		if (pwmleft>40&&pwmleft<=45)
			P32=0,P33=0;
			if (pwmleft>45)
				pwmleft=0;
	}
	if (mode==2)              //调参警告
	{
		pwmleft++;
		 if (pwmleft<=40)       //调参警告
	P34=0,P35=1;
		if (pwmleft>40&&pwmleft<=45)
		P34=0,P35=0;
			if (pwmleft>45)
				pwmleft=0;
	}
	if(mode==3)
	{
		pwmleft++;
		 if (pwmleft<=5)       //调参警告
		 {P32=0,P33=1;
		P34=1,P35=0;}
		if (pwmleft>5&&pwmleft<=7)
		{P32=0,P33=0;
	    P34=0,P35=0;}
			if (pwmleft>7)
				pwmleft=0;
	}
	if(mode==4)
	{
		pwmleft++;
		 if (pwmleft<=5)       //调参警告
		 {P32=1,P33=0;
	    P34=0,P35=1;}
		if (pwmleft>5&&pwmleft<=7)
		{P32=0,P33=0;
		  P34=0,P35=0;}
			if (pwmleft>7)
				pwmleft=0;
	}
		if(mode==0)
		{
      pwmleft=0;
    //  pwmright=0;
		}

}


//简易版，稳定可用