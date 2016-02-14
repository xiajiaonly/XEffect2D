#include "XStdHead.h"
#include "XDragObjectBase.h"
namespace XE{
void XDragObjectBase::mouseEventProc(float x,float y,XMouseState mouseState)
{
	switch(mouseState)
	{
	case MOUSE_LEFT_BUTTON_DCLICK:
	case MOUSE_LEFT_BUTTON_DOWN:
		if(getIsInDragRect(x,y))
		{
			POINT p;
			GetCursorPos(&p);
			m_oldPosX = p.x;
			m_oldPosY = p.y;
			m_isMouseBtnDown = XTrue;
		}
		break;
	case MOUSE_MOVE:
		if(m_isMouseBtnDown)
		{
			POINT p;
			GetCursorPos(&p);
			if(m_oldPosX != p.x || m_oldPosY != p.y)
			{
				XVector2 tmp = getPosition();
				tmp.x += p.x - m_oldPosX;
				tmp.y += p.y - m_oldPosY;
				tmp = correctPos(tmp);
				setPosition(tmp.x,tmp.y);
				m_oldPosX = p.x;
				m_oldPosY = p.y;
			}
		}
		break;
	case MOUSE_LEFT_BUTTON_UP:
		if(m_isMouseBtnDown)
			m_isMouseBtnDown = false;
		break;
	}
}
}