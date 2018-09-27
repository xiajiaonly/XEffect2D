#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XResourceManager.h"
#include "XResourcePack.h"
#include "XControl/XControls.h"
#include "XMedia/XSoundCommon.h"
#include "XFile.h"
namespace XE{
void XResourceTex::release()
{
	if(glIsTexture(m_texID)) 
	{
#if WITH_ENGINE_STATISTICS
		XEG.decStaticsticsTexInfo(m_texID);
#endif
		LogNull("delete texture:%d",m_texID);
		glDeleteTextures(1,&m_texID);
	}
}
void XResourceSound::release()
{
	if(m_handle == NULL) return;
	XCurSndCore.clearSound(m_handle);
	m_handle = NULL;
}
bool XResourceInfo::update(XResPos resPos, bool isInThread)			//资源的载入函数
{
	if (!m_isInited) return false;	//防止重复的载入
	if (resPos == RES_SYS_DEF) resPos = getDefResPos();
	XResourceTex* temp = nullptr;
	switch (m_type)
	{
	case RESOURCE_TYPE_TEXTURE:
		temp = (XResourceTex*)m_pointer;
		if (temp == NULL) return XFalse;
		if (!XGL::TextureLoadEx(temp->m_texID, m_resName.c_str(), &temp->m_width,
			&temp->m_height, resPos, isInThread))
		{
			LogNull("Update Res:%s not match!", m_resName.c_str());
			return XFalse;
		}
		LogNull("Update Res:%s sucess", m_resName.c_str());
		return true;
	default:
		return false;
	}
	return true;
}
XBool XResourceInfo::load(XResPos resPos, bool isInThread)			//资源的载入函数
{
	if(m_isInited) return XTrue;	//防止重复的载入
	if(resPos == RES_SYS_DEF) resPos = getDefResPos();
	//更具不同的类型调用不同的载入函数
	std::string logstr = "";
	switch(m_type)
	{
	case RESOURCE_TYPE_NULL:
		return XFalse;
		break;
	case RESOURCE_TYPE_TEXTURE:
		{
			XResourceTex * temp = XMem::createMem<XResourceTex>();
			if(temp == NULL) return XFalse;
			if (!XGL::TextureLoadEx(temp->m_texID, m_resName.c_str(), &temp->m_width, &temp->m_height,
				resPos, isInThread))
			{
				XMem::XDELETE(temp);
				return XFalse;
			}
			logstr += XString::toString(temp->m_texID) + "|";
			m_pointer = temp;
		}
		break;
	case RESOURCE_TYPE_SOUND:
		{
			XResourceSound * temp = XMem::createMem<XResourceSound>();
			if(temp == NULL) return XFalse;
			switch(resPos)
			{
			case RES_LOCAL_FOLDER:	//从文件夹中读取资源
				if(!XCurSndCore.loadSound(m_resName.c_str(),temp->m_handle))
				{
					logstr += "Sound load error!";
					LogStr(logstr.c_str());
					XMem::XDELETE(temp);
					return XFalse;
				}
				break;
			case RES_LOCAL_PACK:	//从资源包中读取资源
				{
					int lengthTemp = XResPack.getFileLength(m_resName.c_str());
					unsigned char *p = XMem::createArrayMem<unsigned char>(lengthTemp + 1);
					if(p == NULL) 
					{
						XMem::XDELETE(temp);
						return XFalse;
					}

					XResPack.unpackResource(m_resName.c_str(),p);
					if(!XCurSndCore.loadSound(p,lengthTemp,temp->m_handle))
					{
						logstr += "Sound load error!";
						LogStr(logstr.c_str());
						XMem::XDELETE(temp);
						return XFalse;
					}
					XMem::XDELETE_ARRAY(p);
				}
				break;
			case RES_WEB:	//从网页中读取资源
				logstr += "Sound load error!";
				LogStr(logstr.c_str());
				XMem::XDELETE(temp);
				return XFalse;
				break;
			case RES_AUTO:	//依次读取资源
				{
				//packer
					int lengthTemp = XResPack.getFileLength(m_resName.c_str());
					unsigned char *p = XMem::createArrayMem<unsigned char>(lengthTemp + 1);
					if(p != NULL) 
					{
						XResPack.unpackResource(m_resName.c_str(),p);
						if(XCurSndCore.loadSound(p,lengthTemp,temp->m_handle))
						{
							XMem::XDELETE_ARRAY(p);
							break;
						}
						XMem::XDELETE_ARRAY(p);
					}
				//folder
					if(XCurSndCore.loadSound(m_resName.c_str(),temp->m_handle))
						break;
				//web
					logstr += "Sound load error!";
					LogStr(logstr.c_str());
					XMem::XDELETE(temp);
					return XFalse;
				}
				break;
			}
			m_pointer = temp;
		}
		break;
	case RESOURCE_TYPEXBUTTON_TEX:
		{
			XButtonSkin * temp = XMem::createMem<XButtonSkin>();
			if(temp == NULL) return XFalse;
			if(!temp->initEx(m_resName.c_str(),resPos))
			{
				XMem::XDELETE(temp);
				return XFalse;
			}
			m_pointer = temp;
		}
		break;
	case RESOURCE_TYPEXCHECK_TEX:
		{
			XCheckSkin * temp = XMem::createMem<XCheckSkin>();
			if(temp == NULL) return XFalse;
			if(!temp->initEx(m_resName.c_str(),resPos))
			{
				XMem::XDELETE(temp);
				return XFalse;
			}
			m_pointer = temp;
		}
		break;
	case RESOURCE_TYPEXEDIT_TEX:
		{
			XEditSkin * temp = XMem::createMem<XEditSkin>();
			if(temp == NULL) return XFalse;
			if(!temp->initEx(m_resName.c_str(),resPos))
			{
				XMem::XDELETE(temp);
				return XFalse;
			}
			m_pointer = temp;
		}
		break;
	case RESOURCE_TYPEXSLIDER_TEX:
		{
			XSliderSkin * temp = XMem::createMem<XSliderSkin>();
			if(temp == NULL) return XFalse;
			if(!temp->initEx(m_resName.c_str(),resPos))
			{
				XMem::XDELETE(temp);
				return XFalse;
			}
			m_pointer = temp;
		}
		break;
	case RESOURCE_TYPEXPROGRESS_TEX:
		{
			XProgressSkin * temp = XMem::createMem<XProgressSkin>();
			if(temp == NULL) return XFalse;
			if(!temp->initEx(m_resName.c_str(),resPos))
			{
				XMem::XDELETE(temp);
				return XFalse;
			}
			m_pointer = temp;
		}
		break;
	case RESOURCE_TYPEXMULTILIST_TEX:
		{
			XMultiListSkin * temp = XMem::createMem<XMultiListSkin>();
			if(temp == NULL) return XFalse;
			if(!temp->initEx(m_resName.c_str(),resPos))
			{
				XMem::XDELETE(temp);
				return XFalse;
			}
			m_pointer = temp;
		}
		break;
	case RESOURCE_TYPEXCOMBO_TEX:
		{
			XComboSkin * temp = XMem::createMem<XComboSkin>();
			if(temp == NULL) return XFalse;
			if(!temp->initEx(m_resName.c_str(),resPos))
			{
				XMem::XDELETE(temp);
				return XFalse;
			}
			m_pointer = temp;
		}
		break;
	case RESOURCE_TYPEXDIRLIST_TEX:
		{
			XDirListSkin * temp = XMem::createMem<XDirListSkin>();
			if(temp == NULL) return XFalse;
			if(!temp->initEx(m_resName.c_str(),resPos))
			{
				XMem::XDELETE(temp);
				return XFalse;
			}
			m_pointer = temp;
		}
		break;
	case RESOURCE_TYPEXPASSWORDPAD_TEX:
		{
			XPasswordPadSkin * temp = XMem::createMem<XPasswordPadSkin>();
			if(temp == NULL) return XFalse;
			if(!temp->initEx(m_resName.c_str(),resPos))
			{
				XMem::XDELETE(temp);
				return XFalse;
			}
			m_pointer = temp;
		}
		break;
	}
	logstr += "Load Res:" + m_resName;
	LogStr(logstr.c_str());

	m_isInited = XTrue;
	return XTrue;
}
void XResourceInfo::release()			//资源的释放函数
{
	if(!m_isInited) return;
	//释放实际的资源
	if(m_counter > 0) LogNull("There is something wrong! - %d:%s",m_counter, m_resName.c_str());

	switch(m_type)
	{
	case RESOURCE_TYPE_TEXTURE:
		{
			XResourceTex * temp = (XResourceTex *)m_pointer;
			XMem::XDELETE(temp);
			m_pointer = NULL;
		}
		break;
	case RESOURCE_TYPE_SOUND:
		{
			XResourceSound * temp = (XResourceSound *)m_pointer;
			XMem::XDELETE(temp);
			m_pointer = NULL;
		}
		break;
	case RESOURCE_TYPEXBUTTON_TEX:
		{
			XButtonSkin * temp = (XButtonSkin *)m_pointer;
			XMem::XDELETE(temp);
			m_pointer = NULL;
		}
		break;
	case RESOURCE_TYPEXCHECK_TEX:
		{
			XCheckSkin * temp = (XCheckSkin *)m_pointer;
			XMem::XDELETE(temp);
			m_pointer = NULL;
		}
		break;
	case RESOURCE_TYPEXEDIT_TEX:
		{
			XEditSkin * temp = (XEditSkin *)m_pointer;
			XMem::XDELETE(temp);
			m_pointer = NULL;
		}
		break;
	case RESOURCE_TYPEXSLIDER_TEX:
		{
			XSliderSkin * temp = (XSliderSkin *)m_pointer;
			XMem::XDELETE(temp);
			m_pointer = NULL;
		}
		break;
	case RESOURCE_TYPEXPROGRESS_TEX:
		{
			XProgressSkin * temp = (XProgressSkin *)m_pointer;
			XMem::XDELETE(temp);
			m_pointer = NULL;
		}
		break;
	case RESOURCE_TYPEXMULTILIST_TEX:
		{
			XMultiListSkin * temp = (XMultiListSkin *)m_pointer;
			XMem::XDELETE(temp);
			m_pointer = NULL;
		}
		break;
	case RESOURCE_TYPEXCOMBO_TEX:
		{
			XComboSkin * temp = (XComboSkin *)m_pointer;
			XMem::XDELETE(temp);
			m_pointer = NULL;
		}
		break;
	case RESOURCE_TYPEXDIRLIST_TEX:
		{
			XDirListSkin * temp = (XDirListSkin *)m_pointer;
			XMem::XDELETE(temp);
			m_pointer = NULL;
		}
		break;
	case RESOURCE_TYPEXPASSWORDPAD_TEX:
		{
			XPasswordPadSkin * temp = (XPasswordPadSkin *)m_pointer;
			XMem::XDELETE(temp);
			m_pointer = NULL;
		}
		break;
	}
	m_isInited = XFalse;
}
XResourceInfo *XResourceManager::loadResource(const char * name, XResourceType type,
	XResPos resPos, bool isInThread)	//载入指定资源
{
	if (name == NULL) return NULL;
	if (m_canReuse)
	{
		XResourceInfo* temp = isLoad(name);
		if (temp != NULL && temp->m_type == type)
		{//已经载入,并且资源是匹配的
			++temp->m_counter;	//增加一层引用
			LogNull("%d:%s", temp->m_counter, temp->m_resName.c_str());
			return temp;
		}
		else
		{//尚未载入
			temp = XMem::createMem<XResourceInfo>();
			if (temp == NULL) return NULL;
			temp->m_resName = name;
			temp->m_type = type;
			if (temp->load(resPos, isInThread))
			{
				m_resourceBuff.push_back(temp);
				++m_resourceSum;
				++temp->m_counter;
				return temp;	//资源载入成功
			}
			else
			{//资源载入失败
				XMem::XDELETE(temp);
				return NULL;
			}
		}
	}
	else
	{
		XResourceInfo* temp = XMem::createMem<XResourceInfo>();
		if (temp == NULL) return NULL;
		temp->m_resName = name;
		temp->m_type = type;
		if (temp->load(resPos, isInThread))
		{
			m_resourceBuff.push_back(temp);
			++m_resourceSum;
			++temp->m_counter;
			return temp;	//资源载入成功
		}
		else
		{//资源载入失败
			XMem::XDELETE(temp);
			return NULL;
		}
	}
}
bool XResourceManager::updateRes(XResourceInfo* &res, const char* name,
	XResPos resPos, bool isInThread)
{
	if (name == NULL) return false;
	if (m_canReuse)
	{
		m_mutex.Lock();
		XResourceInfo* temp = isLoad(name);
		if (temp != NULL && temp->m_type == res->m_type)
		{//已经载入,并且资源是匹配的
			++temp->m_counter;	//增加一层引用
			LogNull("%d:%s", temp->m_counter, temp->m_resName.c_str());
			releaseResource(res);
			res = temp;
			m_mutex.Unlock();
			return true;
		}
		else
		{//尚未载入，尝试改变内容，由于资源的复用性，
			if (res->m_counter > 1) return false;	//已经被复用
			std::string tmpName = res->m_resName;
			res->m_resName = name;
			if (!res->update(resPos, isInThread))
			{
				res->m_resName = tmpName;
				m_mutex.Unlock();
				return false;
			}
			m_mutex.Unlock();
			return true;
		}
	}
	else
	{
		std::string tmpName = res->m_resName;
		res->m_resName = name;
		if (!res->update(resPos, isInThread))
		{
			res->m_resName = tmpName;
			return false;
		}
		return true;
	}
//	m_mutex.Unlock();
//	return true;
}
XBool XResourceManager::releaseResource(const XResourceInfo *p)	//释放一个资源
{
	if(m_resourceBuff.size() <= 0)//这里产生问题是由于定义以及释放的先后造成的，理论上应该没什么大问题
	{//由于析构顺序造成的问题，这里需要考虑
		LogStr("Error!");
		return XFalse;
	}
	std::list<XResourceInfo *>::iterator it;
	m_mutex.Lock();
	for(it = m_resourceBuff.begin();it != m_resourceBuff.end();++ it)
	{
		if(* it == p)
		{
			-- (*it)->m_counter;
			LogNull("%d:%s",(*it)->m_counter,(*it)->m_resName.c_str());
			if((*it)->m_counter <= 0)
			{//释放掉这个资源
				XMem::XDELETE((* it));
				m_resourceBuff.erase(it);
				-- m_resourceSum;
			}
			m_mutex.Unlock();
			return XTrue;
		}
	}
	m_mutex.Unlock();
	LogStr("Error:invalid resource!");
	return XFalse;
}
void XResourceManager::release()				//释放所有的资源
{
	std::list<XResourceInfo *>::iterator it;
	for(it = m_resourceBuff.begin();it != m_resourceBuff.end();++ it)
	{
		//(* it)->release();
		LogNull("%d:%s",(*it)->m_counter,(*it)->m_resName.c_str());
		XMem::XDELETE((* it));
	}
	m_resourceBuff.clear();
	m_resourceSum = 0;
}
XBool XResourceManager::isLoaded(const char * name)	//判断指定资源是否已经载入
{
	if(name == NULL) return XFalse;
	m_mutex.Lock();
	for(auto it = m_resourceBuff.begin();it != m_resourceBuff.end();++ it)
	{
		if((*it)->isLoaded() && XFile::fileNameCompare(name,(*it)->m_resName.c_str()))
		{
			m_mutex.Unlock();
			return XTrue;
		}
	}
	m_mutex.Unlock();
	return XFalse;
}
XResourceInfo * XResourceManager::isLoad(const char * name)
{
	if(name == NULL || m_resourceBuff.size() == 0) return NULL;
	m_mutex.Lock();
	for(auto it = m_resourceBuff.begin();it != m_resourceBuff.end();++ it)
	{
		if((*it)->isLoaded() && XFile::fileNameCompare(name,(*it)->m_resName.c_str()))
		{
			m_mutex.Unlock();
			return (*it);
		}
	}
	m_mutex.Unlock();
	return NULL;
}
}