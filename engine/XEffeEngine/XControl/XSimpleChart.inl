INLINE void XSimpleChart::clearAll()
{
	m_datas.clear();
	m_isFirstPush = false;	//是否是第一次放入数据
	m_maxData = 0.0f;	//推入数据的最大值
	m_minData = 0.0f;	//推入数据的最小值
	m_curData = 0.0f;
	m_localMaxData = 0.0f;
	m_localMinData = 0.0f;
	m_localAverageData = 0.0f;
	m_localSDData = 0.0f;
	m_rateY = 1.0f;
	updateInfoStr();
}
INLINE void XSimpleChart::updateInfoStr()
{
	char tmpStr[256];
	switch (m_showInfo)
	{
	case INFO_NULL:break;
	case INFO_CUR_VALUE:
		sprintf(tmpStr,"Val:%8.5f",m_curData);
		m_infoFont.setString(tmpStr);
		break;
	case INFO_ALL_RANGLE:
		sprintf(tmpStr,"ARng:%8.5f - %8.5f",m_minData,m_maxData);
		m_infoFont.setString(tmpStr);
		break;
	case INFO_LOCAL_RANGLE:
		sprintf(tmpStr,"LRng:%8.5f - %8.5f",m_localMinData,m_localMaxData);
		m_infoFont.setString(tmpStr);
		break;
	case INFO_LOCAL_AVERAGE:
		sprintf(tmpStr,"Ave:%8.5f",m_localAverageData);
		m_infoFont.setString(tmpStr);
		break;
	case INFO_LOCAL_SD:
		sprintf(tmpStr,"SD:%8.5f",m_localSDData);
		m_infoFont.setString(tmpStr);
		break;
	}
}
INLINE void XSimpleChart::setPosition(float x,float y)
{
	m_position.set(x,y);
	m_caption.setPosition(m_position.x + 1.0f * m_scale.x,m_position.y + 1.0f * m_scale.y);
	m_infoFont.setPosition(m_position.x + 1.0f * m_scale.x,m_position.y + (1.0f + 32.0f) * m_scale.y);
	m_curMouseRect.set(m_position,m_position + m_size * m_scale);
	updateChildPos();
}
INLINE void XSimpleChart::setScale(float x,float y)
{
	if(x < 0.0f || y < 0.0f) return;
	m_scale.set(x,y);
	m_caption.setScale(m_scale);
	m_caption.setPosition(m_position.x + 1.0f * m_scale.x,m_position.y + 1.0f * m_scale.y);
	m_infoFont.setScale(m_scale);
	m_infoFont.setPosition(m_position.x + 1.0f * m_scale.x,m_position.y + (1.0f + 32.0f) * m_scale.y);
	m_curMouseRect.set(m_position,m_position + m_size * m_scale);
	updateChildScale();
}
INLINE void XSimpleChart::setColor(float r,float g,float b,float a)
{
	m_color.setColor(r,g,b,a);
	m_caption.setColor(m_textColor * m_color);
	m_infoFont.setColor(m_textColor * m_color);
	updateChildColor();
}
INLINE void XSimpleChart::setAlpha(float a)
{
	m_color.setA(a);
	m_caption.setColor(m_textColor * m_color);
	m_infoFont.setColor(m_textColor * m_color);
	updateChildAlpha();
}
INLINE void XSimpleChart::setCaption(const char *p)
{
	if(!m_isInited) return;
	if(p == NULL)
	{
		m_showCaption = false;
	}else
	{
		m_caption.setString(p);
		m_showCaption = true;
	}
}
INLINE XBool XSimpleChart::isInRect(float x,float y)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(x,y);
}
INLINE XVector2 XSimpleChart::getBox(int order)			//获取四个顶点的坐标，目前先不考虑旋转和缩放
{
	if(!m_isInited) return XVector2::zero;
	switch(order)
	{
	case 0:return XVector2(m_curMouseRect.left,m_curMouseRect.top);
	case 1:return XVector2(m_curMouseRect.right,m_curMouseRect.top);
	case 2:return XVector2(m_curMouseRect.right,m_curMouseRect.bottom);
	case 3:return XVector2(m_curMouseRect.left,m_curMouseRect.bottom);
	}
	return XVector2::zero;
}
INLINE XBool XSimpleChart::mouseProc(float x,float y,XMouseState mouseState)		//对于鼠标动作的响应函数
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible) return XFalse; 	//如果不可见直接退出
	if(m_withAction && m_isInAction) return XTrue;	//如果支持动作播放而且正在播放动画那么不接受鼠标控制

	return m_curMouseRect.isInRect(x,y);
}
INLINE XBool XSimpleChart::canGetFocus(float x,float y)				//用于判断当前物件是否可以获得焦点
{
	if(!m_isInited ||		//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||		//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	return m_curMouseRect.isInRect(x,y);
}