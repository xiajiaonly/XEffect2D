namespace XCtrl
{
	INLINE XCtrlObjType getCtrlTypeByString(const std::string &name)
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
		if(name == "CTRL_OBJ_CHART") return CTRL_OBJ_CHART;		//图标框
		if(name == "CTRL_OBJ_SIMPLECHART") return CTRL_OBJ_SIMPLECHART;		//图标框
		if(name == "CTRL_OBJ_TAB") return CTRL_OBJ_TAB;			//标签控件
		if(name == "CTRL_OBJ_TEXT") return CTRL_OBJ_TEXT;			//文字控件
		if(name == "CTRL_OBJ_SIMPLELINE") return CTRL_OBJ_SIMPLELINE;	//分割线控件
		if(name == "CTRL_OBJ_PROGRESSRING") return CTRL_OBJ_PROGRESSRING;	//进度条的物件
		if(name == "CTRL_OBJ_CALENDAR") return CTRL_OBJ_CALENDAR;		//日历控件
		if(name == "CTRL_OBJ_MENU") return CTRL_OBJ_MENU;			//菜单类的控件
		if(name == "CTRL_OBJ_IMAGE") return CTRL_OBJ_IMAGE;		//图片的控件
		if(name == "CTRL_OBJ_BTNBAR") return CTRL_OBJ_BTNBAR;		//按钮条的控件
		if(name == "CTRL_OBJ_TOOLBAR") return CTRL_OBJ_TOOLBAR;		//工具条的控件
		if(name == "CTRL_OBJ_PROPERTYBOX") return CTRL_OBJ_PROPERTYBOX;		//工具条的控件
		if(name == "CTRL_OBJ_COLORCHOOSE") return CTRL_OBJ_COLORCHOOSE;		//工具条的控件
		if(name == "CTRL_OBJ_SUBWINDOW") return CTRL_OBJ_SUBWINDOW;		//工具条的控件
		if(name == "CTRL_OBJ_SOFTBOARD") return CTRL_OBJ_SOFTBOARD;		//工具条的控件
		if(name == "CTRL_OBJ_PARAMCTRL") return CTRL_OBJ_PARAMCTRL;		//工具条的控件
		if(name == "CTRL_OBJ_FUNCTION") return CTRL_OBJ_FUNCTION;	//绘图函数	
		return CTRL_OBJ_NULL;
	}
	INLINE std::string getCtrlTypeByString(XCtrlObjType type)
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
		case CTRL_OBJ_CHART:return "CTRL_OBJ_CHART";		//图标框
		case CTRL_OBJ_SIMPLECHART:return "CTRL_OBJ_SIMPLECHART";	//按钮物件
		case CTRL_OBJ_TAB:return "CTRL_OBJ_TAB";			//标签控件
		case CTRL_OBJ_TEXT:return "CTRL_OBJ_TEXT";			//文字控件
		case CTRL_OBJ_SIMPLELINE:return "CTRL_OBJ_SIMPLELINE";	//分割线控件
		case CTRL_OBJ_PROGRESSRING:return "CTRL_OBJ_PROGRESSRING";	//进度条的物件
		case CTRL_OBJ_CALENDAR:return "CTRL_OBJ_CALENDAR";		//日历控件
		case CTRL_OBJ_MENU:return "CTRL_OBJ_MENU";			//菜单类的控件
		case CTRL_OBJ_IMAGE:return "CTRL_OBJ_IMAGE";		//图片的控件
		case CTRL_OBJ_BTNBAR:return "CTRL_OBJ_BTNBAR";		//按钮条的控件
		case CTRL_OBJ_TOOLBAR:return "CTRL_OBJ_TOOLBAR";		//工具条的控件
		case CTRL_OBJ_PROPERTYBOX:return "CTRL_OBJ_PROPERTYBOX";		//属性框
		case CTRL_OBJ_COLORCHOOSE:return "CTRL_OBJ_COLORCHOOSE";		//属性框
		case CTRL_OBJ_SUBWINDOW:return "CTRL_OBJ_SUBWINDOW";		//属性框
		case CTRL_OBJ_SOFTBOARD:return "CTRL_OBJ_SOFTBOARD";		//属性框
		case CTRL_OBJ_PARAMCTRL:return "CTRL_OBJ_PARAMCTRL";		//属性框
		case CTRL_OBJ_FUNCTION:return "CTRL_OBJ_FUNCTION";	//绘图函数
		}	
		return "";
	}
	INLINE std::string getCtrlNameByType(XCtrlObjType type)
	{
		switch(type)
		{
		case CTRL_OBJ_NULL:return "NULL";		//无效的物件
		case CTRL_OBJ_BUTTON:return "XButton";	//按钮物件
		case CTRL_OBJ_BUTTONEX:return "XButtonEx";	//按钮物件
		case CTRL_OBJ_CHECK:return "XCheck";		//复选框的物件
		case CTRL_OBJ_EDIT:return "XEdit";		//输入框的物件
		case CTRL_OBJ_MOUSERIGHTBUTTONMENU:return "XMouseRightButtonMenu";	//鼠标右键菜单的物件
		case CTRL_OBJ_SLIDER:return "XSlider";	//滑动条的物件
		case CTRL_OBJ_RADIOS:return "XRadios";	//单选框的物件
		case CTRL_OBJ_PROGRESS:return "XProgress";	//进度条的物件
		case CTRL_OBJ_MUTITEXT:return "XMultiText";	//多行文本的物件
		case CTRL_OBJ_MUTILIST:return "XMultiList";	//多列列表框
		case CTRL_OBJ_COMBO:return "XCombo";		//下拉列表框的物件
		case CTRL_OBJ_DIRECTORYLIST:return "XDirectoryList";	//路径列表框
		case CTRL_OBJ_GROUP:return "XGroup";		//群组框
		case CTRL_OBJ_POINTCTRL:return "XPointCtrl";		//点原
		case CTRL_OBJ_LINECTRL:return "XLineCtrl";		//线原
		case CTRL_OBJ_SLIDEREX:return "XSliderEx";	//滑动条的物件
		case CTRL_OBJ_PASSWORDPAD:return "XPasswordPad";	//密码输入板
		case CTRL_OBJ_IMAGELIST:return "XImageList";		//图像陈列框
		case CTRL_OBJ_CHART:return "XChart";		//图标框
		case CTRL_OBJ_SIMPLECHART:return "XSimpleChart";		//图标框
		case CTRL_OBJ_TAB:return "XTab";			//标签控件
		case CTRL_OBJ_TEXT:return "XText";			//文字控件
		case CTRL_OBJ_SIMPLELINE:return "XSimpleLine";	//分割线控件
		case CTRL_OBJ_PROGRESSRING:return "XProgressRing";	//进度条的物件
		case CTRL_OBJ_CALENDAR:return "XCalendar";		//日历控件
		case CTRL_OBJ_MENU:return "XMenu";			//菜单类的控件
		case CTRL_OBJ_IMAGE:return "XImage";		//图片的控件
		case CTRL_OBJ_BTNBAR:return "XButtonBar";		//按钮条的控件
		case CTRL_OBJ_TOOLBAR:return "XToolBar";		//工具条的控件
		case CTRL_OBJ_PROPERTYBOX:return"XPropertyBox";
		case CTRL_OBJ_COLORCHOOSE:return"XColorChoose";
		case CTRL_OBJ_SUBWINDOW:return"XSubWindow";
		case CTRL_OBJ_SOFTBOARD:return"XSoftBoard";
		case CTRL_OBJ_PARAMCTRL:return"XParameterCtrl";
		case CTRL_OBJ_FUNCTION:return "NULL";	//绘图函数
		}	
		return "";
	}
}