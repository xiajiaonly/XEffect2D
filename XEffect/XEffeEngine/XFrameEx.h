#ifndef _JIA_XFRAMEEX_
#define _JIA_XFRAMEEX_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------

//优化说明
//1、按照字体图像文件那样，每帧的尺寸都相同，按一定的横纵排列在一个图片上，播放时自动裁剪
//2、做一个小程序，自动对图像进行有效尺寸的裁剪，然后将所有的图片经过计算进行拼合。
//小程序提取编号为x的帧的位置和尺寸，以及偏移位置，保存在指定的文件中

#include "XSprite.h"

#define MAX_FRAME_FILENAME_LENGTH 256	//文件名的长度
//#define MAX_FRAME_SUM 256		//序列帧最多只能256帧

class _XFrameEx:public _XObjectBasic	
{
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是重新写的序列帧的类的东西
private:
	_XSprite *m_pSprite;
	//------------------------------------------------------------------
	_XSCounter *m_cp;		//引用计数器
	int m_resoursePosition;	//资源位置 0:外部 1:内部
	float angle;			//精灵的角度
	char m_isVisiable;
public:
	void setAngle(float temp);
	float getAngle() const;
	void setVisiable() {m_isVisiable = 1;}					//设置物件可见
	void disVisiable() {m_isVisiable = 0;}						//设置物件不可见
	char getVisiable() const {return m_isVisiable;}					//获取物件是否可见的状态 
private:
	float x, y;			//精灵的位置
	float xsize, ysize;	//精灵的缩放尺寸
	float alpha;			//透明度
	float colorRed;		//红色
	float colorGreen;		//绿色
	float colorBlue;		//蓝色
	char m_isOverturn;		//是否x方向左右翻转

	int m_allFramesSum;							//序列帧动画中总帧数
	int m_allKeyFramesSum;						//序列帧动画中关键帧的数量
	float m_nowFramesNumble;					//当前播放到第几帧
	float m_actionSpeed;						//序列帧播放的速度
	char m_isLoop;								//序列帧是否可以循环
	int m_startFrame;							//从哪一帧开始
	char m_isEnd;								//序列帧是否播放完成
	char m_isEndImmediately;					//动画是否为立刻结束类型
	char m_isSetEnd;							//is end by user			//是否设置序列帧结束
	char m_isDisappearAtEnd;					//动画帧是否在播放完成之后消失

	char *m_frameName;				//序列帧文件名
	int *m_keyFrameArray;			//序列帧的关键帧编号序列
public:
	void setAttribute(const _XVector2& position,			//序列帧播放的位置
		int loop,				//序列帧是否循环
		int endImmediately,		//序列帧是否立即结束
		int startFrame,			//序列帧的起始帧
		float actionSpeed,		//序列帧的播放速度(这个数值建议在0.05左右)
		int disappearAtEnd,		//序列帧动画播放完成之后是否消失
		int isOverturn = 0);	//是否翻转

	void setPosition(const _XVector2& position);			//设置精灵的坐标
	void setPosition(float a,float b);			//设置精灵的坐标
	_XVector2 getPosition() const;

	void setSize(const _XVector2& size);		//设置精灵的缩放比例
	void setSize(float x,float y);	//设置精灵的缩放比例
	_XVector2 getSize() const;

	int getIsEnd() const;	//获得序列帧是否播放完成
	void setEnd();	//设置序列正播放完成
	void reset();	//设置已经播放完成的序列帧动画重新播放
	void move(int timeDelay);	//推进序列帧播放
	int getAllFramesSum() const
	{
		return m_allFramesSum;
	}
	float getNowFramesNumber() const
	{
		return m_nowFramesNumble;
	}

	_XSprite * getNowSprite(char frameFlag = 0);	//返回当前帧的精灵指针，如果 frameFlag = 1则返回的是整个精灵序列的指针
private:
	char m_isInited;		//是否初始化
	char m_blendTypeScr;	//blend模式，参看OpenGL中Blend的部分
	char m_blendTypeDst;
public:
	void setBlendType(int typeScr,int typeDst)	//设置混合模式
	{
		if(typeScr < 0) typeScr = 0;
		if(typeDst < 0) typeDst = 0;
		if(typeScr > 8 ) typeScr = 8;
		if(typeDst > 7 ) typeDst = 7;
		m_blendTypeScr = typeScr;
		m_blendTypeDst = typeDst;
		for(int i = 0;i < m_allKeyFramesSum;++ i)
		{
			m_pSprite[i].setBlendType(m_blendTypeScr,m_blendTypeDst);
		}
	}
	void (*m_pShaderProc)(void);	//定义一个shader的函数指针

	int init(const char *filename,			//图片的名称
		int resoursePosition = 0);		//资源的位置				
private:
	int releaseMem();			//释放自己的资源，对于被外界引用的资源需要根据引用计数器来决定
public:
	void release(){}			//保留原有的接口向下兼容，但实际上啥也不做
	void draw();			//显示序列帧
	void setColor(float r,float g,float b,float a);		//小于0不会改变这一元素
	void setAlpha(float a);
	float getAlpha() const;

	void setActionSpeed(float actionSpeed);	//设置序列帧播放的速度
	//下面是为物件管理定义的接口
	int isInRect(float x,float y);	//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);	//获取四个顶点的坐标，目前先不考虑旋转和缩放
private:
	int m_isACopy;	//判断是否为复制体，复制体没有自己的资源所以释放资源的时候不需要处理
	_XFrameEx(const _XFrameEx &temp);	//重载赋值构造函数为私有，防止意外调用而造成错误
public:
	_XFrameEx& operator = (const _XFrameEx& temp);	//建立一个同质体，对于指针的内部资源，使用目标的资源，(共用资源，但是会造成，没有自己自身的资源而造成问题)
	void setACopy(const _XFrameEx& temp);		//建立一个同质体，对于指针的内部资源，使用目标的资源，(共用资源，但是会造成，没有自己自身的资源而造成问题)

	void setRestart();			//设置重新回到起始帧开始播放

	_XFrameEx();
	~_XFrameEx();
};

#include "XFrameEx.inl"

#endif