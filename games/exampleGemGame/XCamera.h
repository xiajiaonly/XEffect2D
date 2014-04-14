#ifndef _JIA_XCAMERA_
#define _JIA_XCAMERA_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2011.12.10
//--------------------------------
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

class _XCamera
{
private:
	char m_isInited;	//是否初始化

	videoInput m_VI;		//摄像头的设备指针
	int m_deviceSum;		//摄像头设备的数量
	int m_cameraWidth;		//摄像头的宽度
	int m_cameraHight;		//摄像头的高度
	int m_cameraTexW;		//摄像头贴图宽度
	int m_cameraTexH;		//摄像头贴图高度
	int m_buffSize;			//摄像头数据的大小
	int m_deviceOrder;		//摄像头设备的编号
	unsigned char *m_frameDataBuff;	//摄像头每帧的数据
	unsigned char *m_texDataBuff;	//贴图的数据
	int m_px;
	int m_py;
	GLuint m_cameraTex;		//摄像头贴图的指针

	char m_isWork;	//设置设备是否工作
public:
	_XSprite m_cameraSprite;	//摄像机的贴图，贴图位置以及相关属性可以修改这个成员来获得

	int init(int width,int hight,int deviceOrder = 0,int FPS = 30);
	void release();
	int upDateFrame();
	void draw();
	void getData(unsigned char * p) const
	{
		if(m_isInited == 0) return;
		memcpy(p,m_frameDataBuff,m_buffSize);
	}
	int getBuffSize() const
	{
		return m_buffSize;
	}
	void setWork()
	{
		m_isWork = 1;
	}
	void setStop()
	{
		m_isWork = 0;
	}

	_XCamera()
	:m_isInited(0)
	,m_frameDataBuff(NULL)
	,m_texDataBuff(NULL)
	,m_isWork(1)
	{
	}
};

#endif