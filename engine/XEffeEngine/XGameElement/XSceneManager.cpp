#include "XStdHead.h"
#include "XSceneManager.h"
#include "XScene.h"
#include "../XShaderGLSL.h"
#include "../XSprite.h"
namespace XE{
XSceneManager::XSceneManager()
	:m_currentScene(NULL)
	,m_futureScene(NULL)
	,m_isChange(false)
{
	m_sceneFbo = XMem::createMem<XFBO>();
	m_sprite = XMem::createMem<XSprite>();
	if(m_sceneFbo == NULL) LogStr("Mem Error!");
	else m_sceneFbo->init(getWindowWidth(),getWindowHeight());
	if(m_sprite == NULL) LogStr("Mem Error!");
	else m_sprite->init(getWindowWidth(),getWindowHeight(),0);
}
XSceneManager::~XSceneManager()
{
	XMem::XDELETE(m_sceneFbo);
	XMem::XDELETE(m_sprite);
}
void XSceneManager::update(float dt)
{
	if(m_isChange)
	{
		if(m_currentScene != NULL) m_currentScene->update(dt);
		if(m_futureScene != NULL) m_futureScene->update(dt);
		//下面是场景变换的相关动作
		m_timer += dt * m_changeSpeed;
		if(m_timer >= 1.0f)
		{//场景变换完成
			m_timer = 1.0f;
			m_isChange = false;
			if(m_currentScene != NULL) m_currentScene->exited();
			if(m_futureScene != NULL) m_futureScene->entered();
			m_currentScene = m_futureScene;
			m_futureScene = NULL;
		}
	}else
	{
		if(m_currentScene != NULL) m_currentScene->update(dt);
	}
}
void XSceneManager::draw()
{
	if(m_isChange)
	{//这里需要使用FBO来进行过度，目前尚未实现
		switch(m_changeMode)
		{
		case SCENE_MODE_NULL:
			if(m_currentScene != NULL) m_currentScene->draw();
			break;
		case SCENE_MODE_FADE_IN:	//场景淡入
			m_sceneFbo->useFBO();
			XEG.clearScreen(XFColor(0.0f,0.0f,0.0f,1.0f));
			if(m_futureScene != NULL) m_futureScene->draw();
			m_sceneFbo->removeFBO();

			if(m_currentScene != NULL) m_currentScene->draw();

			m_sprite->setAlpha(m_timer);
			m_sprite->draw(m_sceneFbo->getTexture(0));
			break;
		case SCENE_MODE_FADE_OUT:	//场景淡出
			m_sceneFbo->useFBO();
			XEG.clearScreen(XFColor(0.0f,0.0f,0.0f,1.0f));
			if(m_currentScene != NULL) m_currentScene->draw();
			m_sceneFbo->removeFBO();

			if(m_futureScene != NULL) m_futureScene->draw();

			m_sprite->setAlpha(1.0f - m_timer);
			m_sprite->draw(m_sceneFbo->getTexture(0));
			break;
		case SCENE_MODE_BLACK:		//淡黑
			if(m_timer < 0.5f)
			{
				if(m_currentScene != NULL) m_currentScene->draw();
				XRender::drawFillBox(0.0f,0.0f,getWindowWidth(),getWindowHeight(),0,0,0,m_timer * 2.0f);
			}else
			{
				if(m_futureScene != NULL) m_futureScene->draw();
				XRender::drawFillBox(0.0f,0.0f,getWindowWidth(),getWindowHeight(),0,0,0,(1.0f - m_timer) * 2.0f);
			}
			break;
		case SCENE_MODE_WHITE:		//淡白
			if(m_timer < 0.5f)
			{
				if(m_currentScene != NULL) m_currentScene->draw();
				XRender::drawFillBox(0.0f,0.0f,getWindowWidth(),getWindowHeight(),1.0f,1.0f,1.0f,m_timer * 2.0f);
			}else
			{
				if(m_futureScene != NULL) m_futureScene->draw();
				XRender::drawFillBox(0.0f,0.0f,getWindowWidth(),getWindowHeight(),1.0f,1.0f,1.0f,(1.0f - m_timer) * 2.0f);
			}
			break;
		}
	}else
	{
		if(m_currentScene != NULL) m_currentScene->draw();
	}
}
bool XSceneManager::goToScene(const std::string &name,XSceneChangeMode mode,float time)	//设置场景更换
{
	if(m_isChange) return false;	//正在变换场景的时候不能变换场景
	XScene * tmp = getScene(name);
	if(tmp == NULL || tmp == m_currentScene) return false;	//不允许的场景切换方式
	if(time <= 0.0f) time = 1.0f; 
	switch(mode)
	{
	case SCENE_MODE_NULL:
		if(m_currentScene != NULL)
		{
			m_currentScene->willExit();
			m_currentScene->exited();
		}
		m_currentScene = tmp;
		m_currentScene->willEnter();
		m_currentScene->entered();
		break;
	case SCENE_MODE_FADE_IN:	//场景淡入
	case SCENE_MODE_FADE_OUT:	//场景淡出
	case SCENE_MODE_BLACK:		//淡黑
	case SCENE_MODE_WHITE:		//淡白
		m_isChange = true;
		m_changeMode = mode;			//变化模式
		m_timer = 0.0f;					//发生变化的计时
		m_changeSpeed = 1.0f / time;	//场景变化的速度
		m_futureScene = tmp;
		m_futureScene->willEnter();
		if(m_currentScene != NULL) m_currentScene->willExit();
		break;
	}
	return true;
}
bool XSceneManager::addScene(XScene* newScene)			//添加场景
{
	if(newScene == NULL) return false;
	if(getScene(newScene->getSceneName()) != NULL) return false;
	m_scenes.insert(std::pair<std::string,XScene*>(newScene->getSceneName(),newScene));
	return true;
}
void XSceneManager::input(const XInputEvent &event) 
{
	if(m_isChange)
	{
		if(m_currentScene != NULL) m_currentScene->input(event);
		if(m_futureScene != NULL) m_futureScene->input(event);
	}else
	{
		if(m_currentScene != NULL) m_currentScene->input(event);
	}
}
std::string XSceneManager::getCurrentSceneName()const
{
	if(m_currentScene == NULL) return "";
	return m_currentScene->getSceneName();
}

#if !WITH_INLINE_FILE
#include "XSceneManager.inl"
#endif
}