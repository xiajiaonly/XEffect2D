//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2011.12.10
//--------------------------------
#include "XCamera.h"

int _XCamera::init(int width,int hight,int deviceOrder,int FPS)
{
	if(m_isInited != 0) return 1;

	m_deviceSum = m_VI.listDevices();	
	m_VI.setIdealFramerate(deviceOrder,FPS);	//设置视频捕捉率，默认为30	
	m_VI.setUseCallback(true);
	if(m_VI.setupDevice(deviceOrder,width,hight) == false) return 0; 

	m_cameraWidth = m_VI.getWidth(deviceOrder);
	m_cameraHight = m_VI.getHeight(deviceOrder);
	m_buffSize = m_VI.getSize(deviceOrder);
	m_deviceOrder = deviceOrder;
	try
	{
		m_frameDataBuff = new unsigned char[m_buffSize];	//用于存储摄像头的图片资源
		if(m_frameDataBuff == NULL) return 0;
	}catch(...)
	{
		return 0;
	}

	if(m_cameraSprite.init(m_cameraWidth,m_cameraHight) == 0) return 0;
	//判断贴图尺寸
	if(isNPOT(m_cameraWidth,m_cameraHight))
	{
		m_cameraTexW = (int)powf(2.0, ceilf(logf((float)m_cameraWidth)/logf(2.0f)));
		m_cameraTexH = (int)powf(2.0, ceilf(logf((float)m_cameraHight)/logf(2.0f)));
		try
		{
			m_texDataBuff = new unsigned char[m_cameraTexW * m_cameraTexH * 3];
			if(m_texDataBuff == NULL) return 0;
		}catch(...)
		{
			return 0;
		}
		memset(m_texDataBuff,0,m_cameraTexW * m_cameraTexH * 3);
		m_px = (m_cameraTexW - m_cameraWidth) / 2;
		m_py = (m_cameraTexH - m_cameraHight) / 2;
	}else
	{
		m_cameraTexW = m_cameraWidth;
		m_cameraTexH = m_cameraHight;
	}

	//绑定设备贴图
	glGenTextures(1,&m_cameraTex);
	glBindTexture(GL_TEXTURE_2D, m_cameraTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_cameraTexW, m_cameraTexH, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	m_isInited = 1;
	return 1;
}

void _XCamera::release()
{
	if(m_isInited == 0) return;

	XDELETE_ARRAY(m_frameDataBuff);
	XDELETE_ARRAY(m_texDataBuff);
	m_VI.stopDevice(m_deviceOrder);
	m_isInited = 0;
}

int _XCamera::upDateFrame()
{
	if(m_isInited == 0) return 0;
	if(m_VI.isFrameNew(m_deviceOrder) && m_isWork != 0)	//如果摄像头有图像更新则作下面的操作
	{
		m_VI.getPixels(m_deviceOrder,m_frameDataBuff, true, true); 	//填充数据到指定空间对于OpenGL	

		if(m_cameraTexW == m_cameraWidth && m_cameraTexH == m_cameraHight)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, m_cameraTex);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 
				m_cameraTexW, m_cameraTexH, GL_RGB,//GL_LUMINANCE, 
				GL_UNSIGNED_BYTE, m_frameDataBuff);
			glDisable(GL_TEXTURE_2D);
		}else
		{
			int TW = m_cameraTexW * 3;
			int CW = m_cameraWidth * 3;
			for(int i = m_py;i < m_cameraHight + m_py;++ i)
			{
				memcpy(m_texDataBuff + i * TW + m_px * 3,m_frameDataBuff + (i - m_py) * CW,CW);
			}

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, m_cameraTex);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 
				m_cameraTexW, m_cameraTexH, GL_RGB,//GL_LUMINANCE, 
				GL_UNSIGNED_BYTE, m_texDataBuff);
			glDisable(GL_TEXTURE_2D);
		}

		return 1;
	}
	return 0;
}

void _XCamera::draw()
{
	if(m_isInited == 0) return;
	m_cameraSprite.draw(&m_cameraTex);
}
