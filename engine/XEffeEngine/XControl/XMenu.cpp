#include "XStdHead.h"
#include "XMenu.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
namespace XE{
XMenuItem *XMenuGroup::getItemByHotKey(char k)
{//这里的k必须要符合要求，由于这里会迭代调用，所以这里不判断k是否符合要求
	XMenuItem * ret;
	for(unsigned int i = 0;i < m_items.size();++ i)
	{
		ret = m_items[i]->getItemByHotKey(k);
		if(ret != NULL) return ret;
	}
	return NULL;
}
XMenuItem *XMenuGroup::getItemByHotChooseKey(char k)
{//这里的k必须要符合要求，由于这里会迭代调用，所以这里不判断k是否符合要求
	for(unsigned int i = 0;i < m_items.size();++ i)
	{
		if(m_items[i]->m_hotChooseKey == k) return m_items[i];
	}
	return NULL;
}
void XMenuGroup::setAllCallBackFun(void (*callbackFun)(void *,const std::string &),void *p)
{
	for(unsigned int i = 0;i < m_items.size();++ i)
	{
		m_items[i]->setAllCallBackFun(callbackFun,p);
	}
}
XBool XMenuGroup::setChooseItem(const XMenuItem *it)
{
	if(it == NULL) return XFalse;
	for(unsigned int i = 0;i < m_items.size();++ i)
	{
		if(m_items[i] != it) continue;
		//还原原有的选择
		if(m_chooseIndex >= 0 && m_chooseIndex < m_items.size())
			m_items[m_chooseIndex]->m_isMouseOn = XFalse;
		//设置新的选择
		m_chooseIndex = i;
		m_items[i]->m_isMouseOn = XTrue;
		return XTrue;
	}
	return XFalse;
}
void XMenuGroup::addIndex()//增加编号
{
	if(m_chooseIndex >= 0 && m_chooseIndex < m_items.size())
		m_items[m_chooseIndex]->m_isMouseOn = XFalse;
	if(m_chooseIndex < 0)
	{
		initIndex(XTrue);
		return;
	}
	int next = m_chooseIndex + 1;
	while(XTrue)
	{
		if(next == m_chooseIndex)
		{//没有可以选择的则为之原有的选择
			next = -1;
			break;
		}
		if(next >= 0 && next >= m_items.size())
		{
			next = 0;
			continue;
		}
		if(m_items[next]->m_type == MENU_ITEM_TYPE_DLT)
		{
			++ next;
			continue;
		}
		if(!m_items[next]->m_isEnable)
		{
			++ next;
			continue;
		}
		break;
	}
	if(next >= 0) m_chooseIndex = next;
	if(m_chooseIndex >= 0 && m_chooseIndex < m_items.size())
		m_items[m_chooseIndex]->m_isMouseOn = XTrue;
}	
void XMenuGroup::desIndex()	//减少编号
{
	if(m_chooseIndex >= 0 && m_chooseIndex < m_items.size())
		m_items[m_chooseIndex]->m_isMouseOn = XFalse;
	if(m_chooseIndex < 0)
	{
		initIndex(XFalse);
		return;
	}
	int next = m_chooseIndex - 1;
	while(XTrue)
	{
		if(next == m_chooseIndex)
		{//没有可以选择的则为之原有的选择
			next = -1;
			continue;
		}
		if(next < 0)
		{
			next = (int)(m_items.size()) - 1;
			continue;
		}
		if(m_items[next]->m_type == MENU_ITEM_TYPE_DLT)
		{
			-- next;
			continue;
		}
		if(!m_items[next]->m_isEnable)
		{
			-- next;
			continue;
		}
		break;
	}
	if(next >= 0) m_chooseIndex = next;
	if(m_chooseIndex >= 0 && m_chooseIndex < m_items.size())
		m_items[m_chooseIndex]->m_isMouseOn = XTrue;
}
void XMenuGroup::initIndex(XBool orderFlag)
{
	int index = -1;
	if(orderFlag)
	{//正序查找最近的可以选择的
		for(unsigned int i = 0;i < m_items.size();++ i)
		{
			if(m_items[i]->m_type != MENU_ITEM_TYPE_DLT &&
				m_items[i]->m_isEnable)
			{
				index = i;
				break;
			}
		}
	}else
	{//反序查找可以选择额
		for(int i = (int)(m_items.size()) - 1;i >= 0;-- i)
		{
			if(m_items[i]->m_type != MENU_ITEM_TYPE_DLT &&
				m_items[i]->m_isEnable)
			{
				index = i;
				break;
			}
		}
	}
	m_chooseIndex = index;
	for(unsigned int i = 0;i < m_items.size();++ i)
	{//设置所有的选项为非选择状态
		m_items[i]->m_isMouseOn = XFalse;
	}
	if(m_chooseIndex >= 0 && m_chooseIndex < m_items.size())
		m_items[m_chooseIndex]->m_isMouseOn = XTrue;
}	//初始化编号
void XMenuGroup::updatePos()
{
	if(m_items.size() <= 0) return;
	XVector2 pos;	//组第一个菜单项所在的位置
	if(m_parent->m_owner == NULL) pos = m_parent->m_position;
	else
	{
		if(m_parent->m_owner->m_type == MENU_TYPE_VERTICAL)
		{//垂直的
			pos = m_parent->m_position;
			pos.x += m_parent->m_size.x * m_parent->m_scaleSize.x;
		}else
		{//水平的
			pos = m_parent->m_position;
			pos.y += m_parent->m_size.y * m_parent->m_scaleSize.y;
		}
	}
	m_items[0]->setPosition(pos.x,pos.y);
	//下面遍历所有的控件并设置其位置
	if(m_type == MENU_TYPE_VERTICAL)
	{
		XMenuItem *tmpItem = NULL;
		for(unsigned int i = 1; i < m_items.size();++ i)
		{
			tmpItem = m_items[i - 1];
			m_items[i]->setPosition(tmpItem->m_position.x,tmpItem->m_position.y + tmpItem->m_size.y * tmpItem->m_scaleSize.y);
		}
	}else
	{
		XMenuItem *tmpItem = NULL;
		for(unsigned int i = 1; i < m_items.size();++ i)
		{
			tmpItem = m_items[i - 1];
			m_items[i]->setPosition(tmpItem->m_position.x + tmpItem->m_size.x * tmpItem->m_scaleSize.x,tmpItem->m_position.y);
		}
	}
}
void XMenuGroup::updateColor()
{
	if(m_items.size() <= 0) return;
	for(unsigned int i = 0; i < m_items.size();++ i)
	{
		m_items[i]->setColor(m_parent->m_color);
	}
}
void XMenuGroup::updateSize()
{//尚未实现,发生缩放变化，其位置也会发生变化
	if(m_items.size() <= 0) return;
	for(unsigned int i = 0; i < m_items.size();++ i)
	{
		m_items[i]->setScale(m_parent->m_scaleSize.x,m_parent->m_scaleSize.y);
	}
}
XBool XMenuItem::init(const std::string &name,const XFontUnicode &font,float/*fontsize*/,XMenuGroup *owner,XMenuItemType type)
{
	if(owner == NULL) return XFalse;
	if(owner != NULL && type != MENU_ITEM_TYPE_DLT && owner->getItemByNameInGroup(name) != NULL) return XFalse;//检查名字的唯一性
	m_originalName = name;
	m_owner = owner;
	if(owner == NULL || owner->m_parent == NULL) m_levelIndex = 0;
	else m_levelIndex = owner->m_parent->m_levelIndex + 1;
	m_type = type;
	m_position = owner->m_insertPos;
	if(owner->m_parent != NULL)
		m_scaleSize = owner->m_parent->m_scaleSize;
	else 
		m_scaleSize.set(1.0f,1.0f);

	m_isEnable = XTrue;

	if(m_type == MENU_ITEM_TYPE_DLT)
	{//如果是分隔符的话这里处理
		m_isVisible = XFalse;
		m_size.set(m_owner->m_width,5.0f);
		owner->m_insertPos.y += 5.0f;
		return XTrue;
	}

	if(!m_font.setACopy(font)) return XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_font);
#endif
	m_font.setColor(0.0f,0.0f,0.0f,1.0f);
	updateStr();

	m_size.set(0,32.0f);
	if(m_owner->m_width <= 0)
	{
		if(m_hotChooseKey >= 0) m_size.x = m_font.getMaxPixelWidth() + 2.0f;	//需要计算宽度
		else m_size.x = m_font.getMaxPixelWidth() + 50.0f;	//需要计算宽度
	}else
	{//这里需要计算显示的字符串的长度
		m_size.x = m_owner->m_width;
	}
	if(m_levelIndex < 2 && owner->m_type == MENU_TYPE_HORIZONTA) m_font.setPosition(m_position + XVector2(m_size.x * 0.5f,16));
	else
	{
		m_font.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
		m_font.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
		if(m_type == MENU_ITEM_TYPE_CHK)
		{
			if(m_isChecked) m_showName = "X " + m_showName;
			else m_showName = "* " + m_showName;
			m_font.setPosition(m_position + XVector2(2.0f,0.0f));
		}else
		{
			m_font.setPosition(m_position + XVector2(36.0f,0.0f));
		}
	}
	if(owner->m_parent != NULL && owner->m_parent->m_isOpen) m_isVisible = XTrue;
	else m_isVisible = XFalse;

	if(m_withIcon &&  m_type == MENU_ITEM_TYPE_BTN)
		m_spriteIcon.setPosition(m_position);

	switch(owner->m_type)
	{
	case MENU_TYPE_VERTICAL:	//垂直的
		owner->m_insertPos.y += 32.0f;
		break;
	case MENU_TYPE_HORIZONTA:	//水平的
		owner->m_insertPos.x += m_size.x;
		break;
	}
	return XTrue;
}
void XMenuItem::setString(const std::string &name)
{
	if(m_levelIndex == 0 ||	//根层不需要处理
		m_type == MENU_ITEM_TYPE_DLT) return;	//分隔符不能设置字体
	m_originalName = name;

	updateStr();

	if(m_owner->m_width <= 0)
	{
		if(m_hotChooseKey >= 0) m_size.x = m_font.getMaxPixelWidth() + 2.0f;	//需要计算宽度
		else m_size.x = m_font.getMaxPixelWidth() + 50.0f;
	}else
	{//这里需要计算显示的字符串的长度
		m_size.x = m_owner->m_width;	//需要计算宽度
	}
	if(m_levelIndex < 2 && m_owner->m_type == MENU_TYPE_HORIZONTA) m_font.setPosition(m_position + XVector2(m_size.x * 0.5f,16));
}
void XMenuItem::beTriggered()	//菜单项被触发时调用这个函数
{
	if(!m_isEnable || !m_isVisible) return;	//这种状态下不能触发事件
	if(m_callbackFun != NULL) m_callbackFun(m_pClass,getFullStr());
	if(m_type == MENU_ITEM_TYPE_CHK)
	{
		m_isChecked = !m_isChecked;
		//下面改变文字显示
		if(m_isChecked) m_showName[0] = 'X';
		else m_showName[0] = '*';
		m_font.setString(m_showName.c_str());
	}
}
std::string XMenuItem::getFullStr() const	//获取当前菜单项的全路径
{//实时的获取当前菜单项的全路径，这里可以考虑优化为空间换时间的方式
	std::string ret = "";
	const XMenuItem * it = this;
	while(XTrue)
	{
		if(it == NULL) return ret;
		ret = it->m_originalName + ";" + ret;
		if(it->m_owner == NULL) return ret;
		it = it->m_owner->m_parent;
	}
	return ret;
}
void XMenuItem::setPosition(float x,float y)
{
	m_position.set(x,y);	//设置自身的位置
	if(m_withIcon &&  m_type == MENU_ITEM_TYPE_BTN)
		m_spriteIcon.setPosition(m_position);
	if(m_type != MENU_ITEM_TYPE_DLT)
	{
		if(m_levelIndex < 2 && (m_owner == NULL || m_owner->m_type == MENU_TYPE_HORIZONTA)) 
			m_font.setPosition(m_position + XVector2(m_size.x * 0.5f * m_scaleSize.x,16 * m_scaleSize.y));
		else
		{
			if(m_type == MENU_ITEM_TYPE_CHK)
				m_font.setPosition(m_position + XVector2(2.0f,0.0f) * m_scaleSize);
			else
				m_font.setPosition(m_position + XVector2(36.0f,0.0f) * m_scaleSize);
		}
	}
	//所有子菜单的位置都需要修改
	if(m_child != NULL) m_child->updatePos();
}
void XMenuItem::setColor(const XFColor &c)
{
	m_color = c;
	if(m_withIcon &&  m_type == MENU_ITEM_TYPE_BTN)
		m_spriteIcon.setColor(c);
	if(m_type != MENU_ITEM_TYPE_DLT)
	{
		m_font.setAlpha(c.fA);
	}
	//所有子菜单的位置都需要修改
	if(m_child != NULL) m_child->updateColor();
}
void XMenuItem::setScale(float x,float y)
{
	m_scaleSize.set(x,y);
	m_font.setScale(m_scaleSize);
	if(m_withIcon &&  m_type == MENU_ITEM_TYPE_BTN)
	{
		float size = 32.0f * m_scaleSize.x / m_spriteIcon.getW();
		float tmp = 32.0f * m_scaleSize.y / m_spriteIcon.getH();
		m_spriteIcon.setScale((size < tmp)?size:tmp);
	}
	//尚未完成
	if(m_child != NULL) m_child->updateSize();
}
XBool XMenuItem::mouseProc(float x,float y,XMouseState mouseState)
{
	if(m_type == MENU_ITEM_TYPE_DLT) return XFalse;

	if(m_isEnable && m_isVisible)//判断是否悬浮
	{
		m_isMouseOn = XRect(m_position.x,m_position.y,m_position.x + m_size.x * m_scaleSize.x,m_position.y + m_size.y * m_scaleSize.y).isInRect(x,y);
		if(m_child != NULL)
		{
			if(m_isOpen && m_child->mouseProc(x,y,mouseState)) return XTrue;
		}else
		{//判断菜单项事件是否触发
			if((mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_LEFT_BUTTON_DCLICK)
				&& m_isMouseOn)
			{//点击事件
				beTriggered();
				return XTrue;
			}
		}
	}
	return XFalse;
}
XBool XMenuItem::setIcon(const char *filename,XResourcePosition res)
{
	if(m_type != MENU_ITEM_TYPE_BTN ||
		filename == NULL) return XFalse;
	if(m_withIcon) m_spriteIcon.release();
	if(!m_spriteIcon.init(filename,res,POINT_LEFT_TOP)) return XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteIcon);
#endif
	float size = 32.0f * m_scaleSize.x / m_spriteIcon.getW();
	float tmp = 32.0f * m_scaleSize.y / m_spriteIcon.getH();
	m_spriteIcon.setScale((size < tmp)?size:tmp);
	m_spriteIcon.setPosition(m_position);
	m_withIcon = XTrue;
	return XTrue;
}
void XMenuItem::draw()
{
	if(!m_isVisible) return;
	if(m_type == MENU_ITEM_TYPE_DLT)
	{
		XRender::drawFillBox(m_position.x,m_position.y,m_size.x * m_scaleSize.x,m_size.y * m_scaleSize.y,
			XCCS::normalColor * m_color);
		XRender::drawLine(m_position.x,m_position.y + m_size.y * 0.5f * m_scaleSize.y,
			m_position.x + m_size.x * m_scaleSize.x,m_position.y + m_size.y * 0.5f * m_scaleSize.y,2.0f,
			XCCS::blackOnColor * m_color);
		XRender::drawLine(m_position.x,m_position.y + m_size.y * 0.5f * m_scaleSize.y,
			m_position.x + m_size.x * m_scaleSize.x,m_position.y + m_size.y * 0.5f * m_scaleSize.y,1.0f,
			XCCS::mouseColor * m_color);
	}else
	{
		if(!m_isEnable) XRender::drawFillBox(m_position.x,m_position.y,m_size.x * m_scaleSize.x,m_size.y * m_scaleSize.y,
			XCCS::mouseColor * m_color); else
		if(m_isMouseOn) XRender::drawFillBox(m_position.x,m_position.y,m_size.x * m_scaleSize.x,m_size.y * m_scaleSize.y,
			XCCS::onColor * m_color); else
		XRender::drawFillBox(m_position.x,m_position.y,m_size.x * m_scaleSize.x,m_size.y * m_scaleSize.y,
			XCCS::normalColor * m_color);

		m_font.draw();
		if(m_child != NULL && m_isOpen) m_child->draw();
		if(m_withIcon &&  m_type == MENU_ITEM_TYPE_BTN)
			m_spriteIcon.draw();
	}
}
void XMenuItem::updateStr()
{
	if(m_type == MENU_ITEM_TYPE_DLT) return;
	//这里需要设置快捷键显示出来
	if(m_owner->m_width <= 0)
	{
		m_showName = m_originalName;
		if(m_hotChooseKey >= 0)
		{
			if(!m_isHotChooseActive) m_showName = m_showName + "(" + m_hotChooseKey + ")";		//不激活时显示大写
			else m_showName = m_showName + "(" + (char)(m_hotChooseKey - ('A' - 'a')) + ")";	//激活时显示小写
		}
		m_font.setString(m_showName.c_str());
	}else
	{//这里需要计算显示的字符串的长度
		int stringShowWidth = 0;
		if(m_levelIndex >= 2) stringShowWidth = (floor((m_owner->m_width - 36.0f) / m_font.getTextSize().x) * 2) - 1;
		else stringShowWidth = (floor(m_owner->m_width / m_font.getTextSize().x) * 2) - 1;
		if(m_originalName.length() <= stringShowWidth)
		{
			m_showName = m_originalName;
		}else
		if(stringShowWidth >= 2)
		{
			if(m_originalName.length() >= m_maxMenuItemLength - 1) 
			{
				printf("XMenu item's name length error!\n");
				return;	//长度非法
			}
			char p[m_maxMenuItemLength];
			strcpy(p,m_originalName.c_str());
			if(XString::isAtUnicodeEnd(m_originalName.c_str(),stringShowWidth - 2))
			{

				p[stringShowWidth] = '\0';
				p[stringShowWidth - 1] = '.';
				p[stringShowWidth - 2] = '.';
			}else
			{
				p[stringShowWidth] = '\0';
				p[stringShowWidth - 1] = '.';
				p[stringShowWidth - 2] = '.';
				p[stringShowWidth - 3] = '.';
			} 
			m_showName = p;
		}else
		{
			m_showName = "";
		}
		if(m_hotChooseKey >= 0)
		{
			if(!m_isHotChooseActive) m_showName = m_showName + "(" + m_hotChooseKey + ")";
			else m_showName = m_showName + "(" + (char)(m_hotChooseKey - ('A' - 'a')) + ")";
		}
		if(m_levelIndex >= 2 && m_child != NULL)
		{//这里需要填补足够的空格
			while((int)(m_showName.length()) < stringShowWidth - 1)
			{
				//if((int)(m_showName.length()) >= stringShowWidth - 1) break;
				m_showName += " ";
			}
			m_showName += ">";
		}
		if(m_type == MENU_ITEM_TYPE_CHK)
		{
			if(m_isChecked) m_showName = "X " + m_showName;
			else m_showName = "* " + m_showName;
		}
		m_font.setString(m_showName.c_str());
	}
}
XMenuItem * XMenu::getItemFromGroup(char *p,XMenuGroup *g)
{
	if(g == NULL || p == NULL) return NULL;
	int offset = XString::getCharPosition(p,';');
	if(offset == 0 || offset >= 1024) return NULL;	//非法的数据
	if(offset == -1)
	{//完整的字符串
		for(unsigned int i = 0;i < g->m_items.size();++ i)
		{
			if(strcmp(p,g->m_items[i]->m_originalName.c_str()) == 0)
			{
				return g->m_items[i];
			}
		}
		return NULL;	//找不到匹配的
	}else
	{
		char tmp[XMenuItem::m_maxMenuItemLength];
		memcpy(tmp,p,offset);
		tmp[offset] = '\0';
		int len = strlen(p);
		if(len - 1 == offset)
		{//以';'分隔符结束
			for(unsigned int i = 0;i < g->m_items.size();++ i)
			{
				if(strcmp(tmp,g->m_items[i]->m_originalName.c_str()) == 0)
				{
					return g->m_items[i];
				}
			}
			return NULL;	//找不到匹配的
		}else
		{
			for(unsigned int i = 0;i < g->m_items.size();++ i)
			{
				if(strcmp(tmp,g->m_items[i]->m_originalName.c_str()) == 0)
				{//继续在下一个层次中寻找
					return getItemFromGroup(p + offset + 1,g->m_items[i]->m_child);
				}
			}
			return NULL;	//找不到匹配的
		}
	}
	//return NULL;
}
XBool XMenu::setMenuItemStr(const std::string &pos,const std::string &name)
{//设置菜单项的名字
	if(name.length() >= XMenuItem::m_maxMenuItemLength - 1) return XFalse;	//字符串太长
	XMenuItem * tmpX = getItemByName(pos);
	if(tmpX == NULL || tmpX->m_type == MENU_ITEM_TYPE_DLT) return XFalse;
	if(tmpX->m_owner != NULL && tmpX->m_owner->getItemByNameInGroup(name) != NULL) return XFalse;	//如果重名则不能修改，名字具有唯一性

	tmpX->setString(name);
	if(tmpX->m_owner != NULL && tmpX->m_owner->m_type == MENU_TYPE_HORIZONTA)
	{//如果是水平的，这需要重新排位置
		int size = tmpX->m_owner->m_items.size();
		for(int i = 1;i < size;++ i)
		{
			tmpX->m_owner->m_items[i]->setPosition(tmpX->m_owner->m_items[i - 1]->m_position.x + tmpX->m_owner->m_items[i - 1]->m_size.x,
				tmpX->m_owner->m_items[i - 1]->m_position.y);
		}
		tmpX->m_owner->m_insertPos.x = tmpX->m_owner->m_items[size - 1]->m_position.x + tmpX->m_owner->m_items[size - 1]->m_size.x;
	}
	return XTrue;
}
void XMenu::ctrlProc(void *pClass,const std::string &pos)
{
	XMenu &pPar = *(XMenu *)pClass;
	pPar.m_curChooseMenuStr = pos;
	if(pPar.m_eventProc != NULL) pPar.m_eventProc(pPar.m_pClass,pPar.m_objectID,MNU_CHOOSE);
	else XCtrlManager.eventProc(pPar.m_objectID,MNU_CHOOSE);
}
XBool XMenu::addAItem(const std::string &pos,const std::string &name,int width,XMenuItemType type)
{//向菜单中增加一个内容
	if(name.length() >= XMenuItem::m_maxMenuItemLength - 1) return XFalse;	//字符串太长
	if(XString::getCharPosition(name.c_str(),';') >= 0) return XFalse;	//名字中不能有分隔符
	if(width >= 0 && width <= 256) width = 256;
	if(pos == "root" || pos == "root;")
	{//根项上添加
		if(type != MENU_ITEM_TYPE_BTN) return XFalse;	//根上不能加其他类型
		if(m_rootItem.m_child == NULL)
		{//子组尚未建立
			m_rootItem.m_isOpen = XTrue;
			m_rootItem.m_child = XMem::createMem<XMenuGroup>();
			if(m_rootItem.m_child == NULL) return XFalse;
			//下面初始化相关信息
			switch(m_type)
			{
			case MENU_TYPE_VERTICAL:
				m_rootItem.m_child->m_type = MENU_TYPE_VERTICAL;
				m_rootItem.m_child->m_width = 256;
				break;
			case MENU_TYPE_HORIZONTA:
				m_rootItem.m_child->m_type = MENU_TYPE_HORIZONTA;
				m_rootItem.m_child->m_width = -1;
				break;
			}

			m_rootItem.m_child->m_parent = &m_rootItem;
			m_rootItem.m_child->m_insertPos = m_position;
		}else
		{//需要检查名字的唯一性
			if(m_rootItem.m_child->getItemByNameInGroup(name) != NULL) return XFalse;
		}
		//建立一个配置项并增加进去
		XMenuItem * tmp = XMem::createMem<XMenuItem>();
		if(tmp == NULL) return XFalse;
		tmp->init(name,m_font,m_fontSize,m_rootItem.m_child);
		tmp->setCallbackFun(ctrlProc,this);
		m_rootItem.m_child->m_items.push_back(tmp);
	}else
	{//非根项上添加或者非法操作
		XMenuItem * tmpX = getItemByName(pos);
		if(tmpX == NULL ||
			tmpX->m_type != MENU_ITEM_TYPE_BTN ||	//不能再其他类型的控件项上添加新的控件
			type != MENU_ITEM_TYPE_BTN && tmpX->m_levelIndex < 1) return XFalse;
		if(tmpX->m_child == NULL)
		{//建立子物体
			if(width < 256) width = 256;	//最小宽度必须是256，否则容不小需要的空间
			tmpX->m_child = XMem::createMem<XMenuGroup>();
			if(tmpX->m_child == NULL) return XFalse;
			//下面初始化相关信息
			tmpX->m_child->m_parent = tmpX;
			tmpX->m_child->m_insertPos = tmpX->m_position;
			switch(tmpX->m_owner->m_type)
			{
			case MENU_TYPE_HORIZONTA:
				tmpX->m_child->m_insertPos.y += tmpX->m_size.y;
				break;
			case MENU_TYPE_VERTICAL:
				tmpX->m_child->m_insertPos.x += tmpX->m_size.x;
				break;
			}
			tmpX->m_child->m_type = MENU_TYPE_VERTICAL;
			tmpX->m_child->m_width = width;
		}else
		{
			if(type != MENU_ITEM_TYPE_DLT && tmpX->m_child->getItemByNameInGroup(name) != NULL) return XFalse;
		}
		//建立一个配置项并增加进去
		XMenuItem * tmp = XMem::createMem<XMenuItem>();
		if(tmp == NULL) return XFalse;
		tmp->init(name,m_font,m_fontSize,tmpX->m_child,type);
		tmp->setCallbackFun(ctrlProc,this);
		tmpX->m_child->m_items.push_back(tmp);
	}
	m_needPostProcess = XTrue;
	return XTrue;
}
XMenuItem * XMenu::getItemByName(const std::string &name)
{//尚未完成
	if(name == "root" || name == "root;") return &m_rootItem;
	int offset = XString::getCharPosition(name.c_str(),';');
	if(offset <= 0 || offset >= 1024) return NULL;	//非法的数据
	char tmp[XMenuItem::m_maxMenuItemLength];
	memcpy(tmp,name.c_str(),offset);
	tmp[offset] = '\0';
	if(strcmp(tmp,"root") != 0) return NULL;
	char *p = XMem::createArrayMem<char>(name.length() + 1);
	if(p == NULL) return NULL;
	strcpy(p,name.c_str());
	XMenuItem * ret = getItemFromGroup(p + offset + 1,m_rootItem.m_child);
	XMem::XDELETE_ARRAY(p);
	return ret;
}
XBool XMenu::mouseProc(float x,float y,XMouseState mouseState)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	if(m_rootItem.mouseProc(x,y,mouseState))
	{//触发菜单项
		if(m_hotChooseKeyIsActive)
		{
			m_hotChooseKeyIsActive = XFalse;
			m_rootItem.setHotChooseActive(m_hotChooseKeyIsActive);
		}
		closeAll();
		m_oldChooseItem = NULL;
		m_isClicked = XFalse;
		return XTrue;
	}
	if(mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_LEFT_BUTTON_DCLICK)
	{
		if(m_hotChooseKeyIsActive)
		{//只要发生鼠标点击事件就取消选择激活状态
			m_hotChooseKeyIsActive = XFalse;
			m_rootItem.setHotChooseActive(m_hotChooseKeyIsActive);
			closeAll();
			m_oldChooseItem = NULL;
			m_isClicked = XFalse;
		}
		if(m_rootItem.isInRect(x,y))
		{//点击事件发生在菜单上
			XMenuItem * tmp = m_rootItem.getItem(x,y);
			if(!m_isClicked)
			{
				m_isClicked = XTrue;
				if(tmp != NULL) tmp->setOpen();
			}else
			{
				if(tmp != NULL && tmp->m_levelIndex < 2)
				{
					closeAll();
					m_oldChooseItem = NULL;
					m_isClicked = XFalse;
					return XTrue;
				}
			}
			return XTrue;
		}else
		{
			if(m_isClicked)
			{
				closeAll();
				m_oldChooseItem = NULL;
				m_isClicked = XFalse;
				return XTrue;
			}
		}
	}
	if(m_isClicked && mouseState == MOUSE_MOVE)
	{//打开相关的菜单(尚未完成)
		//获取当前鼠标所在位置的菜单项
		XMenuItem * tmp = m_rootItem.getItem(x,y);
		if(tmp != NULL && tmp != m_oldChooseItem)
		{
			tmp->setOpen();	//如果可以则打开当前选项
			//判断目标是否发生改变
			if(m_oldChooseItem != NULL)
			{
				//判断关闭条件
				while(XTrue)
				{
					if(m_oldChooseItem == NULL || getIsPath(m_oldChooseItem,tmp)) break;	//如果在路径上则退出
					//否则则关闭，并进一步判断
					m_oldChooseItem->disOpen();
					if(m_oldChooseItem->m_owner == NULL) break;
					m_oldChooseItem = m_oldChooseItem->m_owner->m_parent;
				}
			}
			m_oldChooseItem = tmp;
		}
	}
	return XFalse;
}
XBool XMenu::keyboardProc(int keyOrder,XKeyState keyState)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	if(keyState == KEY_STATE_DOWN)
	{
		switch(keyOrder)
		{
		case XKEY_LALT:
		case XKEY_RALT:
			m_hotChooseKeyIsActive = !m_hotChooseKeyIsActive;
			m_rootItem.setHotChooseActive(m_hotChooseKeyIsActive);
			if(!m_hotChooseKeyIsActive)//关闭所有的菜单项
			{
				closeAll();
				m_oldChooseItem = NULL;
				m_isClicked = XFalse;
			}else if(m_isClicked)
			{
				closeAll();
				m_oldChooseItem = NULL;
				m_isClicked = XFalse;
			}
			break;
		}
		if(m_hotChooseKeyIsActive)
		{//下面是热键的处理
			switch(keyOrder)
			{
			case XKEY_LEFT:
				if(m_oldChooseItem == NULL)
				{//选择的是根
					if(m_rootItem.m_child != NULL)
						m_rootItem.m_child->desIndex();
				}else
				{
					if(m_oldChooseItem->m_child != NULL)
					{
						switch(m_oldChooseItem->m_child->m_type)
						{
						case MENU_TYPE_VERTICAL://左选或者是打开操作
							if(m_oldChooseItem->m_owner != NULL)
							{
								if(m_oldChooseItem->m_levelIndex < 2 && 
									m_oldChooseItem->m_owner->m_type == MENU_TYPE_HORIZONTA)
								{
									XMenuItem *it = m_rootItem.m_child->getChooseItem();
									closeAll();
									m_rootItem.m_child->setChooseItem(it);
									m_rootItem.m_child->desIndex();
									//并且打开这个项
									it = m_rootItem.m_child->getChooseItem();
									it->setOpen();
									it->m_child->initIndex(XTrue);
									m_oldChooseItem = it;
								}else
								{
									XMenuItem * it = m_oldChooseItem->m_owner->getChooseItem();
									if(it != NULL)
									{
										it->disOpen();
										m_oldChooseItem = it->m_owner->m_parent;
									}
								}
							}
							break;
						case MENU_TYPE_HORIZONTA:
							m_oldChooseItem->m_child->desIndex();
							break;
						}
					}
				}
				break;
			case XKEY_RIGHT:
				if(m_oldChooseItem == NULL)
				{//选择的是根
					if(m_rootItem.m_child != NULL)
					{
						m_rootItem.m_child->addIndex();
					}
				}else
				{
					if(m_oldChooseItem->m_child != NULL)
					{
						switch(m_oldChooseItem->m_child->m_type)
						{
						case MENU_TYPE_VERTICAL://左选或者是关闭操作
							{
								XMenuItem * it = m_oldChooseItem->m_child->getChooseItem();
								if(it != NULL)
								{
									if(it->m_child != NULL)
									{//打开操作
										it->setOpen();
										it->m_child->initIndex(XTrue);
										m_oldChooseItem = it;
									}else
									{//回溯
										if(m_oldChooseItem->m_owner != NULL && 
											m_oldChooseItem->m_owner->m_type == MENU_TYPE_HORIZONTA)
										{
											it = m_rootItem.m_child->getChooseItem();
											closeAll();
											m_rootItem.m_child->setChooseItem(it);
											m_rootItem.m_child->addIndex();
											//并且打开这个项
											it = m_rootItem.m_child->getChooseItem();
											it->setOpen();
											it->m_child->initIndex(XTrue);
											m_oldChooseItem = it;
										}
									}
								}
							}
							break;
						case MENU_TYPE_HORIZONTA:
							m_oldChooseItem->m_child->addIndex();
							break;
						}
					}
				}
				break;
			case XKEY_UP:
				if(m_oldChooseItem == NULL)
				{//选择的是根
					if(m_rootItem.m_child != NULL)
					{
						XMenuItem * it = m_rootItem.m_child->getChooseItem();
						if(it != NULL && it->m_child != NULL)
						{//打开操作
							it->setOpen();
							it->m_child->initIndex(XTrue);
							m_oldChooseItem = it;
						}
					}
				}else
				{
					if(m_oldChooseItem->m_child != NULL)
					{
						switch(m_oldChooseItem->m_child->m_type)
						{
						case MENU_TYPE_VERTICAL:
							m_oldChooseItem->m_child->desIndex();
							break;
						case MENU_TYPE_HORIZONTA://左选或者是关闭操作
							break;
						}
					}
				}
				break;
			case XKEY_DOWN:
				if(m_oldChooseItem == NULL)
				{//选择的是根
					if(m_rootItem.m_child != NULL)
					{
						XMenuItem * it = m_rootItem.m_child->getChooseItem();
						if(it != NULL && it->m_child != NULL)
						{//打开操作
							it->setOpen();
							it->m_child->initIndex(XTrue);
							m_oldChooseItem = it;
						}
					}
				}else
				{
					if(m_oldChooseItem->m_child != NULL)
					{
						switch(m_oldChooseItem->m_child->m_type)
						{
						case MENU_TYPE_VERTICAL:
							m_oldChooseItem->m_child->addIndex();
							break;
						case MENU_TYPE_HORIZONTA://左选或者是关闭操作
							break;
						}
					}
				}
				break;
			case XKEY_RETURN:	//确认动作
				if(m_oldChooseItem == NULL)
				{//选择的是根
					XMenuItem * it = m_rootItem.m_child->getChooseItem();
					if(it != NULL && it->m_child != NULL)
					{//打开操作
						it->setOpen();
						it->m_child->initIndex(XTrue);
						m_oldChooseItem = it;
					}
				}else
				{
					XMenuItem * it = m_oldChooseItem->m_child->getChooseItem();
					if(it != NULL)
					{
						if(it->m_child != NULL)
						{//打开操作
							it->setOpen();
							it->m_child->initIndex(XTrue);
							m_oldChooseItem = it;
						}else
						{//确定操作
							//这里调用回调函数
							it->beTriggered();
							//收拢菜单
							closeAll();
							m_oldChooseItem = NULL;
							m_isClicked = XFalse;
							//取消菜单的键盘激活
							m_hotChooseKeyIsActive = XFalse;
							m_rootItem.setHotChooseActive(m_hotChooseKeyIsActive);
						}
					}
				}
				break;
			default:
				{
					char k = XEE::keyValueToChar((XKeyValue)keyOrder);
					if(k < 'A' || k > 'Z') return XFalse;
					if(m_oldChooseItem == NULL)
					{//选择的是根
						XMenuItem *it = m_rootItem.getItemByHotChooseKey(k);
						if(it != NULL)
						{
							m_rootItem.m_child->setChooseItem(it);
							it->setOpen();
							it->m_child->initIndex();
							m_oldChooseItem = it;
						}
					}else
					{
						XMenuItem *it = m_oldChooseItem->getItemByHotChooseKey(k);
						if(it != NULL)
						{
							if(it->m_child == NULL)
							{//使得事件生效
								it->beTriggered();	//设置触发事件
								//收拢菜单
								closeAll();
								m_oldChooseItem = NULL;
								m_isClicked = XFalse;
								//取消菜单的键盘激活
								m_hotChooseKeyIsActive = XFalse;
								m_rootItem.setHotChooseActive(m_hotChooseKeyIsActive);
							}else
							{
								m_oldChooseItem->m_owner->setChooseItem(it);
								it->setOpen();
								m_oldChooseItem = it;
							}
						}
					}
				}
				break;
			}
		}
	}
	return XFalse;
}
XBool XMenu::getIsPath(XMenuItem *s,XMenuItem *d)
{
	while(XTrue)
	{
		if(s == d) return XTrue;
		if(d == NULL) return XFalse;
		if(d->m_owner == NULL) return XFalse;
		d = d->m_owner->m_parent;
	}
	return XFalse;
}
void XMenu::closeAll()	//折叠所有的菜单项
{
	if(m_rootItem.m_child == NULL) return;
	for(unsigned int i = 0;i < m_rootItem.m_child->m_items.size();++ i)
	{
		m_rootItem.m_child->m_items[i]->disOpen();
		m_rootItem.m_child->resetChoose();
	}
}
//XBool XMenu::setAllCBFunction(void (*callbackFun)(void *,const std::string &),void *p)
//{//尚未实现
//	m_rootItem.setAllCallBackFun(callbackFun,p);
//	return XTrue;
//}
XBool XMenu::initWithoutSkin(const XFontUnicode &font,float captionSize,XMenuType type)
{
	if(m_isInited) return XFalse;
	if(captionSize <= 0.0f) return XFalse;
	if(!m_font.setACopy(font)) return XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_font);
#endif

	m_fontSize = captionSize;
	m_type = type;

	m_position.set(0.0f,0.0f);
	m_scale.set(1.0f,1.0f);
	m_rootItem.m_originalName = "root";
	m_rootItem.m_position = m_position;

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;
	m_isInited = XTrue;

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	return XTrue;
}
void XMenu::release()
{
	if(!m_isInited) return;
	XCtrlManager.decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif

	//下面释放相关资源
	XMem::XDELETE(m_rootItem.m_child);
	m_isInited = XFalse;
}
#if !WITH_INLINE_FILE
#include "XMenu.inl"
#endif
}