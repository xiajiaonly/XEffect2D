#ifndef _JIA_XBYTEFUN_
#define _JIA_XBYTEFUN_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.1.1
//--------------------------------
#include "XOSDefine.h"
union _XBytes
{
	unsigned int bytes;
	struct 
	{
	unsigned int b00:1,b01:1,b02:1,b03:1,b04:1,b05:1,b06:1,b07:1;
	unsigned int b08:1,b09:1,b0a:1,b0b:1,b0c:1,b0d:1,b0e:1,b0f:1;
	unsigned int b10:1,b11:1,b12:1,b13:1,b14:1,b15:1,b16:1,b17:1;
	unsigned int b18:1,b19:1,b1a:1,b1b:1,b1c:1,b1d:1,b1e:1,b1f:1;
	}bits;
};
//bits operate
inline int getBit(unsigned char ch,int pos)			//获取ch某一位的值
{
	if(pos < 0 || pos >=8) return 0;
	//unsigned char temp = 0x01;
	//temp = temp << pos;
	//temp = temp & ch;
	//if(temp == 0) return 0;
	//else return 1;
	return ((0x01 << pos) & ch);
}
inline _XBool setBit(unsigned char &ch,int pos,int state)	//设置ch某一位的值
{
	if(pos < 0 || pos >= 8) return XFalse;
	
	unsigned char temp = (0x01 << pos);
	if(state == 0) ch = ch & (~temp);
	else ch = ch | temp;
	return XTrue;
}
inline _XBool setBitAnti(unsigned char &ch,int pos)		//将ch某一位的值置反
{
	if(pos < 0 || pos >= 8) return XFalse;
	unsigned char temp = (0x01 << pos);
	//if(temp & (* ch) != 0) (* ch) = (* ch) | temp;
	if(getBit(ch,pos) == 0) ch = ch | temp;
	else ch = ch & (~temp);
	return XTrue;
}
inline void setByteAnti(unsigned char &ch)	//将ch逐位取反
{
	ch = ~ch;
}
//取ch的pos位之后len长的位数的值
//unsigned char ch = 01101010;
//getByteValue(ch,1,3) = 110
inline int getByteValue(unsigned char ch,int pos,int len)
{
	if(pos < 0 || pos >= 8) return 0;
	if(len <= 0 || pos + len > 8) return 0;
	unsigned char temp = ch << pos;
	temp = temp >> (pos + (8 - pos - len));
	return temp;
}
extern unsigned int getByteValue(unsigned char *ch,int byteSum,int pos,int len,bool flag = true);

#endif