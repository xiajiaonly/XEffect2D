#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XButton.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "XResourcePack.h"
namespace XE{
XButtonSkin::XButtonSkin()
:m_isInited(XFalse)
,buttonNormal(NULL)			//普通状态下的按钮贴图
,buttonDown(NULL)			//按下状态下的按钮贴图
,buttonOn(NULL)				//悬浮状态下的按钮贴图
,buttonDisable(NULL)		//无效状态下的按钮贴图
,m_areaPointSum(0)
,m_pArea(NULL)
{}
XBool XButtonSkin::init(const char *normal,const char *down,const char *on,const char *disable,XResPos resPos)
{
	if(m_isInited) return XFalse;
	if(normal == NULL) return XFalse;
	int ret = 1;

	if(normal != NULL && 
		(buttonNormal = createATextureData(normal,resPos)) == NULL) ret = 0;
	if(down != NULL && ret != 0 && 
		(buttonDown = createATextureData(down,resPos)) == NULL) ret = 0;
	if(on != NULL && ret != 0 && 
		(buttonOn = createATextureData(on,resPos)) == NULL) ret = 0;
	if(disable != NULL && ret != 0 && 
		(buttonDisable = createATextureData(disable,resPos)) == NULL) ret = 0;
	
	if(ret == 0)
	{
		releaseTex();
		return XFalse;
	}
	m_isInited = XTrue;
	return XTrue;
}
#define BUTTON_CONFIG_FILENAME "Button.txt"
bool XButtonSkin::loadFromFolder(const char *filename,XResPos resPos)	//从文件夹中载入资源
{
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,BUTTON_CONFIG_FILENAME);
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
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
	if((buttonNormal = createATextureData(tempFilename,resPos)) == NULL)
	{//资源读取失败
		fclose(fp);
		return XFalse;
	}
	//down
	if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
	if(flag == 0)
	{//没有普通状态是不行的
		releaseTex();
		fclose(fp);
		return XFalse;
	}
	if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
	if((buttonDown = createATextureData(tempFilename,resPos)) == NULL)
	{//资源读取失败
		releaseTex();
		fclose(fp);
		return XFalse;
	}
	//on
	if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
	if(flag == 0)
	{//没有普通状态是不行的
		releaseTex();
		fclose(fp);
		return XFalse;
	}
	if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
	if((buttonOn = createATextureData(tempFilename,resPos)) == NULL)
	{//资源读取失败
		releaseTex();
		fclose(fp);
		return XFalse;
	}
	//disable
	if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
	if(flag == 0)
	{//没有普通状态是不行的
		releaseTex();
		fclose(fp);
		return XFalse;
	}
	if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
	if((buttonDisable = createATextureData(tempFilename,resPos)) == NULL)
	{//资源读取失败
		releaseTex();
		fclose(fp);
		return XFalse;
	}
	//读取两组数据
	int l,t,r,b,x,y;
	if(fscanf(fp,"%d,%d,%d,%d,",&l,&t,&r,&b) != 4) {fclose(fp);return XFalse;}
	m_mouseRect.set(l,t,r,b);
	if(fscanf(fp,"%d,%d,",&x,&y) != 2) {fclose(fp);return XFalse;}
	m_fontPosition.set(x,y);
	//下面读取多边形信息
	if(fscanf(fp,"%d,",&m_areaPointSum) != 1) {fclose(fp);return XFalse;}
	if(m_areaPointSum > 0)
	{
		m_pArea = XMem::createArrayMem<XVec2>(m_areaPointSum);	//rational:MLK
		if(m_pArea == NULL)
		{
			releaseTex();
			fclose(fp);
			return XFalse;
		}
		//下面读取点整的数据
		for(int i = 0;i < m_areaPointSum;++ i)
		{
			if(fscanf(fp,"%d,%d,",&x,&y) != 2) {fclose(fp);return XFalse;}
			m_pArea[i].set(x,y);
		}
	}
	//所有数据读取完成
	fclose(fp);
	return true;
}
bool XButtonSkin::loadFromPacker(const char *filename,XResPos resPos)	//从压缩包中载入资源
{
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,BUTTON_CONFIG_FILENAME);
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
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
	if((buttonNormal = createATextureData(tempFilename,resPos)) == NULL)
	{//资源读取失败
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	//down
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
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
	if((buttonDown = createATextureData(tempFilename,resPos)) == NULL)
	{//资源读取失败
		releaseTex();
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	//on
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
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
	if((buttonOn = createATextureData(tempFilename,resPos)) == NULL)
	{//资源读取失败
		releaseTex();
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	//disable
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
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
	if((buttonDisable = createATextureData(tempFilename,resPos)) == NULL)
	{//资源读取失败
		releaseTex();
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	//读取两组数据
	int l,t,r,b,x,y;
	if(sscanf((char *)(p + offset),"%d,%d,%d,%d,",&l,&t,&r,&b) != 4) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
	m_mouseRect.set(l,t,r,b);
	if(sscanf((char *)(p + offset),"%d,%d,",&x,&y) != 2) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
	m_fontPosition.set(x,y);
	//下面读取多边形信息
	if(sscanf((char *)(p + offset),"%d,",&m_areaPointSum) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
	if(m_areaPointSum > 0)
	{
		m_pArea = XMem::createArrayMem<XVec2>(m_areaPointSum);
		if(m_pArea == NULL)
		{
			releaseTex();
			XMem::XDELETE_ARRAY(p);
			return XFalse;
		}
		//下面读取点整的数据
		for(int i = 0;i < m_areaPointSum;++ i)
		{
			if(sscanf((char *)(p + offset),"%d,%d,",&x,&y) != 2) {XMem::XDELETE_ARRAY(p);return XFalse;}
			offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
			m_pArea[i].set(x,y);
		}
	}
	//所有数据读取完成
	XMem::XDELETE_ARRAY(p);
	return true;
}
bool XButtonSkin::loadFromWeb(const char *filename,XResPos resPos)		//从网页中读取资源
{
	return false;
}

XBool XButtonSkin::initEx(const char *filename,XResPos resPos)
{
	if(m_isInited) return XFalse;
	if(filename == NULL) return XFalse;
	//先打开配置文件
	if(resPos == RES_SYS_DEF) resPos = getDefResPos();
	switch(resPos)
	{
	case RES_LOCAL_PACK:
		if(!loadFromPacker(filename,resPos)) return false;
		break;
	case RES_LOCAL_FOLDER:
		if(!loadFromFolder(filename,resPos)) return false;
		break;
	case RES_WEB:
		if(!loadFromWeb(filename,resPos)) return false;
		break;
	case RES_AUTO:
		if(!loadFromPacker(filename,resPos) && !loadFromFolder(filename,resPos) &&
			!loadFromWeb(filename,resPos)) return false;
		break;
	}
	m_isInited = XTrue;
	return XTrue;
}

XButton::XButton()
	:m_isInited(XFalse)
	, m_buttonNormal(NULL)
	, m_buttonDown(NULL)
	, m_buttonOn(NULL)
	, m_buttonDisable(NULL)
	, m_hotKey(-1)
	, m_curButtonState(BUTTON_STATE_NORMAL)
	, m_upMousePoint(-1.0f)
	, m_resInfo(NULL)
	, m_withoutTex(XFalse)
	, m_withNormalIcon(XFalse)
	, m_withDisableIcon(XFalse)
	, m_iconSize(1.0f)
	, m_iconPosition(0.0f)
	, m_symbolType(BTN_SYMBOL_NULL)
	, m_buttonStyle(BTN_STYLE_NORMAL)
	, m_isCheck(false)
{
	m_ctrlType = CTRL_OBJ_BUTTON;
}
void XButton::release()
{
	if (!m_isInited) return;
	XCtrlManager.decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	if(m_resInfo != NULL)
	{
		XResManager.releaseResource(m_resInfo);
		m_resInfo = NULL;
	}
	m_isInited = false;
}
void XButton::setTexture(const XButtonSkin& tex)
{
	if(tex.buttonNormal == NULL) return;//按键的普通状态的贴图不能为空，否则直接返回错误
	m_buttonNormal = tex.buttonNormal;
	if(tex.buttonDown != NULL) m_buttonDown = tex.buttonDown;
	else m_buttonDown = tex.buttonNormal;
	
	if(tex.buttonOn != NULL) m_buttonOn = tex.buttonOn;
	else m_buttonOn = tex.buttonNormal;
	
	if(tex.buttonDisable != NULL) m_buttonDisable = tex.buttonDisable;
	else m_buttonDisable = tex.buttonNormal;
}
XBool XButton::initProc(const XFontUnicode& font,const char *caption,float captionSize)
{
	if(!m_caption.setACopy(font)) return XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_MIDDLE); //设置字体居中对齐
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //设置字体居中对齐
	m_textColor.set(0.0f,1.0f);
	if(m_curButtonState == BUTTON_STATE_DISABLE) m_caption.setColor((m_textColor * m_color).anti());
	else m_caption.setColor(m_textColor * m_color);							//设置字体的颜色为黑色

	m_scale.set(1.0f);
	//这里距中对齐，所以这里的位置需要计算
	m_caption.setString(caption);
	m_caption.setPosition(m_position + m_textPosition * m_scale);
	m_textSize.set(captionSize);
	m_caption.setScale(m_textSize * m_scale);

	m_curMouseRect.set(m_position + m_mouseRect.getLT() * m_scale,
		m_position + m_mouseRect.getRB() * m_scale);

	m_curButtonState = BUTTON_STATE_NORMAL;

	if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,BTN_INIT);
	else XCtrlManager.eventProc(m_objectID,BTN_INIT);

	m_isVisible = m_isEnable = m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	return XTrue;
}
XBool XButton::init(const XVec2& position,	//控件所在的位置
		const XRect& Area,				//控件鼠标响应的区间
		const XButtonSkin &tex,		//控件的贴图信息
		const char *caption,const XFontUnicode& font,float captionSize,const XVec2& textPosition)	//控件标题的相关信息
{
	if(m_isInited) return XTrue;	//如果已经初始化了则防止重复初始化
	if(captionSize <= 0) return XFalse;
	if(Area.getHeight() <= 0 || Area.getWidth() <= 0) return XFalse;	//按钮的控件必须要有一个鼠标响应区域
	m_comment.init();
	m_mouseRect = Area;
	m_position = position;
	m_textPosition = textPosition;
	if(tex.buttonNormal == NULL) return XFalse;//按键的普通状态的贴图不能为空，否则直接返回错误
	m_buttonNormal = tex.buttonNormal;
	if(tex.buttonDown != NULL) m_buttonDown = tex.buttonDown;
	else m_buttonDown = tex.buttonNormal;
	if(tex.buttonOn != NULL) m_buttonOn = tex.buttonOn;
	else m_buttonOn = tex.buttonNormal;
	if(tex.buttonDisable != NULL) m_buttonDisable = tex.buttonDisable;
	else m_buttonDisable = tex.buttonNormal;
	m_withoutTex = XFalse;

	if(!initProc(font,caption,captionSize)) return XFalse;						//设置字体的颜色为黑色

	m_sprite.init(m_buttonNormal->texture.m_w,m_buttonNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_sprite);
#endif
	m_sprite.setPosition(m_position);
	m_sprite.setScale(m_scale);
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);

	//这里距中对齐，所以这里的位置需要计算
	m_isInited = XTrue;
	return XTrue;
}
XBool XButton::initWithoutSkin(const char *caption,const XFontUnicode& font,
		float captionSize,const XRect& area,const XVec2& textPosition)	//没有贴图的形式,直接用写屏绘图函数绘图(尚未实现，未完成工作之一)
{
	if(m_isInited) return XTrue;	//如果已经初始化了则防止重复初始化
	if(captionSize <= 0 ||
		area.getHeight() <= 0 || area.getWidth() <= 0) return XFalse;	//按钮的控件必须要有一个鼠标响应区域
	m_comment.init();
	m_mouseRect = area;
	m_position.reset();
	m_textPosition = textPosition;
	m_withoutTex = XTrue;

	if(!initProc(font,caption,captionSize)) return XFalse;						//设置字体的颜色为黑色

	m_isInited = XTrue;
	return XTrue;
}
void XButton::setPosition(const XVec2& p)
{
	if(!m_isInited) return;
	m_position = p;
	XVec2 tmpPos = m_position;
	XVec2 tmpSize = m_scale;
	if(m_isInAction && !m_actionMoveData.getIsEnd())
	{
		tmpPos = m_mouseRect.getSize();
		tmpPos = tmpPos * (m_actionMoveData.getCurData() * m_scale - m_scale) * 0.5f;
		tmpPos = m_position - tmpPos;
		tmpSize = m_actionMoveData.getCurData() * m_scale;
	}
	m_curMouseRect.set(tmpPos + m_mouseRect.getLT() * tmpSize,
		tmpPos + m_mouseRect.getRB() * tmpSize);
	m_caption.setPosition(tmpPos + m_textPosition * tmpSize);
	if(!m_withoutTex) m_sprite.setPosition(tmpPos);

	if(m_withNormalIcon) m_normalIcon.setPosition(tmpPos + m_iconPosition * tmpSize);
	if(m_withDisableIcon) m_normalIcon.setPosition(tmpPos + m_iconPosition * tmpSize);

	mouseProc(m_upMousePoint,MOUSE_MOVE);
	updateChildPos();
}
void XButton::setScale(const XVec2& s)
{
	if(!m_isInited ||
		s.x <= 0 || s.y <= 0) return;
	m_scale = s;
	XVec2 tmpPos = m_position;
	XVec2 tmpSize = m_scale;
	if(m_isInAction && !m_actionMoveData.getIsEnd())
	{
		tmpPos = m_mouseRect.getSize();
		tmpPos = tmpPos * (m_actionMoveData.getCurData() * m_scale - m_scale) * 0.5f;
		tmpPos = m_position - tmpPos;
		tmpSize = m_actionMoveData.getCurData() * m_scale;
	}
	m_curMouseRect.set(tmpPos + m_mouseRect.getLT() * tmpSize,
		tmpPos + m_mouseRect.getRB() * tmpSize);
	m_caption.setPosition(tmpPos + m_textPosition * tmpSize);
	m_caption.setScale(m_textSize * tmpSize);
	if(!m_withoutTex) m_sprite.setScale(tmpSize);

	if (m_withNormalIcon)
	{
		m_normalIcon.setPosition(tmpPos + m_iconPosition * tmpSize);
		m_normalIcon.setScale(m_iconSize * tmpSize);
	}
	if (m_withDisableIcon)
	{
		m_normalIcon.setPosition(tmpPos + m_iconPosition * tmpSize);
		m_disableIcon.setScale(m_iconSize * tmpSize);
	}

	mouseProc(m_upMousePoint,MOUSE_MOVE);
	updateChildScale();
}
XBool XButton::mouseProc(const XVec2& p,XMouseState mouseState)
{
	m_upMousePoint = p;
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible) return XFalse; 	//如果不可见直接退出
	if(m_withAction && m_isInAction) return XTrue;	//如果支持动作播放而且正在播放动画那么不接受鼠标控制
	if(m_isSilent) return XFalse;
	
	if(m_curMouseRect.isInRect(p))
	{//鼠标动作在范围内
		if(!m_isMouseInRect)
		{
			m_isMouseInRect = XTrue;
			m_comment.setShow();
		}
		m_comment.updatePosition(p + XVec2(0.0f, 16.0f));
		if(mouseState != MOUSE_MOVE && m_comment.getIsShow())
			m_comment.disShow();	//鼠标的任意操作都会让说明框消失
		if(m_isEnable)
		{
			if(mouseState == MOUSE_MOVE && m_curButtonState == BUTTON_STATE_NORMAL)
			{
				m_curButtonState = BUTTON_STATE_ON;

				if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,BTN_MOUSE_ON);
				else XCtrlManager.eventProc(m_objectID,BTN_MOUSE_ON);

				if(m_withAction)
				{//这里测试一个动态效果
					m_isInAction = XTrue;
					m_actionMoveData.set(1.0f,1.1f,0.02f,MD_MODE_SIN_MULT,1);
					m_lightMD.set(1.0f,2.0f,0.002f,MD_MODE_SIN_MULT);
					m_oldPos = m_position;
					m_oldSize = m_scale;
				}
			}else
			if((mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_LEFT_BUTTON_DCLICK) && 
				(m_curButtonState == BUTTON_STATE_NORMAL || m_curButtonState == BUTTON_STATE_ON))
			{
				m_curButtonState = BUTTON_STATE_DOWN;
				if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,BTN_MOUSE_DOWN);
				else XCtrlManager.eventProc(m_objectID,BTN_MOUSE_DOWN);
			}else
			if(mouseState == MOUSE_LEFT_BUTTON_UP && m_curButtonState == BUTTON_STATE_DOWN)
			{
				m_curButtonState = BUTTON_STATE_ON;
				if(m_buttonStyle == BTN_STYLE_CHECK)
					m_isCheck = !m_isCheck;
				if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,BTN_MOUSE_UP);
				else XCtrlManager.eventProc(m_objectID,BTN_MOUSE_UP);
				m_isBeChoose = XTrue;	//控件处于焦点状态
			}
		}
	}else
	{
		if(m_isMouseInRect)
		{
			m_isMouseInRect = XFalse;
			m_comment.disShow();
		}
		if(m_isEnable &&
			(m_curButtonState == BUTTON_STATE_ON || m_curButtonState == BUTTON_STATE_DOWN))
		{
			m_curButtonState = BUTTON_STATE_NORMAL;
		}
	}
	return XTrue;
}
XBool XButton::keyboardProc(int keyOrder,XKeyState keyState)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	if(m_withAction && m_isInAction) return XTrue;	//如果支持动作播放而且正在播放动画那么不接受鼠标控制
	if(m_isSilent) return XFalse;

	if(keyState == KEY_STATE_UP &&	//按键弹起时才作相应
		m_curButtonState == BUTTON_STATE_NORMAL &&	//按钮只有在普通状态下才能响应快捷键，防止冲突
		(keyOrder == m_hotKey || (keyOrder ==  XKEY_RETURN && m_isBeChoose)))
	{
		if(m_buttonStyle == BTN_STYLE_CHECK)
			m_isCheck = !m_isCheck;
		if(m_eventProc != NULL)
		{
			m_eventProc(m_pClass,m_objectID,BTN_MOUSE_DOWN);
			m_eventProc(m_pClass,m_objectID,BTN_MOUSE_UP);
		}else
		{
			XCtrlManager.eventProc(m_objectID,BTN_MOUSE_DOWN);
			XCtrlManager.eventProc(m_objectID,BTN_MOUSE_UP);
		}
		return XTrue;
	}
	return XFalse;
}
void XButton::draw()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	if(m_withoutTex)
	{
		XVec2 tmpPos = m_position;
		XVec2 tmpSize = m_scale;
		if(m_isInAction && !m_actionMoveData.getIsEnd())
		{
			tmpPos = m_mouseRect.getSize() * (m_actionMoveData.getCurData() * m_scale - m_scale) * 0.5f;
			tmpPos = m_position - tmpPos;
			tmpSize = m_actionMoveData.getCurData() * m_scale;
		}

		switch(m_curButtonState)
		{
		case BUTTON_STATE_NORMAL:
			if(m_buttonStyle == BTN_STYLE_CHECK && m_isCheck)
				XRender::drawFillRectExA(tmpPos + m_mouseRect.getLT() * tmpSize,
					m_mouseRect.getSize() * tmpSize,
					XCCS::downColor * m_color,true);//,true); 
			//		m_mouseRect.getHeight()) * tmpSize,0.5f * m_color.r,0.5f * m_color.g,0.5f * m_color.b,m_color.a,true);//,true); 
			else
				XRender::drawFillRectExA(tmpPos + m_mouseRect.getLT() * tmpSize,
					m_mouseRect.getSize() * tmpSize,
					XCCS::normalColor * m_color,true);//,true); 
			//		m_mouseRect.getHeight()) * tmpSize,0.75f * m_color.r,0.75f * m_color.g,0.75f * m_color.b,m_color.a,true);//,true); 
			break;
		case BUTTON_STATE_DOWN:
			XRender::drawFillRectExA(tmpPos + m_mouseRect.getLT() * tmpSize,
				m_mouseRect.getSize() * tmpSize,
				XCCS::downColor * m_color,true);//,true); 
		//		m_mouseRect.getHeight()) * tmpSize,0.5f * m_color.r,0.5f * m_color.g,0.5f * m_color.b,m_color.a,true);//,true); 
			break;
		case BUTTON_STATE_ON:
			if(m_buttonStyle == BTN_STYLE_CHECK && m_isCheck)
				XRender::drawFillRectExA(tmpPos + m_mouseRect.getLT() * tmpSize,
					m_mouseRect.getSize() * tmpSize,
					XCCS::downColor * m_color,true);//,true);
			//		m_mouseRect.getHeight()) * tmpSize,0.85f * m_color.r,0.85f * m_color.g,0.85f * m_color.b,m_color.a,true);//,true); 
			else
				XRender::drawFillRectExA(tmpPos + m_mouseRect.getLT() * tmpSize,
					m_mouseRect.getSize() * tmpSize,
					XCCS::onColor * m_color,true);//,true);
			//		m_mouseRect.getHeight()) * tmpSize,0.85f * m_color.r,0.85f * m_color.g,0.85f * m_color.b,m_color.a,true);//,true); 
			break;
		case BUTTON_STATE_DISABLE:
			XRender::drawFillRectExA(tmpPos + m_mouseRect.getLT() * tmpSize,
				m_mouseRect.getSize() * tmpSize,
				XCCS::disableColor * m_color,true,false,true);
		//		m_mouseRect.getHeight()) * tmpSize,0.35f * m_color.r,0.35f * m_color.g,0.35f * m_color.b,m_color.a,true);//,true); 
			break;
		}
	}else
	{
		switch(m_curButtonState)
		{
		case BUTTON_STATE_NORMAL:
			if(m_buttonStyle == BTN_STYLE_CHECK && m_isCheck)
				m_sprite.draw(m_buttonDown);
			else
				m_sprite.draw(m_buttonNormal);
			break;
		case BUTTON_STATE_DOWN:m_sprite.draw(m_buttonDown);break;
		case BUTTON_STATE_ON:m_sprite.draw(m_buttonOn);break;
		case BUTTON_STATE_DISABLE:m_sprite.draw(m_buttonDisable);break;
		}
	}
	//显示按钮的字体
	m_caption.draw();
	if(m_curButtonState == BUTTON_STATE_DISABLE)
	{
		if(m_withDisableIcon) m_disableIcon.draw();else
		if(m_withNormalIcon) m_normalIcon.draw();
	}else
	{
		if(m_withNormalIcon) m_normalIcon.draw();else
		if(m_withDisableIcon) m_disableIcon.draw();
	}
	//下面显示符号
	if(m_symbolType != BTN_SYMBOL_NULL)
	{
		float tmpSize = (std::min)(m_curMouseRect.getHeight(),m_curMouseRect.getWidth()) * 0.5f * 0.65f;
		if(tmpSize <= 0.0f) tmpSize = 1.0f;
		switch(m_symbolType)
		{
		case BTN_SYMBOL_LINE:		//减号
			XRender::drawLine(m_caption.getPosition() + XVec2(-tmpSize,0.0f),
				m_caption.getPosition() + XVec2(tmpSize,0.0f),2.0f,m_caption.getColor());
			break;
		case BTN_SYMBOL_CIRCLE:		//圆形
			XRender::drawCircleLine(m_caption.getPosition(),tmpSize,36,2.0f,m_caption.getColor());
			break;
		case BTN_SYMBOL_RECT:		//矩形
			XRender::drawRect(m_caption.getPosition(),XVec2(tmpSize),2.0f,m_caption.getColor());
			break;
		case BTN_SYMBOL_CROSS:		//十字架
			XRender::drawCross(m_caption.getPosition(),tmpSize,2.0f,m_caption.getColor());
			break;
		case BTN_SYMBOL_TRIANGLE:	//三角形
			XRender::drawTriangle(m_caption.getPosition(),tmpSize,2.0f,m_caption.getColor());
			break;
		case BTN_SYMBOL_MASK_RIGHT:	//对号
			XRender::drawMaskRight(m_caption.getPosition(),XVec2(tmpSize),2.0f,m_caption.getColor());
			break;
		case BTN_SYMBOL_MASK_WRONG:	//错号
			XRender::drawMaskWrong(m_caption.getPosition(),XVec2(tmpSize),2.0f,m_caption.getColor());
			break;
		case BTN_SYMBOL_LEFT:		//左
			XRender::drawLeft(m_caption.getPosition(),XVec2(tmpSize),2.0f,m_caption.getColor());
			break;
		case BTN_SYMBOL_RIGHT:		//右
			XRender::drawRight(m_caption.getPosition(),XVec2(tmpSize),2.0f,m_caption.getColor());
			break;
		case BTN_SYMBOL_UP:			//上
			XRender::drawUp(m_caption.getPosition(),XVec2(tmpSize),2.0f,m_caption.getColor());
			break;
		case BTN_SYMBOL_DOWN:		//下
			XRender::drawDown(m_caption.getPosition(),XVec2(tmpSize),2.0f,m_caption.getColor());
			break;
		case BTN_SYMBOL_SAVE:
			XRender::drawSave(m_caption.getPosition(), XVec2(tmpSize), 2.0f, m_caption.getColor());
			break;
		case BTN_SYMBOL_LOAD:
			XRender::drawLoad(m_caption.getPosition(), XVec2(tmpSize), 2.0f, m_caption.getColor());
			break;
		case BTN_SYMBOL_DEF:
			XRender::drawDefault(m_caption.getPosition(), XVec2(tmpSize), 2.0f, m_caption.getColor());
			break;
		case BTN_SYMBOL_DOWNLOAD:
			XRender::drawDownload(m_caption.getPosition(), XVec2(tmpSize), 2.0f, m_caption.getColor());
			break;
		case BTN_SYMBOL_UPDATE:
			XRender::drawUpdate(m_caption.getPosition(), XVec2(tmpSize), 2.0f, m_caption.getColor());
			break;
		}
	}
}
XBool XButton::setACopy(const XButton &temp)
{
	if(& temp == this) return XTrue;	//防止自身赋值
	if(!temp.m_isInited || !XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
		XObjManager.addAObject(this);
#endif
	}
	m_isInited = temp.m_isInited;					//按钮是否被初始化
	m_curButtonState = temp.m_curButtonState;		//当前的按钮状态

	if(!m_caption.setACopy(temp.m_caption))	 return XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif

	m_withNormalIcon = temp.m_withNormalIcon;		//是否拥有普通icon
	if(m_withNormalIcon)
	{
		m_normalIcon.setACopy(temp.m_normalIcon);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&m_normalIcon);
#endif
	}
	m_withDisableIcon = temp.m_withDisableIcon;		//是否拥有无效icon
	if(m_withDisableIcon)
	{
		m_disableIcon.setACopy(temp.m_disableIcon);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&m_disableIcon);
#endif
	}
	m_iconSize = temp.m_iconSize;		//icon的尺寸
	m_iconPosition = temp.m_iconPosition;	//icon的位置

	if(m_resInfo != NULL) XResManager.releaseResource(m_resInfo);
	m_resInfo = XResManager.copyResource(temp.m_resInfo);
	m_withoutTex = temp.m_withoutTex;

	m_buttonNormal = temp.m_buttonNormal;			//普通状态下的按钮贴图(无论如何，这个贴图一定要有)
	m_buttonDown = temp.m_buttonDown;			//按下状态下的按钮贴图
	m_buttonOn = temp.m_buttonOn;				//悬浮状态下的按钮贴图
	m_buttonDisable = temp.m_buttonDisable;			//无效状态下的按钮贴图

	m_sprite.setACopy(temp.m_sprite);	//用于显示贴图的精灵
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_sprite);
#endif
	m_textPosition = temp.m_textPosition;	//文字显示的位置，是相对于控件的位置来定的
	m_textColor = temp.m_textColor;
	m_textSize = temp.m_textSize;		//文字显示的尺寸，这个尺寸会与空间的缩放尺寸叠加
	m_upMousePoint = temp.m_upMousePoint;	//上次记录的鼠标位置

	m_actionMoveData = temp.m_actionMoveData;	//动态效果的变量
	m_oldPos = temp.m_oldPos;				//动作播放时的位置
	m_oldSize = temp.m_oldSize;			//动作播放时的大小
	m_lightMD = temp.m_lightMD;
	m_lightRect = temp.m_lightRect;

	m_hotKey = temp.m_hotKey;
	m_symbolType = temp.m_symbolType;
	return XTrue;
}
void XButton::setNormalIcon(const char * filename,XResPos resPos)
{
	if(filename == NULL) return;
	if(m_withNormalIcon) m_normalIcon.release();
	if(m_normalIcon.init(filename,resPos,POINT_LEFT_TOP))
	{
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&m_normalIcon);
#endif
		m_withNormalIcon = true;

		m_normalIcon.setColor(m_color);
		m_normalIcon.setPosition(m_position + m_iconPosition * m_scale);
		m_normalIcon.setScale(m_iconSize * m_scale);
	}
}
void XButton::setNormalIcon(const XSprite &icon)
{
	if(m_withNormalIcon) m_normalIcon.release();
	m_normalIcon.setACopy(icon);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_normalIcon);
#endif
	m_withNormalIcon = true;

	m_normalIcon.setColor(m_color);
	m_normalIcon.setPosition(m_position + m_iconPosition * m_scale);
	m_normalIcon.setScale(m_iconSize * m_scale);
}
void XButton::setDisableIcon(const char * filename,XResPos resPos)
{
	if(filename == NULL) return;
	if(m_withDisableIcon) m_disableIcon.release();
	if(m_disableIcon.init(filename,resPos,POINT_LEFT_TOP))
	{
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&m_disableIcon);
#endif
		m_withDisableIcon = true;

		m_disableIcon.setColor(m_color);
		m_disableIcon.setPosition(m_position + m_iconPosition * m_scale);
		m_disableIcon.setScale(m_iconSize * m_scale);
	}
}
void XButton::setDisableIcon(const XSprite &icon)
{
	if(m_withDisableIcon) m_disableIcon.release();
	m_disableIcon.setACopy(icon);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_disableIcon);
#endif
	m_withDisableIcon = true;

	m_disableIcon.setColor(m_color);
	m_disableIcon.setPosition(m_position + m_iconPosition * m_scale);
	m_disableIcon.setScale(m_iconSize * m_scale);
}
void XButton::removeIcon()	//去掉所有的icon
{
	if(m_withNormalIcon)
	{
		m_normalIcon.release();
		m_withNormalIcon = false;
	}
	if(m_withDisableIcon)
	{
		m_disableIcon.release();
		m_withDisableIcon = false;
	}
}
void XButton::update(float stepTime)
{
	m_comment.update(stepTime);
	if(m_isInAction)
	{//处于动作过程中计算动作的实施
		m_actionMoveData.move(stepTime);
		if(m_actionMoveData.getIsEnd())
			m_isInAction = false;	//动作播放完成
		setScale(m_scale);
		setPosition(m_position);
	}
	//扩散效果的表现
	if(!m_lightMD.getIsEnd())
	{
		m_lightMD.move(stepTime);
		XVec2 pos = m_oldPos + m_mouseRect.getSize() * m_oldSize * 0.5f;
		XVec2 size = m_mouseRect.getSize() * m_oldSize * m_lightMD.getCurData() * 0.5f;
		m_lightRect.set(pos - size,pos + size);
	}
}
void XButton::setWidth(int width)
{
	if(!m_withoutTex || width <= 0 || !m_isInited) return;	//如果使用贴图，则这里直接返回
	m_mouseRect.right = m_mouseRect.left + width;
	m_textPosition.x = m_mouseRect.getCenter().x;
	m_caption.setPosition(m_position + m_textPosition * m_scale);

	m_curMouseRect.set(m_position + m_mouseRect.getLT() * m_scale,
		m_position + m_mouseRect.getRB() * m_scale);
}
#if !WITH_INLINE_FILE
#include "XButton.inl"
#endif
}