#include "XStdHead.h"
#include "XSubWindow.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "../XXml.h"
namespace XE{
void XSubWindow::ctrlProc(void *pClass,int id,int eventID)
{
	if(eventID != XButton::BTN_MOUSE_DOWN) return;
	XSubWindow &pPar = *(XSubWindow *)pClass;
	if(id == pPar.m_titleButton[0].getControlID())
	{//最小化或者正常化的按钮
		switch(pPar.m_windowState)
		{
		case STATE_NORMAL:pPar.changeStage(STATE_MINISIZE);break;
		case STATE_MINISIZE:pPar.changeStage(STATE_NORMAL);break;
		}
		return;
	}
	if(id == pPar.m_titleButton[1].getControlID())
	{//关闭窗口的按钮
		return;
	}
	if(id == pPar.m_titleButton[2].getControlID())
	{//待扩展的按钮
		return;
	}
}
XBool XSubWindow::initWithoutSkin(const XVector2 &area,const char * title)	//无皮肤的初始化
{
	if(m_isInited) return XFalse;
	//下面需要判断输入参数的合法性
	if(area.x <= 128.0f || area.y <= 128.0f) return XFalse;
	m_area.set(0.0f,0.0f,area.x,area.y);

	m_position.set(0.0f,0.0f);
	m_scale.set(1.0f,1.0f);
	m_color.setColor(1.0f,1.0f,1.0f,1.0f);
	m_titleFont.setACopy(getDefaultFont());
	m_titleFont.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
	m_titleFont.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE);
	m_titleFont.setPosition(m_position.x + 2.0f,m_position.y + 16.0f);
	m_titleFont.setColor(XFColor::black);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_titleFont);
#endif

	m_titleButton[0].initWithoutSkin("-",32.0f);
	m_titleButton[0].setPosition(m_position.x + m_area.right - 96.0f,m_position.y);
	m_titleButton[0].setEventProc(ctrlProc,this);
//	m_titleButton[0].setWithAction(XFalse);
	m_titleButton[1].initWithoutSkin("X",32.0f);
	m_titleButton[1].setPosition(m_position.x + m_area.right - 64.0f,m_position.y);
	m_titleButton[1].setEventProc(ctrlProc,this);
//	m_titleButton[1].setWithAction(XFalse);
	m_titleButton[2].initWithoutSkin("<",32.0f);
	m_titleButton[2].setPosition(m_position.x + m_area.right - 32.0f,m_position.y);
	m_titleButton[2].disable();
	m_titleButton[2].setEventProc(ctrlProc,this);
//	m_titleButton[2].setWithAction(XFalse);
	XCtrlManager.decreaseAObject(&m_titleButton[0]);	//注销这个物件
	XCtrlManager.decreaseAObject(&m_titleButton[1]);	//注销这个物件
	XCtrlManager.decreaseAObject(&m_titleButton[2]);	//注销这个物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_titleButton[0]);
	XObjManager.decreaseAObject(&m_titleButton[1]);
	XObjManager.decreaseAObject(&m_titleButton[2]);
#endif

	updateMouseRect();
	m_isInited = XTrue;
	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
	XCtrlManager.moveToFirst(XCtrlManager.findObjectID(this));
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif

	setTitle(title);
	setAlpha(0.5f);
	return XTrue;
}
void XSubWindow::release()
{
	XCtrlManager.decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	m_ctrlManager.clearAllObject();	//这里需要清空所有的物件
}
void XSubWindow::updateMouseRect()
{
	switch(m_windowState)
	{
	case STATE_NORMAL:
		if(m_windowType != TYPE_NO_TITLE)
		{
			m_curMouseRect.set(m_position.x + m_area.left * m_scale.x,m_position.y + m_area.top * m_scale.y,
				m_position.x + m_area.right * m_scale.x,m_position.y + (m_area.bottom + 32.0f) * m_scale.y);
		}else
		{
			m_curMouseRect.set(m_position.x + m_area.left * m_scale.x,m_position.y + m_area.top * m_scale.y,
				m_position.x + m_area.right * m_scale.x,m_position.y + m_area.bottom * m_scale.y);
		}
		break;
	case STATE_MINISIZE:
		m_curMouseRect.set(m_position.x + m_area.left * m_scale.x,m_position.y + m_area.top * m_scale.y,
			m_position.x + 256.0f * m_scale.x,m_position.y + 32.0f * m_scale.y);
		break;
	}
}
void XSubWindow::draw()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	if(m_windowState == STATE_NORMAL)
	{
		if(m_windowType != TYPE_NO_TITLE)
		{//描绘标题的相关内容
			XRender::drawFillBoxA(m_position + XVector2(m_area.left,m_area.top),
				XVector2(m_area.getWidth(),32.0f) * m_scale,
				XCCS::normalColor * m_color,true);
			m_titleFont.draw();
			m_titleButton[0].draw();
			m_titleButton[1].draw();
			m_titleButton[2].draw();
		}
		//下面描绘工作区
		if(m_withBackGround)
		{//如果存在背景，则下面开始描绘背景
			if(m_windowType == TYPE_NO_TITLE)
			{
				XRender::drawFillBoxA(m_position + XVector2(m_area.left,m_area.top),
					XVector2(m_area.getWidth(),m_area.getHeight()) * m_scale,
					XCCS::downColor * m_color,true);
			}else
			{
				XRender::drawFillBoxA(m_position + XVector2(m_area.left,m_area.top + 32.0f) * m_scale,
					XVector2(m_area.getWidth(),m_area.getHeight()) * m_scale,
					XCCS::downColor * m_color,true);
			}
		}
		//下面开始描绘所有的物件
	}else
	if(m_windowState == STATE_MINISIZE)
	{
		XRender::drawFillBoxA(m_position + XVector2(m_area.left,m_area.top),
			XVector2(256.0f,32.0f) * m_scale,
			0.75f * m_color.fR,0.75f * m_color.fG,0.75f * m_color.fB,1.0f * m_color.fA,true);
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
	m_ctrlManager.draw();
}
void XSubWindow::drawUp()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	if(m_windowState == STATE_NORMAL)
	{
		if(m_windowType != TYPE_NO_TITLE)
		{//描绘标题的相关内容
			m_titleButton[0].drawUp();
			m_titleButton[1].drawUp();
			m_titleButton[2].drawUp();
		}
	}else
	if(m_windowState == STATE_MINISIZE)
	{
		m_titleButton[0].drawUp();
		m_titleButton[1].drawUp();
		m_titleButton[2].drawUp();
	}
//	for(unsigned int i = 0;i < m_objects.size();++ i)
//	{
//		m_objects[i].obj->drawUp();
//	}
}
XBool XSubWindow::mouseProc(float x,float y,XMouseState mouseState)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible) return XFalse; 	//如果不可见直接退出
	if(m_withAction && m_isInAction) return XTrue;	//如果支持动作播放而且正在播放动画那么不接受鼠标控制
	m_curMousePosition.set(x,y);
	if(m_curMouseRect.isInRect(x,y))
	{//下面判断拖拽动作
		if(m_isBeChoose)
		{
			m_titleButton[0].mouseProc(x,y,mouseState);
			m_titleButton[1].mouseProc(x,y,mouseState);
			m_titleButton[2].mouseProc(x,y,mouseState);
		}
		switch(mouseState)
		{
		case MOUSE_LEFT_BUTTON_DCLICK:	
			//改变窗口的状态
			if(m_windowType != TYPE_NO_TITLE && getIsInDragRect(x,y))
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
	if(!m_isBeChoose) return XFalse;		
	if(m_windowType != TYPE_NO_TITLE)
		mouseEventProc(x,y,mouseState);
	m_ctrlManager.mouseProc(x,y,mouseState);
	return XFalse;
}
XBool XSubWindow::keyboardProc(int keyOrder,XKeyState keyState)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible) return XFalse; 	//如果不可见直接退出
	if(m_withAction && m_isInAction) return XTrue;	//如果支持动作播放而且正在播放动画那么不接受鼠标控制
	m_ctrlManager.keyProc(keyOrder,keyState);
	return XFalse;
}
XBool XSubWindow::addACtrlObj(XControlBasic *obj)	//向窗口中添加一个控件
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

	tmpObj.obj->setColor(m_color);
	tmpObj.obj->setScale(tmpObj.scale * m_scale);
	return true;
}
void XSubWindow::setFocus() 
{
	m_isBeChoose = XTrue;
	XCtrlManager.moveToFirst(XCtrlManager.findObjectID(this));
//	for(unsigned int i = 0;i < m_objects.size();++ i)
//	{
//		XCtrlManager.moveAndInsert(XCtrlManager.findObjectID(m_objects[i].obj),XCtrlManager.findObjectID(this));
//	}
	setAlpha(1.0f);
}	
int XSubWindow::getObjectIndex(XControlBasic *obj)
{//ok
	if(obj == NULL) return -1;
	for(unsigned int i = 0;i < m_objects.size();++ i)
	{
		if(m_objects[i].obj == obj) return i;
	}
	return -1;
}
void XSubWindow::setPosition(float x,float y)
{
	if(!m_isInited) return;
	m_position.set(x,y);
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
	updateMouseRect();
	for(unsigned int i = 0;i < m_objects.size();++ i)
	{
		m_objects[i].obj->setPosition(m_objects[i].pos * m_scale + m_position);
	}
}
void XSubWindow::setScale(float x,float y)
{
	if(!m_isInited) return;
	m_scale.set(x,y);
	m_titleFont.setScale(x,y);
	m_titleButton[0].setScale(m_scale);
	m_titleButton[1].setScale(m_scale);
	m_titleButton[2].setScale(m_scale);
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
	updateMouseRect();
	for(unsigned int i = 0;i < m_objects.size();++ i)
	{
		m_objects[i].obj->setPosition(m_objects[i].pos * m_scale + m_position);
		m_objects[i].obj->setScale(m_objects[i].scale * m_scale);
	}
}
void XSubWindow::setMiniSize()
{
	if(m_windowType == TYPE_NO_TITLE) return;	//无窗口模式不能设置为最小化
	m_windowState = STATE_MINISIZE;
	//改变字体的显示，改变按钮的位置

	m_titleFont.setString(XString::getCanShowString(m_titleString.c_str(),256.0f - 96.0f,15.0f).c_str());
		
	m_titleButton[0].setCaptionText("口");
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
void XSubWindow::setNormalSize()
{
	m_windowState = STATE_NORMAL;
	//改变字体的显示，改变按钮的位置

	m_titleFont.setString(XString::getCanShowString(m_titleString.c_str(),m_area.getWidth() - 96.0f,15.0f).c_str());
		
	m_titleButton[0].setCaptionText("-");
	m_titleButton[0].setPosition(m_position.x + (m_area.right - 96.0f) * m_scale.x,m_position.y);
	m_titleButton[1].setPosition(m_position.x + (m_area.right - 64.0f) * m_scale.x,m_position.y);
	m_titleButton[2].setPosition(m_position.x + (m_area.right - 32.0f) * m_scale.x,m_position.y);
	updateMouseRect();
	for(unsigned int i = 0;i < m_objects.size();++ i)
	{
		m_objects[i].obj->setVisible();
	}
}
void XSubWindow::setColor(float r,float g,float b,float a)
{
	m_color.setColor(r,g,b,a);
	m_titleFont.setAlpha(a);
	m_titleButton[0].setColor(m_color);
	m_titleButton[1].setColor(m_color);
	m_titleButton[2].setColor(m_color);
	for(unsigned int i = 0;i < m_objects.size();++ i)
	{
		m_objects[i].obj->setColor(m_color);
	}
}
void XSubWindow::setAlpha(float a)
{
	m_color.setA(a);
	m_titleFont.setAlpha(a);
	m_titleButton[0].setAlpha(a);
	m_titleButton[1].setAlpha(a);
	m_titleButton[2].setAlpha(a);
	for(unsigned int i = 0;i < m_objects.size();++ i)
	{
		m_objects[i].obj->setAlpha(a);
	}
}
void XSubWindow::changeStage(XSubWindowState state)
{
	if(!m_isInited) return;
	if(m_windowState == state) return;
	switch(state)
	{
	case STATE_NORMAL:setNormalSize();break;
	case STATE_MINISIZE:setMiniSize();break;
	}
	if(m_eventProc != 0) m_eventProc(m_pClass,getControlID(),SWD_STATE_CHANGE);
	else XCtrlManager.eventProc(getControlID(),SWD_STATE_CHANGE);
}
void XSubWindow::setLostFocus()	//设置失去焦点
{
	if(!m_isInited ||		//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||		//如果不可见直接退出
		!m_isEnable) return;		//如果无效则直接退出
	setAlpha(0.5f);
	m_isBeChoose = XFalse;	//控件处于焦点状态
	if(m_eventProc != 0) m_eventProc(m_pClass,getControlID(),SWD_LOSE_FOCUS);
	else XCtrlManager.eventProc(getControlID(),SWD_LOSE_FOCUS);
}
XBool XSubWindow::saveState(TiXmlNode &e)
{
	if(!m_needSaveAndLoad) return XTrue;	//如果不需要保存则直接返回
	//遍历所有的控件，保存其状态
	TiXmlElement elmNode(m_ctrlName.c_str());
	bool flag = true;
	for(unsigned int i = 0;i < m_objects.size();++ i)
	{
		if(!m_objects[i].obj->saveState(elmNode)) flag = false;
	}
	if(e.InsertEndChild(elmNode) == NULL) flag = false;
	return flag;
}
XBool XSubWindow::loadState(TiXmlNode *e)
{
	if(!m_needSaveAndLoad) return XTrue;	//如果不需要保存则直接返回
	//遍历所有的空间，读取其状态
	TiXmlNode * keyNode = e->FirstChildElement(m_ctrlName.c_str());
	if(keyNode == NULL) return XFalse;
	bool flag = true;
	for(unsigned int i = 0;i < m_objects.size();++ i)
	{
		if(!m_objects[i].obj->loadState(keyNode)) flag = false;
	}
	return flag;
}
#if !WITH_INLINE_FILE
#include "XSubWindow.inl"
#endif
}