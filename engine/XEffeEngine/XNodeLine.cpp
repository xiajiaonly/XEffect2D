//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XNodeLine.h"
#include "XBasicFun.h"
#include "XBasicWindow.h"
//#include "stdio.h"
//#include <math.h>
#include "XResourcePack.h"
#include "XObjectManager.h"
#include "XBasicOpenGL.h"

_XNodeLine::_XNodeLine()
:m_node(NULL)
,m_isACopy(0)
,m_myOrder(0)
,m_leadMode(LEAD_MODE_FOREWORD)	//曲线的引导模式
,m_lineLength(0)					//曲线的总长度
//,m_nodeSum(0)						//引导曲线中的引导节点的数量
,m_offsetPosition(0,0)
,m_size(1.0f,1.0f)
,m_isLoop(0)
,m_isVisiable(1)
{
	m_node = createArrayMem<_XNode>(MAX_NODE_SUM);
	if(m_node == NULL) printf("Memory error!\n");
	m_bezierPoint = createArrayMem<_XVector2>(MAX_NODE_SUM * 2);
	if(m_bezierPoint == NULL) printf("Memory error!\n");
	try
	{
		m_cp = new _XSCounter;
	}catch(...)
	{
		printf("Memory error!\n");
	}
	m_specialPoint.isEnable = 0;
	clearUp();
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_NODELINE);
#endif
}

_XNodeLine::~_XNodeLine()
{
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{//自身没有引用需要释放
		XDELETE_ARRAY(m_node);
		XDELETE_ARRAY(m_bezierPoint);
		XDELETE(m_cp);
	}
	release();
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(this);
#endif
}

void _XNodeLine::addOneNode(float x,float y)		//向曲线头部添加一个点
{
	if(m_isACopy != 0) return;//副本不能进行这个操作
	int prevNodeNumber;
	if(m_nodeSum >= MAX_NODE_SUM || m_node == NULL) return;	//如果记录的电已经满了则直接返回
	m_node[m_nodeSum].myPoint.set(x,y);		//记录当前的点的信息
	m_node[m_nodeSum].toNextAngle = 0;
	m_node[m_nodeSum].toNextCos = 0;
	m_node[m_nodeSum].toNextSin = 0;
	m_node[m_nodeSum].toNextLength = 0;
	if(m_nodeSum > 0)
	{//填写上一个点的相关引导信息
		prevNodeNumber = m_nodeSum - 1;
		m_node[prevNodeNumber].toNextAngle = m_node[prevNodeNumber].myPoint.getAngle(m_node[m_nodeSum].myPoint);
		m_node[prevNodeNumber].toNextCos = cos(m_node[prevNodeNumber].toNextAngle);
		m_node[prevNodeNumber].toNextSin = sin(m_node[prevNodeNumber].toNextAngle);
		m_node[prevNodeNumber].toNextLength = m_node[prevNodeNumber].myPoint.getLength(m_node[m_nodeSum].myPoint);

		//填写当前点的相关信息
		m_node[m_nodeSum].toPreviousAngle = m_node[prevNodeNumber].toNextAngle + PI;
		if(m_node[m_nodeSum].toPreviousAngle >= PI2) m_node[m_nodeSum].toPreviousAngle -= PI2;
		m_node[m_nodeSum].toPreviousCos = cos(m_node[m_nodeSum].toPreviousAngle);
		m_node[m_nodeSum].toPreviousSin = sin(m_node[m_nodeSum].toPreviousAngle);
		m_node[m_nodeSum].toPreviousLength = m_node[prevNodeNumber].toNextLength;
		m_lineLength += m_node[m_nodeSum].toPreviousLength;
		
		if(m_isLoop == 0)
		{
			//最后一个点的方向与前一个点的方向保持一致
			m_node[m_nodeSum].toNextAngle = m_node[prevNodeNumber].toNextAngle;
			m_node[m_nodeSum].toNextCos = m_node[prevNodeNumber].toNextCos;
			m_node[m_nodeSum].toNextSin = m_node[prevNodeNumber].toNextSin;
			if(m_nodeSum == 1)
			{//头的点保持后一个点的方向
				m_node[0].toPreviousAngle = m_node[1].toPreviousAngle;
				m_node[0].toPreviousCos = m_node[1].toPreviousCos;
				m_node[0].toPreviousSin = m_node[1].toPreviousSin;
			}
		}
	}
	++ m_nodeSum;
	if(m_isLoop != 0) setLoop();	//设置曲线闭合
	m_needUpdateData = XTrue;
}

void _XNodeLine::setOneNode(float x,float y,int nodeOrder)
{
	if(m_isACopy != 0) return;//副本不能进行这个操作
	if(m_node == NULL) return;
	if(nodeOrder == 0)
	{//头
		m_lineLength -= m_node[0].toNextLength;
		m_node[0].myPoint.set(x,y);
		m_node[0].toNextAngle = m_node[0].myPoint.getAngle(m_node[1].myPoint);
		m_node[0].toNextCos = cos(m_node[0].toNextAngle);
		m_node[0].toNextSin = sin(m_node[0].toNextAngle);
		m_node[0].toNextLength = m_node[0].myPoint.getLength(m_node[1].myPoint);

		m_lineLength += m_node[0].toNextLength;
		m_node[1].toPreviousAngle = m_node[0].toNextAngle + PI;
		if(m_node[1].toPreviousAngle >= PI2) m_node[1].toPreviousAngle -= PI2;
		m_node[1].toPreviousCos = cos(m_node[1].toPreviousAngle);
		m_node[1].toPreviousSin = sin(m_node[1].toPreviousAngle);
		m_node[1].toPreviousLength = m_node[0].toNextLength;
		if(m_isLoop == 0)
		{
			if(m_nodeSum > 1)
			{//头的点保持后一个点的方向
				m_node[0].toPreviousAngle = m_node[1].toPreviousAngle;
				m_node[0].toPreviousCos = m_node[1].toPreviousCos;
				m_node[0].toPreviousSin = m_node[1].toPreviousSin;
			}
			if(m_nodeSum == 2)
			{
				m_node[1].toNextAngle = m_node[0].toNextAngle;
				m_node[1].toNextCos = m_node[0].toNextCos;
				m_node[1].toNextSin = m_node[0].toNextSin;
			}
		}
		if(m_isLoop != 0) setLoop();
	}else
	if(nodeOrder == m_nodeSum - 1)
	{//尾
		m_lineLength -= m_node[nodeOrder].toPreviousLength;
		m_node[nodeOrder].myPoint.set(x,y);		//记录当前的点的信息
		m_node[nodeOrder].toNextAngle = 0;
		m_node[nodeOrder].toNextCos = 0;
		m_node[nodeOrder].toNextSin = 0;
		m_node[nodeOrder].toNextLength = 0;
		if(m_nodeSum > 1)
		{//填写上一个点的相关引导信息
			m_node[nodeOrder - 1].toNextAngle = m_node[nodeOrder - 1].myPoint.getAngle(m_node[nodeOrder].myPoint);
			m_node[nodeOrder - 1].toNextCos = cos(m_node[nodeOrder - 1].toNextAngle);
			m_node[nodeOrder - 1].toNextSin = sin(m_node[nodeOrder - 1].toNextAngle);
			m_node[nodeOrder - 1].toNextLength = m_node[nodeOrder - 1].myPoint.getLength(m_node[nodeOrder].myPoint);

			//填写当前点的相关信息
			m_node[nodeOrder].toPreviousAngle = m_node[nodeOrder - 1].toNextAngle + PI;
			if(m_node[nodeOrder].toPreviousAngle >= PI2) m_node[nodeOrder].toPreviousAngle -= PI2;
			m_node[nodeOrder].toPreviousCos = cos(m_node[nodeOrder].toPreviousAngle);
			m_node[nodeOrder].toPreviousSin = sin(m_node[nodeOrder].toPreviousAngle);
			m_node[nodeOrder].toPreviousLength = m_node[nodeOrder - 1].toNextLength;
			m_lineLength += m_node[nodeOrder].toPreviousLength;
			//最后一个点的方向与前一个点的方向保持一致
			if(m_isLoop == 0)
			{
				m_node[nodeOrder].toNextAngle = m_node[nodeOrder - 1].toNextAngle;
				m_node[nodeOrder].toNextCos = m_node[nodeOrder - 1].toNextCos;
				m_node[nodeOrder].toNextSin = m_node[nodeOrder - 1].toNextSin;
				if(m_nodeSum == 2)
				{//头的点保持后一个点的方向
					m_node[0].toPreviousAngle = m_node[1].toPreviousAngle;
					m_node[0].toPreviousCos = m_node[1].toPreviousCos;
					m_node[0].toPreviousSin = m_node[1].toPreviousSin;
				}
			}
		}
		if(m_isLoop != 0) setLoop();	//设置曲线闭合
	}else
	{//中间
		m_lineLength -= m_node[nodeOrder].toPreviousLength;
		m_lineLength -= m_node[nodeOrder].toNextLength;

		m_node[nodeOrder].myPoint.set(x,y);
		m_node[nodeOrder].toNextAngle = m_node[nodeOrder].myPoint.getAngle(m_node[nodeOrder + 1].myPoint);
		m_node[nodeOrder].toNextCos = cos(m_node[nodeOrder].toNextAngle);
		m_node[nodeOrder].toNextSin = sin(m_node[nodeOrder].toNextAngle);
		m_node[nodeOrder].toNextLength = m_node[nodeOrder].myPoint.getLength(m_node[nodeOrder + 1].myPoint);

		m_lineLength += m_node[nodeOrder].toNextLength;

		m_node[nodeOrder + 1].toPreviousAngle = m_node[nodeOrder].toNextAngle + PI;
		if(m_node[nodeOrder + 1].toPreviousAngle >= PI2) m_node[nodeOrder + 1].toPreviousAngle -= PI2;
		m_node[nodeOrder + 1].toPreviousCos = cos(m_node[nodeOrder + 1].toPreviousAngle);
		m_node[nodeOrder + 1].toPreviousSin = sin(m_node[nodeOrder + 1].toPreviousAngle);
		m_node[nodeOrder + 1].toPreviousLength = m_node[nodeOrder].toNextLength;

		m_node[nodeOrder].toPreviousAngle = m_node[nodeOrder].myPoint.getAngle(m_node[nodeOrder - 1].myPoint);
		m_node[nodeOrder].toPreviousCos = cos(m_node[nodeOrder].toPreviousAngle);
		m_node[nodeOrder].toPreviousSin = sin(m_node[nodeOrder].toPreviousAngle);
		m_node[nodeOrder].toPreviousLength = m_node[nodeOrder].myPoint.getLength(m_node[nodeOrder - 1].myPoint);
		m_lineLength += m_node[nodeOrder].toPreviousLength;

		m_node[nodeOrder - 1].toNextAngle = m_node[nodeOrder].toPreviousAngle + PI;
		if(m_node[nodeOrder - 1].toNextAngle >= PI2) m_node[nodeOrder - 1].toNextAngle -= PI2;
		m_node[nodeOrder - 1].toNextCos = cos(m_node[nodeOrder - 1].toNextAngle);
		m_node[nodeOrder - 1].toNextSin = sin(m_node[nodeOrder - 1].toNextAngle);
		m_node[nodeOrder - 1].toNextLength = m_node[nodeOrder - 1].myPoint.getLength(m_node[nodeOrder].myPoint);
		if(m_isLoop == 0)
		{
			if(nodeOrder == 1)
			{//头的点保持后一个点的方向
				m_node[0].toPreviousAngle = m_node[1].toPreviousAngle;
				m_node[0].toPreviousCos = m_node[1].toPreviousCos;
				m_node[0].toPreviousSin = m_node[1].toPreviousSin;
			}
			if(nodeOrder == m_nodeSum - 2 && m_isLoop == 0)
			{//最后一个点要保持前一个点的方向
				m_node[nodeOrder + 1].toNextAngle = m_node[nodeOrder].toNextAngle;
				m_node[nodeOrder + 1].toNextCos = m_node[nodeOrder].toNextCos;
				m_node[nodeOrder + 1].toNextSin = m_node[nodeOrder].toNextSin;
			}
		}
	}
	m_needUpdateData = XTrue;	//新插入点，需要重新计算差值点
}

void _XNodeLine::decreaseOneNode()
{
	if(m_isACopy != 0) return;//副本不能进行这个操作
	if(m_nodeSum <= 0 || m_node == NULL) return;
	m_lineLength -= m_node[m_nodeSum - 1].toPreviousLength;
	m_node[m_nodeSum - 1].myPoint.set(0,0);
//	m_node[m_nodeSum - 1].toNextAngle = 0;
//	m_node[m_nodeSum - 1].toNextCos = 0;
//	m_node[m_nodeSum - 1].toNextSin = 0;
	m_node[m_nodeSum - 1].toNextLength = 0;
	m_node[m_nodeSum - 1].toPreviousAngle = 0;
	m_node[m_nodeSum - 1].toPreviousCos = 0;
	m_node[m_nodeSum - 1].toPreviousSin = 0;
	m_node[m_nodeSum - 1].toPreviousLength = 0;
	-- m_nodeSum;
	if(m_isLoop != 0) setLoop();	//设置曲线闭合
	m_needUpdateData = XTrue;
}

void _XNodeLine::addOneNode(float x,float y,int nodeOrder)		//向曲线中间添加一个点
{
	if(m_isACopy != 0) return;//副本不能进行这个操作
	if(m_nodeSum >= MAX_NODE_SUM || m_node == NULL) return;	//如果记录的电已经满了则直接返回
	if(nodeOrder == 0)
	{//头
		for(int i = m_nodeSum;i > 0;-- i)
		{
			m_node[i] = m_node[i - 1];
		}
		m_node[0].myPoint.set(x,y);
		m_node[0].toNextAngle = m_node[0].myPoint.getAngle(m_node[1].myPoint);
		m_node[0].toNextCos = cos(m_node[0].toNextAngle);
		m_node[0].toNextSin = sin(m_node[0].toNextAngle);
		m_node[0].toNextLength = m_node[0].myPoint.getLength(m_node[1].myPoint);
		++ m_nodeSum;
		//m_lineLength += m_node[1].toPreviousLength;	//这行应该是有逻辑错误的
		m_lineLength += m_node[0].toNextLength;
		m_node[1].toPreviousAngle = m_node[0].toNextAngle + PI;
		if(m_node[1].toPreviousAngle >= PI2) m_node[1].toPreviousAngle -= PI2;
		m_node[1].toPreviousCos = cos(m_node[1].toPreviousAngle);
		m_node[1].toPreviousSin = sin(m_node[1].toPreviousAngle);
		m_node[1].toPreviousLength = m_node[0].toNextLength;
		if(m_isLoop == 0)
		{
			if(m_nodeSum > 1)
			{//头这个点保持前一个点的方向
				m_node[0].toPreviousAngle = m_node[1].toPreviousAngle;
				m_node[0].toPreviousCos = m_node[1].toPreviousCos;
				m_node[0].toPreviousSin = m_node[1].toPreviousSin;
			}
			if(m_nodeSum == 2)
			{
				m_node[1].toNextAngle = m_node[0].toNextAngle;
				m_node[1].toNextCos = m_node[0].toNextCos;
				m_node[1].toNextSin = m_node[0].toNextSin;
			}
		}else
		{
			setLoop();
		}
	}else
	if(nodeOrder == m_nodeSum)
	{//尾
		addOneNode(x,y);
	}else
	{//中间
		for(int i = m_nodeSum;i > nodeOrder;-- i)
		{
			m_node[i] = m_node[i - 1];
		}
		m_lineLength -= m_node[nodeOrder].toPreviousLength;
		m_node[nodeOrder].myPoint.set(x,y);
		m_node[nodeOrder].toNextAngle = m_node[nodeOrder].myPoint.getAngle(m_node[nodeOrder + 1].myPoint);
		m_node[nodeOrder].toNextCos = cos(m_node[nodeOrder].toNextAngle);
		m_node[nodeOrder].toNextSin = sin(m_node[nodeOrder].toNextAngle);
		m_node[nodeOrder].toNextLength = m_node[nodeOrder].myPoint.getLength(m_node[nodeOrder + 1].myPoint);
		m_lineLength += m_node[nodeOrder].toNextLength;

		m_node[nodeOrder + 1].toPreviousAngle = m_node[nodeOrder].toNextAngle + PI;
		if(m_node[nodeOrder + 1].toPreviousAngle >= PI2) m_node[nodeOrder + 1].toPreviousAngle -= PI2;
		m_node[nodeOrder + 1].toPreviousCos = cos(m_node[nodeOrder + 1].toPreviousAngle);
		m_node[nodeOrder + 1].toPreviousSin = sin(m_node[nodeOrder + 1].toPreviousAngle);
		m_node[nodeOrder + 1].toPreviousLength = m_node[nodeOrder].toNextLength;

		m_node[nodeOrder].toPreviousAngle = m_node[nodeOrder].myPoint.getAngle(m_node[nodeOrder - 1].myPoint);
		m_node[nodeOrder].toPreviousCos = cos(m_node[nodeOrder].toPreviousAngle);
		m_node[nodeOrder].toPreviousSin = sin(m_node[nodeOrder].toPreviousAngle);
		m_node[nodeOrder].toPreviousLength = m_node[nodeOrder].myPoint.getLength(m_node[nodeOrder - 1].myPoint);
		m_lineLength += m_node[nodeOrder].toPreviousLength;

		m_node[nodeOrder - 1].toNextAngle = m_node[nodeOrder].toPreviousAngle + PI;
		if(m_node[nodeOrder - 1].toNextAngle >= PI2) m_node[nodeOrder - 1].toNextAngle -= PI2;
		m_node[nodeOrder - 1].toNextCos = cos(m_node[nodeOrder - 1].toNextAngle);
		m_node[nodeOrder - 1].toNextSin = sin(m_node[nodeOrder - 1].toNextAngle);
		m_node[nodeOrder - 1].toNextLength = m_node[nodeOrder - 1].myPoint.getLength(m_node[nodeOrder].myPoint);
		++ m_nodeSum;
		if(m_isLoop == 0)
		{
			if(nodeOrder == 1)
			{//头这个点保持前一个点的方向
				m_node[0].toPreviousAngle = m_node[1].toPreviousAngle;
				m_node[0].toPreviousCos = m_node[1].toPreviousCos;
				m_node[0].toPreviousSin = m_node[1].toPreviousSin;
			}
			if(nodeOrder == m_nodeSum - 2)
			{//最后一个点与前一个点保持一致
				m_node[m_nodeSum - 1].toNextAngle = m_node[nodeOrder].toNextAngle;
				m_node[m_nodeSum - 1].toNextCos = m_node[nodeOrder].toNextCos;
				m_node[m_nodeSum - 1].toNextSin = m_node[nodeOrder].toNextSin;
			}
		}else
		{
			setLoop();	//设置曲线闭合
		}
	}
	m_needUpdateData = XTrue;
}

void _XNodeLine::decreaseOneNode(int nodeOrder)
{
	//检查数据的合理性
	if(m_isACopy != 0) return;//副本不能进行这个操作
	if(nodeOrder <= 0 || nodeOrder >= m_nodeSum) return;
	if(nodeOrder == 0)
	{//头
		for(int i = 0;i < m_nodeSum - 1;++ i)
		{
			m_node[i] = m_node[i + 1];
		}
		m_lineLength -= m_node[0].toPreviousLength;
	//	m_node[0].toPreviousAngle = 0;
	//	m_node[0].toPreviousCos = 0;
	//	m_node[0].toPreviousSin = 0;
		m_node[0].toPreviousLength = 0;
		//这个一行是为了中和decreaseOneNode函数中的减操作而增加的
		m_lineLength += m_node[m_nodeSum - 1].toPreviousLength;
		decreaseOneNode();
	}else
	if(nodeOrder == m_nodeSum - 1)
	{//尾
		decreaseOneNode();
	}else
	{//中间
		m_lineLength -= m_node[nodeOrder].toPreviousLength;
		m_lineLength -= m_node[nodeOrder + 1].toPreviousLength;
		for(int i = nodeOrder;i < m_nodeSum - 1;++ i)
		{
			m_node[i] = m_node[i + 1];
		}
		m_node[nodeOrder - 1].toNextAngle = m_node[nodeOrder - 1].myPoint.getAngle(m_node[nodeOrder].myPoint);
		m_node[nodeOrder - 1].toNextCos = cos(m_node[nodeOrder - 1].toNextAngle);
		m_node[nodeOrder - 1].toNextSin = sin(m_node[nodeOrder - 1].toNextAngle);
		m_node[nodeOrder - 1].toNextLength = m_node[nodeOrder - 1].myPoint.getLength(m_node[nodeOrder].myPoint);
		m_lineLength += m_node[nodeOrder - 1].toNextLength;
		m_node[nodeOrder].toPreviousAngle = m_node[nodeOrder - 1].toNextAngle + PI;
		if(m_node[nodeOrder].toPreviousAngle >= PI2) m_node[nodeOrder].toPreviousAngle -= PI2;
		m_node[nodeOrder].toPreviousCos = cos(m_node[nodeOrder].toPreviousAngle);
		m_node[nodeOrder].toPreviousSin = sin(m_node[nodeOrder].toPreviousAngle);
		m_node[nodeOrder].toPreviousLength = m_node[nodeOrder - 1].toNextLength;
		//这个一行是为了中和decreaseOneNode函数中的减操作而增加的
		m_lineLength += m_node[m_nodeSum - 1].toPreviousLength;
		decreaseOneNode();
	}
	m_needUpdateData = XTrue;
}

void _XNodeLine::clearUp()
{//只需要清除第一个点的信息即可
	if(m_isACopy != 0) return;//副本不能进行这个操作
	m_node[0].myPoint.set(0,0);
	m_node[0].toNextAngle = 0;
	m_node[0].toNextCos = 0;
	m_node[0].toNextSin = 0;
	m_node[0].toNextLength = 0;
	m_node[0].toPreviousAngle = 0;
	m_node[0].toPreviousCos = 0;
	m_node[0].toPreviousSin = 0;
	m_node[0].toPreviousLength = 0;
	m_nodeSum = 0;
	m_lineLength = 0;
	m_isLoop = 0;
}

_XBool _XNodeLine::saveNodeLine()
{
	if(m_isACopy) return XFalse;//副本不能进行这个操作
	if(m_nodeSum <= 1) return XFalse;
	FILE *fp;
	char fileName[] = NODELINE_DATA_FILE_NAME;
	if(m_myOrder >= 0) fileName[13] = m_myOrder % 10 + '0';
	if(m_myOrder >= 10) fileName[12] = (m_myOrder/10) % 10 + '0';
	if(m_myOrder >= 100) fileName[11] = (m_myOrder/100) % 10 + '0';
	if(m_myOrder >= 1000) fileName[10] = (m_myOrder/1000) % 10 + '0';
	if(m_myOrder >= 10000) fileName[9] = (m_myOrder/10000) % 10 + '0';
	//打开文件
	if((fp = fopen(fileName,"wb")) == NULL)
	{
		printf("Line file open error!\n");
		return 0;
	}
	//写入数据
	fwrite(&m_nodeSum,sizeof(int),1,fp);			//节点数量
	fwrite(&m_lineLength,sizeof(float),1,fp);		//引导线长度
	//依次写入节点信息
	for(int i = 0;i < m_nodeSum;++ i)
	{
		fwrite(&m_node[i],sizeof(_XNode),1,fp);	
	}
	//写入完成之后关闭文件
	fclose(fp);
	return XTrue;
}
_XBool _XNodeLine::getNodeLine(_XResourcePosition resoursePosition)
{
	if(m_isACopy) return XFalse;//副本不能进行这个操作
	char fileName[MAX_FILE_NAME_LENGTH] = "";
	sprintf(fileName,"%s%s",BASE_RESOURCE_PATH,NODELINE_DATA_FILE_NAME);
	int len = strlen(fileName);
	if(m_myOrder >= 0) fileName[len - 5] = m_myOrder % 10 + '0';
	if(m_myOrder >= 10) fileName[len - 6] = (m_myOrder/10) % 10 + '0';
	if(m_myOrder >= 100) fileName[len - 7] = (m_myOrder/100) % 10 + '0';
	if(m_myOrder >= 1000) fileName[len - 8] = (m_myOrder/1000) % 10 + '0';
	if(m_myOrder >= 10000) fileName[len - 9] = (m_myOrder/10000) % 10 + '0';
	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = XEE::defaultResourcePosition;
	if(resoursePosition == RESOURCE_LOCAL_FOLDER)
	{
		FILE *fp = NULL;
		if((fp = fopen(fileName,"rb")) == NULL)
		{//打开文件
			printf("Line file open error!\n");
			return XFalse;
		}
		//读取数据
		fread(&m_nodeSum,sizeof(int),1,fp);		//节点数量
		fread(&m_lineLength,sizeof(float),1,fp);		//引导线长度
		//依次读取节点信息
		for(int i = 0;i < m_nodeSum;++ i)
		{
			fread(&m_node[i],sizeof(_XNode),1,fp);	
		}
		//读取完成之后关闭文件
		fclose(fp);
	}else
	if(resoursePosition == RESOURCE_LOCAL_PACK)
	{
		//从资源文件中提取资源
		unsigned char *p = _XResourcePack::GetInstance().getFileData(fileName);
		if(p == NULL)
		{
			printf("Load NodeLine Data Error!\n");
			return XFalse;
		}
		//将提取的数据释放到指定的位置
		int offset = 0;
		memcpy(&m_nodeSum,p + offset,sizeof(int));
		offset += sizeof(int);
		memcpy(&m_lineLength,p + offset,sizeof(float));
		offset += sizeof(float);
		//依次写入节点信息
		for(int i = 0;i < m_nodeSum;++ i)
		{
			memcpy(&m_node[i],p + offset,sizeof(_XNode));
			offset += sizeof(_XNode);
		}
		XDELETE_ARRAY(p);
	}
	m_needUpdateData = XTrue;
	return XTrue;
}
void _XNodeLine::updateData()
{
	if(!m_needUpdateData) return;
	m_needUpdateData = XFalse;	//下面更新数据
	if(m_nodeSum < 2) return;
	//下面开始计算差值关键点(每个点有两个引导点)
	_XVector2 tempPoint0;
	_XVector2 tempPoint1;
	_XVector2 tempPoint2;
	//第一个点要分开计算
	tempPoint0 = (m_node[0].myPoint + m_node[m_nodeSum - 1].myPoint) * 0.5f;
	tempPoint1 = (m_node[0].myPoint + m_node[1].myPoint) * 0.5f;
	tempPoint2 = (tempPoint0 + tempPoint1) * 0.5f;
	m_bezierPoint[0] = tempPoint0 + m_node[0].myPoint - tempPoint2;
	m_bezierPoint[1] = tempPoint1 + m_node[0].myPoint - tempPoint2;
	//最后一个点要分开计算
	//tempPoint0 = (m_node[0].myPoint + m_node[m_nodeSum - 1].myPoint) * 0.5f;
	tempPoint1 = (m_node[m_nodeSum - 1].myPoint + m_node[m_nodeSum - 2].myPoint) * 0.5f;
	tempPoint2 = (tempPoint0 + tempPoint1) * 0.5f;
	m_bezierPoint[(m_nodeSum - 1) << 1] = tempPoint1 + m_node[m_nodeSum - 1].myPoint - tempPoint2;
	m_bezierPoint[((m_nodeSum - 1) << 1) + 1] = tempPoint0 + m_node[m_nodeSum - 1].myPoint - tempPoint2;
	for(int i = 1;i < m_nodeSum - 1;++ i)
	{
		tempPoint0 = (m_node[i].myPoint + m_node[i - 1].myPoint) * 0.5f;
		tempPoint1 = (m_node[i].myPoint + m_node[i + 1].myPoint) * 0.5f;
		tempPoint2 = (tempPoint0 + tempPoint1) * 0.5f;
		m_bezierPoint[i << 1] = tempPoint0 + m_node[i].myPoint - tempPoint2;
		m_bezierPoint[(i << 1) + 1] = tempPoint1 + m_node[i].myPoint - tempPoint2;
	}
}
//注意最少需要有2个点
void _XNodeLine::moveSpecialPoint(int timeDelay,int isLoop,_XBool needBezier)
{
	if(m_nodeSum <= 1) return;
	updateData();	//如果有必要更新差值数据
	if(isLoop != 0 && !m_isLoop)
	{
		setLoop();
	}
	if(m_specialPoint.isEnable && !m_specialPoint.isEnd)
	{
		m_specialPoint.nowLength += timeDelay * m_specialPoint.speed;
		if((m_leadMode & LEAD_MODE_FOREWORD) != 0)
		{//顺序
			while(1)
			{
				if(m_specialPoint.nowLength > m_specialPoint.upNodeLength + m_node[m_specialPoint.upNodeOrder].toNextLength)
				{//已经越过下面一个点
					if(isLoop == 0)
					{//不循环
						if(m_specialPoint.upNodeOrder + 1 == m_nodeSum - 1)
						{//已经到达终点
							m_specialPoint.isEnd = 1;
							m_specialPoint.angle = m_node[m_nodeSum - 2].toNextAngle;
							m_specialPoint.nowLength = m_lineLength;
							m_specialPoint.position = m_node[m_nodeSum - 1].myPoint;
							m_specialPoint.upNodeOrder = m_nodeSum - 2;
							m_specialPoint.upNodeLength = m_lineLength - m_node[m_nodeSum - 1].toPreviousLength;
							break;
						}else
						{
							++ m_specialPoint.upNodeOrder;
							m_specialPoint.upNodeLength += m_node[m_specialPoint.upNodeOrder].toPreviousLength;
							if(needBezier)
							{//这里是需要增加的部分
								m_bezierSpline.init(m_node[m_specialPoint.upNodeOrder].myPoint,
									m_bezierPoint[(m_specialPoint.upNodeOrder << 1) + 1],
									m_bezierPoint[(m_specialPoint.upNodeOrder + 1) << 1],
									m_node[m_specialPoint.upNodeOrder + 1].myPoint);
							}
						}
					}else
					{//循环
						if(m_specialPoint.upNodeOrder == m_nodeSum - 1)
						{//重新到达起点
							m_specialPoint.isEnd = 1;
							m_specialPoint.angle = m_node[0].toNextAngle;
							m_specialPoint.nowLength = m_lineLength + m_node[m_nodeSum - 1].toNextLength;
							m_specialPoint.position = m_node[0].myPoint;
							m_specialPoint.upNodeOrder = m_nodeSum - 1;
							m_specialPoint.upNodeLength = m_lineLength;
							break;
						}else
						{
							++ m_specialPoint.upNodeOrder;
							m_specialPoint.upNodeLength += m_node[m_specialPoint.upNodeOrder].toPreviousLength;
							if(needBezier)
							{//这里是需要增加的部分
								if(m_specialPoint.upNodeOrder == m_nodeSum - 1)
								{//最后一个点
									m_bezierSpline.init(m_node[m_specialPoint.upNodeOrder].myPoint,
										m_bezierPoint[(m_specialPoint.upNodeOrder << 1) + 1],
										m_bezierPoint[0],
										m_node[0].myPoint);
								}else
								{
									m_bezierSpline.init(m_node[m_specialPoint.upNodeOrder].myPoint,
										m_bezierPoint[(m_specialPoint.upNodeOrder << 1) + 1],
										m_bezierPoint[(m_specialPoint.upNodeOrder + 1) << 1],
										m_node[m_specialPoint.upNodeOrder + 1].myPoint);
								}
							}
						}
					}
				}else
				{//没有越过下面一个点
					if(m_specialPoint.upNodeOrder == m_nodeSum - 1)
					{//经过最后一个点，重新向起点
						if(needBezier)
						{//这里是需要增加的部分
							float tempPos = (m_specialPoint.nowLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toNextLength;
							m_specialPoint.position = m_bezierSpline.getBezierSplineValue(tempPos);
							m_specialPoint.angle = m_bezierSpline.getBezierSplineAngle(tempPos);
						}else
						{
							m_specialPoint.position.x = m_node[m_specialPoint.upNodeOrder].myPoint.x 
								+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength) * m_node[m_specialPoint.upNodeOrder].toNextCos;
							m_specialPoint.position.y = m_node[m_specialPoint.upNodeOrder].myPoint.y 
								+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength) * m_node[m_specialPoint.upNodeOrder].toNextSin;
							if(m_node[0].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle <= PI
								&& m_node[0].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle >= -PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toNextAngle 
									+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toNextLength 
									* (m_node[0].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle);
							}else
							if(m_node[0].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle > PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toNextAngle 
									+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toNextLength 
									* (PI2 - (m_node[0].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle));
							}else
							if(m_node[0].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle < -PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toNextAngle 
									+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toNextLength 
									* (PI2 + (m_node[0].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle));
							}
						}
					}else
					{//正常的点
						if(needBezier)
						{//这里是需要增加的部分
							float tempPos = (m_specialPoint.nowLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toNextLength;
							m_specialPoint.position = m_bezierSpline.getBezierSplineValue(tempPos);
							m_specialPoint.angle = m_bezierSpline.getBezierSplineAngle(tempPos);
						}else
						{
							m_specialPoint.position.x = m_node[m_specialPoint.upNodeOrder].myPoint.x 
								+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength) * m_node[m_specialPoint.upNodeOrder].toNextCos;
							m_specialPoint.position.y = m_node[m_specialPoint.upNodeOrder].myPoint.y 
								+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength) * m_node[m_specialPoint.upNodeOrder].toNextSin;
							if(m_node[m_specialPoint.upNodeOrder + 1].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle <= PI
								&& m_node[m_specialPoint.upNodeOrder + 1].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle >= -PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toNextAngle 
									+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toNextLength 
									* (m_node[m_specialPoint.upNodeOrder + 1].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle);
							}else
							if(m_node[m_specialPoint.upNodeOrder + 1].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle > PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toNextAngle 
									+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toNextLength 
									* (m_node[m_specialPoint.upNodeOrder + 1].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle - PI2);
							}else
							if(m_node[m_specialPoint.upNodeOrder + 1].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle < -PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toNextAngle 
									+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toNextLength 
									* (PI2 + (m_node[m_specialPoint.upNodeOrder + 1].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle));
							}	
						}
					}
					break;
				}
			}
		}else
		{//逆行
			while(1)
			{
				if(m_specialPoint.nowLength > m_specialPoint.upNodeLength + m_node[m_specialPoint.upNodeOrder].toPreviousLength)
				{//以及越过下面一个点
					if(isLoop == 0)
					{//不循环
						if(m_specialPoint.upNodeOrder == 1)
						{//已经到达终点
							m_specialPoint.isEnd = 1;
							m_specialPoint.angle = m_node[1].toPreviousAngle;
							m_specialPoint.nowLength = m_lineLength;
							m_specialPoint.position = m_node[0].myPoint;
							m_specialPoint.upNodeOrder = 1;
							m_specialPoint.upNodeLength = m_lineLength - m_node[0].toNextLength;
							break;
						}else
						{
							-- m_specialPoint.upNodeOrder;
							m_specialPoint.upNodeLength += m_node[m_specialPoint.upNodeOrder].toNextLength;
							if(needBezier)
							{//这里是需要增加的部分
								m_bezierSpline.init(m_node[m_specialPoint.upNodeOrder].myPoint,
									m_bezierPoint[(m_specialPoint.upNodeOrder << 1)],
									m_bezierPoint[((m_specialPoint.upNodeOrder - 1) << 1) + 1],
									m_node[m_specialPoint.upNodeOrder - 1].myPoint);
							}
						}
					}else
					{
						if(m_specialPoint.upNodeOrder == 0)
						{//已经到达终点
							m_specialPoint.isEnd = 1;
							m_specialPoint.angle = m_node[m_nodeSum - 1].toPreviousAngle;
							m_specialPoint.nowLength = m_lineLength + m_node[0].toPreviousLength;
							m_specialPoint.position = m_node[m_nodeSum - 1].myPoint;
							m_specialPoint.upNodeOrder = 0;
							m_specialPoint.upNodeLength = m_lineLength;
							break;
						}else
						{
							-- m_specialPoint.upNodeOrder;
							m_specialPoint.upNodeLength += m_node[m_specialPoint.upNodeOrder].toNextLength;
							if(needBezier)
							{//这里是需要增加的部分
								if(m_specialPoint.upNodeOrder == 0)
								{
									m_bezierSpline.init(m_node[m_specialPoint.upNodeOrder].myPoint,
										m_bezierPoint[(m_specialPoint.upNodeOrder << 1)],
										m_bezierPoint[((m_nodeSum - 1) << 1) + 1],
										m_node[m_nodeSum - 1].myPoint);
								}else
								{
									m_bezierSpline.init(m_node[m_specialPoint.upNodeOrder].myPoint,
										m_bezierPoint[(m_specialPoint.upNodeOrder << 1)],
										m_bezierPoint[((m_specialPoint.upNodeOrder - 1) << 1) + 1],
										m_node[m_specialPoint.upNodeOrder - 1].myPoint);
								}
							}
						}
					}
				}else
				{//没有越过下面一个点
					if(m_specialPoint.upNodeOrder == 0)
					{//走向逆行的起点
						if(needBezier)
						{//这里是需要增加的部分
							float tempPos = (m_specialPoint.nowLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toPreviousLength;
							m_specialPoint.position = m_bezierSpline.getBezierSplineValue(tempPos);
							m_specialPoint.angle = m_bezierSpline.getBezierSplineAngle(tempPos);
						}else
						{
							m_specialPoint.position.x = m_node[m_specialPoint.upNodeOrder].myPoint.x 
								+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength) * m_node[m_specialPoint.upNodeOrder].toPreviousCos;
							m_specialPoint.position.y = m_node[m_specialPoint.upNodeOrder].myPoint.y 
								+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength) * m_node[m_specialPoint.upNodeOrder].toPreviousSin;
							if(m_node[m_nodeSum - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle <= PI
								&& m_node[m_nodeSum - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle >= -PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toPreviousAngle 
									+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toPreviousLength 
									* (m_node[m_nodeSum - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle);
							}else
							if(m_node[m_nodeSum - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle > PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toPreviousAngle 
									+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toPreviousLength 
									* ((m_node[m_nodeSum - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle) - PI2);
							}else
							if(m_node[m_nodeSum - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle < -PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toPreviousAngle 
									+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toPreviousLength 
									* (PI2 + (m_node[m_nodeSum - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle));
							}
						}
					}else
					{//普通的点
						if(needBezier)
						{//这里是需要增加的部分
							float tempPos = (m_specialPoint.nowLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toPreviousLength;
							m_specialPoint.position = m_bezierSpline.getBezierSplineValue(tempPos);
							m_specialPoint.angle = m_bezierSpline.getBezierSplineAngle(tempPos);
						}else
						{
							m_specialPoint.position.x = m_node[m_specialPoint.upNodeOrder].myPoint.x 
								+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength) * m_node[m_specialPoint.upNodeOrder].toPreviousCos;
							m_specialPoint.position.y = m_node[m_specialPoint.upNodeOrder].myPoint.y 
								+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength) * m_node[m_specialPoint.upNodeOrder].toPreviousSin;
							if(m_node[m_specialPoint.upNodeOrder - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle <= PI
								&& m_node[m_specialPoint.upNodeOrder - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle >=  -PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toPreviousAngle 
									+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toPreviousLength 
									* (m_node[m_specialPoint.upNodeOrder - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle);
							}else
							if(m_node[m_specialPoint.upNodeOrder - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle > PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toPreviousAngle 
									+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toPreviousLength 
									* ((m_node[m_specialPoint.upNodeOrder - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle) - PI2);
							}else
							if(m_node[m_specialPoint.upNodeOrder - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle < -PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toPreviousAngle 
									+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toPreviousLength 
									* (PI2 + (m_node[m_specialPoint.upNodeOrder - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle));
							}
						}
					}
					break;
				}
			}
		}
	}
}

void _XNodeLine::resetSpecialPoint(_XBool needBezier)
{
	if(m_nodeSum <= 1) return;
	updateData();	//如果有必要更新差值数据
	if((m_leadMode & LEAD_MODE_FOREWORD) != 0)
	{//顺序
		m_specialPoint.isEnable = XTrue;
		m_specialPoint.isEnd = XFalse;
		m_specialPoint.nowLength = 0.0f;
		m_specialPoint.position.set(m_node[0].myPoint.x,m_node[0].myPoint.y);
		m_specialPoint.angle = m_node[0].toNextAngle;
		m_specialPoint.upNodeLength = 0.0f;
		m_specialPoint.upNodeOrder = 0;
		if(needBezier)
		{
			m_bezierSpline.init(m_node[0].myPoint,m_bezierPoint[1],	m_bezierPoint[2],m_node[1].myPoint);
		}
	}else
	{//逆行
		m_specialPoint.isEnable = XTrue;
		m_specialPoint.isEnd = XFalse;
		m_specialPoint.nowLength = 0.0f;
		m_specialPoint.position.set(m_node[m_nodeSum - 1].myPoint.x,m_node[m_nodeSum - 1].myPoint.y);
		m_specialPoint.angle = m_node[m_nodeSum - 1].toPreviousAngle;
		m_specialPoint.upNodeLength = 0.0f;
		m_specialPoint.upNodeOrder = m_nodeSum - 1;
		if(needBezier)
		{
			m_bezierSpline.init(m_node[m_nodeSum - 1].myPoint,m_bezierPoint[(m_nodeSum - 1) << 1],m_bezierPoint[((m_nodeSum - 2) << 1) + 1],m_node[m_nodeSum - 2].myPoint);
		}
	}
}
void _XNodeLine::setLoop()
{
	if(m_isACopy) return;//副本不能进行这个操作
	if(m_nodeSum < 2) return;	//小余两个点是不能循环的
	//将第一个点和最后一个点连接起来
	m_node[m_nodeSum - 1].toNextAngle = m_node[m_nodeSum - 1].myPoint.getAngle(m_node[0].myPoint);
	m_node[m_nodeSum - 1].toNextCos = cos(m_node[m_nodeSum - 1].toNextAngle);
	m_node[m_nodeSum - 1].toNextSin = sin(m_node[m_nodeSum - 1].toNextAngle);
	m_node[m_nodeSum - 1].toNextLength = m_node[m_nodeSum - 1].myPoint.getLength(m_node[0].myPoint);
	if(m_isLoop)
	{//如果引导线原来就是闭合的，这里需要减去原来的长度
		m_lineLength -= m_node[0].toPreviousLength;
	}
	m_node[0].toPreviousAngle = m_node[m_nodeSum - 1].toNextAngle + PI;
	if(m_node[0].toPreviousAngle >= PI2) m_node[0].toPreviousAngle -= PI2;
	m_node[0].toPreviousCos = cos(m_node[0].toPreviousAngle);
	m_node[0].toPreviousSin = sin(m_node[0].toPreviousAngle);
	m_node[0].toPreviousLength = m_node[m_nodeSum - 1].toNextLength;

	m_lineLength += m_node[0].toPreviousLength; //加上新的长度
	m_isLoop = XTrue;
}
_XBool _XNodeLine::setACopy(const _XNodeLine &temp)
{
	if(this == &temp) return XFalse;		//防止自生拷贝
	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	//释放自身的资源
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{//自身没有引用需要释放
		XDELETE_ARRAY(m_node);
		XDELETE_ARRAY(m_bezierPoint);
		XDELETE(m_cp);
	}
	m_cp = temp.m_cp;

//	if(m_isACopy == 0)
//	{//如果本身不是副本则需要释放资源
//		XDELETE_ARRAY(m_node);
//		m_isACopy = 1;
//	}
	//拷贝属性
	m_isVisiable = temp.m_isVisiable;					//本身引导线的编号
	m_myOrder = temp.m_myOrder;					//本身引导线的编号
	m_node = temp.m_node;						//引导线中的节点
	m_bezierPoint = temp.m_bezierPoint;	
	m_leadMode = temp.m_leadMode;				//曲线的引导模式
	m_isLoop = temp.m_isLoop;					//曲线是否闭合0不闭合 1闭合，注意闭合的点，首点和尾点不能重合，否则将会失去尾点的方向

	m_offsetPosition = temp.m_offsetPosition;	//整体曲线的偏移位置
	m_size = temp.m_size;						//这个曲线的所放笔需要按照比例进行，不能够使用水平和垂直的不同比例，否则将会造成曲线中角度的失真严重
//	m_isACopy = temp.m_isACopy;					//是否是一个副本

	m_lineLength = temp.m_lineLength;			//曲线的总长度
	m_nodeSum = temp.m_nodeSum;					//引导曲线中的引导节点的数量
	m_specialPoint = temp.m_specialPoint;		//曲线上的移动的点
	return XTrue;
}
_XNodeLine::_XNodeLine(const _XNodeLine &temp)
{
	//m_node = createArrayMem<_XNode>(MAX_NODE_SUM);
	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	m_cp = temp.m_cp;

	//拷贝属性
	m_isVisiable = temp.m_isVisiable;					//本身引导线的编号
	m_myOrder = temp.m_myOrder;					//本身引导线的编号
	m_leadMode = temp.m_leadMode;				//曲线的引导模式
	m_isLoop = temp.m_isLoop;					//曲线是否闭合0不闭合 1闭合，注意闭合的点，首点和尾点不能重合，否则将会失去尾点的方向

	m_offsetPosition = temp.m_offsetPosition;	//整体曲线的偏移位置
	m_size = temp.m_size;						//这个曲线的所放笔需要按照比例进行，不能够使用水平和垂直的不同比例，否则将会造成曲线中角度的失真严重

	m_lineLength = temp.m_lineLength;			//曲线的总长度
	m_nodeSum = temp.m_nodeSum;					//引导曲线中的引导节点的数量
	m_specialPoint = temp.m_specialPoint;		//曲线上的移动的点
	m_node = temp.m_node;						//引导线中的节点
	m_bezierPoint = temp.m_bezierPoint;						//引导线中的节点
	m_needUpdateData = temp.m_needUpdateData;						//引导线中的节点
	m_bezierSpline = temp.m_bezierSpline;						//引导线中的节点
//	for(int i = 0;i < m_nodeSum;++ i)
//	{
//		m_node[i] = temp.m_node[i];
//	}
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_NODELINE);
#endif

	m_isACopy = XFalse;
}
_XNodeLine& _XNodeLine::operator = (const _XNodeLine& temp)
{
	if(&temp == this) return *this;
	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	//释放自身的资源
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{//自身没有引用需要释放
		XDELETE_ARRAY(m_node);
		XDELETE_ARRAY(m_bezierPoint);
		XDELETE(m_cp);
	}
	m_cp = temp.m_cp;

//	if(m_node == NULL || m_isACopy != 0)
//	{//资源已经被释放，或者资源不属于自己，则这里重新分配内存空间
//		m_node = createArrayMem<_XNode>(MAX_NODE_SUM);
//	}
	m_isVisiable = temp.m_isVisiable;					//本身引导线的编号
	m_myOrder = temp.m_myOrder;					//本身引导线的编号
	m_leadMode = temp.m_leadMode;				//曲线的引导模式
	m_isLoop = temp.m_isLoop;					//曲线是否闭合0不闭合 1闭合，注意闭合的点，首点和尾点不能重合，否则将会失去尾点的方向

	m_offsetPosition = temp.m_offsetPosition;	//整体曲线的偏移位置
	m_size = temp.m_size;						//这个曲线的所放笔需要按照比例进行，不能够使用水平和垂直的不同比例，否则将会造成曲线中角度的失真严重

	m_lineLength = temp.m_lineLength;			//曲线的总长度
	m_nodeSum = temp.m_nodeSum;					//引导曲线中的引导节点的数量
	m_specialPoint = temp.m_specialPoint;		//曲线上的移动的点
	m_node = temp.m_node;						//引导线中的节点
	m_bezierPoint = temp.m_bezierPoint;	
	m_needUpdateData = temp.m_needUpdateData;						//引导线中的节点
	m_bezierSpline = temp.m_bezierSpline;						//引导线中的节点
//	for(int i = 0;i < m_nodeSum;++ i)
//	{
//		m_node[i] = temp.m_node[i];
//	}
	m_isACopy = XFalse;

	return *this;
}
_XBool _XNodeLine::isInRect(float x,float y)
{
//	return getIsInRect(_XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
	_XVector2 point(x,y);
	_XVector2 pointS,pointE;
	if(m_nodeSum == 1)
	{//只有一个点的节点曲线，判断两点间的距离
		if(point.getLength(_XVector2(m_node[0].myPoint.x * m_size.x + m_offsetPosition.x,
			m_node[0].myPoint.y * m_size.y + m_offsetPosition.y)) <= 5) return XTrue;
		else return XFalse;
	}else
	if(m_nodeSum >= 1)
	{
		for(int i = 0;i < m_nodeSum - 1;++ i)
		{
			pointS.set(m_node[i].myPoint.x * m_size.x + m_offsetPosition.x,
				m_node[i].myPoint.y * m_size.y + m_offsetPosition.y);
			pointE.set(m_node[i + 1].myPoint.x * m_size.x + m_offsetPosition.x,
				m_node[i + 1].myPoint.y * m_size.y + m_offsetPosition.y);
			if(minDistancePointToLine(point,pointS,pointE) <= 5) return XTrue;
		}
	}
	return XFalse;
}
_XVector2 _XNodeLine::getBox(int order)
{//这里需要返回包围盒
	_XVector2 ret;
	ret.set(0.0f,0.0f);
	float left = 0.0f;
	float right = 0.0f;
	float top = 0.0f;
	float bottom = 0.0f;
	if(m_nodeSum > 0)
	{//如果节点曲线中有节点，则这里计算节点的包围盒
		left = m_node[0].myPoint.x;
		right = m_node[0].myPoint.x;
		top = m_node[0].myPoint.y;
		bottom = m_node[0].myPoint.y;
		//下面开始遍历所有的点，寻找包围盒子
		for(int i = 1;i < m_nodeSum;++ i)
		{
			if(m_node[i].myPoint.x > right) right = m_node[i].myPoint.x;
			if(m_node[i].myPoint.x < left) left = m_node[i].myPoint.x;
			if(m_node[i].myPoint.y > bottom) bottom = m_node[i].myPoint.y;
			if(m_node[i].myPoint.y < top) top = m_node[i].myPoint.y;
		}
	}
	//下面对包围盒子进行扩展，防止单点或者单线的问题
	left -= 5;
	right += 5;
	top -= 5;
	bottom += 5;
	if(order == 0) ret.set(left * m_size.x + m_offsetPosition.x,	top * m_size.y + m_offsetPosition.y);else
	if(order == 1) ret.set(right * m_size.x + m_offsetPosition.x,	top * m_size.y + m_offsetPosition.y);else
	if(order == 2) ret.set(right * m_size.x + m_offsetPosition.x,	bottom * m_size.y + m_offsetPosition.y);else
	if(order == 3) ret.set(left * m_size.x + m_offsetPosition.x,	bottom * m_size.y + m_offsetPosition.y);
	return ret;
}
void _XNodeLine::draw()
{
	for(int j = 0;j < m_nodeSum - 1;++ j)
	{
		//描绘这个点
		if(j == 0)
		{//第一个点用绿色
			drawPoint(m_node[j].myPoint.x * m_size.x + m_offsetPosition.x,
				m_node[j].myPoint.y * m_size.y + m_offsetPosition.y,4,0.0f,1.0f,0.0f);
		}else
		{//其他点用红色
			drawPoint(m_node[j].myPoint.x * m_size.x + m_offsetPosition.x,
				m_node[j].myPoint.y * m_size.y + m_offsetPosition.y,4,1.0f,0.0f,0.0f);
		}
		drawLine(m_node[j].myPoint * m_size + m_offsetPosition,m_node[j + 1].myPoint * m_size + m_offsetPosition);
	}
	if(m_isLoop)
	{
		drawLine(m_node[m_nodeSum - 1].myPoint * m_size + m_offsetPosition,m_node[0].myPoint * m_size + m_offsetPosition);
	}
	drawLine(m_specialPoint.position,m_specialPoint.angle * RADIAN_TO_ANGLE,20.0f);
}