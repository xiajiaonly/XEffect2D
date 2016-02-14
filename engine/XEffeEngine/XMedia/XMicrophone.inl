INLINE void XMicrophone::captureStart() const	//设置开始录音
{
	if(m_captureState == CAPTURE_STATE_STOP
		&& m_isInited) alcCaptureStart(m_pCaptureDev); 
}
INLINE void XMicrophone::captureStop() const		//设置停止录音
{
	if(m_captureState == CAPTURE_STATE_START
		&& m_isInited) alcCaptureStop(m_pCaptureDev); 
}
INLINE void XMicrophone::release()
{
	if(!m_isInited) return;
	alcMakeContextCurrent(NULL);
	alcCaptureCloseDevice(m_pCaptureDev); 
	XMem::XDELETE_ARRAY(m_captureData);

	m_isInited = XFalse;
}