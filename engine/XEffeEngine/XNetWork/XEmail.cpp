#include "XStdHead.h"
#include "XEmail.h" 
#include "XFile.h"
namespace XE{   
void XEmailSender::Char2Base64(char* pBuff64,char* pSrcBuff,int iLen)  
{  
    //1   1   1   1   1   1   1   1  
    // 分配给pBuff64  ↑ 分配给pBuff64+1  
    //         point所在的位置  
    static const char Base64Encode[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";//base64所映射的字符表  
    int point = 2;	//每一个源字符拆分的位置，可取2,4,6；初始为2  
    int iIndex = 0;	//base64字符的索引  
    char n=0;		//上一个源字符的残留值  
    for(int i = 0;i < iLen;++ i)  
    {  
		switch(point)
		{
		case 2:
            iIndex = ((*pSrcBuff)>>point)&0x3f;	//取得pSrcBuff的高point位  
			break;
		case 4:
            iIndex = ((*pSrcBuff)>>point)&0xf;	//取得pSrcBuff的高point位  
			break;
		case 6:
            iIndex = ((*pSrcBuff)>>point)&0x3;	//取得pSrcBuff的高point位  
			break;
		}  
        iIndex += n;//与pSrcBuff-1的低point结合组成Base64的索引  
		assert(iIndex < 65);
        *pBuff64 ++ = Base64Encode[iIndex];	//由索引表得到pBuff64  
        n = ((*pSrcBuff)<<(6-point));		//计算源字符中的残留值  
        n = n&0x3f;		//确保n的最高两位为0  
        point += 2;		//源字符的拆分位置上升2  
        if(point == 8)	//如果拆分位置为8说明pSrcBuff有6位残留，可以组成一个完整的Base64字符，所以直接再组合一次  
        {  
            iIndex = (*pSrcBuff)&0x3f;		//提取低6位，这就是索引了  
            *pBuff64++ = Base64Encode[iIndex]; 
            n = 0;		//残留值为0  
            point = 2;	//拆分位置设为2  
        }  
        ++ pSrcBuff;  
    }  
    if(n != 0) *pBuff64++ = Base64Encode[n];  
	switch(iLen%3)//如果源字符串长度不是3的倍数要用'='补全  
	{
	case 2:
		*pBuff64 = '=';  
		break;
	case 1:
		*pBuff64++ = '=';  
        *pBuff64 = '='; 
		break;
	} 
}  
void XEmailSender::AddFilePath(char * pcFilePath)	//添加附件路径  
{  
    if(pcFilePath == NULL) return;
	for(unsigned int i = 0;i < m_pcFilePathList.size();++ i)  
    {  
		if(strcmp(m_pcFilePathList[i].c_str(),pcFilePath) == 0)	//如果已经存在就不用再添加了  
			return;  
    }  
	m_pcFilePathList.push_back(pcFilePath);  
}
void XEmailSender::DeleteFilePath(char* pcFilePath)//删除附件路径  
{    
	for(unsigned int i = 0;i < m_pcFilePathList.size();++ i)  
    {
		if(strcmp(m_pcFilePathList[i].c_str(),pcFilePath) == 0)//找到并删除它，如果没找到就算了  
        {  
			//for(int j = i;j < (int)(m_pcFilePathList.size()) - 1;++ j)
			//{
			//	m_pcFilePathList[j] = m_pcFilePathList[j + 1];
			//} 
			//m_pcFilePathList.pop_back();
			m_pcFilePathList.erase(m_pcFilePathList.begin() + i);
            return;  
        }  
    }  
}    
int XEmailSender::GetFileData(const char* FilePath)  
{  
    if(FilePath == NULL) return 0;  
	//从文件中读取数据
	FILE *fp = NULL;
	if((fp = fopen(FilePath,"rb")) == NULL) return 0;
	int len = XFile::getFileLen(fp);
	if(len <= 0)
	{
		fclose(fp);
		return 0;
	}
	m_pcFileBuff = XMem::createArrayMem<char>(len + 1);
	if(m_pcFileBuff == NULL)
	{
		fclose(fp);
		return 0;
	}
	fread(m_pcFileBuff,len,1,fp);
	fclose(fp);
    return len;  
} 
bool XEmailSender::CReateSocket(SOCKET &sock)  
{  
    WORD wVersionRequested;  
    WSADATA wsaData;  
    wVersionRequested = MAKEWORD( 2, 2 );  
    if(WSAStartup(wVersionRequested,&wsaData) != 0) return false;  
    if(LOBYTE(wsaData.wVersion) != 2 ||  
        HIBYTE(wsaData.wVersion) != 2 )  
    {  
        WSACleanup( );  
        return false;   
    }  
    sock = socket(AF_INET,SOCK_STREAM,IPPROTO_IP);  
    if (sock == INVALID_SOCKET) return false;  
  
    sockaddr_in servaddr;  
    memset(&servaddr,0,sizeof(sockaddr_in));  
    servaddr.sin_family = AF_INET;  
    servaddr.sin_port = htons(25);//发邮件一般都是25端口  
    if(m_sMailInfo.m_pcIPName == "")  
    {  
		servaddr.sin_addr.s_addr = inet_addr(m_sMailInfo.m_pcIPAddr.c_str());//直接使用IP地址  
    }else  
    {  
        struct hostent *hp=gethostbyname(m_sMailInfo.m_pcIPName.c_str());//使用名称  
		if(hp == NULL) return false;
        servaddr.sin_addr.s_addr=*(int*)(*hp->h_addr_list);  
    }  
 
    return !(connect(sock,(sockaddr*)&servaddr,sizeof(servaddr)) == SOCKET_ERROR);  //建立连接    
}  
bool XEmailSender::Logon(SOCKET &sock)  
{//这里没有对返回结果进行判断可能会存在问题
    recv(sock,m_cReceiveBuff,1024,0);  
  
    memset(m_cSendBuff,0,sizeof(m_cSendBuff));  
    sprintf_s(m_cSendBuff,"HELO []\r\n");  
    //sprintf_s(m_cSendBuff,"HELO mailto:xiajia_1981@sina.cn\r\n");  
    send(sock,m_cSendBuff,strlen(m_cSendBuff),0);//开始会话  
	memset(m_cReceiveBuff,0,1024);
    recv(sock,m_cReceiveBuff,1024,0);  
    if(m_cReceiveBuff[0]!='2' || m_cReceiveBuff[1]!='5' || m_cReceiveBuff[2]!='0')  
		return false;  

	memset(m_cSendBuff,0,sizeof(m_cSendBuff));  
    sprintf_s(m_cSendBuff,"AUTH LOGIN\r\n");  
    send(sock,m_cSendBuff,strlen(m_cSendBuff),0);//请求登录  
	memset(m_cReceiveBuff,0,1024);
    recv(sock,m_cReceiveBuff,1024,0);  
    if(m_cReceiveBuff[0]!='3' || m_cReceiveBuff[1]!='3' || m_cReceiveBuff[2]!='4')  
		return false;  
    memset(m_cSendBuff,0,sizeof(m_cSendBuff));  
	char tmpStr[1024];
	int len = strlen(m_sMailInfo.m_pcUserName.c_str());
	if(len >= 1024) return false;
	memcpy(tmpStr,m_sMailInfo.m_pcUserName.c_str(),len);
    Char2Base64(m_cSendBuff,tmpStr,len);  
    m_cSendBuff[strlen(m_cSendBuff)]='\r';  
    m_cSendBuff[strlen(m_cSendBuff)]='\n';  
    send(sock,m_cSendBuff,strlen(m_cSendBuff),0);//发送用户名  
	memset(m_cReceiveBuff,0,1024);
    recv(sock,m_cReceiveBuff,1024,0);  
    if(m_cReceiveBuff[0]!='3' || m_cReceiveBuff[1]!='3' || m_cReceiveBuff[2]!='4')  
	return false;
	  
    memset(m_cSendBuff,0,sizeof(m_cSendBuff));  
	len = strlen(m_sMailInfo.m_pcUserPassWord.c_str());
	if(len >= 1024) return false;
	memcpy(tmpStr,m_sMailInfo.m_pcUserPassWord.c_str(),len);
    Char2Base64(m_cSendBuff,tmpStr,len);  
    m_cSendBuff[strlen(m_cSendBuff)]='\r';  
    m_cSendBuff[strlen(m_cSendBuff)]='\n';  
    send(sock,m_cSendBuff,strlen(m_cSendBuff),0);//发送用户密码  
	memset(m_cReceiveBuff,0,1024);
    recv(sock,m_cReceiveBuff,1024,0);  
    if(m_cReceiveBuff[0]!='2' || m_cReceiveBuff[1]!='3' || m_cReceiveBuff[2]!='5')  
		return false;   
    return true;//登录成功  
}   
bool XEmailSender::SendHead(SOCKET &sock)  
{    
    memset(m_cSendBuff,0,sizeof(m_cSendBuff));  
	sprintf_s(m_cSendBuff,"MAIL FROM:<%s>\r\n",m_sMailInfo.m_pcSender.c_str());  
      
    if(send(sock,m_cSendBuff,strlen(m_cSendBuff),0) != strlen(m_cSendBuff))  
		return false;  
    recv(sock,m_cReceiveBuff,1024,0);  
  
    memset(m_cSendBuff,0,sizeof(m_cSendBuff));  
	sprintf_s(m_cSendBuff,"RCPT TO:<%s>\r\n",m_sMailInfo.m_pcReceiver.c_str());  
    if(send(sock,m_cSendBuff,strlen(m_cSendBuff),0) != strlen(m_cSendBuff))  
		return false;   
    recv(sock,m_cReceiveBuff,1024,0);  
  
    memset(m_cSendBuff,0,sizeof(m_cSendBuff));  
    memcpy(m_cSendBuff,"DATA\r\n",strlen("DATA\r\n"));  
    if(send(sock,m_cSendBuff,strlen(m_cSendBuff),0) != strlen(m_cSendBuff))  
		return false;  
    recv(sock,m_cReceiveBuff,1024,0);  
  
    memset(m_cSendBuff,0,sizeof(m_cSendBuff));  
	sprintf_s(m_cSendBuff,"From:\"%s\"<%s>\r\n",m_sMailInfo.m_pcSenderName.c_str(),m_sMailInfo.m_pcSender.c_str());  
	sprintf_s(&m_cSendBuff[strlen(m_cSendBuff)],150,"To:\"%s\"<%s>\r\n",m_sMailInfo.m_pcReceiverName.c_str(),m_sMailInfo.m_pcReceiver.c_str());  
	sprintf_s(&m_cSendBuff[strlen(m_cSendBuff)],150,"Subject:%s\r\nMime-Version: 1.0\r\nContent-Type: multipart/mixed;   boundary=\"INVT\"\r\n\r\n",m_sMailInfo.m_pcTitle.c_str());  
    return (send(sock,m_cSendBuff,strlen(m_cSendBuff),0) == strlen(m_cSendBuff));
}  
bool XEmailSender::SendFileBody(SOCKET &sock)  
{  
    const char* filePath;  
    int rt;  
    int len;  
    int pt=0;   
	for(unsigned int i = 0;i < m_pcFilePathList.size();++ i)  
    {  
        pt = 0;  
		filePath = m_pcFilePathList[i].c_str();  
        len = GetFileData(filePath);  
		std::string fileName = XFile::getFilenameFormPath(filePath);
  
		sprintf_s(m_cSendBuff,"--INVT\r\nContent-Type: application/octet-stream;\r\n  name=\"%s\"\r\nContent-Transfer-Encoding: base64\r\nContent-Disposition: attachment;\r\n  filename=\"%s\"\r\n\r\n",fileName.c_str(),fileName.c_str());  
        send(sock,m_cSendBuff,strlen(m_cSendBuff),0);  
        while (pt<len)  
        {  
            memset(m_cSendBuff,0,sizeof(m_cSendBuff));  
            Char2Base64(m_cSendBuff,&m_pcFileBuff[pt],min(len-pt,3000));  
            m_cSendBuff[strlen(m_cSendBuff)]='\r';  
            m_cSendBuff[strlen(m_cSendBuff)]='\n';  
            rt=send(sock,m_cSendBuff,strlen(m_cSendBuff),0);  
            pt+=min(len-pt,3000);  
            if(rt != strlen(m_cSendBuff))  
				return false;  
        }  
        if(len != 0) XMem::XDELETE_ARRAY(m_pcFileBuff);
	}
	return true;
}
bool XEmailSender::SendEnd(SOCKET &sock)  
{  
    sprintf_s(m_cSendBuff,"--INVT--\r\n.\r\n");  
    send(sock,m_cSendBuff,strlen(m_cSendBuff),0);  
 
    sprintf_s(m_cSendBuff,"QUIT\r\n");  
    send(sock,m_cSendBuff,strlen(m_cSendBuff),0);  
    closesocket(sock);  
    WSACleanup();  
    return true;  
}  
bool XEmailSender::SendMail(sMailInfo &smailInfo)  
{  
	m_sMailInfo = smailInfo;
    SOCKET sock;  
    if(!CReateSocket(sock))
	{
		LogNull("建立连接失败");
		return false;  //建立连接
	}
    if(!Logon(sock))
	{
		LogNull("登陆邮箱服务器失败");
		return false;  //登录邮箱
	}
    if(!SendHead(sock))
	{
		LogNull("发送邮件头失败");
		return false;  //发送邮件头 
	}
    if(!SendTextBody(sock))
	{
		LogNull("发送文本体失败");
		return false;  //发送邮件文本部分
	}
    if(!SendFileBody(sock))
	{
		LogNull("发送附件失败");
		return false;  //发送附件 
	}
    if(!SendEnd(sock))
	{
		LogNull("发送结束符失败");
		return false;  //结束邮件，并关闭sock  
	}
    return true;  
} 
#if !WITH_INLINE_FILE
#include "XEmail.inl"
#endif
}