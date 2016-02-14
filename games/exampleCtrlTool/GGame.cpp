#include "GGame.h"
using namespace XE;
void ctrlFun(void *pClass,int id,int eventID)
{
	GGame &pPar = *(GGame *)pClass;
	if(id == pPar.m_winSizeXEdt.getControlID())
	{//窗口宽度
		if(eventID == XEdit::EDT_INPUT_OVER && pPar.m_winSizeXEdt.getEdtIsNumber())
			pPar.m_winWidth = pPar.m_winSizeXEdt.getAsFloat();
		return;
	}
	if(id == pPar.m_winSizeYEdt.getControlID())
	{//窗口高度
		if(eventID == XEdit::EDT_INPUT_OVER && pPar.m_winSizeYEdt.getEdtIsNumber())
			pPar.m_winHeight = pPar.m_winSizeYEdt.getAsFloat();
		return;
	}
	if(id == pPar.m_chooseCtrlMltlst.getControlID())
	{//进行选择
		if(eventID == XMultiList::MLTLST_SELECT) 
		{
			pPar.m_curChooseCtrlID = pPar.getCtrlIDFromList();
			pPar.updateCtrlProperty();
		}
		return;
	}
	if(id == pPar.m_saveBtn.getControlID())
	{//保存
		if(eventID == XButton::BTN_MOUSE_DOWN) 
			pPar.saveToFile();
		return;
	}
	if(id == pPar.m_readBtn.getControlID())
	{//读取
		if(eventID == XButton::BTN_MOUSE_DOWN) 
			pPar.loadFromFile();
		return;
	}
	if(id == pPar.m_codeBtn.getControlID())
	{//生成代码
		if(eventID == XButton::BTN_MOUSE_DOWN) 
			pPar.createCode();
		return;
	}
	if(id == pPar.m_delBtn.getControlID())
	{//删除指定控件
		if(eventID == XButton::BTN_MOUSE_DOWN) 
			//pPar.delACtrl();
			pPar.delSelectCtrl();
		return;
	}
	if(id == pPar.m_ctrlAffiliationEdt.getControlID())
	{//需要更新从属关系
		if(eventID != XEdit::EDT_INPUT_OVER) return;
		int index = pPar.getCurChooseCtrlIndex();
		if(index >= 0)
		{
			pPar.m_ctrls[index].affiliationStr = pPar.m_ctrlAffiliationEdt.getString();
			pPar.updateAffiliation();
		}
		return;
	}
	if(id == pPar.m_ctrlCommentEdt.getControlID())
	{//更新控件的说明
		if(eventID != XEdit::EDT_INPUT_OVER) return;
		int index = pPar.getCurChooseCtrlIndex();
		if(index >= 0)
			pPar.m_ctrls[index].ctrlComment = pPar.m_ctrlCommentEdt.getString();
		return;
	}
	if(id == pPar.m_ctrlExplanationEdt.getControlID())
	{//更新控件的代码注释
		if(eventID != XEdit::EDT_INPUT_OVER) return;
		int index = pPar.getCurChooseCtrlIndex();
		if(index >= 0)
			pPar.m_ctrls[index].codeExplanation = pPar.m_ctrlExplanationEdt.getString();
		return;
	}
	if(id == pPar.m_withCallbackChk.getControlID())
	{//是否使用回调函数
		if(eventID != XCheck::CHK_STATE_CHANGE) return; 
		int index = pPar.getCurChooseCtrlIndex();
		if(index >= 0) pPar.m_ctrls[index].withCallback = pPar.m_withCallbackChk.getState();
	}
	if(id == pPar.m_cmbAllMenuSumEdt.getControlID() ||
		id == pPar.m_cmbShowMenuSumEdt.getControlID())
	{//combo菜单数量发生变化
		if(eventID != XEdit::EDT_INPUT_OVER) return;
		int index = pPar.getCurChooseCtrlIndex();
		if(index >= 0)
		{
			pPar.m_ctrls[index].showMenuSum = pPar.m_cmbShowMenuSumEdt.getAsInt();
			pPar.m_ctrls[index].allMenuSum = pPar.m_cmbAllMenuSumEdt.getAsInt();
			switch(pPar.m_ctrls[index].pCtrl->getCtrlType())
			{
			case CTRL_OBJ_COMBO:
				{
					XCombo *tmp = XMem::createMem<XCombo>();
					tmp->initWithoutSkin(pPar.m_ctrlPixelSizeXEdt.getAsFloat(),
						pPar.m_cmbAllMenuSumEdt.getAsInt(),
						pPar.m_cmbShowMenuSumEdt.getAsInt());
					tmp->setPosition(pPar.m_ctrlPosXEdt.getAsFloat(),
						pPar.m_ctrlPosYEdt.getAsFloat());
					tmp->setScale(pPar.m_ctrlScaleSizeXEdt.getAsFloat(),
						pPar.m_ctrlScaleSizeYEdt.getAsFloat());
					tmp->setMenuStr(pPar.m_ctrlCaptionEdt.getString());
					XMem::XDELETE(pPar.m_ctrls[index].pCtrl);
					pPar.m_ctrls[index].pCtrl = tmp;
				}
				break;
			case CTRL_OBJ_MUTILIST:
				{
					XMultiList *tmp = XMem::createMem<XMultiList>();
					tmp->initWithoutSkin(XVector2(pPar.m_ctrlPixelSizeXEdt.getAsFloat(),
						pPar.m_ctrlPixelSizeYEdt.getAsFloat()),
						pPar.m_cmbAllMenuSumEdt.getAsInt(),
						pPar.m_cmbShowMenuSumEdt.getAsInt());
					tmp->setPosition(pPar.m_ctrlPosXEdt.getAsFloat(),
						pPar.m_ctrlPosYEdt.getAsFloat());
					tmp->setScale(pPar.m_ctrlScaleSizeXEdt.getAsFloat(),
						pPar.m_ctrlScaleSizeYEdt.getAsFloat());
					tmp->setTitleStr(pPar.m_ctrlCaptionEdt.getString());
					XMem::XDELETE(pPar.m_ctrls[index].pCtrl);
					pPar.m_ctrls[index].pCtrl = tmp;
				}
				break;
			case CTRL_OBJ_TAB:
				{
					XTab *tmp = XMem::createMem<XTab>();
					tmp->initWithoutSkin(XVector2(pPar.m_ctrlPixelSizeXEdt.getAsFloat(),
						pPar.m_ctrlPixelSizeYEdt.getAsFloat()));
					tmp->setPosition(pPar.m_ctrlPosXEdt.getAsInt(),
						pPar.m_ctrlPosYEdt.getAsInt());
					tmp->setScale(pPar.m_ctrlScaleSizeXEdt.getAsFloat(),
						pPar.m_ctrlScaleSizeYEdt.getAsFloat());
					int sum = pPar.m_cmbAllMenuSumEdt.getAsInt();
					for(int i = 0;i < sum;++ i)
					{
						tmp->addATab("tab" + XString::toString(i));
					}
					tmp->setTabsStr(pPar.m_ctrlCaptionEdt.getString());
					XMem::XDELETE(pPar.m_ctrls[index].pCtrl);
					pPar.m_ctrls[index].pCtrl = tmp;
					pPar.updateAffiliation();//下面需要更新从属关系
				}
				break;
			}
		}
		return;
	}
	//下面是属性发生变化是修改控件的属性
	if(id == pPar.m_ctrlPosXEdt.getControlID() ||
		id == pPar.m_ctrlPosYEdt.getControlID())
	{//位置
		if(eventID != XEdit::EDT_INPUT_OVER) return;
		int index = pPar.getCurChooseCtrlIndex();
		if(index >= 0 && pPar.m_ctrlPosXEdt.getEdtIsNumber() &&
			pPar.m_ctrlPosYEdt.getEdtIsNumber())
		{
			pPar.m_ctrls[index].pCtrl->setPosition(pPar.m_ctrlPosXEdt.getAsFloat(),
				pPar.m_ctrlPosYEdt.getAsFloat());
		}
		return;
	}
	if(id == pPar.m_ctrlNameEdt.getControlID())
	{//名字
		if(eventID != XEdit::EDT_INPUT_OVER) return;
		int index = pPar.getCurChooseCtrlIndex();
		if(index >= 0)
		{
			//这里需要检查名字的唯一性
			if(pPar.getCtrlIndexByName(pPar.m_ctrlNameEdt.getString()) >= 0)
			{//名字重复，这里回复原有设置
				pPar.m_ctrlNameEdt.setString(pPar.m_ctrls[index].ctrlName.c_str());
			}else
			{
				pPar.m_ctrls[index].ctrlName = pPar.m_ctrlNameEdt.getString();
				pPar.updateAffiliation();
				//下面需要检查列表框是否需要更新
				for(int i = 0;i < pPar.m_chooseCtrlMltlst.getLineSum();++ i)
				{
					if(atoi(pPar.m_chooseCtrlMltlst.getBoxStr(i,0)) == pPar.m_curChooseCtrlID)
					{//列表框也需要更新
						pPar.m_chooseCtrlMltlst.setBoxStr(pPar.m_ctrlNameEdt.getString(),i,1);
						break;
					}
				}
			}
		}
		return;
	}
	if(id == pPar.m_ctrlPixelSizeXEdt.getControlID() ||
		id == pPar.m_ctrlPixelSizeYEdt.getControlID())
	{//像素x，当存在从属关系是这里会存在bug，不能直接删除物件，必须先从父物体中删除该物件
		if(eventID != XEdit::EDT_INPUT_OVER) return;
		int index = pPar.getCurChooseCtrlIndex();
		if(index >= 0)
		{
			switch(pPar.m_ctrls[index].pCtrl->getCtrlType())
			{
				case CTRL_OBJ_NULL:break;
				case CTRL_OBJ_BUTTON:
					{
						pPar.clearAffiliation();
						XButton *tmp = XMem::createMem<XButton>();
						tmp->initWithoutSkin(pPar.m_ctrlCaptionEdt.getString(),
							XVector2(pPar.m_ctrlPixelSizeXEdt.getAsFloat(),
							pPar.m_ctrlPixelSizeYEdt.getAsFloat()));
						tmp->setPosition(pPar.m_ctrlPosXEdt.getAsFloat(),
							pPar.m_ctrlPosYEdt.getAsFloat());
						tmp->setScale(pPar.m_ctrlScaleSizeXEdt.getAsFloat(),
							pPar.m_ctrlScaleSizeYEdt.getAsFloat());
						XMem::XDELETE(pPar.m_ctrls[index].pCtrl);
						pPar.m_ctrls[index].pCtrl = tmp;
						pPar.updateAffiliation();	//这里需要更新从属关系，否则会造成crash
					}
					break;		
				case CTRL_OBJ_BTNBAR:break;			
				case CTRL_OBJ_BUTTONEX:break;		
				case CTRL_OBJ_CALENDAR:break;			
				case CTRL_OBJ_CHART:break;			
				case CTRL_OBJ_CHECK:break;		//不支持	
				case CTRL_OBJ_COMBO:
					{
						pPar.clearAffiliation();
						XCombo *tmp = XMem::createMem<XCombo>();
						tmp->initWithoutSkin(pPar.m_ctrlPixelSizeXEdt.getAsFloat(),
							pPar.m_cmbAllMenuSumEdt.getAsInt(),
							pPar.m_cmbShowMenuSumEdt.getAsInt());
						tmp->setPosition(pPar.m_ctrlPosXEdt.getAsFloat(),
							pPar.m_ctrlPosYEdt.getAsFloat());
						tmp->setScale(pPar.m_ctrlScaleSizeXEdt.getAsFloat(),
							pPar.m_ctrlScaleSizeYEdt.getAsFloat());
						tmp->setMenuStr(pPar.m_ctrlCaptionEdt.getString());
						XMem::XDELETE(pPar.m_ctrls[index].pCtrl);
						pPar.m_ctrls[index].pCtrl = tmp;
						pPar.updateAffiliation();	//这里需要更新从属关系，否则会造成crash
					}
					break;			
				case CTRL_OBJ_DIRECTORYLIST:break;		
				case CTRL_OBJ_EDIT:
					{
						pPar.clearAffiliation();
						XEdit *tmp = XMem::createMem<XEdit>();
						tmp->initWithoutSkin(XVector2(pPar.m_ctrlPixelSizeXEdt.getAsFloat(),
							pPar.m_ctrlPixelSizeYEdt.getAsFloat()),pPar.m_ctrlCaptionEdt.getString());
						tmp->setPosition(pPar.m_ctrlPosXEdt.getAsFloat(),
							pPar.m_ctrlPosYEdt.getAsFloat());
						tmp->setScale(pPar.m_ctrlScaleSizeXEdt.getAsFloat(),
							pPar.m_ctrlScaleSizeYEdt.getAsFloat());
						XMem::XDELETE(pPar.m_ctrls[index].pCtrl);
						pPar.m_ctrls[index].pCtrl = tmp;
						pPar.updateAffiliation();	//这里需要更新从属关系，否则会造成crash
					}
					break;			
				case CTRL_OBJ_IMAGE:break;			
				case CTRL_OBJ_GROUP:break;		
				case CTRL_OBJ_IMAGELIST:break;			
				case CTRL_OBJ_LINECTRL:break;		
				case CTRL_OBJ_MENU:break;			
				case CTRL_OBJ_MOUSERIGHTBUTTONMENU:break;		
				case CTRL_OBJ_MUTILIST:
					{
						pPar.clearAffiliation();
						XMultiList *tmp = XMem::createMem<XMultiList>();
						tmp->initWithoutSkin(XVector2(pPar.m_ctrlPixelSizeXEdt.getAsFloat(),
							pPar.m_ctrlPixelSizeYEdt.getAsFloat()),
							pPar.m_cmbAllMenuSumEdt.getAsInt(),
							pPar.m_cmbShowMenuSumEdt.getAsInt());
						tmp->setPosition(pPar.m_ctrlPosXEdt.getAsFloat(),
							pPar.m_ctrlPosYEdt.getAsFloat());
						tmp->setScale(pPar.m_ctrlScaleSizeXEdt.getAsFloat(),
							pPar.m_ctrlScaleSizeYEdt.getAsFloat());
						tmp->setTitleStr(pPar.m_ctrlCaptionEdt.getString());
						XMem::XDELETE(pPar.m_ctrls[index].pCtrl);
						pPar.m_ctrls[index].pCtrl = tmp;
						pPar.updateAffiliation();	//这里需要更新从属关系，否则会造成crash
					}
					break;		
				case CTRL_OBJ_MUTITEXT:
					{
						pPar.clearAffiliation();
						XMultiText *tmp = XMem::createMem<XMultiText>();
						tmp->initWithoutSkin(XVector2(pPar.m_ctrlPixelSizeXEdt.getAsFloat(),
							pPar.m_ctrlPixelSizeYEdt.getAsFloat()),pPar.m_ctrlCaptionEdt.getString());
						tmp->setPosition(pPar.m_ctrlPosXEdt.getAsFloat(),
							pPar.m_ctrlPosYEdt.getAsFloat());
						tmp->setScale(pPar.m_ctrlScaleSizeXEdt.getAsFloat(),
							pPar.m_ctrlScaleSizeYEdt.getAsFloat());
						XMem::XDELETE(pPar.m_ctrls[index].pCtrl);
						pPar.m_ctrls[index].pCtrl = tmp;
						pPar.updateAffiliation();	//这里需要更新从属关系，否则会造成crash
					}
					break;		
				case CTRL_OBJ_PASSWORDPAD:break;		
				case CTRL_OBJ_POINTCTRL:break;			
				case CTRL_OBJ_PROGRESS:break;		
				case CTRL_OBJ_PROGRESSRING:break;		
				case CTRL_OBJ_RADIOS:break;		
				case CTRL_OBJ_SIMPLELINE:
					{
						pPar.clearAffiliation();
						XSimpleLine *tmp = XMem::createMem<XSimpleLine>();
						tmp->init(pPar.m_ctrlPixelSizeXEdt.getAsFloat());
						tmp->setPosition(pPar.m_ctrlPosXEdt.getAsFloat(),
							pPar.m_ctrlPosYEdt.getAsFloat());
						tmp->setScale(pPar.m_ctrlScaleSizeXEdt.getAsFloat(),
							pPar.m_ctrlScaleSizeYEdt.getAsFloat());
						XMem::XDELETE(pPar.m_ctrls[index].pCtrl);
						pPar.m_ctrls[index].pCtrl = tmp;
						pPar.updateAffiliation();	//这里需要更新从属关系，否则会造成crash
					}
					break;		
				case CTRL_OBJ_SLIDER:break;		
				case CTRL_OBJ_SLIDEREX:break;		
				case CTRL_OBJ_TAB:
					{
						pPar.clearAffiliation();
						XTab *tmp = XMem::createMem<XTab>();
						tmp->initWithoutSkin(XVector2(pPar.m_ctrlPixelSizeXEdt.getAsFloat(),
							pPar.m_ctrlPixelSizeYEdt.getAsFloat()));
						tmp->setPosition(pPar.m_ctrlPosXEdt.getAsFloat(),
							pPar.m_ctrlPosYEdt.getAsFloat());
						tmp->setScale(pPar.m_ctrlScaleSizeXEdt.getAsFloat(),
							pPar.m_ctrlScaleSizeYEdt.getAsFloat());
						int sum = pPar.m_cmbAllMenuSumEdt.getAsInt();
						for(int i = 0;i < sum;++ i)
						{
							tmp->addATab("tab" + XString::toString(i));
						}
						tmp->setTabsStr(pPar.m_ctrlCaptionEdt.getString());
						XMem::XDELETE(pPar.m_ctrls[index].pCtrl);
						pPar.m_ctrls[index].pCtrl = tmp;
						pPar.updateAffiliation();//下面需要更新从属关系
					}
					break;				
				case CTRL_OBJ_TEXT:break;		//不支持	
				case CTRL_OBJ_TOOLBAR:break;			
				case CTRL_OBJ_FUNCTION:break;
			}
		}
		return;
	}
	if(id == pPar.m_ctrlScaleSizeXEdt.getControlID() ||
		id == pPar.m_ctrlScaleSizeYEdt.getControlID())
	{//缩放
		if(eventID != XEdit::EDT_INPUT_OVER) return;
		int index = pPar.getCurChooseCtrlIndex();
		if(index >= 0 && pPar.m_ctrlScaleSizeXEdt.getEdtIsNumber() && 
			pPar.m_ctrlScaleSizeYEdt.getEdtIsNumber())
		{
			pPar.m_ctrls[index].pCtrl->setScale(pPar.m_ctrlScaleSizeXEdt.getAsFloat(),
				pPar.m_ctrlScaleSizeYEdt.getAsFloat());
		}
		return;
	}
	if(id == pPar.m_ctrlCaptionEdt.getControlID())
	{//标题
		if(eventID != XEdit::EDT_INPUT_OVER) return;
		int index = pPar.getCurChooseCtrlIndex();
		if(index >= 0)
		{
			switch(pPar.m_ctrls[index].pCtrl->getCtrlType())
			{
				case CTRL_OBJ_NULL:break;
				case CTRL_OBJ_BUTTON:
					((XButton *)pPar.m_ctrls[index].pCtrl)->setCaptionText(pPar.m_ctrlCaptionEdt.getString());
					break;		
				case CTRL_OBJ_BTNBAR:break;			
				case CTRL_OBJ_BUTTONEX:break;		
				case CTRL_OBJ_CALENDAR:break;			
				case CTRL_OBJ_CHART:break;			
				case CTRL_OBJ_CHECK:
					((XCheck *)pPar.m_ctrls[index].pCtrl)->setCaptionText(pPar.m_ctrlCaptionEdt.getString());
					break;			
				case CTRL_OBJ_COMBO:
					((XCombo *)pPar.m_ctrls[index].pCtrl)->setMenuStr(pPar.m_ctrlCaptionEdt.getString());
					break;			
				case CTRL_OBJ_DIRECTORYLIST:break;		
				case CTRL_OBJ_EDIT:
					((XEdit *)pPar.m_ctrls[index].pCtrl)->setString(pPar.m_ctrlCaptionEdt.getString());
					break;			
				case CTRL_OBJ_IMAGE:break;			
				case CTRL_OBJ_GROUP:break;		
				case CTRL_OBJ_IMAGELIST:break;			
				case CTRL_OBJ_LINECTRL:break;		
				case CTRL_OBJ_MENU:break;			
				case CTRL_OBJ_MOUSERIGHTBUTTONMENU:break;		
				case CTRL_OBJ_MUTILIST:
					((XMultiList *)pPar.m_ctrls[index].pCtrl)->setTitleStr(pPar.m_ctrlCaptionEdt.getString());
					break;		
				case CTRL_OBJ_MUTITEXT:
					((XMultiText *)pPar.m_ctrls[index].pCtrl)->setString(pPar.m_ctrlCaptionEdt.getString());
					break;		
				case CTRL_OBJ_PASSWORDPAD:break;		
				case CTRL_OBJ_POINTCTRL:break;			
				case CTRL_OBJ_PROGRESS:break;		
				case CTRL_OBJ_PROGRESSRING:break;		
				case CTRL_OBJ_RADIOS:break;		
				case CTRL_OBJ_SIMPLELINE:break;	//不支持	
				case CTRL_OBJ_SLIDER:break;		
				case CTRL_OBJ_SLIDEREX:break;		
				case CTRL_OBJ_TAB:
					((XTab *)pPar.m_ctrls[index].pCtrl)->setTabsStr(pPar.m_ctrlCaptionEdt.getString());
					pPar.updateAffiliation();
					break;				
				case CTRL_OBJ_TEXT:
					((XText *)pPar.m_ctrls[index].pCtrl)->setString(pPar.m_ctrlCaptionEdt.getString());
					break;				
				case CTRL_OBJ_TOOLBAR:break;			
				case CTRL_OBJ_FUNCTION:break;
			}
		}
		return;
	}
}
void drawTop()
{
	if(XEG.m_pGame != NULL) ((GGame *)XEG.m_pGame)->drawUp();
}
bool GGame::createUI()
{
	m_tab.initWithoutSkin(XVector2(512.0f,1200.0f));
	m_tab.setScale(0.5f);
	m_tab.setPosition(4.0f,36.0f);
	m_tab.addATab("控件");
	m_tab.addATab("属性");
	m_tab.addATab("系统");

	m_subWindow.initWithoutSkin(XVector2(264.0f,660.0f),"面板");
	m_subWindow.addACtrlObj(&m_tab);
	m_subWindow.setAutoShrink(true);

	//下面是控件列表
	m_ctrlChooseRds.initWithoutSkin(30,XVector2(0.0f,32.0f),XVector2(32.0f,32.0f),XVector2(34.0f,16.0f));
	m_ctrlChooseRds.setScale(0.5f);
	m_ctrlChooseRds.setPosition(6.0f,56.0f);
	m_ctrlChooseRds.setRadioText("无",0);
	m_ctrlChooseRds.setRadioText("按钮",1);
	m_ctrlChooseRds.setRadioText("按钮条",2);
	m_ctrlChooseRds.setRadioState(false,2);
	m_ctrlChooseRds.setRadioText("异形按钮",3);
	m_ctrlChooseRds.setRadioState(false,3);
	m_ctrlChooseRds.setRadioText("日历",4);
	m_ctrlChooseRds.setRadioState(false,4);
	m_ctrlChooseRds.setRadioText("数据表",5);
	m_ctrlChooseRds.setRadioState(false,5);
	m_ctrlChooseRds.setRadioText("单选框",6);
	m_ctrlChooseRds.setRadioText("下拉菜单",7);
	m_ctrlChooseRds.setRadioText("路径选择",8);
	m_ctrlChooseRds.setRadioState(false,8);
	m_ctrlChooseRds.setRadioText("输入框",9);
	m_ctrlChooseRds.setRadioText("群组框",10);
	m_ctrlChooseRds.setRadioState(false,10);
	m_ctrlChooseRds.setRadioText("图片控件",11);
	m_ctrlChooseRds.setRadioState(false,11);
	m_ctrlChooseRds.setRadioText("图片列表",12);
	m_ctrlChooseRds.setRadioState(false,12);
	m_ctrlChooseRds.setRadioText("线控件",13);
	m_ctrlChooseRds.setRadioState(false,13);
	m_ctrlChooseRds.setRadioText("菜单",14);
	m_ctrlChooseRds.setRadioState(false,14);
	m_ctrlChooseRds.setRadioText("右键菜单",15);
	m_ctrlChooseRds.setRadioState(false,15);
	m_ctrlChooseRds.setRadioText("多行列表",16);
	m_ctrlChooseRds.setRadioText("复杂多行列表",17);
	m_ctrlChooseRds.setRadioState(false,17);
	m_ctrlChooseRds.setRadioText("多行文本",18);
	m_ctrlChooseRds.setRadioText("密码板",19);
	m_ctrlChooseRds.setRadioState(false,19);
	m_ctrlChooseRds.setRadioText("点控件",20);
	m_ctrlChooseRds.setRadioState(false,20);
	m_ctrlChooseRds.setRadioText("进度条",21);
	m_ctrlChooseRds.setRadioState(false,21);
	m_ctrlChooseRds.setRadioText("环形进度条",22);
	m_ctrlChooseRds.setRadioState(false,22);
	m_ctrlChooseRds.setRadioText("单选表",23);
	m_ctrlChooseRds.setRadioState(false,23);
	m_ctrlChooseRds.setRadioText("分割线",24);
	m_ctrlChooseRds.setRadioText("滑动条",25);
	m_ctrlChooseRds.setRadioState(false,25);
	m_ctrlChooseRds.setRadioText("微调滑动条",26);
	m_ctrlChooseRds.setRadioState(false,26);
	//m_ctrlChooseRds.setRadioText("子窗口",27);
	m_ctrlChooseRds.setRadioText("分页控件",27);
	m_ctrlChooseRds.setRadioText("文本",28);
	m_ctrlChooseRds.setRadioText("工具条",29);
	m_ctrlChooseRds.setRadioState(false,29);
	m_tab.addObjToTab(&m_ctrlChooseRds,"控件");
	//属性
	int h = 56;
	m_chooseCtrlMltlst.initWithoutSkin(XVector2(512.0f,512.0f),3,0);
	m_chooseCtrlMltlst.setTitleStr("ID",0);
	m_chooseCtrlMltlst.setRowWidth(64,0);
	m_chooseCtrlMltlst.setTitleStr("Name",1);
	m_chooseCtrlMltlst.setRowWidth(200,1);
	m_chooseCtrlMltlst.setTitleStr("Type",2);
	m_chooseCtrlMltlst.setRowWidth(200,2);
	m_chooseCtrlMltlst.setPosition(6.0f,h);
	m_chooseCtrlMltlst.setScale(0.5f);
	m_chooseCtrlMltlst.setEventProc(ctrlFun,this);
	m_tab.addObjToTab(&m_chooseCtrlMltlst,"属性");
	h += 258;
	m_delBtn.initWithoutSkin("删除",128);
	m_delBtn.setPosition(6.0f,h);
	m_delBtn.setScale(0.5f);
	m_delBtn.setEventProc(ctrlFun,this);
	m_curMousePosTxt.init("x:0,y:0");
	m_curMousePosTxt.setPosition(6.0f + 66.0f,h);
	m_curMousePosTxt.setScale(0.5f);
	m_tab.addObjToTab(&m_curMousePosTxt,"属性");
	m_tab.addObjToTab(&m_delBtn,"属性");
	h += 18;
	m_ctrlNameTxt.init("名称:");
	m_ctrlNameTxt.setScale(0.5f);
	m_ctrlNameTxt.setPosition(6.0f,h);
	m_ctrlNameEdt.initWithoutSkin(256.0f,"name");
	m_ctrlNameEdt.setScale(0.5f);
	m_ctrlNameEdt.setPosition(6.0f + 38.0f,h);
	m_ctrlNameEdt.setEventProc(ctrlFun,this);
	m_tab.addObjToTab(&m_ctrlNameTxt,"属性");
	m_tab.addObjToTab(&m_ctrlNameEdt,"属性");
	h += 18;
	m_ctrlPosTxt.init("位置:");
	m_ctrlPosTxt.setScale(0.5f);
	m_ctrlPosTxt.setPosition(6.0f,h);
	m_ctrlPosXEdt.initWithoutSkin(128.0f,"200");
	m_ctrlPosXEdt.setScale(0.5f);
	m_ctrlPosXEdt.setPosition(6.0f + 38.0f,h);
	m_ctrlPosXEdt.setEventProc(ctrlFun,this);
	m_ctrlPosYEdt.initWithoutSkin(128.0f,"200");
	m_ctrlPosYEdt.setScale(0.5f);
	m_ctrlPosYEdt.setPosition(6.0f + 102.0f,h);
	m_ctrlPosYEdt.setEventProc(ctrlFun,this);
	m_tab.addObjToTab(&m_ctrlPosTxt,"属性");
	m_tab.addObjToTab(&m_ctrlPosXEdt,"属性");
	m_tab.addObjToTab(&m_ctrlPosYEdt,"属性");
	h += 18;
	m_ctrlPixelSizeTxt.init("尺寸:");
	m_ctrlPixelSizeTxt.setScale(0.5f);
	m_ctrlPixelSizeTxt.setPosition(6.0f,h);
	m_ctrlPixelSizeXEdt.initWithoutSkin(128.0f,"200");
	m_ctrlPixelSizeXEdt.setScale(0.5f);
	m_ctrlPixelSizeXEdt.setPosition(6.0f + 38.0f,h);
	m_ctrlPixelSizeXEdt.setEventProc(ctrlFun,this);
	m_ctrlPixelSizeYEdt.initWithoutSkin(128.0f,"200");
	m_ctrlPixelSizeYEdt.setScale(0.5f);
	m_ctrlPixelSizeYEdt.setPosition(6.0f + 102.0f,h);
	m_ctrlPixelSizeYEdt.setEventProc(ctrlFun,this);
	m_tab.addObjToTab(&m_ctrlPixelSizeTxt,"属性");
	m_tab.addObjToTab(&m_ctrlPixelSizeXEdt,"属性");
	m_tab.addObjToTab(&m_ctrlPixelSizeYEdt,"属性");
	h += 18;
	m_ctrlScaleSizeTxt.init("缩放:");
	m_ctrlScaleSizeTxt.setScale(0.5f);
	m_ctrlScaleSizeTxt.setPosition(6.0f,h);
	m_ctrlScaleSizeXEdt.initWithoutSkin(128.0f,"200");
	m_ctrlScaleSizeXEdt.setScale(0.5f);
	m_ctrlScaleSizeXEdt.setPosition(6.0f + 38.0f,h);
	m_ctrlScaleSizeXEdt.setEventProc(ctrlFun,this);
	m_ctrlScaleSizeYEdt.initWithoutSkin(128.0f,"200");
	m_ctrlScaleSizeYEdt.setScale(0.5f);
	m_ctrlScaleSizeYEdt.setPosition(6.0f + 102.0f,h);
	m_ctrlScaleSizeYEdt.setEventProc(ctrlFun,this);
	m_tab.addObjToTab(&m_ctrlScaleSizeTxt,"属性");
	m_tab.addObjToTab(&m_ctrlScaleSizeXEdt,"属性");
	m_tab.addObjToTab(&m_ctrlScaleSizeYEdt,"属性");
	h += 18;
	m_ctrlCaptionTxt.init("标题:");
	m_ctrlCaptionTxt.setScale(0.5f);
	m_ctrlCaptionTxt.setPosition(6.0f,h);
	m_ctrlCaptionEdt.initWithoutSkin(256.0f,"caption");
	m_ctrlCaptionEdt.setScale(0.5f);
	m_ctrlCaptionEdt.setPosition(6.0f + 38.0f,h);
	m_ctrlCaptionEdt.setEventProc(ctrlFun,this);
	m_tab.addObjToTab(&m_ctrlCaptionTxt,"属性");
	m_tab.addObjToTab(&m_ctrlCaptionEdt,"属性");
	h += 18;
	m_ctrlAffiliationTxt.init("从属关系:");
	m_ctrlAffiliationTxt.setScale(0.5f);
	m_ctrlAffiliationTxt.setPosition(6.0f,h);
	m_ctrlAffiliationEdt.initWithoutSkin(256.0f,"NULL");
	m_ctrlAffiliationEdt.setScale(0.5f);
	m_ctrlAffiliationEdt.setPosition(6.0f + 68.0f,h);
	m_ctrlAffiliationEdt.setEventProc(ctrlFun,this);
	m_tab.addObjToTab(&m_ctrlAffiliationTxt,"属性");
	m_tab.addObjToTab(&m_ctrlAffiliationEdt,"属性");
	h += 18;
	m_ctrlCommentTxt.init("说明:");
	m_ctrlCommentTxt.setScale(0.5f);
	m_ctrlCommentTxt.setPosition(6.0f,h);
	m_ctrlCommentEdt.initWithoutSkin(256.0f,"NULL");
	m_ctrlCommentEdt.setScale(0.5f);
	m_ctrlCommentEdt.setPosition(6.0f + 38.0f,h);
	m_ctrlCommentEdt.setEventProc(ctrlFun,this);
	m_tab.addObjToTab(&m_ctrlCommentTxt,"属性");
	m_tab.addObjToTab(&m_ctrlCommentEdt,"属性");
	h += 18;
	m_ctrlExplanationTxt.init("注释:");
	m_ctrlExplanationTxt.setScale(0.5f);
	m_ctrlExplanationTxt.setPosition(6.0f,h);
	m_ctrlExplanationEdt.initWithoutSkin(256.0f,"NULL");
	m_ctrlExplanationEdt.setScale(0.5f);
	m_ctrlExplanationEdt.setPosition(6.0f + 38.0f,h);
	m_ctrlExplanationEdt.setEventProc(ctrlFun,this);
	m_tab.addObjToTab(&m_ctrlExplanationTxt,"属性");
	m_tab.addObjToTab(&m_ctrlExplanationEdt,"属性");
	h += 18;
	m_withCallbackChk.initWithoutSkin("是否事件响应");
	m_withCallbackChk.setScale(0.5f);
	m_withCallbackChk.setPosition(6.0f,h);
	m_withCallbackChk.setEventProc(ctrlFun,this);
	m_tab.addObjToTab(&m_withCallbackChk,"属性");
	h += 18;
	m_cmbAllMenuSumTxt.init("总菜单数:");
	m_cmbAllMenuSumTxt.setScale(0.5f);
	m_cmbAllMenuSumTxt.setPosition(6.0f,h);
	m_cmbAllMenuSumEdt.initWithoutSkin(128,"5");
	m_cmbAllMenuSumEdt.setScale(0.5f);
	m_cmbAllMenuSumEdt.setPosition(6.0f + 68.0f,h);
	m_cmbAllMenuSumEdt.setEventProc(ctrlFun,this);
	m_tab.addObjToTab(&m_cmbAllMenuSumTxt,"属性");
	m_tab.addObjToTab(&m_cmbAllMenuSumEdt,"属性");
	h += 18;
	m_cmbShowMenuSumTxt.init("显示菜单数:");
	m_cmbShowMenuSumTxt.setScale(0.5f);
	m_cmbShowMenuSumTxt.setPosition(6.0f,h);
	m_cmbShowMenuSumEdt.initWithoutSkin(128,"3");
	m_cmbShowMenuSumEdt.setScale(0.5f);
	m_cmbShowMenuSumEdt.setPosition(6.0f + 84.0f,h);
	m_cmbShowMenuSumEdt.setEventProc(ctrlFun,this);
	m_tab.addObjToTab(&m_cmbShowMenuSumTxt,"属性");
	m_tab.addObjToTab(&m_cmbShowMenuSumEdt,"属性");
	//系统
	h = 56;
	m_winSizeTxt.init("窗口尺寸:");
	m_winSizeTxt.setScale(0.5f);
	m_winSizeTxt.setPosition(6.0f,h);
	m_winSizeXEdt.initWithoutSkin(128.0f,"800");
	m_winSizeXEdt.setScale(0.5f);
	m_winSizeXEdt.setPosition(6.0f + 68.0f,h);
	m_winSizeXEdt.setEventProc(ctrlFun,this);
	m_winSizeYEdt.initWithoutSkin(128.0f,"600");
	m_winSizeYEdt.setScale(0.5f);
	m_winSizeYEdt.setPosition(6.0f + 132.0f,h);
	m_winSizeYEdt.setEventProc(ctrlFun,this);
	m_tab.addObjToTab(&m_winSizeTxt,"系统");
	m_tab.addObjToTab(&m_winSizeXEdt,"系统");
	m_tab.addObjToTab(&m_winSizeYEdt,"系统");
	h += 18;
	m_prjNameTxt.init("工程名称:");
	m_prjNameTxt.setScale(0.5f);
	m_prjNameTxt.setPosition(6.0f,h);
	m_prjNameEdt.initWithoutSkin(XVector2(256.0f,32.0f),"defaultUI");
	m_prjNameEdt.setScale(0.5f);
	m_prjNameEdt.setPosition(6.0f + 68.0f,h);
	m_tab.addObjToTab(&m_prjNameTxt,"系统");
	m_tab.addObjToTab(&m_prjNameEdt,"系统");
	h += 18;
	m_saveBtn.initWithoutSkin("保存",XVector2(128.0f,32.0f));
	m_saveBtn.setScale(0.5f);
	m_saveBtn.setPosition(6.0f,h);
	m_saveBtn.setEventProc(ctrlFun,this);
	m_readBtn.initWithoutSkin("读取",XVector2(128.0f,32.0f));
	m_readBtn.setScale(0.5f);
	m_readBtn.setPosition(6.0f + 64.0f,h);
	m_readBtn.setEventProc(ctrlFun,this);
	m_codeBtn.initWithoutSkin("生成",XVector2(128.0f,32.0f));
	m_codeBtn.setScale(0.5f);
	m_codeBtn.setPosition(6.0f + 128.0f,h);
	m_codeBtn.setEventProc(ctrlFun,this);
	m_tab.addObjToTab(&m_saveBtn,"系统");
	m_tab.addObjToTab(&m_readBtn,"系统");
	m_tab.addObjToTab(&m_codeBtn,"系统");
	return true;
}
bool GGame::init()
{
	XEE::setVSync(false);
	if(!createUI()) return false;
	//变量初始化
	//控件
	//属性
	//系统
	//下面是为了键盘移动而定义的变量
	m_keyUp.init(XKEY_UP,500,10);
	m_keyDown.init(XKEY_DOWN,500,10);
	m_keyLeft.init(XKEY_LEFT,500,10);
	m_keyRight.init(XKEY_RIGHT,500,10);

	m_winWidth = 800;
	m_winHeight = 600;
	XEG.m_drawAfterCtrl = drawTop;
	return true;
}
void GGame::move(float stepTime)
{
	if(m_keyUp.move(stepTime))
	{
		//int index = getCurChooseCtrlIndex();
		//if(index >= 0 && m_ctrlPosXEdt.getEdtIsNumber() &&
		//	m_ctrlPosYEdt.getEdtIsNumber())
		//{
		//	m_ctrls[index].pCtrl->setPosition(m_ctrlPosXEdt.getAsFloat(),
		//		m_ctrlPosYEdt.getAsFloat() - 1.0f);
		//	//下面更新数据
		//	m_ctrlPosYEdt.setString(XString::toString((int)(m_ctrls[index].pCtrl->getPosition().y)).c_str());
		//}
		//这里需要移动所有被选中的控件
		int index;
		for(int i = 0;i < m_curChooseCtrls.size();++ i)
		{
			index = getCtrlIndexByID(m_curChooseCtrls[i]);
			if(index >= 0)
			{
				if(m_curChooseCtrls[i] == m_curChooseCtrlID && m_ctrlPosXEdt.getEdtIsNumber() &&
					m_ctrlPosYEdt.getEdtIsNumber())
				{
					m_ctrls[index].pCtrl->setPosition(m_ctrlPosXEdt.getAsFloat(),
						m_ctrlPosYEdt.getAsFloat() - 1.0f);
					//下面更新数据
					m_ctrlPosYEdt.setString(XString::toString((int)(m_ctrls[index].pCtrl->getPosition().y)).c_str());
				}
				if(m_curChooseCtrls[i] != m_curChooseCtrlID)
				{
					m_ctrls[index].pCtrl->setPosition(m_ctrls[index].pCtrl->getPosition().x,
						m_ctrls[index].pCtrl->getPosition().y - 1.0f);
				}
			}
		}
	}
	if(m_keyDown.move(stepTime))
	{
		//int index = getCurChooseCtrlIndex();
		//if(index >= 0 && m_ctrlPosXEdt.getEdtIsNumber() &&
		//	m_ctrlPosYEdt.getEdtIsNumber())
		//{
		//	m_ctrls[index].pCtrl->setPosition(m_ctrlPosXEdt.getAsFloat(),
		//		m_ctrlPosYEdt.getAsFloat() + 1.0f);
		//	//下面更新数据
		//	m_ctrlPosYEdt.setString(XString::toString((int)(m_ctrls[index].pCtrl->getPosition().y)).c_str());
		//}
		int index;
		for(int i = 0;i < m_curChooseCtrls.size();++ i)
		{
			index = getCtrlIndexByID(m_curChooseCtrls[i]);
			if(index >= 0)
			{
				if(m_curChooseCtrls[i] == m_curChooseCtrlID && m_ctrlPosXEdt.getEdtIsNumber() &&
					m_ctrlPosYEdt.getEdtIsNumber())
				{
					m_ctrls[index].pCtrl->setPosition(m_ctrlPosXEdt.getAsFloat(),
						m_ctrlPosYEdt.getAsFloat() + 1.0f);
					//下面更新数据
					m_ctrlPosYEdt.setString(XString::toString((int)(m_ctrls[index].pCtrl->getPosition().y)).c_str());
				}
				if(m_curChooseCtrls[i] != m_curChooseCtrlID)
				{
					m_ctrls[index].pCtrl->setPosition(m_ctrls[index].pCtrl->getPosition().x,
						m_ctrls[index].pCtrl->getPosition().y + 1.0f);
				}
			}
		}
	}
	if(m_keyLeft.move(stepTime))
	{
		//int index = getCurChooseCtrlIndex();
		//if(index >= 0 && m_ctrlPosXEdt.getEdtIsNumber() &&
		//	m_ctrlPosYEdt.getEdtIsNumber())
		//{
		//	m_ctrls[index].pCtrl->setPosition(m_ctrlPosXEdt.getAsFloat() - 1.0f,
		//		m_ctrlPosYEdt.getAsFloat());
		//	//下面更新数据
		//	m_ctrlPosXEdt.setString(XString::toString((int)(m_ctrls[index].pCtrl->getPosition().x)).c_str());
		//}
		int index;
		for(int i = 0;i < m_curChooseCtrls.size();++ i)
		{
			index = getCtrlIndexByID(m_curChooseCtrls[i]);
			if(index >= 0)
			{
				if(m_curChooseCtrls[i] == m_curChooseCtrlID && m_ctrlPosXEdt.getEdtIsNumber() &&
					m_ctrlPosYEdt.getEdtIsNumber())
				{
					m_ctrls[index].pCtrl->setPosition(m_ctrlPosXEdt.getAsFloat() - 1.0f,
						m_ctrlPosYEdt.getAsFloat());
					//下面更新数据
					m_ctrlPosXEdt.setString(XString::toString((int)(m_ctrls[index].pCtrl->getPosition().x)).c_str());
				}
				if(m_curChooseCtrls[i] != m_curChooseCtrlID)
				{
					m_ctrls[index].pCtrl->setPosition(m_ctrls[index].pCtrl->getPosition().x - 1.0f,
						m_ctrls[index].pCtrl->getPosition().y);
				}
			}
		}
	}
	if(m_keyRight.move(stepTime))
	{
		//int index = getCurChooseCtrlIndex();
		//if(index >= 0 && m_ctrlPosXEdt.getEdtIsNumber() &&
		//	m_ctrlPosYEdt.getEdtIsNumber())
		//{
		//	m_ctrls[index].pCtrl->setPosition(m_ctrlPosXEdt.getAsFloat() + 1.0f,
		//		m_ctrlPosYEdt.getAsFloat());
		//	//下面更新数据
		//	m_ctrlPosXEdt.setString(XString::toString((int)(m_ctrls[index].pCtrl->getPosition().x)).c_str());
		//}
		int index;
		for(int i = 0;i < m_curChooseCtrls.size();++ i)
		{
			index = getCtrlIndexByID(m_curChooseCtrls[i]);
			if(index >= 0)
			{
				if(m_curChooseCtrls[i] == m_curChooseCtrlID && m_ctrlPosXEdt.getEdtIsNumber() &&
					m_ctrlPosYEdt.getEdtIsNumber())
				{
					m_ctrls[index].pCtrl->setPosition(m_ctrlPosXEdt.getAsFloat() + 1.0f,
						m_ctrlPosYEdt.getAsFloat());
					//下面更新数据
					m_ctrlPosXEdt.setString(XString::toString((int)(m_ctrls[index].pCtrl->getPosition().x)).c_str());
				}
				if(m_curChooseCtrls[i] != m_curChooseCtrlID)
				{
					m_ctrls[index].pCtrl->setPosition(m_ctrls[index].pCtrl->getPosition().x + 1.0f,
						m_ctrls[index].pCtrl->getPosition().y);
				}
			}
		}
	}
}
void GGame::draw()
{
	XRender::drawBox(0.0f,0.0f,m_winWidth,m_winHeight);	//描绘窗口的界线
}
void GGame::drawUp()
{
	//由于在控件之下，所以看不到
	for(int i = 0;i < m_ctrls.size();++ i)
	{
		if(m_ctrls[i].isMouseOn)
		{
			XRender::drawBox(m_ctrls[i].pCtrl->getBox(0),
				m_ctrls[i].pCtrl->getBox(1),
				m_ctrls[i].pCtrl->getBox(2),
				m_ctrls[i].pCtrl->getBox(3),1.0f,0.0f,0.0f,1.0f);
		}
		for(int j = 0;j < m_curChooseCtrls.size();++ j)
		{
			if(m_curChooseCtrls[j] == m_ctrls[i].ctrlID)
			{
				if(m_curChooseCtrls[j] == m_curChooseCtrlID)
					XRender::drawBox(m_ctrls[i].pCtrl->getBox(0),
						m_ctrls[i].pCtrl->getBox(1),
						m_ctrls[i].pCtrl->getBox(2),
						m_ctrls[i].pCtrl->getBox(3),1.0f,1.0f,0.0f,0.0f);
				else
					XRender::drawBox(m_ctrls[i].pCtrl->getBox(0),
						m_ctrls[i].pCtrl->getBox(1),
						m_ctrls[i].pCtrl->getBox(2),
						m_ctrls[i].pCtrl->getBox(3),1.0f,1.0f,1.0f,0.0f);
				break;
			}
		}
	}
	//下面显示被选择的控件
	if(m_isMouseSelect)
		XRender::drawRectAntiColor(XRect(m_mouseCurPos.x,m_mouseCurPos.y,
			m_mouseDownPos.x,m_mouseDownPos.y),1.0f,XRender::LS_DASHES);
}
void GGame::input(const XInputEvent &event)
{
	switch(event.type)
	{
	case EVENT_RESIZE://窗口大小发生变化
		//m_tab.setPosition(XEE::windowData.w - 256.0f,0.0f);
		break;
	case EVENT_MOUSE:
		switch(event.mouseState)
		{
		case MOUSE_MOVE:
			m_curMousePosTxt.setString(("x:" + XString::toString((int)(XEG.getMousePos().x)) +
				"y:" + XString::toString((int)(XEG.getMousePos().y))).c_str());
			//下面计算控件书否处于鼠标悬浮状态
			for(int i = 0;i < m_ctrls.size();++ i)
			{
				if(m_ctrls[i].pCtrl->getVisible() &&
					m_ctrls[i].pCtrl->isInRect(XEG.getMousePos().x,XEG.getMousePos().y))
					m_ctrls[i].isMouseOn = XTrue;
				else m_ctrls[i].isMouseOn = XFalse;
			}
			if(m_isMouseSelect)
			{//下面是鼠标片选
				m_mouseCurPos.set(event.mouseX,event.mouseY);
				updateMouseSelect();
			}
			if(m_isMouseDrag)
			{//鼠标拖动控件
				XVector2 dPos = XVector2(event.mouseX,event.mouseY) - m_mouseDragPos;
				m_mouseDragPos.set(event.mouseX,event.mouseY);
				int index;
				for(int i = 0;i < m_curChooseCtrls.size();++ i)
				{
					index = getCtrlIndexByID(m_curChooseCtrls[i]);
					if(index >= 0)
					{
						if(m_curChooseCtrls[i] == m_curChooseCtrlID && m_ctrlPosXEdt.getEdtIsNumber() &&
							m_ctrlPosYEdt.getEdtIsNumber())
						{
							m_ctrls[index].pCtrl->setPosition(m_ctrlPosXEdt.getAsFloat() + dPos.x,
								m_ctrlPosYEdt.getAsFloat() + dPos.y);
							//下面更新数据
							m_ctrlPosXEdt.setString(XString::toString((int)(m_ctrls[index].pCtrl->getPosition().x)).c_str());
							m_ctrlPosYEdt.setString(XString::toString((int)(m_ctrls[index].pCtrl->getPosition().y)).c_str());
						}
						if(m_curChooseCtrls[i] != m_curChooseCtrlID)
						{
							m_ctrls[index].pCtrl->setPosition(m_ctrls[index].pCtrl->getPosition() + dPos);
						}
					}
				}
			}
			break;
		case MOUSE_LEFT_BUTTON_DOWN:
			//XEE::makeABeep();
			if(m_subWindow.isInRect(event.mouseX,event.mouseY)) break;	//如果在子窗口上按下鼠标则这里不做处理
			//下面在制定位置建立相关的控件
			if(getCurChooseCtrlType() != CTRL_OBJ_NULL)
			{//鼠标左键创建一个控件
				if(XRect(0.0f,0.0f,m_winWidth,m_winHeight).isInRect(event.mouseX,event.mouseY))
				{
					m_curChooseCtrls.clear();
					m_curCtrlPos.set(event.mouseX,event.mouseY);
					addACtrl(getCurChooseCtrlType());
				}
			}else
			{//选择物件更新控件列表
				if(XRect(0.0f,0.0f,m_winWidth,m_winHeight).isInRect(event.mouseX,event.mouseY))
				{
					bool isDrag = false;	//判断是否准备拖动
					if(!m_isCtrlDown)
					{//如果没有按下Ctrl键
						//这里判断是否为准备拖动
						for(int i = 0;i < m_curChooseCtrls.size();++ i)
						{
							int index = getCtrlIndexByID(m_curChooseCtrls[i]);
							if(index >= 0 && m_ctrls[index].pCtrl->getVisible() && 
								m_ctrls[index].pCtrl->isInRect(event.mouseX,event.mouseY))
							{
								isDrag = true;
								break;
							}
						}
						if(!isDrag)
							m_curChooseCtrls.clear();
					}
					if(!isDrag && !updateCtrlList(event.mouseX,event.mouseY))
					{
						m_isMouseSelect = true;
						m_mouseDownPos.set(event.mouseX,event.mouseY);
						m_mouseCurPos = m_mouseDownPos;
					}else
					{
						if(!m_isCtrlDown)
						{//被拖动
							m_isMouseDrag = true;
							m_mouseDragPos.set(event.mouseX,event.mouseY);
						}
					}
				}
			}
			break;
		case MOUSE_LEFT_BUTTON_UP:
			m_isMouseSelect = false;
			m_isMouseDrag = false;
			break;
		}
		break;
	case EVENT_KEYBOARD:
		switch(event.keyState)
		{
		case KEY_STATE_DOWN:
			switch(event.keyValue)
			{
			case XKEY_LCTRL:
				m_isCtrlDown = true;
				break;
			}
			break;
		case KEY_STATE_UP:
			switch(event.keyValue)
			{
			case XKEY_LCTRL:
				m_isCtrlDown = false;
				break;
			}
			break;
		}
		break;
	}
	if(!m_subWindow.isFocus())
	{//选择了物件才能移动
		m_keyUp.updateState(event);
		m_keyDown.updateState(event);
		m_keyLeft.updateState(event);
		m_keyRight.updateState(event);
	}
}
void GGame::release()
{
	for(int i = 0;i < m_ctrls.size();++ i)
	{
		XMem::XDELETE(m_ctrls[i].pCtrl);
	}
	m_ctrls.clear();
	m_chooseCtrlMltlst.setLineSum(0);
	m_ctrlID = 0;
}
XCtrlObjType GGame::getCurChooseCtrlType()const	//获取目前选择的控件的类型
{
	static const XCtrlObjType ctrlTypeMap[] = {
		CTRL_OBJ_NULL,		
		CTRL_OBJ_BUTTON,	
		CTRL_OBJ_BTNBAR,		
		CTRL_OBJ_BUTTONEX,	
		CTRL_OBJ_CALENDAR,		
		CTRL_OBJ_CHART,		
		CTRL_OBJ_CHECK,		
		CTRL_OBJ_COMBO,		
		CTRL_OBJ_DIRECTORYLIST,	
		CTRL_OBJ_EDIT,		
		CTRL_OBJ_IMAGE,		
		CTRL_OBJ_GROUP,	
		CTRL_OBJ_IMAGELIST,		
		CTRL_OBJ_LINECTRL,		
		CTRL_OBJ_MENU,			
		CTRL_OBJ_MOUSERIGHTBUTTONMENU,	
		CTRL_OBJ_MUTILIST,	
		CTRL_OBJ_NULL,	//带选项的多行列表
		CTRL_OBJ_MUTITEXT,	
		CTRL_OBJ_PASSWORDPAD,	
		CTRL_OBJ_POINTCTRL,		
		CTRL_OBJ_PROGRESS,	
		CTRL_OBJ_PROGRESSRING,	
		CTRL_OBJ_RADIOS,	
		CTRL_OBJ_SIMPLELINE,	
		CTRL_OBJ_SLIDER,	
		CTRL_OBJ_SLIDEREX,	
		CTRL_OBJ_TAB,			
		CTRL_OBJ_TEXT,			
		CTRL_OBJ_TOOLBAR,		
		CTRL_OBJ_FUNCTION,};
	if(m_ctrlChooseRds.getCurChoose() < 0 || 
		m_ctrlChooseRds.getCurChoose() >= 30) return CTRL_OBJ_NULL;
	return ctrlTypeMap[m_ctrlChooseRds.getCurChoose()];
}
int GGame::getNewMouseChooseCtrl(float x,float y)			//获取目前鼠标选择的控件的编号
{
	for(int i = 0;i < m_ctrls.size();++ i)
	{
		if(m_ctrls[i].pCtrl->getVisible() && 
			m_ctrls[i].pCtrl->isInRect(x,y)) return i;
	}
	return -1;
}
void GGame::setCurChooseCtrlType(XCtrlObjType type)
{
	switch(type)
	{
	case CTRL_OBJ_NULL:m_ctrlChooseRds.setChoosed(0);break;		
	case CTRL_OBJ_BUTTON:m_ctrlChooseRds.setChoosed(1);break;		
	case CTRL_OBJ_BTNBAR:m_ctrlChooseRds.setChoosed(2);break;			
	case CTRL_OBJ_BUTTONEX:m_ctrlChooseRds.setChoosed(3);break;		
	case CTRL_OBJ_CALENDAR:m_ctrlChooseRds.setChoosed(4);break;			
	case CTRL_OBJ_CHART:m_ctrlChooseRds.setChoosed(5);break;			
	case CTRL_OBJ_CHECK:m_ctrlChooseRds.setChoosed(6);break;			
	case CTRL_OBJ_COMBO:m_ctrlChooseRds.setChoosed(7);break;			
	case CTRL_OBJ_DIRECTORYLIST:m_ctrlChooseRds.setChoosed(8);break;		
	case CTRL_OBJ_EDIT:m_ctrlChooseRds.setChoosed(9);break;			
	case CTRL_OBJ_IMAGE:m_ctrlChooseRds.setChoosed(10);break;			
	case CTRL_OBJ_GROUP:m_ctrlChooseRds.setChoosed(11);break;		
	case CTRL_OBJ_IMAGELIST:m_ctrlChooseRds.setChoosed(12);break;			
	case CTRL_OBJ_LINECTRL:m_ctrlChooseRds.setChoosed(13);break;		
	case CTRL_OBJ_MENU:m_ctrlChooseRds.setChoosed(14);break;			
	case CTRL_OBJ_MOUSERIGHTBUTTONMENU:m_ctrlChooseRds.setChoosed(15);break;		
	case CTRL_OBJ_MUTILIST:m_ctrlChooseRds.setChoosed(16);break;	
	//带选项的多行列表
	case CTRL_OBJ_MUTITEXT:m_ctrlChooseRds.setChoosed(18);break;		
	case CTRL_OBJ_PASSWORDPAD:m_ctrlChooseRds.setChoosed(19);break;		
	case CTRL_OBJ_POINTCTRL:m_ctrlChooseRds.setChoosed(20);break;			
	case CTRL_OBJ_PROGRESS:m_ctrlChooseRds.setChoosed(21);break;		
	case CTRL_OBJ_PROGRESSRING:m_ctrlChooseRds.setChoosed(22);break;		
	case CTRL_OBJ_RADIOS:m_ctrlChooseRds.setChoosed(23);break;		
	case CTRL_OBJ_SIMPLELINE:m_ctrlChooseRds.setChoosed(24);break;		
	case CTRL_OBJ_SLIDER:m_ctrlChooseRds.setChoosed(25);break;		
	case CTRL_OBJ_SLIDEREX:m_ctrlChooseRds.setChoosed(26);break;		
	case CTRL_OBJ_TAB:m_ctrlChooseRds.setChoosed(27);break;				
	case CTRL_OBJ_TEXT:m_ctrlChooseRds.setChoosed(28);break;				
	case CTRL_OBJ_TOOLBAR:m_ctrlChooseRds.setChoosed(29);break;			
	case CTRL_OBJ_FUNCTION:m_ctrlChooseRds.setChoosed(0);break;	
	}
}
XBool GGame::delSelectCtrl()	//删除所有选择的控件
{
	int index;
	for(int i = 0;i < m_curChooseCtrls.size();++ i)
	{
		index = getCtrlIndexByID(m_curChooseCtrls[i]);
		if(index >= 0)
		{//下面删除指定控件
			GCtrlInfo tmp = m_ctrls[index];
			for(int j = index;j < m_ctrls.size() - 1;++ j)
			{
				m_ctrls[j] = m_ctrls[j + 1];
			}
			m_ctrls.pop_back();
			if(tmp.pCtrl->getCtrlType() == CTRL_OBJ_TAB)//释放从属关系
				((XTab *)tmp.pCtrl)->clearAllObj();	//清除所有的从属关系
			if(m_curChooseCtrlID == tmp.ctrlID)
				m_curChooseCtrlID = -1;
			XMem::XDELETE(tmp.pCtrl);
			//这里需要更新列表
			m_chooseCtrlMltlst.deleteLine(m_chooseCtrlMltlst.getSelectIndex());
			updateAffiliation();
		}
	}
	return true;
}
XBool GGame::delACtrl()
{
	int index = getCurChooseCtrlIndex();
	if(index >= 0)
	{//下面删除指定控件
		GCtrlInfo tmp = m_ctrls[index];
		for(int i = index;i < m_ctrls.size() - 1;++ i)
		{
			m_ctrls[i] = m_ctrls[i + 1];
		}
		m_ctrls.pop_back();
		if(tmp.pCtrl->getCtrlType() == CTRL_OBJ_TAB)//释放从属关系
			((XTab *)tmp.pCtrl)->clearAllObj();	//清除所有的从属关系
		if(m_curChooseCtrlID == tmp.ctrlID)
			m_curChooseCtrlID = -1;
		XMem::XDELETE(tmp.pCtrl);
		//这里需要更新列表
		m_chooseCtrlMltlst.deleteLine(m_chooseCtrlMltlst.getSelectIndex());
		updateAffiliation();
		return XTrue;
	}
	return XFalse;
}
XBool GGame::addACtrl(XCtrlObjType type)
{
	switch(type)
	{
	case CTRL_OBJ_NULL:
		return XFalse;	//无效的参数	
	case CTRL_OBJ_BUTTON:
		{
			GCtrlInfo tmp;
			XButton *pObj = XMem::createMem<XButton>();
			if(pObj == NULL) return XFalse;
			pObj->initWithoutSkin("按钮",128);
			pObj->setPosition(m_curCtrlPos);
			tmp.ctrlID = m_ctrlID;
			tmp.ctrlName = "m_btn" + XString::toString(m_ctrlID);
			tmp.pCtrl = pObj;
			++ m_ctrlID;
			if(!m_isCtrlDown)
				setCurChooseCtrlType(CTRL_OBJ_NULL);
			m_ctrls.push_back(tmp);
			return XTrue;
		}
		break;		
	case CTRL_OBJ_BTNBAR:break;			
	case CTRL_OBJ_BUTTONEX:break;		
	case CTRL_OBJ_CALENDAR:break;			
	case CTRL_OBJ_CHART:break;			
	case CTRL_OBJ_CHECK:
		{
			GCtrlInfo tmp;
			XCheck *pObj = XMem::createMem<XCheck>();
			if(pObj == NULL) return XFalse;
			pObj->initWithoutSkin("单选项");
			pObj->setPosition(m_curCtrlPos);
			tmp.ctrlID = m_ctrlID;
			tmp.ctrlName = "m_chk" + XString::toString(m_ctrlID);
			tmp.pCtrl = pObj;
			++ m_ctrlID;
			if(!m_isCtrlDown)
				setCurChooseCtrlType(CTRL_OBJ_NULL);
			m_ctrls.push_back(tmp);
			return XTrue;
		}
		break;			
	case CTRL_OBJ_COMBO:
		{
			GCtrlInfo tmp;
			XCombo *pObj = XMem::createMem<XCombo>();
			if(pObj == NULL) return XFalse;
			pObj->initWithoutSkin(128.0f,5,3);
			pObj->setPosition(m_curCtrlPos);
			tmp.ctrlID = m_ctrlID;
			tmp.ctrlName = "m_cmb" + XString::toString(m_ctrlID);
			for(int i = 0;i < 5;++ i)
			{
				pObj->setMenuStr(("menu" + XString::toString(i)).c_str(),i);
			}
			tmp.pCtrl = pObj;
			tmp.allMenuSum = 5;
			tmp.showMenuSum = 3;
			++ m_ctrlID;
			if(!m_isCtrlDown)
				setCurChooseCtrlType(CTRL_OBJ_NULL);
			m_ctrls.push_back(tmp);
			return XTrue;
		}
		break;			
	case CTRL_OBJ_DIRECTORYLIST:break;		
	case CTRL_OBJ_EDIT:
		{
			GCtrlInfo tmp;
			XEdit *pObj = XMem::createMem<XEdit>();
			if(pObj == NULL) return XFalse;
			pObj->initWithoutSkin(128.0f,"输入框");
			pObj->setPosition(m_curCtrlPos);
			tmp.ctrlID = m_ctrlID;
			tmp.ctrlName = "m_edt" + XString::toString(m_ctrlID);
			tmp.pCtrl = pObj;
			++ m_ctrlID;
			if(!m_isCtrlDown)
				setCurChooseCtrlType(CTRL_OBJ_NULL);
			m_ctrls.push_back(tmp);
			return XTrue;
		}
		break;			
	case CTRL_OBJ_IMAGE:break;			
	case CTRL_OBJ_GROUP:break;		
	case CTRL_OBJ_IMAGELIST:break;			
	case CTRL_OBJ_LINECTRL:break;		
	case CTRL_OBJ_MENU:break;			
	case CTRL_OBJ_MOUSERIGHTBUTTONMENU:break;		
	case CTRL_OBJ_MUTILIST:
		{
			GCtrlInfo tmp;
			XMultiList *pObj = XMem::createMem<XMultiList>();
			if(pObj == NULL) return XFalse;
			pObj->initWithoutSkin(XVector2(256.0f,256.0f),3,0);
			pObj->setPosition(m_curCtrlPos);
			tmp.ctrlID = m_ctrlID;
			tmp.ctrlName = "m_mltLst" + XString::toString(m_ctrlID);
			tmp.allMenuSum = 3;
			tmp.showMenuSum = 0;
			tmp.pCtrl = pObj;
			++ m_ctrlID;
			if(!m_isCtrlDown)
				setCurChooseCtrlType(CTRL_OBJ_NULL);
			m_ctrls.push_back(tmp);
			return XTrue;
		}
		break;		
	case CTRL_OBJ_MUTITEXT:
		{
			GCtrlInfo tmp;
			XMultiText *pObj = XMem::createMem<XMultiText>();
			if(pObj == NULL) return XFalse;
			pObj->initWithoutSkin(XVector2(256.0f,256.0f),"多行文本框");
			pObj->setPosition(m_curCtrlPos);
			tmp.ctrlID = m_ctrlID;
			tmp.ctrlName = "m_mltTxt" + XString::toString(m_ctrlID);
			tmp.pCtrl = pObj;
			++ m_ctrlID;
			if(!m_isCtrlDown)
				setCurChooseCtrlType(CTRL_OBJ_NULL);
			m_ctrls.push_back(tmp);
			return XTrue;
		}
		break;		
	case CTRL_OBJ_PASSWORDPAD:break;		
	case CTRL_OBJ_POINTCTRL:break;			
	case CTRL_OBJ_PROGRESS:break;		
	case CTRL_OBJ_PROGRESSRING:break;		
	case CTRL_OBJ_RADIOS:break;		
	case CTRL_OBJ_SIMPLELINE:
		{
			GCtrlInfo tmp;
			XSimpleLine *pObj = XMem::createMem<XSimpleLine>();
			if(pObj == NULL) return XFalse;
			pObj->init(128.0f);
			pObj->setPosition(m_curCtrlPos);
			tmp.ctrlID = m_ctrlID;
			tmp.ctrlName = "m_spl" + XString::toString(m_ctrlID);
			tmp.pCtrl = pObj;
			++ m_ctrlID;
			if(!m_isCtrlDown)
				setCurChooseCtrlType(CTRL_OBJ_NULL);
			m_ctrls.push_back(tmp);
			return XTrue;
		}
		break;		
	case CTRL_OBJ_SLIDER:break;		
	case CTRL_OBJ_SLIDEREX:break;		
	case CTRL_OBJ_TAB:
		{
			GCtrlInfo tmp;
			XTab *pObj = XMem::createMem<XTab>();
			if(pObj == NULL) return XFalse;
			pObj->initWithoutSkin(XVector2(256.0f,256.0f));
			pObj->setPosition(m_curCtrlPos);
			for(int i = 0;i < 3;++ i)
			{
				pObj->addATab("tab" + XString::toString(i));
			}
			tmp.ctrlID = m_ctrlID;
			tmp.withCallback = XFalse;
			tmp.ctrlName = "m_tab" + XString::toString(m_ctrlID);
			tmp.pCtrl = pObj;
			tmp.allMenuSum = 3;
			tmp.showMenuSum = 0;
			++ m_ctrlID;
			if(!m_isCtrlDown)
				setCurChooseCtrlType(CTRL_OBJ_NULL);
			m_ctrls.push_back(tmp);
			return XTrue;
		}
		break;				
	case CTRL_OBJ_TEXT:
		{
			GCtrlInfo tmp;
			XText *pObj = XMem::createMem<XText>();
			if(pObj == NULL) return XFalse;
			pObj->init("静态文本");
			pObj->setPosition(m_curCtrlPos);
			tmp.ctrlID = m_ctrlID;
			tmp.withCallback = XFalse;
			tmp.ctrlName = "m_txt" + XString::toString(m_ctrlID);
			tmp.pCtrl = pObj;
			++ m_ctrlID;
			if(!m_isCtrlDown)
				setCurChooseCtrlType(CTRL_OBJ_NULL);
			m_ctrls.push_back(tmp);
			return XTrue;
		}
		break;				
	case CTRL_OBJ_TOOLBAR:break;			
	case CTRL_OBJ_FUNCTION:
		return XFalse;	//无效的参数
	}
	return XFalse;
}
int GGame::isSelect(int id)
{
	for(int i = 0;i < m_curChooseCtrls.size(); ++ i)
	{
		if(m_curChooseCtrls[i] == id) return i;
	}
	return -1;
}
bool GGame::updateCtrlList(float x,float y)
{
	bool ret = false;
	m_chooseCtrlMltlst.setLineSum(0);
	for(int i = 0;i < m_ctrls.size();++ i)
	{
		if(m_ctrls[i].pCtrl->getVisible() && 
			m_ctrls[i].pCtrl->isInRect(x,y))
		{//添加控件
			ret = true;
			int index = isSelect(m_ctrls[i].ctrlID);
			if(index >= 0)//去掉这个选择
				m_curChooseCtrls.erase(m_curChooseCtrls.begin() + index);	//选择中加入该控件
			else
				m_curChooseCtrls.push_back(m_ctrls[i].ctrlID);	//选择中加入该控件
		}
	}
	//下面更新列表框的数据
	for(int i = 0;i < m_curChooseCtrls.size();++ i)
	{
		int order = getCtrlIndexByID(m_curChooseCtrls[i]);
		if(order < 0) continue;
		m_chooseCtrlMltlst.addALine();
		int index = m_chooseCtrlMltlst.getLineSum() - 1;
		m_chooseCtrlMltlst.setBoxStr(XString::toString(m_ctrls[order].ctrlID).c_str(),index,0);
		m_chooseCtrlMltlst.setBoxStr(m_ctrls[order].ctrlName.c_str(),index,1);
		m_chooseCtrlMltlst.setBoxStr(XCtrl::getCtrlTypeByString(m_ctrls[order].pCtrl->getCtrlType()).c_str(),index,2);
	}
	if(m_curChooseCtrls.size() > 0)
	{
		m_curChooseCtrlID = m_curChooseCtrls[m_curChooseCtrls.size() - 1];
		updateCtrlProperty();
	}
	return ret;
}
void GGame::updateMouseSelect()
{
	XRect rect;
	rect.setAndOrder(m_mouseCurPos.x,m_mouseCurPos.y,m_mouseDownPos.x,m_mouseDownPos.y);
	m_curChooseCtrls.clear();
	m_chooseCtrlMltlst.setLineSum(0);
	for(int i = 0;i < m_ctrls.size();++ i)
	{
		if(m_ctrls[i].pCtrl->getVisible() && 
			XRect(m_ctrls[i].pCtrl->getBox(0).x,m_ctrls[i].pCtrl->getBox(0).y,
			m_ctrls[i].pCtrl->getBox(2).x,m_ctrls[i].pCtrl->getBox(2).y).isCrash(rect))
		{//添加控件
			int index = isSelect(m_ctrls[i].ctrlID);
			if(index >= 0)//去掉这个选择
				m_curChooseCtrls.erase(m_curChooseCtrls.begin() + index);	//选择中加入该控件
			else
				m_curChooseCtrls.push_back(m_ctrls[i].ctrlID);	//选择中加入该控件
		}
	}
	//下面更新列表框的数据
	for(int i = 0;i < m_curChooseCtrls.size();++ i)
	{
		int order = getCtrlIndexByID(m_curChooseCtrls[i]);
		if(order < 0) continue;
		m_chooseCtrlMltlst.addALine();
		int index = m_chooseCtrlMltlst.getLineSum() - 1;
		m_chooseCtrlMltlst.setBoxStr(XString::toString(m_ctrls[order].ctrlID).c_str(),index,0);
		m_chooseCtrlMltlst.setBoxStr(m_ctrls[order].ctrlName.c_str(),index,1);
		m_chooseCtrlMltlst.setBoxStr(XCtrl::getCtrlTypeByString(m_ctrls[order].pCtrl->getCtrlType()).c_str(),index,2);
	}
	if(m_curChooseCtrls.size() > 0)
	{
		m_curChooseCtrlID = m_curChooseCtrls[m_curChooseCtrls.size() - 1];
		updateCtrlProperty();
	}
}
int GGame::getCtrlIDFromList()
{
	int index = m_chooseCtrlMltlst.getSelectIndex();
	if(index < 0) return -1;
	return atoi(m_chooseCtrlMltlst.getBoxStr(index,0));
}
void GGame::updateCtrlProperty()
{
	int order = getCtrlIndexByID(m_curChooseCtrlID);
	if(order < 0) return;
	m_ctrlNameEdt.setString(m_ctrls[order].ctrlName.c_str());
	XVector2 pos = m_ctrls[order].pCtrl->getPosition();
	m_ctrlPosXEdt.setString(XString::toString((int)(pos.x)).c_str());
	m_ctrlPosYEdt.setString(XString::toString((int)(pos.y)).c_str());
	XVector2 size = m_ctrls[order].pCtrl->getPixelSize();
	m_ctrlPixelSizeXEdt.setString(XString::toString((int)(size.x)).c_str());
	m_ctrlPixelSizeYEdt.setString(XString::toString((int)(size.y)).c_str());
	XVector2 scale = m_ctrls[order].pCtrl->getScale();
	m_ctrlScaleSizeXEdt.setString(XString::toString(scale.x).c_str());
	m_ctrlScaleSizeYEdt.setString(XString::toString(scale.y).c_str());
	m_ctrlAffiliationEdt.setString(m_ctrls[order].affiliationStr.c_str());
	m_ctrlCommentEdt.setString(m_ctrls[order].ctrlComment.c_str());
	m_ctrlExplanationEdt.setString(m_ctrls[order].codeExplanation.c_str());
	switch(m_ctrls[order].pCtrl->getCtrlType())
	{
	case CTRL_OBJ_NULL:break;
	case CTRL_OBJ_BUTTON:
		m_ctrlCaptionEdt.enable();
		m_ctrlCaptionEdt.setString(((XButton *)m_ctrls[order].pCtrl)->getCaptionString());
		m_withCallbackChk.setState(m_ctrls[order].withCallback);
		m_withCallbackChk.enable();
		m_cmbAllMenuSumTxt.disVisible();
		m_cmbAllMenuSumEdt.disVisible();
		m_cmbShowMenuSumTxt.disVisible();
		m_cmbShowMenuSumEdt.disVisible();
		break;		
	case CTRL_OBJ_BTNBAR:break;			
	case CTRL_OBJ_BUTTONEX:break;		
	case CTRL_OBJ_CALENDAR:break;			
	case CTRL_OBJ_CHART:break;			
	case CTRL_OBJ_CHECK:
		m_ctrlCaptionEdt.enable();
		m_ctrlCaptionEdt.setString(((XCheck *)m_ctrls[order].pCtrl)->getCaptionString());
		m_withCallbackChk.setState(m_ctrls[order].withCallback);
		m_withCallbackChk.enable();
		m_cmbAllMenuSumTxt.disVisible();
		m_cmbAllMenuSumEdt.disVisible();
		m_cmbShowMenuSumTxt.disVisible();
		m_cmbShowMenuSumEdt.disVisible();
		break;			
	case CTRL_OBJ_COMBO:
		m_ctrlCaptionEdt.enable();
		m_ctrlCaptionEdt.setString(((XCombo *)m_ctrls[order].pCtrl)->getMenuStr().c_str());
		m_cmbAllMenuSumEdt.setString(XString::toString(m_ctrls[order].allMenuSum).c_str());
		m_cmbShowMenuSumEdt.setString(XString::toString(m_ctrls[order].showMenuSum).c_str());
		m_withCallbackChk.setState(m_ctrls[order].withCallback);
		m_cmbAllMenuSumTxt.setVisible();
		m_cmbAllMenuSumEdt.setVisible();
		m_cmbShowMenuSumTxt.setVisible();
		m_cmbShowMenuSumEdt.setVisible();
		break;			
	case CTRL_OBJ_DIRECTORYLIST:break;		
	case CTRL_OBJ_EDIT:
		m_ctrlCaptionEdt.enable();
		m_ctrlCaptionEdt.setString(((XEdit *)m_ctrls[order].pCtrl)->getString());
		m_withCallbackChk.setState(m_ctrls[order].withCallback);
		m_withCallbackChk.enable();
		m_cmbAllMenuSumTxt.disVisible();
		m_cmbAllMenuSumEdt.disVisible();
		m_cmbShowMenuSumTxt.disVisible();
		m_cmbShowMenuSumEdt.disVisible();
		break;			
	case CTRL_OBJ_IMAGE:break;			
	case CTRL_OBJ_GROUP:break;		
	case CTRL_OBJ_IMAGELIST:break;			
	case CTRL_OBJ_LINECTRL:break;		
	case CTRL_OBJ_MENU:break;			
	case CTRL_OBJ_MOUSERIGHTBUTTONMENU:break;		
	case CTRL_OBJ_MUTILIST:
		m_ctrlCaptionEdt.enable();
		m_ctrlCaptionEdt.setString(((XMultiList *)m_ctrls[order].pCtrl)->getTitleStr().c_str());
		m_cmbAllMenuSumEdt.setString(XString::toString(m_ctrls[order].allMenuSum).c_str());
		m_cmbShowMenuSumEdt.setString(XString::toString(m_ctrls[order].showMenuSum).c_str());
		m_withCallbackChk.setState(m_ctrls[order].withCallback);
		m_withCallbackChk.disable();
		m_cmbAllMenuSumTxt.setVisible();
		m_cmbAllMenuSumEdt.setVisible();
		m_cmbShowMenuSumTxt.setVisible();
		m_cmbShowMenuSumEdt.setVisible();
		break;		
	case CTRL_OBJ_MUTITEXT:
		m_ctrlCaptionEdt.enable();
		m_ctrlCaptionEdt.setString(((XMultiText *)m_ctrls[order].pCtrl)->getALineString(0));
		m_withCallbackChk.setState(m_ctrls[order].withCallback);
		m_withCallbackChk.disable();
		m_cmbAllMenuSumTxt.disVisible();
		m_cmbAllMenuSumEdt.disVisible();
		m_cmbShowMenuSumTxt.disVisible();
		m_cmbShowMenuSumEdt.disVisible();
		break;		
	case CTRL_OBJ_PASSWORDPAD:break;		
	case CTRL_OBJ_POINTCTRL:break;			
	case CTRL_OBJ_PROGRESS:break;		
	case CTRL_OBJ_PROGRESSRING:break;		
	case CTRL_OBJ_RADIOS:break;		
	case CTRL_OBJ_SIMPLELINE:
		m_ctrlCaptionEdt.disable();
		m_withCallbackChk.setState(m_ctrls[order].withCallback);
		m_withCallbackChk.disable();
		m_cmbAllMenuSumTxt.disVisible();
		m_cmbAllMenuSumEdt.disVisible();
		m_cmbShowMenuSumTxt.disVisible();
		m_cmbShowMenuSumEdt.disVisible();
		break;		
	case CTRL_OBJ_SLIDER:break;		
	case CTRL_OBJ_SLIDEREX:break;		
	case CTRL_OBJ_TAB:
		m_ctrlCaptionEdt.enable();
		m_ctrlCaptionEdt.setString(((XTab *)m_ctrls[order].pCtrl)->getTabsStr().c_str());
		m_cmbAllMenuSumEdt.setString(XString::toString(m_ctrls[order].allMenuSum).c_str());
		m_withCallbackChk.setState(m_ctrls[order].withCallback);
		m_withCallbackChk.enable();
		m_cmbAllMenuSumTxt.setVisible();
		m_cmbAllMenuSumEdt.setVisible();
		m_cmbShowMenuSumTxt.disVisible();
		m_cmbShowMenuSumEdt.disVisible();
		break;				
	case CTRL_OBJ_TEXT:
		m_ctrlCaptionEdt.enable();
		m_ctrlCaptionEdt.setString(((XText *)m_ctrls[order].pCtrl)->getCaptionString());
		m_withCallbackChk.setState(m_ctrls[order].withCallback);
		m_withCallbackChk.disable();
		m_cmbAllMenuSumTxt.disVisible();
		m_cmbAllMenuSumEdt.disVisible();
		m_cmbShowMenuSumTxt.disVisible();
		m_cmbShowMenuSumEdt.disVisible();
		break;				
	case CTRL_OBJ_TOOLBAR:break;			
	case CTRL_OBJ_FUNCTION:break;
	}
}
int GGame::getCtrlIndexByID(int id)const
{
	if(id < 0) return -1;
	for(int i = 0;i < m_ctrls.size();++ i)
	{
		if(m_ctrls[i].ctrlID == id) return i;
	}
	return -1;
}
int GGame::getCtrlIndexByName(const std::string &name)const
{
	for(int i = 0;i < m_ctrls.size();++ i)
	{
		if(m_ctrls[i].ctrlName == name) return i;
	}
	return -1;
}
XBool GGame::saveToFile()
{
	TiXmlDocument doc(CFG_FILENAME);
	TiXmlDeclaration declaration("1.0","gb2312","yes");
	doc.InsertEndChild(declaration);
	TiXmlElement elmRoot("root");
	if(!XXml::addLeafNode(elmRoot,"winWidth",XString::toString(m_winWidth))) return XFalse;
	if(!XXml::addLeafNode(elmRoot,"winHeight",XString::toString(m_winHeight))) return XFalse;
	if(!XXml::addLeafNode(elmRoot,"projectName",m_prjNameEdt.getString())) return XFalse;
	//下面保存控件信息
	for(int i = 0;i < m_ctrls.size();++ i)
	{
		TiXmlElement elmNode("ctrlsInfo");  
		if(!XXml::addLeafNode(elmNode,"ctrlID",XString::toString(m_ctrls[i].ctrlID))) return XFalse;
		if(!XXml::addLeafNode(elmNode,"ctrlName",m_ctrls[i].ctrlName)) return XFalse;
		if(!XXml::addLeafNode(elmNode,"affiliation",m_ctrls[i].affiliationStr)) return XFalse;
		if(!XXml::addLeafNode(elmNode,"ctrlComment",m_ctrls[i].ctrlComment)) return XFalse;
		if(!XXml::addLeafNode(elmNode,"codeExplanation",m_ctrls[i].codeExplanation)) return XFalse;
		if(!XXml::addLeafNode(elmNode,"ctrlType",XCtrl::getCtrlTypeByString(m_ctrls[i].pCtrl->getCtrlType()))) return XFalse;
		if(!XXml::addLeafNode(elmNode,"withCallback",(bool)m_ctrls[i].withCallback)) return XFalse;
		//下面是控件的相关参数
		if(!XXml::addLeafNode(elmNode,"ctrlPosX",XString::toString(m_ctrls[i].pCtrl->getPosition().x))) return XFalse;
		if(!XXml::addLeafNode(elmNode,"ctrlPosY",XString::toString(m_ctrls[i].pCtrl->getPosition().y))) return XFalse;
		if(!XXml::addLeafNode(elmNode,"ctrlPixelX",XString::toString(m_ctrls[i].pCtrl->getPixelSize().x))) return XFalse;
		if(!XXml::addLeafNode(elmNode,"ctrlPixelY",XString::toString(m_ctrls[i].pCtrl->getPixelSize().y))) return XFalse;
		if(!XXml::addLeafNode(elmNode,"ctrlScaleX",XString::toString(m_ctrls[i].pCtrl->getScale().x))) return XFalse;
		if(!XXml::addLeafNode(elmNode,"ctrlScaleY",XString::toString(m_ctrls[i].pCtrl->getScale().y))) return XFalse;
		//下面是根据控件的类型写入特殊的参数
		switch(m_ctrls[i].pCtrl->getCtrlType())
		{
		case CTRL_OBJ_NULL:break;
		case CTRL_OBJ_BUTTON:
			if(!XXml::addLeafNode(elmNode,"ctrlCaption",((XButton *)m_ctrls[i].pCtrl)->getCaptionString())) return XFalse;
			break;		
		case CTRL_OBJ_BTNBAR:break;			
		case CTRL_OBJ_BUTTONEX:break;		
		case CTRL_OBJ_CALENDAR:break;			
		case CTRL_OBJ_CHART:break;			
		case CTRL_OBJ_CHECK:
			if(!XXml::addLeafNode(elmNode,"ctrlCaption",((XCheck *)m_ctrls[i].pCtrl)->getCaptionString())) return XFalse;
			break;			
		case CTRL_OBJ_COMBO:
			if(!XXml::addLeafNode(elmNode,"ctrlCaption",((XCombo *)m_ctrls[i].pCtrl)->getMenuStr())) return XFalse;
			if(!XXml::addLeafNode(elmNode,"allMenuSum",XString::toString(m_ctrls[i].allMenuSum))) return XFalse;
			if(!XXml::addLeafNode(elmNode,"showMenuSum",XString::toString(m_ctrls[i].showMenuSum))) return XFalse;
			break;			
		case CTRL_OBJ_DIRECTORYLIST:break;		
		case CTRL_OBJ_EDIT:
			if(!XXml::addLeafNode(elmNode,"ctrlCaption",((XEdit *)m_ctrls[i].pCtrl)->getString())) return XFalse;
			break;			
		case CTRL_OBJ_IMAGE:break;			
		case CTRL_OBJ_GROUP:break;		
		case CTRL_OBJ_IMAGELIST:break;			
		case CTRL_OBJ_LINECTRL:break;		
		case CTRL_OBJ_MENU:break;			
		case CTRL_OBJ_MOUSERIGHTBUTTONMENU:break;		
		case CTRL_OBJ_MUTILIST:
			if(!XXml::addLeafNode(elmNode,"ctrlCaption",((XMultiList *)m_ctrls[i].pCtrl)->getTitleStr())) return XFalse;
			if(!XXml::addLeafNode(elmNode,"allMenuSum",XString::toString(m_ctrls[i].allMenuSum))) return XFalse;
			if(!XXml::addLeafNode(elmNode,"showMenuSum",XString::toString(m_ctrls[i].showMenuSum))) return XFalse;
			break;		
		case CTRL_OBJ_MUTITEXT:
			if(!XXml::addLeafNode(elmNode,"ctrlCaption",((XMultiText *)m_ctrls[i].pCtrl)->getALineString(0))) return XFalse;
			break;		
		case CTRL_OBJ_PASSWORDPAD:break;		
		case CTRL_OBJ_POINTCTRL:break;			
		case CTRL_OBJ_PROGRESS:break;		
		case CTRL_OBJ_PROGRESSRING:break;		
		case CTRL_OBJ_RADIOS:break;		
		case CTRL_OBJ_SIMPLELINE:break;	//无参数	
		case CTRL_OBJ_SLIDER:break;		
		case CTRL_OBJ_SLIDEREX:break;		
		case CTRL_OBJ_TAB:
			if(!XXml::addLeafNode(elmNode,"ctrlCaption",((XTab *)m_ctrls[i].pCtrl)->getTabsStr())) return XFalse;
			if(!XXml::addLeafNode(elmNode,"allMenuSum",XString::toString(m_ctrls[i].allMenuSum))) return XFalse;
			break;				
		case CTRL_OBJ_TEXT:
			if(!XXml::addLeafNode(elmNode,"ctrlCaption",((XText *)m_ctrls[i].pCtrl)->getCaptionString())) return XFalse;
			break;				
		case CTRL_OBJ_TOOLBAR:break;			
		case CTRL_OBJ_FUNCTION:break;
		}
		
		if(elmRoot.InsertEndChild(elmNode) == NULL) return XFalse;  
	}

	if(doc.InsertEndChild(elmRoot) == NULL) return XFalse;
	if(!doc.SaveFile()) return XFalse;
	return XTrue;
}
XBool GGame::loadFromFile()
{
	TiXmlDocument doc(CFG_FILENAME);
	if(!doc.LoadFile()) return XFalse;
	TiXmlNode *rootNode = doc.FirstChild("root");
	if(rootNode == NULL) return XFalse;
	TiXmlNode *keyNode = NULL;

	std::string tmpString;
	bool tmpB;
	if(XXml::getXmlAsInt(rootNode,"winWidth",m_winWidth)) 
		m_winSizeXEdt.setString(XString::toString(m_winWidth).c_str());
	if(XXml::getXmlAsInt(rootNode,"winHeight",m_winHeight)) 
		m_winSizeYEdt.setString(XString::toString(m_winHeight).c_str());
	if(XXml::getXmlAsString(rootNode,"projectName",tmpString)) 
		m_prjNameEdt.setString(tmpString.c_str());
	//下面依次读取所有控件的信息并建立控件
	release();
	keyNode = rootNode->FirstChildElement("ctrlsInfo");
	while(1)
	{
		if(keyNode == NULL) break;
		GCtrlInfo tmp;
		XXml::getXmlAsInt(keyNode,"ctrlID",tmp.ctrlID);
		if(tmp.ctrlID >= m_ctrlID) m_ctrlID = tmp.ctrlID + 1;
		XXml::getXmlAsString(keyNode,"ctrlName",tmp.ctrlName);
		XXml::getXmlAsString(keyNode,"affiliation",tmp.affiliationStr);
		XXml::getXmlAsString(keyNode,"ctrlComment",tmp.ctrlComment);
		XXml::getXmlAsString(keyNode,"codeExplanation",tmp.codeExplanation);
		XXml::getXmlAsString(keyNode,"ctrlType",tmpString);
		XCtrlObjType type = XCtrl::getCtrlTypeByString(tmpString);
		XXml::getXmlAsBool(keyNode,"withCallback",tmpB);
		tmp.withCallback = tmpB;
		//下面读取属性建立控件
		XVector2 pos;
		XVector2 pixel;
		XVector2 scale;
		XXml::getXmlAsFloat(keyNode,"ctrlPosX",pos.x);
		XXml::getXmlAsFloat(keyNode,"ctrlPosY",pos.y);
		XXml::getXmlAsFloat(keyNode,"ctrlPixelX",pixel.x);
		XXml::getXmlAsFloat(keyNode,"ctrlPixelY",pixel.y);
		XXml::getXmlAsFloat(keyNode,"ctrlScaleX",scale.x);
		XXml::getXmlAsFloat(keyNode,"ctrlScaleX",scale.y);
		switch(type)
		{
		case CTRL_OBJ_NULL:break;
		case CTRL_OBJ_BUTTON:
			XXml::getXmlAsString(keyNode,"ctrlCaption",tmpString);
			tmp.pCtrl = XMem::createMem<XButton>();
			((XButton *)tmp.pCtrl)->initWithoutSkin(tmpString.c_str(),pixel);
			((XButton *)tmp.pCtrl)->setPosition(pos);
			((XButton *)tmp.pCtrl)->setScale(scale);
			break;		
		case CTRL_OBJ_BTNBAR:break;			
		case CTRL_OBJ_BUTTONEX:break;		
		case CTRL_OBJ_CALENDAR:break;			
		case CTRL_OBJ_CHART:break;			
		case CTRL_OBJ_CHECK:
			XXml::getXmlAsString(keyNode,"ctrlCaption",tmpString);
			tmp.pCtrl = XMem::createMem<XCheck>();
			((XCheck *)tmp.pCtrl)->initWithoutSkin(tmpString.c_str());
			((XCheck *)tmp.pCtrl)->setPosition(pos);
			((XCheck *)tmp.pCtrl)->setScale(scale);
			break;			
		case CTRL_OBJ_COMBO:
			XXml::getXmlAsString(keyNode,"ctrlCaption",tmpString);
			XXml::getXmlAsInt(keyNode,"allMenuSum",tmp.allMenuSum);
			XXml::getXmlAsInt(keyNode,"showMenuSum",tmp.showMenuSum);
			tmp.pCtrl = XMem::createMem<XCombo>();
			((XCombo *)tmp.pCtrl)->initWithoutSkin(pixel.x,tmp.allMenuSum,tmp.showMenuSum);
			((XCombo *)tmp.pCtrl)->setPosition(pos);
			((XCombo *)tmp.pCtrl)->setScale(scale);
			((XCombo *)tmp.pCtrl)->setMenuStr(tmpString.c_str());
			break;			
		case CTRL_OBJ_DIRECTORYLIST:break;		
		case CTRL_OBJ_EDIT:
			XXml::getXmlAsString(keyNode,"ctrlCaption",tmpString);
			tmp.pCtrl = XMem::createMem<XEdit>();
			((XEdit *)tmp.pCtrl)->initWithoutSkin(pixel,tmpString.c_str());
			((XEdit *)tmp.pCtrl)->setPosition(pos);
			((XEdit *)tmp.pCtrl)->setScale(scale);
			break;			
		case CTRL_OBJ_IMAGE:break;			
		case CTRL_OBJ_GROUP:break;		
		case CTRL_OBJ_IMAGELIST:break;			
		case CTRL_OBJ_LINECTRL:break;		
		case CTRL_OBJ_MENU:break;			
		case CTRL_OBJ_MOUSERIGHTBUTTONMENU:break;		
		case CTRL_OBJ_MUTILIST:
			XXml::getXmlAsString(keyNode,"ctrlCaption",tmpString);
			XXml::getXmlAsInt(keyNode,"allMenuSum",tmp.allMenuSum);
			XXml::getXmlAsInt(keyNode,"showMenuSum",tmp.showMenuSum);
			tmp.pCtrl = XMem::createMem<XMultiList>();
			((XMultiList *)tmp.pCtrl)->initWithoutSkin(pixel,tmp.allMenuSum,tmp.showMenuSum);
			((XMultiList *)tmp.pCtrl)->setPosition(pos);
			((XMultiList *)tmp.pCtrl)->setScale(scale);
			((XMultiList *)tmp.pCtrl)->setTitleStr(tmpString.c_str());
			break;		
		case CTRL_OBJ_MUTITEXT:
			XXml::getXmlAsString(keyNode,"ctrlCaption",tmpString);
			tmp.pCtrl = XMem::createMem<XMultiText>();
			((XMultiText *)tmp.pCtrl)->initWithoutSkin(pixel,tmpString.c_str());
			((XMultiText *)tmp.pCtrl)->setPosition(pos);
			((XMultiText *)tmp.pCtrl)->setScale(scale);
			break;		
		case CTRL_OBJ_PASSWORDPAD:break;		
		case CTRL_OBJ_POINTCTRL:break;			
		case CTRL_OBJ_PROGRESS:break;		
		case CTRL_OBJ_PROGRESSRING:break;		
		case CTRL_OBJ_RADIOS:break;		
		case CTRL_OBJ_SIMPLELINE:
			tmp.pCtrl = XMem::createMem<XSimpleLine>();
			((XSimpleLine *)tmp.pCtrl)->init(pixel.x);
			((XSimpleLine *)tmp.pCtrl)->setPosition(pos);
			((XSimpleLine *)tmp.pCtrl)->setScale(scale);
			break;		
		case CTRL_OBJ_SLIDER:break;		
		case CTRL_OBJ_SLIDEREX:break;		
		case CTRL_OBJ_TAB:
			XXml::getXmlAsString(keyNode,"ctrlCaption",tmpString);
			XXml::getXmlAsInt(keyNode,"allMenuSum",tmp.allMenuSum);
			XXml::getXmlAsInt(keyNode,"showMenuSum",tmp.showMenuSum);
			tmp.pCtrl = XMem::createMem<XTab>();
			((XTab *)tmp.pCtrl)->initWithoutSkin(pixel);
			((XTab *)tmp.pCtrl)->setPosition(pos);
			((XTab *)tmp.pCtrl)->setScale(scale);
			for(int i = 0;i < tmp.allMenuSum;++ i)
			{
				((XTab *)tmp.pCtrl)->addATab("tab" + XString::toString(i));
			}
			((XTab *)tmp.pCtrl)->setTabsStr(tmpString.c_str());
			break;				
		case CTRL_OBJ_TEXT:
			XXml::getXmlAsString(keyNode,"ctrlCaption",tmpString);
			tmp.pCtrl = XMem::createMem<XText>();
			((XText *)tmp.pCtrl)->init(tmpString.c_str());
			((XText *)tmp.pCtrl)->setPosition(pos);
			((XText *)tmp.pCtrl)->setScale(scale);
			break;				
		case CTRL_OBJ_TOOLBAR:break;			
		case CTRL_OBJ_FUNCTION:break;
		}
			
		m_ctrls.push_back(tmp);
		keyNode = keyNode->NextSiblingElement("ctrlsInfo");
	}
	updateAffiliation();
	return XTrue;
}
XBool GGame::createCode()
{
	//这里根据具体的设置生成代码文件，便于使用
	//写入h文件
	std::string filename = std::string(m_prjNameEdt.getString()) + ".h";
	FILE * fp = NULL;
	if((fp = fopen(filename.c_str(),"w")) == NULL) return XFalse;
	//下面写入公共部分
	fprintf(fp,"#pragma once\n");
	fprintf(fp,"#include \"XEffeEngine.h\"\n\n");
	fprintf(fp,"class %s\n{\n",m_prjNameEdt.getString());
	fprintf(fp,"public:\n");
	fprintf(fp,"	%s(){}\n",m_prjNameEdt.getString());
	fprintf(fp,"	~%s(){}\n",m_prjNameEdt.getString());
	fprintf(fp,"	bool createUI();\n",m_prjNameEdt.getString());
	fprintf(fp,"	bool disableAll();\n",m_prjNameEdt.getString());
	fprintf(fp,"	bool enableAll();\n",m_prjNameEdt.getString());
	fprintf(fp,"	bool visibleAll();\n",m_prjNameEdt.getString());
	fprintf(fp,"	bool hideAll();\n",m_prjNameEdt.getString());
	//下面写入控件的定义
	for(int i =0;i < m_ctrls.size();++ i)
	{
		filename = "	XE::" + XCtrl::getCtrlNameByType(m_ctrls[i].pCtrl->getCtrlType()) + " ";
		//下面需要判断是否要写入注释
		if(m_ctrls[i].codeExplanation == "NULL" || m_ctrls[i].codeExplanation == "null")
			filename += m_ctrls[i].ctrlName + ";\n";
		else
			filename += m_ctrls[i].ctrlName + ";//" + m_ctrls[i].codeExplanation + "\n";
		fprintf(fp,filename.c_str());
	}
	fprintf(fp,"};\n");
	fclose(fp);

	//写入cpp文件
	filename = std::string(m_prjNameEdt.getString()) + ".cpp";
	if((fp = fopen(filename.c_str(),"w")) == NULL) return XFalse;
	filename = m_prjNameEdt.getString();
	filename += ".h";
	fprintf(fp,("#include \"" + filename + "\"\n\n").c_str());
	fprintf(fp,"using namespace XE;\n");
	fprintf(fp,"bool %s::createUI()\n{\n",m_prjNameEdt.getString());
	//这里写入控件的初始化
	for(int i = 0;i < m_ctrls.size();++ i)
	{
		switch(m_ctrls[i].pCtrl->getCtrlType())
		{
		case CTRL_OBJ_NULL:break;
		case CTRL_OBJ_BUTTON:
			filename = "	" + m_ctrls[i].ctrlName + ".initWithoutSkin(\"" + 
				((XButton *)m_ctrls[i].pCtrl)->getCaptionString() + "\",XVector2(" +
				XString::toString(((XButton *)m_ctrls[i].pCtrl)->getPixelSize().x) + "," +
				XString::toString(((XButton *)m_ctrls[i].pCtrl)->getPixelSize().y) + "));\n";
			fprintf(fp,filename.c_str());
			filename = "	" + m_ctrls[i].ctrlName + ".setPosition(" +
				XString::toString(((XButton *)m_ctrls[i].pCtrl)->getPosition().x) + "," +
				XString::toString(((XButton *)m_ctrls[i].pCtrl)->getPosition().y) + ");\n";
			fprintf(fp,filename.c_str());
			filename = "	" + m_ctrls[i].ctrlName + ".setScale(" +
				XString::toString(((XButton *)m_ctrls[i].pCtrl)->getScale().x) + "," +
				XString::toString(((XButton *)m_ctrls[i].pCtrl)->getScale().y) + ");\n";
			fprintf(fp,filename.c_str());
		//	if(m_ctrls[i].withCallback)
		//	{//下面要注册回调函数
		//		filename = "	" + m_ctrls[i].ctrlName + ".setMouseDownCB(ctrlFun,this);\n";
		//		fprintf(fp,filename.c_str());
		//	}
			break;		
		case CTRL_OBJ_BTNBAR:break;			
		case CTRL_OBJ_BUTTONEX:break;		
		case CTRL_OBJ_CALENDAR:break;			
		case CTRL_OBJ_CHART:break;			
		case CTRL_OBJ_CHECK:
			filename = "	" + m_ctrls[i].ctrlName + ".initWithoutSkin(\"" + 
				((XCheck *)m_ctrls[i].pCtrl)->getCaptionString() + "\");\n";
			fprintf(fp,filename.c_str());
			filename = "	" + m_ctrls[i].ctrlName + ".setPosition(" +
				XString::toString(((XCheck *)m_ctrls[i].pCtrl)->getPosition().x) + "," +
				XString::toString(((XCheck *)m_ctrls[i].pCtrl)->getPosition().y) + ");\n";
			fprintf(fp,filename.c_str());
			filename = "	" + m_ctrls[i].ctrlName + ".setScale(" +
				XString::toString(((XCheck *)m_ctrls[i].pCtrl)->getScale().x) + "," +
				XString::toString(((XCheck *)m_ctrls[i].pCtrl)->getScale().y) + ");\n";
			fprintf(fp,filename.c_str());
			if(m_ctrls[i].withCallback)
			{//下面要注册回调函数
				filename = "	" + m_ctrls[i].ctrlName + ".setStateChangeCB(ctrlFun,this);\n";
				fprintf(fp,filename.c_str());
			}
			break;			
		case CTRL_OBJ_COMBO:
			filename = "	" + m_ctrls[i].ctrlName + ".initWithoutSkin(" + 
				XString::toString(((XCombo *)m_ctrls[i].pCtrl)->getPixelSize().x) + "," + 
				XString::toString(m_ctrls[i].allMenuSum) + "," + XString::toString(m_ctrls[i].showMenuSum) + ");\n";
			fprintf(fp,filename.c_str());
			filename = "	" + m_ctrls[i].ctrlName + ".setPosition(" +
				XString::toString(((XCombo *)m_ctrls[i].pCtrl)->getPosition().x) + "," +
				XString::toString(((XCombo *)m_ctrls[i].pCtrl)->getPosition().y) + ");\n";
			fprintf(fp,filename.c_str());
			filename = "	" + m_ctrls[i].ctrlName + ".setScale(" +
				XString::toString(((XCombo *)m_ctrls[i].pCtrl)->getScale().x) + "," +
				XString::toString(((XCombo *)m_ctrls[i].pCtrl)->getScale().y) + ");\n";
			fprintf(fp,filename.c_str());
			filename = "	" + m_ctrls[i].ctrlName + ".setMenuStr(\"" +
				((XCombo *)m_ctrls[i].pCtrl)->getMenuStr() + "\");\n";
			fprintf(fp,filename.c_str());
			if(m_ctrls[i].withCallback)
			{//下面要注册回调函数
				filename = "	" + m_ctrls[i].ctrlName + ".setCallbackFun(ctrlFun,this);\n";
				fprintf(fp,filename.c_str());
			}
			break;			
		case CTRL_OBJ_DIRECTORYLIST:break;		
		case CTRL_OBJ_EDIT:
			filename = "	" + m_ctrls[i].ctrlName + ".initWithoutSkin(" + "XVector2(" +
				XString::toString(((XEdit *)m_ctrls[i].pCtrl)->getPixelSize().x) + "," +
				XString::toString(((XEdit *)m_ctrls[i].pCtrl)->getPixelSize().y) + "),\"" + 
				((XEdit *)m_ctrls[i].pCtrl)->getString() + "\");\n";
			fprintf(fp,filename.c_str());
			filename = "	" + m_ctrls[i].ctrlName + ".setPosition(" +
				XString::toString(((XEdit *)m_ctrls[i].pCtrl)->getPosition().x) + "," +
				XString::toString(((XEdit *)m_ctrls[i].pCtrl)->getPosition().y) + ");\n";
			fprintf(fp,filename.c_str());
			filename = "	" + m_ctrls[i].ctrlName + ".setScale(" +
				XString::toString(((XEdit *)m_ctrls[i].pCtrl)->getScale().x) + "," +
				XString::toString(((XEdit *)m_ctrls[i].pCtrl)->getScale().y) + ");\n";
			fprintf(fp,filename.c_str());
			if(m_ctrls[i].withCallback)
			{//下面要注册回调函数
				filename = "	" + m_ctrls[i].ctrlName + ".setCallbackFun(NULL,ctrlFun,this);\n";
				fprintf(fp,filename.c_str());
			}
			break;			
		case CTRL_OBJ_IMAGE:break;			
		case CTRL_OBJ_GROUP:break;		
		case CTRL_OBJ_IMAGELIST:break;			
		case CTRL_OBJ_LINECTRL:break;		
		case CTRL_OBJ_MENU:break;			
		case CTRL_OBJ_MOUSERIGHTBUTTONMENU:break;		
		case CTRL_OBJ_MUTILIST:
			filename = "	" + m_ctrls[i].ctrlName + ".initWithoutSkin(XVector2(" +
				XString::toString(((XMultiList *)m_ctrls[i].pCtrl)->getPixelSize().x) + "," +
				XString::toString(((XMultiList *)m_ctrls[i].pCtrl)->getPixelSize().y) + ")," + 
				XString::toString(m_ctrls[i].allMenuSum) + "," + XString::toString(m_ctrls[i].showMenuSum) + ");\n";
			fprintf(fp,filename.c_str());
			filename = "	" + m_ctrls[i].ctrlName + ".setPosition(" +
				XString::toString(((XMultiList *)m_ctrls[i].pCtrl)->getPosition().x) + "," +
				XString::toString(((XMultiList *)m_ctrls[i].pCtrl)->getPosition().y) + ");\n";
			fprintf(fp,filename.c_str());
			filename = "	" + m_ctrls[i].ctrlName + ".setScale(" +
				XString::toString(((XMultiList *)m_ctrls[i].pCtrl)->getScale().x) + "," +
				XString::toString(((XMultiList *)m_ctrls[i].pCtrl)->getScale().y) + ");\n";
			fprintf(fp,filename.c_str());
			filename = "	" + m_ctrls[i].ctrlName + ".setTitleStr(\"" +
				((XMultiList *)m_ctrls[i].pCtrl)->getTitleStr() + "\");\n";
			fprintf(fp,filename.c_str());
			if(m_ctrls[i].withCallback)
			{//下面要注册回调函数
				filename = "	" + m_ctrls[i].ctrlName + ".m_funDClick(ctrlFun,this);\n";
				fprintf(fp,filename.c_str());
			}
			break;		
		case CTRL_OBJ_MUTITEXT:
			filename = "	" + m_ctrls[i].ctrlName + ".initWithoutSkin(XVector2(" +
				XString::toString(((XMultiText *)m_ctrls[i].pCtrl)->getPixelSize().x) + "," +
				XString::toString(((XMultiText *)m_ctrls[i].pCtrl)->getPixelSize().y) + "),\"" + 
				((XMultiText *)m_ctrls[i].pCtrl)->getALineString(0) + "\");\n";
			fprintf(fp,filename.c_str());
			filename = "	" + m_ctrls[i].ctrlName + ".setPosition(" +
				XString::toString(((XMultiText *)m_ctrls[i].pCtrl)->getPosition().x) + "," +
				XString::toString(((XMultiText *)m_ctrls[i].pCtrl)->getPosition().y) + ");\n";
			fprintf(fp,filename.c_str());
			filename = "	" + m_ctrls[i].ctrlName + ".setScale(" +
				XString::toString(((XMultiText *)m_ctrls[i].pCtrl)->getScale().x) + "," +
				XString::toString(((XMultiText *)m_ctrls[i].pCtrl)->getScale().y) + ");\n";
			fprintf(fp,filename.c_str());
			break;		
		case CTRL_OBJ_PASSWORDPAD:break;		
		case CTRL_OBJ_POINTCTRL:break;			
		case CTRL_OBJ_PROGRESS:break;		
		case CTRL_OBJ_PROGRESSRING:break;		
		case CTRL_OBJ_RADIOS:break;		
		case CTRL_OBJ_SIMPLELINE:
			filename = "	" + m_ctrls[i].ctrlName + ".init(" + 
				XString::toString(((XCombo *)m_ctrls[i].pCtrl)->getPixelSize().x) + ");\n";
			fprintf(fp,filename.c_str());
			filename = "	" + m_ctrls[i].ctrlName + ".setPosition(" +
				XString::toString(((XCombo *)m_ctrls[i].pCtrl)->getPosition().x) + "," +
				XString::toString(((XCombo *)m_ctrls[i].pCtrl)->getPosition().y) + ");\n";
			fprintf(fp,filename.c_str());
			filename = "	" + m_ctrls[i].ctrlName + ".setScale(" +
				XString::toString(((XCombo *)m_ctrls[i].pCtrl)->getScale().x) + "," +
				XString::toString(((XCombo *)m_ctrls[i].pCtrl)->getScale().y) + ");\n";
			fprintf(fp,filename.c_str());
			break;		
		case CTRL_OBJ_SLIDER:break;		
		case CTRL_OBJ_SLIDEREX:break;		
		case CTRL_OBJ_TAB:
			filename = "	" + m_ctrls[i].ctrlName + ".initWithoutSkin(XVector2(" +
				XString::toString(((XTab *)m_ctrls[i].pCtrl)->getPixelSize().x) + "," +
				XString::toString(((XTab *)m_ctrls[i].pCtrl)->getPixelSize().y) +  "));\n";
			fprintf(fp,filename.c_str());
			filename = "	" + m_ctrls[i].ctrlName + ".setPosition(" +
				XString::toString(((XTab *)m_ctrls[i].pCtrl)->getPosition().x) + "," +
				XString::toString(((XTab *)m_ctrls[i].pCtrl)->getPosition().y) + ");\n";
			fprintf(fp,filename.c_str());
			filename = "	" + m_ctrls[i].ctrlName + ".setScale(" +
				XString::toString(((XTab *)m_ctrls[i].pCtrl)->getScale().x) + "," +
				XString::toString(((XTab *)m_ctrls[i].pCtrl)->getScale().y) + ");\n";
			fprintf(fp,filename.c_str());
			//建立分页
			fprintf(fp,("	for(int i = 0;i < " + XString::toString(m_ctrls[i].allMenuSum) + ";++ i)\n	{\n").c_str());
			fprintf(fp,("		" + m_ctrls[i].ctrlName + ".addATab(\"tab\" + XString::toString(i));\n	}\n").c_str());
			filename = "	" + m_ctrls[i].ctrlName + ".setTabsStr(\"" +
				((XTab *)m_ctrls[i].pCtrl)->getTabsStr() + "\");\n";
			fprintf(fp,filename.c_str());
			if(m_ctrls[i].withCallback)
			{//下面要注册回调函数
				filename = "	" + m_ctrls[i].ctrlName + ".setChooseTabCB(ctrlFun,this);\n";
				fprintf(fp,filename.c_str());
			}
			break;				
		case CTRL_OBJ_TEXT:
			filename = "	" + m_ctrls[i].ctrlName + ".init(\"" + 
				((XText *)m_ctrls[i].pCtrl)->getCaptionString() + "\");\n";
			fprintf(fp,filename.c_str());
			filename = "	" + m_ctrls[i].ctrlName + ".setPosition(" +
				XString::toString(((XText *)m_ctrls[i].pCtrl)->getPosition().x) + "," +
				XString::toString(((XText *)m_ctrls[i].pCtrl)->getPosition().y) + ");\n";
			fprintf(fp,filename.c_str());
			filename = "	" + m_ctrls[i].ctrlName + ".setScale(" +
				XString::toString(((XText *)m_ctrls[i].pCtrl)->getScale().x) + "," +
				XString::toString(((XText *)m_ctrls[i].pCtrl)->getScale().y) + ");\n";
			fprintf(fp,filename.c_str());
			break;				
		case CTRL_OBJ_TOOLBAR:break;			
		case CTRL_OBJ_FUNCTION:break;
		}
		if(m_ctrls[i].ctrlComment == "NULL" || m_ctrls[i].ctrlComment == "null") continue;
		//下面写入说明
		filename = "	" + m_ctrls[i].ctrlName + ".setComment(\"" +
			m_ctrls[i].ctrlComment + "\");\n";
		fprintf(fp,filename.c_str());
	}
	//下面要写入从属关系(尚未完成)
	for(int i = 0;i < m_ctrls.size();++ i)
	{
		if(m_ctrls[i].affiliationStr == "NULL" ||
			m_ctrls[i].affiliationStr == "null") continue;
		int start = -1;
		int end = 0;
		int index = 0;
		std::string ctrlName;
		std::string tabName;
		int ctrlIndex;
		char * tmpStr = XMem::createArrayMem<char>(m_ctrls[i].affiliationStr.length() + 1);
		strcpy(tmpStr,m_ctrls[i].affiliationStr.c_str());
		while(true)
		{
			end = XString::getCharPosition(tmpStr + start + 1,':');
			if(end < 0) break;	//非法的数据
			tmpStr[start + 1 + end] = '\0';
			ctrlName = tmpStr + start + 1;
			start += end + 1;
			end = XString::getCharPosition(tmpStr + start + 1,';');
			if(end < 0) break;	//非法的数据
			tmpStr[start + 1 + end] = '\0';
			tabName = tmpStr + start + 1;
			start += end + 1;
			//添加从属关系
			ctrlIndex = getCtrlIndexByName(ctrlName);
			if(ctrlIndex < 0) continue;	//目标控件不存在
			if(m_ctrls[ctrlIndex].pCtrl->getCtrlType() != CTRL_OBJ_TAB) continue;	//目前只支持tab
			//((XTab *)m_ctrls[ctrlIndex].pCtrl)->addObjToTab(m_ctrls[i].pCtrl,tabName);
			fprintf(fp,("	" + m_ctrls[ctrlIndex].ctrlName + 
				".addObjToTab(&" + m_ctrls[i].ctrlName + ",\"" + tabName + "\");\n").c_str());
		}
		XMem::XDELETE_ARRAY(tmpStr);
	}
	fprintf(fp,"	return true;\n}\n");
	//下面建立全体的使能和显隐函数
	fprintf(fp,"bool %s::disableAll()\n{\n",m_prjNameEdt.getString());
	//这里写入控件的初始化
	for(int i = 0;i < m_ctrls.size();++ i)
	{
		switch(m_ctrls[i].pCtrl->getCtrlType())
		{
		case CTRL_OBJ_NULL:break;
		case CTRL_OBJ_TOOLBAR:break;			
		case CTRL_OBJ_FUNCTION:break;
		case CTRL_OBJ_TEXT:break;
		case CTRL_OBJ_SIMPLELINE:break;
		default:
			filename = "	" + m_ctrls[i].ctrlName + ".disable();\n";
			fprintf(fp,filename.c_str());
			break;
		}
	}
	fprintf(fp,"	return true;\n}\n");
	fprintf(fp,"bool %s::enableAll()\n{\n",m_prjNameEdt.getString());
	//这里写入控件的初始化
	for(int i = 0;i < m_ctrls.size();++ i)
	{
		switch(m_ctrls[i].pCtrl->getCtrlType())
		{
		case CTRL_OBJ_NULL:break;
		case CTRL_OBJ_TOOLBAR:break;			
		case CTRL_OBJ_FUNCTION:break;
		case CTRL_OBJ_TEXT:break;
		case CTRL_OBJ_SIMPLELINE:break;
		default:
			filename = "	" + m_ctrls[i].ctrlName + ".enable();\n";
			fprintf(fp,filename.c_str());
			break;
		}
	}
	fprintf(fp,"	return true;\n}\n");
	fprintf(fp,"bool %s::visibleAll()\n{\n",m_prjNameEdt.getString());
	//这里写入控件的初始化
	for(int i = 0;i < m_ctrls.size();++ i)
	{
		switch(m_ctrls[i].pCtrl->getCtrlType())
		{
		case CTRL_OBJ_NULL:break;
		case CTRL_OBJ_TOOLBAR:break;			
		case CTRL_OBJ_FUNCTION:break;
		default:
			filename = "	" + m_ctrls[i].ctrlName + ".setVisible();\n";
			fprintf(fp,filename.c_str());
			break;
		}
	}
	fprintf(fp,"	return true;\n}\n");
	fprintf(fp,"bool %s::hideAll()\n{\n",m_prjNameEdt.getString());
	//这里写入控件的初始化
	for(int i = 0;i < m_ctrls.size();++ i)
	{
		switch(m_ctrls[i].pCtrl->getCtrlType())
		{
		case CTRL_OBJ_NULL:break;
		case CTRL_OBJ_TOOLBAR:break;			
		case CTRL_OBJ_FUNCTION:break;
		default:
			filename = "	" + m_ctrls[i].ctrlName + ".disVisible();\n";
			fprintf(fp,filename.c_str());
			break;
		}
	}
	fprintf(fp,"	return true;\n}\n");
	fclose(fp);
	return XTrue;
}
void GGame::clearAffiliation()
{
	//清空所有从属关系
	for(int i = 0;i < m_ctrls.size();++ i)
	{
		switch(m_ctrls[i].pCtrl->getCtrlType())
		{
		case CTRL_OBJ_NULL:break;
		case CTRL_OBJ_BUTTON:break;		
		case CTRL_OBJ_BTNBAR:break;			
		case CTRL_OBJ_BUTTONEX:break;		
		case CTRL_OBJ_CALENDAR:break;			
		case CTRL_OBJ_CHART:break;			
		case CTRL_OBJ_CHECK:break;			
		case CTRL_OBJ_COMBO:break;			
		case CTRL_OBJ_DIRECTORYLIST:break;		
		case CTRL_OBJ_EDIT:break;			
		case CTRL_OBJ_IMAGE:break;			
		case CTRL_OBJ_GROUP:break;		
		case CTRL_OBJ_IMAGELIST:break;			
		case CTRL_OBJ_LINECTRL:break;		
		case CTRL_OBJ_MENU:break;			
		case CTRL_OBJ_MOUSERIGHTBUTTONMENU:break;		
		case CTRL_OBJ_MUTILIST:break;	
		case CTRL_OBJ_MUTITEXT:break;		//不支持回调函数	
		case CTRL_OBJ_PASSWORDPAD:break;		
		case CTRL_OBJ_POINTCTRL:break;			
		case CTRL_OBJ_PROGRESS:break;		
		case CTRL_OBJ_PROGRESSRING:break;		
		case CTRL_OBJ_RADIOS:break;		
		case CTRL_OBJ_SIMPLELINE:break;	//不支持回调函数		
		case CTRL_OBJ_SLIDER:break;		
		case CTRL_OBJ_SLIDEREX:break;		
		case CTRL_OBJ_TAB:
			((XTab *)m_ctrls[i].pCtrl)->clearAllObj();	//清除所有的从属关系
			break;				
		case CTRL_OBJ_TEXT:break;			//不支持回调函数	
		case CTRL_OBJ_TOOLBAR:break;			
		case CTRL_OBJ_FUNCTION:break;
		}
		m_ctrls[i].pCtrl->setVisible();	//全部显示出来
	}
}
void GGame::updateAffiliation()
{//这里需要更新从属关系
	clearAffiliation();	//清空原有的从属关系
	//建立新的从属关系
	for(int i = 0;i < m_ctrls.size();++ i)
	{
		if(m_ctrls[i].affiliationStr == "NULL" ||
			m_ctrls[i].affiliationStr == "null") continue;
		int start = -1;
		int end = 0;
		int index = 0;
		std::string ctrlName;
		std::string tabName;
		int ctrlIndex;
		char * tmpStr = XMem::createArrayMem<char>(m_ctrls[i].affiliationStr.length() + 1);
		strcpy(tmpStr,m_ctrls[i].affiliationStr.c_str());
		while(true)
		{
			end = XString::getCharPosition(tmpStr + start + 1,':');
			if(end < 0) break;	//非法的数据
			tmpStr[start + 1 + end] = '\0';
			ctrlName = tmpStr + start + 1;
			start += end + 1;
			end = XString::getCharPosition(tmpStr + start + 1,';');
			if(end < 0) break;	//非法的数据
			tmpStr[start + 1 + end] = '\0';
			tabName = tmpStr + start + 1;
			start += end + 1;
			//添加从属关系
			ctrlIndex = getCtrlIndexByName(ctrlName);
			if(ctrlIndex < 0) continue;	//目标控件不存在
			if(m_ctrls[ctrlIndex].pCtrl->getCtrlType() != CTRL_OBJ_TAB) continue;	//目前只支持tab
			((XTab *)m_ctrls[ctrlIndex].pCtrl)->addObjToTab(m_ctrls[i].pCtrl,tabName);
		}
		XMem::XDELETE_ARRAY(tmpStr);
	}
}