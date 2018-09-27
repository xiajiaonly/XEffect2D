#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XCamera.h"
namespace XE{
XBool XCamera::init(XPixelsInputInfo &data)
{
	if (m_isInited) return XTrue;
	if (data.pixelsInputType != TYPE_CAM_NORMAL) return XFalse;
	m_needReset = true;

	if (data.isSilence)
		m_VI.setVerbose(false);		//关闭后台信息输出
//	m_VI.setComMultiThreaded(true);	//使用多线程的方式

	m_deviceSum = m_VI.listDevices(data.isSilence);
	if (m_deviceSum <= 0) return XFalse;					//没有找到设备
	m_VI.setIdealFramerate(data.deviceOrder, data.fps);	//设置视频捕捉率，默认为30	
//	m_VI.setUseCallback(XTrue);
	//if(!m_VI.setupDevice(data.deviceOrder,data.w,data.h,VI_S_VIDEO)) return XFalse; 
	//m_VI.setAutoReconnectOnFreeze(data.deviceOrder,true,data.fps);
	//m_VI.setFormat(data.deviceOrder,VI_NTSC_M);
	if (!data.withDefaultFormat)
		m_VI.setRequestedMediaSubType(VI_MEDIASUBTYPE_MJPG);
	if (!m_VI.setupDevice(data.deviceOrder, data.w, data.h)) return XFalse;

	m_pixelsWidth = m_VI.getWidth(data.deviceOrder);
	m_pixelsHeight = m_VI.getHeight(data.deviceOrder);
	if (m_pixelsWidth != data.w || m_pixelsHeight != data.h)
	{
		m_VI.stopDevice(data.deviceOrder);
		return XFalse;
	}
	m_buffSize = m_VI.getSize(data.deviceOrder);
	m_deviceOrder = data.deviceOrder;

	m_frameDataBuff = XMem::createArrayMem<unsigned char>(m_buffSize);
	memset(m_frameDataBuff, 0, m_buffSize);
	if (m_frameDataBuff == NULL) return XFalse;
	m_frameDataBuff1 = XMem::createArrayMem<unsigned char>(m_buffSize);
	if (m_frameDataBuff1 == NULL)
	{
		XMem::XDELETE_ARRAY(m_frameDataBuff);
		return XFalse;
	}
#ifdef WITH_GL_TEXTRUE
	if (!m_pixelsSprite.init(m_pixelsWidth, m_pixelsHeight, 0))
	{
		XMem::XDELETE_ARRAY(m_frameDataBuff);
		XMem::XDELETE_ARRAY(m_frameDataBuff1);
		return XFalse;
	}
#endif
	//判断贴图尺寸
	//if(XMath::isNPOT(m_cameraWidth,m_cameraHeight))
	//{
	//	m_cameraTexWidth = XMath::getMinWellSize2n(m_cameraWidth);
	//	m_cameraTexHeight = XMath::getMinWellSize2n(m_cameraHeight);
	//	m_texDataBuff = XMem::createArrayMem<unsigned char>(m_cameraTexWidth * m_cameraTexHeight * 3);
	//	if(m_texDataBuff == NULL)
	//	{
	//		XMem::XDELETE_ARRAY(m_frameDataBuff);
	//		XMem::XDELETE_ARRAY(m_frameDataBuff1);
	//		return XFalse;
	//	}
	//	memset(m_texDataBuff,0,m_cameraTexHeight * m_cameraTexHeight * 3);
	//	m_px = (m_cameraTexWidth - m_cameraWidth) >> 1;
	//	m_py = (m_cameraTexHeight - m_cameraHeight) >> 1;
	//}else
	//{
	//	m_cameraTexWidth = m_cameraWidth;
	//	m_cameraTexHeight = m_cameraHeight;
	//}
	//建立贴图
	m_colorMode = COLOR_RGB;
#ifdef WITH_GL_TEXTRUE
	m_pixelsTex.createTexture(m_pixelsWidth, m_pixelsHeight, m_colorMode);
	if (data.needReset) m_pixelsTex.reset();
#endif
	m_curInfo = data;
	m_isInited = XTrue;
	return XTrue;
}
void XCamera::release()
{
	if(!m_isInited) return;

	XMem::XDELETE_ARRAY(m_frameDataBuff1);
	XMem::XDELETE_ARRAY(m_frameDataBuff);
	XMem::XDELETE_ARRAY(m_texDataBuff);
	m_VI.stopDevice(m_deviceOrder);
//	m_cameraSprite.release();
//	m_cameraTex.release();
	m_isInited = XFalse;
	m_needReset = false;
}
XBool XCamera::updateFrame()
{
	if (m_needReset)
		m_updateTimer += XEG.getLastFrameTime() * 0.001f;
	if (!m_isInited) return XFalse;
	if (!m_VI.isFrameNew(m_deviceOrder)) return false;	//如果摄像头有图像更新则作下面的操作
	m_updateTimer = 0.0f;
	if (!m_isWork) return XFalse;
	if (m_isLeft2Right)
	{//下面进行左右翻转操作
		if (!m_isUp2Down) m_VI.getPixels(m_deviceOrder, m_frameDataBuff1, m_r2b, XTrue); 	//不进行上下翻转
		else m_VI.getPixels(m_deviceOrder, m_frameDataBuff1, m_r2b, XFalse);	//上下翻转
		//下面进行左右翻转
		int tempH = 0;
		int tempH1 = 0;
		for (int h = 0; h < m_pixelsHeight; ++h)
		{
			tempH = h * m_pixelsWidth * 3;
			tempH1 = tempH + (m_pixelsWidth - 1) * 3;
			for (int w = 0, k = 0; w < m_pixelsWidth; ++w)
			{
				memcpy(m_frameDataBuff + tempH + k, m_frameDataBuff1 + tempH1 - k, 3);
				//	*(m_frameDataBuff + tempH + k) = *(m_frameDataBuff1 + tempH1 - k);
				//	*(m_frameDataBuff + tempH + k + 1) = *(m_frameDataBuff1 + tempH1 - k + 1);
				//	*(m_frameDataBuff + tempH + k + 2) = *(m_frameDataBuff1 + tempH1 - k + 2);
				k += 3;
			}
		}
	}
	else
	{
		if (!m_isUp2Down) m_VI.getPixels(m_deviceOrder, m_frameDataBuff, m_r2b, XTrue); 	//填充数据到指定空间对于OpenGL	
		else m_VI.getPixels(m_deviceOrder, m_frameDataBuff, m_r2b, XFalse);
	}

#ifdef WITH_GL_TEXTRUE
//	if(m_cameraTexWidth == m_cameraWidth && m_cameraTexHeight == m_cameraHeight)
//	{
		if(m_r2b) m_pixelsTex.updateTexture(m_frameDataBuff);
		else m_pixelsTex.updateTextureR2B(m_frameDataBuff);
//	}else
//	{//进行图像扩展
//		int TW = m_cameraTexWidth * 3;
//		int CW = m_cameraWidth * 3;
//		for(int i = m_py;i < m_cameraHeight + m_py;++ i)
//		{
//			memcpy(m_texDataBuff + i * TW + m_px * 3,m_frameDataBuff + (i - m_py) * CW,CW);
//		}
//		m_cameraTex.updateTexture(m_texDataBuff);
//	}
#endif
	return XTrue;
}
bool XCamera::reset()
{
	LogStr("设备故障,重新启动设备!");
	release();
	return init(m_curInfo);
}
}