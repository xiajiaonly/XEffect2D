#ifndef _JIA_XFLASHSTGOBJ_
#define _JIA_XFLASHSTGOBJ_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.3
//--------------------------------
//#define WITH_FLASH	//如果需要支持这个类的话，需要开启这个开关
#ifdef WITH_FLASH
#include "XStageObject.h"
#include "XFlashSprite.h"
namespace XE{
//精灵类的场景物件
class XFlashStgObj:public XStageObject
{
private:
	bool m_isInited;	//是否完成初始化
	XFlashSprite *m_pFSprite;
public:
	XFlashStgObj()
		:m_isInited(false)
		,m_pFSprite(NULL)
	{}
	bool init(XFlashSprite *pSprite,const XVec2& originOffset)
	{
		if(m_isInited || pSprite == NULL) return false;

		m_pFSprite = pSprite;
		m_originOffset = originOffset;

		m_isInited = true;
		return true;
	}
//下面两个接口是被内部调用的，外部不能调用
	virtual void setPosition(const XVec2& p)
	{
		if(!m_isInited) return;
		m_pFSprite->setPosition(p);
	}
	virtual void setScale(const XVec2& s)
	{
		if(!m_isInited) return;
		m_pFSprite->setScale(s);
	}
	virtual void draw()
	{
		if(!m_isInited || !m_visible) return;
		m_pFSprite->draw();
	}
	virtual void update(float stepTime)
	{
		if(!m_isInited) return;
		m_pFSprite->update();
	}
};
}
#endif
#endif