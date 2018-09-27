#ifndef _JIA_XSCENEMANAGER_
#define _JIA_XSCENEMANAGER_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.5.21
//--------------------------------
#include <map>
namespace XE{
class XScene;
enum XSceneChangeMode
{//场景变换的模式
	SCENE_MODE_NULL,		//直接变换没有任何效果
	SCENE_MODE_FADE_IN,		//场景淡入
	SCENE_MODE_FADE_OUT,	//场景淡出
	SCENE_MODE_BLACK,		//淡黑
	SCENE_MODE_WHITE,		//淡白
	//TODO:你可以在这里添加新的场景过度模式
};
class XFBO;
class XSprite;
class XSceneManager
{
protected:
	XSceneManager();
	XSceneManager(const XSceneManager&);
	XSceneManager &operator= (const XSceneManager&);
	virtual ~XSceneManager();
public:
	static XSceneManager& getInstance()
	{
		static XSceneManager m_instance;
		return m_instance;
	}
public:
	void update(float dt);
	void draw();
	
	bool addScene(XScene* newScene);			//添加场景
	bool goToScene(const std::string& name,XSceneChangeMode mode = SCENE_MODE_NULL,float time = 1.0f);	//设置场景更换
	bool isChanged()const{return !m_isChange;}	//判断场景是否已经变换完成

	void input(const XInputEvent &event);
	unsigned int getNumScenes()const{return m_scenes.size();}
	XScene* getCurrentScene(){return m_currentScene;}
	XScene* getScene(const std::string& name);
	const std::string& getCurrentSceneName()const;
	
private:
	std::map <std::string,XScene *> m_scenes; //所有场景的列表
			
	XScene* m_currentScene;		//当前场景
	XScene* m_futureScene;			//接下来的场景

	XSceneChangeMode m_changeMode;	//变化模式
	float m_timer;					//发生变化的计时
	float m_changeSpeed;			//场景变化的速度

	XFBO *m_sceneFbo;
	XSprite *m_sprite;
	bool m_isChange;				//是否在发生变化
};
#if WITH_INLINE_FILE
#include "XSceneManager.inl"
#endif
#define XSManager XSceneManager::getInstance()
}
#endif