#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XActionObjectManager.h"
#include "XSprite.h"
#include "XFrameEx.h"
//对action中的资源进行管理的类
#include "string.h"
namespace XE{
char * XActionObjectDescription::getStrFromData()
{//将数据转换成字符串
	if(!m_isEnable) return NULL;
	char * str = NULL;
	str = XMem::createArrayMem<char>(MAX_ACTION_OBJECT_DES_LEN);
	if(str == NULL) return NULL;
	//下面开始打印数据
	int offset = 0;
	sprintf(str + offset,"ID:%d,\n",m_objectID);
	offset = strlen(str);
	sprintf(str + offset,"Name:%s\n",m_objectFilename);
	offset = strlen(str);
	sprintf(str + offset,"Type:%d,\n",m_objectType);
	offset = strlen(str);
	switch(m_objectType)
	{
	case OBJ_NULL:break;		//do nothing
	case OBJ_SPRITE:
		{
			char *paramStr = ((XSpriteParamData *)m_paramData)->getStrFromData();
			if(paramStr == NULL)
			{
				XMem::XDELETE_ARRAY(str);
				return NULL;
			}
			sprintf(str + offset,"%s",paramStr);
			XMem::XDELETE_ARRAY(paramStr);
		}
		break;
	case OBJ_FRAMEEX:
		{
			char *paramStr = ((XFrameExParamData *)m_paramData)->getStrFromData();
			if(paramStr == NULL)
			{
				XMem::XDELETE_ARRAY(str);
				return NULL;
			}
			sprintf(str + offset,"%s",paramStr);
			XMem::XDELETE_ARRAY(paramStr);
		}
		break;
		break;
	}
	return str;
}
void XActionObjectDescription::releaseParamData()
{
	if(m_paramData == NULL) return;
	switch(m_objectType)
	{
	case OBJ_NULL:
		XMem::XDELETE(m_paramData);
		break;
	case OBJ_SPRITE:
		{
			XSpriteParamData *tmp = (XSpriteParamData *)m_paramData;
			XMem::XDELETE(tmp);
			m_paramData = NULL;
		}
		break;
	case OBJ_FRAMEEX:
		{
			XFrameExParamData *tmp = (XFrameExParamData *)m_paramData;
			XMem::XDELETE(tmp);
			m_paramData = NULL;
		}
		break;
	}
}
int XActionObjectDescription::getDataFromStr(const char * str)
{//从字符串提取数据
	if(m_isEnable ||
		str == NULL) return 0;
	//下面开始打印数据
	int offset = 0;
	if(sscanf(str + offset,"ID:%d,\n",&m_objectID) != 1) return offset;
	offset += XString::getCharPosition(str + offset,',') + 1;
	if(m_objectFilename == NULL) m_objectFilename = XMem::createArrayMem<char>(MAX_FILE_NAME_LENGTH);
	if(m_objectFilename == NULL) return 0;
	if(sscanf(str + offset,"Name:%s\n",m_objectFilename) != 1) return offset;	//文件名中不能使用','号
	offset += XString::getCharPosition(str + offset,',') + 1;	//存在问题！！！！！@@@@@@@@@@@@@@@
	XObjectType tmpType;
	if(sscanf(str + offset,"Type:%d,\n",&tmpType) != 1) return offset;
	offset += XString::getCharPosition(str + offset,',') + 1;
	switch(tmpType)
	{
	case OBJ_NULL:
		releaseParamData();
		break;		//do nothing
	case OBJ_SPRITE:
		{
			releaseParamData();
			XSpriteParamData * paramData = XMem::createMem<XSpriteParamData>();
			int len = paramData->getDataFromStr(str + offset);
			if(len == 0) 
			{
				XMem::XDELETE_ARRAY(m_objectFilename);
				return 0;//读取失败
			}
			offset += len;
			m_paramData = paramData;
		}
		break;
	case OBJ_FRAMEEX:
		{
			releaseParamData();
			XFrameExParamData * paramData = XMem::createMem<XFrameExParamData>();
			int len = paramData->getDataFromStr(str + offset);
			if(len == 0) 
			{
				XMem::XDELETE_ARRAY(m_objectFilename);
				return 0;//读取失败
			}
			offset += len;
			m_paramData = paramData;
		}
		break;
	}
	m_objectType = tmpType;
	m_isEnable = XTrue;
	return offset;
}
XBool XActionObjectDescription::getDataFromFile(FILE *fp)
{
	if(m_isEnable ||
		fp == NULL) return XFalse;
	if(fscanf(fp,"ID:%d,\n",&m_objectID) != 1) return XFalse;
	if(m_objectFilename == NULL) m_objectFilename = XMem::createArrayMem<char>(MAX_FILE_NAME_LENGTH);
	if(m_objectFilename == NULL) return XFalse;
	if(fscanf(fp,"Name:%s\n",m_objectFilename) != 1) return XFalse;
	if(fscanf(fp,"Type:%d,\n",&m_objectType) != 1) return XFalse;
	switch(m_objectType)
	{
	case OBJ_NULL:
		releaseParamData();
		break;		//do nothing
	case OBJ_SPRITE:
		{
			releaseParamData();
			XSpriteParamData * paramData = XMem::createMem<XSpriteParamData>();
			if(!paramData->getDataFromFile(fp))
			{
				XMem::XDELETE_ARRAY(m_objectFilename);
				return XFalse;
			}
			m_paramData = paramData;
		}
		break;
	case OBJ_FRAMEEX:
		{
			releaseParamData();
			XFrameExParamData * paramData = XMem::createMem<XFrameExParamData>();
			if(!paramData->getDataFromFile(fp))
			{
				XMem::XDELETE_ARRAY(m_objectFilename);
				return XFalse;
			}
			m_paramData = paramData;
		}
		break;
	}
	m_isEnable = XTrue;
	return XTrue;
}
XObjectBasic * XActionObjectDescription::createAObject()
{//根据物件的描述重新建立一个物体
	if(!m_isEnable) return NULL;
	switch(m_objectType)
	{
	case OBJ_NULL: return NULL;break;
	case OBJ_SPRITE: 
		{
			XSprite *temp = XMem::createMem<XSprite>();
			if(temp == NULL || m_paramData == NULL) return NULL;
			XSpriteParamData * tempParamData = (XSpriteParamData *) m_paramData;
			if(!temp->init(m_objectFilename,tempParamData->resourcePosition,tempParamData->changePoint))
			{
				XMem::XDELETE(temp);
				return NULL;
			}	//目前先使用默认参数，因为参数保存不完整，后面需要修正
			//注册物件
			XActionObjectManager::GetInstance().addAObject(temp,this);
			return temp;
		}
		break;
	case OBJ_FRAMEEX: 
		{
			XFrameEx *temp = XMem::createMem<XFrameEx>();
			if(temp == NULL || m_paramData == NULL) return NULL;
			XFrameExParamData * tempParamData = (XFrameExParamData *) m_paramData;
			if(!temp->init(m_objectFilename,tempParamData->resourcePosition))
			{
				XMem::XDELETE(temp);
				return NULL;
			}	//目前先使用默认参数，因为参数保存不完整，后面需要修正
			//初始化物件的参数
			temp->setAttribute(tempParamData->pos,tempParamData->loop,tempParamData->endImmediately,tempParamData->startFrame,
				tempParamData->actionSpeed,tempParamData->disappearAtEnd,tempParamData->isOverturn);
			//注册物件
			XActionObjectManager::GetInstance().addAObject(temp,this);
			return temp;
		}
		break;
	}
	return NULL;
}
}