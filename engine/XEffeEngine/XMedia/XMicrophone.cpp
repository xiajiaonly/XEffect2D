#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XMicrophone.h"
namespace XE{
void XMicrophone::printAllCaptureDevice()
{
	LogStr("可以使用的录音设备有:"); 
	const ALCchar *ptr = alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER); 
	while (*ptr)
	{ 
		LogNull("   %s\n", ptr); 
		ptr += strlen(ptr) + 1; 
	}
}
XBool XMicrophone::openDevice(const ALCchar *devicename,ALCuint frequency,ALCenum format,ALCsizei buffersize)
{
	if(m_isInited) return XFalse;		//防止重复打开设备
	m_frequency = frequency;		//音频的频率
	m_format = format;				//音频的格式
	m_buffersize = buffersize;		//音频的缓存空间大小
	LogStr("打开默认的录音设备:"); 
	m_pCaptureDev = alcCaptureOpenDevice(devicename,frequency,format,buffersize); 
	if(m_pCaptureDev == NULL) 
	{  
		LogStr("   打开录音设备失败!"); 
		return XFalse; 
	} 
	//const ALCchar * devices = alcGetString(m_pCaptureDev, ALC_CAPTURE_DEVICE_SPECIFIER); 
	LogNull("   已经打开录音设备 %s\n", alcGetString(m_pCaptureDev, ALC_CAPTURE_DEVICE_SPECIFIER));

	m_dataBuffMaxSize = 10000;	//默认的最大数据大小
	m_captureData = XMem::createArrayMem<unsigned char>(m_dataBuffMaxSize);
	if(m_captureData == NULL) return XFalse;
	m_curDataBuffSize = 0;

	switch(format)
	{
	case AL_FORMAT_MONO8:m_byteRate = 1;break;
	case AL_FORMAT_MONO16:m_byteRate = 2;break;
	case AL_FORMAT_STEREO8:m_byteRate = 2;break;
	case AL_FORMAT_STEREO16:m_byteRate = 4;break;
	}

	m_isInited = XTrue;		//标记设备已经打开
	return XTrue;
}
XBool XMicrophone::openDeviceByIndex(int index,ALCuint frequency, ALCenum format, ALCsizei buffersize)	//按编号打开设备
{
	if(m_isInited) return XFalse;		//防止重复打开设备
	//寻找到指定的设备
	std::string deviceName = "";
	const ALCchar *ptr = alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER); 
	while (*ptr)
	{ 
		if(index == 0)
		{
			deviceName = ptr;
			break;
		}
		ptr += strlen(ptr) + 1; 
		-- index;
	}
	if(deviceName.size() <= 0)
	{//没有找到指定的设备
		LogStr("没有指定的设备!"); 
		return XFalse;
	}
	//打开指定设备
	m_frequency = frequency;		//音频的频率
	m_format = format;				//音频的格式
	m_buffersize = buffersize;		//音频的缓存空间大小
	m_pCaptureDev = alcCaptureOpenDevice(deviceName.c_str(),frequency,format,buffersize); 
	if(m_pCaptureDev == NULL) 
	{  
		LogStr("   打开录音设备失败!"); 
		return XFalse; 
	} 
	//const ALCchar devices = alcGetString(m_pCaptureDev, ALC_CAPTURE_DEVICE_SPECIFIER); 
	LogNull("   已经打开录音设备 %s\n",alcGetString(m_pCaptureDev, ALC_CAPTURE_DEVICE_SPECIFIER));

	m_dataBuffMaxSize = 10000;	//默认的最大数据大小
	m_captureData = XMem::createArrayMem<unsigned char>(m_dataBuffMaxSize);
	if(m_captureData == NULL) return XFalse;
	m_curDataBuffSize = 0;

	switch(format)
	{
	case AL_FORMAT_MONO8:m_byteRate = 1;break;
	case AL_FORMAT_MONO16:m_byteRate = 2;break;
	case AL_FORMAT_STEREO8:m_byteRate = 2;break;
	case AL_FORMAT_STEREO16:m_byteRate = 4;break;
	}

	m_isInited = XTrue;		//标记设备已经打开
	return XTrue;
}
unsigned char * XMicrophone::getCaptureData(int * size)
{
	int ret = 0;
	if(size == NULL) size = & ret; 
	(* size) = 0;
	if(!m_isInited) return NULL;
	alcGetIntegerv(m_pCaptureDev,ALC_CAPTURE_SAMPLES,1,size); 
	if((*size) > 0)	//有录音采样数据 
	{//下面取出采样数据
		m_curDataBuffSize = (*size) * m_byteRate;
		if(m_curDataBuffSize <= m_dataBuffMaxSize)
		{
			alcCaptureSamples(m_pCaptureDev,m_captureData,(*size));	//取出采样数据
			return m_captureData;	//返回有读取到采样数据
		}else
		{//这里需要重新分配内存空间
			XMem::XDELETE_ARRAY(m_captureData);
			m_dataBuffMaxSize = m_curDataBuffSize;
			m_captureData = XMem::createArrayMem<unsigned char>(m_dataBuffMaxSize);//新建内存空间
			if(m_captureData == NULL) 
			{
				m_dataBuffMaxSize = 0;
				return NULL;	
			}
			alcCaptureSamples(m_pCaptureDev,m_captureData,(*size));	//取出采样数据
			return m_captureData;	//返回有读取到采样数据
		}
		//printf("%d\n",(*size));
	}
	return NULL;	//返回没有读取到采样数据
}
#if !WITH_INLINE_FILE
#include "XMicrophone.inl"
#endif
}