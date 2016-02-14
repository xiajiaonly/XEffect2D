#include "XStdHead.h"
#include "XCtrlManagerBase.h"
#include "XButton.h"
#include "XCheck.h"
#include "XEdit.h"
#include "XMouseRightButtonMenu.h"
#include "XSlider.h"
#include "XRadios.h"
#include "XMultiText.h"
#include "XMultiList.h"
#include "XCombo.h"
#include "../XGameBasic.h"
namespace XE{
XBool XCtrlManagerBase::addACtrl(XControlBasic * object)
{
	if(object == NULL) return XFalse;
	if(object->m_ctrlSpecialType == CTRL_TYPE_NORMAL)
	{
		if(findObjectID(object) >= 0) return XFalse;				//防止物件重复注册
		XCtrlObjetInfo tmp;
		tmp.pObject = object;
		tmp.needInput = XTrue;
		tmp.needDraw = XTrue;
		m_ctrlDrawOrderMap.push_back(m_ctrlObjInfos.size());
		m_ctrlObjInfos.push_back(tmp);
	//	printf("add one ctrl obj:%d\n",m_objectSum - 1);
	}else
	{
		if(findSpecialObjectID(object) >= 0) return XFalse;
		m_specialCtrls.push_back(object);
	}
	return XTrue;
}
XBool XCtrlManagerBase::addAFunction(void (* function)())			//注册一个物件,返回注册的ID，实际上是序列编号，-1为注册失败
{
	if(findObjectID(function) >= 0) return XFalse;				//防止物件重复注册
	XCtrlObjetInfo tmp;
	tmp.pFunction = function;
	m_ctrlDrawOrderMap.push_back(m_ctrlObjInfos.size());
	m_ctrlObjInfos.push_back(tmp);
//	printf("add one ctrl obj:%d\n",m_objectSum - 1);
	return XTrue;
}
void XCtrlManagerBase::clearAllObject()
{
	m_specialCtrls.clear();
	m_ctrlObjInfos.clear();
	m_ctrlDrawOrderMap.clear();
	m_focusOrder = -1;
}
void XCtrlManagerBase::decreaseAObject(int objectID)
{
	if(objectID < 0 || objectID >= m_ctrlObjInfos.size()) return;	//输入参数非法
	if(objectID == (int)(m_ctrlObjInfos.size()) - 1)
	{//删除的是最后一个元素
		for(unsigned int i = 0;i < m_ctrlDrawOrderMap.size();++ i)
		{
			if(m_ctrlDrawOrderMap[i] == objectID)
			{
				m_ctrlDrawOrderMap.erase(m_ctrlDrawOrderMap.begin() + i);
				break;
			}
		}
		m_ctrlObjInfos.pop_back();
	}else
	{
		m_ctrlObjInfos.erase(m_ctrlObjInfos.begin() + objectID);
		for(unsigned int i = 0;i < m_ctrlDrawOrderMap.size();++ i)
		{
			if(m_ctrlDrawOrderMap[i] == objectID)
			{
				m_ctrlDrawOrderMap.erase(m_ctrlDrawOrderMap.begin() + i);
				break;
			}
		}
		for(unsigned int i = 0;i < m_ctrlDrawOrderMap.size();++ i)
		{
			if(m_ctrlDrawOrderMap[i] > objectID) --m_ctrlDrawOrderMap[i];
		}
	}
	if(m_focusOrder == objectID) m_focusOrder = -1;	//焦点物件被删除的话，这里需要重置焦点
//	printf("add one ctrl obj:%d\n",m_objectSum - 1);
}
int XCtrlManagerBase::findObjectID(const void * object) const
{
	if(object == NULL) return -1;
	for(unsigned int i = 0;i < m_ctrlObjInfos.size();++ i)
	{
		if(m_ctrlObjInfos[i].pObject == object) return i;
	}
	return -1;
}
int XCtrlManagerBase::findSpecialObjectID(const void * object) const	//寻找特殊的物件
{
	if(object == NULL) return -1;
	for(unsigned int i = 0;i < m_specialCtrls.size();++ i)
	{
		if(m_specialCtrls[i] == object) return i;
	}
	return -1;
}
int XCtrlManagerBase::findObjectID(void (* function)()) const
{
	if(function == NULL) return -1;
	for(unsigned int i = 0;i < m_ctrlObjInfos.size();++ i)
	{
		if(m_ctrlObjInfos[i].pFunction == function) return i;
	}
	return -1;
}
void XCtrlManagerBase::mouseProc(int x,int y,XMouseState mouseState)
{
	//if(m_focusOrder >= 0 && m_focusOrder < m_ctrlObjInfos.size())
	//{
	//	if(m_ctrlObjInfos[m_focusOrder].pObject != NULL &&
	//		!m_ctrlObjInfos[m_focusOrder].pObject->m_isBeChoose)
	//	{//原来焦点的位物体失去焦点，则设置为焦点复位
	//		m_focusOrder = -1;
	//	}
	//}
	for(int i = 0;i < m_specialCtrls.size();++ i)
	{//下面描绘特殊物体更新
		if(m_specialCtrls[i]->mouseProc(x,y,mouseState) &&
			m_specialCtrls[i]->m_ctrlSpecialType == CTRL_TYPE_MOUSE)
			return;	//阻断消息传递
	}
	int index;
	if(m_focusOrder < 0 || m_focusOrder >= m_ctrlObjInfos.size())
	{//当前没有焦点控件
		if(mouseState != MOUSE_MOVE)
		{
			for(unsigned int i = 0;i < m_ctrlObjInfos.size();++ i)
			{
				index = m_ctrlDrawOrderMap[i];
				if(m_ctrlObjInfos[index].pObject != NULL
					&& m_ctrlObjInfos[index].needInput) 
				{
					if(m_ctrlObjInfos[index].pObject->canGetFocus(x,y)) 
					{
						m_focusOrder = index;
						break;
					}
				}
			}
		}else
		{
			for(unsigned int i = 0;i < m_ctrlObjInfos.size();++ i)
			{
				index = m_ctrlDrawOrderMap[i];
				if(index != m_focusOrder && m_ctrlObjInfos[index].pObject != NULL
					&& m_ctrlObjInfos[index].needInput) 
				{//不是焦点的物件也可以响应鼠标悬浮事件
					m_ctrlObjInfos[index].pObject->mouseProc(x,y,mouseState);
				}
			}
		}
	}else
	{
		if(mouseState != MOUSE_MOVE)
		{
			if(m_ctrlObjInfos[m_focusOrder].pObject->canLostFocus(x,y))
			{//如果当前物件可以失去焦点，则这里可以处理焦点争执问题
				int tempFocus = -1;
				for(unsigned int i = 0;i < m_ctrlObjInfos.size();++ i)
				{
					index = m_ctrlDrawOrderMap[i];
					if(m_ctrlObjInfos[index].pObject != NULL &&
						m_ctrlObjInfos[index].needInput && 
						m_ctrlObjInfos[index].pObject->canGetFocus(x,y)) 
					{
						tempFocus = index;
						break;
					}
				}
				if(tempFocus >= 0 && tempFocus != m_focusOrder) 
				{
					if(m_ctrlObjInfos[m_focusOrder].pObject != NULL) 
					{
						m_ctrlObjInfos[m_focusOrder].pObject->setLostFocus();
					}
					m_focusOrder = tempFocus;//切换焦点
				}
			}
		}
	}
	if(m_focusOrder >= 0 && m_focusOrder < m_ctrlObjInfos.size())
	{
		if(m_ctrlObjInfos[m_focusOrder].pObject != NULL && m_ctrlObjInfos[m_focusOrder].needInput) 
		{
			m_ctrlObjInfos[m_focusOrder].pObject->mouseProc(x,y,mouseState);
		}
		if(mouseState == MOUSE_MOVE)
		{
			for(unsigned int i = 0;i < m_ctrlObjInfos.size();++ i)
			{
				index = m_ctrlDrawOrderMap[i];
				if(index != m_focusOrder && m_ctrlObjInfos[index].pObject != NULL && m_ctrlObjInfos[index].needInput) 
				{//不是焦点的物件也可以响应鼠标悬浮事件
					m_ctrlObjInfos[index].pObject->mouseProc(x,y,mouseState);
				}
			}
		}
	}
	//for(int i = 0;i < m_objectSum;++ i)
	//{
	//	if(m_objectType[m_objectDrawOrderMap[i]] != CTRL_OBJ_FUNCTION && m_objectType[m_objectDrawOrderMap[i]] != CTRL_OBJ_NULL) 
	//	{
	//		m_pObject[m_objectDrawOrderMap[i]].pObject->mouseProc(x,y,mouseState);
	//	//	if(m_pObject[m_objectDrawOrderMap[i]].pObject->isInRect(x,y)) break;	//消息不能向下传递，如果在范围内则不能穿透当前物件
	//	}
	//}
}
void XCtrlManagerBase::keyProc(int keyOrder,XKeyState keyState)
{
	for(int i = 0;i < m_specialCtrls.size();++ i)
	{//下面描绘特殊物体更新
		if(m_specialCtrls[i]->keyboardProc(keyOrder,keyState) &&
			m_specialCtrls[i]->m_ctrlSpecialType == CTRL_TYPE_KYBOARD)
			return;	//阻断消息传递
	}
	if(keyOrder == XKEY_LSHIFT)
	{
		switch(keyState)
		{
		case KEY_STATE_UP:m_isShiftDown = false;break;
		case KEY_STATE_DOWN:m_isShiftDown = true;break;
		}
	}
	//int index;
	if(!m_isShiftDown)
	{//正向:注意，使用tab键进行切换的顺序是原始顺序，由于原始顺序是不会变化的，可以保证切换的遍历完整性
		if(keyOrder == XKEY_TAB && keyState == KEY_STATE_UP)
		{//切换焦点
			bool beMoveFocus = false;
			unsigned int i;
			if(m_focusOrder < 0) i = 0;	//第一次选择
			else 
			{
				if(m_ctrlObjInfos[m_focusOrder].pObject != NULL && 
					m_ctrlObjInfos[m_focusOrder].pObject->canBeChoose() &&
					m_ctrlObjInfos[m_focusOrder].needInput &&
					!m_ctrlObjInfos[m_focusOrder].pObject->isFocus())
					i = m_focusOrder;	//当前物体可以获取焦点
				else
					i = m_focusOrder + 1;	//当前物体已经不能再获取焦点
			}
			for(;i < m_ctrlObjInfos.size();++ i)
			{//找到一个可以处于焦点状态的控件
				if(m_ctrlObjInfos[i].pObject != NULL && 
					m_ctrlObjInfos[i].needInput && 
					m_ctrlObjInfos[i].pObject->canBeChoose())
				{
					if(m_focusOrder >= 0) m_ctrlObjInfos[m_focusOrder].pObject->setLostFocus();
					m_ctrlObjInfos[i].pObject->setFocus();
					m_focusOrder = i;
					beMoveFocus = true;
					break;
				}
			}
			if(!beMoveFocus && m_focusOrder >= 0)
			{//选择焦点
				for(i = 0;i < m_ctrlObjInfos.size();++ i)
				{
					if(m_ctrlObjInfos[i].pObject != NULL &&
						m_ctrlObjInfos[i].needInput &&
						m_ctrlObjInfos[i].pObject->canBeChoose())
					{
						m_ctrlObjInfos[m_focusOrder].pObject->setLostFocus();
						m_ctrlObjInfos[i].pObject->setFocus();
						m_focusOrder = i;
						beMoveFocus = true;
						break;
					}
				}
			}
		}
	}else
	{//反向:注意，使用tab键进行切换的顺序是原始顺序，由于原始顺序是不会变化的，可以保证切换的遍历完整性
		if(keyOrder == XKEY_TAB && keyState == KEY_STATE_UP)
		{//切换焦点
			bool beMoveFocus = false;
			int i;
			if(m_focusOrder < 0) i = (int)(m_ctrlObjInfos.size()) - 1;
			else 
			{
				if(m_ctrlObjInfos[m_focusOrder].pObject != NULL && 
					m_ctrlObjInfos[m_focusOrder].pObject->canBeChoose() &&
					m_ctrlObjInfos[m_focusOrder].needInput &&
					!m_ctrlObjInfos[m_focusOrder].pObject->isFocus())
					i = m_focusOrder;
				else
					i = m_focusOrder - 1;
			}
			for(;i >= 0;-- i)
			{
				if(m_ctrlObjInfos[i].pObject != NULL && 
					m_ctrlObjInfos[i].needInput &&
					m_ctrlObjInfos[i].pObject->canBeChoose())
				{
					if(m_focusOrder >= 0) m_ctrlObjInfos[m_focusOrder].pObject->setLostFocus();
					m_ctrlObjInfos[i].pObject->setFocus();
					m_focusOrder = i;
					beMoveFocus = true;
					break;
				}
			}
			if(!beMoveFocus && m_focusOrder >= 0)
			{
				for(i = (int)(m_ctrlObjInfos.size()) - 1;i >= 0;-- i)
				{
					if(m_ctrlObjInfos[i].pObject != NULL &&
						m_ctrlObjInfos[i].needInput &&
						m_ctrlObjInfos[i].pObject->canBeChoose())
					{
						m_ctrlObjInfos[m_focusOrder].pObject->setLostFocus();
						m_ctrlObjInfos[i].pObject->setFocus();
						m_focusOrder = i;
						beMoveFocus = true;
						break;
					}
				}
			}
		}
	}
	if(m_focusOrder < 0 || m_focusOrder >= m_ctrlObjInfos.size()) return;
	if(m_ctrlObjInfos[m_focusOrder].pObject != NULL && m_ctrlObjInfos[m_focusOrder].needInput) 
			m_ctrlObjInfos[m_focusOrder].pObject->keyboardProc(keyOrder,keyState);
	//for(int i = 0;i < m_objectSum;++ i)
	//{
	//	if(m_objectType[m_objectDrawOrderMap[i]] != CTRL_OBJ_FUNCTION && m_objectType[m_objectDrawOrderMap[i]] != CTRL_OBJ_NULL) 
	//		m_pObject[m_objectDrawOrderMap[i]].pObject->keyboardProc(keyOrder,keyState);
	//}
}
void XCtrlManagerBase::update(float stepTime)
{
	if(m_ctrlObjInfos.size() <= 0) return;
	int index;
	for(int i = (int)(m_ctrlObjInfos.size()) - 1;i >= 0;-- i)
	{
		index = m_ctrlDrawOrderMap[i];
		if(m_ctrlObjInfos[index].pObject != NULL) 
			m_ctrlObjInfos[index].pObject->update(stepTime);
	}
	//下面描绘特殊物体更新
	for(int i = 0;i < m_specialCtrls.size();++ i)
	{
		m_specialCtrls[i]->update(stepTime);
	}
}
void XCtrlManagerBase::draw()
{
	if(m_isAutoDraw == 0 ||
		m_ctrlObjInfos.size() <= 0) return;
	int index;
	//描绘下层
	for(int i = (int)(m_ctrlObjInfos.size()) - 1;i >= 0;-- i)
	{
		index = m_ctrlDrawOrderMap[i];
		if(!m_ctrlObjInfos[index].needDraw) continue;
		if(m_ctrlObjInfos[index].pFunction != NULL) 
			m_ctrlObjInfos[index].pFunction();
		else 
			m_ctrlObjInfos[index].pObject->draw();
	}
	//描绘上层
	XControlBasic *p = NULL;
	for(int i = (int)(m_ctrlObjInfos.size()) - 1;i >= 0;-- i)
	{
		index = m_ctrlDrawOrderMap[i];
		if(m_ctrlObjInfos[index].pObject == NULL) continue; 
		p = m_ctrlObjInfos[index].pObject;
		if(m_ctrlObjInfos[index].needDraw) p->drawUp();
		if(p->isFocus() && m_ctrlObjInfos[i].needDraw && m_withWireframe)//如果处于激活状态，则这里画一个边框，标记为可以接受鼠标和键盘事件
			XRender::drawRectAntiColor(XRect(p->getBox(0) - XVector2(1.0f,1.0f),p->getBox(2) + XVector2(1.0f,1.0f)),0.5f,XRender::LS_DASHES);
	}
	//下面描绘特殊物体
	for(int i = 0;i < m_specialCtrls.size();++ i)
	{
		m_specialCtrls[i]->draw();
	}
	for(int i = 0;i < m_specialCtrls.size();++ i)
	{
		m_specialCtrls[i]->drawUp();
	}
}
void XCtrlManagerBase::moveAndInsert(int objectID,int insertObjectID)
{
	if(objectID < 0 || objectID >= m_ctrlObjInfos.size() ||	//数据非法
		insertObjectID < 0 || insertObjectID >= m_ctrlObjInfos.size() ||	//数据非法
		objectID == insertObjectID) return;	//多余的操作
	int index1 = -1;
	int index2 = -1;
	for(unsigned int i = 0;i < m_ctrlDrawOrderMap.size();++ i)
	{
		if(m_ctrlDrawOrderMap[i] == objectID) index1 = i;else
		if(m_ctrlDrawOrderMap[i] == insertObjectID) index2 = i;
		if(index1 >= 0 && index2 >= 0) break;
	}
	if(index1 > index2)
	{
		for(int i = index1;i > index2;-- i)
		{
			m_ctrlDrawOrderMap[i] = m_ctrlDrawOrderMap[i - 1];
		}
		m_ctrlDrawOrderMap[index2] = objectID;
	}else
	{
		for(int i = index1;i < index2;++ i)
		{
			m_ctrlDrawOrderMap[i] = m_ctrlDrawOrderMap[i + 1];
		}
		m_ctrlDrawOrderMap[index2] = objectID;
	}
	//if(m_ctrlDrawOrderMap[objectID] > m_ctrlDrawOrderMap[insertObjectID])
	//{//下插上移
	//	for(unsigned int i = 0;i < m_ctrlObjInfos.size();++ i)
	//	{
	//		if(m_ctrlDrawOrderMap[i] < m_ctrlDrawOrderMap[objectID] && 
	//			m_ctrlDrawOrderMap[i] >= m_ctrlDrawOrderMap[insertObjectID]) ++m_ctrlDrawOrderMap[i];
	//	}
	//	m_ctrlDrawOrderMap[objectID] = m_ctrlDrawOrderMap[insertObjectID] - 1;
	//}else
	//{//上插下移
	//	for(unsigned int i = 0;i < m_ctrlObjInfos.size();++ i)
	//	{
	//		if(m_ctrlDrawOrderMap[i] > m_ctrlDrawOrderMap[objectID] && 
	//			m_ctrlDrawOrderMap[i] <= m_ctrlDrawOrderMap[insertObjectID]) --m_ctrlDrawOrderMap[i];
	//	}
	//	m_ctrlDrawOrderMap[objectID] = m_ctrlDrawOrderMap[insertObjectID] + 1;
	//}
}
void XCtrlManagerBase::eventProc(int id,int eventID)
{
	if(m_pGame != NULL) m_pGame->ctrlEventProc(id,eventID);
}
}