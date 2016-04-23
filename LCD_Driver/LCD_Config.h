#ifndef __LCD_CONFIG_H__
#define __LCD_CONFIG_H__

typedef	unsigned short LCDBYTE;			//�ض����й�LCD��,Ҳ��LCD���е�����λ��
typedef unsigned short DOTBYTE;			//�ض����й�LCD��,Ҳ��LCD�ĵ�������
typedef signed short  DISWORD;			//�ض�����LCD_Dis.c�ļ���һЩ�м䴦������ݵĿ��

typedef const unsigned char FLASH1;	//�ض����������д�ŵ�CODE��������������
#ifndef NULL
#define NULL    ((void *)0)
#endif

//#ifndef FALSE
//#define FALSE   (0)
//#endif

//#ifndef TRUE
//#define TRUE    (1)
//#endif

typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long  DWORD;
typedef unsigned int   BOOL;

#define COLOR_Red		0xf800			//��
#define COLOR_Orange	0xfc00			//��
#define COLOR_Yellow	0xffe0			//��
#define COLOR_Green		0x07e0			//��
#define COLOR_Cyan		0x07ff			//��
#define COLOR_Blue		0x001f			//��
#define COLOR_Purple	0x8010			//��
#define COLOR_Black		0x0000			//��ɫ
#define COLOR_White		0xffff			//��ɫ


#endif
