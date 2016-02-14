#include "XStdHead.h"
#include "XImageCtrl.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
namespace XE{
XBool XImageCtrl::init(const char *filename)			//从文件中读取图片
{
	if(m_isInited) return XFalse;
	if(filename == NULL) return XFalse;
	if(!m_texture.load(filename)) return XFalse;
	m_sprite.init(m_texture);
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_sprite);
#endif
	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;
	XCtrlManager.addACtrl(this);
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
XBool XImageCtrl::init(int w,int h,XColorMode mode)	//建立空的图片
{
	if(m_isInited) return XFalse;
	if(w <= 0 || h <= 0) return XFalse;
	if(!m_texture.createTexture(w,h,mode)) return XFalse;
	m_sprite.init(m_texture);
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);
	m_pixelSum = w * h;
	//这里需要对数据进行初始化(尚未完成)
	m_texture.reset();
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_sprite);
#endif
	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;
	XCtrlManager.addACtrl(this);
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
void XImageCtrl::release()
{
	if(m_isInited) return;
	XCtrlManager.decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif	
	m_isInited = false;
}
#if !WITH_INLINE_FILE
#include "XImageCtrl.inl"
#endif
}