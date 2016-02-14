#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XButtonEx.h"
#include "XObjectManager.h"
#include "XControlManager.h"
namespace XE{
XButtonEx::XButtonEx()
:m_isInited(XFalse)
,m_buttonNormal(NULL)		
,m_buttonDown(NULL)	
,m_buttonOn(NULL)		
,m_buttonDisable(NULL)	
,m_hotKey(-1)
,m_curButtonState(BUTTON_STATE_NORMAL)
,m_upMousePoint(0,0)
,m_pArea(NULL)
,m_pCurArea(NULL)
,m_resInfo(NULL)
,m_colorRate(NULL)
{
	m_ctrlType = CTRL_OBJ_BUTTONEX;
}
void XButtonEx::setTexture(const XButtonSkin& tex)
{
	if(tex.buttonNormal == NULL) return;
	m_buttonNormal = tex.buttonNormal;
	if(tex.buttonDown != NULL) m_buttonDown = tex.buttonDown;
	else m_buttonDown = tex.buttonNormal;
	
	if(tex.buttonOn != NULL) m_buttonOn = tex.buttonOn;
	else m_buttonOn = tex.buttonNormal;
	
	if(tex.buttonDisable != NULL) m_buttonDisable = tex.buttonDisable;
	else m_buttonDisable = tex.buttonNormal;
}
XBool XButtonEx::init(const XVector2& position,		//控件的位置
		const XVector2 *area,int pointSum,	//用于描述按钮的响应范围的序列点，已经点的数量
		const XButtonSkin &tex,					//按钮的个中贴图信息
		const char *caption,float captionSize,
		const XVector2 &captionPosition,	//按钮上现实的文字的相关信息
		const XFontUnicode &font)
{
	if(m_isInited) return XFalse;
	if(area == NULL || pointSum <= 2) return XFalse;	//控件必须要有一个合适的响应区间，不然会造成错误
	if(tex.buttonNormal == NULL) return XFalse;		//按键的普通状态的贴图不能为空，否则直接返回错误
	if(captionSize <= 0) return XFalse;
	m_position = position;
	m_textPosition = captionPosition;
	m_buttonNormal = tex.buttonNormal;
	if(tex.buttonDown != NULL) m_buttonDown = tex.buttonDown;
	else m_buttonDown = tex.buttonNormal;
	if(tex.buttonOn != NULL) m_buttonOn = tex.buttonOn;
	else m_buttonOn = tex.buttonNormal;
	if(tex.buttonDisable != NULL) m_buttonDisable = tex.buttonDisable;
	else m_buttonDisable = tex.buttonNormal;
	m_comment.init();

	m_pArea = XMem::createArrayMem<XVector2>(pointSum);
	if(m_pArea == NULL) return XFalse;
	m_pCurArea = XMem::createArrayMem<XVector2>(pointSum);
	if(m_pCurArea == NULL)
	{
		XMem::XDELETE_ARRAY(m_pArea);
		return XFalse;
	}
	m_areaPointSum = pointSum;
	m_centerPos.set(0.0f,0.0f);
	for(int i = 0;i < m_areaPointSum;++ i)
	{
		m_centerPos += area[i];
		m_pArea[i] = area[i];
	//	m_pCurArea[i].x = m_position.x + area[i].x * m_scale.x;
	//	m_pCurArea[i].y = m_position.y + area[i].y * m_scale.y;
		m_pCurArea[i] = m_position + area[i] * m_scale;
	}
	m_centerPos /= pointSum;

	if(!m_caption.setACopy(font)) return XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_MIDDLE); //设置字体居中对齐
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //设置字体居中对齐
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//设置字体的颜色为黑色
	m_scale.set(1.0f,1.0f);
	m_sprite.init(m_buttonNormal->texture.m_w,m_buttonNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_sprite);
#endif
	m_sprite.setPosition(m_position);
	m_sprite.setScale(m_scale);
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);
	m_color = XFColor::white;
	m_sprite.setColor(m_color);
	//这里距中对齐，所以这里的位置需要计算
	m_caption.setString(caption);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_scale.x,m_position.y + m_textPosition.y * m_scale.y);
	m_textSize.set(captionSize,captionSize);
	m_caption.setScale(m_textSize.x * m_scale.x,m_textSize.y * m_scale.y);

	m_curButtonState = BUTTON_STATE_NORMAL;

	if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,BTNEX_INIT);
	else XCtrlManager.eventProc(m_objectID,BTNEX_INIT);

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;
	m_withoutTex = XFalse;

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
XBool XButtonEx::initWithoutSkin(const XVector2 *area,int pointSum,const char *caption,	//按钮上现实的文字的相关信息
		const XFontUnicode &font,const XVector2 &captionPosition,float captionSize)
{//尚未实现
	if(m_isInited ||
		area == NULL || pointSum <= 2 ||	//控件必须要有一个合适的响应区间，不然会造成错误
		captionSize <= 0) return XFalse;
	m_position.set(0.0f,0.0f);
	m_textPosition = captionPosition;
	m_comment.init();

	m_pArea = XMem::createArrayMem<XVector2>(pointSum);
	if(m_pArea == NULL) return XFalse;
	m_pCurArea = XMem::createArrayMem<XVector2>(pointSum);
	if(m_pCurArea == NULL)
	{
		XMem::XDELETE_ARRAY(m_pArea);
		return XFalse;
	}
	m_colorRate = XMem::createArrayMem<float>(pointSum);
	if(m_colorRate == NULL)
	{
		XMem::XDELETE_ARRAY(m_pArea);
		XMem::XDELETE_ARRAY(m_pCurArea);
		return XFalse;
	}
	m_areaPointSum = pointSum;
	float max = area[0].y;
	float min = area[0].y;
	m_centerPos.set(0.0f,0.0f);
	for(int i = 0;i < m_areaPointSum;++ i)
	{
		m_centerPos += area[i];
		m_pArea[i] = area[i];
	//	m_pCurArea[i].x = m_position.x + area[i].x * m_scale.x;
	//	m_pCurArea[i].y = m_position.y + area[i].y * m_scale.y;
		m_pCurArea[i] = m_position + area[i] * m_scale;
		if(area[i].y > max) max = area[i].y;
		if(area[i].y < min) min = area[i].y;
	}
	m_centerPos /= pointSum;
	//这里计算颜色
	for(int i = 0;i < m_areaPointSum;++ i)
	{
		m_colorRate[i] = XMath::maping1D(area[i].y,min,max,0.9f,1.1f);
		m_colorRate[i] = 2.0f - m_colorRate[i];
	}

	if(!m_caption.setACopy(font)) return XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_MIDDLE); //设置字体居中对齐
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //设置字体居中对齐
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//设置字体的颜色为黑色
	m_scale.set(1.0f,1.0f);

	m_color = XFColor::white;
	//这里距中对齐，所以这里的位置需要计算
	m_caption.setString(caption);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_scale.x,m_position.y + m_textPosition.y * m_scale.y);
	m_textSize.set(captionSize,captionSize);
	m_caption.setScale(m_textSize.x * m_scale.x,m_textSize.y * m_scale.y);

	m_curButtonState = BUTTON_STATE_NORMAL;

	if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,BTNEX_INIT);
	else XCtrlManager.eventProc(m_objectID,BTNEX_INIT);

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;
	m_withoutTex = XFalse;

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_withoutTex = XTrue;
	m_isInited = XTrue;
	return XTrue;
}
void XButtonEx::setPosition(float x,float y)
{
	if(!m_isInited) return;
	m_position.set(x,y);
	for(int i = 0;i < m_areaPointSum;++ i)
	{
	//	m_pCurArea[i].x = m_position.x + m_pArea[i].x * m_scale.x;
	//	m_pCurArea[i].y = m_position.y + m_pArea[i].y * m_scale.y;
		m_pCurArea[i] = m_position + m_pArea[i] * m_scale;
	}
	//m_caption.setPosition(m_position.x + m_textPosition.x * m_scale.x,m_position.y + m_textPosition.y * m_scale.y);
	m_caption.setPosition(m_position + m_textPosition * m_scale);
	m_sprite.setPosition(m_position);
	mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
}
void XButtonEx::setScale(float x,float y)
{
	if(!m_isInited ||
		(x <= 0 && y <= 0)) return;
	m_scale.set(x,y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_scale.x,m_position.y + m_textPosition.y * m_scale.y);
	m_caption.setScale(m_textSize.x * m_scale.x,m_textSize.y * m_scale.y);
	m_sprite.setScale(m_scale);
	for(int i = 0;i < m_areaPointSum;++ i)
	{
	//	m_pCurArea[i].x = m_position.x + m_pArea[i].x * m_scale.x;
	//	m_pCurArea[i].y = m_position.y + m_pArea[i].y * m_scale.y;
		m_pCurArea[i] = m_position + m_pArea[i] * m_scale;
	}
	mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
}
XBool XButtonEx::mouseProc(float x,float y,XMouseState mouseState)
{
	m_upMousePoint.set(x,y);
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	if(m_withAction && m_isInAction) return XFalse;	//如果支持动作播放而且正在播放动画那么不接受鼠标控制

	if(XMath::getIsInRect(m_upMousePoint,m_pCurArea,m_areaPointSum))
	{//鼠标动作在范围内
		if(!m_isMouseInRect)
		{
			m_isMouseInRect = XTrue;
			m_comment.setShow();
			setCommentPos(x,y + 16.0f);
		}
		if(mouseState != MOUSE_MOVE && m_comment.getIsShow())
			m_comment.disShow();	//鼠标的任意操作都会让说明框消失
		if(mouseState == MOUSE_MOVE && m_curButtonState == BUTTON_STATE_NORMAL)
		{
			m_curButtonState = BUTTON_STATE_ON;
			if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,BTNEX_MOUSE_ON);
			else XCtrlManager.eventProc(m_objectID,BTNEX_MOUSE_ON);
			if(m_withAction)
			{//这里测试一个动态效果
				m_isInAction = XTrue;
				m_actionMoveData.set(1.0f,1.1f,0.02f,MOVE_DATA_MODE_SIN_MULT,1);
				m_lightMD.set(1.0f,2.0f,0.002f,MOVE_DATA_MODE_SIN_MULT);
				m_oldPos = m_position;
				m_oldSize = m_scale;
			}
		}else
		if((mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_LEFT_BUTTON_DCLICK) && 
			(m_curButtonState == BUTTON_STATE_NORMAL || m_curButtonState == BUTTON_STATE_ON))
		{
			m_curButtonState = BUTTON_STATE_DOWN;
			if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,BTNEX_MOUSE_DOWN);
			else XCtrlManager.eventProc(m_objectID,BTNEX_MOUSE_DOWN);
		}else
		if(mouseState == MOUSE_LEFT_BUTTON_UP && m_curButtonState == BUTTON_STATE_DOWN)
		{
			m_curButtonState = BUTTON_STATE_ON;
			if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,BTNEX_MOUSE_UP);
			else XCtrlManager.eventProc(m_objectID,BTNEX_MOUSE_UP);
			m_isBeChoose = XTrue;	//控件处于焦点状态
		}
	}else
	{
		if(m_isMouseInRect)
		{
			m_isMouseInRect = XFalse;
			m_comment.disShow();
		}
		if(m_curButtonState == BUTTON_STATE_ON || m_curButtonState == BUTTON_STATE_DOWN)
		{
			m_curButtonState = BUTTON_STATE_NORMAL;
		}
	}
	return XTrue;
}
XBool XButtonEx::keyboardProc(int keyOrder,XKeyState keyState)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	if(m_withAction && m_isInAction) return XFalse;	//如果支持动作播放而且正在播放动画那么不接受鼠标控制

	if(keyState == KEY_STATE_UP)
	{//按键弹起时才作相应
		if(m_curButtonState == BUTTON_STATE_NORMAL)
		{//按钮只有在普通状态下才能响应快捷键，防止冲突
			if(keyOrder == m_hotKey || (keyOrder ==  XKEY_RETURN && m_isBeChoose))
			{
				if(m_eventProc != NULL)
				{
					m_eventProc(m_pClass,m_objectID,BTNEX_MOUSE_DOWN);
					m_eventProc(m_pClass,m_objectID,BTNEX_MOUSE_UP);
				}else
				{
					XCtrlManager.eventProc(m_objectID,BTNEX_MOUSE_DOWN);
					XCtrlManager.eventProc(m_objectID,BTNEX_MOUSE_UP);
				}
				return XTrue;
			}
		}
	}
	return XFalse;
}
void XButtonEx::draw()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出

	if(!m_withoutTex)
	{
		switch(m_curButtonState)
		{
		case BUTTON_STATE_NORMAL: m_sprite.draw(m_buttonNormal);break;
		case BUTTON_STATE_DOWN: m_sprite.draw(m_buttonDown);break;
		case BUTTON_STATE_ON: m_sprite.draw(m_buttonOn);break;
		case BUTTON_STATE_DISABLE: m_sprite.draw(m_buttonDisable);break;
		}
	}else
	{
		switch(m_curButtonState)
		{
		case BUTTON_STATE_NORMAL: XRender::drawFillPolygonExA(m_pCurArea,m_colorRate,m_areaPointSum,XCCS::normalColor * m_color);break;
		case BUTTON_STATE_DOWN: XRender::drawFillPolygonExA(m_pCurArea,m_colorRate,m_areaPointSum,XCCS::downColor * m_color);break;
		case BUTTON_STATE_ON: XRender::drawFillPolygonExA(m_pCurArea,m_colorRate,m_areaPointSum,XCCS::onColor * m_color);break;
		case BUTTON_STATE_DISABLE: XRender::drawFillPolygonExA(m_pCurArea,m_colorRate,m_areaPointSum,XCCS::disableColor * m_color);break;
		}
	}
	//显示按钮的字体
	m_caption.draw();
}
void XButtonEx::drawUp()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	XVector2 tmp = m_centerPos * (m_lightMD.getCurData() * m_oldSize - m_oldSize);
	for(int i = 0;i < m_areaPointSum;++ i)
	{
		XRender::drawLine(m_oldPos - tmp + m_pArea[i] * m_oldSize * m_lightMD.getCurData(),
			m_oldPos - tmp + m_pArea[(i + 1)%m_areaPointSum] * m_oldSize * m_lightMD.getCurData(),
			1.0f * m_lightMD.getCurData() * 2.0f,1.0f,1.0f,1.0f,(1.0f - m_lightMD.getCurTimer()) * 0.5f);
	}
	m_comment.draw();
}
void XButtonEx::release()
{
	if(!m_isInited) return;

	XMem::XDELETE_ARRAY(m_pArea);
	XMem::XDELETE_ARRAY(m_pCurArea);
	XMem::XDELETE_ARRAY(m_colorRate);
	m_areaPointSum = 0;
	if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,BTNEX_RELEASE);
	else XCtrlManager.eventProc(m_objectID,BTNEX_RELEASE);

	XCtrlManager.decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	if(m_resInfo != NULL)
	{
		XResManager.releaseResource(m_resInfo);
		m_resInfo = NULL;
	}
	m_isInited = XFalse;
}
XBool XButtonEx::setACopy(const XButtonEx &temp)
{
	if(& temp == this) return XTrue;	//防止自身赋值
	if(!temp.m_isInited) return XFalse;
	if(m_isInited) release();
	if(!XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
		XObjManager.addAObject(this);
#endif
	}

	m_isInited = temp.m_isInited;					//按钮是否被初始化
	m_curButtonState = temp.m_curButtonState;		//当前的按钮状态

	if(m_resInfo != NULL) XResManager.releaseResource(m_resInfo);
	m_resInfo = XResManager.copyResource(temp.m_resInfo);

	if(!m_caption.setACopy(temp.m_caption))	return XFalse;		//按钮的标题
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_withoutTex = temp.m_withoutTex;
	m_areaPointSum = temp.m_areaPointSum;
	m_pArea = XMem::createArrayMem<XVector2>(m_areaPointSum);
	if(m_pArea == NULL) return XFalse;
	m_pCurArea = XMem::createArrayMem<XVector2>(m_areaPointSum);
	if(m_pCurArea == NULL)
	{
		XMem::XDELETE_ARRAY(m_pArea);
		return XFalse;
	}
	if(m_withoutTex)
	{
		m_colorRate = XMem::createArrayMem<float>(m_areaPointSum);
		if(m_colorRate == NULL)
		{
			XMem::XDELETE_ARRAY(m_pArea);
			XMem::XDELETE_ARRAY(m_pCurArea);
			return XFalse;
		}
		for(int i = 0;i < m_areaPointSum;++ i)
		{
			m_pArea[i] = temp.m_pArea[i];
			m_pCurArea[i] = temp.m_pCurArea[i];
			m_colorRate[i] = temp.m_colorRate[i];
		}
	}else
	{
		for(int i = 0;i < m_areaPointSum;++ i)
		{
			m_pArea[i] = temp.m_pArea[i];
			m_pCurArea[i] = temp.m_pCurArea[i];
		}
	}

	m_textColor = temp.m_textColor;

	m_buttonNormal = temp.m_buttonNormal;			//普通状态下的按钮贴图(无论如何，这个贴图一定要有)
	m_buttonDown = temp.m_buttonDown;				//按下状态下的按钮贴图
	m_buttonOn = temp.m_buttonOn;					//悬浮状态下的按钮贴图
	m_buttonDisable = temp.m_buttonDisable;			//无效状态下的按钮贴图

	m_sprite.setACopy(temp.m_sprite);		//用于显示贴图的精灵
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_sprite);
#endif
	m_textPosition = temp.m_textPosition;	//文字显示的位置，是相对于控件的位置来定的
	m_textSize = temp.m_textSize;			//文字显示的尺寸，这个尺寸会与空间的缩放尺寸叠加
	m_upMousePoint = temp.m_upMousePoint;	//上次记录的鼠标位置

	m_actionMoveData = temp.m_actionMoveData;	//动态效果的变量
	m_oldPos = temp.m_oldPos;				//动作播放时的位置
	m_oldSize = temp.m_oldSize;			//动作播放时的大小
	m_centerPos = temp.m_centerPos;			//动作播放时的大小
	m_lightMD = temp.m_lightMD;

	m_hotKey = temp.m_hotKey;
	return XTrue;
}
XVector2 XButtonEx::getBox(int order)
{//寻找所有点中最外围的点
	if(!m_isInited) return XVector2::zero;
	float left = m_pCurArea[0].x;
	float right = m_pCurArea[0].x;
	float top = m_pCurArea[0].y;
	float bottom = m_pCurArea[0].y;
	for(int i = 0;i < m_areaPointSum;++ i)
	{
		if(m_pCurArea[i].x < left) left = m_pCurArea[i].x;
		if(m_pCurArea[i].x > right) right = m_pCurArea[i].x;
		if(m_pCurArea[i].y < top) top = m_pCurArea[i].y;
		if(m_pCurArea[i].y > bottom) bottom = m_pCurArea[i].y;
	}
	switch(order)
	{
	case 0: return XVector2(left,top);
	case 1: return XVector2(right,top);
	case 2: return XVector2(right,bottom);
	case 3: return XVector2(left,bottom);
	}

	return XVector2::zero;
}
inline void XButtonEx::update(float stepTime)
{
	m_comment.update(stepTime);
	if(m_isInAction)
	{//处于动作过程中计算动作的实施
		m_actionMoveData.move(stepTime);
		if(m_actionMoveData.getIsEnd()) m_isInAction = false;	//动作播放完成
		setScale(m_actionMoveData.getCurData() * m_oldSize);
		//这里需要计算中点
		XVector2 tmp = m_centerPos * (m_actionMoveData.getCurData() * m_oldSize - m_oldSize);
		setPosition(m_oldPos - tmp);
	}
	if(!m_lightMD.getIsEnd())
	{
		m_lightMD.move(stepTime);
	//	XVector2 pos(m_oldPos.x + m_mouseRect.getWidth() * m_oldSize.x * 0.5f,
	//		m_oldPos.y + m_mouseRect.getHeight() * m_oldSize.y * 0.5f);
	//	XVector2 size(m_mouseRect.getWidth() * m_oldSize.x * m_lightMD.getCurData() * 0.5f,
	//		m_mouseRect.getHeight() * m_oldSize.y * m_lightMD.getCurData() * 0.5f);
	//	m_lightRect.set(pos.x - size.x,pos.y - size.y,pos.x + size.x,pos.y + size.y);
	}
}
#if !WITH_INLINE_FILE
#include "XButtonEx.inl"
#endif
}