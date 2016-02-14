#ifndef _JIA_XMOVEDATA_
#define _JIA_XMOVEDATA_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.3
//--------------------------------
#include "../XOSDefine.h"
//#include "stdio.h"
//#include "XMath.h"
namespace XE{
//下面是一个数据动态变化的类，可以用于实现一些动态的曲线以及动态的效果
enum XMoveDataMode
{
	MOVE_DATA_MODE_NULL,		//无效的数据
	MOVE_DATA_MODE_LINE,		//线性变化
	MOVE_DATA_MODE_SIN,			//sin曲线变化,	先快后慢		计算消耗:中
	MOVE_DATA_MODE_SIN_MULT,	//sin曲线变化,	先快后慢		计算消耗:小
	MOVE_DATA_MODE_SQRT2,		//4次开方,		先快后慢		计算消耗:大
	MOVE_DATA_MODE_SQRT2_MULT,	//使用乘方实现,	先快后慢		计算消耗:中
	MOVE_DATA_MODE_COS,			//cos曲线变化,	先慢后快		计算消耗:中
	MOVE_DATA_MODE_COS_MULT,	//cos曲线变化,	先慢后快		计算消耗:小
	MOVE_DATA_MODE_DSIN,		//sin曲线变化,	先快后慢再快	计算消耗:中	
	MOVE_DATA_MODE_DCOS,		//cos曲线变化,	先慢后快再慢	计算消耗:中
	MOVE_DATA_MODE_DSIN_MULT,	//sin曲线变化,	先快后慢再快	计算消耗:小	
	MOVE_DATA_MODE_DCOS_MULT,	//cos曲线变化,	先慢后快再慢	计算消耗:小
	MOVE_DATA_MODE_SHAKE,		//衰减震荡曲线，先快后慢(尚未实现)
	MOVE_DATA_MODE_MAX,
};

class XMoveData
{
private:
	float m_startData;	//起始数值
	float m_endData;	//终止数值
	float m_curData;	//当前的数值
	float m_speed;
	float m_timer;		//时间标尺
	int m_loopTimer;	//循环的次数
	
	XBool m_isEnd;	//运动是否结束1:结束 0:运动中
	int m_isLoop;	//是否循环,这里是镜像循环,以及循环次数
	XMoveDataMode m_mode;
public:
	float getStartData() const {return m_startData;}
	float getEndData() const {return m_endData;}
	float getSpeed() const {return m_speed;}
	int getLoopTimer() const {return m_isLoop;}
	XMoveDataMode getMoveMode() const {return m_mode;}
public:
	//isLoop:0,不循环,>0循环的次数,<0无限循环
	void set(float start,float end,float speed,XMoveDataMode mode = MOVE_DATA_MODE_SIN_MULT,int isLoop = 0);
	//time为动作的持续时间
	void setEx(float start,float end,float time,XMoveDataMode mode = MOVE_DATA_MODE_SIN_MULT,int isLoop = 0)
	{
		set(start,end,1.0f / time * 0.001f,mode,isLoop);
	}
	void reset();	//设置重新开始
	void move(float delay);
	float getValue(float time);	//计算time时刻的数值:单位ms
	float getCurData() const {return m_curData;}
	float getCurTimer() const {return m_timer;}
	XBool getIsEnd() const {return m_isEnd;}
	void setEnd();
	XMoveData()
		:m_startData(0.0f)
		,m_endData(0.0f)
		,m_curData(0.0f)
		,m_isEnd(XTrue)
		,m_mode(MOVE_DATA_MODE_SIN)
		,m_isLoop(0)
	{}
	void getParamStr(char * str);
};
#if WITH_INLINE_FILE
#include "XMoveData.inl"
#endif
}
#endif