#include "util.h"

float string_to_float(char *str)
{
    u8 i,j,k,negative=0;
    #define s_temp str
    float result=0,result_1=0;
    for(i=0;i<10;i++)
    {
         j=str[i];
        if(j==0||((j<'0'||j>'9')&&(j!='.')&&(j!='-')))break;             
    } 
    k=j=i;//��ֵ�ĸ���
    for(i=0;i<j;i++)//����С�����λ�ã�������С����λ�ڵ�i+1λ
    {
         if(s_temp[i]=='.')break;         
    }
    
    for(j=0;j<i;j++)
    {
         if(s_temp[j]=='-')     {negative=1;continue;}        
         result=result*10+(s_temp[j]-'0');            
    }
    j++;//��1��j=i+1����С�����λ��
    i=j;//��һ��С����λ��
    for(;j<k;j++)
    {
         if(s_temp[j]<'0'||s_temp[j]>'9')break;//�Ƿ��ַ�������
        result_1=result_1*10+(s_temp[j]-'0');      
    }
    for(j=0;j<(k-i);j++)result_1*=0.1;
    result+=result_1;
    
    if(negative)result=-result;
    return result;
}

char *my_itoa(long val, char *string, int radix)
{
    int nCnt = 0;
	int nNum = 1;
	int nFlag = 0;
	int nIndex = 0;
    int nTime = 0;
	int nTemp = radix;
	char pcDest[33] = "\0";
	char pcHex[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	if ((radix < 2) || (radix > 36))
	{
		return NULL;
	}                                      //�жϽ������Ƿ����Ҫ��

	if (val < 0)
	{
		string[0] = '-';
		nFlag = 1;
		val = abs(val);
	}                                     //�ж���������Ƿ�Ϊ����

	while(nNum > 0)
	{
		nCnt = val % nTemp;
		val = val - nCnt;
		nCnt = nCnt / ( nTemp / radix);
		if (nCnt > 0)
		{
			if ((radix < 16) || (nCnt < 10))
			{
				pcDest[nIndex] = nCnt + '0';
			}
			else if (nCnt >= 10)
			{
				pcDest[nIndex] = pcHex[nCnt - 10];
			}
		}
		else if (nCnt == 0)
		{
			pcDest[nIndex] = '0';
		}
		nNum = val / nTemp;
		nTemp = nTemp * radix;
		nIndex ++;
	}

	nIndex --;
	if(radix == 16)
	{
#if 0
		if (nFlag)
		{
			string[0] = '-';
			string[1] = '0';
			string[2] = 'X';
			nTime = 3;
		}
		else
		{
			string[0] = '0';
			string[1] = 'X';
			nTime = 2;
		}
#endif	
        if (nIndex == 0) 
		{
			string[nTime] = '0';
			nTime ++;
		}

		for ( ; nIndex >= 0; nIndex --)
		{
			string[nTime]  = pcDest[nIndex];
			nTime ++;
		}
	}                                           //16���ƴ���
	else if (radix == 8)
	{
		if (nFlag)
		{
			string[0] = '-';
			string[1] = '0';
			nTime = 2;
		}
		else
		{
			string[0] = '0';
			nTime = 1;
		}
   
        
		
		for ( ; nIndex >= 0; nIndex --)
		{
			string[nTime] = pcDest[nIndex];
			nTime ++;
		}
		
	}                                             //8���ƴ���
	else
	{
		if (nFlag)
		{
			string[0] = '-';
			nTime = 1;
		}
		else
		{
			nTime = 0;
		}

		for ( ; nIndex >= 0; nIndex --)
		{
			string[nTime] = pcDest[nIndex];
			nTime ++;
		}
	}                                              //�������ƴ���
	 
	return string;
}