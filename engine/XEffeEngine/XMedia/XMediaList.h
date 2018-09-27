#ifndef _JIA_XMEDIALIST_
#define _JIA_XMEDIALIST_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.8.4
//--------------------------------
#include <vector>
namespace XE{
//这是关于播放列表的一些定义

//播放列表的循环模式
enum XMediaListPlayerMode
{
	PLAY_MODE_SERIAL,	//顺序播放
	PLAY_MODE_RAND,		//随机播放
};

//播放列表的单元设计
//头停顿时间 单位ms
//循环次数 -1无限循环 0不循环 1循环一次 n循环n次
//循环头停顿时间 单位ms
//播放视屏文件的路径
//循环尾停顿时间 单位ms
//尾停顿时间 单位ms

struct  XMediaListItem
{
	int m_headDelayTime;	//ms
	int m_loopTimes;		//循环次数
	int m_loopHeadDelayTime;	//ms
	std::string m_videoFilename;
	int m_loopEndDelayTime;	//ms
	int m_endDelayTime;		//结束停顿的时间
	bool m_stopAtEnd;		//是否在结束完成之后停顿
	XMediaListItem()
		:m_headDelayTime(0)
		,m_loopTimes(0)
		,m_loopHeadDelayTime(0)
		,m_videoFilename("")
		,m_loopEndDelayTime(0)
		,m_endDelayTime(0)
		,m_stopAtEnd(false)
	{}
};
#define DEFAULT_PLAY_LIST_FILENAME "playList.xml"
class XMediaList
{
private:
	std::vector<XMediaListItem> m_playList;
	XMediaListPlayerMode m_playMode;	//尚未实现
	int m_loopTimes;					//尚未实现
public:
	bool loadFromFile(const char * filename = NULL);	//从文件中读取列表
	bool saveToFile(const char * filename = NULL);		//将列表写入到文件中
	bool checkList() { return true; }		//检查列表数据是否正确
	void addAItem2List(const XMediaListItem & c) { m_playList.push_back(c); }
	XMediaListPlayerMode getPlayMode()const { return m_playMode; }
	int getLoopTimes()const { return m_loopTimes; }
	unsigned int getItemSum() { return m_playList.size(); }
	const XMediaListItem * getItem(int index)
	{
		if (index < 0 || index >= m_playList.size()) return NULL;
		return &m_playList[index];
	}

	XMediaList()
		:m_loopTimes(0)
		, m_playMode(PLAY_MODE_SERIAL)
	{}
};
}
#endif