#ifndef _JIA_XRESOURCEMANAGER_
#define _JIA_XRESOURCEMANAGER_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.1.1
//--------------------------------
//这是一个资源管理器的类，是所有资源的最上层管理器
#include <list>
#include "XCritical.h"

namespace XE{
//资源的类型
enum XResourceType
{
	RESOURCE_TYPE_NULL,			//资源的类型
	RESOURCE_TYPE_TEXTURE,		//贴图的资源
	RESOURCE_TYPE_SOUND,		//音效的资源
	RESOURCE_TYPE_MUSIC,		//声音的资源(尚未实现)
	//控件的资源也需要被资源管理器管理
	RESOURCE_TYPEXBUTTON_TEX,	//按钮的资源
	RESOURCE_TYPEXCHECK_TEX,	//按复选框的资源
	RESOURCE_TYPEXEDIT_TEX,	//按复选框的资源
	RESOURCE_TYPEXSLIDER_TEX,	//按复选框的资源
	RESOURCE_TYPEXPROGRESS_TEX,	//按复选框的资源
	RESOURCE_TYPEXMULTILIST_TEX,	//按复选框的资源
	RESOURCE_TYPEXCOMBO_TEX,	//按复选框的资源
	RESOURCE_TYPEXDIRLIST_TEX,	//按复选框的资源
	RESOURCE_TYPEXPASSWORDPAD_TEX,	//密码输入框的资源
};
class XResourceTex
{
public:
	unsigned int m_texID;
	int m_width;
	int m_height;
	XResourceTex()
		:m_texID(0)
	{}
	~XResourceTex() {release();}
	void release();
};
class XResourceSound
{
public:
	void *m_handle;		//音效资源的ID
	XResourceSound()
		:m_handle(NULL)
	{}
	~XResourceSound(){release();}
	void release();
};
//这个类的指针不能直接用指针赋值，需要调用资源管理器的接口来赋值
class XResourceInfo
{
private:
	XBool m_isInited;		//资源是否已经载入
	void release();			//资源的释放函数
public:
	int m_ID;				//资源的ID
	std::string m_resName;			//资源的名字，不区分大小写的资源文件以及路径名
	XResourceType m_type;	//资源的类型
	int m_counter;			//资源的引用计数器

	void *m_pointer;		//资源实体的指针

	XBool load(XResPos resPos, bool isInThread = false);			//资源的载入函数
	bool update(XResPos resPos, bool isInThread = false);	//更新资源
	XBool isLoaded() const { return m_isInited; }
	XResourceInfo()
		:m_isInited(XFalse)
		, m_pointer(NULL)
		, m_counter(0)
		, m_type(RESOURCE_TYPE_NULL)
	{
		static int index = 0;
		m_ID = index++;
	}
	//重载赋值操作符、拷贝构造函数和析构函数
	XResourceInfo(XResourceInfo &temp)
	{
		m_isInited = temp.m_isInited;
		m_ID = temp.m_ID;				//资源的ID
		m_resName = temp.m_resName;			//资源的名字，不区分大小写的资源文件以及路径名
		m_type = temp.m_type;	//资源的类型
		++m_counter;			//资源的引用计数器
		m_pointer = temp.m_pointer;		//资源实体的指针
	}
	~XResourceInfo()
	{
		if (m_counter <= 0) release();
	}
	XResourceInfo &operator= (const XResourceInfo & temp)
	{
		if (this == &temp) return *this;
		m_isInited = temp.m_isInited;
		m_ID = temp.m_ID;				//资源的ID
		m_resName = temp.m_resName;			//资源的名字，不区分大小写的资源文件以及路径名
		m_type = temp.m_type;	//资源的类型
		++m_counter;			//资源的引用计数器
		m_pointer = temp.m_pointer;		//资源实体的指针
		return *this;
	}
};
//主要是为了解决同样的资源重复载入的问题，目前只完成图项部分
class XResourceManager
{
public:
	XResourceManager()
		:m_resourceSum(0)
		, m_canReuse(true)
	{}
	virtual ~XResourceManager() { release(); }
protected:
	XResourceManager(const XResourceManager&);
	XResourceManager &operator= (const XResourceManager&);
private:
	XCritical m_mutex;	//确保多线程安全
	//std::vector<XResourceInfo *> m_resourceBuff;	//资源的堆栈
	std::list<XResourceInfo *> m_resourceBuff;
	int m_resourceSum;			//目前资源的数量
	void release();				//释放所有的资源
	bool m_canReuse;		//是否允许资源复用
private:
	XBool isLoaded(const char * name);	//判断指定资源是否已经载入
	XResourceInfo * isLoad(const char * name);
public:
	//是否允许资源复用，资源复用时，多线程资源更新必须要互斥，性能较低
	//如果不需要资源复用，则资源更新效率较高
	void setCanReuse(bool flag) { m_canReuse = flag; }
	XResourceInfo *loadResource(const char* name, XResourceType type,
		XResPos resPos, bool isInThread = false);	//载入指定资源
	bool updateRes(XResourceInfo* &res, const char* name,
		XResPos resPos, bool isInThread = false);
	XBool releaseResource(const XResourceInfo *p);	//释放一个资源
	XResourceInfo *copyResource(XResourceInfo *p)
	{
		if (!m_canReuse || p == nullptr) return nullptr;
		++p->m_counter;
		return p;
	}
};
}
#endif