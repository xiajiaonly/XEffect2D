#include "XStdHead.h"
#include "XMediaList.h"
#include "XXml.h"
namespace XE{
bool XMediaList::loadFromFile(const char * filename)	//从文件中读取列表
{
	std::string filenameStr;
	if(filename == NULL) filenameStr = DEFAULT_PLAY_LIST_FILENAME;
	else filenameStr = filename;

	TiXmlDocument doc(filenameStr);
	if(!doc.LoadFile()) return false;
	TiXmlNode *rootNode = doc.FirstChild("root");
	if(rootNode == NULL) return false;

	int tmp;
	XXml::getXmlAsInt(rootNode,"playMode",tmp);
	m_playMode = (XMediaListPlayerMode)tmp;
	XXml::getXmlAsInt(rootNode,"loopTimes",m_loopTimes);
	//下面读取相关数据
	m_playList.clear();
	TiXmlNode *keyNode = rootNode->FirstChildElement("itemInfo");
	while(true)
	{
		if(keyNode == NULL) break;
		XMediaListItem tmpItem;
		XXml::getXmlAsInt(keyNode,"headDelayTime", tmpItem.m_headDelayTime);
		XXml::getXmlAsInt(keyNode,"loopTimes", tmpItem.m_loopTimes);
		XXml::getXmlAsInt(keyNode,"loopHeadDelayTime", tmpItem.m_loopHeadDelayTime);
		XXml::getXmlAsString(keyNode,"videoFilename", tmpItem.m_videoFilename);
		XXml::getXmlAsInt(keyNode,"loopEndDelayTime", tmpItem.m_loopEndDelayTime);
		XXml::getXmlAsInt(keyNode,"endDelayTime", tmpItem.m_endDelayTime);
		XXml::getXmlAsBool(keyNode,"stopAtEnd", tmpItem.m_stopAtEnd);
		m_playList.push_back(tmpItem);

		keyNode = keyNode->NextSiblingElement("itemInfo");
	}

	return true;
}
bool XMediaList::saveToFile(const char * filename)
{
	std::string filenameStr;
	if(filename == NULL) filenameStr = DEFAULT_PLAY_LIST_FILENAME;
	else filenameStr = filename;

	TiXmlDocument doc(filenameStr);
	TiXmlDeclaration declaration("1.0","gb2312","yes");
	doc.InsertEndChild(declaration);
	TiXmlElement elmRoot("root");

	if(!XXml::addLeafNode(elmRoot,"playMode",XString::toString((int)(m_playMode)))) return false;
	if(!XXml::addLeafNode(elmRoot,"loopTimes",XString::toString(m_loopTimes))) return false;
	for(unsigned int i = 0;i < m_playList.size();++ i)
	{
		TiXmlElement elmNode("itemInfo");  
		if(!XXml::addLeafNode(elmNode,"headDelayTime",XString::toString(m_playList[i].m_headDelayTime))) return false;
		if(!XXml::addLeafNode(elmNode,"loopTimes",XString::toString(m_playList[i].m_loopTimes))) return false;
		if(!XXml::addLeafNode(elmNode,"loopHeadDelayTime",XString::toString(m_playList[i].m_loopHeadDelayTime))) return false;
		if(!XXml::addLeafNode(elmNode,"videoFilename",m_playList[i].m_videoFilename)) return false;
		if(!XXml::addLeafNode(elmNode,"loopEndDelayTime",XString::toString(m_playList[i].m_loopEndDelayTime))) return false;
		if(!XXml::addLeafNode(elmNode,"endDelayTime",XString::toString(m_playList[i].m_endDelayTime))) return false;
		if(!XXml::addLeafNode(elmNode,"stopAtEnd",m_playList[i].m_stopAtEnd)) return false;
		if(elmRoot.InsertEndChild(elmNode) == NULL) return false;  
	}
	if(doc.InsertEndChild(elmRoot) == NULL) return false;
	return doc.SaveFile();
}
}
