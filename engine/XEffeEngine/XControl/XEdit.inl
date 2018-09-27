INLINE void XEditSkin::releaseTex()
{
	XMem::XDELETE(editNormal);
	XMem::XDELETE(editDisable);
	XMem::XDELETE(editSelect);
	XMem::XDELETE(editInsert);
	XMem::XDELETE(editUpon);
}
INLINE void XEditSkin::release()
{
	if(!m_isInited) return;
	releaseTex();
	m_isInited = XFalse;
}
INLINE void XEdit::upDateInsertShowPosition()	//根据实际情况更新光标显示的位置
{
	setImmPos();
	if(m_withoutTex) return;
	m_spriteInsert.setPosition(m_position.x + m_textPosition.x * m_scale.x 
		+ (m_curInsertPoint - m_curShowStart) * m_curTextWidth,
		m_position.y + m_textPosition.y * m_scale.y - m_curTextHeight * 0.5f);
}
INLINE XBool XEdit::initEx(const XVec2& position,	//上面接口的简化版本
	const XEditSkin &tex,			
	const char *str,					
	const XFontUnicode& font,			
	float strSize,	
	XMouseRightButtonMenu * mouseMenu)
{
	return init(position,tex.m_mouseRect,tex,str,font,strSize,mouseMenu);
}
INLINE XBool XEdit::initPlus(const char * path,			//控件的贴图
	const char *str,					//控件的初始化字符串
	const XFontUnicode& font,			//控件的字体
	float strSize,	//控件的字体信息
	XMouseRightButtonMenu * mouseMenu,//控件的右键菜单
	XResPos resPos)
{
	if(m_isInited || path == NULL) return XFalse;
	m_resInfo = XResManager.loadResource(path,RESOURCE_TYPEXEDIT_TEX,resPos);
	if(m_resInfo == NULL) return XFalse;
	return initEx(XVec2::zero,*(XEditSkin *)m_resInfo->m_pointer,str,font,strSize,mouseMenu);
}
INLINE void XEdit::drawUp()				//描绘小菜单
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	//显示鼠标右键菜单
	if(m_mouseRightButtonMenu != NULL)
	{
		m_mouseRightButtonMenu->draw();
		m_mouseRightButtonMenu->drawUp();
	}
	if(!m_insertActionMD.getIsEnd())
	{
		float curData = m_insertActionMD.getCurData();
		float x = m_position.x + m_textPosition.x * m_scale.x + (m_curInsertPoint - m_curShowStart) * m_curTextWidth;
		float y = m_position.y + m_textPosition.y * m_scale.y - m_curTextHeight * 0.5f * curData;
		float r = XMath::lineSlerp<float>(1.0f,0.25f * m_color.r,m_insertActionMD.getCurTimer());
		float g = XMath::lineSlerp<float>(1.0f,0.25f * m_color.g,m_insertActionMD.getCurTimer());
		float b = XMath::lineSlerp<float>(1.0f,0.25f * m_color.b,m_insertActionMD.getCurTimer());
		XRender::drawLine(XVec2(x,y),XVec2(x,y + m_curTextHeight * curData),1.0f * curData,
			XFColor(r,g,b,m_color.a * m_insertActionMD.getCurTimer()));
	}
	if(m_withoutTex && !m_lightMD.getIsEnd())
	{
		XRender::drawRect(m_lightRect,1.0f * m_lightMD.getCurData() * 2.0f,XFColor(1.0f,(1.0f - m_lightMD.getCurTimer()) * 0.5f));
	}
	m_comment.draw();
}
INLINE XBool XEdit::canGetFocus(const XVec2& p)	//用于判断当前物件是否可以获得焦点
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	return isInRect(p);
}
INLINE XBool XEdit::canLostFocus(const XVec2&)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XTrue;		//如果无效则直接退出
	if(m_selectMouseDown) return false;
	if(m_mouseRightButtonMenu != NULL && m_mouseRightButtonMenu->getVisible()) return XFalse;
	//if(m_isBeChoose) return XFalse;
	return XTrue;
}
INLINE void XEdit::setColor(const XFColor& c)
{
	m_color = c;
	m_spriteBackGround.setColor(m_color);
	m_spriteSelect.setColor(m_color);
	m_spriteInsert.setColor(m_color);
	//m_caption.setColor(m_textColor * m_color);
	if(m_withPromptStr && strlen(m_curString) <= 0)
	{//如果有提示文字
		m_caption.setColor(XFColor(0.65f,1.0f) * m_color);
	}else
	{//如果没有提示文字
		m_caption.setColor(m_textColor * m_color);
	}
	if(!isStringPassable())
		m_caption.setColor(XFColor::red * m_color);

	if(m_mouseRightButtonMenu != NULL)
		m_mouseRightButtonMenu->setColor(m_color);
	updateChildColor();
}	//设置按钮的颜色
INLINE void XEdit::setAlpha(float a) 
{
	m_color.setA(a);
	m_spriteBackGround.setColor(m_color);
	m_spriteSelect.setColor(m_color);
	m_spriteInsert.setColor(m_color);
//	m_caption.setColor(m_textColor * m_color);
	if(m_withPromptStr && strlen(m_curString) <= 0)
	{//如果有提示文字
		m_caption.setColor(XFColor(0.65f,1.0f) * m_color);
	}else
	{//如果没有提示文字
		m_caption.setColor(m_textColor * m_color);
	}
	if(!isStringPassable())
		m_caption.setColor(XFColor::red * m_color);
	if(m_mouseRightButtonMenu != NULL)
		m_mouseRightButtonMenu->setAlpha(a);
	updateChildAlpha();
}
INLINE XBool XEdit::isInRect(const XVec2& p)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(p);
}
INLINE XVec2 XEdit::getBox(int order)			//获取四个顶点的坐标，目前先不考虑旋转和缩放
{
	if(!m_isInited) return XVec2::zero;
	switch(order)
	{
	case 0:return m_curMouseRect.getLT();
	case 1:return m_curMouseRect.getRT();
	case 2:return m_curMouseRect.getRB();
	case 3:return m_curMouseRect.getLB();
	}
	return XVec2::zero;
}
INLINE void XEdit::deleteSelectStr()
{
	if(getSelectEnd() == m_curStringLength)
	{//片选到结束
		m_curStringLength -= getSelectLength();
		m_curString[getSelectHead()] = '\0';
		changeInsertPoint(getSelectHead() - m_curInsertPoint);
	}else
	{
		strcpy_s(m_curString + getSelectHead(),m_maxInputStringLength - getSelectHead(),m_curString + getSelectEnd());
		m_curStringLength -= getSelectLength();
		changeInsertPoint(getSelectHead() - m_curInsertPoint);
	}
}
INLINE void XEdit::setPosition(const XVec2& p)
{
	if(!m_isInited) return;	//如果没有初始化直接退出
	m_position = p;
	m_curMouseRect.set(m_position + m_mouseRect.getLT() * m_scale,
		m_position + m_mouseRect.getRB() * m_scale);
	m_caption.setPosition(m_position + m_textPosition * m_scale);
	if(!m_withoutTex) m_spriteBackGround.setPosition(m_position);
	upDateInsertShowPosition();
	upDataSelectShow();
	updateChildPos();
}
INLINE void XEdit::disable()					//使无效
{
	m_isEnable = XFalse;
	if(m_withAction && m_isBeChoose)
	{
		m_insertActionMD.set(10.0f,1.0f,0.002f,MD_MODE_SQRT2_MULT);
	}
	m_isBeChoose = XFalse;
	if(m_mouseRightButtonMenu != NULL) m_mouseRightButtonMenu->disVisible(); //取消右键菜单的显示
}
INLINE void XEdit::enable()					//使能
{
	m_isEnable = XTrue;
}
INLINE  char *XEdit::getString() const				//返回输入的字符串
{
	return m_curString;
}
INLINE int XEdit::getSelectLength() const				//或得选区的长度
{
	if(m_selectEnd < m_selectStart) return m_selectStart - m_selectEnd;
	else return m_selectEnd - m_selectStart;
}
INLINE int XEdit::getSelectHead() const				//获得选取的头
{
	if(m_selectEnd < m_selectStart)return  m_selectEnd;
	else return m_selectStart;
}
INLINE int XEdit::getSelectEnd() const				//获得选取的尾
{
	if(m_selectEnd < m_selectStart) return m_selectStart;
	else return m_selectEnd;
}
//INLINE void XEdit::setCallbackFun(void (* funInputChenge)(void *,int),void (* funInputOver)(void *,int),void *pClass)		//设置回调函数
//{
//	m_funInputChenge = funInputChenge;
//	m_funInputOver = funInputOver;
//	if(pClass != NULL) m_pClass = pClass;
//	else m_pClass = this;
//}
INLINE XBool XEdit::getEdtIsNumber()	//输入的数据是否为数值
{
	if(XString::getIsNumber(getString())) return XTrue;
	return XString::getIsHexNumber(getString()) >= 0;
}
INLINE int XEdit::getAsInt()
{
	if(XString::getIsNumber(getString())) return atoi(getString());
	return XString::getIsHexNumber(getString());
}
INLINE long long XEdit::getAsInt64()
{
	if(XString::getIsNumber(getString())) return _atoi64(getString());
	return XString::getIsHexNumber64(getString());
}
//XBool getAsBool();
INLINE float XEdit::getAsFloat()
{
	if(XString::getIsNumber(getString())) return XString::toValue<float>(getString());
	return XString::getIsHexNumber(getString());
}
INLINE void XEdit::setTextColor(const XFColor& color) 
{
	m_textColor = color;
	m_caption.setColor(m_textColor * m_color);
}
INLINE void XEdit::setIsPassword(bool flag) 
{
	if((m_isPassword && flag) ||
		(!m_isPassword && !flag)) return;	//重复的设置
	m_isPassword = flag;
	if (m_withPromptStr && strlen(m_curString) <= 0)
		m_caption.setIsPassWord(false);
	else
		m_caption.setIsPassWord(m_isPassword);
	//upDataShowStr();
}