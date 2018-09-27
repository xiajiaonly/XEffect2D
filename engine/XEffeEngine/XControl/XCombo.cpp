#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XCombo.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "XResourcePack.h"
namespace XE{
void XCombo::funCtrlProc(void*pClass,int id,int eventID)
{
	if(eventID != XButton::BTN_MOUSE_DOWN) return;
	XCombo &pPar = *(XCombo *) pClass;
	if(id == pPar.m_buttom[0].getControlID())
	{
		pPar.changeDownMenuState();
	//	pPar.m_mouseButtonDownIsUseable = 1;
		return;
	}
	if(id == pPar.m_buttom[1].getControlID())
	{
		if(pPar.m_menuStartDrawOrder > 0)
		{
			-- pPar.m_menuStartDrawOrder;
			pPar.updateString();
			if(pPar.m_menuStartDrawOrder + pPar.m_menuDrawSum < pPar.m_menuSum) pPar.m_buttom[2].enable();
			if(pPar.m_menuStartDrawOrder == 0) pPar.m_buttom[1].disable();
		}
	// 	pPar.m_mouseButtonDownIsUseable = 1;
		return;
	}
	if(id == pPar.m_buttom[2].getControlID())
	{
		if(pPar.m_menuStartDrawOrder + pPar.m_menuDrawSum < pPar.m_menuSum)
		{
			++ pPar.m_menuStartDrawOrder;
			pPar.updateString();
			if(pPar.m_menuStartDrawOrder > 0) pPar.m_buttom[1].enable();
			if(pPar.m_menuStartDrawOrder + pPar.m_menuDrawSum >= pPar.m_menuSum) pPar.m_buttom[2].disable();
		}
	// 	pPar.m_mouseButtonDownIsUseable = 1;
		return;
	}
	for(int i = 0;i < pPar.m_menuDrawSum;++ i)
	{
		if(id == pPar.m_buttom[i + 3].getControlID())
		{
			//pPar.m_isDrawDownMenu = XFalse;
			pPar.changeDownMenuState();
			if(pPar.m_curChooseOrder != i + pPar.m_menuStartDrawOrder)
			{
				pPar.m_curChooseOrder = i + pPar.m_menuStartDrawOrder;
				pPar.updateString();
				if(pPar.m_eventProc != NULL) pPar.m_eventProc(pPar.m_pClass,pPar.m_objectID,CMB_DATA_CHANGE);
				else XCtrlManager.eventProc(pPar.m_objectID,CMB_DATA_CHANGE);
			}
			break;
		}
	}
// 	pPar.m_mouseButtonDownIsUseable = 1;
}

XComboSkin::XComboSkin()
	:m_isInited(XFalse)
	, comboInputNormal(NULL)				//下拉菜单输入框的普通状态
	, comboInputDisable(NULL)			//无效状态下的按钮贴图
{}
XBool XComboSkin::init(const char *inputNormal,const char *inputDisable,
		 const char *downButtonNormal,const char *downButtonOn,const char *downButtonDown,const char *downButtonDisable,
		 const char *downMenuUpNormal,const char *downMenuUpOn,const char *downMenuUpDown,const char *downMenuUpDisable,
		 const char *downMenuNormal,const char *downMenuOn,const char *downMenuDown,const char *downMenuDisable,
		 const char *downMenuDownNormal,const char *downMenuDownOn,const char *downMenuDownDown,const char *downMenuDownDisable,
		 XResPos resPos)
{
	if(m_isInited) return XFalse;	//防止重复初始化
	if(inputNormal == NULL || inputDisable == NULL ||
		downButtonNormal == NULL || downButtonOn == NULL || downButtonDown == NULL || downButtonDisable == NULL ||
		downMenuUpNormal == NULL || downMenuUpOn == NULL || downMenuUpDown == NULL || downMenuUpDisable == NULL ||
		downMenuNormal == NULL || downMenuOn == NULL || downMenuDown == NULL || downMenuDisable == NULL ||
		downMenuDownNormal == NULL || downMenuDownOn == NULL || downMenuDownDown == NULL || downMenuDownDisable == NULL)
	{//贴图的数据不能为空
		return XFalse;
	}
	int ret = 1;
	//下面依次载入所有的贴图资源
	if((comboInputNormal = createATextureData(inputNormal,resPos)) == NULL) ret = 0;
	if(ret != 0 &&
		(comboInputDisable = createATextureData(inputDisable,resPos)) == NULL) ret = 0;
	if(ret != 0 && 
		!downButtonTex.init(downButtonNormal,downButtonDown,downButtonOn,downButtonDisable,resPos))
	{
		ret = 0;
	}
	if(ret != 0 && 
		!downMenuUpButtonTex.init(downMenuUpNormal,downMenuUpDown,downMenuUpOn,downMenuUpDisable,resPos))
	{
		ret = 0;
	}
	if(ret != 0 && 
		!downMenuButtonTex.init(downMenuNormal,downMenuDown,downMenuOn,downMenuDisable,resPos))
	{
		ret = 0;
	}
	if(ret != 0 && 
		!downMenuDownButtonTex.init(downMenuDownNormal,downMenuDownDown,downMenuDownOn,downMenuDownDisable,resPos))
	{
		ret = 0;
	}
	if(ret == 0)
	{
		releaseTex();
		return XFalse;
	}

	m_isInited = XTrue;
	return XTrue;
}
#define COMBO_CONFIG_FILENAME "Combo.txt"
bool XComboSkin::loadFromFolder(const char *filename,XResPos resPos)	//从文件夹中载入资源
{
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,COMBO_CONFIG_FILENAME);
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
	if((comboInputNormal = createATextureData(tempFilename,resPos)) == NULL)
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
	if((comboInputDisable = createATextureData(tempFilename,resPos)) == NULL)
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
	if(!downButtonTex.initEx(tempFilename,resPos))
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
	if(!downMenuUpButtonTex.initEx(tempFilename,resPos))
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
	if(!downMenuButtonTex.initEx(tempFilename,resPos))
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
	if(!downMenuDownButtonTex.initEx(tempFilename,resPos))
	{//资源读取失败
		releaseTex();
		fclose(fp);
		return XFalse;
	}
	//读取两组数据
	int l,t,r,b;
	if(fscanf(fp,"%d,%d,%d,%d,",&l,&t,&r,&b) != 4) {fclose(fp);return XFalse;}
	m_mouseRect.set(l,t,r,b);
	//所有数据读取完成
	fclose(fp);
	return true;
}
bool XComboSkin::loadFromPacker(const char *filename,XResPos resPos)	//从压缩包中载入资源
{
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,COMBO_CONFIG_FILENAME);
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
	if((comboInputNormal = createATextureData(tempFilename,resPos)) == NULL)
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
	if((comboInputDisable = createATextureData(tempFilename,resPos)) == NULL)
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
	if(!downButtonTex.initEx(tempFilename,resPos))
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
	if(!downMenuUpButtonTex.initEx(tempFilename,resPos))
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
	if(!downMenuButtonTex.initEx(tempFilename,resPos))
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
	if(!downMenuDownButtonTex.initEx(tempFilename,resPos))
	{//资源读取失败
		releaseTex();
		XMem::XDELETE_ARRAY(p);
		return XFalse;
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
bool XComboSkin::loadFromWeb(const char *filename,XResPos resPos)		//从网页中读取资源
{
	return false;
}
XBool XComboSkin::initEx(const char *filename,XResPos resPos)
{
	if(m_isInited || filename == NULL) return XFalse;
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,COMBO_CONFIG_FILENAME);
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
XCombo::XCombo()
	:m_isInited(XFalse)
	, m_comboInputNormal(NULL)			//下拉菜单输入框的普通状态
	, m_comboInputDisable(NULL)			//无效状态下的按钮贴图
	, m_menuData("")
	, m_buttom(NULL)
	, m_resInfo(NULL)
	, m_withoutTex(XFalse)
	, m_curStyle(CMB_STYLE_DOWN)
{
	m_ctrlType = CTRL_OBJ_COMBO;
}
void XCombo::release()
{
	if(!m_isInited) return;
	XMem::XDELETE_ARRAY(m_buttom);
	//XMem::XDELETE_ARRAY(m_menuData);
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
XBool XCombo::init(const XComboSkin &tex,	//控件的贴图
		const XVec2& position,		//控件的位置
		const XRect& inputArea,		//输入框的有效范围
		const XRect& downButtonArea,	//下拉按钮的响应范围
		const XRect& downMenuUpArea,	//上翻页的响应范围
		const XRect& downMenuArea,		//选择项的响应范围
		const XRect& downMenuDownArea,	//下翻页的响应范围
		int menuSum,					//下拉菜单中的选项的数量
		int drawMenuSum,				//下拉菜单中显示的菜单项的数量
		const XFontUnicode& font,		//显示文字使用的字体
		float fontSize)				//字体的大小
{
	if(m_isInited ||				//防止重复初始化
		tex.comboInputNormal == NULL ||	//如果传入的贴图为空，或者贴图的内容为空，则返回失败
		menuSum <= 0 ||					//菜单中不能没有菜单项
		drawMenuSum <= 0 || drawMenuSum > menuSum ||	
		fontSize <= 0 ||
		inputArea.getHeight() <= 0 || inputArea.getWidth() <= 0) return XFalse;	//空间必须要有一个响应区间，不然会出现除零错误
	m_position = position;

	//复制贴图
	m_comboInputNormal = tex.comboInputNormal;					//下拉菜单输入框的普通状态
	m_comboInputDisable = tex.comboInputDisable;				//无效状态下的按钮贴图

	//拷贝数值
	m_scale.set(1.0f);
	m_menuSum = menuSum;
	m_menuDrawSum = drawMenuSum;
	m_menuStartDrawOrder = 0;
	m_inputArea = inputArea;
	m_downButtonArea = downButtonArea;
	m_downMenuUpArea = downMenuUpArea;
	m_downMenuArea = downMenuArea;
	m_downMenuDownArea = downMenuDownArea;
	m_withoutTex = XFalse;
	m_comment.init();
	m_mouseRect = inputArea;
	if(!m_caption.setACopy(font)) return XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //设置字体居中对齐
	m_textColor.set(0.0f,1.0f);
	m_caption.setColor(m_textColor);							//设置字体的颜色为黑色
	m_caption.setString("");
	m_caption.setPosition(m_position + XVec2(m_inputArea.left + m_comboLeftDistance,
		m_inputArea.top + m_inputArea.getHeight() * 0.5f + m_comboTopDistance) * m_scale);
	m_textSize.set(fontSize);
	m_caption.setScale(m_textSize * m_scale);

	//建立按钮
	m_buttom = XMem::createArrayMem<XButton>(m_menuDrawSum + 3);
	if(m_buttom == NULL)return XFalse;
	//下面初始化所有按钮
	//XButtonTexture tempButtonTex;
	//tempButtonTex.set(m_comboDownButtonNormal,m_comboDownButtonDown,m_comboDownButtonOn,m_comboDownButtonDisable);
	if(!m_buttom[0].init(m_position + XVec2((m_inputArea.right - m_downButtonArea.left) * m_scale.x,0.0f),
		m_downButtonArea,tex.downButtonTex,"",font,m_textSize.x,XVec2::zero)) return XFalse;		//下拉菜单左边的下拉菜单的按钮
	m_buttom[0].setScale(m_scale);
	m_buttom[0].setEventProc(funCtrlProc,this);
	//tempButtonTex.set(m_comboDownMenuUpNormal,m_comboDownMenuUpDown,m_comboDownMenuUpOn,m_comboDownMenuUpDisable);
	if(!m_buttom[1].init(XVec2::zero,m_downMenuUpArea,tex.downMenuUpButtonTex,"",font,m_textSize.x,XVec2::zero)) return XFalse;		//下拉菜单，菜单上面的按钮
	m_buttom[1].setScale(m_scale);
	m_buttom[1].setEventProc(funCtrlProc,this);
	//tempButtonTex.set(m_comboDownMenuDownNormal,m_comboDownMenuDownDown,m_comboDownMenuDownOn,m_comboDownMenuDownDisable);
	if(!m_buttom[2].init(XVec2::zero,m_downMenuDownArea,tex.downMenuDownButtonTex,"",font,m_textSize.x,XVec2::zero)) return XFalse;		//下拉菜单，菜单下面面的按钮
	m_buttom[2].setScale(m_scale);
	m_buttom[2].setEventProc(funCtrlProc,this);

	//tempButtonTex.set(m_comboDownMenuNormal,m_comboDownMenuDown,m_comboDownMenuOn,m_comboDownMenuDisable);
	for(int i = 0;i < m_menuDrawSum;++ i)
	{
		if(!m_buttom[3 + i].init(XVec2::zero,m_downMenuArea,tex.downMenuButtonTex,"",font,m_textSize.x,
			m_downMenuArea.getCenter())) return XFalse;		//下拉菜单，菜单下面面的按钮
		m_buttom[3 + i].setScale(m_scale);
		m_buttom[3 + i].setEventProc(funCtrlProc,this);
	}
	updateMenuBtnPosition();
	//从控件管理器中注销这些按钮
	for(int i = 0;i < m_menuDrawSum + 3;++ i)
	{
		XCtrlManager.decreaseAObject(&(m_buttom[i]));	//注销这个物件
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&m_buttom[i]);
#endif
	}

	//计算菜单中可以显示的文字数量
	m_menuTextWidth = (downMenuArea.getWidth() - m_comboLeftDistance) * m_scale.x/ (m_caption.getTextSize().x * m_caption.getScale().x * 0.5f);	//这个需要计算
	if(m_menuTextWidth < 3) return XFalse;
	m_captionTextWidth = (downMenuArea.getWidth() - downButtonArea.getWidth() - m_comboLeftDistance) * m_scale.x/ (m_caption.getTextSize().x * m_caption.getScale().x * 0.5f);	//这个需要计算
	if(m_captionTextWidth < 3) return XFalse;

	m_sprite.init(m_comboInputNormal->texture.m_w,m_comboInputNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_sprite);
#endif
	m_sprite.setPosition(m_position);
	m_sprite.setScale(m_scale);
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);

	//数据存储
	m_menuData = "";
//	m_menuData = XMem::createArrayMem<char>(m_comboMaxMenuLength);
//	if(m_menuData == NULL) return XFalse;		//内存分配失败
//	m_menuData[0] = '\0';
	char tempMenu[32] = "MenuXXXX\n";
	for(int i = 0;i < m_menuSum;++ i)
	{//初始化所有的菜单项的内容为空格
		sprintf_s(tempMenu,32,"Menu%04d\n",i);
		m_menuData = m_menuData + tempMenu;
	}
	m_isDrawDownMenu = XFalse;
	m_curChooseOrder = 0;

	m_isVisible = m_isEnable = m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;

	updateString();
	return XTrue;
}
void XCombo::updateMenuBtnPosition()
{
	XVec2 tempPosition;
	switch (m_curStyle)
	{
	case CMB_STYLE_DOWN:
		m_buttom[1].setPosition(m_position.x,m_position.y + (m_inputArea.bottom - m_downMenuUpArea.top) * m_scale.y);
		tempPosition.set(m_position.x,m_position.y + (m_inputArea.bottom + m_downMenuUpArea.getHeight() 
			+ m_downMenuArea.getHeight() * m_menuDrawSum - m_downMenuDownArea.top) * m_scale.y);
		m_buttom[2].setPosition(tempPosition);
		for(int i = 0;i < m_menuDrawSum;++ i)
		{
			tempPosition.set(m_position.x,m_position.y + (m_inputArea.bottom + m_downMenuUpArea.getHeight() 
				+ m_downMenuArea.getHeight() * i - m_downMenuArea.top) * m_scale.y);
			m_buttom[3 + i].setPosition(tempPosition);
		}
		m_allArea.set(m_position + m_inputArea.getLT() * m_scale,
			m_position + XVec2(m_downMenuArea.right, m_inputArea.top + m_inputArea.getHeight() + m_downMenuUpArea.getHeight()
				+ m_downMenuArea.getHeight() * m_menuDrawSum + m_downMenuDownArea.getHeight()) * m_scale);
		break;
	case CMB_STYLE_UP:
		tempPosition.set(m_position.x,m_position.y + (m_inputArea.top - m_downMenuDownArea.getHeight() 
			- m_downMenuArea.getHeight() * m_menuDrawSum - m_downMenuDownArea.bottom) * m_scale.y);
		m_buttom[1].setPosition(tempPosition);
		m_buttom[2].setPosition(m_position.x,m_position.y + (m_inputArea.top - m_downMenuUpArea.bottom) * m_scale.y);
		for(int i = 0;i < m_menuDrawSum;++ i)
		{
			tempPosition.set(m_position.x,m_position.y + (m_inputArea.top - m_downMenuDownArea.getHeight() 
				- m_downMenuArea.getHeight() * (i + 1) - m_downMenuArea.top) * m_scale.y);
			m_buttom[3 + i].setPosition(tempPosition);
		}
		m_allArea.set(m_position.x + m_inputArea.left * m_scale.x,
			m_position.y + (m_inputArea.top - m_downMenuUpArea.getHeight() 
			- m_downMenuArea.getHeight() * m_menuDrawSum - m_downMenuDownArea.getHeight()) * m_scale.y,
			m_position.x + m_downMenuArea.right * m_scale.x,
			m_position.y + m_inputArea.bottom * m_scale.y);
		break;
	}
}
void XCombo::setInputLen(int len)
{
	if(!m_withoutTex || len <= 0 || !m_isInited) return;
	if((len + DEFAULT_COMBO_BT_SIZE - m_comboLeftDistance) * m_scale.x / 
		(m_caption.getTextSize().x * m_caption.getScale().x * 0.5f) < 3) return;
	if((len + DEFAULT_COMBO_BT_SIZE - m_downButtonArea.getWidth() - m_comboLeftDistance) * m_scale.x / 
		(m_caption.getTextSize().x * m_caption.getScale().x * 0.5f) < 3) return;
	m_inputArea.set(0.0f,0.0f,len,DEFAULT_COMBO_BT_SIZE);

	m_downMenuUpArea.set(0.0f,0.0f,len + DEFAULT_COMBO_BT_SIZE,DEFAULT_COMBO_UD_HEIGHT);
	m_downMenuArea.set(0.0f,0.0f,len + DEFAULT_COMBO_BT_SIZE,DEFAULT_COMBO_MN_HEIGHT);
	m_downMenuDownArea.set(0.0f,0.0f,len + DEFAULT_COMBO_BT_SIZE,DEFAULT_COMBO_UD_HEIGHT);
	m_mouseRect.set(0.0f,0.0f,len,DEFAULT_COMBO_BT_SIZE);
	switch (m_curStyle)
	{
	case CMB_STYLE_DOWN:
		m_allArea.set(m_position + m_inputArea.getLT() * m_scale,
			m_position + XVec2(m_downMenuArea.right, m_inputArea.top + m_inputArea.getHeight() + m_downMenuUpArea.getHeight()
				+ m_downMenuArea.getHeight() * m_menuDrawSum + m_downMenuDownArea.getHeight()) * m_scale);
		break;
	case CMB_STYLE_UP:
		m_allArea.set(m_position.x + m_inputArea.left * m_scale.x,
			m_position.y + (m_inputArea.top - m_downMenuUpArea.getHeight() 
			- m_downMenuArea.getHeight() * m_menuDrawSum - m_downMenuDownArea.getHeight()) * m_scale.y,
			m_position.x + m_downMenuArea.right * m_scale.x,
			m_position.y + m_inputArea.bottom * m_scale.y);
		break;
	}

	m_buttom[0].setPosition(m_position.x + (m_inputArea.right - m_downButtonArea.left) * m_scale.x,m_position.y);
	for(int i = 1;i < m_menuDrawSum + 3;++ i)
	{
		m_buttom[i].setWidth(m_downMenuUpArea.getWidth());
	}
	//计算菜单中可以显示的文字数量
	m_menuTextWidth = (m_downMenuArea.getWidth() - m_comboLeftDistance) * m_scale.x/ (m_caption.getTextSize().x * m_caption.getScale().x * 0.5f);	//这个需要计算
//	if(m_menuTextWidth < 3) return;		//有问题的数据
	m_captionTextWidth = (m_downMenuArea.getWidth() - m_downButtonArea.getWidth() - m_comboLeftDistance) * m_scale.x/ (m_caption.getTextSize().x * m_caption.getScale().x * 0.5f);	//这个需要计算
//	if(m_captionTextWidth < 3) return;	//有问题的数据
	updateString();
}
XBool XCombo::initWithoutSkin(int inputLen,
		int menuSum,					//下拉菜单中的选项的数量
		int drawMenuSum,				//下拉菜单中显示的菜单项的数量
		const XFontUnicode& font,		//显示文字使用的字体
		float fontSize)
{
	if(m_isInited ||				//防止重复初始化
		inputLen <= 0 ||
		menuSum <= 0 ||					//菜单中不能没有菜单项
		drawMenuSum <= 0 || drawMenuSum > menuSum ||
		fontSize <= 0) return XFalse;
	m_position.reset();
	//拷贝数值
	m_scale.set(1.0f);
	m_menuSum = menuSum;
	m_menuDrawSum = drawMenuSum;
	m_menuStartDrawOrder = 0;
	m_inputArea.set(0.0f,0.0f,inputLen,DEFAULT_COMBO_BT_SIZE);
	m_downButtonArea.set(0.0f,DEFAULT_COMBO_BT_SIZE);

	m_downMenuUpArea.set(0.0f,0.0f,inputLen + DEFAULT_COMBO_BT_SIZE,DEFAULT_COMBO_UD_HEIGHT);
	m_downMenuArea.set(0.0f,0.0f,inputLen + DEFAULT_COMBO_BT_SIZE,DEFAULT_COMBO_MN_HEIGHT);
	m_downMenuDownArea.set(0.0f,0.0f,inputLen + DEFAULT_COMBO_BT_SIZE,DEFAULT_COMBO_UD_HEIGHT);
	m_withoutTex = XTrue;
	m_comment.init();
	m_mouseRect.set(0.0f,0.0f,inputLen,DEFAULT_COMBO_BT_SIZE);

	if(!m_caption.setACopy(font)) return XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //设置字体居中对齐
	m_textColor.set(0.0f,1.0f);
	m_caption.setColor(m_textColor);							//设置字体的颜色为黑色
	m_caption.setString("");
	m_caption.setPosition(m_position + XVec2(m_inputArea.left + m_comboLeftDistance,
		m_inputArea.top + m_inputArea.getHeight() * 0.5f + m_comboTopDistance) * m_scale);
	m_textSize.set(fontSize);
	m_caption.setScale(m_textSize * m_scale);
	//建立按钮
	m_buttom = XMem::createArrayMem<XButton>(m_menuDrawSum + 3);
	if(m_buttom == NULL) return XFalse;
	//下面初始化所有按钮
	if(!m_buttom[0].initWithoutSkin("",font,m_textSize.x,m_downButtonArea,m_downButtonArea.getCenter())) return XFalse;	//下拉菜单左边的下拉菜单的按钮
	m_buttom[0].setPosition(m_position.x + (m_inputArea.right - m_downButtonArea.left) * m_scale.x,m_position.y);
	m_buttom[0].setScale(m_scale);
	m_buttom[0].setEventProc(funCtrlProc,this);
	m_buttom[0].setSymbol(BTN_SYMBOL_DOWN);
	if(!m_buttom[1].initWithoutSkin("",font,m_textSize.x,m_downMenuUpArea,m_downMenuUpArea.getCenter())) return XFalse;//下拉菜单，菜单上面的按钮
	m_buttom[1].setScale(m_scale);
	m_buttom[1].setEventProc(funCtrlProc,this);
	m_buttom[1].setSymbol(BTN_SYMBOL_UP);
	if(!m_buttom[2].initWithoutSkin("",font,m_textSize.x,m_downMenuDownArea,m_downMenuDownArea.getCenter())) return XFalse;		//下拉菜单，菜单下面面的按钮
	m_buttom[2].setScale(m_scale);
	m_buttom[2].setEventProc(funCtrlProc,this);
	m_buttom[2].setSymbol(BTN_SYMBOL_DOWN);

	for(int i = 0;i < m_menuDrawSum;++ i)
	{
		if(!m_buttom[3 + i].initWithoutSkin("",font,m_textSize.x,m_downMenuArea,
			m_downMenuArea.getLT() + m_downMenuArea.getSize() * 0.5f)) return XFalse;		//下拉菜单，菜单下面面的按钮
		m_buttom[3 + i].setScale(m_scale);
		m_buttom[3 + i].setEventProc(funCtrlProc,this);
	}
	updateMenuBtnPosition();

	//从控件管理器中注销这些按钮
	for(int i = 0;i < m_menuDrawSum + 3;++ i)
	{
		XCtrlManager.decreaseAObject(&(m_buttom[i]));	//注销这个物件
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&m_buttom[i]);
#endif
	}

	//计算菜单中可以显示的文字数量
	m_menuTextWidth = (m_downMenuArea.getWidth() - m_comboLeftDistance) * m_scale.x/ (m_caption.getTextSize().x * m_caption.getScale().x * 0.5f);	//这个需要计算
	if(m_menuTextWidth < 3) return XFalse;
	m_captionTextWidth = (m_downMenuArea.getWidth() - m_downButtonArea.getWidth() - m_comboLeftDistance) * m_scale.x/ (m_caption.getTextSize().x * m_caption.getScale().x * 0.5f);	//这个需要计算
	if(m_captionTextWidth < 3) return XFalse;

	//数据存储
	m_menuData = "";
	//m_menuData = XMem::createArrayMem<char>(m_comboMaxMenuLength);
	//if(m_menuData == NULL) return XFalse;		//内存分配失败
	//m_menuData[0] = '\0';
	char tempMenu[32] = "MenuXXXX\n";
	for(int i = 0;i < m_menuSum;++ i)
	{//初始化所有的菜单项的内容为空格
		sprintf_s(tempMenu,32,"Menu%04d\n",i);
		m_menuData = m_menuData + tempMenu;
	}
	m_isDrawDownMenu = XFalse;
	m_curChooseOrder = 0;

	m_isVisible = m_isEnable = m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;

	updateString();
	return XTrue;
}
bool XCombo::resetMenuSum(int menuSum,int showMenuSum)
{
	if(!m_isInited) return false;
	if(menuSum <= 0 || showMenuSum <= 0 || showMenuSum > menuSum) return false;
	XMem::XDELETE_ARRAY(m_buttom);
	m_menuSum = menuSum;
	m_menuDrawSum = showMenuSum;
	//建立按钮
	m_buttom = XMem::createArrayMem<XButton>(m_menuDrawSum + 3);
	if(m_buttom == NULL) return XFalse;
	//下面初始化所有按钮
	if(!m_buttom[0].initWithoutSkin("",m_caption,m_textSize.x,m_downButtonArea,m_downButtonArea.getCenter())) return XFalse;	//下拉菜单左边的下拉菜单的按钮
	m_buttom[0].setPosition(m_position.x + (m_inputArea.right - m_downButtonArea.left) * m_scale.x,m_position.y);
	m_buttom[0].setScale(m_scale);
	m_buttom[0].setEventProc(funCtrlProc,this);
	m_buttom[0].setSymbol(BTN_SYMBOL_DOWN);
	if(!m_buttom[1].initWithoutSkin("",m_caption,m_textSize.x,m_downMenuUpArea,m_downMenuUpArea.getCenter())) return XFalse;//下拉菜单，菜单上面的按钮
	m_buttom[1].setScale(m_scale);
	m_buttom[1].setEventProc(funCtrlProc,this);
	m_buttom[1].setSymbol(BTN_SYMBOL_UP);
	if(!m_buttom[2].initWithoutSkin("",m_caption,m_textSize.x,m_downMenuDownArea,m_downMenuDownArea.getCenter())) return XFalse;		//下拉菜单，菜单下面面的按钮
	m_buttom[2].setScale(m_scale);
	m_buttom[2].setEventProc(funCtrlProc,this);
	m_buttom[2].setSymbol(BTN_SYMBOL_DOWN);

	for(int i = 0;i < m_menuDrawSum;++ i)
	{
		if(!m_buttom[3 + i].initWithoutSkin("",m_caption,m_textSize.x,m_downMenuArea,
			m_downMenuArea.getLT() + m_downMenuArea.getSize() * 0.5f)) return XFalse;		//下拉菜单，菜单下面面的按钮
		m_buttom[3 + i].setScale(m_scale);
		m_buttom[3 + i].setEventProc(funCtrlProc,this);
	}
	//从控件管理器中注销这些按钮
	for(int i = 0;i < m_menuDrawSum + 3;++ i)
	{
		XCtrlManager.decreaseAObject(&(m_buttom[i]));	//注销这个物件
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&m_buttom[i]);
#endif
	}

	updateMenuBtnPosition();


	if(m_curChooseOrder >= m_menuSum) m_curChooseOrder = 0;
	int sum = 0;
	for(unsigned int i = 0;i < m_menuData.size();++ i)
	{
		if(m_menuData[i] == '\n')
		{
			++ sum;
			if(sum == m_menuSum)
			{
				m_menuData = m_menuData.substr(0,i + 1);
				break;
			}
		}
	}
	if(sum < m_menuSum)
	{//不够的部分不足，这里有一处内存检查，可能会造成越界
		for(int i = 0;i < m_menuSum - sum;++ i)
		{
			m_menuData += " \n";
		}
	}
	updateString();
	return true;
}
void XCombo::draw()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	m_buttom[0].draw();
	if(m_withoutTex)
	{
		if(m_isEnable)
		{
			XRender::drawFillRectExA(m_position + m_inputArea.getLT() * m_scale,
				m_inputArea.getSize() * m_scale, XCCS::blackOnColor * m_color, true);
		}else
		{
			XRender::drawFillRectExA(m_position + m_inputArea.getLT() * m_scale,
				m_inputArea.getSize() * m_scale, XCCS::specialColor * m_color, true);
		}
	}else
	{
		if(m_isEnable) m_sprite.draw(m_comboInputNormal);
		else m_sprite.draw(m_comboInputDisable);
	}
	m_caption.draw();
}
void XCombo::drawUp()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	if(m_isDrawDownMenu || m_isInAction)
	{
		m_buttom[1].draw();
		for(int i = 0;i < m_menuDrawSum;++ i)
		{
			m_buttom[i + 3].draw();
		}
		m_buttom[2].draw();
	}
	for(int i = 0;i < m_menuDrawSum + 3;++ i)
	{
		m_buttom[i].drawUp();
	}
	m_comment.draw();
}
XBool XCombo::mouseProc(const XVec2& p,XMouseState mouseState)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	//if(m_withAction && m_isInAction) return XFalse;	//如果支持动作播放而且正在播放动画那么不接受鼠标控制
	if(m_isSilent) return XFalse;
	if(isInRect(p))
	{
		if(!m_isMouseInRect)
		{
			m_isMouseInRect = XTrue;
			m_comment.setShow();
		}
		m_comment.updatePosition(p + XVec2(0.0f, 16.0f));
		if(mouseState != MOUSE_MOVE && m_comment.getIsShow())
			m_comment.disShow();	//鼠标的任意操作都会让说明框消失
	}else
	{
		if(m_isMouseInRect)
		{
			m_isMouseInRect = XFalse;
			m_comment.disShow();
		}
	}
	switch(mouseState)
	{
	case MOUSE_LEFT_BUTTON_DOWN:
	case MOUSE_LEFT_BUTTON_DCLICK:
		//m_mouseButtonDownIsUseable = 0;
		if(m_allArea.isInRect(p)) 
		{
			if(m_isDrawDownMenu)
				m_isBeChoose = XTrue;	//控件处于焦点状态
		}else
		{
			if(m_isDrawDownMenu) changeDownMenuState();
		}
		if(m_isDrawDownMenu)
			m_isBeChoose = XTrue;	//控件处于焦点状态

		if(XRect(m_position + m_inputArea.getLT() * m_scale,
			m_position + m_inputArea.getRB() * m_scale).isInRect(p))
		{
			m_isBeChoose = XTrue;
			changeDownMenuState();
		}
		break;
	case MOUSE_WHEEL_UP_DOWN:
		if(isInRect(p))
		{
			if(!m_isDrawDownMenu)
			{
				if(m_curChooseOrder > 0)
				{
					-- m_curChooseOrder;
					updateString();
					if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,CMB_DATA_CHANGE);
					else XCtrlManager.eventProc(m_objectID,CMB_DATA_CHANGE);
				}
			}else
			{//相当于上下选择
				funCtrlProc(this,m_buttom[1].getControlID(),XButton::BTN_MOUSE_DOWN);
			}
		}
		break;
	case MOUSE_WHEEL_DOWN_DOWN:
		if(isInRect(p))
		{
			if(!m_isDrawDownMenu)
			{
				if(m_curChooseOrder < m_menuSum - 1)
				{
					++ m_curChooseOrder;
					updateString();
					if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,CMB_DATA_CHANGE);
					else XCtrlManager.eventProc(m_objectID,CMB_DATA_CHANGE);
				}
			}else
			{
				funCtrlProc(this,m_buttom[2].getControlID(),XButton::BTN_MOUSE_DOWN);
			}
		}
		break;
	}
	m_buttom[0].mouseProc(p,mouseState);	//下拉按钮对鼠标事件进行响应
	if(m_isDrawDownMenu)
	{
		for(int i = 1;i < m_menuDrawSum + 3;++ i)
		{
			m_buttom[i].mouseProc(p,mouseState);
		}
	}
//	if(mouseState == MOUSE_STATE_DOWN && m_mouseButtonDownIsUseable == 0)
//	{
//		m_isDrawDownMenu = 0;
//	}
	return XTrue;
}
XBool XCombo::keyboardProc(int keyOrder,XKeyState keyState)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable ||
		!m_isBeChoose) return XFalse;		//如果无效则直接退出
	if(m_withAction && m_isInAction) return XFalse;	//如果支持动作播放而且正在播放动画那么不接受鼠标控制
	if(m_isSilent) return XFalse;
	if(keyState == KEY_STATE_UP)
	{
		switch(keyOrder)
		{
		case XKEY_UP:
			if(m_curChooseOrder > 0)
			{
				-- m_curChooseOrder;
				updateString();
				if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,CMB_DATA_CHANGE);
				else XCtrlManager.eventProc(m_objectID,CMB_DATA_CHANGE);
			}
			break;
		case XKEY_DOWN:
			if(m_curChooseOrder < m_menuSum - 1)
			{
				++ m_curChooseOrder;
				updateString();
				if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,CMB_DATA_CHANGE);
				else XCtrlManager.eventProc(m_objectID,CMB_DATA_CHANGE);
			}
			break;
		}
	}
	return XTrue;
}
XBool XCombo::setMenuStr(const char *str)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		str == NULL) return XFalse;
	int sum = XString::getCharSum(str,';');
	if(sum < 0) sum = 0;
	int len = strlen(str);
	if(str[len - 1] != ';') sum += 1;		//不是以';'结束
	if(sum > m_menuSum) sum = m_menuSum;	//超过上限则只选择上限
	int offset = 0;
	for(int i = 0;i < sum;++ i)
	{
		len = XString::getCharPosition(str + offset,';');
		std::string tmpStr = str + offset;
		if(len >= 0) tmpStr.at(len) = '\0';
		if(!setMenuStr(tmpStr.c_str(),i)) return XFalse;
		offset += len + 1;
	}
	return XTrue;
}
XBool XCombo::setMenuStr(const char *str,int order)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		order < 0 || order >= m_menuSum ||
		str == NULL) return XFalse;
	//	str == NULL ||
	//	strlen(str) >= m_comboMaxMenuLength) return XFalse;
	std::string tmp1 = str;
	//这里检查字符串是否符合要求
	//for(int i = 0;i < m_comboMaxMenuLength;++ i)
	for(unsigned int i = 0;i < tmp1.size();++ i)
	{
		if(tmp1[i] == '\0') break;
		if(tmp1[i] == '\n')
		{
			tmp1[i] = '\0';
			break;
		}
	}

//	int len = strlen(m_menuData);
//	int addLen = strlen(temp1);
//	char temp[m_comboMaxMenuLength];
	if(order == 0)
	{//第一个元素
		unsigned int i;
		for(i = 0;i < m_menuData.size();++i)
		{
			if(m_menuData[i] == '\n')
			{
				m_menuData = m_menuData.substr(i,(int)(m_menuData.size()) - i);
				break;
			}
		}
		if(m_menuData[0] != '\n') m_menuData = "";	//字符串以\0结束
	//	if(len + addLen - i>= m_comboMaxMenuLength) return XFalse;
		m_menuData = tmp1.c_str() + m_menuData;
		updateString();
	}else
	{//后面的元素
		int start = 0;
		int end = 0;
		unsigned int i = 0;
		int j = 0;
		for(i = 0;i < m_menuData.size();++ i)
		{
			if(m_menuData[i] == '\n')
			{
				++j;
				if(j == order) start = i;
				if(j == order + 1)
				{
					end = i;
					break;
				}
			}
		}
		if(start == 0 && end == 0) return XFalse;	//没有找到合适的位置
		//if(len + addLen - (end - start) >= m_comboMaxMenuLength) return XFalse;
		std::string tmp;
		if(end > start) tmp = m_menuData.substr(end,(int)(m_menuData.size()) - end);
		else
		{
			tmp = "\n";
		}
		m_menuData = m_menuData.substr(0,start + 1) + tmp1 + tmp;
		updateString();
	}
	return XTrue;
}
void XCombo::updateString()
{
	if(!m_isInited) return;	//如果没有初始化直接退出
//	char temp[m_comboMaxMenuLength];
	int start = 0;
	int order = 0;
//	int len = strlen(m_menuData);
	std::string tmp;
	for(unsigned int i = 0;i < m_menuData.size();++ i)
	{
		if(m_menuData[i] == '\n')
		{
			if(order >= m_menuStartDrawOrder && order < m_menuStartDrawOrder + m_menuDrawSum)
			{
				tmp = m_menuData.substr(start,i - start);
				//memcpy(temp,m_menuData + start,i - start);
				//temp[i - start] = '\0';
				if(i - start >= m_menuTextWidth)
				{//如果超过显示长度，则显示"..."
					assert(m_menuTextWidth >= 2);
					tmp[m_menuTextWidth] = '\0';
					tmp[m_menuTextWidth - 1] = '.';
					if(XString::isAtUnicodeEnd(tmp.c_str(),m_menuTextWidth - 2)) tmp[m_menuTextWidth - 2] = '.';
				}
				m_buttom[order - m_menuStartDrawOrder + 3].setCaptionText(tmp.c_str());
			//	if(order == m_curChooseOrder)
			//	{
			//		m_caption.setString(temp);
			//	}
			}//else
			if(order == m_curChooseOrder)
			{
				tmp = m_menuData.substr(start,i - start);
				//memcpy(temp,m_menuData + start,i - start);
				//temp[i - start] = '\0';
				if(i - start >= m_captionTextWidth && m_captionTextWidth > 1)
				{//如果超过显示长度，则显示"..."
					tmp[m_captionTextWidth] = '\0';
					tmp[m_captionTextWidth - 1] = '.';
					if(XString::isAtUnicodeEnd(tmp.c_str(),m_captionTextWidth - 2)) tmp[m_captionTextWidth - 2] = '.';
				}
				m_caption.setString(tmp.c_str());
			}
			start = i + 1;
			++ order;
		}
	}
}
void XCombo::setScale(const XVec2&s)			//设置控件的尺寸
{
	if(s.x <= 0 || s.y <= 0 ||	//尺寸的值必须合理
		!m_isInited) return;				//如果没有初始化，则直接返回	
	m_scale = s;
	m_caption.setPosition(m_position + XVec2(m_inputArea.left + m_comboLeftDistance,
		m_inputArea.top + m_inputArea.getHeight() * 0.5f + m_comboTopDistance) * m_scale);
	m_caption.setScale(m_textSize * m_scale);
	m_buttom[0].setPosition(m_position.x + (m_inputArea.right - m_downButtonArea.left) * m_scale.x,m_position.y);
	m_buttom[0].setScale(m_scale);
	m_buttom[1].setScale(m_scale);
	m_buttom[2].setScale(m_scale);
	for(int i = 0;i < m_menuDrawSum;++ i)
	{
		m_buttom[3 + i].setScale(m_scale);
	}
	updateMenuBtnPosition();
	if(!m_withoutTex) m_sprite.setScale(m_scale);
}
void XCombo::setPosition(const XVec2&p)	//设置空间的位置
{
	if(!m_isInited) return;				//如果没有初始化，则直接返回
	m_position = p;
	m_caption.setPosition(m_position.x + (m_inputArea.left + m_comboLeftDistance) * m_scale.x,
		m_position.y + (m_inputArea.top + m_inputArea.getHeight() * 0.5f + m_comboTopDistance) * m_scale.y);
	m_buttom[0].setPosition(m_position.x + (m_inputArea.right - m_downButtonArea.left) * m_scale.x,m_position.y);

	updateMenuBtnPosition();
	if(!m_withoutTex) m_sprite.setPosition(m_position);
}
XBool XCombo::setACopy(const XCombo &temp)
{
	if(& temp == this) return XTrue;	//防止自身赋值
	if(!temp.m_isInited) return XFalse;
	if(m_isInited) release();	//这里需要释放原有的资源
	if(!XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
		XObjManager.addAObject(this);
#endif
	}
	m_isInited = temp.m_isInited;

	if(m_resInfo != NULL) XResManager.releaseResource(m_resInfo);
	m_resInfo = XResManager.copyResource(temp.m_resInfo);
	m_withoutTex = temp.m_withoutTex;

	m_comboInputNormal = temp.m_comboInputNormal;					//下拉菜单输入框的普通状态
	m_comboInputDisable = temp.m_comboInputDisable;					//无效状态下的按钮贴图

	m_menuSum = temp.m_menuSum;							//下拉菜单中的总菜单项数量
	m_menuDrawSum = temp.m_menuDrawSum;					//下拉菜单中显示的菜单的数量
	m_menuStartDrawOrder = temp.m_menuStartDrawOrder;	//下拉菜单中开始显示的第一个菜单项的编号
	m_menuTextWidth = temp.m_menuTextWidth;				//下拉菜单中显示的文字的文字宽度
	m_captionTextWidth = temp.m_captionTextWidth;				//下拉菜单中显示的文字的文字宽度

	m_isDrawDownMenu = temp.m_isDrawDownMenu;			//是否显示下拉菜单
	m_curChooseOrder = temp.m_curChooseOrder;			//当前选择的菜单项的编号

	m_caption.setACopy(temp.m_caption);			//文字的字体
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_sprite.setACopy(temp.m_sprite);		//用于显示贴图的精灵
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_sprite);
#endif
	m_textSize = temp.m_textSize;			//文字显示的尺寸，这个尺寸会与空间的缩放尺寸叠加
	m_textColor = temp.m_textColor;			//文字显示的尺寸，这个尺寸会与空间的缩放尺寸叠加

	m_menuData = temp.m_menuData;

	m_buttom = XMem::createArrayMem<XButton>(m_menuDrawSum + 3);
	if(m_buttom == NULL) 
	{
		//XMem::XDELETE_ARRAY(m_menuData);
		return XFalse;
	}

	for(int i = 0;i < m_menuDrawSum + 3;++ i)
	{
		m_buttom[i].setACopy(temp.m_buttom[i]);	//这样复制的话，回调函数会造成问题
		if(i >= 3) m_buttom[i].setEventProc(funCtrlProc,this);
		XCtrlManager.decreaseAObject(&(m_buttom[i]));	//从控件管理器中注销这些按钮
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&m_buttom[i]);
#endif
	}
	//为了避免错误，这里重新注册回调函数
	m_buttom[0].setEventProc(funCtrlProc,this);
	m_buttom[1].setEventProc(funCtrlProc,this);
	m_buttom[2].setEventProc(funCtrlProc,this);

	m_inputArea = temp.m_inputArea;			//输入框的响应范围
	m_downButtonArea = temp.m_downButtonArea;	//下拉按钮的响应范围
	m_downMenuUpArea = temp.m_downMenuUpArea;		//上翻页的响应范围
	m_downMenuArea = temp.m_downMenuArea;		//选择项的响应范围
	m_downMenuDownArea = temp.m_downMenuDownArea;	//下翻页的响应范围
	m_allArea = temp.m_allArea;
	m_actionMoveData = temp.m_actionMoveData;
	return XTrue;
}
XVec2 XCombo::getBox(int order)
{
	if(!m_isInited) return XVec2::zero;
	if(m_isDrawDownMenu)
	{//是否显示下拉菜单
		switch(order)
		{
		case 0: return m_allArea.getLT();
		case 1: return m_allArea.getRT();
		case 2: return m_allArea.getRB();
		case 3: return m_allArea.getLB();
		}
	}else
	{//
		switch (m_curStyle)
		{
		case CMB_STYLE_DOWN:
			switch(order)
			{
			case 0: return m_allArea.getLT();
			case 1: return m_allArea.getRT();
			case 2: return XVec2(m_allArea.right,m_position.y + m_inputArea.bottom * m_scale.y);
			case 3: return XVec2(m_allArea.left,m_position.y + m_inputArea.bottom * m_scale.y);
			}
			break;
		case CMB_STYLE_UP:
			switch(order)
			{
			case 0: return XVec2(m_allArea.left,m_position.y + m_inputArea.top * m_scale.y);
			case 1: return XVec2(m_allArea.right,m_position.y + m_inputArea.top * m_scale.y);
			case 2: return m_allArea.getRB();
			case 3: return m_allArea.getLB();
			}
			break;
		}
	}
	return XVec2::zero;
}
void XCombo::changeDownMenuState()
{
	if(!m_isDrawDownMenu)
	{
		m_isDrawDownMenu = XTrue;
	//	m_buttom[1].disable();
	//	if(m_menuSum > m_menuDrawSum) m_buttom[2].enable();
	//	else m_buttom[2].disable();
		if(m_curChooseOrder <= 0
			|| m_menuSum <= m_menuDrawSum) //选择的是第一项或者能全部选择下，则默认从第一项开始
			m_menuStartDrawOrder = 0;
		else
		{
			int tmp = m_menuDrawSum >> 1;
			if(m_curChooseOrder >= tmp) m_menuStartDrawOrder = m_curChooseOrder - tmp;
			if(m_curChooseOrder + tmp >= m_menuSum) m_menuStartDrawOrder = m_menuSum - m_menuDrawSum;
		}
		if(m_menuStartDrawOrder == 0) m_buttom[1].disable();
		else m_buttom[1].enable();
		if(m_menuStartDrawOrder + m_menuDrawSum >= m_menuSum) m_buttom[2].disable();
		else m_buttom[2].enable();

		updateString();
		if(m_withAction)
		{//这里测试一个动态效果
			m_isInAction = XTrue;
			m_isDraw = true;
			m_actionMoveData.set(0.0f,1.0f,0.005f);
			for(int i = 0;i < m_menuDrawSum + 2;++ i)
			{
				m_buttom[1 + i].setWithAction(XFalse);
			}
		}
		for(int i = 0;i < m_menuDrawSum + 2;++ i)
		{//设置下拉按钮可见
			m_buttom[1 + i].setVisible();
		}
	}else
	{
		m_isDrawDownMenu = XFalse;
		if(m_withAction)
		{//这里测试一个动态效果
			m_isInAction = XTrue;
			m_isDraw = false;
			m_actionMoveData.set(1.0f,0.0f,0.005f);
			for(int i = 0;i < m_menuDrawSum + 2;++ i)
			{
				m_buttom[1 + i].setWithAction(XFalse);
			}
		}else
		{
			for(int i = 0;i < m_menuDrawSum + 2;++ i)
			{//设置下拉按钮不可见
				m_buttom[1 + i].disVisible();
			}
		}
	}
}
std::string XCombo::getMenuStr() const
{
	std::string tmp = m_menuData;
	for(unsigned int i = 0;i < tmp.size();++ i)
	{
		if(tmp[i] == '\n') tmp[i] = ';';
	}
	return tmp;
	//char tmp[m_comboMaxMenuLength];
	//memcpy(tmp,m_menuData,m_comboMaxMenuLength);
	//for(int i = 0;i < m_comboMaxMenuLength;++ i)
	//{
	//	if(tmp[i] == '\0') break;
	//	if(tmp[i] == '\n') tmp[i] = ';';
	//}
	//return tmp;
}
std::string XCombo::getMenuStr(int order)
{
	if(order < 0 || order >= m_menuSum) return XString::gNullStr;
	//char tmp[m_comboMaxMenuLength];
	//memcpy(tmp,m_menuData,m_comboMaxMenuLength);
	std::string tmp = m_menuData;
	int s = 0;
	int index = 0;
	//for(int i = 0;i < m_comboMaxMenuLength;++ i)
	for(unsigned int i = 0;i < tmp.size();++ i)
	{
		if(tmp[i] == '\0')
		{
			if(index == order)
			{
				tmp[i] = '\0';
				return tmp.substr(s,i - s);
			}
			break;
		}
		if(tmp[i] == '\n')
		{
			if(index == order)
			{
				tmp[i] = '\0';
				return tmp.substr(s,i - s);
			}
			++ index;
			if(index == order) s = i + 1;
		}
	}
	return XString::gNullStr;
}
void XCombo::update(float stepTime)
{
	if (!m_isInited) return;
	m_comment.update(stepTime);
	for(int i = 0;i < m_menuDrawSum + 3;++ i)
	{
		m_buttom[i].update(stepTime);
	}
	if(m_isInAction)
	{
		m_actionMoveData.move(stepTime);
		float action = m_actionMoveData.getCurData();
		if(m_actionMoveData.getIsEnd()) 
		{
			m_isInAction = false;
			for(int i = 0;i < m_menuDrawSum + 2;++ i)
			{
				m_buttom[1 + i].setWithAction(XTrue);
			}
			action = 1.0f;	//这里需要恢复
			if(!m_isDraw)
			{
				for(int i = 0;i < m_menuDrawSum + 2;++ i)
				{
					m_buttom[1 + i].disVisible();
				}
			}
		}
		m_buttom[1].setAlpha(action * m_color.a);
		m_buttom[2].setAlpha(action * m_color.a);

		switch (m_curStyle)
		{
		case CMB_STYLE_DOWN:
			//m_buttom[1].setPosition(m_position.x,m_position.y + (m_inputArea.bottom - m_downMenuUpArea.top) * m_scale.y * action);
			m_buttom[2].setPosition(m_position.x,m_position.y + (m_inputArea.bottom + m_downMenuUpArea.getHeight() 
				+ m_downMenuArea.getHeight() * m_menuDrawSum * action - m_downMenuDownArea.top) * m_scale.y);
			for(int i = 0;i < m_menuDrawSum;++ i)
			{
				m_buttom[3 + i].setAlpha(action * m_color.a);
				m_buttom[3 + i].setPosition(m_position.x,m_position.y + (m_inputArea.bottom + m_downMenuUpArea.getHeight() 
					+ m_downMenuArea.getHeight() * i * action - m_downMenuArea.top) * m_scale.y);
			}
			break;
		case CMB_STYLE_UP:
			m_buttom[1].setPosition(m_position.x,m_position.y + (m_inputArea.top - m_downMenuDownArea.getHeight() 
				- m_downMenuArea.getHeight() * m_menuDrawSum - m_downMenuDownArea.bottom) * m_scale.y * action);
			//m_buttom[2].setPosition(m_position.x,m_position.y + (m_inputArea.bottom + m_downMenuUpArea.getHeight() 
			//	+ m_downMenuArea.getHeight() * m_menuDrawSum * action - m_downMenuDownArea.top) * m_scale.y);
			for(int i = 0;i < m_menuDrawSum;++ i)
			{
				m_buttom[3 + i].setAlpha(action * m_color.a);
				m_buttom[3 + i].setPosition(m_position.x,m_position.y + (m_inputArea.top - m_downMenuDownArea.getHeight() 
					- m_downMenuArea.getHeight() * (i + 1) * action  - m_downMenuArea.top) * m_scale.y);
			}
			break;
		}
	}
}
#if !WITH_INLINE_FILE
#include "XCombo.inl"
#endif
}