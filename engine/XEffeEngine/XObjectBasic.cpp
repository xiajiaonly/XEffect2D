#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XObjectBasic.h"
namespace XE{
std::string objectTypeStr[] = {
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
void XObjectBasic::updateChildPos()
{
	for(unsigned int i = 0;i < m_childList.size();++ i)
	{
		m_childList[i]->setPosition(getPosition() + m_childRelativePos[i] * getScale());
	}
	if(m_parent == NULL) return;	//会造成无线迭代
	for(unsigned int i = 0;i < m_parent->m_childList.size();++ i)
	{//寻找到自己的编号
		if(m_parent->m_childList[i] == this)
		{
			m_parent->m_childRelativePos[i] = (getPosition() - m_parent->getPosition()) / m_parent->getScale();
			break;
		}
	}
}
void XObjectBasic::updateChildScale()
{
	for(unsigned int i = 0;i < m_childList.size();++ i)
	{
		m_childList[i]->setPosition(getPosition() + m_childRelativePos[i] * getScale());
		m_childList[i]->setScale(getScale() * m_childRelativeSize[i]);
	}
	if(m_parent == NULL) return;
	for(unsigned int i = 0;i < m_parent->m_childList.size();++ i)
	{//寻找到自己的编号
		if(m_parent->m_childList[i] == this)
		{
			m_parent->m_childRelativeSize[i] = getScale() / m_parent->getScale();
			break;
		}
	}
}
void XObjectBasic::updateChildAngle()
{
	for(unsigned int i = 0;i < m_childList.size();++ i)
	{
		m_childList[i]->setAngle(getAngle() + m_childRelativeAngle[i]);
	}
	if(m_parent == NULL) return;
	for(unsigned int i = 0;i < m_parent->m_childList.size();++ i)
	{//寻找到自己的编号
		if(m_parent->m_childList[i] == this)
		{
			m_parent->m_childRelativeAngle[i] = getAngle() - m_parent->getAngle();;
			break;
		}
	}
}
void XObjectBasic::updateChildAlpha()
{
	for(unsigned int i = 0;i < m_childList.size();++ i)
	{
		m_childList[i]->setAlpha(getColor().fA);
	}
}
void XObjectBasic::updateChildColor()
{
	for(unsigned int i = 0;i < m_childList.size();++ i)
	{
		m_childList[i]->setColor(getColor().fR,getColor().fG,getColor().fB,getColor().fA);
	}
}
void XObjectBasic::updateChildVisible()
{
	if(getVisible())
	{
		for(unsigned int i = 0;i < m_childList.size();++ i)
		{
			m_childList[i]->setVisible();
		}
	}else
	{
		for(unsigned int i = 0;i < m_childList.size();++ i)
		{
			m_childList[i]->disVisible();
		}
	}
}
void XObjectBasic::pushChild(XObjectBasic * child)
{
	if(child == NULL ||
		child == this ||	//不能为自身
		getIsChild(child)) return;
	m_childList.push_back(child);
	child->m_parent = this;
	m_childRelativePos.push_back((child->getPosition() - getPosition()) / getScale());
	m_childRelativeSize.push_back(child->getScale() / getScale());
	m_childRelativeAngle.push_back(child->getAngle() - getAngle());
}
void XObjectBasic::popChild(XObjectBasic * child)
{
	if(child == NULL) return;
	//XBool flag = XFalse;
	//for(unsigned int i = 0;i < m_childList.size();++ i)
	//{
	//	if(m_childList[i] == child) flag = XTrue;
	//	if(flag && i < (int)(m_childList.size()) - 1)
	//	{
	//		m_childList[i] = m_childList[i + 1];
	//		m_childRelativePos[i] = m_childRelativePos[i + 1];	
	//		m_childRelativeSize[i] = m_childRelativeSize[i + 1];	
	//		m_childRelativeAngle[i] = m_childRelativeAngle[i + 1];	
	//	}
	//}
	//if(flag) 
	//{//弹出最后一个
	//	m_childList.pop_back();
	//	m_childRelativePos.pop_back();	
	//	m_childRelativeSize.pop_back();	
	//	m_childRelativeAngle.pop_back();
	//	child->m_parent = NULL;
	//}
	//对上面的优化，优化之后的代码与之前的代码存在逻辑上的差异，优化之前会遍历整个队列，不论是否找到，这可能是之前的逻辑不严密
	for(unsigned int i = 0;i < m_childList.size();++ i)
	{
		if(m_childList[i] == child)
		{
			m_childList.erase(m_childList.begin() + i);
			m_childRelativePos.erase(m_childRelativePos.begin() + i);
			m_childRelativeSize.erase(m_childRelativeSize.begin() + i);
			m_childRelativeAngle.erase(m_childRelativeAngle.begin() + i);
			child->m_parent = NULL;
			break;	//如果已经保证了个子物体的唯一性，则这里可以立即返回，否则要遍历整个序列才能保证逻辑的严密性
		}
	}
}
void XObjectBasic::clearAllChild()
{
	while(m_childList.size() != 0)
	{
		m_childList[(int)(m_childList.size()) - 1]->m_parent = NULL;
		m_childList.pop_back();
	}
	m_childList.clear();	
	m_childRelativePos.clear();	
	m_childRelativeSize.clear();	
	m_childRelativeAngle.clear();	
}
XBool XObjectBasic::getIsChild(XObjectBasic * child)
{
	if(child == NULL) return XFalse;
	for(unsigned int i = 0;i < m_childList.size();++ i)
	{
		if(m_childList[i] == child ||
			m_childList[i]->getIsChild(child)) return XTrue;
	}
	return XFalse;
}
}