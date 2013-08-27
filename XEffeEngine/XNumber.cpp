//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XNumber.h"
#include "XResourcePack.h"
#include "XBasicSDL.h"
#include "XObjectManager.h"

_XNumber::_XNumber()
:m_isInited(0)
,m_rotateBasicPoint(0.0f,0.0f)
,m_angleSin(0.0f)
,m_angleCos(1.0f)
,m_needUpdateData(0)
,m_needShowTextSum(0)
,m_maxPixelWidth(0)
,m_maxPixelHeight(0)
,m_alignmentMode(_NUMBER_ALIGNMENT_MODE_LEFT)
,m_isVisiable(1)
{//字符串保存的时候使用倒序保存，但是符号使用正常的顺序例如：-12.5%，将会保存为-5.12%
	m_string = new char[MAX_NUMBER_LENGTH];
	m_string[0] = '\0';
	m_textPosition = new _XVector2[MAX_NUMBER_LENGTH];
	m_textRect = new _XRect[MAX_NUMBER_LENGTH];
}

_XNumber::~_XNumber()
{
	release();
	m_isInited = 0;
}

int _XNumber::release()
{
	if(m_isInited == 0) return 0;
	XDELETE_ARRAY(m_string);
	XDELETE_ARRAY(m_textPosition);
	XDELETE_ARRAY(m_textRect);
	m_sprite.release();
#if WITH_OBJECT_MANAGER	//在物件管理的类中注销这些物件
	_XObjectManager::GetInstance().decreaseAObject(this);
#endif
	m_isInited = 0;
	return 1;
}

int _XNumber::init(const char *fileName,_XVector2I size,_XVector2I layout,int resoursePosition)
{
	if(m_isInited != 0) return 0;

	if(resoursePosition != RESOURCE_OUTSIDE)
	{
		resoursePosition = RESOURCE_INSIDE;
	}
	m_resoursePosition = resoursePosition;

	m_string[0] = '\0';
	m_position.set(0.0f,0.0f);
	m_setPosition.set(0.0f,0.0f);
	setAngle(0);
	m_alpha = 1.0f;
	m_distance = 0;
	m_size = size;
	if(layout.x <= 0 || layout.y <= 0) return 0;
	m_layout = layout;
	m_showSize.set(1.0,1.0);
	if(m_sprite.init(fileName,m_resoursePosition,POINT_LEFT_TOP) == 0) return 0;
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);
#if WITH_OBJECT_MANAGER	//在物件管理的类中注销这些物件
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
	_XObjectManager::GetInstance().addAObject(this,OBJ_NUMBER);
#endif

	m_isInited = 1;
	return 1;
}

void _XNumber::draw()
{
	if(m_isVisiable == 0) return;
	if(m_isInited == 0) return;
	if(m_needUpdateData != 0)
	{
		m_needUpdateData = 0;
		updateData();
	}
	for(int i = 0;i < m_needShowTextSum;++ i)
	{
		m_sprite.setClipRect(m_textRect[i]);
		m_sprite.setPosition(m_textPosition[i]);
		m_sprite.draw();
	}
}

void _XNumber::updateData()
{
	m_needShowTextSum = 0;
	int now_x = 0;
	int now_y = 0;
	m_maxPixelWidth = 0;
	m_maxPixelHeight = 0;
	float textPixelX = (m_size.x + m_distance) * m_showSize.x;
	for(int i = 0;i < MAX_NUMBER_LENGTH;++ i)
	{
		if(m_string[i] == '\0') break;
		if(m_string[i] < '0' || m_string[i] > '9')
		{//字符
			switch(m_string[i])
			{
			case '+':
				now_x = 10;
				break;
			case '-':
				now_x = 11;
				break;
			case '*':
				now_x = 12;
				break;
			case '/':
				now_x = 13;
				break;
			case '%':
				now_x = 14;
				break;
			case '.':
				now_x = 15;
				break;
			default:
				continue;
				break;
			}
			if(m_layout.y == 0) 
			{
				now_y = 0;
			}else
			{
				now_y = (now_x / m_layout.x) * m_size.y;
			}
			now_x = (now_x % m_layout.x) * m_size.x;
		}else
		{//数字
			now_x = ((m_string[i] - '0') % m_layout.x) * m_size.x;
			if(m_layout.y == 0) 
			{
				now_y = 0;
			}else
			{
				now_y = ((m_string[i] - '0') / m_layout.x) * m_size.y;
			}
		}

		m_textRect[m_needShowTextSum].set(now_x,now_y,now_x + m_size.x,now_y + m_size.y);
		m_textPosition[m_needShowTextSum].set(m_position.x + m_angleCos * textPixelX * i,
			m_position.y + m_angleSin * textPixelX * i);
		m_needShowTextSum ++;

		if(textPixelX * (i + 1) > m_maxPixelWidth)
		{
			m_maxPixelWidth = textPixelX * (i + 1);
		}
	}
	m_maxPixelHeight = m_size.y * m_showSize.y;
	if(m_alignmentMode == _NUMBER_ALIGNMENT_MODE_LEFT)
	{//左对齐，啥也不用做
	}else
	if(m_alignmentMode == _NUMBER_ALIGNMENT_MODE_MIDDLE)
	{//居中，每个文字左移半个宽度
		for(int i = 0;i < m_needShowTextSum;++ i)
		{
			m_textPosition[i].x -= m_maxPixelWidth * 0.5f * m_angleCos;
			m_textPosition[i].y -= m_maxPixelWidth * 0.5f * m_angleSin;
		}
	}else
	if(m_alignmentMode == _NUMBER_ALIGNMENT_MODE_RIGHT)
	{//右对齐，每个文字左移整个宽度
		for(int i = 0;i < m_needShowTextSum;++ i)
		{
			m_textPosition[i].x -= m_maxPixelWidth * m_angleCos;
			m_textPosition[i].y -= m_maxPixelWidth * m_angleSin;
		}
	}
}

int _XNumber::setNumber(int temp)
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
		m_string[0] = tempNumber[0];
		for(i = 1;i < nowCharPoint;++i)
		{
			m_string[i] = tempNumber[nowCharPoint - i];
		}
	}else
	{
		for(i = 0;i < nowCharPoint;++i)
		{
			m_string[i] = tempNumber[nowCharPoint - i - 1];
		}
	}
	m_string[nowCharPoint] = '\0';

	m_needUpdateData = 1;
	return 1;
}

int _XNumber::setNumber(float temp,int decimalPartLength)
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
		m_string[0] = tempNumber[0];
		for(i = 1;i < nowCharPoint;++i)
		{
			m_string[i] = tempNumber[nowCharPoint - i];
		}
	}else
	{
		for(i = 0;i < nowCharPoint;++i)
		{
			m_string[i] = tempNumber[nowCharPoint - i - 1];
		}
	}
	m_string[nowCharPoint] = '\0';

	m_needUpdateData = 1;
	return 1;
}

int _XNumber::setACopy(const _XNumber &temp)
{
	if(& temp == this) return 1;	//防止自身赋值
	if(temp.m_isInited == 0) return 0;	//如果目标没有初始化则直接退出
	//if(m_isInited != 0) 
	//{
	//	m_sprite.release()
	//	//return 0;	//如果自身已经初始化则必须要退出，否则将会造成内存泄露
	//}
	m_isVisiable = temp.m_isVisiable;
	m_alignmentMode = temp.m_alignmentMode;

	m_maxPixelWidth = temp.m_maxPixelWidth;
	m_maxPixelHeight = temp.m_maxPixelHeight;
	m_needUpdateData = temp.m_needUpdateData;
	m_needShowTextSum = temp.m_needShowTextSum;
	for(int i = 0;i < MAX_NUMBER_LENGTH;++ i)
	{
		m_textPosition[i] = temp.m_textPosition[i];
		m_textRect[i] = temp.m_textRect[i];
	}

	strcpy(m_string,temp.m_string);
	m_position = temp.m_position;	//字体的位置
	m_setPosition = temp.m_setPosition;	//字体的位置
	m_angle = temp.m_angle;			//字体的角度
	m_angleSin = temp.m_angleSin;			//字体的角度
	m_angleCos = temp.m_angleCos;			//字体的角度
	m_alpha = temp.m_alpha;
	m_distance = temp.m_distance;		//文字之间的距离	
	m_size = temp.m_size;		//文字的尺寸（像素大小）
	m_layout = temp.m_layout;		//字体图片的布局
	m_showSize = temp.m_showSize;
	m_rotateBasicPoint = temp.m_rotateBasicPoint;
	m_sprite.setACopy(temp.m_sprite);					//贴图
#if WITH_OBJECT_MANAGER	//在物件管理的类中注销这些物件
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
	_XObjectManager::GetInstance().addAObject(this,OBJ_NUMBER);	//addAObject内部已经避免了重复注册的问题，外部暂时不处理这个问题
#endif
	m_isInited = temp.m_isInited;
	return 1;
}

_XNumber& _XNumber::operator = (const _XNumber& temp)
{
	if(& temp == this) return * this;	//防止自身赋值
	if(temp.m_isInited == 0) return * this;	//如果目标没有初始化则直接退出
	//if(m_isInited != 0) 
	//{
	//	m_sprite.release()
	//	//return 0;	//如果自身已经初始化则必须要退出，否则将会造成内存泄露
	//}
	m_isVisiable = temp.m_isVisiable;
	m_alignmentMode = temp.m_alignmentMode;

	m_maxPixelWidth = temp.m_maxPixelWidth;
	m_maxPixelHeight = temp.m_maxPixelHeight;
	m_needUpdateData = temp.m_needUpdateData;
	m_needShowTextSum = temp.m_needShowTextSum;
	for(int i = 0;i < MAX_NUMBER_LENGTH;++ i)
	{
		m_textPosition[i] = temp.m_textPosition[i];
		m_textRect[i] = temp.m_textRect[i];
	}

	strcpy(m_string,temp.m_string);
	m_position = temp.m_position;	//字体的位置
	m_setPosition = temp.m_setPosition;	//字体的位置
	m_angle = temp.m_angle;			//字体的角度
	m_angleSin = temp.m_angleSin;			//字体的角度
	m_angleCos = temp.m_angleCos;			//字体的角度
	m_alpha = temp.m_alpha;
	m_distance = temp.m_distance;		//文字之间的距离	
	m_size = temp.m_size;		//文字的尺寸（像素大小）
	m_layout = temp.m_layout;		//字体图片的布局
	m_showSize = temp.m_showSize;
	m_rotateBasicPoint = temp.m_rotateBasicPoint;
	m_sprite.setACopy(temp.m_sprite);					//贴图
#if WITH_OBJECT_MANAGER	//在物件管理的类中注销这些物件
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
	_XObjectManager::GetInstance().addAObject(this,OBJ_NUMBER);	//addAObject内部已经避免了重复注册的问题，外部暂时不处理这个问题
#endif
	m_isInited = temp.m_isInited;

	return * this;
}

int _XNumber::isInRect(float x,float y)
{
	if(m_isInited == 0) return 0;
	return getIsInRect(_XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
}

_XVector2 _XNumber::getBox(int order)
{//暂时不考虑角度
	_XVector2 ret;
	ret.set(0.0f,0.0f);
	if(m_alignmentMode == _NUMBER_ALIGNMENT_MODE_LEFT)
	{
		if(order == 0) ret = m_setPosition; else
		if(order == 1) ret.set(m_setPosition.x + m_maxPixelWidth * m_angleCos,
			m_setPosition.y + m_maxPixelWidth * m_angleSin); else
		if(order == 2) ret.set(m_setPosition.x + m_maxPixelWidth * m_angleCos - m_maxPixelHeight * m_angleSin,
			m_setPosition.y + m_maxPixelWidth * m_angleSin + m_maxPixelHeight * m_angleCos); else
		if(order == 3) ret.set(m_setPosition.x - m_maxPixelHeight * m_angleSin,
			m_setPosition.y + m_maxPixelHeight * m_angleCos);
	}else
	if(m_alignmentMode == _NUMBER_ALIGNMENT_MODE_MIDDLE)
	{
		if(order == 0) ret.set(m_setPosition.x - m_maxPixelWidth * 0.5f * m_angleCos,
			m_setPosition.y - m_maxPixelWidth * 0.5f * m_angleSin); else
		if(order == 1) ret.set(m_setPosition.x + m_maxPixelWidth * 0.5f * m_angleCos,
			m_setPosition.y + m_maxPixelWidth * 0.5f * m_angleSin); else
		if(order == 2) ret.set(m_setPosition.x + m_maxPixelWidth * 0.5f * m_angleCos - m_maxPixelHeight * m_angleSin,
			m_setPosition.y + m_maxPixelWidth * 0.5f * m_angleSin + m_maxPixelHeight * m_angleCos); else
		if(order == 3) ret.set(m_setPosition.x - m_maxPixelWidth * 0.5f * m_angleCos - m_maxPixelHeight * m_angleSin,
			m_setPosition.y - m_maxPixelWidth * 0.5f * m_angleSin + m_maxPixelHeight * m_angleCos);
	}else
	if(m_alignmentMode == _NUMBER_ALIGNMENT_MODE_RIGHT)
	{
		if(order == 0) ret.set(m_setPosition.x - m_maxPixelWidth * m_angleCos,
			m_setPosition.y - m_maxPixelWidth * m_angleSin); else
		if(order == 1) ret.set(m_setPosition.x,m_setPosition.y); else
		if(order == 2) ret.set(m_setPosition.x - m_maxPixelHeight * m_angleSin,
			m_setPosition.y + m_maxPixelHeight * m_angleCos); else
		if(order == 3) ret.set(m_setPosition.x - m_maxPixelWidth * m_angleCos - m_maxPixelHeight * m_angleSin,
			m_setPosition.y - m_maxPixelWidth * m_angleSin + m_maxPixelHeight * m_angleCos);
	}
	return ret;
}
