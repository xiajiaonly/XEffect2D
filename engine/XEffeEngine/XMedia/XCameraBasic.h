#ifndef _JIA_XCAMERABASIC_
#define _JIA_XCAMERABASIC_
#include "XEffeEngine.h"
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2013.4.9
//--------------------------------
//这是一个摄像头视频数据提取的基类，建议所有的视频数据采集的类都继承自这个类

#define SUPPORT_FOR_HIK (1)	//是否支持海康的摄像头

enum _XCameraType
{
	CAMERA_TYPE_NORMAL,	//普通摄像头
#if SUPPORT_FOR_HIK	
	CAMERA_TYPE_HIK,		//海康的摄像头
#endif
	CAMERA_TYPE_AVER,		//圆刚的摄像头
};
//摄像机的相关参数
struct _XCameraData
{
	int w;				//像素宽
	int h;				//像素高
	int fps;			//帧率
	int deviceOrder;	//设备编号
	_XCameraType cameraType;	//摄像头的型号

	void setNormal(int width,int height,int order = 0,int setfps = 30)
	{
		w = width;
		h = height;
		deviceOrder = order;
		fps = setfps;
		cameraType = CAMERA_TYPE_NORMAL;
	}
#if SUPPORT_FOR_HIK
	char ipAddress[16];	//网络摄像头的网址
	char userName[64];	//用户名
	char pwd[16];		//密码
	int port;			//端口号

	void setHIK(int width,int height,const char *add,const char *usern,
		const char * passward,int p = 8000)
	{
		if(add == NULL || usern == NULL || passward == NULL) return;
		w = width;
		h = height;
		strcpy(ipAddress,add);
		strcpy(userName,usern);
		strcpy(pwd,passward);
		port = p;
		cameraType = CAMERA_TYPE_HIK;
	}
#endif
};

class _XCameraBasic
{
private:
protected:
	_XBool m_isInited;		//是否进行初始化

	int m_cameraWidth;		//摄像机的像素宽
	int m_cameraHeight;		//摄像机的像素高
	int m_cameraTexWidth;	//贴图的像素宽
	int m_cameraTexHeight;	//贴图的像素高度
	int m_buffSize;			//像素数据buff的尺寸

	_XBool m_isWork;			//设置设备的工作状态
public:
	_XTexture m_cameraTex;	//贴图
	int getWidth() const {return m_cameraWidth;}
	int getHeight() const {return m_cameraHeight;}
	int getTexWidth() const {return m_cameraTexWidth;}
	int getTexHeight() const {return m_cameraTexHeight;}
	GLuint getCameraTex() const {return m_cameraTex.m_texture;}
	_XSprite m_cameraSprite;//用于绘图的精灵

	void setWork() {m_isWork = XTrue;}
	void setStop() {m_isWork = XFalse;}
	_XBool getWorkState() const {return m_isWork;}

	virtual _XBool init(_XCameraData &data) = 0;	//初始化并打开设备
	virtual void release() = 0;					//释放设备，并释放资源
	virtual _XBool upDateFrame() = 0;				//更新帧数据

	virtual int getBuffSize() const{return m_buffSize;}	//放像素数据的buff尺寸
	virtual void getData(unsigned char * p) const = 0;	//复制像素数据
	virtual unsigned char * getDataP() const = 0;				//获取数据的指针

	virtual void draw()	//描绘像素数据
	{
		if(!m_isInited) return;
		m_cameraSprite.draw(&m_cameraTex.m_texture);
	}

	_XCameraBasic()
		:m_isInited(XFalse)
		,m_isWork(XTrue)
	{}
	virtual ~_XCameraBasic(){}
};

#endif