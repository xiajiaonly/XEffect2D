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
	MD_MODE_NULL,		//无效的数据
	MD_MODE_LINE,		//线性变化
	MD_MODE_SIN,		//sin曲线变化,	先快后慢		计算消耗:中
	MD_MODE_SIN_MULT,	//sin曲线变化,	先快后慢		计算消耗:小
	MD_MODE_SQRT2,		//4次开方,		先快后慢		计算消耗:大
	MD_MODE_SQRT2_MULT,	//使用乘方实现,	先快后慢		计算消耗:中
	MD_MODE_COS,		//cos曲线变化,	先慢后快		计算消耗:中
	MD_MODE_COS_MULT,	//cos曲线变化,	先慢后快		计算消耗:小
	MD_MODE_DSIN,		//sin曲线变化,	先快后慢再快	计算消耗:中	
	MD_MODE_DCOS,		//cos曲线变化,	先慢后快再慢	计算消耗:中
	MD_MODE_DSIN_MULT,	//sin曲线变化,	先快后慢再快	计算消耗:小	
	MD_MODE_DCOS_MULT,	//cos曲线变化,	先慢后快再慢	计算消耗:小
	MD_MODE_SHAKE,		//衰减震荡曲线，先快后慢
	MD_MODE_SHAKE_EX,	//衰减震荡曲线，先快后慢
	//下面的方式从别处直接引用
	MD_MODE_EIEO,
	MD_MODE_EI,
	MD_MODE_EO,
	MD_MODE_EL,
	MD_MODE_LL,
	MD_MODE_VLL,
	MD_MODE_TANH,
	MD_MODE_SINH,
	MD_MODE_SQUARE,

	MD_MODE_B5,
	MD_MODE_B3,
	MD_MODE_B2,
	MD_MODE_BAF1,
	MD_MODE_BAF2,
	MD_MODE_BAF3,
	MD_MODE_LS,
	MD_MODE_ES,
	MD_MODE_LEIEO,
	MD_MODE_VLEIEO,
	MD_MODE_QEI,
	MD_MODE_QEO,
	MD_MODE_EQEO,
	MD_MODE_CEI,
	MD_MODE_CEO,
	MD_MODE_QUEI,
	MD_MODE_QUEO,
	MD_MODE_BOUNCY,
	//后面都是较复杂的
	MD_MODE_QBP,
	MD_MODE_ESP,
	MD_MODE_CBP,
	MD_MODE_CB2P,
	MD_MODE_SG,
	MD_MODE_OD,
	MD_MODE_EIB,
	MD_MODE_EOB,
	MD_MODE_EOIB,
	MD_MODE_EIOB,

	MD_MODE_EIBo,
	MD_MODE_EOBo,
	MD_MODE_EIOBo,
	MD_MODE_EOIBo,

	MD_MODE_EIE,
	MD_MODE_EOE,
	MD_MODE_EIOE,
	MD_MODE_EOIE,

	MD_MODE_BIC,	//参考ofxAnimatable
	MD_MODE_BOC,	//参考ofxAnimatable

	MD_MODE_SS,
	MD_MODE_SES,

	MD_MODE_MAX,
};
struct XMoveDataParam
{
	float p1;
	float p2;
	float p3;
	float p4;
	XMoveDataParam()
		:p1(0.05f)
		, p2(0.0f)
		, p3(0.0f)
		, p4(0.0f)
	{}
};
#define BOUNCES_MAX	20
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
	void initParam();	//初始化参数表
	float bounceElast;
	float bounceDuration[BOUNCES_MAX];
	float bounceVelocity[BOUNCES_MAX];
	void initCustomBounce();
public:
	void setBounceElast(float tmp)
	{
		bounceElast = tmp;
		initCustomBounce();
	}
public:
	XMoveDataParam m_param;	//曲线的参数，这个参数直接暴露以便于外部修改
	float getValueEx(float percentage);	//范围0.0f - 1.0f
	float getStartData() const {return m_startData;}
	float getEndData() const {return m_endData;}
	float getSpeed() const {return m_speed;}
	int getLoopTimer() const {return m_isLoop;}
	XMoveDataMode getMoveMode() const {return m_mode;}
public:
	//isLoop:0,不循环,>0循环的次数,<0无限循环
	void set(float start,float end,float speed,XMoveDataMode mode = MD_MODE_SIN_MULT,int isLoop = 0);
	//time为动作的持续时间
	void setEx(float start,float end,float time,XMoveDataMode mode = MD_MODE_SIN_MULT,int isLoop = 0)
	{
		set(start,end,1.0f / time * 0.001f,mode,isLoop);
	}
	void reset();	//设置重新开始
	void move(float delay);
	float getValue(float time)	//计算time时刻的数值:单位ms
	{
		if(time < 0.0f) time = 0.0f;
		float curTime = time * m_speed;
		if(m_isLoop == 0)
		{//如果不循环
			if(curTime > 1.0f) curTime = 1.0f;
		}else
		{//循环
			curTime = fmod(curTime,2.0f);
			if(curTime > 1.0f)
				curTime = 2.0f - curTime;
		}
		return getValueEx(curTime);
	}
	float getCurData() const {return m_curData;}
	float getCurTimer() const {return m_timer;}
	void setCurTimer(float t);
	XBool getIsEnd() const {return m_isEnd;}
	void setEnd();
	XMoveData()
		:m_startData(0.0f)
		, m_endData(0.0f)
		, m_curData(0.0f)
		, m_isEnd(XTrue)
		, m_mode(MD_MODE_SIN)
		, m_isLoop(0)
	{}
	void getParamStr(char * str);
	std::string getModeStr();
};
#if WITH_INLINE_FILE
#include "XMoveData.inl"
#endif
}
#endif