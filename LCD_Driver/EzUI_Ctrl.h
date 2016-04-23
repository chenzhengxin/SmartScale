#ifndef __MZTHGUI_CTRL_H__
#define __MZTHGUI_CTRL_H__

#define EZUI_WAITTIMEROUT	1000000

//��ID�Ÿ��ؼ�������ֵ/���ݶ�ȡָ��
extern void EzUI_BodyReadVarCommand(unsigned short ID);
//��ID�Ÿ�ָ���ؼ���ֵ,���ݳ���Ϊ1�ֽ�
extern void EzUI_BodySetVar_Byte(unsigned short ID,unsigned char var);
//��ID�Ÿ�ָ���ؼ���ֵ,���ݳ���Ϊ2�ֽ�
extern void EzUI_BodySetVar_Word(unsigned short ID,unsigned short var);
//��ID�Ÿ�ָ���ؼ���ֵ,���ݳ���Ϊ4�ֽ�
extern void EzUI_BodySetVar_Int(unsigned short ID,unsigned int var);
//��ID�Ÿ�ָ���ؼ���ֵ,���ݳ���Ϊ4�ֽڵĸ�����
extern void EzUI_BodySetVar_Float(unsigned short ID,float var);
//��ID�Ÿ�ָ���ؼ���ֵ,���ݳ��Ȳ���,���ɲ���ָ��,Ҫ������120�ֽ�
extern void EzUI_BodySetVar_BufLen(unsigned short ID,unsigned char *buf,unsigned char len);
//��ID�Ÿ�ָ���ؼ���ֵ,���ݳ��Ȳ���,��Ҫ������120�ֽ�
extern void EzUI_BodySetVar_Buf(unsigned short ID,unsigned char *buf);


//��ID�Ÿ���ֵ�ؼ�������ֵ,��ֵ����������ʾ
//��ؼ�����Ϊ������,����øú������ܻᵼ�����õ���������
extern void EzUI_NumberEdit_SetVarInt(unsigned short ID,int Dat);
//��ID�Ÿ���ֵ�ؼ�������ֵ,��ֵ�Ը�������ʾ
//��ؼ�����Ϊ������,����øú������ܻᵼ�����õ���������
extern void EzUI_NumberEdit_SetVarFloat(unsigned short ID,float Dat);
//��ID�Ÿ���ֵ�ؼ���ȡ��ǰ�ؼ���ֵ,��ֵ����������ʾ
//��ؼ�������Ϊ������,���ȡ�����ݽ�������
//����0��ʾ��ȡ�ɹ�,����0xff��ʾ��ʱ ����0xE0��ʾ�ؼ�������  ����0xE4��ʾ�ؼ�����ָ���Ŀؼ�����
extern unsigned char EzUI_NumberEdit_ReadVarInt(unsigned short ID,int *Dat);
//��ID�Ÿ���ֵ�ؼ���ȡ��ǰ�ؼ���ֵ,��ֵ�Ը�������ʾ
//��ؼ�������Ϊ������,���ȡ�����ݽ�������
//����0��ʾ��ȡ�ɹ�,����0xff��ʾ��ʱ ����0xE0��ʾ�ؼ�������  ����0xE4��ʾ�ؼ�����ָ���Ŀؼ�����
extern unsigned char EzUI_NumberEdit_ReadVarFloat(unsigned short ID,float *Dat);


//��ID�Ÿ��ַ����ؼ������ַ�������
extern void EzUI_StringEdit_SetString(unsigned short ID,unsigned char *Str);
//��ID�Ŷ�ȡ�ַ����ؼ���ǰ���ַ�������
//����0��ʾ��ȡ�ɹ�,����0xff��ʾ��ʱ ����0xE0��ʾ�ؼ�������  ����0xE4��ʾ�ؼ�����ָ���Ŀؼ�����
//��,ͨ��ָ�뷵���ַ����ĳ���
extern unsigned char EzUI_StringEdit_ReadString(unsigned short ID,unsigned char *Buf,unsigned char *len);


//��ID�Ÿ�ָ��������ѡ��ؼ����õ�ǰѡ��
extern void EzUI_Combox_SetSel(unsigned short ID,unsigned char Sel);
//��ID�Ŷ�����ѡ��ؼ���ȡ��ǰ��ѡ��������
//����0��ʾ��ȡ�ɹ�,����0xff��ʾ��ʱ ����0xE0��ʾ�ؼ�������  ����0xE4��ʾ�ؼ�����ָ���Ŀؼ�����
//ͨ��ָ�뷵�ؿؼ���ǰ��ѡ��
extern unsigned char EzUI_Combox_ReadSel(unsigned short ID,char *Sel);


//��ID�Ÿ�ָ����λͼ��ť�ؼ�(����Ϊƹ�ҿ���)����״̬
extern void EzUI_BitmapButton_SetStatus(unsigned short ID,unsigned char Status);
//��ID�Ŷ�ȡָ��λͼ��ť�ؼ�(����Ϊƹ�ҿ���)�ĵ�ǰ״̬
//����0��ʾ��ȡ�ɹ�,����0xff��ʾ��ʱ ����0xE0��ʾ�ؼ�������  ����0xE4��ʾ�ؼ�����ָ���Ŀؼ�����
//ͨ��ָ�뷵�ؿؼ���ǰ״̬
extern unsigned char EzUI_BitmapButton_ReadStatus(unsigned short ID,unsigned char *status);


//��ID�����ý������ؼ��ĵ�ǰ��ֵ
extern void EzUI_ProcessCtrl_SetVar(unsigned short ID,unsigned short Var);
//��ID�Ŷ�ȡ�������ؼ���ǰ��ֵ
//����0��ʾ��ȡ�ɹ�,����0xff��ʾ��ʱ ����0xE0��ʾ�ؼ�������  ����0xE4��ʾ�ؼ�����ָ���Ŀؼ�����
//ͨ��ָ�뷵�ؿؼ���ǰ��ֵ
extern unsigned char EzUI_ProcessCtrl_ReadVar(unsigned short ID,unsigned short *Var);


//��ID�Ÿ����οؼ��Ĳ����߲�����ֵ
extern void EzUI_WaveFormCtrl_InserVar(unsigned short ID,unsigned short Line1,unsigned short Line2,unsigned short Line3,unsigned short Line4);
//��ID�ſ���ָ���Ĳ��οؼ����в������ػ�
extern void EzUI_WaveFormCtrl_ReDrawWave(unsigned short ID);

//��ID�ſ���ָ���Ķ����ؼ�����״̬
extern void EzUI_GifCtrl_SetStatus(unsigned short ID,unsigned char status);
//��ID�Ŷ�ȡ�����ؼ���ǰ״̬
//����0��ʾ��ȡ�ɹ�,����0xff��ʾ��ʱ ����0xE0��ʾ�ؼ�������  ����0xE4��ʾ�ؼ�����ָ���Ŀؼ�����
//ͨ��ָ�뷵�ؿؼ���ǰ״̬
extern unsigned char EzUI_GifCtrl_ReadVar(unsigned short ID,unsigned char *Status);


//��ȡ��ǰ��ʾ����������
//����0��ʾ��ȡ�ɹ�,����0xff��ʾ��ʱ
//ʹ��ָ�뷵��һ��WORD�͵Ľ���������
extern unsigned char EzUI_GetNowActiveGui(unsigned short *index);

//���õ�ǰ�����������
//���л���ǰ��ʾ����
extern void EzUI_SetNowActiveGui(unsigned short index);


//����ؼ�����
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

#define CTRLCLASS_GUIINTERFACE	0xff		//��ʾ����,ָ���������ݲ������κ�һ���ؼ�

#endif

