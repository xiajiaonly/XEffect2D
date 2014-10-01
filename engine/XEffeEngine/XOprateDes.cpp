#include "XOprateDes.h"
void _XOperateManager::clearAll()
{
	m_nowStateIndex = -1;
	m_operateIndex.clear();
	for(int i = 0;i < m_operateDes.size();++ i)
	{
		XDELETE(m_operateDes[i].pData);
	}
	m_operateDes.clear();
}
void _XOperateManager::undo()
{
	if(!canUndo()) return;	//如果不能进行撤销操作，则直接返回
	for(int i = m_operateIndex[m_nowStateIndex];
		i > m_operateIndex[m_nowStateIndex - 1];-- i)
	{//便利所有发生状态变化的物件，将这些物件的状态设置会上一个有效的状态
		for(int j = i - 1;j >= 0;-- j)
		{
			if(m_operateDes[i].pObj == m_operateDes[j].pObj)
			{
				m_operateDes[j].pObj->setOprateState(m_operateDes[j].pData);
				break;
			}
		}
	}
	m_nowStateIndex = m_nowStateIndex - 1;	//标记撤销到上一个动作
	//printf("当前操作!%d\n",m_nowStateIndex);
}
void _XOperateManager::redo()
{
	if(!canRedo()) return;	//如果不能进行重做操作，则直接返回
	for(int i = m_operateIndex[m_nowStateIndex] + 1;
		i <= m_operateIndex[m_nowStateIndex + 1];++ i)
	{//遍历所有的发生状态变化的物件将这些物件的状态设置为当前状态
		m_operateDes[i].pObj->setOprateState(m_operateDes[i].pData);
	}
	m_nowStateIndex = m_nowStateIndex + 1;
	//printf("当前操作!%d\n",m_nowStateIndex);
}
void _XOperateManager::addAOperate(_XBasicOprate *obj)
{//添加一个动作
	if(obj == NULL) return;
	//如果当前状态时经过"撤销"操作的，也就是当前状态不是最新的状态时，将不需要的状态描述和动作索引丢弃
	if(m_nowStateIndex >= 0 && m_nowStateIndex != m_operateIndex.size() - 1)
	{
		int size = m_operateIndex.size() - 1 - m_nowStateIndex;
		for(int i = 0;i < size;++ i)
			m_operateIndex.pop_back();
		if(m_operateIndex[m_nowStateIndex] != m_operateDes.size() - 1)
		{
			size = m_operateDes.size() - 1 - m_operateIndex[m_nowStateIndex];
			for(int i = 0;i < size;++ i)
			{
				XDELETE(m_operateDes[m_operateDes.size() - 1].pData);	//删除动作描述的数据
				m_operateDes.pop_back();
			}
		}
	}
	//如果当前物件的状态与上一个描述重复，则不再重复描述
	for(int i = m_operateDes.size() - 1;i >= 0;-- i)
	{
		if(m_operateDes[i].pObj == obj)
		{
			 if(obj->isSameState(m_operateDes[i].pData))
			 {
				 //printf("添加重复数据,丢弃\n");
				 return;
			 }
			 break;
		}
	}
	//将合理的状态描述推入到状态描述队列中
	_XOperateDes tmp;
	tmp.pObj = obj;
	tmp.pData = obj->getOprateState();
	m_operateDes.push_back(tmp);
	//printf("状态!%d\n",m_operateDes.size());
}
void _XOperateManager::setOperateOver()
{//设置动作完成
	//如果队列中并没有新的动作描述，则直接返回
	if(m_operateDes.size() <= 0 || (m_operateIndex.size() > 0 && 
		m_operateDes.size() - 1 <= m_operateIndex[m_operateIndex.size() - 1])) return;
//	if(!((m_nowStateIndex < 0 && m_operateDes.size() > 0)
//		|| (m_operateIndex.size() > 0 && m_operateDes.size() - 1 > m_operateIndex[m_operateIndex.size() - 1]))) return;
	//这里需要对重复的状态进行整体和剔除,(在一个动作周期类同一个物件存在多个动作描述，这个是没有必要的，只需要保留最后一个状态即可)
	if(m_nowStateIndex < 0)
	{//第一次
		for(int i = 0;i < m_operateDes.size() - 1;++ i)
		{
			for(int j = i + 1;j < m_operateDes.size();++ j)
			{
				if(m_operateDes[i].pObj == m_operateDes[j].pObj)
				{//一个动作周期内存在同一个物件的多个状态描述，只保留最后一个
					_XOperateDes tmp = m_operateDes[i];
					m_operateDes[i] = m_operateDes[j];
					for(int k = j;k < m_operateDes.size() - 1;++ k)
					{
						m_operateDes[k] = m_operateDes[k + 1];
					}
					m_operateDes.pop_back();
					XDELETE(tmp.pData);
					-- j;
					//printf("发生剔除!\n");
				}
			}
		}
	}else
	{//不是第一次
		for(int i = m_operateIndex[m_nowStateIndex] + 1;i < m_operateDes.size();++ i)
		{
			for(int j = i + 1;j < m_operateDes.size();++ j)
			{
				if(m_operateDes[i].pObj == m_operateDes[j].pObj)
				{//一个动作周期内存在同一个物件的多个状态描述，只保留最后一个
					_XOperateDes tmp = m_operateDes[i];
					m_operateDes[i] = m_operateDes[j];
					for(int k = j;k < m_operateDes.size() - 1;++ k)
					{
						m_operateDes[k] = m_operateDes[k + 1];
					}
					m_operateDes.pop_back();
					XDELETE(tmp.pData);
					-- j;
					//printf("发生剔除!\n");
				}
			}
		}
	}
	//记录新的动作加入
	++m_nowStateIndex;
	m_operateIndex.push_back(m_operateDes.size() - 1);
	//printf("操作!%d\n",m_operateIndex.size()); 
}
void _XOperateManager::decreaseObj(_XBasicOprate *obj)
{//(初步测试没有问题)
	if(obj == NULL) return;
	for(int i = m_operateDes.size() - 1;i >= 0;-- i)
	{
		if(m_operateDes[i].pObj == obj)
		{//需要进行整理
			//丢弃这个值
			XDELETE(m_operateDes[i].pData);
			for(int j = i;j < m_operateDes.size() - 1;++ j)
			{
				m_operateDes[j] = m_operateDes[j + 1];
			}
			m_operateDes.pop_back();
			//整理后面的数据
			for(int j = 0;j < m_operateIndex.size();++ j)
			{//二级索引递减，这里会造成重复
				if(m_operateIndex[j] >= i) --m_operateIndex[j];
			}
			//这里剔除重复
			for(int j = 0;j < m_operateIndex.size();++ j)
			{
				if((j < m_operateIndex.size() - 1 && m_operateIndex[j] == m_operateIndex[j + 1]) || 
					m_operateIndex[j] < 0)
				{//发现重复，则丢弃后面一个数据
					if(m_nowStateIndex > j) -- m_nowStateIndex;
					for(int k = j + 1;k < m_operateIndex.size() - 1;++ k)
					{
						m_operateIndex[k] = m_operateIndex[k + 1];
					}
					m_operateIndex.pop_back();
				}
			}
			if(m_operateIndex.size() <= 0) m_nowStateIndex = -1;
		}
	}
}