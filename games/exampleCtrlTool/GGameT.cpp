#include "GGameT.h"
using namespace XE;
bool GGame::init()
{
	XEE::setVSync(false);
	if(!m_ui.createUI()) return false;
	return true;
}
void GGame::move(float stepTime)
{
}
void GGame::draw()
{
}
void GGame::input(const XInputEvent &event)
{
}
void GGame::release()
{
}