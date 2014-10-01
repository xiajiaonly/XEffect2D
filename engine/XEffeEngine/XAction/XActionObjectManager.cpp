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

char * _XActionObjectDescription::getStrFromData()
{//将数据转换成字符串
	if(!m_isEnable) return NULL;
	char * str = NULL;
	str = createArrayMem<char>(MAX_ACTION_OBJECT_DES_LEN);
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
			char *paramStr = ((_XSpriteParamData *)m_paramData)->getStrFromData();
			if(paramStr == NULL)
			{
				XDELETE_ARRAY(str);
				return NULL;
			}
			sprintf(str + offset,"%s",paramStr);
			XDELETE_ARRAY(paramStr);
		}
		break;
	case OBJ_FRAMEEX:
		{
			char *paramStr = ((_XFrameExParamData *)m_paramData)->getStrFromData();
			if(paramStr == NULL)
			{
				XDELETE_ARRAY(str);
				return NULL;
			}
			sprintf(str + offset,"%s",paramStr);
			XDELETE_ARRAY(paramStr);
		}
		break;
		break;
	}
	return str;
}
int _XActionObjectDescription::getDataFromStr(const char * str)
{//从字符串提取数据
	if(m_isEnable) return 0;
	if(str == NULL) return 0;
	//下面开始打印数据
	int offset = 0;
	if(sscanf(str + offset,"ID:%d,\n",&m_objectID) != 1) return offset;
	offset += getCharPosition(str + offset,',') + 1;
	if(m_objectFilename == NULL) m_objectFilename = createArrayMem<char>(MAX_FILE_NAME_LENGTH);
	if(m_objectFilename == NULL) return 0;
	if(sscanf(str + offset,"Name:%s\n",m_objectFilename) != 1) return offset;	//文件名中不能使用','号
	offset += getCharPosition(str + offset,',') + 1;	//存在问题！！！！！@@@@@@@@@@@@@@@
	if(sscanf(str + offset,"Type:%d,\n",&m_objectType) != 1) return offset;
	offset += getCharPosition(str + offset,',') + 1;
	switch(m_objectType)
	{
	case OBJ_NULL:
		if(m_paramData != NULL) XDELETE(m_paramData);//释放不必要的数据
		break;		//do nothing
	case OBJ_SPRITE:
		{
			if(m_paramData != NULL) XDELETE(m_paramData);//释放不必要的数据
			_XSpriteParamData * paramData = createMem<_XSpriteParamData>();
			int len = paramData->getDataFromStr(str + offset);
			if(len == 0) 
			{
				XDELETE_ARRAY(m_objectFilename);
				return 0;//读取失败
			}
			offset += len;
			m_paramData = paramData;
		}
		break;
	case OBJ_FRAMEEX:
		{
			if(m_paramData != NULL) XDELETE(m_paramData);//释放不必要的数据
			_XFrameExParamData * paramData = createMem<_XFrameExParamData>();
			int len = paramData->getDataFromStr(str + offset);
			if(len == 0) 
			{
				XDELETE_ARRAY(m_objectFilename);
				return 0;//读取失败
			}
			offset += len;
			m_paramData = paramData;
		}
		break;
	}
	m_isEnable = XTrue;
	return offset;
}
_XBool _XActionObjectDescription::getDataFromFile(FILE *fp)
{
	if(m_isEnable) return XFalse;
	if(fp == NULL) return XFalse;
	if(fscanf(fp,"ID:%d,\n",&m_objectID) != 1) return XFalse;
	if(m_objectFilename == NULL) m_objectFilename = createArrayMem<char>(MAX_FILE_NAME_LENGTH);
	if(m_objectFilename == NULL) return XFalse;
	if(fscanf(fp,"Name:%s\n",m_objectFilename) != 1) return XFalse;
	if(fscanf(fp,"Type:%d,\n",&m_objectType) != 1) return XFalse;
	switch(m_objectType)
	{
	case OBJ_NULL:
		if(m_paramData != NULL) XDELETE(m_paramData);//释放不必要的数据
		break;		//do nothing
	case OBJ_SPRITE:
		{
			if(m_paramData != NULL) XDELETE(m_paramData);//释放不必要的数据
			_XSpriteParamData * paramData = createMem<_XSpriteParamData>();
			if(!paramData->getDataFromFile(fp))
			{
				XDELETE_ARRAY(m_objectFilename);
				return XFalse;
			}
			m_paramData = paramData;
		}
		break;
	case OBJ_FRAMEEX:
		{
			if(m_paramData != NULL) XDELETE(m_paramData);//释放不必要的数据
			_XFrameExParamData * paramData = createMem<_XFrameExParamData>();
			if(!paramData->getDataFromFile(fp))
			{
				XDELETE_ARRAY(m_objectFilename);
				return XFalse;
			}
			m_paramData = paramData;
		}
		break;
	}
	m_isEnable = XTrue;
	return XTrue;
}
_XObjectBasic * _XActionObjectDescription::createAObject()
{//根据物件的描述重新建立一个物体
	if(!m_isEnable) return NULL;
	switch(m_objectType)
	{
	case OBJ_NULL: return NULL;break;
	case OBJ_SPRITE: 
		{
			_XSprite *temp = createMem<_XSprite>();
			if(temp == NULL) return NULL;
			if(m_paramData == NULL) return NULL;
			_XSpriteParamData * tempParamData = (_XSpriteParamData *) m_paramData;
			if(!temp->init(m_objectFilename,tempParamData->resourcePosition,tempParamData->changePoint))
			{
				XDELETE(temp);
				return NULL;
			}	//目前先使用默认参数，因为参数保存不完整，后面需要修正
			//注册物件
			_XActionObjectManager::GetInstance().addAObject(temp,this);
			return temp;
		}
		break;
	case OBJ_FRAMEEX: 
		{
			_XFrameEx *temp = createMem<_XFrameEx>();
			if(temp == NULL) return NULL;
			if(m_paramData == NULL) return NULL;
			_XFrameExParamData * tempParamData = (_XFrameExParamData *) m_paramData;
			if(!temp->init(m_objectFilename,tempParamData->resourcePosition))
			{
				XDELETE(temp);
				return NULL;
			}	//目前先使用默认参数，因为参数保存不完整，后面需要修正
			//初始化物件的参数
			temp->setAttribute(tempParamData->pos,tempParamData->loop,tempParamData->endImmediately,tempParamData->startFrame,
				tempParamData->actionSpeed,tempParamData->disappearAtEnd,tempParamData->isOverturn);
			//注册物件
			_XActionObjectManager::GetInstance().addAObject(temp,this);
			return temp;
		}
		break;
	}
	return NULL;
}