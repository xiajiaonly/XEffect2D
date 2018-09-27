#include "XStdHead.h"
#include "XSubWindowEx.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "../XXml.h"
namespace XE{
void XSubWindowEx::ctrlProc(void *pClass,int id,int eventID)
{
	XSubWindowEx &ref = *(XSubWindowEx *)pClass;
	if (id == ref.m_horizontalSld.getControlID())
	{
		if (eventID == XSlider::SLD_MOUSE_MOVE || eventID == XSlider::SLD_VALUE_CHANGE)
			ref.updateOffsetBySld();
		return;
	}
	if (id == ref.m_verticalSld.getControlID())
	{
		if (eventID == XSlider::SLD_MOUSE_MOVE || eventID == XSlider::SLD_VALUE_CHANGE)
			ref.updateOffsetBySld();
		return;
	}
	if(eventID != XButton::BTN_MOUSE_DOWN) return;
	if(id == ref.m_titleButton[0].getControlID())
	{//最小化或者正常化的按钮
		switch(ref.m_windowState)
		{
		case STATE_NORMAL:ref.changeStage(STATE_MINISIZE);break;
		case STATE_MINISIZE:ref.changeStage(STATE_NORMAL);break;
		}
		return;
	}
	//下面的功能尚未实现
	//if(id == ref.m_titleButton[1].getControlID())
	//{//关闭窗口的按钮
	//	return;
	//}
	//if(id == ref.m_titleButton[2].getControlID())
	//{//待扩展的按钮
	//	return;
	//}
	if (id == ref.m_resetScaleBtn.getControlID())
	{
		if (ref.m_logicScale == 1.0f) return;	//防止重复的动作
		ref.setLogicScale(1.0f);
		return;
	}
}
XBool XSubWindowEx::initWithoutSkin(const XVec2& area, const char * title, const XVec2& logicArea)	//无皮肤的初始化
{
	if (m_isInited) return XFalse;
	//下面需要判断输入参数的合法性
	if (area.x <= 128.0f || area.y <= 128.0f) return XFalse;
	if (logicArea.x < 128.0f || logicArea.y < 128.0f) return false;
	m_area.set(XVec2::zero, area);

	m_position.reset();
	m_scale.set(1.0f);
	m_color.set(1.0f, 1.0f);
	m_titleFont.setACopy(getDefaultFont());
	m_titleFont.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
	m_titleFont.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE);
	m_titleFont.setPosition(m_position.x + 2.0f, m_position.y + 16.0f);
	m_titleFont.setColor(XFColor::black);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_titleFont);
#endif

	m_titleButton[0].initWithoutSkin("", 32.0f);
	m_titleButton[0].setPosition(m_position.x + m_area.right - 96.0f, m_position.y);
	m_titleButton[0].setEventProc(ctrlProc, this);
	//	m_titleButton[0].setWithAction(XFalse);
	m_titleButton[0].setSymbol(BTN_SYMBOL_LINE);
	m_titleButton[1].initWithoutSkin("", 32.0f);
	m_titleButton[1].setPosition(m_position.x + m_area.right - 64.0f, m_position.y);
	m_titleButton[1].setEventProc(ctrlProc, this);
	//	m_titleButton[1].setWithAction(XFalse);
	m_titleButton[1].setSymbol(BTN_SYMBOL_MASK_WRONG);
	m_titleButton[2].initWithoutSkin("", 32.0f);
	m_titleButton[2].setPosition(m_position.x + m_area.right - 32.0f, m_position.y);
	m_titleButton[2].disable();
	m_titleButton[2].setEventProc(ctrlProc, this);
//	m_titleButton[2].setWithAction(XFalse);
	m_titleButton[2].setSymbol(BTN_SYMBOL_LEFT);
	XCtrlManager.decreaseAObject(&m_titleButton[0]);	//注销这个物件
	XCtrlManager.decreaseAObject(&m_titleButton[1]);	//注销这个物件
	XCtrlManager.decreaseAObject(&m_titleButton[2]);	//注销这个物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_titleButton[0]);
	XObjManager.decreaseAObject(&m_titleButton[1]);
	XObjManager.decreaseAObject(&m_titleButton[2]);
#endif

	m_logicArea.x = XMath::fit4(logicArea.x);
	m_logicArea.y = XMath::fit4(logicArea.y);
	m_viewArea.x = m_area.right - 45.0f;
	m_viewArea.y = m_area.bottom - 50.0f;
	m_horizontalSld.initWithoutSkin(XVec2(m_viewArea.x, 32));
	m_horizontalSld.setPosition(m_position.x + 5.0f, m_position.y + m_area.bottom - 5.0f);
	m_horizontalSld.setEventProc(ctrlProc, this);
	m_horizontalSld.setWithAction(false);
	m_horizontalSld.disable();
	m_verticalSld.initWithoutSkin(XVec2(32, m_viewArea.y), 100.0f, 0.0f, SLIDER_TYPE_VERTICAL);
	m_verticalSld.setPosition(m_position.x + m_area.right - 35.0f, m_position.y + 38.0f);
	m_verticalSld.setEventProc(ctrlProc, this);
	m_verticalSld.setWithAction(false);
	m_verticalSld.disable();
	m_resetScaleBtn.initWithoutSkin("", 32);
	m_resetScaleBtn.setPosition(m_position + XVec2(m_area.right - 35.0f, m_area.bottom - 5.0f) * m_scale);
	//m_resetScaleBtn.disable();
	m_resetScaleBtn.setEventProc(ctrlProc, this);
	//m_resetScaleBtn.setWithAction(XFalse);
	m_resetScaleBtn.setSymbol(BTN_SYMBOL_CIRCLE);
	m_resetScaleBtn.setComment("还原缩放效果");
	XCtrlManager.decreaseAObject(&m_horizontalSld);	//注销这个物件
	XCtrlManager.decreaseAObject(&m_verticalSld);	//注销这个物件
	XCtrlManager.decreaseAObject(&m_resetScaleBtn);	//注销这个物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_horizontalSld);
	XObjManager.decreaseAObject(&m_verticalSld);
	XObjManager.decreaseAObject(&m_resetScaleBtn);
#endif
	if (!m_viewFbo.init(m_logicArea.x, m_logicArea.y, COLOR_RGB)) return false;
	m_logicOffset.reset();
	m_logicScale = 1.0f;

	updateMouseRect();
	m_isInited = m_isVisible = m_isEnable = m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
	XCtrlManager.moveToFirst(XCtrlManager.findObjectID(this));
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif

	setTitle(title);
	setAlpha(0.5f);
	return XTrue;
}
void XSubWindowEx::release()
{
	if (!m_isInited) return;
	XCtrlManager.decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	m_ctrlManager.clearAllObject();	//这里需要清空所有的物件
	m_isInited = false;
}
void XSubWindowEx::updateMouseRect()
{
	switch(m_windowState)
	{
	case STATE_NORMAL:
		if(m_windowType != TYPE_NO_TITLE)
		{
			m_curMouseRect.set(m_position + m_area.getLT() * m_scale,
				m_position + XVec2(m_area.right,m_area.bottom + 32.0f) * m_scale);
		}else
		{
			m_curMouseRect.set(m_position + m_area.getLT() * m_scale,
				m_position + m_area.getRB() * m_scale);
		}
		break;
	case STATE_MINISIZE:
		m_curMouseRect.set(m_position + m_area.getLT() * m_scale,
			m_position + XVec2(256.0f, 32.0f) * m_scale);
		break;
	}
}
void XSubWindowEx::draw()
{
	if (!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	if (m_windowState == STATE_NORMAL)
	{
		if (m_windowType != TYPE_NO_TITLE)
		{//描绘标题的相关内容
			XRender::drawFillRectA(m_position + m_area.getLT(),
				XVec2(m_area.getWidth(), 32.0f) * m_scale,
				XCCS::normalColor * m_color, true);
			m_titleFont.draw();
			m_titleButton[0].draw();
			m_titleButton[1].draw();
			m_titleButton[2].draw();
		}
		//下面描绘工作区
		if (m_withBackGround)
		{//如果存在背景，则下面开始描绘背景
			if (m_windowType == TYPE_NO_TITLE)
			{
				XRender::drawFillRectA(m_position + m_area.getLT(),
					m_area.getSize() * m_scale,
					XCCS::downColor * m_color, true);
			}
			else
			{
				XRender::drawFillRectA(m_position + XVec2(m_area.left, m_area.top + 32.0f) * m_scale,
					m_area.getSize() * m_scale,
					XCCS::downColor * m_color, true);
			}
		}
		//下面开始描绘所有的物件
		m_horizontalSld.draw();
		m_verticalSld.draw();
		m_resetScaleBtn.draw();
		//下面描绘可视区域（尚未完成）
		std::vector<XVec2> v;
		std::vector<XVec2> u;
		v.push_back(m_position + XVec2(5, 38) * m_scale);
		v.push_back(m_position + XVec2(5 + m_viewArea.x, 38) * m_scale);
		v.push_back(m_position + XVec2(5, 38 + m_viewArea.y) * m_scale);
		v.push_back(m_position + (XVec2(5, 38) + m_viewArea) * m_scale);
		u.push_back(m_logicOffset / m_logicArea);
		u.push_back((m_logicOffset + XVec2(m_viewArea.x / m_logicScale,0.0f)) / m_logicArea);
		u.push_back((m_logicOffset + XVec2(0.0f,m_viewArea.y / m_logicScale)) / m_logicArea);
		u.push_back((m_logicOffset + m_viewArea / m_logicScale) / m_logicArea);
		XRender::drawTexture(m_viewFbo.getTexture(), &v[0], &u[0], 4, true, m_color);

		XVec2 size = m_viewArea * m_scale * 0.5f;
		XRender::drawRect(m_position + XVec2(5, 38) * m_scale + size, size,2,XFColor::darkGray);
	}
	else
	//if (m_windowState == STATE_MINISIZE)
	{
		XRender::drawFillRectA(m_position + m_area.getLT(),
			XVec2(256.0f, 32.0f) * m_scale,
			XFColor(m_color * 0.75f, 1.0f * m_color.a), true);
		m_titleFont.draw();
		m_titleButton[0].draw();
		m_titleButton[1].draw();
		m_titleButton[2].draw();
	}
	//描绘子控件
//	for(unsigned int i = 0;i < m_objects.size();++ i)
//	{
//		m_objects[i].obj->draw();
//	}
	m_viewFbo.useFBO(true, m_logicArea.x, m_logicArea.y);
	XEG.clearScreen();
	//XRender::drawRect(m_logicArea * 0.5f, m_logicArea * 0.5f, 5.0f, XFColor::red);
	m_ctrlManager.draw();
	m_ctrlManager.drawUp();
	m_viewFbo.removeFBO();
}
void XSubWindowEx::drawUp()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	if((m_windowState == STATE_NORMAL && m_windowType != TYPE_NO_TITLE) ||
		m_windowState == STATE_MINISIZE)
	{//描绘标题的相关内容
		m_titleButton[0].drawUp();
		m_titleButton[1].drawUp();
		m_titleButton[2].drawUp();
	}
	if (m_windowState == STATE_NORMAL)
	{
		m_horizontalSld.drawUp();
		m_verticalSld.drawUp();
		m_resetScaleBtn.drawUp();
	}
//	for(unsigned int i = 0;i < m_objects.size();++ i)
//	{
//		m_objects[i].obj->drawUp();
//	}
	//m_ctrlManager.drawUp();
}
void XSubWindowEx::updateOffsetBySld()
{
	m_logicOffset.x = (m_logicArea.x - m_viewArea.x / m_logicScale) * m_horizontalSld.getCurValue() * 0.01f;
	m_logicOffset.y = (m_logicArea.y - m_viewArea.y / m_logicScale) * m_verticalSld.getCurValue() * 0.01f;
}
XBool XSubWindowEx::mouseProc(const XVec2& p,XMouseState mouseState)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible) return XFalse; 	//如果不可见直接退出
	if(m_withAction && m_isInAction) return XTrue;	//如果支持动作播放而且正在播放动画那么不接受鼠标控制
	if(m_isSilent) return XFalse;
	m_curMousePosition.set(p);
	if(m_curMouseRect.isInRect(p))
	{//下面判断拖拽动作
		if(m_isBeChoose)
		{
			m_titleButton[0].mouseProc(p,mouseState);
			m_titleButton[1].mouseProc(p,mouseState);
			m_titleButton[2].mouseProc(p,mouseState);

			m_horizontalSld.mouseProc(p,mouseState);
			m_verticalSld.mouseProc(p,mouseState);
			m_resetScaleBtn.mouseProc(p,mouseState);
		}
		switch(mouseState)
		{
		case MOUSE_LEFT_BUTTON_DCLICK:	
			//改变窗口的状态
			if(m_windowType != TYPE_NO_TITLE && getIsInDragRect(p))
			{
				switch(m_windowState)//这里必须要限定在标题范围内，否则很难操作
				{
				case STATE_NORMAL:changeStage(STATE_MINISIZE);break;
				case STATE_MINISIZE:changeStage(STATE_NORMAL);break;
				}
			}
		case MOUSE_LEFT_BUTTON_DOWN:
			setFocus();
			break;
		}
	}
	else
	{
		m_horizontalSld.mouseProc(p, mouseState);
		m_verticalSld.mouseProc(p, mouseState);
	}
	if(!m_isBeChoose) return XFalse;		
	if(m_windowType != TYPE_NO_TITLE)
		mouseEventProc(p,mouseState);
	if (m_isShiftDown)
	{
		if (mouseState == MOUSE_WHEEL_UP_DOWN)
		{//放大
			if (m_logicScale >= 2.0f) return XTrue;
			m_logicScale *= 1.1f;
			if (m_logicScale >= 2.0f) m_logicScale = 2.0f;
			//下面更新控件状态
			setLogicScale(m_logicScale); 
			return true;
		}
		if (mouseState == MOUSE_WHEEL_DOWN_DOWN)
		{//缩小
			XVec2 tmpS = m_viewArea / m_logicArea;
			float scale = (std::max)(tmpS.x, tmpS.y);
			if (m_logicScale <= scale) return XTrue;
			m_logicScale *= 0.9f;
			if (m_logicScale <= scale) m_logicScale = scale;
			//下面更新控件状态
			setLogicScale(m_logicScale);
			return true;
		}
	}
	//这里需要判断鼠标是在显示范围内操作
	if (mouseState == MOUSE_LEFT_BUTTON_UP || mouseState == MOUSE_RIGHT_BUTTON_UP ||
		mouseState == MOUSE_MIDDLE_BUTTON_UP || mouseState == MOUSE_WHEEL_UP_DOWN ||
		mouseState == MOUSE_WHEEL_DOWN_DOWN ||
		XRect(m_position + XVec2(5.0f, 38.0f) * m_scale,
		m_position + (XVec2(5.0f, 38.0f) + m_viewArea) * m_scale).isInRect(p))
	{
		m_ctrlManager.mouseProc((p - (m_position + XVec2(5.0f, 38.0f)) * m_scale) / m_scale / m_logicScale + m_logicOffset,
			mouseState);
	}
	return XFalse;
}
void XSubWindowEx::setLogicScale(float s)
{
	m_logicScale = s;
	float tmp = m_logicArea.x - m_viewArea.x / m_logicScale;
	if (tmp <= 0.0f)
	{
		m_horizontalSld.setCurValue(0.0f);
		updateOffsetBySld();
	}
	else
	{
		tmp = m_logicOffset.x / tmp;
		if (tmp >= 1.0f)
		{
			m_horizontalSld.setCurValue(100.0f);
			updateOffsetBySld();
		}
		else
			m_horizontalSld.setCurValue(tmp * 100.0f);
	}
	tmp = m_logicArea.y - m_viewArea.y / m_logicScale;
	if (tmp <= 0.0f)
	{
		m_verticalSld.setCurValue(0.0f);
		updateOffsetBySld();
	}
	else
	{
		tmp = m_logicOffset.y / tmp;
		if (tmp >= 1.0f)
		{
			m_verticalSld.setCurValue(100.0f);
			updateOffsetBySld();
		}
		else
			m_verticalSld.setCurValue(tmp * 100.0f);
	}
}
XBool XSubWindowEx::addACtrlObj(XControlBasic *obj)	//向窗口中添加一个控件
{
	if(obj == NULL) return XFalse;
	if(getObjectIndex(obj) >= 0) return XFalse;	//不能载入重复的资源
	XSubWindowObj tmpObj;
	tmpObj.obj = obj;
	tmpObj.pos = obj->getPosition() - m_position;
	tmpObj.scale = obj->getScale() / m_scale;
	m_objects.push_back(tmpObj);
	//方案1：控件管理器不负责描绘这个控件
	//XCtrlManager.setNeedDraw(false,obj);	//注销这个物件
	//XCtrlManager.moveAndInsert(XCtrlManager.findObjectID(obj),XCtrlManager.findObjectID(this));
	//方案2：
	XCtrlManager.decreaseAObject(obj);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(obj);
#endif
	m_ctrlManager.addACtrl(obj);

	//tmpObj.obj->setColor(m_color);
	//tmpObj.obj->setScale(tmpObj.scale * m_scale);
	return true;
}
XBool XSubWindowEx::desACtrlObj(XControlBasic* obj)
{
	if (obj == NULL) return XFalse;
	int index = getObjectIndex(obj);
	if(index < 0) return XFalse;	//不能载入重复的资源

	m_objects.erase(m_objects.begin() + index);
	m_ctrlManager.decreaseAObject(obj);
	//方案1：控件管理器不负责描绘这个控件
	//XCtrlManager.setNeedDraw(false,obj);	//注销这个物件
	//XCtrlManager.moveAndInsert(XCtrlManager.findObjectID(obj),XCtrlManager.findObjectID(this));
	//方案2：
	XCtrlManager.addACtrl(obj);
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(obj);
#endif
	return true;
}
void XSubWindowEx::setFocus() 
{
	m_isBeChoose = XTrue;
	XCtrlManager.moveToFirst(XCtrlManager.findObjectID(this));
//	for(unsigned int i = 0;i < m_objects.size();++ i)
//	{
//		XCtrlManager.moveAndInsert(XCtrlManager.findObjectID(m_objects[i].obj),XCtrlManager.findObjectID(this));
//	}
	setAlpha(1.0f);
}	
int XSubWindowEx::getObjectIndex(XControlBasic *obj)
{//ok
	if(obj == NULL) return -1;
	for(unsigned int i = 0;i < m_objects.size();++ i)
	{
		if(m_objects[i].obj == obj) return i;
	}
	return -1;
}
void XSubWindowEx::setPosition(const XVec2& p)
{
	if(!m_isInited) return;
	XVec2 tmpOldPos = m_position;
	m_position = p;
	m_titleFont.setPosition(m_position.x + 2.0f * m_scale.x,m_position.y + 16.0f * m_scale.y);
	switch(m_windowState)
	{
	case STATE_NORMAL:
		m_titleButton[0].setPosition(m_position.x + (m_area.right - 96.0f) * m_scale.x,m_position.y);
		m_titleButton[1].setPosition(m_position.x + (m_area.right - 64.0f) * m_scale.x,m_position.y);
		m_titleButton[2].setPosition(m_position.x + (m_area.right - 32.0f) * m_scale.x,m_position.y);
		break;
	case STATE_MINISIZE:
		m_titleButton[0].setPosition(m_position.x + (256.0f - 96.0f) * m_scale.x,m_position.y);
		m_titleButton[1].setPosition(m_position.x + (256.0f - 64.0f) * m_scale.x,m_position.y);
		m_titleButton[2].setPosition(m_position.x + (256.0f - 32.0f) * m_scale.x,m_position.y);
		break;
	}
	m_horizontalSld.setPosition(m_position + XVec2(5.0f, m_area.bottom - 5.0f) * m_scale);
	m_verticalSld.setPosition(m_position + XVec2(m_area.right - 35.0f, 38.0f) * m_scale);
	m_resetScaleBtn.setPosition(m_position + XVec2(m_area.right - 35.0f, m_area.bottom - 5.0f) * m_scale);
	updateMouseRect();
	//方案1：
	//for(unsigned int i = 0;i < m_objects.size();++ i)
	//{
	//	m_objects[i].obj->setPosition(m_objects[i].pos * m_scale + m_position);
	//}
	//方案2：
	//XVec2 offset;
	//for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
	//{
	//	it->pos = it->obj->getPosition() - tmpOldPos;
	//	it->obj->setPosition(it->pos * m_scale + m_position);
	//}
}
void XSubWindowEx::setScale(const XVec2& s)
{
	if(!m_isInited) return;
	m_scale = s;
	m_titleFont.setScale(m_scale);
	m_titleButton[0].setScale(m_scale);
	m_titleButton[1].setScale(m_scale);
	m_titleButton[2].setScale(m_scale);
	m_titleFont.setPosition(m_position + XVec2(2.0f, 16.0f) * m_scale);
	switch(m_windowState)
	{
	case STATE_NORMAL:
		m_titleButton[0].setPosition(m_position.x + (m_area.right - 96.0f) * m_scale.x,m_position.y);
		m_titleButton[1].setPosition(m_position.x + (m_area.right - 64.0f) * m_scale.x,m_position.y);
		m_titleButton[2].setPosition(m_position.x + (m_area.right - 32.0f) * m_scale.x,m_position.y);
		break;
	case STATE_MINISIZE:
		m_titleButton[0].setPosition(m_position.x + (256.0f - 96.0f) * m_scale.x,m_position.y);
		m_titleButton[1].setPosition(m_position.x + (256.0f - 64.0f) * m_scale.x,m_position.y);
		m_titleButton[2].setPosition(m_position.x + (256.0f - 32.0f) * m_scale.x,m_position.y);
		break;
	}
	m_horizontalSld.setPosition(m_position + XVec2(5.0f, m_area.bottom - 5.0f) * m_scale);
	m_verticalSld.setPosition(m_position + XVec2(m_area.right - 35.0f, 38.0f) * m_scale);
	m_resetScaleBtn.setPosition(m_position + XVec2(m_area.right - 35.0f, m_area.bottom - 5.0f) * m_scale);
	updateMouseRect();
	//for(unsigned int i = 0;i < m_objects.size();++ i)
	//{
	//	m_objects[i].obj->setPosition(m_objects[i].pos * m_scale + m_position);
	//	m_objects[i].obj->setScale(m_objects[i].scale * m_scale);
	//}
}
void XSubWindowEx::setMiniSize()
{
	if(m_windowType == TYPE_NO_TITLE) return;	//无窗口模式不能设置为最小化
	m_windowState = STATE_MINISIZE;
	//改变字体的显示，改变按钮的位置

	m_titleFont.setString(XString::getCanShowString(m_titleString.c_str(),256.0f - 96.0f,15.0f).c_str());
		
	//m_titleButton[0].setCaptionText("口");
	m_titleButton[0].setSymbol(BTN_SYMBOL_RECT);
	m_titleButton[0].setPosition(m_position.x + (256.0f - 96.0f) * m_scale.x,m_position.y);
	m_titleButton[1].setPosition(m_position.x + (256.0f - 64.0f) * m_scale.x,m_position.y);
	m_titleButton[2].setPosition(m_position.x + (256.0f - 32.0f) * m_scale.x,m_position.y);
	updateMouseRect();
	//下面设置所有子物件为不可见
	for(unsigned int i = 0;i < m_objects.size();++ i)
	{
		m_objects[i].obj->disVisible();
	}
}
void XSubWindowEx::setNormalSize()
{
	m_windowState = STATE_NORMAL;
	//改变字体的显示，改变按钮的位置

	m_titleFont.setString(XString::getCanShowString(m_titleString.c_str(),m_area.getWidth() - 96.0f,15.0f).c_str());
		
	//m_titleButton[0].setCaptionText("-");
	m_titleButton[0].setSymbol(BTN_SYMBOL_LINE);
	m_titleButton[0].setPosition(m_position.x + (m_area.right - 96.0f) * m_scale.x,m_position.y);
	m_titleButton[1].setPosition(m_position.x + (m_area.right - 64.0f) * m_scale.x,m_position.y);
	m_titleButton[2].setPosition(m_position.x + (m_area.right - 32.0f) * m_scale.x,m_position.y);
	updateMouseRect();
	for(unsigned int i = 0;i < m_objects.size();++ i)
	{
		m_objects[i].obj->setVisible();
	}
}
void XSubWindowEx::setColor(const XFColor& c)
{
	m_color = c;
	m_titleFont.setAlpha(c.a);
	m_titleButton[0].setColor(m_color);
	m_titleButton[1].setColor(m_color);
	m_titleButton[2].setColor(m_color);
	m_horizontalSld.setColor(m_color);
	m_verticalSld.setColor(m_color);
	m_resetScaleBtn.setColor(m_color);
	//for(unsigned int i = 0;i < m_objects.size();++ i)
	//{
	//	m_objects[i].obj->setColor(m_color);
	//}
}
void XSubWindowEx::setAlpha(float a)
{
	m_color.setA(a);
	m_titleFont.setAlpha(a);
	m_titleButton[0].setAlpha(a);
	m_titleButton[1].setAlpha(a);
	m_titleButton[2].setAlpha(a);
	m_horizontalSld.setAlpha(a);
	m_verticalSld.setAlpha(a);
	m_resetScaleBtn.setAlpha(a);
	//for(unsigned int i = 0;i < m_objects.size();++ i)
	//{
	//	m_objects[i].obj->setAlpha(a);
	//}
}
void XSubWindowEx::changeStage(XSubWindowState state)
{
	if(!m_isInited || m_windowState == state) return;
	switch(state)
	{
	case STATE_NORMAL:setNormalSize();break;
	case STATE_MINISIZE:setMiniSize();break;
	}
	if(m_eventProc != 0) m_eventProc(m_pClass,getControlID(),SWD_STATE_CHANGE);
	else XCtrlManager.eventProc(getControlID(),SWD_STATE_CHANGE);
}
void XSubWindowEx::setLostFocus()	//设置失去焦点
{
	if(!m_isInited ||		//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||		//如果不可见直接退出
		!m_isEnable) return;		//如果无效则直接退出
	setAlpha(0.5f);
	m_isBeChoose = XFalse;	//控件处于焦点状态
	//需要设置所有的子物体失去焦点
	for (int i = 0; i < m_objects.size(); ++i)
	{
		m_objects[i].obj->setLostFocus();
	}
	if(m_eventProc != 0) m_eventProc(m_pClass,getControlID(),SWD_LOSE_FOCUS);
	else XCtrlManager.eventProc(getControlID(),SWD_LOSE_FOCUS);
}
XBool XSubWindowEx::saveState(TiXmlNode &e)
{
	if (!m_needSaveAndLoad) return XTrue;	//如果不需要保存则直接返回
	//遍历所有的控件，保存其状态
	TiXmlElement elmNode(m_ctrlName.c_str());
	bool ret = true;
	for (unsigned int i = 0; i < m_objects.size(); ++i)
	{
		if (!m_objects[i].obj->saveState(elmNode)) ret = false;
	}
	if (e.InsertEndChild(elmNode) == NULL) ret = false;
	return ret;
}
XBool XSubWindowEx::loadState(TiXmlNode *e)
{
	if (!m_needSaveAndLoad) return XTrue;	//如果不需要保存则直接返回
	//遍历所有的空间，读取其状态
	TiXmlNode * keyNode = e->FirstChildElement(m_ctrlName.c_str());
	if (keyNode == NULL) return XFalse;
	bool ret = true;
	for (unsigned int i = 0; i < m_objects.size(); ++i)
	{
		if (!m_objects[i].obj->loadState(keyNode)) ret = false;
	}
	return ret;
}
#if !WITH_INLINE_FILE
#include "XSubWindowEx.inl"
#endif
}