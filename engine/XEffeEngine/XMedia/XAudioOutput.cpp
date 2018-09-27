#include "XStdHead.h"
#include "XAudioOutput.h"
namespace XE{
void XAudioOutput::audioCallBack(void *userData, Uint8 *stream, int len)
{
	XAudioOutput &pPar = *(XAudioOutput *)userData;
	//下面向其中填充数据
	if (pPar.m_audioBuff.getUsage() == 0)
	{//没有数据
		memset(stream, 0, len);
	}
	else
	{
		pPar.m_mutex.Lock();
		if (pPar.m_audioBuff.getUsage() >= len)
		{//填充满数据
			pPar.m_audioBuff.popData(stream, len);
		}
		else
		{//不够的补零
			memcpy(stream, pPar.m_audioBuff.getBuffer(), pPar.m_audioBuff.getUsage());
			//剩下的部分补0
			memset(stream + pPar.m_audioBuff.getUsage(), 0, len - pPar.m_audioBuff.getUsage());
			pPar.m_audioBuff.clear();
		}
		pPar.m_mutex.Unlock();
	}
}
void XAudioOutput::setConversion(int channelSum,AVSampleFormat format,int sampleRate)	//设置转换参数
{
//	if(channelSum == XEE::audioChannel && format == getSampleFormat() && 
//		sampleRate == XEE::audioSampleRate) return;	//没必要进行转码
	if(m_pSwrContext != NULL)
		swr_free(&m_pSwrContext);
	m_pSwrContext = swr_alloc();
	if(m_pSwrContext == NULL) return;
	swr_alloc_set_opts(m_pSwrContext,av_get_default_channel_layout(XEG.getAudioChannelSum()),getSampleFormat(),XEG.getAudioSampleRate(),
		av_get_default_channel_layout(channelSum),format,sampleRate,0,NULL);
	switch(format)
	{
	case AV_SAMPLE_FMT_U8:
	case AV_SAMPLE_FMT_U8P:
		m_conversionPerFrame = channelSum;
		break;
	case AV_SAMPLE_FMT_S16:
	case AV_SAMPLE_FMT_S16P:
		m_conversionPerFrame = channelSum << 1;
		break;
	}
	m_conversionRate = (float)(sampleRate * m_conversionPerFrame) / (float)(XEG.getAudioSampleRate() * XEG.getAudioChannelSum());

	if(swr_init(m_pSwrContext) < 0)
		LogStr("swr_init() fail");
}
bool XAudioOutput::insertData(const unsigned char *data,int dataLen)	//插入数据
{
	if(dataLen <= 0 || data == NULL) return false;
	if(m_withConversion && m_pSwrContext != NULL)
	{
		#define MAX_TMP_BUFF_SIZE (4096)
		if(m_tmpBuffer == NULL)
		{
			m_tmpBuffer = XMem::createArrayMem<unsigned char>(MAX_TMP_BUFF_SIZE);
			if(m_tmpBuffer == NULL) return false;
		}
		//下面进行转码并保存
		int pos = 0;
		int sampleSum = (MAX_TMP_BUFF_SIZE >> 1) * m_conversionRate / m_conversionPerFrame;
		while(true)
		{
			uint8_t *out[] = {m_tmpBuffer}; 
			const uint8_t *in[] = {data + pos,data + dataLen / 2 + pos};//对输入数据进行切分，没有问题
			if((dataLen - pos) / m_conversionPerFrame < sampleSum)
			{
				int ret = swr_convert(m_pSwrContext,out,MAX_TMP_BUFF_SIZE / m_conversionPerFrame,in,(dataLen - pos) / m_conversionPerFrame);  
				if(!insertDataX(m_tmpBuffer,ret * XEG.getAudioChannelSum() * m_conversionPerFrame)) return false;
				break;
			}else
			{
				int ret = swr_convert(m_pSwrContext,out,MAX_TMP_BUFF_SIZE / m_conversionPerFrame,in,sampleSum);  
				assert(ret * XEG.getAudioChannelSum() * m_conversionPerFrame <= MAX_TMP_BUFF_SIZE);
				if(!insertDataX(m_tmpBuffer,ret * XEG.getAudioChannelSum() * m_conversionPerFrame)) return false;
				pos += sampleSum * m_conversionPerFrame;
			}
		}
		return true;
	}else
		return insertDataX(data,dataLen);
}
#if !WITH_INLINE_FILE
#include "XAudioOutput.inl"
#endif
}