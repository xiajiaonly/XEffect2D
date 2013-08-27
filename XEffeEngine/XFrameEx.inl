//inline 函数
inline void _XFrameEx::setPosition(const _XVector2& position)
{
	setPosition(position.x,position.y);
}

inline void _XFrameEx::setPosition(float a,float b)
{
	if(m_isInited == 0) return;
	x = a;
	y = b;
	for(int i = 0;i < m_allKeyFramesSum;++ i)
	{
		m_pSprite[i].setPosition(x,y);
	}
}

inline _XVector2 _XFrameEx::getPosition() const
{
	return _XVector2(x,y);
}

inline void _XFrameEx::setSize(const _XVector2& size)
{
	setSize(size.x,size.y);
}

inline void _XFrameEx::setSize(float x,float y)
{
	if(m_isInited == 0) return;
	xsize = x;
	ysize = y;
	for(int i = 0;i < m_allKeyFramesSum;++ i)
	{
		m_pSprite[i].setSize(xsize,ysize);
	}
}

inline _XVector2 _XFrameEx::getSize()const
{
	return _XVector2(xsize,ysize);
}

inline int _XFrameEx::getIsEnd() const
{
	return m_isEnd;
}

inline void _XFrameEx::setEnd()
{
	if(m_isEndImmediately == 0)
	{
		m_isSetEnd = 1;
	}else
	{
		m_isSetEnd = 1;
		m_isEnd = 1;
	}
}

inline void _XFrameEx::reset()	//设置已经播放完成的序列帧动画重新播放
{
	if(m_isEnd != 0)
	{
		m_isEnd = 0;
		m_isSetEnd = 0;
		m_nowFramesNumble = (float)m_startFrame;
	}
}

inline _XSprite * _XFrameEx::getNowSprite(char frameFlag)
{
	if(m_isInited == 0) return NULL;
	if(frameFlag == 0)
	{
		return &(m_pSprite[(int)(m_nowFramesNumble)]);
	}else
	{
		return m_pSprite;
	}
}

inline void _XFrameEx::setAngle(float temp)
{
	if(m_isInited == 0) return;
	angle = temp;
	for(int i = 0;i < m_allKeyFramesSum;++ i)
	{
		m_pSprite[i].setAngle(angle);
	}
}
inline float _XFrameEx::getAngle() const
{
	return angle;
}

inline void _XFrameEx::setColor(float r,float g,float b,float a)
{
	if(m_isInited == 0) return;
	if(r >= 0) colorRed = r;
	if(g >= 0) colorGreen = g;
	if(b >= 0) colorBlue = b;
	if(a >= 0) alpha = a;
	for(int i = 0;i < m_allKeyFramesSum;++ i)
	{
		m_pSprite[i].setColor(r,g,b,a);
	}
}

inline void _XFrameEx::setAlpha(float a)
{
	if(m_isInited == 0) return;
	if(a >= 0) alpha = a;
	for(int i = 0;i < m_allKeyFramesSum;++ i)
	{
		m_pSprite[i].setAlpha(a);
	}
}

inline float _XFrameEx::getAlpha() const
{
	return alpha;
}

inline void _XFrameEx::setActionSpeed(float actionSpeed)	//设置序列帧播放的速度
{
	m_actionSpeed = actionSpeed;
}

inline void _XFrameEx::setRestart()
{
	m_nowFramesNumble = (float)(m_startFrame);
}
