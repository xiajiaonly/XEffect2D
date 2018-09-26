#ifndef _JIA_GGAME_
#define _JIA_GGAME_
#include "XEffeEngine.h"

//说明期望加入多选移动的功能，一次选择多个控件进行统一的移动。(尚未完成)
//可以通过按下Ctrl按键进行多个控件的选择，
//处于选择状态的控件需要有被选择的标识
//多选的控件可以进行统一的移动和删除

struct GCtrlInfo
{
	int ctrlID;	//ID
	std::string ctrlName;
	XE::XControlBasic * pCtrl;
	XE::XBool withCallback;	//是否使用回调函数
	//目前只支持tab的从属关系
	std::string affiliationStr;	//描述从属关系的字符串:从属关系的描述方式为 ctrname:tabname;
	//下面是XE::XCombo的相关属性
	int allMenuSum;		//总菜单数量
	int showMenuSum;	//显示的菜单数量
	
	std::string ctrlComment;		//控件说明
	std::string codeExplanation;	//代码说明

	//下面是为了辅助显示而定义的变量
	XE::XBool isMouseOn;	//是否鼠标悬浮状态

 	GCtrlInfo()
		:pCtrl(NULL)
		,withCallback(XFalse)
		,allMenuSum(0)
		,showMenuSum(0)
		,affiliationStr("NULL")
		,isMouseOn(XFalse)
		,ctrlComment("NULL")
		,codeExplanation("NULL")
	{}
};
#define CFG_FILENAME ("UIConfig.xml")
class GGame:public XE::XGameBasic
{
public:
	GGame()
		:m_ctrlID(0)
		,m_curChooseCtrlID(-1)
		,m_isCtrlDown(false)
		,m_isMouseSelect(false)
		,m_isMouseDrag(false)
	{}
	virtual ~GGame(){}
protected:
	GGame(const GGame&);
	GGame &operator= (const GGame&);
public:
	bool init();				//TODO:这里进行游戏的初始化
	void move(float stepTime);	//TODO:这里是游戏的idle，也可以通过开启逻辑线程的方式处理
	void draw();				//TODO:绘制的函数
	void input(const XE::XInputEvent &event);	//TODO:键盘或者鼠标输入的事件
	void release();				//TODO:程序结束时的资源释放
	void drawUp();
	void ctrlEventProc(int id,int eventID){}

	int m_curChooseCtrlID;	//当前选择的物件的ID
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面为多选加入
	std::vector<int> m_curChooseCtrls;		//当前所有选择的控件，使用Ctrl+点击选择
	bool m_isCtrlDown;						//ctrl是否按下
	int isSelect(int id);					//判断指定ID的控件是否已经选择
	//鼠标拖动片选
	bool m_isMouseSelect;	//鼠标是否处于片选状态
	XE::XVec2 m_mouseDownPos;	//鼠标按下的位置
	XE::XVec2 m_mouseCurPos;		//当前的鼠标位置
	void updateMouseSelect();
	//鼠标拖动控件
	bool m_isMouseDrag;	//是否鼠标拖动状态
	XE::XVec2 m_mouseDragPos;
	//----------------------------------------------------------

	XE::XSubWindow m_subWindow;	//子窗口
	bool createUI();

	std::vector<GCtrlInfo> m_ctrls;
	int m_ctrlID;
	XE::XVec2 m_curCtrlPos;
	//下面是关于建立控件的相关操作
	XE::XBool addACtrl(XE::XCtrlObjType type);	//建立一个控件
	XE::XBool delACtrl();//删除一个控件
	XE::XBool delSelectCtrl();	//删除所有选择的控件
	int getNewMouseChooseCtrl(float x,float y);			//获取目前鼠标选择的控件的编号
	XE::XCtrlObjType getCurChooseCtrlType()const;	//获取目前选择的控件的类型
	void setCurChooseCtrlType(XE::XCtrlObjType type);
	bool updateCtrlList(float x,float y);	//根据鼠标的位置更新控件列表，返回是否有控件被鼠标击中
	void updateCtrlProperty();		//将当前选择的空间的属性更新到街界面上
	int getCtrlIDFromList();	//从列表框中获取当前选择的控件的ID

	int getCtrlIndexByID(int id)const;
	int getCtrlIndexByName(const std::string &name)const;
	int getCurChooseCtrlIndex()const{return getCtrlIndexByID(m_curChooseCtrlID);}
	XE::XBool saveToFile();		//保存配置文件
	XE::XBool loadFromFile();	//读取配置文件
	XE::XBool createCode();		//建立代码

	friend void ctrlFun(void *pClass,int id);
	XE::XTab m_tab;
	//控件列表
	XE::XRadios m_ctrlChooseRds;
	//属性
	XE::XMultiList m_chooseCtrlMltlst;	//显示当前所有选择的控件
	XE::XText m_curMousePosTxt;
	XE::XButton m_delBtn;	//删除按钮
		//公用属性
	XE::XText m_ctrlPosTxt;	//控件所在的位置
	XE::XEdit m_ctrlPosXEdt;	//控件的坐标
	XE::XEdit m_ctrlPosYEdt;	//控件的坐标
	XE::XText m_ctrlPixelSizeTxt;	//控件的像素大小
	XE::XEdit m_ctrlPixelSizeXEdt;	//控件的像素宽度
	XE::XEdit m_ctrlPixelSizeYEdt;	//控件的像素高度
	XE::XText m_ctrlScaleSizeTxt;	//控件的缩放大小
	XE::XEdit m_ctrlScaleSizeXEdt;	//控件的缩放宽度
	XE::XEdit m_ctrlScaleSizeYEdt;	//控件的缩放高度
	XE::XText m_ctrlNameTxt;		//控件的名称
	XE::XEdit m_ctrlNameEdt;
	XE::XText m_ctrlAffiliationTxt;	//控件的从属关系
	XE::XEdit m_ctrlAffiliationEdt;
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++
	//新加入的属性
	XE::XText m_ctrlCommentTxt;
	XE::XEdit m_ctrlCommentEdt;		//控件的说明
	XE::XText m_ctrlExplanationTxt;
	XE::XEdit m_ctrlExplanationEdt;	//控件的代码注释
	//-----------------------------------------------------
		//私有属性
	XE::XText m_ctrlCaptionTxt;	//控件的标题
	XE::XEdit m_ctrlCaptionEdt;
	XE::XCheck m_withCallbackChk;	//是否使用回调函数
		//combo的参数
	XE::XText m_cmbAllMenuSumTxt;	//下拉菜单的菜单数量
	XE::XEdit m_cmbAllMenuSumEdt;
	XE::XText m_cmbShowMenuSumTxt;	//下拉菜单中显示的菜单数量
	XE::XEdit m_cmbShowMenuSumEdt;	

	void updateAffiliation();
	void clearAffiliation();	//清除从属关系
	//系统
	XE::XText m_winSizeTxt;	//窗口尺寸
	XE::XEdit m_winSizeXEdt;
	XE::XEdit m_winSizeYEdt;
	XE::XText m_prjNameTxt;	//工程名称
	XE::XEdit m_prjNameEdt;
	XE::XButton m_saveBtn;		//保存按钮
	XE::XButton m_readBtn;		//读取按钮
	XE::XButton m_codeBtn;		//生成代码按钮
	//下面是变量
	//控件列表
	int m_winWidth;
	int m_winHeight;
	//属性
	//系统

	//下面用于实现键盘移动物体
	XE::XKeyPressModel m_keyUp;
	XE::XKeyPressModel m_keyDown;
	XE::XKeyPressModel m_keyLeft;
	XE::XKeyPressModel m_keyRight;
};

#endif