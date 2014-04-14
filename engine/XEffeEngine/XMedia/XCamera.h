#ifndef _JIA_XCAMERA_
#define _JIA_XCAMERA_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2011.12.10
//--------------------------------
//这是一个普通摄像头视频数据提取的类，基于videoInput
#include "XCameraBasic.h"
#include "XEffeEngine.h"
#include "videoInput.h"
#pragma comment(lib, "videoInput.lib")

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

class _XCamera:public _XCameraBasic
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
	_XBool m_isUp2Down;		//是否上下翻转
	_XBool m_isLeft2Right;	//是否左右翻转
public:
	void setUp2Down(_XBool flag){m_isUp2Down = flag;}
	void setLeft2Right(_XBool flag){m_isLeft2Right = flag;}

	void getData(unsigned char * p) const
	{
		if(!m_isInited) return;
		memcpy(p,m_frameDataBuff,m_buffSize);
	}
	unsigned char * getDataP() const
	{
		if(!m_isInited) return NULL;
		return m_frameDataBuff;
	}
	//int getBuffSize() const {return m_buffSize;}
	_XBool init(_XCameraData &data);
	_XBool upDateFrame();
	//void draw();
	_XCamera()
		:m_frameDataBuff1(NULL)
		,m_frameDataBuff(NULL)
		,m_texDataBuff(NULL)
		,m_isUp2Down(XFalse)
		,m_isLeft2Right(XFalse)
	{}
	~_XCamera(){release();}
	void release();
};

//class _XCameraFactory:public _XCameraBaiscFactory
//{
//	//+++++++++++++++++++++++++++++++++++++++++++
//	//下面需要将其设计为Singleton模式
//protected:
//	_XCameraFactory(){}
//	_XCameraFactory(const _XCameraFactory&);
//	_XCameraFactory &operator= (const _XCameraFactory&);
//	virtual ~_XCameraFactory(){} 
//public:
//	static _XCameraFactory& GetInstance()
//	{
//		static _XCameraFactory m_instance;
//		return m_instance;
//	}
//	//-------------------------------------------
//public:
//	_XCameraBasic * create(_XCameraData & data)
//	{
//		_XCameraBasic *pCamera = NULL;
//		if(data.cameraType == CAMERA_TYPE_NORMAL)
//		{
//			pCamera = createMem<_XCamera>();
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

#endif