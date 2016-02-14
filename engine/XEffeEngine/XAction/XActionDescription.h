#ifndef _JIA_XACTIONDESCRIPTION_
#define _JIA_XACTIONDESCRIPTION_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.5
//--------------------------------
#include "../XObjectBasic.h"
#include "../XFont/XNumberEx.h"
#include "../XMath/XMoveData.h"
//#include "../XBasicFun.h"
#include <vector>
#include "XActionObjectManager.h"
namespace XE{
#define ACTION_DES_PATH "ResourcePack/ActionDescription/"
enum XActionType
{//动作的类型
	ACTION_TYPE_POSITION,	//位置的变化
	ACTION_TYPE_POSITIONX,	//位置的变化
	ACTION_TYPE_POSITION_Y,	//位置的变化
	ACTION_TYPE_COLOR,		//颜色的变化
	ACTION_TYPE_R,
	ACTION_TYPE_G,
	ACTION_TYPE_B,
	ACTION_TYPE_ALPHA,		//透明度的变化
	ACTION_TYPE_ANGLE,		//角度变化
	ACTION_TYPE_SIZE,		//尺寸的变化
	ACTION_TYPE_SIZE_EX,	//尺寸的变化	//x、y等比例缩放
	ACTION_TYPE_SIZEX,		//尺寸的变化	
	ACTION_TYPE_SIZE_Y,		//尺寸的变化
	ACTION_TYPE_STAY,		//停留状态
	ACTION_TYPE_MAX,
};
extern std::string actionTypeStr[];
struct XActionState
{//物件保持的状态
	XBool isEnable;
	XVector2 position;
	XFColor color;
	float angle;	//[0-360]
	XVector2 size;
	XActionState()
		:angle(0.0f)
		,isEnable(XFalse)
	{}
};
//状态类型
enum XActionStateType
{
	ACTION_STATE_TYPE_POSX,
	ACTION_STATE_TYPE_POS_Y,
	ACTION_STATE_TYPE_COLOR_R,
	ACTION_STATE_TYPE_COLOR_G,
	ACTION_STATE_TYPE_COLOR_B,
	ACTION_STATE_TYPE_COLOR_A,
	ACTION_STATE_TYPE_ANGLE,
	ACTION_STATE_TYPE_SIZEX,
	ACTION_STATE_TYPE_SIZE_Y,
	ACTION_STATE_TYPE_MAX,
};
struct XActionStateDescription
{
	int time;	//状态所处的时间点，这个只在AD_TYPE_STATE_LINE时有效
	XBool isValidPosX;
	XBool isValidPosY;
	XBool isValidColorR;
	XBool isValidColorG;
	XBool isValidColorB;
	XBool isValidColorA;
	XBool isValidAngle;
	XBool isValidSizeX;
	XBool isValidSizeY;

	XVector2 position;
	XFColor color;
	float angle;	//[0-360]
	XVector2 size;
	XActionStateDescription()
		:angle(0.0f)
	{}
};
typedef XActionStateDescription XActionStateDes;
//这是一个对动作描述的类
class XActionCore
{
public:
	XBool m_isEnable;
	XActionType m_actionType;
	//std::vector<XMoveData *> m_moveData;
	XMoveData *m_moveData;
	int m_moveDataSum;
	XObjectBasic* m_object;	//动作的对象
	float m_startTime;				//动作起始时间,相对于自身的时间
	float m_endTime;
	float m_timeLine;				//时间线
	XActionState m_stayState;		//需要保持的状态
	//XBool m_isStart;			//是否开始
	XBool m_isActionStart;	//动作开始，数据开始变化
	XBool m_isEnd;
	XBool set(float startTime,float endTime,XActionType type,XMoveData *md = NULL,XActionState *state = NULL);
	void move(float delay);
	XBool getIsEnd() const {return m_isEnd;}
	void setEnd(){m_isEnd = XTrue;}		//设置结束
	//XBool getIsLoop();
	void setStart();	//设置开始
	XActionCore()
		:m_isEnable(XFalse)
		,m_isEnd(XTrue)
		,m_startTime(0.0f)
		,m_timeLine(0.0f)
		//,m_isStart(XFalse)
		,m_moveData(NULL)
		,m_moveDataSum(0)
	{}
	~XActionCore(){release();}
	void release();
};
//AD中对动作的描述类型
//enum XActionDesType
//{
//	AD_TYPE_AC,				//通过ActionCore的方式描述动作
//	AD_TYPE_STATE_TABLE,	//通过状态表的方式描述动作
//};
class XActionMananger;
class XActionDescription
{
	friend XActionMananger;
private:
	int m_ID;
	XBool m_isEnable;
	std::vector<XActionDescription *> m_otherAction;	//动作的组成部分
	std::vector<int> m_otherActionID;	//这个用于辅助初始化
	XObjectBasic* m_object;			//动作的对象
	std::vector<XActionCore *> m_actionCore;	//动作元
	char m_actionName[MAX_FILE_NAME_LENGTH];			//动作的名称
	float m_startTime;				//动作起始时间
	float m_endTime;				//动作结束时间(貌似没有必要),好像没有用
	float m_timeLine;
	XBool m_isEnd;
	//XBool m_isStart;	//是否开始
	XBool m_isActionStart;	//动作开始，数据开始变化
	//XActionDesType m_actionDesType;
	//+++++++++++++++++++++++++++++++++++++++++++++
	//下面是对AD_TYPE_STATE_LINE模式的描述（尚未实现）
	//说明这个模式下，AD中存放state的列表，时间帧检测当前处于那两个state之间，
	//然后通过线性插值(可扩展为贝塞尔差值)的方式计算当前的状态
private:
	std::vector<XActionStateDes *> m_stateTable;	//状态列表,按时间排序
	int m_minTime;
	int m_maxTime;
	float getASValue(int time,XActionStateType type);
public:
	int getMinTime() {return m_minTime;}	//获得最小的时间点
	int getMaxTime() {return m_maxTime;}	//获得最大的时间点
	int getActionStateDesSum() {return m_stateTable.size();}
	XActionStateDes* getPreviousASD(int time,XActionStateType type);	//获得当前时间点的上一个状态描述
	XActionStateDes* getNextASD(int time,XActionStateType type);		//获得当前时间点的下一个状态描述
	XActionStateDes* getASD(int index)
	{
		if(index < 0 || index >= m_stateTable.size()) return NULL;
		return m_stateTable[index];
	}
	XActionState getActionState(int time);		//获取指定时间的状态(注意这个算法的效率有待优化)
	XBool insertASD(XActionStateDes* ASD);		//插入一个状态描述(按时间排序)
	XBool popASD(const XActionStateDes* ASD);	//弹出一个状态描述,并释放资源
	//XBool checkStateTable();						//检查状态列表的合理性,缺少起点或者终点则，做起点等于终点的处理
	//---------------------------------------------
public:
	float getEndTime() const {return m_endTime;}
	float getStartTime() const {return m_startTime;}
	float getCurTime() const {return m_timeLine;}
	XObjectBasic * getObject() {return m_object;}
	int getOtherActionSum(){return m_otherAction.size();}
	XActionDescription *getOtherAD(int index)
	{
		if(index < 0 || index >= m_otherAction.size()) return NULL;
		return m_otherAction[index];
	}
	int getActionCoreSum(){return m_actionCore.size();}
	XActionCore * getActionCore(int index)
	{
		if(!m_isEnable) return NULL;
		if(index < 0 || index >= m_actionCore.size()) return NULL;
		return m_actionCore[index];
	}

public:
	int getID() const{return m_ID;}
	const char * getName() const{return m_actionName;}
	XBool setName(const char * name)
	{//(尚未实现)
		if(name == NULL) return XFalse;
		if(strlen(name) >= MAX_FILE_NAME_LENGTH) return XFalse;
		if(strcmp(name,m_actionName) == 0) return XTrue;	//名字相同不再重复命名
		//if(XActionMananger::GetInstance().getActionDes(name) != NULL) return XFalse; //重名
		//这里需要检查重名(尚未实现)
		strcpy(m_actionName,name);
		return XTrue;
	}
	void move(float delay);
	void draw();
	void setStart();
	XBool getIsEnd() const {return m_isEnd;}	//是否结束
	void setLoop();							//尚未完成
	XBool set(float startTime,float endTime,XObjectBasic *obj)
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
	int getADChildIndex(XActionDescription * p);	//检查制定的AD在当前AD子列表中的编号
	void setObject(XObjectBasic *obj);
	XBool loadAction(const char * filename,XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//保存动作到文件,后缀".acd"
	XBool saveAction(const char *filename = NULL);	//从文件读取动作,后缀".acd"
	XBool pushAActionCore(XActionCore * p);
	XBool popAActionCore(XActionCore * p);	//从序列中删除一个元素
	XBool replaceAAction(int index,XActionCore * p);
	XBool popAActionCore(int index);
	XBool getIsChildActionDes(XActionDescription * p);
	XBool pushAActionDescription(XActionDescription * p);
	XBool popAActionDescription(int index);
	XActionDescription();
	~XActionDescription(){release();}
	void release();
private:
	bool loadFromFolder(const char *filename);	//从文件夹中载入资源
	bool loadFromPacker(const char *filename);	//从压缩包中载入资源
	bool loadFromWeb(const char *filename);		//从网页中读取资源
};
}
#endif