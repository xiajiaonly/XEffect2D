#ifndef _JIA_XXML_
#define _JIA_XXML_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.4.13
//--------------------------------
//这里是对XML的访问的一些建议的封装，使得对xml的访问更简单
#include "tinyxml.h"
#include "XOSDefine.h"
//xml中对空格的处理通过这里来设置TiXmlBase::SetCondenseWhiteSpace(false);
//tinyxml的空格问题请查阅
//http://sourceforge.net/p/tinyxml/discussion/172103/thread/2270ca9a
//https://sourceforge.net/tracker/index.php?func=detail&aid=3085245&group_id=13559&atid=113559
//下面是关于XML操作的一些函数的封装
inline TiXmlNode * getXmlAsInt(TiXmlNode *node,const char * name,int &value)
{
	static TiXmlNode *keyNode = NULL;
	keyNode = node->FirstChildElement(name);
	if(keyNode != NULL)
	{
		if(keyNode->ToElement()->GetText() == NULL) return NULL;
		value = atoi(keyNode->ToElement()->GetText());
		return keyNode;
	}
	return NULL;
}
inline TiXmlNode * getNextXmlAsInt(TiXmlNode *node,const char * name,int &value)
{
	static TiXmlNode *keyNode = NULL;
	keyNode = node->NextSiblingElement(name);
	if(keyNode != NULL)
	{
		if(keyNode->ToElement()->GetText() == NULL) return NULL;
		value = atoi(keyNode->ToElement()->GetText());
		return keyNode;
	}
	return NULL;
}
inline TiXmlNode * getXmlAsFloat(TiXmlNode *node,const char * name,float &value)
{
	static TiXmlNode *keyNode = NULL;
	keyNode = node->FirstChildElement(name);
	if(keyNode != NULL)
	{
		if(keyNode->ToElement()->GetText() == NULL) return NULL;
		value = atof(keyNode->ToElement()->GetText());
		return keyNode;
	}
	return NULL;
}
inline TiXmlNode * getNextXmlAsFloat(TiXmlNode *node,const char * name,float &value)
{
	static TiXmlNode *keyNode = NULL;
	keyNode = node->NextSiblingElement(name);
	if(keyNode != NULL)
	{
		if(keyNode->ToElement()->GetText() == NULL) return NULL;
		value = atof(keyNode->ToElement()->GetText());
		return keyNode;
	}
	return NULL;
}
extern TiXmlNode * getXmlAsBool(TiXmlNode *node,const char * name,bool &value);
extern TiXmlNode * getNextXmlAsBool(TiXmlNode *node,const char * name,bool &value);
inline TiXmlNode * getXmlAsString(TiXmlNode *node,const char * name,std::string &value)
{
	static TiXmlNode *keyNode = NULL;
	keyNode = node->FirstChildElement(name);
	if(keyNode != NULL)
	{
		if(keyNode->ToElement()->GetText() == NULL)  return NULL;
		value = keyNode->ToElement()->GetText();
		return keyNode;
	}
	return NULL;
}
inline TiXmlNode * getNextXmlAsString(TiXmlNode *node,const char * name,std::string &value)
{
	static TiXmlNode *keyNode = NULL;
	keyNode = node->NextSiblingElement(name);
	if(keyNode != NULL)
	{
		if(keyNode->ToElement()->GetText() == NULL)  return NULL;
		value = keyNode->ToElement()->GetText();
		return keyNode;
	}
	return NULL;
}
inline _XBool addLeafNode(TiXmlNode &pElmParent,const char* pszNode,const char* pszText)
{
	TiXmlElement elmNode(pszNode);  
    TiXmlText elmText(pszText);  
    if(elmNode.InsertEndChild(elmText) == NULL) return XFalse;  
    if(pElmParent.InsertEndChild(elmNode) == NULL) return XFalse;  
    return XTrue;  
}
inline _XBool addComment(TiXmlNode &pElmParent,const char* pszText)
{
	TiXmlComment elmText(pszText); 
    if(pElmParent.InsertEndChild(elmText) == NULL) return XFalse;  
    return XTrue;  
}
inline _XBool addLeafNode(TiXmlNode &pElmParent,const char* pszNode,const std::string &pszText)
{
	return addLeafNode(pElmParent,pszNode,pszText.c_str());
}
extern _XBool addLeafNode(TiXmlNode &pElmParent,const char* pszNode,bool flag);

#endif