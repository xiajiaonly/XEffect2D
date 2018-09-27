INLINE void XTailingD::Draw()	//描绘函数
{
	if(m_isEnd) return;
	DrawBegin();
	DrawBody();
	DrawEnd();
}
INLINE void XTailingDLine::Draw()	//描绘函数
{
	if(m_isEnd) return;
	DrawBegin();
	DrawBody();
	DrawEnd();
}
INLINE void XTailingObject::Update(const float &stepTime,const XRect& rect)
{//物体在运动
	if(!m_isStop) 
	{
		m_position += m_speed * stepTime;
		m_isStop = !rect.isInRect(m_position);
		if(m_tailing != NULL)
			m_tailing->AddPoint(m_position);
	}
	if(m_tailing != NULL)
	{
		if(!m_tailing->GetIsEnd())
			m_tailing->Update(stepTime);
	}
}
INLINE void XTailingObject::Reset(const XVec2& pos,const XVec2& speed,XTailingD *t)
{
	m_isStop = false;
	m_position = pos;
	m_speed = speed;
	m_tailing = t;
}
INLINE void XTailingDLine::DrawEnd()
{
	XGL::DisableBlend();
}
INLINE void XTailingDLine::DrawFE()
{
	glEnd();
	XGL::DisableBlend();
}
INLINE void XTailingDLine::DrawBegin()
{
	XGL::DisableTexture2D();
	XGL::setBlendAlpha();
	glLineWidth(2.0);
}
INLINE void XTailingDLine::DrawFB()
{
	XGL::DisableTexture2D();
	XGL::setBlendAlpha();
	glLineWidth(2.0);
	glBegin(GL_LINES);
}
INLINE void XTailingD::DrawEnd()
{
	glEnd();
	XGL::DisableBlend();
}
INLINE void XTailingDPoint::DrawBegin()
{
	XGL::DisableTexture2D();
	XGL::setBlendAlpha();
	glPointSize(10);
	glBegin(GL_POINTS);
}