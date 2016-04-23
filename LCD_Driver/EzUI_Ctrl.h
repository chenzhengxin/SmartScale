#ifndef __MZTHGUI_CTRL_H__
#define __MZTHGUI_CTRL_H__

#define EZUI_WAITTIMEROUT	1000000

//按ID号给控件发送数值/数据读取指令
extern void EzUI_BodyReadVarCommand(unsigned short ID);
//按ID号给指定控件设值,数据长度为1字节
extern void EzUI_BodySetVar_Byte(unsigned short ID,unsigned char var);
//按ID号给指定控件设值,数据长度为2字节
extern void EzUI_BodySetVar_Word(unsigned short ID,unsigned short var);
//按ID号给指定控件设值,数据长度为4字节
extern void EzUI_BodySetVar_Int(unsigned short ID,unsigned int var);
//按ID号给指定控件设值,数据长度为4字节的浮点数
extern void EzUI_BodySetVar_Float(unsigned short ID,float var);
//按ID号给指定控件设值,数据长度不限,但由参数指定,要求少于120字节
extern void EzUI_BodySetVar_BufLen(unsigned short ID,unsigned char *buf,unsigned char len);
//按ID号给指定控件设值,数据长度不限,但要求少于120字节
extern void EzUI_BodySetVar_Buf(unsigned short ID,unsigned char *buf);


//按ID号给数值控件设置数值,数值以整型数表示
//如控件定义为浮点数,则调用该函数可能会导致设置的数据有误
extern void EzUI_NumberEdit_SetVarInt(unsigned short ID,int Dat);
//按ID号给数值控件设置数值,数值以浮点数表示
//如控件定义为整型数,则调用该函数可能会导致设置的数据有误
extern void EzUI_NumberEdit_SetVarFloat(unsigned short ID,float Dat);
//按ID号给数值控件读取当前控件数值,数值以整型数表示
//如控件所定义为浮点数,则读取的数据将会有误
//返回0表示读取成功,返回0xff表示超时 返回0xE0表示控件不存在  返回0xE4表示控件非所指定的控件类型
extern unsigned char EzUI_NumberEdit_ReadVarInt(unsigned short ID,int *Dat);
//按ID号给数值控件读取当前控件数值,数值以浮点数表示
//如控件所定义为整型数,则读取的数据将会有误
//返回0表示读取成功,返回0xff表示超时 返回0xE0表示控件不存在  返回0xE4表示控件非所指定的控件类型
extern unsigned char EzUI_NumberEdit_ReadVarFloat(unsigned short ID,float *Dat);


//按ID号给字符串控件设置字符串数据
extern void EzUI_StringEdit_SetString(unsigned short ID,unsigned char *Str);
//按ID号读取字符串控件当前的字符串数据
//返回0表示读取成功,返回0xff表示超时 返回0xE0表示控件不存在  返回0xE4表示控件非所指定的控件类型
//另,通过指针返回字符串的长度
extern unsigned char EzUI_StringEdit_ReadString(unsigned short ID,unsigned char *Buf,unsigned char *len);


//按ID号给指定的下拉选框控件设置当前选项
extern void EzUI_Combox_SetSel(unsigned short ID,unsigned char Sel);
//按ID号对下拉选框控件读取当前被选项索引号
//返回0表示读取成功,返回0xff表示超时 返回0xE0表示控件不存在  返回0xE4表示控件非所指定的控件类型
//通过指针返回控件当前所选项
extern unsigned char EzUI_Combox_ReadSel(unsigned short ID,char *Sel);


//按ID号给指定的位图按钮控件(属性为乒乓开关)设置状态
extern void EzUI_BitmapButton_SetStatus(unsigned short ID,unsigned char Status);
//按ID号读取指定位图按钮控件(属性为乒乓开关)的当前状态
//返回0表示读取成功,返回0xff表示超时 返回0xE0表示控件不存在  返回0xE4表示控件非所指定的控件类型
//通过指针返回控件当前状态
extern unsigned char EzUI_BitmapButton_ReadStatus(unsigned short ID,unsigned char *status);


//按ID号设置进度条控件的当前数值
extern void EzUI_ProcessCtrl_SetVar(unsigned short ID,unsigned short Var);
//按ID号读取进度条控件当前数值
//返回0表示读取成功,返回0xff表示超时 返回0xE0表示控件不存在  返回0xE4表示控件非所指定的控件类型
//通过指针返回控件当前数值
extern unsigned char EzUI_ProcessCtrl_ReadVar(unsigned short ID,unsigned short *Var);


//按ID号给波形控件的波形线插入数值
extern void EzUI_WaveFormCtrl_InserVar(unsigned short ID,unsigned short Line1,unsigned short Line2,unsigned short Line3,unsigned short Line4);
//按ID号控制指定的波形控件进行波形线重绘
extern void EzUI_WaveFormCtrl_ReDrawWave(unsigned short ID);

//按ID号控制指定的动画控件设置状态
extern void EzUI_GifCtrl_SetStatus(unsigned short ID,unsigned char status);
//按ID号读取动画控件当前状态
//返回0表示读取成功,返回0xff表示超时 返回0xE0表示控件不存在  返回0xE4表示控件非所指定的控件类型
//通过指针返回控件当前状态
extern unsigned char EzUI_GifCtrl_ReadVar(unsigned short ID,unsigned char *Status);


//获取当前显示界面索引号
//返回0表示读取成功,返回0xff表示超时
//使用指针返回一个WORD型的界面索引号
extern unsigned char EzUI_GetNowActiveGui(unsigned short *index);

//设置当前活动界面索引号
//即切换当前显示界面
extern void EzUI_SetNowActiveGui(unsigned short index);


//定义控件类型
#define BODYCLASS_TOUCH_AREA	0x20
#define BODYCLASS_BITMAP_BUTTON	0x21
#define BODYCLASS_NUMBER_EDIT	0x22
#define BODYCLASS_STRING_EDIT	0x23
#define BODYCLASS_COMBOX_SEL	0x25
#define BODYCLASS_PROCESS_CTRL	0x10
#define BODYCLASS_WAVEFORM_CTRL	0x11
#define BODYCLASS_GIF_CTRL		0x12

#define CTRLCLASS_NUMINPUT_CTRL		0xE0
#define CTRLCLASS_STRINGINPUT_CTRL	0xE2

#define CTRLCLASS_GUIINTERFACE	0xff		//表示界面,指明该项数据不属于任何一个控件

#endif

