//INLINE  函数
INLINE void XFrameEx::setPositionX(float a)
{
	setPosition(a, m_position.y);
}
INLINE void XFrameEx::setPositionY(float b)
{
	setPosition(m_position.x, b);
}
INLINE void XFrameEx::setEnd()
{
	if (!m_isEndImmediately)
	{
		m_isSetEnd = XTrue;
	}
	else
	{
		m_isSetEnd = XTrue;
		m_isEnd = XTrue;
	}
}
INLINE void XFrameEx::reset()	//设置已经播放完成的序列帧动画重新播放
{
	if (!m_isEnd) return;	//如果没有结束，则不能重置，这个逻辑好像并不严谨
	m_isEnd = XFalse;
	m_isSetEnd = XFalse;
	m_curFramesNumble = (float)m_startFrame;
}
INLINE XSprite * XFrameEx::getCurSprite(bool isAllFrames)
{
	if (!m_isInited) return NULL;
	if (isAllFrames)
		return m_pSprite;
	else
	{
		int temp = m_curFramesNumble;
		if (m_curFramesNumble < 0 || m_curFramesNumble >= m_allFramesSum) temp = 0;
		if (m_keyFrameArray[temp] < 0 || m_keyFrameArray[temp] >= m_allKeyFramesSum) return NULL;
		return &(m_pSprite[m_keyFrameArray[temp]]);
	}
}
INLINE float XFrameEx::getAngle() const
{
	return m_angle;
}
INLINE float XFrameEx::getAlpha() const
{
	return m_color.a;
}
INLINE void XFrameEx::setActionSpeed(float actionSpeed)	//设置序列帧播放的速度
{
	m_actionSpeed = actionSpeed;
}
INLINE void XFrameEx::setRestart()
{
	m_curFramesNumble = (float)(m_startFrame);
}
INLINE void XFrameEx::setIsTransformCenter(XTransformMode temp)
{//尚未实现
	if (!m_isInited) return;
	for (int i = 0; i < m_allKeyFramesSum; ++i)
	{
		m_pSprite[i].setIsTransformCenter(temp);
	}
}
INLINE void XFrameEx::setTurnOverMode(XTurnOverMode mode)
{
	if (!m_isInited) return;
	for (int i = 0; i < m_allKeyFramesSum; ++i)
	{
		m_pSprite[i].setTurnOverMode(mode);
	}
}
INLINE void XFrameEx::setBlendModel(XGL::XBlendModel model)
{
	for (int i = 0; i < m_allKeyFramesSum; ++i)
	{
		m_pSprite[i].setBlendModel(model);
	}
}
//INLINE void XFrameEx::setBlendType(int typeScr, int typeDst)	//设置混合模式
//{
//	m_blendTypeScr = XMath::clamp(typeScr, 0, 8);
//	m_blendTypeDst = XMath::clamp(typeDst, 0, 7);
//	for (int i = 0; i < m_allKeyFramesSum; ++i)
//	{
//		m_pSprite[i].setBlendType(m_blendTypeScr, m_blendTypeDst);
//	}
//}
INLINE const XFColor& XFrameEx::getColor() const
{
	if (m_pSprite == NULL) return XFColor::white;
	else return m_pSprite[0].getColor();
}
INLINE void XFrameEx::setVisible()
{
	m_isVisible = XTrue;
	updateChildVisible();
}					//设置物件可见
INLINE void XFrameEx::disVisible()
{
	m_isVisible = XFalse;
	updateChildVisible();
}