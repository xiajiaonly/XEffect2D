//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XAVerCamera.h"

BOOL WINAPI avCameraCB(VIDEO_SAMPLE_INFO VideoInfo, BYTE *pbData,LONG lLength, __int64 tRefTime, LONG lUserData)
{
	_XAVerCamera *pPar = (_XAVerCamera *)lUserData;
	if(pPar->m_isInited)
	{
		if(pPar->m_isWork)
		{
			pPar->m_isNewFrame = XTrue;
			memcpy(pPar->m_frameDataBuff,pbData,lLength);
		}
	}
	return TRUE;
}
_XBool _XAVerCamera::setPixels(const _XCameraData &data)
{
	DWORD sum = VIDEORESOLUTION_640X480;
	if(data.w == 640 && data.h == 480) sum = VIDEORESOLUTION_640X480;else
	if(data.w == 704 && data.h == 576) sum = VIDEORESOLUTION_704X576;else
	if(data.w == 720 && data.h == 480) sum = VIDEORESOLUTION_720X480;else
	if(data.w == 720 && data.h == 480) sum = VIDEORESOLUTION_720X480P;else
	if(data.w == 720 && data.h == 576) sum = VIDEORESOLUTION_720X576;else
	if(data.w == 720 && data.h == 576) sum = VIDEORESOLUTION_720X576P;else
	if(data.w == 1280 && data.h == 720) sum = VIDEORESOLUTION_1280X720P;else
	if(data.w == 1920 && data.h == 1080) sum = VIDEORESOLUTION_1920X1080;else
	if(data.w == 1024 && data.h == 768) sum = VIDEORESOLUTION_1024X768P;else
	if(data.w == 1280 && data.h == 800) sum = VIDEORESOLUTION_1280X800P;else
	if(data.w == 1280 && data.h == 1024) sum = VIDEORESOLUTION_1280X1024P;else
	if(data.w == 1600 && data.h == 1200) sum = VIDEORESOLUTION_1600X1200P;else
	if(data.w == 1680 && data.h == 1050) sum = VIDEORESOLUTION_1680X1050P;else
	if(data.w == 1920 && data.h == 1080) sum = VIDEORESOLUTION_1920X1080P;else
	if(data.w == 1920 && data.h == 1080) sum = VIDEORESOLUTION_1920X1080P_24FPS;else
	if(data.w == 640 && data.h == 480) sum = VIDEORESOLUTION_640X480P;else
	if(data.w == 800 && data.h == 600) sum = VIDEORESOLUTION_800X600P;else
	if(data.w == 1280 && data.h == 768) sum = VIDEORESOLUTION_1280X768P;else
	if(data.w == 1360 && data.h == 768) sum = VIDEORESOLUTION_1360X768P;else
	if(data.w == 160 && data.h == 120) sum = VIDEORESOLUTION_160X120;else
	if(data.w == 176 && data.h == 144) sum = VIDEORESOLUTION_176X144;else
	if(data.w == 240 && data.h == 176) sum = VIDEORESOLUTION_240X176;else
	if(data.w == 240 && data.h == 180) sum = VIDEORESOLUTION_240X180;else
	if(data.w == 320 && data.h == 240) sum = VIDEORESOLUTION_320X240;else
	if(data.w == 352 && data.h == 240) sum = VIDEORESOLUTION_352X240;else
	if(data.w == 352 && data.h == 288) sum = VIDEORESOLUTION_352X288;else
	if(data.w == 640 && data.h == 240) sum = VIDEORESOLUTION_640X240;else
	if(data.w == 640 && data.h == 288) sum = VIDEORESOLUTION_640X288;else
	if(data.w == 720 && data.h == 240) sum = VIDEORESOLUTION_720X240;else
	if(data.w == 720 && data.h == 288) sum = VIDEORESOLUTION_720X288;else
	if(data.w == 80 && data.h == 60) sum = VIDEORESOLUTION_80X60;else
	if(data.w == 88 && data.h == 72) sum = VIDEORESOLUTION_88X72;else
	if(data.w == 128 && data.h == 96) sum = VIDEORESOLUTION_128X96;else
	if(data.w == 1152 && data.h == 864) sum = VIDEORESOLUTION_1152X864P;else
	if(data.w == 1280 && data.h == 960) sum = VIDEORESOLUTION_1280X960P;else
	if(data.w == 180 && data.h == 120) sum = VIDEORESOLUTION_180X120;else
	if(data.w == 180 && data.h == 144) sum = VIDEORESOLUTION_180X144;else
	if(data.w == 360 && data.h == 240) sum = VIDEORESOLUTION_360X240;else
	if(data.w == 360 && data.h == 288) sum = VIDEORESOLUTION_360X288;
	if(AVerSetVideoResolution(m_hHDCaptureDevice,sum) != CAP_EC_SUCCESS) return XFalse;
	return XTrue;
}
_XBool _XAVerCamera::init(_XCameraData &data)
{
	if(m_isInited) return XTrue;
	DWORD sum;
	AVerGetDeviceNum(&sum);
	m_deviceSum = sum;
	if(m_deviceSum <= 0) return XFalse;					//没有找到设备
	if(AVerCreateCaptureObjectEx(data.deviceOrder,DEVICETYPE_HD,NULL,&m_hHDCaptureDevice) != CAP_EC_SUCCESS) return XFalse;
//	if(AVerSetVideoSource(m_hHDCaptureDevice,VIDEOSOURCE_HDMI) != CAP_EC_SUCCESS) return XFalse;
//	if(AVerSetVideoFormat(m_hHDCaptureDevice,VIDEOFORMAT_PAL) != CAP_EC_SUCCESS) return XFalse;
	if(!setPixels(data)) return XFalse;	
//	sum = data.fps;
//	if(AVerSetVideoInputFrameRate(m_hHDCaptureDevice,sum) != CAP_EC_SUCCESS) return XFalse;
	if(AVerStartStreaming(m_hHDCaptureDevice) != CAP_EC_SUCCESS) return XFalse;

	////if(AVerGetVideoResolution(m_hHDCaptureDevice,&sum) != CAP_EC_SUCCESS) return XFalse;
	//INPUT_VIDEO_INFO inputInfo;
	//inputInfo.dwVersion = 1;
	//LONG ret = AVerGetVideoInfo(m_hHDCaptureDevice,&inputInfo);
	//if(ret != CAP_EC_SUCCESS) return XFalse;
	//m_cameraWidth = inputInfo.dwWidth;
	//m_cameraHeight = inputInfo.dwHeight;
	//m_buffSize = m_cameraWidth * m_cameraHeight * 3;
	//m_deviceOrder = data.deviceOrder;
	//if(m_cameraWidth != data.w || m_cameraHeight != data.h)
	//{
	//	if(AVerStopStreaming(m_hHDCaptureDevice) != CAP_EC_SUCCESS) return XFalse;
	//	data.w = m_cameraWidth;
	//	data.h = m_cameraHeight;
	//	if(!setPixels(data)) return XFalse;
	//	if(AVerStartStreaming(m_hHDCaptureDevice) != CAP_EC_SUCCESS) return XFalse;
	//}
	m_cameraWidth = data.w;
	m_cameraHeight = data.h;
	m_buffSize = m_cameraWidth * m_cameraHeight * 3;
	m_deviceOrder = data.deviceOrder;

	VIDEO_CAPTURE_INFO info;
	info.dwCaptureType = CT_SEQUENCE_FRAME;
	info.dwSaveType = ST_CALLBACK_RGB24;
	info.bOverlayMix = FALSE;
	info.dwDurationMode = DURATION_COUNT;
	info.dwDuration = 0xffffffff;
	info.lpCallback = avCameraCB;
	info.lCallbackUserData = (long)this;
	if(AVerCaptureVideoSequenceStart(m_hHDCaptureDevice,info) != CAP_EC_SUCCESS) return XFalse;

	m_frameDataBuff = createArrayMem<unsigned char>(m_buffSize);
	if(m_frameDataBuff == NULL) return XFalse;
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
void _XAVerCamera::release()
{
	if(!m_isInited) return;
	AVerCaptureVideoSequenceStop(m_hHDCaptureDevice);
	AVerStopStreaming(m_hHDCaptureDevice);
	AVerDeleteCaptureObject(m_hHDCaptureDevice);

	XDELETE_ARRAY(m_frameDataBuff1);
	XDELETE_ARRAY(m_frameDataBuff);
	XDELETE_ARRAY(m_texDataBuff);
	m_isInited = XFalse;
}
_XBool _XAVerCamera::upDateFrame()
{
	if(!m_isInited) return XFalse;
	if(m_isNewFrame)
	{//更新数据
		m_pixelsData = m_frameDataBuff;
		if(m_isLeft2Right)
		{//下面进行左右翻转操作
			if(m_isUp2Down)
			{//上下左右翻转
				m_pixelsData = m_frameDataBuff1;
				int tempH = 0;
				int tempH1 = 0;
				for(int h = 0;h < m_cameraHeight;++ h)
				{
					tempH = (m_cameraHeight - 1 - h) * m_cameraWidth * 3;
					tempH1 = h * m_cameraWidth * 3 + (m_cameraWidth - 1) * 3;
					for(int w = 0,k = 0;w < m_cameraWidth;++ w)
					{
						*(m_pixelsData + tempH + k) = *(m_frameDataBuff + tempH1 - k);
						*(m_pixelsData + tempH + k + 1) = *(m_frameDataBuff + tempH1 - k + 1);
						*(m_pixelsData + tempH + k + 2) = *(m_frameDataBuff + tempH1 - k + 2);
						k += 3;
					}
				}
			}else
			{//左右翻转
				m_pixelsData = m_frameDataBuff1;
				int tempH = 0;
				int tempH1 = 0;
				for(int h = 0;h < m_cameraHeight;++ h)
				{
					tempH = h * m_cameraWidth * 3;
					tempH1 = tempH + (m_cameraWidth - 1) * 3;
					for(int w = 0,k = 0;w < m_cameraWidth;++ w)
					{
						*(m_pixelsData + tempH + k) = *(m_frameDataBuff + tempH1 - k);
						*(m_pixelsData + tempH + k + 1) = *(m_frameDataBuff + tempH1 - k + 1);
						*(m_pixelsData + tempH + k + 2) = *(m_frameDataBuff + tempH1 - k + 2);
						k += 3;
					}
				}
			}
		}else
		if(m_isUp2Down)
		{//上下翻转
			m_pixelsData = m_frameDataBuff1;
			int dataSum = m_cameraWidth * 3;
			for(int h = 0;h < m_cameraHeight;++ h)
			{
				memcpy(m_pixelsData + (m_cameraHeight - 1 - h) * dataSum,m_frameDataBuff + h * dataSum,m_cameraWidth * 3);
			}
		}

		if(m_cameraTexWidth == m_cameraWidth && m_cameraTexHeight == m_cameraHeight)
		{
			m_cameraTex.updateTextureR2B(m_pixelsData);
		}else
		{//进行图像扩展
			int TW = m_cameraTexWidth * 3;
			int CW = m_cameraWidth * 3;
			for(int i = m_py;i < m_cameraHeight + m_py;++ i)
			{
				memcpy(m_texDataBuff + i * TW + m_px * 3,m_pixelsData + (i - m_py) * CW,CW);
			}
			m_cameraTex.updateTextureR2B(m_texDataBuff);
		}
	}
	return m_isNewFrame;
}