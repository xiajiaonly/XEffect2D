#include "GGame.h"
#include "systemHook.h"
#pragma comment(lib, "systemHook.lib")

void ctrlFunX(void *pClass,int id)
{
	_GGame &pPar = *(_GGame *)pClass;
	if(id == pPar.m_dataLst.getControlID())
	{//鼠标双击事件
		pPar.sendButtonDown();
	}else
	if(id == pPar.m_dataKMLst.getControlID())
	{//鼠标双击事件，发送指定数据
		pPar.sendSelectData();
	}
}
void ctrlFun(void *pClass,int id)
{
	_GGame &pPar = *(_GGame *)pClass;
	if(id == pPar.m_openBtn.getControlID())
	{//打开发送
		if(!pPar.m_isSenderOpen)
		{
			pPar.m_sender.setup(pPar.m_textIPEdt.getString(),atoi(pPar.m_textPortEdt.getString()));
			pPar.m_sender.setSendAsMessage(true);
			pPar.m_isSenderOpen = true;
			pPar.m_allMsgText.setString("开启完成,是否成功请自己确定.");
		//	pPar.m_openBtn.disable();
		//	pPar.m_closeBtn.enable();
			pPar.m_openBtn.setCaptionText("关闭");
			pPar.m_textIPEdt.disable();
			pPar.m_textPortEdt.disable();
			if(pPar.m_keyMapOscList.size() > 0) startHook();//开启hook
			pPar.m_mouseX = -1;
			pPar.m_mouseY = -1;
		}else
		{
			pPar.m_sender.setup("",0);
			pPar.m_isSenderOpen = false;
			pPar.m_allMsgText.setString("关闭完成.");
		//	pPar.m_openBtn.enable();
		//	pPar.m_closeBtn.disable();
			pPar.m_openBtn.setCaptionText("打开");
			pPar.m_textIPEdt.enable();
			pPar.m_textPortEdt.enable();
			if(pPar.m_keyMapOscList.size() > 0) stopHook();//关闭hook
		}
	}else
	//if(id == pPar.m_closeBtn.getControlID())
	//{//关闭发送
	//	if(pPar.m_isSenderOpen)
	//	{
	//		pPar.m_sender.setup("",0);
	//		pPar.m_isSenderOpen = false;
	//		pPar.m_allMsgText.setString("关闭完成.");
	//		pPar.m_openBtn.enable();
	//		pPar.m_closeBtn.disable();
	//		pPar.m_textIPEdt.enable();
	//		pPar.m_textPortEdt.enable();
	//		if(pPar.m_keyMapOscList.size() > 0) stopHook();//关闭hook
	//	}else
	//	{
	//		pPar.m_allMsgText.setString("尚未开启.");
	//	}
	//}else
	if(id == pPar.m_addBtn.getControlID())
	{//添加
		if(!pPar.checkNowData())
		{
			pPar.m_allMsgText.setString("数据类型与实际的数据不符.");
			return;
		}
		//添加数据
		_GOscData *tmp = createMem<_GOscData>();
		tmp->data = pPar.m_textDataEdt.getString();
		tmp->address = pPar.m_textAddressEdt.getString();
		tmp->dataType = pPar.m_textDataCmb.getNowChooseOrder();
		pPar.m_datasVector.push_back(tmp);
		//更新列表
		pPar.addALineData(tmp);
		pPar.m_allMsgText.setString("数据添加成功.");
	}else
	if(id == pPar.m_delBtn.getControlID())
	{//删除
		//删除数据
		int lineSum = pPar.m_dataLst.getSelectIndex();
		if(lineSum >= 0)
		{
			_GOscData *tmp = pPar.m_datasVector[lineSum];
			for(int i = lineSum;i < pPar.m_datasVector.size() - 1;++ i)
			{
				pPar.m_datasVector[i] = pPar.m_datasVector[i + 1];
			}
			pPar.m_datasVector.pop_back();
			XDELETE(tmp);
			pPar.m_dataLst.deleteLine(lineSum);
			pPar.m_allMsgText.setString("删除成功.");
		}else
		{
			pPar.m_allMsgText.setString("请选择要删除的数据.");
		}
	}else
	if(id == pPar.m_sendBtn.getControlID())
	{//发送
		if(pPar.m_isSendState) pPar.sendOverProc();
		else pPar.sendButtonDown();
	}else
//	if(id == pPar.m_stopBtn.getControlID())
//	{//停止发送
//		if(pPar.m_isSendState) pPar.sendOverProc();
//		else pPar.m_allMsgText.setString("没有发送数据.");
//	}else
	if(id == pPar.m_openRBtn.getControlID())
	{//开启接收
		if(pPar.m_recvState)
		{
			pPar.m_recvState = false;
			pPar.m_receiver.setup(0);
			pPar.m_allMsgText.setString("已关闭接收.");
		//	pPar.m_openRBtn.enable();
		//	pPar.m_closeRBtn.disable();
			pPar.m_openRBtn.setCaptionText("开始接收");
			pPar.m_textRPortEdt.enable();
			pPar.m_supportACKMode.enable();
		}else
		{
			pPar.m_recvState = true;
			pPar.m_receiver.setup(atoi(pPar.m_textRPortEdt.getString()));
			pPar.m_allMsgText.setString("已经开启接收,是否成功请自行判断.");
		//	pPar.m_openRBtn.disable();
		//	pPar.m_closeRBtn.enable();
			pPar.m_openRBtn.setCaptionText("停止接收");
			pPar.m_textRPortEdt.disable();
			pPar.m_supportACKMode.disable();
		}
	}else
	//if(id == pPar.m_closeRBtn.getControlID())
	//{//关闭接收
	//	if(pPar.m_recvState)
	//	{
	//		pPar.m_recvState = false;
	//		pPar.m_receiver.setup(0);
	//		pPar.m_allMsgText.setString("已关闭接收.");
	//		pPar.m_openRBtn.enable();
	//		pPar.m_closeRBtn.disable();
	//		pPar.m_textRPortEdt.enable();
	//		pPar.m_supportACKMode.enable();
	//	}else
	//	{
	//		pPar.m_allMsgText.setString("接收尚未开启.");
	//	}
	//}else
	if(id == pPar.m_clearRecvBtn.getControlID())
	{
		pPar.m_recvText.setString("Recv:");
		pPar.m_allMsgText.setString("接收框数据已经清空.");
	}else
	if(id == pPar.m_saveBtn.getControlID())
	{//保存
		if(!pPar.saveToFile(getChooseFilename(CFG_FILENAME,"保存",false).c_str()))
		{
			pPar.m_allMsgText.setString("写入配置文件失败.");
		}
	}else
	if(id == pPar.m_readBtn.getControlID()) 
	{//读取
		if(pPar.m_recvState || pPar.m_isSendState)
		{
			pPar.m_allMsgText.setString("正在接受或者发送状态不能读取配置文件.");
			return;
		}
		if(!pPar.readFromFile(getChooseFilename(CFG_FILENAME,"读取",true).c_str()))
		{
			pPar.m_allMsgText.setString("读取配置文件失败.");
		}
	}else
	if(id == pPar.m_dataLst.getControlID())
	{//选择跟新数据
		int index = pPar.m_dataLst.getSelectIndex();
		if(index >= 0)
		{
			pPar.m_textDataEdt.setString(pPar.m_datasVector[index]->data.c_str());
			pPar.m_textDataCmb.setNowChooseOrder(pPar.m_datasVector[index]->dataType);
			pPar.m_textAddressEdt.setString(pPar.m_datasVector[index]->address.c_str());
		}
	}else
	if(id == pPar.m_modifyBtn.getControlID())
	{//修改
		int index = pPar.m_dataLst.getSelectIndex();
		if(index >= 0 && pPar.checkNowData())
		{
			pPar.m_datasVector[index]->data = pPar.m_textDataEdt.getString();
			pPar.m_datasVector[index]->dataType = pPar.m_textDataCmb.getNowChooseOrder();
			pPar.m_datasVector[index]->address = pPar.m_textAddressEdt.getString();
			pPar.setLineData(pPar.m_datasVector[index],index);
			pPar.m_allMsgText.setString("修改完成.");
		}else
		{
			pPar.m_allMsgText.setString("请选择需要修改的数据.");
		}
	}else
	if(id == pPar.m_clearBtn.getControlID())
	{//清空
		if(pPar.m_datasVector.size() <= 0)
		{
			pPar.m_allMsgText.setString("列表中没有数据.");
			return;
		}
		pPar.m_dataLst.setLineSum(0);
		for(int i = 0;i < pPar.m_datasVector.size();++ i)
		{
			XDELETE(pPar.m_datasVector[i]);
		}
		pPar.m_datasVector.clear();
		pPar.m_allMsgText.setString("清空完成.");
	}else
	if(id == pPar.m_addKMBtn.getControlID())
	{//添加
		if(!pPar.checkKMData())
		{
			pPar.m_allMsgText.setString("数据数据不合法或者已经存在.");
			return;
		}
		//添加数据
		_GKMOscData *tmp = createMem<_GKMOscData>();
		tmp->keyValue = pPar.m_keySelectCmb.getNowChooseOrder();
		tmp->keyState = pPar.m_keyStateCmb.getNowChooseOrder();
		if(tmp->keyValue == 39 || tmp->keyValue == 40)
		{
			tmp->data = "0 - 255";
			tmp->address = pPar.m_oscAddressEdt.getString();
			tmp->dataType = 0;
		}else
		{
			tmp->data = pPar.m_oscDataEdt.getString();
			tmp->address = pPar.m_oscAddressEdt.getString();
			tmp->dataType = pPar.m_oscDataCmb.getNowChooseOrder();
		}
		pPar.m_keyMapOscList.push_back(tmp);
		//更新列表
		pPar.addALineData(tmp);
		pPar.m_allMsgText.setString("数据添加成功.");
	}else
	if(id == pPar.m_delKMBtn.getControlID())
	{//删除
		//删除数据
		int lineSum = pPar.m_dataKMLst.getSelectIndex();
		if(lineSum >= 0)
		{
			_GKMOscData *tmp = pPar.m_keyMapOscList[lineSum];
			for(int i = lineSum;i < pPar.m_keyMapOscList.size() - 1;++ i)
			{
				pPar.m_keyMapOscList[i] = pPar.m_keyMapOscList[i + 1];
			}
			pPar.m_keyMapOscList.pop_back();
			XDELETE(tmp);
			pPar.m_dataKMLst.deleteLine(lineSum);
			pPar.m_allMsgText.setString("删除成功.");
		}else
		{
			pPar.m_allMsgText.setString("请选择要删除的数据.");
		}
	}else
	if(id == pPar.m_clearKMBtn.getControlID())
	{//清空
		if(pPar.m_keyMapOscList.size() <= 0)
		{
			pPar.m_allMsgText.setString("列表中没有数据.");
			return;
		}
		pPar.m_dataKMLst.setLineSum(0);
		for(int i = 0;i < pPar.m_keyMapOscList.size();++ i)
		{
			XDELETE(pPar.m_keyMapOscList[i]);
		}
		pPar.m_keyMapOscList.clear();
		pPar.m_allMsgText.setString("清空完成.");
	}else
	if(id == pPar.m_modifyKMBtn.getControlID())
	{//修改
		int index = pPar.m_dataKMLst.getSelectIndex();
		if(index >= 0 && pPar.checkKMData(index))
		{
			pPar.m_keyMapOscList[index]->keyValue = pPar.m_keySelectCmb.getNowChooseOrder();
			pPar.m_keyMapOscList[index]->keyState = pPar.m_keyStateCmb.getNowChooseOrder();
			pPar.m_keyMapOscList[index]->data = pPar.m_oscDataEdt.getString();
			pPar.m_keyMapOscList[index]->dataType = pPar.m_oscDataCmb.getNowChooseOrder();
			pPar.m_keyMapOscList[index]->address = pPar.m_oscAddressEdt.getString();
			pPar.setLineData(pPar.m_keyMapOscList[index],index);
			pPar.m_allMsgText.setString("修改完成.");
		}else
		{
			pPar.m_allMsgText.setString("请选择需要修改的数据.");
		}
	}else
	if(id == pPar.m_dataKMLst.getControlID())
	{//选择跟新数据
		int index = pPar.m_dataKMLst.getSelectIndex();
		if(index >= 0)
		{
			pPar.m_keySelectCmb.setNowChooseOrder(pPar.m_keyMapOscList[index]->keyValue);
			pPar.m_keyStateCmb.setNowChooseOrder(pPar.m_keyMapOscList[index]->keyState);
			pPar.m_oscDataEdt.setString(pPar.m_keyMapOscList[index]->data.c_str());
			pPar.m_oscDataCmb.setNowChooseOrder(pPar.m_keyMapOscList[index]->dataType);
			pPar.m_oscAddressEdt.setString(pPar.m_keyMapOscList[index]->address.c_str());
		}
	}
}
_XBool _GGame::checkNowData()
{
	switch(m_textDataCmb.getNowChooseOrder())
	{
	case 0:	//int32
	case 1:	//int64
	case 2:	//float
		if(!getIsNumber(m_textDataEdt.getString())) return XFalse;
		break;
	case 3:	//string
		break;
	case 4:	//blob	//尚未支持
		break;
	}
	return XTrue;
}
_XBool _GGame::checkKMData(int index)
{
	for(int i = 0;i < m_keyMapOscList.size();++ i)
	{
		if(m_keyMapOscList[i]->keyValue == m_keySelectCmb.getNowChooseOrder() &&
			m_keyMapOscList[i]->keyState == m_keyStateCmb.getNowChooseOrder() && 
			i != index) return XFalse;	//相同的设定不能存在
	}
	if(m_keySelectCmb.getNowChooseOrder() != 39 &&
		m_keySelectCmb.getNowChooseOrder() != 40)
	{
		switch(m_oscDataCmb.getNowChooseOrder())
		{
		case 0:	//int32
		case 1:	//int64
		case 2:	//float
			if(!getIsNumber(m_oscDataEdt.getString())) return XFalse;
			break;
		case 3:	//string
			break;
		case 4:	//blob	//尚未支持
			break;
		}
	}
	return XTrue;
}
void _GGame::sendOverProc()
{
	m_isSendState = false;
	m_allMsgText.setString("已停止.");
	//这里将相关的控件使能(尚未完成)
	m_textIPEdt.enable();
	m_textPortEdt.enable();
	m_openBtn.enable();
//	m_closeBtn.enable();
	m_addBtn.enable();
	m_delBtn.enable();
	m_clearBtn.enable();
	m_modifyBtn.enable();
	m_textDataEdt.enable();
	m_textDataCmb.enable();
	m_textAddressEdt.enable();
	m_textTimesEdt.enable();
	m_textSpacingEdt.enable();

	m_addKMBtn.enable();
	m_delKMBtn.enable();
	m_clearKMBtn.enable();
	m_modifyKMBtn.enable();
	m_keySelectCmb.enable();
	m_keyStateCmb.enable();
	m_oscDataEdt.enable();
	m_oscDataCmb.enable();
	m_oscAddressEdt.enable();

	m_sendBtn.enable();
	m_sendBtn.setCaptionText("开始发送");
//	m_stopBtn.enable();	//不做非常严格的逻辑
	m_sendAsMsg.enable();
	m_sendModeCmb.enable();
}
void _GGame::sendSelectData()
{
	if(!m_isSenderOpen)
	{
		m_allMsgText.setString("网络尚未打开,请先打开网络.");
		return;
	}
	//下面发送选中数据
	int index = m_dataKMLst.getSelectIndex();
	if(index >= 0) sendKMData(index);
}
void _GGame::sendKMData(int index)
{
	ofxOscMessage m;
	m.setAddress(m_keyMapOscList[index]->address);
	LogNull("发送一条数据-地址:%s %d",m_keyMapOscList[index]->address.c_str(),index);
	if(m_keyMapOscList[index]->keyValue == 39)
	{
		m.addIntArg(m_mouseX);
	}else
	if(m_keyMapOscList[index]->keyValue == 40)
	{
		m.addIntArg(m_mouseY);
	}else
	{
		switch(m_keyMapOscList[index]->dataType)
		{
		case 0:	//int32
			m.addIntArg(atoi(m_keyMapOscList[index]->data.c_str()));
			break;
		case 1:	//int64
			m.addInt64Arg(atoi(m_keyMapOscList[index]->data.c_str()));
			break;
		case 2:	//float
			m.addFloatArg(atof(m_keyMapOscList[index]->data.c_str()));
			break;
		case 3:	//string
			m.addStringArg(m_keyMapOscList[index]->data);
			break;
		case 4:	//blob尚无支持
			break;
		}
	}
	if(m_sendAsMsg.getState()) m_sender.sendMessage(m);
	else
	{
		ofxOscBundle b;
		b.addMessage(m);
		m_sender.sendBundle(b);
	}
}
void _GGame::sendButtonDown()
{
	if(!m_isSenderOpen)
	{
		m_allMsgText.setString("网络尚未打开,请先打开网络.");
		return;
	}
	if(m_isSendState)
	{
		m_allMsgText.setString("正在发送数据,请稍等.");
	}else
	{//下面开始发送数据
		if(m_sendModeCmb.getNowChooseOrder() == 0
			|| m_sendModeCmb.getNowChooseOrder() == 1)
		{
			int tmp = m_dataLst.getSelectIndex();
			if(tmp < 0)
			{
				m_allMsgText.setString("请选择一个需要发送的OSC地址.");
				return;
			}
			m_nowSendAddr = m_dataLst.getBoxStr(tmp,0);	//获取将要发送的OSC地址
		}
		if(m_sendModeCmb.getNowChooseOrder() == 2)
		{//发送空数据
			int tmp = m_dataLst.getSelectIndex();
			if(tmp < 0)
			{
				if(m_textAddressEdt.getString() != NULL && strlen(m_textAddressEdt.getString()) > 0)
				{
					m_nowSendAddr = m_textAddressEdt.getString();
				}else
				{
					m_allMsgText.setString("请选择或者输入需要发送的OSC地址.");
					return;
				}
			}else
			{
				m_nowSendAddr = m_dataLst.getBoxStr(tmp,0);	//获取将要发送的OSC地址
			}
		}
		if(m_sendModeCmb.getNowChooseOrder() == 3 && !checkNowData())
		{//如果是发送当前数据的话，检查当前数据是否合法
			m_allMsgText.setString("当前数据不合法,请确保数据类型和数据匹配.");
			return;
		}
		m_setTimer = atoi(m_textSpacingEdt.getString());
		m_setTimes = atoi(m_textTimesEdt.getString());
		if(m_setTimer <= 0) m_setTimer = 0;
		if(m_setTimes <= 0) m_setTimes = 0;
		m_isSendState = true;
		m_timer = 0;
		m_times = 0;
		m_allMsgText.setString("已经开始发送数据.");
		//这里将相关的控件无效(尚未完成)
		m_textIPEdt.disable();
		m_textPortEdt.disable();
		m_openBtn.disable();
//		m_closeBtn.disable();
		m_addBtn.disable();
		m_delBtn.disable();
		m_clearBtn.disable();
		m_modifyBtn.disable();
		m_textDataEdt.disable();
		m_textDataCmb.disable();
		m_textAddressEdt.disable();
		m_textTimesEdt.disable();
		m_textSpacingEdt.disable();

		m_addKMBtn.disable();
		m_delKMBtn.disable();
		m_clearKMBtn.disable();
		m_modifyKMBtn.disable();
		m_keySelectCmb.disable();
		m_keyStateCmb.disable();
		m_oscDataEdt.disable();
		m_oscDataCmb.disable();
		m_oscAddressEdt.disable();

//		m_sendBtn.disable();
//		m_stopBtn.enable();
		m_sendBtn.setCaptionText("停止发送");
		m_sendAsMsg.disable();
		m_sendModeCmb.disable();
	}
}
void windowExitFun(void *p)
{
	_GGame &pPar = *(_GGame *)p;
	if(XEE::windowData.withFrame == 0)
	{//没有边框的时候才做这步动作
		pPar.m_moveData.set(1.0f,0.0f,0.002f);
		pPar.m_moveData.reSet();
		pPar.m_isExit = true;
		getWindowPos(pPar.m_windowPosX,pPar.m_windowPosY);
		getWindowSize(pPar.m_windowSizeW,pPar.m_windowSizeH);
	}else
	{
		exitWindow();
	}
}
bool _GGame::init()
{
	setVSync(false);
	getWindowPos(m_windowPosX,m_windowPosY);
	getWindowSize(m_windowSizeW,m_windowSizeH);
	m_moveData.set(0.0f,1.0f,0.002f);
	XEE::customWinTitle.setCallbackFun(windowExitFun,NULL,this);
	m_isFirst = true;
	m_isExit = false;

	m_tab.initWithoutTex(_XVector2(1014.0f,725.0f));
	m_tab.addATab("发送端口设置");
	m_tab.addATab("发送数据设置");
	m_tab.addATab("键盘鼠标模拟");
	m_tab.addATab("接收端设置");
	m_tab.addATab("系统设置");
	m_tab.setPosition(5.0f,5.0f);

	int h = 5 + 40;
	m_textIP.init();
	m_textIP.setPosition(10.0f,h);
	m_textIP.setString("IP:");
	m_textIPEdt.initWithoutTex(_XVector2(256.0f,32.0f),"192.168.0.1");
	m_textIPEdt.setPosition(60.0f,h);
	m_textIPEdt.setComment("输入OSC发送端的IP地址，一般为本机IP，也可以输入127.0.0.1");
	m_tab.addObjToTab(&m_textIP,"发送端口设置");
	m_tab.addObjToTab(&m_textIPEdt,"发送端口设置");

	m_textPort.init();
	m_textPort.setPosition(320.0f,h);
	m_textPort.setString("Port:");
	m_textPortEdt.initWithoutTex(_XVector2(128.0f,32.0f),"12345");
	m_textPortEdt.setPosition(400.0f,h);
	m_textPortEdt.setComment("输入OSC发送端的端口号，端口号需要与接收端的端口号一致，默认为12345");
	m_tab.addObjToTab(&m_textPort,"发送端口设置");
	m_tab.addObjToTab(&m_textPortEdt,"发送端口设置");

	m_openBtn.initWithoutTex("打开",_XVector2(128.0f,32.0f));
	m_openBtn.setPosition(532.0f,h);
	m_openBtn.setMouseDownCB(ctrlFun,this);
	m_openBtn.enable();
	m_openBtn.setWithAction(XTrue);
	m_openBtn.setComment("打开或者关闭OSC发送器，确认IP地址和端口正确，程序不检测其正确性");
//	m_closeBtn.initWithoutTex("关闭",XEE::systemFont,_XRect(0.0f,0.0f,128.0f,32.0f));
//	m_closeBtn.setPosition(664.0f,h);
//	m_closeBtn.setMouseDownCB(ctrlFun,this);
//	m_closeBtn.disable();
//	m_closeBtn.setWithAction(XTrue);
	m_tab.addObjToTab(&m_openBtn,"发送端口设置");
//	m_tab.addObjToTab(&m_closeBtn,"发送端口设置");

	h = 5 + 40;
	m_addBtn.initWithoutTex("添加",_XVector2(70.0f,32.0f));
	m_addBtn.setPosition(10.0f,h);
	m_addBtn.setMouseDownCB(ctrlFun,this);
	m_addBtn.setComment("将左边编辑好的OSC数据信息添加到右边的列表中以便于之后操作");
	m_delBtn.initWithoutTex("删除",_XVector2(70.0f,32.0f));
	m_delBtn.setPosition(84.0f,h);
	m_delBtn.setMouseDownCB(ctrlFun,this);
	m_delBtn.setComment("删除左边列表中选中的数据");
	m_clearBtn.initWithoutTex("清空",_XVector2(70.0f,32.0f));
	m_clearBtn.setPosition(158.0f,h);
	m_clearBtn.setMouseDownCB(ctrlFun,this);
	m_clearBtn.setComment("清空右边列表框中的所有数据");
	m_modifyBtn.initWithoutTex("修改",_XVector2(70.0f,32.0f));
	m_modifyBtn.setPosition(232.0f,h);
	m_modifyBtn.setMouseDownCB(ctrlFun,this);
	m_modifyBtn.setComment("从右边列表中选择一行数据并修改，按下该按钮将修改后的数据更新到列表中");
	m_tab.addObjToTab(&m_addBtn,"发送数据设置");
	m_tab.addObjToTab(&m_delBtn,"发送数据设置");
	m_tab.addObjToTab(&m_clearBtn,"发送数据设置");
	m_tab.addObjToTab(&m_modifyBtn,"发送数据设置");

	m_dataLst.initWithoutTex(_XVector2(709.0f,670.0f),3,0);
	m_dataLst.setPosition(306.0f,h);
	m_dataLst.setTitleStr("Address",0);
	m_dataLst.setRowWidth(250,0);
	m_dataLst.setTitleStr("Data",1);
	m_dataLst.setRowWidth(250,1);
	m_dataLst.setTitleStr("Type",2);
	m_dataLst.setRowWidth(250,2);
	m_dataLst.setWithMouseDrag(false);
	m_dataLst.setSelectFun(ctrlFun,this);
	m_dataLst.setDClickFun(ctrlFunX,this);
	m_tab.addObjToTab(&m_dataLst,"发送数据设置");

	h += 35;
	m_textData.init();
	m_textData.setPosition(10.0f,h);
	m_textData.setString("Data:");
	m_textDataEdt.initWithoutTex(_XVector2(204.0f,32.0f),"1234567");
	m_textDataEdt.setPosition(98.0f,h);
	m_textDataEdt.setComment("OSC消息中对应的数据，请确保输入的数据与下面的数据类型匹配");
	m_tab.addObjToTab(&m_textData,"发送数据设置");
	m_tab.addObjToTab(&m_textDataEdt,"发送数据设置");

	h += 35;
	m_textType.init();
	m_textType.setPosition(10.0f,h);
	m_textType.setString("Type:");
	m_textDataCmb.initWithoutTex(170,5,3);
	m_textDataCmb.setMenuStr("Int32",0);
	m_textDataCmb.setMenuStr("Int64",1);
	m_textDataCmb.setMenuStr("Float",2);
	m_textDataCmb.setMenuStr("String",3);
	m_textDataCmb.setMenuStr("Blob",4);
	m_textDataCmb.setPosition(98.0f,h);
	m_textDataCmb.setComment("OSC消息中数据的数据类型，请确保数据与数据类型匹配");
	m_tab.addObjToTab(&m_textType,"发送数据设置");
	m_tab.addObjToTab(&m_textDataCmb,"发送数据设置");
	h += 37;
	m_textAddress.init();
	m_textAddress.setPosition(10.0f,h);
	m_textAddress.setString("Address:");
	m_textAddressEdt.initWithoutTex(_XVector2(162.0f,32.0f),"/data");
	m_textAddressEdt.setPosition(140.0f,h);
	m_textAddressEdt.setComment("OSC消息中地址");
	m_tab.addObjToTab(&m_textAddress,"发送数据设置");
	m_tab.addObjToTab(&m_textAddressEdt,"发送数据设置");
	h += 35;
	m_textTimes.init();
	m_textTimes.setPosition(10.0f,h);
	m_textTimes.setString("Times:");
	m_textTimesEdt.initWithoutTex(_XVector2(162.0f,32.0f),"1");
	m_textTimesEdt.setPosition(140.0f,h);
	m_textTimesEdt.setComment("OSC消息需要循环发送的次数，启动发送将会根据这个数值来循环发送");
	m_tab.addObjToTab(&m_textTimes,"发送数据设置");
	m_tab.addObjToTab(&m_textTimesEdt,"发送数据设置");
	h += 35;
	m_textSpacing.init();
	m_textSpacing.setPosition(10.0f,h);
	m_textSpacing.setString("Spacing:");
	m_textSpacingEdt.initWithoutTex(_XVector2(162.0f,32.0f),"1");
	m_textSpacingEdt.setPosition(140.0f,h);
	m_textSpacingEdt.setComment("两次发送之间的间隔时间，单位为毫秒(ms)，1毫秒为千分之一秒");
	m_tab.addObjToTab(&m_textSpacing,"发送数据设置");
	m_tab.addObjToTab(&m_textSpacingEdt,"发送数据设置");
	h += 35;
	m_sendModeCmb.initWithoutTex(258,4,3);
	m_sendModeCmb.setMenuStr("发送选中地址",0);
	m_sendModeCmb.setMenuStr("发送选中数据",1);
	m_sendModeCmb.setMenuStr("发送空数据",2);
	m_sendModeCmb.setMenuStr("发送当前数据",3);
	m_sendModeCmb.setPosition(10.0f,h);
	m_sendModeCmb.setNowChooseOrder(1);	//默认为发送选中数据
	m_sendModeCmb.setComment("选择需要的发送模式，共有四种发送模式，如有疑问，请先阅读说明文档");
	m_tab.addObjToTab(&m_sendModeCmb,"发送数据设置");
	h += 37;
	m_sendAsMsg.initWithoutTex("作为消息发送",_XVector2(32.0f,32.0f));
	m_sendAsMsg.setPosition(10.0f,h);
	m_sendAsMsg.setComment("勾选时需要发送的数据作为一条消息发送，否则作为一个数据包发送");
	m_tab.addObjToTab(&m_sendAsMsg,"发送数据设置");
	h += 35;
	m_sendBtn.initWithoutTex("开始发送",_XVector2(292.0f,32.0f));
	m_sendBtn.setPosition(10.0f,h);
	m_sendBtn.setMouseDownCB(ctrlFun,this);
	m_sendBtn.setComment("根据设定发送数据或停止发送，发送前请先确保OSC发送器已经正确打开");
//	m_stopBtn.initWithoutTex("停止",XEE::systemFont,_XRect(0.0f,0.0f,128.0f,32.0f));
//	m_stopBtn.setPosition(142.0f,h);
//	m_stopBtn.setMouseDownCB(ctrlFun,this);
	m_tab.addObjToTab(&m_sendBtn,"发送数据设置");
//	m_tab.addObjToTab(&m_stopBtn,"发送数据设置");
	h = 5 + 40;
	m_recvText.initWithoutTex(_XVector2(740.0f,670.0f),"Recv:");
	m_recvText.setPosition(275.0f,h);
//	m_textRIP.setACopy(m_textIP);
//	m_textRIP.setPosition(10.0f,h.0f);
//	m_textRIP.setString("IP:");
//	m_textRIPEdt.initWithoutTex(_XRect(0.0f,0.0f,256.0f,32.0f),"192.168.0.1",XEE::systemFont);
//	m_textRIPEdt.setPosition(100.0f,h.0f);
	m_textRPort.init();
	m_textRPort.setPosition(10.0f,h);
	m_textRPort.setString("Port:");
	m_textRPortEdt.initWithoutTex(_XVector2(172.0f,32.0f),"12345");
	m_textRPortEdt.setPosition(98.0f,h);
	m_textRPortEdt.setComment("OSC接收器的端口号，必须与相应发送器的端口号一致才能正确接收，默认为12345");
	m_tab.addObjToTab(&m_recvText,"接收端设置");
	m_tab.addObjToTab(&m_textRPort,"接收端设置");
	m_tab.addObjToTab(&m_textRPortEdt,"接收端设置");
	h += 35;
	m_openRBtn.initWithoutTex("开始接收",_XVector2(260.0f,32.0f));
	m_openRBtn.setPosition(10.0f,h);
	m_openRBtn.setMouseDownCB(ctrlFun,this);
	m_openRBtn.enable();
	m_openRBtn.setComment("打开或关闭OSC接收器，请确保接收器端口号输入正确");
//	m_closeRBtn.initWithoutTex("关闭",XEE::systemFont,_XRect(0.0f,0.0f,128.0f,32.0f));
//	m_closeRBtn.setPosition(142.0f,h);
//	m_closeRBtn.setMouseDownCB(ctrlFun,this);
//	m_closeRBtn.disable();
	m_tab.addObjToTab(&m_openRBtn,"接收端设置");
//	m_tab.addObjToTab(&m_closeRBtn,"接收端设置");
	h += 35;
	m_supportACKMode.initWithoutTex("是否问答机制",_XVector2(32.0f,32.0f));
	m_supportACKMode.setPosition(10.0f,h);
	m_supportACKMode.setComment("应答模式，开启时，工具收到某地址的空数据时将会自动返回该地址的数据");
	m_tab.addObjToTab(&m_supportACKMode,"接收端设置");
	h += 35;
	m_clearRecvBtn.initWithoutTex("清除接收框",_XVector2(260.0f,32.0f));
	m_clearRecvBtn.setPosition(10.0f,h);
	m_clearRecvBtn.setMouseDownCB(ctrlFun,this);
	m_clearRecvBtn.setComment("按下该按钮清空接收列表，接收列表中的所有数据将会清空");
	m_tab.addObjToTab(&m_clearRecvBtn,"接收端设置");

	h = 5 + 40;
	m_saveBtn.initWithoutTex("保存",_XVector2(128.0f,32.0f));
	m_saveBtn.setPosition(10.0f,h);
	m_saveBtn.setMouseDownCB(ctrlFun,this);
	m_saveBtn.setComment("将当前界面上的所有数据保存到文件中");
	m_readBtn.initWithoutTex("读取",_XVector2(128.0f,32.0f));
	m_readBtn.setPosition(142.0f,h);
	m_readBtn.setMouseDownCB(ctrlFun,this);
	m_readBtn.setComment("从配置文件中读取上一次保存的配置数据");
	m_tab.addObjToTab(&m_saveBtn,"系统设置");
	m_tab.addObjToTab(&m_readBtn,"系统设置");
	//下面是键盘鼠标映射
	h = 5 + 40;
	m_dataKMLst.initWithoutTex(_XVector2(709.0f,670.0f),5,0);
	m_dataKMLst.setPosition(306.0f,h);
	m_dataKMLst.setTitleStr("Key",0);
	m_dataKMLst.setRowWidth(100,0);
	m_dataKMLst.setTitleStr("State",1);
	m_dataKMLst.setRowWidth(100,1);
	m_dataKMLst.setTitleStr("Address",2);
	m_dataKMLst.setRowWidth(200,2);
	m_dataKMLst.setTitleStr("Data",3);
	m_dataKMLst.setRowWidth(200,3);
	m_dataKMLst.setTitleStr("Type",4);
	m_dataKMLst.setRowWidth(200,4);
	m_dataKMLst.setWithMouseDrag(false);
	m_dataKMLst.setSelectFun(ctrlFun,this);
	m_dataKMLst.setDClickFun(ctrlFunX,this);
	m_tab.addObjToTab(&m_dataKMLst,"键盘鼠标模拟");
	m_addKMBtn.initWithoutTex("添加",_XVector2(70.0f,32.0f));
	m_addKMBtn.setPosition(10.0f,h);
	m_addKMBtn.setMouseDownCB(ctrlFun,this);
	m_addKMBtn.setComment("将左边编辑好的鼠标键盘与OSC的映射添加到映射列表中");
	m_delKMBtn.initWithoutTex("删除",_XVector2(70.0f,32.0f));
	m_delKMBtn.setPosition(84.0f,h);
	m_delKMBtn.setMouseDownCB(ctrlFun,this);
	m_delKMBtn.setComment("删除映射列表中选择的行");
	m_clearKMBtn.initWithoutTex("清空",_XVector2(70.0f,32.0f));
	m_clearKMBtn.setPosition(158.0f,h);
	m_clearKMBtn.setMouseDownCB(ctrlFun,this);
	m_clearKMBtn.setComment("清空左边的映射列表");
	m_modifyKMBtn.initWithoutTex("修改",_XVector2(70.0f,32.0f));
	m_modifyKMBtn.setPosition(232.0f,h);
	m_modifyKMBtn.setMouseDownCB(ctrlFun,this);
	m_modifyKMBtn.setComment("将左边编辑好的值更新到映射列表中选中的行");
	m_tab.addObjToTab(&m_addKMBtn,"键盘鼠标模拟");
	m_tab.addObjToTab(&m_delKMBtn,"键盘鼠标模拟");
	m_tab.addObjToTab(&m_clearKMBtn,"键盘鼠标模拟");
	m_tab.addObjToTab(&m_modifyKMBtn,"键盘鼠标模拟");
	h += 35;
	m_keySelectCmb.initWithoutTex(115,41,10);
	m_keySelectCmb.setMenuStr("Key_0",0);
	m_keySelectCmb.setMenuStr("Key_1",1);
	m_keySelectCmb.setMenuStr("Key_2",2);
	m_keySelectCmb.setMenuStr("Key_3",3);
	m_keySelectCmb.setMenuStr("Key_4",4);
	m_keySelectCmb.setMenuStr("Key_5",5);
	m_keySelectCmb.setMenuStr("Key_6",6);
	m_keySelectCmb.setMenuStr("Key_7",7);
	m_keySelectCmb.setMenuStr("Key_8",8);
	m_keySelectCmb.setMenuStr("Key_9",9);
	m_keySelectCmb.setMenuStr("Key_A",10);
	m_keySelectCmb.setMenuStr("Key_B",11);
	m_keySelectCmb.setMenuStr("Key_C",12);
	m_keySelectCmb.setMenuStr("Key_D",13);
	m_keySelectCmb.setMenuStr("Key_E",14);
	m_keySelectCmb.setMenuStr("Key_F",15);
	m_keySelectCmb.setMenuStr("Key_G",16);
	m_keySelectCmb.setMenuStr("Key_H",17);
	m_keySelectCmb.setMenuStr("Key_I",18);
	m_keySelectCmb.setMenuStr("Key_J",19);
	m_keySelectCmb.setMenuStr("Key_K",20);
	m_keySelectCmb.setMenuStr("Key_L",21);
	m_keySelectCmb.setMenuStr("Key_M",22);
	m_keySelectCmb.setMenuStr("Key_N",23);
	m_keySelectCmb.setMenuStr("Key_O",24);
	m_keySelectCmb.setMenuStr("Key_P",25);
	m_keySelectCmb.setMenuStr("Key_Q",26);
	m_keySelectCmb.setMenuStr("Key_R",27);
	m_keySelectCmb.setMenuStr("Key_S",28);
	m_keySelectCmb.setMenuStr("Key_T",29);
	m_keySelectCmb.setMenuStr("Key_U",30);
	m_keySelectCmb.setMenuStr("Key_V",31);
	m_keySelectCmb.setMenuStr("Key_W",32);
	m_keySelectCmb.setMenuStr("Key_X",33);
	m_keySelectCmb.setMenuStr("Key_Y",34);
	m_keySelectCmb.setMenuStr("Key_Z",35);
	m_keySelectCmb.setMenuStr("Mus_L",36);
	m_keySelectCmb.setMenuStr("Mus_M",37);
	m_keySelectCmb.setMenuStr("Mus_R",38);
	m_keySelectCmb.setMenuStr("Mus_X",39);
	m_keySelectCmb.setMenuStr("Mus_Y",40);
	m_keySelectCmb.setPosition(10.0f,h);
	m_keySelectCmb.setComment("对应的鼠标或者键盘按键");
	m_keyStateCmb.initWithoutTex(105,2,2);
	m_keyStateCmb.setMenuStr("Down",0);
	m_keyStateCmb.setMenuStr("Up",1);
	m_keyStateCmb.setPosition(163.0f,h);
	m_keyStateCmb.setComment("按键的状态");
	m_tab.addObjToTab(&m_keySelectCmb,"键盘鼠标模拟");
	m_tab.addObjToTab(&m_keyStateCmb,"键盘鼠标模拟");
	h += 37;
	m_oscDataTxt.init();
	m_oscDataTxt.setPosition(10.0f,h);
	m_oscDataTxt.setString("Data:");
	m_oscDataEdt.initWithoutTex(_XVector2(204.0f,32.0f),"1234567");
	m_oscDataEdt.setPosition(98.0f,h);
	m_oscDataEdt.setComment("OSC数据");
	m_tab.addObjToTab(&m_oscDataTxt,"键盘鼠标模拟");
	m_tab.addObjToTab(&m_oscDataEdt,"键盘鼠标模拟");
	h += 35;
	m_oscType.init();
	m_oscType.setPosition(10.0f,h);
	m_oscType.setString("Type:");
	m_oscDataCmb.initWithoutTex(170,5,3);
	m_oscDataCmb.setMenuStr("Int32",0);
	m_oscDataCmb.setMenuStr("Int64",1);
	m_oscDataCmb.setMenuStr("Float",2);
	m_oscDataCmb.setMenuStr("String",3);
	m_oscDataCmb.setMenuStr("Blob",4);
	m_oscDataCmb.setPosition(98.0f,h);
	m_oscDataCmb.setComment("OSC数据类型");
	m_tab.addObjToTab(&m_oscType,"键盘鼠标模拟");
	m_tab.addObjToTab(&m_oscDataCmb,"键盘鼠标模拟");
	h += 37;
	m_oscAddress.init();
	m_oscAddress.setPosition(10.0f,h);
	m_oscAddress.setString("Address:");
	m_oscAddressEdt.initWithoutTex(_XVector2(162.0f,32.0f),"/data");
	m_oscAddressEdt.setPosition(140.0f,h);
	m_oscAddressEdt.setComment("OSC地址");
	m_tab.addObjToTab(&m_oscAddress,"键盘鼠标模拟");
	m_tab.addObjToTab(&m_oscAddressEdt,"键盘鼠标模拟");

	m_allMsgText.setACopy(XEE::systemFont);
	m_allMsgText.setString("系统消息");
	m_allMsgText.setPosition(512.0f,748.0f);
	m_allMsgText.setMaxStrLen(512);

	readFromFile();
	LogStr("初始化完成");
	return true;
}
void _GGame::sendNowData(int mode)
{//下面是发送,这里需要修改为发送指定地址的数据
	ofxOscMessage m;
	if(mode == 0)
	{//发送指定地址的数据
		m.setAddress(m_nowSendAddr);
		LogNull("发送一条数据-地址:%s",m_nowSendAddr.c_str());
		for(int i = 0;i < m_datasVector.size();++ i)
		{
			if(m_datasVector[i]->address == m_nowSendAddr)
			{//依次发送指定地址的数据
				switch(m_datasVector[i]->dataType)
				{
				case 0:	//int32
					m.addIntArg(atoi(m_datasVector[i]->data.c_str()));
					break;
				case 1:	//int64
					m.addInt64Arg(atoi(m_datasVector[i]->data.c_str()));
					break;
				case 2:	//float
					m.addFloatArg(atof(m_datasVector[i]->data.c_str()));
					break;
				case 3:	//string
					m.addStringArg(m_datasVector[i]->data);
					break;
				case 4:	//blob尚无支持
					break;
				}
			}
		}
	}else
	if(mode == 1)
	{//发送单条选中数据
		m.setAddress(m_nowSendAddr);
		int i = m_dataLst.getSelectIndex();
		LogNull("发送一条数据-地址:%s %d",m_nowSendAddr.c_str(),i);
		if(i >= 0)
		{
			switch(m_datasVector[i]->dataType)
			{
			case 0:	//int32
				m.addIntArg(atoi(m_datasVector[i]->data.c_str()));
				break;
			case 1:	//int64
				m.addInt64Arg(atoi(m_datasVector[i]->data.c_str()));
				break;
			case 2:	//float
				m.addFloatArg(atof(m_datasVector[i]->data.c_str()));
				break;
			case 3:	//string
				m.addStringArg(m_datasVector[i]->data);
				break;
			case 4:	//blob尚无支持
				break;
			}
		}
	}else
	if(mode == 2)
	{//发送空数据
		m.setAddress(m_nowSendAddr);
		LogNull("发送一条空数据-地址:%s",m_nowSendAddr.c_str());
	}else
	{//发送当前数据
		m.setAddress(m_textAddressEdt.getString());
		switch(m_textDataCmb.getNowChooseOrder())
		{
		case 0:	//int32
			m.addIntArg(atoi(m_textDataEdt.getString()));
			break;
		case 1:	//int64
			m.addInt64Arg(atoi(m_textDataEdt.getString()));
			break;
		case 2:	//float
			m.addFloatArg(atof(m_textDataEdt.getString()));
			break;
		case 3:	//string
			m.addStringArg(m_textDataEdt.getString());
			break;
		case 4:	//blob尚无支持
			break;
		}
	}

	if(m_sendAsMsg.getState()) m_sender.sendMessage(m);
	else
	{
		ofxOscBundle b;
		b.addMessage(m);
		m_sender.sendBundle(b);
	}
}
void _GGame::move(int stepTime)
{
	if(!m_moveData.getIsEnd())
	{
		if(m_isFirst) m_moveData.move(0);
		else m_moveData.move(stepTime);
		float tmpData = m_moveData.getNowData();
		if(XEE::windowData.withFrame == 0)
		{
			setWindowPos(m_windowPosX,
				m_windowPosY + m_windowSizeH * 0.5f * (1.0f - tmpData));
			setWindowSize(m_windowSizeW,m_windowSizeH * tmpData);
			if(m_isExit && m_moveData.getIsEnd()) exitWindow();
		}
	}
	if(m_isFirst) m_isFirst = false;
	ctrlStateManager();
	if(m_recvState)
	{//开启接收
		while(m_receiver.hasWaitingMessages())
		{
			ofxOscMessage m;
			m_receiver.getNextMessage(&m);
			//将消息推入到显示中
			++m_recvSum;
			std::string recvData = "";
			char tmpStr[1024];
			if(isUTF8(m.getAddress()))
			{
				//printf("UTF8\n");
				sprintf(tmpStr,"%d-Addr:%s ",m_recvSum,UTF82ANSI(m.getAddress().c_str()).c_str());
			}else
			{
				sprintf(tmpStr,"%d-Addr:%s ",m_recvSum,m.getAddress().c_str());
			}
			recvData += tmpStr;
			LogNull("接收到一条数据-地址:%s",tmpStr);
			for(int i = 0; i < m.getNumArgs(); i++)
			{//get the argument type
				recvData += "|Type:" + m.getArgTypeName(i);
				recvData += ":";
				switch(m.getArgType(i))
				{
				case OFXOSC_TYPE_INT32:
					recvData += toString(m.getArgAsInt32(i));
					break;
				case OFXOSC_TYPE_INT64:
					recvData += toString(m.getArgAsInt64(i));
					break;
				case OFXOSC_TYPE_FLOAT:
					recvData += toString(m.getArgAsFloat(i));
					break;
				case OFXOSC_TYPE_STRING:
					if(isUTF8(m.getArgAsString(i)))
					{
						//printf("UTF8\n");
						recvData += toString(UTF82ANSI(m.getArgAsString(i).c_str()));
					}else
					{
						recvData += toString(m.getArgAsString(i));
					}
					break;
				default:
					recvData += "unknown";
					break;
				}
			}
			m_recvText.addALine(recvData.c_str());
			if(m_supportACKMode.getState())
			{//如果支持问答机制的话，这里需要检查是否需要问答
				if(strcmp(m.getAddress().c_str(),m_textAddressEdt.getString()) == 0
					&& m.getNumArgs() == 0)
				{//地址是对应的
					sendNowData(0);
				}
			}
		}
	}
	if(m_isSendState)
	{//开启发送
		m_timer += stepTime;
		if(m_timer > m_setTimer)
		{//发送一次
			m_timer = 0;
			sendNowData(m_sendModeCmb.getNowChooseOrder());

			++m_times;
			if(m_times >= m_setTimes) sendOverProc();//发送完成
		}
	}
	if(m_mouseOnTimer < 10000) m_mouseOnTimer += stepTime;
	ctrlMsgProc();
	//下面根据hook的数据发送数据
	if(m_isSenderOpen && m_keyMapOscList.size() > 0)
	{
		_XInputEvent e;
		if(popAInputEvent(e))
		{
			switch(e.type)
			{
			case EVENT_KEYBOARD:
				{
					int index = keyValueToIndex(e.keyValue);
					if(index >= 100) break;
					int state = 0;
					if(e.keyState == KEY_STATE_UP) state = 1;
					for(int i = 0;i < m_keyMapOscList.size();++ i)
					{
						if(m_keyMapOscList[i]->keyValue == index && 
							m_keyMapOscList[i]->keyState == state)
						{//发送这条数据
							sendKMData(i);
						}
					}
				}
				break;
			case EVENT_MOUSE:
				{
					int index = 100;
					int state = 0;
					switch(e.mouseState)
					{
					case MOUSE_LEFT_BUTTON_DOWN:
						index = 36;state = 0;
						break;
					case MOUSE_LEFT_BUTTON_UP:
						index = 36;state = 1;
						break;
					case MOUSE_MIDDLE_BUTTON_DOWN:
						index = 37;state = 0;
						break;
					case MOUSE_MIDDLE_BUTTON_UP:
						index = 37;state = 1;
						break;
					case MOUSE_RIGHT_BUTTON_DOWN:
						index = 38;state = 0;
						break;
					case MOUSE_RIGHT_BUTTON_UP:
						index = 38;state = 1;
						break;
					}
					if(index >= 100 && e.mouseState != MOUSE_MOVE) break;
					for(int i = 0;i < m_keyMapOscList.size();++ i)
					{
						if(m_keyMapOscList[i]->keyValue == index && 
							m_keyMapOscList[i]->keyState == state)
						{//发送这条数据
							sendKMData(i);
						}else
						if(m_keyMapOscList[i]->keyValue == 39 && m_mouseX != e.mouseX && e.mouseX <= 512
							&& m_mouseX != (e.mouseX >> 1))
						{
							m_mouseX = (e.mouseX >> 1);
							sendKMData(i);
						}else
						if(m_keyMapOscList[i]->keyValue == 40 && m_mouseY != e.mouseY && e.mouseY <= 512
							&& m_mouseY != (e.mouseY >> 1))
						{
							m_mouseY = (e.mouseY >> 1);
							sendKMData(i);
						}
					}
				}
				break;
			}
		}
	}
}
int _GGame::keyValueToIndex(_XKeyValue k)
{
	switch(k)
	{
	case XKEY_0: return 0;
	case XKEY_1: return 1;
	case XKEY_2: return 2;
	case XKEY_3: return 3;
	case XKEY_4: return 4;
	case XKEY_5: return 5;
	case XKEY_6: return 6;
	case XKEY_7: return 7;
	case XKEY_8: return 8;
	case XKEY_9: return 9;
	case XKEY_A: return 10;
	case XKEY_B: return 11;
	case XKEY_C: return 12;
	case XKEY_D: return 13;
	case XKEY_E: return 14;
	case XKEY_F: return 15;
	case XKEY_G: return 16;
	case XKEY_H: return 17;
	case XKEY_I: return 18;
	case XKEY_J: return 19;
	case XKEY_K: return 20;
	case XKEY_L: return 21;
	case XKEY_M: return 22;
	case XKEY_N: return 23;
	case XKEY_O: return 24;
	case XKEY_P: return 25;
	case XKEY_Q: return 26;
	case XKEY_R: return 27;
	case XKEY_S: return 28;
	case XKEY_T: return 29;
	case XKEY_U: return 30;
	case XKEY_V: return 31;
	case XKEY_W: return 32;
	case XKEY_X: return 33;
	case XKEY_Y: return 34;
	case XKEY_Z: return 35;
	default:
		return 100;
	}
}
void _GGame::ctrlMsgProc()
{
	if(!m_isShowMsg && m_mouseOnTimer > 2000)
	{
		if(m_textIPEdt.getVisible() && m_textIPEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//IP
			m_allMsgText.setString("输入OSC发送端的IP地址，一般为本机IP，也可以输入127.0.0.1");
		}else
		if(m_textPortEdt.getVisible() && m_textPortEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//Port
			m_allMsgText.setString("输入OSC发送端的端口号，端口号需要与接收端的端口号一致，默认为12345");
		}else
		if(m_openBtn.getVisible() && m_openBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//打开按钮
			m_allMsgText.setString("打开或者关闭OSC发送器，确认IP地址和端口正确，程序不检测其正确性");
		}else
	//	if(m_closeBtn.getVisible() && m_closeBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
	//	{//关闭按钮
	//		m_allMsgText.setString("关闭OSC发送器，必须先打开才能关闭");
	//	}else
		if(m_sendAsMsg.getVisible() && m_sendAsMsg.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//作为消息发送
			m_allMsgText.setString("勾选时需要发送的数据作为一条消息发送，否则作为一个数据包发送.");
		}else
		if(m_addBtn.getVisible() && m_addBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//添加
			m_allMsgText.setString("将左边编辑好的OSC数据信息添加到右边的列表中以便于之后操作。");
		}else
		if(m_delBtn.getVisible() && m_delBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//删除
			m_allMsgText.setString("删除左边列表中选中的数据。");
		}else
		if(m_clearBtn.getVisible() && m_clearBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//清空
			m_allMsgText.setString("清空右边列表框中的所有数据。");
		}else
		if(m_modifyBtn.getVisible() && m_modifyBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//修改
			m_allMsgText.setString("从右边列表中选择一行数据并修改，按下该按钮将修改后的数据更新到列表中。");
		}else
		if(m_textDataEdt.getVisible() && m_textDataEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//data
			m_allMsgText.setString("OSC消息中对应的数据，请确保输入的数据与下面的数据类型匹配。");
		}else
		if(m_textDataCmb.getVisible() && m_textDataCmb.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//type
			m_allMsgText.setString("OSC消息中数据的数据类型，请确保数据与数据类型匹配。");
		}else
		if(m_textAddressEdt.getVisible() && m_textAddressEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//addr
			m_allMsgText.setString("OSC消息中地址。");
		}else
		if(m_textTimesEdt.getVisible() && m_textTimesEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//Times
			m_allMsgText.setString("OSC消息需要循环发送的次数，启动发送将会根据这个数值来循环发送。");
		}else
		if(m_textSpacingEdt.getVisible() && m_textSpacingEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//space
			m_allMsgText.setString("两次发送之间的间隔时间，单位为毫秒(ms)，1毫秒为千分之一秒。");
		}else
		if(m_sendBtn.getVisible() && m_sendBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//send
			m_allMsgText.setString("根据设定发送数据或停止发送，发送前请先确保OSC发送器已经正确打开。");
		}else
//		if(m_stopBtn.getVisible() && m_stopBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//stop
//			m_allMsgText.setString("按下该按钮停止发送。");
//		}else
		if(m_sendModeCmb.getVisible() && m_sendModeCmb.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//发送模式
			m_allMsgText.setString("选择需要的发送模式，共有四种发送模式，如有疑问，请先阅读说明文档。");
		}else
		if(m_dataLst.getVisible() && m_dataLst.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//数据列表
			m_allMsgText.setString("数据列表，可以双击相应数据行进行发送，请确保已经正确打开OSC发送器。");
		}else
		if(m_supportACKMode.getVisible() && m_supportACKMode.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//应答模式
			m_allMsgText.setString("应答模式，开启时，工具收到某地址的空数据时将会自动返回该地址的数据。");
		}else
		if(m_clearRecvBtn.getVisible() && m_clearRecvBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//清空列表
			m_allMsgText.setString("按下该按钮清空接收列表，接收列表中的所有数据将会清空。");
		}else
		if(m_textRPortEdt.getVisible() && m_textRPortEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//接收端口
			m_allMsgText.setString("OSC接收器的端口号，必须与相应发送器的端口号一致才能正确接收，默认为12345");
		}else
		if(m_openRBtn.getVisible() && m_openRBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//打开
			m_allMsgText.setString("打开或关闭OSC接收器，请确保接收器端口号输入正确。");
		}else
//		if(m_closeRBtn.getVisible() && m_closeRBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//关闭
//			m_allMsgText.setString("关闭OSC接收器。");
//		}else
		if(m_saveBtn.getVisible() && m_saveBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//保存
			m_allMsgText.setString("将当前界面上的所有数据保存到config.xml文件中。");
		}else
		if(m_readBtn.getVisible() && m_readBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//读取
			m_allMsgText.setString("从配置文件config.xml中读取上一次保存的配置数据。");
		}else
		if(m_recvText.getVisible() && m_recvText.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//接收列表框
			m_allMsgText.setString("接收的数据将显示于此，选择最末行可以保持光标于末行。");
		}else
		if(m_addKMBtn.getVisible() && m_addKMBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//添加
			m_allMsgText.setString("将左边编辑好的鼠标键盘与OSC的映射添加到映射列表中。");
		}else
		if(m_delKMBtn.getVisible() && m_delKMBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//删除
			m_allMsgText.setString("删除映射列表中选择的行。");
		}else
		if(m_clearKMBtn.getVisible() && m_clearKMBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//清空
			m_allMsgText.setString("清空左边的映射列表。");
		}else
		if(m_modifyKMBtn.getVisible() && m_modifyKMBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//修改
			m_allMsgText.setString("将左边编辑好的值更新到映射列表中选中的行。");
		}else
		if(m_keySelectCmb.getVisible() && m_keySelectCmb.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//键值
			m_allMsgText.setString("对应的鼠标或者键盘按键。");
		}else
		if(m_keyStateCmb.getVisible() && m_keyStateCmb.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//按键状态
			m_allMsgText.setString("按键的状态。");
		}else
		if(m_oscDataEdt.getVisible() && m_oscDataEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//osc数据编辑
			m_allMsgText.setString("OSC数据。");
		}else
		if(m_oscDataCmb.getVisible() && m_oscDataCmb.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//osc数据编辑
			m_allMsgText.setString("OSC数据类型。");
		}else
		if(m_oscAddressEdt.getVisible() && m_oscAddressEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//osc地址
			m_allMsgText.setString("OSC地址。");
		}else
		if(m_dataKMLst.getVisible() && m_dataKMLst.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//鼠标键盘和osc的映射列表
			m_allMsgText.setString("鼠标键盘和osc的映射列表。");
		}
	}
}
void _GGame::draw()
{
//	m_textIP.draw();
//	m_textPort.draw();
//	m_textData.draw();
//	m_textType.draw();
//	m_textAddress.draw();
//	m_textTimes.draw();
//	m_textSpacing.draw();
//	drawLine(10,375,1016,375);
//	m_textRIP.draw();
//	m_textRPort.draw();
	m_allMsgText.draw();
}
void _GGame::input(const _XInputEvent &event)
{
	if(event.type == EVENT_MOUSE)
	{
		m_mouseOnTimer = 0;
		m_isShowMsg = false;
	}
	if(event.type == EVENT_KEYBOARD && 
		event.keyState == KEY_STATE_DOWN &&
		event.keyValue == XKEY_ESCAPE)
	{
		windowExitFun(this);
	}
}
void _GGame::release()
{
	for(int i = 0;i < m_datasVector.size();++ i)
	{
		XDELETE(m_datasVector[i]);
	}
	m_datasVector.clear();
	for(int i = 0;i < m_keyMapOscList.size();++ i)
	{
		XDELETE(m_keyMapOscList[i]);
	}
	stopHook();
	m_keyMapOscList.clear();
}
bool _GGame::saveToFile(const char *filename)
{
	//TiXmlDocument doc(CFG_FILENAME);
	if(filename != NULL && filename[0] == '\0') return false;
	TiXmlDocument doc;
//	if(filename == NULL) doc.crea(CFG_FILENAME);
//	else doc.LoadFile(filename);
	TiXmlDeclaration declaration("1.0","gb2312","yes"); 
	doc.InsertEndChild(declaration);
	TiXmlElement elmRoot("root");

	if(!addLeafNode(elmRoot,"sendIP",m_textIPEdt.getString())) return false;
	if(!addLeafNode(elmRoot,"sendPort",m_textPortEdt.getString())) return false;
	if(!addLeafNode(elmRoot,"sendAsMessage",m_sendAsMsg.getState())) return false;
	if(!addLeafNode(elmRoot,"dataE",m_textDataEdt.getString())) return false;
	if(!addLeafNode(elmRoot,"typeE",toString(m_textDataCmb.getNowChooseOrder()))) return false;
	if(!addLeafNode(elmRoot,"addressE",m_textAddressEdt.getString())) return false;
	if(!addLeafNode(elmRoot,"timeE",m_textTimesEdt.getString())) return false;
	if(!addLeafNode(elmRoot,"spaceE",m_textSpacingEdt.getString())) return false;
	if(!addLeafNode(elmRoot,"sendMode",toString(m_sendModeCmb.getNowChooseOrder()))) return false;

	for(int i = 0;i < m_datasVector.size();++ i)
	{
		TiXmlElement elmNode("data"); 
		if(!addLeafNode(elmNode,"addr",m_datasVector[i]->address)) return false;
		if(!addLeafNode(elmNode,"datas",m_datasVector[i]->data)) return false;
		if(!addLeafNode(elmNode,"type",toString(m_datasVector[i]->dataType))) return false;
		if(elmRoot.InsertEndChild(elmNode) == NULL) return false;  
	}
	if(!addLeafNode(elmRoot,"keyValueE",toString(m_keySelectCmb.getNowChooseOrder()))) return false;
	if(!addLeafNode(elmRoot,"keyStateE",toString(m_keyStateCmb.getNowChooseOrder()))) return false;
	if(!addLeafNode(elmRoot,"oscDataE",m_oscDataEdt.getString())) return false;
	if(!addLeafNode(elmRoot,"oscTypeE",toString(m_oscDataCmb.getNowChooseOrder()))) return false;
	if(!addLeafNode(elmRoot,"oscAddrE",m_oscAddressEdt.getString())) return false;
	for(int i = 0;i < m_keyMapOscList.size();++ i)
	{
		TiXmlElement elmNode("map"); 
		if(!addLeafNode(elmNode,"keyValue",toString(m_keyMapOscList[i]->keyValue))) return false;
		if(!addLeafNode(elmNode,"keyState",toString(m_keyMapOscList[i]->keyState))) return false;
		if(!addLeafNode(elmNode,"addr",m_keyMapOscList[i]->address)) return false;
		if(!addLeafNode(elmNode,"datas",m_keyMapOscList[i]->data)) return false;
		if(!addLeafNode(elmNode,"type",toString(m_keyMapOscList[i]->dataType))) return false;
		if(elmRoot.InsertEndChild(elmNode) == NULL) return false;  
	}
	if(!addLeafNode(elmRoot,"recvPort",m_textRPortEdt.getString())) return false;
	if(!addLeafNode(elmRoot,"ACKMode",m_supportACKMode.getState())) return false;

	if(doc.InsertEndChild(elmRoot) == NULL) return false;
	bool ret;
	if(filename == NULL) ret = doc.SaveFile(CFG_FILENAME);
	else ret = doc.SaveFile(filename);
	if(!ret) return ret;
	//doc.SaveFile();
	m_allMsgText.setString("文件保存完成!");
	return true;
}
bool _GGame::readFromFile(const char *filename)
{
	TiXmlBase::SetCondenseWhiteSpace(false);
	if(filename != NULL && filename[0] == '\0') return false;
	std::string tmp;
	bool tmpB;
	int tmpI;
	TiXmlNode *keyNode = NULL;
	//TiXmlDocument doc(CFG_FILENAME);
	TiXmlDocument doc;
	if(filename == NULL) doc.LoadFile(CFG_FILENAME);
	else doc.LoadFile(filename);

	if(!doc.LoadFile()) return false;
	TiXmlNode *rootNode = NULL;
	rootNode = doc.FirstChild("root");
	if(rootNode == NULL) return false;
	if(getXmlAsString(rootNode,"sendIP",tmp))
		m_textIPEdt.setString(tmp.c_str());
	if(getXmlAsString(rootNode,"sendPort",tmp))
		m_textPortEdt.setString(tmp.c_str());
	if(getXmlAsBool(rootNode,"sendAsMessage",tmpB))
		m_sendAsMsg.setState(tmpB);
	if(getXmlAsString(rootNode,"dataE",tmp))
		m_textDataEdt.setString(tmp.c_str());
	if(getXmlAsInt(rootNode,"typeE",tmpI))
		m_textDataCmb.setNowChooseOrder(tmpI);
	if(getXmlAsString(rootNode,"addressE",tmp))
		m_textAddressEdt.setString(tmp.c_str());
	if(getXmlAsString(rootNode,"timeE",tmp))
		m_textTimesEdt.setString(tmp.c_str());
	if(getXmlAsString(rootNode,"spaceE",tmp))
		m_textSpacingEdt.setString(tmp.c_str());
	if(getXmlAsInt(rootNode,"sendMode",tmpI))
		m_sendModeCmb.setNowChooseOrder(tmpI);
	//下面读取数据
	for(int i = 0;i < m_datasVector.size();++ i)
	{
		XDELETE(m_datasVector[i]);
	}
	m_datasVector.clear();
	m_dataLst.setLineSum(0);
	keyNode = rootNode->FirstChildElement("data");
	while(1)
	{
		if(keyNode == NULL) break;
		_GOscData *tmp = createMem<_GOscData>();
		getXmlAsString(keyNode,"addr",tmp->address);
		getXmlAsString(keyNode,"datas",tmp->data);
		getXmlAsInt(keyNode,"type",tmp->dataType);
		m_datasVector.push_back(tmp);
		addALineData(tmp);

		keyNode = keyNode->NextSiblingElement("data");
	}

	if(getXmlAsInt(rootNode,"keyValueE",tmpI))
		m_keySelectCmb.setNowChooseOrder(tmpI);
	if(getXmlAsInt(rootNode,"keyStateE",tmpI))
		m_keyStateCmb.setNowChooseOrder(tmpI);
	if(getXmlAsString(rootNode,"oscDataE",tmp))
		m_oscDataEdt.setString(tmp.c_str());
	if(getXmlAsInt(rootNode,"oscTypeE",tmpI))
		m_oscDataCmb.setNowChooseOrder(tmpI);
	if(getXmlAsString(rootNode,"oscAddrE",tmp))
		m_oscAddressEdt.setString(tmp.c_str());
	for(int i = 0;i < m_keyMapOscList.size();++ i)
	{
		XDELETE(m_keyMapOscList[i]);
	}
	m_keyMapOscList.clear();
	m_dataKMLst.setLineSum(0);
	keyNode = rootNode->FirstChildElement("map");
	while(1)
	{
		if(keyNode == NULL) break;
		_GKMOscData *tmp = createMem<_GKMOscData>();
		getXmlAsInt(keyNode,"keyValue",tmp->keyValue);
		getXmlAsInt(keyNode,"keyState",tmp->keyState);
		getXmlAsString(keyNode,"addr",tmp->address);
		getXmlAsString(keyNode,"datas",tmp->data);
		getXmlAsInt(keyNode,"type",tmp->dataType);
		m_keyMapOscList.push_back(tmp);
		addALineData(tmp);
		keyNode = keyNode->NextSiblingElement("map");
	}
	//读取其他数据
	if(getXmlAsString(rootNode,"recvPort",tmp))
		m_textRPortEdt.setString(tmp.c_str());
	if(getXmlAsBool(rootNode,"ACKMode",tmpB))
		m_supportACKMode.setState(tmpB);
	return true;
}
void _GGame::addALineData(const _GOscData *tmp)
{
	int lineSum = m_dataLst.getLineSum();
	m_dataLst.addALine();
	setLineData(tmp,lineSum);
}
void _GGame::setLineData(const _GOscData *tmp,int index)
{
	m_dataLst.setBoxStr(tmp->address.c_str(),index,0);
	m_dataLst.setBoxStr(tmp->data.c_str(),index,1);
	m_dataLst.setBoxStr(oscDataTypeToStr(tmp->dataType).c_str(),index,2);
}
void _GGame::addALineData(const _GKMOscData *tmp)
{
	int lineSum = m_dataKMLst.getLineSum();
	m_dataKMLst.addALine();
	setLineData(tmp,lineSum);
}
void _GGame::setLineData(const _GKMOscData *tmp,int index)
{
	m_dataKMLst.setBoxStr(keyValueToStr(tmp->keyValue).c_str(),index,0);
	switch(tmp->keyState)
	{
	case 0:
		m_dataKMLst.setBoxStr("Down",index,1);
		break;
	default:
		m_dataKMLst.setBoxStr("Up",index,1);
		break;
	}
	m_dataKMLst.setBoxStr(tmp->address.c_str(),index,2);
	if(tmp->keyValue == 39 || tmp->keyValue == 40) 
		m_dataKMLst.setBoxStr("0 - 255",index,3); 
	else
		m_dataKMLst.setBoxStr(tmp->data.c_str(),index,3);
	m_dataKMLst.setBoxStr(oscDataTypeToStr(tmp->dataType).c_str(),index,4);
}
std::string _GGame::keyValueToStr(int keyValue)
{
	switch(keyValue)
	{
	case 0:return "KEY_0";
	case 1:return "KEY_1";
	case 2:return "KEY_2";
	case 3:return "KEY_3";
	case 4:return "KEY_4";
	case 5:return "KEY_5";
	case 6:return "KEY_6";
	case 7:return "KEY_7";
	case 8:return "KEY_8";
	case 9:return "KEY_9";
	case 10:return "KEY_A";
	case 11:return "KEY_B";
	case 12:return "KEY_C";
	case 13:return "KEY_D";
	case 14:return "KEY_E";
	case 15:return "KEY_F";
	case 16:return "KEY_G";
	case 17:return "KEY_H";
	case 18:return "KEY_I";
	case 19:return "KEY_J";
	case 20:return "KEY_K";
	case 21:return "KEY_L";
	case 22:return "KEY_M";
	case 23:return "KEY_N";
	case 24:return "KEY_O";
	case 25:return "KEY_P";
	case 26:return "KEY_Q";
	case 27:return "KEY_R";
	case 28:return "KEY_S";
	case 29:return "KEY_T";
	case 30:return "KEY_U";
	case 31:return "KEY_V";
	case 32:return "KEY_W";
	case 33:return "KEY_X";
	case 34:return "KEY_Y";
	case 35:return "KEY_Z";
	case 36:return "Mouse_LeftBtn";
	case 37:return "Mouse_MiddleBtn";
	case 38:return "Mouse_RightBtn";
	case 39:return "Mouse_X";
	case 40:return "Mouse_Y";
	default:
		return "";
	}
}
std::string _GGame::oscDataTypeToStr(int type)
{
	switch(type)
	{
	case 0: return "Int32";
	case 1: return "Int64";
	case 2: return "Float";
	case 3: return "String";
	case 4: return "Blob";
	default:return "";
	}
}
void _GGame::ctrlStateManager()
{
	if(m_recvText.getLineSum() > 1) m_clearRecvBtn.enable();
	else m_clearRecvBtn.disable();
	if(m_sendBtn.getEnable())
	{
		if(m_dataLst.getLineSum() <= 0) m_clearBtn.disable();
		else m_clearBtn.enable();
		if(m_dataLst.getSelectIndex() < 0)
		{
			m_delBtn.disable();
			m_modifyBtn.disable();
		}else
		{
			m_delBtn.enable();
			m_modifyBtn.enable();
		}
		if(m_dataKMLst.getLineSum() <= 0) m_clearKMBtn.disable();
		else m_clearKMBtn.enable();
		if(m_dataKMLst.getSelectIndex() < 0)
		{
			m_delKMBtn.disable();
			m_modifyKMBtn.disable();
		}else
		{
			m_delKMBtn.enable();
			m_modifyKMBtn.enable();
		}
	}
}