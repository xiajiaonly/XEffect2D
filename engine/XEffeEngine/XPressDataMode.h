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
#include "XInputEventCore.h"

enum _XPressState
{
//	PRESS_STATE_LEFT,	//正向压下	
//	PRESS_STATE_RIGHT,	//反向压下
	PRESS_STATE_PRESS,	//用于单向压感模型
	PRESS_STATE_NORMAL,	//释放
};
class _XPressDataSingle	//单向压感模型
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
	_XPressDataSingle()
		:m_startChangeData(1.0f)
		,m_minChangeData(0.01f)
		,m_maxChangeData(0.1f)
	{}
	//normalValue:普通状态下得值
	//maxDeformValue:最大变形状态下的值
	//sensitivity:灵敏度(单位ms),多少时间之后开始变形
	void init(float normalValue,float maxDeformValue,int sensitivity);
	void setParam(float startChange,float minChange,float maxChange)
	{
		m_startChangeData = startChange;	//第一次按下时跳跃的值
		m_minChangeData = minChange;	//变化的最慢速度
		m_maxChangeData = maxChange;	//变化的最快速度
	}
	void setPress();	//设置处于按压状态
	void setRelease(){m_pressState = PRESS_STATE_NORMAL;}	//设置处于释放状态
	void move(int stepTime);
	float getNowData() const {return m_nowData;}
	_XBool isPress() const {return m_pressState != PRESS_STATE_NORMAL;}
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
//压感数据的回复模式
enum _XPressDataRecoverMode
{
	MODE_NO_RECOVER,	//不会回复形变
	MODE_SLOW_RECOVER,	//缓慢回复
	MODE_IMM_RECOVER,	//立即回复
};
class _XPressDataDouble	//双向压感模型
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
	_XPressDataRecoverMode m_recoverMode;		//是否自动恢复0:不回复，1:缓慢恢复，2立即恢复
	int m_sensitivity;			//灵敏度	
	int m_nowPressTimer;		//当前的计时器

	float m_startChangeData;	//单次变化的数据
	float m_minChangeData;	//最小的变化数据
	float m_maxChangeData;	//最大的变化数据
	_XBool m_isRecover;		//数据是否完全恢复

	_XKeyValue m_minKey;
	_XKeyValue m_maxKey;
public:
	//normalValue:普通状态的值
	//maxDeformValue:变形的最大值
	//minDeformValue:变形的最小值
	//sensitivity;灵敏系数，单位 ms
	void init(float normalValue,float maxDeformValue,float minDeformValue,
		int sensitivity,_XPressDataRecoverMode recoverMode = MODE_SLOW_RECOVER);
	void setParam(float startChange,float minChange,float maxChange)
	{
		m_startChangeData = startChange;
		m_minChangeData = minChange;
		m_maxChangeData = maxChange;
	}
	void move(int stepTime);
	float getNowData() const {return m_nowData;}
	_XBool isPress() const {return (m_pressMinState != PRESS_STATE_NORMAL || 
		m_pressMaxState != PRESS_STATE_NORMAL);}
	//下面这个函数尚未进行深度测试
	void setNowData(float data)
	{
		if(data < m_minDeformData) data = m_minDeformData; 
		if(data > m_maxDeformData) data = m_maxDeformData; 
		m_nowData = data;
		m_isRecover = XFalse;
	}
	_XPressDataDouble()
		:m_startChangeData(1.0f)
		,m_minChangeData(0.01f)	//这个数值需要可变
		,m_maxChangeData(0.1f)		//这个数值需要可变
		,m_minKey(XKEY_LEFT)
		,m_maxKey(XKEY_RIGHT)
	{}
	//工作模式1：手动模式
	void setPressMax();
	void setPressMin();
	void setReleaseMin(){m_pressMinState = PRESS_STATE_NORMAL;}
	void setReleaseMax(){m_pressMaxState = PRESS_STATE_NORMAL;}
	//工作模式2：关联按键模式
	void setKeyValue(_XKeyValue min,_XKeyValue max)
	{
		m_minKey = min;
		m_maxKey = max;
	}
	void updateState(const _XInputEvent &e)
	{
		if(e.type == EVENT_KEYBOARD)
		{
			if(e.keyState == KEY_STATE_DOWN)
			{
				if(e.keyValue == m_minKey) setPressMin(); else
				if(e.keyValue == m_maxKey) setPressMax();
			}else
			if(e.keyState == KEY_STATE_UP)
			{
				if(e.keyValue == m_minKey) setReleaseMin(); else
				if(e.keyValue == m_maxKey) setReleaseMax();
			}
		}
	}
};

class _XKeyPressModel
{
private:
	bool m_isInited;
	int m_sensitivity;			//灵敏度,开始累积的时间,单位ms
	int m_pressSpeed;			//连续有效的时间差，单位ms
	_XKeyValue m_keyValue;		//对应的按键值
	bool m_isPress;			//是否按下
	bool m_isKeyDown;
	int m_timer;
public:
	_XKeyPressModel()
		:m_isInited(false)
		,m_sensitivity(500)
		,m_pressSpeed(100)
		,m_keyValue(XKEY_LEFT)
	{}
	bool init(_XKeyValue key,int sensitivity = 500,int pressSpeed = 100)
	{
		m_keyValue = key;
		m_sensitivity = sensitivity; 
		m_pressSpeed = pressSpeed;

		m_isPress = false;
		m_isKeyDown = false;
		m_timer = 0;
		m_isInited = true;
		return true;
	}
	bool move(int stepTime)	//返回是否触发按下时间
	{
		if(!m_isInited) return false;
		if(m_isKeyDown)
		{
			m_timer += stepTime;
			if(m_timer >= m_sensitivity)
			{//下面产生连续按下时间
				m_isPress = true;
				m_timer -= m_pressSpeed;
			}
		}
		if(m_isPress)
		{
			m_isPress = false;
			return true;
		}
		return false;
	}
	void updateState(const _XInputEvent &e)
	{
		if(!m_isInited) return;
		if(e.type == EVENT_KEYBOARD && e.keyValue == m_keyValue)
		{
			if(e.keyState == KEY_STATE_DOWN)
			{
				m_isPress = true;
				m_isKeyDown = true; 
				m_timer = 0;
			}else
			if(e.keyState == KEY_STATE_UP)
			{
				m_isPress = false;
				m_isKeyDown = false; 
			}
		}
	}
};
#endif