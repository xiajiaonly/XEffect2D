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
INLINE float XSprite::getAngle() const{return m_angle;}
INLINE void XSprite::setScale(float x,float y)	//设置精灵的缩放比例
{
	m_scale.set(x,y);
	updateChildScale();
	m_needUpdateData = XTrue;
}
INLINE XVector2 XSprite::getScale() const{return m_scale;}
INLINE void XSprite::setPosition(float x,float y)			//设置精灵的坐标
{
	m_position.set(x,y);
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
INLINE XVector2 XSprite::getPosition() const{return m_position;}
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
	m_needClip = XTrue;
	m_needUpdateData = XTrue;
}
INLINE void XSprite::setClipRect(float left,float top,float right,float bottom)
{
	setClipRect(XRect(left,top,right,bottom));
}
INLINE void XSprite::disClip()
{
	if(m_textureData.isEnableInsideClip == 0)
	{
		if(!m_needClip) return;
		m_needClip = XFalse;
	}else
	{
		m_needClip = XTrue;
		m_clipRect.set(m_textureData.clipInsideRect.left / m_textureData.texture.m_w,
			m_textureData.clipInsideRect.top / m_textureData.texture.m_h,
			m_textureData.clipInsideRect.right / m_textureData.texture.m_w,
			m_textureData.clipInsideRect.bottom / m_textureData.texture.m_h);
	}
	m_needUpdateData = XTrue;
}
INLINE void XSprite::setChangeCenter(const XVector2& center)
{
	m_changeCenter = center;
	m_needUpdateData = XTrue;
}
INLINE XBool XSprite::isInRect(float x,float y)	//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return XMath::getIsInRect(x,y,getBox(0),getBox(1),getBox(2),getBox(3));
}
INLINE XVector2 XSprite::getBox(int order)
{
	if(!m_isInited) return XVector2::zero;
	updateData();
	//if(order >= 0 && order < 4) return m_vPoint[order];
	if(order >= 0 && order < 4) return m_rectPoint[order];
	else return XVector2::zero;
}
INLINE void XSprite::setAlpha(float temp)
{
	if(temp > 1.0f) temp = 1.0f;
	if(temp <0.0f) temp = 0.0f;
	m_color.fA = temp;
	updateChildAlpha();
}
INLINE float XSprite::getAlpha() const{return m_color.fA;}
INLINE void XSprite::setColor(float r,float g,float b,float a)		//小于0不会改变这一元素
{
	if(r >= 0) m_color.fR = r;
	if(g >= 0) m_color.fG = g;
	if(b >= 0) m_color.fB = b;
	if(a >= 0) m_color.fA = a;
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
INLINE XBool XSprite::getVisible() const {return m_isVisible;}	
INLINE void XSprite::setBlendType(int typeScr,int typeDst)
{
	if(typeScr < 0) typeScr = 0;
	if(typeDst < 0) typeDst = 0;
	if(typeScr >= 9 ) typeScr = 8;
	if(typeDst >= 8 ) typeDst = 7;
	m_blendTypeScr = typeScr;
	m_blendTypeDst = typeDst;
}
INLINE void XSprite::setBlendType(int rgbScr,int rgbDst,int aScr,int aDst)
{
	if(rgbScr < 0) rgbScr = 0;
	if(rgbScr >= 9 ) rgbScr = 8;
	if(aScr < 0) aScr = 0;
	if(aScr >= 9 ) aScr = 8;

	if(rgbDst < 0) rgbDst = 0;
	if(rgbDst >= 8 ) rgbDst = 7;
	if(aDst < 0) aDst = 0;
	if(aDst >= 8 ) aDst = 7;
	m_blendRGBScr = rgbScr;
	m_blendRGBDst = rgbDst;
	m_blendAScr = aScr;
	m_blendADst = aDst;
}
INLINE void XSprite::setBlendMode(XGL::XBlendType mode) {m_blendType = mode;}
//shader的接口
INLINE void XSprite::setShaderClass(XShaderGLSL * shader){m_pShader = shader;}
INLINE XTexture * XSprite::getTexture(){return &(m_textureData.texture);}
INLINE XTextureData * XSprite::getTextureData(){return &m_textureData;}
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
	glEnableClientState(GL_VERTEX_ARRAY);	
	glVertexPointer(2, GL_FLOAT, 0, m_vPointer);	//2表示每个顶点由两个量组成，GL_FLOAT表示每个量都是	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);	//启用颜色数组
	glTexCoordPointer(2, GL_FLOAT, 0, m_uPointer);
	glDrawArrays(GL_TRIANGLE_FAN, 0,m_pointSum);	//rational:MLK
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
INLINE XSprite::XSprite(const XSprite& temp)
{
	if(& temp == this) return;
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
INLINE void XSprite::setColor(float r,float g,float b,float a)
{
	if(r >= 0) colorRed = r;
	if(g >= 0) colorGreen = g;
	if(b >= 0) colorBlue = b;
	if(a >= 0) alpha = a;
#if IS_USE_SOLIDIFY
	m_needSolidify = 1;
#endif
}
INLINE void XSprite::setPosition(const XVector2& position)
{
	setPosition(position.x,position.y);
}
INLINE void XSprite::resetLeftRightUpDown()
{
	m_turnOverMode = TURN_OVER_MODE_NULL;
}
INLINE void XSprite::setPosition(float a,float b)
{
	m_setPosition.set(a,b);
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
INLINE void XSprite::setScale(const XVector2& size)
{
	setScale(size.x,size.y);
}
INLINE void XSprite::setScale(float a,float b)
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
	if(temp == 0)
	{
		resetLeftRightUpDown();
	}else
	{
		setLeft2Right();
	}
}
INLINE void XSprite::disClip()
{
	setClipRect(0.0f,0.0f,m_textureData.textureSize.x,m_textureData.textureSize.y);
	m_isEnableOutsideChip = 0;
}
INLINE void XSprite::setClipRect(const XRect &temp)
{//设置在精灵内部进行裁减
	setClipRect(temp.left,temp.top,temp.right,temp.bottom);
}
INLINE int XSprite::isInRect(float x,float y)
{
	if(m_isInited == 0) return 0;
	return getIsInRect(x,y,getBox(0),getBox(1),getBox(2),getBox(3));
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
	glDrawArrays(GL_TRIANGLE_FAN, 0,m_pointArraySize);
	//glDisableClientState(GL_VERTEX_ARRAY);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
#endif