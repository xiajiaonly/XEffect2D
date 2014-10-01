//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XControlManager.h"
#include "XButton.h"
#include "XCheck.h"
#include "XEdit.h"
#include "XMouseRightButtonMenu.h"
#include "XSlider.h"
#include "XRadios.h"
#include "XMultiText.h"
#include "XMultiList.h"
#include "XCombo.h"

_XBool _XControlManager::addACtrl(_XControlBasic * object)
{
	if(findObjectID(object) >= 0) return XFalse;				//防止物件重复注册
	_XCtrlObjetInfo tmp;
	tmp.pObject = object;
	tmp.neadInput = XTrue;
	tmp.needDraw = XTrue;
	m_ctrlDrawOrderMap.push_back(m_ctrlObjInfos.size());
	m_ctrlObjInfos.push_back(tmp);
//	printf("add one ctrl obj:%d\n",m_objectSum - 1);
	return XTrue;
}
_XBool _XControlManager::addAFunction(void (* function)())			//注册一个物件,返回注册的ID，实际上是序列编号，-1为注册失败
{
	if(findObjectID(function) >= 0) return XFalse;				//防止物件重复注册
	_XCtrlObjetInfo tmp;
	tmp.pFunction = function;
	m_ctrlDrawOrderMap.push_back(m_ctrlObjInfos.size());
	m_ctrlObjInfos.push_back(tmp);
//	printf("add one ctrl obj:%d\n",m_objectSum - 1);
	return XTrue;
}
void _XControlManager::decreaseAObject(int objectID)
{
	if(objectID < 0 || objectID >= m_ctrlObjInfos.size()) return;	//输入参数非法
	if(objectID == m_ctrlObjInfos.size() - 1)
	{//删除的是最后一个元素
		m_ctrlObjInfos.pop_back();
		for(int i = 0;i < m_ctrlDrawOrderMap.size();++ i)
		{
			if(m_ctrlDrawOrderMap[i] > m_ctrlDrawOrderMap[objectID]) --m_ctrlDrawOrderMap[i];
		}
		m_ctrlDrawOrderMap.pop_back();
	}else
	{
		int order = m_ctrlDrawOrderMap[objectID];
		for(int i = 0;i < m_ctrlDrawOrderMap.size();++ i)
		{
			if(m_ctrlDrawOrderMap[i] > order) --m_ctrlDrawOrderMap[i];
		}

		for(int i = objectID;i < m_ctrlObjInfos.size() - 1;++ i)
		{
			m_ctrlObjInfos[i] = m_ctrlObjInfos[i + 1];
			m_ctrlDrawOrderMap[i] = m_ctrlDrawOrderMap[i + 1];
		}
		m_ctrlObjInfos.pop_back();
		m_ctrlDrawOrderMap.pop_back();
	}
	if(m_focusOrder == objectID) m_focusOrder = -1;	//焦点物件被删除的话，这里需要重置焦点
//	printf("add one ctrl obj:%d\n",m_objectSum - 1);
}
int _XControlManager::findObjectID(const void * object) const
{
	if(object == NULL) return -1;
	for(int i = 0;i < m_ctrlObjInfos.size();++ i)
	{
		if(m_ctrlObjInfos[i].pObject == object) return i;
	}
	return -1;
}
int _XControlManager::findObjectID(void (* function)()) const
{
	if(function == NULL) return -1;
	for(int i = 0;i < m_ctrlObjInfos.size();++ i)
	{
		if(m_ctrlObjInfos[i].pFunction == function) return i;
	}
	return -1;
}
void _XControlManager::mouseProc(int x,int y,_XMouseState mouseState)
{
	int index;
	if(m_focusOrder < 0 || m_focusOrder >= m_ctrlObjInfos.size())
	{//当前没有焦点控件
		if(mouseState != MOUSE_MOVE)
		{
			for(int i = 0;i < m_ctrlObjInfos.size();++ i)
			{
				index = m_ctrlDrawOrderMap[i];
				if(m_ctrlObjInfos[index].pObject != NULL
					&& m_ctrlObjInfos[index].neadInput) 
				{
					if(m_ctrlObjInfos[index].pObject->canGetFocus(x,y)) 
					{
						m_focusOrder = i;
						break;
					}
				}
			}
		}else
		{
			for(int i = 0;i < m_ctrlObjInfos.size();++ i)
			{
				index = m_ctrlDrawOrderMap[i];
				if(i != m_focusOrder && m_ctrlObjInfos[index].pObject != NULL
					&& m_ctrlObjInfos[index].neadInput) 
				{//不是焦点的物件也可以响应鼠标悬浮事件
					m_ctrlObjInfos[index].pObject->mouseProc(x,y,mouseState);
				}
			}
		}
	}else
	{
		if(mouseState != MOUSE_MOVE)
		{
			if(m_ctrlObjInfos[m_ctrlDrawOrderMap[m_focusOrder]].pObject->canLostFocus(x,y))
			{//如果当前物件可以失去焦点，则这里可以处理焦点争执问题
				int tempFocus = -1;
				for(int i = 0;i < m_ctrlObjInfos.size();++ i)
				{
					index = m_ctrlDrawOrderMap[i];
					if(m_ctrlObjInfos[index].pObject != NULL &&
						m_ctrlObjInfos[index].neadInput) 
					{
						if(m_ctrlObjInfos[index].pObject->canGetFocus(x,y)) 
						{
							tempFocus = i;
							break;
						}
					}
				}
				if(tempFocus >= 0 && tempFocus != m_focusOrder) 
				{
					index = m_ctrlDrawOrderMap[m_focusOrder];
					if(m_ctrlObjInfos[index].pObject != NULL) 
					{
						m_ctrlObjInfos[index].pObject->setLostFocus();
					}
					m_focusOrder = tempFocus;//切换焦点
				}
			}
		}
	}
	if(m_focusOrder >= 0 && m_focusOrder < m_ctrlObjInfos.size())
	{
		index = m_ctrlDrawOrderMap[m_focusOrder];
		if(m_ctrlObjInfos[index].pObject != NULL) 
		{
			m_ctrlObjInfos[index].pObject->mouseProc(x,y,mouseState);
		}
		if(mouseState == MOUSE_MOVE)
		{
			for(int i = 0;i < m_ctrlObjInfos.size();++ i)
			{
				index = m_ctrlDrawOrderMap[i];
				if(i != m_focusOrder && m_ctrlObjInfos[index].pObject != NULL) 
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
void _XControlManager::keyProc(int keyOrder,_XKeyState keyState)
{
	if(keyOrder == XKEY_LSHIFT)
	{
		switch(keyState)
		{
		case KEY_STATE_UP:m_isShiftDown = false;break;
		case KEY_STATE_DOWN:m_isShiftDown = true;break;
		}
	}
	int index;
	if(!m_isShiftDown)
	{//正向
		if(keyOrder == XKEY_TAB && keyState == KEY_STATE_UP)
		{//切换焦点
			bool beMoveFocus = false;
			int i;
			if(m_focusOrder < 0) i = 0;	//第一次选择
			else 
			{
				if(m_ctrlObjInfos[m_focusOrder].pObject != NULL && 
					m_ctrlObjInfos[m_focusOrder].pObject->canBeChoose() &&
					m_ctrlObjInfos[m_focusOrder].neadInput &&
					!m_ctrlObjInfos[m_focusOrder].pObject->isFocus())
					i = m_focusOrder;	//当前物体可以获取焦点
				else
					i = m_focusOrder + 1;	//当前物体已经不能再获取焦点
			}
			for(;i < m_ctrlObjInfos.size();++ i)
			{//找到一个可以处于焦点状态的控件
				index = m_ctrlDrawOrderMap[i];
				if(m_ctrlObjInfos[index].pObject != NULL && 
					m_ctrlObjInfos[index].neadInput && 
					m_ctrlObjInfos[index].pObject->canBeChoose())
				{
					if(m_focusOrder >= 0) m_ctrlObjInfos[m_ctrlDrawOrderMap[m_focusOrder]].pObject->setLostFocus();
					m_ctrlObjInfos[index].pObject->setFocus();
					m_focusOrder = i;
					beMoveFocus = true;
					break;
				}
			}
			if(!beMoveFocus && m_focusOrder >= 0)
			{//选择焦点
				for(int i = 0;i < m_ctrlObjInfos.size();++ i)
				{
					index = m_ctrlDrawOrderMap[i];
					if(m_ctrlObjInfos[index].pObject != NULL &&
						m_ctrlObjInfos[index].neadInput &&
						m_ctrlObjInfos[index].pObject->canBeChoose())
					{
						m_ctrlObjInfos[m_ctrlDrawOrderMap[m_focusOrder]].pObject->setLostFocus();
						m_ctrlObjInfos[index].pObject->setFocus();
						m_focusOrder = i;
						beMoveFocus = true;
						break;
					}
				}
			}
		}
	}else
	{//反向
		if(keyOrder == XKEY_TAB && keyState == KEY_STATE_UP)
		{//切换焦点
			bool beMoveFocus = false;
			int i;
			if(m_focusOrder < 0) i = m_ctrlObjInfos.size() - 1;
			else 
			{
				if(m_ctrlObjInfos[m_focusOrder].pObject != NULL && 
					m_ctrlObjInfos[m_focusOrder].pObject->canBeChoose() &&
					m_ctrlObjInfos[m_focusOrder].neadInput &&
					!m_ctrlObjInfos[m_focusOrder].pObject->isFocus())
					i = m_focusOrder;
				else
					i = m_focusOrder - 1;
			}
			for(;i >= 0;-- i)
			{
				index = m_ctrlDrawOrderMap[i];
				if(m_ctrlObjInfos[index].pObject != NULL && 
					m_ctrlObjInfos[index].neadInput &&
					m_ctrlObjInfos[index].pObject->canBeChoose())
				{
					if(m_focusOrder >= 0) m_ctrlObjInfos[m_ctrlDrawOrderMap[m_focusOrder]].pObject->setLostFocus();
					m_ctrlObjInfos[index].pObject->setFocus();
					m_focusOrder = i;
					beMoveFocus = true;
					break;
				}
			}
			if(!beMoveFocus && m_focusOrder >= 0)
			{
				for(int i = m_ctrlObjInfos.size() - 1;i >= 0;-- i)
				{
					index = m_ctrlDrawOrderMap[i];
					if(m_ctrlObjInfos[index].pObject != NULL &&
						m_ctrlObjInfos[index].neadInput &&
						m_ctrlObjInfos[index].pObject->canBeChoose())
					{
						m_ctrlObjInfos[m_ctrlDrawOrderMap[m_focusOrder]].pObject->setLostFocus();
						m_ctrlObjInfos[index].pObject->setFocus();
						m_focusOrder = i;
						beMoveFocus = true;
						break;
					}
				}
			}
		}
	}
	if(m_focusOrder < 0 || m_focusOrder >= m_ctrlObjInfos.size()) return;
	index = m_ctrlDrawOrderMap[m_focusOrder];
	if(m_ctrlObjInfos[index].pObject != NULL) 
			m_ctrlObjInfos[index].pObject->keyboardProc(keyOrder,keyState);
	//for(int i = 0;i < m_objectSum;++ i)
	//{
	//	if(m_objectType[m_objectDrawOrderMap[i]] != CTRL_OBJ_FUNCTION && m_objectType[m_objectDrawOrderMap[i]] != CTRL_OBJ_NULL) 
	//		m_pObject[m_objectDrawOrderMap[i]].pObject->keyboardProc(keyOrder,keyState);
	//}
}
void _XControlManager::update(int stepTime)
{
	if(m_ctrlObjInfos.size() <= 0) return;
	int index;
	for(int i = m_ctrlObjInfos.size() - 1;i >= 0;-- i)
	{
		index = m_ctrlDrawOrderMap[i];
		if(m_ctrlObjInfos[index].pObject != NULL) 
			m_ctrlObjInfos[index].pObject->update(stepTime);
	}
}
void _XControlManager::draw()
{
	if(m_isAutoDraw == 0 ||
		m_ctrlObjInfos.size() <= 0) return;
	int index;
	////由于tab控件的层级关系，这里优先描绘tab控件，这种方案不好，如果存在tab中嵌套tab就会出现问题，这里需要修正
	//for(int i = m_objectSum - 1;i >= 0;-- i)
	//{
	//	index = m_objectDrawOrderMap[i];
	//	if(m_objectType[index] == CTRL_OBJ_XTAB) m_pObject[index].pObject->draw();
	//}
	//for(int i = m_objectSum - 1;i >= 0;-- i)
	//{
	//	index = m_objectDrawOrderMap[i];
	//	if(m_objectType[index] == CTRL_OBJ_FUNCTION) m_pObject[index].pFunction();
	//	else 
	//	if(m_objectType[index] != CTRL_OBJ_NULL
	//		&& m_objectType[index] != CTRL_OBJ_XTAB) m_pObject[index].pObject->draw();
	//}
	for(int i = m_ctrlObjInfos.size() - 1;i >= 0;-- i)
	{
		index = m_ctrlDrawOrderMap[i];
		if(!m_ctrlObjInfos[index].needDraw) continue;
		//if(m_ctrlObjInfos[index].pFunction != NULL) m_ctrlObjInfos[index].pFunction();
		//else 
		//if(m_ctrlObjInfos[index].pObject != NULL) m_ctrlObjInfos[index].pObject->draw();
		if(m_ctrlObjInfos[index].pFunction != NULL) m_ctrlObjInfos[index].pFunction();
		else m_ctrlObjInfos[index].pObject->draw();
	}
	_XControlBasic *p = NULL;
	for(int i = m_ctrlObjInfos.size() - 1;i >= 0;-- i)
	{
		index = m_ctrlDrawOrderMap[i];
		if(m_ctrlObjInfos[index].pObject != NULL) 
		{
			p = m_ctrlObjInfos[index].pObject;
			if(m_ctrlObjInfos[index].needDraw) p->drawUp();
			if(p->isFocus())
			{//如果处于激活状态，则这里画一个边框，标记为可以接受鼠标和键盘事件
			//	drawRect(_XRect(p->getBox(0) - _XVector2(1.0f,1.0f),p->getBox(2) + _XVector2(1.0f,1.0f)),1,0,0,0,1,1);
				drawRectAntiColor(_XRect(p->getBox(0) - _XVector2(1.0f,1.0f),p->getBox(2) + _XVector2(1.0f,1.0f)),0.5f,1);
			}
		}
	}
}
void _XControlManager::moveAndInsert(int objectID,int insertObjectID)
{
	if(objectID < 0 || objectID >= m_ctrlObjInfos.size() ||	//数据非法
		insertObjectID < 0 || insertObjectID >= m_ctrlObjInfos.size() ||	//数据非法
		objectID == insertObjectID) return;	//多余的操作
	if(m_ctrlDrawOrderMap[objectID] > m_ctrlDrawOrderMap[insertObjectID])
	{//下插上移
		for(int i = 0;i < m_ctrlObjInfos.size();++ i)
		{
			if(m_ctrlDrawOrderMap[i] < m_ctrlDrawOrderMap[objectID] && 
				m_ctrlDrawOrderMap[i] >= m_ctrlDrawOrderMap[insertObjectID]) ++m_ctrlDrawOrderMap[i];
		}
		m_ctrlDrawOrderMap[objectID] = m_ctrlDrawOrderMap[insertObjectID] - 1;
	}else
	{//上插下移
		for(int i = 0;i < m_ctrlObjInfos.size();++ i)
		{
			if(m_ctrlDrawOrderMap[i] > m_ctrlDrawOrderMap[objectID] && 
				m_ctrlDrawOrderMap[i] <= m_ctrlDrawOrderMap[insertObjectID]) --m_ctrlDrawOrderMap[i];
		}
		m_ctrlDrawOrderMap[objectID] = m_ctrlDrawOrderMap[insertObjectID] + 1;
	}
}