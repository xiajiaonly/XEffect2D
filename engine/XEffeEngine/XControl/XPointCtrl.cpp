//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XPointCtrl.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

_XBool _XPointCtrl::init(const _XVector2& position,const _XFontUnicode *font)
{
	if(m_isInited) return false;
	m_position = position;
	if(font == NULL)
	{
		m_withFont = false;
	}else
	{
		m_withFont = true;
		m_font.setACopy(*font);
		m_font.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
		m_font.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&m_font);
#endif
	}
	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_POINTCTRL);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	updateData();

	m_isVisiable = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	m_isInited = true;
	return true;
}
void _XPointCtrl::draw()
{
	if(!m_isInited) return;
	if(!m_isVisiable) return;	//如果不可见直接退出

	switch(m_ctrlMode)
	{
	case CTRL_MODE_COMPLETE:
		//描绘网格
		if(m_withRange)
		{
			float w = m_range.getWidth() / XPOINT_CTRL_LINE_SUM;
			float h = m_range.getHeight() / XPOINT_CTRL_LINE_SUM;
			for(int i = 1;i < XPOINT_CTRL_LINE_SUM;++ i)
			{
				drawLine(m_range.left + w * i,m_range.top + 12.0f,m_range.left + w * i,m_range.bottom - 12.0f,1,1.0f,1.0f,1.0f,0.5f,1);
				drawLine(m_range.left + 12.0f,m_range.top + h * i,m_range.right - 12.0f,m_range.top + h * i,1,1.0f,1.0f,1.0f,0.5f,1);
			}
		}
	case CTRL_MODE_NORMAL:
		//描绘边框和标尺
		if(m_withRange) 
		{
			drawRect(m_range);
			float w = m_range.getWidth() / XPOINT_CTRL_LINE_SUM;
			float h = m_range.getHeight() / XPOINT_CTRL_LINE_SUM;
			for(int i = 1;i < XPOINT_CTRL_LINE_SUM;++ i)
			{
				drawLine(m_range.left + w * i,m_range.top,m_range.left + w * i,m_range.top + 10.0f);
				drawLine(m_range.left + w * i,m_range.bottom,m_range.left + w * i,m_range.bottom - 10.0f);
				drawLine(m_range.left,m_range.top + h * i,m_range.left + 10.0f,m_range.top + h * i);
				drawLine(m_range.right,m_range.top + h * i,m_range.right - 10.0f,m_range.top + h * i);
			}
		}
	case CTRL_MODE_SIMPLE:
		break;
	}
	//描绘点原的位置
	if(m_isDown)
	{
		drawLine(m_position.x - m_truePixelSize.x,m_position.y,m_position.x + m_truePixelSize.x,m_position.y,2,1.0f,0.0f,0.0f);
		drawLine(m_position.x,m_position.y - m_truePixelSize.y,m_position.x,m_position.y + m_truePixelSize.y,2,1.0f,0.0f,0.0f);
	}else
	{
		drawLine(m_position.x - m_truePixelSize.x,m_position.y,m_position.x + m_truePixelSize.x,m_position.y,1,1.0f,1.0f,1.0f);
		drawLine(m_position.x,m_position.y - m_truePixelSize.y,m_position.x,m_position.y + m_truePixelSize.y,1,1.0f,1.0f,1.0f);
	}
	if(m_withFont) m_font.draw();	//显示文字提示
}
_XBool _XPointCtrl::mouseProc(float x,float y,_XMouseState mouseState)
{
	if(!m_isInited) return XFalse;	//如果没有初始化直接退出
	if(!m_isActive) return XFalse;		//没有激活的控件不接收控制
	if(!m_isVisiable) return XFalse;	//如果不可见直接退出
	if(!m_isEnable) return XFalse;		//如果无效则直接退出
	switch(mouseState)
	{
	case MOUSE_MOVE:
		if(m_isDown) 
		{
			setPosition(x,y);
			if(m_funDataChange != NULL) m_funDataChange(m_pClass,m_objectID);
		}
		break;
	case MOUSE_LEFT_BUTTON_DOWN:
		//鼠标按下
		if(_XVector2(x,y).getLengthSqure(m_position) < 100.0f) m_isDown = true;
		break;
	case MOUSE_LEFT_BUTTON_UP:
		if(m_isDown) m_isDown = false;
		break;
	}
	return XTrue;
}
_XBool _XPointCtrl::setACopy(const _XPointCtrl & temp)
{
	if(& temp == this) return XTrue;	//防止自身赋值
	if(!temp.m_isInited) return XFalse;
	if(!_XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_POINTCTRL);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	}

	m_isInited = temp.m_isInited;
	m_position = temp.m_position;	//控件的位置
	m_size = temp.m_size;		//大小
	m_pixelSize = temp.m_pixelSize;	//像素大小
	m_truePixelSize = temp.m_truePixelSize;	//真实的像素尺寸
	if(!m_font.setACopy(temp.m_font))	 return XFalse;
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_font);
#endif
	memcpy(m_textStr,temp.m_textStr,64);		//显示的字符串

	m_withFont = temp.m_withFont;
	m_isDown = temp.m_isDown;		//是否被鼠标拾取
	m_withRange = temp.m_withRange;	//范围设置是否有效
	m_range = temp.m_range;			//可以移动的范围
	m_withMap = temp.m_withMap;		//是否进行映射
	m_mapRange = temp.m_mapRange;	//映射的范围
	m_mapValue = temp.m_mapValue;	//映射的范围

	m_ctrlMode = temp.m_ctrlMode;	//控件模式

	m_funDataChange = temp.m_funDataChange;
	m_pClass = temp.m_pClass;	

	if(temp.m_withFont)
	{
		m_font.setACopy(temp.m_font);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&m_font);
#endif
	}

	return XTrue;
}
void _XPointCtrl::release()
{
	_XControlManager::GetInstance().decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(this);
#endif
}
void _XPointCtrl::updateData()
{
	m_truePixelSize = m_pixelSize * m_size;
	if(m_withRange)
	{//控制在范围内
		if(m_position.x < m_range.left) m_position.x = m_range.left;
		if(m_position.x > m_range.right) m_position.x = m_range.right;
		if(m_position.y < m_range.top) m_position.y = m_range.top;
		if(m_position.y > m_range.bottom) m_position.y = m_range.bottom;
	}
	if(m_withMap)
	{//这里进行映射
		m_mapValue.x = maping1D(m_position.x,m_range.left,m_range.right,m_mapRange.left,m_mapRange.right);
		m_mapValue.y = maping1D(m_position.y,m_range.top,m_range.bottom,m_mapRange.top,m_mapRange.bottom);
		if(m_withFont)
		{
			sprintf(m_textStr,"(%.0f,%.0f)",m_mapValue.x,m_mapValue.y);
			m_font.setString(m_textStr);
			m_font.setPosition(m_position);
		}
	}else
	{
		if(m_withFont)
		{
			sprintf(m_textStr,"(%.0f,%.0f)",m_position.x,m_position.y);
			m_font.setString(m_textStr);
			m_font.setPosition(m_position);
		}
	}
}