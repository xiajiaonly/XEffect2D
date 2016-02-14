#ifndef _JIA_XAUDIOOUTPUT_
#define _JIA_XAUDIOOUTPUT_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.3
//--------------------------------
//这是一个音频输出的类
#include "../XMemory.h"
#include "XFfmpeg.h"
#include "XSoundCommon.h"
//#include "../XThread.h"
#include "../XCritical.h"
namespace XE{
class XAudioOutput
{
protected:
    XAudioOutput()
		:m_audioBuff(NULL)
		,m_buffSize(0)
		,m_curUsage(0)
		,m_isStart(false)
		,m_pSwrContext(NULL)
		,m_tmpBuffer(NULL)
		,m_withConversion(false)
		{}
    XAudioOutput(const XAudioOutput&);
	XAudioOutput &operator= (const XAudioOutput&);
	virtual ~XAudioOutput(){release();}
public:
    static XAudioOutput& GetInstance()
	{
		static XAudioOutput m_instance;
		return m_instance;
	}
private:
	unsigned char *m_audioBuff;	//用于放音频数据
	int m_buffSize;	//缓存的大小
	int m_curUsage;	//当前的使用量
	bool m_isStart;
	XCritical m_mutex;
	static void audioCallBack(void *userdata,Uint8 *stream,int len);	//声音的回调函数
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是为转换定义的参数
	SwrContext *m_pSwrContext;
	unsigned char *m_tmpBuffer;	//数据转换的临时存储空间
	bool m_withConversion;	//是否有转换
	int m_conversionPerFrame;	//转换时的采样转化率
	float m_conversionRate;	//转换率
	//--------------------------------------------------
	bool insertDataX(const unsigned char *data,int dataLen);	//插入数据
public:
	void release();
	AVSampleFormat getSampleFormat()const{return AV_SAMPLE_FMT_S16;}
	bool getIsStart()const{return m_isStart;}
	bool start();	//开始回放
	int getBuffUsage()const{return m_curUsage;}
	float getCurCanPlayTime()const;//返回当前缓存中的数据可以在播放多长的时间	
	bool insertData(const unsigned char *data,int dataLen);	//插入数据
	bool stop();	//停止回放
	void setConversion(int channelSum,AVSampleFormat format,int sampleRate);	//设置转换参数
	void useConversion(bool flag){m_withConversion = flag;}	//启用转换
	bool getIsConversion()const{return m_withConversion;}
};
#define XAudioOut XAudioOutput::GetInstance()
#if WITH_INLINE_FILE
#include "XAudioOutput.inl"
#endif
}

#endif