#include "LCD_Config.h"
#include "hw_Setting.h"


//LCD 初始化函数
extern void LCD_Initial(void);	//如有必要该函数还会包含端口初始化、复位等操作
//画直线函数，s_x、s_y为起始点，e_x、e_y为结束点
extern void Line(LCDBYTE s_x,LCDBYTE s_y,LCDBYTE e_x,LCDBYTE e_y);
//绘图线宽、当前绘图颜色设置
extern void SetPaintMode(BYTE LineWith,LCDBYTE Color);
//选择英文(ASCII 字库),该函数只能选择模块内部的英文字库,具体请参考模块使用手册
extern void FontSet(DOTBYTE Font_NUM,LCDBYTE Color);
//选择英文(ASCII字库),该函数只能选择模块所加载的资源中的英文字库
extern void FontSet_rs(LCDBYTE Font_NUM,LCDBYTE Color);
//选择中文字库,该函数只能选择模块所加载的资源中的中文汉字字库
extern void FontSet_cn(LCDBYTE Font_NUM,LCDBYTE Color);
//显示一个汉字
extern void PutChar_cn(LCDBYTE x,LCDBYTE y,unsigned short * GB) ;
//显示一个汉字字符串
extern void PutString_cn(LCDBYTE x,LCDBYTE y,unsigned char *p);
//设置字符显示的模式,并设置背景色
extern void FontMode(BYTE Mode,LCDBYTE FontBackColor);
//于x、y的坐标上写入一个标准字符
extern void PutChar(DOTBYTE x,DOTBYTE y,char a);
//于x、y的坐标为起始写入一串标准字符串
extern void PutString(DOTBYTE x,DOTBYTE y,char *p);
//于x、y的坐标为中心，绘制一个点
extern void PutPixel(DOTBYTE x,DOTBYTE y);
//于x、y的坐标为中心，绘制一个圆边或实心圆
extern void Circle(DOTBYTE x,DOTBYTE y,DOTBYTE r,BYTE mode);
//绘制一个以left、top和right、bottom为两个对角的矩形框或者实心矩形
extern void Rectangle(DOTBYTE left, DOTBYTE top, DOTBYTE right, DOTBYTE bottom, BYTE Mode);
//直接数字显示
extern void NumberDis(LCDBYTE x,LCDBYTE y,signed int Num,unsigned char len,unsigned char type);
//清屏函数，执行全屏幕清除或填充前景色
extern void ClrScreen(BYTE Mode);
//背光亮度设置
extern void BackLightSet(unsigned short Gra);

extern void LCDSetWaitMode(void);
//设置模块进入省电模式
extern void LCDSetSleep(void);
//使模块退出省电模式
extern void LCDWakeup(void);
//显示一个模块资源中的位图在指定位置上
extern void PutBitmap(LCDBYTE x,LCDBYTE y,unsigned short Index);
//在指定窗口显示局部位图图像
extern void PutBitmap_Window(LCDBYTE x,LCDBYTE y,unsigned short x_w,unsigned short y_h,
					unsigned short x_s,unsigned short y_s,unsigned short Index);

extern void PutBitmap_Ignor(DOTBYTE left, DOTBYTE top, unsigned short Index);

extern void SetAreaInverse(DOTBYTE left, DOTBYTE top, DOTBYTE right, DOTBYTE bottom, LCDBYTE color);

//扩展应用功能......
//读取模块主芯片ID号
extern unsigned char ReadMzTHChipID(unsigned char Num,unsigned short TimerOut,unsigned char *Buf);

extern DOTBYTE X_Witch;							//存下当前所选择的ASCII英文字库的字符点数X轴宽度
extern DOTBYTE Y_Witch;							//存下当前所选择的ASCII英文字库的字符点数Y轴高度
extern DOTBYTE X_Witch_cn;						//存下当前所选择的中文字库的字符点数X轴宽度
extern DOTBYTE Y_Witch_cn;						//存下当前所选择的中文字库的字符点数Y轴高度


