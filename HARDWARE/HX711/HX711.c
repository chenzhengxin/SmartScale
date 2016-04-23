#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "delay.h"
#include "HX711.h"

void AD_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;        //����GPIO��ʼ���ṹ��

  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;         //ѡ��PB.11(TXD)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;         //��������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO���ٶ�Ϊ50MHz
    GPIO_Init(GPIOA,&GPIO_InitStructure);                                 //��ʼ��PB5

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //�ܽ�Ƶ��Ϊ50MHZ
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;          //������� 
    GPIO_Init(GPIOA,&GPIO_InitStructure);
}


unsigned long Read_HX711(void)  //��HX711оƬ��������ݡ� 
{ 
	unsigned long val = 0; 
	unsigned char i = 0; 

	GPIO_SetBits(GPIOA,GPIO_Pin_14);    //DOUT=1 
	GPIO_ResetBits(GPIOA,GPIO_Pin_15);    //SCK=0 
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_14));   //�ȴ�DOUT=0  
	delay_us(1); 
	for(i=0;i<24;i++) 
	{ 
		GPIO_SetBits(GPIOA,GPIO_Pin_15);    //SCK=1 
		val=val<<1; 
		delay_us(1);  
		GPIO_ResetBits(GPIOA,GPIO_Pin_15);    //SCK=0 
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_14))   //DOUT=1 
		val++; 
		delay_us(1); 
	} 
	GPIO_SetBits(GPIOA,GPIO_Pin_15); 
	delay_us(1);
	val=val^0x800000; 
	GPIO_ResetBits(GPIOA,GPIO_Pin_15); 
	delay_us(1);  
	return val;  
}

float Read_Weight_Val(float AD_show_pi)
{
	unsigned long AD_data = 0;
	float AD_show = 0.0;

	AD_data = Read_HX711();
	AD_show = (float)AD_data * (5.0 * 1000 / 16777216);
	AD_show = AD_show / 128;
	if(AD_show_pi < AD_show){
	 	AD_show = AD_show - AD_show_pi;
		AD_show = AD_show * (16.3063) + 0.05; //AD_show * ((140 / 8.7) * 1.0133) + 0.05;
	}else{
		AD_show = 0.0;
	}

    return AD_show; 
}

float Weight_QuPi(void)
{
	u8 i = 5;
	unsigned long AD_data = 0;
	float AD_show_pi = 0.0;

	while(i > 0){
		AD_data = Read_HX711();
		AD_show_pi = (float)AD_data * (5.0 * 1000 / 16777216);
		AD_show_pi = AD_show_pi / 128;
		i--;
	}

	return AD_show_pi;
}
