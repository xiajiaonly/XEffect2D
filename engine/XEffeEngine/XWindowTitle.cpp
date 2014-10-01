#include "XEffeEngine.h"
#include "XWindowTitle.h"

void winTitleCtrProc(void *pClass,int id)
{
	_XWindowTitle &pPar = *(_XWindowTitle *)pClass;
	if(id == pPar.m_minSizeBtn.getControlID())
	{
		if(pPar.m_winMinFun == NULL) minSizeWindow();
		else pPar.m_winMinFun(pPar.m_pClass);
	}else
	if(id == pPar.m_exitBtn.getControlID())
	{
		if(pPar.m_winExitFun == NULL) exitWindow();
		else pPar.m_winExitFun(pPar.m_pClass);
	}
}
void _XWindowTitle::mouseProc(int x,int y,_XMouseState mouseState)
{
	switch(mouseState)
	{
	case MOUSE_MOVE:
		if(m_isMouseBtnDown)
		{//下面移动窗口
			POINT p;
			GetCursorPos(&p);
			if(m_oldPosX != p.x || m_oldPosY != p.y)
			{
				int tmpX,tmpY;
				getWindowPos(tmpX,tmpY);
				setWindowPos(tmpX + p.x - m_oldPosX,
					tmpY + p.y - m_oldPosY);	//这里将会触发鼠标移动事件
				m_oldPosX = p.x;
				m_oldPosY = p.y;
			}
		}
		break;
	case MOUSE_LEFT_BUTTON_DOWN:
		if(m_titleRect.isInRect(x,y))
		{
			m_isMouseBtnDown = true;
			POINT p;
			GetCursorPos(&p);
			m_oldPosX = p.x;
			m_oldPosY = p.y;
		}
		break;
	case MOUSE_LEFT_BUTTON_UP:
		if(m_isMouseBtnDown) m_isMouseBtnDown = false;
		break;
	}
}
bool _XWindowTitle::init()
{
	if(m_isInited) return false;
	m_minSizeBtn.initWithoutTex("-",XEE::systemFont,_XRect(0.0f,0.0f,43.0f,32.0f));
	m_minSizeBtn.setPosition(XEE::windowData.sceneW - 129.0f,-32.0f);
	m_minSizeBtn.setMouseDownCB(winTitleCtrProc,this);
	m_exitBtn.initWithoutTex("X",XEE::systemFont,_XRect(0.0f,0.0f,43.0f,32.0f));
	m_exitBtn.setPosition(XEE::windowData.sceneW - 86.0f,-32.0f);
	m_exitBtn.setMouseDownCB(winTitleCtrProc,this);
	m_iconBtn.initWithoutTex(">",XEE::systemFont,_XRect(0.0f,0.0f,43.0f,32.0f));
	m_iconBtn.setPosition(XEE::windowData.sceneW - 43.0f,-32.0f);
	m_iconBtn.disable();
	m_titleFont.setACopy(XEE::systemFont);
	m_titleFont.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
	m_titleFont.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
	m_titleFont.setString(XEE::windowData.windowTitle.c_str());
	m_titleFont.setColor(_XFColor::white);
	m_titleFont.setPosition(0.0f,-32.0f);
	//m_titleFont.setSize(0.5f,0.5f);
	if(!m_withText) m_titleFont.disVisible();
	m_titleRect.set(0.0f,-32.0f,XEE::windowData.sceneW - 129.0f,0.0f);

	m_isInited = true;
	return true;
}