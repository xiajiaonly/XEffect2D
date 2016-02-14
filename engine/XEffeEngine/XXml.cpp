#include "XStdHead.h"
#include "XXml.h"
#include "XMath\XVector3.h"
#include "XMath\XVector4.h"
#include "XMath\XFColor.h"
#include "XMath\XCColor.h"
#include "XMath\XMatrix2x2.h"
#include "XMath\XMatrix3x3.h"
#include "XMath\XMatrix4x4.h"
namespace XE{
namespace XXml
{
	TiXmlNode * getXmlAsBool(TiXmlNode *node,const char * name,bool &value)
	{
		if (node == NULL || name == NULL) return NULL;
		TiXmlNode *keyNode = node->FirstChildElement(name);
		if(keyNode == NULL ||
			keyNode->ToElement()->GetText() == NULL) return NULL; 
		value = (atoi(keyNode->ToElement()->GetText()) != 0);
		return keyNode;
	}
	TiXmlNode * getNextXmlAsBool(TiXmlNode *node,const char * name,bool &value)
	{
		if (node == NULL || name == NULL) return NULL;
		TiXmlNode *keyNode = node->NextSiblingElement(name);
		if(keyNode == NULL ||
			keyNode->ToElement()->GetText() == NULL) return NULL; 
		value = (atoi(keyNode->ToElement()->GetText()) != 0);
		return keyNode;
	}
	XBool addLeafNode(TiXmlNode &pElmParent,const char* pszNode,bool flag)  
	{  
		if (pszNode == NULL) return XFalse;
		std::string text("0");
		if(flag) text = "1";
		TiXmlElement elmNode(pszNode);  
		TiXmlText elmText(text.c_str());  
		return !(elmNode.InsertEndChild(elmText) == NULL ||
			pElmParent.InsertEndChild(elmNode) == NULL);
		//if(elmNode.InsertEndChild(elmText) == NULL) return XFalse;  
		//if(pElmParent.InsertEndChild(elmNode) == NULL) return XFalse;  
		//return XTrue;  
	} 
	TiXmlNode *getXmlAsVec2(TiXmlNode *node,const char * name,XVector2 &value)
	{
		if (node == NULL || name == NULL) return NULL;
		TiXmlNode *keyNode = node->FirstChildElement(name);
		if(keyNode == NULL ||
			keyNode->ToElement()->GetText() == NULL) return NULL; 
		if(sscanf(keyNode->ToElement()->GetText(),"%f,%f",&value.x,&value.y) != 2) return NULL;
		return keyNode;
	}
	TiXmlNode *getNextXmlAsVec2(TiXmlNode *node,const char * name,XVector2 &value)
	{
		if (node == NULL || name == NULL) return NULL;
		TiXmlNode *keyNode = node->NextSiblingElement(name);
		if(keyNode == NULL ||
			keyNode->ToElement()->GetText() == NULL) return NULL; 
		if(sscanf(keyNode->ToElement()->GetText(),"%f,%f",&value.x,&value.y) != 2) return NULL;
		return keyNode;
	}
	TiXmlNode *getXmlAsVec3(TiXmlNode *node,const char * name,XVector3 &value)
	{
		if (node == NULL || name == NULL) return NULL;
		TiXmlNode *keyNode = node->FirstChildElement(name);
		if(keyNode == NULL ||
			keyNode->ToElement()->GetText() == NULL) return NULL; 
		if(sscanf(keyNode->ToElement()->GetText(),"%f,%f,%f",&value.x,&value.y,&value.z) != 3) return NULL;
		return keyNode;
	}
	TiXmlNode *getNextXmlAsVec3(TiXmlNode *node,const char * name,XVector3 &value)
	{
		if (node == NULL || name == NULL) return NULL;
		TiXmlNode *keyNode = node->NextSiblingElement(name);
		if(keyNode == NULL ||
			keyNode->ToElement()->GetText() == NULL) return NULL; 
		if(sscanf(keyNode->ToElement()->GetText(),"%f,%f,%f",&value.x,&value.y,&value.z) != 3) return NULL;
		return keyNode;
	}
	TiXmlNode *getXmlAsVec4(TiXmlNode *node,const char * name,XVector4 &value)
	{
		if (node == NULL || name == NULL) return NULL;
		TiXmlNode *keyNode = node->FirstChildElement(name);
		if(keyNode == NULL ||
			keyNode->ToElement()->GetText() == NULL) return NULL; 
		if(sscanf(keyNode->ToElement()->GetText(),"%f,%f,%f,%f",&value.x,&value.y,&value.z,&value.w) != 4) return NULL;
		return keyNode;
	}
	TiXmlNode *getNextXmlAsVec4(TiXmlNode *node,const char * name,XVector4 &value)
	{
		if (node == NULL || name == NULL) return NULL;
		TiXmlNode *keyNode = node->NextSiblingElement(name);
		if(keyNode == NULL ||
			keyNode->ToElement()->GetText() == NULL) return NULL; 
		if(sscanf(keyNode->ToElement()->GetText(),"%f,%f,%f,%f",&value.x,&value.y,&value.z,&value.w) != 4) return NULL;
		return keyNode;
	}
	TiXmlNode *getXmlAsFColor(TiXmlNode *node,const char * name,XFColor &value)
	{
		if (node == NULL || name == NULL) return NULL;
		TiXmlNode *keyNode = node->FirstChildElement(name);
		if(keyNode == NULL ||
			keyNode->ToElement()->GetText() == NULL) return NULL; 
		if(sscanf(keyNode->ToElement()->GetText(),"%f,%f,%f,%f",&value.fR,&value.fG,&value.fB,&value.fA) != 4) return NULL;
		return keyNode;
	}
	TiXmlNode *getNextXmlAsFColor(TiXmlNode *node,const char * name,XFColor &value)
	{
		if (node == NULL || name == NULL) return NULL;
		TiXmlNode *keyNode = node->NextSiblingElement(name);
		if(keyNode == NULL ||
			keyNode->ToElement()->GetText() == NULL) return NULL; 
		if(sscanf(keyNode->ToElement()->GetText(),"%f,%f,%f,%f",&value.fR,&value.fG,&value.fB,&value.fA) != 4) return NULL;
		return keyNode;
	}
	TiXmlNode *getXmlAsCColor(TiXmlNode *node,const char * name,XCColor &value)
	{
		if (node == NULL || name == NULL) return NULL;
		TiXmlNode *keyNode = node->FirstChildElement(name);
		if(keyNode == NULL ||
			keyNode->ToElement()->GetText() == NULL) return NULL; 
		//下面这种方式会存在问题，向较小的内存中写入了较大的数据
		//if(sscanf(keyNode->ToElement()->GetText(),"%d,%d,%d,%d",&value.r,&value.g,&value.b,&value.a) != 4) return NULL;
		int r,g,b,a;
		if(sscanf(keyNode->ToElement()->GetText(),"%d,%d,%d,%d",&r,&g,&b,&a) != 4) return NULL;
		value.setColor(r,g,b,a);
		return keyNode;
	}
	TiXmlNode *getNextXmlAsCColor(TiXmlNode *node,const char * name,XCColor &value)
	{
		if (node == NULL || name == NULL) return NULL;
		TiXmlNode *keyNode = node->NextSiblingElement(name);
		if(keyNode == NULL ||
			keyNode->ToElement()->GetText() == NULL) return NULL; 
		//if(sscanf(keyNode->ToElement()->GetText(),"%d,%d,%d,%d",&value.r,&value.g,&value.b,&value.a) != 4) return NULL;
		int r,g,b,a;
		if(sscanf(keyNode->ToElement()->GetText(),"%d,%d,%d,%d",&r,&g,&b,&a) != 4) return NULL;
		value.setColor(r,g,b,a);
		return keyNode;
	}
	TiXmlNode *getXmlAsMatrix2x2(TiXmlNode *node,const char * name,XMatrix2x2 &value)
	{
		if (node == NULL || name == NULL) return NULL;
		TiXmlNode *keyNode = node->FirstChildElement(name);
		if(keyNode == NULL ||
			keyNode->ToElement()->GetText() == NULL) return NULL; 
		if(sscanf(keyNode->ToElement()->GetText(),"%f,%f,%f,%f",&value.data[0],&value.data[1],&value.data[2],&value.data[3]) != 4) return NULL;
		return keyNode;
	}
	TiXmlNode *getNextXmlAsMatrix2x2(TiXmlNode *node,const char * name,XMatrix2x2 &value)
	{
		if (node == NULL || name == NULL) return NULL;
		TiXmlNode *keyNode = node->NextSiblingElement(name);
		if(keyNode == NULL ||
			keyNode->ToElement()->GetText() == NULL) return NULL; 
		if(sscanf(keyNode->ToElement()->GetText(),"%f,%f,%f,%f",&value.data[0],&value.data[1],&value.data[2],&value.data[3]) != 4) return NULL;
		return keyNode;
	}
	TiXmlNode *getXmlAsMatrix3x3(TiXmlNode *node,const char * name,XMatrix3x3 &value)
	{
		if (node == NULL || name == NULL) return NULL;
		TiXmlNode *keyNode = node->FirstChildElement(name);
		if(keyNode == NULL ||
			keyNode->ToElement()->GetText() == NULL) return NULL; 
		if(sscanf(keyNode->ToElement()->GetText(),"%f,%f,%f,%f,%f,%f,%f,%f,%f",&value.data[0],&value.data[1],&value.data[2],&value.data[3],
			&value.data[4],&value.data[5],&value.data[6],&value.data[7],&value.data[8]) != 9) return NULL;
		return keyNode;
	}
	TiXmlNode *getNextXmlAsMatrix3x3(TiXmlNode *node,const char * name,XMatrix3x3 &value)
	{
		if (node == NULL || name == NULL) return NULL;
		TiXmlNode *keyNode = node->NextSiblingElement(name);
		if(keyNode == NULL ||
			keyNode->ToElement()->GetText() == NULL) return NULL; 
		if(sscanf(keyNode->ToElement()->GetText(),"%f,%f,%f,%f,%f,%f,%f,%f,%f",&value.data[0],&value.data[1],&value.data[2],&value.data[3],
			&value.data[4],&value.data[5],&value.data[6],&value.data[7],&value.data[8]) != 9) return NULL;
		return keyNode;
	}
	TiXmlNode *getXmlAsMatrix4x4(TiXmlNode *node,const char * name,XMatrix4x4 &value)
	{
		if (node == NULL || name == NULL) return NULL;
		TiXmlNode *keyNode = node->FirstChildElement(name);
		if(keyNode == NULL ||
			keyNode->ToElement()->GetText() == NULL) return NULL; 
		if(sscanf(keyNode->ToElement()->GetText(),"%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",&value.data[0],&value.data[1],&value.data[2],&value.data[3],
			&value.data[4],&value.data[5],&value.data[6],&value.data[7],&value.data[8],&value.data[9],&value.data[10],&value.data[11],&value.data[12],&value.data[13],
			&value.data[14],&value.data[15]) != 16) return NULL;
		return keyNode;
	}
	TiXmlNode *getNextXmlAsMatrix4x4(TiXmlNode *node,const char * name,XMatrix4x4 &value)
	{
		if (node == NULL || name == NULL) return NULL;
		TiXmlNode *keyNode = node->NextSiblingElement(name);
		if(keyNode == NULL ||
			keyNode->ToElement()->GetText() == NULL) return NULL; 
		if(sscanf(keyNode->ToElement()->GetText(),"%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",&value.data[0],&value.data[1],&value.data[2],&value.data[3],
			&value.data[4],&value.data[5],&value.data[6],&value.data[7],&value.data[8],&value.data[9],&value.data[10],&value.data[11],&value.data[12],&value.data[13],
			&value.data[14],&value.data[15]) != 16) return NULL;
		return keyNode;
	}
	XBool addLeafNode(TiXmlNode &pElmParent,const char* pszNode,const XVector2 &value)
	{
		if (pszNode == NULL) return XFalse;
		char tmp[32];	//可能会由于数据的长度而造成内存溢出，这里应该考虑使用std::string的方法。
		sprintf(tmp,"%f,%f",value.x,value.y);
		return addLeafNode(pElmParent,pszNode,tmp);
	}
	XBool addLeafNode(TiXmlNode &pElmParent,const char* pszNode,const XVector3 &value)
	{
		if (pszNode == NULL) return XFalse;
		char tmp[32];	//可能会由于数据的长度而造成内存溢出，这里应该考虑使用std::string的方法。
		sprintf(tmp,"%f,%f,%f",value.x,value.y,value.z);
		return addLeafNode(pElmParent,pszNode,tmp);
	}
	XBool addLeafNode(TiXmlNode &pElmParent,const char* pszNode,const XVector4 &value)
	{
		if (pszNode == NULL) return XFalse;
		char tmp[64];	//可能会由于数据的长度而造成内存溢出，这里应该考虑使用std::string的方法。
		sprintf(tmp,"%f,%f,%f,%f",value.x,value.y,value.z,value.w);
		return addLeafNode(pElmParent,pszNode,tmp);
	}
	XBool addLeafNode(TiXmlNode &pElmParent,const char* pszNode,const XFColor &value)
	{
		if (pszNode == NULL) return XFalse;
		char tmp[64];	//可能会由于数据的长度而造成内存溢出，这里应该考虑使用std::string的方法。
		sprintf(tmp,"%f,%f,%f,%f",value.fR,value.fG,value.fB,value.fA);
		return addLeafNode(pElmParent,pszNode,tmp);
	}
	XBool addLeafNode(TiXmlNode &pElmParent,const char* pszNode,const XCColor &value)
	{
		if (pszNode == NULL) return XFalse;
		char tmp[64];	//可能会由于数据的长度而造成内存溢出，这里应该考虑使用std::string的方法。
		sprintf(tmp,"%d,%d,%d,%d",value.r,value.g,value.b,value.a);
		return addLeafNode(pElmParent,pszNode,tmp);
	}
	XBool addLeafNode(TiXmlNode &pElmParent,const char* pszNode,const XMatrix2x2 &value)
	{
		if (pszNode == NULL) return XFalse;
		char tmp[64];	//可能会由于数据的长度而造成内存溢出，这里应该考虑使用std::string的方法。
		sprintf(tmp,"%f,%f,%f,%f",value.data[0],value.data[1],value.data[2],value.data[3]);
		return addLeafNode(pElmParent,pszNode,tmp);
	}
	XBool addLeafNode(TiXmlNode &pElmParent,const char* pszNode,const XMatrix3x3 &value)
	{
		if (pszNode == NULL) return XFalse;
		char tmp[128];	//可能会由于数据的长度而造成内存溢出，这里应该考虑使用std::string的方法。
		sprintf(tmp,"%f,%f,%f,%f,%f,%f,%f,%f,%f",value.data[0],value.data[1],value.data[2],value.data[3],
			value.data[4],value.data[5],value.data[6],value.data[7],value.data[8]);
		return addLeafNode(pElmParent,pszNode,tmp);
	}
	XBool addLeafNode(TiXmlNode &pElmParent,const char* pszNode,const XMatrix4x4 &value)
	{
		if (pszNode == NULL) return XFalse;
		char tmp[256];	//可能会由于数据的长度而造成内存溢出，这里应该考虑使用std::string的方法。
		sprintf(tmp,"%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",value.data[0],value.data[1],value.data[2],value.data[3],
			value.data[4],value.data[5],value.data[6],value.data[7],value.data[8],value.data[9],value.data[10],value.data[11],value.data[12],
			value.data[13],value.data[14],value.data[15]);
		return addLeafNode(pElmParent,pszNode,tmp);
	}

}
}