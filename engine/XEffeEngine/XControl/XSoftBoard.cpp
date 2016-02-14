#include "XStdHead.h"
#include "XSoftBoard.h"
namespace XE{
const std::string XSoftBoard::m_numberBtnStrs[m_numberBtnSum] = {"1","2","3","4","5","6","7","8","9","0"};	
const std::string XSoftBoard::m_numberBtnUpStrs[m_numberBtnSum] = {"!","@","#","$","%","^","&","*","(",")"};	
const std::string XSoftBoard::m_letterBtnStrs[m_letterBtnSum] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n",
	"o","p","q","r","s","t","u","v","w","x","y","z"};	
const std::string XSoftBoard::m_letterBtnUpStrs[m_letterBtnSum] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N",
	"O","P","Q","R","S","T","U","V","W","X","Y","Z"};	
const std::string XSoftBoard::m_symbolBtnStrs[m_symbolBtnSum] = {"`","-","=","[","]","\\",";","'",",",".","/"};	
const std::string XSoftBoard::m_symbolBtnUpStrs[m_symbolBtnSum] = {"~","_","+","{","}","|",":","\"","<",">","?"};	
const std::string XSoftBoard::m_specialBtnStrs[m_specialBtnSum] = {"Esc","Tab","Caps","Shift","Ctrl","Win","Alt","Space","Alt","Mnu",	//9
	"Ctrl","Lft","Up","Rgt","Dwn","Bksp","Del","Enter","Shift","Fn","Home","End","Insert","PrtScn","PgUp",	//24
	"PgDn","Pause","ScrLK"};	

void XSoftBoard::btnEventProc(void *pClass,int id,int eventID)
{
	XSoftBoard &pPar = *(XSoftBoard *)pClass;
	if(eventID != XButton::BTN_MOUSE_UP) return;
	//数字按键
	for(int i = 0;i < m_numberBtnSum;++ i)
	{
		if(id == pPar.m_numberBtn[i].getControlID())
		{//下面逻辑功能尚未实现
			keybd_event('0' + ((i + 1) % 10),0,0,0);
			keybd_event('0' + ((i + 1) % 10),0,KEYEVENTF_KEYUP,0);
			return;
		}
	}
	//字母按键
	for(int i = 0;i < m_letterBtnSum;++ i)
	{
		if(id == pPar.m_letterBtn[i].getControlID())
		{//下面逻辑功能尚未实现
			keybd_event('A' + i,0,0,0);
			keybd_event('A' + i,0,KEYEVENTF_KEYUP,0);
			return;
		}
	}
	//符号按键
	for(int i = 0;i < m_symbolBtnSum;++ i)
	{
		if(id == pPar.m_symbolBtn[i].getControlID())
		{//下面逻辑功能尚未实现
			//printf("%d\n",i);	
			switch(i)
			{
			case 0:
				keybd_event(0XC0,0,0,0);		
				keybd_event(0XC0,0,KEYEVENTF_KEYUP,0);		
				break;
			case 1:
				keybd_event(VK_OEM_MINUS,0,0,0);		
				keybd_event(VK_OEM_MINUS,0,KEYEVENTF_KEYUP,0);		
				break;
			case 2:
				keybd_event(VK_OEM_PLUS,0,0,0);		
				keybd_event(VK_OEM_PLUS,0,KEYEVENTF_KEYUP,0);		
				break;
			case 3:
				keybd_event(0xDB,0,0,0);		
				keybd_event(0xDB,0,KEYEVENTF_KEYUP,0);		
				break;
			case 4:
				keybd_event(0xDD,0,0,0);		
				keybd_event(0xDD,0,KEYEVENTF_KEYUP,0);		
				break;
			case 5:
				keybd_event(0xDC,0,0,0);		
				keybd_event(0xDC,0,KEYEVENTF_KEYUP,0);		
				break;
			case 6:
				keybd_event(0xBA,0,0,0);		
				keybd_event(0xBA,0,KEYEVENTF_KEYUP,0);		
				break;
			case 7:
				keybd_event(0xDE,0,0,0);		
				keybd_event(0xDE,0,KEYEVENTF_KEYUP,0);		
				break;
			case 8:
				keybd_event(VK_OEM_COMMA,0,0,0);		
				keybd_event(VK_OEM_COMMA,0,KEYEVENTF_KEYUP,0);		
				break;
			case 9:
				keybd_event(0xBE,0,0,0);		
				keybd_event(0xBE,0,KEYEVENTF_KEYUP,0);		
				break;
			case 10:
				keybd_event(VK_OEM_PERIOD,0,0,0);		
				keybd_event(VK_OEM_PERIOD,0,KEYEVENTF_KEYUP,0);		
				break;
			}
			return;
		}
	}
	//功能按键
	for(int i = 0;i < m_specialBtnSum;++ i)
	{
		if(id == pPar.m_specialBtn[i].getControlID())
		{//下面逻辑功能尚未实现
			//printf("%d\n",i);
			//几个特殊按键的处理
			if(id == pPar.m_specialBtn[2].getControlID())	//caps
				pPar.pressCapsBtn();
			if(id == pPar.m_specialBtn[3].getControlID() || 
				id == pPar.m_specialBtn[18].getControlID())	//shift
				pPar.pressShiftBtn();
			switch(i)
			{
			case 0:
				keybd_event(VK_ESCAPE,0,0,0);		
				keybd_event(VK_ESCAPE,0,KEYEVENTF_KEYUP,0);		
				break;
			case 1:
				keybd_event(VK_TAB,0,0,0);		
				keybd_event(VK_TAB,0,KEYEVENTF_KEYUP,0);		
				break;
			case 2:
				keybd_event(VK_CAPITAL,0,0,0);		
				keybd_event(VK_CAPITAL,0,KEYEVENTF_KEYUP,0);		
				break;
			case 3:
				if(pPar.m_specialBtn[i].getIsCheck())
					keybd_event(VK_LSHIFT,0,0,0);		
				else
					keybd_event(VK_LSHIFT,0,KEYEVENTF_KEYUP,0);		
				break;
			case 4:
				if(pPar.m_specialBtn[i].getIsCheck())
					keybd_event(VK_LCONTROL,0,0,0);		
				else
					keybd_event(VK_LCONTROL,0,KEYEVENTF_KEYUP,0);		
				break;
			case 5:
				keybd_event(VK_LWIN,0,0,0);		
				keybd_event(VK_LWIN,0,KEYEVENTF_KEYUP,0);		
				break;
			case 6:
				if(pPar.m_specialBtn[i].getIsCheck())
					keybd_event(VK_LMENU,0,0,0);		
				else
					keybd_event(VK_LMENU,0,KEYEVENTF_KEYUP,0);		
				break;
			case 7:
				keybd_event(VK_SPACE,0,0,0);		
				keybd_event(VK_SPACE,0,KEYEVENTF_KEYUP,0);		
				break;
			case 8:
				if(pPar.m_specialBtn[i].getIsCheck())
					keybd_event(VK_RMENU,0,0,0);		
				else
					keybd_event(VK_RMENU,0,KEYEVENTF_KEYUP,0);		
				break;
			case 9://菜单键这个不知道
				break;
			case 10:
				if(pPar.m_specialBtn[i].getIsCheck())
					keybd_event(VK_RCONTROL,0,0,0);		
				else
					keybd_event(VK_RCONTROL,0,KEYEVENTF_KEYUP,0);		
				break;
			case 11:
				keybd_event(VK_LEFT,0,0,0);		
				keybd_event(VK_LEFT,0,KEYEVENTF_KEYUP,0);		
				break;
			case 12:
				keybd_event(VK_UP,0,0,0);		
				keybd_event(VK_UP,0,KEYEVENTF_KEYUP,0);		
				break;
			case 13:
				keybd_event(VK_RIGHT,0,0,0);		
				keybd_event(VK_RIGHT,0,KEYEVENTF_KEYUP,0);		
				break;
			case 14:
				keybd_event(VK_DOWN,0,0,0);		
				keybd_event(VK_DOWN,0,KEYEVENTF_KEYUP,0);		
				break;
			case 15:
				keybd_event(VK_BACK,0,0,0);		
				keybd_event(VK_BACK,0,KEYEVENTF_KEYUP,0);		
				break;
			case 16:
				keybd_event(VK_DELETE,0,0,0);		
				keybd_event(VK_DELETE,0,KEYEVENTF_KEYUP,0);		
				break;
			case 17:
				keybd_event(VK_RETURN,0,0,0);		
				keybd_event(VK_RETURN,0,KEYEVENTF_KEYUP,0);		
				break;
			case 18:
				if(pPar.m_specialBtn[i].getIsCheck())
					keybd_event(VK_RSHIFT,0,0,0);		
				else
					keybd_event(VK_RSHIFT,0,KEYEVENTF_KEYUP,0);		
				break;
			case 19://fn
				break;	
			case 20:
				keybd_event(VK_HOME,0,0,0);		
				keybd_event(VK_HOME,0,KEYEVENTF_KEYUP,0);		
				break;
			case 21:
				keybd_event(VK_END,0,0,0);		
				keybd_event(VK_END,0,KEYEVENTF_KEYUP,0);		
				break;
			case 22:
				keybd_event(VK_INSERT,0,0,0);		
				keybd_event(VK_INSERT,0,KEYEVENTF_KEYUP,0);		
				break;
			case 23:
				keybd_event(VK_SNAPSHOT,0,0,0);		
				keybd_event(VK_SNAPSHOT,0,KEYEVENTF_KEYUP,0);		
				break;
			case 24:
				keybd_event(VK_PRIOR,0,0,0);		
				keybd_event(VK_PRIOR,0,KEYEVENTF_KEYUP,0);		
				break;
			case 25:
				keybd_event(VK_NEXT,0,0,0);		
				keybd_event(VK_NEXT,0,KEYEVENTF_KEYUP,0);		
				break;
			case 26:
				keybd_event(VK_PAUSE,0,0,0);		
				keybd_event(VK_PAUSE,0,KEYEVENTF_KEYUP,0);		
				break;
			case 27:
				keybd_event(VK_SCROLL,0,0,0);		
				keybd_event(VK_SCROLL,0,KEYEVENTF_KEYUP,0);		
				break;
			}
			return;
		}
	}
}
void XSoftBoard::pressCapsBtn()
{
	m_isCapsDown = !m_isCapsDown;
	if((m_isCapsDown && !m_isShiftDown) || (!m_isCapsDown && m_isShiftDown))
	{//大写
		for(int i = 0;i < m_letterBtnSum;++ i)
		{
			m_letterBtn[i].setCaptionText(m_letterBtnUpStrs[i].c_str());
		}
	}else
	{//小写
		for(int i = 0;i < m_letterBtnSum;++ i)
		{
			m_letterBtn[i].setCaptionText(m_letterBtnStrs[i].c_str());
		}
	}
}
void XSoftBoard::pressShiftBtn()
{
	if(m_specialBtn[3].getIsCheck() || m_specialBtn[18].getIsCheck())
		m_isShiftDown = true;
	else
		m_isShiftDown = false;
	if(m_isShiftDown)
	{
		for(int i = 0;i < m_numberBtnSum;++ i)
		{
			m_numberBtn[i].setCaptionText(m_numberBtnUpStrs[i].c_str());
		}
		for(int i = 0;i < m_symbolBtnSum;++ i)
		{
			m_symbolBtn[i].setCaptionText(m_symbolBtnUpStrs[i].c_str());
		}
		if(m_isCapsDown)
		{
			for(int i = 0;i < m_letterBtnSum;++ i)
			{
				m_letterBtn[i].setCaptionText(m_letterBtnStrs[i].c_str());
			}	
		}else
		{
			for(int i = 0;i < m_letterBtnSum;++ i)
			{
				m_letterBtn[i].setCaptionText(m_letterBtnUpStrs[i].c_str());
			}	
		}
	}else
	{
		for(int i = 0;i < m_numberBtnSum;++ i)
		{
			m_numberBtn[i].setCaptionText(m_numberBtnStrs[i].c_str());
		}
		for(int i = 0;i < m_symbolBtnSum;++ i)
		{
			m_symbolBtn[i].setCaptionText(m_symbolBtnStrs[i].c_str());
		}
		if(m_isCapsDown)
		{
			for(int i = 0;i < m_letterBtnSum;++ i)
			{
				m_letterBtn[i].setCaptionText(m_letterBtnUpStrs[i].c_str());
			}	
		}else
		{
			for(int i = 0;i < m_letterBtnSum;++ i)
			{
				m_letterBtn[i].setCaptionText(m_letterBtnStrs[i].c_str());
			}	
		}
	}
}
bool XSoftBoard::initWithoutSkin()
{
	if(m_isInited) return false;

	//下面初始化各个按键的状态
	for(int i = 0;i < m_numberBtnSum;++ i)
	{
		m_numberBtn[i].initWithoutSkin(m_numberBtnStrs[i].c_str(),32.0f);	
		m_numberBtn[i].setEventProc(btnEventProc,this);
		addACtrl(&m_numberBtn[i]);
	}
	for(int i = 0;i < m_letterBtnSum;++ i)
	{
		m_letterBtn[i].initWithoutSkin(XString::toString<char>(i + 'a').c_str(),32.0f);
		m_letterBtn[i].setEventProc(btnEventProc,this);
		addACtrl(&m_letterBtn[i]);
	}
	//符号
	m_symbolBtn[0].initWithoutSkin(m_symbolBtnStrs[0].c_str(),32.0f);	//`
	m_symbolBtn[1].initWithoutSkin(m_symbolBtnStrs[1].c_str(),32.0f);	//-
	m_symbolBtn[2].initWithoutSkin(m_symbolBtnStrs[2].c_str(),32.0f);	//=
	m_symbolBtn[3].initWithoutSkin(m_symbolBtnStrs[3].c_str(),32.0f);	//[
	m_symbolBtn[4].initWithoutSkin(m_symbolBtnStrs[4].c_str(),32.0f);	//]
	m_symbolBtn[5].initWithoutSkin(m_symbolBtnStrs[5].c_str(),32.0f);	//反斜杠
	m_symbolBtn[6].initWithoutSkin(m_symbolBtnStrs[6].c_str(),32.0f);	//;
	m_symbolBtn[7].initWithoutSkin(m_symbolBtnStrs[7].c_str(),32.0f);	//'
	m_symbolBtn[8].initWithoutSkin(m_symbolBtnStrs[8].c_str(),32.0f);	//,
	m_symbolBtn[9].initWithoutSkin(m_symbolBtnStrs[9].c_str(),32.0f);	//.
	m_symbolBtn[10].initWithoutSkin(m_symbolBtnStrs[10].c_str(),32.0f);	//.
	for(int i = 0;i < m_symbolBtnSum;++ i)
	{
		m_symbolBtn[i].setEventProc(btnEventProc,this);
		addACtrl(&m_symbolBtn[i]);
	}
	//功能键
	m_specialBtn[0].initWithoutSkin(m_specialBtnStrs[0].c_str(),64.0f);		//Esc
	m_specialBtn[1].initWithoutSkin(m_specialBtnStrs[1].c_str(),80.0f);		//Tab
	m_specialBtn[2].initWithoutSkin(m_specialBtnStrs[2].c_str(),96.0f);		//Caps
	m_specialBtn[3].initWithoutSkin(m_specialBtnStrs[3].c_str(),112.0f);	//Shift
	m_specialBtn[3].setStyle(BTN_STYLE_CHECK);
	m_specialBtn[4].initWithoutSkin(m_specialBtnStrs[4].c_str(),64.0f);		//Ctrl
	m_specialBtn[4].setStyle(BTN_STYLE_CHECK);
	m_specialBtn[5].initWithoutSkin(m_specialBtnStrs[5].c_str(),45.0f);		//Win
	m_specialBtn[6].initWithoutSkin(m_specialBtnStrs[6].c_str(),45.0f);		//Alt
	m_specialBtn[6].setStyle(BTN_STYLE_CHECK);
	m_specialBtn[7].initWithoutSkin(m_specialBtnStrs[7].c_str(),88.0f);		//Space
	m_specialBtn[8].initWithoutSkin(m_specialBtnStrs[8].c_str(),45.0f);		//Alt
	m_specialBtn[8].setStyle(BTN_STYLE_CHECK);
	m_specialBtn[9].initWithoutSkin(m_specialBtnStrs[9].c_str(),45.0f);		//Menu
	m_specialBtn[10].initWithoutSkin(m_specialBtnStrs[10].c_str(),64.0f);	//RCtrl
	m_specialBtn[10].setStyle(BTN_STYLE_CHECK);
	m_specialBtn[11].initWithoutSkin(m_specialBtnStrs[11].c_str(),45.0f);	//left
	m_specialBtn[14].initWithoutSkin(m_specialBtnStrs[14].c_str(),45.0f);	//down
	m_specialBtn[13].initWithoutSkin(m_specialBtnStrs[13].c_str(),45.0f);	//right
	m_specialBtn[19].initWithoutSkin(m_specialBtnStrs[19].c_str(),45.0f);	//fn
	m_specialBtn[15].initWithoutSkin(m_specialBtnStrs[15].c_str(),80.0f);	//bksp
	m_specialBtn[16].initWithoutSkin(m_specialBtnStrs[16].c_str(),64.0f);	//del
	m_specialBtn[17].initWithoutSkin(m_specialBtnStrs[17].c_str(),120.0f);	//Enter
	m_specialBtn[12].initWithoutSkin(m_specialBtnStrs[12].c_str(),45.0f);	//up
	m_specialBtn[18].initWithoutSkin(m_specialBtnStrs[18].c_str(),91.0f);	//Shift
	m_specialBtn[18].setStyle(BTN_STYLE_CHECK);

	m_specialBtn[20].initWithoutSkin(m_specialBtnStrs[20].c_str(),86.0f);	//Home
	m_specialBtn[24].initWithoutSkin(m_specialBtnStrs[24].c_str(),86.0f);	//PgUp
	m_specialBtn[21].initWithoutSkin(m_specialBtnStrs[21].c_str(),86.0f);	//End
	m_specialBtn[25].initWithoutSkin(m_specialBtnStrs[25].c_str(),86.0f);	//PgDn
	m_specialBtn[22].initWithoutSkin(m_specialBtnStrs[22].c_str(),86.0f);	//Insert
	m_specialBtn[26].initWithoutSkin(m_specialBtnStrs[26].c_str(),86.0f);	//Pause
	m_specialBtn[23].initWithoutSkin(m_specialBtnStrs[23].c_str(),86.0f);	//PrtScn
	m_specialBtn[27].initWithoutSkin(m_specialBtnStrs[27].c_str(),86.0f);	//ScrLk
	for(int i = 0;i < m_specialBtnSum;++ i)
	{
		m_specialBtn[i].setEventProc(btnEventProc,this);
		addACtrl(&m_specialBtn[i]);
	}
	m_position.set(0.0f,0.0f);
	setScale(1.0f,1.0f);
	for(int i = m_specialBtnSum - 8;i < m_specialBtnSum;++ i)
	{
		m_specialBtn[i].disVisible();
	}

	m_isInited = XTrue;
	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	return true;
}
void XSoftBoard::draw()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	m_ctrlManager.draw();
	XRender::drawRect(m_curMouseRect);
}
void XSoftBoard::setPosition(float x,float y)
{
//	if(!m_isInited) return;
	m_position.set(x,y);
	for(int i = 0;i < m_numberBtnSum;++ i)
	{
		m_numberBtn[i].setPosition(m_position + XVector2(5.0f + 64.0f + 32.0f * (i + 1.0f) + m_ctrlGap * (i + 2.0f),5.0f) * m_scale);
	}
	m_letterBtn['q' - 'a'].setPosition(m_position + XVector2(5.0f + 80.0f + m_ctrlGap,5.0f + 32.0f + m_ctrlGap) * m_scale);	
	m_letterBtn['w' - 'a'].setPosition(m_position + XVector2(5.0f + 80.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 1.0f,5.0f + 32.0f + m_ctrlGap) * m_scale);	
	m_letterBtn['e' - 'a'].setPosition(m_position + XVector2(5.0f + 80.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 2.0f,5.0f + 32.0f + m_ctrlGap) * m_scale);	
	m_letterBtn['r' - 'a'].setPosition(m_position + XVector2(5.0f + 80.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 3.0f,5.0f + 32.0f + m_ctrlGap) * m_scale);	
	m_letterBtn['t' - 'a'].setPosition(m_position + XVector2(5.0f + 80.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 4.0f,5.0f + 32.0f + m_ctrlGap) * m_scale);	
	m_letterBtn['y' - 'a'].setPosition(m_position + XVector2(5.0f + 80.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 5.0f,5.0f + 32.0f + m_ctrlGap) * m_scale);	
	m_letterBtn['u' - 'a'].setPosition(m_position + XVector2(5.0f + 80.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 6.0f,5.0f + 32.0f + m_ctrlGap) * m_scale);	
	m_letterBtn['i' - 'a'].setPosition(m_position + XVector2(5.0f + 80.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 7.0f,5.0f + 32.0f + m_ctrlGap) * m_scale);	
	m_letterBtn['o' - 'a'].setPosition(m_position + XVector2(5.0f + 80.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 8.0f,5.0f + 32.0f + m_ctrlGap) * m_scale);	
	m_letterBtn['p' - 'a'].setPosition(m_position + XVector2(5.0f + 80.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 9.0f,5.0f + 32.0f + m_ctrlGap) * m_scale);
	m_letterBtn['a' - 'a'].setPosition(m_position + XVector2(5.0f + 96.0f + m_ctrlGap,5.0f + (32.0f + m_ctrlGap) * 2.0f) * m_scale);	
	m_letterBtn['s' - 'a'].setPosition(m_position + XVector2(5.0f + 96.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 1.0f,5.0f + (32.0f + m_ctrlGap) * 2.0f) * m_scale);	
	m_letterBtn['d' - 'a'].setPosition(m_position + XVector2(5.0f + 96.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 2.0f,5.0f + (32.0f + m_ctrlGap) * 2.0f) * m_scale);	
	m_letterBtn['f' - 'a'].setPosition(m_position + XVector2(5.0f + 96.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 3.0f,5.0f + (32.0f + m_ctrlGap) * 2.0f) * m_scale);	
	m_letterBtn['g' - 'a'].setPosition(m_position + XVector2(5.0f + 96.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 4.0f,5.0f + (32.0f + m_ctrlGap) * 2.0f) * m_scale);	
	m_letterBtn['h' - 'a'].setPosition(m_position + XVector2(5.0f + 96.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 5.0f,5.0f + (32.0f + m_ctrlGap) * 2.0f) * m_scale);	
	m_letterBtn['j' - 'a'].setPosition(m_position + XVector2(5.0f + 96.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 6.0f,5.0f + (32.0f + m_ctrlGap) * 2.0f) * m_scale);	
	m_letterBtn['k' - 'a'].setPosition(m_position + XVector2(5.0f + 96.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 7.0f,5.0f + (32.0f + m_ctrlGap) * 2.0f) * m_scale);	
	m_letterBtn['l' - 'a'].setPosition(m_position + XVector2(5.0f + 96.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 8.0f,5.0f + (32.0f + m_ctrlGap) * 2.0f) * m_scale);	
	m_letterBtn['z' - 'a'].setPosition(m_position + XVector2(5.0f + 112.0f + m_ctrlGap,5.0f + (32.0f + m_ctrlGap) * 3.0f) * m_scale);	
	m_letterBtn['x' - 'a'].setPosition(m_position + XVector2(5.0f + 112.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 1.0f,5.0f + (32.0f + m_ctrlGap) * 3.0f) * m_scale);	
	m_letterBtn['c' - 'a'].setPosition(m_position + XVector2(5.0f + 112.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 2.0f,5.0f + (32.0f + m_ctrlGap) * 3.0f) * m_scale);	
	m_letterBtn['v' - 'a'].setPosition(m_position + XVector2(5.0f + 112.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 3.0f,5.0f + (32.0f + m_ctrlGap) * 3.0f) * m_scale);	
	m_letterBtn['b' - 'a'].setPosition(m_position + XVector2(5.0f + 112.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 4.0f,5.0f + (32.0f + m_ctrlGap) * 3.0f) * m_scale);	
	m_letterBtn['n' - 'a'].setPosition(m_position + XVector2(5.0f + 112.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 5.0f,5.0f + (32.0f + m_ctrlGap) * 3.0f) * m_scale);	
	m_letterBtn['m' - 'a'].setPosition(m_position + XVector2(5.0f + 112.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 6.0f,5.0f + (32.0f + m_ctrlGap) * 3.0f) * m_scale);	

	m_symbolBtn[0].setPosition(m_position + XVector2(5.0f + 64.0f + m_ctrlGap,5.0f) * m_scale);
	m_symbolBtn[1].setPosition(m_position + XVector2(5.0f + 64.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 11.0f,5.0f) * m_scale);
	m_symbolBtn[2].setPosition(m_position + XVector2(5.0f + 64.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 12.0f,5.0f) * m_scale);
	m_symbolBtn[3].setPosition(m_position + XVector2(5.0f + 80.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 10.0f,5.0f + 32.0f + m_ctrlGap) * m_scale);
	m_symbolBtn[4].setPosition(m_position + XVector2(5.0f + 80.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 11.0f,5.0f + 32.0f + m_ctrlGap) * m_scale);
	m_symbolBtn[5].setPosition(m_position + XVector2(5.0f + 80.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 12.0f,5.0f + 32.0f + m_ctrlGap) * m_scale);
	m_symbolBtn[6].setPosition(m_position + XVector2(5.0f + 96.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 9.0f,5.0f + (32.0f + m_ctrlGap) * 2.0f) * m_scale);
	m_symbolBtn[7].setPosition(m_position + XVector2(5.0f + 96.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 10.0f,5.0f + (32.0f + m_ctrlGap) * 2.0f) * m_scale);
	m_symbolBtn[8].setPosition(m_position + XVector2(5.0f + 112.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 7.0f,5.0f + (32.0f + m_ctrlGap) * 3.0f) * m_scale);
	m_symbolBtn[9].setPosition(m_position + XVector2(5.0f + 112.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 8.0f,5.0f + (32.0f + m_ctrlGap) * 3.0f) * m_scale);
	m_symbolBtn[10].setPosition(m_position + XVector2(5.0f + 112.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 9.0f,5.0f + (32.0f + m_ctrlGap) * 3.0f) * m_scale);

	m_specialBtn[0].setPosition(m_position + XVector2(5.0f,5.0f) * m_scale);
	m_specialBtn[1].setPosition(m_position + XVector2(5.0f,5.0f + 32.0f + m_ctrlGap) * m_scale);
	m_specialBtn[2].setPosition(m_position + XVector2(5.0f,5.0f + (32.0f + m_ctrlGap) * 2.0f) * m_scale);
	m_specialBtn[3].setPosition(m_position + XVector2(5.0f,5.0f + (32.0f + m_ctrlGap) * 3.0f) * m_scale);
	m_specialBtn[4].setPosition(m_position + XVector2(5.0f,5.0f + (32.0f + m_ctrlGap) * 4.0f) * m_scale);
	m_specialBtn[5].setPosition(m_position + XVector2(5.0f + 64.0f + m_ctrlGap,5.0f + (32.0f + m_ctrlGap) * 4.0f) * m_scale);
	m_specialBtn[6].setPosition(m_position + XVector2(5.0f + 109.0f + m_ctrlGap * 2.0f,5.0f + (32.0f + m_ctrlGap) * 4.0f) * m_scale);
	m_specialBtn[7].setPosition(m_position + XVector2(5.0f + 154.0f + m_ctrlGap * 3.0f,5.0f + (32.0f + m_ctrlGap) * 4.0f) * m_scale);
	m_specialBtn[8].setPosition(m_position + XVector2(5.0f + 242.0f + m_ctrlGap * 4.0f,5.0f + (32.0f + m_ctrlGap) * 4.0f) * m_scale);
	m_specialBtn[9].setPosition(m_position + XVector2(5.0f + 287.0f + m_ctrlGap * 5.0f,5.0f + (32.0f + m_ctrlGap) * 4.0f) * m_scale);
	m_specialBtn[10].setPosition(m_position + XVector2(5.0f + 332.0f + m_ctrlGap * 6.0f,5.0f + (32.0f + m_ctrlGap) * 4.0f) * m_scale);
	m_specialBtn[11].setPosition(m_position + XVector2(5.0f + 396.0f + m_ctrlGap * 7.0f,5.0f + (32.0f + m_ctrlGap) * 4.0f) * m_scale);
	m_specialBtn[14].setPosition(m_position + XVector2(5.0f + 441.0f + m_ctrlGap * 8.0f,5.0f + (32.0f + m_ctrlGap) * 4.0f) * m_scale);
	m_specialBtn[13].setPosition(m_position + XVector2(5.0f + 486.0f + m_ctrlGap * 9.0f,5.0f + (32.0f + m_ctrlGap) * 4.0f) * m_scale);
	m_specialBtn[19].setPosition(m_position + XVector2(5.0f + 531.0f + m_ctrlGap * 10.0f,5.0f + (32.0f + m_ctrlGap) * 4.0f) * m_scale);
	m_specialBtn[15].setPosition(m_position + XVector2(5.0f + 64.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 13.0f,5.0f) * m_scale);
	m_specialBtn[16].setPosition(m_position + XVector2(5.0f + 80.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 13.0f,5.0f + 32.0f + m_ctrlGap) * m_scale);
	m_specialBtn[17].setPosition(m_position + XVector2(5.0f + 96.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 11.0f,5.0f + (32.0f + m_ctrlGap) * 2.0f) * m_scale);
	m_specialBtn[12].setPosition(m_position + XVector2(5.0f + 112.0f + m_ctrlGap + (32.0f + m_ctrlGap) * 10.0f,5.0f + (32.0f + m_ctrlGap) * 3.0f) * m_scale);
	m_specialBtn[18].setPosition(m_position + XVector2(5.0f + 157.0f + m_ctrlGap * 2.0f + (32.0f + m_ctrlGap) * 10.0f,5.0f + (32.0f + m_ctrlGap) * 3.0f) * m_scale);

	m_specialBtn[20].setPosition(m_position + XVector2(5.0f + 630.0f,5.0f) * m_scale);
	m_specialBtn[24].setPosition(m_position + XVector2(5.0f + 630.0f + 86.0f + m_ctrlGap,5.0f) * m_scale);
	m_specialBtn[21].setPosition(m_position + XVector2(5.0f + 630.0f,5.0f + 32.0f + m_ctrlGap) * m_scale);
	m_specialBtn[25].setPosition(m_position + XVector2(5.0f + 630.0f + 86.0f + m_ctrlGap,5.0f + 32.0f + m_ctrlGap) * m_scale);
	m_specialBtn[22].setPosition(m_position + XVector2(5.0f + 630.0f,5.0f + (32.0f + m_ctrlGap) * 2.0f) * m_scale);
	m_specialBtn[26].setPosition(m_position + XVector2(5.0f + 630.0f + 86.0f + m_ctrlGap,5.0f + (32.0f + m_ctrlGap) * 2.0f) * m_scale);
	m_specialBtn[23].setPosition(m_position + XVector2(5.0f + 630.0f,5.0f + (32.0f + m_ctrlGap) * 3.0f) * m_scale);
	m_specialBtn[27].setPosition(m_position + XVector2(5.0f + 630.0f + 86.0f + m_ctrlGap,5.0f + (32.0f + m_ctrlGap) * 3.0f) * m_scale);

	switch(m_softBoardType)
	{
	case TYPE_NORMAL:
		m_curMouseRect.set(m_position,m_position + XVector2(816.0f,186.0f) * m_scale);
		break;
	case TYPE_MINI:
		m_curMouseRect.set(m_position,m_position + XVector2(626.0f,186.0f) * m_scale);
		break;
	}
}
void XSoftBoard::setScale(float x,float y)
{
//	if(!m_isInited) return;
	m_scale.set(x,y);
	setPosition(m_position.x,m_position.y);
	for(int i = 0;i < m_numberBtnSum;++ i)
	{
		m_numberBtn[i].setScale(m_scale);
	}
	for(int i = 0;i < m_letterBtnSum;++ i)
	{
		m_letterBtn[i].setScale(m_scale);
	}
	for(int i = 0;i < m_symbolBtnSum;++ i)
	{
		m_symbolBtn[i].setScale(m_scale);
	}
	for(int i = 0;i < m_specialBtnSum;++ i)
	{
		m_specialBtn[i].setScale(m_scale);
	}
	switch(m_softBoardType)
	{
	case TYPE_NORMAL:
		m_curMouseRect.set(m_position,m_position + XVector2(816.0f,186.0f) * m_scale);
		break;
	case TYPE_MINI:
		m_curMouseRect.set(m_position,m_position + XVector2(626.0f,186.0f) * m_scale);
		break;
	}
}
void XSoftBoard::setSoftBoardType(XSoftBoardType type)
{
	if(type == m_softBoardType) return;
	m_softBoardType = type;
	//下面改变一些属性
	switch(m_softBoardType)
	{
	case TYPE_NORMAL:
		for(int i = m_specialBtnSum - 8;i < m_specialBtnSum;++ i)
		{
			m_specialBtn[22].setVisible();
		}
		m_curMouseRect.set(m_position,m_position + XVector2(816.0f,186.0f) * m_scale);
		break;
	case TYPE_MINI:
		for(int i = m_specialBtnSum - 8;i < m_specialBtnSum;++ i)
		{
			m_specialBtn[22].disVisible();
		}
		m_curMouseRect.set(m_position,m_position + XVector2(626.0f,186.0f) * m_scale);
		break;
	}
}
void XSoftBoard::resetKeyState()
{//退出时需要还原那些已经按下的按键
	if(m_specialBtn[3].getIsCheck())	//Lshift
	{
		m_specialBtn[3].setIsCheck(false);
		keybd_event(VK_LSHIFT,0,KEYEVENTF_KEYUP,0);	
	}
	if(m_specialBtn[4].getIsCheck())
	{
		m_specialBtn[4].setIsCheck(false);
		keybd_event(VK_LCONTROL,0,KEYEVENTF_KEYUP,0);	
	}
	if(m_specialBtn[6].getIsCheck())
	{
		m_specialBtn[6].setIsCheck(false);
		keybd_event(VK_LMENU,0,KEYEVENTF_KEYUP,0);	
	}
	if(m_specialBtn[8].getIsCheck())
	{
		m_specialBtn[8].setIsCheck(false);
		keybd_event(VK_RMENU,0,KEYEVENTF_KEYUP,0);	
	}
	if(m_specialBtn[10].getIsCheck())
	{
		m_specialBtn[10].setIsCheck(false);
		keybd_event(VK_RCONTROL,0,KEYEVENTF_KEYUP,0);	
	}
	if(m_specialBtn[18].getIsCheck())
	{
		m_specialBtn[18].setIsCheck(false);
		keybd_event(VK_RSHIFT,0,KEYEVENTF_KEYUP,0);	
	}
}
}