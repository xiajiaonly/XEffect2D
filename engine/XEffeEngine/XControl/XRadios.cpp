//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XRadios.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "XResourceManager.h"

_XRadios::_XRadios()
:m_isInited(XFalse)			//空间时都已经初始化
,m_radioSum(1)			//选项的数量
,m_nowChoose(0)			//当前所选的项的编号
,m_radio(NULL)			//所有单选项的指针
,m_checkPosition(NULL)
,m_funStateChange(NULL)
,m_resInfo(NULL)
,m_withoutTex(XFalse)
{
}
_XRadios::~_XRadios()
{
	release();
}
void _XRadios::release()	//释放分配的资源
{
	if(!m_isInited) return;
	XDELETE_ARRAY(m_radio);
	XDELETE_ARRAY(m_checkPosition);
	_XControlManager::GetInstance().decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(this);
#endif
	if(m_resInfo != NULL)
	{
		_XResourceManager::GetInstance().releaseResource(m_resInfo);
		m_resInfo = NULL;
	}
	m_isInited = XFalse;
}
_XBool _XRadios::init(int radioSum,			//选项的数量
		const _XVector2& distance,
		const _XVector2& position,	//控件的位置
		const _XRect &Area,			//选择图标的鼠标响应范围
		const _XRadiosTexture *tex,const _XFontUnicode &font,float captionSize,
		const _XVector2& textPosition)	//单选框初始化
{
	if(m_isInited) return XFalse;
	if(Area.getHeight() <= 0 || Area.getWidth() <= 0) return XFalse;	//空间必须要有一个响应区间，不然会出现除零错误
	if(radioSum < 1) return XFalse;	//不能没有选项，必须要有一个选项
	if(captionSize <= 0) return XFalse;
	m_mouseRect = Area;
	m_position = position;
	m_distance = distance;
	m_withoutTex = XFalse;

	if(tex->checkChoosed == NULL || tex->checkDisableChoosed == NULL 
		|| tex->checkDisableDischoose == NULL || tex->checkDischoose == NULL) return XFalse;//如果贴图缺少，也直接返回失败
	//分配内存空间
	m_radioSum = radioSum;
	m_radio = createArrayMem<_XCheck>(m_radioSum);
	if(m_radio == NULL) return XFalse;//如果内存分配失败，直接返回错误
	m_checkPosition = createArrayMem<_XVector2>(m_radioSum);
	if(m_checkPosition == NULL)
	{
		XDELETE_ARRAY(m_radio);
		return XFalse;
	}
	m_size.set(1.0f,1.0f);

	m_texture = tex;
	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //设置字体居中对齐
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
			return XFalse;
		}
		m_radio[i].setState(XFalse);
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
	m_radio[0].setState(XTrue);

	m_isVisiable = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_RADIOS);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XRadios::initEx(int radioSum,			//选项的数量
		const _XVector2& distance,	//每个单选项之间的距离
		const _XVector2& position,	//控件的位置
		const _XRadiosTexture* tex,const _XFontUnicode &font,float captionSize)
{
	if(m_isInited) return XFalse;
	if(tex->m_mouseRect.getHeight() <= 0 || tex->m_mouseRect.getWidth() <= 0) return XFalse;	//空间必须要有一个响应区间，不然会出现除零错误
	if(radioSum < 1) return XFalse;	//不能没有选项，必须要有一个选项
	if(captionSize <= 0) return XFalse;
	m_mouseRect = tex->m_mouseRect;
	m_position = position;
	m_distance = distance;
	m_withoutTex = XFalse;

	if(tex->checkChoosed == NULL || tex->checkDisableChoosed == NULL 
		|| tex->checkDisableDischoose == NULL || tex->checkDischoose == NULL) return XFalse;//如果贴图缺少，也直接返回失败
	//分配内存空间
	m_radioSum = radioSum;
	m_radio = createArrayMem<_XCheck>(m_radioSum);
	if(m_radio == NULL) return XFalse;//如果内存分配失败，直接返回错误
	m_checkPosition = createArrayMem<_XVector2>(m_radioSum);
	if(m_checkPosition == NULL)
	{
		XDELETE_ARRAY(m_radio);
		return XFalse;
	}
	m_size.set(1.0f,1.0f);

	m_texture = tex;
	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //设置字体居中对齐
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//设置字体的颜色为黑色
	m_captionSize = captionSize;
	m_textPosition = tex->m_fontPosition;

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
			return XFalse;
		}
		m_radio[i].setState(XFalse);
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
	m_radio[0].setState(XTrue);

	m_isVisiable = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_RADIOS);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XRadios::initPlus(int radioSum,			//选项的数量
		const _XVector2& distance,	//每个单选项之间的距离
		const char *path,const _XFontUnicode &font,float captionSize,
		_XResourcePosition resoursePosition)
{
	if(m_isInited) return XFalse;
	if(path == NULL) return XFalse;
	m_resInfo = _XResourceManager::GetInstance().loadResource(path,RESOURCE_TYPE_XCHECK_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	_XRadiosTexture * tex = (_XRadiosTexture *)m_resInfo->m_pointer;
	if(tex->m_mouseRect.getHeight() <= 0 || tex->m_mouseRect.getWidth() <= 0) return XFalse;	//空间必须要有一个响应区间，不然会出现除零错误
	if(radioSum < 1) return XFalse;	//不能没有选项，必须要有一个选项
	if(captionSize <= 0) return XFalse;
	m_mouseRect = tex->m_mouseRect;
	m_position.set(0.0f,0.0f);
	m_distance = distance;
	m_withoutTex = XFalse;

	if(tex->checkChoosed == NULL || tex->checkDisableChoosed == NULL 
		|| tex->checkDisableDischoose == NULL || tex->checkDischoose == NULL) return XFalse;//如果贴图缺少，也直接返回失败
	//分配内存空间
	m_radioSum = radioSum;
	m_radio = createArrayMem<_XCheck>(m_radioSum);
	if(m_radio == NULL) return XFalse;//如果内存分配失败，直接返回错误
	m_checkPosition = createArrayMem<_XVector2>(m_radioSum);
	if(m_checkPosition == NULL)
	{
		XDELETE_ARRAY(m_radio);
		return XFalse;
	}
	m_size.set(1.0f,1.0f);

	m_texture = tex;
	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //设置字体居中对齐
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//设置字体的颜色为黑色
	m_captionSize = captionSize;
	m_textPosition = tex->m_fontPosition;

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
			return XFalse;
		}
		m_radio[i].setState(XFalse);
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
	m_radio[0].setState(XTrue);

	m_isVisiable = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_RADIOS);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XRadios::initWithoutTex(int radioSum,
		const _XVector2& distance,
		const _XRect &area,
		const _XFontUnicode &font,float captionSize,
		const _XVector2& textPosition)
{
	if(m_isInited) return XFalse;
	if(radioSum < 1) return XFalse;	//不能没有选项，必须要有一个选项
	if(captionSize <= 0) return XFalse;
	m_mouseRect = area;
	m_position.set(0.0f,0.0f);
	m_distance = distance;
	m_withoutTex = XTrue;

	//分配内存空间
	m_radioSum = radioSum;
	m_radio = createArrayMem<_XCheck>(m_radioSum);
	if(m_radio == NULL) return XFalse;//如果内存分配失败，直接返回错误
	m_checkPosition = createArrayMem<_XVector2>(m_radioSum);
	if(m_checkPosition == NULL)
	{
		XDELETE_ARRAY(m_radio);
		return XFalse;
	}
	m_size.set(1.0f,1.0f);

	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //设置字体居中对齐
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
		if(!m_radio[i].initWithoutTex(" ",m_caption,m_captionSize,m_mouseRect,m_textPosition))
		{
			XDELETE_ARRAY(m_radio);
			XDELETE_ARRAY(m_checkPosition);
			return XFalse;
		}
		m_radio[i].setState(XFalse);
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
	m_radio[0].setState(XTrue);

	m_isVisiable = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_RADIOS);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = XTrue;
	return XTrue;
}
void _XRadios::disable()
{
	if(!m_isInited) return ;	//如果没有初始化直接退出
	if(m_isEnable)
	{
		m_isEnable = XFalse;
		for(int i = 0;i < m_radioSum;++ i)
		{
			m_radio[i].disable();
		}
	}
}
void _XRadios::enable()
{
	if(!m_isInited) return ;	//如果没有初始化直接退出
	if(!m_isEnable)
	{
		m_isEnable = XTrue;
		for(int i = 0;i < m_radioSum;++ i)
		{
			m_radio[i].enable();
		}
	}
}
void _XRadios::draw()
{	
	if(!m_isInited) return ;	//如果没有初始化直接退出
	if(!m_isVisiable) return;	//如果不可见直接退出
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
		m_radio[i].setState(XFalse);
	}
	m_radio[m_nowChoose].setState(XTrue);
	if(m_funStateChange != NULL) m_funStateChange(m_pClass,m_objectID);
}
void _XRadios::setRadioPosition(float x,float y,int order)
{
	if(!m_isInited) return ;	//如果没有初始化直接退出
	if(order < 0 || order >= m_radioSum) return;
	m_checkPosition[order].set(x,y);
	m_radio[order].setPosition(m_position.x + x * m_size.x,m_position.y + y * m_size.y);
}
_XBool _XRadios::canGetFocus(float x,float y)
{
	if(!m_isInited) return XFalse;	//如果没有初始化直接退出
	if(!m_isActive) return XFalse;		//没有激活的控件不接收控制
	if(!m_isVisiable) return XFalse;	//如果不可见直接退出
	if(!m_isEnable) return XFalse;		//如果无效则直接退出
	return isInRect(x,y);
}
_XBool _XRadios::mouseProc(float x,float y,_XMouseState mouseState)
{
	if(!m_isInited) return XFalse;	//如果没有初始化直接退出
	if(!m_isActive) return XFalse;		//没有激活的控件不接收控制
	if(!m_isVisiable) return XFalse;	//如果不可见直接退出
	if(!m_isEnable) return XFalse;		//如果无效则直接退出

	int tempOrder = -1;	//记录被操作的选项的编号
	for(int i = 0;i < m_radioSum;++ i)
	{
		if(m_radio[i].mouseProc(x,y,mouseState) != 0)
		{//这里默认只会有一个选项被操作
			if(m_nowChoose != i) tempOrder = i;
			else m_radio[m_nowChoose].setState(XTrue);
		}
	}
	if(tempOrder != -1)
	{
		//printf("NowChoose:%d,%d\n",tempOrder,m_objectID);
		m_nowChoose = tempOrder;
		for(int i = 0;i < m_radioSum;++ i)
		{
			m_radio[i].setState(XFalse);
		}
		m_radio[m_nowChoose].setState(XTrue);
		if(m_funStateChange != NULL) m_funStateChange(m_pClass,m_objectID);
	}
	return XTrue;
}
_XBool _XRadios::keyboardProc(int keyOrder,_XKeyState keyState)
{
	if(!m_isInited) return XFalse;	//如果没有初始化直接退出
	if(!m_isActive) return XFalse;		//没有激活的控件不接收控制
	if(!m_isVisiable) return XFalse;	//如果不可见直接退出
	if(!m_isEnable) return XFalse;		//如果无效则直接退出

	if(keyState == KEY_STATE_UP && m_isBeChoose)
	{//按键弹起时才作相应
		if(keyOrder == XKEY_LEFT || keyOrder == XKEY_UP)
		{//按下向左向上
			--m_nowChoose;
			if(m_nowChoose < 0) m_nowChoose = m_radioSum - 1;
			for(int i = 0;i < m_radioSum;++ i)
			{
				m_radio[i].setState(XFalse);
			}
			m_radio[m_nowChoose].setState(XTrue);
			if(m_funStateChange != NULL) m_funStateChange(m_pClass,m_objectID);
		}else
		if(keyOrder == XKEY_RIGHT || keyOrder == XKEY_DOWN)
		{//按下向右向下
			++m_nowChoose;
			if(m_nowChoose >= m_radioSum) m_nowChoose = 0;
			for(int i = 0;i < m_radioSum;++ i)
			{
				m_radio[i].setState(XFalse);
			}
			m_radio[m_nowChoose].setState(XTrue);
			if(m_funStateChange != NULL) m_funStateChange(m_pClass,m_objectID);
		}
	}
	return XTrue;
}
void _XRadios::setSize(float x,float y)
{
	if(!m_isInited) return;	//如果没有初始化直接退出
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
	if(!m_isInited) return;	//如果没有初始化直接退出
	m_position.set(x,y);
	//移动所有选项控件的相对位置
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_radio[i].setPosition(m_position.x + m_checkPosition[i].x * m_size.x,m_position.y + m_checkPosition[i].y * m_size.y);
	}
}
_XBool _XRadios::setRadioSum(int radioSum)
{
	if(!m_isInited) return XFalse;	//如果没有初始化直接退出
	if(radioSum <= 0) return XFalse;
#if WITH_OBJECT_MANAGER
	//printf("Now obj sum:%d\n",_XObjectManager::GetInstance().getNowObjectSum());
#endif
	XDELETE_ARRAY(m_radio);
	XDELETE_ARRAY(m_checkPosition);
#if WITH_OBJECT_MANAGER
	//printf("Now obj sum:%d\n",_XObjectManager::GetInstance().getNowObjectSum());
#endif
	m_radioSum = radioSum;
	m_radio = createArrayMem<_XCheck>(m_radioSum);
	if(m_radio == NULL) return XFalse;//如果内存分配失败，直接返回错误

#if WITH_OBJECT_MANAGER
	//printf("Now obj sum:%d\n",_XObjectManager::GetInstance().getNowObjectSum());
#endif
	m_checkPosition = createArrayMem<_XVector2>(m_radioSum);
	if(m_checkPosition == NULL)
	{
		XDELETE_ARRAY(m_radio);
		return XFalse;
	}

	//初始化所有的选项
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_checkPosition[i].set(m_distance.x * i,m_distance.y * i);
		if((!m_withoutTex && m_radio[i].init(_XVector2(m_position.x + m_checkPosition[i].x * m_size.x,m_position.y + m_checkPosition[i].y * m_size.y),
			m_mouseRect,* m_texture," ",m_caption,m_captionSize,m_textPosition) == 0)
			|| (m_withoutTex && m_radio[i].initWithoutTex(" ",m_caption,m_captionSize,m_mouseRect,m_textPosition) == 0))
		{
			XDELETE_ARRAY(m_radio);
			XDELETE_ARRAY(m_checkPosition);
			return XFalse;
		}
		//在控件管理器中注销掉这些物件
		_XControlManager::GetInstance().decreaseAObject(&(m_radio[i]));	
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_radio[i]));
#endif
		m_radio[i].setState(XFalse);
	}
#if WITH_OBJECT_MANAGER
	//printf("Now obj sum:%d\n",_XObjectManager::GetInstance().getNowObjectSum());
#endif

	m_nowChoose = 0;
	m_radio[0].setState(XTrue);
	return XTrue;
}
_XBool _XRadios::setACopy(const _XRadios &temp)
{
	if(&temp == this) return XFalse;
	if(!temp.m_isInited) return XFalse;
	if(m_isInited) release();
	if(!_XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
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

	if(m_resInfo != NULL) _XResourceManager::GetInstance().releaseResource(m_resInfo);
	m_resInfo = _XResourceMng.copyResource(temp.m_resInfo);
	m_withoutTex = temp.m_withoutTex;

	m_texture = temp.m_texture;
	m_caption.setACopy(temp.m_caption);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_captionSize = temp.m_captionSize;
	m_textColor = temp.m_textColor;
	m_textPosition = temp.m_textPosition;
	m_radio = createArrayMem<_XCheck>(m_radioSum);
	if(m_radio == NULL) return XFalse;//如果内存分配失败，直接返回错误

	m_checkPosition = createArrayMem<_XVector2>(m_radioSum);
	if(m_checkPosition == NULL)
	{
		XDELETE_ARRAY(m_radio);
		return XFalse;
	}
	for(int i = 0;i < m_radioSum;++i)
	{
		if(!m_radio[i].setACopy(temp.m_radio[i])) return XFalse;
		m_checkPosition[i] = temp.m_checkPosition[i];
		//在控件管理器中注销掉这些物件
		_XControlManager::GetInstance().decreaseAObject(&(m_radio[i]));	
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_radio[i]));
#endif
	}

	m_funStateChange = temp.m_funStateChange;			//控件状态改变时调用
	m_pClass = temp.m_pClass;
	return XTrue;
}
_XBool _XRadios::isInRect(float x,float y)
{
	if(!m_isInited) return XFalse;
	return getIsInRect(_XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
}
_XVector2 _XRadios::getBox(int order)
{
	_XVector2 ret;
	ret.set(0.0f,0.0f);
	_XVector2 temp;
	if(!m_isInited) return ret;
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
void _XRadios::setRadiosText(const char * temp)
{
	if(!m_isInited) return ;	//如果没有初始化直接退出
	if(temp == NULL) return;
	int sum = getCharSum(temp,';');
	if(sum < 0) sum = 0;
	int len = strlen(temp);
	if(temp[len - 2] != ';') sum += 1;
	if(!setRadioSum(sum)) return;
	int offset = 0;
	for(int i = 0;i < sum;++ i)
	{
		len = getCharPosition(temp + offset,';');
		string tmpStr = temp + offset;
		if(len >= 0) tmpStr.at(len) = '\0';
		setRadioText(tmpStr.c_str(),i);
		offset += len + 1;
	}
}