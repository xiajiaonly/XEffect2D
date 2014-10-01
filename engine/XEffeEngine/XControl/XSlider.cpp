//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include"XSlider.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

_XSliderTexture::_XSliderTexture()
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
_XBool _XSliderTexture::init(const char *LNormal,const char *LDown,const char *LOn,const char *LDisable,
						  const char *BNormal,const char *BDown,const char *BOn,const char *BDisable,_XResourcePosition resoursePosition)
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
#define SLIDER_CONFIG_FILENAME ("Slider.txt")
_XBool _XSliderTexture::initEx(const char *filename,_XResourcePosition resoursePosition)
{
	if(m_isInited ||
		filename == NULL) return XFalse;
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf(tempFilename,"%s/%s",filename,SLIDER_CONFIG_FILENAME);
	//先打开配置文件
	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = XEE::defaultResourcePosition;
	if(resoursePosition == RESOURCE_LOCAL_FOLDER)
	{//外部资源
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
		if((sliderLineNormal = createATextureData(tempFilename,resoursePosition)) == NULL)
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
			if((sliderLineDown = createATextureData(tempFilename,resoursePosition)) == NULL)
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
			if((sliderLineOn = createATextureData(tempFilename,resoursePosition)) == NULL)
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
			if((sliderLineDisable = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		if((sliderButtonNormal = createATextureData(tempFilename,resoursePosition)) == NULL)
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
			if((sliderButtonDown = createATextureData(tempFilename,resoursePosition)) == NULL)
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
			if((sliderButtonOn = createATextureData(tempFilename,resoursePosition)) == NULL)
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
			if((sliderButtonDisable = createATextureData(tempFilename,resoursePosition)) == NULL)
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
	}else
	{
		unsigned char *p = _XResourcePack::GetInstance().getFileData(tempFilename);
		if(p == NULL) return XFalse;
		//下面开始依次读取数据
		int flag = 0;
		char resFilename[MAX_FILE_NAME_LENGTH] = "";
		int offset = 0;
		//normal
		if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag == 0)
		{//没有普通状态是不行的
			XDELETE_ARRAY(p);
			return XFalse;
		}
		if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((sliderLineNormal = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//资源读取失败
			XDELETE_ARRAY(p);
			return XFalse;
		}
		//down
		if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag != 0)
		{
			if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XDELETE_ARRAY(p);return XFalse;}
			offset += getCharPosition((char *)(p + offset),'\n') + 1;
			sprintf(tempFilename,"%s/%s",filename,resFilename);
			if((sliderLineDown = createATextureData(tempFilename,resoursePosition)) == NULL)
			{//资源读取失败
				releaseTex();
				XDELETE_ARRAY(p);
				return XFalse;
			}
		}
		//on
		if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag != 0)
		{
			if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XDELETE_ARRAY(p);return XFalse;}
			offset += getCharPosition((char *)(p + offset),'\n') + 1;
			sprintf(tempFilename,"%s/%s",filename,resFilename);
			if((sliderLineOn = createATextureData(tempFilename,resoursePosition)) == NULL)
			{//资源读取失败
				releaseTex();
				XDELETE_ARRAY(p);
				return XFalse;
			}
		}
		//disable
		if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag != 0)
		{
			if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XDELETE_ARRAY(p);return XFalse;}
			offset += getCharPosition((char *)(p + offset),'\n') + 1;
			sprintf(tempFilename,"%s/%s",filename,resFilename);
			if((sliderLineDisable = createATextureData(tempFilename,resoursePosition)) == NULL)
			{//资源读取失败
				releaseTex();
				XDELETE_ARRAY(p);
				return XFalse;
			}
		}
		//normal
		if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag == 0)
		{//没有普通状态是不行的
			releaseTex();
			XDELETE_ARRAY(p);
			return XFalse;
		}
		if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((sliderButtonNormal = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//资源读取失败
			releaseTex();
			XDELETE_ARRAY(p);
			return XFalse;
		}
		//
		if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag != 0)
		{
			if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XDELETE_ARRAY(p);return XFalse;}
			offset += getCharPosition((char *)(p + offset),'\n') + 1;
			sprintf(tempFilename,"%s/%s",filename,resFilename);
			if((sliderButtonDown = createATextureData(tempFilename,resoursePosition)) == NULL)
			{//资源读取失败
				releaseTex();
				XDELETE_ARRAY(p);
				return XFalse;
			}
		}
		//
		if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag != 0)
		{
			if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XDELETE_ARRAY(p);return XFalse;}
			offset += getCharPosition((char *)(p + offset),'\n') + 1;
			sprintf(tempFilename,"%s/%s",filename,resFilename);
			if((sliderButtonOn = createATextureData(tempFilename,resoursePosition)) == NULL)
			{//资源读取失败
				releaseTex();
				XDELETE_ARRAY(p);
				return XFalse;
			}
		}
		//
		if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag != 0)
		{
			if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XDELETE_ARRAY(p);return XFalse;}
			offset += getCharPosition((char *)(p + offset),'\n') + 1;
			sprintf(tempFilename,"%s/%s",filename,resFilename);
			if((sliderButtonDisable = createATextureData(tempFilename,resoursePosition)) == NULL)
			{//资源读取失败
				releaseTex();
				XDELETE_ARRAY(p);
				return XFalse;
			}
		}
		//读取两组数据
		int l,t,r,b;
		if(sscanf((char *)(p + offset),"%d,%d,%d,%d,",&l,&t,&r,&b) != 4) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		m_mouseRect.set(l,t,r,b);
		if(sscanf((char *)(p + offset),"%d,%d,%d,%d,",&l,&t,&r,&b) != 4) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		m_mouseRectButton.set(l,t,r,b);
		if(sscanf((char *)(p + offset),"%d,%d,",&l,&t) != 2) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		m_fontPosition.set(l,t);
		//所有数据读取完成
		XDELETE_ARRAY(p);
	}
	m_isInited = XTrue;
	return XTrue;
}
_XSlider::_XSlider()
:m_isInited(XFalse)
,m_nowSliderState(SLIDER_STATE_NORMAL)
,m_typeVorH(SLIDER_TYPE_VERTICAL)
,m_sliderLineNormal(NULL)			
,m_sliderLineDown(NULL)			
,m_sliderLineOn(NULL)			
,m_sliderLineDisable(NULL)		
,m_sliderButtonNormal(NULL)			
,m_sliderButtonDown(NULL)	
,m_sliderButtonOn(NULL)				
,m_sliderButtonDisable(NULL)		
,m_funInit(NULL)	
,m_funRelease(NULL)	
,m_funMouseOn(NULL)
,m_funMouseDown(NULL)
,m_funMouseUp(NULL)	
,m_funMouseMove(NULL)
,m_funValueChange(NULL)
,m_pClass(NULL)
,m_minValue(0)	
,m_maxValue(100)	
,m_nowValue(0)		
,m_mouseFlyArea(50,50)	//这个值使用一个系统设定，这个值不能在代码中进行修改，这是不理想的，但是考虑到它存在的无形性，所以默认允许这样操作
,m_needFont(XFalse)
,m_pVariable(NULL)
,m_resInfo(NULL)
,m_withoutTex(XFalse)
,m_textColor(0.0f,0.0f,0.0f,1.0f)
{
	m_ctrlType = CTRL_OBJ_SLIDER;
}
void _XSlider::release()
{
	_XCtrlManger.decreaseAObject(this);	//注销这个物件
	if(m_funRelease != NULL) m_funRelease(m_pClass,m_objectID);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(this);
#endif
	if(m_resInfo != NULL)
	{
		_XResourceManager::GetInstance().releaseResource(m_resInfo);
		m_resInfo = NULL;
	}
}
void _XSlider::setTexture(const _XSliderTexture &tex)
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
_XBool _XSlider::init(const _XVector2& position,	//控件所在的位置
		const _XRect& Area,				//控件的鼠标响应范围
		const _XRect& buttonArea,		//滑动块的鼠标响应范围
		const _XSliderTexture &tex,float max,float min,_XSliderType type)
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

	m_size.set(1.0f,1.0f);

	m_minValue = min;		
	m_maxValue = max;	
	m_nowValue = min;	
	m_upValue = min;
	m_dataChanged = XFalse;
	m_keyOneValue = (m_maxValue - m_minValue) * 0.01f;

	//线的位置不需要计算
	m_lineSprite.init(m_sliderLineNormal->texture.m_w,m_sliderLineNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_lineSprite);
#endif
	m_lineSprite.setPosition(m_position);
	m_lineSprite.setSize(m_size);
	m_lineSprite.setIsTransformCenter(POINT_LEFT_TOP);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x +  + m_mouseRect.right * m_size.x,m_position.y +  + m_mouseRect.bottom * m_size.y);

	m_buttonSprite.init(m_sliderButtonNormal->texture.m_w,m_sliderButtonNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_buttonSprite);
#endif
	m_buttonSprite.setSize(m_size);
	m_buttonSprite.setIsTransformCenter(POINT_LEFT_TOP);
	updateButtonData();

	if(m_funInit != NULL) m_funInit(m_pClass,m_objectID);
	stateChange();

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XCtrlManger.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
//int _XSlider::initWithoutTex(const _XRect& area,const _XRect& buttonArea,_XSliderType type,float max,float min,const _XVector2 &fontPosition)
_XBool _XSlider::initWithoutTex(const _XRect& area,float max,float min,_XSliderType type,const _XVector2 &fontPosition)
{
	if(m_isInited ||
		min >= max) return XFalse;
	m_mouseRect = area;
	m_typeVorH = type;
	if(m_typeVorH == SLIDER_TYPE_HORIZONTAL) m_buttonArea.set(area.top,area.top,area.bottom,area.bottom);
	else m_buttonArea.set(area.left,area.left,area.right,area.right);
	m_textPosition = fontPosition;
	m_position.set(0.0f,0.0f);

	m_withoutTex = XTrue;

	m_size.set(1.0f,1.0f);

	m_minValue = min;		
	m_maxValue = max;	
	m_nowValue = min;	
	m_upValue = min;
	m_dataChanged = XFalse;
	m_keyOneValue = (m_maxValue - m_minValue) * 0.01f;

	//线的位置不需要计算
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x +  + m_mouseRect.right * m_size.x,m_position.y +  + m_mouseRect.bottom * m_size.y);

	updateButtonData();

	if(m_funInit != NULL) m_funInit(m_pClass,m_objectID);
	stateChange();

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XCtrlManger.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XSlider::setFont(const char *caption,const _XFontUnicode &font,float captionSize,const _XVector2 &fontPosition)
{
	if(caption != NULL) strcpy(m_fontString,caption);
	else m_fontString[0] = '\0';
	m_caption.setACopy(font);
	m_caption.setColor(m_textColor * m_color);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //设置字体居中对齐
	//m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor * m_color);	
	if(captionSize < 0) return XFalse;
	m_textSize.set(captionSize,captionSize);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	char str[1024];
	sprintf(str,m_fontString,m_nowValue);
	m_caption.setString(str);
	m_textPosition = fontPosition;
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	if(m_typeVorH == SLIDER_TYPE_VERTICAL) m_caption.setAngle(90.0f);
	else m_caption.setAngle(0.0f);
	m_needFont = XTrue;
	return XTrue;
}
_XBool _XSlider::setFontEx(const char *caption,const _XFontUnicode &font,float captionSize)
{
	if(caption != NULL) strcpy(m_fontString,caption);
	else m_fontString[0] = '\0';
	m_caption.setACopy(font);
	m_caption.setColor(m_textColor * m_color);
#if WITH_OBJECT_MANAGER
_XObjManger.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //设置字体居中对齐
	//m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor * m_color);
	if(captionSize < 0) return XFalse;
	m_textSize.set(captionSize,captionSize);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	char str[1024];
	sprintf(str,m_fontString,m_nowValue);
	m_caption.setString(str);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	if(m_typeVorH == SLIDER_TYPE_VERTICAL) m_caption.setAngle(90.0f);
	else m_caption.setAngle(0.0f);
	m_needFont = XTrue;
	return XTrue;
}
void _XSlider::setPosition(float x,float y)
{
	if(!m_isInited) return;	//如果没有初始化直接退出
	m_position.set(x,y);
	updateChildPos();
	updateButtonData();
	if(!m_withoutTex) m_lineSprite.setPosition(m_position);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x +  + m_mouseRect.right * m_size.x,m_position.y +  + m_mouseRect.bottom * m_size.y);
	//还需要调用鼠标函数
	mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
	if(m_needFont)
		m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
}
void _XSlider::updateButtonData()
{
	//计算滑块的位置
	if(m_typeVorH == SLIDER_TYPE_VERTICAL)
	{//垂直类型
		m_nowButtonPosition.set(m_position.x + (m_mouseRect.left - m_buttonArea.left) * m_size.y + (m_mouseRect.getWidth() - m_buttonArea.getWidth()) * m_size.x * 0.5f,
			m_position.y + (m_mouseRect.top - m_buttonArea.top) * m_size.y + 
			(m_mouseRect.getHeight() - m_buttonArea.getHeight()) * m_size.y * (m_nowValue - m_minValue)/(m_maxValue - m_minValue));
	}else
	if(m_typeVorH == SLIDER_TYPE_HORIZONTAL)
	{//水平类型
		m_nowButtonPosition.set(m_position.x + (m_mouseRect.left - m_buttonArea.left) * m_size.y + 
			(m_mouseRect.getWidth() - m_buttonArea.getWidth()) * m_size.x * (m_nowValue - m_minValue)/(m_maxValue - m_minValue),
			m_position.y + (m_mouseRect.top - m_buttonArea.top) * m_size.y + (m_mouseRect.getHeight() - m_buttonArea.getHeight()) * m_size.y * 0.5f);
	}
	if(!m_withoutTex) m_buttonSprite.setPosition(m_nowButtonPosition);
//	m_nowButtonArea.set(m_nowButtonPosition.x + m_buttonArea.left * m_size.x,m_nowButtonPosition.y + m_buttonArea.top * m_size.y,
//		m_nowButtonPosition.x + m_buttonArea.right * m_size.x,m_nowButtonPosition.y + m_buttonArea.bottom * m_size.y);
	if(m_needFont)
	{
		char str[1024];
		sprintf(str,m_fontString,m_nowValue);
		m_caption.setString(str);
	}
	if(m_pVariable != NULL) * m_pVariable = m_nowValue;	//需要检查这行在这里的合理性
}
void _XSlider::setSize(float x,float y)
{
	if(x <= 0 || y <= 0 ||
		!m_isInited) return;	//如果没有初始化直接退出
	m_size.set(x,y);
	updateChildSize();
	if(!m_withoutTex)
	{
		m_lineSprite.setSize(m_size);
		m_buttonSprite.setSize(m_size);
	}
	updateButtonData();

	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x +  + m_mouseRect.right * m_size.x,m_position.y +  + m_mouseRect.bottom * m_size.y);
	//还需要调用鼠标函数
	mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
	if(m_needFont)
	{
		m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
		m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	}
}
void _XSlider::draw()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出

	if(m_withoutTex)
	{
		if(m_nowSliderState == SLIDER_STATE_NORMAL)
		{
			drawFillBoxExA(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),
				_XVector2(m_mouseRect.getWidth() * m_size.x,m_mouseRect.getHeight() * m_size.y),
				0.5f * m_color.fR,0.5f * m_color.fG,0.5f * m_color.fB,m_color.fA,true);
			drawFillBoxExA(m_nowButtonPosition + _XVector2(m_buttonArea.left * m_size.x,m_buttonArea.top * m_size.y),
				_XVector2(m_buttonArea.getWidth() * m_size.x,m_buttonArea.getHeight() * m_size.y),
				0.75f * m_color.fR,0.75f * m_color.fG,0.75f * m_color.fB,m_color.fA,true);
		//	m_lineSprite.draw(m_sliderLineNormal);
		//	m_buttonSprite.draw(m_sliderButtonNormal);
		}else
		if(m_nowSliderState == SLIDER_STATE_DOWN)
		{
			drawFillBoxExA(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),
				_XVector2(m_mouseRect.getWidth() * m_size.x,m_mouseRect.getHeight() * m_size.y),
				0.4f * m_color.fR,0.4f * m_color.fG,0.4f * m_color.fB,m_color.fA,true);
			drawFillBoxExA(m_nowButtonPosition + _XVector2(m_buttonArea.left * m_size.x,m_buttonArea.top * m_size.y),
				_XVector2(m_buttonArea.getWidth() * m_size.x,m_buttonArea.getHeight() * m_size.y),
				0.65f * m_color.fR,0.65f * m_color.fG,0.65f * m_color.fB,m_color.fA,true);
			//m_lineSprite.draw(m_sliderLineDown);
			//m_buttonSprite.draw(m_sliderButtonDown);
		}else
		if(m_nowSliderState == SLIDER_STATE_ON)
		{
			drawFillBoxExA(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),
				_XVector2(m_mouseRect.getWidth() * m_size.x,m_mouseRect.getHeight() * m_size.y),
				0.6f * m_color.fR,0.6f * m_color.fG,0.6f * m_color.fB,m_color.fA,true);
			drawFillBoxExA(m_nowButtonPosition + _XVector2(m_buttonArea.left * m_size.x,m_buttonArea.top * m_size.y),
				_XVector2(m_buttonArea.getWidth() * m_size.x,m_buttonArea.getHeight() * m_size.y),
				0.85f * m_color.fR,0.85f * m_color.fG,0.85f * m_color.fB,m_color.fA,true);
			//m_lineSprite.draw(m_sliderLineOn);
			//m_buttonSprite.draw(m_sliderButtonOn);
		}else
		if(m_nowSliderState == SLIDER_STATE_DISABLE)
		{
			drawFillBoxExA(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),
				_XVector2(m_mouseRect.getWidth() * m_size.x,m_mouseRect.getHeight() * m_size.y),
				0.25f * m_color.fR,0.25f * m_color.fG,0.25f * m_color.fB,m_color.fA,true);
			drawFillBoxExA(m_nowButtonPosition + _XVector2(m_buttonArea.left * m_size.x,m_buttonArea.top * m_size.y),
				_XVector2(m_buttonArea.getWidth() * m_size.x,m_buttonArea.getHeight() * m_size.y),
				0.5f * m_color.fR,0.5f * m_color.fG,0.5f * m_color.fB,m_color.fA,true);
			//m_lineSprite.draw(m_sliderLineDisable);
			//m_buttonSprite.draw(m_sliderButtonDisable);
		}	
	}else
	{
		if(m_nowSliderState == SLIDER_STATE_NORMAL)
		{
			m_lineSprite.draw(m_sliderLineNormal);
			m_buttonSprite.draw(m_sliderButtonNormal);
		}else
		if(m_nowSliderState == SLIDER_STATE_DOWN)
		{
			m_lineSprite.draw(m_sliderLineDown);
			m_buttonSprite.draw(m_sliderButtonDown);
		}else
		if(m_nowSliderState == SLIDER_STATE_ON)
		{
			m_lineSprite.draw(m_sliderLineOn);
			m_buttonSprite.draw(m_sliderButtonOn);
		}else
		if(m_nowSliderState == SLIDER_STATE_DISABLE)
		{
			m_lineSprite.draw(m_sliderLineDisable);
			m_buttonSprite.draw(m_sliderButtonDisable);
		}
	}
	if(m_needFont) m_caption.draw();
}
_XBool _XSlider::mouseProc(float x,float y,_XMouseState mouseState)
{
	m_upMousePoint.set(x,y);
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	if(m_withAction && m_isInAction) return XFalse;	//如果支持动作播放而且正在播放动画那么不接受鼠标控制
	//SliderLine,滚动条的线的事件

	if(m_nowSliderState == SLIDER_STATE_DOWN)
	{//注意这里是滑动条拖动时，为了是超出范围的拖动仍然有效，所以这个这个特殊处理
		_XRect tempFly(m_nowMouseRect.left - m_mouseFlyArea.x,m_nowMouseRect.top - m_mouseFlyArea.y,
			m_nowMouseRect.right + m_mouseFlyArea.x,m_nowMouseRect.bottom + m_mouseFlyArea.y);
		if(tempFly.isInRect(x,y))
		{//拖动(注意如果这里拖动超出范围，则使用最后一次的有效值)
			if(mouseState == MOUSE_MOVE)
			{//重新计算Slider的当前值
				if(m_typeVorH == SLIDER_TYPE_VERTICAL)
				{//垂直类型
					m_nowValue = m_minValue + (m_maxValue - m_minValue) * (y - m_position.y - m_mouseRect.top * m_size.y) / (m_mouseRect.getHeight() * m_size.y); 
					if(m_nowValue < m_minValue) m_nowValue = m_minValue;
					if(m_nowValue > m_maxValue) m_nowValue = m_maxValue;
				}else
				if(m_typeVorH == SLIDER_TYPE_HORIZONTAL)
				{//水平类型
					m_nowValue = m_minValue + (m_maxValue - m_minValue) * (x - m_position.x - m_mouseRect.left * m_size.x) / (m_mouseRect.getWidth() * m_size.x); 
					if(m_nowValue < m_minValue) m_nowValue = m_minValue;
					if(m_nowValue > m_maxValue) m_nowValue = m_maxValue;
				}
				updateButtonData();
				m_dataChanged = XTrue;	//标记数值发生改变
				if(m_funMouseMove != NULL) m_funMouseMove(m_pClass,m_objectID);
			}else
			if(mouseState == MOUSE_LEFT_BUTTON_UP)
			{//这个弹起事件是在按钮上按下的弹起事件
				m_nowSliderState = SLIDER_STATE_ON;
				if(m_funMouseUp != NULL) m_funMouseUp(m_pClass,m_objectID);
				if(m_dataChanged) stateChange();
				if(m_funValueChange != NULL && m_dataChanged)
				{
					m_dataChanged = XFalse;
					m_funValueChange(m_pClass,m_objectID);
				}
			}
			m_isBeChoose = XTrue;
		}else
		{
			m_nowSliderState = SLIDER_STATE_NORMAL;
			if(m_dataChanged) stateChange();
			if(m_funValueChange != NULL && m_dataChanged)
			{
				m_dataChanged = XFalse;
				m_funValueChange(m_pClass,m_objectID);
			}
		}
	}
	if(m_nowMouseRect.isInRect(x,y))
	{//鼠标动作在范围内
		if(mouseState == MOUSE_MOVE)
		{
			if(m_nowSliderState == SLIDER_STATE_NORMAL)
			{//悬浮
				m_nowSliderState = SLIDER_STATE_ON;
				if(m_funMouseOn != NULL) m_funMouseOn(m_pClass,m_objectID);
				if(m_withAction)
				{//这里测试一个动态效果
					m_isInAction = XTrue;
					m_actionMoveData.set(1.0f,1.1f,0.02f,MOVE_DATA_MODE_SIN_MULT,1);
					m_lightMD.set(1.0f,2.0f,0.002f,MOVE_DATA_MODE_SIN_MULT);
					m_oldPos = m_position;
					m_oldSize = m_size;
				}
			}
		}else
		if((mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_LEFT_BUTTON_DCLICK) && (m_nowSliderState == SLIDER_STATE_NORMAL || m_nowSliderState == SLIDER_STATE_ON))
		{
			m_upValue = m_nowValue;

			//重新计算Slider的当前值
			if(m_typeVorH == SLIDER_TYPE_VERTICAL)
			{//垂直类型
				m_nowValue = m_minValue + (m_maxValue - m_minValue) * (y - m_position.y - m_mouseRect.top * m_size.y) / (m_mouseRect.getHeight() * m_size.y); 
				if(m_nowValue < m_minValue) m_nowValue = m_minValue;
				if(m_nowValue > m_maxValue) m_nowValue = m_maxValue;
			}else
			if(m_typeVorH == SLIDER_TYPE_HORIZONTAL)
			{//水平类型
				m_nowValue = m_minValue + (m_maxValue - m_minValue) * (x - m_position.x - m_mouseRect.left * m_size.x) / (m_mouseRect.getWidth() * m_size.x); 
				if(m_nowValue < m_minValue) m_nowValue = m_minValue;
				if(m_nowValue > m_maxValue) m_nowValue = m_maxValue;
			}
			updateButtonData();
			m_dataChanged = XTrue;	//标记数值发生改变
			m_nowSliderState = SLIDER_STATE_DOWN;
			if(m_funMouseDown != NULL) m_funMouseDown(m_pClass,m_objectID);
		}else
		if(mouseState == MOUSE_LEFT_BUTTON_UP && m_nowSliderState == SLIDER_STATE_DOWN)
		{//这里会触发数值改变事件(这个弹起事件应该是在线上按下的弹起)(*这里貌似不会发生*)
			m_nowSliderState = SLIDER_STATE_ON;
			if(m_funMouseUp != NULL) m_funMouseUp(m_pClass,m_objectID);
			if(m_dataChanged) stateChange();
			if(m_funValueChange != NULL && m_dataChanged)
			{
				m_dataChanged = XFalse;
				m_funValueChange(m_pClass,m_objectID);
			}
		}
	}else
	{
		if(m_nowSliderState == SLIDER_STATE_ON) m_nowSliderState = SLIDER_STATE_NORMAL;
	}
	return XTrue;
}
_XBool _XSlider::keyboardProc(int keyOrder,_XKeyState keyState)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable ||
		!m_isBeChoose) return XFalse;		//如果无效则直接退出
	if(m_withAction && m_isInAction) return XFalse;	//如果支持动作播放而且正在播放动画那么不接受鼠标控制

	if(keyState == KEY_STATE_UP)
	{//按键弹起时才作相应
		if(m_nowSliderState == SLIDER_STATE_NORMAL)	//只能在普通状态下才能使用快捷键，方式重复操作
		{//除了按钮的无效状态之外,其他状态都可以响应快捷键信息
			_XBool flagChanged = XFalse;
			if(m_typeVorH == SLIDER_TYPE_VERTICAL)
			{//垂直
				if(keyOrder ==  XKEY_UP && m_nowValue > m_minValue)
				{
					if(m_nowValue - m_keyOneValue < m_minValue) m_nowValue = m_minValue;
					else m_nowValue -= m_keyOneValue;
					flagChanged = XTrue;
				}else
				if(keyOrder ==  XKEY_DOWN && m_nowValue < m_maxValue)
				{
					if(m_nowValue + m_keyOneValue > m_maxValue) m_nowValue = m_maxValue;
					else m_nowValue += m_keyOneValue;
					flagChanged = XTrue;
				}
			}else
			{//水平
				if(keyOrder ==  XKEY_LEFT && m_nowValue > m_minValue)
				{
					if(m_nowValue - m_keyOneValue < m_minValue) m_nowValue = m_minValue;
					else m_nowValue -= m_keyOneValue;
					flagChanged = XTrue;
				}else
				if(keyOrder ==  XKEY_RIGHT && m_nowValue < m_maxValue)
				{
					if(m_nowValue + m_keyOneValue > m_maxValue) m_nowValue = m_maxValue;
					else m_nowValue += m_keyOneValue;
					flagChanged = XTrue;
				}
			}
			if(flagChanged)
			{
				updateButtonData();
				stateChange();
				if(m_funValueChange != NULL)
				{
					m_dataChanged = XTrue;
					m_funValueChange(m_pClass,m_objectID);
				}
			}
		}
	}
	return XTrue;
}
void _XSlider::setCallbackFun(void (* funInit)(void *,int),
							  void (* funRelease)(void *,int),
							  void (* funMouseOn)(void *,int),
							  void (* funMouseDown)(void *,int),
							  void (* funMouseUp)(void *,int),
							  void (*funValueChange)(void *,int),
							  void (*funMouseMove)(void *,int),
							  void *pClass)
{
	m_funInit = funInit;
	m_funRelease = funRelease;
	m_funMouseOn = funMouseOn;
	m_funMouseDown = funMouseDown;
	m_funMouseUp = funMouseUp;
	m_funValueChange = funValueChange;
	m_funMouseMove = funMouseMove;
	if(pClass != NULL) m_pClass = pClass;
	else m_pClass = this;
}
void _XSlider::setNowValue(float temp,bool cbFun)
{
	if(temp < m_minValue) temp = m_minValue;
	if(temp > m_maxValue) temp = m_maxValue;
	if(temp == m_nowValue) return;
	m_nowValue = temp;
	updateButtonData();
	if(m_funValueChange != NULL && cbFun)
	{
		m_dataChanged = XFalse;
		m_funValueChange(m_pClass,m_objectID);
	}
}
void _XSlider::setRange(float max,float min)
{
	if(min >= max) return;	//这个数据是不合法的
	m_maxValue = max;
	m_minValue = min;
	m_keyOneValue = (m_maxValue - m_minValue) * 0.01f;/// 20.0f;

	if(m_nowValue < m_minValue) m_nowValue = m_minValue;
	if(m_nowValue > m_maxValue) m_nowValue = m_maxValue;
	updateButtonData();
	if(m_funValueChange != NULL)
	{
		m_dataChanged = XFalse;
		m_funValueChange(m_pClass,m_objectID);
	}
}
_XBool _XSlider::setACopy(const _XSlider &temp)			//建立一个副本
{
	if(&temp == this ||
		!temp.m_isInited ||
		!_XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		_XCtrlManger.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
		_XObjManger.addAObject(this);
#endif
	}
	m_needFont = temp.m_needFont;
	if(m_needFont)
	{//拷贝字体
		m_caption.setACopy(temp.m_caption);	//字体
		strcpy(m_fontString,temp.m_fontString);
		m_textPosition = temp.m_textPosition;		//文字的相对位置
		m_textSize = temp.m_textSize;			//文字的尺寸
		m_textColor = temp.m_textColor;		//文字的颜色
	}
	m_pVariable = temp.m_pVariable;

	m_isInited = temp.m_isInited;
	m_nowSliderState = temp.m_nowSliderState;			//滑动条的状态

	if(m_resInfo != NULL) _XResourceManager::GetInstance().releaseResource(m_resInfo);
	m_resInfo = _XResourceMng.copyResource(temp.m_resInfo);
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

	m_funInit = temp.m_funInit;		//初始化
	m_funRelease = temp.m_funRelease;		//资源释放
	m_funMouseOn = temp.m_funMouseOn;		//鼠标悬浮
	m_funMouseDown = temp.m_funMouseDown;	//鼠标按下
	m_funMouseUp = temp.m_funMouseUp;		//鼠标弹起
	m_funMouseMove = temp.m_funMouseMove;	//鼠标拖动花条时调用
	m_funValueChange = temp.m_funValueChange;	//滑动条的数值变化
	m_pClass = temp.m_pClass;				//回调函数的参数

	m_buttonArea = temp.m_buttonArea;		//滑动条中按钮的有效范围,这个范围是按钮的相对图片的响应范围，需要注意
//	m_nowButtonArea = temp.m_nowButtonArea;		//滑动条中按钮的有效范围,这个范围是按钮的相对图片的响应范围，需要注意
	m_buttonSprite.setACopy(temp.m_buttonSprite);	//用于显示贴图的精灵
	m_lineSprite.setACopy(temp.m_lineSprite);		//用于显示贴图的精灵
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_buttonSprite);
	_XObjManger.decreaseAObject(&m_lineSprite);
#endif

	m_upMousePoint = temp.m_upMousePoint;		//上次鼠标在滚动条的位置
	m_mouseFlyArea = temp.m_mouseFlyArea;		//鼠标拖曳是飞出的响应范围
	m_nowButtonPosition = temp.m_nowButtonPosition;//当前滑块按钮的位置
	m_minValue = temp.m_minValue;			//滑动条的最大值
	m_maxValue = temp.m_maxValue;			//滑动条的最小值
	m_nowValue = temp.m_nowValue;			//滑动条的当前值
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
void _XSlider::update(int stepTime)
{
	if(m_isInAction)
	{//处于动作过程中计算动作的实施
		m_actionMoveData.move(stepTime);
		if(m_actionMoveData.getIsEnd()) m_isInAction = false;	//动作播放完成
		setSize(m_actionMoveData.getNowData() * m_oldSize);
		_XVector2 tmp(m_mouseRect.getWidth(),m_mouseRect.getHeight());
		tmp = tmp * (m_actionMoveData.getNowData() * m_oldSize - m_oldSize) * 0.5f;
		setPosition(m_oldPos - tmp);
	}
	if(!m_lightMD.getIsEnd())
	{
		m_lightMD.move(stepTime);
		_XVector2 pos(m_oldPos.x + m_mouseRect.left * m_oldSize.x + m_mouseRect.getWidth() * m_oldSize.x * 0.5f,
			m_oldPos.y + m_mouseRect.top * m_oldSize.y + m_mouseRect.getHeight() * m_oldSize.y * 0.5f);
		_XVector2 size(m_mouseRect.getWidth() * m_oldSize.x * m_lightMD.getNowData() * 0.5f,
			m_mouseRect.getHeight() * m_oldSize.y * m_lightMD.getNowData() * 0.5f);
		m_lightRect.set(pos.x - size.x,pos.y - size.y,pos.x + size.x,pos.y + size.y);
	}
}