//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "glew.h"
#include "XResourceManager.h"
#include "XResourcePack.h"
#include "XBasicOpenGl.h"
#include "XLogBook.h"
#include "XControl/XControls.h"
#include "XBasicWindow.h"
void _XResourceTex::release()
{
	if(glIsTexture(m_texID)) 
	{
		printf("delete texture:%d\n",m_texID);
		glDeleteTextures(1,&m_texID);
	}
}
_XBool _XResourceInfo::load(_XResourcePosition resoursePosition)			//资源的载入函数
{
	if(m_isInited) return XTrue;	//防止重复的载入
	if(m_name == NULL) return XFalse;
	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = XEE::defaultResourcePosition;
	//更具不同的类型调用不同的载入函数
	switch(m_type)
	{
	case RESOURCE_TYPE_NULL:
		return XFalse;
		break;
	case RESOURCE_TYPE_TEXTURE:
		{
			_XResourceTex * temp = createMem<_XResourceTex>();
			if(temp == NULL) return XFalse;
			if(TextureLoadEx(temp->m_texID,m_name,&temp->m_width,&temp->m_height,resoursePosition) == 0) 
			{
				XDELETE(temp);
				return XFalse;
			}
			printf("%d|",temp->m_texID);
			m_pointer = temp;
		}
		break;
	case RESOURCE_TYPE_SOUND:
		{
			_XResourceSound * temp = createMem<_XResourceSound>();
			if(temp == NULL) return XFalse;
			if(resoursePosition == RESOURCE_LOCAL_FOLDER)
			{
				if(!_XSoundHandle.loadSound(m_name,temp->m_handle)) 
				{
					AddLogInfoStr("Sound load error!\n");
					XDELETE(temp);
					return XFalse;
				}
			}else
			if(resoursePosition == RESOURCE_LOCAL_PACK)
			{
				int lengthTemp = _XResourcePack::GetInstance().getFileLength(m_name);
				unsigned char *p = createArrayMem<unsigned char>(lengthTemp + 1);
				if(p == NULL) 
				{
					XDELETE(temp);
					return XFalse;
				}

				_XResourcePack::GetInstance().unpackResource(m_name,p);
				if(!_XSoundHandle.loadSound(p,lengthTemp,temp->m_handle))
				{
					AddLogInfoStr("Sound load error!\n");
					XDELETE(temp);
					return XFalse;
				}
				XDELETE_ARRAY(p);
			}
			m_pointer = temp;
		}
		break;
	case RESOURCE_TYPE_XBUTTON_TEX:
		{
			_XButtonTexture * temp = createMem<_XButtonTexture>();
			if(temp == NULL) return XFalse;
			if(temp->initEx(m_name,resoursePosition) == 0)
			{
				XDELETE(temp);
				return XFalse;
			}
			m_pointer = temp;
		}
		break;
	case RESOURCE_TYPE_XCHECK_TEX:
		{
			_XCheckTexture * temp = createMem<_XCheckTexture>();
			if(temp == NULL) return XFalse;
			if(temp->initEx(m_name,resoursePosition) == 0)
			{
				XDELETE(temp);
				return XFalse;
			}
			m_pointer = temp;
		}
		break;
	case RESOURCE_TYPE_XEDIT_TEX:
		{
			_XEditTexture * temp = createMem<_XEditTexture>();
			if(temp == NULL) return XFalse;
			if(temp->initEx(m_name,resoursePosition) == 0)
			{
				XDELETE(temp);
				return XFalse;
			}
			m_pointer = temp;
		}
		break;
	case RESOURCE_TYPE_XSLIDER_TEX:
		{
			_XSliderTexture * temp = createMem<_XSliderTexture>();
			if(temp == NULL) return XFalse;
			if(temp->initEx(m_name,resoursePosition) == 0)
			{
				XDELETE(temp);
				return XFalse;
			}
			m_pointer = temp;
		}
		break;
	case RESOURCE_TYPE_XPROGRESS_TEX:
		{
			_XProgressTexture * temp = createMem<_XProgressTexture>();
			if(temp == NULL) return XFalse;
			if(temp->initEx(m_name,resoursePosition) == 0)
			{
				XDELETE(temp);
				return XFalse;
			}
			m_pointer = temp;
		}
		break;
	case RESOURCE_TYPE_XMULTILIST_TEX:
		{
			_XMultiListTexture * temp = createMem<_XMultiListTexture>();
			if(temp == NULL) return XFalse;
			if(temp->initEx(m_name,resoursePosition) == 0)
			{
				XDELETE(temp);
				return XFalse;
			}
			m_pointer = temp;
		}
		break;
	case RESOURCE_TYPE_XCOMBO_TEX:
		{
			_XComboTexture * temp = createMem<_XComboTexture>();
			if(temp == NULL) return XFalse;
			if(temp->initEx(m_name,resoursePosition) == 0)
			{
				XDELETE(temp);
				return XFalse;
			}
			m_pointer = temp;
		}
		break;
	case RESOURCE_TYPE_XDIRLIST_TEX:
		{
			_XDirListTexture * temp = createMem<_XDirListTexture>();
			if(temp == NULL) return XFalse;
			if(temp->initEx(m_name,resoursePosition) == 0)
			{
				XDELETE(temp);
				return XFalse;
			}
			m_pointer = temp;
		}
		break;
	}
	printf("Load resource:%s\n",m_name);

	m_isInited = XTrue;
	return XTrue;
}
void _XResourceInfo::release()			//资源的释放函数
{
	if(!m_isInited) return;
	//释放实际的资源
	if(m_counter > 0)
	{
		printf("There is something wrong! - %d:%s\n",m_counter,m_name);
	}
	XDELETE_ARRAY(m_name);
	switch(m_type)
	{
	case RESOURCE_TYPE_TEXTURE:
		{
			_XResourceTex * temp = (_XResourceTex *)m_pointer;
			temp->release();
			XDELETE(m_pointer);
		}
		break;
	case RESOURCE_TYPE_SOUND:
		{
			_XResourceSound * temp = (_XResourceSound *)m_pointer;
			temp->release();
			XDELETE(m_pointer);
		}
		break;
	case RESOURCE_TYPE_XBUTTON_TEX:
		{
			_XButtonTexture * temp = (_XButtonTexture *)m_pointer;
			temp->release();
			XDELETE(m_pointer);
		}
		break;
	case RESOURCE_TYPE_XCHECK_TEX:
		{
			_XCheckTexture * temp = (_XCheckTexture *)m_pointer;
			temp->release();
			XDELETE(m_pointer);
		}
		break;
	case RESOURCE_TYPE_XEDIT_TEX:
		{
			_XEditTexture * temp = (_XEditTexture *)m_pointer;
			temp->release();
			XDELETE(m_pointer);
		}
		break;
	case RESOURCE_TYPE_XSLIDER_TEX:
		{
			_XSliderTexture * temp = (_XSliderTexture *)m_pointer;
			temp->release();
			XDELETE(m_pointer);
		}
		break;
	case RESOURCE_TYPE_XPROGRESS_TEX:
		{
			_XProgressTexture * temp = (_XProgressTexture *)m_pointer;
			temp->release();
			XDELETE(m_pointer);
		}
		break;
	case RESOURCE_TYPE_XMULTILIST_TEX:
		{
			_XMultiListTexture * temp = (_XMultiListTexture *)m_pointer;
			temp->release();
			XDELETE(m_pointer);
		}
		break;
	case RESOURCE_TYPE_XCOMBO_TEX:
		{
			_XComboTexture * temp = (_XComboTexture *)m_pointer;
			temp->release();
			XDELETE(m_pointer);
		}
		break;
	case RESOURCE_TYPE_XDIRLIST_TEX:
		{
			_XDirListTexture * temp = (_XDirListTexture *)m_pointer;
			temp->release();
			XDELETE(m_pointer);
		}
		break;
	}
	m_isInited = XFalse;
}
_XResourceInfo *_XResourceManager::loadResource(const char * name,_XResourceType type,_XResourcePosition resoursePosition)	//载入指定资源
{
	if(name == NULL) return NULL;
	_XResourceInfo * temp = isLoad(name);
	if(temp != NULL && temp->m_type == type)
	{//已经载入,并且资源是匹配的
		++ temp->m_counter;	//增加一层引用
		printf("%d:%s\n",temp->m_counter,temp->m_name);
		return temp;
	}else
	{//尚未载入
		temp = createMem<_XResourceInfo>();
		if(temp == NULL) return NULL;
		temp->m_name = createArrayMem<char>(MAX_FILE_NAME_LENGTH);
		if(temp->m_name == NULL) return NULL;
		strcpy(temp->m_name,name);
		temp->m_type = type;
		if(temp->load(resoursePosition) != 0)
		{
			m_resourceBuff.push_back(temp);
			++ m_resourceSum;
			++ temp->m_counter;
			return temp;	//资源载入成功
		}else
		{//资源载入失败
			XDELETE_ARRAY(temp->m_name);
			XDELETE(temp);
			return NULL;
		}
	}
}
_XBool _XResourceManager::releaseResource(const _XResourceInfo *p)	//释放一个资源
{
	//for(int i = 0;i < m_resourceSum;++ i)
	//{
	//	if(m_resourceBuff[i] == p)
	//	{
	//		m_resourceBuff[i]->m_counter --;
	//		printf("%d:%s\n",m_resourceBuff[i]->m_counter,m_resourceBuff[i]->m_name);
	//		//if(m_resourceBuff[i]->m_counter == 0) //没有引用的时候就可以释放了
	//		if(m_resourceBuff[i]->m_counter < 0){printf("Error\n");}
	//		return XTrue;
	//	}
	//}
	if(m_resourceBuff.size() <= 0)//这里产生问题是由于定义以及释放的先后造成的，理论上应该没什么大问题
	{//由于析构顺序造成的问题，这里需要考虑
		printf("Error!\n");
		return XFalse;
	}
	std::list<_XResourceInfo *>::iterator it;
	for(it = m_resourceBuff.begin();it != m_resourceBuff.end();++ it)
	{
		if(* it == p)
		{
			-- (*it)->m_counter;
			printf("%d:%s\n",(*it)->m_counter,(*it)->m_name);
			if((*it)->m_counter == 0)
			{//释放掉这个资源
				XDELETE((* it));
				m_resourceBuff.erase(it);
				-- m_resourceSum;
			}
			return XTrue;
		}
	}
	printf("Error:invalid resource!\n");
	return XFalse;
}
void _XResourceManager::release()				//释放所有的资源
{
	//int sum = m_resourceBuff.size();
	//if(sum == 0) return;	//防止重复释放
	//for(int i = 0;i < sum;++ i)
	//{
	//	m_resourceBuff[i]->release();
	//	XDELETE(m_resourceBuff[i]);
	//}
	//m_resourceBuff.clear();
	//m_resourceBuff.swap(std::vector<_XResourceInfo *>());
	std::list<_XResourceInfo *>::iterator it;
	for(it = m_resourceBuff.begin();it != m_resourceBuff.end();++ it)
	{
		//(* it)->release();
		XDELETE((* it));
	}
	m_resourceBuff.clear();
	m_resourceSum = 0;
}
_XBool _XResourceManager::isLoaded(const char * name)	//判断指定资源是否已经载入
{
	if(name == NULL) return XFalse;
	//for(int i = 0;i < m_resourceSum;++ i)
	//{
	//	if(m_resourceBuff[i]->isLoaded() && fileNameCompare(name,m_resourceBuff[i]->m_name) != 0)
	//	{
	//		return XTrue;
	//	}
	//}
	std::list<_XResourceInfo *>::iterator it;
	for(it = m_resourceBuff.begin();it != m_resourceBuff.end();++ it)
	{
		if((*it)->isLoaded() && fileNameCompare(name,(*it)->m_name) != 0)
		{
			return XTrue;
		}
	}
	return XFalse;
}
_XResourceInfo * _XResourceManager::isLoad(const char * name)
{
	if(name == NULL) return NULL;
	//for(int i = 0;i < m_resourceSum;++ i)
	//{
	//	if(m_resourceBuff[i]->isLoaded() && fileNameCompare(name,m_resourceBuff[i]->m_name) != 0)
	//	{
	//		return m_resourceBuff[i];
	//	}
	//}
	std::list<_XResourceInfo *>::iterator it;
	for(it = m_resourceBuff.begin();it != m_resourceBuff.end();++ it)
	{
		if((*it)->isLoaded() && fileNameCompare(name,(*it)->m_name) != 0)
		{
			return (*it);
		}
	}
	return NULL;
}