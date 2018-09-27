#ifndef _JIA_XDATAGUESS_
#define _JIA_XDATAGUESS_
#include <deque>
#include "XMath\XVector2.h"
//#define WITH_DEBUG_INFO //是否显示调试信息
namespace XE{
//该类用于对于输入的(x,y)的数据进行分析以便于对未给出的x计算出相应的y使得整个数据平滑
enum XGuessModle
{
	GUESS_MODLE_LINE,		//线性猜测
	GUESS_MODLE_SECOND,		//代加速度的猜测
	GUESS_MODLE_LINE_FIT,	//线性猜测对新数据进行修正	//这个结果较为理想
	GUESS_MODLE_SECOND_FIT,	//代加速度的猜测
	//这是针对于《机械的眼睛》项目而添加的功能
	GUESS_MODLE_PERIODIC,	//周期性重复的数据，这个模式可以使得重复的周期越长久，数据月准确
};
struct XCycleInfo
{
	float m_maxValue;	//周期的最大值
	int m_maxValueSum;	
	float m_minValue;	//周期的最小值
	int m_minValueSum;
	float m_upTime;		//周期的上升时间
	int m_upTimeSum;
	float m_downTime;	//周期的下降时间
	int m_downTimeSum;
	float m_cycleTime;	//平均周期	尚未实现
	float m_upDownRate;	//升降比，上升阶段与下降阶段所占时间的比例 尚未自动计算
	XCycleInfo()
		:m_minValue(0.0f)
		, m_minValueSum(0)
		, m_maxValue(0.0f)
		, m_maxValueSum(0)
		, m_upTime(0.0f)
		, m_upTimeSum(0)
		, m_downTime(0.0f)
		, m_downTimeSum(0)
		, m_cycleTime(0.0f)
		, m_upDownRate(0.71f)
	{}
	bool getIsEnable()const{return m_maxValueSum > 0;}
	void addAMaxValue(float maxV);
	void addAMinValue(float minV);
	void addAUpTime(float upTime);
	void addADownTime(float upTime);
	void reset();
	float getAmplitude()const{return m_maxValue - m_minValue;}
};
enum XCycleState
{
	CYC_STA_NULL,	//未知的状态
	CYC_STA_UP,		//上升阶段
	CYC_STA_DOWN,	//下降阶段
};
class XCycleData
{
private:
	bool m_haveStart;	//是否找到周期起点
	bool m_haveEnd;		//是否找到周期终点
	bool m_haveTop;		//是否找到周期顶点
	float m_startTime;	//开始的时间
	float m_topTime;	//顶点的时间
	float m_endTime;	//结束的时间
	XCycleState m_state;
	float m_maxValue;	//周期内的最大值
	float m_minValue;	//周期内的最小值
	float m_upTime;		//上升的时间
	float m_downTime;	//下降的时间

	//由于数据震荡造成下面两个算法目前存在严重的问题
	bool calStart(const XCycleInfo &info);	//计算开头
	bool calEnd();		//计算结束
	bool calTop();

	int m_upDatasSum;	//上升数据的次数
	int m_downDatasSum;	//下降数据的次数
	float m_startValue;	//开始的数值
	float m_endValue;	//结束的数值
	//默认为1，数值越大可以抵抗更大的数据震荡
	int m_turnThreshold;	//判断结束或者是开始的连续状态阈值
	int m_startEndIndex;	//开始位置结束的编号，结束为止需要从这个位置之后开始判断
	int m_startIndex;
	int m_topIndex;
	int m_endIndex;
public:
	XCycleData()
		:m_haveStart(false)
		, m_haveEnd(false)
		, m_haveTop(false)
		, m_startTime(0.0f)
		, m_endTime(0.0f)
		, m_state(CYC_STA_NULL)
		, m_upDatasSum(0)
		, m_downDatasSum(0)
		, m_turnThreshold(1)
	{}
	void setTurnThreshold(int t)
	{
		if(t >= 1) m_turnThreshold = t;
	}
	std::deque<XVec2> m_datas;
	void pushAData(const XE::XVec2& data,const XCycleInfo &info);
	bool getIsComplete()const{return m_haveEnd;} 
	bool getHaveHead()const{return m_haveStart;}
	float getMinValue()const{return m_minValue;}
	float getMaxValue()const{return m_maxValue;}
	float getUpTime()const{return m_upTime;}
	float getDownTime()const{return m_downTime;}
	int getUpDatasSum()const{return m_upDatasSum;}
	int getDownDatasSum()const{return m_downDatasSum;}
	float getStartValue()const{return m_startValue;}
	float getEndValue()const{return m_endValue;}
	void clear();
	//x为相对于周期起始的时间
	bool getValue(float x,float &y)const;
	float getStartTime()const{return m_startTime;}	//获取周期起始的时间
};
class XDataGuess
{
private:
	std::deque<XVec2> m_datas;
	XGuessModle m_mode;
	//用于周期性的猜测，这是针对于《机械的眼睛》项目而添加的功能
	XCycleData m_cycDatas;	//所有的数据
	XCycleData m_cycDatasTmp;
	std::deque<XCycleData> m_oldCycData;
	XDataGuess *m_pDG;
	bool m_isInited;
	XCycleInfo m_cycleInfo;
	bool m_isCycDataUpdate;
	//下面是一个非常个性化的函数，建议通过回调的方式让外部实现其逻辑
	//bSimple //是否为起始地几个数据
	bool getIsInvalid(const XCycleData &data,bool bSimple = false)const;	//返回数据是否无效 
public:
	XDataGuess()
		:m_mode(GUESS_MODLE_LINE)
		, m_pDG(NULL)
		, m_isInited(false)
	{}
	~XDataGuess(){release();}
	//这个方法必须在初始化的时候调用
	bool init(XGuessModle mode);
	bool getIsCycDataUpdated();
	const std::deque<XCycleData> &getOldCycDatas()const{return m_oldCycData;}
	//需要注意推入的数据必须要按照x拍好顺序的
	void pushAData(const XVec2& data);
	float guessData(float x)const;
	bool getCanGuess()const;	//返回是否可以进行估算
	void release();
	void setTurnThreshold(int t)
	{
		m_cycDatas.setTurnThreshold(t);
		m_cycDatasTmp.setTurnThreshold(t);
	}
};
#if WITH_INLINE_FILE
#include "XDataGuess.inl"
#endif
}
#endif