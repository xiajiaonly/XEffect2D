//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include"XSlider.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

_XSliderTexture::_XSliderTexture()
:m_isInited(0)
,sliderLineNormal(NULL)			
,sliderLineDown(NULL)			
,sliderLineOn(NULL)					
,sliderLineDisable(NULL)			
,sliderButtonNormal(NULL)			
,sliderButtonDown(NULL)			
,sliderButtonOn(NULL)					
,sliderButtonDisable(NULL)			
{
}

_XSliderTexture::~_XSliderTexture()
{
	release();
}

int _XSliderTexture::init(const char *LNormal,const char *LDown,const char *LOn,const char *LDisable,
						  const char *BNormal,const char *BDown,const char *BOn,const char *BDisable,int resoursePosition)
{
	if(m_isInited != 0) return 0;
	if(LNormal == NULL || BNormal == NULL) return 0;

	int ret = 1;
	//分别载入资源	
	if((sliderLineNormal = createATextureData(LNormal,resoursePosition)) == NULL) ret = 0;
	if(LDown != NULL && ret != 0 &&
		(sliderLineDown = createATextureData(LDown,resoursePosition)) == NULL) ret = 0;
	if(LOn != NULL && ret != 0 &&
		(sliderLineOn = createATextureData(LOn,resoursePosition)) == NULL) ret = 0;
	if(LDisable != NULL && ret != 0 &&
		(sliderLineDisable = createATextureData(LDisable,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(sliderButtonNormal = createATextureData(BNormal,resoursePosition)) == NULL) ret = 0;
	if(BDown != NULL && ret != 0 &&
		(sliderButtonDown = createATextureData(BDown,resoursePosition)) == NULL) ret = 0;
	if(BOn != NULL && ret != 0 &&
		(sliderButtonOn = createATextureData(BOn,resoursePosition)) == NULL) ret = 0;
	if(BDisable != NULL && ret != 0 &&
		(sliderButtonDisable = createATextureData(BDisable,resoursePosition)) == NULL) ret = 0;
	if(ret == 0)
	{
		XDELETE(sliderLineNormal);
		XDELETE(sliderLineDown);
		XDELETE(sliderLineOn);
		XDELETE(sliderLineDisable);
		XDELETE(sliderButtonNormal);
		XDELETE(sliderButtonDown);
		XDELETE(sliderButtonOn);
		XDELETE(sliderButtonDisable);
		return 0;
	}

	m_isInited = 1;
	return 1;
}

void _XSliderTexture::release()
{
	if(m_isInited == 0) return;
	XDELETE(sliderLineNormal);
	XDELETE(sliderLineDown);
	XDELETE(sliderLineOn);
	XDELETE(sliderLineDisable);
	XDELETE(sliderButtonNormal);
	XDELETE(sliderButtonDown);
	XDELETE(sliderButtonOn);
	XDELETE(sliderButtonDisable);
	m_isInited = 0;
}

_XSlider::_XSlider()
:m_isInited(0)
,m_nowSliderState(_XSLIDER_STATE_NORMAL)
,m_typeVorH(_XSLIDER_TYPE_VERTICAL)
,m_sliderLineNormal(NULL)			
,m_sliderLineDown(NULL)			
,m_sliderLineOn(NULL)			
,m_sliderLineDisable(NULL)		
,m_sliderButtonNormal(NULL)			
,m_sliderButtonDown(NULL)	
,m_sliderButtonOn(NULL)				
,m_sliderButtonDisable(NULL)		
,m_funInit(NULL)	
,m_funRelease(NULL)	
,m_funMouseOn(NULL)
,m_funMouseDown(NULL)
,m_funMouseUp(NULL)	
,m_funValueChange(NULL)
,m_pClass(NULL)
,m_minValue(0)	
,m_maxValue(100)	
,m_nowValue(0)		
,m_mouseFlyArea(50,50)	//这个值使用一个系统设定，这个值不能在代码中进行修改，这是不理想的，但是考虑到它存在的无形性，所以默认允许这样操作
{
}

_XSlider::~_XSlider()
{
	release();
}
	
void _XSlider::release()
{
	_XControlManager::GetInstance().decreaseAObject(this);	//注销这个物件
	if(m_funRelease != NULL) (* m_funRelease)(m_pClass,m_objectID);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(this);
#endif
}

int _XSlider::init(const _XVector2& position,	//控件所在的位置
		const _XRect& Area,				//控件的鼠标响应范围
		const _XRect& buttonArea,		//滑动块的鼠标响应范围
		const _XSliderTexture &tex,_XSliderType type,float max,float min)
{
	if(m_isInited != 0) return 0;
	if(Area.getHeight() <= 0 || Area.getWidth() <= 0) return 0;	//空间必须要有一个响应区间，不然会出现除零错误
	if(tex.sliderLineNormal == NULL || tex.sliderButtonNormal == NULL) return 0;	//按键的普通状态的贴图不能为空，否则直接返回错误
	if(min >= max) return 0;
	m_mouseRect = Area;
	m_buttonArea = buttonArea;
	m_position = position;

	m_typeVorH = type;

	m_sliderLineNormal = tex.sliderLineNormal;
	m_sliderButtonNormal = tex.sliderButtonNormal;
	if(tex.sliderLineDown != NULL) m_sliderLineDown = tex.sliderLineDown;
	else m_sliderLineDown = tex.sliderLineNormal;
	if(tex.sliderLineOn != NULL) m_sliderLineOn = tex.sliderLineOn;
	else m_sliderLineOn = tex.sliderLineNormal;
	if(tex.sliderLineDisable != NULL) m_sliderLineDisable = tex.sliderLineDisable;
	else m_sliderLineDisable = tex.sliderLineNormal;

	if(tex.sliderButtonDown != NULL) m_sliderButtonDown = tex.sliderButtonDown;
	else m_sliderButtonDown = tex.sliderButtonNormal;
	if(tex.sliderButtonOn != NULL) m_sliderButtonOn = tex.sliderButtonOn;
	else m_sliderButtonOn = tex.sliderButtonNormal;
	if(tex.sliderButtonDisable != NULL) m_sliderButtonDisable = tex.sliderButtonDisable;
	else m_sliderButtonDisable = tex.sliderButtonNormal;

	m_size.set(1.0f,1.0f);

	m_minValue = min;		
	m_maxValue = max;	
	m_nowValue = min;	
	m_upValue = min;
	m_dataChanged = 0;
	m_keyOneValue = (m_maxValue - m_minValue) / 20.0f;

	//线的位置不需要计算
	m_lineSprite.init(m_sliderLineNormal->texture.m_w,m_sliderLineNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_lineSprite);
#endif
	m_lineSprite.setPosition(m_position);
	m_lineSprite.setSize(m_size);
	m_lineSprite.setIsTransformCenter(POINT_LEFT_TOP);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x +  + m_mouseRect.right * m_size.x,m_position.y +  + m_mouseRect.bottom * m_size.y);

	m_buttonSprite.init(m_sliderButtonNormal->texture.m_w,m_sliderButtonNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_buttonSprite);
#endif
	m_buttonSprite.setSize(m_size);
	m_buttonSprite.setIsTransformCenter(POINT_LEFT_TOP);
	updateButtonData();

	if(m_funInit != NULL) (* m_funInit)(m_pClass,m_objectID);

	m_isVisiable = 1;
	m_isEnable = 1;
	m_isActive = 1;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_SLIDER);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = 1;
	return 1;
}

void _XSlider::setPosition(float x,float y)
{
	if(m_isInited == 0) return;	//如果没有初始化直接退出
	m_position.set(x,y);
	updateButtonData();
	m_lineSprite.setPosition(m_position);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x +  + m_mouseRect.right * m_size.x,m_position.y +  + m_mouseRect.bottom * m_size.y);
	//还需要调用鼠标函数
	mouseProc(m_upMousePoint.x,m_upMousePoint.y,_XMOUSE_MOVE);
}

void _XSlider::updateButtonData()
{
	//计算滑块的位置
	if(m_typeVorH == _XSLIDER_TYPE_VERTICAL)
	{//垂直类型
		m_nowButtonPosition.set(m_position.x + (m_mouseRect.left - m_buttonArea.left) * m_size.y + (m_mouseRect.getWidth() - m_buttonArea.getWidth()) * m_size.x * 0.5f,
			m_position.y + (m_mouseRect.top - m_buttonArea.top) * m_size.y + 
			(m_mouseRect.getHeight() - m_buttonArea.getHeight()) * m_size.y * (m_nowValue - m_minValue)/(m_maxValue - m_minValue));
	}else
	if(m_typeVorH == _XSLIDER_TYPE_HORIZONTAL)
	{//水平类型
		m_nowButtonPosition.set(m_position.x + (m_mouseRect.left - m_buttonArea.left) * m_size.y + 
			(m_mouseRect.getWidth() - m_buttonArea.getWidth()) * m_size.x * (m_nowValue - m_minValue)/(m_maxValue - m_minValue),
			m_position.y + (m_mouseRect.top - m_buttonArea.top) * m_size.y + (m_mouseRect.getHeight() - m_buttonArea.getHeight()) * m_size.y * 0.5f);
	}
	m_buttonSprite.setPosition(m_nowButtonPosition);
//	m_nowButtonArea.set(m_nowButtonPosition.x + m_buttonArea.left * m_size.x,m_nowButtonPosition.y + m_buttonArea.top * m_size.y,
//		m_nowButtonPosition.x + m_buttonArea.right * m_size.x,m_nowButtonPosition.y + m_buttonArea.bottom * m_size.y);
}

void _XSlider::setSize(float x,float y)
{
	if(x <= 0 && y <= 0) return;
	if(m_isInited == 0) return;	//如果没有初始化直接退出
	m_size.set(x,y);
	m_lineSprite.setSize(m_size);
	updateButtonData();
	m_buttonSprite.setSize(m_size);

	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x +  + m_mouseRect.right * m_size.x,m_position.y +  + m_mouseRect.bottom * m_size.y);
	//还需要调用鼠标函数
	mouseProc(m_upMousePoint.x,m_upMousePoint.y,_XMOUSE_MOVE);
}

void _XSlider::draw()
{
	if(m_isInited == 0) return ;	//如果没有初始化直接退出
	if(m_isVisiable == 0) return;	//如果不可见直接退出

	if(m_nowSliderState == _XSLIDER_STATE_NORMAL)
	{
		m_lineSprite.draw(m_sliderLineNormal);
		m_buttonSprite.draw(m_sliderButtonNormal);
	}else
	if(m_nowSliderState == _XSLIDER_STATE_DOWN)
	{
		m_lineSprite.draw(m_sliderLineDown);
		m_buttonSprite.draw(m_sliderButtonDown);
	}else
	if(m_nowSliderState == _XSLIDER_STATE_ON)
	{
		m_lineSprite.draw(m_sliderLineOn);
		m_buttonSprite.draw(m_sliderButtonOn);
	}else
	if(m_nowSliderState == _XSLIDER_STATE_DISABLE)
	{
		m_lineSprite.draw(m_sliderLineDisable);
		m_buttonSprite.draw(m_sliderButtonDisable);
	}
}

int _XSlider::canGetFocus(float x,float y)
{
	if(m_isInited == 0) return 0;	//如果没有初始化直接退出
	if(m_isActive == 0) return 0;		//没有激活的控件不接收控制
	if(m_isVisiable == 0) return 0;	//如果不可见直接退出
	if(m_isEnable == 0) return 0;		//如果无效则直接退出
	return isInRect(x,y);
}

int _XSlider::canLostFocus(float x,float y)
{
	return 1;
}

int _XSlider::mouseProc(float x,float y,_XMouseState mouseState)
{
	m_upMousePoint.set(x,y);
	if(m_isInited == 0) return 0;		//如果没有初始化直接退出
	if(m_isActive == 0) return 0;		//没有激活的控件不接收控制
	if(m_isVisiable == 0) return 0;	//如果不可见直接退出
	if(m_isEnable == 0) return 0;		//如果无效则直接退出
	//SliderLine,滚动条的线的事件

	if(m_nowSliderState == _XSLIDER_STATE_DOWN)
	{//注意这里是滑动条拖动时，为了是超出范围的拖动仍然有效，所以这个这个特殊处理
		_XRect tempFly(m_nowMouseRect.left - m_mouseFlyArea.x,m_nowMouseRect.top - m_mouseFlyArea.y,
			m_nowMouseRect.right + m_mouseFlyArea.x,m_nowMouseRect.bottom + m_mouseFlyArea.y);
		if(tempFly.isInRect(x,y))
		{//拖动(注意如果这里拖动超出范围，则使用最后一次的有效值)
			if(mouseState == _XMOUSE_MOVE)
			{//重新计算Slider的当前值
				if(m_typeVorH == _XSLIDER_TYPE_VERTICAL)
				{//垂直类型
					m_nowValue = m_minValue + (m_maxValue - m_minValue) * (y - m_position.y - m_mouseRect.top * m_size.y) / (m_mouseRect.getHeight() * m_size.y); 
					if(m_nowValue < m_minValue) m_nowValue = m_minValue;
					if(m_nowValue > m_maxValue) m_nowValue = m_maxValue;
				}else
				if(m_typeVorH == _XSLIDER_TYPE_HORIZONTAL)
				{//水平类型
					m_nowValue = m_minValue + (m_maxValue - m_minValue) * (x - m_position.x - m_mouseRect.left * m_size.x) / (m_mouseRect.getWidth() * m_size.x); 
					if(m_nowValue < m_minValue) m_nowValue = m_minValue;
					if(m_nowValue > m_maxValue) m_nowValue = m_maxValue;
				}
				updateButtonData();
				m_dataChanged = 1;	//标记数值发生改变
				if(m_funMouseMove != NULL) (* m_funMouseMove)(m_pClass,m_objectID);
			}else
			if(mouseState == _XMOUSE_LEFT_BUTTON_UP)
			{//这个弹起事件是在按钮上按下的弹起事件
				m_nowSliderState = _XSLIDER_STATE_ON;
				if(m_funMouseUp != NULL)(* m_funMouseUp)(m_pClass,m_objectID);
				if(m_funValueChange != NULL && m_dataChanged != 0)
				{
					m_dataChanged = 0;
					(* m_funValueChange)(m_pClass,m_objectID);
				}
			}
		}else
		{
			m_nowSliderState = _XSLIDER_STATE_NORMAL;
			if(m_funValueChange != NULL && m_dataChanged != 0)
			{
				m_dataChanged = 0;
				(* m_funValueChange)(m_pClass,m_objectID);
			}
		}
	}
	if(m_nowMouseRect.isInRect(x,y))
	{//鼠标动作在范围内
		if(mouseState == _XMOUSE_MOVE)
		{
			if(m_nowSliderState == _XSLIDER_STATE_NORMAL)
			{//悬浮
				m_nowSliderState = _XSLIDER_STATE_ON;
				if(m_funMouseOn != NULL) (* m_funMouseOn)(m_pClass,m_objectID);
			}
		}else
		if(mouseState == _XMOUSE_LEFT_BUTTON_DOWN && (m_nowSliderState == _XSLIDER_STATE_NORMAL || m_nowSliderState == _XSLIDER_STATE_ON))
		{
			m_upValue = m_nowValue;

			//重新计算Slider的当前值
			if(m_typeVorH == _XSLIDER_TYPE_VERTICAL)
			{//垂直类型
				m_nowValue = m_minValue + (m_maxValue - m_minValue) * (y - m_position.y - m_mouseRect.top * m_size.y) / (m_mouseRect.getHeight() * m_size.y); 
				if(m_nowValue < m_minValue) m_nowValue = m_minValue;
				if(m_nowValue > m_maxValue) m_nowValue = m_maxValue;
			}else
			if(m_typeVorH == _XSLIDER_TYPE_HORIZONTAL)
			{//水平类型
				m_nowValue = m_minValue + (m_maxValue - m_minValue) * (x - m_position.x - m_mouseRect.left * m_size.x) / (m_mouseRect.getWidth() * m_size.x); 
				if(m_nowValue < m_minValue) m_nowValue = m_minValue;
				if(m_nowValue > m_maxValue) m_nowValue = m_maxValue;
			}
			updateButtonData();
			m_dataChanged = 1;	//标记数值发生改变
			m_nowSliderState = _XSLIDER_STATE_DOWN;
			if(m_funMouseDown != NULL) (* m_funMouseDown)(m_pClass,m_objectID);
		}else
		if(mouseState == _XMOUSE_LEFT_BUTTON_UP && m_nowSliderState == _XSLIDER_STATE_DOWN)
		{//这里会触发数值改变事件(这个弹起事件应该是在线上按下的弹起)
			m_nowSliderState = _XSLIDER_STATE_ON;
			if(m_funMouseUp != NULL) (* m_funMouseUp)(m_pClass,m_objectID);

			if(m_funValueChange != NULL && m_dataChanged != 0)
			{
				m_dataChanged = 0;
				(* m_funValueChange)(m_pClass,m_objectID);
			}
		}
	}else
	{
		if(m_nowSliderState == _XSLIDER_STATE_ON) m_nowSliderState = _XSLIDER_STATE_NORMAL;
	}
	return 1;
}

int _XSlider::keyboardProc(int keyOrder,_XKeyState keyState)
{
	if(m_isInited == 0) return 0;	//如果没有初始化直接退出
	if(m_isActive == 0) return 0;		//没有激活的控件不接收控制
	if(m_isVisiable == 0) return 0;	//如果不可见直接退出
	if(m_isEnable == 0) return 0;		//如果无效则直接退出

	if(keyState == _XKEY_STATE_UP)
	{//按键弹起时才作相应
		if(m_nowSliderState == _XSLIDER_STATE_NORMAL && m_isBeChoose != 0)	//只能在普通状态下才能使用快捷键，方式重复操作
		{//除了按钮的无效状态之外,其他状态都可以响应快捷键信息
			bool flagChanged = false;
			if(m_typeVorH == _XSLIDER_TYPE_VERTICAL)
			{//垂直
				if(keyOrder ==  SDLK_UP && m_nowValue > m_minValue)
				{
					if(m_nowValue - m_keyOneValue < m_minValue) m_nowValue = m_minValue;
					else m_nowValue -= m_keyOneValue;
					flagChanged = true;
				}else
				if(keyOrder ==  SDLK_DOWN && m_nowValue < m_maxValue)
				{
					if(m_nowValue + m_keyOneValue > m_maxValue) m_nowValue = m_maxValue;
					else m_nowValue += m_keyOneValue;
					flagChanged = true;
				}
			}else
			{//水平
				if(keyOrder ==  SDLK_LEFT && m_nowValue > m_minValue)
				{
					if(m_nowValue - m_keyOneValue < m_minValue) m_nowValue = m_minValue;
					else m_nowValue -= m_keyOneValue;
					flagChanged = true;
				}else
				if(keyOrder ==  SDLK_RIGHT && m_nowValue < m_maxValue)
				{
					if(m_nowValue + m_keyOneValue > m_maxValue) m_nowValue = m_maxValue;
					else m_nowValue += m_keyOneValue;
					flagChanged = true;
				}
			}
			if(flagChanged)
			{
				updateButtonData();
				if(m_funValueChange != NULL)
				{
					m_dataChanged = 1;
					(* m_funValueChange)(m_pClass,m_objectID);
				}
			}
		}
	}
	return 1;
}

void _XSlider::setCallbackFun(void (* funInit)(void *,int),
							  void (* funRelease)(void *,int),
							  void (* funMouseOn)(void *,int),
							  void (* funMouseDown)(void *,int),
							  void (* funMouseUp)(void *,int),
							  void (*funValueChange)(void *,int),
							  void (*funMouseMove)(void *,int),
							  void *pClass)
{
	if(funInit != NULL) m_funInit = funInit;
	if(funRelease != NULL) m_funRelease = funRelease;
	if(funMouseOn != NULL) m_funMouseOn = funMouseOn;
	if(funMouseDown != NULL) m_funMouseDown = funMouseDown;
	if(funMouseUp != NULL) m_funMouseUp = funMouseUp;
	if(funValueChange != NULL) m_funValueChange = funValueChange;
	if(funMouseMove != NULL) m_funMouseMove = funMouseMove;
	m_pClass = pClass;
}

void _XSlider::setNowValue(float temp)
{
	if(temp < m_minValue) temp = m_minValue;
	if(temp > m_maxValue) temp = m_maxValue;
	if(temp == m_nowValue) return;
	m_nowValue = temp;
	updateButtonData();
	if(m_funValueChange != NULL)
	{
		m_dataChanged = 0;
		(* m_funValueChange)(m_pClass,m_objectID);
	}
}

void _XSlider::setRange(float max,float min)
{
	if(min >= max) return;	//这个数据是不合法的
	m_maxValue = max;
	m_minValue = min;
	m_keyOneValue = (m_maxValue - m_minValue) / 20.0f;

	if(m_nowValue < m_minValue) m_nowValue = m_minValue;
	if(m_nowValue > m_maxValue) m_nowValue = m_maxValue;
	updateButtonData();
	if(m_funValueChange != NULL)
	{
		m_dataChanged = 0;
		(* m_funValueChange)(m_pClass,m_objectID);
	}
}

int _XSlider::setACopy(const _XSlider &temp)			//建立一个副本
{
	if(&temp == this) return 0;
	if(temp.m_isInited == 0) return 0;
	if(_XControlBasic::setACopy(temp) == 0) return 0;
	if(m_isInited == 0)
	{
		_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_SLIDER);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	}
	m_isInited = temp.m_isInited;
	m_nowSliderState = temp.m_nowSliderState;			//滑动条的状态

	m_typeVorH = temp.m_typeVorH;						//滑动条的类型

	m_sliderLineNormal = temp.m_sliderLineNormal;			//滑动条线的普通状态
	m_sliderLineDown = temp.m_sliderLineDown;			//滑动条线的按下状态
	m_sliderLineOn = temp.m_sliderLineOn;				//滑动条线的悬浮状态
	m_sliderLineDisable = temp.m_sliderLineDisable;			//滑动条线的无效状态
	m_sliderButtonNormal = temp.m_sliderButtonNormal;		//滑动条按钮的普通状态
	m_sliderButtonDown = temp.m_sliderButtonDown;			//滑动条按钮的按下状态
	m_sliderButtonOn = temp.m_sliderButtonOn;			//滑动条按钮的悬浮状态
	m_sliderButtonDisable = temp.m_sliderButtonDisable;		//滑动条按钮的无效状态

	m_funInit = temp.m_funInit;		//初始化
	m_funRelease = temp.m_funRelease;		//资源释放
	m_funMouseOn = temp.m_funMouseOn;		//鼠标悬浮
	m_funMouseDown = temp.m_funMouseDown;	//鼠标按下
	m_funMouseUp = temp.m_funMouseUp;		//鼠标弹起
	m_funMouseMove = temp.m_funMouseMove;	//鼠标拖动花条时调用
	m_funValueChange = temp.m_funValueChange;	//滑动条的数值变化
	m_pClass = temp.m_pClass;				//回调函数的参数

	m_buttonArea = temp.m_buttonArea;		//滑动条中按钮的有效范围,这个范围是按钮的相对图片的响应范围，需要注意
//	m_nowButtonArea = temp.m_nowButtonArea;		//滑动条中按钮的有效范围,这个范围是按钮的相对图片的响应范围，需要注意
	m_buttonSprite.setACopy(temp.m_buttonSprite);	//用于显示贴图的精灵
	m_lineSprite.setACopy(temp.m_lineSprite);		//用于显示贴图的精灵
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_buttonSprite);
	_XObjectManager::GetInstance().decreaseAObject(&m_lineSprite);
#endif

	m_upMousePoint = temp.m_upMousePoint;		//上次鼠标在滚动条的位置
	m_mouseFlyArea = temp.m_mouseFlyArea;		//鼠标拖曳是飞出的响应范围
	m_nowButtonPosition = temp.m_nowButtonPosition;//当前滑块按钮的位置
	m_minValue = temp.m_minValue;			//滑动条的最大值
	m_maxValue = temp.m_maxValue;			//滑动条的最小值
	m_nowValue = temp.m_nowValue;			//滑动条的当前值
	m_upValue = temp.m_upValue;			//滑动条的上一次值(定义这个变量的目的在于如果拖动滑块超出范围之后可以恢复到拖动之前得知，但是目前出于简单的考虑，不做这个动作)
	m_dataChanged = temp.m_dataChanged;			//滑动条的数值是否有被修改
	m_keyOneValue = temp.m_keyOneValue;		//按下一次按键的滑动量(注意这个值比需要>=0,否则将会造成错误)

	return 1;
}

int _XSlider::isInRect(float x,float y)
{
	if(m_isInited == 0) return 0;
	return getIsInRect(_XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
}

_XVector2 _XSlider::getBox(int order)
{
	_XVector2 ret;
	ret.set(0.0f,0.0f);
	if(m_isInited == 0) return ret;
	if(order == 0) ret.set(m_nowMouseRect.left,m_nowMouseRect.top);else
	if(order == 1) ret.set(m_nowMouseRect.right,m_nowMouseRect.top);else
	if(order == 2) ret.set(m_nowMouseRect.right,m_nowMouseRect.bottom);else
	if(order == 3) ret.set(m_nowMouseRect.left,m_nowMouseRect.bottom);
	return ret;
}