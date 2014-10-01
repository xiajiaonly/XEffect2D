#ifndef _JIA_XHIKCAMERA_
#define _JIA_XHIKCAMERA_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.15
//--------------------------------
#include "XCameraBasic.h"

#if SUPPORT_FOR_HIK
#include "XEffeEngine.h"
#include "windows.h"
#include "HIK/HCNetSDK.h"
#include "HIK/plaympeg4.h"

#pragma comment(lib, "../../engine/lib/HIK/PlayCtrl.lib")
#pragma comment(lib, "../../engine/lib/HIK/HCNetSDK.lib")

//接下来将其封装成单子系统
class _XHIKCamera:public _XCameraBasic
{
private:
	LONG m_lUserID;
	NET_DVR_DEVICEINFO_V30 m_structDeviceInfo;
	LONG m_lRealPlayHandle;

	unsigned char *m_dataRGB;		//保存像素数据的指针,这个数据不对外开放
	_XBool m_haveNewFrame;			//是否有新的帧
	_XBool isNewFrame()				//是否有新的帧
	{
		_XBool ret = m_haveNewFrame;
		m_haveNewFrame = XFalse;
		return ret;
	}

	unsigned char *m_dataRGBOut;	//这个是对外的接口
public:
	_XBool init(_XCameraData &data);
	void getData(unsigned char * p) const
	{
		if(!m_isInited) return;
		memcpy(p,m_dataRGBOut,m_buffSize);
	}
	unsigned char * getDataP() const
	{
		if(!m_isInited) return NULL;
		return m_dataRGBOut;
	}
	//int getBuffSize() const {return m_buffSize;}
	_XBool upDateFrame();
	//void draw();

	_XHIKCamera()
		:m_haveNewFrame(XFalse)
		,m_dataRGB(NULL)
	{}
	~_XHIKCamera() {release();}
	void release();
};

//class _XHIKCameraFactory:public _XCameraBaiscFactory
//{
//	//+++++++++++++++++++++++++++++++++++++++++++
//	//下面需要将其设计为Singleton模式
//protected:
//	_XHIKCameraFactory(){}
//	_XHIKCameraFactory(const _XHIKCameraFactory&);
//	_XHIKCameraFactory &operator= (const _XHIKCameraFactory&);
//	virtual ~_XHIKCameraFactory(){} 
//public:
//	static _XHIKCameraFactory& GetInstance()
//	{
//		static _XHIKCameraFactory m_instance;
//		return m_instance;
//	}
//	//-------------------------------------------
//public:
//	_XCameraBasic * create(_XCameraData & data)
//	{
//		_XCameraBasic *pCamera = NULL;
//		if(data.cameraType == CAMERA_TYPE_HIK)
//		{
//			pCamera = createMem<_XHIKCamera>();
//			if(pCamera != NULL) 
//			{
//				if(pCamera->init(data) == 0)
//				{//初始化失败
//					XDELETE(pCamera);
//				}
//			}
//		}
//		return pCamera;
//	}
//};

#endif	//SUPPORT_FOR_HIK

#endif //_JIA_XHIKCAMERA_