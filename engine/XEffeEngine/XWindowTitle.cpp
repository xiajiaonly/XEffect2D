#include "XStdHead.h"
#include "XWindowTitle.h"

namespace XE{
void XWindowTitle::ctrlProc(void *pClass,int id,int eventID)
{
	if(eventID != XButton::BTN_MOUSE_DOWN) return;
	XWindowTitle &pPar = *(XWindowTitle *)pClass;
	if(id == pPar.m_rtBtn[0].getControlID())
	{
		if(pPar.m_winMinFun == NULL) XEG.minSizeWindow();
		else pPar.m_winMinFun(pPar.m_pClass);
	}else
	if(id == pPar.m_rtBtn[1].getControlID())
	{
		if(pPar.m_winExitFun == NULL) XEG.setGameExit();
		else pPar.m_winExitFun(pPar.m_pClass);
	}
}
bool XWindowTitle::init()
{
	if(m_isInited) return false;
	m_rtBtn = XMem::createArrayMem<XButton>(3);
	if (m_rtBtn == nullptr) return false;
	m_rtBtn[0].initWithoutSkin("", getDefaultFont(), XRect(0.0f, MIN_FONT_CTRL_SIZE));
	m_rtBtn[0].setPosition(XEG.m_windowData.sceneW - MIN_FONT_CTRL_SIZE * 3.0f, -MIN_FONT_CTRL_SIZE);
	m_rtBtn[0].setEventProc(ctrlProc,this);
	m_rtBtn[0].setSymbol(BTN_SYMBOL_LINE);
	m_rtBtn[0].setWithAction(false);
	m_rtBtn[0].setWithWireframe(false);
	m_rtBtn[1].initWithoutSkin("", getDefaultFont(), XRect(0.0f, MIN_FONT_CTRL_SIZE));
	m_rtBtn[1].setPosition(XEG.m_windowData.sceneW - MIN_FONT_CTRL_SIZE * 2.0f, -MIN_FONT_CTRL_SIZE);
	m_rtBtn[1].setEventProc(ctrlProc,this);
	m_rtBtn[1].setSymbol(BTN_SYMBOL_MASK_WRONG);
	m_rtBtn[1].setWithAction(false);
	m_rtBtn[1].setWithWireframe(false);
	m_rtBtn[2].initWithoutSkin("", getDefaultFont(), XRect(0.0f, MIN_FONT_CTRL_SIZE));
	m_rtBtn[2].setPosition(XEG.m_windowData.sceneW - MIN_FONT_CTRL_SIZE, -MIN_FONT_CTRL_SIZE);
	m_rtBtn[2].disable();
	m_rtBtn[2].setSymbol(BTN_SYMBOL_LEFT);
	m_rtBtn[2].setWithAction(false);
	m_rtBtn[2].setWithWireframe(false);
	m_titleFont.setACopy(getDefaultFont());
	m_titleFont.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
	m_titleFont.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
	m_titleFont.setString(XEG.m_windowData.windowTitle.c_str());
	m_titleFont.setColor(XFColor::white);
	m_titleFont.setPosition(0.0f,-MIN_FONT_CTRL_SIZE);
	//m_titleFont.setSize(0.5f,0.5f);
	if(!m_withText) m_titleFont.disVisible();
	m_titleRect.set(0.0f,-MIN_FONT_CTRL_SIZE,XEG.m_windowData.sceneW - 129.0f,0.0f);

	m_isInited = true;
	return true;
}
#if !WITH_INLINE_FILE
#include "XWindowTitle.inl"
#endif
}