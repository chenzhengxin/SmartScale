# SmartScale
Create Project
Modify Project
Modify Again
int main()
{
	int i = 0;
	int ret = 0;
	char buf1[] = "工号           姓名             物种             总重量               总金额\r\n";
	char buf2[] = "T12035        罗海斌            海蛎             100市斤            10000人民币\r\n";
	/*文件名中的0508表示月日*/
	unsigned char *buf3 = "/名单0508.TXT"; /*文件名前面的 / 字符不得省略（表示根目录创建该文件）*/

	/*InitUSB()也包含了对STM32的一些硬件的初始化，如有重复可将其删除（对SPI2的初始化切勿删除）
	 *InitUSB()函数里面还有一处需要弹出 “请插入U盘！” 的提示框，具体位置函数里面有标注
	*/
	ret = InitUSB();
	if(!ret){
		return 0; /*操作失败*/
	}
	/*这边建议用OpenUSBFile函数，这样多台称的话所有数据都可以保存在同一个文件中*/
	ret = OpenUSBFile((PUINT8)buf3, 2016, 5, 8, 10, 20);/*如果同名文件存在，则打开继续在文件尾部添加*/
	//ret = CreateUSBFile((PUINT8)buf3);/*如果同名文件存在，则先删除同名文件再重新创建*/
	if(!ret){
		return 0; /*操作失败*/
	}

	/* 为指定文件设置创建日期和时间 */
	ret = SetFileCreateTime(NULL, MAKE_FILE_DATE( 2016, 2, 20 ), MAKE_FILE_TIME( 20, 55, 52 ));
	if(!ret){
		return 0; /*操作失败*/
	}

	/*以下的for循环是模拟从SD卡或者flash中读取一行文件内容或者一个员工数据记录，然后再将读取到的内容写入U盘中*/
	for(i = 0; i < 500; i++)
	{
		if(i == 0){
			ret = WriteDataBufToUSB((char *)buf1);
			if(!ret){
				return 0; /*操作失败*/
			}	
		}else{
		    ret = WriteDataBufToUSB((char *)buf2);
			if(!ret){
				return 0; /*操作失败*/
			}
		}
	}
	ret = CloseUSBFile();
	if(!ret){
		return 0; /*操作失败*/
	}

	/* 为指定文件设置修改日期和时间 */
	ret = SetFileWriteTime((PUINT8)buf3, MAKE_FILE_DATE( 2016, 2, 20 ), MAKE_FILE_TIME( 21, 3, 52 ));
	if(!ret){
		return 0; /*操作失败*/
	}
	
	/*在这里（WaitTakeOutUSB()函数之前）弹出 “数据拷贝完成，请拔出U盘！” 的提示框*/
	WaitTakeOutUSB();

	while(1);

	//return ret;
}

