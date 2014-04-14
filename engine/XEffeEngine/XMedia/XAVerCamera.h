#ifndef _JIA_XAVERCAMERA_
#define _JIA_XAVERCAMERA_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2014.1.1
//--------------------------------
#include "XMedia/XCameraBasic.h"

#include "AVer/AVerCapAPI.h"
#pragma comment(lib,"../lib/AVer/AVerCapAPI.lib")

class _XAVerCamera:public _XCameraBasic
{
private:
	HANDLE m_hHDCaptureDevice;		//摄像头的设备指针
	int m_deviceSum;				//摄像头设备的数量
	int m_deviceOrder;				//摄像头设备的编号
	unsigned char *m_frameDataBuff1;//这个是用于左右翻转的数据
	unsigned char *m_frameDataBuff;	//摄像头每帧的数据
	unsigned char *m_texDataBuff;	//贴图的数据
	unsigned char *m_pixelsData;
	int m_px;
	int m_py;
	_XBool m_isUp2Down;		//是否上下翻转
	_XBool m_isLeft2Right;	//是否左右翻转
	friend BOOL WINAPI avCameraCB(VIDEO_SAMPLE_INFO VideoInfo, BYTE *pbData,LONG lLength, __int64 tRefTime, LONG lUserData);
	_XBool m_isNewFrame;

	_XBool setPixels(const _XCameraData &data);
public:
	void setUp2Down(_XBool flag){m_isUp2Down = flag;}
	void setLeft2Right(_XBool flag){m_isLeft2Right = flag;}

	void getData(unsigned char * p) const
	{
		if(!m_isInited) return;
		memcpy(p,m_pixelsData,m_buffSize);
	}
	unsigned char * getDataP() const
	{
		if(!m_isInited) return NULL;
		return m_pixelsData;
	}
	//int getBuffSize() const {return m_buffSize;}
	_XBool init(_XCameraData &data);
	_XBool upDateFrame();
	//void draw();
	_XAVerCamera()
		:m_frameDataBuff1(NULL)
		,m_frameDataBuff(NULL)
		,m_texDataBuff(NULL)
		,m_isUp2Down(XFalse)
		,m_isLeft2Right(XFalse)
		,m_isNewFrame(XFalse)
	{
	}
	~_XAVerCamera()
	{
		release();
	}
	void release();
};

#endif