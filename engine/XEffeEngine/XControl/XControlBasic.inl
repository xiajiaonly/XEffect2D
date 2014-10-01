inline _XCtrlObjType getCtrlTypeByString(const std::string &name)
{
	if(name == "CTRL_OBJ_NULL") return CTRL_OBJ_NULL;
	if(name == "CTRL_OBJ_BUTTON") return CTRL_OBJ_BUTTON;	//按钮物件
	if(name == "CTRL_OBJ_BUTTONEX") return CTRL_OBJ_BUTTONEX;	//按钮物件
	if(name == "CTRL_OBJ_CHECK") return CTRL_OBJ_CHECK;		//复选框的物件
	if(name == "CTRL_OBJ_EDIT") return CTRL_OBJ_EDIT;		//输入框的物件
	if(name == "CTRL_OBJ_MOUSERIGHTBUTTONMENU") return CTRL_OBJ_MOUSERIGHTBUTTONMENU;	//鼠标右键菜单的物件
	if(name == "CTRL_OBJ_SLIDER") return CTRL_OBJ_SLIDER;	//滑动条的物件
	if(name == "CTRL_OBJ_RADIOS") return CTRL_OBJ_RADIOS;	//单选框的物件
	if(name == "CTRL_OBJ_PROGRESS") return CTRL_OBJ_PROGRESS;	//进度条的物件
	if(name == "CTRL_OBJ_MUTITEXT") return CTRL_OBJ_MUTITEXT;	//多行文本的物件
	if(name == "CTRL_OBJ_MUTILIST") return CTRL_OBJ_MUTILIST;	//多列列表框
	if(name == "CTRL_OBJ_COMBO") return CTRL_OBJ_COMBO;		//下拉列表框的物件
	if(name == "CTRL_OBJ_DIRECTORYLIST") return CTRL_OBJ_DIRECTORYLIST;	//路径列表框
	if(name == "CTRL_OBJ_GROUP") return CTRL_OBJ_GROUP;		//群组框
	if(name == "CTRL_OBJ_POINTCTRL") return CTRL_OBJ_POINTCTRL;		//点原
	if(name == "CTRL_OBJ_LINECTRL") return CTRL_OBJ_LINECTRL;		//线原
	if(name == "CTRL_OBJ_SLIDEREX") return CTRL_OBJ_SLIDEREX;	//滑动条的物件
	if(name == "CTRL_OBJ_PASSWORDPAD") return CTRL_OBJ_PASSWORDPAD;	//密码输入板
	if(name == "CTRL_OBJ_IMAGELIST") return CTRL_OBJ_IMAGELIST;		//图像陈列框
	if(name == "CTRL_OBJ_XCHART") return CTRL_OBJ_XCHART;		//图标框
	if(name == "CTRL_OBJ_XTAB") return CTRL_OBJ_XTAB;			//标签控件
	if(name == "CTRL_OBJ_XTEXT") return CTRL_OBJ_XTEXT;			//文字控件
	if(name == "CTRL_OBJ_XSIMPLELINE") return CTRL_OBJ_XSIMPLELINE;	//分割线控件
	if(name == "CTRL_OBJ_PROGRESSRING") return CTRL_OBJ_PROGRESSRING;	//进度条的物件
	if(name == "CTRL_OBJ_CALENDAR") return CTRL_OBJ_CALENDAR;		//日历控件
	if(name == "CTRL_OBJ_XMENU") return CTRL_OBJ_XMENU;			//菜单类的控件
	if(name == "CTRL_OBJ_XIMAGE") return CTRL_OBJ_XIMAGE;		//图片的控件
	if(name == "CTRL_OBJ_BTNBAR") return CTRL_OBJ_BTNBAR;		//按钮条的控件
	if(name == "CTRL_OBJ_XTOOLBAR") return CTRL_OBJ_XTOOLBAR;		//工具条的控件
	if(name == "CTRL_OBJ_FUNCTION") return CTRL_OBJ_FUNCTION;	//绘图函数	
	return CTRL_OBJ_NULL;
}
inline std::string getCtrlTypeByString(_XCtrlObjType type)
{
	switch(type)
	{
	case CTRL_OBJ_NULL:return "CTRL_OBJ_NULL";		//无效的物件
	case CTRL_OBJ_BUTTON:return "CTRL_OBJ_BUTTON";	//按钮物件
	case CTRL_OBJ_BUTTONEX:return "CTRL_OBJ_BUTTONEX";	//按钮物件
	case CTRL_OBJ_CHECK:return "CTRL_OBJ_CHECK";		//复选框的物件
	case CTRL_OBJ_EDIT:return "CTRL_OBJ_EDIT";		//输入框的物件
	case CTRL_OBJ_MOUSERIGHTBUTTONMENU:return "CTRL_OBJ_MOUSERIGHTBUTTONMENU";	//鼠标右键菜单的物件
	case CTRL_OBJ_SLIDER:return "CTRL_OBJ_SLIDER";	//滑动条的物件
	case CTRL_OBJ_RADIOS:return "CTRL_OBJ_RADIOS";	//单选框的物件
	case CTRL_OBJ_PROGRESS:return "CTRL_OBJ_PROGRESS";	//进度条的物件
	case CTRL_OBJ_MUTITEXT:return "CTRL_OBJ_MUTITEXT";	//多行文本的物件
	case CTRL_OBJ_MUTILIST:return "CTRL_OBJ_MUTILIST";	//多列列表框
	case CTRL_OBJ_COMBO:return "CTRL_OBJ_COMBO";		//下拉列表框的物件
	case CTRL_OBJ_DIRECTORYLIST:return "CTRL_OBJ_DIRECTORYLIST";	//路径列表框
	case CTRL_OBJ_GROUP:return "CTRL_OBJ_GROUP";		//群组框
	case CTRL_OBJ_POINTCTRL:return "CTRL_OBJ_POINTCTRL";		//点原
	case CTRL_OBJ_LINECTRL:return "CTRL_OBJ_LINECTRL";		//线原
	case CTRL_OBJ_SLIDEREX:return "CTRL_OBJ_SLIDEREX";	//滑动条的物件
	case CTRL_OBJ_PASSWORDPAD:return "CTRL_OBJ_PASSWORDPAD";	//密码输入板
	case CTRL_OBJ_IMAGELIST:return "CTRL_OBJ_IMAGELIST";		//图像陈列框
	case CTRL_OBJ_XCHART:return "CTRL_OBJ_XCHART";		//图标框
	case CTRL_OBJ_XTAB:return "CTRL_OBJ_XTAB";			//标签控件
	case CTRL_OBJ_XTEXT:return "CTRL_OBJ_XTEXT";			//文字控件
	case CTRL_OBJ_XSIMPLELINE:return "CTRL_OBJ_XSIMPLELINE";	//分割线控件
	case CTRL_OBJ_PROGRESSRING:return "CTRL_OBJ_PROGRESSRING";	//进度条的物件
	case CTRL_OBJ_CALENDAR:return "CTRL_OBJ_CALENDAR";		//日历控件
	case CTRL_OBJ_XMENU:return "CTRL_OBJ_XMENU";			//菜单类的控件
	case CTRL_OBJ_XIMAGE:return "CTRL_OBJ_XIMAGE";		//图片的控件
	case CTRL_OBJ_BTNBAR:return "CTRL_OBJ_BTNBAR";		//按钮条的控件
	case CTRL_OBJ_XTOOLBAR:return "CTRL_OBJ_XTOOLBAR";		//工具条的控件
	case CTRL_OBJ_FUNCTION:return "CTRL_OBJ_FUNCTION";	//绘图函数
	}	
	return "";
}
inline std::string getCtrlNameByType(_XCtrlObjType type)
{
	switch(type)
	{
	case CTRL_OBJ_NULL:return "NULL";		//无效的物件
	case CTRL_OBJ_BUTTON:return "_XButton";	//按钮物件
	case CTRL_OBJ_BUTTONEX:return "_XButtonEx";	//按钮物件
	case CTRL_OBJ_CHECK:return "_XCheck";		//复选框的物件
	case CTRL_OBJ_EDIT:return "_XEdit";		//输入框的物件
	case CTRL_OBJ_MOUSERIGHTBUTTONMENU:return "_XMouseRightButtonMenu";	//鼠标右键菜单的物件
	case CTRL_OBJ_SLIDER:return "_XSlider";	//滑动条的物件
	case CTRL_OBJ_RADIOS:return "_XRadios";	//单选框的物件
	case CTRL_OBJ_PROGRESS:return "_XProgress";	//进度条的物件
	case CTRL_OBJ_MUTITEXT:return "_XMultiText";	//多行文本的物件
	case CTRL_OBJ_MUTILIST:return "_XMultiList";	//多列列表框
	case CTRL_OBJ_COMBO:return "_XCombo";		//下拉列表框的物件
	case CTRL_OBJ_DIRECTORYLIST:return "_XDirectoryList";	//路径列表框
	case CTRL_OBJ_GROUP:return "_XGroup";		//群组框
	case CTRL_OBJ_POINTCTRL:return "_XPointCtrl";		//点原
	case CTRL_OBJ_LINECTRL:return "_XLineCtrl";		//线原
	case CTRL_OBJ_SLIDEREX:return "_XSliderEx";	//滑动条的物件
	case CTRL_OBJ_PASSWORDPAD:return "_XPasswordPad";	//密码输入板
	case CTRL_OBJ_IMAGELIST:return "_XImageList";		//图像陈列框
	case CTRL_OBJ_XCHART:return "_XChart";		//图标框
	case CTRL_OBJ_XTAB:return "_XTab";			//标签控件
	case CTRL_OBJ_XTEXT:return "_XText";			//文字控件
	case CTRL_OBJ_XSIMPLELINE:return "_XSimpleLine";	//分割线控件
	case CTRL_OBJ_PROGRESSRING:return "_XProgressRing";	//进度条的物件
	case CTRL_OBJ_CALENDAR:return "_XCalendar";		//日历控件
	case CTRL_OBJ_XMENU:return "_XMenu";			//菜单类的控件
	case CTRL_OBJ_XIMAGE:return "_XImage";		//图片的控件
	case CTRL_OBJ_BTNBAR:return "_XButtonBar";		//按钮条的控件
	case CTRL_OBJ_XTOOLBAR:return "_XToolBar";		//工具条的控件
	case CTRL_OBJ_FUNCTION:return "NULL";	//绘图函数
	}	
	return "";
}