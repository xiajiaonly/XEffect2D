INLINE XBool XField2D::Init(int w, int h)
{
	if (m_isInited || w <= 0 || h <= 0) return XFalse;
	m_w = w;
	m_h = h;
	m_sum = w * h;
	m_pVectors = XMem::createArrayMem<XVec2>(w * h);
	if (m_pVectors == NULL) return XFalse;
	m_type = FIELD2D_NULL;
	m_isInited = XTrue;
	return XTrue;
}
INLINE void XField2D::Release()
{
	if (!m_isInited) return;
	XMem::XDELETE_ARRAY(m_pVectors);
	m_isInited = XFalse;
}
INLINE const XVec2& XField2D::GetVector(int x, int y) const
{
	if (!m_isInited || x < 0 || y < 0 || x >= m_w || y >= m_h) return XVec2::zero;
	return m_pVectors[x + m_w * y];
}
INLINE const XVec2& XField2D::GetVectorFast(int index) const
{
	//if (!m_isInited || index < 0 || index >= m_sum) return XVec2::zero;
	if (index < 0 || index >= m_sum) return XVec2::zero;
	return m_pVectors[index];
}
INLINE void XField2D::SetVector(int x, int y, const XVec2& v)
{
	if (!m_isInited || x < 0 || y < 0 || x >= m_w || y >= m_h) return;
	m_pVectors[x + m_w * y] = v;
}
INLINE void XField2D::SetVectorFast(int x, int y, const XVec2& v)
{
	m_pVectors[x + m_w * y] = v;
}
INLINE void XField2D::BlendVector(int index, const XVec2& v, float rate)
{
	m_pVectors[index] = m_pVectors[index] * (1.0f - rate) + v * rate;
}
INLINE void XField2D::Reset()
{
	if (m_sum <= 0) return;
	memset(m_pVectors, 0, m_sum * sizeof(XVec2));
}
