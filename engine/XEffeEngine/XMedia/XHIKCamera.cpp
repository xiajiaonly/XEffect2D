//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XHIKCamera.h"
#include "stdio.h"

#if SUPPORT_FOR_HIK
#include "../XColorSpace.h"

//解码的回调函数
unsigned char* pOutRGB;	//输出的RGB颜色
_XBool *haveNewFrame;		//结合接口
//int *width = NULL;
//int *height = NULL;
void CALLBACK DecCBFun(long/*nPort*/,char * pBuf,long/*nSize*/, FRAME_INFO * pFrameInfo,long/*nReserved1*/,long) 
{
	if(pFrameInfo->nType == T_YV12) YUVToRGB((unsigned char *)pBuf,pOutRGB,pFrameInfo->nWidth,pFrameInfo->nHeight);
	//YUVToRGB((unsigned char *)pBuf,pOutRGB,*width,*height);
	* haveNewFrame = XTrue;
}
//实时解码的回调函数
LONG lPort;
void CALLBACK g_RealDataCallBack_V30(LONG/*lRealHandle*/, DWORD dwDataType, BYTE *pBuffer,DWORD dwBufSize,DWORD/*dwUser*/)
{
	switch(dwDataType)
	{
	case NET_DVR_SYSHEAD:					//系统头
		if(!PlayM4_GetPort(&lPort)) break;	//获取播放库未使用的通道号
		if(dwBufSize > 0)
		{
			if(!PlayM4_SetStreamOpenMode(lPort,STREAME_REALTIME)) break;		//设置实时流播放模式
			if(!PlayM4_OpenStream(lPort,pBuffer,dwBufSize,1024*1024)) break;	//打开流接口,1024 * 1024的数据何来？
			PlayM4_SetDecCallBack(lPort,DecCBFun);
			if(!PlayM4_SetDecCBStream(lPort,1)) break;
			if(!PlayM4_Play(lPort,NULL)) break;//播放开始
		}//T_YV12
	case NET_DVR_STREAMDATA:   //码流数据
		if (dwBufSize > 0 && lPort != -1)
		{
			if(!PlayM4_InputData(lPort,pBuffer,dwBufSize)) break;
		}
	}
}
void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG/*lUserID*/, LONG/*lHandle*/, void *)/*pUser*/
{
	switch(dwType) 
	{
	case EXCEPTION_RECONNECT:    //预览时重连
		printf("----------reconnect--------\n");
		break;
	default:
		break;
	}
}
_XBool _XHIKCamera::upDateFrame()
{
	if(!m_isInited) return XFalse;
	if(isNewFrame() && m_isWork != 0)
	{//更新贴图数据
		memcpy(m_dataRGBOut,m_dataRGB,m_buffSize);
		m_cameraTex.updateTexture(m_dataRGBOut);
		return XTrue;
	}
	return XFalse;
}
void _XHIKCamera::release()
{
	if(!m_isInited) return;
	PlayM4_Stop(lPort);
	PlayM4_CloseFile(lPort);
	NET_DVR_Logout(m_lUserID);
	NET_DVR_Cleanup();

	XDELETE_ARRAY(m_dataRGB);
	XDELETE_ARRAY(m_dataRGBOut);
	//下面需要删除资源
	m_isInited = XFalse;
}
_XBool _XHIKCamera::init(_XCameraData &data)
{
	if(m_isInited) return XTrue;
	m_cameraWidth = data.w;
	m_cameraHeight = data.h;
	//width = &m_cameraWidth;
	//height = & m_cameraHeight;
	m_buffSize = m_cameraWidth * m_cameraHeight * 3;

	m_cameraTexWidth = m_cameraWidth;
	m_cameraTexHeight = m_cameraHeight;
	//初始化摄像头并连接设备
	NET_DVR_Init();	//初始化
	m_lUserID = NET_DVR_Login_V30(data.ipAddress,data.port,data.userName,data.pwd,&m_structDeviceInfo);
	if(m_lUserID < 0)	
	{//连接设备失败
		printf("Login error, %d\n", NET_DVR_GetLastError());
		NET_DVR_Cleanup();
		return XFalse;
	}
	//下面开始连接回调函数并分配内存空间
	m_dataRGB = createArrayMem<unsigned char>(m_buffSize);	//分配内存空间
	if(m_dataRGB == NULL) return XFalse;
	memset(m_dataRGB,0,m_buffSize);
	m_dataRGBOut = createArrayMem<unsigned char>(m_buffSize);
	if(m_dataRGBOut == NULL) return XFalse;

	haveNewFrame = &m_haveNewFrame;
	pOutRGB = m_dataRGB;

	NET_DVR_SetExceptionCallBack_V30(0,NULL,g_ExceptionCallBack,NULL);

	NET_DVR_CLIENTINFO ClientInfo = {0};
	ClientInfo.hPlayWnd = NULL;         //需要SDK解码时句柄设为有效值，仅取流不解码时可设为空
	ClientInfo.lChannel = 0;			//预览通道号
	ClientInfo.lLinkMode = 0;			//最高位(31)为0表示主码流，为1表示子码流0～30位表示连接方式：0－TCP方式；1－UDP方式；2－多播方式；3－RTP方式;
	ClientInfo.sMultiCastIP = NULL;		//多播地址，需要多播预览时配置

	//BOOL bPreviewBlock = XFalse;			//请求码流过程是否阻塞，0：否，1：是
	m_lRealPlayHandle = NET_DVR_RealPlay_V30(m_lUserID,&ClientInfo,NULL,NULL,XFalse);
	if(m_lRealPlayHandle < 0)
	{
		printf("NET_DVR_RealPlay_V30 error\n");
		NET_DVR_Logout(m_lUserID);
		NET_DVR_Cleanup();
		return XFalse;
	}

	if(!NET_DVR_SetRealDataCallBack(m_lRealPlayHandle,g_RealDataCallBack_V30,0))
	{
		printf("NET_DVR_SetRealDataCallBack error\n");
	}

	m_cameraTex.createTexture(m_cameraWidth,m_cameraHeight,COLOR_RGB);	//不允许进行2的n次方扩展，这回造成不兼容
	m_cameraSprite.init(m_cameraWidth,m_cameraHeight,0);

	m_haveNewFrame = XFalse;
	m_isInited = XTrue;
	return XTrue;
}

//void _XHIKCamera::init()
//{
//	pYUV = new unsigned char[m_width * m_height * 1.5];
//	pRGB = new unsigned char[m_width * m_height * 3];
//	pRGB_old = new unsigned char[m_width * m_height * 3];
//	pRGB_old2 = new unsigned char[m_width * m_height * 3];
//	for(int i = 0; i < m_width * m_height * 3 ; i ++)
//	{
//		pRGB_old2[i] = -1;	//蛋疼
//	}
//	memcpy(pRGB,pRGB_old2,m_width * m_height * 3);
//	memcpy(pRGB_old,pRGB_old2,m_width * m_height * 3);
//
//	transferOK = XFalse;
//	//设置回调函数
//	NET_DVR_SetExceptionCallBack_V30(0,NULL,g_ExceptionCallBack,NULL);
//	NET_DVR_CLIENTINFO ClientInfo = {0};
//	ClientInfo.hPlayWnd = NULL;         //需要SDK解码时句柄设为有效值，仅取流不解码时可设为空
//	ClientInfo.lChannel = 0;       //预览通道号
//	ClientInfo.lLinkMode = 0;       //最高位(31)为0表示主码流，为1表示子码流0～30位表示连接方式：0－TCP方式；1－UDP方式；2－多播方式；3－RTP方式;
//	ClientInfo.sMultiCastIP = NULL;   //多播地址，需要多播预览时配置
//
//	BOOL bPreviewBlock = XFalse;       //请求码流过程是否阻塞，0：否，1：是
//	m_lRealPlayHandle = NET_DVR_RealPlay_V30(m_lUserID,&ClientInfo,NULL,NULL,0);
//	if(m_lRealPlayHandle < 0)
//	{
//		printf("NET_DVR_RealPlay_V30 error\n");
//		NET_DVR_Logout(m_lUserID);
//		NET_DVR_Cleanup();
//		return;
//	}
//
//	if(!NET_DVR_SetRealDataCallBack(m_lRealPlayHandle,g_RealDataCallBack_V30,0))
//	{
//		printf("NET_DVR_SetRealDataCallBack error\n");
//	}
//
//	//DWORD dwReturn = 0;
//	//memset(&m_CcdParam, 0, sizeof(m_CcdParam));
//	//m_CcdParam.dwSize = sizeof(m_CcdParam);
//	//if (!NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_CCDPARAMCFG, -1, &m_CcdParam, sizeof(m_CcdParam), &dwReturn))
//	//{
//	//}else
//	//{
//	//}
//	//cout <<(int)m_CcdParam.struWhiteBalance.byWhiteBalanceMode<<endl;
//	//m_CcdParam.struWhiteBalance.byWhiteBalanceMode = 1;
//	//if (!NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_CCDPARAMCFG, -1, &m_CcdParam, sizeof(m_CcdParam)))
//	//{
//	//}else
//	//{
//	//}
//
//	//if (!NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_CCDPARAMCFG, -1, &m_CcdParam, sizeof(m_CcdParam), &dwReturn))
//	//{
//	//}else
//	//{
//	//}
//	//cout <<(int)m_CcdParam.struWhiteBalance.byWhiteBalanceMode<<endl;
//}

#endif //SUPPORT_FOR_HIK