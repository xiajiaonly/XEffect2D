#ifndef _JIA_XFRAME_
#define _JIA_XFRAME_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

//优化说明
//1、按照字体图像文件那样，每帧的尺寸都相同，按一定的横纵排列在一个图片上，播放时自动裁剪
//2、做一个小程序，自动对图像进行有效尺寸的裁剪，然后将所有的图片经过计算进行拼合。
//小程序提取编号为x的帧的位置和尺寸，以及偏移位置，保存在指定的文件中

#include "XOSDefine.h"
#ifdef XEE_OS_WINDOWS
#include "windows.h"
#endif
#include "SDL.h"
#include "SDL_image.h"
#include "gl.h"
#include "XBasicClass.h"

#include "math.h"
#include "XBasicFun.h"
#include "XBasicOpenGL.h"
#include "XShaderGLSL.h"

#define MAX_FRAME_SUM (65)		//序列帧最多只能256帧

class _XFrame
{
private:
	_XSCounter *m_cp;		//引用计数器

	_XResourcePosition m_resoursePosition;	//资源位置 0:外部 1:内部
	_XBool m_isInited;		//是否已经进行了初始化

	float angle;			//精灵的角度
	float angleRadian;		//弧度标志的角度
	float sinAngle;
	float cosAngle;
	_XShaderGLSL *m_pShader;
public:
	void setAngle(float temp);
	float getAngleDegree();	//获得角度
	float getAngleRadian();	//获得弧度
	_XBool m_isFlash;	//是否闪光
	//void (*m_pShaderProc)(void);	//定义一个shader的函数指针
	void setShaderClass(_XShaderGLSL * shader) {m_pShader = shader;}
private:
	float x, y;			//精灵的位置
	float xsize, ysize;	//精灵的缩放尺寸
	float alpha;			//透明度
	float colorRed;		//红色
	float colorGreen;		//绿色
	float colorBlue;		//蓝色
	_XBool m_isOverturn;		//是否x方向左右翻转
	_XBool m_isVisible;

	_XTexture *m_texnum;						//序列帧图片的指针

	int m_allFramesSum;							//序列帧动画中总帧数
	int m_allKeyFramesSum;						//序列帧动画中关键帧的数量
	float m_nowFramesNumble;					//当前播放到第几帧
	float m_actionSpeed;						//序列帧播放的速度
	_XBool m_isLoop;								//序列帧是否可以循环
	int m_startFrame;							//从哪一帧开始
	_XBool m_isEnd;								//序列帧是否播放完成
	_XBool m_isEndImmediately;					//动画是否为立刻结束类型
	_XBool m_isSetEnd;							//is end by user			//是否设置序列帧结束
	_XBool m_isDisappearAtEnd;					//动画帧是否在播放完成之后消失
	//GLint *m_width;
	//GLint *m_height;							//精灵的尺寸
	int *wre;
	int *hre;
	int *xpa;
	int *ypa;

	char *m_frameName;				//序列帧文件名
	int *m_keyFrameArray;			//序列帧的关键帧编号序列

	//++++++++++++++++++++++++++++++++++++
	//为中心偏移而定义的变量（由于图像的旋转和缩放是对于图片中心进行的，所以当序列帧出现中心偏移时，
	//这里需要考虑中心偏移带来的旋转和缩放问题（缩放暂时没有考虑））
	float m_centerX;			//总的序列帧中心坐标
	float m_centerY;
	float *m_centerDW;							//每帧的中心偏移
	float *m_centerDH;			
	_XVector2 *m_keyFramePosition;	//关键帧偏移坐标
	//------------------------------------
public:
	void setVisible() {m_isVisible = XTrue;}					//设置物件可见
	void disVisible() {m_isVisible = XFalse;}						//设置物件不可见
	_XBool getVisible() const {return m_isVisible;}					//获取物件是否可见的状态 

	void setAttribute(const _XVector2& position,			//序列帧播放的位置
		_XBool loop,				//序列帧是否循环
		_XBool endImmediately,		//序列帧是否立即结束
		int startFrame,				//序列帧的起始帧
		float actionSpeed,			//序列帧的播放速度(fps * 0.001)
		_XBool disappearAtEnd,		//序列帧动画播放完成之后是否消失
		_XBool isOverturn = XFalse);	//是否翻转

	void setPosition(const _XVector2& position);			//设置精灵的坐标
	void setPosition(float a,float b);			//设置精灵的坐标
	_XVector2 getPosition() const;

	int getNowX() const;	//获得精灵图片当前的位置X(允许精灵中心偏移之后，这个需要重新考虑)
	int getNowY() const;	//获得精灵图片当前的位置Y(允许精灵中心偏移之后，这个需要重新考虑)

	int getNowWidth() const;	//获得当前帧的宽度
	int getNowHeight() const;	//获得当前帧的高度

	int getInitHalfWidth() const;		//获取第一帧的半宽度,也就是序列帧左上角到序列帧中点的宽度
	int getInitHalfHeight() const;	//获取第一帧的半高度,也就是序列帧左上角到序列帧中点的高度

	void setSize(const _XVector2& size);		//设置精灵的缩放比例
	void setSize(float x,float y);	//设置精灵的缩放比例
	_XVector2 getSize();

	_XBool getIsEnd() const;	//获得序列帧是否播放完成
	void setEnd();	//设置序列正播放完成
	void reset();	//设置已经播放完成的序列帧动画重新播放
	void move(int timeDelay);	//推进序列帧播放

	_XTexture * getTexture(int frameFlag = 0);	//返回当前帧的贴图指针 frameFlag = 1时返回的是序列帧的图片序列数组的指针

	_XBool init(const char *filename,			//图片的名称
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);		//资源的位置				
private:
	_XBool releaseMem();									//释放资源
public:
	void release(){}		//保留原有的接口向下兼容，但实际上啥也不做
	void draw(const GLuint *pTexnum = NULL) const;			//贴图不是序列帧是这里pTexnum为贴图的指针 序列帧时pTexnum为序列帧贴图组的指针
	void setColor(float r,float g,float b,float a);		//小于0不会改变这一元素
	void setAlpha(float alpha);	//设置透明度
	float getAlpha() const;

	void setActionSpeed(float actionSpeed);	//设置序列帧播放的速度
private:
	//char m_haveSelfMemery;			//是否在构造函数中分配内存空间
	//最后返回的是一个应用，所以不需要定义拷贝构造函数
	_XBool m_isACopy;	//判断是否为复制体，复制体没有自己的资源所以释放资源的时候不需要处理
	_XFrame(const _XFrame& temp);	//重载赋值构造函数为私有成员，防止不必要的意外调用
public:
	_XFrame& operator = (const _XFrame& temp);
	void setACopy(const _XFrame& temp);		//建立一个同质体，如果自身有分配内存则这里复制相同的资源，如果没有分配内存空间，这里就指向目标的内存空间

	void setRestart();			//设置重新回到起始帧开始播放

	_XFrame();
	//_XFrame(char withNoMemry);	//不分配内存空间，如果多个序列帧使用同一个图片资源时可以使用这个方法
	~_XFrame();
	//下面为了资源还原定义一些额外的接口
	int getAllKeyFrameSum() const {return m_allKeyFramesSum;}
	_XVector2 getKeyFramePos(int index) const {return m_keyFramePosition[index];}
	int getKeyFrameWre(int index) const {return wre[index];}
	int getKeyFrameHre(int index) const {return hre[index];}
};

#include "XFrame.inl"

#endif