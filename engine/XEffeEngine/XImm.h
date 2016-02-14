#ifndef _JIA_XIMM_
#define _JIA_XIMM_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.4.2
//--------------------------------
#include "XOSDefine.h"
#include <string>
#include <imm.h>
#pragma comment(lib, "imm32.lib")
#pragma comment(lib, "oleaut32.lib")
#pragma comment(lib, "ole32.lib")
namespace XE{
//这是一个关于多windows下的输入法的封装
namespace XImm
{
	extern LRESULT inputMsgProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam,bool &end);
	extern bool getIsInputChange();				//输入法是否发生改变
	extern std::string getImmDescription();		//获取当前输入法的描述文本
	extern std::string getImmFilename();		//获取当前输入法的文件名
	extern void setCompositionPos(float x,float y);			//设置输入法的显示位置
	extern bool getIsCompositionStrChange();	//输入框的字符串是否发生了改变
	extern std::string getCompositionStr();		//获取当前输入框的字符串
	extern bool getCompositionState();			//获取输入框的状态，开始，或者结束
	extern bool getIsCandidateListChange();		//候选列表是否发生更新
	extern bool getHaveCandidateList();			//是否有候选列表
	extern int getCandidateListCount();			//候选列表的数量
	extern std::string getCandidateListStr(int index);	//获取候选列表中的字符串
	extern void showImmUI(bool hideOrShow);		//影藏或显示输入法界面

	extern bool getIsImm();	//判断输入法的类型，输入法可以为IMM或者是TSF
	extern std::string getImeName();	//获取当前输入法的名字
	extern bool getIsEnglish();		//是否是英文输入法
	extern bool getIsFullShape();	//是否是全角
	extern bool getIsSybol();		//是否是中文输入符号
}
}
#endif