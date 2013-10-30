//inline 函数
inline void _XSprite::setAlpha(float temp)
{
	alpha = temp;
#if IS_USE_SOLIDIFY
	m_needSolidify = 1;
#endif
}

inline float _XSprite::getAlpha() const
{
	return alpha;
}

inline void _XSprite::setAngle(float temp)
{
	angle = temp;
	sinAngle = sin(angle * ANGLE_TO_RADIAN);
	cosAngle = cos(angle * ANGLE_TO_RADIAN);
	m_needUpdateInsideData = 1;
#if IS_USE_SOLIDIFY
	m_needSolidify = 1;
#endif
//	setPosition(m_setPosition);
}

inline int _XSprite::getW() const
{
	return m_textureData.texture.m_w;
}

inline int _XSprite::getH() const
{
	return m_textureData.texture.m_h;
}
#if IS_USE_SOLIDIFY
inline void _XSprite::setIsUseSolidify(int temp)
{
	if(temp == 0)
	{
		m_isUseSolidify = 0;
	}else
	{
		m_isUseSolidify = 1;
		m_needSolidify = 1;
	}
};
#endif

inline _XTexture * _XSprite::getTexture()
{
	return &(m_textureData.texture);
}

inline _XTextureData * _XSprite::getTextureData()
{
	return &m_textureData;
}

inline void _XSprite::setColor(const _XFColor& color)
{
	colorRed = color.getR();
	colorGreen = color.getG();
	colorBlue = color.getB();
	alpha = color.getA();
#if IS_USE_SOLIDIFY
	m_needSolidify = 1;
#endif
}

inline void _XSprite::setColor(float r,float g,float b,float a)
{
	if(r >= 0) colorRed = r;
	if(g >= 0) colorGreen = g;
	if(b >= 0) colorBlue = b;
	if(a >= 0) alpha = a;
#if IS_USE_SOLIDIFY
	m_needSolidify = 1;
#endif
}

inline void _XSprite::setPosition(const _XVector2& position)
{
	setPosition(position.x,position.y);
}

inline void _XSprite::resetLeftRightUpDown()
{
	m_turnOverMode = _TURN_OVER_MODE_NULL;
}

inline void _XSprite::setPosition(float a,float b)
{
	m_setPosition.set(a,b);
	m_needUpdateInsideData = 1;

#if IS_USE_SOLIDIFY
	m_needSolidify = 1;
#endif
}


inline void _XSprite::setSize(const _XVector2& size)
{
	setSize(size.x,size.y);
}

inline void _XSprite::setSize(float a,float b)
{
	xsize = a;
	ysize = b;
	m_needUpdateInsideData = 1;

#if IS_USE_SOLIDIFY
	m_needSolidify = 1;
#endif
}

inline void _XSprite::setOverturn(char temp)
{
	if(temp == 0)
	{
		resetLeftRightUpDown();
	}else
	{
		setLeft2Right();
	}
}

inline void _XSprite::disClip()
{
	setClipRect(0.0f,0.0f,m_textureData.textureSize.x,m_textureData.textureSize.y);
	m_isEnableOutsideChip = 0;
}


inline void _XSprite::setClipRect(_XRect temp)
{//设置在精灵内部进行裁减
	setClipRect(temp.left,temp.top,temp.right,temp.bottom);
}

inline int _XSprite::isInRect(float x,float y)
{
	if(m_isInited == 0) return 0;
	return getIsInRect(_XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
}
