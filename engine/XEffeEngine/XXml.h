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
#include "XStringFun.h"
//xml中对空格的处理通过这里来设置TiXmlBase::SetCondenseWhiteSpace(false);
//tinyxml的空格问题请查阅
//http://sourceforge.net/p/tinyxml/discussion/172103/thread/2270ca9a
//https://sourceforge.net/tracker/index.php?func=detail&aid=3085245&group_id=13559&atid=113559
namespace XE{
class XVector2;
class XVector3;
class XVector4;
class XFColor;
class XCColor;
class XMatrix2x2;
class XMatrix3x3;
class XMatrix4x4;
namespace XXml
{
	//下面是关于XML操作的一些函数的封装
	inline TiXmlNode * getXmlAsInt(TiXmlNode *node,const char * name,int &value)
	{
		if (node == NULL || name == NULL) return NULL;
		TiXmlNode *keyNode = node->FirstChildElement(name);
		if(keyNode == NULL || 
			keyNode->ToElement()->GetText() == NULL) return NULL;
		value = atoi(keyNode->ToElement()->GetText());
		return keyNode;
	}
	inline TiXmlNode * getNextXmlAsInt(TiXmlNode *node,const char * name,int &value)
	{
		if (node == NULL || name == NULL) return NULL;
		TiXmlNode *keyNode = node->NextSiblingElement(name);
		if(keyNode == NULL ||
			keyNode->ToElement()->GetText() == NULL) return NULL;
		value = atoi(keyNode->ToElement()->GetText());
		return keyNode;
	}
	inline TiXmlNode * getXmlAsFloat(TiXmlNode *node,const char * name,float &value)
	{
		if (node == NULL || name == NULL) return NULL;
		TiXmlNode *keyNode = node->FirstChildElement(name);
		if(keyNode == NULL ||
			keyNode->ToElement()->GetText() == NULL) return NULL;
		value = (float)atof(keyNode->ToElement()->GetText());
		return keyNode;
	}
	inline TiXmlNode * getNextXmlAsFloat(TiXmlNode *node,const char * name,float &value)
	{
		if (node == NULL || name == NULL) return NULL;
		TiXmlNode *keyNode = node->NextSiblingElement(name);
		if(keyNode == NULL ||
			keyNode->ToElement()->GetText() == NULL) return NULL;
		value = (float)atof(keyNode->ToElement()->GetText());
		return keyNode;
	}
	extern TiXmlNode * getXmlAsBool(TiXmlNode *node,const char * name,bool &value);
	extern TiXmlNode * getNextXmlAsBool(TiXmlNode *node,const char * name,bool &value);
	inline TiXmlNode * getXmlAsString(TiXmlNode *node,const char * name,std::string &value)
	{
		if (node == NULL || name == NULL) return NULL;
		TiXmlNode *keyNode = node->FirstChildElement(name);
		if(keyNode == NULL ||
			keyNode->ToElement()->GetText() == NULL)  return NULL;
#ifdef UNICODE
		value = XString::UTF82ANSI(keyNode->ToElement()->GetText());
#else
		value = keyNode->ToElement()->GetText();
#endif
		return keyNode;
	}
	inline TiXmlNode * getNextXmlAsString(TiXmlNode *node,const char * name,std::string &value)
	{
		if (node == NULL || name == NULL) return NULL;
		TiXmlNode *keyNode = node->NextSiblingElement(name);
		if(keyNode == NULL ||
			keyNode->ToElement()->GetText() == NULL)  return NULL;
#ifdef UNICODE
		value = XString::UTF82ANSI(keyNode->ToElement()->GetText());
#else
		value = keyNode->ToElement()->GetText();
#endif
		return keyNode;
	}
	//下面是是对特殊数据结构的支持
	extern TiXmlNode *getXmlAsVec2(TiXmlNode *node,const char * name,XVector2 &value);
	extern TiXmlNode *getNextXmlAsVec2(TiXmlNode *node,const char * name,XVector2 &value);
	extern TiXmlNode *getXmlAsVec3(TiXmlNode *node,const char * name,XVector3 &value);
	extern TiXmlNode *getNextXmlAsVec3(TiXmlNode *node,const char * name,XVector3 &value);
	extern TiXmlNode *getXmlAsVec4(TiXmlNode *node,const char * name,XVector4 &value);
	extern TiXmlNode *getNextXmlAsVec4(TiXmlNode *node,const char * name,XVector4 &value);
	extern TiXmlNode *getXmlAsFColor(TiXmlNode *node,const char * name,XFColor &value);
	extern TiXmlNode *getNextXmlAsFColor(TiXmlNode *node,const char * name,XFColor &value);
	extern TiXmlNode *getXmlAsCColor(TiXmlNode *node,const char * name,XCColor &value);
	extern TiXmlNode *getNextXmlAsCColor(TiXmlNode *node,const char * name,XCColor &value);
	extern TiXmlNode *getXmlAsMatrix2x2(TiXmlNode *node,const char * name,XMatrix2x2 &value);
	extern TiXmlNode *getNextXmlAsMatrix2x2(TiXmlNode *node,const char * name,XMatrix2x2 &value);
	extern TiXmlNode *getXmlAsMatrix3x3(TiXmlNode *node,const char * name,XMatrix3x3 &value);
	extern TiXmlNode *getNextXmlAsMatrix3x3(TiXmlNode *node,const char * name,XMatrix3x3 &value);
	extern TiXmlNode *getXmlAsMatrix4x4(TiXmlNode *node,const char * name,XMatrix4x4 &value);
	extern TiXmlNode *getNextXmlAsMatrix4x4(TiXmlNode *node,const char * name,XMatrix4x4 &value);

	inline XBool addLeafNode(TiXmlNode &pElmParent,const char* pszNode,const char* pszText)
	{
		if (pszText == NULL || pszNode == NULL) return XFalse;
		TiXmlElement elmNode(pszNode);  
		TiXmlText elmText(pszText);  
		return !(elmNode.InsertEndChild(elmText) == NULL ||
			pElmParent.InsertEndChild(elmNode) == NULL);
		//if(elmNode.InsertEndChild(elmText) == NULL) return XFalse;  
		//if(pElmParent.InsertEndChild(elmNode) == NULL) return XFalse;  
		//return XTrue;  
	}
	inline XBool addComment(TiXmlNode &pElmParent,const char* pszText)
	{
		if (pszText == NULL) return XFalse;
		TiXmlComment elmText(pszText); 
		return !(pElmParent.InsertEndChild(elmText) == NULL);  
	}
	inline XBool addLeafNode(TiXmlNode &pElmParent,const char* pszNode,const std::string &pszText)
	{
		if (pszNode == NULL) return XFalse;
		return addLeafNode(pElmParent,pszNode,pszText.c_str());
	}
	extern XBool addLeafNode(TiXmlNode &pElmParent,const char* pszNode,bool flag);
	inline XBool addLeafNode(TiXmlNode &pElmParent,const char* pszNode,float value)
	{
		return addLeafNode(pElmParent,pszNode,XString::toString(value));
	}
	inline XBool addLeafNode(TiXmlNode &pElmParent,const char* pszNode,int value)
	{
		return addLeafNode(pElmParent,pszNode,XString::toString(value));
	}
	//下main使一些私有的类型
	extern XBool addLeafNode(TiXmlNode &pElmParent,const char* pszNode,const XVector2 &value);
	extern XBool addLeafNode(TiXmlNode &pElmParent,const char* pszNode,const XVector3 &value);
	extern XBool addLeafNode(TiXmlNode &pElmParent,const char* pszNode,const XVector4 &value);
	extern XBool addLeafNode(TiXmlNode &pElmParent,const char* pszNode,const XFColor &value);
	extern XBool addLeafNode(TiXmlNode &pElmParent,const char* pszNode,const XCColor &value);
	extern XBool addLeafNode(TiXmlNode &pElmParent,const char* pszNode,const XMatrix2x2 &value);
	extern XBool addLeafNode(TiXmlNode &pElmParent,const char* pszNode,const XMatrix3x3 &value);
	extern XBool addLeafNode(TiXmlNode &pElmParent,const char* pszNode,const XMatrix4x4 &value);
}
}
#endif