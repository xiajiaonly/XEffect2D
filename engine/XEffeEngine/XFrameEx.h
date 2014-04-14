#ifndef _JIA_XFRAMEEX_
#define _JIA_XFRAMEEX_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------
//新的序列帧的类，已经不支持预设偏移，做这种优化的原因在于新的资源管理的管理器已经对图像的空白部分进行了优化，
//所以没有必要再预先进行优化，以便于节省美术的工作量
#include "XSprite.h"

//#define MAX_FRAME_SUM 256		//序列帧最多只能256帧

class _XFrameEx:public _XObjectBasic	
{
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是重新写的序列帧的类的东西
private:
	_XSprite *m_pSprite;
	//------------------------------------------------------------------
	_XSCounter *m_cp;		//引用计数器
	_XResourcePosition m_resoursePosition;	//资源位置 0:外部 1:内部
	float angle;			//精灵的角度
	_XBool m_isVisiable;
public:
	void setAngle(float temp);
	float getAngle() const;
	void setVisiable() 
	{
		m_isVisiable = XTrue;
		updateChildVisiable();
	}					//设置物件可见
	void disVisiable() 
	{
		m_isVisiable = XFalse;
		updateChildVisiable();
	}					//设置物件不可见
	_XBool getVisiable() const {return m_isVisiable;}			//获取物件是否可见的状态 
private:
	float x, y;				//精灵的位置
	float xsize, ysize;		//精灵的缩放尺寸
	float alpha;			//透明度
	float colorRed;			//红色
	float colorGreen;		//绿色
	float colorBlue;		//蓝色
	_XBool m_isOverturn;		//是否x方向左右翻转

	int m_allFramesSum;				//序列帧动画中总帧数
	int m_allKeyFramesSum;			//序列帧动画中关键帧的数量
	float m_nowFramesNumble;		//当前播放到第几帧
	float m_actionSpeed;			//序列帧播放的速度
	_XBool m_isLoop;					//序列帧是否可以循环
	int m_startFrame;				//从哪一帧开始
	_XBool m_isEnd;					//序列帧是否播放完成
	_XBool m_isEndImmediately;		//动画是否为立刻结束类型
	_XBool m_isSetEnd;				//is end by user			//是否设置序列帧结束
	_XBool m_isDisappearAtEnd;		//动画帧是否在播放完成之后消失

	char *m_frameName;				//序列帧文件名
	int *m_keyFrameArray;			//序列帧的关键帧编号序列
public:
	void setAttribute(const _XVector2& position,			//序列帧播放的位置
		_XBool loop,				//序列帧是否循环
		_XBool endImmediately,		//序列帧是否立即结束
		int startFrame,			//序列帧的起始帧
		float actionSpeed,		//序列帧的播放速度(fps * 0.001)
		_XBool disappearAtEnd,		//序列帧动画播放完成之后是否消失
		_XBool isOverturn = XFalse);	//是否翻转

	using _XObjectBasic::setPosition;
	void setPosition(float a,float b);			//设置精灵的坐标
	void setPositionX(float a);
	void setPositionY(float b);
	_XVector2 getPosition() const;

	using _XObjectBasic::setSize;
	void setSize(float x,float y);	//设置精灵的缩放比例
	_XVector2 getSize() const;

	_XBool getIsEnd() const;	//获得序列帧是否播放完成
	void setEnd();	//设置序列帧播放完成
	void reset();	//设置已经播放完成的序列帧动画重新播放
	void move(int timeDelay);	//推进序列帧播放
	int getAllFramesSum() const {return m_allFramesSum;}
	float getNowFramesNumber() const {return m_nowFramesNumble;}

	_XSprite * getNowSprite(char frameFlag = 0);	//返回当前帧的精灵指针，如果 frameFlag = 1则返回的是整个精灵序列的指针
private:
	_XBool m_isInited;		//是否初始化
	char m_blendTypeScr;	//blend模式，参看OpenGL中Blend的部分
	char m_blendTypeDst;
	_XShaderGLSL *m_pShader;
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
	//void (*m_pShaderProc)(void);	//定义一个shader的函数指针
	void setShaderClass(_XShaderGLSL * shader) {m_pShader = shader;}

	_XBool init(const char *filename,			//图片的名称
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);		//资源的位置	
	//增加一个设置对齐方式的接口
	void setIsTransformCenter(_XTransformMode temp)
	{//尚未实现
		if(!m_isInited) return;
		for(int i = 0;i < m_allKeyFramesSum;++ i)
		{
			m_pSprite[i].setIsTransformCenter(temp);
		}
	}
private:
	_XBool releaseMem();			//释放自己的资源，对于被外界引用的资源需要根据引用计数器来决定
public:
	void release(){}			//保留原有的接口向下兼容，但实际上啥也不做
	void draw();			//显示序列帧

	using _XObjectBasic::setColor;
	void setColor(float r,float g,float b,float a);		//小于0不会改变这一元素
	_XFColor getColor() const 
	{
		if(m_pSprite == NULL) return _XFColor();
		else return m_pSprite[0].getColor();
	}
	void setAlpha(float a);
	float getAlpha() const;

	void setActionSpeed(float actionSpeed);	//设置序列帧播放的速度
	//下面是为物件管理定义的接口
	_XBool isInRect(float x,float y);	//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);	//获取四个顶点的坐标，目前先不考虑旋转和缩放
private:
	_XBool m_isACopy;	//判断是否为复制体，复制体没有自己的资源所以释放资源的时候不需要处理
	_XFrameEx(const _XFrameEx &temp);	//重载赋值构造函数为私有，防止意外调用而造成错误
public:
	_XFrameEx& operator = (const _XFrameEx& temp);	//建立一个同质体，对于指针的内部资源，使用目标的资源，(共用资源，但是会造成，没有自己自身的资源而造成问题)
	void setACopy(const _XFrameEx& temp);		//建立一个同质体，对于指针的内部资源，使用目标的资源，(共用资源，但是会造成，没有自己自身的资源而造成问题)

	void setRestart();			//设置重新回到起始帧开始播放

	_XFrameEx();
	~_XFrameEx();
	virtual void justForTest() {;}
};

struct _XFrameExParamData
{//精灵物件的参数
	_XResourcePosition resourcePosition;	//读取资源的位置
	_XVector2 pos;			//初始位置
	_XBool loop;				//是否循环
	_XBool endImmediately;		
	int startFrame;
	float actionSpeed;
	_XBool disappearAtEnd;
	_XBool isOverturn;

	char * getStrFromData()
	{
		char *tempStr = createArrayMem<char>(256);
		if(tempStr == NULL) return NULL;
		int offset = 0;
		sprintf(tempStr + offset,"ResPos:%d,\n",resourcePosition);
		offset = strlen(tempStr);
		sprintf(tempStr + offset,"ObjPos:%f|%f,\n",pos.x,pos.y);
		offset = strlen(tempStr);
		if(loop) sprintf(tempStr + offset,"Loop:1,\n");
		else sprintf(tempStr + offset,"Loop:0,\n");
		offset = strlen(tempStr);
		if(endImmediately) sprintf(tempStr + offset,"EndIMD:1,\n");
		else sprintf(tempStr + offset,"EndIMD:0,\n");
		offset = strlen(tempStr);
		sprintf(tempStr + offset,"StartFRM:%d,\n",startFrame);
		offset = strlen(tempStr);
		sprintf(tempStr + offset,"Speed:%f,\n",actionSpeed);
		offset = strlen(tempStr);
		if(disappearAtEnd) sprintf(tempStr + offset,"disappear:1,\n");
		else sprintf(tempStr + offset,"disappear:0,\n");
		offset = strlen(tempStr);
		if(isOverturn) sprintf(tempStr + offset,"Overturn:1,\n");
		else sprintf(tempStr + offset,"Overturn:0,\n");
		offset = strlen(tempStr);
		return tempStr;
	}
	int getDataFromStr(const char * str)
	{
		if(str == NULL) return 0;
		int tempData = 0;
		int offset = 0;
		if(sscanf(str + offset,"ResPos:%d,\n",&resourcePosition) != 1) return offset;
		offset += getCharPosition(str + offset,',') + 1;
		sscanf(str + offset,"ObjPos:%f|%f,\n",&pos.x,&pos.y);
		offset += getCharPosition(str + offset,',') + 1;
		sscanf(str + offset,"Loop:%d,\n",&tempData);
		if(tempData == 0) loop = XFalse;
		else loop = XTrue;
		offset += getCharPosition(str + offset,',') + 1;
		sscanf(str + offset,"EndIMD:%d,\n",&tempData);
		if(tempData == 0) endImmediately = XFalse;
		else endImmediately = XTrue;
		offset += getCharPosition(str + offset,',') + 1;
		sscanf(str + offset,"StartFRM:%d,\n",&startFrame);
		offset += getCharPosition(str + offset,',') + 1;
		sscanf(str + offset,"Speed:%f,\n",&actionSpeed);
		offset += getCharPosition(str + offset,',') + 1;
		sscanf(str + offset,"disappear:%d,\n",&tempData);
		if(tempData == 0) disappearAtEnd = XFalse;
		else disappearAtEnd = XTrue;
		offset += getCharPosition(str + offset,',') + 1;
		sscanf(str + offset,"Overturn:%d,\n",&tempData);
		if(tempData == 0) isOverturn = XFalse;
		else isOverturn = XTrue;
		offset += getCharPosition(str + offset,',') + 1;
		return offset;
	}
	_XBool getDataFromFile(FILE *fp)
	{
		if(fp == NULL) return XFalse;
		int tempData = 0;
		fscanf(fp,"ResPos:%d,\n",&resourcePosition);
		fscanf(fp,"ObjPos:%f|%f,\n",&pos.x,&pos.y);
		fscanf(fp,"Loop:%d,\n",&tempData);
		if(tempData == 0) loop = XFalse;
		else loop = XTrue;
		fscanf(fp,"EndIMD:%d,\n",&tempData);
		if(tempData == 0) endImmediately = XFalse;
		else endImmediately = XTrue;
		fscanf(fp,"StartFRM:%d,\n",&startFrame);
		fscanf(fp,"Speed:%f,\n",&actionSpeed);
		fscanf(fp,"disappear:%d,\n",&tempData);
		if(tempData == 0) disappearAtEnd = XFalse;
		else disappearAtEnd = XTrue;
		fscanf(fp,"Overturn:%d,\n",&tempData);
		if(tempData == 0) isOverturn = XFalse;
		else isOverturn = XTrue;
		return XTrue;
	}
	//默认数据
	_XFrameExParamData()
		:resourcePosition(RESOURCE_SYSTEM_DEFINE)
		,pos(0.0f,0.0f)
		,loop(XFalse)
		,endImmediately(XFalse)
		,startFrame(0)
		,actionSpeed(0.03f)
		,disappearAtEnd(XFalse)
		,isOverturn(XFalse)
	{
	}
};

#include "XFrameEx.inl"

#endif