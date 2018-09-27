#include "XStdHead.h"
#include "XDragObjectBase.h"
namespace XE{
void XDragObjectBase::mouseEventProc(const XVec2& p, XMouseState mouseState)
{
	switch (mouseState)
	{
	case MOUSE_LEFT_BUTTON_DCLICK:
	case MOUSE_LEFT_BUTTON_DOWN:
		if (getIsInDragRect(p))
		{
			POINT _p;
			GetCursorPos(&_p);
			m_oldPosX = _p.x;
			m_oldPosY = _p.y;
			m_isMouseBtnDown = XTrue;
		}
		break;
	case MOUSE_MOVE:
		if (m_isMouseBtnDown)
		{
			POINT _p;
			GetCursorPos(&_p);
			if (m_oldPosX != _p.x || m_oldPosY != _p.y)
			{
				XVec2 tmp = getPosition();
				tmp.x += _p.x - m_oldPosX;
				tmp.y += _p.y - m_oldPosY;
				tmp = correctPos(tmp);
				setPosition(tmp);
				m_oldPosX = _p.x;
				m_oldPosY = _p.y;
			}
		}
		break;
	case MOUSE_LEFT_BUTTON_UP:
		if (m_isMouseBtnDown)
			m_isMouseBtnDown = false;
		break;
	}
}
}