#ifndef _JIA_XCHEATMODULE_
#define _JIA_XCHEATMODULE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.2
//--------------------------------
//这是一个命令输入框的类，用于处理一些预先设定好的系统命令
//#include "XOSDefine.h"
//#include "XControl\XEdit.h"
#include "XControl\XMultiText.h"
//Ctrl + Alt + x:显示或者隐藏作弊框
namespace XE{
struct XCheatProc
{
	bool (* eventProc)(void *,const char *,std::string&);
	void *pClass;
	XCheatProc()
		:pClass(NULL)
		,eventProc(NULL)
	{}
};
class XCheatModule
{
private:
	bool m_isInited;
	XEdit m_cheatEdt;
	XMultiText m_cheatMltTxt;
	bool m_visible;
	bool m_ctrlBtnDown;	//是否ctrl按键按下
	bool m_altBtnDown;	//是否alt按键按下

	std::vector<XCheatProc> m_cmdProcs;	//用于处理命令的回调函数
public:
	bool init();
	bool addCmdProc(bool (* eventProc)(void *,const char *,std::string&),void *pClass)
	{
		XCheatProc tmp;
		tmp.eventProc = eventProc;
		tmp.pClass = pClass;
		m_cmdProcs.push_back(tmp);
		return true;
	}
	static void ctrlProc(void*,int,int);
	void commandProc(const char *command);	//下面是命令的处理函数
	XBool keyboardProc(int keyOrder,XKeyState keyState);	//这里响应一些特殊的按键信息

	//下面使用注册回调函数的方式来支持进行功能扩展

	XCheatModule()
		:m_isInited(false)
		,m_visible(false)
		,m_ctrlBtnDown(false)
		,m_altBtnDown(false)
	{}
};
}
#endif