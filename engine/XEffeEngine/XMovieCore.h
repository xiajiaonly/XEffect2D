#ifndef _JIA_MOVIECORE_
#define _JIA_MOVIECORE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.20
//--------------------------------
#include "XEffeEngine.h"
//这是影片播放类的基类，所有的影片播放类都是继承自这个基类
class _XMovieCore
{
protected:
public:
	virtual ~_XMovieCore() {;}
	virtual _XBool load(const char *filename,_XColorMode mode = COLOR_RGB,bool withVoice = true) = 0;	//载入指定影片
	virtual _XBool upDateFrame() = 0;			//返回视频画面是否更新
	virtual void draw() = 0;					//描绘
	virtual void updateTex(_XTexture &tex) = 0;
	virtual unsigned int * getTexture() = 0;
	//virtual void setPauseOrPlay(void) = 0;
	virtual void pause() = 0;
	virtual void play() = 0;
	virtual _XBool isPlaying()const = 0;
	virtual void closeClip(void) = 0;
	virtual void setLoop(_XBool isLoop) = 0;	//设置是否循环
	virtual _XBool getIsLoop()const = 0;	//获取是否循环
	virtual _XBool getIsEnd()const = 0;	//获取是否播放完成
	virtual double getCurrentPlayingTime() const = 0;
};
template<typename T>
class _XMovie
{
private:
	_XMovieCore *m_pObj;
public:
	_XMovie(){m_pObj = createMem<T>();}
	~_XMovie(){XDELETE(m_pObj);}
	_XBool load(const char *filename) {return m_pObj->load(filename);}
	_XBool upDateFrame() {return m_pObj->upDateFrame();}
	void draw() {m_pObj->draw();}
	void updateTex(_XTexture &tex) {m_pObj->updateTex(tex);}
	unsigned int * getTexture() {return m_pObj->getTexture();}
//	void setPauseOrPlay(void) {m_pObj->setPauseOrPlay();}
	virtual void pause(){m_pObj->pause();}
	virtual void play(){m_pObj->play();}
	virtual _XBool isPlaying(){return m_pObj->isPlaying();}
	void closeClip(void) {m_pObj->closeClip();}
	void setLoop(_XBool isLoop) {m_pObj->setLoop(isLoop);}
	_XBool getIsLoop() {return m_pObj->getIsLoop(isLoop);}	//获取是否循环
	_XBool getIsEnd() {return m_pObj->getIsEnd(isLoop);};	//获取是否播放完成
	double getCurrentPlayingTime() const {return m_pObj->getCurrentPlayingTime();};
};
#endif