#include "XStdHead.h"
#include "XVideoSynch.h"
#include "XEffeEngine.h"
namespace XE{
DWORD WINAPI XBroadcastUdp::recvThread(void * pParam)
{
	XBroadcastUdp &par = *(XBroadcastUdp *)pParam;
	par.m_recvThreadState = STATE_START;

	fd_set readfds;
	timeval timeout;
	int testError;
	int nRecvLength;
	unsigned char szRecvBuffer[4096];
	unsigned char data[4096];
	int datalen;
	sockaddr_in addrPeer;
	int nSize = sizeof(addrPeer);
	int prjNameLen;
	std::string projectName;
	int vidNameLen;
	std::string videoName;
	int frameIndex;
	while(par.m_recvThreadState != STATE_SET_TO_END)
	{
		timeout.tv_sec = 0;
		timeout.tv_usec = 0;
		FD_ZERO(&readfds);
		FD_SET(par.m_recvSocket,&readfds);
		testError = select(FD_SETSIZE,&readfds,NULL,NULL,&timeout);
		if(testError > 0 && FD_ISSET(par.m_recvSocket,&readfds))
		{//可以接受数据
			FD_CLR(par.m_recvSocket,&readfds);
			nRecvLength = recvfrom(par.m_recvSocket,(char *)szRecvBuffer,4096,0,(sockaddr*)&addrPeer,&nSize);
			if(nRecvLength > 0)
			{//接收到数据,下面对数据进行处理
				if(par.checkData(szRecvBuffer,nRecvLength))
				{//数据校验正确下面处理数据
					par.decSpecialChar(szRecvBuffer,nRecvLength,data,datalen);
					//下面检验数据
					memcpy(&frameIndex,data + datalen - 6,sizeof(frameIndex));
					memcpy(&prjNameLen,data + 1,sizeof(prjNameLen));
					memcpy(&vidNameLen,data + 1 + sizeof(prjNameLen) + prjNameLen,sizeof(vidNameLen));
					data[1 + sizeof(prjNameLen) + prjNameLen] = '\0';
					projectName = (char *)(data + 1 + sizeof(prjNameLen));
					data[datalen - 6] = '\0';
					videoName = (char *)(data + 1 + sizeof(prjNameLen) + prjNameLen + sizeof(vidNameLen));
				//	printf("recv:\n%s\n%s\n%d\n",projectName.c_str(),videoName.c_str(),frameIndex);
					if(projectName == par.m_name)
					{
						if(frameIndex >= 0)
						{
							par.m_curVideoName = videoName;
							par.m_curFrameIndex = frameIndex;
							par.m_mutex.Lock();
							par.m_timer.start();
							par.m_delayTimer = frameIndex;
							par.m_mutex.Unlock();
							if(par.m_commandFun != NULL) par.m_commandFun(par.m_pClass,STATE_NORMAL);
					//		printf("%d\n",frameIndex);
						}else
						{
							par.m_curVideoName = videoName;
							if(par.m_commandFun != NULL) par.m_commandFun(par.m_pClass,frameIndex);
							printf("%d\n",frameIndex);
						}
					}
				}
			}
		}
		Sleep(1);
	}
	par.m_recvThreadState = STATE_END;
	return 1;
}
bool XBroadcastUdp::checkData(const unsigned char * p,int len)
{
	if(len < 3) return false;
	if(p[0] != 0xcc)
	{
		printf("头错误!\n");
		return false;
	}
	if(p[len - 1] != 0xdd) 
	{
		printf("尾错误!\n");
		return false;
	}
	unsigned char tmpChar = 0x00;
	unsigned char tmpCharC;
	int tmpLen = len;
	if(p[len - 3] == 0xee) 
	{
		tmpLen -= 1;
		tmpCharC = p[len - 2] + 0xcc;
	}else
	{
		tmpCharC = p[len - 2];
	}
	for(int i = 1;i < tmpLen - 2;++ i)
	{
		if(p[i] == 0xee)
		{
			tmpChar += p[i + 1] + 0xcc;
			++ i;
		}else
		{
			tmpChar += p[i];
		}
		if(p[i] == 0xcc || p[i] == 0xdd)
		{
			printf("数据内容错误!\n");
			return false;
		}
	}
	if(tmpChar != tmpCharC)
	{
		printf("数据校验错误!\n");
		return false;
	}
	return true;
}
bool XBroadcastUdp::readFromCFG(const std::string &filename)
{
	int temp;
	TiXmlDocument doc(filename);
	if(!doc.LoadFile()) return false;
	TiXmlNode *rootNode = NULL;
	rootNode = doc.FirstChild("root");
	if(rootNode == NULL) return false;
//	TiXmlNode *keyNode = NULL;

	if(!XXml::getXmlAsString(rootNode,"projectName",m_name)) return false;
	if(!XXml::getXmlAsInt(rootNode,"isServer",temp)) return false;
	if(temp == 0) m_role = VS_ROLE_CLIENT;
	else m_role = VS_ROLE_SERVER;
	if(!XXml::getXmlAsInt(rootNode,"port",m_port)) return false;

	return true;
}
bool XBroadcastUdp::init(const std::string &cfgFileName)
{
	if(m_isInited) return false;
	if(!readFromCFG(cfgFileName)) return false;	//读取配置文件
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2,2),&wsaData) != 0) return false;
	//下面根据配置文件建立连接
	switch(m_role)
	{
	case VS_ROLE_SERVER:	//服务器不停的发送同步数据
		{
			int bBroadcast = 1;
			sockaddr_in addrUDP;
			memset(&addrUDP,0,sizeof(addrUDP));
			addrUDP.sin_family  = AF_INET;
			addrUDP.sin_port  = htons(0);
			addrUDP.sin_addr.s_addr = htonl(INADDR_ANY);
			m_sendSocket = socket(AF_INET,SOCK_DGRAM,0);
			if(m_sendSocket < 0) return false;
			resetSocketAddr(m_sendSocket); 
			if(setsockopt(m_sendSocket,SOL_SOCKET,SO_BROADCAST,(char *)&bBroadcast,sizeof(bBroadcast)) == SOCKET_ERROR) return false;
			if(bind(m_sendSocket,(sockaddr*)&addrUDP,sizeof(addrUDP)) < 0) return false;	//接收端口

			memset(&m_addrUDP,0,sizeof(m_addrUDP)); 
			m_addrUDP.sin_family = AF_INET;
			m_addrUDP.sin_port = htons(m_port);
			m_addrUDP.sin_addr.s_addr = htonl(INADDR_BROADCAST);   
		}
		break;
	case VS_ROLE_CLIENT:	//客户端获取同步数据
		{
			sockaddr_in addrUDP;
			memset(&addrUDP,0,sizeof(addrUDP)); 
			addrUDP.sin_family = AF_INET;
			addrUDP.sin_port = htons(m_port);
			addrUDP.sin_addr.s_addr = htonl(INADDR_ANY);
			m_recvSocket = socket(AF_INET,SOCK_DGRAM,0);
			if(m_recvSocket < 0) return false;
			resetSocketAddr(m_recvSocket);
			if(bind(m_recvSocket,(sockaddr*)&addrUDP,sizeof(addrUDP)) < 0) return false;	//接收端口
			m_recvThreadState = STATE_BEFORE_START;
			if(CreateThread(0,0,recvThread,this,0,NULL) == 0) return false;	//连接线程建立失败
		}
		break;
	}

	m_isInited = true;
	return true;
}
bool XBroadcastUdp::sendInfo()	//向网络发送同步数据
{
	return (sendto(m_sendSocket,(char *)m_dataSend,m_sendLen,0,(sockaddr *)&m_addrUDP,sizeof(m_addrUDP)) >= m_sendLen);
}
//数据传输协议
//头	nameLen		name	videoNameLen	videoName	frameIndex	check	尾
//0xcc	4Bytes		name	4Bytes			name		4Bytes		1Byte	0xdd
//frameIndex可以作为功能字段
void XBroadcastUdp::setCurVideo(const std::string &filename)
{
	m_curVideoName = filename;
	int len = 1 + 4 + m_name.length() + 4 + m_curVideoName.length() + 4 + 1 + 1;
	if(m_dataLen < (len << 1) || m_data == NULL)
	{
		XMem::XDELETE_ARRAY(m_data);
		m_data = XMem::createArrayMem<unsigned char>(len);
		m_dataLen = len;
		m_dataSend = XMem::createArrayMem<unsigned char>(len << 1);
	}
	m_sendLen = len;
	//下面填充数据
	int offset = 0;
	m_data[0] = 0xcc;
	offset += 1;
	len = m_name.length();
	memcpy(m_data + offset,&len,sizeof(len));
	offset += sizeof(len);
	memcpy(m_data + offset,m_name.c_str(),len);
	offset += len;

	len = m_curVideoName.length();
	memcpy(m_data + offset,&len,sizeof(len));
	offset += sizeof(len);
	memcpy(m_data + offset,m_curVideoName.c_str(),len);
	offset += len;

	memcpy(m_data + offset,&m_curFrameIndex,sizeof(m_curFrameIndex));
	offset += sizeof(m_curFrameIndex);
	//计算校验位
	m_data[offset] = getCheck();
	offset += 1;
	m_data[offset] = 0xdd;
	offset += 1;
	addSpecialChar();
}
unsigned char XBroadcastUdp::getCheck()	//计算校验字符
{
	unsigned char tmpChar = 0x00;
	for(int i = 1;i < m_dataLen - 2;++ i)
	{
		tmpChar += m_data[i];
	}
	return tmpChar;
}
void XBroadcastUdp::setCurFrame(int frameIndex)
{
	if(m_role != VS_ROLE_SERVER || !m_isInited) return;
	m_curFrameIndex = frameIndex;
	bool ret = false;
	if(m_data != NULL)
	{
		memcpy(m_data + m_dataLen - 6,&m_curFrameIndex,sizeof(m_curFrameIndex));
		m_data[m_dataLen - 2] = getCheck();
		addSpecialChar();
		ret = sendInfo();
	}
}
void XBroadcastUdp::addSpecialChar()
{
	//下面对字符串进行转义整理
	m_dataSend[0] = 0xcc;	//头
	int index = 1;
	for(int i = 1;i < m_dataLen - 1;++ i)
	{
		if(m_data[i] == 0xcc ||
			m_data[i] == 0xdd ||
			m_data[i] == 0xee)
		{
			m_dataSend[index] = 0xee;
			m_dataSend[index + 1] = m_data[i] - 0xcc;
			index += 2;
		}else
		{
			m_dataSend[index] = m_data[i];
			++ index;
		}
	}
	m_dataSend[index] = 0xdd;//尾
	m_sendLen = index + 1;
}
void XBroadcastUdp::decSpecialChar(const unsigned char * inData,int inLen,unsigned char * outData,int &outLen)	//减去转义字符
{
	outLen = 0;
	for(int i = 0;i < inLen;++ i)
	{
		if(inData[i] != 0xee)
		{
			outData[outLen] = inData[i];
			++ outLen;
		}else
		{
			outData[outLen] = inData[i + 1] + 0xcc;
			++ i;
			++ outLen;
		}
	}
}
void XSimpleVideoSynch::closeClip(void) 
{
	m_movie.closeClip();
	if(m_udp.getRole() == VS_ROLE_SERVER)
		m_udp.setCurFrame(STATE_CLOSE_CLIP);
}
void XSimpleVideoSynch::commandFun(void *pClass,int cmd)
{
	XSimpleVideoSynch &pPar = *(XSimpleVideoSynch *)pClass;
	pPar.m_commandIndex = (XVideoSynchState)cmd;
}
 XBool XSimpleVideoSynch::updateFrame()			//返回视频画面是否更新
{
	if(m_udp.getRole() == VS_ROLE_SERVER)
	{//服务器
		if(m_isReadyToPlay)
		{
			m_playTimer += XEG.getLastFrameTime();
			if(m_playTimer > 500.0f)
			{
				m_isReadyToPlay = false;
				m_udp.setCurFrame(STATE_PLAY);	//发送预加载命令
				m_movie.play();
			}
		}else
		{
			bool tmpIsEnd = m_movie.getIsEnd();
			bool ret = m_movie.updateFrame();
			if(m_udp.getCurFrameIndex() != m_movie.getCurPlayTime() && !tmpIsEnd)
			{
				m_udp.setCurFrame(m_movie.getCurPlayTime());
				if(m_movie.getIsEnd())
					m_udp.setCurFrame(-1);
			}
			return ret;
		}
	}else
	{//客户端
		switch(m_commandIndex)
		{
		case STATE_NORMAL:
			if(m_curVideoName != m_udp.getCurVideoName())
			{
				m_movie.closeClip();
				if(m_movie.load(m_udp.getCurVideoName().c_str(),COLOR_RGB,false))
				{
					m_curVideoName = m_udp.getCurVideoName();
					m_movie.play();
					m_movie.setAutoTime(false);
					m_movie.m_movieSprite->setClipRect(m_offset.x,m_offset.y,m_offset.x + m_size.x,m_offset.y + m_size.y);
					return m_movie.updateFrame();
				}else
				{
					printf("影片载入失败!");
					return XFalse;
				}
			}else
			{//下面使用高精度计时器的方法
				if(m_udp.getTimer() != m_movie.getCurPlayTime())
				{
					if(m_udp.getCurFrameIndex() < m_movie.getCurFrameIndex() && m_movie.getIsEnd())
						m_movie.replay();
					else
					{
						if(!m_movie.isPlaying())
							m_movie.play();
						m_movie.gotoTime(m_udp.getTimer());
					}
				}//bug当服务器结束之后，客户端不会重新开始
	//				if(m_udp.getTimer() > m_movie.getCurPlayTime())
	//				{
	//					m_movie.gotoTime(m_udp.getTimer());
	//				}else
	//				if(m_udp.getTimer() < m_movie.getCurPlayTime())
	//				{//重新播放
	//					m_movie.replay();
	//				}
				//if(m_udp.getCurFrameIndex() != m_movie.getNowPlayTime())
				//{
				//	m_movie.gotoTime(m_udp.getCurFrameIndex());
				//}
				return m_movie.updateFrame();
			}
			break;
		case STATE_PLAYING_END:
			break;
		case STATE_CLOSE_CLIP:
			m_movie.closeClip();
			m_curVideoName = "";
			m_commandIndex = STATE_WAITING;
			break;
		case STATE_LOAD:
			m_movie.closeClip();
			if(m_movie.load(m_udp.getCurVideoName().c_str(),COLOR_RGB,false))
			{
				m_curVideoName = m_udp.getCurVideoName();
				printf("影片载入:%s\n",m_udp.getCurVideoName().c_str());
			}else
				printf("影片载入失败:%s\n",m_udp.getCurVideoName().c_str());
			m_commandIndex = STATE_WAITING;
			break;
		case STATE_PLAY:
			if(!m_movie.isPlaying())
				m_movie.play();
			m_commandIndex = STATE_WAITING;
			break;
		case STATE_WAITING:	//等待状态
			break;
		}
	}
	return XFalse;
}
void XSimpleVideoSynch::play()
{
	if(m_udp.getRole() != VS_ROLE_SERVER) return;
	if(!m_movie.getIsEnd()) 
	{
		m_isReadyToPlay = true;
		m_playTimer = 0;
	}
	else m_movie.play();
}
#if !WITH_INLINE_FILE
#include "XVideoSynch.inl"
#endif
}