INLINE unsigned int XFBO::getTexture(unsigned int order) const	//存在问题
{
	if (order < m_pTextures.size())
		return m_pTextures[order]->m_texture;
	return 0;
}
INLINE XTexture *XFBO::getTexturePtr(unsigned int order)
{
	if (order < m_pTextures.size())
		return m_pTextures[order];
	return 0;
}
INLINE int XFBO::getWidth(unsigned int order) const
{
	if (order < m_pTextures.size()) return m_pTextures[order]->m_w;
	return -1;
}
INLINE int XFBO::getHeight(unsigned int order) const
{
	if (order < m_pTextures.size()) return m_pTextures[order]->m_h;
	return -1;
}
INLINE XColorMode XFBO::getColorMode(unsigned int order) const
{
	if (order < m_pTextures.size()) return m_pTextures[order]->m_mode;
	return COLOR_RGBA;
}
INLINE int XFBO::getW() const
{
	if (m_zeroTexIndex < 0 || m_pTextures.size() == 0
		|| m_zeroTexIndex >= m_pTextures.size()) return 0;
	return m_pTextures[m_zeroTexIndex]->m_w;
}
INLINE int XFBO::getH() const
{
	if (m_zeroTexIndex < 0 || m_pTextures.size() == 0
		|| m_zeroTexIndex >= m_pTextures.size()) return 0;
	return m_pTextures[m_zeroTexIndex]->m_h;
}
INLINE XBool XShaderGLSL::init(const char* vertFile, const char* fragFile,
	XResPos resPos, const char* geomFile)
{
	if (m_isInited) return XTrue;
	if (!XGL::setShader(vertFile, fragFile, geomFile, m_shaderHandle, resPos)) return XFalse;
	m_dataInfos.clear();
	m_texInfos.clear();
	m_UBOInfos.clear();
	m_isInited = XTrue;
	return XTrue;
}
INLINE XBool XShaderGLSL::initFromStr(const char *vStr, const char *fStr, const char * gStr)
{
	if (m_isInited) return XTrue;
	if (!XGL::setShaderFromStr(vStr, fStr, gStr, m_shaderHandle)) return XFalse;
	m_dataInfos.clear();
	m_texInfos.clear();
	m_UBOInfos.clear();
	m_isInited = XTrue;
	return XTrue;
}
INLINE void XShaderGLSL::setGeometryInfo(unsigned int inType, unsigned int outType, int outSum)
{
	if (m_shaderHandle.shaderHandle == 0) m_shaderHandle.shaderHandle = glCreateProgram();
	glProgramParameteriEXT(m_shaderHandle.shaderHandle, GL_GEOMETRY_INPUT_TYPE_EXT, inType);
	glProgramParameteriEXT(m_shaderHandle.shaderHandle, GL_GEOMETRY_OUTPUT_TYPE_EXT, outType);
	glProgramParameteriEXT(m_shaderHandle.shaderHandle, GL_GEOMETRY_VERTICES_OUT_EXT, outSum);
}
INLINE int XShaderGLSL::getGeometryMaxOutputCount()const
{
	int temp = 0;
	glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT, &temp);
	return temp;
}
INLINE XBool XShaderGLSL::connectUBO(const char *uboName, XUBO *ubo)
{
	if (uboName == NULL || ubo == NULL) return XFalse;
	if (m_UBOInfos.size() >= m_maxShaderGLSLDataSum - 1) return XFalse;
	unsigned int handle = glGetUniformBlockIndex(m_shaderHandle.shaderHandle, uboName);
	int index = -1;
	for (unsigned int i = 0; i < m_UBOInfos.size(); ++i)
	{
		if (m_UBOInfos[i].m_handle != handle) continue;
		index = i;
		break;
	}
	if (index < 0)
	{
		XShaderUBOInfo tmp;
		tmp.m_handle = glGetUniformBlockIndex(m_shaderHandle.shaderHandle, uboName);
		tmp.m_pUBO = ubo;
		m_UBOInfos.push_back(tmp);
	}
	else
	{
		m_UBOInfos[index].m_pUBO = ubo;
	}
	return XTrue;
}
INLINE void XUBO::release()
{
	if (!m_isInited) return;
	glDeleteBuffersARB(1, &m_uboHandle);
	m_isInited = XFalse;
}
INLINE void XUBO::useUBO(unsigned int index)	//index为对应的shader中结构的索引
{
	if (!m_isInited) return;
	glBindBufferBase(GL_UNIFORM_BUFFER, index, m_uboHandle);
}
INLINE void XSSBO::release()
{
	if (!m_isInited) return;
	glDeleteBuffersARB(1, &m_ssboHandle);
	m_isInited = XFalse;
}
INLINE void XSSBO::useSSBO(unsigned int index)	//index为对应的shader中结构的索引
{
	if (!m_isInited) return;
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, m_ssboHandle);
}
INLINE void XSSBO::unuseSSBO()	//index为对应的shader中结构的索引
{
	if (!m_isInited) return;
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}
INLINE XBool XGLBuffer::init(int size, const float * p)
{
	if (m_isInited || p == NULL) return XFalse;
	glGenBuffersARB(1, &m_buffHandle);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_buffHandle);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, size * sizeof(float), p, GL_STATIC_DRAW_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);	//取消绑定
	m_isInited = XTrue;
	return XTrue;
}
INLINE void XGLBuffer::useBuff(unsigned int index, int coreSize)	//index为shader中数据的索引参看例子
{
	if (!m_isInited) return;
	m_useIndex = index;
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_buffHandle);
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, coreSize, GL_FLOAT, false, 0, 0);
}
INLINE void XGLBuffer::removeBuff()
{
	if (!m_isInited || m_useIndex < 0) return;
	glDisableVertexAttribArray(m_useIndex);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);	//取消绑定
	m_useIndex = -1;
}
INLINE void XGLBuffer::release()
{
	if (!m_isInited) return;
	glDeleteBuffersARB(1, &m_buffHandle);
	m_isInited = XFalse;
}
INLINE void XPBO::release()
{
	if (!m_isInited) return;
	glDeleteBuffersARB(2, m_pboID);
	m_isInited = XFalse;
}
INLINE void XPBO::bind() const
{
	if (!m_isInited) return;
	if (m_mode == PBO_MODE_PACK) glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, m_pboID[m_curPBOIndex]);
	else glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, m_pboID[m_curPBOIndex]);
}
INLINE void XPBO::unbind() const
{
	if (!m_isInited) return;
	if (m_mode == PBO_MODE_PACK) glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, 0);
	else glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
}
INLINE XBool XVBO::updateDataV(int size, const float *v)
{
	if (!m_isInited || !m_withV || v == NULL) return XFalse;
	if (m_size < size)
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_v);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, size * m_vSize * sizeof(float), v, GL_DYNAMIC_DRAW_ARB);//GL_STATIC_DRAW_ARB);
	}
	else
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_v);
		if (size > 2048)	//这一步优化好像是得其反
		{
			memcpy(glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY_ARB), v, size * m_vSize * sizeof(float));
			glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
		}
		else
			glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, size * m_vSize * sizeof(float), v);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);	//取消绑定
	}
	return XTrue;
}
INLINE XBool XVBO::updateDataT(int size, const float *t)
{
	if (!m_isInited || !m_withT || t == NULL) return XFalse;
	if (m_size < size)
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_t);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, (size << 1) * sizeof(float), t, GL_DYNAMIC_DRAW_ARB);
	}
	else
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_t);
		if (size > 2048)	//这一步优化好像是得其反
		{
			memcpy(glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY_ARB), t, (size << 1) * sizeof(float));
			glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
		}
		else
			glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, (size << 1) * sizeof(float), t);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);	//取消绑定
	}
	return XTrue;
}
INLINE XBool XVBO::updateDataN(int size, const float *n)
{
	if (!m_isInited || !m_withN || n == NULL) return XFalse;
	if (m_size < size)
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_n);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, size * 3 * sizeof(float), n, GL_DYNAMIC_DRAW_ARB);
	}
	else
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_n);
		//	if(size > 2048)	//这一步优化好像是得其反
		//	{
		//		memcpy(glMapBufferARB(GL_ARRAY_BUFFER_ARB,GL_WRITE_ONLY_ARB),n,size * 3 * sizeof(float));
		//		glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
		//	}else
		glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, size * 3 * sizeof(float), n);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);	//取消绑定
	}
	return XTrue;
}
INLINE XBool XVBO::updateDataC(int size, const float *c)
{
	if (!m_isInited || !m_withC || c == NULL) return XFalse;
	if (m_size < size)
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_c);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, (size << 2) * sizeof(float), c, GL_DYNAMIC_DRAW_ARB);
	}
	else
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_c);
		if (size > 2048)	//这一步优化好像是得其反
		{
			memcpy(glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY_ARB), c, (size << 2) * sizeof(float));
			glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
		}
		else
			glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, (size << 2) * sizeof(float), c);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);	//取消绑定
	}
	return XTrue;
}
INLINE XBool XVBO::updateDataI(int size, const unsigned int *i)
{
	if (!m_isInited) return XFalse;
	//if(size != m_indexSize || !m_withI || i == NULL) return false;
	if (!m_withI || i == NULL) return XFalse;
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_i);
	if (m_indexSize < size)
	{
		m_curISize = m_indexSize = size;
		glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, size * sizeof(int), i, GL_STATIC_DRAW_ARB);
	}
	else
	{
		m_curISize = size;
		if (size > 8192)	//这一步优化好像是得其反
		{
			memcpy(glMapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, GL_WRITE_ONLY_ARB), i, size * sizeof(int));
			glUnmapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB);
		}
		else
			glBufferSubDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0, size * sizeof(int), i);
	}

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);	//取消绑定
	return XTrue;
}
INLINE void XVBO::disuse()
{
	if (!m_isInited) return;
	if (m_withVAO)
	{
		glBindVertexArray(0);
		return;
	}
	if (m_withC) glDisableClientState(GL_COLOR_ARRAY);
	if (m_withT) glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	if (m_withN) glDisableClientState(GL_NORMAL_ARRAY);
	if (m_withV) glDisableClientState(GL_VERTEX_ARRAY);
	if (m_withI) glDisableClientState(GL_INDEX_ARRAY);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);	//取消绑定
}
INLINE void XVBO::drawByIndex(unsigned int type, XBool withTex)
{
	if (!m_withI) return;
	use(withTex);
	glDrawElements(type, m_curISize, GL_UNSIGNED_INT, 0);
	disuse();
}
INLINE void XVBO::drawByIndex(unsigned int type, int size, unsigned int indexType, void *pIndex, XBool withTex)
{
	if (m_curISize < size) return;	//这个判断不严谨，并不能阻止越界，检查很宽泛
	use(withTex);
	glDrawElements(type, size, indexType, pIndex);
	disuse();
}
INLINE void XVBO::drawByArray(unsigned int type, XBool withTex)
{
	use(withTex);
	glDrawArrays(type, 0, m_size);
	disuse();
}
INLINE void XVBO::drawByArray(unsigned int type, int index, int size, XBool withTex)
{
	use(withTex);
	glDrawArrays(type, index, size);
	disuse();
}
INLINE void XFBO::bind()
{
	reset();
	_bind();
	opertion(OPER_BIND);
}
INLINE void XFBO::unbind()
{
	_unbind();
	opertion(OPER_UNBIND);
}
INLINE XBool XFBO::_attachTex(unsigned int order)
{
	if (m_pTextures.size() > 1 && order < m_pTextures.size())
	{//挂接贴图
		if (m_MSSum == 0)
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_pTextures[order]->m_texture, 0);//挂接贴图		
		else
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D_MULTISAMPLE, m_pTextures[order]->m_texture, 0);
		m_zeroTexIndex = order;
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
		return true;
	}
	return false;
}
INLINE void XFBO::addOneTexture(int w, int h, XColorMode type, int filter)
{
	if (type != COLOR_RGB && type != COLOR_RGBA &&
		type != COLOR_RGB_F && type != COLOR_RGBA_F)
		LogStr("在部分集成显卡中可能不支持该功能,请确定这一部分.");
	XTexture *tmp = XMem::createMem<XTexture>();
	if (tmp == NULL) return;	//内存分配失败
	if (!tmp->createTexture(w, h, type, -1, filter, m_MSSum))
	{
		XMem::XDELETE(tmp);
		return;
	}
	m_pTextures.push_back(tmp);
}
INLINE void XFBO::updateTexture(void *p, unsigned int index)
{
	if (p == NULL || index >= m_pTextures.size()) return;
	m_pTextures[index]->updateTexture(p);
}
INLINE void XFBO::getPixelFromFboEx(XFBOEx& ex, int dstIndex)
{
	ex.useFBO();
	useFBO();
	attachTex(dstIndex);
	glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, ex.getFboID());
	glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, m_fboId);
	glBlitFramebufferEXT(0, 0, getW(), getH(), 0, 0, ex.getW(), ex.getH(), GL_COLOR_BUFFER_BIT, GL_LINEAR);
	glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, 0);
	glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, 0);
	glReadBuffer(GL_BACK);
	removeFBO();
	ex.removeFBO();
}
INLINE void XFBO::getPixelFromFbo(XFBO& srcFbo, int srcIndex, int dstIndex)
{//这里需要备份现场和还原现场
	useFBO();
	attachTex(dstIndex);
	srcFbo.useFBO();
	srcFbo.attachTex(srcIndex);
	glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, srcFbo.getFboID());
	glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, m_fboId);
	//	GLint samplesSum = 0;
	//	glGetIntegerv(GL_DRAW_FRAMEBUFFER_EXT, &samplesSum);
	//	//glGetFramebufferParameterivEXT(
	//	glGetFramebufferAttachmentParameterivEXT(GL_READ_FRAMEBUFFER_EXT,,,&samplesSum);

	glBlitFramebufferEXT(0, 0, getW(), getH(), 0, 0, srcFbo.getW(), srcFbo.getH(), GL_COLOR_BUFFER_BIT, GL_LINEAR);
	glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, 0);
	glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, 0);	//如果有FBO嵌套的话，这里需要回复
	glReadBuffer(GL_BACK);
	srcFbo.removeFBO();
	removeFBO();
}
INLINE void XFBOEx::release()
{
	if (!m_isInited) return;
	glDeleteRenderbuffersEXT(1, &m_fboId);
	glDeleteRenderbuffersEXT(1, &m_rboId);
	glDeleteFramebuffersEXT(1, &m_rbdId);
	m_isInited = false;
}