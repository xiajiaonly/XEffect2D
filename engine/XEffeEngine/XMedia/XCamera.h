#ifndef _JIA_XCAMERA_
#define _JIA_XCAMERA_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2011.12.10
//--------------------------------
//这是一个普通摄像头视频数据提取的类，基于videoInput
#include "XCameraBasic.h"
#include "videoInput.h"
//注意在包含opencv的时候不需要再包含这个库，否则会造成重复包含，因为opencv中已经包含这个库
//#ifdef _DEBUG
//#ifdef _WIN64
//#pragma comment(lib, "videoInputD64.lib")
//#else
//#pragma comment(lib, "videoInputD.lib")
//#endif
//#else
//#ifdef _WIN64
//#pragma comment(lib, "videoInput64.lib")	//与opencv_videoio300.lib有冲突
//#else
//#pragma comment(lib, "videoInput.lib")	//与opencv_videoio300.lib有冲突
//#endif
//#endif

#pragma comment(linker, "/NODEFAULTLIB:atlthunk.lib")
#include <atlbase.h>

#if (_ATL_VER < 0x0700)
namespace ATL
{
  inline void * __stdcall __AllocStdCallThunk()
  {
  return ::HeapAlloc(::GetProcessHeap(), 0, sizeof(_stdcallthunk));
  }

  inline void __stdcall __FreeStdCallThunk(void *p)
  {
  ::HeapFree(::GetProcessHeap(), 0, p);
  }
};
#endif //(_ATL_VER < 0x0700)
namespace XE{
class XCamera :public XPixelsInputBasic
{
private:
	videoInput m_VI;		//摄像头的设备指针
	int m_deviceSum;		//摄像头设备的数量
	int m_deviceOrder;		//摄像头设备的编号
	unsigned char *m_frameDataBuff1;//这个是用于左右翻转的数据
	unsigned char *m_frameDataBuff;	//摄像头每帧的数据
	unsigned char *m_texDataBuff;	//贴图的数据
	int m_px;
	int m_py;
	XBool m_isUp2Down;		//是否上下翻转
	XBool m_isLeft2Right;	//是否左右翻转
	XBool m_r2b;			//是否需要红绿色反转，如果确认不需要识别颜色的话，可以不必要这么做
public:
	void setR2B(XBool f) { m_r2b = f; }
	bool getNeedR2B()const { return m_r2b; }
	void setUp2Down(XBool flag) { m_isUp2Down = flag; }
	void setLeft2Right(XBool flag) { m_isLeft2Right = flag; }

	void getData(unsigned char * p) const
	{
		if (!m_isInited) return;
		memcpy(p, m_frameDataBuff, m_buffSize);
	}
	unsigned char * getDataP() const
	{
		if (!m_isInited) return NULL;
		return m_frameDataBuff;
	}
	//int getBuffSize() const {return m_buffSize;}
	XBool init(XPixelsInputInfo &data);
	XBool updateFrame();
	bool resart()
	{
		if (!m_isInited) return false;
		return m_VI.restartDevice(m_deviceOrder);
	}
	//void draw();
	XCamera()
		:m_frameDataBuff1(NULL)
		, m_frameDataBuff(NULL)
		, m_texDataBuff(NULL)
		, m_isUp2Down(XFalse)
		, m_isLeft2Right(XFalse)
		, m_updateTimer(0.0f)
		, m_needReset(false)
		, m_px(0)
		, m_py(0)
		, m_r2b(XTrue)
	{}
	~XCamera(){release();}
	void release();
	void showDeviceSetting()
	{
		if(m_isInited) m_VI.showSettingsWindow(m_deviceOrder);
	}
	//+++++++++++++++++++++++++++++++++++++++++++++++
	//下面是关于保护而定义的相关方法
	bool reset();
	bool needReset()
	{
		if(!m_needReset) return false;
		return m_updateTimer > 1.0f;	//如果有1s数据都没有更新则认为设备出现故障
	}
private:
	float m_updateTimer;	//更新的时间
	XPixelsInputInfo m_curInfo;
	bool m_needReset;	//是否需要重新启动
	//-----------------------------------------------
};

//class XCameraFactory:public XCameraBaiscFactory
//{
//	//+++++++++++++++++++++++++++++++++++++++++++
//	//下面需要将其设计为Singleton模式
//protected:
//	XCameraFactory(){}
//	XCameraFactory(const XCameraFactory&);
//	XCameraFactory &operator= (const XCameraFactory&);
//	virtual ~XCameraFactory(){} 
//public:
//	static XCameraFactory& GetInstance()
//	{
//		static XCameraFactory m_instance;
//		return m_instance;
//	}
//	//-------------------------------------------
//public:
//	XCameraBasic * create(XCameraInfo & data)
//	{
//		XCameraBasic *pCamera = NULL;
//		if(data.cameraType == CAMERA_TYPE_NORMAL)
//		{
//			pCamera = XMem::createMem<XCamera>();
//			if(pCamera != NULL) 
//			{
//				if(pCamera->init(data) == 0)
//				{//初始化失败
//					XMem::XDELETE(pCamera);
//				}
//			}
//		}
//		return pCamera;
//	}
//};
}
#endif