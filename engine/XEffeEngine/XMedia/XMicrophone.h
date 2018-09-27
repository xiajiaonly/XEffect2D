#ifndef _JIA_XMICROPHONE_
#define _JIA_XMICROPHONE_
#include "al.h" 
#include "alc.h" 
#include "stdlib.h"
//#include "XBasicFun.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.15
//--------------------------------
//下面是一个关于录音设备的类 基于OpenAL
#pragma comment(lib, "Alut/alut.lib")
#pragma comment(lib, "OpenAL/Win32/OpenAL32.lib")

namespace XE{
enum XCaptureState
{
	CAPTURE_STATE_STOP,		//录音设备停止
	CAPTURE_STATE_START,	//录音设备开始工作
};
class XMicrophone
{
private:
	XBool m_isInited;				//是否进行初始化
	ALCdevice * m_pCaptureDev;		//录音设备的指针
	ALCuint m_frequency;			//音频的频率
	ALCenum m_format;				//音频的格式
	ALCsizei m_buffersize;			//音频的缓存空间大小
	XCaptureState m_captureState;	//录音设备的状态

	unsigned char *m_captureData;	//麦克风录音的内容
	int m_dataBuffMaxSize;			//缓存的最大尺寸
	int m_curDataBuffSize;			//当前有效的缓存尺寸
	int m_byteRate;					//一次采样的字节数
public:
	XCaptureState getCaptureState() const {return m_captureState;}
	void printAllCaptureDevice();	//显示所有的录音设备
	XBool openDevice(const ALCchar *devicename, ALCuint frequency, ALCenum format, ALCsizei buffersize = 20000);	//打开设备
	XBool openDeviceByIndex(int index,ALCuint frequency, ALCenum format, ALCsizei buffersize = 20000);	//按编号打开设备
	void captureStart() const;	//设置开始录音
	void captureStop() const;		//设置停止录音
	//size为采样次数，或者叫音频帧数
	unsigned char * getCaptureData(int * size = NULL);	//读取采样数据
	//一次采样的数据量
	int getByteRate() const {return m_byteRate;}
	void release();
	XMicrophone()
		:m_pCaptureDev(NULL)
		,m_captureState(CAPTURE_STATE_STOP)
		,m_isInited(XFalse)
		,m_captureData(NULL)
		,m_byteRate(1)
	{}
	~XMicrophone(){release();}
};
#if WITH_INLINE_FILE
#include "XMicrophone.inl"
#endif
}
#endif