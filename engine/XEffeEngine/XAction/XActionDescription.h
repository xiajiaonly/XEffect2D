#ifndef _JIA_XACTIONDESCRIPTION_
#define _JIA_XACTIONDESCRIPTION_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.5
//--------------------------------
#include "../XObjectBasic.h"
#include "../XNumberEx.h"
//#include "../XBasicFun.h"
#include <vector>
#include "XActionObjectManager.h"
#define ACTION_DES_PATH ("ResourcePack/ActionDescription/")
enum _XActionType
{//动作的类型
	ACTION_TYPE_POSITION,	//位置的变化
	ACTION_TYPE_POSITION_X,	//位置的变化
	ACTION_TYPE_POSITION_Y,	//位置的变化
	ACTION_TYPE_COLOR,		//颜色的变化
	ACTION_TYPE_R,
	ACTION_TYPE_G,
	ACTION_TYPE_B,
	ACTION_TYPE_ALPHA,		//透明度的变化
	ACTION_TYPE_ANGLE,		//角度变化
	ACTION_TYPE_SIZE,		//尺寸的变化
	ACTION_TYPE_SIZE_EX,	//尺寸的变化	//x、y等比例缩放
	ACTION_TYPE_SIZE_X,		//尺寸的变化	
	ACTION_TYPE_SIZE_Y,		//尺寸的变化
	ACTION_TYPE_STAY,		//停留状态
	ACTION_TYPE_MAX,
};
extern string actionTypeStr[];
struct _XActionState
{//物件保持的状态
	_XBool isEnable;
	_XVector2 position;
	_XFColor color;
	float angle;	//[0-360]
	_XVector2 size;
	_XActionState()
		:angle(0.0f)
		,isEnable(XFalse)
	{
	}
};
//状态类型
enum _XActionStateType
{
	ACTION_STATE_TYPE_POS_X,
	ACTION_STATE_TYPE_POS_Y,
	ACTION_STATE_TYPE_COLOR_R,
	ACTION_STATE_TYPE_COLOR_G,
	ACTION_STATE_TYPE_COLOR_B,
	ACTION_STATE_TYPE_COLOR_A,
	ACTION_STATE_TYPE_ANGLE,
	ACTION_STATE_TYPE_SIZE_X,
	ACTION_STATE_TYPE_SIZE_Y,
	ACTION_STATE_TYPE_MAX,
};
struct _XActionStateDescription
{
	int time;	//状态所处的时间点，这个只在AD_TYPE_STATE_LINE时有效
	_XBool isValidPosX;
	_XBool isValidPosY;
	_XBool isValidColorR;
	_XBool isValidColorG;
	_XBool isValidColorB;
	_XBool isValidColorA;
	_XBool isValidAngle;
	_XBool isValidSizeX;
	_XBool isValidSizeY;

	_XVector2 position;
	_XFColor color;
	float angle;	//[0-360]
	_XVector2 size;
	_XActionStateDescription()
		:angle(0.0f)
	{}
};
typedef _XActionStateDescription _XActionStateDes;
//这是一个对动作描述的类
class _XActionCore
{
public:
	_XBool m_isEnable;
	_XActionType m_actionType;
	//std::vector<_XMoveData *> m_moveData;
	_XMoveData *m_moveData;
	int m_moveDataSum;
	_XObjectBasic* m_object;	//动作的对象
	float m_startTime;				//动作起始时间,相对于自身的时间
	float m_endTime;
	float m_timeLine;				//时间线
	_XActionState m_stayState;		//需要保持的状态
	//_XBool m_isStart;			//是否开始
	_XBool m_isActionStart;	//动作开始，数据开始变化
	_XBool m_isEnd;
	_XBool set(float startTime,float endTime,_XActionType type,_XMoveData *md = NULL,_XActionState *state = NULL);
	void move(int delay);
	_XBool getIsEnd() const {return m_isEnd;}
	void setEnd(){m_isEnd = XTrue;}		//设置结束
	//_XBool getIsLoop();
	void setStart();	//设置开始
	_XActionCore()
		:m_isEnable(XFalse)
		,m_isEnd(XTrue)
		,m_startTime(0.0f)
		,m_timeLine(0.0f)
		//,m_isStart(XFalse)
		,m_moveData(NULL)
		,m_moveDataSum(0)
	{
	}
	~_XActionCore()
	{
		release();
	}
	void release();
};
//AD中对动作的描述类型
//enum _XActionDesType
//{
//	AD_TYPE_AC,				//通过ActionCore的方式描述动作
//	AD_TYPE_STATE_TABLE,	//通过状态表的方式描述动作
//};
class _XActionMananger;
class _XActionDescription
{
	friend _XActionMananger;
private:
	int m_ID;
	_XBool m_isEnable;
	std::vector<_XActionDescription *> m_otherAction;	//动作的组成部分
	std::vector<int> m_otherActionID;	//这个用于辅助初始化
	_XObjectBasic* m_object;			//动作的对象
	std::vector<_XActionCore *> m_actionCore;	//动作元
	char m_actionName[MAX_FILE_NAME_LENGTH];			//动作的名称
	float m_startTime;				//动作起始时间
	float m_endTime;				//动作结束时间(貌似没有必要),好像没有用
	float m_timeLine;
	_XBool m_isEnd;
	//_XBool m_isStart;	//是否开始
	_XBool m_isActionStart;	//动作开始，数据开始变化
	//_XActionDesType m_actionDesType;
	//+++++++++++++++++++++++++++++++++++++++++++++
	//下面是对AD_TYPE_STATE_LINE模式的描述（尚未实现）
	//说明这个模式下，AD中存放state的列表，时间帧检测当前处于那两个state之间，
	//然后通过线性插值(可扩展为贝塞尔差值)的方式计算当前的状态
private:
	std::vector<_XActionStateDes *> m_stateTable;	//状态列表,按时间排序
	int m_minTime;
	int m_maxTime;
	float getASValue(int time,_XActionStateType type);
public:
	int getMinTime() {return m_minTime;}	//获得最小的时间点
	int getMaxTime() {return m_maxTime;}	//获得最大的时间点
	int getActionStateDesSum() {return m_stateTable.size();}
	_XActionStateDes* getPreviousASD(int time,_XActionStateType type);	//获得当前时间点的上一个状态描述
	_XActionStateDes* getNextASD(int time,_XActionStateType type);		//获得当前时间点的下一个状态描述
	_XActionStateDes* getASD(int index)
	{
		if(index < 0 || index >= m_stateTable.size()) return NULL;
		return m_stateTable[index];
	}
	_XActionState getActionState(int time);		//获取指定时间的状态(注意这个算法的效率有待优化)
	_XBool insertASD(_XActionStateDes* ASD);		//插入一个状态描述(按时间排序)
	_XBool popASD(const _XActionStateDes* ASD);	//弹出一个状态描述,并释放资源
	//_XBool checkStateTable();						//检查状态列表的合理性,缺少起点或者终点则，做起点等于终点的处理
	//---------------------------------------------
public:
	float getEndTime() const {return m_endTime;}
	float getStartTime() const {return m_startTime;}
	float getNowTime() const {return m_timeLine;}
	_XObjectBasic * getObject() {return m_object;}
	int getOtherActionSum(){return m_otherAction.size();}
	_XActionDescription *getOtherAD(int index)
	{
		if(index < 0 || index >= m_otherAction.size()) return NULL;
		return m_otherAction[index];
	}
	int getActionCoreSum(){return m_actionCore.size();}
	_XActionCore * getActionCore(int index)
	{
		if(!m_isEnable) return NULL;
		if(index < 0 || index >= m_actionCore.size()) return NULL;
		return m_actionCore[index];
	}

public:
	int getID() const{return m_ID;}
	const char * getName() const{return m_actionName;}
	_XBool setName(const char * name)
	{//(尚未实现)
		if(name == NULL) return XFalse;
		if(strlen(name) >= MAX_FILE_NAME_LENGTH) return XFalse;
		if(strcmp(name,m_actionName) == 0) return XTrue;	//名字相同不再重复命名
		//if(_XActionMananger::GetInstance().getActionDes(name) != NULL) return XFalse; //重名
		//这里需要检查重名(尚未实现)
		strcpy(m_actionName,name);
		return XTrue;
	}
	void move(int delay);
	void draw();
	void setStart();
	_XBool getIsEnd() const {return m_isEnd;}	//是否结束
	void setLoop();							//尚未完成
	_XBool set(float startTime,float endTime,_XObjectBasic *obj)
	{
		if(m_isEnable) return XFalse;
		m_startTime = startTime;
		m_endTime = endTime;
		m_object = obj;
		m_otherAction.clear();
		m_actionCore.clear();

		m_isEnable = XTrue;
		return XTrue;
	}
	int getADChildIndex(_XActionDescription * p);	//检查制定的AD在当前AD子列表中的编号
	void setObject(_XObjectBasic *obj);
	_XBool loadAction(const char * filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//保存动作到文件,后缀".acd"
	_XBool saveAction(const char *filename = NULL);	//从文件读取动作,后缀".acd"
	_XBool pushAActionCore(_XActionCore * p);
	_XBool popAActionCore(_XActionCore * p);	//从序列中删除一个元素
	_XBool replaceAAction(int index,_XActionCore * p);
	_XBool popAActionCore(int index);
	_XBool getIsChildActionDes(_XActionDescription * p);
	_XBool pushAActionDescription(_XActionDescription * p);
	_XBool popAActionDescription(int index);
	_XActionDescription();
	~_XActionDescription()
	{
		release();
	}
	void release();
};

#endif