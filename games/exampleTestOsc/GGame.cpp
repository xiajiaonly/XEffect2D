#include "GGame.h"
#include "XFile.h"
#include "systemHook.h"
#pragma comment(lib, "systemHook.lib")
using namespace XE;
void GGame::ctrlEventProc(int id,int eventID)
{
	if(eventID == XMultiList::MLTLST_DCLICK)
	{
		if(id == m_dataLst.getControlID())
		{//鼠标双击事件
			sendButtonDown();
			return;
		}
		if(id == m_dataKMLst.getControlID())
		{//鼠标双击事件，发送指定数据
			sendSelectData();
			return;
		}
	}
	if(id == m_openBtn.getControlID())
	{//打开发送
		if(eventID != XButton::BTN_MOUSE_DOWN) return;
		if(!XString::getIsUnsignedInt(m_textPortEdt.getString()))
		{
			setMassage("端口号必须为正整数.");
			return;
		}
		if(!XString::getIsValidIp(m_textIPEdt.getString())
			&& !XString::getIsLocalhost(m_textIPEdt.getString()))
		{
			setMassage("非法的IP地址!");
			return;
		}
		if(!m_isSenderOpen)
		{
			try
			{
				m_sender.setup(m_textIPEdt.getString(),atoi(m_textPortEdt.getString()));
			}catch(...)
			{
				if(getLanguageIndex() == INDEX_ENGLISH)
					setMassage("OSC open fail,Please check whether the port has been occupied.");
				else setMassage("osc打开失败,请检查osc端口是否已经被占用.");
				return;
			}
#ifdef WITH_OFXOSC
			m_sender.setSendAsMessage(true);
#endif
			m_isSenderOpen = true;
			if(getLanguageIndex() == INDEX_ENGLISH)
				setMassage("Opened,determine whether successful by yourself.");
			else setMassage("开启完成,是否成功请自己确定.");
		//	pPar.m_openBtn.disable();
		//	pPar.m_closeBtn.enable();
			if(getLanguageIndex() == 0) m_openBtn.setCaptionText("Close");
			else m_openBtn.setCaptionText("关闭");
			m_textIPEdt.disable();
			m_textPortEdt.disable();
			if(m_keyMapOscList.size() > 0) startHook();//开启hook
			m_mouseX = -1;
			m_mouseY = -1;
		}else
		{
#ifdef WITH_OFXOSC
			m_sender.setup("",0);
#else
			m_sender.release();
#endif
			m_isSenderOpen = false;
			if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Chosed.");
			else setMassage("关闭完成.");
		//	pPar.m_openBtn.enable();
		//	pPar.m_closeBtn.disable();
			if(getLanguageIndex() == INDEX_ENGLISH) m_openBtn.setCaptionText("Open");
			else m_openBtn.setCaptionText("打开");
			m_textIPEdt.enable();
			m_textPortEdt.enable();
			if(m_keyMapOscList.size() > 0) stopHook();//关闭hook
		}
		return;
	}
	if(id == m_addBtn.getControlID())
	{//添加
		if(eventID != XButton::BTN_MOUSE_DOWN) return;
		if(!checkNowData())
		{
			if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Data is not correct type.");
			else setMassage("数据类型与实际的数据不符.");
			return;
		}
		//添加数据
		GOscData *tmp = XMem::createMem<GOscData>();
		tmp->data = m_textDataEdt.getString();
		tmp->address = m_textAddressEdt.getString();
		tmp->dataType = m_textDataCmb.getCurChooseOrder();
		m_datasVector.push_back(tmp);
		//更新列表
		addALineData(tmp);
		if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Data added.");
		else setMassage("数据添加成功.");
		return;
	}
	if(id == m_delBtn.getControlID())
	{//删除
		if(eventID != XButton::BTN_MOUSE_DOWN) return;
		//删除数据
		int lineSum = m_dataLst.getSelectIndex();
		if(lineSum >= 0)
		{
			GOscData *tmp = m_datasVector[lineSum];
			for(int i = lineSum;i < (int)(m_datasVector.size()) - 1;++ i)
			{
				m_datasVector[i] = m_datasVector[i + 1];
			}
			m_datasVector.pop_back();
			XMem::XDELETE(tmp);
			m_dataLst.deleteLine(lineSum);
			if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Data deleted.");
			else setMassage("删除成功.");
		}else
		{
			if(getLanguageIndex() == INDEX_ENGLISH) setMassage("please choose the data to delete.");
			else setMassage("请选择要删除的数据.");
		}
		return;
	}
	if(id == m_sendBtn.getControlID())
	{//发送
		if(eventID != XButton::BTN_MOUSE_DOWN) return;
		if(m_isSendState) sendOverProc();
		else sendButtonDown();
		return;
	}
	if(id == m_openRBtn.getControlID())
	{//开启接收
		if(eventID != XButton::BTN_MOUSE_DOWN) return;
		if(m_recvState)
		{
			m_recvState = false;
#ifdef WITH_OFXOSC
			m_receiver.setup(0);
#else
			m_receiver.release();
#endif
			if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Closed receiver.");
			else setMassage("已关闭接收.");
		//	m_openRBtn.enable();
		//	m_closeRBtn.disable();
			m_openRBtn.setCaptionText(m_language.m_openRBtnCloseStr.c_str());
			m_textRPortEdt.enable();
			m_supportACKMode.enable();
		}else
		{
			try
			{
				m_receiver.setup(m_textRPortEdt.getAsInt());
			}catch(...)
			{
				if(getLanguageIndex() == INDEX_ENGLISH) setMassage("OSC open fail,请检查osc端口是否已经被占用.");
				else setMassage("osc接收打开失败,请检查osc端口是否已经被占用.");
				return;
			}
			m_recvState = true;
			if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Receiver opened,是否成功请自行判断.");
			else setMassage("已经开启接收,是否成功请自行判断.");
		//	m_openRBtn.disable();
		//	m_closeRBtn.enable();
			m_openRBtn.setCaptionText(m_language.m_openRBtnOpenStr.c_str());
			m_textRPortEdt.disable();
			m_supportACKMode.disable();
		}
		return;
	}
	if(id == m_clearRecvBtn.getControlID())
	{
		if(eventID != XButton::BTN_MOUSE_DOWN) return;
		m_recvText.setString("Recv:");
		if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Received data have cleared.");
		else setMassage("接收框数据已经清空.");
		return;
	}
	if(id == m_saveBtn.getControlID())
	{//保存
		if(eventID != XButton::BTN_MOUSE_UP) return;
		if(getLanguageIndex() == INDEX_ENGLISH) 
		{
			if(!saveToFile(XFile::getChooseFilename(CFG_FILENAME,"Save",false).c_str()))
				setMassage("Save failed.");
		}else
		{
			if(!saveToFile(XFile::getChooseFilename(CFG_FILENAME,"保存",false).c_str()))
				setMassage("写入配置文件失败.");
		}
		return;
	}
	if(id == m_readBtn.getControlID()) 
	{//读取
		if(eventID != XButton::BTN_MOUSE_UP) return;
		if(m_recvState || m_isSendState)
		{
			if(getLanguageIndex() == INDEX_ENGLISH) setMassage("It is forbidden to read when sender open or receiver open.");
			else setMassage("正在接受或者发送状态不能读取配置文件.");
			return;
		}
		if(getLanguageIndex() == INDEX_ENGLISH) 
		{
			if(!readFromFile(XFile::getChooseFilename(CFG_FILENAME,"Read",true).c_str()))
				setMassage("Read file failed.");
		}else
		{
			if(!readFromFile(XFile::getChooseFilename(CFG_FILENAME,"读取",true).c_str()))
				setMassage("读取配置文件失败.");
		}
		return;
	}
	if(id == m_dataLst.getControlID())
	{//选择跟新数据
		if(eventID != XMultiList::MLTLST_SELECT) return;
		int index = m_dataLst.getSelectIndex();
		if(index >= 0)
		{
			m_textDataEdt.setString(m_datasVector[index]->data.c_str());
			m_textDataCmb.setCurChooseOrder(m_datasVector[index]->dataType);
			m_textAddressEdt.setString(m_datasVector[index]->address.c_str());
		}
		return;
	}
	if(id == m_modifyBtn.getControlID())
	{//修改
		if(eventID != XButton::BTN_MOUSE_DOWN) return;
		int index = m_dataLst.getSelectIndex();
		if(index >= 0 && checkNowData())
		{
			m_datasVector[index]->data = m_textDataEdt.getString();
			m_datasVector[index]->dataType = m_textDataCmb.getCurChooseOrder();
			m_datasVector[index]->address = m_textAddressEdt.getString();
			setLineData(m_datasVector[index],index);
			if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Modifayed.");
			else setMassage("修改完成.");
		}else
		{
			if(getLanguageIndex() == INDEX_ENGLISH) setMassage("please choose the data to modify.");
			else setMassage("请选择需要修改的数据.");
		}
		return;
	}
	if(id == m_clearBtn.getControlID())
	{//清空
		if(eventID != XButton::BTN_MOUSE_DOWN) return;
		if(m_datasVector.size() <= 0)
		{
			if(getLanguageIndex() == INDEX_ENGLISH) setMassage("There is no data to clear.");
			else setMassage("列表中没有数据.");
			return;
		}
		m_dataLst.setLineSum(0);
		for(int i = 0;i < m_datasVector.size();++ i)
		{
			XMem::XDELETE(m_datasVector[i]);
		}
		m_datasVector.clear();
		if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Cleared.");
		else setMassage("清空完成.");
		return;
	}
	if(id == m_addKMBtn.getControlID())
	{//添加
		if(eventID != XButton::BTN_MOUSE_DOWN) return;
		if(!checkKMData())
		{
			if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Dta is invaid or already exist.");
			else setMassage("数据不合法或者已经存在.");
			return;
		}
		//添加数据
		GKMOscData *tmp = XMem::createMem<GKMOscData>();
		tmp->keyValue = m_keySelectCmb.getCurChooseOrder();
		tmp->keyState = m_keyStateCmb.getCurChooseOrder();
		if(tmp->keyValue == 39 || tmp->keyValue == 40)
		{
			tmp->data = "0 - 255";
			tmp->address = m_oscAddressEdt.getString();
			tmp->dataType = 0;
		}else
		{
			tmp->data = m_oscDataEdt.getString();
			tmp->address = m_oscAddressEdt.getString();
			tmp->dataType = m_oscDataCmb.getCurChooseOrder();
		}
		m_keyMapOscList.push_back(tmp);
		//更新列表
		addALineData(tmp);
		if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Data added.");
		else setMassage("数据添加成功.");
		return;
	}
	if(id == m_delKMBtn.getControlID())
	{//删除
		if(eventID != XButton::BTN_MOUSE_DOWN) return;
		//删除数据
		int lineSum = m_dataKMLst.getSelectIndex();
		if(lineSum >= 0)
		{
			GKMOscData *tmp = m_keyMapOscList[lineSum];
			for(int i = lineSum;i < (int)(m_keyMapOscList.size()) - 1;++ i)
			{
				m_keyMapOscList[i] = m_keyMapOscList[i + 1];
			}
			m_keyMapOscList.pop_back();
			XMem::XDELETE(tmp);
			m_dataKMLst.deleteLine(lineSum);
			if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Deleted.");
			else setMassage("删除成功.");
		}else
		{
			if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Please choose the data to clear.");
			else setMassage("请选择要删除的数据.");
		}
		return;
	}
	if(id == m_clearKMBtn.getControlID())
	{//清空
		if(eventID != XButton::BTN_MOUSE_DOWN) return;
		if(m_keyMapOscList.size() <= 0)
		{
			if(getLanguageIndex() == INDEX_ENGLISH) setMassage("There is no data to clear.");
			else setMassage("列表中没有数据.");
			return;
		}
		m_dataKMLst.setLineSum(0);
		for(int i = 0;i < m_keyMapOscList.size();++ i)
		{
			XMem::XDELETE(m_keyMapOscList[i]);
		}
		m_keyMapOscList.clear();
		if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Data cleared.");
		else setMassage("清空完成.");
		return;
	}
	if(id == m_modifyKMBtn.getControlID())
	{//修改
		if(eventID != XButton::BTN_MOUSE_DOWN) return;
		int index = m_dataKMLst.getSelectIndex();
		if(index >= 0 && checkKMData(index))
		{
			m_keyMapOscList[index]->keyValue = m_keySelectCmb.getCurChooseOrder();
			m_keyMapOscList[index]->keyState = m_keyStateCmb.getCurChooseOrder();
			m_keyMapOscList[index]->data = m_oscDataEdt.getString();
			m_keyMapOscList[index]->dataType = m_oscDataCmb.getCurChooseOrder();
			m_keyMapOscList[index]->address = m_oscAddressEdt.getString();
			setLineData(m_keyMapOscList[index],index);
			if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Data modifyed.");
			else setMassage("修改完成.");
		}else
		{
			if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Please choose the data to modify.");
			else setMassage("请选择需要修改的数据.");
		}
		return;
	}
	if(id == m_dataKMLst.getControlID())
	{//选择跟新数据
		if(eventID != XMultiList::MLTLST_SELECT) return;
		int index = m_dataKMLst.getSelectIndex();
		if(index >= 0)
		{
			m_keySelectCmb.setCurChooseOrder(m_keyMapOscList[index]->keyValue);
			m_keyStateCmb.setCurChooseOrder(m_keyMapOscList[index]->keyState);
			m_oscDataEdt.setString(m_keyMapOscList[index]->data.c_str());
			m_oscDataCmb.setCurChooseOrder(m_keyMapOscList[index]->dataType);
			m_oscAddressEdt.setString(m_keyMapOscList[index]->address.c_str());
		}
		return;
	}
	if(id == m_languageCmb.getControlID())
	{
		if(eventID == XCombo::CMB_DATA_CHANGE) updateLanguage();
		return;
	}
}
XBool GGame::checkNowData()
{
	switch(m_textDataCmb.getCurChooseOrder())
	{
	case 0:	//int32
	case 1:	//int64
	case 2:	//float
		if(!XString::getIsNumber(m_textDataEdt.getString())) return XFalse;
		break;
	case 3:	//string
		break;
	case 4:	//blob	//尚未支持
		break;
	}
	return XTrue;
}
XBool GGame::checkKMData(int index)
{
	for(int i = 0;i < m_keyMapOscList.size();++ i)
	{
		if(m_keyMapOscList[i]->keyValue == m_keySelectCmb.getCurChooseOrder() &&
			m_keyMapOscList[i]->keyState == m_keyStateCmb.getCurChooseOrder() && 
			i != index) return XFalse;	//相同的设定不能存在
	}
	if(m_keySelectCmb.getCurChooseOrder() != 39 &&
		m_keySelectCmb.getCurChooseOrder() != 40)
	{
		switch(m_oscDataCmb.getCurChooseOrder())
		{
		case 0:	//int32
		case 1:	//int64
		case 2:	//float
			if(!XString::getIsNumber(m_oscDataEdt.getString())) return XFalse;
			break;
		case 3:	//string
			break;
		case 4:	//blob	//尚未支持
			break;
		}
	}
	return XTrue;
}
void GGame::sendOverProc()
{
	m_textTimesEdt.setString(XString::toString(m_setTimes).c_str());
	m_isSendState = false;
	if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Stoped.");
	else setMassage("已停止.");
	//这里将相关的控件使能(尚未完成)
	//m_textIPEdt.enable();
	//m_textPortEdt.enable();
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
	m_sendBtn.setCaptionText(m_language.m_sendBtnOpenStr.c_str());
//	m_stopBtn.enable();	//不做非常严格的逻辑
	m_sendAsMsg.enable();
	m_sendModeCmb.enable();
}
void GGame::sendSelectData()
{
	if(!m_isSenderOpen)
	{
		if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Please open sender at first.");
		else setMassage("网络尚未打开,请先打开网络.");
		return;
	}
	//下面发送选中数据
	int index = m_dataKMLst.getSelectIndex();
	if(index >= 0) sendKMData(index);
}
void GGame::sendKMData(int index)
{
#ifdef WITH_OFXOSC
	ofxOscMessage m;
#else
	oscpkt::Message m;
#endif
	if(m_codeModeCmb.getCurChooseOrder() == 1)
	{
#ifdef WITH_OFXOSC
		m.setAddress(XString::ANSI2UTF8(m_keyMapOscList[index]->address));
#else
		m.init(XString::ANSI2UTF8(m_keyMapOscList[index]->address));
#endif
		if(getLanguageIndex() == INDEX_ENGLISH) 
			LogNull("Send a data - address:%s %d",XString::ANSI2UTF8(m_keyMapOscList[index]->address).c_str(),index);
		else 
			LogNull("发送一条数据-地址:%s %d",XString::ANSI2UTF8(m_keyMapOscList[index]->address).c_str(),index);
	}else
	{
#ifdef WITH_OFXOSC
		m.setAddress(m_keyMapOscList[index]->address);
#else
		m.init(m_keyMapOscList[index]->address);
#endif
		if(getLanguageIndex() == INDEX_ENGLISH) 
			LogNull("Send a data - address:%s %d",m_keyMapOscList[index]->address.c_str(),index);
		else 
			LogNull("发送一条数据-地址:%s %d",m_keyMapOscList[index]->address.c_str(),index);
	}
	if(m_keyMapOscList[index]->keyValue == 39)
	{
#ifdef WITH_OFXOSC
		m.addIntArg(m_mouseX);
#else
		m.pushInt32(m_mouseX);
#endif
	}else
	if(m_keyMapOscList[index]->keyValue == 40)
	{
#ifdef WITH_OFXOSC
		m.addIntArg(m_mouseY);
#else
		m.pushInt32(m_mouseY);
#endif
	}else
	{
		switch(m_keyMapOscList[index]->dataType)
		{
		case 0:	//int32
#ifdef WITH_OFXOSC
			m.addIntArg(atoi(m_keyMapOscList[index]->data.c_str()));
#else
			m.pushInt32(atoi(m_keyMapOscList[index]->data.c_str()));
#endif
			break;
		case 1:	//int64
#ifdef WITH_OFXOSC
			m.addInt64Arg(atoi(m_keyMapOscList[index]->data.c_str()));
#else
			m.pushInt64(atoi(m_keyMapOscList[index]->data.c_str()));
#endif
			break;
		case 2:	//float
#ifdef WITH_OFXOSC
			m.addFloatArg(atof(m_keyMapOscList[index]->data.c_str()));
#else
			m.pushFloat(atof(m_keyMapOscList[index]->data.c_str()));
#endif
			break;
		case 3:	//string
#ifdef WITH_OFXOSC
			if(m_codeModeCmb.getCurChooseOrder() == 1)
				m.addStringArg(XString::ANSI2UTF8(m_keyMapOscList[index]->data));
			else
				m.addStringArg(m_keyMapOscList[index]->data);
#else
			if(m_codeModeCmb.getCurChooseOrder() == 1)
				m.pushStr(XString::ANSI2UTF8(m_keyMapOscList[index]->data));
			else
				m.pushStr(m_keyMapOscList[index]->data);
#endif
			break;
		case 4:	//blob尚无支持
			break;
		}
	}
#ifdef WITH_OFXOSC
	if(m_sendAsMsg.getState()) m_sender.sendMessage(m);
	else
	{
		ofxOscBundle b;
		b.addMessage(m);
		m_sender.sendBundle(b);
	}
#else
	m_sender.sendMessage(m);
#endif
}
void GGame::sendButtonDown()
{
	if(!m_isSenderOpen)
	{
		if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Please open sender at first.");
		else setMassage("网络尚未打开,请先打开网络.");
		return;
	}
	if(m_isSendState)
	{
		if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Data is sending,please waitting.");
		else setMassage("正在发送数据,请稍等.");
	}else
	{//下面开始发送数据
		if(m_sendModeCmb.getCurChooseOrder() == CHOOSE_ADDR
			|| m_sendModeCmb.getCurChooseOrder() == CHOOSE_DATA)
		{
			int tmp = m_dataLst.getSelectIndex();
			if(tmp < 0)
			{
				if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Please choose a data to sender.");
				else setMassage("请选择一个需要发送的OSC地址.");
				return;
			}
			m_nowSendAddr = m_dataLst.getBoxStr(tmp,0);	//获取将要发送的OSC地址
		}
		if(m_sendModeCmb.getCurChooseOrder() == NULL_DATA)
		{//发送空数据
			int tmp = m_dataLst.getSelectIndex();
			if(tmp < 0)
			{
				if(m_textAddressEdt.getString() != NULL && strlen(m_textAddressEdt.getString()) > 0)
				{
					m_nowSendAddr = m_textAddressEdt.getString();
				}else
				{
					if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Please choose a OSC adress or input a one.");
					else setMassage("请选择或者输入需要发送的OSC地址.");
					return;
				}
			}else
			{
				m_nowSendAddr = m_dataLst.getBoxStr(tmp,0);	//获取将要发送的OSC地址
			}
		}
		if(m_sendModeCmb.getCurChooseOrder() == CURRENT_DATA && !checkNowData())
		{//如果是发送当前数据的话，检查当前数据是否合法
			if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Data is invalid.");
			else setMassage("当前数据不合法,请确保数据类型和数据匹配.");
			return;
		}
		m_setTimer = m_textSpacingEdt.getAsInt();
		m_setTimes = m_textTimesEdt.getAsInt();
		if(m_setTimer <= 0) m_setTimer = 0;
		if(m_setTimes <= 0) 
		{
			m_setTimes = 1;
			m_textTimesEdt.setString("1");
		}
		m_isSendState = true;
		m_timer = 0;
		m_times = 0;
		if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Started to send data.");
		else setMassage("已经开始发送数据.");
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
		m_sendBtn.setCaptionText(m_language.m_sendBtnCloseStr.c_str());
		m_sendAsMsg.disable();
		m_sendModeCmb.disable();
	}
}
void windowExitFun(void *p)
{
	GGame &pPar = *(GGame *)p;
	if(XEG.m_windowData.withFrame == 0)
	{//没有边框的时候才做这步动作
		pPar.m_moveData.set(1.0f,0.0f,0.002f);
		pPar.m_moveData.reset();
		pPar.m_isExit = true;
		XEG.getWindowPos(pPar.m_windowPosX,pPar.m_windowPosY);
		XEG.getWindowSize(pPar.m_windowSizeW,pPar.m_windowSizeH);
	}else
	{
		XEG.setGameExit();
	}
}
void GGame::createUI()
{
	m_tab.initWithoutSkin(XVec2(1014.0f,725.0f));
	m_tab.addATab("发送端口设置");
	m_tab.addATab("发送数据设置");
	m_tab.addATab("键盘鼠标模拟");
	m_tab.addATab("接收端设置");
	m_tab.addATab("系统设置");
	m_tab.setPosition(5.0f,5.0f);
	m_mouseRBtnMnu.initWithoutSkin(4,XRect(0,0,200,50),*XEG.m_systemFont,1.0f,XVec2(100,25)); 
	m_mouseRBtnMnu.setText("剪切(T)",0);
	m_mouseRBtnMnu.setText("复制(C)",1);
	m_mouseRBtnMnu.setText("粘贴(V)",2);
	m_mouseRBtnMnu.setText("撤销(S)",3);
	m_mouseRBtnMnu.setHotKey(XKEY_T,0);
	m_mouseRBtnMnu.setHotKey(XKEY_C,1);
	m_mouseRBtnMnu.setHotKey(XKEY_V,2);
	m_mouseRBtnMnu.setHotKey(XKEY_S,3);
	m_mouseRBtnMnu.setScale(0.5f);

	int h = 5 + 40;
	m_textIP.init("IP:");
	m_textIP.setPosition(10.0f,h);
	m_textIPEdt.initWithoutSkin(256.0f,"192.168.0.1",&m_mouseRBtnMnu);
	m_textIPEdt.setPosition(60.0f,h);
	m_tab.addObjToTab(&m_textIP,"发送端口设置");
	m_tab.addObjToTab(&m_textIPEdt,"发送端口设置");

	m_textPort.init("Port:");
	m_textPort.setPosition(320.0f,h);
	m_textPortEdt.initWithoutSkin(128.0f,"12345",&m_mouseRBtnMnu);
	m_textPortEdt.setPosition(400.0f,h);
	m_tab.addObjToTab(&m_textPort,"发送端口设置");
	m_tab.addObjToTab(&m_textPortEdt,"发送端口设置");

	m_openBtn.initWithoutSkin("打开",128.0f);
	m_openBtn.setPosition(532.0f,h);
	m_openBtn.enable();
	m_openBtn.setWithAction(XTrue);
	m_tab.addObjToTab(&m_openBtn,"发送端口设置");

	h = 5 + 40;
	m_addBtn.initWithoutSkin("添加",70.0f);
	m_addBtn.setPosition(10.0f,h);
	m_delBtn.initWithoutSkin("删除",70.0f);
	m_delBtn.setPosition(84.0f,h);
	m_clearBtn.initWithoutSkin("清空",70.0f);
	m_clearBtn.setPosition(158.0f,h);
	m_modifyBtn.initWithoutSkin("修改",70.0f);
	m_modifyBtn.setPosition(232.0f,h);
	m_tab.addObjToTab(&m_addBtn,"发送数据设置");
	m_tab.addObjToTab(&m_delBtn,"发送数据设置");
	m_tab.addObjToTab(&m_clearBtn,"发送数据设置");
	m_tab.addObjToTab(&m_modifyBtn,"发送数据设置");

	m_dataLst.initWithoutSkin(XVec2(709.0f,670.0f),3,0);
	m_dataLst.setPosition(306.0f,h);
	m_dataLst.setTitleStr("Address;Data;Type;");
	m_dataLst.setRowWidth(250,0);
	m_dataLst.setRowWidth(250,1);
	m_dataLst.setRowWidth(250,2);
	m_dataLst.setWithMouseDrag(false);
	m_tab.addObjToTab(&m_dataLst,"发送数据设置");

	h += 35;
	m_textData.init("Data:");
	m_textData.setPosition(10.0f,h);
	m_textDataEdt.initWithoutSkin(204.0f,"1234567",&m_mouseRBtnMnu);
	m_textDataEdt.setPosition(98.0f,h);
	m_tab.addObjToTab(&m_textData,"发送数据设置");
	m_tab.addObjToTab(&m_textDataEdt,"发送数据设置");

	h += 35;
	m_textType.init("Type:");
	m_textType.setPosition(10.0f,h);
	m_textDataCmb.initWithoutSkin(170,5,3);
	m_textDataCmb.setMenuStr("Int32;Int64;Float;String;Blob;");
	m_textDataCmb.setPosition(98.0f,h);
	m_tab.addObjToTab(&m_textType,"发送数据设置");
	m_tab.addObjToTab(&m_textDataCmb,"发送数据设置");
	h += 37;
	m_textAddress.init("Address:");
	m_textAddress.setPosition(10.0f,h);
	m_textAddressEdt.initWithoutSkin(162.0f,"/data",&m_mouseRBtnMnu);
	m_textAddressEdt.setPosition(140.0f,h);
	m_tab.addObjToTab(&m_textAddress,"发送数据设置");
	m_tab.addObjToTab(&m_textAddressEdt,"发送数据设置");
	h += 35;
	m_textTimes.init("Times:");
	m_textTimes.setPosition(10.0f,h);
	m_textTimesEdt.initWithoutSkin(162.0f,"1",&m_mouseRBtnMnu);
	m_textTimesEdt.setPosition(140.0f,h);
	m_tab.addObjToTab(&m_textTimes,"发送数据设置");
	m_tab.addObjToTab(&m_textTimesEdt,"发送数据设置");
	h += 35;
	m_textSpacing.init("Spacing:");
	m_textSpacing.setPosition(10.0f,h);
	m_textSpacingEdt.initWithoutSkin(162.0f,"1",&m_mouseRBtnMnu);
	m_textSpacingEdt.setPosition(140.0f,h);
	m_tab.addObjToTab(&m_textSpacing,"发送数据设置");
	m_tab.addObjToTab(&m_textSpacingEdt,"发送数据设置");
	h += 35;
	m_sendModeCmb.initWithoutSkin(258,5,3);
	m_sendModeCmb.setMenuStr("发送选中地址;发送选中数据;发送空数据;发送当前数据;发送所有数据;");
	m_sendModeCmb.setPosition(10.0f,h);
	m_sendModeCmb.setCurChooseOrder(1);	//默认为发送选中数据
	m_tab.addObjToTab(&m_sendModeCmb,"发送数据设置");
	h += 37;
	m_sendAsMsg.initWithoutSkin("作为消息发送");
	m_sendAsMsg.setPosition(10.0f,h);
	m_tab.addObjToTab(&m_sendAsMsg,"发送数据设置");
	h += 35;
	m_sendBtn.initWithoutSkin("开始发送",292.0f);
	m_sendBtn.setPosition(10.0f,h);
	m_tab.addObjToTab(&m_sendBtn,"发送数据设置");
	h = 5 + 40;
	m_recvText.initWithoutSkin(XVec2(740.0f,670.0f),"Recv:");
	m_recvText.setPosition(275.0f,h);
	m_textRPort.init("Port:");
	m_textRPort.setPosition(10.0f,h);
	m_textRPortEdt.initWithoutSkin(172.0f,"12345",&m_mouseRBtnMnu);
	m_textRPortEdt.setPosition(98.0f,h);
	m_tab.addObjToTab(&m_recvText,"接收端设置");
	m_tab.addObjToTab(&m_textRPort,"接收端设置");
	m_tab.addObjToTab(&m_textRPortEdt,"接收端设置");
	h += 35;
	m_openRBtn.initWithoutSkin("开始接收",260.0f);
	m_openRBtn.setPosition(10.0f,h);
	m_openRBtn.enable();
	m_tab.addObjToTab(&m_openRBtn,"接收端设置");
	h += 35;
	m_supportACKMode.initWithoutSkin("是否问答机制");
	m_supportACKMode.setPosition(10.0f,h);
	m_tab.addObjToTab(&m_supportACKMode,"接收端设置");
	h += 35;
	m_clearRecvBtn.initWithoutSkin("清除接收框",260.0f);
	m_clearRecvBtn.setPosition(10.0f,h);
	m_tab.addObjToTab(&m_clearRecvBtn,"接收端设置");

	h = 5 + 40;
	m_languageCmb.initWithoutSkin(150,2,2);
	m_languageCmb.setPosition(10.0f,h);
	m_languageCmb.setMenuStr("english;中文;");
	m_tab.addObjToTab(&m_languageCmb,"系统设置");
	h += 35;
	m_codeModeCmb.initWithoutSkin(150,2,2);
	m_codeModeCmb.setPosition(10.0f,h);
	m_codeModeCmb.setMenuStr("GDK;UTF8;");
	m_tab.addObjToTab(&m_codeModeCmb,"系统设置");
	h += 35;
	m_saveBtn.initWithoutSkin("保存",128.0f);
	m_saveBtn.setPosition(10.0f,h);
	m_readBtn.initWithoutSkin("读取",128.0f);
	m_readBtn.setPosition(142.0f,h);
	m_tab.addObjToTab(&m_saveBtn,"系统设置");
	m_tab.addObjToTab(&m_readBtn,"系统设置");
	//下面是键盘鼠标映射
	h = 5 + 40;
	m_dataKMLst.initWithoutSkin(XVec2(709.0f,670.0f),5,0);
	m_dataKMLst.setPosition(306.0f,h);
	m_dataKMLst.setTitleStr("Key;State;Address;Data;Type;");
	m_dataKMLst.setRowWidth(100,0);
	m_dataKMLst.setRowWidth(100,1);
	m_dataKMLst.setRowWidth(200,2);
	m_dataKMLst.setRowWidth(200,3);
	m_dataKMLst.setRowWidth(200,4);
	m_dataKMLst.setWithMouseDrag(false);
	m_tab.addObjToTab(&m_dataKMLst,"键盘鼠标模拟");
	m_addKMBtn.initWithoutSkin("添加",70.0f);
	m_addKMBtn.setPosition(10.0f,h);
	m_delKMBtn.initWithoutSkin("删除",70.0f);
	m_delKMBtn.setPosition(84.0f,h);
	m_clearKMBtn.initWithoutSkin("清空",70.0f);
	m_clearKMBtn.setPosition(158.0f,h);
	m_modifyKMBtn.initWithoutSkin("修改",70.0f);
	m_modifyKMBtn.setPosition(232.0f,h);
	m_tab.addObjToTab(&m_addKMBtn,"键盘鼠标模拟");
	m_tab.addObjToTab(&m_delKMBtn,"键盘鼠标模拟");
	m_tab.addObjToTab(&m_clearKMBtn,"键盘鼠标模拟");
	m_tab.addObjToTab(&m_modifyKMBtn,"键盘鼠标模拟");
	h += 35;
	m_keySelectCmb.initWithoutSkin(115,41,10);
	m_keySelectCmb.setMenuStr("Key_0;Key_1;Key_2;Key_3;Key_4;Key_5;Key_6;Key_7;Key_8;Key_9;Key_A;Key_B;Key_C;Key_D;Key_E;Key_F;Key_G;\
Key_H;Key_I;Key_J;Key_K;Key_L;Key_M;Key_N;Key_O;Key_P;Key_Q;Key_R;Key_S;Key_T;Key_U;Key_V;Key_W;Key_X;Key_Y;Key_Z;Mus_L;Mus_M;Mus_R;Mus_X;Mus_Y;");	//40
	m_keySelectCmb.setPosition(10.0f,h);
	m_keyStateCmb.initWithoutSkin(105,2,2);
	m_keyStateCmb.setMenuStr("Down;Up;");
	m_keyStateCmb.setPosition(163.0f,h);
	m_tab.addObjToTab(&m_keySelectCmb,"键盘鼠标模拟");
	m_tab.addObjToTab(&m_keyStateCmb,"键盘鼠标模拟");
	h += 37;
	m_oscDataTxt.init("Data:");
	m_oscDataTxt.setPosition(10.0f,h);
	m_oscDataEdt.initWithoutSkin(204.0f,"1234567",&m_mouseRBtnMnu);
	m_oscDataEdt.setPosition(98.0f,h);
	m_tab.addObjToTab(&m_oscDataTxt,"键盘鼠标模拟");
	m_tab.addObjToTab(&m_oscDataEdt,"键盘鼠标模拟");
	h += 35;
	m_oscType.init("Type:");
	m_oscType.setPosition(10.0f,h);
	m_oscDataCmb.initWithoutSkin(170,5,3);
	m_oscDataCmb.setMenuStr("Int32;Int64;Float;String;Blob;");
	m_oscDataCmb.setPosition(98.0f,h);
	m_tab.addObjToTab(&m_oscType,"键盘鼠标模拟");
	m_tab.addObjToTab(&m_oscDataCmb,"键盘鼠标模拟");
	h += 37;
	m_oscAddress.init("Address:");
	m_oscAddress.setPosition(10.0f,h);
	m_oscAddressEdt.initWithoutSkin(162.0f,"/data",&m_mouseRBtnMnu);
	m_oscAddressEdt.setPosition(140.0f,h);
	m_tab.addObjToTab(&m_oscAddress,"键盘鼠标模拟");
	m_tab.addObjToTab(&m_oscAddressEdt,"键盘鼠标模拟");

	m_allMsgText.setACopy(*XEG.m_systemFont);
	m_allMsgText.setPosition(512.0f,748.0f);
	m_allMsgText.setMaxStrLen(512);
}
bool GGame::init()
{
//	XGL::showGLInfoToLog();	//显示GL的相关版本信息
//	XEE::setStyle(XEE::COLOR_STYLE_LIGHT);
//	XEE::setVSync(false);
	XEG.getWindowPos(m_windowPosX,m_windowPosY);
	XEG.getWindowSize(m_windowSizeW,m_windowSizeH);
	if(XEG.m_windowData.withFrame == 0)
	{
		XEG.setWindowPos(m_windowPosX,
			m_windowPosY + m_windowSizeH * 0.5f);
		XEG.setWindowSize(m_windowSizeW,0.0f);
	}
	m_moveData.set(0.0f,1.0f,0.002f);//,MOVE_DATA_MODE_SHAKE);
	XEG.m_customWinTitle->setCallbackFun(windowExitFun,NULL,this);
	m_isFirst = true;
	m_isExit = false;

	createUI();
	readFromFile();

	updateLanguage();	//使得语言设计生效
	if(getLanguageIndex() == INDEX_ENGLISH) setMassage("System Infomation.");
	else setMassage("系统消息");
	LogStr("初始化完成");
	return true;
}
void GGame::sendNowData(int mode)
{//下面是发送,这里需要修改为发送指定地址的数据
#ifdef WITH_OFXOSC
	ofxOscMessage m;
#else
	oscpkt::Message m;
#endif
	switch(mode)
	{
	case CHOOSE_ADDR://发送指定地址的数据
		if(m_codeModeCmb.getCurChooseOrder() == 1)
#ifdef WITH_OFXOSC
			m.setAddress(XString::ANSI2UTF8(m_nowSendAddr));
		else
			m.setAddress(m_nowSendAddr);
#else
			m.init(XString::ANSI2UTF8(m_nowSendAddr));
		else
			m.init(m_nowSendAddr);
#endif
		if(getLanguageIndex() == INDEX_ENGLISH) LogNull("Send a data - Address:%s",m_nowSendAddr.c_str());
		else LogNull("发送一条数据-地址:%s",m_nowSendAddr.c_str());
		for(unsigned int i = 0;i < m_datasVector.size();++ i)
		{
			if(m_datasVector[i]->address == m_nowSendAddr)
			{//依次发送指定地址的数据
#ifdef WITH_OFXOSC
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
					if(m_codeModeCmb.getCurChooseOrder() == 1)
						m.addStringArg(XString::ANSI2UTF8(m_datasVector[i]->data));
					else
						m.addStringArg(m_datasVector[i]->data);
					break;
				case 4:	//blob尚无支持
					break;
				}
#else
				switch(m_datasVector[i]->dataType)
				{
				case 0:	//int32
					m.pushInt32(atoi(m_datasVector[i]->data.c_str()));
					break;
				case 1:	//int64
					m.pushInt64(atoi(m_datasVector[i]->data.c_str()));
					break;
				case 2:	//float
					m.pushFloat(atof(m_datasVector[i]->data.c_str()));
					break;
				case 3:	//string
					if(m_codeModeCmb.getCurChooseOrder() == 1)
						m.pushStr(XString::ANSI2UTF8(m_datasVector[i]->data));
					else
						m.pushStr(m_datasVector[i]->data);
					break;
				case 4:	//blob尚无支持
					break;
				}
#endif
			}
		}
		break;
	case CHOOSE_DATA:
		{//发送单条选中数据
			if(m_codeModeCmb.getCurChooseOrder() == 1)
#ifdef WITH_OFXOSC
				m.setAddress(XString::ANSI2UTF8(m_nowSendAddr));
			else
				m.setAddress(m_nowSendAddr);
#else
				m.init(XString::ANSI2UTF8(m_nowSendAddr));
			else
				m.init(m_nowSendAddr);
#endif
			int i = m_dataLst.getSelectIndex();
			if(getLanguageIndex() == INDEX_ENGLISH) LogNull("Send a data - Address:%s %d",m_nowSendAddr.c_str(),i);
			else LogNull("发送一条数据-地址:%s %d",m_nowSendAddr.c_str(),i);
			if(i >= 0)
			{
#ifdef WITH_OFXOSC
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
					if(m_codeModeCmb.getCurChooseOrder() == 1)
						m.addStringArg(XString::ANSI2UTF8(m_datasVector[i]->data));
					else
						m.addStringArg(m_datasVector[i]->data);
					break;
				case 4:	//blob尚无支持
					break;
				}
#else
				switch(m_datasVector[i]->dataType)
				{
				case 0:	//int32
					m.pushInt32(atoi(m_datasVector[i]->data.c_str()));
					break;
				case 1:	//int64
					m.pushInt64(atoi(m_datasVector[i]->data.c_str()));
					break;
				case 2:	//float
					m.pushFloat(atof(m_datasVector[i]->data.c_str()));
					break;
				case 3:	//string
					if(m_codeModeCmb.getCurChooseOrder() == 1)
						m.pushStr(XString::ANSI2UTF8(m_datasVector[i]->data));
					else
						m.pushStr(m_datasVector[i]->data);
					break;
				case 4:	//blob尚无支持
					break;
				}
#endif
			}
		}
		break;
	case NULL_DATA://发送空数据
		if(m_codeModeCmb.getCurChooseOrder() == 1)
#ifdef WITH_OFXOSC
			m.setAddress(XString::ANSI2UTF8(m_nowSendAddr));
		else
			m.setAddress(m_nowSendAddr);
#else
			m.init(XString::ANSI2UTF8(m_nowSendAddr));
		else
			m.init(m_nowSendAddr);
#endif
		if(getLanguageIndex() == INDEX_ENGLISH) LogNull("Send a blank data - Address:%s",m_nowSendAddr.c_str());
		else LogNull("发送一条空数据-地址:%s",m_nowSendAddr.c_str());
		break;
	case CURRENT_DATA://发送当前数据
		if(m_codeModeCmb.getCurChooseOrder() == 1)
#ifdef WITH_OFXOSC
			m.setAddress(XString::ANSI2UTF8(m_textAddressEdt.getString()));
		else
			m.setAddress(m_textAddressEdt.getString());
#else
			m.init(XString::ANSI2UTF8(m_textAddressEdt.getString()));
		else
			m.init(m_textAddressEdt.getString());
#endif
#ifdef WITH_OFXOSC
		switch(m_textDataCmb.getCurChooseOrder())
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
			if(m_codeModeCmb.getCurChooseOrder() == 1)
				m.addStringArg(XString::ANSI2UTF8(m_textDataEdt.getString()));
			else
				m.addStringArg(m_textDataEdt.getString());
			break;
		case 4:	//blob尚无支持
			break;
		}
#else
		switch(m_textDataCmb.getCurChooseOrder())
		{
		case 0:	//int32
			m.pushInt32(atoi(m_textDataEdt.getString()));
			break;
		case 1:	//int64
			m.pushInt64(atoi(m_textDataEdt.getString()));
			break;
		case 2:	//float
			m.pushFloat(atof(m_textDataEdt.getString()));
			break;
		case 3:	//string
			if(m_codeModeCmb.getCurChooseOrder() == 1)
				m.pushStr(XString::ANSI2UTF8(m_textDataEdt.getString()));
			else
				m.pushStr(m_textDataEdt.getString());
			break;
		case 4:	//blob尚无支持
			break;
		}
#endif
		break;
	default://发送列表中的所有数据
		for(unsigned int i = 0;i < m_datasVector.size();++ i)
		{
#ifdef WITH_OFXOSC
			ofxOscMessage tmpM;
#else
			oscpkt::Message tmpM;
#endif
			if(m_codeModeCmb.getCurChooseOrder() == 1)
#ifdef WITH_OFXOSC
				tmpM.setAddress(XString::ANSI2UTF8(m_datasVector[i]->address));
			else
				tmpM.setAddress(m_datasVector[i]->address);
#else
				tmpM.init(XString::ANSI2UTF8(m_datasVector[i]->address));
			else
				tmpM.init(m_datasVector[i]->address);
#endif
			//依次发送指定地址的数据
#ifdef WITH_OFXOSC
			switch(m_datasVector[i]->dataType)
			{
			case 0:	//int32
				tmpM.addIntArg(atoi(m_datasVector[i]->data.c_str()));
				break;
			case 1:	//int64
				tmpM.addInt64Arg(atoi(m_datasVector[i]->data.c_str()));
				break;
			case 2:	//float
				tmpM.addFloatArg(atof(m_datasVector[i]->data.c_str()));
				break;
			case 3:	//string
				if(m_codeModeCmb.getCurChooseOrder() == 1)
					tmpM.addStringArg(XString::ANSI2UTF8(m_datasVector[i]->data));
				else
					tmpM.addStringArg(m_datasVector[i]->data);
				break;
			case 4:	//blob尚无支持
				break;
			}
#else
			switch(m_datasVector[i]->dataType)
			{
			case 0:	//int32
				tmpM.pushInt32(atoi(m_datasVector[i]->data.c_str()));
				break;
			case 1:	//int64
				tmpM.pushInt64(atoi(m_datasVector[i]->data.c_str()));
				break;
			case 2:	//float
				tmpM.pushFloat(atof(m_datasVector[i]->data.c_str()));
				break;
			case 3:	//string
				if(m_codeModeCmb.getCurChooseOrder() == 1)
					tmpM.pushStr(XString::ANSI2UTF8(m_datasVector[i]->data));
				else
					tmpM.pushStr(m_datasVector[i]->data);
				break;
			case 4:	//blob尚无支持
				break;
			}
#endif
#ifdef WITH_OFXOSC
			if(m_sendAsMsg.getState()) m_sender.sendMessage(tmpM);
			else
			{
				ofxOscBundle b;
				b.addMessage(tmpM);
				m_sender.sendBundle(b);
			}
#else
			m_sender.sendMessage(tmpM);
#endif
		}
		return;	//发送完毕之后直接返回
	}

#ifdef WITH_OFXOSC
	if(m_sendAsMsg.getState()) m_sender.sendMessage(m);
	else
	{
		ofxOscBundle b;
		b.addMessage(m);
		m_sender.sendBundle(b);
	}
#else
	m_sender.sendMessage(m);
#endif
}
void GGame::move(float stepTime)
{
	if(!m_moveData.getIsEnd())
	{
		if(m_isFirst) m_moveData.move(0);
		else m_moveData.move(stepTime);
		float tmpData = m_moveData.getCurData();
		if(XEG.m_windowData.withFrame == 0)
		{
			XEG.setWindowPos(m_windowPosX,
				m_windowPosY + m_windowSizeH * 0.5f * (1.0f - tmpData));
			XEG.setWindowSize(m_windowSizeW,m_windowSizeH * tmpData);
			if(m_isExit && m_moveData.getIsEnd()) XEG.setGameExit();
		}
	}
	if(m_isFirst) m_isFirst = false;
	ctrlStateManager();
	if(m_recvState)
	{//开启接收
#ifdef WITH_OFXOSC
		while(m_receiver.hasWaitingMessages())
#else
		while(m_receiver.haveMessage())
#endif
		{
#ifdef WITH_OFXOSC
			ofxOscMessage m;
			m_receiver.getNextMessage(&m);
#else
			oscpkt::Message tmp;
			m_receiver.popAMessage(tmp);
			XOscMessage m(tmp);
#endif
			//将消息推入到显示中
			++m_recvSum;
			std::string recvData = "";
			char tmpStr[1024];
			//if(XString::isUTF8(m.getAddress()))
#ifdef WITH_OFXOSC
			if(m_codeModeCmb.getCurChooseOrder() == 1)
				sprintf(tmpStr,"%d-Addr:%s ",m_recvSum,XString::UTF82ANSI(m.getAddress()).c_str());
			else
				sprintf(tmpStr,"%d-Addr:%s ",m_recvSum,m.getAddress().c_str());
#else
			if(m_codeModeCmb.getCurChooseOrder() == 1)
				sprintf(tmpStr,"%d-Addr:%s ",m_recvSum,XString::UTF82ANSI(m.m_address).c_str());
			else
				sprintf(tmpStr,"%d-Addr:%s ",m_recvSum,m.m_address.c_str());
#endif
			recvData += tmpStr;
			if(getLanguageIndex() == INDEX_ENGLISH) LogNull("Receiver a data - Address:%s",tmpStr);
			else LogNull("接收到一条数据-地址:%s",tmpStr);
#ifdef WITH_OFXOSC
			for(int i = 0; i < m.getNumArgs(); i++)
			{//get the argument type
				recvData += "|Type:" + m.getArgTypeName(i);
				recvData += ":";
				switch(m.getArgType(i))
				{
				case OFXOSC_TYPE_INT32:
					recvData += XString::toString(m.getArgAsInt32(i));
					break;
				case OFXOSC_TYPE_INT64:
					recvData += XString::toString(m.getArgAsInt64(i));
					break;
				case OFXOSC_TYPE_FLOAT:
					recvData += XString::toString(m.getArgAsFloat(i));
					break;
				case OFXOSC_TYPE_STRING:
					//if(XString::isUTF8(m.getArgAsString(i)))
					if(m_codeModeCmb.getCurChooseOrder() == 1)
						recvData += XString::UTF82ANSI(m.getArgAsString(i));
					else
						recvData += m.getArgAsString(i);
					break;
				default:
					recvData += "unknown";
					break;
				}
			}
#else
			for(int i = 0; i < m.m_args.size();++ i)
			{//get the argument type
				recvData += "|Type:" + m.m_args[i].getTypeName();
				recvData += ":";
				switch(m.m_args[i].type)
				{
				case OSC_TYPE_INT32:
					recvData += XString::toString(m.m_args[i].dataI);
					break;
				case OSC_TYPE_INT64:
					recvData += XString::toString(m.m_args[i].dataL);
					break;
				case OSC_TYPE_FLOAT:
					recvData += XString::toString(m.m_args[i].dataF);
					break;
				case OSC_TYPE_STRING:
					//if(XString::isUTF8(m.getArgAsString(i)))
					if(m_codeModeCmb.getCurChooseOrder() == 1)
						recvData += XString::UTF82ANSI(m.m_args[i].dataS);
					else
						recvData += m.m_args[i].dataS;
					break;
				default:
					recvData += "unknown";
					break;
				}
			}
#endif
			m_recvText.addALine(recvData.c_str());
#ifdef WITH_OFXOSC
			if(m_supportACKMode.getState() && m.getNumArgs() == 0)
			{//如果支持问答机制的话，这里需要检查是否需要问答,这里由于后来的优化，造成设计不统一
				std::string tmp = m_nowSendAddr;
				if(m_codeModeCmb.getCurChooseOrder() == 1)
					m_nowSendAddr = XString::UTF82ANSI(m.getAddress());
				else 
					m_nowSendAddr = m.getAddress();
				sendNowData(0);
				m_nowSendAddr = tmp;
			}
#else
			if(m_supportACKMode.getState() && m.m_args.size() == 0)
			{//如果支持问答机制的话，这里需要检查是否需要问答,这里由于后来的优化，造成设计不统一
				std::string tmp = m_nowSendAddr;
				if(m_codeModeCmb.getCurChooseOrder() == 1)
					m_nowSendAddr = XString::UTF82ANSI(m.m_address);
				else 
					m_nowSendAddr = m.m_address;
				sendNowData(0);
				m_nowSendAddr = tmp;
			}
#endif
		}
	}
	if(m_isSendState)
	{//开启发送
		if(m_timer == 0)
		{
			sendNowData(m_sendModeCmb.getCurChooseOrder());

			++m_times;
			if(m_times >= m_setTimes)
			{
				sendOverProc();//发送完成
			}else
				m_textTimesEdt.setString(XString::toString(m_setTimes - m_times).c_str());
		}
		m_timer += stepTime;
		if(m_timer > m_setTimer) m_timer = 0;
	}
//	if(m_mouseOnTimer < 10000) m_mouseOnTimer += stepTime;
//	ctrlMsgProc();
	//下面根据hook的数据发送数据
	if(m_isSenderOpen && m_keyMapOscList.size() > 0)
	{
		XInputEvent e;
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
int GGame::keyValueToIndex(XKeyValue k)
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
//void GGame::ctrlMsgProc()
//{
//	if(!m_isShowMsg && m_mouseOnTimer > 2000)
//	{
//		if(m_textIPEdt.getVisible() && m_textIPEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//IP
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Enter the OSC sender IP,the local IP 127.0.0.1.");
//			else m_allMsgText.setString("输入OSC发送端的IP地址，本机IP可以输入127.0.0.1");
//		}else
//		if(m_textPortEdt.getVisible() && m_textPortEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//Port
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Enter the OSC sender port number,the default is 12345.");
//			else m_allMsgText.setString("输入OSC发送端的端口号，端口号需要与接收端的端口号一致，默认为12345");
//		}else
//		if(m_openBtn.getVisible() && m_openBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//打开按钮
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("OSC transmitter turned on or off , make sure the correct IP and port.");
//			else m_allMsgText.setString("打开或者关闭OSC发送器，确认IP地址和端口正确，程序不检测其正确性");
//		}else
//		if(m_sendAsMsg.getVisible() && m_sendAsMsg.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//作为消息发送
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Checked, the data to be sent as a message, otherwise it is sent as a bundle.");
//			else m_allMsgText.setString("勾选时需要发送的数据作为一条消息发送，否则作为一个数据包发送.");
//		}else
//		if(m_addBtn.getVisible() && m_addBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//添加
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Add the OSC data to list on the right.");
//			else m_allMsgText.setString("将左边编辑好的OSC数据信息添加到右边的列表中以便于之后操作。");
//		}else
//		if(m_delBtn.getVisible() && m_delBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//删除
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Delete the selected data on the left.");
//			else m_allMsgText.setString("删除左边列表中选中的数据。");
//		}else
//		if(m_clearBtn.getVisible() && m_clearBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//清空
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Clear all datas of the right list.");
//			else m_allMsgText.setString("清空右边列表框中的所有数据。");
//		}else
//		if(m_modifyBtn.getVisible() && m_modifyBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//修改
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Update data to the right of the selected row.");
//			else m_allMsgText.setString("从右边列表中选择一行数据并修改，按下该按钮将修改后的数据更新到列表中。");
//		}else
//		if(m_textDataEdt.getVisible() && m_textDataEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//data
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Osc data,make sure that the data and the type match.");
//			else m_allMsgText.setString("OSC消息中对应的数据，请确保输入的数据与下面的数据类型匹配。");
//		}else
//		if(m_textDataCmb.getVisible() && m_textDataCmb.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//type
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("The osc data type.");
//			else m_allMsgText.setString("OSC消息中数据的数据类型，请确保数据与数据类型匹配。");
//		}else
//		if(m_textAddressEdt.getVisible() && m_textAddressEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//addr
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("The osc address.");
//			else m_allMsgText.setString("OSC消息中地址。");
//		}else
//		if(m_textTimesEdt.getVisible() && m_textTimesEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//Times
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("The times of OSC massages sent cycles.");
//			else m_allMsgText.setString("OSC消息需要循环发送的次数，启动发送将会根据这个数值来循环发送。");
//		}else
//		if(m_textSpacingEdt.getVisible() && m_textSpacingEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//space
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("The interval between the two sent in milliseconds (ms).");
//			else m_allMsgText.setString("两次发送之间的间隔时间，单位为毫秒(ms)，1毫秒为千分之一秒。");
//		}else
//		if(m_sendBtn.getVisible() && m_sendBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//send
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Send or stop sending,please make sure the OSC sender has been opened.");
//			else m_allMsgText.setString("根据设定发送数据或停止发送，发送前请先确保OSC发送器已经正确打开。");
//		}else
//		if(m_sendModeCmb.getVisible() && m_sendModeCmb.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//发送模式
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Select the desired transmission mode ,there are four transmission modes.");
//			else m_allMsgText.setString("选择需要的发送模式，共有四种发送模式，如有疑问，请先阅读说明文档。");
//		}else
//		if(m_dataLst.getVisible() && m_dataLst.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//数据列表
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Data list,double-click on the data line to send.");
//			else m_allMsgText.setString("数据列表，可以双击相应数据行进行发送，请确保已经正确打开OSC发送器。");
//		}else
//		if(m_supportACKMode.getVisible() && m_supportACKMode.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//应答模式
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("ACK mode,receives data in the address blank will return data that address.");
//			else m_allMsgText.setString("应答模式，开启时，工具收到某地址的空数据时将会自动返回该地址的数据。");
//		}else
//		if(m_clearRecvBtn.getVisible() && m_clearRecvBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//清空列表
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Clear the reception list.");
//			else m_allMsgText.setString("按下该按钮清空接收列表，接收列表中的所有数据将会清空。");
//		}else
//		if(m_textRPortEdt.getVisible() && m_textRPortEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//接收端口
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("The OSC receiver port number,the default is 12345.");
//			else m_allMsgText.setString("OSC接收器的端口号，必须与相应发送器的端口号一致才能正确接收，默认为12345");
//		}else
//		if(m_openRBtn.getVisible() && m_openRBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//打开
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("OSC receiver on or off.");
//			else m_allMsgText.setString("打开或关闭OSC接收器，请确保接收器端口号输入正确。");
//		}else
//		if(m_saveBtn.getVisible() && m_saveBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//保存
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Save all datas to \"config.xml\".");
//			else m_allMsgText.setString("将当前界面上的所有数据保存到config.xml文件中。");
//		}else
//		if(m_readBtn.getVisible() && m_readBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//读取
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Read the last saved configuration data from \"config.xml\".");
//			else m_allMsgText.setString("从配置文件config.xml中读取上一次保存的配置数据。");
//		}else
//		if(m_recvText.getVisible() && m_recvText.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//接收列表框
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Received data will be displayed here.");
//			else m_allMsgText.setString("接收的数据将显示于此，选择最末行可以保持光标于末行。");
//		}else
//		if(m_addKMBtn.getVisible() && m_addKMBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//添加
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Add the map to the map list.");
//			else m_allMsgText.setString("将左边编辑好的鼠标键盘与OSC的映射添加到映射列表中。");
//		}else
//		if(m_delKMBtn.getVisible() && m_delKMBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//删除
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Delete selected row in the map list.");
//			else m_allMsgText.setString("删除映射列表中选择的行。");
//		}else
//		if(m_clearKMBtn.getVisible() && m_clearKMBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//清空
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Clear map list on the left.");
//			else m_allMsgText.setString("清空左边的映射列表。");
//		}else
//		if(m_modifyKMBtn.getVisible() && m_modifyKMBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//修改
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Update data to the left row selected in the list.");
//			else m_allMsgText.setString("将左边编辑好的值更新到映射列表中选中的行。");
//		}else
//		if(m_keySelectCmb.getVisible() && m_keySelectCmb.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//键值
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Key or mouse button.");
//			else m_allMsgText.setString("对应的鼠标或者键盘按键。");
//		}else
//		if(m_keyStateCmb.getVisible() && m_keyStateCmb.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//按键状态
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Key or button state.");
//			else m_allMsgText.setString("按键的状态。");
//		}else
//		if(m_oscDataEdt.getVisible() && m_oscDataEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//osc数据编辑
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("OSC data.");
//			else m_allMsgText.setString("OSC数据。");
//		}else
//		if(m_oscDataCmb.getVisible() && m_oscDataCmb.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//osc数据编辑
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("OSC data type.");
//			else m_allMsgText.setString("OSC数据类型。");
//		}else
//		if(m_oscAddressEdt.getVisible() && m_oscAddressEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//osc地址
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("OSC address.");
//			else m_allMsgText.setString("OSC地址。");
//		}else
//		if(m_dataKMLst.getVisible() && m_dataKMLst.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//鼠标键盘和osc的映射列表
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Mapping list mouse, keyboard and osc.");
//			else m_allMsgText.setString("鼠标键盘和osc的映射列表。");
//		}
//	}
//}
void GGame::draw()
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
void GGame::input(const XInputEvent &event)
{
//	if(event.type == EVENT_MOUSE)
//	{
//		m_mouseOnTimer = 0;
//		m_isShowMsg = false;
//	}
	if(event.isKeyBoardDown())
	{
		switch(event.keyValue)
		{
		case XKEY_ESCAPE:
			windowExitFun(this);
			break;
//		case XKEY_A:
//			XEE::setWindowAlpha(0.5f);
//			//XEE::setBGColor(XFColor::white);
//			break;
		}
	}
}
void GGame::release()
{
	for(int i = 0;i < m_datasVector.size();++ i)
	{
		XMem::XDELETE(m_datasVector[i]);
	}
	m_datasVector.clear();
	for(int i = 0;i < m_keyMapOscList.size();++ i)
	{
		XMem::XDELETE(m_keyMapOscList[i]);
	}
	stopHook();
	m_keyMapOscList.clear();
}
bool GGame::saveToFile(const char *filename)
{
	//TiXmlDocument doc(CFG_FILENAME);
	if(filename != NULL && filename[0] == '\0') return false;
	TiXmlDocument doc;
//	if(filename == NULL) doc.crea(CFG_FILENAME);
//	else doc.LoadFile(filename);
	TiXmlDeclaration declaration("1.0","gb2312","yes"); 
	doc.InsertEndChild(declaration);
	TiXmlElement elmRoot("root");

	if(!XXml::addLeafNode(elmRoot,"sendIP",m_textIPEdt.getString())) return false;
	if(!XXml::addLeafNode(elmRoot,"sendPort",m_textPortEdt.getString())) return false;
	if(!XXml::addLeafNode(elmRoot,"sendAsMessage",m_sendAsMsg.getState())) return false;
	if(!XXml::addLeafNode(elmRoot,"dataE",m_textDataEdt.getString())) return false;
	if(!XXml::addLeafNode(elmRoot,"typeE",XString::toString(m_textDataCmb.getCurChooseOrder()))) return false;
	if(!XXml::addLeafNode(elmRoot,"addressE",m_textAddressEdt.getString())) return false;
	if(!XXml::addLeafNode(elmRoot,"timeE",m_textTimesEdt.getString())) return false;
	if(!XXml::addLeafNode(elmRoot,"spaceE",m_textSpacingEdt.getString())) return false;
	if(!XXml::addLeafNode(elmRoot,"sendMode",XString::toString(m_sendModeCmb.getCurChooseOrder()))) return false;

	for(int i = 0;i < m_datasVector.size();++ i)
	{
		TiXmlElement elmNode("data"); 
		if(!XXml::addLeafNode(elmNode,"addr",m_datasVector[i]->address)) return false;
		if(!XXml::addLeafNode(elmNode,"datas",m_datasVector[i]->data)) return false;
		if(!XXml::addLeafNode(elmNode,"type",XString::toString(m_datasVector[i]->dataType))) return false;
		if(elmRoot.InsertEndChild(elmNode) == NULL) return false;  
	}
	if(!XXml::addLeafNode(elmRoot,"keyValueE",XString::toString(m_keySelectCmb.getCurChooseOrder()))) return false;
	if(!XXml::addLeafNode(elmRoot,"keyStateE",XString::toString(m_keyStateCmb.getCurChooseOrder()))) return false;
	if(!XXml::addLeafNode(elmRoot,"oscDataE",m_oscDataEdt.getString())) return false;
	if(!XXml::addLeafNode(elmRoot,"oscTypeE",XString::toString(m_oscDataCmb.getCurChooseOrder()))) return false;
	if(!XXml::addLeafNode(elmRoot,"oscAddrE",m_oscAddressEdt.getString())) return false;
	for(int i = 0;i < m_keyMapOscList.size();++ i)
	{
		TiXmlElement elmNode("map"); 
		if(!XXml::addLeafNode(elmNode,"keyValue",XString::toString(m_keyMapOscList[i]->keyValue))) return false;
		if(!XXml::addLeafNode(elmNode,"keyState",XString::toString(m_keyMapOscList[i]->keyState))) return false;
		if(!XXml::addLeafNode(elmNode,"addr",m_keyMapOscList[i]->address)) return false;
		if(!XXml::addLeafNode(elmNode,"datas",m_keyMapOscList[i]->data)) return false;
		if(!XXml::addLeafNode(elmNode,"type",XString::toString(m_keyMapOscList[i]->dataType))) return false;
		if(elmRoot.InsertEndChild(elmNode) == NULL) return false;  
	}
	if(!XXml::addLeafNode(elmRoot,"recvPort",m_textRPortEdt.getString())) return false;
	if(!XXml::addLeafNode(elmRoot,"ACKMode",m_supportACKMode.getState())) return false;
	if(!XXml::addLeafNode(elmRoot,"language",XString::toString(m_languageCmb.getCurChooseOrder()))) return false;
	if(!XXml::addLeafNode(elmRoot,"codeMode",XString::toString(m_codeModeCmb.getCurChooseOrder()))) return false;

	if(doc.InsertEndChild(elmRoot) == NULL) return false;
	bool ret;
	if(filename == NULL) ret = doc.SaveFile(CFG_FILENAME);
	else ret = doc.SaveFile(filename);
	if(!ret) return ret;
	//doc.SaveFile();
	if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Data saved!"); 
	else setMassage("文件保存完成!");
	//测试
	//{
	//	TiXmlDocument doc;
	//	TiXmlDeclaration declaration("1.0","gb2312","yes"); 
	//	doc.InsertEndChild(declaration);
	//	TiXmlElement elmRoot("root");
	//	m_tab.saveState(elmRoot);
	//	doc.InsertEndChild(elmRoot);
	//	doc.SaveFile("Test.xml");
	//}
	return true;
}
bool GGame::readFromFile(const char *filename)
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
	TiXmlNode *rootNode = doc.FirstChild("root");
	if(rootNode == NULL) return false;
	if(XXml::getXmlAsString(rootNode,"sendIP",tmp))
		m_textIPEdt.setString(tmp.c_str());
	if(XXml::getXmlAsString(rootNode,"sendPort",tmp))
		m_textPortEdt.setString(tmp.c_str());
	if(XXml::getXmlAsBool(rootNode,"sendAsMessage",tmpB))
		m_sendAsMsg.setState(tmpB);
	if(XXml::getXmlAsString(rootNode,"dataE",tmp))
		m_textDataEdt.setString(tmp.c_str());
	if(XXml::getXmlAsInt(rootNode,"typeE",tmpI))
		m_textDataCmb.setCurChooseOrder(tmpI);
	if(XXml::getXmlAsString(rootNode,"addressE",tmp))
		m_textAddressEdt.setString(tmp.c_str());
	if(XXml::getXmlAsString(rootNode,"timeE",tmp))
		m_textTimesEdt.setString(tmp.c_str());
	if(XXml::getXmlAsString(rootNode,"spaceE",tmp))
		m_textSpacingEdt.setString(tmp.c_str());
	if(XXml::getXmlAsInt(rootNode,"sendMode",tmpI))
		m_sendModeCmb.setCurChooseOrder(tmpI);
	//下面读取数据
	for(int i = 0;i < m_datasVector.size();++ i)
	{
		XMem::XDELETE(m_datasVector[i]);
	}
	m_datasVector.clear();
	m_dataLst.setLineSum(0);
	keyNode = rootNode->FirstChildElement("data");
	while(keyNode != NULL)
	{
		GOscData *tmp = XMem::createMem<GOscData>();
		XXml::getXmlAsString(keyNode,"addr",tmp->address);
		XXml::getXmlAsString(keyNode,"datas",tmp->data);
		XXml::getXmlAsInt(keyNode,"type",tmp->dataType);
		m_datasVector.push_back(tmp);
		addALineData(tmp);

		keyNode = keyNode->NextSiblingElement("data");
	}

	if(XXml::getXmlAsInt(rootNode,"keyValueE",tmpI))
		m_keySelectCmb.setCurChooseOrder(tmpI);
	if(XXml::getXmlAsInt(rootNode,"keyStateE",tmpI))
		m_keyStateCmb.setCurChooseOrder(tmpI);
	if(XXml::getXmlAsString(rootNode,"oscDataE",tmp))
		m_oscDataEdt.setString(tmp.c_str());
	if(XXml::getXmlAsInt(rootNode,"oscTypeE",tmpI))
		m_oscDataCmb.setCurChooseOrder(tmpI);
	if(XXml::getXmlAsString(rootNode,"oscAddrE",tmp))
		m_oscAddressEdt.setString(tmp.c_str());
	for(int i = 0;i < m_keyMapOscList.size();++ i)
	{
		XMem::XDELETE(m_keyMapOscList[i]);
	}
	m_keyMapOscList.clear();
	m_dataKMLst.setLineSum(0);
	keyNode = rootNode->FirstChildElement("map");
	while(keyNode != NULL)
	{
		GKMOscData *tmp = XMem::createMem<GKMOscData>();
		XXml::getXmlAsInt(keyNode,"keyValue",tmp->keyValue);
		XXml::getXmlAsInt(keyNode,"keyState",tmp->keyState);
		XXml::getXmlAsString(keyNode,"addr",tmp->address);
		XXml::getXmlAsString(keyNode,"datas",tmp->data);
		XXml::getXmlAsInt(keyNode,"type",tmp->dataType);
		m_keyMapOscList.push_back(tmp);
		addALineData(tmp);
		keyNode = keyNode->NextSiblingElement("map");
	}
	//读取其他数据
	if(XXml::getXmlAsString(rootNode,"recvPort",tmp))
		m_textRPortEdt.setString(tmp.c_str());
	if(XXml::getXmlAsBool(rootNode,"ACKMode",tmpB))
		m_supportACKMode.setState(tmpB);
	if(XXml::getXmlAsInt(rootNode,"language",tmpI))
		m_languageCmb.setCurChooseOrder(tmpI);
	if(XXml::getXmlAsInt(rootNode,"codeMode",tmpI))
		m_codeModeCmb.setCurChooseOrder(tmpI);
	//测试
	//TiXmlDocument doc;
	//doc.LoadFile("Test.xml");

	//if(!doc.LoadFile()) return false;
	//TiXmlNode *rootNode = doc.FirstChild("root");
	//if(rootNode == NULL) return false;
	//m_tab.loadState(rootNode);

	return true;
}
void GGame::addALineData(const GOscData *tmp)
{
	int lineSum = m_dataLst.getLineSum();
	m_dataLst.addALine();
	setLineData(tmp,lineSum);
}
void GGame::setLineData(const GOscData *tmp,int index)
{
	m_dataLst.setBoxStr(tmp->address.c_str(),index,0);
	m_dataLst.setBoxStr(tmp->data.c_str(),index,1);
	m_dataLst.setBoxStr(oscDataTypeToStr(tmp->dataType).c_str(),index,2);
}
void GGame::addALineData(const GKMOscData *tmp)
{
	int lineSum = m_dataKMLst.getLineSum();
	m_dataKMLst.addALine();
	setLineData(tmp,lineSum);
}
void GGame::setLineData(const GKMOscData *tmp,int index)
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
std::string GGame::keyValueToStr(int keyValue)
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
	case 39:return "MouseX";
	case 40:return "Mouse_Y";
	default:
		return "";
	}
}
std::string GGame::oscDataTypeToStr(int type)
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
void GGame::ctrlStateManager()
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
//#include <psapi.h>
//获取指定名称的内存使用数，单位为KB
//#define MAX_BUF_SIZE (512)
//int getProcMemoryUsage( const char* pProcessName )
//{
//	WCHAR chBuf[MAX_BUF_SIZE];
//	ZeroMemory( chBuf , MAX_BUF_SIZE );
//	DWORD dwProcs[1024*2];
//	DWORD dwNeeded;
//	if( !EnumProcesses(dwProcs , sizeof(dwProcs) , &dwNeeded  ))
//	{//输出出错信息
//		printf("EnumProcesses Failed (%d).\n",GetLastError());
//		return -1;
//	}
//	//计算有多少个进程ID
//	DWORD dwProcCount =  dwNeeded / sizeof( DWORD );
//
//	HMODULE hMod;
//	DWORD arraySize;
//	char processName[MAX_BUF_SIZE];
//	for( int i=0; i<dwProcCount ; i++ )
//	{
//		DWORD m_processid = dwProcs[i];
//		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,m_processid); 
//		if(hProcess)
//		{
//			if (EnumProcessModules(hProcess,&hMod,sizeof(HMODULE),&arraySize))
//			{
//				GetModuleBaseName(hProcess,hMod,processName,sizeof(processName));
//				if( strcmp(processName , pProcessName )!=0 )
//				{
//					PROCESS_MEMORY_COUNTERS pmc;
//					GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc));
// 
//					printf("%d \n %d\n %d \n %d\n" ,pmc.PeakWorkingSetSize , pmc.PagefileUsage , pmc.QuotaPagedPoolUsage , pmc.QuotaPeakPagedPoolUsage);
//					return pmc.PagefileUsage/1024;
//				}
//			}
//		}
//	}
//	return -1;
//}
void GGame::updateLanguage()
{
	switch(m_languageCmb.getCurChooseOrder())
	{
	case 0:	//英文
		if(!m_language.setCurrentLanguage(XLGG_EN,"languageEN.xml")) return;
		setMassage("Set to English.");
		break;
	case 1:
		if(!m_language.setCurrentLanguage(XLGG_EN,"languageCN.xml")) return;
		setMassage("设置为中文");
		break;
	}
	m_tab.setTabsStr(m_language.m_tabTitileStr.c_str());
	//下面是关于界面元素的定义
	//m_textIP;
	//m_textIPEdt;
	//m_textPort;
	//m_textPortEdt;
	if(m_isSenderOpen) m_openBtn.setCaptionText(m_language.m_openBtnCloseStr.c_str());
	else m_openBtn.setCaptionText(m_language.m_openBtnOpenStr.c_str());
	//m_openBtn;
	////m_closeBtn;
	//++++++++++++++++++++++++++++++
	//下面是为了增加键盘模拟而定义的变量
	m_addKMBtn.setCaptionText(m_language.m_addKMBtnStr.c_str());
	m_delKMBtn.setCaptionText(m_language.m_delKMBtnStr.c_str());
	m_clearKMBtn.setCaptionText(m_language.m_clearKMBtnStr.c_str());	//清空
	m_modifyKMBtn.setCaptionText(m_language.m_modifyKMBtnStr.c_str());	//修改按钮
	//m_keySelectCmb;	//按键的选择
	//m_keyStateCmb;	//按键状态的选择
	m_oscDataTxt.setString(m_language.m_oscDataTxtStr.c_str());
	//m_oscDataEdt;
	m_oscType.setString(m_language.m_oscTypeStr.c_str());
	//m_oscDataCmb;
	m_oscAddress.setString(m_language.m_oscAddressStr.c_str());
	//m_oscAddressEdt;
	//m_dataKMLst;
	//------------------------------
	m_addBtn.setCaptionText(m_language.m_addBtnStr.c_str());
	m_delBtn.setCaptionText(m_language.m_delBtnStr.c_str());
	m_clearBtn.setCaptionText(m_language.m_clearBtnStr.c_str());	//清空
	m_modifyBtn.setCaptionText(m_language.m_modifyBtnStr.c_str());	//修改按钮
	m_textData.setString(m_language.m_textDataStr.c_str());
	//m_textDataEdt;
	m_textType.setString(m_language.m_textTypeStr.c_str());
	//m_textDataCmb;
	m_textAddress.setString(m_language.m_textAddressStr.c_str());
	//m_textAddressEdt;
	m_textTimes.setString(m_language.m_textTimesStr.c_str());
	//m_textTimesEdt;
	m_textSpacing.setString(m_language.m_textSpacingStr.c_str());
	//m_textSpacingEdt;
	if(m_isSendState) m_sendBtn.setCaptionText(m_language.m_sendBtnCloseStr.c_str());
	else m_sendBtn.setCaptionText(m_language.m_sendBtnOpenStr.c_str());
	////XButton m_stopBtn;
	m_sendModeCmb.setMenuStr(m_language.m_sendModeCmbStr.c_str());
	//XMultiList m_dataLst;
	m_sendAsMsg.setCaptionText(m_language.m_sendAsMsgStr.c_str());
	m_supportACKMode.setCaptionText(m_language.m_supportACKModeStr.c_str());
	m_clearRecvBtn.setCaptionText(m_language.m_clearRecvBtnStr.c_str());	//清除接收框的按钮

	//m_languageCmb;		//语言选择

	////m_textRIP;
	////m_textRIPEdt;
	//m_textRPort;
	//m_textRPortEdt;
	if(m_recvState) m_openRBtn.setCaptionText(m_language.m_openRBtnOpenStr.c_str());
	else m_openRBtn.setCaptionText(m_language.m_openRBtnCloseStr.c_str());
	////m_closeRBtn;
	m_saveBtn.setCaptionText(m_language.m_saveBtnStr.c_str());		//保存按钮
	m_readBtn.setCaptionText(m_language.m_readBtnStr.c_str());		//读取配置文件的按钮
		
	m_textIPEdt.setComment(m_language.m_textIPEdtCommentStr.c_str());
	m_textPortEdt.setComment(m_language.m_textPortEdtCommentStr.c_str());
	m_openBtn.setComment(m_language.m_openBtnCommentStr.c_str());
	m_addBtn.setComment(m_language.m_addBtnCommentStr.c_str());
	m_delBtn.setComment(m_language.m_delBtnCommentStr.c_str());
	m_clearBtn.setComment(m_language.m_clearBtnCommentStr.c_str());
	m_modifyBtn.setComment(m_language.m_modifyBtnCommentStr.c_str());
	m_textDataEdt.setComment(m_language.m_textDataEdtCommentStr.c_str());
	m_textDataCmb.setComment(m_language.m_textDataCmbCommentStr.c_str());
	m_textAddressEdt.setComment(m_language.m_textAddressEdtCommentStr.c_str());
	m_textTimesEdt.setComment(m_language.m_textTimesEdtCommentStr.c_str());
	m_textSpacingEdt.setComment(m_language.m_textSpacingEdtCommentStr.c_str());
	m_sendModeCmb.setComment(m_language.m_sendModeCmbCommentStr.c_str());
	m_sendAsMsg.setComment(m_language.m_sendAsMsgCommentStr.c_str());
	m_sendBtn.setComment(m_language.m_sendBtnCommentStr.c_str());
	m_textRPortEdt.setComment(m_language.m_textRPortEdtCommentStr.c_str());
	m_openRBtn.setComment(m_language.m_openRBtnCommentStr.c_str());
	m_supportACKMode.setComment(m_language.m_supportACKModeCommentStr.c_str());
	m_clearRecvBtn.setComment(m_language.m_clearRecvBtnCommentStr.c_str());
	m_languageCmb.setComment(m_language.m_languageCmbCommentStr.c_str());
	m_codeModeCmb.setComment(m_language.m_codeModeCmbCommentStr.c_str());
	m_saveBtn.setComment(m_language.m_saveBtnCommentStr.c_str());
	m_readBtn.setComment(m_language.m_readBtnCommentStr.c_str());
	m_addKMBtn.setComment(m_language.m_addKMBtnCommentStr.c_str());
	m_delKMBtn.setComment(m_language.m_delKMBtnCommentStr.c_str());
	m_clearKMBtn.setComment(m_language.m_clearKMBtnCommentStr.c_str());
	m_modifyKMBtn.setComment(m_language.m_modifyKMBtnCommentStr.c_str());
	m_keySelectCmb.setComment(m_language.m_keySelectCmbCommentStr.c_str());
	m_keyStateCmb.setComment(m_language.m_keyStateCmbCommentStr.c_str());
	m_oscDataEdt.setComment(m_language.m_oscDataEdtCommentStr.c_str());
	m_oscDataCmb.setComment(m_language.m_oscDataCmbCommentStr.c_str());
	m_oscAddressEdt.setComment(m_language.m_oscAddressEdtCommentStr.c_str());
}