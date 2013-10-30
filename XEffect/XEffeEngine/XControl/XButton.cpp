//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XButton.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

_XButtonTexture::_XButtonTexture()
:m_isInited(0)
,buttonNormal(NULL)			//普通状态下的按钮贴图
,buttonDown(NULL)			//按下状态下的按钮贴图
,buttonOn(NULL)				//悬浮状态下的按钮贴图
,buttonDisable(NULL)		//无效状态下的按钮贴图
{
}

_XButtonTexture::~_XButtonTexture()
{
	release();
}

int _XButtonTexture::init(const char *normal,const char *down,const char *on,const char *disable,int resoursePosition)
{
	if(m_isInited != 0) return 0;
	if(normal == NULL) return 0;
	int ret = 1;

	if(normal != NULL && 
		(buttonNormal = createATextureData(normal,resoursePosition)) == NULL) ret = 0;
	if(down != NULL && ret != 0 && 
		(buttonDown = createATextureData(down,resoursePosition)) == NULL) ret = 0;
	if(on != NULL && ret != 0 && 
		(buttonOn = createATextureData(on,resoursePosition)) == NULL) ret = 0;
	if(disable != NULL && ret != 0 && 
		(buttonDisable = createATextureData(disable,resoursePosition)) == NULL) ret = 0;
	
	if(ret == 0)
	{
		XDELETE(buttonNormal);
		XDELETE(buttonDown);
		XDELETE(buttonOn);
		XDELETE(buttonDisable);
		return 0;
	}
	m_isInited = 1;
	return 1;
}

void _XButtonTexture::release()
{
	if(m_isInited == 0) return;
	XDELETE(buttonNormal);
	XDELETE(buttonDown);
	XDELETE(buttonOn);
	XDELETE(buttonDisable);
	m_isInited = 0;
}

_XButton::_XButton()
:m_isInited(0)
,m_buttonNormal(NULL)		
,m_buttonDown(NULL)	
,m_buttonOn(NULL)		
,m_buttonDisable(NULL)	
,m_funInit(NULL)
,m_funRelease(NULL)
,m_funMouseOn(NULL)
,m_funMouseDown(NULL)
,m_funMouseUp(NULL)
,m_pClass(NULL)
,m_hotKey(-1)
,m_nowButtonState(_XBUTTON_STATE_NORMAL)
,m_upMousePoint(0,0)
{
}

_XButton::~_XButton()
{
	release();
}

void _XButton::release()
{
	_XControlManager::GetInstance().decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(this);
#endif
}

void _XButton::setTexture(const _XButtonTexture& tex)
{
	if(tex.buttonNormal == NULL) return;//按键的普通状态的贴图不能为空，否则直接返回错误
	m_buttonNormal = tex.buttonNormal;
	if(tex.buttonDown != NULL) m_buttonDown = tex.buttonDown;
	else m_buttonDown = tex.buttonNormal;
	
	if(tex.buttonOn != NULL) m_buttonOn = tex.buttonOn;
	else m_buttonOn = tex.buttonNormal;
	
	if(tex.buttonDisable != NULL) m_buttonDisable = tex.buttonDisable;
	else m_buttonDisable = tex.buttonNormal;
}

int _XButton::init(const _XVector2& position,	//控件所在的位置
		const _XRect& Area,				//控件鼠标响应的区间
		const _XButtonTexture &tex,		//控件的贴图信息
		const char *caption,const _XFontUnicode &font,float captionSize,_XVector2 textPosition)	//控件标题的相关信息
{
	if(m_isInited != 0) return 1;	//如果已经初始化了则防止重复初始化
	if(captionSize <= 0) return 0;
	if(Area.getHeight() <= 0 || Area.getWidth() <= 0) return 0;	//按钮的控件必须要有一个鼠标响应区域
	m_mouseRect = Area;
	m_position = position;
	m_textPosition = textPosition;
	if(tex.buttonNormal == NULL) return 0;//按键的普通状态的贴图不能为空，否则直接返回错误
	m_buttonNormal = tex.buttonNormal;
	if(tex.buttonDown != NULL) m_buttonDown = tex.buttonDown;
	else m_buttonDown = tex.buttonNormal;
	if(tex.buttonOn != NULL) m_buttonOn = tex.buttonOn;
	else m_buttonOn = tex.buttonNormal;
	if(tex.buttonDisable != NULL) m_buttonDisable = tex.buttonDisable;
	else m_buttonDisable = tex.buttonNormal;

	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(_FONT_ALIGNMENT_MODE_X_MIDDLE); //设置字体居中对齐
	m_caption.setAlignmentModeY(_FONT_ALIGNMENT_MODE_Y_MIDDLE); //设置字体居中对齐
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//设置字体的颜色为黑色

	m_size.set(1.0f,1.0f);
	m_sprite.init(m_buttonNormal->texture.m_w,m_buttonNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
#endif
	m_sprite.setPosition(m_position);
	m_sprite.setSize(m_size);
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);
	//这里距中对齐，所以这里的位置需要计算
	m_caption.setString(caption);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_textSize.set(captionSize,captionSize);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);

	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);

	m_nowButtonState = _XBUTTON_STATE_NORMAL;

	if(m_funInit != NULL) (* m_funInit)(m_pClass,m_objectID);

	m_isVisiable = 1;
	m_isEnable = 1;
	m_isActive = 1;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_BUTTON);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = 1;
	return 1;
}

void _XButton::setPosition(float x,float y)
{
	if(m_isInited == 0) return;
	m_position.set(x,y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_sprite.setPosition(m_position);
	mouseProc(m_upMousePoint.x,m_upMousePoint.y,_XMOUSE_MOVE);
}

void _XButton::setSize(float x,float y)
{
	if(m_isInited == 0) return;
	if(x <= 0 && y <= 0) return;
	m_size.set(x,y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	m_sprite.setSize(m_size);
	mouseProc(m_upMousePoint.x,m_upMousePoint.y,_XMOUSE_MOVE);
}

int _XButton::canGetFocus(float x,float y)
{
	if(m_isInited == 0) return 0;	//如果没有初始化直接退出
	if(m_isActive == 0) return 0;		//没有激活的控件不接收控制
	if(m_isVisiable == 0) return 0;	//如果不可见直接退出
	if(m_isEnable == 0) return 0;		//如果无效则直接退出
	if(m_nowMouseRect.isInRect(x,y)) return 1;
	else return 0;
}

int _XButton::canLostFocus(float x,float y)
{
	return 1;
}

int _XButton::mouseProc(float x,float y,_XMouseState mouseState)
{
	m_upMousePoint.set(x,y);
	if(m_isInited == 0) return 0;	//如果没有初始化直接退出
	if(m_isActive == 0) return 0;		//没有激活的控件不接收控制
	if(m_isVisiable == 0) return 0;	//如果不可见直接退出
	if(m_isEnable == 0) return 0;		//如果无效则直接退出
	
	if(m_nowMouseRect.isInRect(x,y))
	{//鼠标动作在范围内
		if(mouseState == _XMOUSE_MOVE && m_nowButtonState == _XBUTTON_STATE_NORMAL)
		{
			m_nowButtonState = _XBUTTON_STATE_ON;
			if(m_funMouseOn != NULL) (* m_funMouseOn)(m_pClass,m_objectID);
		}else
		if(mouseState == _XMOUSE_LEFT_BUTTON_DOWN && (m_nowButtonState == _XBUTTON_STATE_NORMAL || m_nowButtonState == _XBUTTON_STATE_ON))
		{
			m_nowButtonState = _XBUTTON_STATE_DOWN;
			if(m_funMouseDown != NULL) (* m_funMouseDown)(m_pClass,m_objectID);
		}else
		if(mouseState == _XMOUSE_LEFT_BUTTON_UP && m_nowButtonState == _XBUTTON_STATE_DOWN)
		{
			m_nowButtonState = _XBUTTON_STATE_ON;
			if(m_funMouseUp != NULL) (* m_funMouseUp)(m_pClass,m_objectID);
		}
	}else
	{
		if(m_nowButtonState == _XBUTTON_STATE_ON || m_nowButtonState == _XBUTTON_STATE_DOWN)
		{
			m_nowButtonState = _XBUTTON_STATE_NORMAL;
		}
	}
	return 1;
}

int _XButton::keyboardProc(int keyOrder,_XKeyState keyState)
{
	if(m_isInited == 0) return 0;		//如果没有初始化直接退出
	if(m_isActive == 0) return 0;		//没有激活的控件不接收控制
	if(m_isVisiable == 0) return 0;		//如果不可见直接退出
	if(m_isEnable == 0) return 0;		//如果无效则直接退出

	if(keyState == _XKEY_STATE_UP)
	{//按键弹起时才作相应
		if(m_nowButtonState == _XBUTTON_STATE_NORMAL)
		{//按钮只有在普通状态下才能响应快捷键，防止冲突
			if(keyOrder == m_hotKey || (keyOrder ==  SDLK_RETURN && m_isBeChoose != 0))
			{
				if(m_funMouseDown != NULL) (* m_funMouseDown)(m_pClass,m_objectID);
				if(m_funMouseUp != NULL) (* m_funMouseUp)(m_pClass,m_objectID);
				return 1;
			}
		}
	}
	return 0;
}

void _XButton::draw()
{
	if(m_isInited == 0) return ;	//如果没有初始化直接退出
	if(m_isVisiable == 0) return;	//如果不可见直接退出

	if(m_nowButtonState == _XBUTTON_STATE_NORMAL) m_sprite.draw(m_buttonNormal); else
	if(m_nowButtonState == _XBUTTON_STATE_DOWN) m_sprite.draw(m_buttonDown); else
	if(m_nowButtonState == _XBUTTON_STATE_ON) m_sprite.draw(m_buttonOn); else
	if(m_nowButtonState == _XBUTTON_STATE_DISABLE) m_sprite.draw(m_buttonDisable);

	//显示按钮的字体
	m_caption.draw();
}

void _XButton::setCallbackFun(void (* funInit)(void *,int),
							  void (* funRelease)(void *,int),
							  void (* funMouseOn)(void *,int),
							  void (* funMouseDown)(void *,int),
							  void (* funMouseUp)(void *,int),void *pClass)
{
	if(funInit != NULL) m_funInit = funInit;
	if(funRelease != NULL) m_funRelease = funRelease;
	if(funMouseOn != NULL) m_funMouseOn = funMouseOn;
	if(funMouseDown != NULL) m_funMouseDown = funMouseDown;
	if(funMouseUp != NULL) m_funMouseUp = funMouseUp;
	m_pClass = pClass;
}

int _XButton::setACopy(const _XButton &temp)
{
	if(& temp == this) return 1;	//防止自身赋值
	if(temp.m_isInited == 0) return 0;
	if(_XControlBasic::setACopy(temp) == 0) return 0;
	if(m_isInited == 0)
	{
		_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_BUTTON);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	}
	m_isInited = temp.m_isInited;					//按钮是否被初始化
	m_nowButtonState = temp.m_nowButtonState;		//当前的按钮状态

	if(m_caption.setACopy(temp.m_caption) == 0)	 return 0;
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif

	m_buttonNormal = temp.m_buttonNormal;			//普通状态下的按钮贴图(无论如何，这个贴图一定要有)
	m_buttonDown = temp.m_buttonDown;			//按下状态下的按钮贴图
	m_buttonOn = temp.m_buttonOn;				//悬浮状态下的按钮贴图
	m_buttonDisable = temp.m_buttonDisable;			//无效状态下的按钮贴图

	m_funInit = temp.m_funInit;
	m_funRelease = temp.m_funRelease;
	m_funMouseOn = temp.m_funMouseOn;
	m_funMouseDown = temp.m_funMouseDown;
	m_funMouseUp = temp.m_funMouseUp;
	m_pClass = temp.m_pClass;				//回调函数的参数

	m_sprite.setACopy(temp.m_sprite);	//用于显示贴图的精灵
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
#endif
	m_textPosition = temp.m_textPosition;	//文字显示的位置，是相对于控件的位置来定的
	m_textSize = temp.m_textSize;		//文字显示的尺寸，这个尺寸会与空间的缩放尺寸叠加
	m_upMousePoint = temp.m_upMousePoint;	//上次记录的鼠标位置

	m_hotKey = temp.m_hotKey;
	return 1;
}

int _XButton::isInRect(float x,float y)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(m_isInited == 0) return 0;
	return getIsInRect(_XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
}

_XVector2 _XButton::getBox(int order)			//获取四个顶点的坐标，目前先不考虑旋转和缩放
{	
	_XVector2 ret;
	ret.set(0.0f,0.0f);
	if(m_isInited == 0) return ret;

	if(order == 0) ret.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y);else
	if(order == 1) ret.set(m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.top * m_size.y);else
	if(order == 2) ret.set(m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);else
	if(order == 3) ret.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);

	return ret;
}