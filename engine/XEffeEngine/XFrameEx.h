#ifndef _JIA_XFRAMEEX_
#define _JIA_XFRAMEEX_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------
//新的序列帧的类，已经不支持预设偏移，做这种优化的原因在于新的资源管理的管理器已经对图像的空白部分进行了优化，
//所以没有必要再预先进行优化，以便于节省美术的工作量
#include "XOSDefine.h"
//#include "XObjectBasic.h"
//#include "XShaderGLSL.h"
#include "XSprite.h"
namespace XE{
class XFrameEx:public XObjectBasic	
{
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是重新写的序列帧的类的东西
private:
	XSprite *m_pSprite;
	//------------------------------------------------------------------
	XSCounter *m_cp;		//引用计数器
	XResourcePosition m_resoursePosition;	//资源位置 0:外部 1:内部
	float m_angle;			//精灵的角度
	XBool m_isVisible;
public:
	void setAngle(float temp);
	float getAngle() const;
	void setVisible();					//设置物件可见
	void disVisible();					//设置物件不可见
	XBool getVisible() const {return m_isVisible;}			//获取物件是否可见的状态 
private:
	//float x, y;				//精灵的位置
	XVector2 m_position;
	//float xsize, ysize;		//精灵的缩放尺寸
	XVector2 m_scale;
	XFColor m_color;
	//float alpha;			//透明度
	//float colorRed;			//红色
	//float colorGreen;		//绿色
	//float colorBlue;		//蓝色
	XBool m_isOverturn;		//是否x方向左右翻转

	int m_allFramesSum;				//序列帧动画中总帧数
	int m_allKeyFramesSum;			//序列帧动画中关键帧的数量
	float m_curFramesNumble;		//当前播放到第几帧
	float m_actionSpeed;			//序列帧播放的速度
	XBool m_isLoop;					//序列帧是否可以循环
	int m_startFrame;				//从哪一帧开始
	XBool m_isEnd;					//序列帧是否播放完成
	XBool m_isEndImmediately;		//动画是否为立刻结束类型
	XBool m_isSetEnd;				//is end by user			//是否设置序列帧结束
	XBool m_isDisappearAtEnd;		//动画帧是否在播放完成之后消失
	bool m_isPause;	//是否处于暂停状态
	bool m_isStopAtEnd;	//结束之后是否停留在最后一帧
	int m_pauseAtIndex;	//播放并停留在指定帧

	char *m_frameName;				//序列帧文件名
	int *m_keyFrameArray;			//序列帧的关键帧编号序列,-1为空白帧什么也不敢

	bool loadData(const char *filename,			//图片的名称
		XResourcePosition resoursePosition);//载入资源
	bool loadFromFolder(const char *filename);	//从文件夹中载入资源
	bool loadFromPacker(const char *filename);	//从压缩包中载入资源
	bool loadFromWeb(const char *filename);		//从网页中读取资源
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	bool m_isThreadLoading;		//是否正在多线程载入
	XThreadState m_loadThread;
	std::string m_threadFilename;
	XResourcePosition m_threadResPos;
	bool m_neadUpdateAttribute;	//多线程载入时是否需要更新属性
public:
	//使用多线程的方式从后台载入资源
	bool initThread(const char *filename,			//图片的名称
		XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	static DWORD WINAPI loadThread(void *);
//------------------------------------------------------------
public:
	void setPauseAtIndex(int index){m_pauseAtIndex = index;}	//设置播放并停留在指定帧
	void pause(){m_isPause = true;}
	void resume(){m_isPause = false;}
	bool getIsPause()const{return m_isPause;}
	void setIsStopAtEnd(bool flag){m_isStopAtEnd = flag;}
	bool getIsStopAtEnd()const{return m_isStopAtEnd;}

	void setAttribute(const XVector2& position,			//序列帧播放的位置
		XBool loop,				//序列帧是否循环
		XBool endImmediately,		//序列帧是否立即结束
		int startFrame,			//序列帧的起始帧
		float actionSpeed,		//序列帧的播放速度(fps * 0.001)
		XBool disappearAtEnd,		//序列帧动画播放完成之后是否消失
		XBool isOverturn = XFalse);	//是否翻转

	using XObjectBasic::setPosition;
	void setPosition(float a,float b);			//设置精灵的坐标
	void setPositionX(float a);
	void setPositionY(float b);
	XVector2 getPosition() const;

	using XObjectBasic::setScale;
	void setScale(float a,float b);	//设置精灵的缩放比例
	XVector2 getScale() const;

	XBool getIsEnd() const;	//获得序列帧是否播放完成
	void setEnd();	//设置序列帧播放完成
	void reset();	//设置已经播放完成的序列帧动画重新播放
	void move(float timeDelay);	//推进序列帧播放
	int getAllFramesSum() const {return m_allFramesSum;}
	float getCurFramesNumber() const {return m_curFramesNumble;}

	XSprite * getCurSprite(bool isAllFrames = false);	//返回当前帧的精灵指针，isAllFrames：是否返回整个精灵序列的指针
private:
	XBool m_isInited;		//是否初始化
	char m_blendTypeScr;	//blend模式，参看OpenGL中Blend的部分
	char m_blendTypeDst;
	XShaderGLSL *m_pShader;
public:
	void setBlendType(int typeScr,int typeDst);	//设置混合模式
	//void (*m_pShaderProc)(void);	//定义一个shader的函数指针
	void setShaderClass(XShaderGLSL * shader) {m_pShader = shader;}

	XBool init(const char *filename,			//图片的名称
		XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);		//资源的位置	
	//增加一个设置对齐方式的接口
	void setIsTransformCenter(XTransformMode temp);
private:
	XBool releaseMem();			//释放自己的资源，对于被外界引用的资源需要根据引用计数器来决定
public:
	void release(){}			//保留原有的接口向下兼容，但实际上啥也不做
	void draw();			//显示序列帧

	using XObjectBasic::setColor;
	void setColor(float r,float g,float b,float a);		//小于0不会改变这一元素
	XFColor getColor() const;
	void setAlpha(float a);
	float getAlpha() const;

	void setActionSpeed(float actionSpeed);	//设置序列帧播放的速度
	//下面是为物件管理定义的接口
	XBool isInRect(float x,float y);	//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	XVector2 getBox(int order);	//获取四个顶点的坐标，目前先不考虑旋转和缩放
private:
	XBool m_isACopy;	//判断是否为复制体，复制体没有自己的资源所以释放资源的时候不需要处理
	XFrameEx(const XFrameEx &temp);	//重载赋值构造函数为私有，防止意外调用而造成错误
public:
	XFrameEx& operator = (const XFrameEx& temp);	//建立一个同质体，对于指针的内部资源，使用目标的资源，(共用资源，但是会造成，没有自己自身的资源而造成问题)
	void setACopy(const XFrameEx& temp);		//建立一个同质体，对于指针的内部资源，使用目标的资源，(共用资源，但是会造成，没有自己自身的资源而造成问题)

	void setRestart();			//设置重新回到起始帧开始播放

	XFrameEx();
	~XFrameEx();
	//virtual void justForTest() {;}
};

struct XFrameExParamData
{//精灵物件的参数
	XResourcePosition resourcePosition;	//读取资源的位置
	XVector2 pos;			//初始位置
	XBool loop;				//是否循环
	XBool endImmediately;		
	int startFrame;
	float actionSpeed;
	XBool disappearAtEnd;
	XBool isOverturn;

	char * getStrFromData();
	int getDataFromStr(const char * str);
	XBool getDataFromFile(FILE *fp);
	//默认数据
	XFrameExParamData()
		:resourcePosition(RESOURCE_SYSTEM_DEFINE)
		,pos(0.0f,0.0f)
		,loop(XFalse)
		,endImmediately(XFalse)
		,startFrame(0)
		,actionSpeed(0.03f)
		,disappearAtEnd(XFalse)
		,isOverturn(XFalse)
	{}
};
#if WITH_INLINE_FILE
#include "XFrameEx.inl"
#endif
}
#endif