//========================================================================
// �ļ���: LCD_Dis.c
// ��  ��: Xinqiang Zhang(email: Xinqiang@Mzdesign.com.cn)
//			www.Mzdesign.com.cn
// ��  ��: 2013/02/06
// ��  ��: EzUIϵ��ģ��ӿڲ���� ָ�������ʾ
//
// ��  ��: ��ο�������ʾ����Ӳ������˵���������DataSheet,���Դ�����д����
//			��ӿڳ���;������������STM32ϵ��MCU����Ӧ����ʾģ��ΪEzUIϵ��ģ��
// ��  ��:
//      2013/02/06      First version    Mz Design		
//========================================================================
#include "LCD_Config.h"							//LCD�������ļ�
#include "usart.h"

DOTBYTE X_Witch=10;							//���µ�ǰ��ѡ���ASCIIӢ���ֿ���ַ�����X����
DOTBYTE Y_Witch=20;							//���µ�ǰ��ѡ���ASCIIӢ���ֿ���ַ�����Y��߶�
DOTBYTE X_Witch_cn=28;						//���µ�ǰ��ѡ��������ֿ���ַ�����X����
DOTBYTE Y_Witch_cn=28;						//���µ�ǰ��ѡ��������ֿ���ַ�����Y��߶�

//========================================================================
// ����: void FontSet(int Font_NUM,unsigned int Color)
// ����: Ӣ����������  ѡ��ģ���й̼���������Ӣ���ֿ�
// ����: Font_NUM 	����ѡ��,��ģ��̼����ֿ�Ϊ׼
//		 Color  	�ı���ɫ,�����ں�����ʾ�Լ�Ӣ���ַ���ʾ���ַ���ʾ�Ĳ���  
// ����: ��
// ��ע: 
// �汾:
//      2010/03/04      First version
//========================================================================
void FontSet(LCDBYTE Font_NUM,LCDBYTE Color)
{
	Send_Buffer[0] = (unsigned char)(Font_NUM>>8);
	Send_Buffer[1] = (unsigned char) Font_NUM;
	Send_Buffer[2] = (unsigned char)(Color>>8);
	Send_Buffer[3] = (unsigned char)Color;
	UART_SendPacked(0x81,Send_Buffer,4);
	if(Font_NUM&0x8000)
	{
		Font_NUM = Font_NUM&0x000f;
		switch(Font_NUM)
		{
			case 0:					//ѡ��ģ��̼��Դ�ASCIIӢ���ֿ��0��
				X_Witch = 6;
				Y_Witch = 10;
				break;
			case 1:					//ѡ��ģ��̼��Դ�ASCIIӢ���ֿ��1��
				X_Witch = 7;
				Y_Witch = 14;
				break;
			case 2:					//ѡ��ģ��̼��Դ�ASCIIӢ���ֿ��2��
				X_Witch = 8;
				Y_Witch = 16;
				break;
			case 3:					//ѡ��ģ��̼��Դ�ASCIIӢ���ֿ��3��
				X_Witch = 10;
				Y_Witch = 20;
				break;
			case 4:					//ѡ��ģ��̼��Դ�ASCIIӢ���ֿ��4��
				X_Witch = 16;
				Y_Witch = 32;
				break;
			default:break;
		}
	}
}
//========================================================================
// ����: void PutBitmap(LCDBYTE x,LCDBYTE y,unsigned short Index)  
// ����: ��ʾһ��ģ����Դ�е�λͼ��ָ��λ����
// ����: x  X������     y  Y������
//		 Index  λͼ��Դ����Դ���е����к� 
// ����: ��
// ��ע: 
// �汾:
//      2010/03/04      First version
//		2011/10/07		for MzTHϵ��ģ��_3�汾�̼��޸�
//		2013/05/15		�޸�ΪEzUIϵ��ģ��ӿں���
//========================================================================
void PutBitmap(LCDBYTE x,LCDBYTE y,unsigned short Index) 
{
	Send_Buffer[0] = x>>8;
	Send_Buffer[1] = (unsigned char) x;
	Send_Buffer[2] = y>>8;
	Send_Buffer[3] = (unsigned char) y;
	Send_Buffer[4] = Index>>8;
	Send_Buffer[5] = (unsigned char) Index;
	UART_SendPacked(0x09,Send_Buffer,6);
}
//========================================================================
// ����: void PutBitmap_Window(LCDBYTE x,LCDBYTE y,unsigned short x_w,unsigned short y_h,
//					unsigned short x_s,unsigned short y_s,unsigned short Index)  
// ����: ��ָ��������ʾһ��ģ����Դ�е�λͼ�ľֲ�ͼ��
// ����: x  �������Ͻ�X������     y  �������Ͻ�Y������
//		 x_w ���ڿ��			  y_h ���ڸ߶�
//		 x_s Ҫ��ʾ��ͼ��ľֲ���������Ͻ�X����ʼλ��(�����ͼ�����Ͻ�Ϊԭ��,����Ļ�Լ���ʾλ���޹�)
//		 y_s Ҫ��ʾ��ͼ��ľֲ���������Ͻ�Y����ʼλ��(�����ͼ�����Ͻ�Ϊԭ��,����Ļ�Լ���ʾλ���޹�)
//		 Index  λͼ��Դ����Դ���е����к� 
// ����: ��
// ��ע: 
// �汾:
//		2011/10/07		First version
//========================================================================
void PutBitmap_Window(LCDBYTE x,LCDBYTE y,unsigned short x_w,unsigned short y_h,
					unsigned short x_s,unsigned short y_s,unsigned short Index) 
{
	Send_Buffer[0] = x>>8;
	Send_Buffer[1] = (unsigned char) x;
	Send_Buffer[2] = y>>8;
	Send_Buffer[3] = (unsigned char) y;
	Send_Buffer[4] = x_w>>8;
	Send_Buffer[5] = (unsigned char) x_w;
	Send_Buffer[6] = y_h>>8;
	Send_Buffer[7] = (unsigned char) y_h;
	Send_Buffer[8] = x_s>>8;
	Send_Buffer[9] = (unsigned char) x_s;
	Send_Buffer[10] = y_s>>8;
	Send_Buffer[11] = (unsigned char) y_s;
	Send_Buffer[12] = Index>>8;
	Send_Buffer[13] = (unsigned char) Index;
	UART_SendPacked(0x0d,Send_Buffer,14);
}
//========================================================================
// ����: void PutChar_cn(unsigned char x,unsigned char y,unsigned short * GB) 
// ����: д��һ���������ֿ⺺��
// ����: x  X������     y  Y������
//		 a  GB��  
// ����: ��
// ��ע: ��ע��MCU�Ĵ�С��
// �汾:
//      2010/03/04      First version
//========================================================================
void PutChar_cn(LCDBYTE x,LCDBYTE y,unsigned short * GB) 
{
	Send_Buffer[0] = x>>8;
	Send_Buffer[1] = (unsigned char) x;
	Send_Buffer[2] = y>>8;
	Send_Buffer[3] = (unsigned char) y;
	Send_Buffer[4] = (unsigned char)(*GB&0x00ff);		//���Ͷ����ֿ��к���GB��ĸ߰�λֵ
	Send_Buffer[5] = (unsigned char)((*GB>>8)&0x00ff);	//���Ͷ����ֿ��к���GB��ĵͰ�λֵ
	UART_SendPacked(0x08,Send_Buffer,6);
}
//========================================================================
// ����: void FontMode(BYTE Mode,LCDBYTE FontBackColor) 
// ����: �����ַ���ʾ��ģʽ,�����ñ���ɫ
// ����: Mode
//			����λ 0: ��Ӣ�Ļ����ʾʱ�������ַ�Ϊ׼���϶���
//				   1: ��Ӣ�Ļ����ʾʱ�������ַ�Ϊ׼���ж���
//				   2: ��Ӣ�Ļ����ʾʱ�������ַ�Ϊ׼���¶���	
//			����λ 0: �ַ���ʾʱ�����ַ�����Ч�������ʾ����,Ҳ���ǲ�����������
//					Ϊ����ģʽ
//				   1: ����ģʽ
//		FontBackColor	���ø���ģʽʱ,�ַ��ı���ɫ 
// ����: ��
// ��ע: �����ַ���ʾ��Ч
// �汾:
//      2010/03/04      First version
//		2011/10/07		for MzTHϵ��ģ��_3�汾�̼��޸�
//		2013/05/15		�޸�ΪEzUIϵ��ģ��ӿں���
//========================================================================
void FontMode(BYTE Mode,LCDBYTE FontBackColor)
{
 	Send_Buffer[0] = Mode;//>>8;
//	Send_Buffer[1] = (unsigned char) Font_NUM;
	Send_Buffer[1] = FontBackColor>>8;
	Send_Buffer[2] = (unsigned char) FontBackColor;
	UART_SendPacked(0x85,Send_Buffer,3);
}
//========================================================================
// ����: void PutChar(DOTBYTE x,DOTBYTE y,char a) 
// ����: д��һ����׼�ַ�
// ����: x  X������     y  Y������
//		 a  Ҫ��ʾ���ַ����ֿ��е�ƫ����  
// ����: ��
// ��ע: ASCII�ַ���ֱ������ASCII�뼴��
// �汾:
//      2010/03/04      First version
//========================================================================
void PutChar(DOTBYTE x,DOTBYTE y,char a)       
{
 	Send_Buffer[0] = x>>8;
	Send_Buffer[1] = (unsigned char) x;
	Send_Buffer[2] = y>>8;
	Send_Buffer[3] = (unsigned char) y;
	Send_Buffer[4] = (unsigned char) a;
	UART_SendPacked(0x07,Send_Buffer,5);
}

//========================================================================
// ����: void PutString(DOTBYTE x,DOTBYTE y,char *p)
// ����: ��x��yΪ��ʼ���괦д��һ����׼�ַ�
// ����: x  X������     y  Y������
//		 p  Ҫ��ʾ���ַ���  
// ����: ��
// ��ע: ���������Դ���ASCII�ַ�����ʾ
// �汾:
//      2010/03/04      First version
//========================================================================
void PutString(DOTBYTE x,DOTBYTE y,char *p)
{
	unsigned char Index;
	Send_Buffer[0] = x>>8;
	Send_Buffer[1] = (unsigned char) x;
	Send_Buffer[2] = y>>8;
	Send_Buffer[3] = (unsigned char) y;
	Index = 4;
	while(*p&&Index<68)
	{
		Send_Buffer[Index++] = *p++;
	}
	UART_SendPacked(0x0b,Send_Buffer,Index);
}
//========================================================================
// ����: void SetPaintMode(BYTE LineWith,LCDBYTE Color)
// ����: ��ͼģʽ����
// ����: LineWith ��ͼ�߿�����    Color  ���ص����ɫ,�൱��ǰ��ɫ  
// ����: ��
// ��ע: Mode��Ч
// �汾:
//      2010/03/04      First version
//		2011/09/05		for MzTHϵ��ģ��_3�汾�̼��޸�
//		2013/05/15		�޸�ΪEzUIϵ��ģ��ӿں���
//========================================================================
void SetPaintMode(BYTE LineWith,LCDBYTE Color)
{
	Send_Buffer[0] = LineWith;
	Send_Buffer[1] = Color>>8;
	Send_Buffer[2] = (unsigned char) Color;
	UART_SendPacked(0x84,Send_Buffer,3);
}
//========================================================================
// ����: void PutPixel(DOTBYTE x,DOTBYTE y)
// ����: ��x��y���ϻ���һ��ǰ��ɫ�ĵ�
// ����: x  X������     y  Y������
// ����: ��
// ��ע: ʹ��ǰ��ɫ
// �汾:
//      2010/03/04      First version
//========================================================================
void PutPixel(DOTBYTE x,DOTBYTE y)
{
	Send_Buffer[0] = x>>8;
	Send_Buffer[1] = (unsigned char) x;
	Send_Buffer[2] = y>>8;
	Send_Buffer[3] = (unsigned char) y;
	UART_SendPacked(0x01,Send_Buffer,4); 	
}
//========================================================================
// ����: void Line(LCDBYTE s_x,LCDBYTE s_y,LCDBYTE e_x,LCDBYTE e_y)
// ����: ��s_x��s_yΪ��ʼ���꣬e_x��e_yΪ�����������һ��ֱ��
// ����: x  X������     y  Y������
// ����: ��
// ��ע: ʹ��ǰ��ɫ
// �汾:
//      2010/03/04      First version
//========================================================================
void Line(LCDBYTE s_x,LCDBYTE s_y,LCDBYTE e_x,LCDBYTE e_y)
{  
	Send_Buffer[0] = s_x>>8;
	Send_Buffer[1] = (unsigned char) s_x;
	Send_Buffer[2] = s_y>>8;
	Send_Buffer[3] = (unsigned char) s_y;
	Send_Buffer[4] = e_x>>8;
	Send_Buffer[5] = (unsigned char) e_x;
	Send_Buffer[6] = e_y>>8;
	Send_Buffer[7] = (unsigned char) e_y;
	UART_SendPacked(0x02,Send_Buffer,8);   
}
//========================================================================
// ����: void Circle(DOTBYTE x,DOTBYTE y,DOTBYTE r,BYTE Mode)
// ����: ��x,yΪԲ��RΪ�뾶��һ��Բ(mode = 0) or Բ��(mode = 1)
// ����: 
// ����: ��
// ��ע: ��Բ����ִ�н��������MCU�п��Ź����������幷�Ĳ���
// �汾:
//      2010/03/04      First version
//========================================================================
void Circle(DOTBYTE x,DOTBYTE y,DOTBYTE r,BYTE Mode)
{
	Send_Buffer[0] = x>>8;
	Send_Buffer[1] = (unsigned char) x;
	Send_Buffer[2] = y>>8;
	Send_Buffer[3] = (unsigned char) y;
	Send_Buffer[4] = r>>8;
	Send_Buffer[5] = (unsigned char) r;
	if(Mode==0)
		UART_SendPacked(0x05,Send_Buffer,6);
	else
		UART_SendPacked(0x06,Send_Buffer,6);
}	

//========================================================================
// ����: void Rectangle(DOTBYTE left, DOTBYTE top, DOTBYTE right, 
//						DOTBYTE bottom, BYTE Mode)
// ����: ��x,yΪԲ��RΪ�뾶��һ��Բ(mode = 0) or Բ��(mode = 1)
// ����: left - ���ε����ϽǺ����꣬��Χ0��118
//		 top - ���ε����Ͻ������꣬��Χ0��50
//		 right - ���ε����½Ǻ����꣬��Χ1��119
//		 bottom - ���ε����½������꣬��Χ1��51
//		 Mode - ����ģʽ��������������ֵ֮һ��
//				0:	���ο򣨿��ľ��Σ�
//				1:	�����棨ʵ�ľ��Σ�
// ����: ��
// ��ע: 
// �汾:
//      2010/03/04      First version
//========================================================================
void Rectangle(DOTBYTE left, DOTBYTE top, DOTBYTE right, DOTBYTE bottom, BYTE Mode)
{
	Send_Buffer[0] = left>>8;
	Send_Buffer[1] = (unsigned char) left;
	Send_Buffer[2] = top>>8;
	Send_Buffer[3] = (unsigned char) top;
	Send_Buffer[4] = right>>8;
	Send_Buffer[5] = (unsigned char) right;
	Send_Buffer[6] = bottom>>8;
	Send_Buffer[7] = (unsigned char) bottom;
	if(Mode==0)
		UART_SendPacked(0x03,Send_Buffer,8);
	else
		UART_SendPacked(0x04,Send_Buffer,8);
}
//========================================================================
// ����: void NumberDis(LCDBYTE x,LCDBYTE y,signed int Num,unsigned char len,unsigned char type)  
// ����: ֱ��������ʾ
// ����: x	Ҫ��ʾ�����ֵ����Ͻ�x����ʼλ��
//		 y	Ҫ��ʾ�����ֵ����Ͻ�y����ʼλ��
//		 Num	Ҫ��ʾ������
//		 len	Ҫ��ʾ�����ֵ�ʮ����λ�ĸ���
//		 type	��ʾģʽ
//			0: ȫ��ʾ
//			1��������ʾ
//			2��������ʾ
// ����: ��
// ��ע: ʹ�õ�ǰ���õ�Ӣ�����壬�����ַ�����ģʽ����
// �汾:
//      2010/03/04      First version
//========================================================================
void NumberDis(LCDBYTE x,LCDBYTE y,signed int Num,unsigned char len,unsigned char type) 
{
	unsigned char *temp;
	Send_Buffer[0] = x>>8;
	Send_Buffer[1] = (unsigned char) x;
	Send_Buffer[2] = y>>8;
	Send_Buffer[3] = (unsigned char) y;
	temp = (unsigned char*)&Num;
	Send_Buffer[7] = *temp++;
	Send_Buffer[6] = *temp++;
	Send_Buffer[5] = *temp++;
	Send_Buffer[4] = *temp;
	Send_Buffer[8] = len;
	Send_Buffer[9] = type;

	UART_SendPacked(0x0a,Send_Buffer,10);
}
//========================================================================
// ����: void ClrScreen(BYTE Mode)
// ����: ����������ִ��ȫ��Ļ��������ǰ��ɫ
// ����: Mode   0:ȫ������
//				1:ȫ�����ǰ��ɫ
// ����: ��
// ��ע: 
// �汾:
//      2010/03/04      First version
//========================================================================
void ClrScreen(BYTE Mode)
{
	if(Mode==0)
		Send_Buffer[0] = 0xaa;
	else Send_Buffer[0] = 0x55;
	UART_SendPacked(0x80,Send_Buffer,1);
}

//========================================================================
// ����: void BackLightSet(unsigned short Gra)
// ����: ���õ�ǰ�������ȵȼ�
// ����: Gra   0~500
// ����: ��
// ��ע: 
// �汾:
//      2010/03/04      First version
//========================================================================
void BackLightSet(unsigned short Gra)
{
	Send_Buffer[0] = Gra>>8;
	Send_Buffer[1] = (unsigned char) Gra;
	UART_SendPacked(0x8A,Send_Buffer,2);	
}
//========================================================================
// ����: void LCDSetWaitMode(void)
// ����: ����ģ�����ȴ�ģʽ,��˯��ģʽ��һ�ȼ�,�ɴ�������
// ����: 
// ����: ��
// ��ע: 
// �汾:
//      2010/03/04      First version
//========================================================================
void LCDSetWaitMode(void)
{
	Send_Buffer[0] = 0xa5;
	UART_SendPacked(0x8c,Send_Buffer,1);	
}
//========================================================================
// ����: void LCDSetSleep(void)
// ����: ����ģ�����ʡ��ģʽ
// ����: 
// ����: ��
// ��ע: 
// �汾:
//      2010/03/04      First version
//========================================================================
void LCDSetSleep(void)
{
	Send_Buffer[0] = 0x5a;
	UART_SendPacked(0x8c,Send_Buffer,1);	
}
//========================================================================
// ����: void LCDWakeup(void)
// ����: ��ģ���ʡ��ģʽ�л���
// ����: ��
// ����: ��
// ��ע: ģ�鱻���Ѻ���õ�10ms�����ٶ�ģ����в���
// �汾:
//      2010/03/04      First version
//========================================================================
void LCDWakeup(void)
{
	Send_Buffer[0] = 0xaa;
	UART_SendPacked(0x8c,Send_Buffer,1);
}

//��չӦ�ù���
//��ȡģ����оƬID��
//�ڴ˵ĳ�ʱ����ֻ����ʱʹ�õ�һ����ʱʱ�䲻ȷ���ĳ�ʱ�������û���ʹ��ʱ�������������ѵ���Ҫ���е���
//����0��ʾ�����������ݣ�����1��ʾ��ʱ
unsigned char ReadMzTHChipID(unsigned char Num,unsigned short TimerOut,unsigned char *Buf)
{
	Send_Buffer[0] = Num;
	UART_SendPacked(0x8D,Send_Buffer,1);
	while(TimerOut)
	{
		if(UserTestUARTFIFOCommand()==0x8D)
		{
			while(Num--)
			{
				*Buf = UART_GetByte();
				Buf++;
			}
			return 0;
		}
		TimerDly(100);
	}
	return 1;	
}





