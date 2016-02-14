#ifndef _JIA_XVIDEOSYNCH_
#define _JIA_XVIDEOSYNCH_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.3
//--------------------------------
#include "XCritical.h"
#include "XThread.h"
#include "XNetWork\XNetCommon.h"
#include "XMedia\XMovieFfmpeg.h"
namespace XE{
enum XVideoSynchRole
{
	VS_ROLE_SERVER,	//同步的服务器
	VS_ROLE_CLIENT,	//同步的客户端
};
//数据传输协议
//头	nameLen		name	videoNameLen	videoName	frameIndex	check	尾
//0xcc	4Bytes		name	4Bytes			name		4Bytes		1Byte	0xdd
//0xcc头，0xdd尾，0xee转义
class XBroadcastUdp
{
private:
	bool m_isInited;			//是否完成初始化
	std::string m_name;			//同步的名称
	XVideoSynchRole m_role;	//角色
	int m_port;					//用于广播的端口

	//下面是用于通讯的数据
	int m_sendSocket;	//发送的套接字
	sockaddr_in m_addrUDP;
	unsigned char *m_data;
	unsigned char *m_dataSend;
	int m_dataLen;	//数据buff的大小
	int m_sendLen;	//需要发送的数据大小
	int m_recvSocket;	//接收的套接字

	int m_curFrameIndex;
	std::string m_curVideoName;

	bool readFromCFG(const std::string &filename);
	bool sendInfo();	//向网络发送同步数据
	void addSpecialChar();	//增加转义字符
	unsigned char getCheck();	//计算校验字符
	void decSpecialChar(const unsigned char * inData,int inLen,unsigned char * outData,int &outLen);	//减去转义字符

	XThreadState m_recvThreadState;
	static DWORD WINAPI recvThread(void * pParam);		//接收线程 
	bool checkData(const unsigned char * p,int len);

	//下面是同步时间
	XTimer m_timer;	//高精度计时器
	int m_delayTimer;	//计时器的延迟
	XCritical m_mutex;

	void *m_pClass;
	void (*m_commandFun)(void *,int);	//收到特殊命令时的回调函数
public:
	void setCallbackFun(void *pClass,void (*commandFun)(void *,int))
	{
		m_pClass = pClass;
		m_commandFun = commandFun;
	}
	int getTimer();	//获取毫秒级的时间
	XBroadcastUdp()
		:m_isInited(false)
		,m_curVideoName("")
		,m_curFrameIndex(0)
		,m_dataLen(0)
		,m_data(NULL)
		,m_dataSend(NULL)
		,m_pClass(NULL)
		,m_commandFun(NULL)
	{}
	~XBroadcastUdp(){release();}
	void release();
	bool init(const std::string &cfgFileName);
	void setCurFrame(int frameIndex);
	void setCurVideo(const std::string &filename);
	std::string getCurVideoName() const {return m_curVideoName;}
	int getCurFrameIndex() const {return m_curFrameIndex;}
	XVideoSynchRole getRole() const {return m_role;}
};
enum XVideoSynchState
{
	STATE_NORMAL = 0,		//普通状态
	STATE_PLAYING_END = -1,	//播放结束
	STATE_CLOSE_CLIP = -2,	//关闭影片
	STATE_LOAD = -3,			//预载入影片
	STATE_PLAY = -4,			//开始播放
	STATE_WAITING = -128,		//等待状态
};
//下面是简单视频同步的类
class XSimpleVideoSynch:public XMovieCore
{
private:
	bool m_isInited;
	XBroadcastUdp m_udp;
	XMovieFfmpeg m_movie;
	XVector2 m_offset;
	XVector2 m_size;
	std::string m_curVideoName;
	static void commandFun(void *,int);
	XVideoSynchState m_commandIndex;
	bool m_isReadyToPlay;	//是否准备播放，调用函数播放时，程序并不会立刻开始播放，而是需要等待客户端载入才能开始播放
	float m_playTimer;
public:
	virtual XBool getIsLoad()const{return m_movie.getIsLoad();}
	XBool init(const std::string &filename);
	void setOffset(const XVector2 &offset) {m_offset = offset;}
	void setSize(const XVector2 &size) {m_size = size;}

	virtual XBool load(const char *filename,XColorMode mode = COLOR_RGB,bool withVoice = true);
	virtual XBool updateFrame();			//返回视频画面是否更新
	virtual void draw(){m_movie.draw();}
	virtual void updateTex(XTexture &tex) {m_movie.updateTex(tex);}
	virtual unsigned int * getTexture() {return m_movie.getTexture();}
	//virtual void setPauseOrPlay(void) = 0;
	virtual void pause() {m_movie.pause();}
	virtual void play();
	virtual XBool isPlaying() const {return m_movie.isPlaying();}
	virtual void closeClip(void);
	virtual void setLoop(XBool isLoop) {m_movie.setLoop(isLoop);}	//设置是否循环
	virtual XBool getIsLoop()const {return m_movie.getIsLoop();}	//获取是否循环
	virtual XBool getIsEnd()const {return m_movie.getIsEnd();}	//获取是否播放完成
	virtual double getCurrentPlayingTime() const {return m_movie.getCurrentPlayingTime();}

	XSimpleVideoSynch()
		:m_isInited(false)
		,m_commandIndex(STATE_NORMAL)
		,m_isReadyToPlay(false)
	{}
	virtual ~XSimpleVideoSynch()
	{}
};
//下面是关于媒体播放器的相关数据结构的定义

//列表属性
//名称
//偏移
//尺寸
//循环、随机、顺序
//控制指令：暂停、播放、播放下一个，控制指令对应的osc设置
//列表开始播放是否发送osc指令以及指令设置
//列表结束播放是否发送osc指令以及指令设置

//列表项属性
//头暂停时间
//循环头暂停时间
//影片名称
//循环尾停顿时间
//尾暂停时间
//循环次数，-1：无限循环，0不循环，1循环一次……
//是否停在结尾
//开始播放时是否发送osc指令以及指令设置
//结束播放时是否发送osc指令以及指令设置
enum XOscDataType
{
	TYPE_INT32,
	TYPE_INT64,
	TYPE_FLOAT,
	TYPE_STIRNG,
};
struct XOscCommand
{//osc指令的结构
	bool isEnable;		//是否有效
	std::string ip;		//通讯的IP
	int port;			//通讯的端口号
	std::string addr;	//通讯的地址
	XOscDataType type;	//数据
	int valueI;
	float valueF;
	std::string valueS;
	XOscCommand()
		:isEnable(false)
	{}
};
struct XListItem
{//列表项的结构
	int beginDelay;			//头延迟单位ms
	int loopBeginDelay;		//循环头延迟
	std::string filename;	//文件名
	int loopEndDelay;		//循环尾延迟
	int endDelay;			//播放尾时间
	int loopTimes;			//循环次数
	bool stayAtEnd;			//播放完成之后停止
	XOscCommand cmdAtBegin;	//播放头发送的osc命令
	XOscCommand cmdAtEnd;		//播放尾发送的osc命令
	XListItem()
		:beginDelay(0)
		,loopBeginDelay(0)
		,loopEndDelay(0)
		,endDelay(0)
		,loopTimes(0)
		,stayAtEnd(false)
	{}
};
enum XVideoListMode
{
	MODE_ORDER,	//顺序播放
	MODE_RAND,	//随机播放
};
struct XVideoList
{//播放列表的结构
	bool isEnable;
	std::string name;		//列表名称
	XVector2 offset;		//视频偏移像素坐标
	XVector2 pixelSize;	//使用的视频的像素大小，裁剪的有效尺寸
	bool isLoop;			//是否循环
	XVideoListMode mode;	//循环模式
	XOscCommand cmdStop;	//停止播放接收的osc指令
	XOscCommand cmdPlay;	//开始播放接收的osc指令
	XOscCommand cmdNext;	//下一个接收的osc指令
	XOscCommand cmdAtBegin;	//列表开始播放发送的osc指令
	XOscCommand cmdAtEnd;		//列表播放完成发送的osc指令
	std::vector<XListItem> items;
	XVideoList()
		:isEnable(false)
	{}
};
class XMediaPlayer
{//媒体播放器的类(尚未完成)
private:
public:
};
#if WITH_INLINE_FILE
#include "XVideoSynch.inl"
#endif
}
#endif