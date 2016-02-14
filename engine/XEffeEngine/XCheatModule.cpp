#include "XStdHead.h"
#include "XCheatModule.h"
namespace XE{
bool XCheatModule::init()
{
	if(m_isInited) return false;

	m_cheatEdt.initWithoutSkin(512.0f,"");
	m_cheatEdt.setPosition(0.0f,getSceneHeight() - 16.0f);
	m_cheatEdt.setEventProc(ctrlProc,this); 
	m_cheatEdt.disVisible();
	m_cheatEdt.setScale(0.5f);
	m_cheatMltTxt.initWithoutSkin(XVector2(512.0f,1024.0f),"Msg");
	m_cheatMltTxt.setPosition(0.0f,getSceneHeight() - 16.0f - 512.0f);
	m_cheatMltTxt.disVisible();
	m_cheatMltTxt.setScale(0.5f);

	m_visible = false;

	m_isInited = true;
	return true;
}
void XCheatModule::ctrlProc(void*pClass,int id,int eventID)
{
	if(eventID != XEdit::EDT_INPUT_OVER) return;
	XCheatModule &pPar = *(XCheatModule *)pClass;
	if(id != pPar.m_cheatEdt.getControlID()) return;
	//下面是命令函数
	pPar.commandProc(pPar.m_cheatEdt.getString());
}
void XCheatModule::commandProc(const char *command)
{
	if(command == NULL || command[0] != '-') 
	{
		if(command == NULL || command[0] == '\0') return;	//空字符串
		if(command != NULL)
			m_cheatMltTxt.addALine(command);
		m_cheatMltTxt.addALine("错误或者非法的命令!");
		return;	//非法的命令
	}
	//下面开始处理命令
	//首先是引擎默认的公用部分
	//if(strcmp(command,"-windowWidth") == 0)
	//{//这只是一个示范的例子，后面的功能可以扩展
	//	m_cheatMltTxt.addALine(command);
	//	m_cheatMltTxt.addALine(XString::toString(XEE::windowWidth).c_str());
	//	m_cheatEdt.setString("");
	//	return;
	//}
	//接下来是用户自定义的部分
	m_cheatMltTxt.addALine(command);
	std::string retStr;
	bool isFlag = false;
	for(int i = 0;i < m_cmdProcs.size();++ i)
	{
		if(m_cmdProcs[i].eventProc != NULL && m_cmdProcs[i].eventProc(m_cmdProcs[i].pClass,command,retStr))
		{
			//m_cheatMltTxt.addALine(retStr.c_str());	
			m_cheatMltTxt.addString(retStr.c_str());	//可以支持多行写入
			//m_cheatEdt.setString("");
			//return;	//命令可以穿透，不是唯一的承载关系
			isFlag = true;
		}
	}
	if(isFlag) m_cheatEdt.setString("");
}
XBool XCheatModule::keyboardProc(int keyOrder,XKeyState keyState)
{
	if(!m_isInited) return false;
	switch(keyOrder)
	{
	case XKEY_LCTRL:
	case XKEY_RCTRL:
		if(keyState == KEY_STATE_DOWN)
			m_ctrlBtnDown = true;
		else 
			m_ctrlBtnDown = false;
		break;
	case XKEY_LALT:
	case XKEY_RALT:
		if(keyState == KEY_STATE_DOWN)
			m_altBtnDown = true;
		else 
			m_altBtnDown = false;
		break;
	case XKEY_X:
		if(m_ctrlBtnDown && m_altBtnDown && keyState == KEY_STATE_DOWN)
		{
			m_visible = !m_visible;
			if(m_visible)
			{
				m_cheatEdt.setVisible();
				m_cheatMltTxt.setVisible();
			}else
			{
				m_cheatEdt.disVisible();
				m_cheatMltTxt.disVisible();
			}
		}
		break;
	}
	return XFalse;
}
}