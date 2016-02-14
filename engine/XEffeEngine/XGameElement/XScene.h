#ifndef _JIA_XSCENE_
#define _JIA_XSCENE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.5.21
//--------------------------------
#include "stdio.h"
#include <string>
#include "XInputEventCore.h"
namespace XE{
class XScene
{
public:
	XScene()
		:m_sceneName("x")
	{ printf("XScene Constructor \n"); }	
	virtual ~XScene(){printf("XScene Destructor::%s\n", m_sceneName.c_str());}

	virtual void setup(){printf("XScene setup::%s\n", m_sceneName.c_str());}
		
	virtual void update(float){}
	virtual void draw(){}

	//场景的几个功能函数接口
	virtual void willEnter(){}		//场景将要进入时调用
	virtual void entered(){}		//场景进入完成调用
	virtual void willExit(){}		//场景将要退出时调用
	virtual void exited(){}			//场景退出完成调用
	
	virtual void input(const XInputEvent &) {}
	
	std::string getSceneName()const{return m_sceneName;}
	void setSceneName(const std::string & name){m_sceneName = name;}
private:
	std::string m_sceneName;
};
}
#endif