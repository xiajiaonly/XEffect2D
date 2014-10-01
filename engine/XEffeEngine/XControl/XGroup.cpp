//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XGroup.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

void funXGroupStateBtn(void *pClass,int)
{
	_XGroup & pPar = *(_XGroup *)pClass;
	if(pPar.m_state == STATE_NORMAL) pPar.setState(STATE_MINISIZE);
	else pPar.setState(STATE_NORMAL);
}

_XBool _XGroup::init(const _XVector2& position,
	const _XRect &rect,
	const char *caption,const _XFontUnicode &font,float captionSize)
{
	if(m_isInited) return XFalse;	//防止重复初始化

	m_position = position;
	m_rect = rect;
	m_textSize.set(captionSize);
	m_size.set(1.0f,1.0f);
	m_caption.setACopy(font);
	m_caption.setSize(captionSize);
	m_caption.setString(caption);
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);

	m_state = STATE_NORMAL;
	if(!m_stateBotton.initWithoutTex("-",font,captionSize,_XRect(0.0f,0.0f,XGROUP_STATE_BTN_SIZE,XGROUP_STATE_BTN_SIZE),
		_XVector2(XGROUP_STATE_BTN_SIZE * 0.5f,XGROUP_STATE_BTN_SIZE * 0.5f))) return XFalse;
	m_stateBotton.setMouseDownCB(funXGroupStateBtn,this);
	updateData();

	//_XCtrlManger.decreaseAObject(&m_stateBotton);	//注销这个物件
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_caption);
	_XObjManger.decreaseAObject(&m_stateBotton);
#endif
	//_XCtrlManger.decreaseAObject(&m_stateBotton);
	_XCtrlManger.addACtrl(this);
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	m_isInited = XTrue;
	return XTrue;
}
void _XGroup::draw()
{
	if(!m_isInited ||
		!m_isVisible) return ;	//如果没有初始化直接退出

	if(m_state == STATE_NORMAL)
	{
		int h = m_caption.getMaxPixelHeight();
		int w = m_caption.getMaxPixelWidth();

		drawLine(m_drawRect.left,m_drawRect.top + (h >> 1),m_drawRect.left,m_drawRect.bottom,1,m_color.fR,m_color.fG,m_color.fB,m_color.fA);
		drawLine(m_drawRect.right,m_drawRect.top + (h >> 1),m_drawRect.right,m_drawRect.bottom,1,m_color.fR,m_color.fG,m_color.fB,m_color.fA);
		drawLine(m_drawRect.left,m_drawRect.bottom,m_drawRect.right,m_drawRect.bottom,1,m_color.fR,m_color.fG,m_color.fB,m_color.fA);

		//drawLine(m_drawRect.left,m_drawRect.top + (h >> 1),m_drawRect.left + 20.0f * m_size.x ,m_drawRect.top + (h >> 1),1);
		if(m_drawRect.getWidth() > w + XGROUP_STATE_BTN_SIZE + 10.0f)
			drawLine(m_drawRect.left + (XGROUP_STATE_BTN_SIZE + 10.0f) * m_size.x + w,m_drawRect.top + (h >> 1),
			m_drawRect.right,m_drawRect.top + (h >> 1),1,m_color.fR,m_color.fG,m_color.fB,m_color.fA);
	}else
	{
		drawLine(m_drawRect.left,m_drawRect.top,m_drawRect.left,m_drawRect.bottom,1,m_color.fR,m_color.fG,m_color.fB,m_color.fA);
		drawLine(m_drawRect.right,m_drawRect.top,m_drawRect.right,m_drawRect.bottom,1,m_color.fR,m_color.fG,m_color.fB,m_color.fA);
		drawLine(m_drawRect.left,m_drawRect.bottom,m_drawRect.right,m_drawRect.bottom,1,m_color.fR,m_color.fG,m_color.fB,m_color.fA);
		drawLine(m_drawRect.left,m_drawRect.top,m_drawRect.right,m_drawRect.top,1,m_color.fR,m_color.fG,m_color.fB,m_color.fA);
	}
	m_caption.draw();
//	m_stateBotton.draw();
}
_XBool _XGroup::setACopy(const _XGroup & temp)
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
	m_rect = temp.m_rect;			//控件的范围
	m_funStateChange = temp.m_funStateChange;	//状态发生改变时调用的函数
	m_pClass = temp.m_pClass;

	m_caption.setACopy(temp.m_caption);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_caption);
#endif
	m_textSize = temp.m_textSize;

	return XTrue;
}
void _XGroup::release()
{
	_XCtrlManger.decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(this);
#endif
}