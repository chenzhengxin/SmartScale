//========================================================================
// 文件名: LCD_Dis.c
// 作  者: Xinqiang Zhang(email: Xinqiang@Mzdesign.com.cn)
//			www.Mzdesign.com.cn
// 日  期: 2013/02/06
// 描  述: EzUI系列模块接口层程序集 指令控制显示
//
// 参  考: 请参考具体显示器的硬件描述说明书或者是DataSheet,并以此来编写加速
//			版接口程序;本程序适用于STM32系列MCU，对应的显示模块为EzUI系列模块
// 版  本:
//      2013/02/06      First version    Mz Design		
//========================================================================
#include "LCD_Config.h"							//LCD的配置文件
#include "usart.h"

DOTBYTE X_Witch=10;							//存下当前所选择的ASCII英文字库的字符点数X轴宽度
DOTBYTE Y_Witch=20;							//存下当前所选择的ASCII英文字库的字符点数Y轴高度
DOTBYTE X_Witch_cn=28;						//存下当前所选择的中文字库的字符点数X轴宽度
DOTBYTE Y_Witch_cn=28;						//存下当前所选择的中文字库的字符点数Y轴高度

//========================================================================
// 函数: void FontSet(int Font_NUM,unsigned int Color)
// 描述: 英文字体设置  选择模块中固件所包含的英文字库
// 参数: Font_NUM 	字体选择,以模块固件的字库为准
//		 Color  	文本颜色,作用于汉字显示以及英文字符显示等字符显示的操作  
// 返回: 无
// 备注: 
// 版本:
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
			case 0:					//选择模块固件自带ASCII英文字库第0号
				X_Witch = 6;
				Y_Witch = 10;
				break;
			case 1:					//选择模块固件自带ASCII英文字库第1号
				X_Witch = 7;
				Y_Witch = 14;
				break;
			case 2:					//选择模块固件自带ASCII英文字库第2号
				X_Witch = 8;
				Y_Witch = 16;
				break;
			case 3:					//选择模块固件自带ASCII英文字库第3号
				X_Witch = 10;
				Y_Witch = 20;
				break;
			case 4:					//选择模块固件自带ASCII英文字库第4号
				X_Witch = 16;
				Y_Witch = 32;
				break;
			default:break;
		}
	}
}
//========================================================================
// 函数: void PutBitmap(LCDBYTE x,LCDBYTE y,unsigned short Index)  
// 描述: 显示一个模块资源中的位图在指定位置上
// 参数: x  X轴坐标     y  Y轴坐标
//		 Index  位图资源在资源当中的序列号 
// 返回: 无
// 备注: 
// 版本:
//      2010/03/04      First version
//		2011/10/07		for MzTH系列模块_3版本固件修改
//		2013/05/15		修改为EzUI系列模块接口函数
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
// 函数: void PutBitmap_Window(LCDBYTE x,LCDBYTE y,unsigned short x_w,unsigned short y_h,
//					unsigned short x_s,unsigned short y_s,unsigned short Index)  
// 描述: 在指定窗口显示一个模块资源中的位图的局部图像
// 参数: x  窗口左上角X轴坐标     y  窗口左上角Y轴坐标
//		 x_w 窗口宽度			  y_h 窗口高度
//		 x_s 要显示的图像的局部区域的左上角X轴起始位置(相对于图像左上角为原点,与屏幕以及显示位置无关)
//		 y_s 要显示的图像的局部区域的左上角Y轴起始位置(相对于图像左上角为原点,与屏幕以及显示位置无关)
//		 Index  位图资源在资源当中的序列号 
// 返回: 无
// 备注: 
// 版本:
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
// 函数: void PutChar_cn(unsigned char x,unsigned char y,unsigned short * GB) 
// 描述: 写入一个二级汉字库汉字
// 参数: x  X轴坐标     y  Y轴坐标
//		 a  GB码  
// 返回: 无
// 备注: 请注意MCU的大小端
// 版本:
//      2010/03/04      First version
//========================================================================
void PutChar_cn(LCDBYTE x,LCDBYTE y,unsigned short * GB) 
{
	Send_Buffer[0] = x>>8;
	Send_Buffer[1] = (unsigned char) x;
	Send_Buffer[2] = y>>8;
	Send_Buffer[3] = (unsigned char) y;
	Send_Buffer[4] = (unsigned char)(*GB&0x00ff);		//传送二级字库中汉字GB码的高八位值
	Send_Buffer[5] = (unsigned char)((*GB>>8)&0x00ff);	//传送二级字库中汉字GB码的低八位值
	UART_SendPacked(0x08,Send_Buffer,6);
}
//========================================================================
// 函数: void FontMode(BYTE Mode,LCDBYTE FontBackColor) 
// 描述: 设置字符显示的模式,并设置背景色
// 参数: Mode
//			高四位 0: 中英文混合显示时以中文字符为准靠上对齐
//				   1: 中英文混合显示时以中文字符为准居中对齐
//				   2: 中英文混合显示时以中文字符为准靠下对齐	
//			低四位 0: 字符显示时仅对字符的有效点进行显示操作,也就是不作背景覆盖
//					为叠加模式
//				   1: 覆盖模式
//		FontBackColor	设置覆盖模式时,字符的背景色 
// 返回: 无
// 备注: 仅对字符显示有效
// 版本:
//      2010/03/04      First version
//		2011/10/07		for MzTH系列模块_3版本固件修改
//		2013/05/15		修改为EzUI系列模块接口函数
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
// 函数: void PutChar(DOTBYTE x,DOTBYTE y,char a) 
// 描述: 写入一个标准字符
// 参数: x  X轴坐标     y  Y轴坐标
//		 a  要显示的字符在字库中的偏移量  
// 返回: 无
// 备注: ASCII字符可直接输入ASCII码即可
// 版本:
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
// 函数: void PutString(DOTBYTE x,DOTBYTE y,char *p)
// 描述: 在x、y为起始坐标处写入一串标准字符
// 参数: x  X轴坐标     y  Y轴坐标
//		 p  要显示的字符串  
// 返回: 无
// 备注: 仅能用于自带的ASCII字符串显示
// 版本:
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
// 函数: void SetPaintMode(BYTE LineWith,LCDBYTE Color)
// 描述: 绘图模式设置
// 参数: LineWith 绘图线宽设置    Color  像素点的颜色,相当于前景色  
// 返回: 无
// 备注: Mode无效
// 版本:
//      2010/03/04      First version
//		2011/09/05		for MzTH系列模块_3版本固件修改
//		2013/05/15		修改为EzUI系列模块接口函数
//========================================================================
void SetPaintMode(BYTE LineWith,LCDBYTE Color)
{
	Send_Buffer[0] = LineWith;
	Send_Buffer[1] = Color>>8;
	Send_Buffer[2] = (unsigned char) Color;
	UART_SendPacked(0x84,Send_Buffer,3);
}
//========================================================================
// 函数: void PutPixel(DOTBYTE x,DOTBYTE y)
// 描述: 在x、y点上绘制一个前景色的点
// 参数: x  X轴坐标     y  Y轴坐标
// 返回: 无
// 备注: 使用前景色
// 版本:
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
// 函数: void Line(LCDBYTE s_x,LCDBYTE s_y,LCDBYTE e_x,LCDBYTE e_y)
// 描述: 在s_x、s_y为起始坐标，e_x、e_y为结束坐标绘制一条直线
// 参数: x  X轴坐标     y  Y轴坐标
// 返回: 无
// 备注: 使用前景色
// 版本:
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
// 函数: void Circle(DOTBYTE x,DOTBYTE y,DOTBYTE r,BYTE Mode)
// 描述: 以x,y为圆心R为半径画一个圆(mode = 0) or 圆面(mode = 1)
// 参数: 
// 返回: 无
// 备注: 画圆函数执行较慢，如果MCU有看门狗，请作好清狗的操作
// 版本:
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
// 函数: void Rectangle(DOTBYTE left, DOTBYTE top, DOTBYTE right, 
//						DOTBYTE bottom, BYTE Mode)
// 描述: 以x,y为圆心R为半径画一个圆(mode = 0) or 圆面(mode = 1)
// 参数: left - 矩形的左上角横坐标，范围0到118
//		 top - 矩形的左上角纵坐标，范围0到50
//		 right - 矩形的右下角横坐标，范围1到119
//		 bottom - 矩形的右下角纵坐标，范围1到51
//		 Mode - 绘制模式，可以是下列数值之一：
//				0:	矩形框（空心矩形）
//				1:	矩形面（实心矩形）
// 返回: 无
// 备注: 
// 版本:
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
// 函数: void NumberDis(LCDBYTE x,LCDBYTE y,signed int Num,unsigned char len,unsigned char type)  
// 描述: 直接数字显示
// 参数: x	要显示的数字的左上角x轴起始位置
//		 y	要显示的数字的左上角y轴起始位置
//		 Num	要显示的数字
//		 len	要显示的数字的十进制位的个数
//		 type	显示模式
//			0: 全显示
//			1：靠左显示
//			2：靠右显示
// 返回: 无
// 备注: 使用当前设置的英文字体，并受字符覆盖模式控制
// 版本:
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
// 函数: void ClrScreen(BYTE Mode)
// 描述: 清屏函数，执行全屏幕清除或填充前景色
// 参数: Mode   0:全屏除屏
//				1:全屏填充前景色
// 返回: 无
// 备注: 
// 版本:
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
// 函数: void BackLightSet(unsigned short Gra)
// 描述: 设置当前背光亮度等级
// 参数: Gra   0~500
// 返回: 无
// 备注: 
// 版本:
//      2010/03/04      First version
//========================================================================
void BackLightSet(unsigned short Gra)
{
	Send_Buffer[0] = Gra>>8;
	Send_Buffer[1] = (unsigned char) Gra;
	UART_SendPacked(0x8A,Send_Buffer,2);	
}
//========================================================================
// 函数: void LCDSetWaitMode(void)
// 描述: 设置模块进入等待模式,即睡眠模式第一等级,可触摸唤醒
// 参数: 
// 返回: 无
// 备注: 
// 版本:
//      2010/03/04      First version
//========================================================================
void LCDSetWaitMode(void)
{
	Send_Buffer[0] = 0xa5;
	UART_SendPacked(0x8c,Send_Buffer,1);	
}
//========================================================================
// 函数: void LCDSetSleep(void)
// 描述: 设置模块进入省电模式
// 参数: 
// 返回: 无
// 备注: 
// 版本:
//      2010/03/04      First version
//========================================================================
void LCDSetSleep(void)
{
	Send_Buffer[0] = 0x5a;
	UART_SendPacked(0x8c,Send_Buffer,1);	
}
//========================================================================
// 函数: void LCDWakeup(void)
// 描述: 将模块从省电模式中唤醒
// 参数: 无
// 返回: 无
// 备注: 模块被唤醒后，最好等10ms左右再对模块进行操作
// 版本:
//      2010/03/04      First version
//========================================================================
void LCDWakeup(void)
{
	Send_Buffer[0] = 0xaa;
	UART_SendPacked(0x8c,Send_Buffer,1);
}

//扩展应用功能
//读取模块主芯片ID号
//在此的超时设置只是临时使用的一个延时时间不确定的超时，具体用户在使用时，请具体根据自已的需要进行调整
//返回0表示正常读回数据，返回1表示超时
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





