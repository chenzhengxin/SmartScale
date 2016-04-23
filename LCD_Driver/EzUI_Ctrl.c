//========================================================================
// 文件名: EzUI_Ctrl.c
// 作  者: Xinqiang Zhang(email: Xinqiang@Mzdesign.com.cn)
//			www.Mzdesign.com.cn
// 日  期: 2013/05/30
// 描  述: EzUI系列模块GUI显示控件程序集
//
// 参  考: 请参考具体显示器的硬件描述说明书或者是DataSheet,并以此来编写加速
//			版接口程序;对应的显示模块为EzUI系列模块
// 版  本:
//      2013/05/30      First version    Mz Design
//========================================================================
#include "LCD_Config.h"							//LCD的配置文件
#include "usart.h"
#include "EzUI_Ctrl.h"
extern unsigned char Send_Buffer[SEND_BUFFER_SIZE];

//按ID号给控件发送数值/数据读取指令
void EzUI_BodyReadVarCommand(unsigned short ID)
{
	Send_Buffer[0] = (unsigned char)(ID>>8);
	Send_Buffer[1] = (unsigned char)ID;
	UART_SendPacked(0xE0,Send_Buffer,2);
}

//按ID号给指定控件设值,数据长度为1字节
void EzUI_BodySetVar_Byte(unsigned short ID,unsigned char var)
{
	Send_Buffer[0] = (unsigned char)(ID>>8);
	Send_Buffer[1] = (unsigned char)ID;
	Send_Buffer[2] = var;
	UART_SendPacked(0xE5,Send_Buffer,3);
}
//按ID号给指定控件设值,数据长度为2字节
void EzUI_BodySetVar_Word(unsigned short ID,unsigned short var)
{
	Send_Buffer[0] = (unsigned char)(ID>>8);
	Send_Buffer[1] = (unsigned char)ID;
	Send_Buffer[2] = (unsigned char)(var>>8);
	Send_Buffer[3] = (unsigned char)(var);
	UART_SendPacked(0xE5,Send_Buffer,4);
}
//按ID号给指定控件设值,数据长度为4字节
void EzUI_BodySetVar_Int(unsigned short ID,unsigned int var)
{
	Send_Buffer[0] = (unsigned char)(ID>>8);
	Send_Buffer[1] = (unsigned char)ID;
	Send_Buffer[2] = (unsigned char)(var>>24);
	Send_Buffer[3] = (unsigned char)(var>>16);
	Send_Buffer[4] = (unsigned char)(var>>8);
	Send_Buffer[5] = (unsigned char)(var);
	UART_SendPacked(0xE5,Send_Buffer,6);
}

//按ID号给指定控件设值,数据长度为4字节的浮点数
void EzUI_BodySetVar_Float(unsigned short ID,float var)
{
	unsigned char * Ptr;
	Ptr = (unsigned char *)&var;
	Send_Buffer[0] = (unsigned char)(ID>>8);
	Send_Buffer[1] = (unsigned char)ID;
	Send_Buffer[2] = (unsigned char)Ptr[3];
	Send_Buffer[3] = (unsigned char)Ptr[2];//(var>>16);
	Send_Buffer[4] = (unsigned char)Ptr[1];//(var>>8);
	Send_Buffer[5] = (unsigned char)Ptr[0];//(var);
	UART_SendPacked(0xE5,Send_Buffer,6);
}

//按ID号给指定控件设值,数据长度不限,但要求少于120字节
void EzUI_BodySetVar_BufLen(unsigned short ID,unsigned char *buf,unsigned char len)
{
	unsigned char i;
	Send_Buffer[0] = (unsigned char)(ID>>8);
	Send_Buffer[1] = (unsigned char)ID;
	for(i=0;i<len;i++)
	{
		Send_Buffer[i+2] = (unsigned char)buf[i];
	}
	UART_SendPacked(0xE5,Send_Buffer,i+2);
}

//按ID号给指定控件设值,数据长度不限,但要求少于120字节
void EzUI_BodySetVar_Buf(unsigned short ID,unsigned char *buf)
{
	unsigned char i;
	Send_Buffer[0] = (unsigned char)(ID>>8);
	Send_Buffer[1] = (unsigned char)ID;

	for(i=0;i<120;i++)
	{
		if(buf[i]==0) break;
		Send_Buffer[i+2] = (unsigned char)buf[i];
	}
	
	UART_SendPacked(0xE5,Send_Buffer,i+2);
}

//按ID号给数值控件设置数值,数值以整型数表示
//如控件定义为浮点数,则调用该函数可能会导致设置的数据有误
void EzUI_NumberEdit_SetVarInt(unsigned short ID,int Dat)
{
	EzUI_BodySetVar_Int(ID,(unsigned int)Dat);
}

//按ID号给数值控件设置数值,数值以浮点数表示
//如控件定义为整型数,则调用该函数可能会导致设置的数据有误
void EzUI_NumberEdit_SetVarFloat(unsigned short ID,float Dat)
{
	EzUI_BodySetVar_Float(ID,Dat);
}

//按ID号给数值控件读取当前控件数值,数值以整型数表示
//如控件所定义为浮点数,则读取的数据将会有误
//返回0表示读取成功,返回0xff表示超时 返回0xE0表示控件不存在  返回0xE4表示控件非所指定的控件类型
unsigned char EzUI_NumberEdit_ReadVarInt(unsigned short ID,int *Dat)
{
	unsigned char ReadBack=0,n_Class;
	unsigned short n_ID;
	int n_iTemp;
	EzUI_BodyReadVarCommand(ID);
	n_iTemp = EZUI_WAITTIMEROUT;
	while(n_iTemp--)
	{
		if(UserTestUARTFIFOCommand()==0xE0)
		{
			n_ID = UART_GetWord();					//读取返回的ID号
			if(n_ID==0xffff)						//读取返回的ID号如为0xffff,表示无效
			{
				ReadBack = 0xE0;
				break;
			}
			n_Class = UART_GetByte();				//读取返回的控件类型
			if(n_Class!=BODYCLASS_NUMBER_EDIT)
			{
				ReadBack = 0xE4;
				break;
			}
			*Dat = UART_GetSInt();
			break;
		}
	}
	if(n_iTemp==0) ReadBack = 0xff;
	return ReadBack;
}

//按ID号给数值控件读取当前控件数值,数值以浮点数表示
//如控件所定义为整型数,则读取的数据将会有误
//返回0表示读取成功,返回0xff表示超时 返回0xE0表示控件不存在  返回0xE4表示控件非所指定的控件类型
unsigned char EzUI_NumberEdit_ReadVarFloat(unsigned short ID,float *Dat)
{
	unsigned char ReadBack=0,n_Class;
	unsigned short n_ID;
	int n_iTemp;
	EzUI_BodyReadVarCommand(ID);
	n_iTemp = EZUI_WAITTIMEROUT;
	while(n_iTemp--)
	{
		if(UserTestUARTFIFOCommand()==0xE0)
		{
			n_ID = UART_GetWord();					//读取返回的ID号
			if(n_ID==0xffff)						//读取返回的ID号如为0xffff,表示无效
			{
				ReadBack = 0xE0;
				break;
			}
			n_Class = UART_GetByte();				//读取返回的控件类型
			if(n_Class!=BODYCLASS_NUMBER_EDIT)
			{
				ReadBack = 0xE4;
				break;
			}
			UART_GetFloat(Dat);
			break;
		}
	}
	if(n_iTemp==0) ReadBack = 0xff;
	return ReadBack;
}

//按ID号给字符串控件设置字符串数据
void EzUI_StringEdit_SetString(unsigned short ID,unsigned char *Str)
{
	EzUI_BodySetVar_Buf(ID,Str);
}

//按ID号读取字符串控件当前的字符串数据
//返回0表示读取成功,返回0xff表示超时 返回0xE0表示控件不存在  返回0xE4表示控件非所指定的控件类型
//另,通过指针返回字符串的长度
unsigned char EzUI_StringEdit_ReadString(unsigned short ID,unsigned char *Buf,unsigned char *len)
{
	unsigned char ReadBack=0,n_Class;
	unsigned short n_ID;
	int n_iTemp;
	EzUI_BodyReadVarCommand(ID);
	n_iTemp = EZUI_WAITTIMEROUT;
	while(n_iTemp--)
	{
		if(UserTestUARTFIFOCommand()==0xE0)
		{
			n_ID = UART_GetWord();					//读取返回的ID号
			if(n_ID==0xffff)						//读取返回的ID号如为0xffff,表示无效
			{
				ReadBack = 0xE0;
				break;
			}
			n_Class = UART_GetByte();				//读取返回的控件类型
			if(n_Class!=BODYCLASS_STRING_EDIT)
			{
				ReadBack = 0xE4;
				break;
			}
			*len = UART_GetByte();					//读取字符串长度
			for(n_Class=0;n_Class<*len;n_Class++)
				Buf[n_Class] = UART_GetByte();
			Buf[*len] = 0;
			break;
		}
	}
	if(n_iTemp==0) ReadBack = 0xff;
	return ReadBack;
}

//按ID号给指定的下拉选框控件设置当前选项
void EzUI_Combox_SetSel(unsigned short ID,unsigned char Sel)
{
	EzUI_BodySetVar_Byte(ID,Sel);
}

//按ID号对下拉选框控件读取当前被选项索引号
//返回0表示读取成功,返回0xff表示超时 返回0xE0表示控件不存在  返回0xE4表示控件非所指定的控件类型
//通过指针返回控件当前所选项
unsigned char EzUI_Combox_ReadSel(unsigned short ID,char *Sel)
{
	unsigned char ReadBack=0,n_Class;
	unsigned short n_ID;
	int n_iTemp;
	EzUI_BodyReadVarCommand(ID);
	n_iTemp = EZUI_WAITTIMEROUT;
	while(n_iTemp--)
	{
		if(UserTestUARTFIFOCommand()==0xE0)
		{
			n_ID = UART_GetWord();					//读取返回的ID号
			if(n_ID==0xffff)						//读取返回的ID号如为0xffff,表示无效
			{
				ReadBack = 0xE0;
				break;
			}
			n_Class = UART_GetByte();				//读取返回的控件类型
			if(n_Class!=BODYCLASS_COMBOX_SEL)
			{
				ReadBack = 0xE4;
				break;
			}
			*Sel = UART_GetByte();
			break;
		}
	}
	if(n_iTemp==0) ReadBack = 0xff;
	return ReadBack;
}

//按ID号给指定的位图按钮控件(属性为乒乓开关)设置状态
void EzUI_BitmapButton_SetStatus(unsigned short ID,unsigned char Status)
{
	EzUI_BodySetVar_Byte(ID,Status);
}

//按ID号读取指定位图按钮控件(属性为乒乓开关)的当前状态
//返回0表示读取成功,返回0xff表示超时 返回0xE0表示控件不存在  返回0xE4表示控件非所指定的控件类型
//通过指针返回控件当前状态
unsigned char EzUI_BitmapButton_ReadStatus(unsigned short ID,unsigned char *status)
{
	unsigned char ReadBack=0,n_Class;
	unsigned short n_ID;
	int n_iTemp;
	EzUI_BodyReadVarCommand(ID);
	n_iTemp = EZUI_WAITTIMEROUT;
	while(n_iTemp--)
	{
		if(UserTestUARTFIFOCommand()==0xE0)
		{
			n_ID = UART_GetWord();					//读取返回的ID号
			if(n_ID==0xffff)						//读取返回的ID号如为0xffff,表示无效
			{
				ReadBack = 0xE0;
				break;
			}
			n_Class = UART_GetByte();				//读取返回的控件类型
			if(n_Class!=BODYCLASS_BITMAP_BUTTON)
			{
				ReadBack = 0xE4;
				break;
			}
			*status = UART_GetByte();
			break;
		}
	}
	if(n_iTemp==0) ReadBack = 0xff;
	return ReadBack;
}

//按ID号设置进度条控件的当前数值
void EzUI_ProcessCtrl_SetVar(unsigned short ID,unsigned short Var)
{
	EzUI_BodySetVar_Word(ID,Var);
}

//按ID号读取进度条控件当前数值
//返回0表示读取成功,返回0xff表示超时 返回0xE0表示控件不存在  返回0xE4表示控件非所指定的控件类型
//通过指针返回控件当前数值
unsigned char EzUI_ProcessCtrl_ReadVar(unsigned short ID,unsigned short *Var)
{
	unsigned char ReadBack=0,n_Class;
	unsigned short n_ID;
	int n_iTemp;
	EzUI_BodyReadVarCommand(ID);
	n_iTemp = EZUI_WAITTIMEROUT;
	while(n_iTemp--)
	{
		if(UserTestUARTFIFOCommand()==0xE0)
		{
			n_ID = UART_GetWord();					//读取返回的ID号
			if(n_ID==0xffff)						//读取返回的ID号如为0xffff,表示无效
			{
				ReadBack = 0xE0;
				break;
			}
			n_Class = UART_GetByte();				//读取返回的控件类型
			if(n_Class!=BODYCLASS_PROCESS_CTRL)
			{
				ReadBack = 0xE4;
				break;
			}
			*Var = UART_GetWord();
			UART_GetWord();
			break;
		}
	}
	if(n_iTemp==0) ReadBack = 0xff;
	return ReadBack;
}

//按ID号给波形控件的波形线插入数值
void EzUI_WaveFormCtrl_InserVar(unsigned short ID,unsigned short Line1,unsigned short Line2,unsigned short Line3,unsigned short Line4)
{
	Send_Buffer[0] = (unsigned char)(ID>>8);
	Send_Buffer[1] = (unsigned char)ID;
	Send_Buffer[2] = (unsigned char)(Line1>>8);
	Send_Buffer[3] = (unsigned char)(Line1);
	Send_Buffer[4] = (unsigned char)(Line2>>8);
	Send_Buffer[5] = (unsigned char)(Line2);
	Send_Buffer[6] = (unsigned char)(Line3>>8);
	Send_Buffer[7] = (unsigned char)(Line3);
	Send_Buffer[8] = (unsigned char)(Line4>>8);
	Send_Buffer[9] = (unsigned char)(Line4);
	UART_SendPacked(0xE5,Send_Buffer,10);
}
//按ID号控制指定的波形控件进行波形线重绘
void EzUI_WaveFormCtrl_ReDrawWave(unsigned short ID)
{
	Send_Buffer[0] = (unsigned char)(ID>>8);
	Send_Buffer[1] = (unsigned char)ID;
	Send_Buffer[2] = 3;
	UART_SendPacked(0xE6,Send_Buffer,3);
}

//按ID号控制指定的动画控件设置状态
void EzUI_GifCtrl_SetStatus(unsigned short ID,unsigned char status)
{
	EzUI_BodySetVar_Byte(ID,status);
}
//按ID号读取动画控件当前状态
//返回0表示读取成功,返回0xff表示超时 返回0xE0表示控件不存在  返回0xE4表示控件非所指定的控件类型
//通过指针返回控件当前状态
unsigned char EzUI_GifCtrl_ReadVar(unsigned short ID,unsigned char *Status)
{
	unsigned char ReadBack=0,n_Class;
	unsigned short n_ID;
	int n_iTemp;
	EzUI_BodyReadVarCommand(ID);
	n_iTemp = EZUI_WAITTIMEROUT;
	while(n_iTemp--)
	{
		if(UserTestUARTFIFOCommand()==0xE0)
		{
			n_ID = UART_GetWord();					//读取返回的ID号
			if(n_ID==0xffff)						//读取返回的ID号如为0xffff,表示无效
			{
				ReadBack = 0xE0;
				break;
			}
			n_Class = UART_GetByte();				//读取返回的控件类型
			if(n_Class!=BODYCLASS_GIF_CTRL)
			{
				ReadBack = 0xE4;
				break;
			}
			*Status = UART_GetByte();
			//UART_GetWord();
			break;
		}
	}
	if(n_iTemp==0) ReadBack = 0xff;
	return ReadBack;
}

//获取当前显示界面索引号
//返回0表示读取成功,返回0xff表示超时
//使用指针返回一个WORD型的界面索引号
unsigned char EzUI_GetNowActiveGui(unsigned short *index)
{
	unsigned char ReadBack=0;//,n_Class;
	//unsigned short n_ID;
	int n_iTemp;
	Send_Buffer[0] = 0;
	UART_SendPacked(0xE9,Send_Buffer,1);
	n_iTemp = EZUI_WAITTIMEROUT;
	while(n_iTemp--)
	{
		if(UserTestUARTFIFOCommand()==0xE9)
		{
			*index = UART_GetWord();					//读取返回的ID号
			break;
		}
	}
	if(n_iTemp==0) ReadBack = 0xff;
	return ReadBack;
}

//设置当前活动界面索引号
//即切换当前显示界面
void EzUI_SetNowActiveGui(unsigned short index)
{
	Send_Buffer[0] = (unsigned char)(index>>8);
	Send_Buffer[1] = (unsigned char)(index);
	UART_SendPacked(0xE8,Send_Buffer,2);
}
