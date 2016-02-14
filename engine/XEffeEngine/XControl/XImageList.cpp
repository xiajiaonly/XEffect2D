#include "XStdHead.h"
#include "XImageList.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
namespace XE{
void XImageList::ctrlProc(void *pClass,int id,int eventID)
{
	XImageList& pPar = *(XImageList*)pClass;
	if(pPar.m_leftBtn.getControlID() == id)
	{
		if(eventID == XButton::BTN_MOUSE_DOWN)
		{
			--pPar.m_curShowImageIndex;
			pPar.updateState();
		}
		return;
	}
	if(pPar.m_rightBtn.getControlID() == id)
	{
		if(eventID == XButton::BTN_MOUSE_DOWN)
		{
			++pPar.m_curShowImageIndex;
			pPar.updateState();
		}
		return;
	}
	if(pPar.m_imageSld.getControlID() == id)
	{
		if(eventID == XSlider::SLD_MOUSE_MOVE || eventID == XSlider::SLD_VALUE_CHANGE)
		{
			pPar.m_curShowImageIndex = XMath::toInt((float)((int)(pPar.m_imageList.size()) - pPar.m_showImageSum) * pPar.m_imageSld.getCurValue() * 0.01f);
			pPar.updateState(false);
		}
		return;
	}
}
XBool XImageList::initWithoutSkin(float buttonWidth,	//左右按钮的宽度
	const XVector2 &imageSize,	//图片的尺寸
	int showImageSum)	//显示图片的数量
{
	if(m_isInited) return XFalse;	//防止重复初始化
	if(buttonWidth <= 0.0f || 
		imageSize.x <= 0.0f || 
		imageSize.y <= 0.0f || 
		showImageSum <= 0) return XFalse;	//数据不合法

	m_position.set(0.0f,0.0f);
	m_scale.set(1.0f,1.0f);
	m_buttonWidth = buttonWidth;
	m_imageSize = imageSize;
	m_showImageSum = showImageSum;
	m_curSelectImageIndex = -1;
	m_curShowImageIndex = 0;

	float tmpH = m_imageSize.y + 10.0f;
	XVector2 area[3];
	area[0].set(0.0f,tmpH * 0.5f);
	area[1].set(m_buttonWidth,0.0f);
	area[2].set(m_buttonWidth,tmpH);
	m_leftBtn.initWithoutSkin(area,3," ",getDefaultFont(),XVector2::zero);
	m_leftBtn.setPosition(m_position);
	m_leftBtn.setEventProc(ctrlProc,this);
	m_leftBtn.setWithAction(XFalse);
	XCtrlManager.decreaseAObject(&m_leftBtn);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_leftBtn);
#endif
	area[0].set(0.0f,0.0);
	area[1].set(m_buttonWidth,tmpH * 0.5f);
	area[2].set(0.0f,tmpH);
	m_rightBtn.initWithoutSkin(area,3," ",getDefaultFont(),XVector2::zero);
	m_rightBtn.setPosition(m_position.x + m_imageSize.x * m_showImageSum + m_buttonWidth,m_position.y);
	m_rightBtn.setEventProc(ctrlProc,this);
	m_rightBtn.setWithAction(XFalse);
	XCtrlManager.decreaseAObject(&m_rightBtn);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_rightBtn);
#endif
	m_imageSld.initWithoutSkin(XRect(0.0f,0.0f,m_imageSize.x * m_showImageSum,10.0f));
	m_imageSld.setPosition(m_position.x + m_buttonWidth,m_position.y);
	m_imageSld.setEventProc(ctrlProc,this);
	m_imageSld.setWithAction(XFalse);
	XCtrlManager.decreaseAObject(&m_imageSld);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_imageSld);
#endif
	updateState();
	m_mouseRect.set(0.0f,0.0f,m_buttonWidth * 2.0f + m_imageSize.x * m_showImageSum,m_imageSize.y + 10.0f);
	m_curMouseRect.set(m_position.x + m_mouseRect.left * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y,
		m_position.x + m_mouseRect.right * m_scale.x,m_position.y + m_mouseRect.bottom * m_scale.y);

	//if(m_funInit != NULL) m_funInit(m_pClass,m_objectID);
	if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,IMGLST_INIT);
	else XCtrlManager.eventProc(m_objectID,IMGLST_INIT);	

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
void XImageList::draw()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	for(int i = m_curShowImageIndex;i < m_showImageSum + m_curShowImageIndex;++ i)
	{
		if(i < 0 || i >= m_imageList.size())
		{//无效的值
			XRender::drawFillBoxExA(m_position + XVector2(m_buttonWidth + m_imageSize.x * (i - m_curShowImageIndex),10.0f) * m_scale,m_imageSize * m_scale,
				XCCS::downColor * m_color);
		}else
		{
			m_imageList[i]->draw();
		}
	}
	m_leftBtn.draw();	//左边的按钮
	m_rightBtn.draw();	//右边的按钮
	m_imageSld.draw();	//滑动条
	if(m_curSelectImageIndex >= 0 && m_curSelectImageIndex >= m_curShowImageIndex && m_curSelectImageIndex < m_showImageSum + m_curShowImageIndex)
	{//画个黄色的线框(尚未完成)
		XVector2 pos = m_position + XVector2(m_buttonWidth + m_imageSize.x * (m_curSelectImageIndex - m_curShowImageIndex + 0.5f),10.0f + m_imageSize.y * 0.5f) * m_scale;
		XVector2 size = m_imageSize * m_scale * 0.5f;
		XRender::drawBox(pos.x,pos.y,size.x,size.y,1,m_color);
	}
}	
XBool XImageList::addImage(const char * filename)
{
	if(!m_isInited ||
		filename == NULL) return XFalse;
	XSprite *tmpSpirte = NULL;
	tmpSpirte = XMem::createMem<XSprite>();
	if(tmpSpirte == NULL) return XFalse;
	if(!tmpSpirte->init(filename))
	{
		XMem::XDELETE(tmpSpirte);
		return XFalse;
	}
	tmpSpirte->setColor(m_color);
	tmpSpirte->setIsTransformCenter(POINT_LEFT_TOP);
	tmpSpirte->setScale(m_imageSize.x * m_scale.x / tmpSpirte->getTextureData()->textureSize.x,
				m_imageSize.y  * m_scale.y / tmpSpirte->getTextureData()->textureSize.y);
	m_imageList.push_back(tmpSpirte);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(tmpSpirte);
#endif

	updateState();
	//if((int)(m_imageList.size()) - 1 >= m_curShowImageIndex && (int)(m_imageList.size()) - 1 < m_curShowImageIndex + m_curSelectImageIndex)
	//{//在显示范围之内，需要更新数据
	//	int offset = (int)(m_imageList.size()) - 1 - m_curShowImageIndex;
	//	tmpSpirte->setPosition(m_position + XVector2(m_buttonWidth + m_imageSize.x * offset,10.0f) * m_scale);
	//}
	return XTrue;
}
void XImageList::updateState(bool flag)
{
	if(!m_isInited) return;
	if(m_showImageSum >= m_imageList.size() && m_showImageSum >= 0)
	{
		m_imageSld.setCurValue(0.0f);
		m_imageSld.disable();
	}else
	{
		if(flag) m_imageSld.setCurValue((float)m_curShowImageIndex/(float)((int)(m_imageList.size()) - m_showImageSum) * 100.0f,false);	//这一行必须加上，但是目前有问题
		m_imageSld.enable();
	}
	if(m_curSelectImageIndex >= m_imageList.size() && m_curSelectImageIndex >= 0) m_curSelectImageIndex = -1;	//如果选择超出范围则默认为取消选择
	if(m_curShowImageIndex < 0) m_curShowImageIndex = 0;
	if(m_curShowImageIndex > (int)(m_imageList.size()) - m_showImageSum) m_curShowImageIndex = (int)(m_imageList.size()) - m_showImageSum;
	if(m_curShowImageIndex <= 0) m_leftBtn.disable();
	else m_leftBtn.enable();
	if(m_curShowImageIndex + m_showImageSum >= m_imageList.size() &&
		m_curShowImageIndex + m_showImageSum >= 0) m_rightBtn.disable();
	else m_rightBtn.enable();
	//更新位置
	for(int i = m_curShowImageIndex;i < m_showImageSum + m_curShowImageIndex;++ i)
	{
		if(i >= 0 && i < m_imageList.size())
		{
			m_imageList[i]->setPosition(m_position + XVector2(m_buttonWidth + m_imageSize.x * (i - m_curShowImageIndex),10.0f) * m_scale);
		}
	}
}
void XImageList::release()
{
	if(!m_isInited) return;
	XCtrlManager.decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	//这里进行资源释放
	//if(m_funRelease != NULL) m_funRelease(m_pClass,m_objectID);
	if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,IMGLST_RELEASE);
	else XCtrlManager.eventProc(m_objectID,IMGLST_RELEASE);	
	for(unsigned int i = 0;i < m_imageList.size();++ i)
	{
		XMem::XDELETE(m_imageList[i]);
	}
	m_imageList.clear();
	m_isInited = XFalse;
}
XBool XImageList::mouseProc(float x,float y,XMouseState mouseState)				//对于鼠标动作的响应函数
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	m_leftBtn.mouseProc(x,y,mouseState);	//左边的按钮
	m_rightBtn.mouseProc(x,y,mouseState);	//右边的按钮
	m_imageSld.mouseProc(x,y,mouseState);	//滑动条
	if(isInRect(x,y) && (mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_LEFT_BUTTON_DCLICK))
	{//计算选中状态
		for(int i = m_curShowImageIndex;i < m_showImageSum + m_curShowImageIndex;++ i)
		{
			if(i >= 0 && i < m_imageList.size() && m_imageList[i]->isInRect(x,y))
			{//在范围内
				//if(i != m_curSelectImageIndex && m_funSelectChange != NULL) m_funSelectChange(m_pClass,m_objectID);
				if(i != m_curSelectImageIndex)
				{
					if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,IMGLST_SELECT_CHANGE);
					else XCtrlManager.eventProc(m_objectID,IMGLST_SELECT_CHANGE);
				}
				m_curSelectImageIndex = i;
				break;
			}
		}
		m_isBeChoose = XTrue;
		return XTrue;
	}
	return XFalse;
}
void XImageList::setScale(float x,float y)
{
	if(x <= 0 || y <= 0 ||
		!m_isInited) return;	//如果没有初始化直接退出
	m_scale.set(x,y);
	m_leftBtn.setScale(m_scale);
	m_rightBtn.setScale(m_scale);
	m_rightBtn.setPosition(m_position.x + (m_buttonWidth + m_imageSize.x * m_showImageSum) * m_scale.x,m_position.y);
	m_imageSld.setScale(m_scale);
	m_imageSld.setPosition(m_position.x + m_buttonWidth * m_scale.x,m_position.y);
	for(unsigned int i = 0;i < m_imageList.size();++ i)
	{
		m_imageList[i]->setScale(m_imageSize.x * m_scale.x / m_imageList[i]->getTextureData()->textureSize.x,
			m_imageSize.y  * m_scale.y / m_imageList[i]->getTextureData()->textureSize.y);
	}
	m_curMouseRect.set(m_position.x + m_mouseRect.left * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y,
		m_position.x + m_mouseRect.right * m_scale.x,m_position.y + m_mouseRect.bottom * m_scale.y);
	updateState(false);
}
#if !WITH_INLINE_FILE
#include "XImageList.inl"
#endif
}