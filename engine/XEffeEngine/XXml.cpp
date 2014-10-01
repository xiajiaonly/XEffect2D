#include "XXml.h"

TiXmlNode * getXmlAsBool(TiXmlNode *node,const char * name,bool &value)
{
	static TiXmlNode *keyNode = NULL;
	keyNode = node->FirstChildElement(name);
	if(keyNode != NULL)
	{
		if(keyNode->ToElement()->GetText() == NULL) return NULL; 
		if(atoi(keyNode->ToElement()->GetText()) != 0) value = true;
		else value = false;
		return keyNode;
	}
	return NULL;
}
TiXmlNode * getNextXmlAsBool(TiXmlNode *node,const char * name,bool &value)
{
	static TiXmlNode *keyNode = NULL;
	keyNode = node->NextSiblingElement(name);
	if(keyNode != NULL)
	{
		if(keyNode->ToElement()->GetText() == NULL) return NULL; 
		if(atoi(keyNode->ToElement()->GetText()) != 0) value = true;
		else value = false;
		return keyNode;
	}
	return NULL;
}
_XBool addLeafNode(TiXmlNode &pElmParent,const char* pszNode,bool flag)  
{  
	std::string text;
	if(flag) text = "1";
	else text = "0";
    TiXmlElement elmNode(pszNode);  
	TiXmlText elmText(text.c_str());  
    if(elmNode.InsertEndChild(elmText) == NULL) return XFalse;  
    if(pElmParent.InsertEndChild(elmNode) == NULL) return XFalse;  
    return XTrue;  
} 