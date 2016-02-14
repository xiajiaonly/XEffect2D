#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XObjectManager.h"
#include "XSprite.h"
#include "XFrameEx.h"

#include "XNodeLine.h"
#include "XControl/XControlManager.h"
#include "XWindowTitle.h"
#include "XControl\XMultiListBasic.h"

namespace XE{
void XObjectManager::setShow()	
{
	m_isShowUI = XTrue;
	m_mutiList->setVisible();
}	//设置显示
void XObjectManager::disShow() 
{
	m_isShowUI = XFalse;
	m_mutiList->disVisible();
}	//设置不显示
void XObjectManager::ctrlProc(void*pClass,int,int eventID)
{
	switch(eventID)
	{
	case XMultiListBasic::MLTLST_SELECT:
	//	printf("select change! %d\n",((XObjectManager *)pClass)->m_mutiList->getSelectOrder());
		break;
	case XMultiListBasic::MLTLSTB_CHK_STATE_CHANGE:
		//printf("check change!\n");
		XObjectManager & pPar = *(XObjectManager *)pClass;
		//更新物件状态的改变
		int tempChooseOBJSum = pPar.getCurChooseObjectSum();
		for(int i = 0;i < tempChooseOBJSum;++ i)
		{
			if(pPar.m_objInfo[i].m_lineObjOrder >= 0)
			{
				if(pPar.m_mutiList->getCheckState(1,i)) pPar.m_objInfo[pPar.m_objInfo[i].m_lineObjOrder].m_objCanEdit = XTrue;
				else 
				{
					pPar.m_objInfo[pPar.m_objInfo[i].m_lineObjOrder].m_objCanEdit = XFalse;
					if(pPar.m_objInfo[pPar.m_objInfo[i].m_lineObjOrder].m_objectKeyOption != OBJ_OPTION_NULL) 
						pPar.m_objInfo[pPar.m_objInfo[i].m_lineObjOrder].m_objectKeyOption = OBJ_OPTION_NULL;	//设置成不可编辑状态
				}

				if(pPar.m_mutiList->getCheckState(0,i)) ((XObjectBasic *)(pPar.m_objInfo[pPar.m_objInfo[i].m_lineObjOrder].m_pObject))->setVisible();
				else ((XObjectBasic *)(pPar.m_objInfo[pPar.m_objInfo[i].m_lineObjOrder].m_pObject))->disVisible();
			}else
			{
				break;
			}
		}
		break;
	}
}
XObjectManager::XObjectManager()
	:m_isInited(XFalse)
	,m_isShowUI(XTrue)
	,m_isOption(XTrue)
	,m_editWindowsPos(0)
	,m_canSelect(XTrue)		//默认状态下是允许点选物件的
#if OBJ_MANAGER_WITH_ID
	,m_showObjID(0)
#endif
	,m_ctrlKeyState(KEY_STATE_UP)
	,m_canAddObj(XTrue)
{
	//put single object into auto_ptr object 
	//m_auto_ptr = auto_ptr<XObjectManager>(this);
	m_objInfo.clear();
	m_curMouseOnObjectSum = 0;
	//下面是关于按键连续动作的初始化的数据
	m_keyMaxTime = 500;
	m_keyMinTime = 10;
	for(int i = 0;i < 4;++ i)
	{
		m_keyState[i] = KEY_STATE_UP;
		m_keyTimer[i] = 0;
		m_keyCurTime[i] = m_keyMaxTime;
	}
	m_font = XMem::createMem<XFontUnicode>();
	if(m_font == NULL) LogStr("Mem Error!");
	m_mousePosFont = XMem::createMem<XFontUnicode>();
	if(m_mousePosFont == NULL) LogStr("Mem Error!");
	m_mutiList = XMem::createMem<XMultiListBasic>();
	if(m_mutiList == NULL) LogStr("Mem Error!");
}
int XObjectManager::addAObject(XObjectBasic * object)
{
//	if(!m_isInited) return -1;
	//if(m_curObjectSum >= MAX_OBJECT_SUM) return -1;	//物件已经满了，注册失败
	if(!m_canAddObj ||
		object == NULL ||
		object->m_objType == OBJ_NULL) return -1;	//无效的物件不能注册
	int ret = findObjectID(object);
	if(ret >= 0) return ret;	//如果物件已经注册，则防止重复注册
	//下面开始注册动作
	XObjectInfo tmp;
	tmp.m_objectMouseState = OBJ_STATE_NULL;		//默认状态
	//tmp.m_objectUpMouseState = OBJ_STATE_NULL;		//默认状态

	tmp.m_pObject = object;		//初始化指针无效
//	tmp.m_objectType = object->m_objType;	//无效的物件类型
	tmp.m_objectMousePoint.set(0.0f,0.0f);
	tmp.m_objectKeyOption = OBJ_OPTION_NULL;
	tmp.m_objectSetState = OBJ_OPTION_STATE_NULL;
	
	tmp.m_objectEditParm = -1;
#if OBJ_MANAGER_WITH_ID		//这里会造成递归调用而无线循环的问题
	m_canAddObj = XFalse;
	tmp.m_fontID = XMem::createMem<XFontUnicode>();
	if(tmp.m_fontID == NULL) return -1;
	tmp.m_fontID->setACopy(*m_font);
	decreaseAObject(tmp.m_fontID);
	tmp.m_fontID->setMaxStrLen(5);
	m_canAddObj = XTrue;
#endif

	//初始化物件的状态
	tmp.m_objCanEdit = XFalse;	//不可编辑
	tmp.m_objBeSelect = XFalse;	//不被选择
	
	m_objInfo.push_back(tmp);
	//printf("ObjectSum:%d\n",m_curObjectSum);
	return (int)(m_objInfo.size()) - 1;
}
void XObjectManager::decreaseAObject(unsigned int objectID)
{
//	if(!m_isInited) return;
	if(objectID >= m_objInfo.size()) return;	//传入参数非法
	if(m_objInfo.size() <= 0) return;	//如果序列中没有物件则直接返回
	//从序列中抽取这个注册，并从新整理序列
	if(objectID == m_objInfo.size() - 1)
	{//已经是最后一个序列
#if OBJ_MANAGER_WITH_ID
		XMem::XDELETE(m_objInfo[(int)(m_objInfo.size()) - 1].m_fontID);
#endif
		m_objInfo.pop_back();
	}else
	{//不是最后一个序列
#if OBJ_MANAGER_WITH_ID
		XMem::XDELETE(m_objInfo[objectID].m_fontID);
#endif
		//std::copy(m_objInfo[objectID + 1],m_objInfo[(int)(m_objInfo.size()) - 1],m_objInfo[objectID + 1]);
		for(int i = objectID;i < (int)(m_objInfo.size()) - 1;++ i)
		{
			m_objInfo[i] = m_objInfo[i + 1];
		}
#if OBJ_MANAGER_WITH_ID
		m_objInfo[(int)(m_objInfo.size()) - 1].m_fontID = NULL;
#endif
		m_objInfo.pop_back();
	}
	//printf("ObjectSum:%d\n",m_curObjectSum);
}
void XObjectManager::keyProc(int keyID,XKeyState keyState)
{
	if(!m_isInited	||	//如果没有初始化的话直接返回
		m_objInfo.size() == 0) return;	//如果没有物件直接返回
	if(keyID == XKEY_LCTRL || keyID == XKEY_RCTRL) m_ctrlKeyState = keyState;	//ctrl功能键的状态

	if(m_ctrlKeyState == KEY_STATE_DOWN && keyID == XKEY_U && keyState == KEY_STATE_UP)
	{
		if(!m_isOption) setOption();
		else disOption();
	}

	if(!m_isOption) return;
	switch(keyID)
	{
	case XKEY_H:	//是否显示UI界面
		if(keyState == KEY_STATE_UP &&
			m_ctrlKeyState == KEY_STATE_DOWN)
		{
			if(!m_isShowUI) setShow();
			else disShow();
		}
		break;
	case XKEY_P:	//设置属性窗口的位置
		if(keyState == KEY_STATE_UP &&
			m_ctrlKeyState == KEY_STATE_DOWN)
		{
			++ m_editWindowsPos;
			if(m_editWindowsPos >= 4) m_editWindowsPos = 0;
			setEditWidowsPos();
		}
		break;
	case XKEY_DELETE:	//删除属性窗口中选择的项
		if(keyState == KEY_STATE_UP)
		{//这个存在问题,目前尚不实现
			for(int i = 0;i < m_mutiList->getTableLineSum();++ i)
			{
				if(m_mutiList->getHaveSelectState(i))
				{
					m_objInfo[m_objInfo[i].m_lineObjOrder].m_objBeSelect = XFalse;	//取消选择
				}
			}
			upDateMultiListData();	//更新多行列表框的类容
		}
		break;
	case XKEY_N:		//是否可以使用鼠标点选物件
		if(keyState == KEY_STATE_UP) chengeCanSelectState();
		break;
#if OBJ_MANAGER_WITH_ID
	case XKEY_B:		//是否显示物件的ID
		if(keyState == KEY_STATE_UP) setShowObjID();
		break;
#endif
	case XKEY_Q:	//去除所有操作
		if(keyState == KEY_STATE_UP)
		{
			for(unsigned int i = 0;i < m_objInfo.size();++ i)
			{
				m_objInfo[i].m_objectKeyOption = OBJ_OPTION_NULL;
				updateObjStateInfo(i);
			}
		}
		break;
	case XKEY_E:	//nodeline物件进入编辑状态
		if(keyState == KEY_STATE_UP)
		{//物体编辑状态的切换
			for(unsigned int i = 0;i < m_objInfo.size();++ i)
			{
				if(m_objInfo[i].m_pObject->m_objType == OBJ_NODELINE && 
					m_objInfo[i].m_objBeSelect && m_objInfo[i].m_objCanEdit)
				{
					if(m_objInfo[i].m_objectKeyOption != OBJ_OPTION_EDIT)
					{
						m_objInfo[i].m_objectKeyOption = OBJ_OPTION_EDIT;
						printf("NodeLine Length:%f\n",((XNodeLine *)m_objInfo[i].m_pObject)->getNodeLineLength());
						updateObjStateInfo(i);
					}else
					{
						m_objInfo[i].m_objectKeyOption = OBJ_OPTION_NULL;
						updateObjStateInfo(i);
					}
				}
			}
		}
		break;
	case XKEY_S:	//缩放
		if(keyState == KEY_STATE_UP)
		{//弹起,此时设置所有被选中的物件进行缩放操作
			for(unsigned int i = 0;i < m_objInfo.size();++ i)
			{
				if(m_objInfo[i].m_objBeSelect && m_objInfo[i].m_objCanEdit)
				{//被选中切可编辑
					if(m_objInfo[i].m_pObject->m_objType == OBJ_NODELINE && 
						m_objInfo[i].m_objectKeyOption == OBJ_OPTION_EDIT)
					{//这里保存曲线到文件中
						((XNodeLine *)m_objInfo[i].m_pObject)->saveNodeLine();
					}else
					{
						m_objInfo[i].m_objectKeyOption = OBJ_OPTION_SIZE;
						updateObjStateInfo(i);
					}
				}
			}
		}
		break;
	case XKEY_R:	//旋转
		if(keyState == KEY_STATE_UP)
		{//弹起
			for(unsigned int i = 0;i < m_objInfo.size();++ i)
			{
				if(m_objInfo[i].m_objBeSelect && m_objInfo[i].m_objCanEdit)
				{//被选中且可编辑
					m_objInfo[i].m_objectKeyOption = OBJ_OPTION_ROTATE;
					updateObjStateInfo(i);
				}
			}
		}
		break;
	case XKEY_M:	//移动
		if(keyState == KEY_STATE_UP)
		{//弹起
			for(unsigned int i = 0;i < m_objInfo.size();++ i)
			{
				if(m_objInfo[i].m_objBeSelect && m_objInfo[i].m_objCanEdit)
				{//被选中且可编辑
					m_objInfo[i].m_objectKeyOption = OBJ_OPTION_POSITION;
					updateObjStateInfo(i);
				}
			}
		}
		break;
	case XKEY_D:
		if(keyState == KEY_STATE_UP)
		{//弹起
			for(unsigned int i = 0;i < m_objInfo.size();++ i)
			{
				if(m_objInfo[i].m_objectKeyOption == OBJ_OPTION_EDIT && 
					m_objInfo[i].m_objCanEdit && m_objInfo[i].m_objBeSelect)
				{//引导曲线中删除当前的点
					XNodeLine * temp = (XNodeLine *)m_objInfo[i].m_pObject;
					if(temp->getNodeSum() > 2 && m_objInfo[i].m_objectEditParm >= 0 && m_objInfo[i].m_objectEditParm < temp->getNodeSum()) //必须要大于2个点才能删除点
					{
						temp->decreaseOneNode(m_objInfo[i].m_objectEditParm);
						if(m_objInfo[i].m_objectEditParm >= temp->getNodeSum()) m_objInfo[i].m_objectEditParm = temp->getNodeSum() - 1;
					}
				}
			}
		}
		break;
	case XKEY_A://引导曲线中在当前位置增加一个点
		if(keyState == KEY_STATE_UP)
		{//弹起
			for(unsigned int i = 0;i < m_objInfo.size();++ i)
			{
				if(m_objInfo[i].m_objectKeyOption == OBJ_OPTION_EDIT && 
					m_objInfo[i].m_objCanEdit && m_objInfo[i].m_objBeSelect)
				{//引导曲线中删除当前的点
					XNodeLine * temp = (XNodeLine *)m_objInfo[i].m_pObject;
					if(m_objInfo[i].m_objectEditParm >= 0 && m_objInfo[i].m_objectEditParm < temp->getNodeSum() && 
						temp->getScale().x != 0.0f && temp->getScale().y != 0.0f) //必须要大于2个点才能删除点
					{
						temp->addOneNode((m_curMousePosition.x - temp->getPosition().x) / temp->getScale().x,
							(m_curMousePosition.y - temp->getPosition().y) / temp->getScale().y,
							m_objInfo[i].m_objectEditParm);
					}
				}
			}
		}
		break;
	case XKEY_C:	//改变旋转基准点
		if(keyState == KEY_STATE_UP)
		{//弹起
			for(unsigned int i = 0;i < m_objInfo.size();++ i)
			{
				if(m_objInfo[i].m_objBeSelect && m_objInfo[i].m_objCanEdit)
				{//被选中且可编辑
					m_objInfo[i].m_objectKeyOption = OBJ_OPTION_ROTATEMODE;
					updateObjStateInfo(i);
				}
			}
		}
		break;
	case XKEY_T:	//设置翻转状态
		if(keyState == KEY_STATE_UP)
		{//弹起
			for(unsigned int i = 0;i < m_objInfo.size();++ i)
			{
				if(m_objInfo[i].m_objBeSelect && m_objInfo[i].m_objCanEdit)
				{//被选中且可编辑
					m_objInfo[i].m_objectKeyOption = OBJ_OPTION_TURNOVERMODE;
					updateObjStateInfo(i);
				}
			}
		}
		break;
	case XKEY_LEFT:
		if(keyState == KEY_STATE_DOWN)
		{//按下
			m_keyState[0] = KEY_STATE_DOWN;
			m_keyTimer[0] = 0;
			m_keyCurTime[0] = m_keyMaxTime;
		}else
		{//弹起，下面对物件进行相应的操作
			m_keyState[0] = KEY_STATE_UP;
		}
		break;
	case XKEY_RIGHT:
		if(keyState == KEY_STATE_DOWN)
		{//按下
			m_keyState[1] = KEY_STATE_DOWN;
			m_keyTimer[1] = 0;
			m_keyCurTime[1] = m_keyMaxTime;
		}else
		{//弹起
			m_keyState[1] = KEY_STATE_UP;
		}
		break;
	case XKEY_UP:
		if(keyState == KEY_STATE_DOWN)
		{//按下
			m_keyState[2] = KEY_STATE_DOWN;
			m_keyTimer[2] = 0;
			m_keyCurTime[2] = m_keyMaxTime;
		}else
		{//弹起
			m_keyState[2] = KEY_STATE_UP;
		}
		break;
	case XKEY_DOWN:
		if(keyState == KEY_STATE_DOWN)
		{//按下
			m_keyState[3] = KEY_STATE_DOWN;
			m_keyTimer[3] = 0;
			m_keyCurTime[3] = m_keyMaxTime;
		}else
		{//弹起
			m_keyState[3] = KEY_STATE_UP;
		}
		break;
	}
}
void XObjectManager::objectKeyOption(unsigned int order,XObjectOptionType optionType)
{
	if(order >= m_objInfo.size()) return;
	switch(optionType)
	{
	case OBJ_OPTION_SIZE_DOWN:	//缩小
		switch(m_objInfo[order].m_pObject->m_objType)
		{
		case OBJ_SPRITE:
		case OBJ_FRAMEEX:
		case OBJ_NUMBER:
		case OBJ_FONTUNICODE:
		case OBJ_FONTX:
		case OBJ_NODELINE:
		case OBJ_CONTROL:
			{
				XObjectBasic * temp = (XObjectBasic *)m_objInfo[order].m_pObject;
				XVector2 size = temp->getScale();
				if(size.x - 0.01f >= 0) size.x -= 0.01f;
				else size.x = 0.0f;
				if(size.y - 0.01f >= 0) size.y -= 0.01f;
				else size.y = 0.0f;
				temp->setScale(size);
			}
			break;
		}
		break;
	case OBJ_OPTION_SIZE_ON:	//放大
		switch(m_objInfo[order].m_pObject->m_objType)
		{
		case OBJ_SPRITE:
		case OBJ_FRAMEEX:
		case OBJ_NUMBER:
		case OBJ_FONTUNICODE:
		case OBJ_FONTX:
		case OBJ_NODELINE:
		case OBJ_CONTROL:
			{
				XObjectBasic * temp = (XObjectBasic *)m_objInfo[order].m_pObject;
				XVector2 size = temp->getScale();
				size += 0.01f;
				temp->setScale(size);
			}
			break;
		}
		break;
	case OBJ_OPTION_ROTATE_DOWN:	//角度缩小
		switch(m_objInfo[order].m_pObject->m_objType)
		{
		case OBJ_SPRITE:
		case OBJ_FRAMEEX:
		case OBJ_NUMBER:
		case OBJ_FONTUNICODE:
		case OBJ_FONTX:
		case OBJ_NODELINE:
			{
				XObjectBasic * temp = (XObjectBasic *)m_objInfo[order].m_pObject;
				float angle = temp->getAngle();
				angle -= 0.5f;
				if(angle < 0) angle += 360.0f;
				temp->setAngle(angle);
			}
			break;
		}
		break;
	case OBJ_OPTION_ROTATE_ON:	//角度变大
		switch(m_objInfo[order].m_pObject->m_objType)
		{
		case OBJ_SPRITE:
		case OBJ_FRAMEEX:
		case OBJ_NUMBER:
		case OBJ_FONTUNICODE:
		case OBJ_FONTX:
		case OBJ_NODELINE:
			{
				XObjectBasic * temp = (XObjectBasic *)m_objInfo[order].m_pObject;
				float angle = temp->getAngle();
				angle += 0.5f;
				if(angle > 360.0f) angle -= 360.0f;
				temp->setAngle(angle);
			}
			break;
		}
		break;
	case OBJ_OPTION_ROTATE_MODE_CHANGE:	//旋转模式的变化
		switch(m_objInfo[order].m_pObject->m_objType)
		{
		case OBJ_SPRITE:
			{
				XSprite * temp = (XSprite *)m_objInfo[order].m_pObject;
				XTransformMode mode = temp->getTransformMode();
				if(mode == POINT_LEFT_TOP) temp->setIsTransformCenter(POINT_CENTER);
				else temp->setIsTransformCenter(POINT_LEFT_TOP);
			}
			break;
		//case OBJ_FRAMEEX:
		//	{
		//		XFrameEx * temp = (XFrameEx *)m_pObject[order];
		//		XTransformMode mode = temp->getTransformMode();
		//		if(mode == POINT_LEFT_TOP) temp->setIsTransformCenter(POINT_CENTER);
		//		else temp->setIsTransformCenter(POINT_LEFT_TOP);
		//	}
		//	break;
		}
		break;
	case OBJ_OPTION_TURNOVER_MODE_CHANGE:	//旋转模式的变化
		switch(m_objInfo[order].m_pObject->m_objType)
		{
		case OBJ_SPRITE:
			{
				XSprite * temp = (XSprite *)m_objInfo[order].m_pObject;
				XTurnOverMode mode = temp->getTurnOverMode();
				if(mode == TURN_OVER_MODE_NULL) temp->setLeft2Right(); else
				if(mode == TURN_OVER_MODE_LEFT_TO_RIGHT) temp->setUp2Down();
				else temp->resetLeftRightUpDown();
			}
			break;
		//case OBJ_FRAMEEX:
		//	{
		//		XFrameEx * temp = (XFrameEx *)m_pObject[order];
		//		XTurnOverMode mode = temp->getTurnOverMode();
		//		if(mode == TURN_OVER_MODE_NULL) temp->setLeft2Right(); else
		//		if(mode == TURN_OVER_MODE_LEFT_TO_RIGHT) temp->setUp2Down();
		//		else temp->resetLeftRightUpDown();
		//	}
		//	break;
		}
		break;
	case OBJ_OPTIONX_ON:	//移动
		switch(m_objInfo[order].m_pObject->m_objType)
		{
		case OBJ_SPRITE:
		case OBJ_FRAMEEX:
		case OBJ_NUMBER:
		case OBJ_FONTUNICODE:
		case OBJ_FONTX:
		case OBJ_NODELINE:
		case OBJ_CONTROL:
			{
				XObjectBasic * temp = (XObjectBasic *)m_objInfo[order].m_pObject;
				XVector2 position = temp->getPosition();
				position.x += 1.0f;
				temp->setPosition(position);
			}
			break;
		}
		break;
	case OBJ_OPTIONX_DOWN:	//移动
		switch(m_objInfo[order].m_pObject->m_objType)
		{
		case OBJ_SPRITE:
		case OBJ_FRAMEEX:
		case OBJ_NUMBER:
		case OBJ_FONTUNICODE:
		case OBJ_FONTX:
		case OBJ_NODELINE:
		case OBJ_CONTROL:
			{
				XObjectBasic * temp = (XObjectBasic *)m_objInfo[order].m_pObject;
				XVector2 position = temp->getPosition();
				position.x -= 1.0f;
				temp->setPosition(position);
			}
			break;
		}
		break;
	case OBJ_OPTION_Y_ON:	//移动
		switch(m_objInfo[order].m_pObject->m_objType)
		{
		case OBJ_SPRITE:
		case OBJ_FRAMEEX:
		case OBJ_NUMBER:
		case OBJ_FONTUNICODE:
		case OBJ_FONTX:
		case OBJ_NODELINE:
		case OBJ_CONTROL:
			{
				XObjectBasic * temp = (XObjectBasic *)m_objInfo[order].m_pObject;
				XVector2 position = temp->getPosition();
				position.y += 1.0f;
				temp->setPosition(position);
			}
			break;
		}
		break;
	case OBJ_OPTION_Y_DOWN:	//移动
		switch(m_objInfo[order].m_pObject->m_objType)
		{
		case OBJ_SPRITE:
		case OBJ_FRAMEEX:
		case OBJ_NUMBER:
		case OBJ_FONTUNICODE:
		case OBJ_FONTX:
		case OBJ_NODELINE:
		case OBJ_CONTROL:
			{
				XObjectBasic * temp = (XObjectBasic *)m_objInfo[order].m_pObject;
				XVector2 position = temp->getPosition();
				position.y -= 1.0f;
				temp->setPosition(position);
			}
			break;
		}
		break;
	}
}
void XObjectManager::mouseProc(int x,int y,XMouseState eventType)
{//鼠标响应函数
	if(!m_isInited ||
		!m_isOption) return;
	m_curMouseOnObjectSum = 0;
	XBool stateChangeFlag = XFalse;	//选择的状态是否发生了改变
	if(m_isShowUI && m_mutiList->isInRect(x,y)) return;	//显示空间的时候消息不能向下传递

	switch(eventType)
	{
	case MOUSE_LEFT_BUTTON_DOWN:	//左键按下的动作
		//printf("Mouse Left Botton Down:%d,%d\n",x,y);
		for(unsigned int i = 0;i < m_objInfo.size();++ i)
		{//下面依次对每个物件进行鼠标响应
			switch(m_objInfo[i].m_pObject->m_objType)
			{
			case OBJ_NULL:
				break;
			case OBJ_SPRITE:
			case OBJ_FRAMEEX:
			case OBJ_NUMBER:
			case OBJ_FONTUNICODE:
			case OBJ_FONTX:
			case OBJ_CONTROL:
				{
					XObjectBasic * temp = (XObjectBasic *)m_objInfo[i].m_pObject;
					if(temp->isInRect(x,y))
					{//标记鼠标按下
						m_objInfo[i].m_objectMouseState = OBJ_STATE_MOUSE_DOWN;	//物体处于按下状态

						m_objInfo[i].m_objectMousePoint = temp->getPosition();
						m_objInfo[i].m_objectMousePoint.x -= x;
						m_objInfo[i].m_objectMousePoint.y -= y;
					}
				}
				break;
			case OBJ_NODELINE:
				{
					XObjectBasic * temp = (XObjectBasic *)m_objInfo[i].m_pObject;
					if(temp->isInRect(x,y))
					{//如果在物件上被点击则做出反应
						if(m_objInfo[i].m_objectKeyOption == OBJ_OPTION_EDIT)
						{//处于编辑状态
							XNodeLine *tempNodeLine = (XNodeLine *)m_objInfo[i].m_pObject;
							int tempOrder = -1;
							if(tempNodeLine->getScale().x != 0 && tempNodeLine->getScale().y != 0)
							{//防止除零操作
								tempOrder = tempNodeLine->getNodeOrder((x - tempNodeLine->getPosition().x) / tempNodeLine->getScale().x,
									(y - tempNodeLine->getPosition().y) / tempNodeLine->getScale().y,5);
								if(tempOrder >= 0)// && tempOrder != m_objectEditParm[i])
								{
									m_objInfo[i].m_objectEditParm = tempOrder;

									m_objInfo[i].m_objectMouseState = OBJ_STATE_MOUSE_DOWN;
									m_objInfo[i].m_objectMousePoint = tempNodeLine->getNode(tempOrder);
									m_objInfo[i].m_objectMousePoint.x -= x / tempNodeLine->getScale().x;
									m_objInfo[i].m_objectMousePoint.y -= y / tempNodeLine->getScale().y;
								}
							}
						}else
						{//非编辑状态
							m_objInfo[i].m_objectMouseState = OBJ_STATE_MOUSE_DOWN;
							m_objInfo[i].m_objectMousePoint = temp->getPosition();
							m_objInfo[i].m_objectMousePoint.x -= x;
							m_objInfo[i].m_objectMousePoint.y -= y;
						}
					}
				}
				break;
			default:
				break;
			}
		}
		break;
	case MOUSE_LEFT_BUTTON_UP:		//左键弹起动作
		//printf("Mouse Left Botton Up:%d,%d\n",x,y);
		for(unsigned int i = 0;i < m_objInfo.size();++ i)
		{//下面依次对每个物件进行鼠标响应
			switch(m_objInfo[i].m_pObject->m_objType)
			{
			case OBJ_NULL:
				break;
			case OBJ_SPRITE:
			case OBJ_FRAMEEX:
			case OBJ_NUMBER:
			case OBJ_FONTUNICODE:
			case OBJ_FONTX:
			case OBJ_NODELINE:
			case OBJ_CONTROL:
				{
					if(m_objInfo[i].m_objectMouseState == OBJ_STATE_MOUSE_DOWN)	//只要鼠标弹起，不管是否发生在物件位置都做响应判断
					{//如果在物件上被点击则做出反应
						if(m_objInfo[i].m_pObject->m_objType == OBJ_NODELINE)
						{
							if(m_objInfo[i].m_objectKeyOption != OBJ_OPTION_EDIT)
							{//非编辑状态
								if(m_canSelect) 
								{
									if(!m_objInfo[i].m_objBeSelect) m_objInfo[i].m_objBeSelect = XTrue;
									else 
									{
										m_objInfo[i].m_objBeSelect = XFalse;
										m_objInfo[i].m_objectKeyOption = OBJ_OPTION_NULL;
									}
								}
								m_objInfo[i].m_objectMouseState = OBJ_STATE_NULL;
							}else
							{//编辑状态
								m_objInfo[i].m_objectMouseState = OBJ_STATE_NULL;	//标记没有动作
							}
						}else
						{
							if(m_canSelect) 
							{
								if(!m_objInfo[i].m_objBeSelect) m_objInfo[i].m_objBeSelect = XTrue;
								else 
								{
									m_objInfo[i].m_objBeSelect = XFalse;
									m_objInfo[i].m_objectKeyOption = OBJ_OPTION_NULL;
								}
							}
							m_objInfo[i].m_objectMouseState = OBJ_STATE_NULL;
						}
						stateChangeFlag = XTrue;
					}else
					if(m_objInfo[i].m_objectMouseState == OBJ_STATE_MOUSE_MOVE)
					{//标记鼠标拖动状态结束
						stateChangeFlag = XTrue;
						m_objInfo[i].m_objectMouseState = OBJ_STATE_NULL;
					}
				}
				break;
			default:
				break;
			}
		}
		break;
	case MOUSE_MOVE:		//鼠标移动事件
		m_curMousePosition.set(x,y);
		for(unsigned int i = 0;i < m_objInfo.size();++ i)
		{//下面依次对每个物件进行鼠标响应
			switch(m_objInfo[i].m_pObject->m_objType)
			{
			case OBJ_NULL:
				break;
			case OBJ_SPRITE:
			case OBJ_FRAMEEX:
			case OBJ_NUMBER:
			case OBJ_FONTUNICODE:
			case OBJ_FONTX:
			case OBJ_NODELINE:
			case OBJ_CONTROL:
			{
				if(m_objInfo[i].m_objectMouseState == OBJ_STATE_NULL)	//悬浮事件的判断
				{//如果之前是无鼠标状态
					XObjectBasic * temp = m_objInfo[i].m_pObject;
					if(temp->isInRect(x,y))
					{//则现在为鼠标悬浮状态
						m_objInfo[i].m_objectMouseState = OBJ_STATE_MOUSE_ON;	//设置悬浮事件
						m_curMouseOnObjectSum ++;
					}
				}else
				if(m_objInfo[i].m_objectMouseState == OBJ_STATE_MOUSE_ON)	//如果已经进入悬浮状态，则这里判断是都悬浮状态结束
				{//如果已经处于悬浮状态，则这里判断是否退出悬浮状态
					XObjectBasic * temp = m_objInfo[i].m_pObject;
					if(!temp->isInRect(x,y)) m_objInfo[i].m_objectMouseState = OBJ_STATE_NULL;	//设置悬浮状态结束
				}else
				if((m_objInfo[i].m_objectMouseState == OBJ_STATE_MOUSE_DOWN || 
					m_objInfo[i].m_objectMouseState == OBJ_STATE_MOUSE_MOVE) && m_objInfo[i].m_objCanEdit)	//点击状态的话，这里设置跟随鼠标移动
				{//如果已经处于按下状态或者拖拽状态，并且可以编辑
					if(m_objInfo[i].m_pObject->m_objType != OBJ_NODELINE)
					{//改变物件的位置
						XObjectBasic * temp = m_objInfo[i].m_pObject;
						temp->setPosition(m_objInfo[i].m_objectMousePoint.x + x,m_objInfo[i].m_objectMousePoint.y + y);
					}else
					{//对于引导线可能是改变引导线的位置或者是改变引导线中某个点的位置
						if(m_objInfo[i].m_objectKeyOption == OBJ_OPTION_EDIT)
						{//改变NodeLine中的点的位置
							((XNodeLine *)m_objInfo[i].m_pObject)->setOneNode(m_objInfo[i].m_objectMousePoint.x + x / ((XNodeLine *)m_objInfo[i].m_pObject)->getScale().x,
								m_objInfo[i].m_objectMousePoint.y + y / ((XNodeLine *)m_objInfo[i].m_pObject)->getScale().y,m_objInfo[i].m_objectEditParm);
						}else
						{
							XObjectBasic * temp = m_objInfo[i].m_pObject;
							temp->setPosition(m_objInfo[i].m_objectMousePoint.x + x,m_objInfo[i].m_objectMousePoint.y + y);
						}
					}
					m_objInfo[i].m_objectMouseState = OBJ_STATE_MOUSE_MOVE;	//鼠标拖动状态
				}
			}
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
	if(stateChangeFlag) upDateMultiListData();//如果被选择的状态发生了改变的话，这里需要更新列表框
}
void XObjectManager::upDateMultiListData()
{
	m_mutiList->clearAllSelect();
	m_mutiList->setLineSum(0);
	char tempStr[256] = "";
	int lineOrder = 0;
	for(unsigned int i = 0;i < m_objInfo.size();++ i)
	{
		if(m_objInfo[i].m_objBeSelect)
		{
			m_mutiList->addALine();
			sprintf(tempStr,"%d",i);
			m_mutiList->setBoxStr(tempStr,lineOrder,0);
			sprintf(tempStr,"%.0f,%.0f",m_objInfo[i].m_position.x,m_objInfo[i].m_position.y);
			m_mutiList->setBoxStr(tempStr,lineOrder,3);
			sprintf(tempStr,"%.2f,%.2f",m_objInfo[i].m_scale.x,m_objInfo[i].m_scale.y);
			m_mutiList->setBoxStr(tempStr,lineOrder,4);
			sprintf(tempStr,"%.2f",m_objInfo[i].m_angle);
			m_mutiList->setBoxStr(tempStr,lineOrder,5);
			m_objInfo[i].m_objLineOrder = lineOrder;
			m_objInfo[lineOrder].m_lineObjOrder = i;
			updateObjStateInfo(i);	//必须要初始化了对应关系之后才能更新状态
			//switch(m_objectKeyOption[i])
			//{
			//case OBJ_OPTION_NULL:m_mutiList->setBoxStr("Null",lineOrder,1);break;
			//case OBJ_OPTION_POSITION:m_mutiList->setBoxStr("Position",lineOrder,1);break;
			//case OBJ_OPTION_SIZE:m_mutiList->setBoxStr("Size",lineOrder,1);break;
			//case OBJ_OPTION_ROTATE:m_mutiList->setBoxStr("Rotate",lineOrder,1);break;
			//case OBJ_OPTION_ROTATEMODE:m_mutiList->setBoxStr("RotateMode",lineOrder,1);break;
			//case OBJ_OPTION_TURNOVERMODE:m_mutiList->setBoxStr("TurnOverMode",lineOrder,1);break;
			//}
			//设置这个物件的可编辑状态
			if(!m_objInfo[i].m_objCanEdit) m_mutiList->setCheckState(1,lineOrder,XFalse);
			else m_mutiList->setCheckState(1,lineOrder,XTrue); 

			if((m_objInfo[i].m_pObject)->getVisible()) m_mutiList->setCheckState(0,lineOrder,XTrue);
			else m_mutiList->setCheckState(0,lineOrder,XFalse); 
			lineOrder ++;
		}else
		{
			m_objInfo[i].m_objLineOrder = -1;
		}
	}
}
void XObjectManager::updateObjStateInfo(int order)
{
	switch(m_objInfo[order].m_pObject->m_objType)
	{
	case OBJ_NULL:m_mutiList->setBoxStr("Null",m_objInfo[order].m_objLineOrder,1);break;
	case OBJ_SPRITE:m_mutiList->setBoxStr("Sprite",m_objInfo[order].m_objLineOrder,1);break;
	case OBJ_FRAME:m_mutiList->setBoxStr("Frame",m_objInfo[order].m_objLineOrder,1);break;
	case OBJ_FRAMEEX:m_mutiList->setBoxStr("FrameEx",m_objInfo[order].m_objLineOrder,1);break;
	case OBJ_NUMBER:m_mutiList->setBoxStr("Number",m_objInfo[order].m_objLineOrder,1);break;
	case OBJ_FONTUNICODE:m_mutiList->setBoxStr("FontUnicode",m_objInfo[order].m_objLineOrder,1);break;
	case OBJ_FONTX:m_mutiList->setBoxStr("Fontx",m_objInfo[order].m_objLineOrder,1);break;
	case OBJ_NODELINE:m_mutiList->setBoxStr("NodeLine",m_objInfo[order].m_objLineOrder,1);break;
	case OBJ_CONTROL:m_mutiList->setBoxStr("Control",m_objInfo[order].m_objLineOrder,1);break;
	}
	switch(m_objInfo[order].m_objectKeyOption)
	{
	case OBJ_OPTION_NULL:m_mutiList->setBoxStr("Null",m_objInfo[order].m_objLineOrder,2);break;
	case OBJ_OPTION_POSITION:m_mutiList->setBoxStr("Position",m_objInfo[order].m_objLineOrder,2);break;
	case OBJ_OPTION_SIZE:m_mutiList->setBoxStr("Size",m_objInfo[order].m_objLineOrder,2);break;
	case OBJ_OPTION_ROTATE:m_mutiList->setBoxStr("Rotate",m_objInfo[order].m_objLineOrder,2);break;
	case OBJ_OPTION_ROTATEMODE:m_mutiList->setBoxStr("RotateMode",m_objInfo[order].m_objLineOrder,2);break;
	case OBJ_OPTION_TURNOVERMODE:m_mutiList->setBoxStr("TurnOverMode",m_objInfo[order].m_objLineOrder,2);break;
	}
}
void XObjectManager::updateObjInfo(int order)
{
	//printf("Update Object Info -> %d.\n",order);
	XObjectBasic * temp = m_objInfo[order].m_pObject;
	m_objInfo[order].m_position = temp->getPosition();
	m_objInfo[order].m_scale = temp->getScale();
	m_objInfo[order].m_angle = temp->getAngle();
//	m_objInfo[order].m_alpha = temp->getAngle();

	char tempStr[256] = "";
#if OBJ_MANAGER_WITH_ID
	sprintf(tempStr,"%d",order);
	m_objInfo[order].m_fontID->setString(tempStr);
	XVector2 fontPosition = m_objInfo[order].m_position;
	if(fontPosition.x < 0.0f) fontPosition.x = 0.0f;	//防止文字显示超出屏幕范围，不过这里没有判断上限，只判断了下线
	if(fontPosition.x >= getSceneWidth() - m_objInfo[order].m_fontID->getMaxPixelWidth()) 
		fontPosition.x = getSceneWidth() - m_objInfo[order].m_fontID->getMaxPixelWidth();
	if(fontPosition.y < 0.0f) fontPosition.y = 0.0f;
	if(fontPosition.y >= getSceneHeight() - m_objInfo[order].m_fontID->getMaxPixelHeight()) 
		fontPosition.y = getSceneHeight() - m_objInfo[order].m_fontID->getMaxPixelHeight();
	m_objInfo[order].m_fontID->setPosition(fontPosition);
#endif
	if(m_objInfo[order].m_objLineOrder < 0) return;
	sprintf(tempStr,"%d",order);
	m_mutiList->setBoxStr(tempStr,m_objInfo[order].m_objLineOrder,0);
	updateObjStateInfo(order);
	sprintf(tempStr,"%.0f,%.0f",m_objInfo[order].m_position.x,m_objInfo[order].m_position.y);
	m_mutiList->setBoxStr(tempStr,m_objInfo[order].m_objLineOrder,3);
	sprintf(tempStr,"%.2f,%.2f",m_objInfo[order].m_scale.x,m_objInfo[order].m_scale.y);
	m_mutiList->setBoxStr(tempStr,m_objInfo[order].m_objLineOrder,4);
	sprintf(tempStr,"%.2f",m_objInfo[order].m_angle);
	m_mutiList->setBoxStr(tempStr,m_objInfo[order].m_objLineOrder,5);
}
XBool XObjectManager::checkNeedUpdate(int order)
{
	XObjectInfo &objInfo = m_objInfo[order];
	XObjectBasic * temp = objInfo.m_pObject;
	if(objInfo.m_position.x != temp->getPosition().x || objInfo.m_position.y != temp->getPosition().y) return XTrue;
	if(objInfo.m_scale.x != temp->getScale().x || objInfo.m_scale.y != temp->getScale().y) return XTrue;
	if(objInfo.m_angle != temp->getAngle()) return XTrue;
	if(objInfo.m_pObject->m_objType == OBJ_NODELINE && 
		objInfo.length != ((XNodeLine *)m_objInfo[order].m_pObject)->getNodeLineLength()) return XTrue;
	return XFalse;
}
void XObjectManager::release()
{
#if OBJ_MANAGER_WITH_ID
	for(unsigned int i = 0;i < m_objInfo.size();++ i)
	{
		XMem::XDELETE(m_objInfo[i].m_fontID);
	}
#endif
	m_objInfo.clear();
}
void XObjectManager::draw()	//当物件的数量太多时，这个函数的执行效率较低
{//显示物件的当前状态
	if(!m_isInited ||
		!m_isOption) return;

	for(unsigned int i = 0;i < m_objInfo.size();++ i)
	{
		if(m_objInfo[i].m_objectMouseState != OBJ_STATE_NULL || m_objInfo[i].m_objBeSelect)
		{//被选择状态或者鼠标状态下
			switch(m_objInfo[i].m_pObject->m_objType)
			{
			case OBJ_NULL:
				break;
			case OBJ_SPRITE:
			case OBJ_FRAMEEX:
			case OBJ_NUMBER:
			case OBJ_FONTUNICODE:
			case OBJ_FONTX:
			case OBJ_CONTROL:
				{
					if(checkNeedUpdate(i)) updateObjInfo(i);
					XObjectBasic * temp = m_objInfo[i].m_pObject;
					//在物件的位置描绘上十字
					XRender::drawLine(m_objInfo[i].m_position.x - 5,m_objInfo[i].m_position.y,m_objInfo[i].m_position.x + 5,m_objInfo[i].m_position.y,
						0.5f,1.0f,0.0f,0.0f,1.0f);
					XRender::drawLine(m_objInfo[i].m_position.x,m_objInfo[i].m_position.y - 5,m_objInfo[i].m_position.x,m_objInfo[i].m_position.y + 5,
						0.5f,1.0f,0.0f,0.0f,1.0f);
					//使用线条包围物件
					if(m_objInfo[i].m_objBeSelect)
					{//处于选中状态
						if(m_objInfo[i].m_objCanEdit)
						{//可编辑为白色
							if(m_mutiList->getHaveSelectState(m_objInfo[i].m_objLineOrder))
								XRender::drawBox(temp->getBox(0),temp->getBox(1),temp->getBox(2),temp->getBox(3),1.0f,0.0f,0.0f,1.0f);	//被选中的物件用蓝色
							else XRender::drawBox(temp->getBox(0),temp->getBox(1),temp->getBox(2),temp->getBox(3));
						}else
						{//不可编辑为黑色
							XRender::drawBox(temp->getBox(0),temp->getBox(1),temp->getBox(2),temp->getBox(3),1.0f,0.0f,0.0f,0.0f);
						}
					}else
					if(m_objInfo[i].m_objectMouseState == OBJ_STATE_MOUSE_DOWN)
					{//鼠标按下状态
						XRender::drawBox(temp->getBox(0),temp->getBox(1),temp->getBox(2),temp->getBox(3),1.0f,1.0f,0.0f,0.0f);
					}else
					if(m_objInfo[i].m_objectMouseState == OBJ_STATE_MOUSE_ON)
					{//鼠标悬浮状态
						XRender::drawBox(temp->getBox(0),temp->getBox(1),temp->getBox(2),temp->getBox(3),1.0f,1.0f,1.0f,0.0f);
					}
#if OBJ_MANAGER_WITH_ID
					if(m_showObjID != 0) m_objInfo[i].m_fontID->draw();
#endif
				}
				break;
			case OBJ_NODELINE:
				{
					if(checkNeedUpdate(i)) updateObjInfo(i);
					XObjectBasic * temp = m_objInfo[i].m_pObject;
					//在物件的位置描绘上十字
					XRender::drawLine(m_objInfo[i].m_position.x - 5,m_objInfo[i].m_position.y,m_objInfo[i].m_position.x + 5,m_objInfo[i].m_position.y,
						2,1.0f,0.0f,0.0f,1.0f);
					XRender::drawLine(m_objInfo[i].m_position.x,m_objInfo[i].m_position.y - 5,m_objInfo[i].m_position.x,m_objInfo[i].m_position.y + 5,
						2,1.0f,0.0f,0.0f,1.0f);
					if(m_objInfo[i].m_pObject->m_objType == OBJ_NODELINE)
					{//这里需要显示节点的相关信息
						XNodeLine * tempNodeLine = (XNodeLine *)m_objInfo[i].m_pObject;
						for(int j = 0;j < tempNodeLine->getNodeSum();++ j)
						{
							//描绘这个点
							if(j == 0)
							{//第一个点用绿色
								XRender::drawLine(tempNodeLine->getNode(j).x * tempNodeLine->getScale().x + tempNodeLine->getPosition().x,
									tempNodeLine->getNode(j).y * tempNodeLine->getScale().y + tempNodeLine->getPosition().y - 2,
									tempNodeLine->getNode(j).x * tempNodeLine->getScale().x + tempNodeLine->getPosition().x,
									tempNodeLine->getNode(j).y * tempNodeLine->getScale().y + tempNodeLine->getPosition().y + 2,
									4,0.0f,1.0f,0.0f);
							}else
							if(j == m_objInfo[i].m_objectEditParm)
							{//被选中的点使用白色
								XRender::drawLine(tempNodeLine->getNode(j).x * tempNodeLine->getScale().x + tempNodeLine->getPosition().x,
									tempNodeLine->getNode(j).y * tempNodeLine->getScale().y + tempNodeLine->getPosition().y - 2,
									tempNodeLine->getNode(j).x * tempNodeLine->getScale().x + tempNodeLine->getPosition().x,
									tempNodeLine->getNode(j).y * tempNodeLine->getScale().y + tempNodeLine->getPosition().y + 2,
									4);
							}else
							{//其他点用红色
								XRender::drawLine(tempNodeLine->getNode(j).x * tempNodeLine->getScale().x + tempNodeLine->getPosition().x,
									tempNodeLine->getNode(j).y * tempNodeLine->getScale().y + tempNodeLine->getPosition().y - 2,
									tempNodeLine->getNode(j).x * tempNodeLine->getScale().x + tempNodeLine->getPosition().x,
									tempNodeLine->getNode(j).y * tempNodeLine->getScale().y + tempNodeLine->getPosition().y + 2,
									4,1.0f,0.0f,0.0f);
							}
							if(j < tempNodeLine->getNodeSum() - 1)
							{
								XRender::drawLine(tempNodeLine->getNode(j).x * tempNodeLine->getScale().x + tempNodeLine->getPosition().x,
									tempNodeLine->getNode(j).y * tempNodeLine->getScale().y + tempNodeLine->getPosition().y ,
									tempNodeLine->getNode(j + 1).x * tempNodeLine->getScale().x + tempNodeLine->getPosition().x,
									tempNodeLine->getNode(j + 1).y * tempNodeLine->getScale().y + tempNodeLine->getPosition().y);
							}
						}
						XRender::drawLine(tempNodeLine->getSpecialPointPosition().x,tempNodeLine->getSpecialPointPosition().y - 2,
							tempNodeLine->getSpecialPointPosition().x,tempNodeLine->getSpecialPointPosition().y + 2,
							4,1.0f,1.0f,0.0f);
					}
					if(m_objInfo[i].m_objBeSelect || m_objInfo[i].m_objectMouseState  == OBJ_STATE_MOUSE_MOVE)
					{//编辑状态
						if(m_objInfo[i].m_objectKeyOption == OBJ_OPTION_EDIT)
						{
							XRender::drawBox(temp->getBox(0),temp->getBox(1),temp->getBox(2),temp->getBox(3));
						}
#if OBJ_MANAGER_WITH_ID
						if(m_showObjID != 0) m_objInfo[i].m_fontID->draw();
#endif
					}
				}
				break;
			}
		}
	}
	if(m_mutiList->getVisible()) m_mousePosFont->draw();
}
void XObjectManager::move(float delay)
{
	if(!m_isInited ||
		!m_isOption) return;
	m_mousePosFont->setString((XString::intToStr((int)(getMousePos().x),10) + "," + XString::intToStr((int)(getMousePos().y),10)).c_str());
	if(m_keyState[0] == KEY_STATE_DOWN)
	{//左键动作
		m_keyTimer[0] -= delay;
		if(m_keyTimer[0] <= 0)
		{//按键时间到
			m_keyCurTime[0] = (m_keyCurTime[0] >> 1);
			if(m_keyCurTime[0] < m_keyMinTime) m_keyCurTime[0] = m_keyMinTime;
			m_keyTimer[0] = m_keyCurTime[0];
			//下面响应一次按键动作
			for(unsigned int i = 0;i < m_objInfo.size();++ i)
			{
				switch(m_objInfo[i].m_objectKeyOption)
				{
				case OBJ_OPTION_SIZE:	//缩小
					objectKeyOption(i,OBJ_OPTION_SIZE_DOWN);
					break;
				case OBJ_OPTION_ROTATE:	
					objectKeyOption(i,OBJ_OPTION_ROTATE_DOWN);
					break;
				case OBJ_OPTION_ROTATEMODE:	
					objectKeyOption(i,OBJ_OPTION_ROTATE_MODE_CHANGE);
					break;
				case OBJ_OPTION_TURNOVERMODE:	
					objectKeyOption(i,OBJ_OPTION_TURNOVER_MODE_CHANGE);
					break;
				case OBJ_OPTION_POSITION:	
					objectKeyOption(i,OBJ_OPTIONX_DOWN);
					break;
				}
			}
		}
	}
	if(m_keyState[1] == KEY_STATE_DOWN)
	{//右键键动作
		m_keyTimer[1] -= delay;
		if(m_keyTimer[1] <= 0)
		{//按键时间到
			m_keyCurTime[1] = (m_keyCurTime[1] >> 1);
			if(m_keyCurTime[1] < m_keyMinTime) m_keyCurTime[1] = m_keyMinTime;
			m_keyTimer[1] = m_keyCurTime[1];
			//下面响应一次按键动作
			for(unsigned int i = 0;i < m_objInfo.size();++ i)
			{
				switch(m_objInfo[i].m_objectKeyOption)
				{
				case OBJ_OPTION_SIZE:	//放大
					objectKeyOption(i,OBJ_OPTION_SIZE_ON);
					break;
				case OBJ_OPTION_ROTATE:	
					objectKeyOption(i,OBJ_OPTION_ROTATE_ON);
					break;
				case OBJ_OPTION_POSITION:	
					objectKeyOption(i,OBJ_OPTIONX_ON);
					break;
				}
			}
		}
	}
	if(m_keyState[2] == KEY_STATE_DOWN)
	{//上键键动作
		m_keyTimer[2] -= delay;
		if(m_keyTimer[2] <= 0)
		{//按键时间到
			m_keyCurTime[2] = (m_keyCurTime[2] >> 1);
			if(m_keyCurTime[2] < m_keyMinTime) m_keyCurTime[2] = m_keyMinTime;
			m_keyTimer[2] = m_keyCurTime[2];
			//下面响应一次按键动作
			for(unsigned int i = 0;i < m_objInfo.size();++ i)
			{
				switch(m_objInfo[i].m_objectKeyOption)
				{
				case OBJ_OPTION_POSITION:	
					objectKeyOption(i,OBJ_OPTION_Y_DOWN);
					break;
				}
			}
		}
	}
	if(m_keyState[3] == KEY_STATE_DOWN)
	{//下键键动作
		m_keyTimer[3] -= delay;
		if(m_keyTimer[3] <= 0)
		{//按键时间到
			m_keyCurTime[3] = (m_keyCurTime[3] >> 1);
			if(m_keyCurTime[3] < m_keyMinTime) m_keyCurTime[3] = m_keyMinTime;
			m_keyTimer[3] = m_keyCurTime[3];
			//下面响应一次按键动作
			for(unsigned int i = 0;i < m_objInfo.size();++ i)
			{
			//	switch(m_objInfo[i].m_objectKeyOption)
			//	{
			//	case OBJ_OPTION_POSITION:	
			//		objectKeyOption(i,OBJ_OPTION_Y_ON);
			//		break;
			//	}
				if(m_objInfo[i].m_objectKeyOption == OBJ_OPTION_POSITION)
					objectKeyOption(i,OBJ_OPTION_Y_ON);
			}
		}
	}
}
XBool XObjectManager::init(XResourcePosition resoursePosition)
{
	if(m_isInited) return XTrue;
	//初始化字体
	if(!m_font->initEx((getCommonResPos() + "ResourcePack/pic/ObjectManager/Font").c_str(),
		resoursePosition)) return XFalse;
	decreaseAObject(m_font);
	m_mousePosFont->setACopy(*m_font);
	m_mousePosFont->setColor(XFColor::blue);
	m_mousePosFont->setScale(0.5f);
	decreaseAObject(m_mousePosFont);
//#if OBJ_MANAGER_WITH_ID
//	for(int i = 0;i < MAX_OBJECT_SUM;++ i)
//	{
//		m_objInfo[i].m_fontID.setACopy(m_font);
//		decreaseAObject(&(m_objInfo[i].m_fontID));
//		m_objInfo[i].m_fontID.setMaxStrLen(5);
//	}
//#endif

	//初始化一个列表框
	//if(!m_sliderTextureH.initEx("ResourcePack/pic/ObjectManager/MutiList/SliderH",resoursePosition)) return 0;
	//if(!m_sliderH.initEx(XVector2(200.0f,200.0f),m_sliderTextureH,SLIDER_TYPE_HORIZONTAL,100.0f,0.0f)) return 0;
	//XCtrlManager.decreaseAObject(&m_sliderH);
	//decreaseAObject(&m_sliderH);

	//if(!m_sliderTextureV.initEx("ResourcePack/pic/ObjectManager/MutiList/SliderV",resoursePosition)) return 0;
	//if(!m_sliderV.initEx(XVector2(200.0f,200.0f),m_sliderTextureV,SLIDER_TYPE_VERTICAL,100.0f,0.0f)) return 0;
	//XCtrlManager.decreaseAObject(&m_sliderV);
	//decreaseAObject(&m_sliderV);

	//if(!m_checkTex0.initEx("ResourcePack/pic/ObjectManager/MutiList/checkC",resoursePosition)) return 0;
	//if(!m_checkTex1.initEx("ResourcePack/pic/ObjectManager/MutiList/checkE",resoursePosition)) return 0;
	//if(!m_mutiListTexture.initEx("ResourcePack/pic/ObjectManager/MutiList",resoursePosition)) return 0;

	//if(!m_mutiList->initEx(XVector2(XEE::sceneX + XEE::sceneWidth - 289.0f,XEE::sceneY),m_mutiListTexture,m_checkTex0,m_checkTex1,m_font,1.0f,5,0,m_sliderV,m_sliderH)) return 0;
	if(!m_mutiList->initPlus((getCommonResPos() + "ResourcePack/pic/ObjectManager/MutiList").c_str(),
		*m_font,1.0f,5,0,resoursePosition)) return XFalse;
	//if(!m_mutiList->initWithoutSkin(XRect(64,0,500,500),*m_font,1.0f,5,0)) return 0;
	//m_mutiList->setPosition(XEE::sceneX + XEE::sceneWidth - 289.0f,XEE::sceneY);
	setEditWidowsPos();
	decreaseAObject(m_mutiList);
	m_mutiList->setScale(0.5f,0.5f);
	m_mutiList->setTitleStr("ID",0);
	m_mutiList->setRowWidth(80,0);
	m_mutiList->setTitleStr("Type",1);
	m_mutiList->setRowWidth(100,1);
	m_mutiList->setTitleStr("State",2);
	m_mutiList->setRowWidth(100,2);
	m_mutiList->setTitleStr("Position",3);
	m_mutiList->setRowWidth(120,3);
	m_mutiList->setTitleStr("Size",4);
	m_mutiList->setRowWidth(150,4);
	m_mutiList->setTitleStr("Angle",5);
	m_mutiList->setRowWidth(150,5);

	m_mutiList->setEventProc(ctrlProc,this);

	m_ctrlKeyState = KEY_STATE_UP;
	m_isInited = XTrue;
	return XTrue;
}
XObjectManager::~XObjectManager()
{
	XMem::XDELETE(m_font);
	XMem::XDELETE(m_mousePosFont);
	XMem::XDELETE(m_mutiList);

	//printf("物件管理器已经析构!\n");
	if(!m_isInited) return;
	release();
	m_isInited = XFalse;
}
void XObjectManager::getCurChooseObjectID(int *ID) const
{
	if(!m_isInited ||
		ID == NULL) return;
	int ret = 0;
	for(unsigned int i = 0;i < m_objInfo.size();++ i)
	{
		if(m_objInfo[i].m_objBeSelect) 
		{
			ID[ret] = i;
			++ ret;
		}
	}
}
int XObjectManager::findObjectID(const void * object) const
{
//	if(!m_isInited) return -1;
	for(unsigned int i = 0;i < m_objInfo.size();++ i)
	{
		if(m_objInfo[i].m_pObject == object) return i;
	}
	return -1;
}
void XObjectManager::setEditWidowsPos()					//	2	3
{
	if(!m_isShowUI) return;
	switch(m_editWindowsPos)
	{
	case 0:
		if(XEG.m_windowData.withFrame == 0 && XEG.m_windowData.withCustomTitle)
			m_mutiList->setPosition(XEG.getScenePosX() + XEG.getSceneW() - 289.0f,XEG.getScenePosY() + MIN_FONT_CTRL_SIZE);
		else
			m_mutiList->setPosition(XEG.getScenePosX() + XEG.getSceneW() - 289.0f,XEG.getScenePosY());
		break;
	case 1:
		if(XEG.m_windowData.withFrame == 0 && XEG.m_windowData.withCustomTitle)
			m_mutiList->setPosition(XEG.getScenePosX(), XEG.getScenePosY() + MIN_FONT_CTRL_SIZE);
		else
			m_mutiList->setPosition(XEG.getScenePosX(),XEG.getScenePosY());
		break;
	case 2:m_mutiList->setPosition(XEG.getScenePosX(),XEG.getScenePosY() + XEG.getSceneH() - 257.0f);break;
	case 3:m_mutiList->setPosition(XEG.getScenePosX() + XEG.getSceneW() - 289.0f,XEG.getScenePosY() + XEG.getSceneH() - 257.0f);break;
	}
	m_mousePosFont->setPosition(m_mutiList->getPosition() + XVector2(0.0f,240.0f));
}
#if !WITH_INLINE_FILE
#include "XObjectManager.inl"
#endif
}