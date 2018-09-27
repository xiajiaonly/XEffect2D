#ifndef _JIA_XCAMERABASIC_
#define _JIA_XCAMERABASIC_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2013.4.9
//--------------------------------
//这是一个摄像头视频数据提取的基类，建议所有的视频数据采集的类都继承自这个类
#define WITH_GL_TEXTRUE		//是否提供对OpenGL贴图的支持，如果只是纯粹的获取数据的话，是没有必要对贴图进行支持的
#include "../XOSDefine.h"
#ifdef WITH_GL_TEXTRUE
#include "../XBasicOpenGL.h"
#include "../XSprite.h"
#else
#include "XCommonDefine.h"
#endif
#include "../XBabyBase.h"
namespace XE{
#define SUPPORT_FOR_HIK (1)	//是否支持海康的摄像头

enum XPixelsInputType
{
	TYPE_CAM_NORMAL,	//普通摄像头
#if SUPPORT_FOR_HIK	
	TYPE_CAM_HIK,		//海康的摄像头
#endif
	TYPE_CAM_AVER,		//圆刚的摄像头
	//下面的工作（尚未完成）
	TYPE_VIDEO,		//视频数据
	TYPE_PICTURE,	//图片数据

	TYPE_KINECT_COLOR,	//kinect 的彩色数据
	TYPE_KINECT_DEPTH,	//kinect 的深度数据
	TYPE_KINECT2_COLOR,	//kinect2的彩色数据
	TYPE_KINECT2_DEPTH,	//kinect2的深度数据
};
//摄像机的相关参数
struct XPixelsInputInfo	//原来的XCameraData
{
	XPixelsInputInfo()
		:isSilence(false)
		,withDefaultFormat(true)
	{}
	std::string filename;	//文件名称和路径
	int w;				//像素宽
	int h;				//像素高
	int fps;			//帧率
	int deviceOrder;	//设备编号
	//下面两个变量是为了videoInput而加入的
	bool isSilence;		//是否无声
	bool withDefaultFormat;	//是否使用默认的视频格式
	
	XPixelsInputType pixelsInputType;	//摄像头的型号
	bool needReset;	//是否初始化贴图为黑色

	void setNormal(int width,int height,int order = 0,int setfps = 30)
	{
		w = width;
		h = height;
		deviceOrder = order;
		fps = setfps;
		pixelsInputType = TYPE_CAM_NORMAL;
		needReset = false;
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
		strcpy_s(ipAddress,16,add);
		strcpy_s(userName,64,usern);
		strcpy_s(pwd,16,passward);
		port = p;
		pixelsInputType = TYPE_CAM_HIK;
		needReset = false;
	}
#endif
};
class XPixelsInputBasic:public XBabyBase
{
private:
protected:
	XBool m_isInited;		//是否进行初始化

	int m_pixelsWidth;		//摄像机的像素宽
	int m_pixelsHeight;		//摄像机的像素高
#ifdef WITH_GL_TEXTRUE
//	int m_cameraTexWidth;	//贴图的像素宽
//	int m_cameraTexHeight;	//贴图的像素高度
#endif
	int m_buffSize;			//像素数据buff的尺寸

	XBool m_isWork;			//设置设备的工作状态
	XColorMode m_colorMode;
public:
	XColorMode getColorMode()const{return m_colorMode;}
	int getWidth() const {return m_pixelsWidth;}
	int getHeight() const {return m_pixelsHeight;}
#ifdef WITH_GL_TEXTRUE
//	int getTexWidth() const {return m_cameraTexWidth;}
//	int getTexHeight() const {return m_cameraTexHeight;}
	XTexture m_pixelsTex;	//贴图
	unsigned int getPixelsTex() const {return m_pixelsTex.m_texture;}
	XSprite m_pixelsSprite;//用于绘图的精灵
#endif

	void setWork() {m_isWork = XTrue;}
	void setStop() {m_isWork = XFalse;}
	XBool getWorkState() const {return m_isWork;}

	virtual XBool init(XPixelsInputInfo &info) = 0;	//初始化并打开设备
	virtual void release() = 0;					//释放设备，并释放资源
	virtual XBool updateFrame() = 0;				//更新帧数据

	virtual int getBuffSize() const{return m_buffSize;}	//放像素数据的buff尺寸
	virtual void getData(unsigned char * p) const = 0;	//复制像素数据
	virtual unsigned char * getDataP() const = 0;				//获取数据的指针

	virtual void draw()	//描绘像素数据
	{
#ifdef WITH_GL_TEXTRUE
		if(!m_isInited) return;
		m_pixelsSprite.draw(&m_pixelsTex.m_texture);
#endif
	}

	XPixelsInputBasic()
		:m_isInited(XFalse)
		,m_isWork(XTrue)
	{}
	virtual ~XPixelsInputBasic(){}
};
}
#endif