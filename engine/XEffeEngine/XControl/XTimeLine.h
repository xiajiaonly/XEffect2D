#ifndef _JIA_XTIMELINE_
#define _JIA_XTIMELINE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.4.2
//--------------------------------
#include "../XXml.h"
#include "../XFont/XNumberEx.h"
#include "../XMath/XVector2.h"
#include "../XMath/XMoveData.h"
#include "../XFont/XFontUnicode.h"
#include "XText.h"
#include "XEdit.h"
#include "XSlider.h"
namespace XE{
#define TIMELINE_HEIGHT (32.0f)	//时间线的高度
#define TIMELINE_SLD_WIDTH (32.0f)	//滑动条的宽度
#define TIMELINE_TITLE_LEN (128.0f)	//时间线的标题长度
#define TIMELINE_FRAME_WIDTH (10.0f)	//时间线上一帧的像素宽度
#define DEFAULT_TIMELINE_NAME "timeLine.xml"

//可以考虑的优化
//1、鼠标拖动关键帧
//2、时间线有不可编辑的属性
//3、播放时活动条跟进			*
//4、帧的操作，插入和填充，删除和清空 
//5、片选帧，片选的
//问题
//1、当执行帧率很低时，被跨越过的帧该如何处理？
enum XFrameType
{//帧的类型
	FRAME_TYPE_KEY,		//关键帧
	FRAME_TYPE_INTER,	//补间帧
	FRAME_TYPE_BLANK,	//空白帧
	FRAME_TYPE_LAST,	//持续帧
};
//时间线所支持的数据需要从这个类继承
class XFrameDataBasic
{
protected:
	int type;
public:
	void * pData;
	XFrameDataBasic()
		:pData(NULL)
		,type(0)
	{}
	virtual ~XFrameDataBasic(){}

	virtual XBool writeToXml(TiXmlElement * node) = 0;
	virtual XBool readFromXml(TiXmlNode * node) = 0;
};
extern XFrameDataBasic * createFrameDataBasic(TiXmlNode * node);
struct XOneFrame
{//这个帧拥有对数据的处理权限，帧被删除时数据也会被释放掉
	int frameIndex;				//帧的序号
	int time;					//毫秒单位的时间未知
	int timeLen;				//帧的持续时长,单位毫秒
	XMoveDataMode interpolationMode;			//插值模式
	XFrameDataBasic *pData;					//当前关键帧的数据
	bool overFlag;				//是否已经被时间线跨越过的标志
	XOneFrame()
		:time(0)
		,interpolationMode(MD_MODE_LINE)	//默认为线性插值
		,pData(NULL)
		,overFlag(false)
		,timeLen(0)
	{}
};
class XOneTimeLine
{
public:
	XFontUnicode m_title;
	XVec2 m_position;
	XVec2 m_scale;
	int m_startIndex;	//开始的时间
	int m_width;		//显示的宽度
	float m_frameTimeWidth;	//每一帧的时间宽度 单位为毫秒

	XBool m_isInited;
public:
	void setPosition(const XVec2& p);
	void setScale(const XVec2& s);
	void setCurStartIndex(int index){m_startIndex = index;}
	std::string m_name;	//时间线的名字
	std::vector<XOneFrame> m_keyFrames;	//所有的帧的描述
	XFrameType getFrameType(int index);		//计算当前时间的帧类型
	void setFps(int fps);
	//这里需要提供一个函数用于获取当前的状态
	//float m_getCurFrameData(int time);	//考虑到数据类型，这里尚未考虑好如何实现

	XBool init(const std::string& name,int width,const XVec2& position,const XVec2& scale);
	void draw();
	XOneTimeLine()
		:m_isInited(XFalse)
		,m_frameTimeWidth(33.33f)
		,m_width(256)
		,m_startIndex(0)
	{}
	void addAKeyFrame(int time,int timeLen,XFrameDataBasic * data,XMoveDataMode mode = MD_MODE_NULL);//插入一个关键帧
	void delAKeyFrame(int time);	//删除一个时间点的关键帧
	void moveKeyFrame(int fromTime,int toTime);	//讲一个时间点的帧移动到
	void clearAllFrame();	//清除所有的帧
	~XOneTimeLine(){clearAllFrame();}

	const XOneFrame *getCurKeyFrame(float time);	//当前所在的关键帧
	const XOneFrame *getNextKeyFrame(float time);	//获取下一个关键帧的指针
	void resetAllFrameFlag();
	void setAllFrameFlag();
	void resetAllFrameFlag(int from,int to);	//设置from到to之间的关键帧
	void setAllFrameFlag(int from,int to);
	void resetAllFrameFlagBefore(int time);		//设置time之前的所有时间帧
	void setAllFrameFlagBefore(int time);
	void resetAllFrameFlagAfter(int time);		//设置time之后的所有时间帧
	void setAllFrameFlagAfter(int time);
};
class XTimeLines
{
private:
	int m_objectID;

	XVec2 m_position;
	XVec2 m_scale;
	float m_width;

	std::vector<XOneTimeLine *> m_timeLines;	//时间线

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是工具条的定义
	XText m_curTimeTxt;
	XText m_fpsTxt;
	XEdit m_fpsEdt;
	XText m_allTimeTxt;	//总时长
	XEdit m_allTimeEdt;
	XButton m_playOrStopBtn;	//播放和停止按钮
	XButton m_headBtn;		//回到头的按钮
	XButton m_endBtn;		//回到尾的按钮
	XSlider m_timeSld;		//时间帧的滑动条
	XSlider m_timeLineSld;	//时间线的纵向滑动条
	int m_showTimeLineSum;	//可以显示的时间线的数量
	int m_curStartTimeLineIndex;	//当前显示的最上面一条时间线的编号
	//-------------------------------------------------------

	int m_curStartIndex;	//当前时间线开始的帧编号
	int m_canShowFrameSum;	//可以显示的时间帧数量

	int m_fps;
	float m_frameTime;	//一帧的时间宽度，单位为ms
	int m_allTime;	//总的时间长度，单位ms
	int m_curTimer;	//当前的时间，单位为ms
	int m_curChooseIndex;	//当前选择的是那条时间线

	XBool m_isInited;
	XBool m_visible;	//是否可见
	bool m_isPlaying;
	//++++++++++++++++++++++++++++++++++++++
	//下面是为了实现鼠标拖拽关键帧而定义的变量
	bool m_isMouseDrag;		//是否鼠标拖拽
	bool m_isMouseMove;		//鼠标是否移动
	int m_mouseDragTLIndex;	//鼠标拖拽的时间线编号
	int m_mouseDragKFTime;	//选择的关键帧的时间
	//--------------------------------------
	float m_canShowTime;	//可以显示的总时长

	struct XRuleInfo
	{//标尺的信息
		int ruleData;	//标尺的数据
		XVec2 rulePosition;
		float offset;
	};
	XFontUnicode m_ruleFont;	//标尺的文字
	std::vector<XRuleInfo> m_ruleInfos;	//标尺的信息

//	bool m_needShowRuleHead;	//是否需要显示头部的标尺
//	bool m_needShowRuleMiddle;	//是否需要显示头部的标尺
//	bool m_needShowRuleEnd;	//是否需要显示头部的标尺
//	XFontUnicode m_ruleHead;	//头部的标尺
//	float m_ruleHeadOffset;
//	XFontUnicode m_ruleMiddle;	//中部的标尺
//	float m_ruleMiddleOffset;
//	XFontUnicode m_ruleEnd;	//尾部的标尺
//	float m_ruleEndOffset;

	static void ctrlProc(void*,int,int);
public:
	int getCurChooseIndex() const {return m_curChooseIndex;}
	void play();	//开始播放
	void stop();	//停止播放
	bool getIsPlaying() const {return m_isPlaying;}
	void setCurTimer(int curTime) {m_curTimer = curTime;}
private:
	void updateCtrlsPosition();
	void updateRule();	//根据当前的实际情况跟新当前标尺的状态

	void (*m_funTimeChange)(void *,int ID);	//当前时间发生变化时调用
	void (*m_funPlay)(void *,int ID);	//当前时间发生变化时调用
	void (*m_funStop)(void *,int ID);	//当前时间发生变化时调用
	void *m_pClass;
public:
	void setTimeChangeCB(void (* funTimeChange)(void *,int),
		void (* funPlay)(void *,int),
		void (* funStop)(void *,int),
		void *pClass = NULL)
	{
		m_funTimeChange = funTimeChange;
		m_funPlay = funPlay;
		m_funStop = funStop;
		m_pClass = pClass;
	}
	void setPosition(const XVec2& p);
	void setSize(const XVec2& s);
	void setCurTimeSlider(float p);	//设置当前的百分比位置
	void setCurTimeLineSlider(float p);	//设置当前的百分比位置
	void setToPrev();	//向前走一页
	void setToNext();	//向后走一页
	//下面是关于时间线的操作
	XBool addATimeLine(const std::string& name);
	//向指定的时间线上增加关键帧
	XBool addKeyFrame(const std::string& name,int time,int timeLen,XFrameDataBasic * data,XMoveDataMode mode = MD_MODE_NULL);
	//向指定的时间线上增加关键帧
	XBool addKeyFrame(unsigned int index,int time,int timeLen,XFrameDataBasic * data,XMoveDataMode mode = MD_MODE_NULL);
	XBool delCurKeyFrame(const std::string& name);	//删除当前时间的关键帧，如果当前时间没有关键帧则不删除
	XBool delCurKeyFrame(unsigned int index);	//删除当前时间的关键帧，如果当前时间没有关键帧则不删除
	XBool clearAllFroms(const std::string& name);
	XBool clearAllFroms(unsigned int index);
	void clearAllFroms();
	//下面是鼠标操作的函数
	XBool mouseProc(const XVec2& p,XMouseState mouseState);		//对于鼠标动作的响应函数
	int getTimeLineIndexByName(const std::string& name);
	void setFps(int fps);
	int getFps() const {return m_fps;}
	void setAllTime(int time);
	int getAllTime() const {return m_allTime;}
	XBool init(float width,int showLineSum,int fps = 30);
	void draw();
	void move(float stepTime);
	void release();
	XTimeLines()
		:m_isInited(XFalse)
		,m_fps(30)
		,m_curStartIndex(0)
		,m_allTime(6000)
		,m_curTimer(0)
		,m_isPlaying(false)
		,m_curChooseIndex(-1)
		,m_showTimeLineSum(3)
		,m_curStartTimeLineIndex(0)
//		,m_needShowRuleHead(false)
//		,m_needShowRuleMiddle(false)
//		,m_needShowRuleEnd(false)
//		,m_ruleHeadOffset(0.0f)
//		,m_ruleMiddleOffset(0.0f)
//		,m_ruleEndOffset(0.0f)
		,m_funTimeChange(NULL)
		,m_funPlay(NULL)
		,m_funStop(NULL)
		,m_pClass(NULL)
		,m_objectID(100)
		,m_isMouseDrag(false)
		,m_visible(true)
	{}
	~XTimeLines(){release();}
	int getControlID() const{return m_objectID;}

	//下面是预计的一些接口
	XBool saveToFile(const char * filename = NULL);
	XBool readFromFile(const char * filename = NULL);
	int getCurTimer() const {return m_curTimer;}	//获取当前的时间

	const XOneFrame *getCurKeyFrame(const std::string& name);	//获取当前所在的关键帧
	const XOneFrame *getNextKeyFrame(const std::string& name);	//获取下一个关键帧的指针
	const XOneFrame *getCurKeyFrame(unsigned int index);		//获取当前所在的关键帧
	const XOneFrame *getNextKeyFrame(unsigned int index);		//获取下一个关键帧的指针
	void resetAllTimeLineFrameFlag();					//重置所有的时间帧
	void setAllTimeLineFrameFlag();						//设置所有的时间帧
	void resetAllTimeLineFrameFlag(int index);			//设置指定的时间帧
	void resetAllTimeLineFrameFlag(const std::string& name);	//设置指定的时间帧
	void setAllTimeLineFrameFlag(int index);			//设置指定的时间帧
	void setAllTimeLineFrameFlag(const std::string& name);		//设置指定的时间帧

	void setVisible()
	{
		m_visible = XTrue;
		m_curTimeTxt.setVisible();
		m_fpsTxt.setVisible();
		m_fpsEdt.setVisible();
		m_allTimeTxt.setVisible();	//总时长
		m_allTimeEdt.setVisible();
		m_playOrStopBtn.setVisible();	//播放和停止按钮
		m_headBtn.setVisible();		//回到头的按钮
		m_endBtn.setVisible();		//回到尾的按钮
		m_timeSld.setVisible();		//时间帧的滑动条
		m_timeLineSld.setVisible();	//时间线的纵向滑动条
	}
	void disVisible()
	{
		m_visible = XFalse;
		m_curTimeTxt.disVisible();
		m_fpsTxt.disVisible();
		m_fpsEdt.disVisible();
		m_allTimeTxt.disVisible();	//总时长
		m_allTimeEdt.disVisible();
		m_playOrStopBtn.disVisible();	//播放和停止按钮
		m_headBtn.disVisible();		//回到头的按钮
		m_endBtn.disVisible();		//回到尾的按钮
		m_timeSld.disVisible();		//时间帧的滑动条
		m_timeLineSld.disVisible();	//时间线的纵向滑动条
	}
	XBool getVisible() const {return m_visible;}
};
#if WITH_INLINE_FILE
#include "XTimeLine.inl"
#endif
}
#endif