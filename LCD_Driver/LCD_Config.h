#ifndef __LCD_CONFIG_H__
#define __LCD_CONFIG_H__

typedef	unsigned short LCDBYTE;			//重定义有关LCD的,也即LCD当中的数据位宽
typedef unsigned short DOTBYTE;			//重定义有关LCD的,也即LCD的点阵数量
typedef signed short  DISWORD;			//重定义在LCD_Dis.c文件中一些中间处理的数据的宽度

typedef const unsigned char FLASH1;	//重定义驱动当中存放到CODE的数据类型声明
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

#define COLOR_Red		0xf800			//红
#define COLOR_Orange	0xfc00			//橙
#define COLOR_Yellow	0xffe0			//黄
#define COLOR_Green		0x07e0			//绿
#define COLOR_Cyan		0x07ff			//青
#define COLOR_Blue		0x001f			//蓝
#define COLOR_Purple	0x8010			//紫
#define COLOR_Black		0x0000			//黑色
#define COLOR_White		0xffff			//白色


#endif
