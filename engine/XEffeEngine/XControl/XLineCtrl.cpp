//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XLineCtrl.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

_XBool _XLineCtrl::init(const _XVector2& position,_XLineCtrlType type,const _XFontUnicode *font)
{
	if(m_isInited) return XFalse;
	m_position = position;
	m_type = type;
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
		_XObjManger.decreaseAObject(&m_font);
#endif
		m_stringFont.setACopy(*font);
		m_stringFont.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
		m_stringFont.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
#if WITH_OBJECT_MANAGER
		_XObjManger.decreaseAObject(&m_stringFont);
#endif
	}
	_XCtrlManger.addACtrl(this);
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif
	updateData();

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	m_isInited = XTrue;
	return XTrue;
}
void _XLineCtrl::draw()
{
	if(!m_isInited ||
		!m_isVisible) return;	//如果不可见直接退出
	switch(m_ctrlMode)
	{
	case CTRL_MODE_COMPLETE:
		//描绘刻度
		if(m_withRange)
		{
			float w = (m_range.y - m_range.x) / XLINE_CTRL_LINE_SUM;
			if(m_type == LINE_CTRL_TYPE_X_D || m_type == LINE_CTRL_TYPE_X_U)
			{
				for(int i = 1;i < XLINE_CTRL_LINE_SUM;++ i)
				{
					drawLine(m_range.x + w * i,m_position.y - 3.0f,m_range.x + w * i,m_position.y + 3.0f,1,
						m_color.fR,m_color.fG,m_color.fB,0.75f * m_color.fA);
				}
			}else
			{
				for(int i = 1;i < XLINE_CTRL_LINE_SUM;++ i)
				{
					drawLine(m_position.x - 3.0f,m_range.x + w * i,m_position.x + 3.0f,m_range.x + w * i,1,
						m_color.fR,m_color.fG,m_color.fB,0.75f * m_color.fA);
				}
			}
		}
	case CTRL_MODE_NORMAL:
		//描绘范围
		if(m_withRange)
		{
			if(m_type == LINE_CTRL_TYPE_X_D || m_type == LINE_CTRL_TYPE_X_U)
			{
				drawLine(m_range.x,m_position.y,m_range.y,m_position.y,1.0f,m_color.fR,m_color.fG,m_color.fB,m_color.fA);
				drawLine(m_range.x,m_position.y - 5.0f,m_range.x,m_position.y + 5.0f,1.0f,m_color.fR,m_color.fG,m_color.fB,m_color.fA);
				drawLine(m_range.y,m_position.y - 5.0f,m_range.y,m_position.y + 5.0f,1.0f,m_color.fR,m_color.fG,m_color.fB,m_color.fA);
			}else
			{
				drawLine(m_position.x,m_range.x,m_position.x,m_range.y,1.0f,m_color.fR,m_color.fG,m_color.fB,m_color.fA);
				drawLine(m_position.x - 5.0f,m_range.x,m_position.x + 5.0f,m_range.x,1.0f,m_color.fR,m_color.fG,m_color.fB,m_color.fA);
				drawLine(m_position.x - 5.0f,m_range.y,m_position.x + 5.0f,m_range.y,1.0f,m_color.fR,m_color.fG,m_color.fB,m_color.fA);
			}
		}
	case CTRL_MODE_SIMPLE:
		break;
	}
	//描绘点原的位置
	if(m_isDown)
	{
		switch(m_type)
		{
		case LINE_CTRL_TYPE_X_D:
			drawLine(m_position.x,m_position.y,m_position.x - m_truePixelSize.x,m_position.y + m_truePixelSize.y,2,m_color.fR,0.0f,0.0f,m_color.fA);
			drawLine(m_position.x,m_position.y,m_position.x + m_truePixelSize.x,m_position.y + m_truePixelSize.y,2,m_color.fR,0.0f,0.0f,m_color.fA);
			drawLine(m_position.x,m_position.y,m_position.x,m_position.y + 2.0f * m_truePixelSize.y,2,m_color.fR,0.0f,0.0f,m_color.fA);
			break;
		case LINE_CTRL_TYPE_X_U:
			drawLine(m_position.x,m_position.y,m_position.x - m_truePixelSize.x,m_position.y - m_truePixelSize.y,2,m_color.fR,0.0f,0.0f,m_color.fA);
			drawLine(m_position.x,m_position.y,m_position.x + m_truePixelSize.x,m_position.y - m_truePixelSize.y,2,m_color.fR,0.0f,0.0f,m_color.fA);
			drawLine(m_position.x,m_position.y,m_position.x,m_position.y - 2.0f * m_truePixelSize.y,2,m_color.fR,0.0f,0.0f,m_color.fA);
			break;
		case LINE_CTRL_TYPE_Y_L:
			drawLine(m_position.x,m_position.y,m_position.x - m_truePixelSize.x,m_position.y - m_truePixelSize.y,2,m_color.fR,0.0f,0.0f,m_color.fA);
			drawLine(m_position.x,m_position.y,m_position.x - m_truePixelSize.x,m_position.y + m_truePixelSize.y,2,m_color.fR,0.0f,0.0f,m_color.fA);
			drawLine(m_position.x,m_position.y,m_position.x - 2.0f *  m_truePixelSize.x,m_position.y,2,m_color.fR,0.0f,0.0f,m_color.fA);
			break;
		case LINE_CTRL_TYPE_Y_R:
			drawLine(m_position.x,m_position.y,m_position.x + m_truePixelSize.x,m_position.y - m_truePixelSize.y,2,m_color.fR,0.0f,0.0f,m_color.fA);
			drawLine(m_position.x,m_position.y,m_position.x + m_truePixelSize.x,m_position.y + m_truePixelSize.y,2,m_color.fR,0.0f,0.0f,m_color.fA);
			drawLine(m_position.x,m_position.y,m_position.x + 2.0f * m_truePixelSize.x,m_position.y,2,m_color.fR,0.0f,0.0f,m_color.fA);
			break;
		}
	}else
	{
		switch(m_type)
		{
		case LINE_CTRL_TYPE_X_D:
			drawLine(m_position.x,m_position.y,m_position.x - m_truePixelSize.x,m_position.y + m_truePixelSize.y,1,m_color.fR,m_color.fG,m_color.fB,m_color.fA);
			drawLine(m_position.x,m_position.y,m_position.x + m_truePixelSize.x,m_position.y + m_truePixelSize.y,1,m_color.fR,m_color.fG,m_color.fB,m_color.fA);
			drawLine(m_position.x,m_position.y,m_position.x,m_position.y + 2.0f * m_truePixelSize.y,1,m_color.fR,m_color.fG,m_color.fB,m_color.fA);
			break;
		case LINE_CTRL_TYPE_X_U:
			drawLine(m_position.x,m_position.y,m_position.x - m_truePixelSize.x,m_position.y - m_truePixelSize.y,1,m_color.fR,m_color.fG,m_color.fB,m_color.fA);
			drawLine(m_position.x,m_position.y,m_position.x + m_truePixelSize.x,m_position.y - m_truePixelSize.y,1,m_color.fR,m_color.fG,m_color.fB,m_color.fA);
			drawLine(m_position.x,m_position.y,m_position.x,m_position.y + 2.0f * m_truePixelSize.y,1,m_color.fR,m_color.fG,m_color.fB,m_color.fA);
			break;
		case LINE_CTRL_TYPE_Y_L:
			drawLine(m_position.x,m_position.y,m_position.x - m_truePixelSize.x,m_position.y - m_truePixelSize.y,1,m_color.fR,m_color.fG,m_color.fB,m_color.fA);
			drawLine(m_position.x,m_position.y,m_position.x - m_truePixelSize.x,m_position.y + m_truePixelSize.y,1,m_color.fR,m_color.fG,m_color.fB,m_color.fA);
			drawLine(m_position.x,m_position.y,m_position.x - 2.0f *  m_truePixelSize.x,m_position.y,1,m_color.fR,m_color.fG,m_color.fB,m_color.fA);
			break;
		case LINE_CTRL_TYPE_Y_R:
			drawLine(m_position.x,m_position.y,m_position.x + m_truePixelSize.x,m_position.y - m_truePixelSize.y,1,m_color.fR,m_color.fG,m_color.fB,m_color.fA);
			drawLine(m_position.x,m_position.y,m_position.x + m_truePixelSize.x,m_position.y + m_truePixelSize.y,1,m_color.fR,m_color.fG,m_color.fB,m_color.fA);
			drawLine(m_position.x,m_position.y,m_position.x + 2.0f * m_truePixelSize.x,m_position.y,1,m_color.fR,m_color.fG,m_color.fB,m_color.fA);
			break;
		}
	}
	if(m_withFont) m_font.draw();	//显示文字提示
	if(m_withString) m_stringFont.draw();
}
_XBool _XLineCtrl::mouseProc(float x,float y,_XMouseState mouseState)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	switch(mouseState)
	{
	case MOUSE_MOVE:
		if(m_isDown) 
		{
			if(m_type == LINE_CTRL_TYPE_X_D || m_type == LINE_CTRL_TYPE_X_U) setPosition(x,m_position.y);
			else setPosition(m_position.x,y);
			if(m_funDataChange != NULL) m_funDataChange(m_pClass,m_objectID);
		}
		break;
	case MOUSE_LEFT_BUTTON_DOWN:
	case MOUSE_LEFT_BUTTON_DCLICK:
		//鼠标按下
		if(_XVector2(x,y).getLengthSqure(m_position) < 100.0f)
		{
			m_isDown = true;
			m_isBeChoose = XTrue;
		}
		break;
	case MOUSE_LEFT_BUTTON_UP:
		if(m_isDown) 
		{
			m_isDown = false;
		}
		break;
	}
	return XTrue;
}
_XBool _XLineCtrl::setACopy(const _XLineCtrl & temp)
{
	if(& temp == this) return XTrue;	//防止自身赋值
	if(!temp.m_isInited) return XFalse;
	if(!_XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		_XCtrlManger.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
		_XObjManger.addAObject(this);
#endif
	}

	m_isInited = temp.m_isInited;
	m_position = temp.m_position;	//控件的位置
	m_size = temp.m_size;		//大小
	m_pixelSize = temp.m_pixelSize;	//像素大小
	m_truePixelSize = temp.m_truePixelSize;	//真实的像素尺寸
	if(!m_font.setACopy(temp.m_font))	 return XFalse;
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_font);
#endif
	memcpy(m_textStr,temp.m_textStr,64);		//显示的字符串

	m_withFont = temp.m_withFont;
	m_isDown = temp.m_isDown;		//是否被鼠标拾取
	m_withRange = temp.m_withRange;	//范围设置是否有效
	m_range = temp.m_range;			//可以移动的范围
	m_withMap = temp.m_withMap;		//是否进行映射
	m_mapRange = temp.m_mapRange;	//映射的范围
	m_mapValue = temp.m_mapValue;

	m_ctrlMode = temp.m_ctrlMode;
	m_withString = temp.m_withString;			//是否有字符串
	m_showString = temp.m_showString;		//需要显示的字符串
	if(!m_stringFont.setACopy(temp.m_stringFont))	 return XFalse;
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_stringFont);
#endif

	m_funDataChange = temp.m_funDataChange;
	m_pClass = temp.m_pClass;	

	if(temp.m_withFont)
	{
		m_font.setACopy(temp.m_font);
#if WITH_OBJECT_MANAGER
		_XObjManger.decreaseAObject(&m_font);
#endif
	}

	return XTrue;
}
void _XLineCtrl::release()
{
	_XCtrlManger.decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(this);
#endif
}
void _XLineCtrl::updateData()
{
	m_truePixelSize = m_pixelSize * m_size;
	if(m_withRange)
	{//检测范围
		if(m_type == LINE_CTRL_TYPE_X_D || m_type == LINE_CTRL_TYPE_X_U)
		{
			if(m_position.x > m_range.y) m_position.x = m_range.y;
			if(m_position.x < m_range.x) m_position.x = m_range.x;
		}else
		{
			if(m_position.y > m_range.y) m_position.y = m_range.y;
			if(m_position.y < m_range.x) m_position.y = m_range.x;
		}
	}
	if(m_withMap)
	{//这里进行映射
		if(m_type == LINE_CTRL_TYPE_X_D || m_type == LINE_CTRL_TYPE_X_U)
			m_mapValue = maping1D(m_position.x,m_range.x,m_range.y,m_mapRange.x,m_mapRange.y);
		else
			m_mapValue = maping1D(m_position.y,m_range.x,m_range.y,m_mapRange.x,m_mapRange.y);
		if(m_withFont)
		{
			sprintf(m_textStr,"(%.2f)",m_mapValue);
			m_font.setString(m_textStr);
			if(m_type == LINE_CTRL_TYPE_X_D || m_type == LINE_CTRL_TYPE_X_U)
			{
				m_stringFont.setPosition(m_range.x,m_position.y - m_stringFont.getTextSize().y - 5);
				if(m_position.x <= (m_range.x + m_range.y) * 0.5f) m_font.setPosition(m_position);
				else m_font.setPosition(m_position.x - m_font.getMaxPixelWidth(),m_position.y);
			}else
			{
				m_stringFont.setPosition(m_position.x,m_position.y - m_stringFont.getTextSize().y - 5);
				m_font.setPosition(m_position);	
			}
		}
	}else
	{
		if(m_withFont)
		{
			if(m_type == LINE_CTRL_TYPE_X_D || m_type == LINE_CTRL_TYPE_X_U) 
				sprintf(m_textStr,"(%.0f)",m_position.x);
			else sprintf(m_textStr,"(%.0f)",m_position.y);
			m_font.setString(m_textStr);
			if(m_type == LINE_CTRL_TYPE_X_D || m_type == LINE_CTRL_TYPE_X_U)
			{
				m_stringFont.setPosition(m_range.x,m_position.y - m_stringFont.getTextSize().y - 5);
				if(m_position.x <= (m_range.x + m_range.y) * 0.5f) m_font.setPosition(m_position);
				else m_font.setPosition(m_position.x - m_font.getMaxPixelWidth(),m_position.y);
			}else
			{
				m_stringFont.setPosition(m_position.x,m_position.y - m_stringFont.getTextSize().y - 5);
				m_font.setPosition(m_position);
			}
		}
	}
}