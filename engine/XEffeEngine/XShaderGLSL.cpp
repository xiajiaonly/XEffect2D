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
		glUniform2i(m_handle,((XVec2 *)m_p)->x,((XVec2 *)m_p)->y);
		break;
	case DATA_TYPE_2FLOAT:
		glUniform2f(m_handle,((XVec2 *)m_p)->x,((XVec2 *)m_p)->y);
		break;
	case DATA_TYPE_3INT:
		glUniform3i(m_handle,((XVec3 *)m_p)->x,((XVec3 *)m_p)->y,((XVec3 *)m_p)->z);
		break;
	case DATA_TYPE_3FLOAT:
		glUniform3f(m_handle,((XVec3 *)m_p)->x,((XVec3 *)m_p)->y,((XVec3 *)m_p)->z);
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
	int handle = glGetUniformLocation(m_shaderHandle.shaderHandle,name);
	int index = -1;
	for(unsigned int i = 0;i < m_dataInfos.size(); ++ i)
	{
		if (m_dataInfos[i].m_handle != handle) continue;
		index = i;
		break;
	}
	if(index < 0)
	{
		XShaderDataInfo tmp;
		tmp.m_handle = handle;
		tmp.m_type = type;
		tmp.m_p = p;
		tmp.m_size = size;
		m_dataInfos.push_back(tmp);
	}else
	{
		m_dataInfos[index].m_type = type;
		m_dataInfos[index].m_p = p;
		m_dataInfos[index].m_size = size;
	}
	return XTrue;
}
//链接贴图
XBool XShaderGLSL::connectTexture(const char *name,unsigned int * tex,XShaderTexType type)
{
	if(name == NULL ||
		(m_texInfos.size() != 0 && tex == NULL) ||
		m_texInfos.size() >= 32 - 1) return XFalse;

	int handle = glGetUniformLocation(m_shaderHandle.shaderHandle,name);
	int index = -1;
	for(unsigned int i = 0;i < m_texInfos.size(); ++ i)
	{
		if (m_texInfos[i].texHandle != handle) continue;
		index = i;
		break;
	}
	if(index < 0)
	{
		XShaderTexInfo tmp;
		tmp.texHandle = glGetUniformLocation(m_shaderHandle.shaderHandle,name);
		tmp.texID = tex;
		tmp.texType = type;
		m_texInfos.push_back(tmp);
	}else
	{
		m_texInfos[index].texID = tex;
		m_texInfos[index].texType = type;
	}
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
	for(auto it = m_dataInfos.begin();it != m_dataInfos.end();++ it)
	{
		it->updateData();
	}
	for(auto it = m_UBOInfos.begin();it != m_UBOInfos.end();++ it)
	{
		it->updateData();
	}
}
void XShaderGLSL::useShader(bool withTex0)	//使用shader
{
	if(!m_isInited) return;
	glUseProgram(m_shaderHandle.shaderHandle);
	//glUseProgramObjectARB(m_shaderHandle);
	updateData();
	if(m_texInfos.size() > 0)
		glUniform1i(m_texInfos[0].texHandle,0);

	if(withTex0 && m_texInfos.size() > 0 && m_texInfos[0].texID != NULL)
	{//这里绑定0
		glActiveTexture(GL_TEXTURE0);
		switch(m_texInfos[0].texType)
		{
		case TEX_TYPE_2D:
			XGL::EnableTexture2D();
			XGL::BindTexture2D(*(m_texInfos[0].texID),0);
			break;
		case TEX_TYPE_CUBE://GL_TEXTURE_CUBE_MAP贴图
			glEnable(GL_TEXTURE_CUBE_MAP);
			glBindTexture(GL_TEXTURE_CUBE_MAP,*(m_texInfos[0].texID));
			break;
		}
	}
	bindTex(1);
	glActiveTexture(GL_TEXTURE0);
}
void XShaderGLSL::bindTex(int index)
{
	for(unsigned int i = index;i < m_texInfos.size();++ i)
	{
		glUniform1i(m_texInfos[i].texHandle,i);
		if(m_texInfos[i].texID == NULL) continue;
		glActiveTexture(GL_TEXTURE0 + i);
		switch(m_texInfos[i].texType)
		{
		case TEX_TYPE_2D:
			XGL::EnableTexture2D();
			XGL::BindTexture2D(*(m_texInfos[i].texID),i);
			XGL::DisableTexture2D();
			break;
		case TEX_TYPE_CUBE://GL_TEXTURE_CUBE_MAP贴图
			glEnable(GL_TEXTURE_CUBE_MAP);
			glBindTexture(GL_TEXTURE_CUBE_MAP,*(m_texInfos[i].texID));
			glDisable(GL_TEXTURE_CUBE_MAP);
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
		glUniform1i(m_texInfos[0].texHandle,0);

	if(tex0 == 0) return;	//不需要绑定贴图
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
	bindTex(1);
	glActiveTexture(GL_TEXTURE0);
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
XBool XSSBO::init(int size,int sum, const void * p)
{
	if (m_isInited || p == NULL) return XFalse;
	int maxS = 0;
	glGetIntegerv(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &maxS);
	if (size > maxS) return XFalse;
	m_coreSum = sum;
	m_size = size;
	glGenBuffersARB(1, &m_ssboHandle);
	glBindBufferARB(GL_SHADER_STORAGE_BUFFER, m_ssboHandle);
	glBufferDataARB(GL_SHADER_STORAGE_BUFFER, size, p, GL_DYNAMIC_COPY);

//	glDispatchCompute(m_coreSum, 1, 1);
//	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
//	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

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
	case COLOR_RGB_F:m_dataSize = m_w * (m_h * 3) * sizeof(float);break;
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
	case COLOR_RGBA:glReadPixels(m_px,m_py,m_wx,m_wy,GL_RGBA,GL_UNSIGNED_BYTE,0);break;	//注意尺寸4字节对齐，否则会造成crash
	case COLOR_BGRA:glReadPixels(m_px,m_py,m_wx,m_wy,GL_BGRA,GL_UNSIGNED_BYTE,0);break;	//注意尺寸4字节对齐，否则会造成crash
	case COLOR_RGB:	glReadPixels(m_px,m_py,m_wx,m_wy,GL_RGB,GL_UNSIGNED_BYTE,0);break;	//注意尺寸4字节对齐，否则会造成crash
	case COLOR_BGR:	glReadPixels(m_px,m_py,m_wx,m_wy,GL_BGR,GL_UNSIGNED_BYTE,0);break;	//注意尺寸4字节对齐，否则会造成crash
	case COLOR_RED:	glReadPixels(m_px,m_py,m_wx,m_wy,GL_RED,GL_UNSIGNED_BYTE,0);break;	//注意尺寸4字节对齐，否则会造成crash
	case COLOR_GRAY:glReadPixels(m_px,m_py,m_wx,m_wy,GL_LUMINANCE,GL_UNSIGNED_BYTE,0);break;	//注意尺寸4字节对齐，否则会造成crash
	case COLOR_RGBA_F:glReadPixels(m_px,m_py,m_wx,m_wy,GL_RGBA,GL_FLOAT,0);break;	//注意尺寸4字节对齐，否则会造成crash
	case COLOR_RGB_F:glReadPixels(m_px,m_py,m_wx,m_wy,GL_RGB,GL_FLOAT,0);break;	//注意尺寸4字节对齐，否则会造成crash
	case COLOR_GRAY_F:glReadPixels(m_px,m_py,m_wx,m_wy,GL_RED,GL_FLOAT,0);break;	//注意尺寸4字节对齐，否则会造成crash
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
	m_curISize = m_indexSize = iSize;
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
		glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, iSize * sizeof(int),i,GL_DYNAMIC_DRAW_ARB);
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
			m_curISize = temp.m_curISize;
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
		m_curISize = m_indexSize = iSize;
		if(m_withI && i != NULL)
		{
			glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,m_i);
			glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, iSize * sizeof(int),i,GL_DYNAMIC_DRAW_ARB);
		}
	}else
	{
		m_curISize = iSize;
		if(m_withI && i != NULL)
		{
			glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,m_i);
			if(iSize > 8192)
			{
				memcpy(glMapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,GL_WRITE_ONLY_ARB),i, iSize * sizeof(int));
				glUnmapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB);
			}else
				glBufferSubDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB,0, iSize * sizeof(int),i);
		}
	}
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
	XBool getIsFramebufferReadyN()
	{
		//检查FBO状态
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		switch (status)
		{
		case GL_FRAMEBUFFER_COMPLETE:
			LogStr("Framebuffer complete.");
			return XTrue;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			LogStr("[ERROR] Framebuffer incomplete: Attachment is NOT complete.");
			return XFalse;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			LogStr("[ERROR] Framebuffer incomplete: No image is attached to FBO.");
			return XFalse;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			LogStr("[ERROR] Framebuffer incomplete: Draw buffer.");
			return XFalse;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			LogStr("[ERROR] Framebuffer incomplete: Read buffer.");
			return XFalse;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			LogStr("[ERROR] Unsupported by FBO implementation.");
			return XFalse;
		default:
			LogStr("[ERROR] Unknow error.");
			return XFalse;
		}
	}
}
std::vector<XFBOInfo> XFboBase::m_fboBuffer;
void XFboBase::opertion(XFBOOpertion op,bool isNew,int w,int h,const std::vector<int>*indexs)
{//为了防止FBO嵌套造成问题，这里需要做相应的队列处理(尚未完成)
//	if(m_fboBuffer.size() <= 0) return;	//开启这一行将会造成FBO无法嵌套
	switch (op)
	{
	case XE::OPER_USEFBO:
		//下面检查数据的合法性
		for(int i = int(m_fboBuffer.size()) - 1;i >= 0;-- i)
		{
			switch (m_fboBuffer[i].opertion)
			{
			case XE::OPER_USEFBO:
				if(m_fboBuffer[i].pClass == this)
					LogStr("Warning:重复调用useFbo()");
				m_fboBuffer.push_back(XFBOInfo(this,op,isNew,w,h));
				return;
			case XE::OPER_BIND:
				if(m_fboBuffer[i].pClass == this)
					LogStr("Warning:重复调用bind()");
				m_fboBuffer.push_back(XFBOInfo(this,op,isNew,w,h));
				return;
			case XE::OPER_REMOVEFBO:	//不会有的记录
			case XE::OPER_UNBIND:		//不会有的记录
			case XE::OPER_ATTACHTEX:	//不重要的记录
			case XE::OPER_ATTACHTEXS:	//不重要的记录
				break;
			}
		}
		m_fboBuffer.push_back(XFBOInfo(this,op,isNew,w,h));
		break;
	case XE::OPER_REMOVEFBO:
		for(int i = int(m_fboBuffer.size()) - 1;i >= 0;-- i)
		{
			switch (m_fboBuffer[i].opertion)
			{
			case XE::OPER_USEFBO:
				if(m_fboBuffer[i].pClass != this)
				{
					LogStr("Warning:FBO错误的调用关系");
					m_fboBuffer.pop_back();
					break;
				}else
				{
					m_fboBuffer.pop_back();	//正确的
					resumeBuff();
					return;
				}
			case XE::OPER_BIND:
				LogStr("Warning:FBO错误的调用关系");
				m_fboBuffer.pop_back();
				break;
			case XE::OPER_REMOVEFBO:	//不会有的记录
			case XE::OPER_UNBIND:		//不会有的记录
			case XE::OPER_ATTACHTEX:	//不重要的记录
			case XE::OPER_ATTACHTEXS:	//不重要的记录
				m_fboBuffer.pop_back();
				break;
			}
		}
		break;
	case XE::OPER_BIND:
		for(int i = int(m_fboBuffer.size()) - 1;i >= 0;-- i)
		{
			switch (m_fboBuffer[i].opertion)
			{
			case XE::OPER_USEFBO:
				if(m_fboBuffer[i].pClass == this)
					LogStr("Warning:重复调用useFbo()");
				m_fboBuffer.push_back(XFBOInfo(this,OPER_BIND));
				return;
			case XE::OPER_BIND:
				if(m_fboBuffer[i].pClass == this)
					LogStr("Warning:重复调用bind()");
				m_fboBuffer.push_back(XFBOInfo(this,OPER_BIND));
				return;
			case XE::OPER_REMOVEFBO:	//不会有的记录
			case XE::OPER_UNBIND:		//不会有的记录
			case XE::OPER_ATTACHTEX:	//不重要的记录
			case XE::OPER_ATTACHTEXS:	//不重要的记录
				break;
			}
		}
		m_fboBuffer.push_back(XFBOInfo(this,OPER_BIND));
		break;
	case XE::OPER_UNBIND:
		for(int i = int(m_fboBuffer.size()) - 1;i >= 0;-- i)
		{
			switch (m_fboBuffer[i].opertion)
			{
			case XE::OPER_USEFBO:
				LogStr("Warning:FBO错误的调用关系");
				m_fboBuffer.pop_back();
				break;
			case XE::OPER_BIND:
				if(m_fboBuffer[i].pClass != this)
				{
					LogStr("Warning:FBO错误的调用关系");
					m_fboBuffer.pop_back();
					break;
				}else
				{
					m_fboBuffer.pop_back();	//正确的
					resumeBuff();
					return;
				}
			case XE::OPER_REMOVEFBO:	//不会有的记录
			case XE::OPER_UNBIND:		//不会有的记录
			case XE::OPER_ATTACHTEX:	//不重要的记录
			case XE::OPER_ATTACHTEXS:	//不重要的记录
				m_fboBuffer.pop_back();
				break;
			}
		}
		break;
	case XE::OPER_ATTACHTEX:
		for(int i = int(m_fboBuffer.size()) - 1;i >= 0;-- i)
		{
			switch (m_fboBuffer[i].opertion)
			{
			case XE::OPER_USEFBO:
				if(m_fboBuffer[i].pClass != this)
					LogStr("Warning:FBO错误的调用关系");
				m_fboBuffer.push_back(XFBOInfo(this,OPER_ATTACHTEX));
				//m_fboBuffer[int(m_fboBuffer.size()) - 1].texIndexs.push_back((*indexs)[0]);
				m_fboBuffer[int(m_fboBuffer.size()) - 1].texIndexs.push_back(w);
				return;
			case XE::OPER_BIND:
				if(m_fboBuffer[i].pClass != this)
					LogStr("Warning:FBO错误的调用关系");
				m_fboBuffer.push_back(XFBOInfo(this,OPER_ATTACHTEX));
				//m_fboBuffer[int(m_fboBuffer.size()) - 1].texIndexs.push_back((*indexs)[0]);
				m_fboBuffer[int(m_fboBuffer.size()) - 1].texIndexs.push_back(w);
				return;
			case XE::OPER_REMOVEFBO:	//不会有的记录
			case XE::OPER_UNBIND:		//不会有的记录
			case XE::OPER_ATTACHTEX:	//不重要的记录
			case XE::OPER_ATTACHTEXS:	//不重要的记录
				m_fboBuffer.pop_back();
				break;
			}
		}
		LogStr("Warning:FBO错误的调用关系");
		break;
	case XE::OPER_ATTACHTEXS:
		for(int i = int(m_fboBuffer.size()) - 1;i >= 0;-- i)
		{
			switch (m_fboBuffer[i].opertion)
			{
			case XE::OPER_USEFBO:
				if(m_fboBuffer[i].pClass != this)
					LogStr("Warning:FBO错误的调用关系");
				m_fboBuffer.push_back(XFBOInfo(this,OPER_ATTACHTEXS));
				m_fboBuffer[int(m_fboBuffer.size()) - 1].texIndexs = *indexs;
				return;
			case XE::OPER_BIND:
				if(m_fboBuffer[i].pClass != this)
					LogStr("Warning:FBO错误的调用关系");
				m_fboBuffer.push_back(XFBOInfo(this,OPER_ATTACHTEXS));
				m_fboBuffer[int(m_fboBuffer.size()) - 1].texIndexs = *indexs;
				return;
			case XE::OPER_REMOVEFBO:	//不会有的记录
			case XE::OPER_UNBIND:		//不会有的记录
			case XE::OPER_ATTACHTEX:	//不重要的记录
			case XE::OPER_ATTACHTEXS:	//不重要的记录
				m_fboBuffer.pop_back();
				break;
			}
		}
		LogStr("Warning:FBO错误的调用关系");
		break;
	}
}
void XFBOInfo::doIt()
{
	switch (opertion)
	{
	case XE::OPER_USEFBO:
		pClass->_useFBO(isNewSize,width,height);
		break;
	case XE::OPER_REMOVEFBO:	//不可能存在的数据
		pClass->_removeFBO();
		break;
	case XE::OPER_BIND:
		pClass->_bind();
		break;
	case XE::OPER_UNBIND:	//不可能存在的数据
		pClass->_unbind();
		break;
	case XE::OPER_ATTACHTEX:
		pClass->_attachTex(texIndexs[0]);
		break;
	case XE::OPER_ATTACHTEXS:
		pClass->_attachTexs(texIndexs);
		break;
	}
}
void XFboBase::reset()
{
	if(m_fboBuffer.size() <= 0) return;	//没有动作直接返回
	for(int i = int(m_fboBuffer.size()) - 1;i >= 0;-- i)
	{
		switch (m_fboBuffer[i].opertion)
		{
		case XE::OPER_USEFBO:	//下面的代码可以优化
			m_fboBuffer[i].pClass->_removeFBO();	//这样会形成记录，这一步不需要记录
			return;
		case XE::OPER_BIND:	//下面的代码可以优化
			return;
		case XE::OPER_REMOVEFBO:	//不会有的记录
		case XE::OPER_UNBIND:		//不会有的记录
		case XE::OPER_ATTACHTEX:	//不重要的记录
		case XE::OPER_ATTACHTEXS:	//不重要的记录
			break;
		}
	}
}
void XFboBase::resumeBuff()
{
	if(m_fboBuffer.size() <= 0) return;	//没有动作直接返回
	int sum = m_fboBuffer.size();
	for(int i = sum - 1;i >= 0;-- i)
	{
		switch (m_fboBuffer[i].opertion)
		{
		case XE::OPER_USEFBO:	//下面的代码可以优化
			for(int j = i;j < sum;++ j)
			{
				m_fboBuffer[j].doIt();
			}
			return;
		case XE::OPER_BIND:	//下面的代码可以优化
			for(int j = i;j < sum;++ j)
			{
				m_fboBuffer[j].doIt();
			}
			return;
		case XE::OPER_REMOVEFBO:	//不会有的记录
		case XE::OPER_UNBIND:		//不会有的记录
		case XE::OPER_ATTACHTEX:	//不重要的记录
		case XE::OPER_ATTACHTEXS:	//不重要的记录
			break;
		}
	}
}
XBool XFBO::init(int w, int h, XColorMode type, bool withRBO, int filter, int MSSum)
{
	if (m_pTextures.size() != 0) return XTrue;
	//检查是否支持FBO
	if (!XEG.getIsFBOSupported()) return XFalse;
	m_MSSum = MSSum;
	if (w % 4 != 0) return false;	//宽度必须要四字节对齐
//	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &m_maxTextureSum);
	//建立贴图
	addOneTexture(w, h, type, filter);
	m_type = type;
	if (type == COLOR_DEPTH)	//这里需要测试是否可以合并这种判断
	{
#ifdef FBO_WITH_EXT
		glGenFramebuffersEXT(1, &m_fboId);					//建立FBO
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fboId);	//绑定FBO
		glGenRenderbuffersEXT(1, &m_rboId);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_rboId);
		//老逻辑(感觉这里存在逻辑问题，需要检查代码，找到修改成这样的原因)
		//if (m_MSSum == 0) glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGBA, w, h);
		//else glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, m_MSSum, GL_RGBA, w, h);
		//glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, m_rboId);//挂接深度渲染点
		//认为正确的代码
		if (m_MSSum == 0) glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, w, h);
		else glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, m_MSSum, GL_DEPTH_COMPONENT, w, h);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_rboId);//挂接深度渲染点

		if (m_MSSum == 0)
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, m_pTextures[0]->m_texture, 0);//挂接贴图		
		else
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D_MULTISAMPLE, m_pTextures[0]->m_texture, 0);
#else
		glGenFramebuffers(1, &m_fboId);					//建立FBO
		glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);	//绑定FBO
		glGenRenderbuffers(1, &m_rboId);
		glBindRenderbuffer(GL_RENDERBUFFER, m_rboId);
		//老逻辑(感觉这里存在逻辑问题，需要检查代码，找到修改成这样的原因)
		//if (m_MSSum == 0) glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, w, h);
		//else glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_MSSum, GL_RGBA, w, h);
		//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_rboId);//挂接深度渲染点
		//认为正确的代码
		if (m_MSSum == 0) glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
		else glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_MSSum, GL_DEPTH_COMPONENT, w, h);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rboId);//挂接深度渲染点

		if (m_MSSum == 0)
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_pTextures[0]->m_texture, 0);//挂接贴图		
		else
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, m_pTextures[0]->m_texture, 0);
#endif
	}
	else
	{
#ifdef FBO_WITH_EXT
		//建立一个framebuffer object，当退出时需要删除
		glGenFramebuffersEXT(1, &m_fboId);					//建立FBO
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fboId);	//绑定FBO
		//glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,GL_RGBA,w,h);
		//使用浮点的GL_RGBA32F_ARB，可以避免残影不消退的问题，尚未搞好
		//glRenderbufferStorageMultisample(GL_RENDERBUFFER,GL_RGBA32F_ARB,GL_DEPTH_STENCIL,w,h);
		//glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,m_fboId);

		if (withRBO)
		{//建立深度渲染buff，不建立对于不需要深度计算的应用貌似也没有问题
			glGenRenderbuffersEXT(1, &m_rboId);
			glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_rboId);
			//老逻辑(感觉这里存在逻辑问题，需要检查代码，找到修改成这样的原因)
			//if (m_MSSum == 0) glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, w, h);
			//else glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, m_MSSum, GL_DEPTH_COMPONENT, w, h);    //启用多重采样
			//glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_rboId);
			//认为正确的代码
			if (m_MSSum == 0) glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGBA, w, h);
			else glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, m_MSSum, GL_RGBA, w, h);    //启用多重采样
			glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, m_rboId);
		}
		if (m_MSSum == 0)
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_pTextures[0]->m_texture, 0);//挂接贴图		
		else
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D_MULTISAMPLE, m_pTextures[0]->m_texture, 0);
#else
		//建立一个framebuffer object，当退出时需要删除
		glGenFramebuffers(1, &m_fboId);					//建立FBO
		glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);	//绑定FBO
		//glRenderbufferStorage(GL_RENDERBUFFER,GL_RGBA,w,h);
		//使用浮点的GL_RGBA32F_ARB，可以避免残影不消退的问题，尚未搞好
		//glRenderbufferStorageMultisample(GL_RENDERBUFFER,GL_RGBA32F_ARB,GL_DEPTH_STENCIL,w,h);
		//glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,m_fboId);

		if (withRBO)
		{//建立深度渲染buff，不建立对于不需要深度计算的应用貌似也没有问题
			glGenRenderbuffers(1, &m_rboId);
			glBindRenderbuffer(GL_RENDERBUFFER, m_rboId);
			//老逻辑(感觉这里存在逻辑问题，需要检查代码，找到修改成这样的原因)
			//if (m_MSSum == 0) glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
			//else glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_MSSum, GL_DEPTH_COMPONENT, w, h);    //启用多重采样
			//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rboId);
			//认为正确的代码
			if (m_MSSum == 0) glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, w, h);
			else glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_MSSum, GL_RGBA, w, h);    //启用多重采样
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_rboId);
		}
		if (m_MSSum == 0)
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pTextures[0]->m_texture, 0);//挂接贴图		
		else
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_pTextures[0]->m_texture, 0);
#endif
	}
	m_zeroTexIndex = 0;

	//如果你没有挂接贴图的话就取消颜色缓存
	//例如当你只需要对贴图进行深度渲染的时候
	//否则glCheckFramebufferStatusEXT将不会准备就绪.
	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);

#ifdef FBO_WITH_EXT
	if (!XGL::getIsFramebufferReady()) return XFalse;
	if (m_MSSum != 0) glEnable(GL_MULTISAMPLE);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
#else
	if (!XGL::getIsFramebufferReadyN()) return XFalse;
	if (m_MSSum != 0) glEnable(GL_MULTISAMPLE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif
	return XTrue;
}
void XFBO::draw(unsigned int index, bool verticalFlip)
{
	if(index >= m_pTextures.size()) return;
	if(m_MSSum > 0)
	{//OF中是使用两个fbo进行倒(下面的逻辑有问题，当不使用FBO嵌套时图像方向不对)
		LogStr("纹理多重采样下建议不要直接使用这个方法而是通过非多重采样的FBO调用getPixelFromFbo中转数据之后在描绘");
		_bind();	//useFBO;
		_attachTex(index);
		_unbind();	//removeFBO;
		//useFBO();
		//attachTex(index);
		//removeFBO();
#ifdef FBO_WITH_EXT
		glBindFramebufferEXT(GL_READ_FRAMEBUFFER, m_fboId); // Make sure your multisampled FBO is the read framebuffer
		if(verticalFlip) glBlitFramebuffer(0, getH(), getW(), 0, 0, 0, getW(), getH(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
		else glBlitFramebuffer(0, 0, getW(), getH(),0,0,getW(), getH(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glBindFramebufferEXT(GL_READ_FRAMEBUFFER, 0); // Make sure your multisampled FBO is the read framebuffer
#else
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fboId); // Make sure your multisampled FBO is the read framebuffer
		if (verticalFlip) glBlitFramebuffer(0, getH(), getW(), 0, 0, 0, getW(), getH(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
		else glBlitFramebuffer(0, 0, getW(), getH(), 0, 0, getW(), getH(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0); // Make sure your multisampled FBO is the read framebuffer
#endif
	}else
	{
		glColor4fv(XFColor::white);
		XRender::drawBlankPlane(XVec2(getW(),getH()),getTexture(index));
	}
}
void XFBO::_bind()
{
#ifdef FBO_WITH_EXT
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,m_fboId);
#else
	glBindFramebuffer(GL_FRAMEBUFFER,m_fboId);
#endif
	if(!XEG.getIsMultiSampleSupported()) return;
//	glDisable(GL_MULTISAMPLE);
	glDisable(GL_POLYGON_SMOOTH);			//开启这里会造成贴图破损
//	glDisable(GL_POINT_SMOOTH);		//开启各种抗锯齿功能
//	if(XEG.getIsLineSmooth()) glDisable(GL_LINE_SMOOTH);
}
void XFBO::_unbind()
{
#ifdef FBO_WITH_EXT
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // unbind
#else
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // unbind
#endif
	if(!XEG.getIsMultiSampleSupported()) return;
//	glEnable(GL_MULTISAMPLE);
	glEnable(GL_POLYGON_SMOOTH);			//开启这里会造成贴图破损
//	glEnable(GL_POINT_SMOOTH);		//开启各种抗锯齿功能
//	if(XEG.getIsLineSmooth()) glEnable(GL_LINE_SMOOTH);
}
void XFBO::_useFBO(bool newSize,int w,int h)
{
	reset();
	if(newSize)
	{
		glMatrixMode(GL_PROJECTION);					//设置当前矩阵模式 （对投影矩阵应用之后的矩阵操作）
		glPushMatrix();
		glLoadIdentity();								//变换坐标系函数
		switch(XEG.m_windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0:
		case WINDOW_ROTATE_MODE_180:
			glOrtho(0,w,0,h,-1,1);
			break;
		case WINDOW_ROTATE_MODE_90:
		case WINDOW_ROTATE_MODE_270:
			glOrtho(w,0,h,0,-1,1);
			break;
		}
		glViewport(0,0,w,h);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		switch(XEG.m_windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0: break;	//do nothing
		case WINDOW_ROTATE_MODE_90: 
		case WINDOW_ROTATE_MODE_270: 
			glTranslatef(w,h,0.0f);
			glRotatef(180.0f,0.0f,0.0f,1.0f);
			break;
		case WINDOW_ROTATE_MODE_180: break;
		}	
	}else
	{
		glMatrixMode(GL_PROJECTION);					//设置当前矩阵模式 （对投影矩阵应用之后的矩阵操作）
		glPushMatrix();
		glLoadIdentity();								//变换坐标系函数
		switch(XEG.m_windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0:
		case WINDOW_ROTATE_MODE_180:
			glOrtho(0.0,XEG.m_windowData.sceneW,0.0,XEG.m_windowData.sceneH,-1.0,1.0);
			break;
		case WINDOW_ROTATE_MODE_90:
		case WINDOW_ROTATE_MODE_270:
			glOrtho(XEG.m_windowData.sceneW,0.0,XEG.m_windowData.sceneH,0.0,-1.0,1.0);
			break;
		}
		glViewport(0,0,XEG.m_windowData.sceneW,XEG.m_windowData.sceneH);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		switch(XEG.m_windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0: break;	//do nothing
		case WINDOW_ROTATE_MODE_90: 
		case WINDOW_ROTATE_MODE_270: 
			glTranslatef(XEG.m_windowData.sceneW,XEG.m_windowData.sceneH,0.0f);
			glRotatef(180.0f,0.0f,0.0f,1.0f);
			break;
		case WINDOW_ROTATE_MODE_180: break;
		}
	}
	_bind();
}
//同时绑定多个FBO
XBool XFBO::attachTexs(int sum,int index,...)
{
	if(sum <= 0 ||
		sum > XEG.getMaxTextureSum()) return XFalse;
	std::vector<int> indexs;
	va_list arg_ptr;
	va_start(arg_ptr,index);
	indexs.push_back(index);
	for(int i = 1;i < sum;++ i)
	{
		indexs.push_back(va_arg(arg_ptr, int));
		if(indexs[i] < 0) return XFalse;
	}
	va_end(arg_ptr);
	return attachTexs(indexs);
}
XBool XFBO::_attachTexs(const std::vector<int> &indexs)
{
#ifdef FBO_WITH_EXT
	//然后开始分别绑定贴图
	for(unsigned int i = 0;i < indexs.size();++ i)
	{
		if(m_MSSum == 0)
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + i, GL_TEXTURE_2D, m_pTextures[indexs[i]]->m_texture, 0);//挂接贴图		
		else
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + i, GL_TEXTURE_2D_MULTISAMPLE, m_pTextures[indexs[i]]->m_texture, 0 );
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
	glDrawBuffers(indexs.size(),buffers);
	return glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) == GL_FRAMEBUFFER_COMPLETE_EXT;
#else
	//然后开始分别绑定贴图
	for (unsigned int i = 0; i < indexs.size(); ++i)
	{
		if (m_MSSum == 0)
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_pTextures[indexs[i]]->m_texture, 0);//挂接贴图		
		else
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D_MULTISAMPLE, m_pTextures[indexs[i]]->m_texture, 0);
	}
	m_zeroTexIndex = indexs[0];
	//m_upTextureSum = sum;
	GLenum buffers[] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3,
		GL_COLOR_ATTACHMENT4,GL_COLOR_ATTACHMENT5,
		GL_COLOR_ATTACHMENT6,GL_COLOR_ATTACHMENT7,
		GL_COLOR_ATTACHMENT8,GL_COLOR_ATTACHMENT9,
		GL_COLOR_ATTACHMENT10,GL_COLOR_ATTACHMENT11,
		GL_COLOR_ATTACHMENT12,GL_COLOR_ATTACHMENT13,
		GL_COLOR_ATTACHMENT14,GL_COLOR_ATTACHMENT15 };
	glDrawBuffers(indexs.size(), buffers);
	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
#endif
}
void XFBO::_removeFBO()
{
	_unbind();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);					//设置当前矩阵模式 （对投影矩阵应用之后的矩阵操作）
	glPopMatrix();
	switch(XEG.m_windowData.rotateMode)
	{
	case WINDOW_ROTATE_MODE_0:
	case WINDOW_ROTATE_MODE_180:
		glViewport(0,0,getWindowWidth(),getWindowHeight());
		break;
	case WINDOW_ROTATE_MODE_90:
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
#ifdef FBO_WITH_EXT
	if(m_fboId != 0)
	{
		glDeleteFramebuffersEXT(1, &m_fboId);
		m_fboId = 0;
	}
	if(m_type == COLOR_DEPTH && m_rboId != 0)
	{
		glDeleteRenderbuffersEXT(1, &m_rboId);
		m_rboId = 0;
	}
#else
	if (m_fboId != 0)
	{
		glDeleteFramebuffers(1, &m_fboId);
		m_fboId = 0;
	}
	if (m_type == COLOR_DEPTH && m_rboId != 0)
	{
		glDeleteRenderbuffers(1, &m_rboId);
		m_rboId = 0;
	}
#endif
}
bool XFBOEx::init(int w,int h,XColorMode type, int MSSum)
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
	case COLOR_RGB_F:colorMode = GL_RGB;break;
	default:	//其他格式不支持该操作
		return false;
	}
#ifdef FBO_WITH_EXT
	glGenFramebuffersEXT(1,&m_fboId);				//建立FBO
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,m_fboId);  

	glGenRenderbuffersEXT(1,&m_rboId);			//建立渲染目标
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,m_rboId);	
	glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, MSSum,colorMode,w,h);  //启用多重采样
	//glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,0);  
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_RENDERBUFFER_EXT,m_rboId);//挂接渲染目标

	glGenRenderbuffersEXT(1,&m_rbdId);			//建立深度渲染目标
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,m_rbdId);  
	glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, MSSum,GL_DEPTH_COMPONENT,w,h);    //启用多重采样
	//glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,0);  
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,GL_DEPTH_ATTACHMENT_EXT,GL_RENDERBUFFER,m_rbdId);  //挂接深度渲染目标

	if(!XGL::getIsFramebufferReady()) return false;
	glEnable(GL_MULTISAMPLE);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);		//FBO建立完成
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,0);  
#else
	glGenFramebuffers(1,&m_fboId);				//建立FBO
	glBindFramebuffer(GL_FRAMEBUFFER,m_fboId);  

	glGenRenderbuffers(1,&m_rboId);			//建立渲染目标
	glBindRenderbuffer(GL_RENDERBUFFER,m_rboId);	
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, MSSum,colorMode,w,h);  //启用多重采样
	//glBindRenderbuffer(GL_RENDERBUFFER,0);  
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_RENDERBUFFER,m_rboId);//挂接渲染目标

	glGenRenderbuffers(1,&m_rbdId);			//建立深度渲染目标
	glBindRenderbuffer(GL_RENDERBUFFER,m_rbdId);  
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, MSSum,GL_DEPTH_COMPONENT,w,h);    //启用多重采样
	//glBindRenderbuffer(GL_RENDERBUFFER,0);  
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,m_rbdId);  //挂接深度渲染目标

	if(!XGL::getIsFramebufferReadyN()) return false;
	glEnable(GL_MULTISAMPLE);
	glBindFramebuffer(GL_FRAMEBUFFER,0);		//FBO建立完成
	glBindRenderbuffer(GL_RENDERBUFFER,0);  
#endif
	m_isInited = true;
	return true;
}
void XFBOEx::_useFBO(bool newSize,int w,int h)
{
	reset();
	if(newSize)
	{
		glMatrixMode(GL_PROJECTION);					//设置当前矩阵模式 （对投影矩阵应用之后的矩阵操作）
		glPushMatrix();
		glLoadIdentity();								//变换坐标系函数
		switch(XEG.m_windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0:
		case WINDOW_ROTATE_MODE_180:
			glOrtho(0.0,w,0.0,h,-1.0,1.0);
			break;
		case WINDOW_ROTATE_MODE_90:
		case WINDOW_ROTATE_MODE_270:
			glOrtho(w,0.0,h,0.0,-1.0,1.0);
			break;
		}
		glViewport(0,0,w,h);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		switch(XEG.m_windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0: break;	//do nothing
		case WINDOW_ROTATE_MODE_90: 
		case WINDOW_ROTATE_MODE_270: 
			glTranslatef(w,h,0.0f);
			glRotatef(180.0f,0.0f,0.0f,1.0f);
			break;
		case WINDOW_ROTATE_MODE_180: break;
		}	
	}else
	{
		glMatrixMode(GL_PROJECTION);					//设置当前矩阵模式 （对投影矩阵应用之后的矩阵操作）
		glPushMatrix();
		glLoadIdentity();								//变换坐标系函数
		switch(XEG.m_windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0:
		case WINDOW_ROTATE_MODE_180:
			glOrtho(0.0,XEG.m_windowData.sceneW,0.0,XEG.m_windowData.sceneH,-1.0,1.0);
			break;
		case WINDOW_ROTATE_MODE_90:
		case WINDOW_ROTATE_MODE_270:
			glOrtho(XEG.m_windowData.sceneW,0.0,XEG.m_windowData.sceneH,0.0,-1.0,1.0);
			break;
		}
		glViewport(0,0,XEG.m_windowData.sceneW,XEG.m_windowData.sceneH);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		switch(XEG.m_windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0: break;	//do nothing
		case WINDOW_ROTATE_MODE_90: 
		case WINDOW_ROTATE_MODE_270: 
			glTranslatef(XEG.m_windowData.sceneW,XEG.m_windowData.sceneH,0.0f);
			glRotatef(180.0f,0.0f,0.0f,1.0f);
			break;
		case WINDOW_ROTATE_MODE_180: break;
		}
	}
	_bind();
}
void XFBOEx::_removeFBO()
{
	_unbind();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);					//设置当前矩阵模式 （对投影矩阵应用之后的矩阵操作）
	glPopMatrix();
	if (gFrameworkData.pFrameWork == NULL) return;
	switch (gFrameworkData.pFrameWork->m_windowData.rotateMode)
	{
	case WINDOW_ROTATE_MODE_0:
	case WINDOW_ROTATE_MODE_180:
		glViewport(0,0,getWindowWidth(),getWindowHeight());
		break;
	case WINDOW_ROTATE_MODE_90:
	case WINDOW_ROTATE_MODE_270:
		glViewport(0,0,getWindowHeight(),getWindowWidth());
		break;
	}
}
namespace XRender
{
	void drawBlankPlane(const XVec2& p, const XVec2& s, unsigned int tex, 
		XShaderGLSL *pShader, const XFColor&color)
	{
		if (tex == 0) return; //无效的图片
		glColor4fv(color);
		if (pShader != NULL) pShader->useShaderEx(tex);
		else
		{
			glActiveTexture(GL_TEXTURE0);
			XGL::EnableTexture2D();
			XGL::BindTexture2D(tex);
		}
		glBegin(GL_QUADS);
		glNormal3f(0, 0, -1);
		glTexCoord2fv(XVec2::zero);
		glVertex2fv(p);

		glNormal3f(0, 0, -1);
		glTexCoord2fv(XVec2::zeroOne);
		glVertex2f(p.x, p.y + s.y);

		glNormal3f(0, 0, -1);
		glTexCoord2fv(XVec2::one);
		glVertex2fv(p + s);

		glNormal3f(0, 0, -1);
		glTexCoord2fv(XVec2::oneZero);
		glVertex2f(p.x + s.x, p.y);
		glEnd();
		if (pShader != NULL) pShader->disShader();
	}
	void drawBlankPlanePlus(const XVec2& p, const XVec2& s, unsigned int tex,
		XShaderGLSL *pShader, const XFColor&color)
	{
		if (tex == 0) return; //无效的图片
		glColor4fv(color);
		if (pShader != NULL)
		{
			pShader->useShaderEx(tex);
		//	pShader->bindDefaults();
		}
		else
		{
			glActiveTexture(GL_TEXTURE0);
			XGL::EnableTexture2D();
			XGL::BindTexture2D(tex);
		}
		glBegin(GL_QUADS);
//		glVertexAttrib3f(NOR_ATT, 0, 0, -1);
//		glVertexAttrib4fv(COL_ATT, XFColor::white);
		glVertexAttrib2fv(TEX_ATT, XVec2::zero);
		glVertexAttrib2fv(POS_ATT, p);

//		glVertexAttrib3f(NOR_ATT, 0, 0, -1);
//		glVertexAttrib4fv(COL_ATT, XFColor::white);
		glVertexAttrib2fv(TEX_ATT, XVec2::zeroOne);
		glVertexAttrib2f(POS_ATT, p.x, p.y + s.y);

//		glVertexAttrib3f(NOR_ATT, 0, 0, -1);
//		glVertexAttrib4fv(COL_ATT, XFColor::white);
		glVertexAttrib2fv(TEX_ATT, XVec2::one);
		glVertexAttrib2fv(POS_ATT, p + s);

//		glVertexAttrib3f(NOR_ATT, 0, 0, -1);
//		glVertexAttrib4fv(COL_ATT, XFColor::white);
		glVertexAttrib2fv(TEX_ATT, XVec2::oneZero);
		glVertexAttrib2f(POS_ATT, p.x + s.x, p.y);
		glEnd();
		if (pShader != NULL) pShader->disShader();
	}
	void drawBlankPlane(const std::vector<XVec2>& pos, unsigned int tex, XShaderGLSL* pShader, const XFColor& color)
	{
		if (pos.size() < 4) return;	//数据数量不够
		glColor4fv(color);
		if (pShader != NULL)
		{
			pShader->useShaderEx(tex);
		}
		else
		{
			glActiveTexture(GL_TEXTURE0);
			XGL::EnableTexture2D();
			XGL::BindTexture2D(tex);
		}
		glBegin(GL_QUADS);
		glTexCoord2fv(XVec2::zero);
		glVertex2fv(pos[0]);
		glTexCoord2fv(XVec2::zeroOne);
		glVertex2fv(pos[1]);
		glTexCoord2fv(XVec2::one);
		glVertex2fv(pos[2]);
		glTexCoord2fv(XVec2::oneZero);
		glVertex2fv(pos[3]);
		glEnd();
		if (pShader != NULL)
			pShader->disShader();
	}
	void drawBlankPlaneEx(const std::vector<XVec2>& pos, unsigned int tex, XShaderGLSL* pShader, const XFColor& color, int subSum)
	{
		if (subSum == 1) return drawBlankPlane(pos, tex, pShader, color);
		if (pos.size() < 4 || subSum < 1) return;	//数据数量不够
		glColor4fv(color);
		if (pShader != NULL)
		{
			pShader->useShaderEx(tex);
		}
		else
		{
			glActiveTexture(GL_TEXTURE0);
			XGL::EnableTexture2D();
			XGL::BindTexture2D(tex);
		}
		float step = 1.0f / static_cast<float>(subSum);
		XVec2 ul, dl, ur, dr, pu, pd, pou, pod;
		float tmpH = 0.0f, tmpW = 0.0f;
		dl = pos[0];
		dr = pos[3];
		XVec2 offsetL = (pos[1] - pos[0]) * step;
		XVec2 offsetR = (pos[2] - pos[3]) * step;
		for (int h = 0; h < subSum; ++h, tmpH += step)
		{
			ul = dl;
			dl += offsetL;
			ur = dr;
			dr += offsetR;
			tmpW = 0.0f;
			pu = ul;
			pd = dl;
			pou = (ur - ul) * step;
			pod = (dr - dl) * step;
			glBegin(GL_TRIANGLE_STRIP);
			for (int w = 0; w <= subSum; ++w, tmpW += step)
			{
				glTexCoord2fv(XVec2(tmpW, tmpH));
				glVertex2fv(pu);

				glTexCoord2fv(XVec2(tmpW, tmpH + step));
				glVertex2fv(pd);
				pu += pou;
				pd += pod;
			}
			glEnd();
		}
		if (pShader != NULL)
			pShader->disShader();
	}
}
#if !WITH_INLINE_FILE
#include "XShaderGLSL.inl"
#endif
}