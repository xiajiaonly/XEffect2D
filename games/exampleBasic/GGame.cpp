#include "GGame.h"
using namespace XE;
void GGame::ctrlEventProc(int id,int eventID)
{
}
bool GGame::init()
{
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
	if(event.isKeyBoardDown())
	{
		switch (event.keyValue)
		{
		case XKEY_S:
			break;
		}
	}
}
void GGame::release()
{
}