#ifndef _JIA_ACTIONOBJECTMANAGER_
#define _JIA_ACTIONOBJECTMANAGER_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.5
//--------------------------------
//对action中的资源进行管理的类
//#include "XBasicFun.h"
#include "XObjectBasic.h"
namespace XE{
//enum XActionObjectType
//{
//	ACTION_OBJ_TYPE_NULL,		//未定义
//	ACTION_OBJ_TYPE_SPRITE,		//精灵
//	ACTION_OBJ_TYPE_FRAMEEX,	//序列帧Ex
//};
#define MAX_ACTION_OBJECT_DES_LEN (1024)
//action中物件的描述
struct XActionObjectDescription
{
	XBool m_isEnable;			//是否有效
	int m_objectID;				//物件的ID
	char *m_objectFilename;		//物件的文件名 MAX_FILE_NAME_LENGTH
	XObjectType m_objectType;
	void * m_paramData;			//定义的参数数据，不同的物件参数数据不同所以这里需要额外定义

	XActionObjectDescription()
		:m_objectFilename(NULL)
		,m_objectType(OBJ_NULL)
		,m_paramData(NULL)
		,m_isEnable(XFalse)
	{}
	~XActionObjectDescription(){release();}
	void release()
	{
		if(!m_isEnable) return;
		XMem::XDELETE_ARRAY(m_objectFilename);
		releaseParamData();
		m_isEnable = XFalse;
	}
	char * getStrFromData();
	int getDataFromStr(const char * str);
	void releaseParamData();	//释放参数的内存空间
	XBool getDataFromFile(FILE *fp);
	//加入引用计数器
	//资源释放
	//等

	XObjectBasic * createAObject();	//根据数据生成具体的物件
};
#define MAX_ACTION_OBJECT_SUM (1024)	//最多的物件数量
class XActionObjectManager
{
	//+++++++++++++++++++++++++++++++++++++++++++
	//下面需要将其设计为Singleton模式
protected:
	XActionObjectManager()
		:m_objectSum(0)
	{
		m_pObject = XMem::createArrayMem<XObjectBasic *>(MAX_ACTION_OBJECT_SUM);
		m_pObjDes = XMem::createArrayMem<XActionObjectDescription *>(MAX_ACTION_OBJECT_SUM);
	}
	XActionObjectManager(const XActionObjectManager&);
	XActionObjectManager &operator= (const XActionObjectManager&);
	virtual ~XActionObjectManager(){release();}
public:
	static XActionObjectManager& GetInstance()
	{
		static XActionObjectManager m_instance;
		return m_instance;
	}
	//-------------------------------------------
private:
	XObjectBasic ** m_pObject;	//物件的指针队列
	XActionObjectDescription ** m_pObjDes;	//物件的描述
	int m_objectSum;						//物件的数量

	void release()
	{//尚未实现
		for(int i = 0;i < m_objectSum;++ i)
		{
			XMem::XDELETE(m_pObject[i]);
			XMem::XDELETE(m_pObjDes[i]);
		}
		m_objectSum = 0;
		XMem::XDELETE_ARRAY(m_pObject);
		XMem::XDELETE_ARRAY(m_pObjDes);
	}
public:
	void releaseAllObject()
	{//释放掉所有的物件资源
		for(int i = 0;i < m_objectSum;++ i)
		{
			XMem::XDELETE(m_pObject[i]);
			XMem::XDELETE(m_pObjDes[i]);
		}
		m_objectSum = 0;
	}
	XActionObjectDescription * getObjDes(const XObjectBasic * pObj)
	{//根据物件指针获取物件的描述信息
		if(pObj == NULL) return NULL;
		for(int i = 0;i < m_objectSum;++ i)
		{
			if(m_pObject[i] == pObj) return m_pObjDes[i];
		}
		return NULL;
	}
	int addAObject(XObjectBasic * pObj,XActionObjectDescription *pDes)
	{
		if(m_objectSum >= MAX_ACTION_OBJECT_SUM) return 0;
		if(pObj == NULL || pDes == NULL) return 0;
		if(getObjDes(pObj) != NULL) return 0;	//防止重复加载
		m_pObject[m_objectSum] = pObj;
		m_pObjDes[m_objectSum] = pDes;
		++ m_objectSum;
		return 1;
	}
	XBool decreaseAObject(XObjectBasic * pObj)
	{
		if(pObj == NULL) return XFalse;
		for(int i = 0;i < m_objectSum;++ i)
		{
			if(m_pObject[i] == pObj)
			{
				XMem::XDELETE(m_pObject[i]);
				XMem::XDELETE(m_pObjDes[i]);
				for(int j = i;j < m_objectSum - 1;++ j)
				{
					m_pObject[j] = m_pObject[j + 1];
					m_pObjDes[j] = m_pObjDes[j + 1];
				}
				-- m_objectSum;
				return XTrue;
			}
		}
		return XFalse;
	}
};
}
#endif //_JIA_ACTIONOBJECTMANAGER_
