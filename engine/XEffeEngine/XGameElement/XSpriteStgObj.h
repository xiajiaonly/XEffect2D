#ifndef _JIA_XSPRITESTGOBJ_
#define _JIA_XSPRITESTGOBJ_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.3
//--------------------------------
#include "XStageObject.h"
#include "XSprite.h"
namespace XE{
//精灵类的场景物件
class XSpriteStgObj:public XStageObject
{
private:
	bool m_isInited;	//是否完成初始化
	XSprite *m_pSprite;
public:
	XSpriteStgObj()
		:m_isInited(false)
		,m_pSprite(NULL)
	{}
	virtual ~XSpriteStgObj(){}
	bool init(XSprite *pSprite,const XVec2& originOffset)
	{
		if(m_isInited || pSprite == NULL) return false;

		m_pSprite = pSprite;
		m_originOffset = originOffset;

		m_isInited = true;
		return true;
	}
//下面两个接口是被内部调用的，外部不能调用
	virtual void setPosition(const XVec2& p)
	{
		if(!m_isInited) return;
		m_pSprite->setPosition(p);
	}
	virtual void setScale(const XVec2& s)
	{
		if(!m_isInited) return;
		m_pSprite->setScale(s);
	}
	virtual void draw()
	{
		if(!m_isInited || !m_visible) return;
		m_pSprite->draw();
	}
	virtual void update(float/* stepTime*/){}
};
}
#endif