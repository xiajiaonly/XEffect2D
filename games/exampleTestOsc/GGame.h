#ifndef _JIA_GGAME_
#define _JIA_GGAME_
#include "XEffeEngine.h"
#define WITH_OFXOSC
#ifdef WITH_OFXOSC
#include "ofxOsc.h"
#else
#include "XOsc.h"
#endif
#include "GLanguage.h"

struct GOscData
{
	std::string data;		//数据
	std::string address;	//地址
	int dataType;			//数据类型
};
struct GKMOscData
{
	int keyValue;
	int keyState;
	std::string data;		//数据
	std::string address;	//地址
	int dataType;			//数据类型
};
enum GSendMode
{
	CHOOSE_ADDR,	//发送选中地址
	CHOOSE_DATA,	//发送选中数据
	NULL_DATA,		//发送空数据
	CURRENT_DATA,	//发送当前数据
	ALL_LIST_DATA,	//发送列表中的所有数据
};
#define CFG_FILENAME ("config.xml")

class GGame:public XE::XGameBasic
{
protected:
	GGame(const GGame&);
	GGame &operator= (const GGame&);
public:
	GGame()
		:m_isSenderOpen(false)
		,m_isSendState(false)
		,m_recvState(false)
		//,m_mouseOnTimer(0)
		//,m_isShowMsg(false)
		,m_recvSum(0)
	{}
	virtual ~GGame(){}
public:
	bool init();				//TODO:这里进行游戏的初始化
	void move(float stepTime);	//TODO:这里是游戏的idle，也可以通过开启逻辑线程的方式处理
	void draw();				//TODO:绘制的函数
	void input(const XE::XInputEvent &event);	//TODO:键盘或者鼠标输入的事件
	void release();				//TODO:程序结束时的资源释放
	void ctrlEventProc(int id,int eventID);//TODO:控件的消息处理函数
	//下面是为了窗口实时变化控制而定义的变量
	int m_windowPosX;		//窗口的位置
	int m_windowPosY;		//窗口的位置
	int m_windowSizeW;		//窗口的尺寸
	int m_windowSizeH;		//窗口的尺寸
	XE::XMoveData m_moveData;
	bool m_isFirst;
	bool m_isExit;	//是否触发了退出事件
	friend void windowExitFun(void *p);
private:
	XE::XTab m_tab;
	XE::XMouseRightButtonMenu m_mouseRBtnMnu; //输入框的右键菜单
	//下面是关于界面元素的定义
	XE::XText m_textIP;
	XE::XEdit m_textIPEdt;
	XE::XText m_textPort;
	XE::XEdit m_textPortEdt;
	XE::XButton m_openBtn;
	//XButton m_closeBtn;
	//++++++++++++++++++++++++++++++
	//下面是为了增加键盘模拟而定义的变量
	XE::XButton m_addKMBtn;
	XE::XButton m_delKMBtn;
	XE::XButton m_clearKMBtn;	//清空
	XE::XButton m_modifyKMBtn;	//修改按钮
	XE::XCombo m_keySelectCmb;	//按键的选择
	XE::XCombo m_keyStateCmb;	//按键状态的选择
	XE::XText m_oscDataTxt;
	XE::XEdit m_oscDataEdt;
	XE::XText m_oscType;
	XE::XCombo m_oscDataCmb;
	XE::XText m_oscAddress;
	XE::XEdit m_oscAddressEdt;
	XE::XMultiList m_dataKMLst;
	std::vector<GKMOscData *> m_keyMapOscList;	//映射数据
	std::string keyValueToStr(int keyValue);
	std::string oscDataTypeToStr(int type);
	int keyValueToIndex(XE::XKeyValue k);
	XE::XBool checkKMData(int index = -1);	//检查当前的数据是否合法
	void sendSelectData();	//发送当前选中数据
	void sendKMData(int index);
	int m_mouseX;
	int m_mouseY;
	//------------------------------
	XE::XButton m_addBtn;
	XE::XButton m_delBtn;
	XE::XButton m_clearBtn;	//清空
	XE::XButton m_modifyBtn;	//修改按钮
	XE::XText m_textData;
	XE::XEdit m_textDataEdt;
	XE::XText m_textType;
	XE::XCombo m_textDataCmb;
	XE::XText m_textAddress;
	XE::XEdit m_textAddressEdt;
	XE::XText m_textTimes;
	XE::XEdit m_textTimesEdt;
	XE::XText m_textSpacing;
	XE::XEdit m_textSpacingEdt;
	XE::XButton m_sendBtn;
	//XButton m_stopBtn;
	XE::XCombo m_sendModeCmb;
	XE::XMultiList m_dataLst;
	XE::XCheck m_sendAsMsg;
	XE::XCheck m_supportACKMode;
	XE::XButton m_clearRecvBtn;	//清除接收框的按钮

	XE::XCombo m_languageCmb;		//语言选择
	XE::XCombo m_codeModeCmb;		//编码方式的选择

//	XE::XFontUnicode m_textRIP;
//	XE::XEdit m_textRIPEdt;
	XE::XText m_textRPort;
	XE::XEdit m_textRPortEdt;
	XE::XButton m_openRBtn;
	//XE::XButton m_closeRBtn;

	XE::XButton m_saveBtn;		//保存按钮
	XE::XButton m_readBtn;		//读取配置文件的按钮
	void createUI();
	bool saveToFile(const char *filename = NULL);		//保存到文件
	bool readFromFile(const char *filename = NULL);	//从文件读取
	void addALineData(const GOscData *tmp);
	void setLineData(const GOscData *tmp,int index);
	void addALineData(const GKMOscData *tmp);
	void setLineData(const GKMOscData *tmp,int index);

	bool m_recvState;		//接收的状态
	int m_recvSum;	//接收的统计计数

	XE::XMultiText m_recvText;
	XE::XFontUnicode m_allMsgText;	//总体的提示文字
	//下面是关于功能的定义
	//发送部分
#ifdef WITH_OFXOSC
	ofxOscSender m_sender;
#else
	XE::XOscSender m_sender;
#endif
	bool m_isSenderOpen;	//是否已经打开发送设备
	std::vector<GOscData *> m_datasVector;
	bool m_isSendState;		//是否正在发送过程中
	int m_timer;	//发送计时器
	int m_times;	//发送次数累加器
	int m_setTimer;
	int m_setTimes;
	void sendOverProc();
	void sendButtonDown();
	XE::XBool checkNowData();	//检查当前数据的正确性
	void sendNowData(int mode);
	std::string m_nowSendAddr;		//当前发送的数据的索引号

#ifdef WITH_OFXOSC
	ofxOscReceiver m_receiver;
#else
	XE::XOscReceiver m_receiver;
#endif
	//下面用于鼠标悬浮提示
	//int m_mouseOnTimer;	//用于记录鼠标没有移动处于悬停状态的时间	
	//bool m_isShowMsg;	//是否已经显示提示
	//void ctrlMsgProc();
private:
	void ctrlStateManager();	//根据当前的情况更新界面控件状态
	void setMassage(const char * str)
	{
		XE::LogStr(str);
		m_allMsgText.setString(str);
	}
	void updateLanguage();	//更具语言选择更新界面上的文字(目前尚未生效)
	enum GLanguageIndex
	{
		INDEX_ENGLISH,
		INDEX_CHINESE,
	};
	GLanguageIndex getLanguageIndex() 
	{
		switch(m_languageCmb.getCurChooseOrder())
		{
		case 0: return INDEX_ENGLISH;
		case 1: return INDEX_CHINESE;
		}
		return INDEX_ENGLISH;
	}

	GLanguage m_language;	//多语言的支持
};

#endif