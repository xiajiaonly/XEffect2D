#include "XStdHead.h"
#include "XOprateDes.h"
namespace XE{
void XOperateDes::release()
{//这里可能存在问题，当pObj的实体被释放时，这里会造成crash。
	if(pObj == NULL) return;
	pObj->releaseOprateStateData(pData);
	pData = NULL;
}
void XOperateManager::clearAll()
{
	m_curStateIndex = -1;
	m_operateIndex.clear();
	for(unsigned int i = 0;i < m_operateDes.size();++ i)
	{
		m_operateDes[i].release();
		//XMem::XDELETE(m_operateDes[i].pData);
	}
	m_operateDes.clear();
}
void XOperateManager::undo()
{
	if(!canUndo()) return;	//如果不能进行撤销操作，则直接返回
	for(int i = m_operateIndex[m_curStateIndex];
		i > m_operateIndex[m_curStateIndex - 1];-- i)
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
	m_curStateIndex = m_curStateIndex - 1;	//标记撤销到上一个动作
	//printf("当前操作!%d\n",m_curStateIndex);
}
void XOperateManager::redo()
{
	if(!canRedo()) return;	//如果不能进行重做操作，则直接返回
	for(int i = m_operateIndex[m_curStateIndex] + 1;
		i <= m_operateIndex[m_curStateIndex + 1];++ i)
	{//遍历所有的发生状态变化的物件将这些物件的状态设置为当前状态
		m_operateDes[i].pObj->setOprateState(m_operateDes[i].pData);
	}
	m_curStateIndex = m_curStateIndex + 1;
	//printf("当前操作!%d\n",m_curStateIndex);
}
void XOperateManager::addAOperate(XBasicOprate *obj)
{//添加一个动作
	if(obj == NULL) return;
	//如果当前状态时经过"撤销"操作的，也就是当前状态不是最新的状态时，将不需要的状态描述和动作索引丢弃
	if(m_curStateIndex >= 0 && m_curStateIndex != (int)(m_operateIndex.size()) - 1)
	{
		int size = (int)(m_operateIndex.size()) - 1 - m_curStateIndex;
		for(int i = 0;i < size;++ i)
			m_operateIndex.pop_back();
		//if(size > 0)
		//	m_operateIndex.erase(m_operateIndex.begin() + m_operateIndex.size() - size,m_operateIndex.end());
		if(m_operateIndex[m_curStateIndex] != (int)(m_operateDes.size()) - 1)
		{
			size = (int)(m_operateDes.size()) - 1 - m_operateIndex[m_curStateIndex];
			for(int i = 0;i < size;++ i)
			{
				m_operateDes[(int)(m_operateDes.size()) - 1].release();
				//XMem::XDELETE(m_operateDes[(int)(m_operateDes.size()) - 1].pData);	//删除动作描述的数据
				m_operateDes.pop_back();
			}
		}
	}
	//如果当前物件的状态与上一个描述重复，则不再重复描述
	for(int i = (int)(m_operateDes.size()) - 1;i >= 0;-- i)
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
	XOperateDes tmp;
	tmp.pObj = obj;
	tmp.pData = obj->getOprateState();
	m_operateDes.push_back(tmp);
	//printf("状态!%d\n",m_operateDes.size());
}
void XOperateManager::setOperateOver()
{//设置动作完成
	//如果队列中并没有新的动作描述，则直接返回
	if(m_operateDes.size() <= 0 || (m_operateIndex.size() > 0 && 
		(int)(m_operateDes.size()) - 1 <= m_operateIndex[(int)(m_operateIndex.size()) - 1])) return;
//	if(!((m_curStateIndex < 0 && m_operateDes.size() > 0)
//		|| (m_operateIndex.size() > 0 && (int)(m_operateDes.size()) - 1 > m_operateIndex[(int)(m_operateIndex.size()) - 1]))) return;
	//这里需要对重复的状态进行整体和剔除,(在一个动作周期类同一个物件存在多个动作描述，这个是没有必要的，只需要保留最后一个状态即可)
	if(m_curStateIndex < 0)
	{//第一次
		for(int i = 0;i < (int)(m_operateDes.size()) - 1;++ i)
		{
			for(int j = i + 1;j < m_operateDes.size();++ j)
			{
				if(m_operateDes[i].pObj == m_operateDes[j].pObj)
				{//一个动作周期内存在同一个物件的多个状态描述，只保留最后一个
					XOperateDes tmp = m_operateDes[i];
					m_operateDes[i] = m_operateDes[j];
				//	for(int k = j;k < (int)(m_operateDes.size()) - 1;++ k)
				//	{
				//		m_operateDes[k] = m_operateDes[k + 1];
				//	}
				//	m_operateDes.pop_back();
					m_operateDes.erase(m_operateDes.begin() + j);
					//XMem::XDELETE(tmp.pData);
					tmp.release();
					-- j;
					//printf("发生剔除!\n");
				}
			}
		}
	}else
	{//不是第一次
		for(int i = m_operateIndex[m_curStateIndex] + 1;i < m_operateDes.size();++ i)
		{
			for(int j = i + 1;j < m_operateDes.size();++ j)
			{
				if(m_operateDes[i].pObj == m_operateDes[j].pObj)
				{//一个动作周期内存在同一个物件的多个状态描述，只保留最后一个
					XOperateDes tmp = m_operateDes[i];
					m_operateDes[i] = m_operateDes[j];
				//	for(int k = j;k < (int)(m_operateDes.size()) - 1;++ k)
				//	{
				//		m_operateDes[k] = m_operateDes[k + 1];
				//	}
				//	m_operateDes.pop_back();
					m_operateDes.erase(m_operateDes.begin() + j);
					//XMem::XDELETE(tmp.pData);
					tmp.release();
					-- j;
					//printf("发生剔除!\n");
				}
			}
		}
	}
	//记录新的动作加入
	++m_curStateIndex;
	m_operateIndex.push_back((int)(m_operateDes.size()) - 1);
	//printf("操作!%d\n",m_operateIndex.size()); 
}
void XOperateManager::decreaseObj(XBasicOprate *obj)
{//(初步测试没有问题)
	if(obj == NULL) return;
	for(int i = (int)(m_operateDes.size()) - 1;i >= 0;-- i)
	{
		if(m_operateDes[i].pObj == obj)
		{//需要进行整理
			//丢弃这个值
			m_operateDes[i].release();
			//XMem::XDELETE(m_operateDes[i].pData);
		//	for(int j = i;j < (int)(m_operateDes.size()) - 1;++ j)
		//	{
		//		m_operateDes[j] = m_operateDes[j + 1];
		//	}
		//	m_operateDes.pop_back();
			m_operateDes.erase(m_operateDes.begin() + i);
			//整理后面的数据
			for(unsigned int j = 0;j < m_operateIndex.size();++ j)
			{//二级索引递减，这里会造成重复
				if(m_operateIndex[j] >= i) --m_operateIndex[j];
			}
			//这里剔除重复
			for(unsigned int j = 0;j < m_operateIndex.size();++ j)
			{
				if((j < (int)(m_operateIndex.size()) - 1 && m_operateIndex[j] == m_operateIndex[j + 1]) || 
					m_operateIndex[j] < 0)
				{//发现重复，则丢弃后面一个数据
					if(m_curStateIndex > j) -- m_curStateIndex;
				//	for(int k = j + 1;k < (int)(m_operateIndex.size()) - 1;++ k)
				//	{
				//		m_operateIndex[k] = m_operateIndex[k + 1];
				//	}
				//	m_operateIndex.pop_back();
					m_operateIndex.erase(m_operateIndex.begin() + j + 1);
				}
			}
			if(m_operateIndex.size() <= 0) m_curStateIndex = -1;
		}
	}
}
#if !WITH_INLINE_FILE
#include "XOprateDes.inl"
#endif
}