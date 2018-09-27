INLINE void XParameterCtrl::setCurValue(float value)
{
	m_curData = value;
	m_font.setString(XString::toString(m_curData).c_str());
}
INLINE void XParameterCtrl::setParamType(XParamType type, float iRate, float rRate)
{
	m_paramType = type;
	m_reduceRate = rRate;
	m_increaseRate = iRate;
}
INLINE XBool XParameterCtrl::canGetFocus(const XVec2& p)				//用于判断当前物件是否可以获得焦点
{
	if (!m_isInited ||		//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||		//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	return m_curMouseRect.isInRect(p);
}
INLINE void XParameterCtrl::setPosition(const XVec2& p)
{
	m_position = p;
	m_btn[0].setPosition(m_position);
	m_btn[1].setPosition(m_position.x + m_pixelSize.x * m_scale.x - m_pixelSize.y * m_scale.y, m_position.y);
	m_font.setPosition(m_position + m_pixelSize * m_scale * 0.5f);	//字放在正中间
	m_curMouseRect.set(m_position, m_position + m_pixelSize * m_scale);
}
INLINE void XParameterCtrl::setScale(const XVec2& s)
{
	m_scale = s;
	m_btn[0].setScale(m_scale);
	m_btn[0].setPosition(m_position);
	m_btn[1].setScale(m_scale);
	m_btn[1].setPosition(m_position.x + m_pixelSize.x * m_scale.x - m_pixelSize.y * m_scale.y, m_position.y);
	m_font.setScale(m_scale);
	m_font.setPosition(m_position + m_pixelSize * m_scale * 0.5f);	//字放在正中间
	m_curMouseRect.set(m_position, m_position + m_pixelSize * m_scale);
}
INLINE void XParameterCtrl::setColor(const XFColor& c)
{
	m_color = c;
	m_font.setColor(m_textColor * m_color);
	m_btn[0].setColor(m_color);
	m_btn[1].setColor(m_color);
	updateChildColor();
}
INLINE void XParameterCtrl::setAlpha(float a)
{
	m_color.setA(a);
	m_font.setColor(m_textColor * m_color);
	m_btn[0].setColor(m_color);
	m_btn[1].setColor(m_color);
	updateChildColor();
}
INLINE void XParameterCtrl::draw()
{
	if (!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
								//描绘中间的背景
	XRender::drawFillRectA(m_position, m_pixelSize * m_scale, XFColor::gray, true);
	m_font.draw();
	m_btn[0].draw();
	m_btn[1].draw();
}
INLINE void XParameterCtrl::drawUp()
{
	if (!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	m_btn[0].drawUp();
	m_btn[1].drawUp();
}
INLINE void XParameterCtrl::update(float stepTime)
{
	if (!m_isInited) return;
	m_btn[0].update(stepTime);
	m_btn[1].update(stepTime);
}
INLINE XBool XParameterCtrl::mouseProc(const XVec2& p, XMouseState mouseState)		//对于鼠标动作的响应函数
{
	if (!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible) return XFalse; 	//如果不可见直接退出
	if (m_withAction && m_isInAction) return XTrue;	//如果支持动作播放而且正在播放动画那么不接受鼠标控制
	if (m_isSilent) return XFalse;
	XBool ret = m_btn[0].mouseProc(p, mouseState);
	XBool ret1 = m_btn[1].mouseProc(p, mouseState);
	return (ret || ret1);
}
INLINE XBool XParameterCtrl::keyboardProc(int keyOrder, XKeyState keyState)			//返回是否触发按键动作
{
	if (!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	if (m_withAction && m_isInAction) return XTrue;	//如果支持动作播放而且正在播放动画那么不接受鼠标控制
	if (m_isSilent) return XFalse;
	XBool ret = m_btn[0].keyboardProc(keyOrder, keyState);
	XBool ret1 = m_btn[1].keyboardProc(keyOrder, keyState);
	return (ret || ret1);
}
INLINE void XParameterCtrl::setLostFocus()	//设置失去焦点
{
	if (!m_isInited ||		//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||		//如果不可见直接退出
		!m_isEnable) return;		//如果无效则直接退出

	m_isBeChoose = XFalse;	//控件处于焦点状态
}
INLINE XBool XParameterCtrl::isInRect(const XVec2& p)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if (!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(p);
}
INLINE XVec2 XParameterCtrl::getBox(int order)			//获取四个顶点的坐标，目前先不考虑旋转和缩放
{
	if (!m_isInited) return XVec2::zero;
	switch (order)
	{
	case 0:return m_curMouseRect.getLT();
	case 1:return m_curMouseRect.getRT();
	case 2:return m_curMouseRect.getRB();
	case 3:return m_curMouseRect.getLB();
	}
	return XVec2::zero;
}
