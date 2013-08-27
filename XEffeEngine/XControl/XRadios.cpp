//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XRadios.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

_XRadios::_XRadios()
:m_isInited(0)			//空间时都已经初始化
,m_radioSum(1)			//选项的数量
,m_nowChoose(0)			//当前所选的项的编号
,m_radio(NULL)			//所有单选项的指针
,m_checkPosition(NULL)
,m_funStateChange(NULL)
{
}

_XRadios::~_XRadios()
{
	release();
}

void _XRadios::release()	//释放分配的资源
{
	if(m_isInited == 0) return;
	XDELETE_ARRAY(m_radio);
	XDELETE_ARRAY(m_checkPosition);
	_XControlManager::GetInstance().decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(this);
#endif
	m_isInited = 0;
}

int _XRadios::init(int radioSum,			//选项的数量
		const _XVector2& distance,
		const _XVector2& position,	//控件的位置
		const _XRect &Area,			//选择图标的鼠标响应范围
		const _XRadiosTexture *tex,const _XFontUnicode &font,float captionSize,
		const _XVector2& textPosition)	//单选框初始化
{
	if(m_isInited != 0) return 0;
	if(Area.getHeight() <= 0 || Area.getWidth() <= 0) return 0;	//空间必须要有一个响应区间，不然会出现除零错误
	if(radioSum < 1) return 0;	//不能没有选项，必须要有一个选项
	if(captionSize <= 0) return 0;
	m_mouseRect = Area;
	m_position = position;
	m_distance = distance;

	if(tex->checkChoosed == NULL || tex->checkDisableChoosed == NULL 
		|| tex->checkDisableDischoose == NULL || tex->checkDischoose == NULL) return 0;//如果贴图缺少，也直接返回失败
	//分配内存空间
	m_radioSum = radioSum;
	try
	{
		m_radio = new _XCheck[m_radioSum];
		if(m_radio == NULL) return 0;//如果内存分配失败，直接返回错误
	}catch(...)
	{
		return 0;
	}
	try
	{
		m_checkPosition = new _XVector2[m_radioSum];
		if(m_checkPosition == NULL)
		{
			XDELETE_ARRAY(m_radio);
			return 0;
		}
	}catch(...)
	{
		XDELETE_ARRAY(m_radio);
		return 0;
	}
	m_size.set(1.0f,1.0f);

	m_texture = tex;
	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(_FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
	m_caption.setAlignmentModeY(_FONT_ALIGNMENT_MODE_Y_MIDDLE); //设置字体居中对齐
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//设置字体的颜色为黑色
	m_captionSize = captionSize;
	m_textPosition = textPosition;

#if WITH_OBJECT_MANAGER
	//printf("Now obj sum:%d\n",_XObjectManager::GetInstance().getNowObjectSum());
#endif
	//初始化所有的选项
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_checkPosition[i].set(m_distance.x * i,m_distance.y * i);
		if(m_radio[i].init(_XVector2(m_position.x + m_checkPosition[i].x * m_size.x,m_position.y + m_checkPosition[i].y * m_size.y),
			m_mouseRect,* m_texture," ",m_caption,m_captionSize,m_textPosition) == 0)
		{
			XDELETE_ARRAY(m_radio);
			XDELETE_ARRAY(m_checkPosition);
			return 0;
		}
		m_radio[i].setState(false);
	//在物件管理器中注销掉这些物件
		_XControlManager::GetInstance().decreaseAObject(&(m_radio[i]));
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_radio[i]));
#endif
	}

#if WITH_OBJECT_MANAGER
	//printf("Now obj sum:%d\n",_XObjectManager::GetInstance().getNowObjectSum());
#endif
	m_nowChoose = 0;
	m_radio[0].setState(true);

	m_isVisiable = 1;
	m_isEnable = 1;
	m_isActive = 1;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_RADIOS);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = 1;
	return 1;
}

void _XRadios::disable()
{
	if(m_isInited == 0) return ;	//如果没有初始化直接退出
	if(m_isEnable != 0)
	{
		m_isEnable = 0;
		for(int i = 0;i < m_radioSum;++ i)
		{
			m_radio[i].disable();
		}
	}
}

void _XRadios::enable()
{
	if(m_isInited == 0) return ;	//如果没有初始化直接退出
	if(m_isEnable == 0)
	{
		m_isEnable = 1;
		for(int i = 0;i < m_radioSum;++ i)
		{
			m_radio[i].enable();
		}
	}
}

void _XRadios::draw()
{	
	if(m_isInited == 0) return ;	//如果没有初始化直接退出
	if(m_isVisiable == 0) return;	//如果不可见直接退出
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_radio[i].draw();
	}
}

void _XRadios::setChoosed(int temp)
{
	if(temp < 0 || temp >= m_radioSum) return;
	if(temp == m_nowChoose) return;
	m_nowChoose = temp;
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_radio[i].setState(false);
	}
	m_radio[m_nowChoose].setState(true);
	if(m_funStateChange != NULL) (* m_funStateChange)(m_pClass,m_objectID);
}


void _XRadios::setRadioPosition(float x,float y,int order)
{
	if(m_isInited == 0) return ;	//如果没有初始化直接退出
	if(order < 0 || order >= m_radioSum) return;
	m_checkPosition[order].set(x,y);
	m_radio[order].setPosition(m_position.x + x * m_size.x,m_position.y + y * m_size.y);
}

int _XRadios::canGetFocus(float x,float y)
{
	if(m_isInited == 0) return 0;	//如果没有初始化直接退出
	if(m_isActive == 0) return 0;		//没有激活的控件不接收控制
	if(m_isVisiable == 0) return 0;	//如果不可见直接退出
	if(m_isEnable == 0) return 0;		//如果无效则直接退出
	return isInRect(x,y);
}

int _XRadios::canLostFocus(float x,float y)
{
	return 1;
}

int _XRadios::mouseProc(float x,float y,_XMouseState mouseState)
{
	if(m_isInited == 0) return 0;	//如果没有初始化直接退出
	if(m_isActive == 0) return 0;		//没有激活的控件不接收控制
	if(m_isVisiable == 0) return 0;	//如果不可见直接退出
	if(m_isEnable == 0) return 0;		//如果无效则直接退出

	int tempOrder = -1;	//记录被操作的选项的编号
	for(int i = 0;i < m_radioSum;++ i)
	{
		if(m_radio[i].mouseProc(x,y,mouseState) != 0)
		{//这里默认只会有一个选项被操作
			if(m_nowChoose != i) tempOrder = i;
			else m_radio[m_nowChoose].setState(true);
		}
	}
	if(tempOrder != -1)
	{
		//printf("NowChoose:%d,%d\n",tempOrder,m_objectID);
		m_nowChoose = tempOrder;
		for(int i = 0;i < m_radioSum;++ i)
		{
			m_radio[i].setState(false);
		}
		m_radio[m_nowChoose].setState(true);
		if(m_funStateChange != NULL) (* m_funStateChange)(m_pClass,m_objectID);
	}
	return 1;
}

int _XRadios::keyboardProc(int keyOrder,_XKeyState keyState)
{
	if(m_isInited == 0) return 0;	//如果没有初始化直接退出
	if(m_isActive == 0) return 0;		//没有激活的控件不接收控制
	if(m_isVisiable == 0) return 0;	//如果不可见直接退出
	if(m_isEnable == 0) return 0;		//如果无效则直接退出

	if(keyState == _XKEY_STATE_UP && m_isBeChoose != 0)
	{//按键弹起时才作相应
		if(keyOrder == SDLK_LEFT || keyOrder == SDLK_UP)
		{//按下向左向上
			--m_nowChoose;
			if(m_nowChoose < 0) m_nowChoose = m_radioSum - 1;
			for(int i = 0;i < m_radioSum;++ i)
			{
				m_radio[i].setState(false);
			}
			m_radio[m_nowChoose].setState(true);
			if(m_funStateChange != NULL) (* m_funStateChange)(m_pClass,m_objectID);
		}else
		if(keyOrder == SDLK_RIGHT || keyOrder == SDLK_DOWN)
		{//按下向右向下
			++m_nowChoose;
			if(m_nowChoose >= m_radioSum) m_nowChoose = 0;
			for(int i = 0;i < m_radioSum;++ i)
			{
				m_radio[i].setState(false);
			}
			m_radio[m_nowChoose].setState(true);
			if(m_funStateChange != NULL) (* m_funStateChange)(m_pClass,m_objectID);
		}
	}
	return 1;
}

void _XRadios::setSize(float x,float y)
{
	if(m_isInited == 0) return;	//如果没有初始化直接退出
	if(x <= 0 && y <= 0) return;
	m_size.set(x,y);
	//移动所有选项控件的相对位置
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_radio[i].setPosition(m_position.x + m_checkPosition[i].x * m_size.x,m_position.y + m_checkPosition[i].y * m_size.y);
		m_radio[i].setSize(m_size);
	}
}

void _XRadios::setPosition(float x,float y)
{
	if(m_isInited == 0) return;	//如果没有初始化直接退出
	m_position.set(x,y);
	//移动所有选项控件的相对位置
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_radio[i].setPosition(m_position.x + m_checkPosition[i].x * m_size.x,m_position.y + m_checkPosition[i].y * m_size.y);
	}
}

int _XRadios::setRadioSum(int radioSum)
{
	if(m_isInited == 0) return 0;	//如果没有初始化直接退出
	if(radioSum <= 0) return 0;
#if WITH_OBJECT_MANAGER
	//printf("Now obj sum:%d\n",_XObjectManager::GetInstance().getNowObjectSum());
#endif
	XDELETE_ARRAY(m_radio);
	XDELETE_ARRAY(m_checkPosition);
#if WITH_OBJECT_MANAGER
	//printf("Now obj sum:%d\n",_XObjectManager::GetInstance().getNowObjectSum());
#endif
	m_radioSum = radioSum;
	try
	{
		m_radio = new _XCheck[m_radioSum];
		if(m_radio == NULL) return 0;//如果内存分配失败，直接返回错误
	}catch(...)
	{
		return 0;
	}
#if WITH_OBJECT_MANAGER
	//printf("Now obj sum:%d\n",_XObjectManager::GetInstance().getNowObjectSum());
#endif
	try
	{
		m_checkPosition = new _XVector2[m_radioSum];
		if(m_checkPosition == NULL)
		{
			XDELETE_ARRAY(m_radio);
			return 0;
		}
	}catch(...)
	{
		XDELETE_ARRAY(m_radio);
		return 0;
	}
	//初始化所有的选项
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_checkPosition[i].set(m_distance.x * i,m_distance.y * i);
		if(m_radio[i].init(_XVector2(m_position.x + m_checkPosition[i].x * m_size.x,m_position.y + m_checkPosition[i].y * m_size.y),
			m_mouseRect,* m_texture," ",m_caption,m_captionSize,m_textPosition) == 0)
		{
			XDELETE_ARRAY(m_radio);
			XDELETE_ARRAY(m_checkPosition);
			return 0;
		}
		//在控件管理器中注销掉这些物件
		_XControlManager::GetInstance().decreaseAObject(&(m_radio[i]));	
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_radio[i]));
#endif
		m_radio[i].setState(false);
	}
#if WITH_OBJECT_MANAGER
	//printf("Now obj sum:%d\n",_XObjectManager::GetInstance().getNowObjectSum());
#endif

	m_nowChoose = 0;
	m_radio[0].setState(true);
	return 1;
}

int _XRadios::setACopy(const _XRadios &temp)
{
	if(&temp == this) return 0;
	if(temp.m_isInited == 0) return 0;
	if(m_isInited != 0) release();
	if(_XControlBasic::setACopy(temp) == 0) return 0;
	if(m_isInited == 0)
	{
		_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_RADIOS);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	}

	m_isInited = temp.m_isInited;
	m_radioSum = temp.m_radioSum;	//选项的数量
	m_nowChoose = temp.m_nowChoose;	//当前所选的项的编号
	m_distance = temp.m_distance;	//当前所选的项的编号

	m_texture = temp.m_texture;
	m_caption.setACopy(temp.m_caption);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_captionSize = temp.m_captionSize;
	m_textColor = temp.m_textColor;
	m_textPosition = temp.m_textPosition;

	try
	{
		m_radio = new _XCheck[m_radioSum];
		if(m_radio == NULL) return 0;//如果内存分配失败，直接返回错误
	}catch(...)
	{
		return 0;
	}
	try
	{
		m_checkPosition = new _XVector2[m_radioSum];
		if(m_checkPosition == NULL)
		{
			XDELETE_ARRAY(m_radio);
			return 0;
		}
	}catch(...)
	{
		XDELETE_ARRAY(m_radio);
		return 0;
	}
	for(int i = 0;i < m_radioSum;++i)
	{
		if(m_radio[i].setACopy(temp.m_radio[i]) == 0) return 0;
		m_checkPosition[i] = temp.m_checkPosition[i];
		//在控件管理器中注销掉这些物件
		_XControlManager::GetInstance().decreaseAObject(&(m_radio[i]));	
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_radio[i]));
#endif
	}

	m_funStateChange = temp.m_funStateChange;			//控件状态改变时调用
	m_pClass = temp.m_pClass;
	return 1;
}

int _XRadios::isInRect(float x,float y)
{
	if(m_isInited == 0) return 0;
	return getIsInRect(_XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
}

_XVector2 _XRadios::getBox(int order)
{
	_XVector2 ret;
	ret.set(0.0f,0.0f);
	_XVector2 temp;
	if(m_isInited == 0) return ret;
	//遍历所有的按钮，找到最外围的框，每次都遍历，会不会太吃效率
	if(order == 0)
	{
		ret = m_radio[0].getBox(0);
		for(int i = 0;i < m_radioSum;++i)
		{
			temp = m_radio[i].getBox(0);
			if(temp.x < ret.x) ret.x = temp.x;
			if(temp.y < ret.y) ret.y = temp.y;
		}
	}else
	if(order == 1)
	{
		ret = m_radio[0].getBox(1);
		for(int i = 0;i < m_radioSum;++i)
		{
			temp = m_radio[i].getBox(1);
			if(temp.x > ret.x) ret.x = temp.x;
			if(temp.y < ret.y) ret.y = temp.y;
		}
	}else
	if(order == 2)
	{
		ret = m_radio[0].getBox(2);
		for(int i = 0;i < m_radioSum;++i)
		{
			temp = m_radio[i].getBox(2);
			if(temp.x > ret.x) ret.x = temp.x;
			if(temp.y > ret.y) ret.y = temp.y;
		}
	}else
	if(order == 3)
	{
		ret = m_radio[0].getBox(3);
		for(int i = 0;i < m_radioSum;++i)
		{
			temp = m_radio[i].getBox(3);
			if(temp.x < ret.x) ret.x = temp.x;
			if(temp.y > ret.y) ret.y = temp.y;
		}
	}
	return ret;
}