#ifndef _JIA_XLOADING_
#define _JIA_XLOADING_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:       2011.12.10
//--------------------------------
#include "XEffeEngine.h"
#include "XOSDefine.h"

#ifdef XEE_OS_WINDOWS
#include "windows.h"
namespace XE{
class XLoading
{
private:
	XResPos m_resoursePosition;	//资源位置 0:外部 1:内部
public:
	float m_loadPresent;		//载入进度
	char m_isShow;			//是否显示载入内容
	char m_isEnd;			//0没有结束 1资源载入完成 2结束

//	HDC m_hDC;				//绘图句柄
	XSprite m_loadingBG;		//载入画面背景
	XSprite m_gameTitle;
	XSprite m_loadingText;
	XSprite m_loadingTextLight;
	XMoveData m_gameTitleMoveData;
	XProgress m_process;
	XVec2 m_windowSize;

public:
	static DWORD WINAPI loadingProc(void * pParam);	//载入的线程
	void init(XVec2 windowSize,XResPos resoursePosition = RES_LOCAL_FOLDER);
	void move();				//进度条动作
	void draw();				//描绘进度条
	void setStart();			//设置开始进度条
	void setEnd();				//设置结束进度条
	void release();				//释放资源
	void setLoadPresent(float present);		//设置加载进度
};

inline void XLoading::setLoadPresent(float present)		//设置加载进度
{
	m_loadPresent = present;
	m_process.setValue(m_loadPresent);
}

inline void XLoading::move()
{
	if(m_loadPresent < 100.0)
	{
		m_loadPresent += 0.1f;
		m_process.setValue(m_loadPresent);

		m_gameTitleMoveData.move(10);
		if(m_gameTitleMoveData.getIsEnd())
		{
			m_gameTitleMoveData.set(120.0f,350.0f,0.05f,MD_MODE_LINE);
		}
		m_loadingTextLight.setPosition(680.0f + 128.0f - 32.0f,350.0f - 256.0f + 32.0f + m_gameTitleMoveData.getCurData());
	}
}

inline void XLoading::draw()
{
	m_loadingBG.draw();
	//m_process.draw();
	m_gameTitle.draw();
	m_loadingText.draw();
	m_loadingTextLight.draw();
}

inline void XLoading::release()
{
	m_loadingBG.release();
	m_gameTitle.release();
	m_loadingText.release();
	m_loadingTextLight.release();
	m_process.release();
}
}
#endif

#ifdef XEE_OS_LINUX
#include <pthread.h>
#include <unistd.h>           //usleep()
#include "glx.h"
namespace XE{
class XLoading
{
private:
	int m_resoursePosition;	//资源位置 0:外部 1:内部
public:
	float m_loadPresent;		//载入进度
	char m_isShow;			//是否显示载入内容
	char m_isEnd;			//0没有结束 1资源载入完成 2结束

	//下面主要是用于多线程容器切换
	Display *m_mainDisplay;		
	GLXPbuffer m_mainPBuffer;	//主线程容器的相关信息
	GLXContext m_mainContext;
	GLXPbuffer m_threadPBuffer;	//辅助线程的相关信息
	GLXContext m_threadContext;

	int (*m_loadFunc)(void);
	void setLoadFunc(int (*loadFunc)(void))
	{
		m_loadFunc = loadFunc;
	}

	XSprite m_loadingBG;		//载入画面背景
	XProgress m_process;
	XVec2 m_windowSize;

	XNumber Number;
	_XProgressTexture ProgressTexture;

public:
    static void *loadingProc(void * pParam);	//载入的线程

	void init(XVec2 windowSize,int resoursePosition = 0);
	void move();				//进度条动作
	void draw();				//描绘进度条
	void setStart();			//设置开始进度条
	void release();				//释放资源
	void setLoadPresent(float present);		//设置加载进度

	XLoading()
	:m_loadFunc(NULL)
	{
	}
};

inline void XLoading::setLoadPresent(float present)		//设置加载进度
{
	m_loadPresent = present;
	m_process.setValue(m_loadPresent);
}

inline void XLoading::move()
{
	if(m_loadPresent < 100.0)
	{
		m_loadPresent += 0.1;
		m_process.setValue(m_loadPresent);
	}
}

inline void XLoading::draw()
{
	m_loadingBG.draw();
	m_process.draw();
}

inline void XLoading::release()
{
	m_loadingBG.release();
	Number.release();
	ProgressTexture.release();
}
}
#endif

#endif
