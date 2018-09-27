#include "XStdHead.h"
#include "XPropertyBox.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "XMedia\XSerialPort.h"

namespace XE{
void XPropertyBox::ctrlProc(void*pClass,int id,int eventID)
{
	XPropertyBox &pPar = *(XPropertyBox *)pClass;
	if(id == pPar.m_slider.getControlID())
	{
		if(eventID == XSlider::SLD_MOUSE_MOVE ||
			eventID == XSlider::SLD_VALUE_CHANGE)
		{
			int curIndex = XMath::toInt(pPar.m_slider.getCurValue() * ((int)(pPar.m_propertyLines.size()) - 
				pPar.m_maxLineSum) * 0.01f);
			if(curIndex != pPar.m_curStartLineIndex) 
				pPar.setCurStartLineIndex(curIndex);
		}
		return;
	}
}
void XPropertyBox::updateSliderState()
{
	if(m_propertyLines.size() <= m_maxLineSum && m_maxLineSum >= 0)
	{
		m_curStartLineIndex = 0;
		m_slider.disVisible();
	}else
	{
		m_slider.setVisible();
		if(m_curStartLineIndex > (int)(m_propertyLines.size()) - m_maxLineSum)
			m_curStartLineIndex = (int)(m_propertyLines.size()) - m_maxLineSum;
		m_slider.setCurValue((float)(m_curStartLineIndex) / (float)((int)(m_propertyLines.size()) - m_maxLineSum) * 100.0f,
			true);
	}
	updateRect();
}
XBool XPropertyBox::addAPropertyLine(const char * name, XPropertyLineType type, int menuSum, int showMenuSum)
{
	XPropertyLine tmp;
	tmp.type = type;
	switch (tmp.type)
	{
	case PROPERTY_BLANK:
		m_propertyLines.push_back(tmp);
		updateSliderState();
		setCurStartLineIndex(m_curStartLineIndex);
		return XTrue;
	case PROPERTY_TITLE:	//只显示名字的行
		if (name == NULL) return XFalse;
		tmp.name = name;
		tmp.pNameFont = XMem::createMem<XFontUnicode>();
		if (tmp.pNameFont == NULL) return XFalse;
		tmp.pNameFont->setACopy(m_font);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(tmp.pNameFont);
#endif
		tmp.pNameFont->setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
		tmp.pNameFont->setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
		tmp.pNameFont->setColor(XFColor::black);
		tmp.pNameFont->setString(XString::getCanShowString(tmp.name.c_str(), m_width - 10.0f, 16.0f).c_str());
		tmp.pNameFont->setScale(m_scale);
		if (m_haveTitle)
			tmp.pNameFont->setPosition(m_position +
				XVec2(0.0f, MIN_FONT_CTRL_SIZE * m_scale.y * ((int)(m_propertyLines.size()) + 1)));
		else
			tmp.pNameFont->setPosition(m_position +
				XVec2(0.0f, MIN_FONT_CTRL_SIZE * m_scale.y * (int)(m_propertyLines.size())));
		m_propertyLines.push_back(tmp);
		updateSliderState();
		setCurStartLineIndex(m_curStartLineIndex);
		return XTrue;
	case PROPERTY_EDIT:	//输入框
		if (name == NULL) return XFalse;
		tmp.name = name;
		tmp.pNameFont = XMem::createMem<XFontUnicode>();
		if (tmp.pNameFont == NULL) return XFalse;
		tmp.pNameFont->setACopy(m_font);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(tmp.pNameFont);
#endif		
		tmp.pNameFont->setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
		tmp.pNameFont->setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
		tmp.pNameFont->setColor(XFColor::black);
		tmp.pNameFont->setString(XString::getCanShowString(tmp.name.c_str(), m_nameWidth, 16.0f).c_str());
		tmp.pNameFont->setScale(m_scale);
		if (m_haveTitle)
			tmp.pNameFont->setPosition(m_position +
				XVec2(0.0f, MIN_FONT_CTRL_SIZE * m_scale.y * ((int)(m_propertyLines.size()) + 1)));
		else
			tmp.pNameFont->setPosition(m_position +
				XVec2(0.0f, MIN_FONT_CTRL_SIZE * m_scale.y * (int)(m_propertyLines.size())));
		{
			XEdit *pTmp = XMem::createMem<XEdit>();
			if (pTmp == NULL) return XFalse;
			pTmp->initWithoutSkin(m_width - m_nameWidth, " ");
			XCtrlManager.decreaseAObject(pTmp);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(pTmp);
#endif			
			m_ctrlManager.addACtrl(pTmp);
			pTmp->setScale(m_scale);
			if (m_haveTitle)
				pTmp->setPosition(m_position +
					XVec2(m_nameWidth, MIN_FONT_CTRL_SIZE * m_scale.y * ((int)(m_propertyLines.size()) + 1)));
			else
				pTmp->setPosition(m_position +
					XVec2(m_nameWidth, MIN_FONT_CTRL_SIZE * m_scale.y * (int)(m_propertyLines.size())));
			tmp.pCtrl = pTmp;
		}
		m_propertyLines.push_back(tmp);
		updateSliderState();
		setCurStartLineIndex(m_curStartLineIndex);
		return XTrue;
	case PROPERTY_CHECK:	//选择项
		if (name == NULL) return XFalse;
		tmp.name = name;
		tmp.pNameFont = XMem::createMem<XFontUnicode>();
		if (tmp.pNameFont == NULL) return XFalse;
		tmp.pNameFont->setACopy(m_font);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(tmp.pNameFont);
#endif		
		tmp.pNameFont->setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
		tmp.pNameFont->setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
		tmp.pNameFont->setColor(XFColor::black);
		tmp.pNameFont->setString(XString::getCanShowString(tmp.name.c_str(), m_nameWidth, 16.0f).c_str());
		tmp.pNameFont->setScale(m_scale);
		if (m_haveTitle)
			tmp.pNameFont->setPosition(m_position +
				XVec2(0.0f, MIN_FONT_CTRL_SIZE * m_scale.y * ((int)(m_propertyLines.size()) + 1)));
		else
			tmp.pNameFont->setPosition(m_position +
				XVec2(0.0f, MIN_FONT_CTRL_SIZE * m_scale.y * ((int)(m_propertyLines.size()))));
		{
			XCheck *pTmp = XMem::createMem<XCheck>();
			if (pTmp == NULL) return XFalse;
			pTmp->initWithoutSkin("");
			XCtrlManager.decreaseAObject(pTmp);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(pTmp);
#endif			
			m_ctrlManager.addACtrl(pTmp);
			pTmp->setScale(m_scale);
			if (m_haveTitle)
				pTmp->setPosition(m_position +
					XVec2(m_nameWidth, MIN_FONT_CTRL_SIZE * m_scale.y * ((int)(m_propertyLines.size()) + 1)));
			else
				pTmp->setPosition(m_position +
					XVec2(m_nameWidth, MIN_FONT_CTRL_SIZE * m_scale.y * (int)(m_propertyLines.size())));
			tmp.pCtrl = pTmp;
		}
		m_propertyLines.push_back(tmp);
		updateSliderState();
		setCurStartLineIndex(m_curStartLineIndex);
		return XTrue;
	case PROPERTY_COMBO:	//选项
		if (name == NULL) return XFalse;
		tmp.name = name;
		tmp.pNameFont = XMem::createMem<XFontUnicode>();
		if (tmp.pNameFont == NULL) return XFalse;
		tmp.pNameFont->setACopy(m_font);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(tmp.pNameFont);
#endif	
		tmp.pNameFont->setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
		tmp.pNameFont->setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
		tmp.pNameFont->setColor(XFColor::black);
		tmp.pNameFont->setString(XString::getCanShowString(tmp.name.c_str(), m_nameWidth, 16.0f).c_str());
		tmp.pNameFont->setScale(m_scale);
		if (m_haveTitle)
			tmp.pNameFont->setPosition(m_position +
				XVec2(0.0f, MIN_FONT_CTRL_SIZE * m_scale.y * ((int)(m_propertyLines.size()) + 1)));
		else
			tmp.pNameFont->setPosition(m_position +
				XVec2(0.0f, MIN_FONT_CTRL_SIZE * m_scale.y * (int)(m_propertyLines.size())));
		{
			XCombo *pTmp = XMem::createMem<XCombo>();
			if (pTmp == NULL) return XFalse;
			pTmp->initWithoutSkin(m_width - m_nameWidth - MIN_FONT_CTRL_SIZE, menuSum, showMenuSum);
			XCtrlManager.decreaseAObject(pTmp);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(pTmp);
#endif			
			m_ctrlManager.addACtrl(pTmp);
			pTmp->setScale(m_scale);
			if (m_haveTitle)
				pTmp->setPosition(m_position +
					XVec2(m_nameWidth, MIN_FONT_CTRL_SIZE * m_scale.y * ((int)(m_propertyLines.size()) + 1)));
			else
				pTmp->setPosition(m_position +
					XVec2(m_nameWidth, MIN_FONT_CTRL_SIZE * m_scale.y * (int)(m_propertyLines.size())));
			tmp.pCtrl = pTmp;
		}
		m_propertyLines.push_back(tmp);
		updateSliderState();
		setCurStartLineIndex(m_curStartLineIndex);
		return XTrue;
	}
	return XFalse;
}
XBool XPropertyBox::setPropertyName(unsigned int index, const char * name)
{
	if (index >= m_propertyLines.size()) return XFalse;
	if (name == NULL) return XFalse;
	switch (m_propertyLines[index].type)
	{
	case PROPERTY_BLANK:return XFalse;	//空白行，空行，什么都没有
	case PROPERTY_TITLE:	//只显示名字的行
		m_propertyLines[index].name = name;
		if (m_propertyLines[index].pNameFont != NULL)
			m_propertyLines[index].pNameFont->setString(XString::getCanShowString(m_propertyLines[index].name.c_str(), m_width - 10.0f, 16.0f).c_str());
		return XTrue;
	case PROPERTY_EDIT:		//输入框
	case PROPERTY_CHECK:	//选择项
	case PROPERTY_COMBO:	//选项
		m_propertyLines[index].name = name;
		if (m_propertyLines[index].pNameFont != NULL)
			m_propertyLines[index].pNameFont->setString(XString::getCanShowString(m_propertyLines[index].name.c_str(), m_nameWidth, 16.0f).c_str());
		return XTrue;
	}
	return XFalse;
}
void XPropertyBox::draw()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	int h = 0;
	if(m_haveTitle) h = 1;
	XFColor tmpColor = XCCS::normalColor * m_color;
	XFColor tmpDColor = XCCS::lightSpecialColor * m_color;
	float w = m_width * m_scale.x;
	XRender::drawFillRectA(m_position,XVec2(w,
		MIN_FONT_CTRL_SIZE * (m_maxLineSum + h) * m_scale.y),tmpColor,true);
	//在滑动条下面添加以个背景
//	XRender::drawFillRectA(m_position + XVec2(w,h * MIN_FONT_CTRL_SIZE * m_scale.y),
//		XVec2(MIN_FONT_CTRL_SIZE * m_scale.x,MIN_FONT_CTRL_SIZE * m_maxLineSum * m_scale.y),tmpColor.r,tmpColor.g,tmpColor.b,tmpColor.a,true);
	if(m_haveTitle)
	{
		if(m_slider.getVisible())
			XRender::drawFillRectA(m_position,XVec2(m_width + MIN_FONT_CTRL_SIZE,MIN_FONT_CTRL_SIZE) * m_scale,
				tmpColor,true);
		else 
			XRender::drawFillRectA(m_position,XVec2(w,MIN_FONT_CTRL_SIZE * m_scale.y),
				tmpColor,true);
		m_titleTxt.draw();
	}
	//下面依次显示所有的属性项
	int size = m_propertyLines.size();
	if(size - m_curStartLineIndex > m_maxLineSum) size = m_maxLineSum + m_curStartLineIndex;
	XFColor tmpLColor = tmpColor;
	for(int i = m_curStartLineIndex;i < size;++ i)
	{
		if(m_propertyLines[i].getIsFocus()) tmpColor = tmpDColor;
		else tmpColor = tmpLColor;
		switch(m_propertyLines[i].type)
		{
		case PROPERTY_BLANK:	//空白行，空行，什么都没有
			XRender::drawFillRectA(m_position + XVec2(0.0f,h * MIN_FONT_CTRL_SIZE * m_scale.y),
				XVec2(w,MIN_FONT_CTRL_SIZE * m_scale.y),tmpColor,true);
			break;
		case PROPERTY_TITLE:	//只显示名字的行
			XRender::drawFillRectA(m_position + XVec2(0.0f,h * MIN_FONT_CTRL_SIZE * m_scale.y),
				XVec2(w,MIN_FONT_CTRL_SIZE * m_scale.y),tmpColor,true);
			m_propertyLines[i].draw();
			break;
		case PROPERTY_EDIT:		//输入框
			XRender::drawFillRectA(m_position + XVec2(0.0f,h * MIN_FONT_CTRL_SIZE * m_scale.y),
				XVec2(m_nameWidth,MIN_FONT_CTRL_SIZE) * m_scale,tmpColor,true);
			m_propertyLines[i].draw();
			break;
		case PROPERTY_CHECK:	//选择项
			XRender::drawFillRectA(m_position + XVec2(0.0f,h * MIN_FONT_CTRL_SIZE * m_scale.y),
				XVec2(w,MIN_FONT_CTRL_SIZE * m_scale.y),tmpColor,true);
			XRender::drawFillRectA(m_position + XVec2(0.0f,h * MIN_FONT_CTRL_SIZE * m_scale.y),
				XVec2(m_nameWidth,MIN_FONT_CTRL_SIZE) * m_scale,tmpColor,true);
			m_propertyLines[i].draw();
			break;
		case PROPERTY_COMBO:	//选项
			XRender::drawFillRectA(m_position + XVec2(0.0f,h * MIN_FONT_CTRL_SIZE * m_scale.y),
				XVec2(m_nameWidth,MIN_FONT_CTRL_SIZE) * m_scale,tmpColor,true);
			m_propertyLines[i].draw();
			break;
		}
		++ h;
	}
	m_slider.draw();
	m_ctrlManager.draw();
}
XBool XPropertyBox::initWithoutSkin(int width,int maxLineSum)
{
	if(m_isInited) return XFalse;
	if(width < 128 || maxLineSum < 1) return XFalse;
	m_width = width;
	m_maxLineSum = maxLineSum;
	m_nameWidth = (width >> 1);
	m_position.reset();
	m_scale.set(1.0f);

	m_font.setACopy(getDefaultFont());
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_font);
#endif
	m_titleTxt.init();
	m_titleTxt.setPosition(m_position.x + 5.0f,m_position.y);
	m_titleTxt.setColor(XFColor::black);
	XCtrlManager.decreaseAObject(&m_titleTxt);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_titleTxt);
#endif

	m_slider.initWithoutSkin(XVec2(MIN_FONT_CTRL_SIZE,m_maxLineSum * MIN_FONT_CTRL_SIZE),100.0f,0.0f,SLIDER_TYPE_VERTICAL);
	m_slider.setWithAction(XFalse);
	m_slider.setScale(m_scale);
	m_slider.setEventProc(ctrlProc,this);
	m_slider.disVisible();
	XCtrlManager.decreaseAObject(&m_slider);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_slider);
#endif
	setCurStartLineIndex(0);

	updateRect();

	m_isInited = m_isVisible = m_isEnable = m_isActive = XTrue;
	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	return XTrue;
}
void XPropertyBox::updateRect()
{
	if(m_haveTitle)
	{
		if(m_slider.getVisible())
			m_curMouseRect.set(m_position,
				m_position + XVec2(m_width + MIN_FONT_CTRL_SIZE,
				MIN_FONT_CTRL_SIZE * (m_maxLineSum + 1)) * m_scale);
		else
			m_curMouseRect.set(m_position, m_position + XVec2(m_width,
				MIN_FONT_CTRL_SIZE * (m_maxLineSum + 1)) * m_scale);
	}else 
	{
		if(m_slider.getVisible())
			m_curMouseRect.set(m_position, m_position + XVec2(m_width + MIN_FONT_CTRL_SIZE,
				MIN_FONT_CTRL_SIZE * m_maxLineSum) * m_scale);
		else
			m_curMouseRect.set(m_position, m_position + XVec2(m_width,
				MIN_FONT_CTRL_SIZE * m_maxLineSum) * m_scale);
	}
}
void XPropertyBox::setScale(const XVec2& s)
{
	m_scale = s;
	m_titleTxt.setPosition(m_position + XVec2(5.0f * m_scale.x, 0.0f));
	m_titleTxt.setScale(m_scale);
	if(m_haveTitle)
		m_slider.setPosition(m_position + XVec2(m_width, MIN_FONT_CTRL_SIZE) * m_scale);
	else 
		m_slider.setPosition(m_position + XVec2(m_width * m_scale.x, 0.0f));
	m_slider.setScale(m_scale);
	//下面更新所有属性栏的位置
	setCurStartLineIndex(m_curStartLineIndex);
	updateRect();
	for(unsigned int i = 0;i < m_propertyLines.size();++ i)
	{
		switch(m_propertyLines[i].type)
		{
		case PROPERTY_BLANK:	//空白行，空行，什么都没有
			break;
		case PROPERTY_TITLE:	//只显示名字的行
			if(m_propertyLines[i].pNameFont != NULL) 
				m_propertyLines[i].pNameFont->setScale(m_scale);
			break;
		case PROPERTY_EDIT:		//输入框
			if(m_propertyLines[i].pNameFont != NULL) 
				m_propertyLines[i].pNameFont->setScale(m_scale);
			if(m_propertyLines[i].pCtrl != NULL)
				((XEdit *)m_propertyLines[i].pCtrl)->setScale(m_scale);
			break;
		case PROPERTY_CHECK:	//选择项
			if(m_propertyLines[i].pNameFont != NULL) 
				m_propertyLines[i].pNameFont->setScale(m_scale);
			if(m_propertyLines[i].pCtrl != NULL)
				((XCheck *)m_propertyLines[i].pCtrl)->setScale(m_scale);
			break;
		case PROPERTY_COMBO:	//选项
			if(m_propertyLines[i].pNameFont != NULL) 
				m_propertyLines[i].pNameFont->setScale(m_scale);
			if(m_propertyLines[i].pCtrl != NULL)
				((XCombo *)m_propertyLines[i].pCtrl)->setScale(m_scale);
			break;
		}
	}
}
void XPropertyBox::setCurStartLineIndex(unsigned int index)
{//设置当前起始行的编号
	if(index >= m_propertyLines.size()) return;	//非法的数值
	m_curStartLineIndex = index;
	//下面需要更新所有属性栏的位置
	for(unsigned int i = 0;i < m_propertyLines.size();++ i)
	{
		if((i < m_curStartLineIndex) ||
			i >= m_curStartLineIndex + m_maxLineSum)
		{//隐藏掉这些东西
			switch(m_propertyLines[i].type)
			{
			case PROPERTY_BLANK:	//空白行，空行，什么都没有
				break;
			case PROPERTY_TITLE:	//只显示名字的行
				break;
			case PROPERTY_EDIT:		//输入框
				if(m_propertyLines[i].pCtrl != NULL)
					((XEdit *)m_propertyLines[i].pCtrl)->disVisible();
				break;
			case PROPERTY_CHECK:	//选择项
				if(m_propertyLines[i].pCtrl != NULL)
					((XCheck *)m_propertyLines[i].pCtrl)->disVisible();
				break;
			case PROPERTY_COMBO:	//选项
				if(m_propertyLines[i].pCtrl != NULL)
					((XCombo *)m_propertyLines[i].pCtrl)->disVisible();
				break;
			}
		}else
		{//下面这些需要显示，并更新位置
			switch(m_propertyLines[i].type)
			{
			case PROPERTY_BLANK:	//空白行，空行，什么都没有
				break;
			case PROPERTY_TITLE:	//只显示名字的行
				if(m_haveTitle) 
					m_propertyLines[i].pNameFont->setPosition(m_position + 
					XVec2(0.0f,MIN_FONT_CTRL_SIZE * m_scale.y * (i - m_curStartLineIndex + 1)));
				else 
					m_propertyLines[i].pNameFont->setPosition(m_position + 
					XVec2(0.0f,MIN_FONT_CTRL_SIZE * m_scale.y * (i - m_curStartLineIndex)));
				break;
			case PROPERTY_EDIT:		//输入框
				if(m_haveTitle) 
				{
					m_propertyLines[i].pNameFont->setPosition(m_position + 
					XVec2(0.0f,MIN_FONT_CTRL_SIZE * m_scale.y * (i - m_curStartLineIndex + 1)));
					if(m_propertyLines[i].pCtrl != NULL)
						((XEdit *)m_propertyLines[i].pCtrl)->setPosition(m_position + XVec2(m_nameWidth,
							MIN_FONT_CTRL_SIZE * (i - m_curStartLineIndex + 1)) * m_scale);
				}else
				{
					m_propertyLines[i].pNameFont->setPosition(m_position + 
					XVec2(0.0f,MIN_FONT_CTRL_SIZE * m_scale.y * (i - m_curStartLineIndex)));
					if(m_propertyLines[i].pCtrl != NULL)
						((XEdit *)m_propertyLines[i].pCtrl)->setPosition(m_position + XVec2(m_nameWidth,
							MIN_FONT_CTRL_SIZE * (i - m_curStartLineIndex)) * m_scale);
				}
				((XEdit *)m_propertyLines[i].pCtrl)->setVisible();
				break;
			case PROPERTY_CHECK:	//选择项
				if(m_haveTitle) 
				{
					m_propertyLines[i].pNameFont->setPosition(m_position + 
					XVec2(0.0f,MIN_FONT_CTRL_SIZE * m_scale.y * (i - m_curStartLineIndex + 1)));
					if(m_propertyLines[i].pCtrl != NULL)
						((XCheck *)m_propertyLines[i].pCtrl)->setPosition(m_position + XVec2(m_nameWidth,
							MIN_FONT_CTRL_SIZE * (i - m_curStartLineIndex + 1)) * m_scale);
				}else
				{
					m_propertyLines[i].pNameFont->setPosition(m_position + 
					XVec2(0.0f,MIN_FONT_CTRL_SIZE * m_scale.y * (i - m_curStartLineIndex)));
					if(m_propertyLines[i].pCtrl != NULL)
						((XCheck *)m_propertyLines[i].pCtrl)->setPosition(m_position + XVec2(m_nameWidth,
							MIN_FONT_CTRL_SIZE * (i - m_curStartLineIndex)) * m_scale);
				}
				((XCheck *)m_propertyLines[i].pCtrl)->setVisible();
				break;
			case PROPERTY_COMBO:	//选项
				if(m_haveTitle) 
				{
					m_propertyLines[i].pNameFont->setPosition(m_position + 
					XVec2(0.0f,MIN_FONT_CTRL_SIZE * m_scale.y * (i - m_curStartLineIndex + 1)));
					if(m_propertyLines[i].pCtrl != NULL)
						((XCombo *)m_propertyLines[i].pCtrl)->setPosition(m_position + XVec2(m_nameWidth,
							MIN_FONT_CTRL_SIZE * (i - m_curStartLineIndex + 1)) * m_scale);
				}else
				{
					m_propertyLines[i].pNameFont->setPosition(m_position + 
					XVec2(0.0f,MIN_FONT_CTRL_SIZE * m_scale.y * (i - m_curStartLineIndex)));
					if(m_propertyLines[i].pCtrl != NULL)
						((XCombo *)m_propertyLines[i].pCtrl)->setPosition(m_position + XVec2(m_nameWidth,
							MIN_FONT_CTRL_SIZE * (i - m_curStartLineIndex)) * m_scale);
				}
				((XCombo *)m_propertyLines[i].pCtrl)->setVisible();
				break;
			}
		}
	}
}
void XPropertyBox::release()
{
	if (!m_isInited) return;
	XCtrlManager.decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	m_ctrlManager.clearAllObject();	//这里需要清空所有的物件
	//下面释放资源
	for (unsigned int i = 0; i < m_propertyLines.size(); ++i)
	{
		switch (m_propertyLines[i].type)
		{
		case PROPERTY_BLANK:	//空白行，空行，什么都没有
			break;
		case PROPERTY_TITLE:	//只显示名字的行
			XMem::XDELETE(m_propertyLines[i].pNameFont);
			break;
		case PROPERTY_EDIT:		//输入框
			XMem::XDELETE(m_propertyLines[i].pNameFont);
			XMem::XDELETE(m_propertyLines[i].pCtrl);
			break;
		case PROPERTY_CHECK:	//选择项
			XMem::XDELETE(m_propertyLines[i].pNameFont);
			XMem::XDELETE(m_propertyLines[i].pCtrl);
			break;
		case PROPERTY_COMBO:	//选项
			XMem::XDELETE(m_propertyLines[i].pNameFont);
			XMem::XDELETE(m_propertyLines[i].pCtrl);
			break;
		}
	}
	m_propertyLines.clear();
	m_isInited = false;
}
void XPropertyBox::setNameWidth(int width)
{
	if (width <= 0 || width >= m_width) return;
	m_nameWidth = width;
	//下面要更新所有的数据(尚未实现)
	for (unsigned int i = 0; i < m_propertyLines.size(); ++i)
	{
		switch (m_propertyLines[i].type)
		{
		case PROPERTY_BLANK:	//空白行，空行，什么都没有
			break;
		case PROPERTY_TITLE:	//只显示名字的行
			break;
		case PROPERTY_EDIT:		//输入框
			if (m_propertyLines[i].pNameFont != NULL)
				m_propertyLines[i].pNameFont->setString(XString::getCanShowString(m_propertyLines[i].name.c_str(),
					m_nameWidth, 16.0f).c_str());
			if (m_propertyLines[i].pCtrl != NULL)
				((XEdit *)m_propertyLines[i].pCtrl)->setInputLen(m_width - m_nameWidth);
			break;
		case PROPERTY_CHECK:	//选择项
			if (m_propertyLines[i].pNameFont != NULL)
				m_propertyLines[i].pNameFont->setString(XString::getCanShowString(m_propertyLines[i].name.c_str(),
					m_nameWidth, 16.0f).c_str());
			if (m_propertyLines[i].pCtrl != NULL)
				//((XCheck *)m_propertyLines[i].pCtrl)->set(m_width - m_nameWidth);
				break;
		case PROPERTY_COMBO:	//选项
			if (m_propertyLines[i].pNameFont != NULL)
				m_propertyLines[i].pNameFont->setString(XString::getCanShowString(m_propertyLines[i].name.c_str(),
					m_nameWidth, 16.0f).c_str());
			//这个功能尚未实现,如果要实现这个功能，需要修改很多基础的控件设计
			if (m_propertyLines[i].pCtrl != NULL)
				((XCombo *)m_propertyLines[i].pCtrl)->setInputLen(m_width - m_nameWidth - DEFAULT_COMBO_MN_HEIGHT);
			break;
		}
	}
	setCurStartLineIndex(m_curStartLineIndex);
}
void XPropertyBox::disable()
{
	m_isEnable = XFalse;
	m_isBeChoose = XFalse;
	m_slider.disable();
	for(unsigned int i = 0;i < m_propertyLines.size();++ i)
	{
		m_propertyLines[i].disable();
	}
}
void XPropertyBox::enable()
{
	m_isEnable = XTrue;
	m_isBeChoose = XFalse;
	m_slider.enable();
	for(unsigned int i = 0;i < m_propertyLines.size();++ i)
	{
		m_propertyLines[i].enable();
	}
}
void setAsSerialPortInfo(XPropertyBox &pb, const char* title)
{
	pb.setNameWidth(150.0f);
	pb.setTitle(title);
	pb.addAPropertyLine("串口号:", PROPERTY_EDIT);
	pb.setPropertyStr(0, "1");
	pb.setPropertyCtrlPrama(0, XEdit::TYPE_UNSIGNED_INT);
	pb.addAPropertyLine("比特率:", PROPERTY_EDIT);
	pb.setPropertyStr(1, "9600");
	pb.setPropertyCtrlPrama(1, XEdit::TYPE_UNSIGNED_INT);
	pb.addAPropertyLine("校验方式:", PROPERTY_COMBO, 5, 3);
	pb.setPropertyMenuStr(2, 0, "NO");
	pb.setPropertyMenuStr(2, 1, "ODD");
	pb.setPropertyMenuStr(2, 2, "EVEN");
	pb.setPropertyMenuStr(2, 3, "MARK");
	pb.setPropertyMenuStr(2, 4, "SPACE");
	pb.setPropertyIndex(2, 0);
	pb.addAPropertyLine("事件触发:", PROPERTY_CHECK);
	pb.setPropertyState(3, XFalse);
	pb.addAPropertyLine("异步方式:", PROPERTY_CHECK);
	pb.setPropertyState(4, XFalse);
	pb.addAPropertyLine("数据位:", PROPERTY_COMBO, 5, 3);
	pb.setPropertyMenuStr(5, 0, "4");
	pb.setPropertyMenuStr(5, 1, "5");
	pb.setPropertyMenuStr(5, 2, "6");
	pb.setPropertyMenuStr(5, 3, "7");
	pb.setPropertyMenuStr(5, 4, "8");
	pb.setPropertyIndex(5, 4);
	pb.addAPropertyLine("停止位:", PROPERTY_COMBO, 3, 3);
	pb.setPropertyMenuStr(6, 0, "1");
	pb.setPropertyMenuStr(6, 1, "1.5");
	pb.setPropertyMenuStr(6, 2, "2");
	pb.setPropertyIndex(6, 0);
	pb.addAPropertyLine("DRT:", PROPERTY_COMBO, 3, 3);
	pb.setPropertyMenuStr(7, 0, "Disable");
	pb.setPropertyMenuStr(7, 1, "Enable");
	pb.setPropertyMenuStr(7, 2, "HandShake");
	pb.setPropertyIndex(7, 1);
	pb.addAPropertyLine("RTS:", PROPERTY_COMBO, 4, 3);
	pb.setPropertyMenuStr(8, 0, "Disable");
	pb.setPropertyMenuStr(8, 1, "Enable");
	pb.setPropertyMenuStr(8, 2, "HandShake");
	pb.setPropertyMenuStr(8, 3, "Toggle");
	pb.setPropertyIndex(8, 1);
	pb.addAPropertyLine("等待接收:", PROPERTY_EDIT);
	pb.setPropertyStr(9, "5");
	pb.setPropertyCtrlPrama(9, XEdit::TYPE_UNSIGNED_INT);
}
bool getSerialPortInfo(const XPropertyBox& pb, XSerialPortInfo& tmpInfo)
{
	std::string tmpPort = pb.getPropertyStr(S_PPB_PORT);
	std::string tmpBitRate = pb.getPropertyStr(S_PPB_BITRATE);
	std::string tmpWaitRecv = pb.getPropertyStr(S_PPB_WAIT_RECV);
	if (XString::getIsNumber(tmpPort.c_str())) tmpInfo.nPort = atoi(tmpPort.c_str());
	else tmpInfo.nPort = XString::getIsHexNumber(tmpPort.c_str());
	if (tmpInfo.nPort < 0) return false;
	if (XString::getIsNumber(tmpBitRate.c_str())) tmpInfo.nBaud = atoi(tmpBitRate.c_str());
	else tmpInfo.nBaud = XString::getIsHexNumber(tmpBitRate.c_str());
	if (tmpInfo.nBaud < 0) return false;
	if (XString::getIsNumber(tmpWaitRecv.c_str())) tmpInfo.waitingDataTime = atoi(tmpWaitRecv.c_str());
	else tmpInfo.waitingDataTime = XString::getIsHexNumber(tmpWaitRecv.c_str());
	if (tmpInfo.waitingDataTime < 0) return false;

	tmpInfo.mode = SP_MODE_AUTO;
	tmpInfo.nParity = pb.getPropertyIndex(S_PPB_MODE);
	tmpInfo.withEventMode = pb.getPropertyState(S_PPB_WITHEVENT);
	tmpInfo.withOverLaped = pb.getPropertyState(S_PPB_WITHOLP);
	tmpInfo.byteSize = pb.getPropertyIndex(S_PPB_DATABIT) + 4;
	tmpInfo.stopBits = pb.getPropertyIndex(S_PPB_STOPBIT);
	tmpInfo.dtr = pb.getPropertyIndex(S_PPB_DTR);
	tmpInfo.rts = pb.getPropertyIndex(S_PPB_RTS);
	return true;
}
void setSerialPortInfo(XPropertyBox& pb, const XSerialPortInfo& info)
{
	pb.setPropertyStr(S_PPB_PORT, XString::toString(info.nPort).c_str());
	pb.setPropertyStr(S_PPB_BITRATE, XString::toString(info.nBaud).c_str());
	pb.setPropertyIndex(S_PPB_MODE, info.nParity);
	pb.setPropertyState(S_PPB_WITHEVENT, info.withEventMode);
	pb.setPropertyState(S_PPB_WITHOLP, info.withOverLaped);
	pb.setPropertyIndex(S_PPB_DATABIT, info.byteSize - 4);
	pb.setPropertyIndex(S_PPB_STOPBIT, info.stopBits);
	pb.setPropertyIndex(S_PPB_DTR, info.dtr);
	pb.setPropertyIndex(S_PPB_RTS, info.rts);
	pb.setPropertyStr(S_PPB_WAIT_RECV, XString::toString(info.waitingDataTime).c_str());
}
#if !WITH_INLINE_FILE
#include "XPropertyBox.inl"
#endif
}