INLINE void XAVStream::closeAudio()		//关闭音频流
{
	avcodec_close(m_audioST->codec);
	//av_free(m_audioOutbuf);
	XMem::XDELETE_ARRAY(m_audioOutbuf);
	XMem::XDELETE_ARRAY(m_audioTempBuff);
}
INLINE void XAVStream::closeVideo()		//关闭视频流
{
	avcodec_close(m_videoST->codec);
	//av_free(m_videoOutbuf);
	XMem::XDELETE_ARRAY(m_videoOutbuf);
}
INLINE int XAVStream::imgConvert(AVPicture *dst,const AVPicture *src,int src_height)
{
	if(m_pSwsContext == NULL) return 0;
	return sws_scale(m_pSwsContext, src->data, src->linesize,  
        0,src_height, dst->data, dst->linesize); 
}
INLINE XBool XAVStream::audioNeedData() const
{
	int len = 0;
	switch(m_audioInfo.sampleFormat)
	{
	case AV_SAMPLE_FMT_U8:len = 1;break;
	case AV_SAMPLE_FMT_S16:len = 2;break;
	case AV_SAMPLE_FMT_S32:len = 4;break;
	case AV_SAMPLE_FMT_FLT:len = 4;break;
	case AV_SAMPLE_FMT_U8P:len = 1;break;
	case AV_SAMPLE_FMT_S16P:len = 2;break;
	case AV_SAMPLE_FMT_S32P:len = 4;break;
	case AV_SAMPLE_FMT_FLTP:len = 4;break;
	}
	return m_videoFrameIndex * m_audioInfo.sampleRate * m_audioInfo.channelSum * len / m_videoFrameRate > m_inputAudioDataSum;
}
INLINE void XAVStream::addFrameAudio(XMicrophone & mic)
{
	int ret = 0;
	unsigned char * audioData = mic.getCaptureData(&ret);
	if(ret != 0 && audioNeedData()) addFrameAudio(audioData,ret * mic.getByteRate());//读取音频数据，并写入到音频文件中
}
INLINE void XAVStream::addFrameAudio(XAudioStream & aStr)
{
	while(audioNeedData())
	{
		if(aStr.getAFrame()) addFrameAudio(aStr.getData(),aStr.getDataLen());
		//Sleep(1);
	}
}
INLINE void XAVStream::setStop()
{
	if(m_audioDeviceType == AUDIO_DEVICE_TYPE_MIC)
		((XMicrophone *)m_audioDevice)->captureStop();
	m_isStop = XTrue;
}
INLINE void XAVStream::setStart()
{
	if(m_audioDeviceType == AUDIO_DEVICE_TYPE_MIC)
		((XMicrophone *)m_audioDevice)->captureStart();
	m_isStop = XFalse;
}