#ifndef _JIA_GGAME_
#define _JIA_GGAME_
#include "XEffeEngine.h"
#include "XMath\XField2D.h"
#include "XShaderGLSL.h"
#include "XControl\XSimpleChart.h"

//优化说明
//1、加入一个整体的模糊效果
//2、加入一个随机性的闪烁效果	+
//3、大小加入随机性
//4、亮度加入随机性
#define MAX_SUM (200000)
//#define WITH_THREAD	//是否使用多线程优化
//#define WITH_VBO		//是否使用vbo优化
#define WITH_FIELD		//是否使用场的作用
#define WITH_FBO		//是否使用FBO
#define WITH_GAUSS		//是否使用高斯模糊，必须要使用FBO

class GGame:public XE::XGameBasic
{
public:
	GGame(){}
	virtual ~GGame(){}
protected:
	GGame(const GGame&);
	GGame &operator= (const GGame&);
public:
	bool init();
	void move(float stepTime);
	void draw();
	void input(const XE::XInputEvent &event);
	void release()
	{
#ifdef WITH_THREAD
		m_logicThread0.threadEnd();
		m_logicThread1.threadEnd();
#endif
	}
	void ctrlEventProc(int id,int eventID){}
	void ResetParticles(int index);
	XE::XTailingDLine m_tailing[MAX_SUM];
	XE::XTailingObject m_tailingObj[MAX_SUM];
#ifdef WITH_FIELD
	float m_r[MAX_SUM];
	XE::XField2D m_field2D;
#endif

#ifdef WITH_THREAD
	XE::XThread m_logicThread0;
	XE::XThread m_logicThread1;
	static void ThreadFun0(void *);
	static void ThreadFun1(void *);
#endif
#ifdef WITH_VBO
	XE::XVBO m_vbo;
	float *m_v;
	float *m_c;
	int *m_sum;
	int m_allSum;
#endif
#ifdef WITH_FBO
	XE::XFBO m_fbo;
	XE::XSprite m_fboSprite;
#endif
#ifdef WITH_GAUSS
	XE::XShaderGLSL m_shader;
#endif
	XE::XSimpleChart m_frameData;
};
#endif