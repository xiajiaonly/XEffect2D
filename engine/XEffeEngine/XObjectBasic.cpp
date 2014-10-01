//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XObjectBasic.h"

string objectTypeStr[] = {
	"OBJ_NULL",			//无效的物件
	"OBJ_SPRITE",			//精灵的物件
	"OBJ_FRAME",			//序列帧的物件
	"OBJ_FRAMEEX",		//序列帧的物件
	"OBJ_NUMBER",			//数字字符串的物件
	"OBJ_FONTUNICODE",	//显示字符的物件
	"OBJ_FONTX",			//显示字符的物件
	"OBJ_NODELINE",		//节点曲线的物件
	"OBJ_CONTROL",		//控件的物件
};
void _XObjectBasic::updateChildPos()
{
	for(int i = 0;i < m_childList.size();++ i)
	{
		m_childList[i]->setPosition(getPosition() + m_childRelativePos[i] * getSize());
	}
	if(m_parent != NULL)	//会造成无线迭代
	{
		for(int i = 0;i < m_parent->m_childList.size();++ i)
		{//寻找到自己的编号
			if(m_parent->m_childList[i] == this)
			{
				m_parent->m_childRelativePos[i] = (getPosition() - m_parent->getPosition()) / m_parent->getSize();
				break;
			}
		}
	}
}
void _XObjectBasic::updateChildSize()
{
	for(int i = 0;i < m_childList.size();++ i)
	{
		m_childList[i]->setPosition(getPosition() + m_childRelativePos[i] * getSize());
		m_childList[i]->setSize(getSize() * m_childRelativeSize[i]);
	}
	if(m_parent != NULL)
	{
		for(int i = 0;i < m_parent->m_childList.size();++ i)
		{//寻找到自己的编号
			if(m_parent->m_childList[i] == this)
			{
				m_parent->m_childRelativeSize[i] = getSize() / m_parent->getSize();
				break;
			}
		}
	}
}
void _XObjectBasic::updateChildAngle()
{
	for(int i = 0;i < m_childList.size();++ i)
	{
		m_childList[i]->setAngle(getAngle() + m_childRelativeAngle[i]);
	}
	if(m_parent != NULL)
	{
		for(int i = 0;i < m_parent->m_childList.size();++ i)
		{//寻找到自己的编号
			if(m_parent->m_childList[i] == this)
			{
				m_parent->m_childRelativeAngle[i] = getAngle() - m_parent->getAngle();;
				break;
			}
		}
	}
}
void _XObjectBasic::updateChildAlpha()
{
	for(int i = 0;i < m_childList.size();++ i)
	{
		m_childList[i]->setAlpha(getColor().fA);
	}
}
void _XObjectBasic::updateChildColor()
{
	for(int i = 0;i < m_childList.size();++ i)
	{
		m_childList[i]->setColor(getColor().fR,getColor().fG,getColor().fB,getColor().fA);
	}
}
void _XObjectBasic::updateChildVisible()
{
	if(getVisible())
	{
		for(int i = 0;i < m_childList.size();++ i)
		{
			m_childList[i]->setVisible();
		}
	}else
	{
		for(int i = 0;i < m_childList.size();++ i)
		{
			m_childList[i]->disVisible();
		}
	}
}
void _XObjectBasic::pushChild(_XObjectBasic * child)
{
	if(child == NULL) return;
	if(child == this) return;	//不能为自身
	if(getIsChild(child)) return;
	m_childList.push_back(child);
	child->m_parent = this;
	_XVector2 pos = (child->getPosition() - getPosition()) / getSize();
	m_childRelativePos.push_back(pos);
	_XVector2 size = child->getSize() / getSize();
	m_childRelativeSize.push_back(size);
	float angle = child->getAngle() - getAngle();
	m_childRelativeAngle.push_back(angle);
}
void _XObjectBasic::popChild(_XObjectBasic * child)
{
	if(child == NULL) return;
	_XBool flag = XFalse;
	for(int i = 0;i < m_childList.size();++ i)
	{
		if(m_childList[i] == child) flag = XTrue;
		if(flag && i < m_childList.size() - 1)
		{
			m_childList[i] = m_childList[i + 1];
			m_childRelativePos[i] = m_childRelativePos[i + 1];	
			m_childRelativeSize[i] = m_childRelativeSize[i + 1];	
			m_childRelativeAngle[i] = m_childRelativeAngle[i + 1];	
		}
	}
	if(flag) 
	{//弹出最后一个
		m_childList.pop_back();
		m_childRelativePos.pop_back();	
		m_childRelativeSize.pop_back();	
		m_childRelativeAngle.pop_back();
		child->m_parent = NULL;
	}
}
void _XObjectBasic::clearAllChild()
{
	while(m_childList.size() != 0)
	{
		m_childList[m_childList.size() - 1]->m_parent = NULL;
		m_childList.pop_back();
	}
	m_childList.clear();	
	m_childRelativePos.clear();	
	m_childRelativeSize.clear();	
	m_childRelativeAngle.clear();	
}
_XBool _XObjectBasic::getIsChild(_XObjectBasic * child)
{
	if(child == NULL) return XFalse;
	for(int i = 0;i < m_childList.size();++ i)
	{
		if(m_childList[i] == child) return XTrue;
		if(m_childList[i]->getIsChild(child)) return XTrue;
	}
	return XFalse;
}