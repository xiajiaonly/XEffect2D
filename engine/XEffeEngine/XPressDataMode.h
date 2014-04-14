#ifndef _JIA_PRESSDATAMODE_
#define _JIA_PRESSDATAMODE_
//++++++++++++++++++++++++++++++++
//Author:	  贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2013.4.10
//--------------------------------
//这是一个压感数据模型，就是当压下时数据开始变化，当释放时数据开始恢复
//可以分为单向压感模型和双向压感模型
#include "XOSDefine.h"

enum _XPressState
{
//	PRESS_STATE_LEFT,	//正向压下	
//	PRESS_STATE_RIGHT,	//反向压下
	PRESS_STATE_PRESS,	//用于单向压感模型
	PRESS_STATE_NORMAL,	//释放
};
class _XPressDataModeS	//单向压感模型
{
private:
	float m_maxDeformData;		//最大变形数据
	float m_normalData;			//还原数据
	float m_nowDeformData;		//当前的变形数据
	float m_nowData;			//当前的数据
	_XPressState m_pressState;	//压力状态
	//单向压感是必须能恢复的
	//_XBool m_canRecover;		//是否自动恢复
	int m_sensitivity;			//灵敏度	
	int m_nowPressTimer;		//当前的计时器

	float m_startChangeData;	//单次变化的数据
	float m_minChangeData;	//最小的变化数据
	float m_maxChangeData;	//最大的变化数据
	//_XBool m_isRecover;		//数据是否完全恢复
	float m_dataFlag;		//数据变换的符号
public:
	void init(float normalValue,float maxDeformValue,int sensitivity);
	void setParam(float startChange,float minChange,float maxChange)
	{
		m_startChangeData = startChange;
		m_minChangeData = minChange;
		m_maxChangeData = maxChange;
	}
	void setPress();
	void setNormal(){m_pressState = PRESS_STATE_NORMAL;}
	void move(int stepTime);
	float getNowData() const {return m_nowData;}
	_XBool isPress() const 
	{
		if(m_pressState != PRESS_STATE_NORMAL) return XTrue;
		else return XFalse;
	}
	//下面这个函数尚未进行深度测试
	void setNowData(float data)
	{//范围检查
		if(m_normalData > m_maxDeformData)
		{
			if(data < m_maxDeformData) data = m_maxDeformData; 
			if(data > m_normalData) data = m_normalData; 
		}else
		{
			if(data > m_maxDeformData) data = m_maxDeformData; 
			if(data < m_normalData) data = m_normalData; 
		}
		m_nowData = data;
	}

};
class _XPressDataModeD	//双向压感模型
{
private:
	float m_minDeformData;		//最小变形数据
	float m_maxDeformData;		//最大变形数据
	float m_normalData;			//还原数据
	float m_nowDeformData;		//当前的变形数据
	float m_nowData;			//当前的数据
	_XPressState m_pressMinState;	//压力状态
	_XPressState m_pressMaxState;	//压力状态
	//单向压感是必须能恢复的
	char m_canRecover;		//是否自动恢复0:不回复，1:缓慢恢复，2立即恢复
	int m_sensitivity;			//灵敏度	
	int m_nowPressTimer;		//当前的计时器

	float m_startChangeData;	//单次变化的数据
	float m_minChangeData;	//最小的变化数据
	float m_maxChangeData;	//最大的变化数据
	_XBool m_isRecover;		//数据是否完全恢复
public:
	void init(float normalValue,float maxDeformValue,float minDeformValue,int sensitivity,char canRecover = 1);
	void setParam(float startChange,float minChange,float maxChange)
	{
		m_startChangeData = startChange;
		m_minChangeData = minChange;
		m_maxChangeData = maxChange;
	}
	void setPressMax();
	void setPressMin();
	void setNormalMin(){m_pressMinState = PRESS_STATE_NORMAL;}
	void setNormalMax(){m_pressMaxState = PRESS_STATE_NORMAL;}
	void move(int stepTime);
	float getNowData() const {return m_nowData;}
	_XBool isPress() const 
	{
		if(m_pressMinState != PRESS_STATE_NORMAL || m_pressMaxState != PRESS_STATE_NORMAL) return XTrue;
		else return XFalse;
	}
	//下面这个函数尚未进行深度测试
	void setNowData(float data)
	{
		if(data < m_minDeformData) data = m_minDeformData; 
		if(data > m_maxDeformData) data = m_maxDeformData; 
		m_nowData = data;
		m_isRecover = XFalse;
	}
};
#endif