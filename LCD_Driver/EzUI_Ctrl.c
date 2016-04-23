//========================================================================
// �ļ���: EzUI_Ctrl.c
// ��  ��: Xinqiang Zhang(email: Xinqiang@Mzdesign.com.cn)
//			www.Mzdesign.com.cn
// ��  ��: 2013/05/30
// ��  ��: EzUIϵ��ģ��GUI��ʾ�ؼ�����
//
// ��  ��: ��ο�������ʾ����Ӳ������˵���������DataSheet,���Դ�����д����
//			��ӿڳ���;��Ӧ����ʾģ��ΪEzUIϵ��ģ��
// ��  ��:
//      2013/05/30      First version    Mz Design
//========================================================================
#include "LCD_Config.h"							//LCD�������ļ�
#include "usart.h"
#include "EzUI_Ctrl.h"
extern unsigned char Send_Buffer[SEND_BUFFER_SIZE];

//��ID�Ÿ��ؼ�������ֵ/���ݶ�ȡָ��
void EzUI_BodyReadVarCommand(unsigned short ID)
{
	Send_Buffer[0] = (unsigned char)(ID>>8);
	Send_Buffer[1] = (unsigned char)ID;
	UART_SendPacked(0xE0,Send_Buffer,2);
}

//��ID�Ÿ�ָ���ؼ���ֵ,���ݳ���Ϊ1�ֽ�
void EzUI_BodySetVar_Byte(unsigned short ID,unsigned char var)
{
	Send_Buffer[0] = (unsigned char)(ID>>8);
	Send_Buffer[1] = (unsigned char)ID;
	Send_Buffer[2] = var;
	UART_SendPacked(0xE5,Send_Buffer,3);
}
//��ID�Ÿ�ָ���ؼ���ֵ,���ݳ���Ϊ2�ֽ�
void EzUI_BodySetVar_Word(unsigned short ID,unsigned short var)
{
	Send_Buffer[0] = (unsigned char)(ID>>8);
	Send_Buffer[1] = (unsigned char)ID;
	Send_Buffer[2] = (unsigned char)(var>>8);
	Send_Buffer[3] = (unsigned char)(var);
	UART_SendPacked(0xE5,Send_Buffer,4);
}
//��ID�Ÿ�ָ���ؼ���ֵ,���ݳ���Ϊ4�ֽ�
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

//��ID�Ÿ�ָ���ؼ���ֵ,���ݳ���Ϊ4�ֽڵĸ�����
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

//��ID�Ÿ�ָ���ؼ���ֵ,���ݳ��Ȳ���,��Ҫ������120�ֽ�
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

//��ID�Ÿ�ָ���ؼ���ֵ,���ݳ��Ȳ���,��Ҫ������120�ֽ�
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

//��ID�Ÿ���ֵ�ؼ�������ֵ,��ֵ����������ʾ
//��ؼ�����Ϊ������,����øú������ܻᵼ�����õ���������
void EzUI_NumberEdit_SetVarInt(unsigned short ID,int Dat)
{
	EzUI_BodySetVar_Int(ID,(unsigned int)Dat);
}

//��ID�Ÿ���ֵ�ؼ�������ֵ,��ֵ�Ը�������ʾ
//��ؼ�����Ϊ������,����øú������ܻᵼ�����õ���������
void EzUI_NumberEdit_SetVarFloat(unsigned short ID,float Dat)
{
	EzUI_BodySetVar_Float(ID,Dat);
}

//��ID�Ÿ���ֵ�ؼ���ȡ��ǰ�ؼ���ֵ,��ֵ����������ʾ
//��ؼ�������Ϊ������,���ȡ�����ݽ�������
//����0��ʾ��ȡ�ɹ�,����0xff��ʾ��ʱ ����0xE0��ʾ�ؼ�������  ����0xE4��ʾ�ؼ�����ָ���Ŀؼ�����
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
			n_ID = UART_GetWord();					//��ȡ���ص�ID��
			if(n_ID==0xffff)						//��ȡ���ص�ID����Ϊ0xffff,��ʾ��Ч
			{
				ReadBack = 0xE0;
				break;
			}
			n_Class = UART_GetByte();				//��ȡ���صĿؼ�����
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

//��ID�Ÿ���ֵ�ؼ���ȡ��ǰ�ؼ���ֵ,��ֵ�Ը�������ʾ
//��ؼ�������Ϊ������,���ȡ�����ݽ�������
//����0��ʾ��ȡ�ɹ�,����0xff��ʾ��ʱ ����0xE0��ʾ�ؼ�������  ����0xE4��ʾ�ؼ�����ָ���Ŀؼ�����
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
			n_ID = UART_GetWord();					//��ȡ���ص�ID��
			if(n_ID==0xffff)						//��ȡ���ص�ID����Ϊ0xffff,��ʾ��Ч
			{
				ReadBack = 0xE0;
				break;
			}
			n_Class = UART_GetByte();				//��ȡ���صĿؼ�����
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

//��ID�Ÿ��ַ����ؼ������ַ�������
void EzUI_StringEdit_SetString(unsigned short ID,unsigned char *Str)
{
	EzUI_BodySetVar_Buf(ID,Str);
}

//��ID�Ŷ�ȡ�ַ����ؼ���ǰ���ַ�������
//����0��ʾ��ȡ�ɹ�,����0xff��ʾ��ʱ ����0xE0��ʾ�ؼ�������  ����0xE4��ʾ�ؼ�����ָ���Ŀؼ�����
//��,ͨ��ָ�뷵���ַ����ĳ���
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
			n_ID = UART_GetWord();					//��ȡ���ص�ID��
			if(n_ID==0xffff)						//��ȡ���ص�ID����Ϊ0xffff,��ʾ��Ч
			{
				ReadBack = 0xE0;
				break;
			}
			n_Class = UART_GetByte();				//��ȡ���صĿؼ�����
			if(n_Class!=BODYCLASS_STRING_EDIT)
			{
				ReadBack = 0xE4;
				break;
			}
			*len = UART_GetByte();					//��ȡ�ַ�������
			for(n_Class=0;n_Class<*len;n_Class++)
				Buf[n_Class] = UART_GetByte();
			Buf[*len] = 0;
			break;
		}
	}
	if(n_iTemp==0) ReadBack = 0xff;
	return ReadBack;
}

//��ID�Ÿ�ָ��������ѡ��ؼ����õ�ǰѡ��
void EzUI_Combox_SetSel(unsigned short ID,unsigned char Sel)
{
	EzUI_BodySetVar_Byte(ID,Sel);
}

//��ID�Ŷ�����ѡ��ؼ���ȡ��ǰ��ѡ��������
//����0��ʾ��ȡ�ɹ�,����0xff��ʾ��ʱ ����0xE0��ʾ�ؼ�������  ����0xE4��ʾ�ؼ�����ָ���Ŀؼ�����
//ͨ��ָ�뷵�ؿؼ���ǰ��ѡ��
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
			n_ID = UART_GetWord();					//��ȡ���ص�ID��
			if(n_ID==0xffff)						//��ȡ���ص�ID����Ϊ0xffff,��ʾ��Ч
			{
				ReadBack = 0xE0;
				break;
			}
			n_Class = UART_GetByte();				//��ȡ���صĿؼ�����
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

//��ID�Ÿ�ָ����λͼ��ť�ؼ�(����Ϊƹ�ҿ���)����״̬
void EzUI_BitmapButton_SetStatus(unsigned short ID,unsigned char Status)
{
	EzUI_BodySetVar_Byte(ID,Status);
}

//��ID�Ŷ�ȡָ��λͼ��ť�ؼ�(����Ϊƹ�ҿ���)�ĵ�ǰ״̬
//����0��ʾ��ȡ�ɹ�,����0xff��ʾ��ʱ ����0xE0��ʾ�ؼ�������  ����0xE4��ʾ�ؼ�����ָ���Ŀؼ�����
//ͨ��ָ�뷵�ؿؼ���ǰ״̬
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
			n_ID = UART_GetWord();					//��ȡ���ص�ID��
			if(n_ID==0xffff)						//��ȡ���ص�ID����Ϊ0xffff,��ʾ��Ч
			{
				ReadBack = 0xE0;
				break;
			}
			n_Class = UART_GetByte();				//��ȡ���صĿؼ�����
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

//��ID�����ý������ؼ��ĵ�ǰ��ֵ
void EzUI_ProcessCtrl_SetVar(unsigned short ID,unsigned short Var)
{
	EzUI_BodySetVar_Word(ID,Var);
}

//��ID�Ŷ�ȡ�������ؼ���ǰ��ֵ
//����0��ʾ��ȡ�ɹ�,����0xff��ʾ��ʱ ����0xE0��ʾ�ؼ�������  ����0xE4��ʾ�ؼ�����ָ���Ŀؼ�����
//ͨ��ָ�뷵�ؿؼ���ǰ��ֵ
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
			n_ID = UART_GetWord();					//��ȡ���ص�ID��
			if(n_ID==0xffff)						//��ȡ���ص�ID����Ϊ0xffff,��ʾ��Ч
			{
				ReadBack = 0xE0;
				break;
			}
			n_Class = UART_GetByte();				//��ȡ���صĿؼ�����
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

//��ID�Ÿ����οؼ��Ĳ����߲�����ֵ
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
//��ID�ſ���ָ���Ĳ��οؼ����в������ػ�
void EzUI_WaveFormCtrl_ReDrawWave(unsigned short ID)
{
	Send_Buffer[0] = (unsigned char)(ID>>8);
	Send_Buffer[1] = (unsigned char)ID;
	Send_Buffer[2] = 3;
	UART_SendPacked(0xE6,Send_Buffer,3);
}

//��ID�ſ���ָ���Ķ����ؼ�����״̬
void EzUI_GifCtrl_SetStatus(unsigned short ID,unsigned char status)
{
	EzUI_BodySetVar_Byte(ID,status);
}
//��ID�Ŷ�ȡ�����ؼ���ǰ״̬
//����0��ʾ��ȡ�ɹ�,����0xff��ʾ��ʱ ����0xE0��ʾ�ؼ�������  ����0xE4��ʾ�ؼ�����ָ���Ŀؼ�����
//ͨ��ָ�뷵�ؿؼ���ǰ״̬
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
			n_ID = UART_GetWord();					//��ȡ���ص�ID��
			if(n_ID==0xffff)						//��ȡ���ص�ID����Ϊ0xffff,��ʾ��Ч
			{
				ReadBack = 0xE0;
				break;
			}
			n_Class = UART_GetByte();				//��ȡ���صĿؼ�����
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

//��ȡ��ǰ��ʾ����������
//����0��ʾ��ȡ�ɹ�,����0xff��ʾ��ʱ
//ʹ��ָ�뷵��һ��WORD�͵Ľ���������
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
			*index = UART_GetWord();					//��ȡ���ص�ID��
			break;
		}
	}
	if(n_iTemp==0) ReadBack = 0xff;
	return ReadBack;
}

//���õ�ǰ�����������
//���л���ǰ��ʾ����
void EzUI_SetNowActiveGui(unsigned short index)
{
	Send_Buffer[0] = (unsigned char)(index>>8);
	Send_Buffer[1] = (unsigned char)(index);
	UART_SendPacked(0xE8,Send_Buffer,2);
}
