//INLINE 函数
#if WITHXSPRITE_EX
INLINE void XSprite::setAngle(float angle)		//[0-360]
{
	m_angle = angle;	//反一下方向
	//m_sinAngle = sinf((360.0f - m_angle) * DEGREE2RADIAN);				//角度的零时变量
	//m_cosAngle = cosf((360.0f - m_angle) * DEGREE2RADIAN);
	m_sinAngle = cos((270.0f - m_angle) * DEGREE2RADIAN);				//角度的零时变量
	m_cosAngle = cos((360.0f - m_angle) * DEGREE2RADIAN);
	updateChildAngle();
	m_needUpdateData = XTrue;
}
INLINE float XSprite::getAngle() const { return m_angle; }
INLINE void XSprite::setScale(const XVec2& s)	//设置精灵的缩放比例
{
	m_scale = s;
	updateChildScale();
	m_needUpdateData = XTrue;
}
INLINE void XSprite::fitScale(float x, float y, bool deformation)
{
	if (deformation) setScale(x / getW(), y / getH());
	else setScale((std::min)(x / getW(), y / getH()));
}
INLINE void XSprite::fitScale(const XVec2& s, bool deformation)
{
	if (deformation) setScale(s / XVec2(getW(), getH()));
	else setScale((std::min)(s.x / getW(), s.y / getH()));
}
INLINE const XVec2& XSprite::getScale() const { return m_scale; }
INLINE void XSprite::setPosition(const XVec2& p)			//设置精灵的坐标
{
	m_position = p;
	updateChildPos();
	m_needUpdateData = XTrue;
}
INLINE void XSprite::setPositionX(float x)
{
	m_position.x = x;
	updateChildPos();
	m_needUpdateData = XTrue;
}
INLINE void XSprite::setPositionY(float y)
{
	m_position.y = y;
	updateChildPos();
	m_needUpdateData = XTrue;
}
INLINE const XVec2& XSprite::getPosition() const { return m_position; }
INLINE void XSprite::setTurnOverMode(XTurnOverMode mode)
{
	m_turnOverMode = mode;
	m_needUpdateData = XTrue;
}
INLINE void XSprite::setClipRect(const XRect& clipRect)
{
	//if(m_textureData.isEnableInsideClip == 0)
	//{
	//	m_clipRect.set(clipRect.left / m_pixelSize.x,clipRect.top / m_pixelSize.y,clipRect.right / m_pixelSize.x,clipRect.bottom / m_pixelSize.y);
	//}else
	//{
	//	m_clipRect.set((clipRect.left + m_textureData.clipInsideRect.left) / m_textureData.texture.m_w,
	//		(clipRect.top + m_textureData.clipInsideRect.top) / m_textureData.texture.m_h,
	//		(clipRect.right + m_textureData.clipInsideRect.left) / m_textureData.texture.m_w,
	//		(clipRect.bottom + m_textureData.clipInsideRect.top) / m_textureData.texture.m_h);
	//}
	m_clipRect = clipRect;
	m_needClip = m_needUpdateData = XTrue;
}
INLINE void XSprite::setClipRect(float left, float top, float right, float bottom)
{
	setClipRect(XRect(left, top, right, bottom));
}
INLINE void XSprite::disClip()
{
	if (m_textureData.isEnableInsideClip == 0)
	{
		if (!m_needClip) return;
		m_needClip = XFalse;
	}
	else
	{
		m_needClip = XTrue;
		m_clipRect.set(m_textureData.clipInsideRect.left / m_textureData.texture.m_w,
			m_textureData.clipInsideRect.top / m_textureData.texture.m_h,
			m_textureData.clipInsideRect.right / m_textureData.texture.m_w,
			m_textureData.clipInsideRect.bottom / m_textureData.texture.m_h);
	}
	m_needUpdateData = XTrue;
}
INLINE void XSprite::setChangeCenter(const XVec2& center)
{
	m_changeCenter = center;
	m_needUpdateData = XTrue;
}
INLINE XBool XSprite::isInRect(const XVec2& p)	//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if (!m_isInited) return XFalse;
	return XMath::getIsInRect(p, getBox(0), getBox(1), getBox(2), getBox(3));
}
INLINE XVec2 XSprite::getBox(int order)
{
	if (!m_isInited) return XVec2::zero;
	updateData();
	//if(order >= 0 && order < 4) return m_vPoint[order];
	if (order >= 0 && order < 4) return m_rectPoint[order];
	else return XVec2::zero;
}
INLINE void XSprite::setAlpha(float temp)
{
	m_color.a = XMath::clamp(temp, 0.0f, 1.0f);
	updateChildAlpha();
}
INLINE float XSprite::getAlpha() const { return m_color.a; }
INLINE void XSprite::setColor(const XFColor&c)		//小于0不会改变这一元素
{
	if (c.r >= 0.0f) m_color.r = c.r;
	if (c.g >= 0.0f) m_color.g = c.g;
	if (c.b >= 0.0f) m_color.b = c.b;
	if (c.a >= 0.0f) m_color.a = c.a;
	updateChildColor();
}
INLINE void XSprite::setVisible()
{
	m_isVisible = XTrue;
	updateChildVisible();
}					//设置物件可见
INLINE void XSprite::disVisible()
{
	m_isVisible = XFalse;
	updateChildVisible();
}						//设置物件不可见
INLINE XBool XSprite::getVisible() const { return m_isVisible; }
//INLINE void XSprite::setBlendType(int typeScr, int typeDst)
//{
//	m_blendTypeScr = XMath::clamp(typeScr, 0, 8);
//	m_blendTypeDst = XMath::clamp(typeDst, 0, 7);
//}
//INLINE void XSprite::setBlendType(int rgbScr, int rgbDst, int aScr, int aDst)
//{
//	m_blendRGBScr = XMath::clamp(rgbScr, 0, 8);
//	m_blendRGBDst = XMath::clamp(rgbDst, 0, 7);
//	m_blendAScr = XMath::clamp(aScr, 0, 8);;
//	m_blendADst = XMath::clamp(aDst, 0, 7);
//}
//INLINE void XSprite::setBlendMode(XGL::XBlendType mode) { m_blendType = mode; }
INLINE void XSprite::setBlendModel(XGL::XBlendModel model)
{
	m_blendModel = model;
}
//shader的接口
INLINE void XSprite::setShaderClass(XShaderGLSL * shader) { m_pShader = shader; }
INLINE XTexture * XSprite::getTexture() { return &(m_textureData.texture); }
INLINE XTextureData * XSprite::getTextureData() { return &m_textureData; }
INLINE void XSprite::setAngleClip(float angle)
{
	m_needAngleClip = XTrue;
	m_clipAngle = angle;	//这个角度需要化简
	m_needUpdateData = XTrue;
}
INLINE void XSprite::disAngleClip()
{
	m_needAngleClip = XFalse;
	m_needUpdateData = XTrue;
}
INLINE void XSprite::drawInside()
{
	//方案1：
	//glBegin(GL_TRIANGLE_FAN);
	//for (int i = 0,index = 0; i < m_pointSum; ++i,index += 2)
	//{
	//	glTexCoord2f(m_uPointer[index], m_uPointer[index + 1]);
	//	glVertex2f(m_vPointer[index], m_vPointer[index + 1]);
	//}
	//glEnd();
	//方案2：
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, m_vPointer);	//2表示每个顶点由两个量组成，GL_FLOAT表示每个量都是	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);	//启用颜色数组
	glTexCoordPointer(2, GL_FLOAT, 0, m_uPointer);
	glDrawArrays(GL_TRIANGLE_FAN, 0, m_pointSum);	//rational:MLK
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
INLINE XSprite::XSprite(const XSprite& temp)
{
	if (&temp == this) return;
}
#else
INLINE void XSprite::setAlpha(float temp)
{
	alpha = temp;
#if IS_USE_SOLIDIFY
	m_needSolidify = 1;
#endif
}
INLINE float XSprite::getAlpha() const
{
	return alpha;
}
INLINE void XSprite::setAngle(float temp)
{
	angle = temp;
	sinAngle = sin(angle * DEGREE2RADIAN);
	cosAngle = cos(angle * DEGREE2RADIAN);
	m_needUpdateInsideData = 1;
#if IS_USE_SOLIDIFY
	m_needSolidify = 1;
#endif
	//	setPosition(m_setPosition);
}
INLINE int XSprite::getW() const
{
	return m_textureData.texture.m_w;
}
INLINE int XSprite::getH() const
{
	return m_textureData.texture.m_h;
}
#if IS_USE_SOLIDIFY
INLINE void XSprite::setIsUseSolidify(int temp)
{
	if (temp == 0)
	{
		m_isUseSolidify = 0;
	}
	else
	{
		m_isUseSolidify = 1;
		m_needSolidify = 1;
	}
};
#endif
INLINE XTexture * XSprite::getTexture()
{
	return &(m_textureData.texture);
}
INLINE XTextureData * XSprite::getTextureData()
{
	return &m_textureData;
}
INLINE void XSprite::setColor(const XFColor& color)
{
	colorRed = color.getR();
	colorGreen = color.getG();
	colorBlue = color.getB();
	alpha = color.getA();
#if IS_USE_SOLIDIFY
	m_needSolidify = 1;
#endif
}
INLINE void XSprite::setColor(float r, float g, float b, float a)
{
	if (r >= 0) colorRed = r;
	if (g >= 0) colorGreen = g;
	if (b >= 0) colorBlue = b;
	if (a >= 0) alpha = a;
#if IS_USE_SOLIDIFY
	m_needSolidify = 1;
#endif
}
INLINE void XSprite::setPosition(const XVec2& position)
{
	setPosition(position.x, position.y);
}
INLINE void XSprite::resetLeftRightUpDown()
{
	m_turnOverMode = TURN_OVER_MODE_NULL;
}
INLINE void XSprite::setPosition(float a, float b)
{
	m_setPosition.set(a, b);
	m_needUpdateInsideData = 1;

#if IS_USE_SOLIDIFY
	m_needSolidify = 1;
#endif
}
INLINE void XSprite::setPositionX(float x)
{
	m_setPosition.x = x;
	m_needUpdateInsideData = 1;

#if IS_USE_SOLIDIFY
	m_needSolidify = 1;
#endif
}
INLINE void XSprite::setPositionY(float y)
{
	m_setPosition.y = y;
	m_needUpdateInsideData = 1;

#if IS_USE_SOLIDIFY
	m_needSolidify = 1;
#endif
}
INLINE void XSprite::setScale(const XVec2& size)
{
	setScale(size.x, size.y);
}
INLINE void XSprite::setScale(float a, float b)
{
	xsize = a;
	ysize = b;
	m_needUpdateInsideData = 1;

#if IS_USE_SOLIDIFY
	m_needSolidify = 1;
#endif
}
INLINE void XSprite::setSize(float s)
{
	xsize = s;
	ysize = s;
	m_needUpdateInsideData = 1;

#if IS_USE_SOLIDIFY
	m_needSolidify = 1;
#endif
}
INLINE void XSprite::setOverturn(char temp)
{
	if (temp == 0)
	{
		resetLeftRightUpDown();
	}
	else
	{
		setLeft2Right();
	}
}
INLINE void XSprite::disClip()
{
	setClipRect(0.0f, 0.0f, m_textureData.textureSize.x, m_textureData.textureSize.y);
	m_isEnableOutsideChip = 0;
}
INLINE void XSprite::setClipRect(const XRect& temp)
{//设置在精灵内部进行裁减
	setClipRect(temp.left, temp.top, temp.right, temp.bottom);
}
INLINE int XSprite::isInRect(float x, float y)
{
	if (m_isInited == 0) return 0;
	return getIsInRect(x, y, getBox(0), getBox(1), getBox(2), getBox(3));
}
INLINE void XSprite::drawInside()
{
	////glBegin(GL_QUADS);
	//glBegin(GL_POLYGON);
	//for(int i = 0; i < m_pointArraySize;++ i)
	//{
	//	glTexCoord2f(m_pointArray[i << 1].x,m_pointArray[i << 1].y);
	//	glVertex2f	(m_pointArray[(i << 1) + 1].x,m_pointArray[(i << 1) + 1].y);
	//}
	//glEnd();	
	//可以优化的参考代码
	glEnableClientState(GL_VERTEX_ARRAY);		//表示启用顶点数组
	glVertexPointer(2, GL_FLOAT, 0, m_vPoint);	//2表示每个顶点由两个量组成，GL_FLOAT表示每个量都是	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);//启用颜色数组
	glTexCoordPointer(2, GL_FLOAT, 0, m_uPoint);
	glDrawArrays(GL_TRIANGLE_FAN, 0, m_pointArraySize);
	//glDisableClientState(GL_VERTEX_ARRAY);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
#endif