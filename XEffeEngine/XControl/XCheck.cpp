//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XCheck.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

_XCheckTexture::_XCheckTexture()
:m_isInited(0)
,checkChoosed(NULL)			//选择按钮选中的贴图		
,checkDischoose(NULL)			//选择按钮未选中的贴图		
,checkDisableChoosed(NULL)		//无效状态下选择按钮选中的贴图		
,checkDisableDischoose(NULL)	//无效状态下选择按钮未选中的贴图	
{
}

_XCheckTexture::~_XCheckTexture()
{
	release();
}

int _XCheckTexture::init(const char *choosed,const char *disChoose,const char *disableChoosed,const char *disableDischoose,int resoursePosition)
{
	if(m_isInited != 0) return 0;
	//注意这里四种状态的贴图都必须要有，否则将不能初始化
	if(choosed == NULL || disChoose == NULL || disableChoosed == NULL || disableDischoose == NULL) return 0;
	int ret = 1;
	
	if((checkChoosed = createATextureData(choosed,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(checkDischoose = createATextureData(disChoose,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(checkDisableChoosed = createATextureData(disableChoosed,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(checkDisableDischoose = createATextureData(disableDischoose,resoursePosition)) == NULL) ret = 0;
	
	if(ret == 0)
	{
		XDELETE(checkChoosed);
		XDELETE(checkDischoose);
		XDELETE(checkDisableChoosed);
		XDELETE(checkDisableDischoose);
		return 0;
	}

	m_isInited = 1;
	return 1;
}

void _XCheckTexture::release()
{
	if(m_isInited == 0) return;
	XDELETE(checkChoosed);
	XDELETE(checkDischoose);
	XDELETE(checkDisableChoosed);
	XDELETE(checkDisableDischoose);
	m_isInited = 0;
}

_XCheck::_XCheck()
:m_isInited(0)					//进度条是否被初始化
,m_checkChoosed(NULL)			//选择按钮选中的贴图
,m_checkDischoose(NULL)		//选择按钮未选中的贴图
,m_checkDisableChoosed(NULL)	//无效状态下选择按钮选中的贴图
,m_checkDisableDischoose(NULL)	//无效状态下选择按钮未选中的贴图
,m_funInit(NULL)				//控件初始化的时候调用，（这个目前没有实际生效）
,m_funRelease(NULL)				//控件注销的时候调用，（这个目前没有实际生效）
,m_funMouseOn(NULL)				//鼠标悬浮时调用
,m_funMouseDown(NULL)			//鼠标按下时调用		
,m_funMouseUp(NULL)				//鼠标弹起时调用
,m_funStateChange(NULL)			//控件状态改变时调用
,m_pClass(NULL)
,m_state(false)
,m_withCaption(1)
{
}

_XCheck::~_XCheck()
{
	release();
}

void _XCheck::release()
{
	_XControlManager::GetInstance().decreaseAObject(this);	//注销这个物件
	if(m_funRelease != NULL) (* m_funRelease)(m_pClass,m_objectID);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(this);
#endif
}

//注意：这里默认四个状态的贴图尺寸一致，否则将会出现意想不到的问题
int _XCheck::init(const _XVector2 & position,	//控件的位置
		const _XRect &Area,					//控件的图片的响应范围
		const _XCheckTexture &tex,			//控件的贴图
		const char *caption,const _XFontUnicode &font,float captionSize,	//显示的字体的相关信息
		const _XVector2 &textPosition)		//字体的位置
{
	if(m_isInited != 0) return 0;
	if(Area.getHeight() <= 0 || Area.getWidth() <= 0) return 0;	//空间必须要有一个响应区间，不然会出现除零错误
	m_position = position;
	m_mouseRect = Area;

	if(tex.checkChoosed == NULL || tex.checkDisableChoosed == NULL 
		|| tex.checkDisableDischoose == NULL || tex.checkDischoose == NULL) return 0;
	m_checkChoosed = tex.checkChoosed;			//选择按钮选中的贴图
	m_checkDischoose = tex.checkDischoose;		//选择按钮未选中的贴图
	m_checkDisableChoosed = tex.checkDisableChoosed;	//无效状态下选择按钮选中的贴图
	m_checkDisableDischoose = tex.checkDisableDischoose;	//无效状态下选择按钮未选中的贴图

	m_caption.setACopy(font);			//进度条的标题
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(_FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
	m_caption.setAlignmentModeY(_FONT_ALIGNMENT_MODE_Y_MIDDLE); //设置字体居中对齐
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//设置字体的颜色为黑色

	m_size.set(1.0f,1.0f);
	m_textPosition = textPosition;		//文字的相对位置

	m_sprite.init(m_checkChoosed->texture.m_w,m_checkChoosed->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
#endif
	m_sprite.setPosition(m_position);
	m_sprite.setSize(m_size);
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);

	m_caption.setString(caption);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	if(captionSize < 0) return 0;
	m_textSize.set(captionSize,captionSize);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);

	m_mouseClickArea.set(m_caption.getBox(0).x,m_caption.getBox(0).y,m_caption.getBox(2).x,m_caption.getBox(2).y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);

	m_state = false;				//复选框的选择状态
	if(m_funInit != NULL) (* m_funInit)(m_pClass,m_objectID);

	m_isVisiable = 1;
	m_isEnable = 1;
	m_isActive = 1;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_CHECK);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = 1;
	return 1;
}

void _XCheck::draw()
{
	if(m_isInited == 0) return ;	//如果没有初始化直接退出
	if(m_isVisiable == 0) return;	//如果不可见直接退出
	if(m_state)
	{
		if(m_isEnable != 0)m_sprite.draw(m_checkChoosed);
		else m_sprite.draw(m_checkDisableChoosed);
	}else
	{
		if(m_isEnable != 0)m_sprite.draw(m_checkDischoose);
		else m_sprite.draw(m_checkDisableDischoose);
	}
	if(m_withCaption != 0) m_caption.draw();	//如果需要则描绘文字内容
}

void _XCheck::setPosition(float x,float y)
{
	if(m_isInited == 0) return;	//如果没有初始化直接退出
	m_position.set(x,y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_sprite.setPosition(m_position);
	m_mouseClickArea.set(m_caption.getBox(0).x,m_caption.getBox(0).y,m_caption.getBox(2).x,m_caption.getBox(2).y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
}

void _XCheck::setSize(float x,float y)
{
	if(x <= 0 && y <= 0) return;
	if(m_isInited == 0) return;	//如果没有初始化直接退出
	m_size.set(x,y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	m_sprite.setSize(m_size);
	m_mouseClickArea.set(m_caption.getBox(0).x,m_caption.getBox(0).y,m_caption.getBox(2).x,m_caption.getBox(2).y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
}

void _XCheck::setText(const char *temp)
{
	if(temp == NULL) return;
	m_caption.setString(temp);
	m_mouseClickArea.set(m_caption.getBox(0).x,m_caption.getBox(0).y,m_caption.getBox(2).x,m_caption.getBox(2).y);
}

int _XCheck::canGetFocus(float x,float y)
{
	if(m_isInited == 0) return 0;	//如果没有初始化直接退出
	if(m_isActive == 0) return 0;		//没有激活的控件不接收控制
	if(m_isVisiable == 0) return 0;	//如果不可见直接退出
	if(m_isEnable == 0) return 0;		//如果无效则直接退出
	if(m_withCaption != 0)
	{
		if(m_mouseClickArea.isInRect(x,y) || m_nowMouseRect.isInRect(x,y)) return 1;
	}else
	{
		if(m_nowMouseRect.isInRect(x,y)) return 1;
	}
	return 0;
}

int _XCheck::canLostFocus(float x,float y)
{
	return 1;
}

int _XCheck::mouseProc(float x,float y,_XMouseState mouseState)
{
	if(m_isInited == 0) return 0;	//如果没有初始化直接退出
	if(m_isActive == 0) return 0;		//没有激活的控件不接收控制
	if(m_isVisiable == 0) return 0;	//如果不可见直接退出
	if(m_isEnable == 0) return 0;		//如果无效则直接退出

	if(mouseState == _XMOUSE_LEFT_BUTTON_UP)
	{
		if(m_withCaption != 0)
		{
			if(m_mouseClickArea.isInRect(x,y) || m_nowMouseRect.isInRect(x,y))
			{//操作有效，执行相关操作
				m_state = !m_state;
				if(m_funMouseUp != NULL) (* m_funMouseUp)(m_pClass,m_objectID);
				if(m_funStateChange != NULL) (* m_funStateChange)(m_pClass,m_objectID);
				return 1;
			}
		}else
		{
			if(m_nowMouseRect.isInRect(x,y))
			{//操作有效，执行相关操作
				m_state = !m_state;
				if(m_funMouseUp != NULL) (* m_funMouseUp)(m_pClass,m_objectID);
				if(m_funStateChange != NULL) (* m_funStateChange)(m_pClass,m_objectID);
				return 1;
			}
		}
	}
	return 0;
}

int _XCheck::keyboardProc(int keyOrder,_XKeyState keyState)
{//回车或者空格可以改变这个控件的选择状态
	if(m_isInited == 0) return 0;	//如果没有初始化直接退出
	if(m_isActive == 0) return 0;		//没有激活的控件不接收控制
	if(m_isVisiable == 0) return 0;	//如果不可见直接退出
	if(m_isEnable == 0) return 0;		//如果无效则直接退出

	if(keyState == _XKEY_STATE_UP)
	{//按键弹起时才作相应
		if((keyOrder == SDLK_RETURN || keyOrder == SDLK_SPACE) && m_isBeChoose != 0)
		{//按下空格键或者回车键都有效
			m_state = !m_state;
			if(m_funStateChange != NULL)(* m_funStateChange)(m_pClass,m_objectID);
		}
	}
	return 1;
}

void _XCheck::setCallbackFun(void (* funInit)(void *,int),
							 void (* funRelease)(void *,int),
							 void (* funMouseOn)(void *,int),
							 void (* funMouseDown)(void *,int),
							 void (* funMouseUp)(void *,int),
							 void (*funStateChange)(void *,int),void *pClass)
{//实际上这里可以不用先判断，可以允许调用者注销
	if(funInit != NULL) m_funInit = funInit;				//控件初始化的时候调用，（这个目前没有实际生效）
	if(funRelease != NULL) m_funRelease = funRelease;				//控件注销的时候调用，（这个目前没有实际生效）
	if(funMouseOn != NULL) m_funMouseOn = funMouseOn;				//鼠标悬浮时调用
	if(funMouseDown != NULL) m_funMouseDown = funMouseDown;			//鼠标按下时调用		
	if(funMouseUp != NULL) m_funMouseUp = funMouseUp;				//鼠标弹起时调用
	if(funStateChange != NULL) m_funStateChange = funStateChange;			//控件状态改变时调用
	m_pClass = pClass;
}

int _XCheck::setACopy(const _XCheck &temp)
{
	if(& temp == this) return 1;	//防止自身赋值
	if(temp.m_isInited == 0) return 0;
	if(_XControlBasic::setACopy(temp) == 0) return 0;

	if(m_isInited == 0)
	{
		_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_CHECK);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	}

	m_isInited = temp.m_isInited;					//进度条是否被初始化
	if(m_caption.setACopy(temp.m_caption) == 0)	return 0;		//进度条的标题
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif

	m_checkChoosed = temp.m_checkChoosed;			//选择按钮选中的贴图
	m_checkDischoose = temp.m_checkDischoose;		//选择按钮未选中的贴图
	m_checkDisableChoosed = temp.m_checkDisableChoosed;	//无效状态下选择按钮选中的贴图
	m_checkDisableDischoose = temp.m_checkDisableDischoose;	//无效状态下选择按钮未选中的贴图

	m_funInit = temp.m_funInit;				//控件初始化的时候调用，（这个目前没有实际生效）
	m_funRelease = temp.m_funRelease;				//控件注销的时候调用，（这个目前没有实际生效）
	m_funMouseOn = temp.m_funMouseOn;				//鼠标悬浮时调用，（这个目前没有实际生效）
	m_funMouseDown = temp.m_funMouseDown;			//鼠标按下时调用，（这个目前没有实际生效）		
	m_funMouseUp = temp.m_funMouseUp;				//鼠标弹起时调用
	m_funStateChange = temp.m_funStateChange;			//控件状态改变时调用
	m_pClass = temp.m_pClass;

	m_withCaption = temp.m_withCaption;
	m_sprite.setACopy(temp.m_sprite);			//用于显示贴图的精灵
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
#endif
	m_textPosition = temp.m_textPosition;		//文字的相对位置
	m_textSize = temp.m_textSize;			//文字的尺寸

	m_state = temp.m_state;				//复选框的选择状态
	m_textColor = temp.m_textColor;				//复选框的选择状态
	m_mouseClickArea = temp.m_mouseClickArea;	//鼠标点击的响应范围

	return 1;
}

int _XCheck::isInRect(float x,float y)
{
	if(m_isInited == 0) return 0;
	return getIsInRect(_XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
}

_XVector2 _XCheck::getBox(int order)
{
	_XVector2 ret;
	ret.set(0.0f,0.0f);
	if(m_isInited == 0) return ret;
	float left = 0.0f;
	float right = 0.0f;
	float top = 0.0f;
	float bottom = 0.0f;
	if(m_withCaption != 0)
	{
		if(m_nowMouseRect.left <= m_mouseClickArea.left) left = m_nowMouseRect.left;
		else left = m_mouseClickArea.left;
		if(m_nowMouseRect.top <= m_mouseClickArea.top) top = m_nowMouseRect.top;
		else top = m_mouseClickArea.top;
		if(m_nowMouseRect.right <= m_mouseClickArea.right) right = m_mouseClickArea.right;
		else right = m_nowMouseRect.right;
		if(m_nowMouseRect.bottom <= m_mouseClickArea.bottom) bottom = m_mouseClickArea.bottom;
		else bottom = m_nowMouseRect.bottom;
	}else
	{
		left = m_nowMouseRect.left;
		top = m_nowMouseRect.top;
		right = m_mouseClickArea.right;
		bottom = m_mouseClickArea.bottom;
	}

	if(order == 0) ret.set(left,top);else
	if(order == 1) ret.set(right,top);else
	if(order == 2) ret.set(right,bottom);else
	if(order == 3) ret.set(left,bottom);

	return ret;
}
