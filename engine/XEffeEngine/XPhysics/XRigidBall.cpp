//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.12.10
//--------------------------------
#include "XRigidBall.h"

float gravity = 9.80f;
//float gravity = 0.00f;
#define CRASH_SPEED_DOWN (1.0f)

int _XLiquidBalls::init(int liquidBallSum,int lineSum)
{
	if(m_isInited != 0) return 0;
	if(liquidBallSum <= 0) return 0;
	if(lineSum <= 0) return 0;

	m_liquidBallSum = liquidBallSum;
	m_pRigidBall = createArrayMem<_XRigidBall>(m_liquidBallSum);
	if(m_pRigidBall == NULL) return 0;

	m_lineSum = lineSum;
	m_pLine = createArrayMem<_XLine>(m_lineSum);
	if(m_pLine == NULL) 
	{
		XDELETE_ARRAY(m_pRigidBall);
		return 0;
	}

	//m_area.set(50.0f,50.0f,1000.0f,700.0f);
	//m_liquidBallDataSum = ((1 + m_liquidBallSum + m_lineSum - 1) * (m_liquidBallSum + m_lineSum - 1)) >> 1;
	//线段需要碰撞的判断为三个物体，线段本体以及线段两个端点
	m_liquidBallDataSum = (((3 * m_lineSum) + (m_liquidBallSum - 1 + 3 * m_lineSum)) * m_liquidBallSum) >> 1;
//	m_pLiquidBallData = createArrayMem<double>(m_liquidBallDataSum);
//	if(m_pLiquidBallData == NULL) return 0;
//	m_pLiquidBallFlag = createArrayMem<char>(m_liquidBallDataSum);
//	if(m_pLiquidBallFlag == NULL) return 0;

	m_pCrashData = createArrayMem<_XCrashData>(m_liquidBallDataSum);
	if(m_pCrashData == NULL)
	{
		XDELETE_ARRAY(m_pRigidBall);
		XDELETE_ARRAY(m_pLine);
		return 0;
	}
	m_haveError = 0;
	m_isInited = 1;

	return 1;
}

void _XLiquidBalls::move(int timeDelay)
{
	if(m_isInited == 0) return;
	if(m_isEnable == 0) return;
	if(m_haveError != 0) return;
	crashProc(timeDelay);
}

int _XLiquidBalls::checkProc()//检查数据是否合格
{
	//进行正确性检查
	for(int i = 0;i < m_liquidBallSum - 1;++ i)
	{
		if(m_pRigidBall[i].getIsEnable() != 0)
		{
			for(int j = i + 1;j < m_liquidBallSum;++ j)
			{
				if(m_pRigidBall[j].getIsEnable() != 0)
				{
					if((m_pRigidBall[i].m_position.x - m_pRigidBall[j].m_position.x) * (m_pRigidBall[i].m_position.x - m_pRigidBall[j].m_position.x) +
						(m_pRigidBall[i].m_position.y - m_pRigidBall[j].m_position.y) * (m_pRigidBall[i].m_position.y - m_pRigidBall[j].m_position.y) <
						(m_pRigidBall[i].m_radius + m_pRigidBall[j].m_radius - 2.0f) * (m_pRigidBall[i].m_radius + m_pRigidBall[j].m_radius - 2.0f) - 1.0f)
					{
						m_haveError = 1;
//						printf("haha:%d,%d--------\n",i,j);
						return 0;
					}
				}
			}
		}
	}
	for(int i = 0;i < m_liquidBallSum;++ i)
	{
		if(m_pRigidBall[i].getIsEnable() != 0)
		{
			if(m_pRigidBall[i].m_position.y >= 700.0f - m_pRigidBall[i].m_radius + 3.0f
				|| m_pRigidBall[i].m_position.y <= 50.0f + m_pRigidBall[i].m_radius - 3.0f
				|| m_pRigidBall[i].m_position.x >= 1000.0f - m_pRigidBall[i].m_radius + 3.0f
				|| m_pRigidBall[i].m_position.x <= 50.0f + m_pRigidBall[i].m_radius - 3.0f)
			{
				m_haveError = 1;
//				printf("Out:%d,+++++++++\n",i);
				return 0;
			}
			if((m_pRigidBall[i].m_position.y <= 500.0f && m_pRigidBall[i].m_position.y >= 300.0f && 
				m_pRigidBall[i].m_position.x >= 300.0f - m_pRigidBall[i].m_radius + 1.0f && m_pRigidBall[i].m_position.x <= 500.0f + m_pRigidBall[i].m_radius - 1.0f)
				|| (m_pRigidBall[i].m_position.x <= 500.0f && m_pRigidBall[i].m_position.x >= 300.0f && 
				m_pRigidBall[i].m_position.y >= 300.0f - m_pRigidBall[i].m_radius + 1.0f && m_pRigidBall[i].m_position.y <= 500.0f + m_pRigidBall[i].m_radius - 1.0f)
				/*m_pRigidBall[i].m_position.y <= 500.0f + m_pRigidBall[i].m_radius * 0.707f - 3.0f
				&& m_pRigidBall[i].m_position.y >= 300.0f - m_pRigidBall[i].m_radius * 0.707f + 3.0f
				&& m_pRigidBall[i].m_position.x <= 500.0f + m_pRigidBall[i].m_radius * 0.707f - 3.0f
				&& m_pRigidBall[i].m_position.x >= 300.0f - m_pRigidBall[i].m_radius * 0.707f + 3.0f*/)
			{
				m_haveError = 1;
//				printf("Into:%d,+++++++++\n",i);
				return 0;
			}
			if( (m_pRigidBall[i].m_position.y - 300.0f) * (m_pRigidBall[i].m_position.y - 300.0f) + (m_pRigidBall[i].m_position.x - 300.0f) * (m_pRigidBall[i].m_position.x - 300.0f) < (m_pRigidBall[i].m_radius - 3.0f)*(m_pRigidBall[i].m_radius - 3.0f) ||
				(m_pRigidBall[i].m_position.y - 300.0f) * (m_pRigidBall[i].m_position.y - 300.0f) + (m_pRigidBall[i].m_position.x - 500.0f) * (m_pRigidBall[i].m_position.x - 500.0f) < (m_pRigidBall[i].m_radius - 3.0f)*(m_pRigidBall[i].m_radius - 3.0f) ||
				(m_pRigidBall[i].m_position.y - 500.0f) * (m_pRigidBall[i].m_position.y - 500.0f) + (m_pRigidBall[i].m_position.x - 300.0f) * (m_pRigidBall[i].m_position.x - 300.0f) < (m_pRigidBall[i].m_radius - 3.0f)*(m_pRigidBall[i].m_radius - 3.0f) ||
				(m_pRigidBall[i].m_position.y - 500.0f) * (m_pRigidBall[i].m_position.y - 500.0f) + (m_pRigidBall[i].m_position.x - 500.0f) * (m_pRigidBall[i].m_position.x - 500.0f) < (m_pRigidBall[i].m_radius - 3.0f)*(m_pRigidBall[i].m_radius - 3.0f))
			{
				m_haveError = 1;
//				printf("HitPoint:%d,+++++++++\n",i);
				return 0;
			}
		}
	}
	return 1;
}

//暂时不计算球与墙面的碰撞
void _XLiquidBalls::crashProc(int timeDelay)
{
	if(m_isInited == 0) return;
	if(m_isEnable == 0) return;
	for(int i = 0;i < m_liquidBallDataSum;++ i)
	{
		m_pCrashData[i].m_needFrash = 1;
		//m_pCrashData[i].m_needFrash = 1;	//初始化所有的相互距离的相关数据都需要计算
	}
	//for(int i = 0;i < m_liquidBallSum;++ i)
	//{
	//	m_pRigidBall[i].m_upCrashOrder = -1;
	//}
	m_deep = 0;
	crashInsideProc(timeDelay,0.0f);
	//进行正确性检查
	checkProc();
}

void _XLiquidBalls::crashTimerProc(double timeDelay,double upTime)
{
	m_crashObj0 = 0;	//如果在时间段内发生了碰撞，则这里为发生碰撞的两个物体的编号
	m_crashObj1 = 0;
	m_minTimer = -1.0f;
	m_crashSameTimeSum = 0;
	m_crashOrder = 0;
	int order = 0;
	for(int i = 0;i < m_liquidBallSum;++ i)
	{
		if(m_pRigidBall[i].getIsEnable() != 0)
		{//有效
			for(int j = i + 1;j < m_liquidBallSum + m_lineSum;++ j)
			{
				if(j < m_liquidBallSum)
				{//与物体的碰撞
					if(m_pRigidBall[j].getIsEnable() != 0)
					{
						if(m_pCrashData[order].m_needFrash != 0)
						//if(1)
						{
							m_pCrashData[order].m_crashTime = getCrashTimerB2B(m_pRigidBall[i].m_positionS,m_pRigidBall[i].m_speed,_XVector2(0.0f,gravity),	//这个速度需要乘以0.0001
								m_pRigidBall[j].m_positionS,m_pRigidBall[j].m_speed,_XVector2(0.0f,gravity),
								m_pRigidBall[i].m_radius + m_pRigidBall[j].m_radius,timeDelay);
							m_pCrashData[order].m_needFrash = 0;
							if(m_pCrashData[order].m_crashTime >= 0 && m_minTimer > 0 && abs(m_pCrashData[order].m_crashTime - m_minTimer) <= 0.0001f)
							{//多元同时碰撞的数量
								m_crashSameTimeSum++;
							}
							if(m_pCrashData[order].m_crashTime >= 0)
							{//判断是否为最小值
								//这里暂时先不进行优化，之后在进行这部分工作
								m_pCrashData[order].m_crashAngle = m_pRigidBall[i].m_positionS.getAngle(m_pRigidBall[j].m_positionS);
								if(m_pCrashData[order].m_crashTime < m_minTimer || m_minTimer < 0)
								{
									m_minTimer = m_pCrashData[order].m_crashTime;	//最小值
									m_crashObj0 = i;
									m_crashObj1 = j;
									m_crashOrder = order;
								}
							}
						}else
						{
							if(m_pCrashData[order].m_crashTime >= 0)
							{
								m_pCrashData[order].m_crashTime -= upTime;
								if(m_pCrashData[order].m_crashTime < 0) m_pCrashData[order].m_crashTime = 0;
								if(m_pCrashData[order].m_crashTime >= 0 && m_minTimer > 0 && abs(m_pCrashData[order].m_crashTime - m_minTimer) <= 0.0001f)
								{//多元同时碰撞的数量
									m_crashSameTimeSum++;
								}
								if(m_pCrashData[order].m_crashTime >= 0)
								{//判断是否为最小值
									if(m_pCrashData[order].m_crashTime < m_minTimer || m_minTimer < 0)
									{
										m_minTimer = m_pCrashData[order].m_crashTime;	//最小值
										m_crashObj0 = i;
										m_crashObj1 = j;
										m_crashOrder = order;
									}
								}
							}
						}
					}else
					{
						m_pCrashData[order].m_crashTime = -1.0; 
					}
					++ order;
				}else
				{//与线段的碰撞
					if(m_pLine[j - m_liquidBallSum].getIsEnable() != 0)
					{
						//if(m_pCrashData[order].m_needFrash != 0)
						if(1)
						{//需要进行碰撞判断
							//double tempTime;
							//线段本体
							m_pCrashData[order].m_crashTime = getCrashTimerB2L(m_pRigidBall[i].m_positionS,m_pRigidBall[i].m_speed,_XVector2(0.0f,gravity),
								m_pLine[j - m_liquidBallSum],m_pRigidBall[i].m_radius,timeDelay);
							//m_pCrashData[order].m_crashTime = getCrashTimerB2L(m_pRigidBall[i].m_positionS,m_pRigidBall[i].m_speed,_XVector2(0.0f,gravity),
							//	m_pLine[j - m_liquidBallSum].getStartPosition(),m_pLine[j - m_liquidBallSum].getEndPosition(),m_pRigidBall[i].m_radius,timeDelay);

							//tempTime = getCrashTimerB2L(m_pRigidBall[i].m_positionS,m_pRigidBall[i].m_speed,_XVector2(0.0f,gravity),
							//	&(m_pLine[j - m_liquidBallSum]),m_pRigidBall[i].m_radius,timeDelay);
							//if(m_pCrashData[order].m_needFrash == 0 && tempTime >= 0 && m_pCrashData[order].m_crashTime < 0)
							//{
							//	printf("Error!\n");
							//}
							//m_pCrashData[order].m_crashTime = tempTime;

							m_pCrashData[order].m_needFrash = 0;
							if(m_pCrashData[order].m_crashTime >= 0 && m_minTimer > 0 && abs(m_pCrashData[order].m_crashTime - m_minTimer) <= 0.0001f)
							{//多元同时碰撞的数量
								m_crashSameTimeSum++;
							}
							if(m_pCrashData[order].m_crashTime >= 0 )
							{//判断是否为最小值
								m_pCrashData[order].m_crashAngle = m_pLine[j - m_liquidBallSum].getLineAngle();
								if(m_pCrashData[order].m_crashTime < m_minTimer || m_minTimer < 0)
								{
									m_minTimer = m_pCrashData[order].m_crashTime;	//最小值
									m_crashObj0 = i;
									m_crashObj1 = m_liquidBallSum + (j - m_liquidBallSum) * 3 + 0;
									m_crashOrder = order;
								}
							}
							++order;
							//线段端点
							m_pCrashData[order].m_crashTime = getCrashTimerB2P(m_pRigidBall[i].m_positionS,m_pRigidBall[i].m_speed,_XVector2(0.0f,gravity),
								m_pLine[j - m_liquidBallSum].getStartPosition(),m_pRigidBall[i].m_radius,timeDelay);

							//tempTime = getCrashTimerB2P(m_pRigidBall[i].m_positionS,m_pRigidBall[i].m_speed,_XVector2(0.0f,gravity),
							//	m_pLine[j - m_liquidBallSum].getStartPosition(),m_pRigidBall[i].m_radius,timeDelay);
							//if(m_pCrashData[order].m_needFrash == 0 && tempTime >= 0 && m_pCrashData[order].m_crashTime < 0)
							//{
							//	printf("Error!\n");
							//}
							//m_pCrashData[order].m_crashTime = tempTime;

							m_pCrashData[order - 1].m_needFrash = 0;
							if(m_pCrashData[order].m_crashTime >= 0 && m_minTimer > 0 && abs(m_pCrashData[order].m_crashTime - m_minTimer) <= 0.0001f)
							{//多元同时碰撞的数量
								m_crashSameTimeSum++;
							}
							if(m_pCrashData[order].m_crashTime >= 0 )
							{//判断是否为最小值
								m_pCrashData[order].m_crashAngle = m_pRigidBall[i].m_positionS.getAngle(m_pLine[j - m_liquidBallSum].getStartPosition());
								if(m_pCrashData[order].m_crashTime < m_minTimer || m_minTimer < 0)
								{
									m_minTimer = m_pCrashData[order].m_crashTime;	//最小值
									m_crashObj0 = i;
									m_crashObj1 = m_liquidBallSum + (j - m_liquidBallSum) * 3 + 1;
									m_crashOrder = order;
								}
							}
							++order;
							//线段端点
							m_pCrashData[order].m_crashTime = getCrashTimerB2P(m_pRigidBall[i].m_positionS,m_pRigidBall[i].m_speed,_XVector2(0.0f,gravity),
								m_pLine[j - m_liquidBallSum].getEndPosition(),m_pRigidBall[i].m_radius,timeDelay);

							//tempTime = getCrashTimerB2P(m_pRigidBall[i].m_positionS,m_pRigidBall[i].m_speed,_XVector2(0.0f,gravity),
							//	m_pLine[j - m_liquidBallSum].getEndPosition(),m_pRigidBall[i].m_radius,timeDelay);
							//if(m_pCrashData[order].m_needFrash == 0 && tempTime >= 0 && m_pCrashData[order].m_crashTime < 0)
							//{
							//	printf("Error!\n");
							//}
							//m_pCrashData[order].m_crashTime = tempTime;

							m_pCrashData[order - 2].m_needFrash = 0;
							if(m_pCrashData[order].m_crashTime >= 0 && m_minTimer > 0 && abs(m_pCrashData[order].m_crashTime - m_minTimer) <= 0.0001f)
							{//多元同时碰撞的数量
								m_crashSameTimeSum++;
							}
							if(m_pCrashData[order].m_crashTime >= 0 )
							{//判断是否为最小值
								m_pCrashData[order].m_crashAngle = m_pRigidBall[i].m_positionS.getAngle(m_pLine[j - m_liquidBallSum].getEndPosition());
								if(m_pCrashData[order].m_crashTime < m_minTimer || m_minTimer < 0)
								{
									m_minTimer = m_pCrashData[order].m_crashTime;	//最小值
									m_crashObj0 = i;
									m_crashObj1 = m_liquidBallSum + (j - m_liquidBallSum) * 3 + 2;
									m_crashOrder = order;
								}
							}
							++order;
						}else
						{	//线段本体
							if(m_pCrashData[order].m_crashTime >= 0)
							{
								m_pCrashData[order].m_crashTime -= upTime;
								if(m_pCrashData[order].m_crashTime < 0) m_pCrashData[order].m_crashTime = 0;
								if(m_pCrashData[order].m_crashTime >= 0 && m_minTimer > 0 && abs(m_pCrashData[order].m_crashTime - m_minTimer) <= 0.0001f)
								{//多元同时碰撞的数量
									m_crashSameTimeSum++;
								}
								if(m_pCrashData[order].m_crashTime >= 0 )
								{//判断是否为最小值
									if(m_pCrashData[order].m_crashTime < m_minTimer || m_minTimer < 0)
									{
										m_minTimer = m_pCrashData[order].m_crashTime;	//最小值
										m_crashObj0 = i;
										m_crashObj1 = m_liquidBallSum + (j - m_liquidBallSum) * 3 + 0;
										m_crashOrder = order;
									}
								}
							}
							++order;
							//线段端点
							if(m_pCrashData[order].m_crashTime >= 0)
							{
								m_pCrashData[order].m_crashTime -= upTime;
								if(m_pCrashData[order].m_crashTime < 0) m_pCrashData[order].m_crashTime = 0;
								if(m_pCrashData[order].m_crashTime >= 0 && m_minTimer > 0 && abs(m_pCrashData[order].m_crashTime - m_minTimer) <= 0.0001f)
								{//多元同时碰撞的数量
									m_crashSameTimeSum++;
								}
								if(m_pCrashData[order].m_crashTime >= 0 )
								{//判断是否为最小值
									if(m_pCrashData[order].m_crashTime < m_minTimer || m_minTimer < 0)
									{
										m_minTimer = m_pCrashData[order].m_crashTime;	//最小值
										m_crashObj0 = i;
										m_crashObj1 = m_liquidBallSum + (j - m_liquidBallSum) * 3 + 1;
										m_crashOrder = order;
									}
								}
							}
							++order;
							//线段端点
							if(m_pCrashData[order].m_crashTime >= 0)
							{
								m_pCrashData[order].m_crashTime -= upTime;
								if(m_pCrashData[order].m_crashTime < 0) m_pCrashData[order].m_crashTime = 0;
								if(m_pCrashData[order].m_crashTime >= 0 && m_minTimer > 0 && abs(m_pCrashData[order].m_crashTime - m_minTimer) <= 0.0001f)
								{//多元同时碰撞的数量
									m_crashSameTimeSum++;
								}
								if(m_pCrashData[order].m_crashTime >= 0 )
								{//判断是否为最小值
									if(m_pCrashData[order].m_crashTime < m_minTimer || m_minTimer < 0)
									{
										m_minTimer = m_pCrashData[order].m_crashTime;	//最小值
										m_crashObj0 = i;
										m_crashObj1 = m_liquidBallSum + (j - m_liquidBallSum) * 3 + 2;
										m_crashOrder = order;
									}
								}
							}
							++order;
						}
					}else
					{
						m_pCrashData[order].m_crashTime = -1.0;	//线段本体
						++order;
						m_pCrashData[order].m_crashTime = -1.0;	//线段端点
						++order;
						m_pCrashData[order].m_crashTime = -1.0;	//线段端点
						++order;
					}
				}
			}
		}else
		{//无效
			for(int j = i + 1;j < m_liquidBallSum + m_lineSum;++ j)
			{
				if(j < m_liquidBallSum)
				{
					m_pCrashData[order].m_crashTime = -1.0; 
					++ order;
				}else
				{
					m_pCrashData[order].m_crashTime = -1.0;	//线段本体
					++order;
					m_pCrashData[order].m_crashTime = -1.0;	//线段端点
					++order;
					m_pCrashData[order].m_crashTime = -1.0;	//线段端点
					++order;
				}
			}
		}
	}
	if(order > m_liquidBallDataSum)
	{
		m_haveError = 1;
//		printf("Error!\n");
	}
}

int _XLiquidBalls::crashInsideProc(double timeDelay,double upTime)
{
	m_deep ++;
	if(m_deep >= 512) 
	{
		m_haveError = 1;
//		printf("Deep Too large!\n");
		return 1;	//防止迭代深度过深
	}
	//初始化一些信息
	//计算所有的点
	for(int i = 0;i < m_liquidBallSum;++ i)
	{
		if(m_pRigidBall[i].getIsEnable() != 0)
		{
			m_pRigidBall[i].m_positionS = m_pRigidBall[i].m_position;
		}
	}
	//计算碰撞时间
	crashTimerProc(timeDelay,upTime);

	//下面进行状态更新
	if(m_minTimer >= 0)
	{
//		printf("Crash in same time:%d\n",m_crashSameTimeSum);
	//	if(m_crashSameTimeSum > 1)
	//	{
	//		m_haveError = 1;
	//		printf("same time Crash too more!\n");
	//	}
//		printf("dx:%f,dy:%f,px:%f,py:%f\n",m_pRigidBall[m_crashObj0].m_speed.x,m_pRigidBall[m_crashObj0].m_speed.y,
//			m_pRigidBall[m_crashObj0].m_position.x,m_pRigidBall[m_crashObj0].m_position.y);
		for(int i = 0;i < m_liquidBallSum;++ i)
		{
			if(m_pRigidBall[i].getIsEnable() != 0)
			{
				m_pRigidBall[i].m_positionS.set(m_pRigidBall[i].m_position.x + m_pRigidBall[i].m_speed.x * m_minTimer * 0.001f,
					m_pRigidBall[i].m_position.y + m_pRigidBall[i].m_speed.y * m_minTimer * 0.001f + 0.5f * gravity * (m_minTimer * 0.001f) * (m_minTimer * 0.001f));
				m_pRigidBall[i].m_position = m_pRigidBall[i].m_positionS;
				//计算空气阻力
				//计算重力
				{
					m_pRigidBall[i].m_speed.y += gravity * m_minTimer * 0.001f;
				}
			}
		}
		//下面进行碰撞处理
		{//主要是速度的变化问题
			//与物体j,i,相关的都需要重新标记，计算(这部分优化工作本身在逻辑上没有问题，但是如果考虑浮点数结余误差，对于重合或者几乎要重合的物体会存在结余误差造成的严重后果，需要进一步考虑)
			int tempBasic = 0;
			if(m_crashObj1 < m_liquidBallSum)
			{//两个运动物体的碰撞
				for(int i = 0;i <= m_crashObj1;++ i)
				{
					if(m_pRigidBall[i].getIsEnable() != 0)
					{//有效
						for(int j = i + 1;j < m_liquidBallSum + m_lineSum;++ j)
						{
							if(i == m_crashObj0 || j == m_crashObj1 
								|| i == m_crashObj1 || j == m_crashObj0) m_pCrashData[tempBasic].m_needFrash = 1;
							if(j >= m_liquidBallSum) tempBasic += 3;
							else tempBasic ++;
						}
					}else
					{
						tempBasic ++;
					}
				}
			}else
			{//运动物体与固定线段的碰撞
				for(int i = 0;i < m_liquidBallSum;++ i)
				{
					if(m_pRigidBall[i].getIsEnable() != 0)
					{//有效
						for(int j = i + 1;j < m_liquidBallSum + m_lineSum;++ j)
						{
							if(i == m_crashObj0 || j == m_crashObj0) m_pCrashData[tempBasic].m_needFrash = 1;
							if(j >= m_liquidBallSum) tempBasic += 3;
							else tempBasic ++;
						}
					}else
					{
						tempBasic ++;
					}
				}
			}
			//{
			//	printf("%d,%d:%d\n",m_crashObj0,m_crashObj1,m_liquidBallDataSum);
			//	tempBasic = 0;
			//	for(int i = 0;i < m_liquidBallSum;++ i)
			//	{
			//		for(int j = 0;j < i;j++)
			//		{
			//			printf(" ");
			//		}
			//		for(int j = i + 1;j < m_liquidBallSum + m_lineSum;++ j)
			//		{
			//			if(j >= m_liquidBallSum) 
			//			{
			//				printf("%d",m_pCrashData[tempBasic].m_needFrash);
			//				tempBasic ++;
			//				printf("%d",m_pCrashData[tempBasic].m_needFrash);
			//				tempBasic ++;
			//				printf("%d",m_pCrashData[tempBasic].m_needFrash);
			//				tempBasic ++;
			//			}else
			//			{
			//				printf("%d",m_pCrashData[tempBasic].m_needFrash);
			//				tempBasic ++;
			//			}
			//		}
			//		printf("\n");
			//	}
			//	m_haveError = 1;
			//}
			//m_pRigidBall[m_crashObj0].m_upCrashOrder = m_crashObj1;	//标记两个物体发生了碰撞
			//下面将质量的作用引入
			if(m_crashObj1 < m_liquidBallSum)
			{//目标为一个物件
				//拆分速度
				float tempAngle = m_pCrashData[m_crashOrder].m_crashAngle;//碰撞角度
				float tempSin = sin(tempAngle);
				float tempCos = cos(tempAngle);
				float speedx1 = m_pRigidBall[m_crashObj0].m_speed.x;
				float speedy1 = m_pRigidBall[m_crashObj0].m_speed.y;
				float dx1 = speedx1 * tempCos + speedy1 * tempSin;
				float dy1 = speedy1 * tempCos - speedx1 * tempSin;
				float speedx2 = m_pRigidBall[m_crashObj1].m_speed.x;
				float speedy2 = m_pRigidBall[m_crashObj1].m_speed.y;
				float dx2 = speedx2 * tempCos + speedy2 * tempSin;
				float dy2 = speedy2 * tempCos - speedx2 * tempSin;
				//计算对冲分量的后值
				float Rx1 = (dx1 * (m_pRigidBall[m_crashObj0].m_mass - m_pRigidBall[m_crashObj1].m_mass) + 2.0f * m_pRigidBall[m_crashObj1].m_mass * dx2) / (m_pRigidBall[m_crashObj0].m_mass + m_pRigidBall[m_crashObj1].m_mass);
				float Rx2 = (dx2 * (m_pRigidBall[m_crashObj1].m_mass - m_pRigidBall[m_crashObj0].m_mass) + 2.0f * m_pRigidBall[m_crashObj0].m_mass * dx1) / (m_pRigidBall[m_crashObj1].m_mass + m_pRigidBall[m_crashObj0].m_mass);
				dx1 = Rx1;
				dx2 = Rx2;
				//再次进行坐标系旋转
				tempSin = sin(-tempAngle);
				tempCos = cos(-tempAngle);
				speedx1 = dx1 * tempCos + dy1 * tempSin;
				speedy1 = dy1 * tempCos - dx1 * tempSin;
				speedx2 = dx2 * tempCos + dy2 * tempSin;
				speedy2 = dy2 * tempCos - dx2 * tempSin;
				//还原值
				m_pRigidBall[m_crashObj0].m_speed.x = speedx1 * CRASH_SPEED_DOWN;
				m_pRigidBall[m_crashObj0].m_speed.y = speedy1 * CRASH_SPEED_DOWN;
				m_pRigidBall[m_crashObj1].m_speed.x = speedx2 * CRASH_SPEED_DOWN;
				m_pRigidBall[m_crashObj1].m_speed.y = speedy2 * CRASH_SPEED_DOWN;
				//打印计算结果
//				printf("Crash!D:%d,%d,%d,A:%3.2f\n",m_deep,m_crashObj0,m_crashObj1,tempAngle /PI * 180.0f);
			}else
			{//目标为线段
				//打印计算结果
//				printf("Crash!D:%d,%d,%d -->\n",m_deep,m_crashObj0,m_crashObj1,m_pRigidBall[m_crashObj0].m_speed.x,m_pRigidBall[m_crashObj0].m_speed.y);
				if((m_crashObj1 - m_liquidBallSum) % 3 == 0)
				{//线段本体
					//float tempAngle = m_pLine[(m_crashObj1 - m_liquidBallSum) / 3].getLineAngle();//碰撞角度
					//float tempSin = sin(tempAngle);
					//float tempCos = cos(tempAngle);
					//float speedx1 = m_pRigidBall[m_crashObj0].m_speed.x;
					//float speedy1 = m_pRigidBall[m_crashObj0].m_speed.y;
					//float dx1 = speedx1 * tempCos + speedy1 * tempSin;
					//float dy1 = -speedy1 * tempCos + speedx1 * tempSin;
					////再次进行坐标系旋转
					//tempSin = sin(-tempAngle);
					//tempCos = cos(-tempAngle);
					//speedx1 = dx1 * tempCos + dy1 * tempSin;
					//speedy1 = dy1 * tempCos - dx1 * tempSin;
					////还原值
					//m_pRigidBall[m_crashObj0].m_speed.x = speedx1 * CRASH_SPEED_DOWN;
					//m_pRigidBall[m_crashObj0].m_speed.y = speedy1 * CRASH_SPEED_DOWN;
					//下面对算法进行优化
					float tempAngle = m_pCrashData[m_crashOrder].m_crashAngle;//碰撞角度
					float tempSin = sin(2.0f * tempAngle);
					float tempCos = cos(2.0f * tempAngle);
					float speedx1 = m_pRigidBall[m_crashObj0].m_speed.y * tempSin + m_pRigidBall[m_crashObj0].m_speed.x * tempCos;
					float speedy1 = -m_pRigidBall[m_crashObj0].m_speed.y * tempCos + m_pRigidBall[m_crashObj0].m_speed.x * tempSin;
					//还原值
					m_pRigidBall[m_crashObj0].m_speed.x = speedx1 * CRASH_SPEED_DOWN;
					m_pRigidBall[m_crashObj0].m_speed.y = speedy1 * CRASH_SPEED_DOWN;
				}else
				if((m_crashObj1 - m_liquidBallSum) % 3 == 1)
				{//线段起点
					//float tempAngle = m_pRigidBall[m_crashObj0].m_positionS.getAngle(m_pLine[(m_crashObj1 - m_liquidBallSum) / 3].getStartPosition());//碰撞角度
					//float tempSin = sin(tempAngle);
					//float tempCos = cos(tempAngle);
					//float speedx1 = m_pRigidBall[m_crashObj0].m_speed.x;
					//float speedy1 = m_pRigidBall[m_crashObj0].m_speed.y;
					//float dx1 = -speedx1 * tempCos - speedy1 * tempSin;
					//float dy1 = speedy1 * tempCos - speedx1 * tempSin;
					////再次进行坐标系旋转
					//tempSin = sin(-tempAngle);
					//tempCos = cos(-tempAngle);
					//speedx1 = dx1 * tempCos + dy1 * tempSin;
					//speedy1 = dy1 * tempCos - dx1 * tempSin;
					////还原值
					//m_pRigidBall[m_crashObj0].m_speed.x = speedx1 * CRASH_SPEED_DOWN;
					//m_pRigidBall[m_crashObj0].m_speed.y = speedy1 * CRASH_SPEED_DOWN;
					//对上面的算法进行化简
					float tempAngle = m_pCrashData[m_crashOrder].m_crashAngle;//碰撞角度
					float tempSin = sin(2.0f * tempAngle);
					float tempCos = cos(2.0f * tempAngle);
					float speedx1 = -m_pRigidBall[m_crashObj0].m_speed.y * tempSin - m_pRigidBall[m_crashObj0].m_speed.x * tempCos;
					float speedy1 = m_pRigidBall[m_crashObj0].m_speed.y * tempCos - m_pRigidBall[m_crashObj0].m_speed.x * tempSin;
					m_pRigidBall[m_crashObj0].m_speed.x = speedx1 * CRASH_SPEED_DOWN;
					m_pRigidBall[m_crashObj0].m_speed.y = speedy1 * CRASH_SPEED_DOWN;
				}else
				if((m_crashObj1 - m_liquidBallSum) % 3 == 2)
				{//线段终点
					//float tempAngle = m_pRigidBall[m_crashObj0].m_positionS.getAngle(m_pLine[(m_crashObj1 - m_liquidBallSum) / 3].getEndPosition());//碰撞角度
					//float tempSin = sin(tempAngle);
					//float tempCos = cos(tempAngle);
					//float speedx1 = m_pRigidBall[m_crashObj0].m_speed.x;
					//float speedy1 = m_pRigidBall[m_crashObj0].m_speed.y;
					//float dx1 = -speedx1 * tempCos - speedy1 * tempSin;
					//float dy1 = speedy1 * tempCos - speedx1 * tempSin;
					////再次进行坐标系旋转
					//tempSin = sin(-tempAngle);
					//tempCos = cos(-tempAngle);
					//speedx1 = dx1 * tempCos + dy1 * tempSin;
					//speedy1 = dy1 * tempCos - dx1 * tempSin;
					////还原值
					//m_pRigidBall[m_crashObj0].m_speed.x = speedx1 * CRASH_SPEED_DOWN;
					//m_pRigidBall[m_crashObj0].m_speed.y = speedy1 * CRASH_SPEED_DOWN;
					//对上面的算法进行化简
					float tempAngle = m_pCrashData[m_crashOrder].m_crashAngle;//碰撞角度
					float tempSin = sin(2.0f * tempAngle);
					float tempCos = cos(2.0f * tempAngle);
					float speedx1 = -m_pRigidBall[m_crashObj0].m_speed.y * tempSin - m_pRigidBall[m_crashObj0].m_speed.x * tempCos;
					float speedy1 = m_pRigidBall[m_crashObj0].m_speed.y * tempCos - m_pRigidBall[m_crashObj0].m_speed.x * tempSin;
					m_pRigidBall[m_crashObj0].m_speed.x = speedx1 * CRASH_SPEED_DOWN;
					m_pRigidBall[m_crashObj0].m_speed.y = speedy1 * CRASH_SPEED_DOWN;
				}
			}
		}
//		printf("Crash time:%f,dx:%f,dy:%f,px:%f,py:%f,r:%f\n",m_minTimer,m_pRigidBall[m_crashObj0].m_speed.x,m_pRigidBall[m_crashObj0].m_speed.y,
//			m_pRigidBall[m_crashObj0].m_position.x,m_pRigidBall[m_crashObj0].m_position.y,m_pRigidBall[m_crashObj0].m_radius);
		if(timeDelay - m_minTimer <= 0) return 1;
		return crashInsideProc(timeDelay - m_minTimer,m_minTimer);
	}else
	{
		for(int i = 0;i < m_liquidBallSum;++ i)
		{
			
			if(m_pRigidBall[i].getIsEnable() != 0)
			{
				m_pRigidBall[i].m_positionS.set(m_pRigidBall[i].m_position.x + m_pRigidBall[i].m_speed.x * timeDelay * 0.001f,
					m_pRigidBall[i].m_position.y + m_pRigidBall[i].m_speed.y * timeDelay * 0.001f + 0.5f * gravity * (timeDelay * 0.001f) * (timeDelay * 0.001f));
				m_pRigidBall[i].m_position = m_pRigidBall[i].m_positionS;
				//计算空气阻力
				//计算重力
				{
					m_pRigidBall[i].m_speed.y += gravity * timeDelay * 0.001f;
				}
			}
		}
		//if(checkProc() != 1)
		//{//如果检查数据发现问题，下面进行数据还原
		//	for(int i = 0;i < m_liquidBallSum;++ i)
		//	{
		//		if(m_pRigidBall[i].getIsEnable() != 0)
		//		{
		//			//计算空气阻力
		//			{
		//				//float dx = abs(m_pRigidBall[i].m_speed.x * timeDelay * 0.001f);
		//				//float tempDx = m_pRigidBall[i].m_speed.x * m_pRigidBall[i].m_speed.x - abs(2.0f * m_pRigidBall[i].m_airResistance * m_pRigidBall[i].m_speed.x * dx) / (m_pRigidBall[i].m_mass * 0.001f);
		//				//if(tempDx <= 0) 
		//				//{
		//				//	m_pRigidBall[i].m_speed.x = 0.0f;
		//				//}else
		//				//{
		//				//	if(m_pRigidBall[i].m_speed.x < 0) m_pRigidBall[i].m_speed.x = -sqrt(tempDx);
		//				//	else m_pRigidBall[i].m_speed.x = sqrt(tempDx);
		//				//}
		//				//float dy = abs(m_pRigidBall[i].m_speed.y * timeDelay * 0.001f);
		//				//float tempDy = m_pRigidBall[i].m_speed.y * m_pRigidBall[i].m_speed.y - abs(2.0f * m_pRigidBall[i].m_airResistance * m_pRigidBall[i].m_speed.y * dy) / (m_pRigidBall[i].m_mass * 0.001f);
		//				//if(tempDy < 0) m_pRigidBall[i].m_speed.y = 0.0f;
		//				//else
		//				//{
		//				//	if(m_pRigidBall[i].m_speed.y < 0) m_pRigidBall[i].m_speed.y = -sqrt(tempDy);
		//				//	else m_pRigidBall[i].m_speed.y = sqrt(tempDy);
		//				//}
		//			}
		//			//计算重力
		//			{
		//				m_pRigidBall[i].m_speed.y -= gravity * timeDelay * 0.001f;
		//				//float dy = abs(m_pRigidBall[i].m_speed.y * timeDelay * 0.001f);
		//				//float tempDy = m_pRigidBall[i].m_speed.y * m_pRigidBall[i].m_speed.y - 2.0f * gravity * dy;
		//				//if(tempDy < 0) m_pRigidBall[i].m_speed.y = 0.0f;
		//				//else
		//				//{
		//				//	if(m_pRigidBall[i].m_speed.y < 0) m_pRigidBall[i].m_speed.y = -sqrt(tempDy);
		//				//	else m_pRigidBall[i].m_speed.y = sqrt(tempDy);
		//				//}
		//			}
		//			m_pRigidBall[i].m_positionS.set(m_pRigidBall[i].m_position.x - m_pRigidBall[i].m_speed.x * timeDelay * 0.001f,
		//				m_pRigidBall[i].m_position.y - m_pRigidBall[i].m_speed.y * timeDelay * 0.001f - 0.5f * gravity * (timeDelay * 0.001f) * (timeDelay * 0.001f));
		//			m_pRigidBall[i].m_position = m_pRigidBall[i].m_positionS;
		//		}
		//	}
		//	//重新演示计算过程
		//	float m_minTimer = -1.0f;
		//	crashTimerProc(timeDelay,upTime);
		//}
		return 1;	//完成计算
	}
}

int _XLiquidBalls::addOneBall(const _XVector2& position,float radius,float mass,_XVector2 speed,float airResistance,float adhereRate,float adhereRadius)	//增加一个例子，返回增加是否成功
{
	if(m_isInited == 0) return 0;
	if(m_isEnable == 0) return 0;
	if(radius < 4.0f) radius = 4.0f;
	for(int i = 0;i < m_liquidBallSum;++ i)
	{
		if(m_pRigidBall[i].getIsEnable() == 0)
		{
			m_pRigidBall[i].init(position,radius,mass,speed,airResistance,adhereRate,adhereRadius);
			m_pRigidBall[i].setEnable(1);
			return 1;
		}
	}
	return 0;
}

int _XLiquidBalls::addOneLine(const _XVector2& startPosition,_XVector2 endPosition)
{
	if(m_isInited == 0) return 0;
	if(m_isEnable == 0) return 0;
	for(int i = 0;i < m_lineSum;++ i)
	{
		if(m_pLine[i].getIsEnable() == 0)
		{
			m_pLine[i].set(startPosition,endPosition);
			m_pLine[i].setEnable(1);
			return 1;
		}
	}
	return 0;
}

_XLiquidBalls::_XLiquidBalls()
:m_isEnable(0)
,m_isInited(0)
,m_liquidBallSum(0)
//,m_pRigidBall(NULL)
//,m_pLiquidBallData(NULL)
,m_pCrashData(NULL)
{
}
