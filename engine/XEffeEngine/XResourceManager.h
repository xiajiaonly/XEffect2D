#ifndef _JIA_XRESOURCEMANAGER_
#define _JIA_XRESOURCEMANAGER_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.1.1
//--------------------------------
//这是一个资源管理器的类，是所有资源的最上层管理器
//#include <vector>
#include <list>
#include "XBasicFun.h"
#include "XSoundCommon.h"
//资源的类型
enum _XResourceType
{
	RESOURCE_TYPE_NULL,			//资源的类型
	RESOURCE_TYPE_TEXTURE,		//贴图的资源
	RESOURCE_TYPE_SOUND,		//音效的资源
	RESOURCE_TYPE_MUSIC,		//声音的资源(尚未实现)
	//控件的资源也需要被资源管理器管理
	RESOURCE_TYPE_XBUTTON_TEX,	//按钮的资源
	RESOURCE_TYPE_XCHECK_TEX,	//按复选框的资源
	RESOURCE_TYPE_XEDIT_TEX,	//按复选框的资源
	RESOURCE_TYPE_XSLIDER_TEX,	//按复选框的资源
	RESOURCE_TYPE_XPROGRESS_TEX,	//按复选框的资源
	RESOURCE_TYPE_XMULTILIST_TEX,	//按复选框的资源
	RESOURCE_TYPE_XCOMBO_TEX,	//按复选框的资源
	RESOURCE_TYPE_XDIRLIST_TEX,	//按复选框的资源
};
class _XResourceTex
{
public:
	unsigned int m_texID;
	int m_width;
	int m_height;
	_XResourceTex()
		:m_texID(0)
	{}
	~_XResourceTex() {release();}
	void release();
};
class _XResourceSound
{
public:
	void *m_handle;		//音效资源的ID
	_XResourceSound()
		:m_handle(NULL)
	{}
	~_XResourceSound(){release();}
	void release()
	{
		if(m_handle != NULL)
		{
			_XSoundHandle.clearSound(m_handle);
			m_handle = NULL;
		}
	}
};
//这个类的指针不能直接用指针赋值，需要调用资源管理器的接口来赋值
class _XResourceInfo
{
private:
	_XBool m_isInited;		//资源是否已经载入
	void release();			//资源的释放函数
public:
	int m_ID;				//资源的ID
	char *m_name;			//资源的名字，不区分大小写的资源文件以及路径名
	_XResourceType m_type;	//资源的类型
	int m_counter;			//资源的引用计数器

	void *m_pointer;		//资源实体的指针

	_XBool load(_XResourcePosition resoursePosition);			//资源的载入函数
	_XBool isLoaded() const {return m_isInited;}
	_XResourceInfo()
		:m_isInited(XFalse)
		,m_name(NULL)
		,m_pointer(NULL)
		,m_counter(0)
		,m_type(RESOURCE_TYPE_NULL)
	{
		static int index = 0;
		m_ID = index ++;
	}
	//重载赋值操作符、拷贝构造函数和析构函数
	_XResourceInfo(_XResourceInfo &temp)
	{
		m_isInited = temp.m_isInited;
		m_ID = temp.m_ID;				//资源的ID
		m_name = temp.m_name;			//资源的名字，不区分大小写的资源文件以及路径名
		m_type = temp.m_type;	//资源的类型
		++ m_counter;			//资源的引用计数器
		m_pointer = temp.m_pointer;		//资源实体的指针
	}
	~_XResourceInfo()
	{
		if(m_counter <= 0) release();
	}
	_XResourceInfo &operator= (const _XResourceInfo & temp)
	{
		if(this == & temp) return * this;
		m_isInited = temp.m_isInited;
		m_ID = temp.m_ID;				//资源的ID
		m_name = temp.m_name;			//资源的名字，不区分大小写的资源文件以及路径名
		m_type = temp.m_type;	//资源的类型
		++ m_counter;			//资源的引用计数器
		m_pointer = temp.m_pointer;		//资源实体的指针
		return * this;
	}
};
//主要是为了解决同样的资源重复载入的问题，目前只完成图项部分
class _XResourceManager	
{
	//+++++++++++++++++++++++++++++++++++++++++++
	//下面需要将其设计为Singleton模式
protected:
	_XResourceManager()
		:m_resourceSum(0)
	{
	}
	_XResourceManager(const _XResourceManager&);
	_XResourceManager &operator= (const _XResourceManager&);
	virtual ~_XResourceManager()
	{
		release();
	}
public:
	static _XResourceManager& GetInstance()
	{
		static _XResourceManager m_instance;
		return m_instance;
	}
	//-------------------------------------------
private:
	//std::vector<_XResourceInfo *> m_resourceBuff;	//资源的堆栈
	std::list<_XResourceInfo *> m_resourceBuff;
	int m_resourceSum;			//目前资源的数量
	void release();				//释放所有的资源
private:
	_XBool isLoaded(const char * name);	//判断指定资源是否已经载入
	_XResourceInfo * isLoad(const char * name);
public:
	_XResourceInfo *loadResource(const char * name,_XResourceType type,_XResourcePosition resoursePosition);	//载入指定资源
	_XBool releaseResource(const _XResourceInfo *p);	//释放一个资源
	_XResourceInfo *copyResource(_XResourceInfo *p)
	{
		if(p != NULL) ++ p->m_counter;
		return p;
	}
};

#define _XResourceMng _XResourceManager::GetInstance()

#endif