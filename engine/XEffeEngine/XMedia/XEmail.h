#ifndef _JIA_XEMAIL_
#define _JIA_XEMAIL_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.8.29
//--------------------------------
#include <iostream>
#include <String.h>
#include <fstream>
#include <string>
#include "XBasicFun.h"
#include "XBasicClass.h"
#include <List>
#include <vector>
//这是一个用于发送邮件的类,以进行初步测试通过
struct sMailInfo	//邮件信息  
{  
	std::string m_pcUserName;		//用户登录邮箱的名称  
	std::string m_pcUserPassWord;	//用户登录邮箱的密码  
	std::string m_pcSenderName;	//用户发送时显示的名称  
	std::string m_pcSender;		//发送者的邮箱地址  
	std::string m_pcReceiver;		//接收者的邮箱地址  
	std::string m_pcReceiverName;		//收件人的名称  
	std::string m_pcTitle;		//邮箱标题  
	std::string m_pcBody;			//邮件文本正文  
	std::string m_pcIPAddr;		//服务器的IP  
	std::string m_pcIPName;		//服务器的名称（IP与名称二选一，优先取名称）  
	sMailInfo()
		:m_pcUserName("")
		,m_pcUserPassWord("")
		,m_pcSenderName("")
		,m_pcSender("")
		,m_pcReceiver("")
		,m_pcReceiverName("")
		,m_pcTitle("")
		,m_pcBody("")
		,m_pcIPAddr("")
		,m_pcIPName("")
	{}  
}; 
class _XEmailSender
{
public:  
    _XEmailSender()
		:m_pcFileBuff(NULL)
	{
		memset(m_cSendBuff,0,sizeof(m_cSendBuff));  
		memset(m_cReceiveBuff,0,sizeof(m_cReceiveBuff));  
	}
	~_XEmailSender(){DeleteAllPath();}
public:  
    bool SendMail(sMailInfo &smailInfo);		//发送邮件，需要在发送的时候初始化邮件信息  
    void AddFilePath(char * pcFilePath);		//添加附件的决定路径到附件列表中  
    void DeleteFilePath(char* pcFilePath);		//删除附件路径，如果有的话  
	void DeleteAllPath(){m_pcFilePathList.clear();}					//删除全部附件的路径  
protected:  
    void Char2Base64(char* pBuff64,char* pSrcBuff,int iLen);	//把char类型转换成Base64类型  
    bool  CReateSocket(SOCKET &sock);	//建立socket连接  
    bool Logon(SOCKET &sock);			//登录邮箱，主要进行发邮件前的准备工作  
    int GetFileData(const char* FilePath);	//由文件路径获取附件内容  
  
    bool SendHead(SOCKET &sock);		//发送邮件头  
    bool SendTextBody(SOCKET &sock)	//发送邮件文本正文  
	{
		memset(m_cSendBuff,0,sizeof(m_cSendBuff));  
		sprintf_s(m_cSendBuff,"--INVT\r\nContent-Type: text/plain;\r\n  charset=\"gb2312\"\r\n\r\n%s\r\n\r\n",m_sMailInfo.m_pcBody.c_str());  
		return (send(sock,m_cSendBuff,strlen(m_cSendBuff),0) == strlen(m_cSendBuff)); 
	}
    bool SendFileBody(SOCKET &sock);	//发送邮件附件  
    bool SendEnd(SOCKET &sock);			//发送邮件结尾  
protected:  
    //CList<char*,char*> m_pcFilePathList;	//记录附件路径  
	//std::list<char*,char*> m_pcFilePathList;	//记录附件路径  
	std::vector<std::string> m_pcFilePathList;
  
    char  m_cSendBuff[4096];		//发送缓冲区  
    char  m_cReceiveBuff[1024];  
    char* m_pcFileBuff;				//指向附件内容  
    sMailInfo m_sMailInfo;  
};
#include "XEmail.inl"
#endif