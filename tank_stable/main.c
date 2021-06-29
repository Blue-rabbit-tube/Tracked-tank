#include "STC8.H"
#include "intrins.h"

unsigned char tmp2;
int pwmleft=0;
//int pwmright=0;
int mode=0;
/**********************************************************
*��ʱ�ж�
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
*io����
***********************************************************/
void io()
{
P3M0=0x00;
P3M1=0x00;
}

/**********************************************************
*���ڲ��ִ���
***********************************************************/

#define FOSC 27000000L			  //MUC��ƵΪ27mhz��
#define BAUD 9600				  //����2�����Ĳ�����Ϊ9600
#define TM  (65536-(FOSC/4/BAUD)) //�����ֵ

//����2
//����2��ʼ������ 


void uart2_init() 
 {

 S2CON = 0x50;       //REN=1�����н���״̬�����ڹ���ģʽ2     	                 
  T2L=TM;			  //���ô���2������	 ��ʼֵ
  T2H=TM>>8;                    
  AUXR=0X14;		  //������ʱ��T2  ���ҿ���1Tģʽ
  IE2=0X01;           //����2�ж�ʹ��                                                           
  EA   = 1;           // �����ж� 
}

void uart2_send()
{
   	ES= 0;

   	   
  //��������
  S2BUF=0x33;						//����0x0a
  while(S2CON&S2TI==0);				// �������ݴ���	(S2TI�����жϱ�־)
  S2CON&=~S2TI;						//���־λ
	   
     

    }   


void uart2_receive() interrupt 8 using 2
{
  if(S2CON&S2RI)						//����յ����� S2RI��λ
  {
   tmp2= S2BUF;	
   S2CON&=~S2RI;						//���־λ  
  }
}


/*���������*/
void main()
  { 
		unsigned char dongzuo;
		int finish;
		uart2_init();             //����2��ʼ��
		finish=0;
		clock();                  //��ʱ����ʼ��
		io();                     //io��ʼ��
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
	  if (pwmleft<=40)        //���ξ���
    P32=1,P33=0;
		if (pwmleft>40&&pwmleft<=45)
			P32=0,P33=0;
			if (pwmleft>45)
				pwmleft=0;
	}
	if (mode==2)              //���ξ���
	{
		pwmleft++;
		 if (pwmleft<=40)       //���ξ���
	P34=0,P35=1;
		if (pwmleft>40&&pwmleft<=45)
		P34=0,P35=0;
			if (pwmleft>45)
				pwmleft=0;
	}
	if(mode==3)
	{
		pwmleft++;
		 if (pwmleft<=5)       //���ξ���
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
		 if (pwmleft<=5)       //���ξ���
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


//���װ棬�ȶ�����