//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XMicrophone.h"
#include "stdio.h"
#include "string.h"

void _XMicrophone::printAllCaptureDevice()
{
	const ALCchar * devices; 
	const ALCchar * ptr; 
	printf("可以使用的录音设备有:\n"); 
	devices = alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER); 
	ptr = devices; 
	while (*ptr)
	{ 
		printf("   %s\n", ptr); 
		ptr += strlen(ptr) + 1; 
	}
}
_XBool _XMicrophone::openDevice(const ALCchar *devicename,ALCuint frequency,ALCenum format,ALCsizei buffersize)
{
	if(m_isInited) return XFalse;		//防止重复打开设备
	const ALCchar * devices = NULL; 
	m_frequency = frequency;		//音频的频率
	m_format = format;				//音频的格式
	m_buffersize = buffersize;		//音频的缓存空间大小
	printf("打开默认的录音设备:\n"); 
	m_pCaptureDev = alcCaptureOpenDevice(devicename,frequency,format,buffersize); 
	if(m_pCaptureDev == NULL) 
	{  
		printf("   打开录音设备失败!\n"); 
		return XFalse; 
	} 
	devices = alcGetString(m_pCaptureDev, ALC_CAPTURE_DEVICE_SPECIFIER); 
	printf("   已经打开录音设备 %s\n", devices);

	m_dataBuffMaxSize = 10000;	//默认的最大数据大小
	m_captureData = createArrayMem<unsigned char>(m_dataBuffMaxSize);
	if(m_captureData == NULL) return XFalse;
	m_nowDataBuffSize = 0;

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
unsigned char * _XMicrophone::getCaptureData(int * size)
{
	int ret = 0;
	if(size == NULL) size = & ret; 
	(* size) = 0;
	if(!m_isInited) return NULL;
	alcGetIntegerv(m_pCaptureDev,ALC_CAPTURE_SAMPLES,1,size); 
	if((*size) > 0)	//有录音采样数据 
	{//下面取出采样数据
		m_nowDataBuffSize = (*size) << 2;
		if(m_nowDataBuffSize <= m_dataBuffMaxSize)
		{
			alcCaptureSamples(m_pCaptureDev,m_captureData,(*size));	//取出采样数据
			return m_captureData;	//返回有读取到采样数据
		}else
		{//这里需要重新分配内存空间
			XDELETE_ARRAY(m_captureData);
			m_dataBuffMaxSize = m_nowDataBuffSize;
			m_captureData = createArrayMem<unsigned char>(m_dataBuffMaxSize);//新建内存空间
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