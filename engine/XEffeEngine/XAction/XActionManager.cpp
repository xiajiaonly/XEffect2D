#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XActionManager.h"
#include "../XDirectory.h"
namespace XE{
void XActionMananger::loadActionDesFromFold(const XDir * dir)
{
	if(dir == NULL) return;	//空的文件夹直接返回
	int sum = dir->files.size();
	if(sum <= 0) return;
	XFileInfo * tempFile = NULL;
	for(int i = 0;i < sum;++ i)
	{
		tempFile = dir->files[i];
		if(tempFile->isDirectory)
		{//文件夹
			loadActionDesFromFold(tempFile->directory);
		}else
		{
			if(isActionDescriptionFile(tempFile->filename.c_str()))
			{//合法的文件
				XActionDescription * tempAD = XMem::createMem<XActionDescription>();
				if(tempAD == NULL)
				{
					printf("ERROR:Memory ERROR!\n");
					return;
				}
				if(!tempAD->loadAction(tempFile->allPath.c_str(),RES_LOCAL_FOLDER))	//资源位置的参数尚需要商榷
				{
					printf("ERROR:Action description load ERROR:%s\n",tempFile->allPath.c_str());
					return;
				}
				m_actionDescriptions.insert(std::pair<int,XActionDescription *>(tempAD->getID(),tempAD));
				m_actionDesMap.insert(std::pair<std::string,int>(tempAD->getName(),tempAD->getID()));
				m_actionIDList.push_back(tempAD->getID());
			}
		//	else
		//	{//无效的文件
		//	}
		}
	}
}
XBool XActionMananger::loadAllActionDes(const char * path)	//将指定目录下的动作全部载入 
{//内部资源读取会存在问题
	if(m_isInited) return XFalse;
	char tempStr[MAX_FILE_NAME_LENGTH];
	if(path == NULL) strcpy_s(tempStr,MAX_FILE_NAME_LENGTH,ACTION_DES_PATH);	//使用默认路径
	else strcpy_s(tempStr,MAX_FILE_NAME_LENGTH,path);

	XDirectory tempDir;
	if(!tempDir.initEx(tempStr))
	{
		if(!tempDir.init(tempStr)) return XFalse;
	}
	loadActionDesFromFold(&tempDir.m_curDirectory);
	//遍历所有的动作描述，并将其应用关系理清楚
	std::map<int,XActionDescription *>::iterator it = m_actionDescriptions.begin();
	XActionDescription * tempAD = NULL;
	for(;it != m_actionDescriptions.end();++ it)
	{
		int sum = it->second->m_otherActionID.size();
		for(int i = 0;i < sum;++ i)
		{
			tempAD = getActionDes(it->second->m_otherActionID[i]);
			if(tempAD == NULL)
			{//找不到需要的数据，这里是严重的错误，将会导致数据不完整而不能进行播放
				printf("ERROR:Action description data is incomplete!\n");
				return XFalse;
			}
			it->second->pushAActionDescription(tempAD);
		}
	}

	m_isInited = XTrue;
	return XTrue;
}
XBool XActionMananger::saveAllActionDes()
{
	if(!m_isInited) return XFalse;
	//遍历所有的动作并保存
	std::map<int,XActionDescription *>::iterator it = m_actionDescriptions.begin();
	for(;it != m_actionDescriptions.end();++ it)
	{
		if(it->second != NULL) it->second->saveAction();
	}
	return XTrue;
}
XBool XActionMananger::getIsReferenced(XActionDescription * AD)
{
	if(AD == NULL) return XFalse;
	//遍历所有的AD，检查引用
	XActionDescription *tempAD = NULL;
	std::map<int,XActionDescription *>::iterator it = m_actionDescriptions.begin();
	for(;it != m_actionDescriptions.end();++ it)
	{
		tempAD = it->second;
		int allADSum = tempAD->getOtherActionSum();
		if(allADSum <= 0) continue;
		for(int i = 0;i < allADSum;++ i)
		{
			if(tempAD->getOtherAD(i) == AD) return XTrue;	//被应用
		}
	}
	return XFalse;		//没有被引用
}
XBool XActionMananger::isActionDescriptionFile(const char * filename)
{
	if(filename == NULL) return XFalse;
	int len = strlen(filename);
	if( (filename[len - 1] == 'd' || filename[len - 1] == 'D') &&
		(filename[len - 2] == 'c' || filename[len - 2] == 'C') &&
		(filename[len - 3] == 'a' || filename[len - 3] == 'A') &&
		filename[len - 4] == '.')
	{
		return XTrue;
	}
	return XFalse;
}
void XActionMananger::release()	//资源释放
{//释放已经载入的资源
	if(!m_isInited) return;
	//遍历所有的AD并释放
	std::map<int,XActionDescription *>::iterator it = m_actionDescriptions.begin();
	for(;it != m_actionDescriptions.end();++ it)
	{
		XMem::XDELETE(it->second);
	}
	m_actionDescriptions.clear();

	m_actionDesMap.clear();
	m_actionIDList.clear();
	m_isInited = XFalse;
}
void XActionMananger::deleteAActionDesNoReference(XActionDescription * AD)
{//删除指定的AD
	if(AD == NULL) return;
	std::map<int,XActionDescription *>::iterator it = m_actionDescriptions.begin();
	for(;it != m_actionDescriptions.end();++ it)
	{
		if(it->second == AD)
		{
			std::map<std::string,int>::iterator itTemp = m_actionDesMap.find(AD->getName());	//删除指定的
			m_actionDesMap.erase(itTemp);

			int sum = m_actionIDList.size();
			for(int i = 0;i < sum;++ i)
			{
				if(m_actionIDList[i] == AD->getID())
				{
				//	for(int j = i;j < sum - 1;++ j)
				//	{
				//		m_actionIDList[j] = m_actionIDList[j + 1];
				//	}
				//	m_actionIDList.pop_back();
					m_actionIDList.erase(m_actionIDList.begin() + i);
					break;
				}
			}

			//没有释放这个AD的物件资源
			XActionObjectManager::GetInstance().decreaseAObject(AD->getObject());
			XMem::XDELETE(it->second);
			m_actionDescriptions.erase(it);
			break;
		}
	}
}
XBool XActionMananger::setADName(XActionDescription * AD,const char *name)
{
	if(AD == NULL) return XFalse;
	if(name == NULL) return XFalse;
	std::map<int,XActionDescription *>::iterator it = m_actionDescriptions.begin();
	for(;it != m_actionDescriptions.end();++ it)
	{
		if(it->second == AD)
		{
			std::map<std::string,int>::iterator itTemp = m_actionDesMap.find(AD->getName());	//删除指定的
			if(AD->setName(name))
			{//跟名成功，修改名字与index的对应关系
				m_actionDesMap.erase(itTemp);
				m_actionDesMap.insert(std::pair<std::string,int>(AD->getName(),AD->getID()));
				return XTrue;
			}
			break;
		}
	}
	return XFalse;
}
}