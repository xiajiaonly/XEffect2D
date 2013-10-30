//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XNumberEx.h"
#include "XResourcePack.h"

_XNumberTexture::_XNumberTexture()
:m_isInited(0)
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
{
}
//fileName:pic/number_00/number_00.png
int _XNumberTexture::init(const char *fileName,int resoursePosition)
{
	if(m_isInited != 0) return 1;
	if(fileName == NULL) return 0;
	if(resoursePosition != RESOURCE_OUTSIDE)
	{
		resoursePosition = RESOURCE_INSIDE;
	}
	m_resoursePosition = resoursePosition;
	//下面开始分配内存空间
	try
	{
		m_texture00 = new _XTextureData;
		if(m_texture00 == NULL) return 0;
		m_texture01 = new _XTextureData;
		if(m_texture01 == NULL) return 0;
		m_texture02 = new _XTextureData;
		if(m_texture02 == NULL) return 0;
		m_texture03 = new _XTextureData;
		if(m_texture03 == NULL) return 0;
		m_texture04 = new _XTextureData;
		if(m_texture04 == NULL) return 0;
		m_texture05 = new _XTextureData;
		if(m_texture05 == NULL) return 0;
		m_texture06 = new _XTextureData;
		if(m_texture06 == NULL) return 0;
		m_texture07 = new _XTextureData;
		if(m_texture07 == NULL) return 0;
		m_texture08 = new _XTextureData;
		if(m_texture08 == NULL) return 0;
		m_texture09 = new _XTextureData;
		if(m_texture09 == NULL) return 0;
		m_texture0a = new _XTextureData;
		if(m_texture0a == NULL) return 0;
		m_texture0b = new _XTextureData;
		if(m_texture0b == NULL) return 0;
		m_texture0c = new _XTextureData;
		if(m_texture0c == NULL) return 0;
		m_texture0d = new _XTextureData;
		if(m_texture0d == NULL) return 0;
		m_texture0e = new _XTextureData;
		if(m_texture0e == NULL) return 0;
		m_texture0f = new _XTextureData;
		if(m_texture0f == NULL) return 0;
	}catch(...)
	{
		return 0;
	}
	//载入图片资源
	int len = strlen(fileName);
	if(len <= 0 || len >= MAX_NUMBER_LENGTH) return 0;
	char tempFileName[MAX_NUMBER_LENGTH];
	strcpy(tempFileName,fileName);
/*	fileName[len - 5] = '0';
	if(m_texture00->load(fileName,1,m_resoursePosition) == 0) return 0;
	fileName[len - 5] = '1';
	if(m_texture01->load(fileName,1,m_resoursePosition) == 0) return 0;
	fileName[len - 5] = '2';
	if(m_texture02->load(fileName,1,m_resoursePosition) == 0) return 0;
	fileName[len - 5] = '3';
	if(m_texture03->load(fileName,1,m_resoursePosition) == 0) return 0;
	fileName[len - 5] = '4';
	if(m_texture04->load(fileName,1,m_resoursePosition) == 0) return 0;
	fileName[len - 5] = '5';
	if(m_texture05->load(fileName,1,m_resoursePosition) == 0) return 0;
	fileName[len - 5] = '6';
	if(m_texture06->load(fileName,1,m_resoursePosition) == 0) return 0;
	fileName[len - 5] = '7';
	if(m_texture07->load(fileName,1,m_resoursePosition) == 0) return 0;
	fileName[len - 5] = '8';
	if(m_texture08->load(fileName,1,m_resoursePosition) == 0) return 0;
	fileName[len - 5] = '9';
	if(m_texture09->load(fileName,1,m_resoursePosition) == 0) return 0;
	fileName[len - 5] = 'a';
	if(m_texture0a->load(fileName,1,m_resoursePosition) == 0) return 0;
	fileName[len - 5] = 'b';
	if(m_texture0b->load(fileName,1,m_resoursePosition) == 0) return 0;
	fileName[len - 5] = 'c';
	if(m_texture0c->load(fileName,1,m_resoursePosition) == 0) return 0;
	fileName[len - 5] = 'd';
	if(m_texture0d->load(fileName,1,m_resoursePosition) == 0) return 0;
	fileName[len - 5] = 'e';
	if(m_texture0e->load(fileName,1,m_resoursePosition) == 0) return 0;
	fileName[len - 5] = 'f';
	if(m_texture0f->load(fileName,1,m_resoursePosition) == 0) return 0;
*/
	tempFileName[len - 5] = '0';
	m_texture00->load(tempFileName,m_resoursePosition);
	tempFileName[len - 5] = '1';
	m_texture01->load(tempFileName,m_resoursePosition);
	tempFileName[len - 5] = '2';
	m_texture02->load(tempFileName,m_resoursePosition);
	tempFileName[len - 5] = '3';
	m_texture03->load(tempFileName,m_resoursePosition);
	tempFileName[len - 5] = '4';
	m_texture04->load(tempFileName,m_resoursePosition);
	tempFileName[len - 5] = '5';
	m_texture05->load(tempFileName,m_resoursePosition);
	tempFileName[len - 5] = '6';
	m_texture06->load(tempFileName,m_resoursePosition);
	tempFileName[len - 5] = '7';
	m_texture07->load(tempFileName,m_resoursePosition);
	tempFileName[len - 5] = '8';
	m_texture08->load(tempFileName,m_resoursePosition);
	tempFileName[len - 5] = '9';
	m_texture09->load(tempFileName,m_resoursePosition);
	tempFileName[len - 5] = 'a';
	m_texture0a->load(tempFileName,m_resoursePosition);
	tempFileName[len - 5] = 'b';
	m_texture0b->load(tempFileName,m_resoursePosition);
	tempFileName[len - 5] = 'c';
	m_texture0c->load(tempFileName,m_resoursePosition);
	tempFileName[len - 5] = 'd';
	m_texture0d->load(tempFileName,m_resoursePosition);
	tempFileName[len - 5] = 'e';
	m_texture0e->load(tempFileName,m_resoursePosition);
	tempFileName[len - 5] = 'f';
	m_texture0f->load(tempFileName,m_resoursePosition);

	m_isInited = 1;
	return 1;
}

void _XNumberTexture::release()
{
	if(m_isInited == 0) return;
	m_texture00->release();
	m_texture01->release();
	m_texture02->release();
	m_texture03->release();
	m_texture04->release();
	m_texture05->release();
	m_texture06->release();
	m_texture07->release();
	m_texture08->release();
	m_texture09->release();
	m_texture0a->release();
	m_texture0b->release();
	m_texture0c->release();
	m_texture0d->release();
	m_texture0e->release();
	m_texture0f->release();
	XDELETE(m_texture00);
	XDELETE(m_texture01);
	XDELETE(m_texture02);
	XDELETE(m_texture03);
	XDELETE(m_texture04);
	XDELETE(m_texture05);
	XDELETE(m_texture06);
	XDELETE(m_texture07);
	XDELETE(m_texture08);
	XDELETE(m_texture09);
	XDELETE(m_texture0a);
	XDELETE(m_texture0b);
	XDELETE(m_texture0c);
	XDELETE(m_texture0d);
	XDELETE(m_texture0e);
	XDELETE(m_texture0f);

	m_isInited = 0;
}

_XNumberEx::_XNumberEx()
:m_isInited(0)
,m_rotateBasicPoint(0.0f,0.0f)
,m_blendTypeScr(4)
,m_blendTypeDst(5)
,m_angleSin(0.0f)
,m_angleCos(1.0f)
{//字符串保存的时候使用倒序保存，但是符号使用正常的顺序例如：-12.5%，将会保存为-5.12%
	//try
	//{
	//	m_number = NULL;
	//	m_number = new char[MAX_NUMBER_LENGTH];
	//	if(m_number == NULL)
	//	{
	//		printf("memory error!\n");
	//	}
	//	m_number[0] = '\0';
	//}catch(...)
	//{
	//	printf("memory error!\n");
	//}
	m_number = new char[MAX_NUMBER_LENGTH];
	m_number[0] = '\0';
}

int _XNumberEx::init(const _XNumberTexture *numberTexture,_XVector2I size)
{
	if(m_isInited != 0) return 0;
	if(numberTexture == NULL) return 0;
	m_numberTexture = numberTexture;

	m_number[0] = '\0';
	m_position.set(0.0f,0.0f);
	m_setPosition.set(0.0f,0.0f);
	setAngle(0);
	m_distance = 0;
	m_size = size;
	m_showSize.set(1.0,1.0);
//	if(m_sprite.init(1,m_size.x,m_size.y,0,POINT_LEFT_TOP) == 0) return 0;
	if(m_sprite.init(m_numberTexture->m_texture00->texture.m_w,m_numberTexture->m_texture00->texture.m_h,
		0,POINT_LEFT_TOP) == 0) return 0;
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);

	m_isInited = 1;
	return 1;
}

void _XNumberEx::draw()
{
	if(m_isInited == 0) return;
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

int _XNumberEx::setNumber(int temp)
{
	char tempNumber[MAX_NUMBER_LENGTH];
	int nowCharPoint = 0;
	if(temp < 0) 
	{
		tempNumber[nowCharPoint] = '-';
		++ nowCharPoint;
		temp = -temp;
	}
	while(1)
	{
		tempNumber[nowCharPoint] = temp%10 + '0';
		temp = temp / 10.0f;
		++ nowCharPoint;
		if(nowCharPoint >= MAX_NUMBER_LENGTH) return 0;
		if(temp == 0) break;
	}
	int i = 0;;
	if(tempNumber[0] < '0' || tempNumber[0] > '9')
	{//如果是符号位则不变
		m_number[0] = tempNumber[0];
		for(i = 1;i < nowCharPoint;++i)
		{
			m_number[i] = tempNumber[nowCharPoint - i];
		}
	}else
	{
		for(i = 0;i < nowCharPoint;++i)
		{
			m_number[i] = tempNumber[nowCharPoint - i - 1];
		}
	}
	m_number[nowCharPoint] = '\0';
	return 1;
}

int _XNumberEx::setNumber(float temp,int decimalPartLength)
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
	int nowCharPoint = 0;
	if(tempZ < 0) 
	{
		tempNumber[nowCharPoint] = '-';
		++ nowCharPoint;
		tempZ = -tempZ;
	}

	//如果这个数的整数部分为0则需要在最后在字符串的前面加一个0
	char flag = 0;
	if(temp < 1.0 && temp > -1.0 && tempZ != 0)
	{//如果整数部分是0，则需要添加这个
		flag = 1;
	}

	int sumLength = 0;
	while(1)
	{
		tempNumber[nowCharPoint] = tempZ%10 + '0';
		tempZ = tempZ / 10.0f;
		++ nowCharPoint;
		++ sumLength;
		if(nowCharPoint >= MAX_NUMBER_LENGTH) return 0;
		if(sumLength == decimalPartLength && decimalPartLength > 0)
		{
			tempNumber[nowCharPoint] = '.';
			++ nowCharPoint;
		}
		if(tempZ == 0) break;
	}

	//如果这个数的整数部分为0则需要在最后在字符串的前面加一个0
	if(flag == 1)
	{
		tempNumber[nowCharPoint] = '0';
		++ nowCharPoint;
	}

	int i = 0;
	if(tempNumber[0] < '0' || tempNumber[0] > '9')
	{//如果是符号位则不变
		m_number[0] = tempNumber[0];
		for(i = 1;i < nowCharPoint;++i)
		{
			m_number[i] = tempNumber[nowCharPoint - i];
		}
	}else
	{
		for(i = 0;i < nowCharPoint;++i)
		{
			m_number[i] = tempNumber[nowCharPoint - i - 1];
		}
	}
	m_number[nowCharPoint] = '\0';
	return 1;
}

int _XNumberEx::setACopy(const _XNumberEx &temp)
{
	if(& temp == this) return 1;	//防止自身赋值
	if(temp.m_isInited == 0) return 0;	//如果目标没有初始化则直接退出

	m_angleSin = temp.m_angleSin;
	m_angleCos = temp.m_angleCos;

	m_blendTypeScr = temp.m_blendTypeScr;
	m_blendTypeDst = temp.m_blendTypeDst;
	m_numberTexture = temp.m_numberTexture;
	strcpy(m_number,temp.m_number);
	m_position = temp.m_position;	//字体的位置
	m_setPosition = temp.m_setPosition;
	m_angle = temp.m_angle;			//字体的角度
	m_distance = temp.m_distance;		//文字之间的距离	
	m_size = temp.m_size;		//文字的尺寸（像素大小）
	m_showSize = temp.m_showSize;
	m_rotateBasicPoint = temp.m_rotateBasicPoint;
	m_sprite.setACopy(temp.m_sprite);					//贴图
	m_isInited = temp.m_isInited;
	return 1;
}

_XNumberEx& _XNumberEx::operator = (const _XNumberEx& temp)
{
	if(& temp == this) return * this;	//防止自身赋值
	if(temp.m_isInited == 0) return * this;	//如果目标没有初始化则直接退出

	m_angleSin = temp.m_angleSin;
	m_angleCos = temp.m_angleCos;

	m_blendTypeScr = temp.m_blendTypeScr;
	m_blendTypeDst = temp.m_blendTypeDst;
	m_numberTexture = temp.m_numberTexture;
	strcpy(m_number,temp.m_number);
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

int _XMoveNumber::move(int delay)
{
	float tempSum;
	if((int)(m_armNumber) != m_nowNumber)
	{
		tempSum = m_armNumber - m_nowNumber;
		if(0.0075f * delay < 1.0f)	//变化的比例不能超过1，否则就会放大了
		{
			tempSum *= 0.0075f * delay;
		}
		if(tempSum < 1.0f && tempSum > -1.0f)	//最小的变化值不能太小
		{
			if(tempSum < 0) tempSum = -1.0f;
			else tempSum = 1.0f;
		}
		m_nowNumber += tempSum;
		return 1;
	}
	return 0;
}

void _XMoveData::move(int delay)
{
	if(m_isEnd == 0)
	{
		m_timer += 0.01f * delay * m_speed;
		if(m_isLoop == 0)
		{//如果不循环
			if(m_timer >= 1.0f)
			{
				m_isEnd = 1;
				m_timer = 1.0f;
				m_nowData = m_endData;
			}else
			{
				if(m_mode == MOVE_DATA_MODE_LINE)
				{
					m_nowData = m_startData + (m_endData - m_startData) * m_timer;
				}else
				if(m_mode == MOVE_DATA_MODE_SIN)
				{
					m_nowData = m_startData + (m_endData - m_startData) * sin(m_timer * PI_HALF);
				}else
				if(m_mode == MOVE_DATA_MODE_SIN_MULT)
				{
					m_nowData = m_startData + (m_endData - m_startData) * (1.0f - (m_timer - 1.0f) * (m_timer - 1.0f));
				}else
				if(m_mode == MOVE_DATA_MODE_SQRT2)
				{
					m_nowData = m_startData + (m_endData - m_startData) * pow(m_timer,0.25f);
				}else
				if(m_mode == MOVE_DATA_MODE_SQRT2_MULT)
				{
					m_nowData = m_startData + (m_endData - m_startData) * (1.0f - (m_timer - 1.0f) * (m_timer - 1.0f) * (m_timer - 1.0f) * (m_timer - 1.0f));
				}else
				if(m_mode == MOVE_DATA_MODE_COS)
				{
					m_nowData = m_startData + (m_endData - m_startData) * (1.0f - cos(m_timer * PI_HALF));
				}else
				if(m_mode == MOVE_DATA_MODE_COS_MULT)
				{
					m_nowData = m_startData + (m_endData - m_startData) * m_timer * m_timer;
				}else
				if(m_mode == MOVE_DATA_MODE_DSIN)
				{
					if(m_timer < 0.5f)
					{
						m_nowData = m_startData + (m_endData - m_startData) * sin(m_timer * PI) * 0.5f;
					}else
					{
						m_nowData = m_startData + (m_endData - m_startData) * (2.0f - sin(m_timer * PI)) * 0.5f;
					}
				}else
				if(m_mode == MOVE_DATA_MODE_DCOS)
				{
					m_nowData = m_startData + (m_endData - m_startData) * (1.0f - cos(m_timer * PI)) * 0.5f;
				}else
				if(m_mode == MOVE_DATA_MODE_DSIN_MULT)
				{
					if(m_timer < 0.5f)
					{
						m_nowData = m_startData + (m_endData - m_startData) * (0.25f - (m_timer - 0.5f) * (m_timer - 0.5f)) * 2.0f;
					}else
					{
						m_nowData = m_startData + (m_endData - m_startData) * (0.5f + (m_timer - 0.5f) * (m_timer - 0.5f) * 2.0f);
					}
				}else
				if(m_mode == MOVE_DATA_MODE_DCOS_MULT)
				{
					if(m_timer < 0.5f)
					{
						m_nowData = m_startData + (m_endData - m_startData) * m_timer * m_timer * 2.0f;
					}else
					{
						m_nowData = m_startData + (m_endData - m_startData) * (1.0f - (m_timer - 1.0f) * (m_timer - 1.0f) * 2.0f);
					}
				}
			}
		}else
		{//如果循环
			if(m_timer >= 2.0f)
			{
				m_timer = 0.0f;
				m_nowData = m_startData;
				if(m_loopTimer > 0)
				{
					-- m_loopTimer;
					if(m_loopTimer == 0)
					{
						m_isEnd = 1;
					}
				}
			}else
			{
				if(m_timer >= 1.0f)
				{
					if(m_mode == MOVE_DATA_MODE_LINE)
					{
						m_nowData = m_startData + (m_endData - m_startData) * (2.0f - m_timer);
					}else
					if(m_mode == MOVE_DATA_MODE_SIN)
					{
						m_nowData = m_startData + (m_endData - m_startData) * sin((2.0f - m_timer) * PI_HALF);
					}else
					if(m_mode == MOVE_DATA_MODE_SIN_MULT)
					{
						m_nowData = m_startData + (m_endData - m_startData) * (1.0f - (m_timer - 1.0f) * (m_timer - 1.0f));
					}else
					if(m_mode == MOVE_DATA_MODE_SQRT2)
					{
						m_nowData = m_startData + (m_endData - m_startData) * pow(2.0f - m_timer,0.25f);
					}else
					if(m_mode == MOVE_DATA_MODE_SQRT2_MULT)
					{
						m_nowData = m_startData + (m_endData - m_startData) * (1.0f - (m_timer - 1.0f) * (m_timer - 1.0f) * (m_timer - 1.0f) * (m_timer - 1.0f));
					}else
					if(m_mode == MOVE_DATA_MODE_DSIN)
					{
						if(m_timer < 1.5f)
						{
							m_nowData = m_startData + (m_endData - m_startData) * (2.0f - sin((2.0f - m_timer) * PI)) * 0.5f;
						}else
						{
							m_nowData = m_startData + (m_endData - m_startData) * sin((2.0f - m_timer) * PI) * 0.5f;
						}
					}else
					if(m_mode == MOVE_DATA_MODE_COS)
					{
						m_nowData = m_startData + (m_endData - m_startData) * (1.0f - cos((2.0f - m_timer) * PI_HALF));
					}else
					if(m_mode == MOVE_DATA_MODE_COS_MULT)
					{
						m_nowData = m_startData + (m_endData - m_startData) * (2.0f - m_timer) * (2.0f - m_timer);
					}else
					if(m_mode == MOVE_DATA_MODE_DCOS)
					{
						m_nowData = m_startData + (m_endData - m_startData) * (1.0f - cos((2.0f - m_timer) * PI)) * 0.5f;
					}else
					if(m_mode == MOVE_DATA_MODE_DSIN_MULT)
					{
						if(m_timer < 1.5f)
						{
							m_nowData = m_startData + (m_endData - m_startData) * (0.5f + (m_timer - 1.5f) * (m_timer - 1.5f) * 2.0f);
						}else
						{
							m_nowData = m_startData + (m_endData - m_startData) * (0.25f - (m_timer - 1.5f) * (m_timer - 1.5f)) * 2.0f;
						}
					}else
					if(m_mode == MOVE_DATA_MODE_DCOS_MULT)
					{
						if(m_timer < 1.5f)
						{
							m_nowData = m_startData + (m_endData - m_startData) * (1.0f - (m_timer - 1.0f) * (m_timer - 1.0f) * 2.0f);
						}else
						{
							m_nowData = m_startData + (m_endData - m_startData) * (m_timer - 2.0f) * (m_timer - 2.0f) * 2.0f;
						}
					}
				}else
				{
					if(m_mode == MOVE_DATA_MODE_LINE)
					{
						m_nowData = m_startData + (m_endData - m_startData) * m_timer;
					}else
					if(m_mode == MOVE_DATA_MODE_SIN)
					{
						m_nowData = m_startData + (m_endData - m_startData) * sin(m_timer * PI_HALF);
					}else
					if(m_mode == MOVE_DATA_MODE_SIN_MULT)
					{
						m_nowData = m_startData + (m_endData - m_startData) * (1.0f - (m_timer - 1.0f) * (m_timer - 1.0f));
					}else
					if(m_mode == MOVE_DATA_MODE_SQRT2)
					{
						m_nowData = m_startData + (m_endData - m_startData) * pow(m_timer,0.25f);
					}else
					if(m_mode == MOVE_DATA_MODE_SQRT2_MULT)
					{
						m_nowData = m_startData + (m_endData - m_startData) * (1.0f - (m_timer - 1.0f) * (m_timer - 1.0f) * (m_timer - 1.0f) * (m_timer - 1.0f));
					}else
					if(m_mode == MOVE_DATA_MODE_DSIN)
					{
						if(m_timer < 0.5f)
						{
							m_nowData = m_startData + (m_endData - m_startData) * sin(m_timer * PI) * 0.5f;
						}else
						{
							m_nowData = m_startData + (m_endData - m_startData) * (2.0f - sin(m_timer * PI)) * 0.5f;
						}
					}else
					if(m_mode == MOVE_DATA_MODE_COS)
					{
						m_nowData = m_startData + (m_endData - m_startData) * (1.0f - cos(m_timer * PI_HALF));
					}else
					if(m_mode == MOVE_DATA_MODE_COS_MULT)
					{
						m_nowData = m_startData + (m_endData - m_startData) * m_timer * m_timer;
					}else
					if(m_mode == MOVE_DATA_MODE_DCOS)
					{
						m_nowData = m_startData + (m_endData - m_startData) * (1.0f - cos(m_timer * PI)) * 0.5f;
					}else
					if(m_mode == MOVE_DATA_MODE_DSIN_MULT)
					{
						if(m_timer < 0.5f)
						{
							m_nowData = m_startData + (m_endData - m_startData) * (0.25f - (m_timer - 0.5f) * (m_timer - 0.5f)) * 2.0f;
						}else
						{
							m_nowData = m_startData + (m_endData - m_startData) * (0.5f + (m_timer - 0.5f) * (m_timer - 0.5f) * 2.0f);
						}
					}else
					if(m_mode == MOVE_DATA_MODE_DCOS_MULT)
					{
						if(m_timer < 0.5f)
						{
							m_nowData = m_startData + (m_endData - m_startData) * m_timer * m_timer * 2.0f;
						}else
						{
							m_nowData = m_startData + (m_endData - m_startData) * (1.0f - (m_timer - 1.0f) * (m_timer - 1.0f) * 2.0f);
						}
					}
				}
			}
		}
	}
}

