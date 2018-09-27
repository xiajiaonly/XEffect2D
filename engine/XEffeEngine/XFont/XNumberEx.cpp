#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XNumberEx.h"
#include "XResourcePack.h"
namespace XE{
XNumberTexture::XNumberTexture()
:m_isInited(XFalse)
,m_texture00(NULL)
,m_texture01(NULL)
,m_texture02(NULL)
,m_texture03(NULL)
,m_texture04(NULL)
,m_texture05(NULL)
,m_texture06(NULL)
,m_texture07(NULL)
,m_texture08(NULL)
,m_texture09(NULL)
,m_texture0a(NULL)
,m_texture0b(NULL)
,m_texture0c(NULL)
,m_texture0d(NULL)
,m_texture0e(NULL)
,m_texture0f(NULL)
{}
//fileName:pic/number_00/number_00.png
XBool XNumberTexture::init(const char *fileName,XResPos resPos)
{
	if(m_isInited ||
		fileName == NULL) return XFalse;
	m_resoursePosition = resPos;
	//下面开始分配内存空间
	m_texture00 = XMem::createMem<XTextureData>();
	m_texture01 = XMem::createMem<XTextureData>();
	m_texture02 = XMem::createMem<XTextureData>();
	m_texture03 = XMem::createMem<XTextureData>();
	m_texture04 = XMem::createMem<XTextureData>();
	m_texture05 = XMem::createMem<XTextureData>();
	m_texture06 = XMem::createMem<XTextureData>();
	m_texture07 = XMem::createMem<XTextureData>();
	m_texture08 = XMem::createMem<XTextureData>();
	m_texture09 = XMem::createMem<XTextureData>();
	m_texture0a = XMem::createMem<XTextureData>();
	m_texture0b = XMem::createMem<XTextureData>();
	m_texture0c = XMem::createMem<XTextureData>();
	m_texture0d = XMem::createMem<XTextureData>();
	m_texture0e = XMem::createMem<XTextureData>();
	m_texture0f = XMem::createMem<XTextureData>();
	if(m_texture00 == NULL || m_texture01 == NULL || m_texture02 == NULL || m_texture03 == NULL
		|| m_texture04 == NULL || m_texture05 == NULL || m_texture06 == NULL || m_texture07 == NULL
		|| m_texture08 == NULL || m_texture09 == NULL || m_texture0a == NULL || m_texture0b == NULL
		|| m_texture0c == NULL || m_texture0d == NULL || m_texture0e == NULL || m_texture0f == NULL)
	{
		XMem::XDELETE(m_texture00);XMem::XDELETE(m_texture01);XMem::XDELETE(m_texture02);XMem::XDELETE(m_texture03);
		XMem::XDELETE(m_texture04);XMem::XDELETE(m_texture05);XMem::XDELETE(m_texture06);XMem::XDELETE(m_texture07);
		XMem::XDELETE(m_texture08);XMem::XDELETE(m_texture09);XMem::XDELETE(m_texture0a);XMem::XDELETE(m_texture0b);
		XMem::XDELETE(m_texture0c);XMem::XDELETE(m_texture0d);XMem::XDELETE(m_texture0e);XMem::XDELETE(m_texture0f);
		return XFalse;
	}
	//载入图片资源
	int len = strlen(fileName);
	if(len <= 5 || len >= MAX_NUMBER_LENGTH) return XFalse;
	char tempFileName[MAX_NUMBER_LENGTH];
	strcpy_s(tempFileName,MAX_NUMBER_LENGTH,fileName);
/*	fileName[len - 5] = '0';
	if(!m_texture00->load(fileName,1,m_resoursePosition)) return XFalse;
	fileName[len - 5] = '1';
	if(!m_texture01->load(fileName,1,m_resoursePosition)) return XFalse;
	fileName[len - 5] = '2';
	if(!m_texture02->load(fileName,1,m_resoursePosition)) return XFalse;
	fileName[len - 5] = '3';
	if(!m_texture03->load(fileName,1,m_resoursePosition)) return XFalse;
	fileName[len - 5] = '4';
	if(!m_texture04->load(fileName,1,m_resoursePosition)) return XFalse;
	fileName[len - 5] = '5';
	if(!m_texture05->load(fileName,1,m_resoursePosition)) return XFalse;
	fileName[len - 5] = '6';
	if(!m_texture06->load(fileName,1,m_resoursePosition)) return XFalse;
	fileName[len - 5] = '7';
	if(!m_texture07->load(fileName,1,m_resoursePosition)) return XFalse;
	fileName[len - 5] = '8';
	if(!m_texture08->load(fileName,1,m_resoursePosition)) return XFalse;
	fileName[len - 5] = '9';
	if(!m_texture09->load(fileName,1,m_resoursePosition)) return XFalse;
	fileName[len - 5] = 'a';
	if(!m_texture0a->load(fileName,1,m_resoursePosition)) return XFalse;
	fileName[len - 5] = 'b';
	if(!m_texture0b->load(fileName,1,m_resoursePosition)) return XFalse;
	fileName[len - 5] = 'c';
	if(!m_texture0c->load(fileName,1,m_resoursePosition)) return XFalse;
	fileName[len - 5] = 'd';
	if(!m_texture0d->load(fileName,1,m_resoursePosition)) return XFalse;
	fileName[len - 5] = 'e';
	if(!m_texture0e->load(fileName,1,m_resoursePosition)) return XFalse;
	fileName[len - 5] = 'f';
	if(!m_texture0f->load(fileName,1,m_resoursePosition)) return XFalse;
*/
	char& ref = tempFileName[len - 5];
	ref = '0';
	m_texture00->load(tempFileName,m_resoursePosition);
	ref = '1';
	m_texture01->load(tempFileName,m_resoursePosition);
	ref = '2';
	m_texture02->load(tempFileName,m_resoursePosition);
	ref = '3';
	m_texture03->load(tempFileName,m_resoursePosition);
	ref = '4';
	m_texture04->load(tempFileName,m_resoursePosition);
	ref = '5';
	m_texture05->load(tempFileName,m_resoursePosition);
	ref = '6';
	m_texture06->load(tempFileName,m_resoursePosition);
	ref = '7';
	m_texture07->load(tempFileName,m_resoursePosition);
	ref = '8';
	m_texture08->load(tempFileName,m_resoursePosition);
	ref = '9';
	m_texture09->load(tempFileName,m_resoursePosition);
	ref = 'a';
	m_texture0a->load(tempFileName,m_resoursePosition);
	ref = 'b';
	m_texture0b->load(tempFileName,m_resoursePosition);
	ref = 'c';
	m_texture0c->load(tempFileName,m_resoursePosition);
	ref = 'd';
	m_texture0d->load(tempFileName,m_resoursePosition);
	ref = 'e';
	m_texture0e->load(tempFileName,m_resoursePosition);
	ref = 'f';
	m_texture0f->load(tempFileName,m_resoursePosition);

	m_isInited = XTrue;
	return XTrue;
}
void XNumberTexture::release()
{
	if(!m_isInited) return;
	//m_texture00->release();
	//m_texture01->release();
	//m_texture02->release();
	//m_texture03->release();
	//m_texture04->release();
	//m_texture05->release();
	//m_texture06->release();
	//m_texture07->release();
	//m_texture08->release();
	//m_texture09->release();
	//m_texture0a->release();
	//m_texture0b->release();
	//m_texture0c->release();
	//m_texture0d->release();
	//m_texture0e->release();
	//m_texture0f->release();
	XMem::XDELETE(m_texture00);
	XMem::XDELETE(m_texture01);
	XMem::XDELETE(m_texture02);
	XMem::XDELETE(m_texture03);
	XMem::XDELETE(m_texture04);
	XMem::XDELETE(m_texture05);
	XMem::XDELETE(m_texture06);
	XMem::XDELETE(m_texture07);
	XMem::XDELETE(m_texture08);
	XMem::XDELETE(m_texture09);
	XMem::XDELETE(m_texture0a);
	XMem::XDELETE(m_texture0b);
	XMem::XDELETE(m_texture0c);
	XMem::XDELETE(m_texture0d);
	XMem::XDELETE(m_texture0e);
	XMem::XDELETE(m_texture0f);

	m_isInited = XFalse;
}
XNumberEx::XNumberEx()
:m_isInited(XFalse)
,m_rotateBasicPoint(0.0f)
//,m_blendTypeScr(4)
//,m_blendTypeDst(5)
,m_angleSin(0.0f)
,m_angleCos(1.0f)
{//字符串保存的时候使用倒序保存，但是符号使用正常的顺序例如：-12.5%，将会保存为-5.12%
//	m_number = XMem::createArrayMem<char>(MAX_NUMBER_LENGTH);
//	if(m_number == NULL)
//	{
//		printf("memory error!\n");
//	}
//	m_number[0] = '\0';
	m_number = XMem::createArrayMem<char>(MAX_NUMBER_LENGTH);
	m_number[0] = '\0';
}
XBool XNumberEx::init(const XNumberTexture *numberTexture,const XVec2& size)
{
	if(m_isInited ||
		numberTexture == NULL) return XFalse;
	m_numberTexture = numberTexture;

	m_number[0] = '\0';
	m_position.reset();
	m_setPosition.reset();
	setAngle(0);
	m_distance = 0;
	m_size = size;
	m_showSize.set(1.0);
//	if(!m_sprite.init(1,m_size.x,m_size.y,0,POINT_LEFT_TOP)) return XFalse;
	if(!m_sprite.init(m_numberTexture->m_texture00->texture.m_w,m_numberTexture->m_texture00->texture.m_h,
		0,POINT_LEFT_TOP)) return XFalse;
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);

	m_isInited = XTrue;
	return XTrue;
}
void XNumberEx::draw()
{
	if(!m_isInited) return;
//	if(m_isFlash != 0) m_sprite.m_isFlash = 1;
//	else m_sprite.m_isFlash = 0;
	float tempX = m_angleCos * (m_size.x + m_distance) * m_showSize.x;
	float tempY = m_angleSin * (m_size.x + m_distance) * m_showSize.x;

	for(int i = 0;i < MAX_NUMBER_LENGTH;++ i)
	{
		if(m_number[i] == '\0') break;
		m_sprite.setPosition(m_position.x + tempX * i,
			m_position.y + tempY * i);
		switch(m_number[i])
		{
		case '0':
			m_sprite.draw(m_numberTexture->m_texture00);
			break;
		case '1':
			m_sprite.draw(m_numberTexture->m_texture01);
			break;
		case '2':
			m_sprite.draw(m_numberTexture->m_texture02);
			break;
		case '3':
			m_sprite.draw(m_numberTexture->m_texture03);
			break;
		case '4':
			m_sprite.draw(m_numberTexture->m_texture04);
			break;
		case '5':
			m_sprite.draw(m_numberTexture->m_texture05);
			break;
		case '6':
			m_sprite.draw(m_numberTexture->m_texture06);
			break;
		case '7':
			m_sprite.draw(m_numberTexture->m_texture07);
			break;
		case '8':
			m_sprite.draw(m_numberTexture->m_texture08);
			break;
		case '9':
			m_sprite.draw(m_numberTexture->m_texture09);
			break;
		case '+':
			m_sprite.draw(m_numberTexture->m_texture0a);
			break;
		case '-':
			m_sprite.draw(m_numberTexture->m_texture0b);
			break;
		case '*':
			m_sprite.draw(m_numberTexture->m_texture0c);
			break;
		case '/':
			m_sprite.draw(m_numberTexture->m_texture0d);
			break;
		case '%':
			m_sprite.draw(m_numberTexture->m_texture0e);
			break;
		case '.':
			m_sprite.draw(m_numberTexture->m_texture0f);
			break;
		default:
			continue;
			break;
		}
	}
}
XBool XNumberEx::setNumber(int temp)
{
	char tempNumber[MAX_NUMBER_LENGTH];
	int curCharPoint = 0;
	if(temp < 0) 
	{
		tempNumber[curCharPoint] = '-';
		++ curCharPoint;
		temp = -temp;
	}
	while(true)
	{
		tempNumber[curCharPoint] = temp%10 + '0';
		temp = temp / 10.0f;
		++ curCharPoint;
		if(curCharPoint >= MAX_NUMBER_LENGTH) return XFalse;
		if(temp == 0) break;
	}
	int i = 0;
	if(tempNumber[0] < '0' || tempNumber[0] > '9')
	{//如果是符号位则不变
		m_number[0] = tempNumber[0];
		for(i = 1;i < curCharPoint;++i)
		{
			m_number[i] = tempNumber[curCharPoint - i];
		}
	}else
	{
		for(i = 0;i < curCharPoint;++i)
		{
			m_number[i] = tempNumber[curCharPoint - i - 1];
		}
	}
	m_number[curCharPoint] = '\0';
	return XTrue;
}
XBool XNumberEx::setNumber(float temp,int decimalPartLength)
{
	char tempNumber[MAX_NUMBER_LENGTH];
	if(decimalPartLength < 0) decimalPartLength = 0;
	int tempZ = 0;
	float tempBack = temp;
	for(int i = 0;i < decimalPartLength;++ i)
	{
		tempBack *= 10.0;
	}
	tempZ = tempBack;
	int curCharPoint = 0;
	if(tempZ < 0) 
	{
		tempNumber[curCharPoint] = '-';
		++ curCharPoint;
		tempZ = -tempZ;
	}

	//如果这个数的整数部分为0则需要在最后在字符串的前面加一个0
	char flag = 0;
	if(temp < 1.0 && temp > -1.0 && tempZ != 0)
	{//如果整数部分是0，则需要添加这个
		flag = 1;
	}

	int sumLength = 0;
	while(true)
	{
		tempNumber[curCharPoint] = tempZ%10 + '0';
		tempZ = tempZ / 10.0f;
		++ curCharPoint;
		++ sumLength;
		if(curCharPoint >= MAX_NUMBER_LENGTH) return XFalse;
		if(sumLength == decimalPartLength && decimalPartLength > 0)
		{
			tempNumber[curCharPoint] = '.';
			++ curCharPoint;
		}
		if(tempZ == 0) break;
	}

	//如果这个数的整数部分为0则需要在最后在字符串的前面加一个0
	if(flag == 1)
	{
		tempNumber[curCharPoint] = '0';
		++ curCharPoint;
	}

	int i = 0;
	if(tempNumber[0] < '0' || tempNumber[0] > '9')
	{//如果是符号位则不变
		m_number[0] = tempNumber[0];
		for(i = 1;i < curCharPoint;++i)
		{
			m_number[i] = tempNumber[curCharPoint - i];
		}
	}else
	{
		for(i = 0;i < curCharPoint;++i)
		{
			m_number[i] = tempNumber[curCharPoint - i - 1];
		}
	}
	m_number[curCharPoint] = '\0';
	return XTrue;
}
XBool XNumberEx::setACopy(const XNumberEx &temp)
{
	if(& temp == this ||
		!temp.m_isInited) return XFalse;	//如果目标没有初始化则直接退出

	m_angleSin = temp.m_angleSin;
	m_angleCos = temp.m_angleCos;

//	m_blendTypeScr = temp.m_blendTypeScr;
//	m_blendTypeDst = temp.m_blendTypeDst;
	m_numberTexture = temp.m_numberTexture;
	strcpy_s(m_number,MAX_NUMBER_LENGTH,temp.m_number);
	m_position = temp.m_position;	//字体的位置
	m_setPosition = temp.m_setPosition;
	m_angle = temp.m_angle;			//字体的角度
	m_distance = temp.m_distance;		//文字之间的距离	
	m_size = temp.m_size;		//文字的尺寸（像素大小）
	m_showSize = temp.m_showSize;
	m_rotateBasicPoint = temp.m_rotateBasicPoint;
	m_sprite.setACopy(temp.m_sprite);					//贴图
	m_isInited = temp.m_isInited;
	return XTrue;
}
XNumberEx& XNumberEx::operator = (const XNumberEx& temp)
{
	if(& temp == this ||
		!temp.m_isInited) return * this;	//如果目标没有初始化则直接退出

	m_angleSin = temp.m_angleSin;
	m_angleCos = temp.m_angleCos;

//	m_blendTypeScr = temp.m_blendTypeScr;
//	m_blendTypeDst = temp.m_blendTypeDst;
	m_numberTexture = temp.m_numberTexture;
	strcpy_s(m_number,MAX_NUMBER_LENGTH,temp.m_number);
	m_position = temp.m_position;	//字体的位置
	m_setPosition = temp.m_setPosition;
	m_angle = temp.m_angle;			//字体的角度
	m_distance = temp.m_distance;		//文字之间的距离	
	m_size = temp.m_size;		//文字的尺寸（像素大小）
	m_showSize = temp.m_showSize;
	m_rotateBasicPoint = temp.m_rotateBasicPoint;
	m_sprite.setACopy(temp.m_sprite);					//贴图
	m_isInited = temp.m_isInited;
	return * this;
}
#if !WITH_INLINE_FILE
#include "XNumberEx.inl"
#endif
}