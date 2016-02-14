#ifndef _JIA_GLANGUAGE_
#define _JIA_GLANGUAGE_
#include "XLanguage.h"
class GLanguage:public XE::XLanguageBasic
{
protected:
	virtual XE::XBool loadFromFile(const std::string &filename);	//从文件中读取字符串
	virtual XE::XBool saveToFile(const std::string &filename);	//从文件中读取字符串
public:
	virtual XE::XBool setCurrentLanguage(XE::XLanguage language,const std::string &filename);	//从指定文件中载入字符串资源并使之生效
	GLanguage()
		:m_tabTitileStr("发送端口设置;发送数据设置;键盘鼠标模拟;接收端设置;系统设置;")
		,m_openBtnOpenStr("打开")
		,m_openBtnCloseStr("关闭")
		,m_addKMBtnStr("添加")
		,m_delKMBtnStr("删除")
		,m_clearKMBtnStr("清空")
		,m_modifyKMBtnStr("修改")
		,m_oscDataTxtStr("数据:")
		,m_oscTypeStr("类型:")
		,m_oscAddressStr("地址:")
		,m_addBtnStr("添加")
		,m_delBtnStr("删除")
		,m_clearBtnStr("清空")
		,m_modifyBtnStr("修改")
		,m_textDataStr("数据:")
		,m_textTypeStr("类型:")
		,m_textAddressStr("地址:")
		,m_textTimesStr("次数:")
		,m_textSpacingStr("间隔:")
		,m_sendBtnOpenStr("停止发送")
		,m_sendBtnCloseStr("开始发送")
		,m_sendModeCmbStr("发送选中地址;发送选中数据;发送空数据;发送当前数据;")
		,m_sendAsMsgStr("作为消息发送")
		,m_supportACKModeStr("是否问答机制")
		,m_clearRecvBtnStr("清除接收框")
		,m_openRBtnOpenStr("停止接收")
		,m_openRBtnCloseStr("开始接收")
		,m_saveBtnStr("保存")
		,m_readBtnStr("读取")
		//下面是一些控件的注释
		,m_textIPEdtCommentStr("输入OSC发送端的IP地址，本机IP时可以输入127.0.0.1")
		,m_textPortEdtCommentStr("输入OSC发送端的端口号，端口号需要与接收端的端口号一致，默认为12345")
		,m_openBtnCommentStr("打开或者关闭OSC发送器，确认IP地址和端口正确，程序不检测其正确性")
		,m_addBtnCommentStr("将左边编辑好的OSC数据信息添加到右边的列表中以便于之后操作")
		,m_delBtnCommentStr("删除左边列表中选中的数据")
		,m_clearBtnCommentStr("清空右边列表框中的所有数据")
		,m_modifyBtnCommentStr("从右边列表中选择一行数据并修改，按下该按钮将修改后的数据更新到列表中")
		,m_textDataEdtCommentStr("OSC消息中对应的数据，请确保输入的数据与下面的数据类型匹配")
		,m_textDataCmbCommentStr("OSC消息中数据的数据类型，请确保数据与数据类型匹配")
		,m_textAddressEdtCommentStr("OSC消息中地址")
		,m_textTimesEdtCommentStr("OSC消息需要循环发送的次数，启动发送将会根据这个数值来循环发送")
		,m_textSpacingEdtCommentStr("两次发送之间的间隔时间，单位为毫秒(ms)，1毫秒为千分之一秒")
		,m_sendModeCmbCommentStr("选择需要的发送模式，共有四种发送模式，如有疑问，请先阅读说明文档")
		,m_sendAsMsgCommentStr("勾选时需要发送的数据作为一条消息发送，否则作为一个数据包发送")
		,m_sendBtnCommentStr("根据设定发送数据或停止发送，发送前请先确保OSC发送器已经正确打开")
		,m_textRPortEdtCommentStr("OSC接收器的端口号，必须与相应发送器的端口号一致才能正确接收，默认为12345")
		,m_openRBtnCommentStr("打开或关闭OSC接收器，请确保接收器端口号输入正确")
		,m_supportACKModeCommentStr("应答模式，开启时，工具收到某地址的空数据时将会自动返回该地址的数据")
		,m_clearRecvBtnCommentStr("按下该按钮清空接收列表，接收列表中的所有数据将会清空")
		,m_languageCmbCommentStr("选择使用的语言")
		,m_codeModeCmbCommentStr("选择使用的编码方式")
		,m_saveBtnCommentStr("将当前界面上的所有数据保存到文件中")
		,m_readBtnCommentStr("从配置文件中读取上一次保存的配置数据")
		,m_addKMBtnCommentStr("将左边编辑好的鼠标键盘与OSC的映射添加到映射列表中")
		,m_delKMBtnCommentStr("删除映射列表中选择的行")
		,m_clearKMBtnCommentStr("清空左边的映射列表")
		,m_modifyKMBtnCommentStr("将左边编辑好的值更新到映射列表中选中的行")
		,m_keySelectCmbCommentStr("对应的鼠标或者键盘按键")
		,m_keyStateCmbCommentStr("按键的状态")
		,m_oscDataEdtCommentStr("OSC数据")
		,m_oscDataCmbCommentStr("OSC数据类型")
		,m_oscAddressEdtCommentStr("OSC地址")
	{}
	virtual ~GLanguage(){}
	//下面是对需要的字符串进行定义
	std::string m_tabTitileStr;
	std::string m_openBtnOpenStr;
	std::string m_openBtnCloseStr;
	std::string m_addKMBtnStr;
	std::string m_delKMBtnStr;
	std::string m_clearKMBtnStr;
	std::string m_modifyKMBtnStr;
	std::string m_oscDataTxtStr;
	std::string m_oscTypeStr;
	std::string m_oscAddressStr;
	std::string m_addBtnStr;
	std::string m_delBtnStr;
	std::string m_clearBtnStr;
	std::string m_modifyBtnStr;
	std::string m_textDataStr;
	std::string m_textTypeStr;
	std::string m_textAddressStr;
	std::string m_textTimesStr;
	std::string m_textSpacingStr;
	std::string m_sendBtnOpenStr;
	std::string m_sendBtnCloseStr;
	std::string m_sendModeCmbStr;
	std::string m_sendAsMsgStr;
	std::string m_supportACKModeStr;
	std::string m_clearRecvBtnStr;
	std::string m_openRBtnOpenStr;
	std::string m_openRBtnCloseStr;
	std::string m_saveBtnStr;
	std::string m_readBtnStr;
	//下面是一些控件的注释
	std::string m_textIPEdtCommentStr;//("输入OSC发送端的IP地址，本机IP时可以输入127.0.0.1");
	std::string m_textPortEdtCommentStr;//("输入OSC发送端的端口号，端口号需要与接收端的端口号一致，默认为12345");
	std::string m_openBtnCommentStr;//("打开或者关闭OSC发送器，确认IP地址和端口正确，程序不检测其正确性");
	std::string m_addBtnCommentStr;//("将左边编辑好的OSC数据信息添加到右边的列表中以便于之后操作");
	std::string m_delBtnCommentStr;//("删除左边列表中选中的数据");
	std::string m_clearBtnCommentStr;//("清空右边列表框中的所有数据");
	std::string m_modifyBtnCommentStr;//("从右边列表中选择一行数据并修改，按下该按钮将修改后的数据更新到列表中");
	std::string m_textDataEdtCommentStr;//("OSC消息中对应的数据，请确保输入的数据与下面的数据类型匹配");
	std::string m_textDataCmbCommentStr;//("OSC消息中数据的数据类型，请确保数据与数据类型匹配");
	std::string m_textAddressEdtCommentStr;//("OSC消息中地址");
	std::string m_textTimesEdtCommentStr;//("OSC消息需要循环发送的次数，启动发送将会根据这个数值来循环发送");
	std::string m_textSpacingEdtCommentStr;//("两次发送之间的间隔时间，单位为毫秒(ms)，1毫秒为千分之一秒");
	std::string m_sendModeCmbCommentStr;//("选择需要的发送模式，共有四种发送模式，如有疑问，请先阅读说明文档");
	std::string m_sendAsMsgCommentStr;//("勾选时需要发送的数据作为一条消息发送，否则作为一个数据包发送");
	std::string m_sendBtnCommentStr;//("根据设定发送数据或停止发送，发送前请先确保OSC发送器已经正确打开");
	std::string m_textRPortEdtCommentStr;//("OSC接收器的端口号，必须与相应发送器的端口号一致才能正确接收，默认为12345");
	std::string m_openRBtnCommentStr;//("打开或关闭OSC接收器，请确保接收器端口号输入正确");
	std::string m_supportACKModeCommentStr;//("应答模式，开启时，工具收到某地址的空数据时将会自动返回该地址的数据");
	std::string m_clearRecvBtnCommentStr;//("按下该按钮清空接收列表，接收列表中的所有数据将会清空");
	std::string m_languageCmbCommentStr;//("选择使用的语言");
	std::string m_codeModeCmbCommentStr;//("选择编码方式");
	std::string m_saveBtnCommentStr;//("将当前界面上的所有数据保存到文件中");
	std::string m_readBtnCommentStr;//("从配置文件中读取上一次保存的配置数据");
	std::string m_addKMBtnCommentStr;//("将左边编辑好的鼠标键盘与OSC的映射添加到映射列表中");
	std::string m_delKMBtnCommentStr;//("删除映射列表中选择的行");
	std::string m_clearKMBtnCommentStr;//("清空左边的映射列表");
	std::string m_modifyKMBtnCommentStr;//("将左边编辑好的值更新到映射列表中选中的行");
	std::string m_keySelectCmbCommentStr;//("对应的鼠标或者键盘按键");
	std::string m_keyStateCmbCommentStr;//("按键的状态");
	std::string m_oscDataEdtCommentStr;//("OSC数据");
	std::string m_oscDataCmbCommentStr;//("OSC数据类型");
	std::string m_oscAddressEdtCommentStr;//("OSC地址");
};
#endif