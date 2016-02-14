#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XEdit.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "../XImm.h"
#include "XResourcePack.h"

namespace XE{
char XEdit::m_copyString[m_maxInputStringLength] = "";
XEditSkin::XEditSkin()
:m_isInited(XFalse)
,editNormal(NULL)			//输入框普通状态
,editDisable(NULL)			//输入框无效状态
,editSelect(NULL)			//输入框片选颜色
,editInsert(NULL)			//输入框插入标记
,editUpon(NULL)
{}
XBool XEditSkin::init(const char *normal,const char *disable,const char *select,const char *insert,const char *upon,XResourcePosition resoursePosition)
{
	if(m_isInited ||	//防止重复初始化
		normal == NULL || disable == NULL || insert == NULL || select == NULL) return XFalse;//这些关键的资源不能为空
	int ret = 1;
	if((editNormal = createATextureData(normal,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(editDisable = createATextureData(disable,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(editInsert = createATextureData(insert,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(editSelect = createATextureData(select,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(editUpon = createATextureData(upon,resoursePosition)) == NULL) ret = 0;

	if(ret == 0)
	{
		releaseTex();
		return XFalse;
	}

	m_isInited = XTrue;
	return XTrue;
}
#define EDIT_CONFIG_FILENAME "Edit.txt"
bool XEditSkin::loadFromFolder(const char *filename,XResourcePosition resPos)	//从文件夹中载入资源
{
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf(tempFilename,"%s/%s",filename,EDIT_CONFIG_FILENAME);
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
	if((editNormal = createATextureData(tempFilename,resPos)) == NULL)
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
	sprintf(tempFilename,"%s/%s",filename,resFilename);
	if((editDisable = createATextureData(tempFilename,resPos)) == NULL)
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
	sprintf(tempFilename,"%s/%s",filename,resFilename);
	if((editSelect = createATextureData(tempFilename,resPos)) == NULL)
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
	sprintf(tempFilename,"%s/%s",filename,resFilename);
	if((editInsert = createATextureData(tempFilename,resPos)) == NULL)
	{//资源读取失败
		releaseTex();
		fclose(fp);
		return XFalse;
	}
	//upon
	if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
	if(flag != 0)
	{
		if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((editUpon = createATextureData(tempFilename,resPos)) == NULL)
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
	//所有数据读取完成
	fclose(fp);
	return true;
}
bool XEditSkin::loadFromPacker(const char *filename,XResourcePosition resPos)	//从压缩包中载入资源
{
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf(tempFilename,"%s/%s",filename,EDIT_CONFIG_FILENAME);
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
	if((editNormal = createATextureData(tempFilename,resPos)) == NULL)
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
	sprintf(tempFilename,"%s/%s",filename,resFilename);
	if((editDisable = createATextureData(tempFilename,resPos)) == NULL)
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
	sprintf(tempFilename,"%s/%s",filename,resFilename);
	if((editSelect = createATextureData(tempFilename,resPos)) == NULL)
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
	sprintf(tempFilename,"%s/%s",filename,resFilename);
	if((editInsert = createATextureData(tempFilename,resPos)) == NULL)
	{//资源读取失败
		releaseTex();
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	//upon
	if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),':') + 1;
	if(flag != 0)
	{
		if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
		offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((editUpon = createATextureData(tempFilename,resPos)) == NULL)
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
	//所有数据读取完成
	XMem::XDELETE_ARRAY(p);
	return true;
}
bool XEditSkin::loadFromWeb(const char *filename,XResourcePosition resPos)		//从网页中读取资源
{
	return true;
}
XBool XEditSkin::initEx(const char *filename,XResourcePosition resoursePosition)
{
	if(m_isInited ||
		filename == NULL) return XFalse;
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf(tempFilename,"%s/%s",filename,EDIT_CONFIG_FILENAME);
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
XEdit::XEdit()
	:m_isInited(XFalse)					//是否初始化
	,m_mouseRightButtonMenu(NULL)	//鼠标右键菜单
	,m_curString(NULL)				//当前输入的字符串
	,m_tempCurString(NULL)
	//,m_insertString(NULL)			//插入的字符串
	//,m_selectString(NULL)			//选取的字符串
	,m_haveSelect(XFalse)
	,m_editNormal(NULL)				//输入框普通状态
	,m_editDisable(NULL)			//输入框无效状态
	,m_editSelect(NULL)				//输入框片选颜色
	,m_editInsert(NULL)				//输入框插入标记
	//,m_funInputChenge(NULL)			//输入内容发生改变的时候调用
	//,m_funInputOver(NULL)			//确认输入结束之后的时候调用
	,m_resInfo(NULL)
	,m_withoutTex(XFalse)
	,m_curKeyDown(XFalse)
	,m_isPassword(false)
	,m_withPromptStr(false)
	,m_editType(TYPE_STRING)
{//这里初始化几个按键的状态
	if(XEE::getCapsLockState()) m_keyCapsLockState = 1;
	else m_keyCapsLockState = 0;
	if(XEE::getNumLockState()) m_keyNumLockState = 1;
	else m_keyNumLockState = 0;
	m_ctrlType = CTRL_OBJ_EDIT;
}
XBool XEdit::init(const XVector2& position,		//控件的位置
		const XRect& Area,					//控件的鼠标响应区间
		const XEditSkin &tex,			//控件的贴图
		const char *str,					//控件的初始化字符串
		const XFontUnicode &font,			//控件的字体
		float strSize,	//控件的字体信息
		XMouseRightButtonMenu * mouseMenu)		//控件的右键菜单
{
	if(m_isInited ||
		tex.editNormal == NULL || tex.editInsert == NULL || tex.editDisable == NULL || tex.editSelect == NULL) return XFalse;

	//需要注意的是输入范围必须要能显示最少一个字符，否则将会造成问题，目前这里并没有代码去判断，但是实际使用中需要注意这个问题
	if(Area.getWidth() <= 0 || Area.getHeight() <= 0) return XFalse;	//输入范围不能为空
	if(strSize <= 0) return XFalse;		//字符串的大小不能为非法值
	if(str != NULL && strlen(str) >= m_maxInputStringLength) return XFalse;
	if(mouseMenu != NULL)
	{//这里需要改进，这里使用指针，会受到别处修改的影响
		m_mouseRightButtonMenu = XMem::createMem<XMouseRightButtonMenu>();
		if(m_mouseRightButtonMenu == NULL) return XFalse;	
		m_mouseRightButtonMenu->setACopy(* mouseMenu);
		//从控件管理器中将这个控件注销掉
		XCtrlManager.decreaseAObject(m_mouseRightButtonMenu);	//注销这个物件
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(m_mouseRightButtonMenu);
#endif
	}

	//下面开始付值
	m_mouseRect = Area;	//记录鼠标的响应范围
	m_position = position;
	m_withoutTex = XFalse;
	m_comment.init();

	m_editNormal = tex.editNormal;			//输入框普通状态
	m_editDisable = tex.editDisable;		//输入框无效状态
	m_editSelect = tex.editSelect;			//输入框片选颜色
	m_editInsert = tex.editInsert;			//输入框插入标记
	m_editUpon = tex.editUpon;			//输入框插入标记
	m_scale.set(1.0f,1.0f);
	m_curString = XMem::createArrayMem<char>(m_maxInputStringLength);
	if(m_curString == NULL) 
	{
		XMem::XDELETE(m_mouseRightButtonMenu);
		return XFalse;
	}		
	m_curString[0] = '\0';
	m_tempCurString = XMem::createArrayMem<char>(m_maxInputStringLength);
	if(m_tempCurString == NULL) 
	{
		XMem::XDELETE_ARRAY(m_curString);
		XMem::XDELETE(m_mouseRightButtonMenu);
		return XFalse;
	}		
	m_tempCurString[0] = '\0';
//	m_insertString = XMem::createArrayMem<char>(m_maxInputStringLength);	//当前输入的字符串
//	if(m_insertString == NULL) return XFalse;
//	m_insertString[0] = '\0';
//	m_selectString = XMem::createArrayMem<char>(m_maxInputStringLength);	//当前输入的字符串
//	if(m_selectString == NULL) return XFalse;
//	m_selectString[0] = '\0';
	m_curShowString = XMem::createArrayMem<char>(m_maxInputStringLength);
	if(m_curShowString == NULL) 
	{
		XMem::XDELETE_ARRAY(m_tempCurString);
		XMem::XDELETE_ARRAY(m_curString);
		XMem::XDELETE(m_mouseRightButtonMenu);
		return XFalse;
	}		
	m_curShowString[0] = '\0';

	if(!m_caption.setACopy(font)) return XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //设置字体居中对齐
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//设置字体的颜色为黑色

	m_textPosition.set(m_mouseRect.left + m_textEdgeWidth,m_mouseRect.top + m_mouseRect.getHeight() * 0.5f);			//文字显示的位置，是相对于控件的位置来定的
	m_textSize.set(strSize,strSize);				//文字显示的尺寸，这个尺寸会与空间的缩放尺寸叠加
	if(str != NULL)
	{
		m_caption.setString(str);
		strcpy(m_curString,str);
		m_curInsertPoint = strlen(m_curString);	//当前选择输入的位置
		m_curStringLength = strlen(m_curString);	//当前输入字符串的长度
	}else
	{
		m_caption.setString(" ");
		m_curString[0] = '\0';
		m_curInsertPoint = 0;	//当前选择输入的位置
		m_curStringLength = 0;	//当前输入字符串的长度
	}
	m_caption.setPosition(m_position.x + m_textPosition.x * m_scale.x,m_position.y + m_textPosition.y * m_scale.y);
	m_caption.setScale(m_textSize.x * m_scale.x,m_textSize.y * m_scale.y);
	m_curMouseRect.set(m_position.x + m_mouseRect.left * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y,
		m_position.x + m_mouseRect.right * m_scale.x,m_position.y + m_mouseRect.bottom * m_scale.y);
	//m_caption.m_isPassWord = 1;
//	m_selectText.setACopy(* font);
//	m_selectText.setString(" ");
//	m_selectText.setPosition(m_objRect.left + (m_edgeDistance.left + m_textPosition.x) * m_scale.x,m_objRect.top + (m_edgeDistance.top + m_textPosition.y) * m_scale.y);
//	m_selectText.setScale(m_textSize.x * m_scale.x,m_textSize.y * m_scale.y);

	m_haveSelect = XFalse;		//是否在字符串中有选择
	m_selectMouseDown = XFalse;
	m_selectStart = -1;		//选择的起始位置
	m_selectEnd = -1;		//选择的结束位置

	m_spriteBackGround.init(m_editNormal->texture.m_w,m_editNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteBackGround);
#endif
	m_spriteBackGround.setPosition(m_position);
	m_spriteBackGround.setScale(m_scale);
	m_spriteBackGround.setIsTransformCenter(POINT_LEFT_TOP);

	m_spriteSelect.init(m_editSelect->texture.m_w,m_editSelect->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteSelect);
#endif
	m_spriteSelect.setPosition(m_position);
	m_spriteSelect.setScale(m_scale);
	m_spriteSelect.setIsTransformCenter(POINT_LEFT_TOP);

	//计算输入框中可以显示的字符串的长度(这里取0.5，是因为显示英文字符但是却是使用的中文字体库)
	m_curTextWidth = m_caption.getTextSize().x * m_caption.getScale().x * 0.5f;
	m_curTextHeight = m_caption.getTextSize().y * m_caption.getScale().y;
	m_canShowLength = (m_mouseRect.getWidth() - 2.0f * m_textEdgeWidth) * m_scale.x / m_curTextWidth;
	m_curShowStart = 0;	//初始化当前显示的起始位置
	upDataShowStr();

	m_spriteInsert.init(m_editInsert->texture.m_w,m_editInsert->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteInsert);
#endif
	upDateInsertShowPosition();
	m_spriteInsert.setScale(m_textSize.x * m_scale.x,m_textSize.y * m_scale.y);
	m_spriteInsert.setIsTransformCenter(POINT_LEFT_TOP);

	//printf("Can Show String Length:%d\n",m_canShowLength);

	m_timer = 0;

	m_keyShiftState = 0;
	m_keyCapsLockState = 0;
	m_keyNumLockState = 1;
	m_keyCtrlState = 0;

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;
	m_isBeChoose = XFalse; 

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
XBool XEdit::initWithoutSkin(const XRect& area,
		const char *str,					//控件的初始化字符串
		const XFontUnicode &font,			//控件的字体
		float strSize,	//控件的字体信息
		XMouseRightButtonMenu * mouseMenu)
{
	if(m_isInited ||
	//需要注意的是输入范围必须要能显示最少一个字符，否则将会造成问题，目前这里并没有代码去判断，但是实际使用中需要注意这个问题
		strSize <= 0 ||		//字符串的大小不能为非法值
		(str != NULL && strlen(str) >= m_maxInputStringLength)) return XFalse;
	if(mouseMenu != NULL)
	{//这里需要改进，这里使用指针，会受到别处修改的影响
		m_mouseRightButtonMenu = XMem::createMem<XMouseRightButtonMenu>();
		if(m_mouseRightButtonMenu == NULL) return XFalse;	
		m_mouseRightButtonMenu->setACopy(* mouseMenu);
		//从控件管理器中将这个控件注销掉
		XCtrlManager.decreaseAObject(m_mouseRightButtonMenu);	//注销这个物件
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(m_mouseRightButtonMenu);
#endif
	}

	//下面开始付值
	m_mouseRect = area;	//记录鼠标的响应范围
	m_position.set(0.0f,0.0f);
	m_withoutTex = XTrue;
	m_comment.init();

	m_scale.set(1.0f,1.0f);
	m_curString = XMem::createArrayMem<char>(m_maxInputStringLength);
	if(m_curString == NULL) 
	{
		XMem::XDELETE(m_mouseRightButtonMenu);
		return XFalse;
	}		
	m_curString[0] = '\0';
	m_tempCurString = XMem::createArrayMem<char>(m_maxInputStringLength);
	if(m_tempCurString == NULL) 
	{
		XMem::XDELETE_ARRAY(m_curString);
		XMem::XDELETE(m_mouseRightButtonMenu);
		return XFalse;
	}		
	m_tempCurString[0] = '\0';
	m_curShowString = XMem::createArrayMem<char>(m_maxInputStringLength);
	if(m_curShowString == NULL) 
	{
		XMem::XDELETE_ARRAY(m_tempCurString);
		XMem::XDELETE_ARRAY(m_curString);
		XMem::XDELETE(m_mouseRightButtonMenu);
		return XFalse;
	}		
	m_curShowString[0] = '\0';

	if(!m_caption.setACopy(font)) return XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //设置字体居中对齐
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//设置字体的颜色为黑色

	m_textPosition.set(m_mouseRect.left + m_textEdgeWidth,m_mouseRect.top + m_mouseRect.getHeight() * 0.5f);			//文字显示的位置，是相对于控件的位置来定的
	m_textSize.set(strSize,strSize);				//文字显示的尺寸，这个尺寸会与空间的缩放尺寸叠加
	if(str != NULL)
	{
		m_caption.setString(str);
		strcpy(m_curString,str);
		m_curInsertPoint = strlen(m_curString);	//当前选择输入的位置
		m_curStringLength = strlen(m_curString);	//当前输入字符串的长度
	}else
	{
		m_caption.setString(" ");
		m_curString[0] = '\0';
		m_curInsertPoint = 0;	//当前选择输入的位置
		m_curStringLength = 0;	//当前输入字符串的长度
	}
	m_caption.setPosition(m_position.x + m_textPosition.x * m_scale.x,m_position.y + m_textPosition.y * m_scale.y);
	m_caption.setScale(m_textSize.x * m_scale.x,m_textSize.y * m_scale.y);
	m_curMouseRect.set(m_position.x + m_mouseRect.left * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y,
		m_position.x + m_mouseRect.right * m_scale.x,m_position.y + m_mouseRect.bottom * m_scale.y);

	m_haveSelect = XFalse;		//是否在字符串中有选择
	m_selectMouseDown = XFalse;
	m_selectStart = -1;		//选择的起始位置
	m_selectEnd = -1;		//选择的结束位置

	//计算输入框中可以显示的字符串的长度(这里取0.5，是因为显示英文字符但是却是使用的中文字体库)
	m_curTextWidth = m_caption.getTextSize().x * m_caption.getScale().x * 0.5f;
	m_curTextHeight = m_caption.getTextSize().y * m_caption.getScale().y;
	m_canShowLength = (m_mouseRect.getWidth() - 2.0f * m_textEdgeWidth) * m_scale.x / m_curTextWidth;
	m_curShowStart = 0;	//初始化当前显示的起始位置
	upDataShowStr();

	upDateInsertShowPosition();

	//printf("Can Show String Length:%d\n",m_canShowLength);
	m_timer = 0;

	m_keyShiftState = 0;
	m_keyCapsLockState = 0;
	m_keyNumLockState = 1;
	m_keyCtrlState = 0;

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;
	m_isBeChoose = XFalse; 

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
void XEdit::upDataShowStr()
{//还没有实现
	if(m_curStringLength <= m_curShowStart + m_canShowLength) 
	{
		m_curShowStart = m_curStringLength - m_canShowLength;
		if(m_curShowStart < 0) m_curShowStart = 0;
		if(!XString::isAtUnicodeEnd(m_curString,m_curShowStart)) ++m_curShowStart; //如果头临界、则多移动一位
		strcpy(m_curShowString,m_curString + m_curShowStart);
	}else
	{
		if(XString::isAtUnicodeEnd(m_curString,m_curShowStart + m_canShowLength)) 
		{
			memcpy(m_curShowString,m_curString + m_curShowStart,m_canShowLength);
			m_curShowString[m_canShowLength] = '\0';
		}else 
		{
			memcpy(m_curShowString,m_curString + m_curShowStart,m_canShowLength - 1);
			m_curShowString[m_canShowLength - 1] = '\0';
		}
	}
	//跟新到显示中
	if(m_withPromptStr && strlen(m_curString) <= 0)
	{//如果有提示文字
		m_caption.setColor(XFColor(0.65f,0.65f,0.65f,1.0f) * m_color);
		m_caption.setString(XString::getCanShowString(m_promptStr.c_str(),m_canShowLength).c_str());
	}else
	{//如果没有提示文字
		m_caption.setColor(m_textColor * m_color);
		m_caption.setString(m_curShowString);
	}
	if(!isStringPassable())
		m_caption.setColor(XFColor::red * m_color);
}
void XEdit::draw()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	
	if(m_withoutTex)
	{
		if(!m_isEnable) 
		{
			XRender::drawFillBoxExA(m_position + XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y),
				XVector2(m_mouseRect.getWidth() * m_scale.x,m_mouseRect.getHeight() * m_scale.y),XCCS::downColor * m_color,true,false,true);
		}else 
		{
			XRender::drawFillBoxExA(m_position + XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y),
				XVector2(m_mouseRect.getWidth() * m_scale.x,m_mouseRect.getHeight() * m_scale.y),XCCS::normalColor * m_color,true,false,true);
		}

		if(m_haveSelect) 
			XRender::drawFillBoxExA(XVector2(m_selectRect.left,m_selectRect.top),XVector2(m_selectRect.right,m_selectRect.bottom),XCCS::specialColor * m_color,true,false,true);

		//显示当前输入的字符串
		m_caption.draw();
		if(m_insertActionMD.getIsEnd())
		{
			if(m_isEnable && m_isBeChoose)
			{
				if(m_timer > 250) 
				{
					float x = m_position.x + m_textPosition.x * m_scale.x + (m_curInsertPoint - m_curShowStart) * m_curTextWidth;
					float y = m_position.y + m_textPosition.y * m_scale.y - m_curTextHeight * 0.5f;
					XRender::drawLine(x,y,x,y + m_curTextHeight,1,XCCS::blackDownColor * m_color);
					//m_spriteInsert.draw(m_editInsert);
				}
			}
		}
		//不画上层
	//	if(m_editUpon != NULL) 
	//	{
	//		m_spriteBackGround.draw(m_editUpon);
	//	}
	}else
	{
		if(!m_isEnable) m_spriteBackGround.draw(m_editDisable);	//如果无效则显示无效,不显示插入符号
		else m_spriteBackGround.draw(m_editNormal);//如果有效则显示有效,显示插入符号

		if(m_haveSelect) m_spriteSelect.draw(m_editSelect);//显示选择的范围

		//显示当前输入的字符串
		m_caption.draw();
		if(m_isEnable && m_isBeChoose && m_timer > 250)
			m_spriteInsert.draw(m_editInsert);

		if(m_editUpon != NULL) m_spriteBackGround.draw(m_editUpon);	
	}
}
void XEdit::setImmPos()
{
	float x = m_position.x + m_textPosition.x * m_scale.x + (m_curInsertPoint - m_curShowStart) * m_curTextWidth;
	float y = m_position.y + m_textPosition.y * m_scale.y - m_curTextHeight * 0.5f;
	XImm::setCompositionPos(x,y);
}
void XEdit::release()
{
	if(!m_isInited) return;
	XMem::XDELETE_ARRAY(m_curString);
	XMem::XDELETE_ARRAY(m_tempCurString);
	//XMem::XDELETE_ARRAY(m_insertString);
	//XMem::XDELETE_ARRAY(m_selectString);
	XMem::XDELETE_ARRAY(m_curShowString);
	XMem::XDELETE(m_mouseRightButtonMenu);
	XCtrlManager.decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	if(m_resInfo != NULL)
	{
		XResManager.releaseResource(m_resInfo);
		m_resInfo = NULL;
	}
	m_isInited = XFalse;
}
void XEdit::setString(const char *str)
{//这个函数面临很复杂的逻辑环境，如果用户正在处于输入阶段或者正在处于片选阶段的时候这个修改将会造成较为大的影响，所以这里可能会做一些不人性化的处理
	if(!m_isInited ||
		str == NULL) return;
	if(!isStringPassable()) return;	//如果数据不符合要求则直接返回
	m_caption.setString(str);
	strcpy(m_curString,str);
	m_curInsertPoint = strlen(m_curString);	//当前选择输入的位置
	m_curStringLength = strlen(m_curString);	//当前输入字符串的长度
	//取消所有片选的信息
	m_haveSelect = XFalse;		//是否在字符串中有选择
	m_selectMouseDown = XFalse;
	m_selectStart = -1;		//选择的起始位置
	m_selectEnd = -1;		//选择的结束位置
	//计算输入框中可以显示的字符串的长度(这里取0.5，是因为显示英文字符但是却是使用的中文字体库)
	m_canShowLength = (m_mouseRect.getWidth() - 2.0f * m_textEdgeWidth) * m_scale.x / m_curTextWidth;
	m_curShowStart = 0;	//初始化当前显示的起始位置
	upDataShowStr();

	upDateInsertShowPosition();
}
bool XEdit::stringPlaster()
{
#ifdef XEE_OS_WINDOWS
	std::string tmp = XString::getTextFromClipboard();
	if(tmp == "") m_copyString[0] = '\0';	//没有数
	else 
	{
		if(tmp.size() >= m_maxInputStringLength - 1)
		{
			memcpy(m_copyString,tmp.c_str(),m_maxInputStringLength - 1);
			m_copyString[m_maxInputStringLength - 1] = '\0';
		}else
		{
			memcpy(m_copyString,tmp.c_str(),tmp.size());
			m_copyString[tmp.size()] = '\0';
		}
	}
#endif
	if(m_copyString[0] == '\0' ||
		!getCopyStrHaveEnd()) return false;
	if(m_haveSelect)
	{//存在片选(这里实际是替换动作)
		if(m_curStringLength + strlen(m_copyString) - getSelectLength() > m_maxInputStringLength - 2)
		{//如果超出总长度，则这里需要谨慎处理，目前定义为不作处理
		}else
		{							
			//1、删除片选内容
			deleteSelectStr();
			m_haveSelect = XFalse;	//片选结束
			//2、插入复制内容
			if(m_curInsertPoint == m_curStringLength)
			{//如果是在字符串的末尾添加
				strcat(m_curString,m_copyString);
				m_curStringLength = strlen(m_curString);
				//m_curInsertPoint = m_curStringLength;
				changeInsertPoint(m_curStringLength - m_curInsertPoint);
				return true;
			}else
			{
				strcpy(m_tempCurString,m_curString + m_curInsertPoint);
				strcpy(m_curString + m_curInsertPoint,m_copyString);
				strcat(m_curString,m_tempCurString);
				m_curStringLength = strlen(m_curString);
				//m_curInsertPoint += strlen(m_copyString);
				changeInsertPoint(strlen(m_copyString));
				return true;
			}
		}
	}else
	{//没有片选
		//判断粘贴的内容是否会超出字符串的总长度
		if(m_curStringLength + strlen(m_copyString) > m_maxInputStringLength - 2)
		{//如果超出总长度，则这里需要谨慎处理，目前定义为不作处理
		}else
		{
			if(m_curInsertPoint == m_curStringLength)
			{//如果是在字符串的末尾添加
				strcat(m_curString,m_copyString);
				m_curStringLength = strlen(m_curString);
			//	m_curInsertPoint = m_curStringLength;
				changeInsertPoint(m_curStringLength - m_curInsertPoint);
				return true;
			}else
			{
				strcpy(m_tempCurString,m_curString + m_curInsertPoint);
				strcpy(m_curString + m_curInsertPoint,m_copyString);
				strcat(m_curString,m_tempCurString);
				m_curStringLength = strlen(m_curString);
				//m_curInsertPoint += strlen(m_copyString);
				changeInsertPoint(strlen(m_copyString));
				return true;
			}
		}
	}
	return false;
}
void XEdit::mouseRightMenuProc()
{
	int TextChange = 0;
	switch(m_mouseRightButtonMenu->getLastChoose())
	{
	case 0://剪切
		if(!m_haveSelect) break;//存在片选，没有片选则什么也不作
		//1、拷贝片选的内容
#ifdef XEE_OS_WINDOWS
		memcpy(m_copyString,m_curString + getSelectHead(),getSelectLength());
		m_copyString[getSelectLength()] = '\0';
		XString::setTextToClipboard(m_copyString);
#endif
#ifdef XEE_OS_LINUX
		memcpy(m_copyString,m_curString + getSelectHead(),getSelectLength());
		m_copyString[getSelectLength()] = '\0';
#endif
		//2、删除片选的内容
		deleteSelectStr();
		m_haveSelect = XFalse;	//片选结束
		TextChange = 1;
		break;
	case 1://复制
		stringCopy();
		break;
	case 2://粘贴
		if(stringPlaster()) TextChange = 1;
		break;
	case 3://撤销(如果存在片选择取消片选)
		if(m_haveSelect) m_haveSelect = XFalse;
		break;
	}
//	printf("%d\n",m_mouseRightButtonMenu->getLastChoose());
	if(TextChange == 0) return;	//如果没有发生变化，则直接退出
	upDataShowStr();	//因为字符串被改变，所以这里需要更新字符的显示
//	m_caption.setString(m_curString);
//	m_spriteInsert.setPosition(m_objRect.left + (m_edgeDistance.left + m_textPosition.x) * m_scale.x + m_curInsertPoint * m_caption.m_scale.x * m_caption.m_showSize.x * 0.5,
//		m_objRect.top + (m_edgeDistance.top + m_textPosition.y) * m_scale.y);
	//if(m_funInputChenge != NULL) m_funInputChenge(m_pClass,m_objectID);
	if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,EDT_INPUT_CHANGE);
	else XCtrlManager.eventProc(m_objectID,EDT_INPUT_CHANGE);
	//当数据发生变化的时候播放动作
	if(m_withAction)
	{//这里测试一个动态效果
	//	m_isInAction = XTrue;
		m_lightMD.set(1.0f,2.0f,0.002f,MOVE_DATA_MODE_SIN_MULT);
		m_oldPos = m_position;
		m_oldSize = m_scale;
	}
}
void XEdit::changeInsertPoint(int sum)
{
	if(sum == 0) return;
	int tempNewPosition = m_curInsertPoint + sum;
	if(tempNewPosition < 0) return;
	if(tempNewPosition > m_curStringLength) return;
	//这里需要检查中文的问题
	if(!XString::isAtUnicodeEnd(m_curString,tempNewPosition)) 
	{//如果遇到中文临界就多移动一位,(这个方法不对)
		if(sum < 0) changeInsertPoint(sum - 1);//后退
		else changeInsertPoint(sum + 1);//前进
		return;
	}
	//可以移动
	int tempOldPosition = m_curInsertPoint;		//旧光标的位置
	m_curInsertPoint = tempNewPosition;
	//检查是否要移动显示数据
	if(m_curInsertPoint < m_curShowStart || m_curInsertPoint > m_curShowStart + m_canShowLength)
	{//超出显示范围需要修改显示范围
		if(m_curInsertPoint < m_curShowStart)
		{//向前移动
			if(tempOldPosition != m_curShowStart)
			{//如果之前光标没有在输入框的开头，则需要跟新到开头
				m_curShowStart = m_curInsertPoint;
				upDateInsertShowPosition();
			}else
			{
				m_curShowStart = m_curInsertPoint;
			}
		}else
		if(m_curInsertPoint > m_curShowStart + m_canShowLength)
		{//向后移动
			if(tempOldPosition != m_curShowStart + m_canShowLength)
			{//如果之前的光标不在输入框结尾则需要更新光标到结尾
				m_curShowStart = m_curInsertPoint - m_canShowLength;
				if(!XString::isAtUnicodeEnd(m_curString,m_curShowStart)) ++m_curShowStart; //如果头临界、则多移动一位
				upDateInsertShowPosition();
			}else
			{
				m_curShowStart = m_curInsertPoint - m_canShowLength;
				if(!XString::isAtUnicodeEnd(m_curString,m_curShowStart)) ++m_curShowStart; //如果头临界、则多移动一位
				upDateInsertShowPosition();
			}
		}
		upDataShowStr();
	}else
	{//不需要修改显示范围
		upDateInsertShowPosition();
	}
}
void XEdit::upDataSelectShow()
{
	int tempHead = getSelectHead();
	int tempEnd = getSelectEnd();
	if(tempHead < m_curShowStart) tempHead = m_curShowStart;
	if(tempHead >= m_curShowStart + m_canShowLength)
	{//这个情况不需要显示
		tempHead = m_curShowStart + m_canShowLength;
	}
	if(tempEnd <= m_curShowStart)
	{//这种情况也不需要显示
		tempEnd = m_curShowStart;
	}
	if(tempEnd > m_curShowStart + m_canShowLength) tempEnd = m_curShowStart + m_canShowLength;
	//更具实际的情况计算选区位置和选区图片需要放置的尺寸
	if(m_withoutTex)
	{
		if(tempEnd - tempHead < m_canShowLength)
		{
			m_selectRect.set(m_position.x + m_textPosition.x * m_scale.x + (tempHead - m_curShowStart) * m_curTextWidth,
				m_position.y + m_textPosition.y * m_scale.y - m_curTextHeight * 0.5f,(tempEnd - tempHead) * m_curTextWidth,m_curTextHeight);
		}else
		{
			m_selectRect.set(m_position.x + m_textPosition.x * m_scale.x + (tempHead - m_curShowStart) * m_curTextWidth,
				m_position.y + m_textPosition.y * m_scale.y - m_curTextHeight * 0.5f,m_canShowLength * m_curTextWidth,m_curTextHeight);
		}
	}else
	{
		m_spriteSelect.setPosition(m_position.x + m_textPosition.x * m_scale.x + (tempHead - m_curShowStart) * m_curTextWidth,
			m_position.y + m_textPosition.y * m_scale.y - m_curTextHeight * 0.5f);
		if(tempEnd - tempHead < m_canShowLength)
		{
			m_spriteSelect.setScale((tempEnd - tempHead) * m_curTextWidth /m_editSelect->textureSize.x,
				m_curTextHeight / m_editSelect->textureSize.y);
		}else
		{
			m_spriteSelect.setScale(m_canShowLength * m_curTextWidth /m_editSelect->textureSize.x,
				m_curTextHeight / m_editSelect->textureSize.y);
		}
	}
}
XBool XEdit::mouseProc(float x,float y,XMouseState mouseState)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	if(m_withAction && m_isInAction) return XFalse;	//如果支持动作播放而且正在播放动画那么不接受鼠标控制
	//计算鼠标响应区域的位置
//	XRect temp;
//	temp.set(m_objRect.left,m_objRect.top,m_objRect.left + m_objRect.getWidth() * m_scale.x,m_objRect.top + m_objRect.getHeight() * m_scale.y);
	if(m_mouseRightButtonMenu != NULL)
	{//右键菜单处理在这里进行
		XBool flagMouseMenu = m_mouseRightButtonMenu->getVisible();
		bool ret = false;
		if(mouseState == MOUSE_RIGHT_BUTTON_UP)
		{//如果是右键单击产生动作需要在控件范围内
			if(m_curMouseRect.isInRect(x,y) && m_isBeChoose)
				ret = m_mouseRightButtonMenu->mouseProc(x,y,mouseState);
		}else
		{
			ret = m_mouseRightButtonMenu->mouseProc(x,y,mouseState);
		}
		if(!flagMouseMenu && m_mouseRightButtonMenu->getVisible())
		{//右键菜单出现，这里需要整理右键菜单的状态
			m_mouseRightButtonMenu->setMnuState(m_haveSelect,0); 
			m_mouseRightButtonMenu->setMnuState(m_haveSelect,1); 
#ifdef XEE_OS_WINDOWS	//有选择才能粘贴
			m_mouseRightButtonMenu->setMnuState(XString::getTextFromClipboard() != "",2); 
#else
			m_mouseRightButtonMenu->setMnuState(m_copyString[0] != '\0',2); 
#endif
			m_mouseRightButtonMenu->setMnuState(m_haveSelect,3); 
		}
		//if(flagMouseMenu && !m_mouseRightButtonMenu->getVisible())
		if(ret)
		{//按下了快捷键(这里实现对应菜单项的功能)
			mouseRightMenuProc();//右键菜单的响应
			if(m_mouseRightButtonMenu->getLastChoose() != -1)//如果菜单有操作，则这里的消息不后传
				return XTrue;	//对右键菜单的操作生效的话，则这个鼠标消息不会左重复处理
		}
	} 
	if(m_curMouseRect.isInRect(x,y))
	{//鼠标在范围内点击
		if(!m_isMouseInRect)
		{
			m_isMouseInRect = XTrue;
			m_comment.setShow();
			setCommentPos(x,y + 16.0f);
		}
		if(mouseState != MOUSE_MOVE && m_comment.getIsShow())
			m_comment.disShow();	//鼠标的任意操作都会让说明框消失
		//下面是左右键都可以出发
		if(mouseState == MOUSE_LEFT_BUTTON_DOWN || 
			mouseState == MOUSE_LEFT_BUTTON_DCLICK || 
			mouseState == MOUSE_RIGHT_BUTTON_DOWN)
		{//点击	
			if(!m_isBeChoose)
			{
				m_isBeChoose = XTrue;//如果控件没有处于激活状态则激活
				if(m_withAction)
					m_insertActionMD.set(10.0f,1.0f,0.005f);//,MOVE_DATA_MODE_SQRT2_MULT);
			}
			if(m_curStringLength > 0 &&//字符串中有字符时，可以同通过鼠标选择插入位置(这里的常数10是为了方便选择头或者尾)
				m_curMouseRect.isInRect(x,y))
			{//鼠标按下动作有效
				//计算插入位置
				if(x <= m_position.x + m_textPosition.x * m_scale.x)
				{//头
					if(m_curInsertPoint != m_curShowStart) changeInsertPoint(m_curShowStart - m_curInsertPoint);
				}else
				if(x >= m_position.x + (m_mouseRect.right - m_textEdgeWidth) * m_scale.x)
				{//尾
					if(m_curStringLength <= m_canShowLength)
					{
						if(m_curInsertPoint != m_curStringLength) changeInsertPoint(m_curStringLength - m_curInsertPoint);
					}else
					{//则到显示的结束
						if(m_curInsertPoint != m_curShowStart + m_canShowLength) changeInsertPoint(m_curShowStart + m_canShowLength - m_curInsertPoint);
					}
				}else
				{//范围内
					//这里使用0.5的常数是因为目前使用的是英文输入但是使用的字体是中文字体库，所以要加少一半的占用像素
					//为了提高选择时的感觉，这里错位半个字符的宽度，使选择的时候更具有感觉
					int tempInsertPoint = (x + m_curTextWidth * 0.5f - m_position.x - m_textPosition.x * m_scale.x) / m_curTextWidth;
					if(tempInsertPoint + m_curShowStart > m_curStringLength)
					{
						if(m_curInsertPoint != m_curStringLength) changeInsertPoint(m_curStringLength - m_curInsertPoint);
					}else
					{
						if(m_curInsertPoint != tempInsertPoint + m_curShowStart) changeInsertPoint(tempInsertPoint + m_curShowStart - m_curInsertPoint);
					}
				}
				switch(mouseState)
				{
				case MOUSE_LEFT_BUTTON_DOWN://如果是左键按下，则可能是片选属性
					m_haveSelect = XFalse;
					m_selectEnd = m_curInsertPoint;
					m_selectStart = m_curInsertPoint;
					m_selectMouseDown = XTrue;
					break;
				case MOUSE_LEFT_BUTTON_DCLICK:	//这里设置为双击全选
					chooseAllStr();
					break;
				case MOUSE_RIGHT_BUTTON_DOWN://如果是右键按下操作不再范围内也将会造成片选结束
					if(m_curInsertPoint > getSelectEnd() || m_curInsertPoint < getSelectHead())
						m_haveSelect = XFalse;
					break;
				}
			}
		}
	}else
	{//如果在范围外点击则去激活
		if(m_isMouseInRect)
		{
			m_isMouseInRect = XFalse;
			m_comment.disShow();
		}
		if((mouseState == MOUSE_LEFT_BUTTON_DOWN || 
			mouseState == MOUSE_LEFT_BUTTON_DCLICK || 
			mouseState == MOUSE_RIGHT_BUTTON_DOWN) &&//点击
			m_isBeChoose)
		{
			//setLostFocus();	//设置失去焦点
			m_isBeChoose = XFalse;//如果控件没有处于激活状态则激活
			if(m_withAction)
				m_insertActionMD.set(1.0f,0.0f,0.002f);
		}
	}
	//双击为全选
	if(mouseState == MOUSE_LEFT_BUTTON_DOWN)// || mouseState == MOUSE_LEFT_BUTTON_DCLICK)
	{//任何鼠标左键按下操作都回造成片选结束
		if(m_haveSelect) m_haveSelect = XFalse;
	}

	if(mouseState == MOUSE_MOVE &&
		m_selectMouseDown)
	{//如果鼠标按下，并且拖动，则作为片选动作处理
		//计算片选的位置
		if(x <= m_position.x + m_textPosition.x * m_scale.x)
		{//头
			if(m_curShowStart > 0 && x <= m_position.x + m_textPosition.x * m_scale.x - m_textEdgeWidth) 
			{//如果超过头，可以向前移动
				m_selectEnd = m_curShowStart - 1;
				if(!XString::isAtUnicodeEnd(m_curString,m_selectEnd)) --m_selectEnd;
			}else 
			{
				m_selectEnd = m_curShowStart;
			}
		}else
		if(x >= m_position.x + (m_mouseRect.right - m_textEdgeWidth) * m_scale.x)
		{//尾
			if(m_curShowStart + m_canShowLength >= m_curStringLength)
			{//如果字符串没有显示满
				m_selectEnd = m_curStringLength;
			}else
			{
				if(x >= m_position.x + (m_mouseRect.right - m_textEdgeWidth) * m_scale.x + m_textEdgeWidth)
				{//如果超过尾可以向后移动
					m_selectEnd = m_curShowStart + m_canShowLength + 1;
					if(!XString::isAtUnicodeEnd(m_curString,m_selectEnd)) ++m_selectEnd;
				}else
				{
					m_selectEnd = m_curShowStart + m_canShowLength;
				}
			}
		}else
		{//这里使用0.5的常数是因为目前使用的是英文输入但是使用的字体是中文字体库，所以要加少一半的占用像素
			int tempEnd = m_selectEnd;
			int tempPosition = (x - m_position.x - m_textPosition.x * m_scale.x) / m_curTextWidth;
			if(tempPosition + m_curShowStart >= m_curStringLength) m_selectEnd = m_curStringLength;
			else m_selectEnd = m_curShowStart + tempPosition;

			if(!XString::isAtUnicodeEnd(m_curString,m_selectEnd))
			{
				if(m_selectEnd > tempEnd) ++m_selectEnd;
				else --m_selectEnd;
			}
		}
		if(m_selectEnd != m_curInsertPoint) changeInsertPoint(m_selectEnd - m_curInsertPoint);	//插入符号也要跟随变化
		if(m_selectEnd != m_selectStart)
		{//片选有效
			m_haveSelect = XTrue;
			upDataSelectShow();
		}else m_haveSelect = XFalse;	//片选无效
	}
	if(m_isBeChoose && (mouseState == MOUSE_LEFT_BUTTON_UP || mouseState == MOUSE_RIGHT_BUTTON_UP) &&//如果是弹起事件，则不论是否在范围内，只要控件处于激活状态，则会做出反应
		m_selectMouseDown)//片选结束判断
	{
		m_haveSelect = (m_selectEnd != m_selectStart);
		//if(m_selectEnd != m_selectStart) m_haveSelect = XTrue;	//片选有效
		//else m_haveSelect = XFalse;	//片选无效
		m_selectMouseDown = XFalse;
	}
	return XTrue;
}
XBool XEdit::keyboardProc(int keyOrder,XKeyState keyState)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	if(m_withAction && m_isInAction) return XFalse;	//如果支持动作播放而且正在播放动画那么不接受鼠标控制

	if(m_mouseRightButtonMenu != NULL &&
		m_mouseRightButtonMenu->getVisible())
	{//如果右键菜单可见,则所有按键都不会对输入框造成影响
		//m_mouseRightButtonMenu->keyboardProc(keyOrder,keyState);
		//if(!m_mouseRightButtonMenu->getVisible())
		if(m_mouseRightButtonMenu->keyboardProc(keyOrder,keyState))
		{//按下了快捷键(这里实现对应菜单项的功能)
			mouseRightMenuProc();//右键菜单的响应
		}
		return XFalse;
	}
	if(!m_isBeChoose) return XFalse;
	switch(keyState)
	{
	case KEY_STATE_DOWN:
		switch(keyOrder)
		{
		case XKEY_RSHIFT:
		case XKEY_LSHIFT://shift键按下
			m_keyShiftState = 1;
			break;
		case XKEY_CAPSLOCK://CapsLock键按下
			m_keyCapsLockState = 1;
			break;
		case XKEY_NUMLOCK://XKEY_NUMLOCK键按下
			m_keyNumLockState = 1;
			break;
		case XKEY_RCTRL:
		case XKEY_LCTRL://Ctrl键按下
			m_keyCtrlState = 1;
			break;
		case XKEY_UNKNOWN:
			break;
		default:
		//	printf("keyDown:%d\n",keyOrder);
			m_curKeyDown = XTrue;
			m_curKeyDownTimer = 0;
			m_curKeyRepTimer = 1000;
			m_curKey = keyOrder;
			if(keyProc(keyOrder)) return XTrue;	//按键弹起时才作相应 
			break;
		}
		break;
	case KEY_STATE_UP:
		switch(keyOrder)
		{
		case XKEY_RSHIFT:
		case XKEY_LSHIFT://shift键弹起
			m_keyShiftState = 0;
			break;
		case XKEY_CAPSLOCK://CapsLock键弹起
			m_keyCapsLockState = 0;
			break;
		case XKEY_NUMLOCK://XKEY_NUMLOCK键弹起
			m_keyNumLockState = 0;
			break;
		case XKEY_RCTRL:
		case XKEY_LCTRL://Ctrl键弹起
			m_keyCtrlState = 0;
			break;
		case XKEY_MENU://菜单键被按下，则相当于是鼠标右击(菜单键弹起才有响应)
			if(m_mouseRightButtonMenu != NULL)
				m_mouseRightButtonMenu->mouseProc(m_position.x + m_textPosition.x * m_scale.x + (m_curInsertPoint - m_curShowStart) * m_curTextWidth,
					m_position.y + m_textPosition.y * m_scale.y,MOUSE_RIGHT_BUTTON_UP);
			break;
		case XKEY_UNKNOWN:
			break;
		default:
			m_curKeyDown = XFalse;
			break;
		}
		break;
	}

	return XFalse;
}
XBool XEdit::keyProc(int keyOrder)
{
	int TextChange = 0;		//文本内容是否发生变化
	if(m_keyCtrlState == 0)
	{//ctrl没有按下的所有输入才能处理
		if(m_haveSelect == 0)
		{//没有片选的时候
			if(m_curStringLength < m_maxInputStringLength - 2)
			{//如果输入长度小于最大长度，则可以继续输入，否则只能输入一些功能
				//这里是可打印字符的处理			
				TextChange = keyJudgement(keyOrder);
			}
		}else
		{//存在片选的时候
			if(m_curStringLength - getSelectLength() < m_maxInputStringLength - 2)
			{//如果输入长度小于最大长度，则可以继续输入，否则只能输入一些功能
				//这里是可打印字符的处理			
				TextChange = keyJudgement(keyOrder);
			}
		}
	}else
	{//ctrl按下的所有输入作为功能键处理
		switch(keyOrder)
		{
		case XKEY_A://ctrl + a 全选	//20150625新加入
			chooseAllStr();
			break;
		case XKEY_C://ctrl + c 复制
			stringCopy();
			break;
		case XKEY_V://ctrl + v 粘贴  //这里需要根据输入框类型判断当前输入是否有效(尚未完成)
			if(stringPlaster()) TextChange = 1;
			break;
		}
	}
	if(keyOrder ==  XKEY_BACKSPACE)
	{//回退键
		if(!m_haveSelect)
		{//没有片选的时候
			if(m_curInsertPoint > 0)
			{
				if(!XString::isAtUnicodeEnd(m_curString,m_curInsertPoint - 1))
				{
					m_curStringLength -= 2;
					changeInsertPoint(-2);
					if(m_curStringLength == m_curInsertPoint) m_curString[m_curInsertPoint] = '\0';	//字符串末尾
					else strcpy(m_curString + m_curInsertPoint,m_curString + m_curInsertPoint + 2);
				}else
				{
					-- m_curStringLength;
					changeInsertPoint(-1);
					if(m_curStringLength == m_curInsertPoint) m_curString[m_curInsertPoint] = '\0';	//字符串末尾
					else strcpy(m_curString + m_curInsertPoint,m_curString + m_curInsertPoint + 1);
				}
				TextChange = 1;
			}
		}else
		{//存在片选的时候
			if(!m_selectMouseDown)
			{//片选已经结束
				//1、删除片选内容
				deleteSelectStr();
				m_haveSelect = XFalse;	//片选结束
				TextChange = 1;
			}else
			{//片选尚未结束
				//作结束动作
				if(m_selectEnd != m_selectStart)
				{//片选有效
					m_haveSelect = XTrue;
					//1、删除片选内容
					deleteSelectStr();
				}else m_haveSelect = XFalse;	//片选无效

				m_haveSelect = XFalse;	//片选结束
				TextChange = 1;
				//然后再作重新开始动作
				m_selectEnd = m_curInsertPoint;
				m_selectStart = m_curInsertPoint;
				m_selectMouseDown = XTrue;
			}
		}
	}else
	if(keyOrder == XKEY_DELETE || (m_keyShiftState == m_keyNumLockState && keyOrder ==  XKEY_N_PERIOD))
	{//Delete按键
		if(!m_haveSelect)
		{//没有片选的时候
			if(m_curStringLength == m_curInsertPoint)
			{//字符串末尾不作处理
			}else
			{
				//这里需要判断后面那个字符是否为中文，如果为中文的话，则需要删除两个字符
				if(m_curString[m_curInsertPoint] < 0)
				{
					strcpy(m_curString + m_curInsertPoint,m_curString + m_curInsertPoint + 2);
					m_curStringLength -= 2;
				}else
				{
					strcpy(m_curString + m_curInsertPoint,m_curString + m_curInsertPoint + 1);
					-- m_curStringLength;
				}
				TextChange = 1;
			}
		}else
		{//存在片选的时候
			if(!m_selectMouseDown)
			{//片选已经结束
				//1、删除片选内容
				deleteSelectStr();
				m_haveSelect = XFalse;	//片选结束
				TextChange = 1;
			}else
			{//片选尚未结束
				//作结束动作
				if(m_selectEnd != m_selectStart)
				{//片选有效
					m_haveSelect = XTrue;
					//1、删除片选内容
					deleteSelectStr();
				}else m_haveSelect = XFalse;	//片选无效

				TextChange = 1;
				//然后再作重新开始动作
				m_haveSelect = XFalse;
				m_selectEnd = m_curInsertPoint;
				m_selectStart = m_curInsertPoint;
				m_selectMouseDown = XTrue;
			}
		}
	}else
	if(keyOrder ==  XKEY_LEFT || (m_keyShiftState == m_keyNumLockState && keyOrder ==  XKEY_N4))
	{//向左案件弹起(字符串不会改变，但是会改变插入的位置)
		if(m_keyShiftState == 0)
		{//如果没有按下shift键
			if(!m_selectMouseDown)
			{
				if(!m_haveSelect) changeInsertPoint(-1);	//没有片选,移动光标的位置
				else
				{//存在片选
				//	m_curInsertPoint = m_selectStart;
				//	m_spriteInsert.setPosition(m_objRect.left + (m_edgeDistance.left + m_textPosition.x) * m_scale.x + m_curInsertPoint * m_caption.m_scale.x * m_caption.m_showSize.x * 0.5,
				//		m_objRect.top + (m_edgeDistance.top + m_textPosition.y) * m_scale.y);
					changeInsertPoint(m_selectStart - m_curInsertPoint);
					m_haveSelect = XFalse;
				}
			}
		}else
		{//如果按下shift键则为片选动作
			if(!m_haveSelect)
			{//标记开始片选
				m_haveSelect = XFalse;
				m_selectEnd = m_curInsertPoint;
				m_selectStart = m_curInsertPoint;
				//然后移动光标，并修改片选状态
				if(m_curInsertPoint > 0)
				{
					changeInsertPoint(-1);
					m_selectEnd = m_curInsertPoint;
				}
				if(m_selectEnd != m_selectStart)
				{//片选有效
					m_haveSelect = XTrue;
					upDataSelectShow();
				}else m_haveSelect = XFalse;//片选无效
			}else
			{//修改片选结果
				//然后移动光标，并修改片选状态
				if(m_curInsertPoint > 0)
				{
					changeInsertPoint(-1);
					m_selectEnd = m_curInsertPoint;
				}
				if(m_selectEnd != m_selectStart)
				{//片选有效
					m_haveSelect = XTrue;
					upDataSelectShow();
				}else m_haveSelect = XFalse;//片选无效
			}
		}
	}else
	if(keyOrder ==  XKEY_RIGHT || (m_keyShiftState == m_keyNumLockState && keyOrder ==  XKEY_N6))
	{//向左案件弹起(字符串不会改变，但是会改变插入的位置)
		if(m_keyShiftState == 0)
		{//如果没有按下shift键
			if(!m_selectMouseDown)
			{
				if(!m_haveSelect) changeInsertPoint(1);	//没有片选,移动光标的位置
				else
				{//存在片选
					changeInsertPoint(m_selectEnd - m_curInsertPoint);
					m_haveSelect = XFalse;
				}
			}
		}else
		{//如果按下shift键则为片选动作
			if(!m_haveSelect)
			{//标记开始片选
				m_haveSelect = XFalse;
				m_selectEnd = m_curInsertPoint;
				m_selectStart = m_curInsertPoint;
				//然后移动光标，并修改片选状态
				if(m_curInsertPoint < m_curStringLength)
				{
					changeInsertPoint(1);
					m_selectEnd = m_curInsertPoint;
				}
				if(m_selectEnd != m_selectStart)
				{//片选有效
					m_haveSelect = XTrue;
					upDataSelectShow();
				}else m_haveSelect = XFalse;//片选无效
			}else
			{//修改片选结果
				//然后移动光标，并修改片选状态
				if(m_curInsertPoint < m_curStringLength)
				{
					changeInsertPoint(1);
					m_selectEnd = m_curInsertPoint;
				}
				if(m_selectEnd != m_selectStart)
				{//片选有效
					m_haveSelect = XTrue;
					upDataSelectShow();
				}else m_haveSelect = XFalse;	//片选无效
			}
		}
	}else
	if(keyOrder ==  XKEY_HOME || (m_keyShiftState == m_keyNumLockState && keyOrder ==  XKEY_N7))
	{//home
		if(m_keyShiftState == 0)
		{//如果shift没有按下
			if(!m_selectMouseDown)
			{//如果没有鼠标选择(如果有鼠标选择则按键无效)
				if(m_curInsertPoint != 0) changeInsertPoint(-m_curInsertPoint);
				if(m_haveSelect) m_haveSelect = XFalse;
			}
		}else
		{//如果shift按下了
			if(!m_haveSelect)
			{//标记开始片选
				m_haveSelect = XFalse;
				m_selectEnd = m_curInsertPoint;
				m_selectStart = m_curInsertPoint;
				//然后移动光标，并修改片选状态
				if(m_curInsertPoint > 0)
				{//将选择移动到头
					changeInsertPoint(-m_curInsertPoint);
					m_selectEnd = m_curInsertPoint;
				}
				if(m_selectEnd != m_selectStart)
				{//片选有效
					m_haveSelect = XTrue;
					upDataSelectShow();
				}else m_haveSelect = XFalse;	//片选无效
			}else
			{//修改片选结果
				//然后移动光标，并修改片选状态
				if(m_curInsertPoint > 0)
				{//将选择移动到头
					changeInsertPoint(-m_curInsertPoint);
					m_selectEnd = m_curInsertPoint;
				}
				if(m_selectEnd != m_selectStart)
				{//片选有效
					m_haveSelect = XTrue;
					upDataSelectShow();
				}else m_haveSelect = XFalse;	//片选无效
			}
		}
	}else
	if(keyOrder ==  XKEY_END || (m_keyShiftState == m_keyNumLockState && keyOrder ==  XKEY_N1))
	{//end(鼠标没有片选动作时才能生效)
		if(m_keyShiftState == 0)
		{//如果shift没有按下
			if(!m_selectMouseDown)
			{//如果没有鼠标选择(如果有鼠标选择则按键无效)
				if(m_curInsertPoint != m_curStringLength) changeInsertPoint(m_curStringLength - m_curInsertPoint);
				if(m_haveSelect) m_haveSelect = XFalse;
			}
		}else
		{//如果shift按下了
			if(!m_haveSelect)
			{//标记开始片选
				m_haveSelect = XFalse;
				m_selectEnd = m_curInsertPoint;
				m_selectStart = m_curInsertPoint;
				//然后移动光标，并修改片选状态
				if(m_curInsertPoint < m_curStringLength)
				{//将选择移动到头
					changeInsertPoint(m_curStringLength - m_curInsertPoint);
					m_selectEnd = m_curInsertPoint;
				}
				if(m_selectEnd != m_selectStart)
				{//片选有效
					m_haveSelect = XTrue;
					upDataSelectShow();
				}else m_haveSelect = XFalse;	//片选无效
			}else
			{//修改片选结果
				//然后移动光标，并修改片选状态
				if(m_curInsertPoint < m_curStringLength)
				{//将选择移动到头
					changeInsertPoint(m_curStringLength - m_curInsertPoint);
					m_selectEnd = m_curInsertPoint;
				}
				if(m_selectEnd != m_selectStart)
				{//片选有效
					m_haveSelect = XTrue;
					upDataSelectShow();
				}else m_haveSelect = XFalse;	//片选无效
			}
		}
	}

	if(TextChange != 0)
	{
		//如果字符串发生改变，需要更新字符串的内容
		upDataShowStr();	//跟新字符串的显示
		//if(m_funInputChenge != NULL) m_funInputChenge(m_pClass,m_objectID);
		if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,EDT_INPUT_CHANGE);
		else XCtrlManager.eventProc(m_objectID,EDT_INPUT_CHANGE);
		//当数据发生变化的时候播放动作
		if(m_withAction)
		{//这里测试一个动态效果
		//	m_isInAction = XTrue;
			m_lightMD.set(1.0f,2.0f,0.002f,MOVE_DATA_MODE_SIN_MULT);
			m_oldPos = m_position;
			m_oldSize = m_scale;
		}
		return XTrue;
	}
	if(keyOrder ==  XKEY_RETURN || keyOrder == XKEY_N_ENTER)
	{//回车键
		//if(m_haveSelect) m_haveSelect = XFalse;	//回车键取消选择
		//if(m_funInputOver != NULL) m_funInputOver(m_pClass,m_objectID);
		if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,EDT_INPUT_OVER);
		else XCtrlManager.eventProc(m_objectID,EDT_INPUT_OVER);
		if(m_withAction)
		{//这里测试一个动态效果
		//	m_isInAction = XTrue;
			m_lightMD.set(1.0f,2.0f,0.002f,MOVE_DATA_MODE_SIN_MULT);
			m_oldPos = m_position;
			m_oldSize = m_scale;
		}
		return XTrue;
	}
	return XFalse;
}
void XEdit::setLostFocus() 
{
	if(m_haveSelect) m_haveSelect = XFalse;
	m_isBeChoose = XFalse;
	m_selectMouseDown = XFalse;
	m_curKeyDown = XFalse;
	//printf("失去焦点!\n");
	//下面这里在弹出窗口时会造成bug，窗口角点的bug
	if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,EDT_INPUT_OVER);
	else XCtrlManager.eventProc(m_objectID,EDT_INPUT_OVER);
}
void XEdit::setScale(float x,float y)
{
	if(x <= 0 || y <= 0 ||
		!m_isInited) return;	//如果没有初始化直接退出
	m_scale.set(x,y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_scale.x,m_position.y + m_textPosition.y * m_scale.y);
	m_caption.setScale(m_textSize.x * m_scale.x,m_textSize.y * m_scale.y);
	m_curMouseRect.set(m_position.x + m_mouseRect.left * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y,
		m_position.x + m_mouseRect.right * m_scale.x,m_position.y + m_mouseRect.bottom * m_scale.y);
	m_curTextWidth = m_caption.getTextSize().x * m_caption.getScale().x * 0.5f;
	m_curTextHeight = m_caption.getTextSize().y * m_caption.getScale().y;
//	m_selectText.setPosition(m_objRect.left + (m_edgeDistance.left + m_textPosition.x) * m_scale.x,m_objRect.top + (m_edgeDistance.top + m_textPosition.y) * m_scale.y);
//	m_selectText.setScale(m_scale);
	//m_spriteBackGround.setPosition(m_position);
	if(!m_withoutTex) m_spriteBackGround.setScale(m_scale);
	upDateInsertShowPosition();
	//m_spriteSelect.setScale(m_scale);
	upDataSelectShow();
	if(!m_withoutTex) m_spriteInsert.setScale(m_textSize.x * m_scale.x,m_textSize.y * m_scale.y);

	updateChildScale();
}
XBool XEdit::setACopy(const XEdit &temp)			//设置为一个拷贝公用部分资源		
{
	if(&temp == this ||	//防止自身赋值
		!temp.m_isInited) return XFalse;
	if(m_isInited) release();//如果自身已经初始化，则需要释放资源
	if(!XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
		XObjManager.addAObject(this);
#endif
	}

	m_isInited = temp.m_isInited;	//是否初始化
	if(m_resInfo != NULL) XResManager.releaseResource(m_resInfo);
	m_resInfo = XResManager.copyResource(temp.m_resInfo);
	m_withoutTex = temp.m_withoutTex;
	m_selectRect = temp.m_selectRect;

	if(temp.m_mouseRightButtonMenu != NULL)
	{
		m_mouseRightButtonMenu = XMem::createMem<XMouseRightButtonMenu>();
		if(m_mouseRightButtonMenu == NULL) return XFalse;

		m_mouseRightButtonMenu->setACopy(* (temp.m_mouseRightButtonMenu));	//鼠标右键菜单
		//从控件管理器中将这个控件注销掉
		XCtrlManager.decreaseAObject(m_mouseRightButtonMenu);	//注销这个物件
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(m_mouseRightButtonMenu);
#endif
	}

	if(!m_caption.setACopy(temp.m_caption))	return XFalse;		//显示当前输入的字符串
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_curString = XMem::createArrayMem<char>(m_maxInputStringLength);
	strcpy(m_curString,temp.m_curString);
	m_tempCurString = XMem::createArrayMem<char>(m_maxInputStringLength);
	if(m_tempCurString == NULL) 
	{
		XMem::XDELETE_ARRAY(m_curString);
		return XFalse;
	}
	strcpy(m_tempCurString,temp.m_tempCurString);
	m_curShowString	= XMem::createArrayMem<char>(m_maxInputStringLength);
	if(m_curShowString == NULL)
	{
		XMem::XDELETE_ARRAY(m_curString);
		XMem::XDELETE_ARRAY(m_tempCurString);
		return XFalse;
	}
	strcpy(m_curShowString,temp.m_curShowString);

	m_curInsertPoint = temp.m_curInsertPoint;	//当前选择输入的位置,整个字符串的整体位置
	m_curStringLength = temp.m_curStringLength;	//当前输入字符串的长度
	m_haveSelect = temp.m_haveSelect;		//是否在字符串中有选择
	m_selectMouseDown = temp.m_selectMouseDown;	//鼠标是否按下准备片选
	m_selectStart = temp.m_selectStart;		//选择的起始位置
	m_selectEnd = temp.m_selectEnd;		//选择的结束位置

	m_editNormal = temp.m_editNormal;		//输入框普通状态
	m_editDisable = temp.m_editDisable;		//输入框无效状态
	m_editSelect = temp.m_editSelect;		//输入框片选颜色
	m_editInsert = temp.m_editInsert;		//输入框插入标记
	m_editUpon = temp.m_editUpon;		//输入框插入标记

	m_editType = temp.m_editType;		//输入框的类型

	m_spriteBackGround.setACopy(temp.m_spriteBackGround);	//用于显示输入框的背景贴图
	m_spriteSelect.setACopy(temp.m_spriteSelect);		//用于显示选择内容的背景颜色
	m_spriteInsert.setACopy(temp.m_spriteInsert);		//用于显示插入符号
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteBackGround);
	XObjManager.decreaseAObject(&m_spriteSelect);
	XObjManager.decreaseAObject(&m_spriteInsert);
#endif
	m_textPosition = temp.m_textPosition;			//文字显示的位置，是相对于控件的位置来定的
	m_textSize = temp.m_textSize;				//文字显示的尺寸，这个尺寸会与空间的缩放尺寸叠加

	//m_funInputChenge = temp.m_funInputChenge;		//输入内容发生改变的时候调用
	//m_funInputOver = temp.m_funInputOver;		//确认输入结束之后的时候调用
	//m_pClass = temp.m_pClass;

	m_timer = temp.m_timer;	//这是插入符号闪烁时使用的时间标记，这个时间使用不准确的计时方式
	//下面是用于按键判断的中间变量
	m_keyShiftState = temp.m_keyShiftState;		//shift按键的状态	0弹起，1按下
	m_keyCapsLockState = temp.m_keyCapsLockState;	//CapsLock按键的状态	1有效，0无效
	m_keyNumLockState = temp.m_keyNumLockState;		//NumLock按键的状态	1有效，0无效
	m_keyCtrlState = temp.m_keyCtrlState;		//Ctrl按键的状态 0弹起，1按下

	m_canShowLength = temp.m_canShowLength;		//输入框中可以显示的字符串的长度
	m_curShowStart = temp.m_curShowStart;			//当前输入框中显示的字符串的起始位置

	m_textColor = temp.m_textColor;
	m_curTextWidth = temp.m_curTextWidth;
	m_curTextHeight = temp.m_curTextHeight;

	m_oldPos = temp.m_oldPos;				//动作播放时的位置
	m_oldSize = temp.m_oldSize;			//动作播放时的大小
	m_lightMD = temp.m_lightMD;
	m_lightRect = temp.m_lightRect;

	m_isPassword = temp.m_isPassword;

	return XTrue;
}
void XEdit::insertChar(const char * ch,int len)
{//这里需要根据输入框类型判断当前输入是否有效(尚未完成)
	if(!m_isBeChoose) return;
	if(ch == NULL ||
		len <= 0 ||
		m_curStringLength + len >= m_maxInputStringLength - 1) return;
	if(m_haveSelect)
	{//存在片选状态
		if(!m_selectMouseDown)
		{//片选已经结束
			//1、删除片选内容
			deleteSelectStr();
			m_haveSelect = XFalse;	//片选结束

			if(m_curInsertPoint == m_curStringLength)
			{//末尾
				memcpy(m_curString + m_curInsertPoint,ch,len);
			//	++ m_curInsertPoint;
				m_curStringLength += len;
				m_curString[m_curStringLength] = '\0';
			}else
			{//中间插入
				//strcpy(m_curString + m_curInsertPoint + 1,m_curString + m_curInsertPoint);
				strcpy(m_tempCurString,m_curString + m_curInsertPoint);
				memcpy(m_curString + m_curInsertPoint,ch,len);
				strcpy(m_curString + m_curInsertPoint + len,m_tempCurString);
			//	++ m_curInsertPoint;
				m_curStringLength += len;
			}
			changeInsertPoint(len);
		}else
		{//片选尚未结束
			//作结束动作
			if(m_selectEnd != m_selectStart)
			{//片选有效
				//插入按键
				//1、删除片选内容
				deleteSelectStr();
				m_haveSelect = XFalse;	//片选结束
				if(m_curInsertPoint == m_curStringLength)
				{//末尾
					memcpy(m_curString + m_curInsertPoint,ch,len);
					//++ m_curInsertPoint;
					m_curStringLength += len;
					m_curString[m_curStringLength] = '\0';
				}else
				{//中间插入
					//strcpy(m_curString + m_curInsertPoint + 1,m_curString + m_curInsertPoint);
					strcpy(m_tempCurString,m_curString + m_curInsertPoint);
					memcpy(m_curString + m_curInsertPoint,ch,len);
					strcpy(m_curString + m_curInsertPoint + len,m_tempCurString);
					//++ m_curInsertPoint;
					m_curStringLength += len;
				}
				changeInsertPoint(len);
			}
			//然后再作重新开始动作
			m_haveSelect = XFalse;
			m_selectEnd = m_curInsertPoint;
			m_selectStart = m_curInsertPoint;
			m_selectMouseDown = XTrue;
		}
	}else
	{//不存在片选
		if(m_curInsertPoint == m_curStringLength)
		{//末尾
			memcpy(m_curString + m_curInsertPoint,ch,len);
			//++ m_curInsertPoint;
			m_curStringLength += len;
			m_curString[m_curStringLength] = '\0';
		}else
		{//中间插入
			//strcpy(m_curString + m_curInsertPoint + 1,m_curString + m_curInsertPoint);
			strcpy(m_tempCurString,m_curString + m_curInsertPoint);
			//m_curString[m_curInsertPoint] = tempChar;
			memcpy(m_curString + m_curInsertPoint,ch,len);
			strcpy(m_curString + m_curInsertPoint + len,m_tempCurString);
			//++ m_curInsertPoint;
			m_curStringLength += len;
		}
		changeInsertPoint(len);

		if(m_selectMouseDown)
		{//如果按下了片选鼠标，则光标要随着走
			m_selectEnd = m_curInsertPoint;
			m_selectStart = m_curInsertPoint;
		}
	}

	//如果字符串发生改变，需要更新字符串的内容
	upDataShowStr();	//跟新字符串的显示
	//if(m_funInputChenge != NULL) m_funInputChenge(m_pClass,m_objectID);
	if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,EDT_INPUT_CHANGE);
	else XCtrlManager.eventProc(m_objectID,EDT_INPUT_CHANGE);
	//当数据发生变化的时候播放动作
	if(m_withAction)
	{//这里测试一个动态效果
	//	m_isInAction = XTrue;
		m_lightMD.set(1.0f,2.0f,0.002f,MOVE_DATA_MODE_SIN_MULT);
		m_oldPos = m_position;
		m_oldSize = m_scale;
	}

	return;
}
#ifdef XEE_OS_WINDOWS	//windows系统下通过输入法来输入字符
int XEdit::keyJudgement(int)
#else
int XEdit::keyJudgement(int keyOrder)
#endif
{
#ifdef XEE_OS_WINDOWS	//windows系统下通过输入法来输入字符
	return 0;
#endif
#ifdef XEE_OS_LINUX
	int TextChange = 0;
	char tempChar = 0;
//	printf("%d\n",keyOrder);
	if(keyOrder == XKEY_SPACE)
	{
		tempChar = ' ';
		TextChange = 1;
	}else
	if(m_keyShiftState == 0)
	{
		TextChange = 1;
		switch(keyOrder)
		{
		case XKEY_0:tempChar = '0';break;
		case XKEY_1:tempChar = '1';break;
		case XKEY_2:tempChar = '2';break;
		case XKEY_3:tempChar = '3';break;
		case XKEY_4:tempChar = '4';break;
		case XKEY_5:tempChar = '5';break;
		case XKEY_6:tempChar = '6';break;
		case XKEY_7:tempChar = '7';break;
		case XKEY_8:tempChar = '8';break;
		case XKEY_9:tempChar = '9';break;
		//符号按键
		case XKEY_BACKQUOTE:tempChar = '`';break;
		case XKEY_MINUS:tempChar = '-';break;
		case XKEY_EQUALS:tempChar = '=';break;
		case XKEY_LEFTBRACKET:tempChar = '[';break;
		case XKEY_RIGHTBRACKET:tempChar = ']';break;
		case XKEY_SEMICOLON:tempChar = ';';break;
		case XKEY_QUOTE:tempChar = '\'';break;
		case XKEY_BACKSLASH:tempChar = '\\';break;
		case XKEY_COMMA:tempChar = ',';break;
		case XKEY_PERIOD:tempChar = '.';break;
		case XKEY_SLASH:tempChar = '/';break;
		default:TextChange = 0;break;
		}
	}else
	{
		TextChange = 1;
		switch(keyOrder)
		{
		case XKEY_0:tempChar = ')';break;
		case XKEY_1:tempChar = '!';break;
		case XKEY_2:tempChar = '@';break;
		case XKEY_3:tempChar = '#';break;
		case XKEY_4:tempChar = '$';break;
		case XKEY_5:tempChar = '%';break;
		case XKEY_6:tempChar = '^';break;
		case XKEY_7:tempChar = '&';break;
		case XKEY_8:tempChar = '*';break;
		case XKEY_9:tempChar = '(';break;
			//符号按键
		case XKEY_BACKQUOTE:tempChar = '~';break;
		case XKEY_MINUS:tempChar = '_';break;
		case XKEY_EQUALS:tempChar = '+';break;
		case XKEY_LEFTBRACKET:tempChar = '{';break;
		case XKEY_RIGHTBRACKET:tempChar = '}';break;
		case XKEY_SEMICOLON:tempChar = ':';break;
		case XKEY_QUOTE:tempChar = '\"';break;
		case XKEY_BACKSLASH:tempChar = '|';break;
		case XKEY_COMMA:tempChar = '<';break;
		case XKEY_PERIOD:tempChar = '>';break;
		case XKEY_SLASH:tempChar = '?';break;
		default:TextChange = 0;break;
		}
	}
	//字母键
	if(TextChange == 0)
	{
	//	if((m_keyShiftState == 0 && m_keyCapsLockState == 0) || (m_keyShiftState == 1 || m_keyCapsLockState == 1))
		if(m_keyShiftState == m_keyCapsLockState)
		{
			TextChange = 1;
			switch(keyOrder)
			{
			case XKEY_a:tempChar = 'a';break;
			case XKEY_b:tempChar = 'b';break;
			case XKEY_c:tempChar = 'c';break;
			case XKEY_d:tempChar = 'd';break;
			case XKEY_e:tempChar = 'e';break;
			case XKEY_f:tempChar = 'f';break;
			case XKEY_g:tempChar = 'g';break;
			case XKEY_h:tempChar = 'h';break;
			case XKEY_i:tempChar = 'i';break;
			case XKEY_j:tempChar = 'j';break;
			case XKEY_k:tempChar = 'k';break;
			case XKEY_l:tempChar = 'l';break;
			case XKEY_m:tempChar = 'm';break;
			case XKEY_n:tempChar = 'n';break;
			case XKEY_o:tempChar = 'o';break;
			case XKEY_p:tempChar = 'p';break;
			case XKEY_q:tempChar = 'q';break;
			case XKEY_r:tempChar = 'r';break;
			case XKEY_s:tempChar = 's';break;
			case XKEY_t:tempChar = 't';break;
			case XKEY_u:tempChar = 'u';break;
			case XKEY_v:tempChar = 'v';break;
			case XKEY_w:tempChar = 'w';break;
			case XKEY_x:tempChar = 'x';break;
			case XKEY_y:tempChar = 'y';break;
			case XKEY_z:tempChar = 'z';break;
			default:TextChange = 0;break;
			}
		}else
		{
			TextChange = 1;
			switch(keyOrder)
			{
			case XKEY_a:tempChar = 'A';break;
			case XKEY_b:tempChar = 'B';break;
			case XKEY_c:tempChar = 'C';break;
			case XKEY_d:tempChar = 'D';break;
			case XKEY_e:tempChar = 'E';break;
			case XKEY_f:tempChar = 'F';break;
			case XKEY_g:tempChar = 'G';break;
			case XKEY_h:tempChar = 'H';break;
			case XKEY_i:tempChar = 'I';break;
			case XKEY_j:tempChar = 'J';break;
			case XKEY_k:tempChar = 'K';break;
			case XKEY_l:tempChar = 'L';break;
			case XKEY_m:tempChar = 'M';break;
			case XKEY_n:tempChar = 'N';break;
			case XKEY_o:tempChar = 'O';break;
			case XKEY_p:tempChar = 'P';break;
			case XKEY_q:tempChar = 'Q';break;
			case XKEY_r:tempChar = 'R';break;
			case XKEY_s:tempChar = 'S';break;
			case XKEY_t:tempChar = 'T';break;
			case XKEY_u:tempChar = 'U';break;
			case XKEY_v:tempChar = 'V';break;
			case XKEY_w:tempChar = 'W';break;
			case XKEY_x:tempChar = 'X';break;
			case XKEY_y:tempChar = 'Y';break;
			case XKEY_z:tempChar = 'Z';break;
			default:TextChange = 0;break;
			}
		}
	}
	//小键盘
	if(TextChange == 0)
	{
		if(m_keyShiftState != m_keyNumLockState)
		{
			TextChange = 1;
			switch(keyOrder)
			{
			case XKEY_KP0:tempChar = '0';break;
			case XKEY_KP1:tempChar = '1';break;
			case XKEY_KP2:tempChar = '2';break;
			case XKEY_KP3:tempChar = '3';break;
			case XKEY_KP4:tempChar = '4';break;
			case XKEY_KP5:tempChar = '5';break;
			case XKEY_KP6:tempChar = '6';break;
			case XKEY_KP7:tempChar = '7';break;
			case XKEY_KP8:tempChar = '8';break;
			case XKEY_KP9:tempChar = '9';break;
			case XKEY_KP_PERIOD:tempChar = '.';break;
			default:TextChange = 0;break;
			}
		}
		if(TextChange == 0)
		{
			TextChange = 1;
			switch(keyOrder)
			{
			case XKEY_KP_DIVIDE:tempChar = '/';break;
			case XKEY_KP_MULTIPLY:tempChar = '*';break;
			case XKEY_KP_MINUS:tempChar = '-';break;
			case XKEY_KP_PLUS:tempChar = '+';break;
			default:TextChange = 0;break;
			}
		}
	}
	if(TextChange != 0) insertChar(&tempChar,1);
	return TextChange;
#endif
}
void XEdit::setInputLen(int len)
{//设置输入框的长度
	if(len <= 0) return;
	m_mouseRect.right = m_mouseRect.left + len;
	m_textPosition.set(m_mouseRect.left + m_textEdgeWidth,m_mouseRect.top + m_mouseRect.getHeight() * 0.5f);			//文字显示的位置，是相对于控件的位置来定的
	m_curMouseRect.set(m_position.x + m_mouseRect.left * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y,
	m_position.x + m_mouseRect.right * m_scale.x,m_position.y + m_mouseRect.bottom * m_scale.y);
	m_canShowLength = (m_mouseRect.getWidth() - 2.0f * m_textEdgeWidth) * m_scale.x / m_curTextWidth;
	//这里需要改变几个值
	if(m_curInsertPoint < m_curShowStart) m_curShowStart = m_curInsertPoint;
	if(m_curInsertPoint - m_curShowStart > m_canShowLength) m_curShowStart = m_curInsertPoint - m_canShowLength;
	upDataShowStr();
	upDateInsertShowPosition();
}
void XEdit::update(float stepTime)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	m_comment.update(stepTime);
	if(m_isEnable && m_isBeChoose)
	{//输入光标闪烁的计时
		m_timer += stepTime;
		if(m_timer > 500) m_timer -= 500;
		if(m_curKeyDown)
		{
			m_curKeyDownTimer += stepTime;
			if(m_curKeyDownTimer > 500)
			{//开始重复计数
				m_curKeyRepTimer += stepTime;
				if(m_curKeyRepTimer >= 50)
				{
					m_curKeyRepTimer = 0;
					keyProc(m_curKey);
				}
			}
		}
	}
	if(!m_lightMD.getIsEnd())
	{
		m_lightMD.move(stepTime);
		XVector2 pos(m_oldPos.x + m_mouseRect.getWidth() * 0.5f * m_oldSize.x,
			m_oldPos.y + m_mouseRect.getHeight() * 0.5f * m_oldSize.y);
		XVector2 size(m_mouseRect.getWidth() * m_oldSize.x * m_lightMD.getCurData() * 0.5f,
			m_mouseRect.getHeight() * m_oldSize.y * m_lightMD.getCurData() * 0.5f);
		m_lightRect.set(pos.x - size.x,pos.y - size.y,pos.x + size.x,pos.y + size.y);
	}
	if(!m_insertActionMD.getIsEnd())
	{
		m_insertActionMD.move(stepTime);
	}
	if(m_mouseRightButtonMenu != NULL)
		m_mouseRightButtonMenu->update(stepTime);
}
#if !WITH_INLINE_FILE
#include "XEdit.inl"
#endif
}