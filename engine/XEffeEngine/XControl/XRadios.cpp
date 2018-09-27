#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XRadios.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
namespace XE{
XRadios::XRadios()
	:m_isInited(XFalse)			//空间时都已经初始化
	, m_radioSum(1)			//选项的数量
	, m_curChoose(0)			//当前所选的项的编号
	, m_radio(NULL)			//所有单选项的指针
	, m_checkPosition(NULL)
	//,m_funStateChange(NULL)
	, m_resInfo(NULL)
	, m_withoutTex(XFalse)
{
	m_ctrlType = CTRL_OBJ_RADIOS;
}
void XRadios::release()	//释放分配的资源
{
	if (!m_isInited) return;
	XMem::XDELETE_ARRAY(m_radio);
	XMem::XDELETE_ARRAY(m_checkPosition);
	XCtrlManager.decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	if (m_resInfo != NULL)
	{
		XResManager.releaseResource(m_resInfo);
		m_resInfo = NULL;
	}
	m_isInited = XFalse;
}
XBool XRadios::init(int radioSum,			//选项的数量
		const XVec2& distance,
		const XVec2& position,	//控件的位置
		const XRect& area,			//选择图标的鼠标响应范围
		const XRadiosSkin *tex,const XFontUnicode& font,float captionSize,
		const XVec2& textPosition)	//单选框初始化
{
	if(m_isInited || area.getHeight() <= 0 || area.getWidth() <= 0) return XFalse;	//空间必须要有一个响应区间，不然会出现除零错误
	if(radioSum < 1) return XFalse;	//不能没有选项，必须要有一个选项
	if(captionSize <= 0) return XFalse;
	m_mouseRect = area;
	m_position = position;
	m_distance = distance;
	m_withoutTex = XFalse;

	if(tex->checkChoosed == NULL || tex->checkDisableChoosed == NULL 
		|| tex->checkDisableDischoose == NULL || tex->checkDischoose == NULL) return XFalse;//如果贴图缺少，也直接返回失败
	//分配内存空间
	m_radioSum = radioSum;
	m_radio = XMem::createArrayMem<XCheck>(m_radioSum);
	if(m_radio == NULL) return XFalse;//如果内存分配失败，直接返回错误
	m_checkPosition = XMem::createArrayMem<XVec2>(m_radioSum);
	if(m_checkPosition == NULL)
	{
		XMem::XDELETE_ARRAY(m_radio);
		return XFalse;
	}
	m_scale.set(1.0f);

	m_texture = tex;
	if(!m_caption.setACopy(font)) return XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //设置字体居中对齐
	m_textColor.set(0.0f,1.0f);
	m_caption.setColor(m_textColor);							//设置字体的颜色为黑色
	m_captionSize = captionSize;
	m_textPosition = textPosition;

#if WITH_OBJECT_MANAGER
	//printf("Cur obj sum:%d\n",XObjManager.getCurObjectSum());
#endif
	//初始化所有的选项
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_checkPosition[i].set(m_distance * i);
		if(!m_radio[i].init(m_position + m_checkPosition[i] * m_scale,
			m_mouseRect,* m_texture," ",m_caption,m_captionSize,m_textPosition))
		{
			XMem::XDELETE_ARRAY(m_radio);
			XMem::XDELETE_ARRAY(m_checkPosition);
			return XFalse;
		}
		m_radio[i].setState(XFalse);
		m_radio[i].setPosition(m_position + m_checkPosition[i] * m_scale);

		//在物件管理器中注销掉这些物件
		XCtrlManager.decreaseAObject(&(m_radio[i]));
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_radio[i]));
#endif
	}

#if WITH_OBJECT_MANAGER
	//printf("Cur obj sum:%d\n",XObjManager.getCurObjectSum());
#endif
	m_curChoose = 0;
	m_radio[0].setState(XTrue);

	m_isVisible = m_isEnable = m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
XBool XRadios::initWithoutSkin(int radioSum,
		const XVec2& distance,
		const XRect& area,
		const XFontUnicode& font,float captionSize,
		const XVec2& textPosition)
{
	if(m_isInited ||
		radioSum < 1 ||	//不能没有选项，必须要有一个选项
		captionSize <= 0) return XFalse;
	m_mouseRect = area;
	m_position.reset();
	m_distance = distance;
	m_withoutTex = XTrue;

	//分配内存空间
	m_radioSum = radioSum;
	m_radio = XMem::createArrayMem<XCheck>(m_radioSum);
	if(m_radio == NULL) return XFalse;//如果内存分配失败，直接返回错误
	m_checkPosition = XMem::createArrayMem<XVec2>(m_radioSum);
	if(m_checkPosition == NULL)
	{
		XMem::XDELETE_ARRAY(m_radio);
		return XFalse;
	}
	m_scale.set(1.0f);

	if(!m_caption.setACopy(font)) XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //设置字体居中对齐
	m_textColor.set(0.0f,1.0f);
	m_caption.setColor(m_textColor);							//设置字体的颜色为黑色
	m_captionSize = captionSize;
	m_textPosition = textPosition;

#if WITH_OBJECT_MANAGER
	//printf("Cur obj sum:%d\n",XObjManager.getCurObjectSum());
#endif
	//初始化所有的选项
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_checkPosition[i].set(m_distance * i);
		if(!m_radio[i].initWithoutSkin(" ",m_caption,m_captionSize,m_mouseRect,m_textPosition))
		{
			XMem::XDELETE_ARRAY(m_radio);
			XMem::XDELETE_ARRAY(m_checkPosition);
			return XFalse;
		}
		m_radio[i].setState(XFalse);
		m_radio[i].setPosition(m_position + m_checkPosition[i] * m_scale);
	//在物件管理器中注销掉这些物件
		XCtrlManager.decreaseAObject(&(m_radio[i]));
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_radio[i]));
#endif
	}

#if WITH_OBJECT_MANAGER
	//printf("Cur obj sum:%d\n",XObjManager.getCurObjectSum());
#endif
	m_curChoose = 0;
	m_radio[0].setState(XTrue);
	stateChange();

	m_isVisible = m_isEnable = m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
void XRadios::setChoosed(int temp)
{
	if(temp < 0 || temp >= m_radioSum ||
		temp == m_curChoose) return;
	m_curChoose = temp;
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_radio[i].setState(XFalse);
	}
	m_radio[m_curChoose].setState(XTrue);
	if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,RDS_STATE_CHANGE);
	else XCtrlManager.eventProc(m_objectID,RDS_STATE_CHANGE);
}
XBool XRadios::mouseProc(const XVec2& p,XMouseState mouseState)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	if(m_isSilent) return XFalse;

	int tempOrder = -1;	//记录被操作的选项的编号
	for(int i = 0;i < m_radioSum;++ i)
	{
		if(m_radio[i].mouseProc(p,mouseState))
		{//这里默认只会有一个选项被操作
			if(m_curChoose != i) tempOrder = i;
			else m_radio[m_curChoose].setState(XTrue);
		}
	}
	if(tempOrder != -1)
	{
		//printf("CurChoose:%d,%d\n",tempOrder,m_objectID);
		m_curChoose = tempOrder;
		for(int i = 0;i < m_radioSum;++ i)
		{
			m_radio[i].setState(XFalse);
		}
		m_radio[m_curChoose].setState(XTrue);
		if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,RDS_STATE_CHANGE);
		else XCtrlManager.eventProc(m_objectID,RDS_STATE_CHANGE);
		m_isBeChoose = XTrue;
		stateChange();
	}
	return XTrue;
}
XBool XRadios::keyboardProc(int keyOrder,XKeyState keyState)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	if(m_isSilent) return XFalse;

	if(keyState == KEY_STATE_UP && m_isBeChoose)
	{//按键弹起时才作相应
		if(keyOrder == XKEY_LEFT || keyOrder == XKEY_UP)
		{//按下向左向上
			--m_curChoose;
			if(m_curChoose < 0) m_curChoose = m_radioSum - 1;
			for(int i = 0;i < m_radioSum;++ i)
			{
				m_radio[i].setState(XFalse);
			}
			m_radio[m_curChoose].setState(XTrue);
			if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,RDS_STATE_CHANGE);
			else XCtrlManager.eventProc(m_objectID,RDS_STATE_CHANGE);
			stateChange();
		}else
		if(keyOrder == XKEY_RIGHT || keyOrder == XKEY_DOWN)
		{//按下向右向下
			++m_curChoose;
			if(m_curChoose >= m_radioSum) m_curChoose = 0;
			for(int i = 0;i < m_radioSum;++ i)
			{
				m_radio[i].setState(XFalse);
			}
			m_radio[m_curChoose].setState(XTrue);
			if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,RDS_STATE_CHANGE);
			else XCtrlManager.eventProc(m_objectID,RDS_STATE_CHANGE);
			stateChange();
		}
	}
	return XTrue;
}
void XRadios::setScale(const XVec2& s)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		s.x <= 0 || s.y <= 0) return;
	m_scale = s;
	//移动所有选项控件的相对位置
	for(int i = 0;i < m_radioSum;++ i)
	{
	//	m_radio[i].setPosition(m_position.x + m_checkPosition[i].x * m_scale.x,m_position.y + m_checkPosition[i].y * m_scale.y);
		m_radio[i].setPosition(m_position + m_checkPosition[i] * m_scale);
		m_radio[i].setScale(m_scale);
	}
}
void XRadios::setPosition(const XVec2& p)
{
	if(!m_isInited) return;	//如果没有初始化直接退出
	m_position = p;
	//移动所有选项控件的相对位置
	for(int i = 0;i < m_radioSum;++ i)
	{
	//	m_radio[i].setPosition(m_position.x + m_checkPosition[i].x * m_scale.x,m_position.y + m_checkPosition[i].y * m_scale.y);
		m_radio[i].setPosition(m_position + m_checkPosition[i] * m_scale);
	}
}
XBool XRadios::setRadioSum(int radioSum)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		radioSum <= 0) return XFalse;
	if(radioSum == m_radioSum) return true;
#if WITH_OBJECT_MANAGER
	//printf("Cur obj sum:%d\n",XObjManager.getCurObjectSum());
#endif
	XMem::XDELETE_ARRAY(m_radio);
	XMem::XDELETE_ARRAY(m_checkPosition);
#if WITH_OBJECT_MANAGER
	//printf("Cur obj sum:%d\n",XObjManager.getCurObjectSum());
#endif
	m_radioSum = radioSum;
	m_radio = XMem::createArrayMem<XCheck>(m_radioSum);
	if(m_radio == NULL) return XFalse;//如果内存分配失败，直接返回错误

#if WITH_OBJECT_MANAGER
	//printf("Cur obj sum:%d\n",XObjManager.getCurObjectSum());
#endif
	m_checkPosition = XMem::createArrayMem<XVec2>(m_radioSum);
	if(m_checkPosition == NULL)
	{
		XMem::XDELETE_ARRAY(m_radio);
		return XFalse;
	}

	//初始化所有的选项
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_checkPosition[i].set(m_distance * i);
	//	if((!m_withoutTex && !m_radio[i].init(XVec2(m_position.x + m_checkPosition[i].x * m_scale.x,m_position.y + m_checkPosition[i].y * m_scale.y),
		if((!m_withoutTex && !m_radio[i].init(m_position + m_checkPosition[i] * m_scale,
			m_mouseRect,* m_texture," ",m_caption,m_captionSize,m_textPosition))
			|| (m_withoutTex && !m_radio[i].initWithoutSkin(" ",m_caption,m_captionSize,m_mouseRect,m_textPosition)))
		{
			XMem::XDELETE_ARRAY(m_radio);
			XMem::XDELETE_ARRAY(m_checkPosition);
			return XFalse;
		}
		m_radio[i].setPosition(m_position + m_checkPosition[i] * m_scale);
		//在控件管理器中注销掉这些物件
		XCtrlManager.decreaseAObject(&(m_radio[i]));	
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_radio[i]));
#endif
		m_radio[i].setState(XFalse);
	}
#if WITH_OBJECT_MANAGER
	//printf("Cur obj sum:%d\n",XObjManager.getCurObjectSum());
#endif

	m_curChoose = 0;
	m_radio[0].setState(XTrue);
	return XTrue;
}
XBool XRadios::setACopy(const XRadios &temp)
{
	if(&temp == this ||
		!temp.m_isInited) return XFalse;
	if(m_isInited) release();
	if(!XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
		XObjManager.addAObject(this);
#endif
	}

	m_isInited = temp.m_isInited;
	m_radioSum = temp.m_radioSum;	//选项的数量
	m_curChoose = temp.m_curChoose;	//当前所选的项的编号
	m_distance = temp.m_distance;	//当前所选的项的编号

	if(m_resInfo != NULL) XResManager.releaseResource(m_resInfo);
	m_resInfo = XResManager.copyResource(temp.m_resInfo);
	m_withoutTex = temp.m_withoutTex;

	m_texture = temp.m_texture;
	m_caption.setACopy(temp.m_caption);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_captionSize = temp.m_captionSize;
	m_textColor = temp.m_textColor;
	m_textPosition = temp.m_textPosition;
	m_radio = XMem::createArrayMem<XCheck>(m_radioSum);
	if(m_radio == NULL) return XFalse;//如果内存分配失败，直接返回错误

	m_checkPosition = XMem::createArrayMem<XVec2>(m_radioSum);
	if(m_checkPosition == NULL)
	{
		XMem::XDELETE_ARRAY(m_radio);
		return XFalse;
	}
	for(int i = 0;i < m_radioSum;++i)
	{
		if(!m_radio[i].setACopy(temp.m_radio[i])) return XFalse;
		m_checkPosition[i] = temp.m_checkPosition[i];
		//在控件管理器中注销掉这些物件
		XCtrlManager.decreaseAObject(&(m_radio[i]));	
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_radio[i]));
#endif
	}

	m_funStateChange = temp.m_funStateChange;			//控件状态改变时调用
	m_pClass = temp.m_pClass;
	return XTrue;
}
XVec2 XRadios::getBox(int order)
{
	if(!m_isInited) return XVec2::zero;
	XVec2 ret;
	XVec2 temp;
	//遍历所有的按钮，找到最外围的框，每次都遍历，会不会太吃效率
	switch(order)
	{
	case 0:
		ret = m_radio[0].getBox(0);
		for(int i = 0;i < m_radioSum;++i)
		{
			temp = m_radio[i].getBox(0);
			if(temp.x < ret.x) ret.x = temp.x;
			if(temp.y < ret.y) ret.y = temp.y;
		}
		return ret;
	case 1:
		ret = m_radio[0].getBox(1);
		for(int i = 0;i < m_radioSum;++i)
		{
			temp = m_radio[i].getBox(1);
			if(temp.x > ret.x) ret.x = temp.x;
			if(temp.y < ret.y) ret.y = temp.y;
		}
		return ret;
	case 2:
		ret = m_radio[0].getBox(2);
		for(int i = 0;i < m_radioSum;++i)
		{
			temp = m_radio[i].getBox(2);
			if(temp.x > ret.x) ret.x = temp.x;
			if(temp.y > ret.y) ret.y = temp.y;
		}
		return ret;
	case 3:
		ret = m_radio[0].getBox(3);
		for(int i = 0;i < m_radioSum;++i)
		{
			temp = m_radio[i].getBox(3);
			if(temp.x < ret.x) ret.x = temp.x;
			if(temp.y > ret.y) ret.y = temp.y;
		}
		return ret;
	}
	return XVec2::zero;
}
void XRadios::setRadiosText(const char * temp)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		temp == NULL) return;
	int sum = XString::getCharSum(temp,';');
	if(sum < 0) sum = 0;
	int len = strlen(temp);
	if(temp[len - 1] != ';') sum += 1;	//不是以';'结束
	if(!setRadioSum(sum)) return;
	int offset = 0;
	for(int i = 0;i < sum;++ i)
	{
		len = XString::getCharPosition(temp + offset,';');
		std::string tmpStr = temp + offset;
		if(len >= 0) tmpStr.at(len) = '\0';
		setRadioText(tmpStr.c_str(),i);
		offset += len + 1;
	}
}
#if !WITH_INLINE_FILE
#include "XRadios.inl"
#endif
}