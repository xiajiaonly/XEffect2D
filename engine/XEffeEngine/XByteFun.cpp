//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XByteFun.h"
unsigned int getByteValue(unsigned char *ch,int byteSum,int pos,int len,bool flag)
{
	if(len > 32) return 0;	//最多只能是4byte个字节
	if(pos < 0 || pos >= (byteSum << 3)) return 0;
	if(len <= 0 || pos + len >= (byteSum << 3)) return 0;
	unsigned int temp = 0;
	int head = (pos >> 3);
	int end = ((pos + len) >> 3);
	if(flag)
	{//大头
		if(head == end) return getByteValue(ch[head],pos % 8,len);
		for(int i = head;i <= end;++ i)
		{
			if(i == head) temp = getByteValue(ch[i],pos % 8,8 - (pos % 8));
			else
			if(i == end) temp = (temp << ((pos + len) % 8)) + getByteValue(ch[i],0,(pos + len) % 8);
			else temp = (temp << 8) + ch[i];
		}
	}else
	{//小头(未测试)
		if(head == end) return getByteValue(ch[byteSum - head],pos % 8,len);
		for(int i = head;i <= end;++ i)
		{
			if(i == head) temp = getByteValue(ch[byteSum - i],pos % 8,8 - (pos % 8));
			else
			if(i == end) temp = (temp << ((pos + len) % 8)) + getByteValue(ch[byteSum - i],0,(pos + len) % 8);
			else temp = (temp << 8) + ch[byteSum - i];
		}
	}
	return temp;
}