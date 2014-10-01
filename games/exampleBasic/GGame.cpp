#include "GGame.h"

bool _GGame::init()
{
	return true;
}
void _GGame::move(int stepTime)
{
}
void _GGame::draw()
{
}
void _GGame::input(const _XInputEvent &event)
{
	if(event.type == EVENT_KEYBOARD
		&& event.keyState == KEY_STATE_DOWN
		&& event.keyValue == XKEY_S)
	{}
}
void _GGame::release()
{
}