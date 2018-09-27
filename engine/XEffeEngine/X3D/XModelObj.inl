INLINE XVec2 XModelObj::getTexture(unsigned int index)const
{
	if (index >= m_texturePoint.size()) return XVec2::zero;
	return m_texturePoint[index];
}
INLINE XVec3 XModelObj::getVector(unsigned int index) const
{
	if (index >= m_point.size()) return XVec3::zero;
	return m_point[index];
}
INLINE XVec3 XModelObj::getNormal(unsigned int index)const
{
	if (index >= m_normal.size()) return XVec3::zero;
	return m_normal[index];
}
INLINE int XModelObj::getFaceSum() const
{
	if (m_group.size() <= 0) return 0;
	return m_group[0]->face.size();
}
INLINE XVectorIndex3 XModelObj::getFaceVectorIndex(unsigned int index) const
{
	if (m_group.size() <= 0 || index >= m_group[0]->face.size()) return XVectorIndex3::zero;
	return m_group[0]->face[index].vIndex;
}
INLINE XVectorIndex3 XModelObj::getFaceTextureIndex(unsigned int index) const
{
	if (m_group.size() <= 0 || index >= m_group[0]->face.size()) return XVectorIndex3::zero;
	return m_group[0]->face[index].tIndex;
}
INLINE XVectorIndex3 XModelObj::getFaceNormalIndex(unsigned int index) const
{
	if (m_group.size() <= 0 || index >= m_group[0]->face.size()) return XVectorIndex3::zero;
	return m_group[0]->face[index].nIndex;
}
INLINE void XModelObj::useMatrix(const XBasic3DObject *base)	//Ê¹ÓÃ¾ØÕó
{
	updateMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	//	glLoadIdentity();

	if (base != NULL) glMultMatrixf(base->getMatrix());
	glMultMatrixf(m_matrix);
}
INLINE int XModelObj::getOriFaceSum()const
{
	if (m_group.size() <= 0) return 0;
	return m_group[0]->originalFaces.size();
}
INLINE std::vector<int> XModelObj::getOriFaceVIndex(unsigned int index) const
{
	if (m_group.size() <= 0 || index >= m_group[0]->originalFaces.size()) return std::vector<int>();
	return m_group[0]->originalFaces[index].vsIndex;
}
INLINE std::vector<int> XModelObj::getOriFaceTIndex(unsigned int index) const
{
	if (m_group.size() <= 0 || index >= m_group[0]->originalFaces.size()) return std::vector<int>();
	return m_group[0]->originalFaces[index].tsIndex;
}
INLINE std::vector<int> XModelObj::getOriFaceNIndex(unsigned int index) const
{
	if (m_group.size() <= 0 || index >= m_group[0]->originalFaces.size()) return std::vector<int>();
	return m_group[0]->originalFaces[index].nsIndex;
}