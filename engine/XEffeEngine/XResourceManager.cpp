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
XBool XResourceInfo::load(XResourcePosition resoursePosition)			//资源的载入函数
{
	if(m_isInited) return XTrue;	//防止重复的载入
	if(m_name == NULL) return XFalse;
	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = getDefResPos();
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
			if(!XGL::TextureLoadEx(temp->m_texID,m_name,&temp->m_width,&temp->m_height,resoursePosition)) 
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
			switch(resoursePosition)
			{
			case RESOURCE_LOCAL_FOLDER:	//从文件夹中读取资源
				if(!XCurSndCore.loadSound(m_name,temp->m_handle)) 
				{
					logstr += "Sound load error!";
					LogStr(logstr.c_str());
					XMem::XDELETE(temp);
					return XFalse;
				}
				break;
			case RESOURCE_LOCAL_PACK:	//从资源包中读取资源
				{
					int lengthTemp = XResPack.getFileLength(m_name);
					unsigned char *p = XMem::createArrayMem<unsigned char>(lengthTemp + 1);
					if(p == NULL) 
					{
						XMem::XDELETE(temp);
						return XFalse;
					}

					XResPack.unpackResource(m_name,p);
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
			case RESOURCE_WEB:	//从网页中读取资源
				logstr += "Sound load error!";
				LogStr(logstr.c_str());
				XMem::XDELETE(temp);
				return XFalse;
				break;
			case RESOURCE_AUTO:	//依次读取资源
				{
				//packer
					int lengthTemp = XResPack.getFileLength(m_name);
					unsigned char *p = XMem::createArrayMem<unsigned char>(lengthTemp + 1);
					if(p != NULL) 
					{
						XResPack.unpackResource(m_name,p);
						if(XCurSndCore.loadSound(p,lengthTemp,temp->m_handle))
						{
							XMem::XDELETE_ARRAY(p);
							break;
						}
						XMem::XDELETE_ARRAY(p);
					}
				//folder
					if(XCurSndCore.loadSound(m_name,temp->m_handle)) 
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
			if(!temp->initEx(m_name,resoursePosition))
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
			if(!temp->initEx(m_name,resoursePosition))
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
			if(!temp->initEx(m_name,resoursePosition))
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
			if(!temp->initEx(m_name,resoursePosition))
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
			if(!temp->initEx(m_name,resoursePosition))
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
			if(!temp->initEx(m_name,resoursePosition))
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
			if(!temp->initEx(m_name,resoursePosition))
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
			if(!temp->initEx(m_name,resoursePosition))
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
			if(!temp->initEx(m_name,resoursePosition))
			{
				XMem::XDELETE(temp);
				return XFalse;
			}
			m_pointer = temp;
		}
		break;
	}
	logstr += "Load resource:" + std::string(m_name);
	LogStr(logstr.c_str());

	m_isInited = XTrue;
	return XTrue;
}
void XResourceInfo::release()			//资源的释放函数
{
	if(!m_isInited) return;
	//释放实际的资源
	if(m_counter > 0) LogNull("There is something wrong! - %d:%s",m_counter,m_name);

	XMem::XDELETE_ARRAY(m_name);
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
XResourceInfo *XResourceManager::loadResource(const char * name,XResourceType type,XResourcePosition resoursePosition)	//载入指定资源
{
	if(name == NULL) return NULL;
	XResourceInfo * temp = isLoad(name);
	if(temp != NULL && temp->m_type == type)
	{//已经载入,并且资源是匹配的
		++ temp->m_counter;	//增加一层引用
		LogNull("%d:%s",temp->m_counter,temp->m_name);
		return temp;
	}else
	{//尚未载入
		temp = XMem::createMem<XResourceInfo>();
		if(temp == NULL) return NULL;
		temp->m_name = XMem::createArrayMem<char>(MAX_FILE_NAME_LENGTH);
		if(temp->m_name == NULL) return NULL;
		strcpy(temp->m_name,name);
		temp->m_type = type;
		if(temp->load(resoursePosition))
		{
			m_resourceBuff.push_back(temp);
			++ m_resourceSum;
			++ temp->m_counter;
			return temp;	//资源载入成功
		}else
		{//资源载入失败
			XMem::XDELETE_ARRAY(temp->m_name);
			XMem::XDELETE(temp);
			return NULL;
		}
	}
}
XBool XResourceManager::releaseResource(const XResourceInfo *p)	//释放一个资源
{
	//for(int i = 0;i < m_resourceSum;++ i)
	//{
	//	if(m_resourceBuff[i] == p)
	//	{
	//		m_resourceBuff[i]->m_counter --;
	//		LogNull("%d:%s",m_resourceBuff[i]->m_counter,m_resourceBuff[i]->m_name);
	//		//if(m_resourceBuff[i]->m_counter == 0) //没有引用的时候就可以释放了
	//		if(m_resourceBuff[i]->m_counter < 0){LogStr("Error");}
	//		return XTrue;
	//	}
	//}
	if(m_resourceBuff.size() <= 0)//这里产生问题是由于定义以及释放的先后造成的，理论上应该没什么大问题
	{//由于析构顺序造成的问题，这里需要考虑
		LogStr("Error!");
		return XFalse;
	}
	std::list<XResourceInfo *>::iterator it;
	for(it = m_resourceBuff.begin();it != m_resourceBuff.end();++ it)
	{
		if(* it == p)
		{
			-- (*it)->m_counter;
			LogNull("%d:%s",(*it)->m_counter,(*it)->m_name);
			if((*it)->m_counter <= 0)
			{//释放掉这个资源
				XMem::XDELETE_ARRAY((*it)->m_name);
				XMem::XDELETE((* it));
				m_resourceBuff.erase(it);
				-- m_resourceSum;
			}
			return XTrue;
		}
	}
	LogStr("Error:invalid resource!");
	return XFalse;
}
void XResourceManager::release()				//释放所有的资源
{
	//int sum = m_resourceBuff.size();
	//if(sum == 0) return;	//防止重复释放
	//for(int i = 0;i < sum;++ i)
	//{
	//	m_resourceBuff[i]->release();
	//	XMem::XDELETE(m_resourceBuff[i]);
	//}
	//m_resourceBuff.clear();
	//m_resourceBuff.swap(std::vector<XResourceInfo *>());
	std::list<XResourceInfo *>::iterator it;
	for(it = m_resourceBuff.begin();it != m_resourceBuff.end();++ it)
	{
		//(* it)->release();
		LogNull("%d:%s",(*it)->m_counter,(*it)->m_name);
		XMem::XDELETE_ARRAY((*it)->m_name);
		XMem::XDELETE((* it));
	}
	m_resourceBuff.clear();
	m_resourceSum = 0;
}
XBool XResourceManager::isLoaded(const char * name)	//判断指定资源是否已经载入
{
	if(name == NULL) return XFalse;
	//for(int i = 0;i < m_resourceSum;++ i)
	//{
	//	if(m_resourceBuff[i]->isLoaded() && fileNameCompare(name,m_resourceBuff[i]->m_name))
	//	{
	//		return XTrue;
	//	}
	//}
	std::list<XResourceInfo *>::iterator it;
	for(it = m_resourceBuff.begin();it != m_resourceBuff.end();++ it)
	{
		if((*it)->isLoaded() && XFile::fileNameCompare(name,(*it)->m_name))
		{
			return XTrue;
		}
	}
	return XFalse;
}
XResourceInfo * XResourceManager::isLoad(const char * name)
{
	if(name == NULL) return NULL;
	//for(int i = 0;i < m_resourceSum;++ i)
	//{
	//	if(m_resourceBuff[i]->isLoaded() && fileNameCompare(name,m_resourceBuff[i]->m_name))
	//	{
	//		return m_resourceBuff[i];
	//	}
	//}
	std::list<XResourceInfo *>::iterator it;
	for(it = m_resourceBuff.begin();it != m_resourceBuff.end();++ it)
	{
		if((*it)->isLoaded() && XFile::fileNameCompare(name,(*it)->m_name))
		{
			return (*it);
		}
	}
	return NULL;
}
}