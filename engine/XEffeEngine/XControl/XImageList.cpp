//#include "XEffeEngine.h"
#include "XImageList.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "../XBasicWindow.h"

void imageListCtrCB(void * pClass,int id)
{
	_XImageList& pPar = *(_XImageList*)pClass;
	if(pPar.m_leftBtn.getControlID() == id)
	{
		--pPar.m_nowShowImageIndex;
		pPar.updateState();
	}else
	if(pPar.m_rightBtn.getControlID() == id)
	{
		++pPar.m_nowShowImageIndex;
		pPar.updateState();
	}else
	if(pPar.m_imageSld.getControlID() == id)
	{
		pPar.m_nowShowImageIndex = toInt((float)(pPar.m_imageList.size() - pPar.m_showImageSum) * pPar.m_imageSld.getNowValue() * 0.01f);
		pPar.updateState(false);
	}
}
_XBool _XImageList::initWithoutTex(float buttonWidth,	//左右按钮的宽度
	const _XVector2 &imageSize,	//图片的尺寸
	int showImageSum)	//显示图片的数量
{
	if(m_isInited) return XFalse;	//防止重复初始化
	if(buttonWidth <= 0.0f || 
		imageSize.x <= 0.0f || 
		imageSize.y <= 0.0f || 
		showImageSum <= 0) return XFalse;	//数据不合法

	m_position.set(0.0f,0.0f);
	m_size.set(1.0f,1.0f);
	m_buttonWidth = buttonWidth;
	m_imageSize = imageSize;
	m_showImageSum = showImageSum;
	m_nowSelectImageIndex = -1;
	m_nowShowImageIndex = 0;

	float tmpH = m_imageSize.y + 10.0f;
	_XVector2 area[3];
	area[0].set(0.0f,tmpH * 0.5f);
	area[1].set(m_buttonWidth,0.0f);
	area[2].set(m_buttonWidth,tmpH);
	m_leftBtn.initWithoutTex(area,3," ",XEE::systemFont,_XVector2::zero);
	m_leftBtn.setPosition(m_position);
	m_leftBtn.setMouseDownCB(imageListCtrCB,this);
	m_leftBtn.setWithAction(XFalse);
	_XCtrlManger.decreaseAObject(&m_leftBtn);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_leftBtn);
#endif
	area[0].set(0.0f,0.0);
	area[1].set(m_buttonWidth,tmpH * 0.5f);
	area[2].set(0.0f,tmpH);
	m_rightBtn.initWithoutTex(area,3," ",XEE::systemFont,_XVector2::zero);
	m_rightBtn.setPosition(m_position.x + m_imageSize.x * m_showImageSum + m_buttonWidth,m_position.y);
	m_rightBtn.setMouseDownCB(imageListCtrCB,this);
	m_rightBtn.setWithAction(XFalse);
	_XCtrlManger.decreaseAObject(&m_rightBtn);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_rightBtn);
#endif
	m_imageSld.initWithoutTex(_XRect(0.0f,0.0f,m_imageSize.x * m_showImageSum,10.0f));
	m_imageSld.setPosition(m_position.x + m_buttonWidth,m_position.y);
	m_imageSld.setDataChangeCB(imageListCtrCB,imageListCtrCB,this);
	m_imageSld.setWithAction(XFalse);
	_XCtrlManger.decreaseAObject(&m_imageSld);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_imageSld);
#endif
	updateState();
	m_mouseRect.set(0.0f,0.0f,m_buttonWidth * 2.0f + m_imageSize.x * m_showImageSum,m_imageSize.y + 10.0f);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);

	if(m_funInit != NULL) m_funInit(m_pClass,m_objectID);

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;
	m_isInited = XTrue;

	_XCtrlManger.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif
	return XTrue;
}
void _XImageList::draw()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	for(int i = m_nowShowImageIndex;i < m_showImageSum + m_nowShowImageIndex;++ i)
	{
		if(i < 0 || i >= m_imageList.size())
		{//无效的值
			drawFillBoxExA(m_position + _XVector2(m_buttonWidth + m_imageSize.x * (i - m_nowShowImageIndex),10.0f) * m_size,m_imageSize * m_size,
				0.5f * m_color.fA,0.5f * m_color.fG,0.5 * m_color.fB,m_color.fA);
		}else
		{
			m_imageList[i]->draw();
		}
	}
	m_leftBtn.draw();	//左边的按钮
	m_rightBtn.draw();	//右边的按钮
	m_imageSld.draw();	//滑动条
	if(m_nowSelectImageIndex >= 0 && m_nowSelectImageIndex >= m_nowShowImageIndex && m_nowSelectImageIndex < m_showImageSum + m_nowShowImageIndex)
	{//画个黄色的线框(尚未完成)
		_XVector2 pos = m_position + _XVector2(m_buttonWidth + m_imageSize.x * (m_nowSelectImageIndex - m_nowShowImageIndex + 0.5f),10.0f + m_imageSize.y * 0.5f) * m_size;
		_XVector2 size = m_imageSize * m_size * 0.5f;
		drawBox(pos.x,pos.y,size.x,size.y,1,m_color.fR,m_color.fG,0,m_color.fA);
	}
}	
_XBool _XImageList::addImage(const char * filename)
{
	if(!m_isInited ||
		filename == NULL) return XFalse;
	_XSprite *tmpSpirte = NULL;
	tmpSpirte = createMem<_XSprite>();
	if(tmpSpirte == NULL) return XFalse;
	if(!tmpSpirte->init(filename))
	{
		XDELETE(tmpSpirte);
		return XFalse;
	}
	tmpSpirte->setColor(m_color);
	tmpSpirte->setIsTransformCenter(POINT_LEFT_TOP);
	tmpSpirte->setSize(m_imageSize.x * m_size.x / tmpSpirte->getTextureData()->textureSize.x,
				m_imageSize.y  * m_size.y / tmpSpirte->getTextureData()->textureSize.y);
	m_imageList.push_back(tmpSpirte);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(tmpSpirte);
#endif

	updateState();
	//if(m_imageList.size() - 1 >= m_nowShowImageIndex && m_imageList.size() - 1 < m_nowShowImageIndex + m_nowSelectImageIndex)
	//{//在显示范围之内，需要更新数据
	//	int offset = m_imageList.size() - 1 - m_nowShowImageIndex;
	//	tmpSpirte->setPosition(m_position + _XVector2(m_buttonWidth + m_imageSize.x * offset,10.0f) * m_size);
	//}
	return XTrue;
}
void _XImageList::updateState(bool flag)
{
	if(!m_isInited) return;
	if(m_showImageSum >= m_imageList.size())
	{
		m_imageSld.setNowValue(0.0f);
		m_imageSld.disable();
	}else
	{
		if(flag) m_imageSld.setNowValue((float)m_nowShowImageIndex/(float)(m_imageList.size() - m_showImageSum) * 100.0f,false);	//这一行必须加上，但是目前有问题
		m_imageSld.enable();
	}
	if(m_nowSelectImageIndex >= m_imageList.size()) m_nowSelectImageIndex = -1;	//如果选择超出范围则默认为取消选择
	if(m_nowShowImageIndex < 0) m_nowShowImageIndex = 0;
	if(m_nowShowImageIndex > m_imageList.size() - m_showImageSum) m_nowShowImageIndex = m_imageList.size() - m_showImageSum;
	if(m_nowShowImageIndex <= 0) m_leftBtn.disable();
	else m_leftBtn.enable();
	if(m_nowShowImageIndex + m_showImageSum >= m_imageList.size()) m_rightBtn.disable();
	else m_rightBtn.enable();
	//更新位置
	for(int i = m_nowShowImageIndex;i < m_showImageSum + m_nowShowImageIndex;++ i)
	{
		if(i >= 0 && i < m_imageList.size())
		{
			m_imageList[i]->setPosition(m_position + _XVector2(m_buttonWidth + m_imageSize.x * (i - m_nowShowImageIndex),10.0f) * m_size);
		}
	}
}
void _XImageList::release()
{
	if(!m_isInited) return;
	_XCtrlManger.decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(this);
#endif
	//这里进行资源释放
	if(m_funRelease != NULL) m_funRelease(m_pClass,m_objectID);
	for(int i = 0;i < m_imageList.size();++ i)
	{
		XDELETE(m_imageList[i]);
	}
	m_imageList.clear();
	m_isInited = XFalse;
}
_XBool _XImageList::mouseProc(float x,float y,_XMouseState mouseState)				//对于鼠标动作的响应函数
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
		for(int i = m_nowShowImageIndex;i < m_showImageSum + m_nowShowImageIndex;++ i)
		{
			if(i >= 0 && i < m_imageList.size() && m_imageList[i]->isInRect(x,y))
			{//在范围内
				if(i != m_nowSelectImageIndex && m_funSelectChange != NULL) m_funSelectChange(m_pClass,m_objectID);
				m_nowSelectImageIndex = i;
				break;
			}
		}
		m_isBeChoose = XTrue;
		return XTrue;
	}
	return XFalse;
}
void _XImageList::setSize(float x,float y)
{
	if(x <= 0 || y <= 0 ||
		!m_isInited) return;	//如果没有初始化直接退出
	m_size.set(x,y);
	m_leftBtn.setSize(m_size);
	m_rightBtn.setSize(m_size);
	m_rightBtn.setPosition(m_position.x + (m_buttonWidth + m_imageSize.x * m_showImageSum) * m_size.x,m_position.y);
	m_imageSld.setSize(m_size);
	m_imageSld.setPosition(m_position.x + m_buttonWidth * m_size.x,m_position.y);
	for(int i = 0;i < m_imageList.size();++ i)
	{
		m_imageList[i]->setSize(m_imageSize.x * m_size.x / m_imageList[i]->getTextureData()->textureSize.x,
			m_imageSize.y  * m_size.y / m_imageList[i]->getTextureData()->textureSize.y);
	}
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	updateState(false);
}