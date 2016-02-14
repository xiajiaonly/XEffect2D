#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XShaderGLSL.h"
#include "XMath/XVector2.h"
#include "XMath/XVector3.h"

namespace XE{
void XShaderDataInfo::updateData()	//更新数据
{
	if(m_p == NULL) return;
	switch(m_type)
	{
	case DATA_TYPE_INT:
		glUniform1i(m_handle,* (int *)(m_p));
		break;
	case DATA_TYPE_FLOAT:
		glUniform1f(m_handle,* (float *)(m_p));
		break;
	case DATA_TYPE_INT_ARRAY:
		glUniform1iv(m_handle,m_size,(int *)(m_p));
		break;
	case DATA_TYPE_FLOAT_ARRAY:
		glUniform1fv(m_handle,m_size,(float *)(m_p));
		break;
	case DATA_TYPE_2INT_ARRAY:
		glUniform2iv(m_handle,m_size,(int *)(m_p));
		break;
	case DATA_TYPE_2FLOAT_ARRAY:
		glUniform2fv(m_handle,m_size,(float *)(m_p));
		break;
	case DATA_TYPE_3INT_ARRAY:
		glUniform3iv(m_handle,m_size,(int *)(m_p));
		break;
	case DATA_TYPE_3FLOAT_ARRAY:
		glUniform3fv(m_handle,m_size,(float *)(m_p));
		break;
	case DATA_TYPE_3FLOAT_MATRIX:
		glUniformMatrix3fv(m_handle,m_size,GL_FALSE,(float *)(m_p));
		break;
	case DATA_TYPE_4FLOAT_MATRIX:
		glUniformMatrix4fv(m_handle,m_size,GL_FALSE,(float *)(m_p));
		break;
	case DATA_TYPE_2INT:
		glUniform2i(m_handle,((XVector2 *)m_p)->x,((XVector2 *)m_p)->y);
		break;
	case DATA_TYPE_2FLOAT:
		glUniform2f(m_handle,((XVector2 *)m_p)->x,((XVector2 *)m_p)->y);
		break;
	case DATA_TYPE_3INT:
		glUniform3i(m_handle,((XVector3 *)m_p)->x,((XVector3 *)m_p)->y,((XVector3 *)m_p)->z);
		break;
	case DATA_TYPE_3FLOAT:
		glUniform3f(m_handle,((XVector3 *)m_p)->x,((XVector3 *)m_p)->y,((XVector3 *)m_p)->z);
		break;
	case DATA_TYPE_4FLOAT:
		glUniform4f(m_handle,((float *)m_p)[0],((float *)m_p)[1],((float *)m_p)[2],((float *)m_p)[3]);
		break;
	}
}
//链接数据
XBool XShaderGLSL::connectData(const char *name,XShaderDataType type,int size,void *p)
{
	if(size < 0 ||
		p == NULL ||
		name == NULL ||
		!m_isInited ||
		m_dataInfos.size() >= m_maxShaderGLSLDataSum - 1) return XFalse;
	XShaderDataInfo tmp;
	tmp.m_handle = glGetUniformLocation(m_shaderHandle.shaderHandle,name);
	tmp.m_type = type;
	tmp.m_p = p;
	tmp.m_size = size;
	m_dataInfos.push_back(tmp);
	return XTrue;
}
//链接贴图
XBool XShaderGLSL::connectTexture(const char *name,unsigned int * tex,XShaderTexType type)
{
	if(name == NULL ||
		(m_texInfos.size() != 0 && tex == NULL) ||
		m_texInfos.size() >= 32 - 1) return XFalse;

	XSaderTexInfo tmp;
	tmp.texHandle = glGetUniformLocation(m_shaderHandle.shaderHandle,name);
	tmp.texID = tex;
	tmp.texType = type;
	m_texInfos.push_back(tmp);
	return XTrue;
}
XBool XShaderGLSL::getUBOInfo(XShaderUBOData &uboData,int valueSum,const char *uboName,const char **valueNames)	//连接UBO
{
	if(valueSum <= 0 || valueSum >= m_maxShaderGLSLDataSum) return XFalse;
	uboData.m_handle = glGetUniformBlockIndex(m_shaderHandle.shaderHandle,uboName);  //获取结构的编号 
	if(GL_INVALID_INDEX == uboData.m_handle) return XFalse;
	glGetActiveUniformBlockiv(m_shaderHandle.shaderHandle,  
		                        uboData.m_handle,  
		                        GL_UNIFORM_BLOCK_DATA_SIZE,  
								&uboData.m_size);					//获取结构的尺寸
	glGetUniformIndices(m_shaderHandle.shaderHandle,valueSum,valueNames,uboData.m_index);  //获得每个变量的标签
	glGetActiveUniformsiv(m_shaderHandle.shaderHandle,valueSum,uboData.m_index,GL_UNIFORM_OFFSET,uboData.m_offset);	//获取每个变量的偏移
	return XTrue;
}
void XShaderGLSL::updateData()
{
	if(!m_isInited) return;
	//下面更新数据
	for(unsigned int i = 0;i < m_dataInfos.size();++ i)
	{
		m_dataInfos[i].updateData();
	}
	for(unsigned int i = 0;i < m_UBOInfos.size();++ i)
	{
		m_UBOInfos[i].updateData();
	}
}
void XShaderGLSL::useShader(bool withTex0)	//使用shader
{
	if(!m_isInited) return;
	glUseProgram(m_shaderHandle.shaderHandle);
	//glUseProgramObjectARB(m_shaderHandle);
	updateData();
	unsigned int i = 1;
	if(withTex0) i = 0;
	else
	if(m_texInfos.size() > 0)
	{
		i = 1;
		glUniform1i(m_texInfos[0].texHandle,0);
	}
	for(;i < m_texInfos.size();++ i)
	{
		glUniform1i(m_texInfos[i].texHandle,i);
		glActiveTexture(GL_TEXTURE0 + i);
		switch(m_texInfos[i].texType)
		{
		case TEX_TYPE_2D:
			XGL::EnableTexture2D();
			XGL::BindTexture2D(*(m_texInfos[i].texID),i);
			break;
		case TEX_TYPE_CUBE://GL_TEXTURE_CUBE_MAP贴图
			glEnable(GL_TEXTURE_CUBE_MAP);
			glBindTexture(GL_TEXTURE_CUBE_MAP,*(m_texInfos[i].texID));
			break;
		}
	}
}
void XShaderGLSL::useShaderEx(unsigned int tex0,XShaderTexType type)
{
	if(!m_isInited) return;
	glUseProgram(m_shaderHandle.shaderHandle);
	//glUseProgramObjectARB(m_shaderHandle);
	updateData();
	//绑定第一张贴图
	if(m_texInfos.size() > 0)
	{
		glUniform1i(m_texInfos[0].texHandle,0);
	}
	glActiveTexture(GL_TEXTURE0);
	switch(type)
	{
	case TEX_TYPE_2D:
		XGL::EnableTexture2D();
		XGL::BindTexture2D(tex0);
		break;
	case TEX_TYPE_CUBE://GL_TEXTURE_CUBE_MAP贴图
		glEnable(GL_TEXTURE_CUBE_MAP);
		glBindTexture(GL_TEXTURE_CUBE_MAP,tex0);
		break;
	}
	//绑定其他的贴图
	for(unsigned int i = 1;i < m_texInfos.size();++ i)
	{
		glUniform1i(m_texInfos[i].texHandle,i);
		glActiveTexture(GL_TEXTURE0 + i);
		switch(m_texInfos[i].texType)
		{
		case TEX_TYPE_2D:
			XGL::EnableTexture2D();
			XGL::BindTexture2D(*(m_texInfos[i].texID),i);
			break;
		case TEX_TYPE_CUBE://GL_TEXTURE_CUBE_MAP贴图
			glEnable(GL_TEXTURE_CUBE_MAP);
			glBindTexture(GL_TEXTURE_CUBE_MAP,*(m_texInfos[i].texID));
			break;
		}
	}
}
void XShaderGLSL::disShader()
{
	glUseProgram(0);
	//对贴图进行反操作
	if(m_texInfos.size() <= 1) return;
	for(unsigned int i = 1;i < m_texInfos.size();++ i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		switch(m_texInfos[i].texType)
		{
		case TEX_TYPE_2D:XGL::BindTexture2D(0,i);break;
		case TEX_TYPE_CUBE:glBindTexture(GL_TEXTURE_CUBE_MAP,0);break;
		}
	}
	glActiveTexture(GL_TEXTURE0);
}
void XShaderGLSL::release()
{
	if(m_shaderHandle.shaderHandle == 0) return;
	if(m_shaderHandle.shaderV != 0)
	{
		glDetachShader(m_shaderHandle.shaderHandle,m_shaderHandle.shaderV);
		glDeleteShader(m_shaderHandle.shaderV);
		m_shaderHandle.shaderV = 0;
	}
	if(m_shaderHandle.shaderF != 0)
	{
		glDetachShader(m_shaderHandle.shaderHandle,m_shaderHandle.shaderF);
		glDeleteShader(m_shaderHandle.shaderF);
		m_shaderHandle.shaderF = 0;
	}
	if(m_shaderHandle.shaderG != 0)
	{
		glDetachShader(m_shaderHandle.shaderHandle,m_shaderHandle.shaderG);
		glDeleteShader(m_shaderHandle.shaderG);
		m_shaderHandle.shaderG = 0;
	}
	glDeleteProgram(m_shaderHandle.shaderHandle);
	m_shaderHandle.shaderHandle = 0;
}
XBool XUBO::init(int size,const void * p)
{
	if(m_isInited || p == NULL) return XFalse;
	int maxS = 0;
	glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE,&maxS);
	if(size > maxS) return XFalse;
	glGenBuffersARB(1,&m_uboHandle);  
	glBindBufferARB(GL_UNIFORM_BUFFER,m_uboHandle);  
	glBufferDataARB(GL_UNIFORM_BUFFER,size,p,GL_DYNAMIC_DRAW);

	m_isInited = XTrue;
	return XTrue;
}
XBool XPBO::init(int w,int h,int px,int py,int wx,int wy,XColorMode colorType,XPboMode mode)
{
	if(m_isInited) return XTrue;
	m_curPBOIndex = 0;
	m_mode = mode;
	m_w = w;
	m_h = h;
	m_px = px;
	m_py = py;
	m_wx = wx;
	m_wy = wy;
	m_colorType = colorType;
	switch(m_colorType)
	{
	case COLOR_RGBA:
	case COLOR_BGRA:m_dataSize = m_w * (m_h << 2);break;
	case COLOR_RGB:
	case COLOR_BGR:	m_dataSize = m_w * m_h * 3;break;
	case COLOR_RED:
	case COLOR_GRAY:m_dataSize = m_w * m_h;break;
	case COLOR_RGBA_F:m_dataSize = m_w * (m_h << 2) * sizeof(float);break;
	case COLOR_GRAY_F:m_dataSize = m_w * m_h * sizeof(float);break;
	default:	//其他格式不支持该操作
		return XFalse;
	}
	glGenBuffersARB(2,m_pboID);
	if(m_mode == PBO_MODE_PACK)
	{
		glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB,m_pboID[0]);
		glBufferDataARB(GL_PIXEL_PACK_BUFFER_ARB,m_dataSize,0,GL_STREAM_READ_ARB);	//这里可以绑定一块数据，如果不给数据的话，opengl会自己分配内存空间
		glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB,m_pboID[1]);
		glBufferDataARB(GL_PIXEL_PACK_BUFFER_ARB,m_dataSize,0,GL_STREAM_READ_ARB);
		glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB,0);
	}else
	{
		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB,m_pboID[0]);
		glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB,m_dataSize,0,GL_STREAM_DRAW_ARB);	//这里可以绑定一块数据，如果不给数据的话，opengl会自己分配内存空间
		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB,m_pboID[1]);
		glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB,m_dataSize,0,GL_STREAM_DRAW_ARB);
		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB,0);
	}

	m_isInited = XTrue;
	return XTrue;
}
XBool XPBO::getPixel(unsigned char * buff,int target)
{
	if(!m_isInited ||
		buff == NULL ||
		m_mode == PBO_MODE_UNPACK) return XFalse;
	int nextIndex = m_curPBOIndex;
	m_curPBOIndex = (m_curPBOIndex + 1) % 2;

	glReadBuffer(target);
	glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB,m_pboID[m_curPBOIndex]);
	switch(m_colorType)
	{
	case COLOR_RGBA:glReadPixels(m_px,m_py,m_wx,m_wy,GL_RGBA,GL_UNSIGNED_BYTE,0);break;
	case COLOR_BGRA:glReadPixels(m_px,m_py,m_wx,m_wy,GL_BGRA,GL_UNSIGNED_BYTE,0);break;
	case COLOR_RGB:	glReadPixels(m_px,m_py,m_wx,m_wy,GL_RGB,GL_UNSIGNED_BYTE,0);break;
	case COLOR_BGR:	glReadPixels(m_px,m_py,m_wx,m_wy,GL_BGR,GL_UNSIGNED_BYTE,0);break;
	case COLOR_RED:	glReadPixels(m_px,m_py,m_wx,m_wy,GL_RED,GL_UNSIGNED_BYTE,0);break;
	case COLOR_GRAY:glReadPixels(m_px,m_py,m_wx,m_wy,GL_LUMINANCE,GL_UNSIGNED_BYTE,0);break;
	case COLOR_RGBA_F:glReadPixels(m_px,m_py,m_wx,m_wy,GL_RGBA,GL_FLOAT,0);break;
	case COLOR_GRAY_F:glReadPixels(m_px,m_py,m_wx,m_wy,GL_RED,GL_FLOAT,0);break;
	default:	//其他格式不支持该操作
		glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB,0);
		glDrawBuffer(GL_BACK);
		return XFalse;
	}
	glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB,m_pboID[nextIndex]);
	unsigned char * src = (unsigned char *)glMapBufferARB(GL_PIXEL_PACK_BUFFER_ARB,GL_READ_ONLY_ARB);
	if(src != NULL)
	{//已经获取数据
		memcpy(buff,src,m_dataSize);
		glUnmapBufferARB(GL_PIXEL_PACK_BUFFER_ARB);
		glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB,0);
		glDrawBuffer(GL_BACK);
		return XTrue;
	}else
	{
		glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB,0);
		glDrawBuffer(GL_BACK);
		return XFalse;
	}
}
XBool XPBO::setPixel(unsigned char * buff)
{
	if(!m_isInited ||
		buff == NULL ||
		m_mode == PBO_MODE_PACK) return XFalse;
	//int nextIndex = m_curPBOIndex;
	m_curPBOIndex = (m_curPBOIndex + 1) & 0x1;

	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB,m_pboID[m_curPBOIndex]);
	glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB,m_dataSize, 0, GL_STREAM_DRAW_ARB);
    GLubyte * src = (GLubyte *)glMapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, GL_WRITE_ONLY_ARB);
    if(src != NULL)
    {
		memcpy(src,buff,m_dataSize);
        glUnmapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB); // release pointer to mapping buffer
		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB,0);
		return XTrue;
    }else
	{
		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB,0);
		return XFalse;
	}
}
XBool XVBO::initX(int size,const float *v,const float *t,const float *n,const float *c,
				   int iSize,const unsigned int *i,bool withVAO)
{
	if(m_isInited ||
		size <= 0) return XFalse;
	if(v == NULL) m_withV = XFalse; else m_withV = XTrue;
	if(t == NULL) m_withT = XFalse; else m_withT = XTrue;
	if(n == NULL) m_withN = XFalse; else m_withN = XTrue;
	if(c == NULL) m_withC = XFalse; else m_withC = XTrue;
	if(i == NULL) m_withI = XFalse; else m_withI = XTrue;
	m_withVAO = withVAO;
	m_size = size;
	m_indexSize = iSize;
	if(m_withV)
	{
		glGenBuffersARB(1,&m_v);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_v);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB,size * m_vSize * sizeof(float),v,GL_DYNAMIC_DRAW_ARB);
	}
	if(m_withT)
	{
		glGenBuffersARB(1,&m_t);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_t);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB,(size << 1) * sizeof(float),t,GL_DYNAMIC_DRAW_ARB);
	}
	if(m_withN)
	{
		glGenBuffersARB(1,&m_n);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_n);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB,size * 3 * sizeof(float),n,GL_DYNAMIC_DRAW_ARB);
	}
	if(m_withC)
	{
		glGenBuffersARB(1,&m_c);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_c);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB,(size << 2) * sizeof(float),c,GL_DYNAMIC_DRAW_ARB);
	}
	if(m_withI)
	{
		glGenBuffersARB(1,&m_i);
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,m_i);
		glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB,m_indexSize * sizeof(int),i,GL_DYNAMIC_DRAW_ARB);
	}
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);	//取消绑定
	
	if(m_withVAO)
	{//建立并初始化VAO
		glGenVertexArrays(1,&m_vao); // Create our Vertex Array Object  
		glBindVertexArray(m_vao);
		if(m_withC)
		{
			glEnableClientState(GL_COLOR_ARRAY);	
			glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_c);
			glColorPointer(4,GL_FLOAT,0,NULL);
		}
		if(m_withT)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_t);
			glTexCoordPointer(2,GL_FLOAT,0,NULL);
		}
		if(m_withN)
		{
			glEnableClientState(GL_NORMAL_ARRAY);
			glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_n);
			glNormalPointer(GL_FLOAT,0,NULL);
		}
		if(m_withV)
		{
			glEnableClientState(GL_VERTEX_ARRAY);	
			glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_v);
			glVertexPointer(m_vSize,GL_FLOAT,0,NULL);
		}
		if(m_withI)
		{
			glEnableClientState(GL_INDEX_ARRAY);	
			glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,m_i);
			glIndexPointer(GL_INT,0,NULL);
		}
	//	glEnableVertexAttribArray(0); // Disable our Vertex Array Object  
		glBindVertexArray(0); 
	}
	m_isInited = XTrue;
	return XTrue;
}
XBool XVBO::setAsSubject(const XVBO &temp,XBool v,XBool n,XBool c,XBool t,XBool i,XBool cv,XBool cn,XBool cc,XBool ct,XBool ci)
{
	if(!temp.m_isInited) return XFalse;	//如果目标没有初始化则不能作为从属
	//if(m_isInited) release(); //如果自身已经初始化，则这里释放资源
	if(!m_isInited || m_isSubject) return XFalse;	//如果已经有从属关系，则不能重复设置从属关系
	if(cv && !temp.m_withV) return XFalse;	//目标没有相关属性
	if(cn && !temp.m_withN) return XFalse;
	if(cc && !temp.m_withC) return XFalse;
	if(ct && !temp.m_withT) return XFalse;
	if(ci && !temp.m_withI) return XFalse;
	//初始化相关属性
	if(v) m_withV = XTrue; else m_withV = XFalse;
	if(t) m_withT = XTrue; else m_withT = XFalse;
	if(n) m_withN = XTrue; else m_withN = XFalse;
	if(c) m_withC = XTrue; else m_withC = XFalse;
	if(i) m_withI = XTrue; else m_withI = XFalse;
	m_size = temp.m_size;
	m_vSize = temp.m_vSize;
	if(m_withVAO)
	{
		if(!m_isSubject) glDeleteVertexArrays(1, &m_vao);	//释放自身的资源
		m_vao = temp.m_vao;
	}
	if(v)
	{
		if(cv) 
		{
			if(m_withV && !m_subjectV) glDeleteBuffersARB(1,&m_v);	//释放掉原有的资源
			m_v = temp.m_v;
			m_subjectV = XTrue;
		}else 
		{
			//glGenBuffersARB(1,&m_v);
			m_subjectV = XFalse;
		}
	}
	if(n)
	{
		if(cn) 
		{
			if(m_withN && !m_subjectN) glDeleteBuffersARB(1,&m_n);	//释放掉原有的资源
			m_n = temp.m_n;
			m_subjectN = XTrue;
		}else
		{
			//glGenBuffersARB(1,&m_n);
			m_subjectN = XFalse;
		}
	}
	if(c)
	{
		if(cc) 
		{
			if(m_withC && !m_subjectC) glDeleteBuffersARB(1,&m_c);	//释放掉原有的资源
			m_c = temp.m_c;
			m_subjectC = XTrue;
		}else
		{
			//glGenBuffersARB(1,&m_c);
			m_subjectC = XFalse;
		}
	}
	if(t)
	{
		if(ct)
		{
			if(m_withT && !m_subjectT) glDeleteBuffersARB(1,&m_t);	//释放掉原有的资源
			m_t = temp.m_t;
			m_subjectT = XTrue;
		}else
		{
			//glGenBuffersARB(1,&m_t);
			m_subjectT = XFalse;
		}
	}
	if(i)
	{
		if(ci)
		{
			m_indexSize = temp.m_indexSize;
			if(m_withI && !m_subjectI) glDeleteBuffersARB(1,&m_i);	//释放掉原有的资源
			m_i = temp.m_i;
			m_subjectI = XTrue;
		}else
		{
			//glGenBuffersARB(1,&m_i);
			m_subjectI = XFalse;
		}
	}
	m_isSubject = XTrue;
	m_isInited = XTrue;
	return XTrue;
}
XBool XVBO::updateDateX(int size,const float *v,const float *t,const float *n,const float *c,
						 int iSize,const unsigned int *i)
{
	if(m_size < size)
	{//这里需要重新分配
		m_size = size;
		if(m_withV && v != NULL)
		{
			glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_v);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB,size * m_vSize * sizeof(float),v,GL_DYNAMIC_DRAW_ARB);//GL_STATIC_DRAW_ARB);
		}
		if(m_withT && t != NULL)
		{
			glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_t);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB,(size << 1) * sizeof(float),t,GL_DYNAMIC_DRAW_ARB);
		}
		if(m_withN && n != NULL)
		{
			glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_n);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB,size * 3 * sizeof(float),n,GL_DYNAMIC_DRAW_ARB);
		}
		if(m_withC && c != NULL)
		{
			glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_c);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB,(size << 2) * sizeof(float),c,GL_DYNAMIC_DRAW_ARB);
		}
	}else
	{//这里不需要重新分配
		if(m_withV && v != NULL)
		{
			glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_v);
			if(size > 2048)
			{
				memcpy(glMapBufferARB(GL_ARRAY_BUFFER_ARB,GL_WRITE_ONLY_ARB),v,size * m_vSize * sizeof(float));
				glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
			}else
				glBufferSubDataARB(GL_ARRAY_BUFFER_ARB,0,size * m_vSize * sizeof(float),v);
		}
		if(m_withT && t != NULL)
		{
			glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_t);
			if(size > 2048)
			{
				memcpy(glMapBufferARB(GL_ARRAY_BUFFER_ARB,GL_WRITE_ONLY_ARB),t,(size << 1) * sizeof(float));
				glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
			}else
				glBufferSubDataARB(GL_ARRAY_BUFFER_ARB,0,(size << 1) * sizeof(float),t);
		}
		if(m_withN && n != NULL)
		{
			glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_n);
			if(size > 2048)
			{
				memcpy(glMapBufferARB(GL_ARRAY_BUFFER_ARB,GL_WRITE_ONLY_ARB),n,size * 3 * sizeof(float));
				glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
			}else
				glBufferSubDataARB(GL_ARRAY_BUFFER_ARB,0,size * 3 * sizeof(float),n);
		}
		if(m_withC && c != NULL)
		{
			glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_c);
			if(size > 2048)
			{
				memcpy(glMapBufferARB(GL_ARRAY_BUFFER_ARB,GL_WRITE_ONLY_ARB),c,(size << 2) * sizeof(float));
				glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
			}else
				glBufferSubDataARB(GL_ARRAY_BUFFER_ARB,0,(size << 2) * sizeof(float),c);
		}
	}
	if(m_indexSize < iSize)
	{
		if(m_withI && i != NULL)
		{
			glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,m_i);
			glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB,m_indexSize * sizeof(int),i,GL_DYNAMIC_DRAW_ARB);
		}
	}else
	{
		if(m_withI && i != NULL)
		{
			glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,m_i);
			if(m_indexSize > 8192)
			{
				memcpy(glMapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,GL_WRITE_ONLY_ARB),i,m_indexSize * sizeof(int));
				glUnmapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB);
			}else
				glBufferSubDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB,0,m_indexSize * sizeof(int),i);
		}
	}
	m_indexSize = iSize;
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);	//取消绑定
	return XTrue;
}
void XVBO::use(XBool withTex)
{
	if(!m_isInited) return;
	if(m_withVAO) 
	{
		glBindVertexArray(m_vao);
		return;
	}
	if(m_withC)
	{
		glEnableClientState(GL_COLOR_ARRAY);	
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_c);
		glColorPointer(4,GL_FLOAT,0,NULL);
	}
	if(m_withT && withTex)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_t);
		glTexCoordPointer(2,GL_FLOAT,0,NULL);
	}
	if(m_withN)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_n);
		glNormalPointer(GL_FLOAT,0,NULL);
	}
	if(m_withV)
	{
		glEnableClientState(GL_VERTEX_ARRAY);	
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_v);
		glVertexPointer(m_vSize,GL_FLOAT,0,NULL);
	}
	if(m_withI)
	{
		glEnableClientState(GL_INDEX_ARRAY);	
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,m_i);
		glIndexPointer(GL_INT,0,NULL);
	}
}
void XVBO::release()
{
	if(!m_isInited) return;
	if(m_isSubject)
	{
		if(m_withVAO) glDeleteVertexArrays(1, &m_vao);
		if(m_withV && !m_subjectV) glDeleteBuffersARB(1,&m_v);
		if(m_withN && !m_subjectN) glDeleteBuffersARB(1,&m_n);
		if(m_withT && !m_subjectT) glDeleteBuffersARB(1,&m_t);
		if(m_withC && !m_subjectC) glDeleteBuffersARB(1,&m_c);
		if(m_withI && !m_subjectI) glDeleteBuffersARB(1,&m_i);
	}else
	{
		if(m_withV) glDeleteBuffersARB(1,&m_v);
		if(m_withN) glDeleteBuffersARB(1,&m_n);
		if(m_withT) glDeleteBuffersARB(1,&m_t);
		if(m_withC) glDeleteBuffersARB(1,&m_c);
		if(m_withI) glDeleteBuffersARB(1,&m_i);
	}
	m_isInited = XFalse;
}
namespace XGL
{
	XBool getIsFramebufferReady()
	{
		//检查FBO状态
		GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
		switch(status)
		{
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			LogStr("Framebuffer complete.");
			return XTrue;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
			LogStr("[ERROR] Framebuffer incomplete: Attachment is NOT complete.");
			return XFalse;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
			LogStr("[ERROR] Framebuffer incomplete: No image is attached to FBO.");
			return XFalse;
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			LogStr("[ERROR] Framebuffer incomplete: Attached images have different dimensions.");
			return XFalse;
		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			LogStr("[ERROR] Framebuffer incomplete: Color attached images have different internal formats.");
			return XFalse;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
			LogStr("[ERROR] Framebuffer incomplete: Draw buffer.");
			return XFalse;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
			LogStr("[ERROR] Framebuffer incomplete: Read buffer.");
			return XFalse;
		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			LogStr("[ERROR] Unsupported by FBO implementation.");
			return XFalse;
		default:
			LogStr("[ERROR] Unknow error.");
			return XFalse;
		}
	}
}
XBool XFBO::init(int w,int h,XColorMode type)
{
	if(m_pTextures.size() != 0) return XTrue;
	//检查是否支持FBO
	if(!XEG.getIsFBOSupported()) return XFalse;

//	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &m_maxTextureSum);
	//建立贴图
	addOneTexture(w,h,type);
	m_type = type;
	if(type == COLOR_DEPTH)	//这里需要测试是否可以合并这种判断
	{
		glGenFramebuffersEXT(1,&m_fboId);					//建立FBO
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,m_fboId);	//绑定FBO
		glGenRenderbuffersEXT(1, &m_rboId);					
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,m_rboId);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,GL_RGBA,w,h);

		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT,m_rboId);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_DEPTH_ATTACHMENT_EXT,GL_TEXTURE_2D,m_pTextures[0]->m_texture,0);
		m_zeroTexIndex = 0;
	}else
	{
		//建立一个framebuffer object，当退出时需要删除
		glGenFramebuffersEXT(1, &m_fboId);					//建立FBO
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fboId);	//绑定FBO
		//glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,GL_RGBA,w,h);
		//使用浮点的GL_RGBA32F_ARB，可以避免残影不消退的问题，尚未搞好
		//glRenderbufferStorageMultisample(GL_RENDERBUFFER,GL_RGBA32F_ARB,GL_DEPTH_STENCIL,w,h);
		//glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,m_fboId);

		//建立深度渲染buff，不建立对于不需要深度计算的应用貌似也没有问题
		//glGenRenderbuffersEXT(1, &m_rboId);					
		//glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_rboId);
		//glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, w, h);
		//glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

		//挂接贴图
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_pTextures[0]->m_texture, 0);
		m_zeroTexIndex = 0;
		//挂接深度渲染点
		//glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT,m_rboId);
	}

    //如果你没有挂接贴图的话就取消颜色缓存
    //例如当你只需要对贴图进行深度渲染的时候
    //否则glCheckFramebufferStatusEXT将不会准备就绪.
    //glDrawBuffer(GL_NONE);
    //glReadBuffer(GL_NONE);

	if(!XGL::getIsFramebufferReady()) return XFalse;

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	return XTrue;
}
void XFBO::useFBO(bool newSize,int w,int h)
{
	if(newSize)
	{
		glMatrixMode(GL_PROJECTION);					//设置当前矩阵模式 （对投影矩阵应用之后的矩阵操作）
		glPushMatrix();
		glLoadIdentity();								//变换坐标系函数
		switch(XEG.m_windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0:
			glOrtho(0,w,0,h,-1,1);
			glViewport(0,0,w,h);
			break;
		case WINDOW_ROTATE_MODE_90:
			glOrtho(w,0,h,0,-1,1);
			glViewport(0,0,w,h);
			break;
		case WINDOW_ROTATE_MODE_180:
			glOrtho(0,w,0,h,-1,1);
			glViewport(0,0,w,h);
			break;
		case WINDOW_ROTATE_MODE_270:
			glOrtho(w,0,h,0,-1,1);
			glViewport(0,0,w,h);
			break;
		}
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		switch(XEG.m_windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0: break;	//do nothing
		case WINDOW_ROTATE_MODE_90: 
			glTranslatef(w,h,0);
			glRotatef(180,0,0,1);
			break;
		case WINDOW_ROTATE_MODE_180: 
			break;
		case WINDOW_ROTATE_MODE_270: 
			glTranslatef(w,h,0);
			glRotatef(180,0,0,1);
			break;
		}	
	}else
	{
		glMatrixMode(GL_PROJECTION);					//设置当前矩阵模式 （对投影矩阵应用之后的矩阵操作）
		glPushMatrix();
		glLoadIdentity();								//变换坐标系函数
		switch(XEG.m_windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0:
			glOrtho(0,XEG.m_windowData.sceneW,0,XEG.m_windowData.sceneH,-1,1);
			glViewport(0,0,XEG.m_windowData.sceneW,XEG.m_windowData.sceneH);
			break;
		case WINDOW_ROTATE_MODE_90:
			glOrtho(XEG.m_windowData.sceneW,0,XEG.m_windowData.sceneH,0,-1,1);
			glViewport(0,0,XEG.m_windowData.sceneW,XEG.m_windowData.sceneH);
			break;
		case WINDOW_ROTATE_MODE_180:
			glOrtho(0,XEG.m_windowData.sceneW,0,XEG.m_windowData.sceneH,-1,1);
			glViewport(0,0,XEG.m_windowData.sceneW,XEG.m_windowData.sceneH);
			break;
		case WINDOW_ROTATE_MODE_270:
			glOrtho(XEG.m_windowData.sceneW,0,XEG.m_windowData.sceneH,0,-1,1);
			glViewport(0,0,XEG.m_windowData.sceneW,XEG.m_windowData.sceneH);
			break;
		}
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		switch(XEG.m_windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0: break;	//do nothing
		case WINDOW_ROTATE_MODE_90: 
			glTranslatef(XEG.m_windowData.sceneW,XEG.m_windowData.sceneH,0);
			glRotatef(180,0,0,1);
			break;
		case WINDOW_ROTATE_MODE_180: 
			break;
		case WINDOW_ROTATE_MODE_270: 
			glTranslatef(XEG.m_windowData.sceneW,XEG.m_windowData.sceneH,0);
			glRotatef(180,0,0,1);
			break;
		}
	}
	bind();
}
//同时绑定多个FBO
XBool XFBO::attachTexs(int sum,int index,...)
{
	if(sum <= 0 ||
		sum > XEG.getMaxTextureSum()) return XFalse;
	int indexs[16] = {-1,-1,-1,-1,-1,
						-1,-1,-1,-1,-1,
						-1,-1,-1,-1,-1,-1};	//最多可以使用16个贴图
	va_list arg_ptr;
	va_start(arg_ptr,index);
	indexs[0] = index;
	for(int i = 1;i < sum;++ i)
	{
		indexs[i] = va_arg(arg_ptr, int);
		if(indexs[i] < 0) return XFalse;
	}
	va_end(arg_ptr);
	//然后开始分别绑定贴图
	for(int i = 0;i < sum;++ i)
	{
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT + i,GL_TEXTURE_2D,m_pTextures[indexs[i]]->m_texture,0);
	}
	m_zeroTexIndex = indexs[0];
	//m_upTextureSum = sum;
	GLenum buffers[] = {GL_COLOR_ATTACHMENT0_EXT,GL_COLOR_ATTACHMENT1_EXT,
		GL_COLOR_ATTACHMENT2_EXT,GL_COLOR_ATTACHMENT3_EXT,
		GL_COLOR_ATTACHMENT4_EXT,GL_COLOR_ATTACHMENT5_EXT,
		GL_COLOR_ATTACHMENT6_EXT,GL_COLOR_ATTACHMENT7_EXT,
		GL_COLOR_ATTACHMENT8_EXT,GL_COLOR_ATTACHMENT9_EXT,
		GL_COLOR_ATTACHMENT10_EXT,GL_COLOR_ATTACHMENT11_EXT,
		GL_COLOR_ATTACHMENT12_EXT,GL_COLOR_ATTACHMENT13_EXT,
		GL_COLOR_ATTACHMENT14_EXT,GL_COLOR_ATTACHMENT15_EXT};
	glDrawBuffers(sum,buffers);
	if(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT)
		return XFalse;
	return XTrue;
}
void XFBO::removeFBO()
{
	unbind();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);					//设置当前矩阵模式 （对投影矩阵应用之后的矩阵操作）
	glPopMatrix();
	switch(XEG.m_windowData.rotateMode)
	{
	case WINDOW_ROTATE_MODE_0:
		glViewport(0,0,getWindowWidth(),getWindowHeight());
		break;
	case WINDOW_ROTATE_MODE_90:
		glViewport(0,0,getWindowHeight(),getWindowWidth());
		break;
	case WINDOW_ROTATE_MODE_180:
		glViewport(0,0,getWindowWidth(),getWindowHeight());
		break;
	case WINDOW_ROTATE_MODE_270:
		glViewport(0,0,getWindowHeight(),getWindowWidth());
		break;
	}
}
void XFBO::release()
{
	for(unsigned int i = 0;i < m_pTextures.size();++ i)
	{
		XMem::XDELETE(m_pTextures[i]);
	}
	m_pTextures.clear();

	glDeleteFramebuffersEXT(1, &m_fboId);
	if(m_type == COLOR_DEPTH) glDeleteRenderbuffersEXT(1, &m_rboId);
}
bool XFBOEx::init(int w,int h,XColorMode type)
{
	if(m_isInited) return false;
	m_w = w;
	m_h = h;
	unsigned int colorMode = GL_RGBA;
	switch(type)
	{
	case COLOR_RGBA:colorMode = GL_RGBA;break;
	case COLOR_RGB:	colorMode = GL_RGB;break;
	case COLOR_DEPTH:colorMode = GL_DEPTH_COMPONENT;break;
	case COLOR_RGBA_F:colorMode = GL_RGBA;break;
	default:	//其他格式不支持该操作
		return false;
	}
	glGenFramebuffersEXT(1,&m_fboId);				//建立FBO
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,m_fboId);  

	glGenRenderbuffersEXT(1,&m_rboId);			//建立渲染目标
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,m_rboId);	
	glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT,4,colorMode,w,h);  //启用多重采样
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,0);  
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_RENDERBUFFER_EXT,m_rboId);//挂接渲染目标

	glGenRenderbuffersEXT(1,&m_rbdId);			//建立深度渲染目标
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,m_rbdId);  
	glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT,4,GL_DEPTH_COMPONENT,w,h);    //启用多重采样
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,0);  
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,GL_DEPTH_ATTACHMENT_EXT,GL_RENDERBUFFER,m_rbdId);  //挂接深度渲染目标

	if(!XGL::getIsFramebufferReady()) return false;
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);		//FBO建立完成

	m_isInited = true;
	return true;
}
void XFBOEx::useFBO(bool newSize,int w,int h)
{
	if(newSize)
	{
		glMatrixMode(GL_PROJECTION);					//设置当前矩阵模式 （对投影矩阵应用之后的矩阵操作）
		glPushMatrix();
		glLoadIdentity();								//变换坐标系函数
		switch(XEG.m_windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0:
			glOrtho(0,w,0,h,-1,1);
			glViewport(0,0,w,h);
			break;
		case WINDOW_ROTATE_MODE_90:
			glOrtho(w,0,h,0,-1,1);
			glViewport(0,0,w,h);
			break;
		case WINDOW_ROTATE_MODE_180:
			glOrtho(0,w,0,h,-1,1);
			glViewport(0,0,w,h);
			break;
		case WINDOW_ROTATE_MODE_270:
			glOrtho(w,0,h,0,-1,1);
			glViewport(0,0,w,h);
			break;
		}
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		switch(XEG.m_windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0: break;	//do nothing
		case WINDOW_ROTATE_MODE_90: 
			glTranslatef(w,h,0);
			glRotatef(180,0,0,1);
			break;
		case WINDOW_ROTATE_MODE_180: 
			break;
		case WINDOW_ROTATE_MODE_270: 
			glTranslatef(w,h,0);
			glRotatef(180,0,0,1);
			break;
		}	
	}else
	{
		glMatrixMode(GL_PROJECTION);					//设置当前矩阵模式 （对投影矩阵应用之后的矩阵操作）
		glPushMatrix();
		glLoadIdentity();								//变换坐标系函数
		switch(XEG.m_windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0:
			glOrtho(0,XEG.m_windowData.sceneW,0,XEG.m_windowData.sceneH,-1,1);
			glViewport(0,0,XEG.m_windowData.sceneW,XEG.m_windowData.sceneH);
			break;
		case WINDOW_ROTATE_MODE_90:
			glOrtho(XEG.m_windowData.sceneW,0,XEG.m_windowData.sceneH,0,-1,1);
			glViewport(0,0,XEG.m_windowData.sceneW,XEG.m_windowData.sceneH);
			break;
		case WINDOW_ROTATE_MODE_180:
			glOrtho(0,XEG.m_windowData.sceneW,0,XEG.m_windowData.sceneH,-1,1);
			glViewport(0,0,XEG.m_windowData.sceneW,XEG.m_windowData.sceneH);
			break;
		case WINDOW_ROTATE_MODE_270:
			glOrtho(XEG.m_windowData.sceneW,0,XEG.m_windowData.sceneH,0,-1,1);
			glViewport(0,0,XEG.m_windowData.sceneW,XEG.m_windowData.sceneH);
			break;
		}
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		switch(XEG.m_windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0: break;	//do nothing
		case WINDOW_ROTATE_MODE_90: 
			glTranslatef(XEG.m_windowData.sceneW,XEG.m_windowData.sceneH,0);
			glRotatef(180,0,0,1);
			break;
		case WINDOW_ROTATE_MODE_180: 
			break;
		case WINDOW_ROTATE_MODE_270: 
			glTranslatef(XEG.m_windowData.sceneW,XEG.m_windowData.sceneH,0);
			glRotatef(180,0,0,1);
			break;
		}
	}
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,m_fboId);
}
void XFBOEx::removeFBO()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // unbind
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);					//设置当前矩阵模式 （对投影矩阵应用之后的矩阵操作）
	glPopMatrix();
	if (gFrameworkData.pFrameWork == NULL) return;
	switch (gFrameworkData.pFrameWork->m_windowData.rotateMode)
	{
	case WINDOW_ROTATE_MODE_0:
		glViewport(0,0,getWindowWidth(),getWindowHeight());
		break;
	case WINDOW_ROTATE_MODE_90:
		glViewport(0,0,getWindowHeight(),getWindowWidth());
		break;
	case WINDOW_ROTATE_MODE_180:
		glViewport(0,0,getWindowWidth(),getWindowHeight());
		break;
	case WINDOW_ROTATE_MODE_270:
		glViewport(0,0,getWindowHeight(),getWindowWidth());
		break;
	}
}
namespace XRender
{
	void drawBlankPlane(int w,int h,unsigned int tex,XShaderGLSL *pShader)
	{
		if(pShader != NULL)
		{
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			pShader->useShader();
		}
		glActiveTexture(GL_TEXTURE0);
		XGL::EnableTexture2D();
		XGL::BindTexture2D(tex);
		glBegin(GL_QUADS);
			glTexCoord2f(0,0);
			glVertex2f(0,0);
			glTexCoord2f(0,1);
			glVertex2f(0,h);
			glTexCoord2f(1,1);
			glVertex2f(w,h);
			glTexCoord2f(1,0);
			glVertex2f(w,0);
		glEnd();
		if(pShader != NULL)
		{
			pShader->disShader();
			glPopAttrib();
		}
	}
}
#if !WITH_INLINE_FILE
#include "XShaderGLSL.inl"
#endif
}