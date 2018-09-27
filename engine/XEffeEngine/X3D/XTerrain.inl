INLINE float XTerrain::getHeight(float x,float z) const
{
	if(x < 0 || x >= m_width) return 0.0f;
	if(z < 0 || z >= m_height) return 0.0f;
	return m_terrainData[(int)(z * m_width + x)];
}
INLINE XVec2 XTerrain::getTexturePosition(float x,float z) const
{
	return XVec2(XMath::clamp<float>(x, 0.0f, m_width) / m_width, XMath::clamp<float>(z, 0.0f, m_height) / m_height);
}
INLINE void XTerrain::updateTexture(const void * data)
{
	if(data == NULL ||
		!m_isInited ||
		!m_withTexture) return;
	m_texture.texture.updateTexture(data);
}
INLINE void XTerrain::setTexture(unsigned int tex)	//这个函数用于测试
{
	if(!m_isInited ||
		!m_withTexture) return;
	m_texture.texture.m_texture = tex;
}
INLINE void XTerrain::setTexture(const XTextureData &tex)
{
	if(!m_isInited ||
		!m_withTexture) return;
	m_texture.setACopy(tex);
}
INLINE void XTerrain::release()
{
	if(!m_isInited) return;
	XMem::XDELETE_ARRAY(m_terrainData);
	XMem::XDELETE_ARRAY(m_tempFitTerrain);
	m_vbo.release();
	releaseMemory();
	m_isInited = XFalse;
}
INLINE void XTerrain::setReflectRate(float rate)
{
	if(rate < 0.0f) rate = 0.0f;
	if(rate > 1.0f) rate = 1.0f;
	m_reflectRate = rate;
}
INLINE XVec3 XTerrain::myNormalize(float x,float z) 
{//经过特定优化的归一化函数
	float d = 1.0f / sqrt(x * x + 1.0f + z * z);
	return XVec3(x * d,d,z * d);
}
INLINE XVec3 XTerrain::getMyNormal(int x,bool flag)	
{//经过特定优化的求法线函数
	//XVec3 w,v;
	if(flag) return myNormalize(m_newH[x] - m_newH[x + 1],m_newH[x] - m_newH[x + m_width]);	//上半个面
	else return myNormalize(m_newH[x + m_width] - m_newH[x + 1 + m_width],m_newH[x + 1] - m_newH[x + 1 + m_width]);	//下半个面
}