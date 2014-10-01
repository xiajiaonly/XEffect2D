//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "X2DPacker.h"
//按照高度排序
void _X2DPacker::orderObjectByWidth()
{//高度排序
	_XObjectBox temp;
	_XBool flag = XFalse;
	while(true)
	{
		flag = XFalse;
		for(int i = 0;i < m_objectBoxSum - 1;++ i)
		{
			if(m_objectBox[i].m_size.x < m_objectBox[i + 1].m_size.x)
			{
				temp = m_objectBox[i];
				m_objectBox[i] = m_objectBox[i + 1];
				m_objectBox[i + 1] = temp;
				flag = XTrue;
			}
		}
		if(!flag) break;	//排序完成
	}
}
//按照面积排序
void _X2DPacker::orderObjectByArea()
{
	_XObjectBox temp;
	_XBool flag = XFalse;
	while(true)
	{
		flag = XFalse;
		for(int i = 0;i < m_objectBoxSum - 1;++ i)
		{
			if(m_objectBox[i].m_acreage < m_objectBox[i + 1].m_acreage)
			{
				temp = m_objectBox[i];
				m_objectBox[i] = m_objectBox[i + 1];
				m_objectBox[i + 1] = temp;
				flag = XTrue;
			}
		}
		if(!flag) break;
	}
}
//按照最大边排序，然后再按面积排序
void _X2DPacker::orderObjectByBigEdgeAndArea()
{
	orderObjectByArea();	//首先按面积排序
	_XObjectBox temp;
	_XBool flag = XFalse;
	while(true)
	{
		flag = XFalse;
		for(int i = 0;i < m_objectBoxSum - 1;++ i)
		{
			if(max(m_objectBox[i].m_size.x,m_objectBox[i].m_size.y) < max(m_objectBox[i + 1].m_size.x,m_objectBox[i + 1].m_size.y))
			{
				temp = m_objectBox[i];
				m_objectBox[i] = m_objectBox[i + 1];
				m_objectBox[i + 1] = temp;
				flag = XTrue;
			}
		}
		if(!flag) break;
	}
}

int _X2DPacker::findResult(int orderMode)
{
	//这里没有进行详细的数据合理性检查，之后需要进行详细的检查
	if(m_basicBox == NULL || m_objectBox == NULL) return 0;
	if(m_objectBoxSum >= 2)
	{//首先对物件进行排序，优先放入大的物件
		if(orderMode == 1) orderObjectByWidth();
		else if(orderMode == 2) orderObjectByBigEdgeAndArea();
		else orderObjectByArea();
	}
	//对物件进行编号
	for(int i = 0;i < m_objectBoxSum;++ i)
	{
		m_objectBox[i].m_order = i;
	}
	//检查装箱数据是否有效
	int checkFlag = 0;
	//1、物件的最大尺寸不能大于箱子的尺寸
	//for(int i = 0;i < m_objectBoxSum - 1;++ i)
	if(!m_canRotate)
	{//不能旋转的装箱问题
		for(int i = 0;i < m_objectBoxSum;++ i)
		{
			checkFlag = 0;
			for(int j = 0;j < m_basicBoxSum;++ j)
			{//这里考虑装箱的大小可以不一致
				if(m_basicBox[j].m_size.x >= m_objectBox[i].m_size.x &&
					m_basicBox[j].m_size.y >= m_objectBox[i].m_size.y)
				{//能有箱子装得下
					checkFlag = 1;
					break;
				}
			}
			if(checkFlag == 0) return 0;	//存在超大物件所有箱子都装不下
		}
	}else
	{//可以旋转的装箱问题
		for(int i = 0;i < m_objectBoxSum;++ i)
		{
			checkFlag = 0;
			for(int j = 0;j < m_basicBoxSum;++ j)
			{//这里考虑装箱的大小可以不一致
				if((m_basicBox[j].m_size.x >= m_objectBox[i].m_size.x &&
					m_basicBox[j].m_size.y >= m_objectBox[i].m_size.y)
					|| (m_basicBox[j].m_size.x >= m_objectBox[i].m_size.y &&
					m_basicBox[j].m_size.y >= m_objectBox[i].m_size.x))
				{//能有箱子装得下
					checkFlag = 1;
					break;
				}
			}
			if(checkFlag == 0) return 0;	//存在超大物件所有箱子都装不下
		}
	}
	//下面将所有的物件放入箱子中
	for(int i = 0;i < m_objectBoxSum;++ i)
	{
		if(putInABox(m_objectBox[i]) == 0) return 0;	//存在装不下的物件
	}
	return 1;
}

int _X2DPacker::putInABox(_XObjectBox &objectBox)
{
	if(objectBox.m_basicBoxOrder >= 0) return 1;//如果物件已经放入过了，则不重新放
	for(int i = 0;i < m_basicBoxSum;++ i)
	{//尝试所有的箱子，看是否能放入
		if(canPutIn(objectBox,m_basicBox[i]) == 1) return 1;	//如果能放入则直接放入，这里不使用优化机制
	}
	return 0;	//所有盒子都装不下则直接返回装箱失败
}

void _X2DPacker::updatePutOperate(_XObjectBox &objectBox,_XBasicBox &basicBox,_XBool needRotate,float positionX,float positionY)
{
	_XVector2 tempSize(0.0f,0.0f);
	if(!needRotate) tempSize.set(objectBox.m_size.x,objectBox.m_size.y);
	else tempSize.set(objectBox.m_size.y,objectBox.m_size.x);

	objectBox.m_basicBoxOrder = basicBox.m_order;
	objectBox.m_position.set(positionX,positionY);
	objectBox.m_AABB.set(positionX,positionY,positionX + tempSize.x,positionY + tempSize.y);	//设置碰撞区域
	objectBox.m_needRotate = needRotate;
	basicBox.m_objectBoxesOrder[basicBox.m_beUsed] = objectBox.m_order;		//标记已经放入的物件的编号
	basicBox.m_beUsed ++;	//标记已经放入了一个物件
	basicBox.m_releaseAcreage -= objectBox.m_acreage;		//减去已经放入的面积
}

void _X2DPacker::updateNewPoint(_XBasicBox &basicBox,int sizeX,int sizeY,int pointOrder)
{
	basicBox.m_mayPosition[basicBox.m_mayPositionSum].set(basicBox.m_mayPosition[pointOrder].x,basicBox.m_mayPosition[pointOrder].y + sizeY);	//下边的点
	//由于当前这个点已经放了东西，所以这个点可以直接被替换掉
	basicBox.m_mayPosition[pointOrder].set(basicBox.m_mayPosition[pointOrder].x + sizeX,basicBox.m_mayPosition[pointOrder].y);	//左边的点
	basicBox.m_mayPositionSum ++;
}

void _X2DPacker::updateNewXY(_XBasicBox &basicBox,int x,int y)
{
	_XBool isInsert = XFalse;	//是否已经插入
	for(int i = 0;i < basicBox.m_mayPositionXSum;++ i)
	{
		if(basicBox.m_mayPositionX[i] > x)
		{//插入
			for(int j = basicBox.m_mayPositionXSum;j > i;-- j)
			{
				basicBox.m_mayPositionX[j] = basicBox.m_mayPositionX[j - 1];
			}
			basicBox.m_mayPositionX[i] = x;
			basicBox.m_mayPositionXSum ++;
			isInsert = XTrue;
			break;
		}else
		if(basicBox.m_mayPositionX[i] == x) 
		{
			isInsert = XTrue;
			break;
		}
	}
	if(!isInsert)
	{//插入失败
		basicBox.m_mayPositionX[basicBox.m_mayPositionXSum] = x;
		basicBox.m_mayPositionXSum ++;
	}
	isInsert = XFalse;
	for(int i = 0;i < basicBox.m_mayPositionYSum;++ i)
	{
		if(basicBox.m_mayPositionY[i] > y)
		{//插入
			for(int j = basicBox.m_mayPositionYSum;j > i;-- j)
			{
				basicBox.m_mayPositionY[j] = basicBox.m_mayPositionY[j - 1];
			}
			basicBox.m_mayPositionY[i] = y;
			basicBox.m_mayPositionYSum ++;
			isInsert = XTrue;
			break;
		}else
		if(basicBox.m_mayPositionY[i] == y) 
		{
			isInsert = XTrue;
			break;
		}
	}
	if(!isInsert)
	{//插入失败
		basicBox.m_mayPositionY[basicBox.m_mayPositionYSum] = y;
		basicBox.m_mayPositionYSum ++;
	}
}
//这个函数用于判断物件是否能够放入指定的箱子
int _X2DPacker::canPutIn(_XObjectBox &objectBox,_XBasicBox &basicBox)	
{
	if(basicBox.m_releaseAcreage < objectBox.m_acreage) return 0;	//剩余面积不够直接返回失败
	if(basicBox.m_beUsed == 0)
	{//这个箱子没有装过东西，则直接左上角对齐放置
		if(basicBox.m_size.x >= objectBox.m_size.x && basicBox.m_size.y >= objectBox.m_size.y)
		{//直接放置就可以放得下
			updatePutOperate(objectBox,basicBox,XFalse,0,0);
			if(m_optimizeLevel == 0) updateNewPoint(basicBox,objectBox.m_size.x,objectBox.m_size.y,0);
			else updateNewXY(basicBox,objectBox.m_size.x,objectBox.m_size.y);
			return 1;
		}
		if(m_canRotate && basicBox.m_size.x >= objectBox.m_size.y && basicBox.m_size.y >= objectBox.m_size.x)
		{//考虑旋转放入
			updatePutOperate(objectBox,basicBox,XTrue,0,0);
			if(m_optimizeLevel == 0) updateNewPoint(basicBox,objectBox.m_size.y,objectBox.m_size.x,0);
			else updateNewXY(basicBox,objectBox.m_size.y,objectBox.m_size.x);
			return 1;
		}
		return 0;
	}else
	{//这个箱子被装过东西
		//遍历所有的放入点，找到第一个合适的放入点
		int pointOK = 0;	//这个点是否会与其他的盒子发生碰撞
		_XRect objRect1;
		if(m_optimizeLevel == 1)
		{
			float tempPx,tempPy;
			for(int k = 0;k < basicBox.m_mayPositionYSum;++ k)
			{
				tempPy = (float)(basicBox.m_mayPositionY[k]);
				//这里是为了优化速度，对于不需要判断的点直接放弃
				if((!m_canRotate && tempPy + objectBox.m_size.y > basicBox.m_size.y)
					|| (m_canRotate && tempPy + objectBox.m_size.x > basicBox.m_size.y && tempPy + objectBox.m_size.y > basicBox.m_size.y)) continue;
				for(int i = 0;i < basicBox.m_mayPositionXSum;++ i)
				{
					tempPx = (float)(basicBox.m_mayPositionX[i]);
					if((!m_canRotate && tempPx + objectBox.m_size.x > basicBox.m_size.x)
						|| (m_canRotate && tempPx + objectBox.m_size.y > basicBox.m_size.x && tempPx + objectBox.m_size.x > basicBox.m_size.x)) break;

					pointOK = 0;
					if(tempPx + objectBox.m_size.x <= basicBox.m_size.x
						&& tempPy + objectBox.m_size.y <= basicBox.m_size.y)//判断边界是否越界
					{
						pointOK = 1;
						//检查与其他盒子的碰撞问题
						objRect1.set(tempPx,tempPy,tempPx + objectBox.m_size.x,tempPy + objectBox.m_size.y);
						pointOK = 1;
						for(int j = 0;j < basicBox.m_beUsed;++ j)
						{
							if(objRect1.isCrash(m_objectBox[basicBox.m_objectBoxesOrder[j]].m_AABB))
							{
								pointOK = 0;
								break;
							}
						}
					}
					if(pointOK != 0)
					{//这个点可以
						updatePutOperate(objectBox,basicBox,XFalse,tempPx,tempPy);
						//由于物件的放入更新新的可能
						updateNewXY(basicBox,tempPx + objectBox.m_size.x,tempPy + objectBox.m_size.y);
						return 1;
					}else
					if(m_canRotate)
					{//当前组合不符合要求，考虑旋转
						pointOK = 0;
						if(tempPx + objectBox.m_size.y <= basicBox.m_size.x
							&& tempPy + objectBox.m_size.x <= basicBox.m_size.y)//判断边界是否越界
						{
							pointOK = 1;
							//检查与其他盒子的碰撞问题
							objRect1.set(tempPx,tempPy,tempPx + objectBox.m_size.y,tempPy + objectBox.m_size.x);
							pointOK = 1;
							for(int j = 0;j < basicBox.m_beUsed;++ j)
							{
								if(objRect1.isCrash(m_objectBox[basicBox.m_objectBoxesOrder[j]].m_AABB))
								{
									pointOK = 0;
									break;
								}
							}
						}
						if(pointOK != 0)
						{//这个点可以
							updatePutOperate(objectBox,basicBox,XTrue,tempPx,tempPy);
							//由于物件的放入更新新的可能
							updateNewXY(basicBox,tempPx + objectBox.m_size.y,tempPy + objectBox.m_size.x);
							return 1;
						}else
						{
							continue;
						}
					}else
					{
						continue;
					}
				}
			}
		}else
		{
			for(int i = 0;i < basicBox.m_mayPositionSum;++ i)
			{
				pointOK = 0;
				if(basicBox.m_mayPosition[i].x + objectBox.m_size.x <= basicBox.m_size.x
					&& basicBox.m_mayPosition[i].y + objectBox.m_size.y <= basicBox.m_size.y)//判断边界是否越界
				{
					pointOK = 1;
					//检查与其他盒子的碰撞问题
					objRect1.set(basicBox.m_mayPosition[i].x,basicBox.m_mayPosition[i].y,
						basicBox.m_mayPosition[i].x + objectBox.m_size.x,basicBox.m_mayPosition[i].y + objectBox.m_size.y);
					for(int j = 0;j < basicBox.m_beUsed;++ j)
					{
						if(objRect1.isCrash(m_objectBox[basicBox.m_objectBoxesOrder[j]].m_AABB))
						{
							pointOK = 0;
							break;
						}
					}
				}
				if(pointOK != 0)
				{//这个点可以
					updatePutOperate(objectBox,basicBox,XFalse,basicBox.m_mayPosition[i].x,basicBox.m_mayPosition[i].y);
					updateNewPoint(basicBox,objectBox.m_size.x,objectBox.m_size.y,i);
					return 1;
				}else
				{//这个点不行，则考虑旋转
					if(m_canRotate)
					{//允许旋转
						pointOK = 0;
						if(basicBox.m_mayPosition[i].x + objectBox.m_size.y <= basicBox.m_size.x
							&& basicBox.m_mayPosition[i].y + objectBox.m_size.x <= basicBox.m_size.y)//判断边界是否越界
						{
							pointOK = 1;
							//检查与其他盒子的碰撞问题
							objRect1.set(basicBox.m_mayPosition[i].x,basicBox.m_mayPosition[i].y,
								basicBox.m_mayPosition[i].x + objectBox.m_size.y,basicBox.m_mayPosition[i].y + objectBox.m_size.x);
							pointOK = 1;
							for(int j = 0;j < basicBox.m_beUsed;++ j)
							{
								if(objRect1.isCrash(m_objectBox[basicBox.m_objectBoxesOrder[j]].m_AABB))
								{
									pointOK = 0;
									break;
								}
							}
						}
						if(pointOK == 0)
						{//这个点不行
							continue;
						}else
						{//这个点可以
							updatePutOperate(objectBox,basicBox,XTrue,basicBox.m_mayPosition[i].x,basicBox.m_mayPosition[i].y);
							updateNewPoint(basicBox,objectBox.m_size.y,objectBox.m_size.x,i);
							return 1;
						}
					}else
					{//如果不允许旋转则直接继续
						continue;
					}
				}
			}
		}
	}
	return 0;
}

int _X2DPacker::getNeedBoxSum() const
{
	if(m_basicBox == NULL) return 0;
	for(int i = 0;i < m_basicBoxSum;++ i)
	{
		if(m_basicBox[i].m_beUsed == 0) return i;
	}
	return 0;
}

void _X2DPacker::reset()
{
	for(int i = 0;i < m_basicBoxSum;i ++)
	{
		m_basicBox[i].m_beUsed = 0;
		m_basicBox[i].m_releaseAcreage = m_basicBox[i].m_size.x * m_basicBox[i].m_size.y;

		m_basicBox[i].m_mayPositionSum = 1;
		m_basicBox[i].m_mayPosition[0].set(0.0f,0.0f);

		m_basicBox[i].m_mayPositionXSum = 1;
		m_basicBox[i].m_mayPositionX[0] = 0;
		m_basicBox[i].m_mayPositionYSum = 1;
		m_basicBox[i].m_mayPositionY[0] = 0;
	}
	for(int i = 0;i < m_objectBoxSum;i ++)
	{
		m_objectBox[i].m_order = i;
		m_objectBox[i].m_setOrder = i;
		m_objectBox[i].m_position.set(0.0f,0.0f);
		m_objectBox[i].m_basicBoxOrder = -1;
	}

}