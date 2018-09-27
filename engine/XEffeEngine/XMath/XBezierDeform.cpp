#include "XStdHead.h"
#include "XBezierDeform.h"
#include "XInputEventCore.h"
#include "XMath\XBezierSpline.h"
#include "XXml.h"
namespace XE{
bool XBezierDeform::init(int w, int h, int handX, int handY, int vSum)
{
	if (m_isInited) return false;
	if (!m_mesh.init(w, h, handX, handY, vSum)) return false;

	m_changeOldPoint.resize(m_mesh.getHandleSum());
	XEG.addEventProc(this, &XBezierDeform::inputProc);
	m_isInited = true;
	return true;
}
bool XBezierDeform::initFromFile(const char* filename)
{
	if (m_isInited) return false;
	if (!m_mesh.initFromFile(filename)) return false;
	if (filename != nullptr)
	{
		m_withFilename = true;
		m_filename = filename;
	}

	m_changeOldPoint.resize(m_mesh.getHandleSum());
	XEG.addEventProc(this, &XBezierDeform::inputProc);
	m_isInited = true;
	return true;
}
void XBezierDeform::release()
{
	if (!m_isInited) return;
	XEG.desEventProc(this, &XBezierDeform::inputProc);
	m_isInited = false;
}
void XBezierDeform::inputProc(const XInputEvent &event)
{
	XVec2 curMousePos = getMousePos();
	curMousePos = (curMousePos - m_mouseOffset) / m_mouseSacle;
	if (event.isKeyBoardDown())
	{
		if (!m_isInputEnable) return;
		switch (event.keyValue)
		{
		case XKEY_S:
			if (m_withFilename)
			{
				if (m_mesh.save(m_filename.c_str())) XEG.setTopMsgStr("保存成功");
				else XEG.setTopMsgStr("保存失败");
			}
			else
			{
				if (m_mesh.save()) XEG.setTopMsgStr("保存成功");
				else XEG.setTopMsgStr("保存失败");
			}
			break;
		case XKEY_L:
			if (m_withFilename)
			{
				if (m_mesh.load())
				{
					XEG.setTopMsgStr("读取成功");
					m_mesh.updateMesh();
				}
				else XEG.setTopMsgStr("读取失败");
			}
			else
			{
				if (m_mesh.load(m_filename.c_str()))
				{
					XEG.setTopMsgStr("读取成功");
					m_mesh.updateMesh();
				}
				else XEG.setTopMsgStr("读取失败");
			}
			break;
		case XKEY_A:m_showHandle = !m_showHandle; break;
		case XKEY_Q:reset(); break;
		case XKEY_M:
			if (m_isMouseDown)
			{
				m_mouseDownPos = curMousePos;
				m_changeOldPoint = m_mesh.getHandlePoses();
			}
			m_changeType = BD_CHANGE_MOVE;
			break;
		case XKEY_R:
			if (m_isMouseDown)
			{
				m_mouseDownPos = curMousePos;
				m_changeOldPoint = m_mesh.getHandlePoses();
			}
			m_changeType = BD_CHANGE_ROTATE;
			break;
		case XKEY_C:
			if (m_isMouseDown)
			{
				m_mouseDownPos = curMousePos;
				m_changeOldPoint = m_mesh.getHandlePoses();
			}
			m_changeType = BD_CHANGE_SCALE;
			break;
		case XKEY_LEFT:
			if (m_mesh.getSpecialIndex() >= 0)
			{
				int index = m_mesh.getSpecialIndex();
				XVec2 tmp = m_mesh.getHandlePos(index);
				m_mesh.setHandlePos(tmp + XVec2(-1.0f, 0.0f), index);
				m_mesh.updateMesh();
			}
			break;
		case XKEY_RIGHT:
			if (m_mesh.getSpecialIndex() >= 0)
			{
				int index = m_mesh.getSpecialIndex();
				XVec2 tmp = m_mesh.getHandlePos(index);
				m_mesh.setHandlePos(tmp + XVec2::oneZero, index);
				m_mesh.updateMesh();
			}
			break;
		case XKEY_UP:
			if (m_mesh.getSpecialIndex() >= 0)
			{
				int index = m_mesh.getSpecialIndex();
				XVec2 tmp = m_mesh.getHandlePos(index);
				m_mesh.setHandlePos(tmp + XVec2(0.0f, -1.0f), index);
				m_mesh.updateMesh();
			}
			break;
		case XKEY_DOWN:
			if (m_mesh.getSpecialIndex() >= 0)
			{
				int index = m_mesh.getSpecialIndex();
				XVec2 tmp = m_mesh.getHandlePos(index);
				m_mesh.setHandlePos(tmp + XVec2::zeroOne, index);
				m_mesh.updateMesh();
			}
			break;
		}
	}
	if (event.isKeyBoardUp())
	{
		switch (event.keyValue)
		{
		case XKEY_M:if (m_changeType == BD_CHANGE_MOVE) m_changeType = BD_CHANGE_NULL; break;
		case XKEY_R:if (m_changeType == BD_CHANGE_ROTATE) m_changeType = BD_CHANGE_NULL; break;
		case XKEY_C:if (m_changeType == BD_CHANGE_SCALE) m_changeType = BD_CHANGE_NULL; break;
		}
	}
	if (event.type == EVENT_MOUSE)
	{
		if (m_changeType == BD_CHANGE_NULL)
		{
			switch (event.mouseState)
			{
			case MOUSE_LEFT_BUTTON_DOWN:
				m_isMouseDown = true;
				m_oldMousePos = curMousePos;
				break;
			case MOUSE_LEFT_BUTTON_UP:m_isMouseDown = false; break;
			case MOUSE_MOVE:
				if (!m_showHandle) break;
				if (!m_isMouseDown)
				{
					m_mesh.setSpecialIndex(m_mesh.getNearestPointIndex(getScalePos(curMousePos), m_mouseTouchDistance));
				}
				else
				{
					if (m_mesh.getSpecialIndex() >= 0)
					{
						XVec2 offset = curMousePos - m_oldMousePos;
						if (m_withScale) offset /= m_scale;
						int index = m_mesh.getSpecialIndex();
						offset += m_mesh.getHandlePos(index);
						m_mesh.setHandlePos(offset, index);
						m_mesh.updateMesh();
						m_oldMousePos = curMousePos;
					}
				}
				break;
			}
		}
		else
		if (m_showHandle)
		{//下面做相应的计算
			switch (event.mouseState)
			{
			case MOUSE_LEFT_BUTTON_DOWN:
				m_isMouseDown = true;
				m_mouseDownPos = curMousePos;
				m_oldMousePos = curMousePos;
				m_changeOldPoint = m_mesh.getHandlePoses();
				break;
			case MOUSE_LEFT_BUTTON_UP:m_isMouseDown = false; break;
			case MOUSE_MOVE:
				if (!m_isMouseDown) break;
				switch (m_changeType)
				{
				case BD_CHANGE_MOVE:
					if (m_withScale)
					{
						XVec2 offset = curMousePos - m_mouseDownPos;
						offset /= m_scale;
						for (int i = 0; i < m_mesh.getHandleSum(); ++i)
							m_mesh.setHandlePos(m_changeOldPoint[i] + offset, i);
					}
					else
					{
						XVec2 offset = curMousePos - m_mouseDownPos;
						for (int i = 0; i < m_mesh.getHandleSum(); ++i)
							m_mesh.setHandlePos(m_changeOldPoint[i] + offset, i);
					}
					m_mesh.updateMesh();
					break;
				case BD_CHANGE_SCALE:
					{
						float r = 1.0f;
						if (curMousePos.x <= m_mouseDownPos.x) r = (m_mouseDownPos.x - curMousePos.x) / 300.0f + 1.0f;	//放大
						else r = 1.0f - (curMousePos.x - m_mouseDownPos.x) / 600.0f;//缩小
						if (r < 0.0f) r = 0.0f;
						for (int i = 0; i < m_mesh.getHandleSum(); ++i)
							m_mesh.setHandlePos((m_changeOldPoint[i] - m_mouseDownPos) * r + m_mouseDownPos, i);
						m_mesh.updateMesh();
					}
					break;
				case BD_CHANGE_ROTATE:
					{
						float r = (curMousePos.x - m_mouseDownPos.x) / 300.0f * PI;
						float sr = sin(r);
						float cr = cos(r);
						XVec2 tmp;
						for (int i = 0; i < m_mesh.getHandleSum(); ++i)
						{
							tmp = m_changeOldPoint[i] - m_mouseDownPos;
							m_mesh.setHandlePos(XVec2(tmp.x * cr - tmp.y * sr, tmp.x * sr + tmp.y * cr) + m_mouseDownPos, i);
						}
						m_mesh.updateMesh();
					}
					break;
				}
				break;
			}
		}
	}
}
}