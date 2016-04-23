#include "LCD_Config.h"
#include "hw_Setting.h"


//LCD ��ʼ������
extern void LCD_Initial(void);	//���б�Ҫ�ú�����������˿ڳ�ʼ������λ�Ȳ���
//��ֱ�ߺ�����s_x��s_yΪ��ʼ�㣬e_x��e_yΪ������
extern void Line(LCDBYTE s_x,LCDBYTE s_y,LCDBYTE e_x,LCDBYTE e_y);
//��ͼ�߿���ǰ��ͼ��ɫ����
extern void SetPaintMode(BYTE LineWith,LCDBYTE Color);
//ѡ��Ӣ��(ASCII �ֿ�),�ú���ֻ��ѡ��ģ���ڲ���Ӣ���ֿ�,������ο�ģ��ʹ���ֲ�
extern void FontSet(DOTBYTE Font_NUM,LCDBYTE Color);
//ѡ��Ӣ��(ASCII�ֿ�),�ú���ֻ��ѡ��ģ�������ص���Դ�е�Ӣ���ֿ�
extern void FontSet_rs(LCDBYTE Font_NUM,LCDBYTE Color);
//ѡ�������ֿ�,�ú���ֻ��ѡ��ģ�������ص���Դ�е����ĺ����ֿ�
extern void FontSet_cn(LCDBYTE Font_NUM,LCDBYTE Color);
//��ʾһ������
extern void PutChar_cn(LCDBYTE x,LCDBYTE y,unsigned short * GB) ;
//��ʾһ�������ַ���
extern void PutString_cn(LCDBYTE x,LCDBYTE y,unsigned char *p);
//�����ַ���ʾ��ģʽ,�����ñ���ɫ
extern void FontMode(BYTE Mode,LCDBYTE FontBackColor);
//��x��y��������д��һ����׼�ַ�
extern void PutChar(DOTBYTE x,DOTBYTE y,char a);
//��x��y������Ϊ��ʼд��һ����׼�ַ���
extern void PutString(DOTBYTE x,DOTBYTE y,char *p);
//��x��y������Ϊ���ģ�����һ����
extern void PutPixel(DOTBYTE x,DOTBYTE y);
//��x��y������Ϊ���ģ�����һ��Բ�߻�ʵ��Բ
extern void Circle(DOTBYTE x,DOTBYTE y,DOTBYTE r,BYTE mode);
//����һ����left��top��right��bottomΪ�����Խǵľ��ο����ʵ�ľ���
extern void Rectangle(DOTBYTE left, DOTBYTE top, DOTBYTE right, DOTBYTE bottom, BYTE Mode);
//ֱ��������ʾ
extern void NumberDis(LCDBYTE x,LCDBYTE y,signed int Num,unsigned char len,unsigned char type);
//����������ִ��ȫ��Ļ��������ǰ��ɫ
extern void ClrScreen(BYTE Mode);
//������������
extern void BackLightSet(unsigned short Gra);

extern void LCDSetWaitMode(void);
//����ģ�����ʡ��ģʽ
extern void LCDSetSleep(void);
//ʹģ���˳�ʡ��ģʽ
extern void LCDWakeup(void);
//��ʾһ��ģ����Դ�е�λͼ��ָ��λ����
extern void PutBitmap(LCDBYTE x,LCDBYTE y,unsigned short Index);
//��ָ��������ʾ�ֲ�λͼͼ��
extern void PutBitmap_Window(LCDBYTE x,LCDBYTE y,unsigned short x_w,unsigned short y_h,
					unsigned short x_s,unsigned short y_s,unsigned short Index);

extern void PutBitmap_Ignor(DOTBYTE left, DOTBYTE top, unsigned short Index);

extern void SetAreaInverse(DOTBYTE left, DOTBYTE top, DOTBYTE right, DOTBYTE bottom, LCDBYTE color);

//��չӦ�ù���......
//��ȡģ����оƬID��
extern unsigned char ReadMzTHChipID(unsigned char Num,unsigned short TimerOut,unsigned char *Buf);

extern DOTBYTE X_Witch;							//���µ�ǰ��ѡ���ASCIIӢ���ֿ���ַ�����X����
extern DOTBYTE Y_Witch;							//���µ�ǰ��ѡ���ASCIIӢ���ֿ���ַ�����Y��߶�
extern DOTBYTE X_Witch_cn;						//���µ�ǰ��ѡ��������ֿ���ַ�����X����
extern DOTBYTE Y_Witch_cn;						//���µ�ǰ��ѡ��������ֿ���ַ�����Y��߶�


