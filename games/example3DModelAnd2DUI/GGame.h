#ifndef _JIA_GGAME_
#define _JIA_GGAME_
#include "XEffeEngine.h"
#include "X3D/X3DWorld.h"

class GGame:public XE::XGameBasic3D
{
public:
	GGame()
		:m_srcData(NULL)
		,m_dstData(NULL)
		,m_mapData(NULL)
		,m_isPlay(false)
		,m_nowTimer(0)
	{}
	virtual ~GGame(){}
protected:
	GGame(const GGame&);
	GGame &operator= (const GGame&);
public:
	bool init();				//TODO:这里进行游戏的初始化
	void move(float stepTime);	//TODO:这里是游戏的idle，也可以通过开启逻辑线程的方式处理
	void draw();				//TODO:绘制的函数
	void draw3D(XE::XBool withTex);				//TODO:绘制的函数
	void input(const XE::XInputEvent &event);	//TODO:键盘或者鼠标输入的事件
	void release();				//TODO:程序结束时的资源释放
	void ctrlEventProc(int id,int eventID);

	XE::XText m_filenameTxt;	//文件路径
	XE::XEdit m_filenameEdt;
	XE::XText m_radiusTxt;		//模糊半径
	XE::XEdit m_radiusEdt;
	XE::XButton m_updateBtn;	//更新按钮

	XE::XSprite m_srcSprite;
	XE::XSprite m_dstSprite; 
	XE::XTextureData m_srcPic;
	XE::XTextureData m_dstPic;
	unsigned char *m_srcData;
	unsigned char *m_dstData;
	void updateData();
	XE::XTerrain m_tmpTerrain;	//地形
	float *m_mapData;	//地形数据

	XE::XSprite m_backSprite;
	XE::XShaderGLSL m_tmpShader;
	bool m_isPlay;
	int m_nowTimer;
};

#endif