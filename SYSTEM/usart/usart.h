#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.csom
//�޸�����:2011/6/14
//�汾��V1.4
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
////////////////////////////////////////////////////////////////////////////////// 	
//#define USART_REC_LEN  			200  	//�����������ֽ��� 200
//#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
//extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
//extern u16 USART_RX_STA;         		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��
//void uart_init(u32 bound);

#define SEND_BUFFER_SIZE	128
extern void TimerDly(unsigned int Timer);

extern unsigned char Send_Buffer[SEND_BUFFER_SIZE];

extern unsigned short m_UARTFIFO_PackedDatNum;

extern void uart_printer_init(u32 bound);
extern void uart_lcd_init(u32 bound);
extern void UART_DMA_Tx_Enable(void);
extern void UART_DMA_Rx_Enable(void);
extern void UART_SendPacked(unsigned char Command,unsigned char * p,unsigned char num);
extern void UART_SendOneByte(unsigned char Dat);
extern void UART_SetBaudRate(unsigned int Br);

#define UART_RX_BUFFER_SIZE		512

extern unsigned char UserTestUARTFIFOCommand(void);
extern unsigned char UART_GetByte(void);
extern unsigned short UART_GetWord(void);
extern signed int UART_GetSInt(void);
extern void UART_GetFloat(float *n_siTemp);

#endif


