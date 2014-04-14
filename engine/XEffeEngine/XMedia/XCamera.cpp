//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XCamera.h"

_XBool _XCamera::init(_XCameraData &data)
{
	if(m_isInited) return XTrue;

	m_deviceSum = m_VI.listDevices();	
	if(m_deviceSum <= 0) return XFalse;					//没有找到设备
	m_VI.setIdealFramerate(data.deviceOrder,data.fps);	//设置视频捕捉率，默认为30	
//	m_VI.setUseCallback(XTrue);
	//if(m_VI.setupDevice(data.deviceOrder,data.w,data.h,VI_S_VIDEO) == XFalse) return XFalse; 
//	m_VI.setAutoReconnectOnFreeze(data.deviceOrder,true,10);
//	m_VI.setFormat(data.deviceOrder,VI_NTSC_M);
	if(m_VI.setupDevice(data.deviceOrder,data.w,data.h) == XFalse) return XFalse; 

	m_cameraWidth = m_VI.getWidth(data.deviceOrder);
	m_cameraHeight = m_VI.getHeight(data.deviceOrder);
	m_buffSize = m_VI.getSize(data.deviceOrder);
	m_deviceOrder = data.deviceOrder;

	m_frameDataBuff = createArrayMem<unsigned char>(m_buffSize);
	if(m_frameDataBuff == NULL) return 0;
	m_frameDataBuff1 = createArrayMem<unsigned char>(m_buffSize);
	if(m_frameDataBuff1 == NULL)
	{
		XDELETE_ARRAY(m_frameDataBuff);
		return XFalse;
	}
	if(m_cameraSprite.init(m_cameraWidth,m_cameraHeight) == 0) return XFalse;
	//判断贴图尺寸
	if(isNPOT(m_cameraWidth,m_cameraHeight))
	{
		m_cameraTexWidth = getMinWellSize2n(m_cameraWidth);
		m_cameraTexHeight = getMinWellSize2n(m_cameraHeight);
		m_texDataBuff = createArrayMem<unsigned char>(m_cameraTexWidth * m_cameraTexHeight * 3);
		if(m_texDataBuff == NULL)
		{
			XDELETE_ARRAY(m_frameDataBuff);
			XDELETE_ARRAY(m_frameDataBuff1);
			return XFalse;
		}
		memset(m_texDataBuff,0,m_cameraTexHeight * m_cameraTexHeight * 3);
		m_px = (m_cameraTexWidth - m_cameraWidth) >> 1;
		m_py = (m_cameraTexHeight - m_cameraHeight) >> 1;
	}else
	{
		m_cameraTexWidth = m_cameraWidth;
		m_cameraTexHeight = m_cameraHeight;
	}
	//建立贴图
	m_cameraTex.createTexture(m_cameraTexWidth,m_cameraTexHeight,COLOR_RGB);

	m_isInited = XTrue;
	return XTrue;
}
void _XCamera::release()
{
	if(!m_isInited) return;

	XDELETE_ARRAY(m_frameDataBuff1);
	XDELETE_ARRAY(m_frameDataBuff);
	XDELETE_ARRAY(m_texDataBuff);
	m_VI.stopDevice(m_deviceOrder);
	m_isInited = XFalse;
}
_XBool _XCamera::upDateFrame()
{
	if(!m_isInited) return XFalse;
	if(m_VI.isFrameNew(m_deviceOrder) && m_isWork)	//如果摄像头有图像更新则作下面的操作
	{
		if(m_isLeft2Right)
		{//下面进行左右翻转操作
			if(!m_isUp2Down) m_VI.getPixels(m_deviceOrder,m_frameDataBuff1,XTrue,XTrue); 	//不进行上下翻转
			else m_VI.getPixels(m_deviceOrder,m_frameDataBuff1,XTrue,XFalse);	//上下翻转
			//下面进行左右翻转
			int tempH = 0;
			int tempH1 = 0;
			for(int h = 0;h < m_cameraHeight;++ h)
			{
				tempH = h * m_cameraWidth * 3;
				tempH1 = tempH + (m_cameraWidth - 1) * 3;
				for(int w = 0,k = 0;w < m_cameraWidth;++ w)
				{
					*(m_frameDataBuff + tempH + k) = *(m_frameDataBuff1 + tempH1 - k);
					*(m_frameDataBuff + tempH + k + 1) = *(m_frameDataBuff1 + tempH1 - k + 1);
					*(m_frameDataBuff + tempH + k + 2) = *(m_frameDataBuff1 + tempH1 - k + 2);
					k += 3;
				}
			}
		}else
		{
			if(!m_isUp2Down) m_VI.getPixels(m_deviceOrder,m_frameDataBuff,XTrue,XTrue); 	//填充数据到指定空间对于OpenGL	
			else m_VI.getPixels(m_deviceOrder,m_frameDataBuff,XTrue,XFalse);
		}

		if(m_cameraTexWidth == m_cameraWidth && m_cameraTexHeight == m_cameraHeight)
		{
			m_cameraTex.updateTexture(m_frameDataBuff);
		}else
		{//进行图像扩展
			int TW = m_cameraTexWidth * 3;
			int CW = m_cameraWidth * 3;
			for(int i = m_py;i < m_cameraHeight + m_py;++ i)
			{
				memcpy(m_texDataBuff + i * TW + m_px * 3,m_frameDataBuff + (i - m_py) * CW,CW);
			}
			m_cameraTex.updateTexture(m_texDataBuff);
		}
		return XTrue;
	}
	return XFalse;
}