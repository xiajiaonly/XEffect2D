INLINE void XSliderEx::disable()//使控件无效
{
	m_mainSld.disable();
	m_secondarySld.disable();
	m_chooseBtn.disable();
	m_isEnable = XFalse;
	m_isBeChoose = XFalse;
	m_secondarySld.disVisible();
	m_chooseBtn.disVisible();
}
INLINE void XSliderEx::enable()//使控件有效
{
	m_mainSld.enable();
	m_secondarySld.enable();
	m_chooseBtn.enable();
	m_isEnable = XTrue;
}
INLINE float XSliderEx::getCurValue() const	//获取滑块当前的值
{
	return m_mainSld.getCurValue();
}
INLINE float XSliderEx::getMaxValue() const
{
	return m_mainSld.getMaxValue();
}
INLINE float XSliderEx::getMinValue() const
{
	return m_mainSld.getMinValue();
}
//INLINE void XSliderEx::setCallbackFun(void (* funInit)(void *,int),
//	void (* funRelease)(void *,int),
//	void (* funMouseOn)(void *,int),
//	void (* funMouseDown)(void *,int),
//	void (* funMouseUp)(void *,int),
//	void (* funValueChange)(void *,int),
//	void (* funMouseMove)(void *,int),
//	void *pClass)
//{
//	m_funInit = funInit;
//	m_funRelease = funRelease;
//	m_funMouseOn = funMouseOn;
//	m_funMouseDown = funMouseDown;		//有效
//	m_funMouseUp = funMouseUp;
//	m_funValueChange = funValueChange;
//	m_funMouseMove = funMouseMove;
//	m_pClass = pClass;
//}
INLINE void XSliderEx::setCurValue(float temp, bool withEvent)
{
	m_mainSld.setCurValue(temp, withEvent);
}
INLINE void XSliderEx::setRange(float max,float min)
{
	m_mainSld.setRange(max,min);
}
INLINE XBool XSliderEx::isInRect(const XVec2& p) //点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return XMath::getIsInRect(p,getBox(0),getBox(1),getBox(2),getBox(3));
}
INLINE XVec2 XSliderEx::getBox(int order)		//获取四个顶点的坐标，目前先不考虑旋转和缩放
{
	if(!m_isInited) return XVec2::zero;
	if(m_mainSld.m_typeVorH == SLIDER_TYPE_HORIZONTAL)
	{
		if(m_chooseBtn.getVisible() && m_secondarySld.getVisible())
		{
			switch(order)
			{
			case 0:return m_mainSld.getBox(0);
			case 1:return m_chooseBtn.getBox(1);
			case 2:return XVec2(m_chooseBtn.getBox(2).x,m_secondarySld.getBox(2).y);
			case 3:return m_secondarySld.getBox(3);
			}
		}else
		if(m_chooseBtn.getVisible())
		{
			switch(order)
			{
			case 0:return m_mainSld.getBox(0);
			case 1:return m_chooseBtn.getBox(1);
			case 2:return m_chooseBtn.getBox(2);
			case 3:return m_mainSld.getBox(3);
			}
		}else
		if(m_secondarySld.getVisible())
		{
			switch(order)
			{
			case 0:return m_mainSld.getBox(0);
			case 1:return m_mainSld.getBox(1);
			case 2:return m_secondarySld.getBox(2);
			case 3:return m_secondarySld.getBox(3);
			}
		}else
		{
			return m_mainSld.getBox(order);
		}
	}else
	{//这里可能会有问题
		if(m_chooseBtn.getVisible() && m_secondarySld.getVisible())
		{
			switch(order)
			{
			case 0:return m_secondarySld.getBox(0);
			case 1:return m_mainSld.getBox(1);
			case 2:return m_chooseBtn.getBox(2);
			case 3:return XVec2(m_secondarySld.getBox(3).x,m_chooseBtn.getBox(3).y);
			}
		}else
		if(m_chooseBtn.getVisible())
		{
			switch(order)
			{
			case 0:return m_mainSld.getBox(0);
			case 1:return m_mainSld.getBox(1);
			case 2:return m_chooseBtn.getBox(2);
			case 3:return m_chooseBtn.getBox(3);
			}
		}else
		if(m_secondarySld.getVisible())
		{
			switch(order)
			{
			case 0:return m_secondarySld.getBox(0);
			case 1:return m_mainSld.getBox(1);
			case 2:return m_mainSld.getBox(2);
			case 3:return m_secondarySld.getBox(3);
			}
		}else
		{
			return m_mainSld.getBox(order);
		}
	}
	return XVec2::zero;
}
INLINE void XSliderEx::setColor(const XFColor& c)
{
	if(!m_isInited) return;
	m_color = c;
	m_mainSld.setColor(m_color);
	m_secondarySld.setColor(m_color);
	m_chooseBtn.setColor(m_color);
	updateChildColor();
}//设置字体的颜色
INLINE void XSliderEx::setAlpha(float a)
{
	if(!m_isInited) return;
	m_color.setA(a);
	m_mainSld.setColor(m_color);
	m_secondarySld.setColor(m_color);
	m_chooseBtn.setColor(m_color);
	updateChildAlpha();
}
//下面是一些公共方法
INLINE XBool XSliderEx::setFont(const char *caption,const XFontUnicode& font,float captionSize,const XVec2& fontPosition)
{
	return m_mainSld.setFont(caption,font,captionSize,fontPosition);
}
INLINE XBool XSliderEx::keyboardProc(int keyOrder,XKeyState keyState)
{
	if(m_isSilent) return XFalse;
	return m_mainSld.keyboardProc(keyOrder,keyState);
}
//INLINE void XSliderEx::setLostFocus() 
//{
//	if(!m_isInited ||	//如果没有初始化直接退出
//		!m_isActive ||		//没有激活的控件不接收控制
//		!m_isVisible ||	//如果不可见直接退出
//		!m_isEnable) return;		//如果无效则直接退出
//
//	//m_mainSld.setLostFocus();
//	//m_secondarySld.setLostFocus();
//	//m_chooseBtn.setLostFocus();
//	m_isBeChoose = XFalse;
//}
INLINE void XSliderEx::setVisible()
{
	m_isVisible = XTrue;
	m_mainSld.setVisible();
	//m_secondarySld.setVisible();
	//m_chooseBtn.setVisible();
	updateChildVisible();
}
INLINE void XSliderEx::disVisible()
{
	m_isVisible = XFalse;
	m_isBeChoose = XFalse;
	m_mainSld.disVisible();
	m_secondarySld.disVisible();
	m_chooseBtn.disVisible();
	updateChildVisible();
}
INLINE void XSliderEx::draw()//描绘滑动条
{
	if(!m_isInited ||			//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出

	m_mainSld.draw();
	m_secondarySld.draw();
	m_chooseBtn.draw();
}
INLINE void XSliderEx::drawUp()
{
	if(!m_isInited ||			//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出

	m_mainSld.drawUp();
	m_secondarySld.drawUp();
	m_chooseBtn.drawUp();

	m_comment.draw();
}
INLINE XBool XSliderEx::mouseProc(const XVec2& p,XMouseState mouseState)	//对于鼠标动作的响应函数
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	if(m_isSilent) return XFalse;

	m_mainSld.mouseProc(p,mouseState);
	m_secondarySld.mouseProc(p,mouseState);
	m_chooseBtn.mouseProc(p,mouseState);
	if (m_mainSld.isInRect(p))
	{
		if (mouseState == MOUSE_LEFT_BUTTON_UP) m_isBeChoose = true;
		if (!m_isMouseInRect)
		{
			m_isMouseInRect = XTrue;
			m_comment.setShow();
			setCommentPos(p + XVec2(0.0f, 16.0f));
		}
		if (mouseState != MOUSE_MOVE && m_comment.getIsShow())
			m_comment.disShow();	//鼠标的任意操作都会让说明框消失
	}
	else
	{
		if (m_isMouseInRect)
		{
			m_isMouseInRect = XFalse;
			m_comment.disShow();
		}
	}
	return XTrue;
}
INLINE XBool XSliderEx::canGetFocus(const XVec2& p)	//用于判断当前物件是否可以获得焦点
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	return isInRect(p);
}
INLINE void XSliderEx::setOprateState(void * data)
{
	float index = *(float *)data;
	setCurValue(index);
}
INLINE void *XSliderEx::getOprateState() const
{
	float *data = XMem::createMem<float>();
	*data = getCurValue();
	return data;
}
INLINE void XSliderEx::releaseOprateStateData(void *p)
{
	float *data = (float *)p;
	XMem::XDELETE(data);
}
INLINE bool XSliderEx::isSameState(void * data)
{
	if(data == NULL) return false;
	return(*(float*)data == getCurValue());
}