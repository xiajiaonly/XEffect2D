#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include"XSlider.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "XResourcePack.h"
namespace XE{
XSliderSkin::XSliderSkin()
:m_isInited(XFalse)
,sliderLineNormal(NULL)			
,sliderLineDown(NULL)			
,sliderLineOn(NULL)					
,sliderLineDisable(NULL)			
,sliderButtonNormal(NULL)			
,sliderButtonDown(NULL)			
,sliderButtonOn(NULL)					
,sliderButtonDisable(NULL)			
{}
XBool XSliderSkin::init(const char *LNormal,const char *LDown,const char *LOn,const char *LDisable,
						  const char *BNormal,const char *BDown,const char *BOn,const char *BDisable,XResourcePosition resoursePosition)
{
	if(m_isInited ||
		LNormal == NULL || BNormal == NULL) return XFalse;

	int ret = 1;
	//分别载入资源	
	if((sliderLineNormal = createATextureData(LNormal,resoursePosition)) == NULL) ret = 0;
	if(LDown != NULL && ret != 0 &&
		(sliderLineDown = createATextureData(LDown,resoursePosition)) == NULL) ret = 0;
	if(LOn != NULL && ret != 0 &&
		(sliderLineOn = createATextureData(LOn,resoursePosition)) == NULL) ret = 0;
	if(LDisable != NULL && ret != 0 &&
		(sliderLineDisable = createATextureData(LDisable,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(sliderButtonNormal = createATextureData(BNormal,resoursePosition)) == NULL) ret = 0;
	if(BDown != NULL && ret != 0 &&
		(sliderButtonDown = createATextureData(BDown,resoursePosition)) == NULL) ret = 0;
	if(BOn != NULL && ret != 0 &&
		(sliderButtonOn = createATextureData(BOn,resoursePosition)) == NULL) ret = 0;
	if(BDisable != NULL && ret != 0 &&
		(sliderButtonDisable = createATextureData(BDisable,resoursePosition)) == NULL) ret = 0;
	if(ret == 0)
	{
		releaseTex();
		return XFalse;
	}

	m_isInited = XTrue;
	return XTrue;
}
#define SLIDER_CONFIG_FILENAME "Slider.txt"
bool XSliderSkin::loadFromFolder(const char *filename,XResourcePosition resPos)	//从文件夹中载入资源
{
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf(tempFilename,"%s/%s",filename,SLIDER_CONFIG_FILENAME);
	FILE *fp = NULL;
	if((fp = fopen(tempFilename,"r")) == NULL) return XFalse; //信息文件读取失败
	//下面开始依次读取数据
	int flag = 0;
	char resFilename[MAX_FILE_NAME_LENGTH] = "";
	//normal
	if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
	if(flag == 0)
	{//没有普通状态是不行的
		fclose(fp);
		return XFalse;
	}
	if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
	sprintf(tempFilename,"%s/%s",filename,resFilename);
	if((sliderLineNormal = createATextureData(tempFilename,resPos)) == NULL)
	{//资源读取失败
		fclose(fp);
		return XFalse;
	}
	//down
	if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
	if(flag != 0)
	{
		if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((sliderLineDown = createATextureData(tempFilename,resPos)) == NULL)
		{//资源读取失败
			releaseTex();
			fclose(fp);
			return XFalse;
		}
	}
	//on
	if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
	if(flag != 0)
	{
		if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((sliderLineOn = createATextureData(tempFilename,resPos)) == NULL)
		{//资源读取失败
			releaseTex();
			fclose(fp);
			return XFalse;
		}
	}
	//disable
	if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
	if(flag != 0)
	{
		if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((sliderLineDisable = createATextureData(tempFilename,resPos)) == NULL)
		{//资源读取失败
			releaseTex();
			fclose(fp);
			return XFalse;
		}
	}
	//
	if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
	if(flag == 0)
	{//没有普通状态是不行的
		releaseTex();
		fclose(fp);
		return XFalse;
	}
	if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
	sprintf(tempFilename,"%s/%s",filename,resFilename);
	if((sliderButtonNormal = createATextureData(tempFilename,resPos)) == NULL)
	{//资源读取失败
		releaseTex();
		fclose(fp);
		return XFalse;
	}
	//
	if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
	if(flag != 0)
	{
		if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((sliderButtonDown = createATextureData(tempFilename,resPos)) == NULL)
		{//资源读取失败
			releaseTex();
			fclose(fp);
			return XFalse;
		}
	}
	//
	if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
	if(flag != 0)
	{
		if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((sliderButtonOn = createATextureData(tempFilename,resPos)) == NULL)
		{//资源读取失败
			releaseTex();
			fclose(fp);
			return XFalse;
		}
	}
	//
	if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
	if(flag != 0)
	{
		if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((sliderButtonDisable = createATextureData(tempFilename,resPos)) == NULL)
		{//资源读取失败
			releaseTex();
			fclose(fp);
			return XFalse;
		}
	}
	//读取两组数据
	int l,t,r,b;
	if(fscanf(fp,"%d,%d,%d,%d,",&l,&t,&r,&b) != 4) {fclose(fp);return XFalse;}
	m_mouseRect.set(l,t,r,b);
	if(fscanf(fp,"%d,%d,%d,%d,",&l,&t,&r,&b) != 4) {fclose(fp);return XFalse;}
	m_mouseRectButton.set(l,t,r,b);
	if(fscanf(fp,"%d,%d,",&l,&t) != 2) {fclose(fp);return XFalse;}
	m_fontPosition.set(l,t);
	//所有数据读取完成
	fclose(fp);
	return true;
}
bool XSliderSkin::loadFromPacker(const char *filename,XResourcePosition resPos)	//从压缩包中载入资源
{
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf(tempFilename,"%s/%s",filename,SLIDER_CONFIG_FILENAME);
	unsigned char *p = XResPack.getFileData(tempFilename);
	if(p == NULL) return XFalse;
	//下面开始依次读取数据
	int flag = 0;
	char resFilename[MAX_FILE_NAME_LENGTH] = "";
	int offset = 0;
	//normal
	if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),':') + 1;
	if(flag == 0)
	{//没有普通状态是不行的
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
	sprintf(tempFilename,"%s/%s",filename,resFilename);
	if((sliderLineNormal = createATextureData(tempFilename,resPos)) == NULL)
	{//资源读取失败
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	//down
	if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),':') + 1;
	if(flag != 0)
	{
		if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
		offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((sliderLineDown = createATextureData(tempFilename,resPos)) == NULL)
		{//资源读取失败
			releaseTex();
			XMem::XDELETE_ARRAY(p);
			return XFalse;
		}
	}
	//on
	if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),':') + 1;
	if(flag != 0)
	{
		if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
		offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((sliderLineOn = createATextureData(tempFilename,resPos)) == NULL)
		{//资源读取失败
			releaseTex();
			XMem::XDELETE_ARRAY(p);
			return XFalse;
		}
	}
	//disable
	if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),':') + 1;
	if(flag != 0)
	{
		if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
		offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((sliderLineDisable = createATextureData(tempFilename,resPos)) == NULL)
		{//资源读取失败
			releaseTex();
			XMem::XDELETE_ARRAY(p);
			return XFalse;
		}
	}
	//normal
	if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),':') + 1;
	if(flag == 0)
	{//没有普通状态是不行的
		releaseTex();
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
	sprintf(tempFilename,"%s/%s",filename,resFilename);
	if((sliderButtonNormal = createATextureData(tempFilename,resPos)) == NULL)
	{//资源读取失败
		releaseTex();
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	//
	if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),':') + 1;
	if(flag != 0)
	{
		if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
		offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((sliderButtonDown = createATextureData(tempFilename,resPos)) == NULL)
		{//资源读取失败
			releaseTex();
			XMem::XDELETE_ARRAY(p);
			return XFalse;
		}
	}
	//
	if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),':') + 1;
	if(flag != 0)
	{
		if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
		offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((sliderButtonOn = createATextureData(tempFilename,resPos)) == NULL)
		{//资源读取失败
			releaseTex();
			XMem::XDELETE_ARRAY(p);
			return XFalse;
		}
	}
	//
	if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),':') + 1;
	if(flag != 0)
	{
		if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
		offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((sliderButtonDisable = createATextureData(tempFilename,resPos)) == NULL)
		{//资源读取失败
			releaseTex();
			XMem::XDELETE_ARRAY(p);
			return XFalse;
		}
	}
	//读取两组数据
	int l,t,r,b;
	if(sscanf((char *)(p + offset),"%d,%d,%d,%d,",&l,&t,&r,&b) != 4) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
	m_mouseRect.set(l,t,r,b);
	if(sscanf((char *)(p + offset),"%d,%d,%d,%d,",&l,&t,&r,&b) != 4) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
	m_mouseRectButton.set(l,t,r,b);
	if(sscanf((char *)(p + offset),"%d,%d,",&l,&t) != 2)
	{
		offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
		m_fontPosition.set(l,t);
	}else
	{
		m_fontPosition.set(0,0);
	}
	//所有数据读取完成
	XMem::XDELETE_ARRAY(p);
	return true;
}
bool XSliderSkin::loadFromWeb(const char *filename,XResourcePosition resPos)		//从网页中读取资源
{
	return false;
}
XBool XSliderSkin::initEx(const char *filename,XResourcePosition resoursePosition)
{
	if(m_isInited ||
		filename == NULL) return XFalse;
	//先打开配置文件
	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = getDefResPos();
	switch(resoursePosition)
	{
	case RESOURCE_LOCAL_PACK:
		if(!loadFromPacker(filename,resoursePosition)) return false;
		break;
	case RESOURCE_LOCAL_FOLDER:
		if(!loadFromFolder(filename,resoursePosition)) return false;
		break;
	case RESOURCE_WEB:
		if(!loadFromWeb(filename,resoursePosition)) return false;
		break;
	case RESOURCE_AUTO:
		if(!loadFromPacker(filename,resoursePosition) && !loadFromFolder(filename,resoursePosition) &&
			!loadFromWeb(filename,resoursePosition)) return false;
		break;
	}
	m_isInited = XTrue;
	return XTrue;
}
XSlider::XSlider()
:m_isInited(XFalse)
,m_curSliderState(SLIDER_STATE_NORMAL)
,m_typeVorH(SLIDER_TYPE_VERTICAL)
,m_sliderLineNormal(NULL)			
,m_sliderLineDown(NULL)			
,m_sliderLineOn(NULL)			
,m_sliderLineDisable(NULL)		
,m_sliderButtonNormal(NULL)			
,m_sliderButtonDown(NULL)	
,m_sliderButtonOn(NULL)				
,m_sliderButtonDisable(NULL)		
,m_minValue(0)	
,m_maxValue(100)	
,m_curValue(0)		
,m_mouseFlyArea(50,50)	//这个值使用一个系统设定，这个值不能在代码中进行修改，这是不理想的，但是考虑到它存在的无形性，所以默认允许这样操作
,m_needFont(XFalse)
,m_pVariable(NULL)
,m_resInfo(NULL)
,m_withoutTex(XFalse)
,m_textColor(0.0f,0.0f,0.0f,1.0f)
{
	m_ctrlType = CTRL_OBJ_SLIDER;
}
void XSlider::release()
{
	XCtrlManager.decreaseAObject(this);	//注销这个物件
	if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,SLD_RELEASE);
	else XCtrlManager.eventProc(m_objectID,SLD_RELEASE);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	if(m_resInfo != NULL)
	{
		XResManager.releaseResource(m_resInfo);
		m_resInfo = NULL;
	}
}
void XSlider::setTexture(const XSliderSkin &tex)
{
	m_sliderLineNormal = tex.sliderLineNormal;
	m_sliderButtonNormal = tex.sliderButtonNormal;
	if(tex.sliderLineDown != NULL) m_sliderLineDown = tex.sliderLineDown;
	else m_sliderLineDown = tex.sliderLineNormal;
	if(tex.sliderLineOn != NULL) m_sliderLineOn = tex.sliderLineOn;
	else m_sliderLineOn = tex.sliderLineNormal;
	if(tex.sliderLineDisable != NULL) m_sliderLineDisable = tex.sliderLineDisable;
	else m_sliderLineDisable = tex.sliderLineNormal;

	if(tex.sliderButtonDown != NULL) m_sliderButtonDown = tex.sliderButtonDown;
	else m_sliderButtonDown = tex.sliderButtonNormal;
	if(tex.sliderButtonOn != NULL) m_sliderButtonOn = tex.sliderButtonOn;
	else m_sliderButtonOn = tex.sliderButtonNormal;
	if(tex.sliderButtonDisable != NULL) m_sliderButtonDisable = tex.sliderButtonDisable;
	else m_sliderButtonDisable = tex.sliderButtonNormal;
}
XBool XSlider::init(const XVector2& position,	//控件所在的位置
		const XRect& Area,				//控件的鼠标响应范围
		const XRect& buttonArea,		//滑动块的鼠标响应范围
		const XSliderSkin &tex,float max,float min,XSliderType type)
{
	if(m_isInited) return XFalse;
	if(Area.getHeight() <= 0 || Area.getWidth() <= 0) return XFalse;	//空间必须要有一个响应区间，不然会出现除零错误
	if(tex.sliderLineNormal == NULL || tex.sliderButtonNormal == NULL) return XFalse;	//按键的普通状态的贴图不能为空，否则直接返回错误
	if(min >= max) return XFalse;
	m_mouseRect = Area;
	m_buttonArea = buttonArea;
	m_position = position;

	m_typeVorH = type;
	m_withoutTex = XFalse;

	setTexture(tex);

	m_scale.set(1.0f,1.0f);

	m_minValue = min;		
	m_maxValue = max;	
	m_curValue = min;	
	m_upValue = min;
	m_dataChanged = XFalse;
	m_keyOneValue = (m_maxValue - m_minValue) * 0.01f;

	//线的位置不需要计算
	m_lineSprite.init(m_sliderLineNormal->texture.m_w,m_sliderLineNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_lineSprite);
#endif
	m_lineSprite.setPosition(m_position);
	m_lineSprite.setScale(m_scale);
	m_lineSprite.setIsTransformCenter(POINT_LEFT_TOP);
	m_curMouseRect.set(m_position.x + m_mouseRect.left * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y,
		m_position.x +  + m_mouseRect.right * m_scale.x,m_position.y +  + m_mouseRect.bottom * m_scale.y);

	m_buttonSprite.init(m_sliderButtonNormal->texture.m_w,m_sliderButtonNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_buttonSprite);
#endif
	m_buttonSprite.setScale(m_scale);
	m_buttonSprite.setIsTransformCenter(POINT_LEFT_TOP);
	updateButtonData();

	if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,SLD_INIT);
	else XCtrlManager.eventProc(m_objectID,SLD_INIT);
	stateChange();

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
//int XSlider::initWithoutSkin(const XRect& area,const XRect& buttonArea,XSliderType type,float max,float min,const XVector2 &fontPosition)
XBool XSlider::initWithoutSkin(const XRect& area,float max,float min,XSliderType type,const XVector2 &fontPosition,bool narrowBtn)
{
	if(m_isInited ||
		min >= max) return XFalse;
	m_mouseRect = area;
	m_typeVorH = type;
	if(narrowBtn)
	{
		if(m_typeVorH == SLIDER_TYPE_HORIZONTAL) m_buttonArea.set(area.top,area.top,area.bottom * 0.5f,area.bottom);
		else m_buttonArea.set(area.left,area.left,area.right,area.right * 0.5f);
	}else
	{
		if(m_typeVorH == SLIDER_TYPE_HORIZONTAL) m_buttonArea.set(area.top,area.top,area.bottom,area.bottom);
		else m_buttonArea.set(area.left,area.left,area.right,area.right);
	}
	m_textPosition = fontPosition;
	m_position.set(0.0f,0.0f);

	m_withoutTex = XTrue;

	m_scale.set(1.0f,1.0f);

	m_minValue = min;		
	m_maxValue = max;	
	m_curValue = min;	
	m_upValue = min;
	m_dataChanged = XFalse;
	m_keyOneValue = (m_maxValue - m_minValue) * 0.01f;

	//线的位置不需要计算
	m_curMouseRect.set(m_position.x + m_mouseRect.left * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y,
		m_position.x +  + m_mouseRect.right * m_scale.x,m_position.y +  + m_mouseRect.bottom * m_scale.y);

	updateButtonData();

	if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,SLD_INIT);
	else XCtrlManager.eventProc(m_objectID,SLD_INIT);
	stateChange();

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
XBool XSlider::setFont(const char *caption,const XFontUnicode &font,float captionSize,const XVector2 &fontPosition)
{
	if(caption != NULL) m_fontString = caption;
	else m_fontString = "";
	if(!m_caption.setACopy(font)) return XFalse;
	m_caption.setColor(m_textColor * m_color);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //设置字体居中对齐
	//m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor * m_color);	
	if(captionSize < 0) return XFalse;
	m_textSize.set(captionSize,captionSize);
	m_caption.setScale(m_textSize.x * m_scale.x,m_textSize.y * m_scale.y);
	char str[1024];
	sprintf(str,m_fontString.c_str(),m_curValue);
	m_caption.setString(str);
	m_textPosition = fontPosition;
	m_caption.setPosition(m_position.x + m_textPosition.x * m_scale.x,m_position.y + m_textPosition.y * m_scale.y);
	if(m_typeVorH == SLIDER_TYPE_VERTICAL) m_caption.setAngle(90.0f);
	else m_caption.setAngle(0.0f);
	m_needFont = XTrue;
	return XTrue;
}
void XSlider::setPosition(float x,float y)
{
	if(!m_isInited) return;	//如果没有初始化直接退出
	m_position.set(x,y);
	updateChildPos();
	updateButtonData();
	if(!m_withoutTex) m_lineSprite.setPosition(m_position);
	m_curMouseRect.set(m_position.x + m_mouseRect.left * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y,
		m_position.x +  + m_mouseRect.right * m_scale.x,m_position.y +  + m_mouseRect.bottom * m_scale.y);
	//还需要调用鼠标函数
	mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
	if(m_needFont)
		m_caption.setPosition(m_position.x + m_textPosition.x * m_scale.x,m_position.y + m_textPosition.y * m_scale.y);
}
void XSlider::updateButtonData()
{
	//计算滑块的位置
	if(m_typeVorH == SLIDER_TYPE_VERTICAL)
	{//垂直类型
		m_curButtonPosition.set(m_position.x + (m_mouseRect.left - m_buttonArea.left) * m_scale.y + (m_mouseRect.getWidth() - m_buttonArea.getWidth()) * m_scale.x * 0.5f,
			m_position.y + (m_mouseRect.top - m_buttonArea.top) * m_scale.y + 
			(m_mouseRect.getHeight() - m_buttonArea.getHeight()) * m_scale.y * (m_curValue - m_minValue)/(m_maxValue - m_minValue));
	}else
	if(m_typeVorH == SLIDER_TYPE_HORIZONTAL)
	{//水平类型
		m_curButtonPosition.set(m_position.x + (m_mouseRect.left - m_buttonArea.left) * m_scale.y + 
			(m_mouseRect.getWidth() - m_buttonArea.getWidth()) * m_scale.x * (m_curValue - m_minValue)/(m_maxValue - m_minValue),
			m_position.y + (m_mouseRect.top - m_buttonArea.top) * m_scale.y + (m_mouseRect.getHeight() - m_buttonArea.getHeight()) * m_scale.y * 0.5f);
	}
	if(!m_withoutTex) m_buttonSprite.setPosition(m_curButtonPosition);
//	m_curButtonArea.set(m_curButtonPosition.x + m_buttonArea.left * m_scale.x,m_curButtonPosition.y + m_buttonArea.top * m_scale.y,
//		m_curButtonPosition.x + m_buttonArea.right * m_scale.x,m_curButtonPosition.y + m_buttonArea.bottom * m_scale.y);
	if(m_needFont)
	{
		char str[1024];
		sprintf(str,m_fontString.c_str(),m_curValue);
		m_caption.setString(str);
	}
	if(m_pVariable != NULL) * m_pVariable = m_curValue;	//需要检查这行在这里的合理性
}
void XSlider::setScale(float x,float y)
{
	if(x <= 0 || y <= 0 ||
		!m_isInited) return;	//如果没有初始化直接退出
	m_scale.set(x,y);
	updateChildScale();
	if(!m_withoutTex)
	{
		m_lineSprite.setScale(m_scale);
		m_buttonSprite.setScale(m_scale);
	}
	updateButtonData();

	m_curMouseRect.set(m_position.x + m_mouseRect.left * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y,
		m_position.x +  + m_mouseRect.right * m_scale.x,m_position.y +  + m_mouseRect.bottom * m_scale.y);
	//还需要调用鼠标函数
	mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
	if(m_needFont)
	{
		m_caption.setPosition(m_position.x + m_textPosition.x * m_scale.x,m_position.y + m_textPosition.y * m_scale.y);
		m_caption.setScale(m_textSize.x * m_scale.x,m_textSize.y * m_scale.y);
	}
}
void XSlider::draw()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出

	if(m_withoutTex)
	{
		if(m_curSliderState == SLIDER_STATE_NORMAL)
		{
			XRender::drawFillBoxExA(m_position + XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y),
				XVector2(m_mouseRect.getWidth() * m_scale.x,m_mouseRect.getHeight() * m_scale.y),
				XCCS::downColor * m_color,true);
			XRender::drawFillBoxExA(m_curButtonPosition + XVector2(m_buttonArea.left * m_scale.x,m_buttonArea.top * m_scale.y),
				XVector2(m_buttonArea.getWidth() * m_scale.x,m_buttonArea.getHeight() * m_scale.y),
				XCCS::normalColor * m_color,true);
		//	m_lineSprite.draw(m_sliderLineNormal);
		//	m_buttonSprite.draw(m_sliderButtonNormal);
		}else
		if(m_curSliderState == SLIDER_STATE_DOWN)
		{
			XRender::drawFillBoxExA(m_position + XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y),
				XVector2(m_mouseRect.getWidth() * m_scale.x,m_mouseRect.getHeight() * m_scale.y),
				XCCS::mouseColor * m_color,true);
			XRender::drawFillBoxExA(m_curButtonPosition + XVector2(m_buttonArea.left * m_scale.x,m_buttonArea.top * m_scale.y),
				XVector2(m_buttonArea.getWidth() * m_scale.x,m_buttonArea.getHeight() * m_scale.y),
				XCCS::lightSpecialColor * m_color,true);
			//m_lineSprite.draw(m_sliderLineDown);
			//m_buttonSprite.draw(m_sliderButtonDown);
		}else
		if(m_curSliderState == SLIDER_STATE_ON)
		{
			XRender::drawFillBoxExA(m_position + XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y),
				XVector2(m_mouseRect.getWidth() * m_scale.x,m_mouseRect.getHeight() * m_scale.y),
				XCCS::specialColor * m_color,true);
			XRender::drawFillBoxExA(m_curButtonPosition + XVector2(m_buttonArea.left * m_scale.x,m_buttonArea.top * m_scale.y),
				XVector2(m_buttonArea.getWidth() * m_scale.x,m_buttonArea.getHeight() * m_scale.y),
				XCCS::onColor * m_color,true);
			//m_lineSprite.draw(m_sliderLineOn);
			//m_buttonSprite.draw(m_sliderButtonOn);
		}else
		if(m_curSliderState == SLIDER_STATE_DISABLE)
		{
			XRender::drawFillBoxExA(m_position + XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y),
				XVector2(m_mouseRect.getWidth() * m_scale.x,m_mouseRect.getHeight() * m_scale.y),
				XCCS::blackDownColor * m_color,true);
			XRender::drawFillBoxExA(m_curButtonPosition + XVector2(m_buttonArea.left * m_scale.x,m_buttonArea.top * m_scale.y),
				XVector2(m_buttonArea.getWidth() * m_scale.x,m_buttonArea.getHeight() * m_scale.y),
				XCCS::downColor * m_color,true);
			//m_lineSprite.draw(m_sliderLineDisable);
			//m_buttonSprite.draw(m_sliderButtonDisable);
		}	
	}else
	{
		if(m_curSliderState == SLIDER_STATE_NORMAL)
		{
			m_lineSprite.draw(m_sliderLineNormal);
			m_buttonSprite.draw(m_sliderButtonNormal);
		}else
		if(m_curSliderState == SLIDER_STATE_DOWN)
		{
			m_lineSprite.draw(m_sliderLineDown);
			m_buttonSprite.draw(m_sliderButtonDown);
		}else
		if(m_curSliderState == SLIDER_STATE_ON)
		{
			m_lineSprite.draw(m_sliderLineOn);
			m_buttonSprite.draw(m_sliderButtonOn);
		}else
		if(m_curSliderState == SLIDER_STATE_DISABLE)
		{
			m_lineSprite.draw(m_sliderLineDisable);
			m_buttonSprite.draw(m_sliderButtonDisable);
		}
	}
	if(m_needFont) m_caption.draw();
}
float XSlider::getMapValue(float v)
{
	float ret = 0.0f;
	if(m_typeVorH == SLIDER_TYPE_VERTICAL)
	{//垂直类型
		ret = m_minValue + (m_maxValue - m_minValue) * (v - m_position.y - (m_mouseRect.top + m_mouseRect.getWidth() * 0.5f) * m_scale.y) /
			((m_mouseRect.getHeight() - m_mouseRect.getWidth()) * m_scale.y); 
		if(ret < m_minValue) ret = m_minValue;
		if(ret > m_maxValue) ret = m_maxValue;
	}else
	if(m_typeVorH == SLIDER_TYPE_HORIZONTAL)
	{//水平类型
		ret = m_minValue + (m_maxValue - m_minValue) * (v - m_position.x - (m_mouseRect.left + m_mouseRect.getHeight() * 0.5f) * m_scale.x) / 
			((m_mouseRect.getWidth() - m_mouseRect.getHeight()) * m_scale.x); 
		if(ret < m_minValue) ret = m_minValue;
		if(ret > m_maxValue) ret = m_maxValue;
	}
	return ret;
}
XBool XSlider::mouseProc(float x,float y,XMouseState mouseState)
{
	m_upMousePoint.set(x,y);
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	if(m_withAction && m_isInAction) return XFalse;	//如果支持动作播放而且正在播放动画那么不接受鼠标控制
	//SliderLine,滚动条的线的事件

	if(m_curSliderState == SLIDER_STATE_DOWN)
	{//注意这里是滑动条拖动时，为了是超出范围的拖动仍然有效，所以这个这个特殊处理
		XRect tempFly(m_curMouseRect.left - m_mouseFlyArea.x,m_curMouseRect.top - m_mouseFlyArea.y,
			m_curMouseRect.right + m_mouseFlyArea.x,m_curMouseRect.bottom + m_mouseFlyArea.y);
		if(tempFly.isInRect(x,y))
		{//拖动(注意如果这里拖动超出范围，则使用最后一次的有效值)
			switch(mouseState)
			{
			case MOUSE_MOVE://重新计算Slider的当前值
				if(m_typeVorH == SLIDER_TYPE_VERTICAL) m_curValue = getMapValue(y);else
				if(m_typeVorH == SLIDER_TYPE_HORIZONTAL) m_curValue = getMapValue(x);
				updateButtonData();
				m_dataChanged = XTrue;	//标记数值发生改变
				if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,SLD_MOUSE_MOVE);
				else XCtrlManager.eventProc(m_objectID,SLD_MOUSE_MOVE);
				break;
			case MOUSE_LEFT_BUTTON_UP://这个弹起事件是在按钮上按下的弹起事件
				m_curSliderState = SLIDER_STATE_ON;
				if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,SLD_MOUSE_UP);
				else XCtrlManager.eventProc(m_objectID,SLD_MOUSE_UP);
				if(m_dataChanged)
				{
					stateChange();
					if(m_eventProc != NULL)
						m_eventProc(m_pClass,m_objectID,SLD_VALUE_CHANGE);
					else
						XCtrlManager.eventProc(m_objectID,SLD_VALUE_CHANGE);
					m_dataChanged = XFalse;
				}
				break;
			}
			m_isBeChoose = XTrue;
		}else
		{
			m_curSliderState = SLIDER_STATE_NORMAL;
			if(m_dataChanged)
			{
				stateChange();
				if(m_eventProc != NULL)
					m_eventProc(m_pClass,m_objectID,SLD_VALUE_CHANGE);
				else
					XCtrlManager.eventProc(m_objectID,SLD_VALUE_CHANGE);
				m_dataChanged = XFalse;
			}
		}
	}
	if(m_curMouseRect.isInRect(x,y))
	{//鼠标动作在范围内
		switch(mouseState)
		{
		case MOUSE_MOVE:
			if(m_curSliderState != SLIDER_STATE_NORMAL) break; //悬浮
			m_curSliderState = SLIDER_STATE_ON;
			if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,SLD_MOUSE_ON);
			else XCtrlManager.eventProc(m_objectID,SLD_MOUSE_ON);
			if(m_withAction)
			{//这里测试一个动态效果
				m_isInAction = XTrue;
				m_actionMoveData.set(1.0f,1.1f,0.02f,MOVE_DATA_MODE_SIN_MULT,1);
				m_lightMD.set(1.0f,2.0f,0.002f,MOVE_DATA_MODE_SIN_MULT);
				m_oldPos = m_position;
				m_oldSize = m_scale;
			} 
			break;
		case MOUSE_LEFT_BUTTON_DOWN:
		case MOUSE_LEFT_BUTTON_DCLICK:
			if(m_curSliderState != SLIDER_STATE_NORMAL && m_curSliderState != SLIDER_STATE_ON) break;
			m_upValue = m_curValue;

			//重新计算Slider的当前值
			if(m_typeVorH == SLIDER_TYPE_VERTICAL) m_curValue = getMapValue(y);else
			if(m_typeVorH == SLIDER_TYPE_HORIZONTAL) m_curValue = getMapValue(x);

			updateButtonData();
			m_dataChanged = XTrue;	//标记数值发生改变
			m_curSliderState = SLIDER_STATE_DOWN;
			if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,SLD_MOUSE_DOWN);
			else XCtrlManager.eventProc(m_objectID,SLD_MOUSE_DOWN);
			break;
		case MOUSE_LEFT_BUTTON_UP:
			if(m_curSliderState != SLIDER_STATE_DOWN) break;
			//这里会触发数值改变事件(这个弹起事件应该是在线上按下的弹起)(*这里貌似不会发生*)
			m_curSliderState = SLIDER_STATE_ON;
			if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,SLD_MOUSE_UP);
			else XCtrlManager.eventProc(m_objectID,SLD_MOUSE_UP);
			if(m_dataChanged)
			{
				stateChange();
				if(m_eventProc != NULL)
					m_eventProc(m_pClass,m_objectID,SLD_VALUE_CHANGE);
				else
					XCtrlManager.eventProc(m_objectID,SLD_VALUE_CHANGE);
				m_dataChanged = XFalse;
			}
			break;
		case MOUSE_WHEEL_UP_UP:
			if(m_curSliderState != SLIDER_STATE_ON) break;
			if(m_curValue >= m_maxValue) break;
			m_curValue += m_keyOneValue * 0.1f;
			if(m_curValue >= m_maxValue) m_curValue = m_maxValue;
			updateButtonData();
			stateChange();
			if(m_eventProc != NULL)
				m_eventProc(m_pClass,m_objectID,SLD_VALUE_CHANGE);
			else
				XCtrlManager.eventProc(m_objectID,SLD_VALUE_CHANGE);
			m_dataChanged = XFalse;
			break;
		case MOUSE_WHEEL_DOWN_UP:
			if(m_curSliderState != SLIDER_STATE_ON) break;
			if(m_curValue <= m_minValue) break;
			m_curValue -= m_keyOneValue * 0.1f;
			if(m_curValue <= m_minValue) m_curValue = m_minValue;
			updateButtonData();
			stateChange();
			if(m_eventProc != NULL)
				m_eventProc(m_pClass,m_objectID,SLD_VALUE_CHANGE);
			else
				XCtrlManager.eventProc(m_objectID,SLD_VALUE_CHANGE);
			m_dataChanged = XFalse; 
			break;
		}
	}else
	{
		if(m_curSliderState == SLIDER_STATE_ON) m_curSliderState = SLIDER_STATE_NORMAL;
	}
	return XTrue;
}
XBool XSlider::keyboardProc(int keyOrder,XKeyState keyState)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable ||
		!m_isBeChoose) return XFalse;		//如果无效则直接退出
	if(m_withAction && m_isInAction) return XFalse;	//如果支持动作播放而且正在播放动画那么不接受鼠标控制

	if(keyState == KEY_STATE_UP && m_curSliderState == SLIDER_STATE_NORMAL)
	{//按键弹起时才作相应
		//只能在普通状态下才能使用快捷键，方式重复操作
		//除了按钮的无效状态之外,其他状态都可以响应快捷键信息
		XBool flagChanged = XFalse;
		if(m_typeVorH == SLIDER_TYPE_VERTICAL)
		{//垂直
			switch(keyOrder)
			{
			case XKEY_UP:
				if(m_curValue <= m_minValue) break;
				if(m_curValue - m_keyOneValue < m_minValue) m_curValue = m_minValue;
				else m_curValue -= m_keyOneValue;
				flagChanged = XTrue;
				break;
			case XKEY_DOWN:
				if(m_curValue >= m_maxValue) break;
				if(m_curValue + m_keyOneValue > m_maxValue) m_curValue = m_maxValue;
				else m_curValue += m_keyOneValue;
				flagChanged = XTrue;
				break;
			}
		}else
		{//水平
			switch(keyOrder)
			{
			case XKEY_LEFT:
				if(m_curValue <= m_minValue) break;
				if(m_curValue - m_keyOneValue < m_minValue) m_curValue = m_minValue;
				else m_curValue -= m_keyOneValue;
				flagChanged = XTrue;
				break;
			case XKEY_RIGHT:
				if(m_curValue >= m_maxValue) break;
				if(m_curValue + m_keyOneValue > m_maxValue) m_curValue = m_maxValue;
				else m_curValue += m_keyOneValue;
				flagChanged = XTrue;
				break;
			}
		}
		if(flagChanged)
		{
			updateButtonData();
			stateChange();
			if(m_eventProc != NULL)
				m_eventProc(m_pClass,m_objectID,SLD_VALUE_CHANGE);
			else
				XCtrlManager.eventProc(m_objectID,SLD_VALUE_CHANGE);
			m_dataChanged = XFalse;
		}
	}
	return XTrue;
}
void XSlider::setCurValue(float temp,bool cbFun)
{
	if(temp < m_minValue) temp = m_minValue;
	if(temp > m_maxValue) temp = m_maxValue;
	if(temp == m_curValue) return;
	m_curValue = temp;
	updateButtonData();
	if(cbFun)
	{
		if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,SLD_VALUE_CHANGE);
		else XCtrlManager.eventProc(m_objectID,SLD_VALUE_CHANGE);
		m_dataChanged = XFalse;
	}
}
void XSlider::setRange(float max,float min)
{
	if(min >= max) return;	//这个数据是不合法的
	m_maxValue = max;
	m_minValue = min;
	m_keyOneValue = (m_maxValue - m_minValue) * 0.01f;/// 20.0f;

	if(m_curValue < m_minValue) m_curValue = m_minValue;
	if(m_curValue > m_maxValue) m_curValue = m_maxValue;
	updateButtonData();
	if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,SLD_VALUE_CHANGE);
	else XCtrlManager.eventProc(m_objectID,SLD_VALUE_CHANGE);
	m_dataChanged = XFalse;
}
XBool XSlider::setACopy(const XSlider &temp)			//建立一个副本
{
	if(&temp == this ||
		!temp.m_isInited ||
		!XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
		XObjManager.addAObject(this);
#endif
	}
	m_needFont = temp.m_needFont;
	if(m_needFont)
	{//拷贝字体
		m_caption.setACopy(temp.m_caption);	//字体
		m_fontString = temp.m_fontString;
		m_textPosition = temp.m_textPosition;		//文字的相对位置
		m_textSize = temp.m_textSize;			//文字的尺寸
		m_textColor = temp.m_textColor;		//文字的颜色
	}
	m_pVariable = temp.m_pVariable;

	m_isInited = temp.m_isInited;
	m_curSliderState = temp.m_curSliderState;			//滑动条的状态

	if(m_resInfo != NULL) XResManager.releaseResource(m_resInfo);
	m_resInfo = XResManager.copyResource(temp.m_resInfo);
	m_typeVorH = temp.m_typeVorH;						//滑动条的类型
	m_withoutTex = temp.m_withoutTex;						//滑动条的类型

	m_sliderLineNormal = temp.m_sliderLineNormal;			//滑动条线的普通状态
	m_sliderLineDown = temp.m_sliderLineDown;			//滑动条线的按下状态
	m_sliderLineOn = temp.m_sliderLineOn;				//滑动条线的悬浮状态
	m_sliderLineDisable = temp.m_sliderLineDisable;			//滑动条线的无效状态
	m_sliderButtonNormal = temp.m_sliderButtonNormal;		//滑动条按钮的普通状态
	m_sliderButtonDown = temp.m_sliderButtonDown;			//滑动条按钮的按下状态
	m_sliderButtonOn = temp.m_sliderButtonOn;			//滑动条按钮的悬浮状态
	m_sliderButtonDisable = temp.m_sliderButtonDisable;		//滑动条按钮的无效状态

	m_buttonArea = temp.m_buttonArea;		//滑动条中按钮的有效范围,这个范围是按钮的相对图片的响应范围，需要注意
//	m_curButtonArea = temp.m_curButtonArea;		//滑动条中按钮的有效范围,这个范围是按钮的相对图片的响应范围，需要注意
	m_buttonSprite.setACopy(temp.m_buttonSprite);	//用于显示贴图的精灵
	m_lineSprite.setACopy(temp.m_lineSprite);		//用于显示贴图的精灵
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_buttonSprite);
	XObjManager.decreaseAObject(&m_lineSprite);
#endif

	m_upMousePoint = temp.m_upMousePoint;		//上次鼠标在滚动条的位置
	m_mouseFlyArea = temp.m_mouseFlyArea;		//鼠标拖曳是飞出的响应范围
	m_curButtonPosition = temp.m_curButtonPosition;//当前滑块按钮的位置
	m_minValue = temp.m_minValue;			//滑动条的最大值
	m_maxValue = temp.m_maxValue;			//滑动条的最小值
	m_curValue = temp.m_curValue;			//滑动条的当前值
	m_upValue = temp.m_upValue;			//滑动条的上一次值(定义这个变量的目的在于如果拖动滑块超出范围之后可以恢复到拖动之前得知，但是目前出于简单的考虑，不做这个动作)
	m_dataChanged = temp.m_dataChanged;			//滑动条的数值是否有被修改
	m_keyOneValue = temp.m_keyOneValue;		//按下一次按键的滑动量(注意这个值比需要>=0,否则将会造成错误)

	m_actionMoveData = temp.m_actionMoveData;	//动态效果的变量
	m_oldPos = temp.m_oldPos;				//动作播放时的位置
	m_oldSize = temp.m_oldSize;			//动作播放时的大小
	m_lightMD = temp.m_lightMD;
	m_lightRect = temp.m_lightRect;

	return XTrue;
}
void XSlider::update(float stepTime)
{
	if(m_isInAction)
	{//处于动作过程中计算动作的实施
		m_actionMoveData.move(stepTime);
		if(m_actionMoveData.getIsEnd()) m_isInAction = false;	//动作播放完成
		setScale(m_actionMoveData.getCurData() * m_oldSize);
		XVector2 tmp(m_mouseRect.getWidth(),m_mouseRect.getHeight());
		tmp = tmp * (m_actionMoveData.getCurData() * m_oldSize - m_oldSize) * 0.5f;
		setPosition(m_oldPos - tmp);
	}
	if(!m_lightMD.getIsEnd())
	{
		m_lightMD.move(stepTime);
		XVector2 pos(m_oldPos.x + m_mouseRect.left * m_oldSize.x + m_mouseRect.getWidth() * m_oldSize.x * 0.5f,
			m_oldPos.y + m_mouseRect.top * m_oldSize.y + m_mouseRect.getHeight() * m_oldSize.y * 0.5f);
		XVector2 size(m_mouseRect.getWidth() * m_oldSize.x * m_lightMD.getCurData() * 0.5f,
			m_mouseRect.getHeight() * m_oldSize.y * m_lightMD.getCurData() * 0.5f);
		m_lightRect.set(pos.x - size.x,pos.y - size.y,pos.x + size.x,pos.y + size.y);
	}
}
#if !WITH_INLINE_FILE
#include "XSlider.inl"
#endif
}