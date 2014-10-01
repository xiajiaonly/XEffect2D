#ifndef _JIA_GGAME_
#define _JIA_GGAME_
#include "XEffeEngine.h"
#include "ofxOsc.h"

struct _GOscData
{
	std::string data;		//数据
	std::string address;	//地址
	int dataType;			//数据类型
};
struct _GKMOscData
{
	int keyValue;
	int keyState;
	std::string data;		//数据
	std::string address;	//地址
	int dataType;			//数据类型
};
#define CFG_FILENAME ("config.xml")

class _GGame
{
protected:
	_GGame()
		:m_isSenderOpen(false)
		,m_isSendState(false)
		,m_recvState(false)
		,m_mouseOnTimer(0)
		,m_isShowMsg(false)
		,m_recvSum(0)
	{}
	_GGame(const _GGame&);
	_GGame &operator= (const _GGame&);
	virtual ~_GGame(){}
public:
	static _GGame& GetInstance()
	{
		static _GGame m_instance;
		return m_instance;
	}
public:
	bool init();				//TODO:这里进行游戏的初始化
	void move(int stepTime);	//TODO:这里是游戏的idle，也可以通过开启逻辑线程的方式处理
	void draw();				//TODO:绘制的函数
	void input(const _XInputEvent &event);	//TODO:键盘或者鼠标输入的事件
	void release();				//TODO:程序结束时的资源释放
	//下面是为了窗口实时变化控制而定义的变量
	int m_windowPosX;		//窗口的位置
	int m_windowPosY;		//窗口的位置
	int m_windowSizeW;		//窗口的尺寸
	int m_windowSizeH;		//窗口的尺寸
	_XMoveData m_moveData;
	bool m_isFirst;
	bool m_isExit;	//是否触发了退出事件
	friend void windowExitFun(void *p);
private:
	_XTab m_tab;
	//下面是关于界面元素的定义
	_XText m_textIP;
	_XEdit m_textIPEdt;
	_XText m_textPort;
	_XEdit m_textPortEdt;
	_XButton m_openBtn;
	//_XButton m_closeBtn;
	//++++++++++++++++++++++++++++++
	//下面是为了增加键盘模拟而定义的变量
	_XButton m_addKMBtn;
	_XButton m_delKMBtn;
	_XButton m_clearKMBtn;	//清空
	_XButton m_modifyKMBtn;	//修改按钮
	_XCombo m_keySelectCmb;	//按键的选择
	_XCombo m_keyStateCmb;	//按键状态的选择
	_XText m_oscDataTxt;
	_XEdit m_oscDataEdt;
	_XText m_oscType;
	_XCombo m_oscDataCmb;
	_XText m_oscAddress;
	_XEdit m_oscAddressEdt;
	_XMultiList m_dataKMLst;
	std::vector<_GKMOscData *> m_keyMapOscList;	//映射数据
	std::string keyValueToStr(int keyValue);
	std::string oscDataTypeToStr(int type);
	int keyValueToIndex(_XKeyValue k);
	_XBool checkKMData(int index = -1);	//检查当前的数据是否合法
	void sendSelectData();	//发送当前选中数据
	void sendKMData(int index);
	int m_mouseX;
	int m_mouseY;
	//------------------------------
	_XButton m_addBtn;
	_XButton m_delBtn;
	_XButton m_clearBtn;	//清空
	_XButton m_modifyBtn;	//修改按钮
	_XText m_textData;
	_XEdit m_textDataEdt;
	_XText m_textType;
	_XCombo m_textDataCmb;
	_XText m_textAddress;
	_XEdit m_textAddressEdt;
	_XText m_textTimes;
	_XEdit m_textTimesEdt;
	_XText m_textSpacing;
	_XEdit m_textSpacingEdt;
	_XButton m_sendBtn;
	//_XButton m_stopBtn;
	_XCombo m_sendModeCmb;
	_XMultiList m_dataLst;
	_XCheck m_sendAsMsg;
	_XCheck m_supportACKMode;
	_XButton m_clearRecvBtn;	//清除接收框的按钮

//	_XFontUnicode m_textRIP;
//	_XEdit m_textRIPEdt;
	_XText m_textRPort;
	_XEdit m_textRPortEdt;
	_XButton m_openRBtn;
	//_XButton m_closeRBtn;

	_XButton m_saveBtn;		//保存按钮
	_XButton m_readBtn;		//读取配置文件的按钮
	bool saveToFile(const char *filename = NULL);		//保存到文件
	bool readFromFile(const char *filename = NULL);	//从文件读取
	void addALineData(const _GOscData *tmp);
	void setLineData(const _GOscData *tmp,int index);
	void addALineData(const _GKMOscData *tmp);
	void setLineData(const _GKMOscData *tmp,int index);

	bool m_recvState;		//接收的状态
	int m_recvSum;	//接收的统计计数

	_XMultiText m_recvText;
	_XFontUnicode m_allMsgText;	//总体的提示文字
	friend void ctrlFun(void *,int);
	friend void ctrlFunX(void *,int);
	//下面是关于功能的定义
	//发送部分
	ofxOscSender m_sender;
	bool m_isSenderOpen;	//是否已经打开发送设备
	std::vector<_GOscData *> m_datasVector;
	bool m_isSendState;		//是否正在发送过程中
	int m_timer;	//发送计时器
	int m_times;	//发送次数累加器
	int m_setTimer;
	int m_setTimes;
	void sendOverProc();
	void sendButtonDown();
	_XBool checkNowData();	//检查当前数据的正确性
	void sendNowData(int mode);
	std::string m_nowSendAddr;		//当前发送的数据的索引号

	ofxOscReceiver m_receiver;
	//下面用于鼠标悬浮提示
	int m_mouseOnTimer;	//用于记录鼠标没有移动处于悬停状态的时间	
	bool m_isShowMsg;	//是否已经显示提示
	void ctrlMsgProc();
private:
	void ctrlStateManager();	//根据当前的情况更新界面控件状态
};

#endif