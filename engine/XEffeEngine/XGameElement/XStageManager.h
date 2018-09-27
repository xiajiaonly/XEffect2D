#ifndef _JIA_XSTAGEMANAGER_
#define _JIA_XSTAGEMANAGER_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.3
//--------------------------------
#include "XStageObject.h"
#include <vector>
//这是一个舞台的类，用于在2.5D的舞台上表现2D的物体，目前功能比较粗糙
//首先要支持sprite、frame、flash和movie
namespace XE{
//舞台的信息
class XStageInfo
{
public:
	XVec3 m_cameraPosition;	//摄像机的位置
	float m_near;	//场景的近截面	，缩放系数为1.0	，up为0
	float m_far;	//场景的远截面	，缩放系数为m_scaleRate	，up为m_depthToUpRate
	float m_scaleRate;	//深度的缩放系数
	float m_depthToUpRate;	//深度向上走的系数，
	XVec3 screenToGround(float x,float y)const;	//输入屏幕坐标，返回对应的地面坐标
	XVec2 worldToScreen(const XVec3& worldPos)const{return worldToScreen(worldPos.x,worldPos.y,worldPos.z);}	//世界坐标转换成屏幕坐标
	XVec2 worldToScreen(float x,float y,float z)const;//世界坐标转换成屏幕坐标
	XVec2 getCameraScale(const XVec3& worldPos)const{return getCameraScale(worldPos.x,worldPos.y,worldPos.z);}	//获取摄像机的缩放系数
	XVec2 getCameraScale(float x,float y,float z)const	//获取摄像机的缩放系数
	{
		return XVec2(XMath::mapping1DEx(z - m_cameraPosition.z, m_near, m_far, 1.0f, m_scaleRate));
	}
	const bool check()const	//检查参数是否合法
	{
		if(m_scaleRate <= 0.0f) return false;	//非法的参数
		if(m_far <= m_near) return false;
		return true;
	}
};

class XStageManager
{
private:
	bool m_isInited;
	XStageInfo m_info;	//舞台信息
	std::vector<XStageObject *> m_objects;	//舞台上的所有物件
	void sortObject();	//按照z顺序对物件进行排序，并计算物体的显示信息
	void qSort(int low,int high)
	{//代码来自百度
		if (low >= high) return;
		int first = low;
		int last = high;
		XStageObject *key = m_objects[first];	//用字表的第一个记录作为枢轴
		while (first<last)
		{
			while(first < last && m_objects[last]->m_position.z >= key->m_position.z)
				--last;
			m_objects[first] = m_objects[last];	//将比第一个小的移到低端
			while(first < last && m_objects[first]->m_position.z <= key->m_position.z)
				++first;
			m_objects[last] = m_objects[first];	//将比第一个大的移到高端
		}
		m_objects[first] = key;	//枢轴记录到位
		qSort(low, first - 1);
		qSort(first + 1, high);
	}
public:
	void release()
	{
		if(!m_isInited) return;
		m_objects.clear();
		m_isInited = false;
	}
	void setCameraPosition(const XVec3& pos)
	{
		m_info.m_cameraPosition = pos;
	}
	void setCameraPosition(float x,float y,float z)
	{
		m_info.m_cameraPosition.set(x,y,z);
	}
	XStageManager()
		:m_isInited(false)
	{}
	~XStageManager(){release();}
	void init(const XStageInfo &info)
	{
		if(m_isInited || !info.check()) return;
		m_info = info;
		m_isInited = true;
	}
	XStageInfo getStageInfo()const{return m_info;}
	void setStageInfo(const XStageInfo &info){if(info.check()) m_info = info;}
	void draw();
	void drawFloor(int size);
	void update(float stepTime);
	bool addAObject(XStageObject *obj);			//向场景中增加一个物件，返回添加是否成功
	bool removeAObject(XStageObject *obj);		//从场景中移除一个物件
	bool haveObject(const XStageObject *obj);	//获取场景中指定物体的编号，如果不存在则返回-1
	XVec3 screenToGround(float x,float y)const{return m_info.screenToGround(x,y);}	//输入屏幕坐标，返回对应的地面坐标
	XVec2 worldToScreen(float x,float y,float z)const{return m_info.worldToScreen(x,y,z);}	//世界坐标转换成屏幕坐标
};
}
#endif