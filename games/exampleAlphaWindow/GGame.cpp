#include "GGame.h"
using namespace XE;
bool GGame::init()
{
	//XEE::setBGColor(XFColor::black);
	//m_sprite.init("ResourcePack/pic/obj.png");
	XCtrlManager.setWithWireframe(false);	//设置不描绘线框
	if(!m_button.initPlus("ResourcePack/pic/Button","按钮",*XEG.m_systemFont,0.5f)) return false;
	m_button.setPosition(64.0f,96.0f);

	m_progress.initWithoutSkin(100,80);
	m_progress.setPosition(128,128);
	m_progress.setValue(50.0f);
	m_progress.setWithVirtual(true);
	return true;
}
void GGame::move(float stepTime)
{
	int x,y;
	XEG.getWindowPos(x,y);
	m_progress.setValue(x / 1024.0f * 100.0f);
	m_winPos.set(x, y);
}
void GGame::draw()
{
	//m_sprite.draw();
}
void GGame::input(const XInputEvent &event)
{
	if(event.type == EVENT_MOUSE)
	{
		mouseEventProc(XVec2(event.mouseX,event.mouseY),event.mouseState);
	}
}
void GGame::release()
{
}
void GGame::setPosition(float x,float y) {XEG.setWindowPos(x,y);}
XBool GGame::getIsInDragRect(float x,float y)
{
	return XRect(0,0,XEG.getWinW(),XEG.getWinH()).isInRect(x,y);
}