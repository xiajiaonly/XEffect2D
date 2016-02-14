#include "XStdHead.h"
#include "XStageManager.h"
namespace XE{
XVector3 XStageInfo::screenToGround(float x,float y)const
{
	XVector3 ret;
	ret.y = 0.0f;
	//ret.z = XMath::maping1DEx(getSceneHeight() - y,0.0f,m_depthToUpRate,m_near,m_far) + m_cameraPosition.z;
	ret.z = (getSceneHeight() - y + m_cameraPosition.y) / (m_depthToUpRate - m_cameraPosition.y * (m_scaleRate - 1.0f)) * (m_far - m_near) + m_near + m_cameraPosition.z;
//	tmpz = ((ret.z - m_cameraPosition.z) - m_near) / (m_far - m_near)
//	tmpz * m_depthToUpRate - cy * (tmpz * (m_scaleRate - 1.0f) + 1.0) = getSceneHeight() - y
//	tmpz * (m_depthToUpRate - cy * (m_scaleRate - 1.0f)) = getSceneHeight() - y + cy
//	(in - imin) / (imax - imin) * (omax - omin) + omin;
	float scale = XMath::maping1DEx((ret.z - m_cameraPosition.z),m_near,m_far,1.0f,m_scaleRate);
	if(scale <= 0.0f) ret.x = 0.0f;
	else
		ret.x = (x - getSceneWidth() * 0.5f) / scale + m_cameraPosition.x;
	return ret;
}
XVector2 XStageInfo::worldToScreen(float x,float y,float z)const//世界坐标转换成屏幕坐标
{
	XVector2 ret;
	float scale = XMath::maping1DEx(z - m_cameraPosition.z,m_near,m_far,1.0f,m_scaleRate);
	ret.x =	getSceneWidth() * 0.5f + (x - m_cameraPosition.x) * scale;
	ret.y = (y - m_cameraPosition.y) * scale + XMath::maping1DEx(z - m_cameraPosition.z,m_near,m_far,0.0f,m_depthToUpRate);
	ret.y = getSceneHeight() - ret.y;
	return ret;
}
void XStageManager::sortObject()
{
	qSort(0,m_objects.size() - 1);
	XVector2 screenPos;
	XVector2 screenScale;
	for(int i = 0;i < m_objects.size();++ i)
	{
		screenScale = m_info.getCameraScale(m_objects[i]->m_position) * m_objects[i]->m_scale;
		screenPos = m_info.worldToScreen(m_objects[i]->m_position);
		screenPos -= m_objects[i]->m_originOffset * screenScale;
		m_objects[i]->setPosition(screenPos);
		m_objects[i]->setScale(screenScale);
	}
}
void XStageManager::draw()
{
	if(!m_isInited) return;
	sortObject();//对所有物件进行排序，并计算其渲染参数
	for(int i = m_objects.size() - 1;i >= 0;-- i)//一次渲染所有的物件
	{
		m_objects[i]->draw();
	}
}
void XStageManager::drawFloor(int size)
{
	if(!m_isInited) return;
	for(int i = 0;i <= size;i += 32)
	{
		XRender::drawLine(worldToScreen(i,0,m_info.m_near),worldToScreen(i,0,m_info.m_far),1,1,0,0,1);
		XRender::drawLine(worldToScreen(-i,0,m_info.m_near),worldToScreen(-i,0,m_info.m_far),1,1,0,0,1);
	}
	for(int i = m_info.m_near;i <= m_info.m_far;i += 32)
	{
		XRender::drawLine(worldToScreen(size,0,i),worldToScreen(-size,0,i),1,1,0,0,1);
	}
}
void XStageManager::update(float stepTime)
{
	if(!m_isInited) return;
	for(int i = m_objects.size() - 1;i >= 0;-- i)//一次渲染所有的物件
	{
		m_objects[i]->update(stepTime);
	}
}
bool XStageManager::addAObject(XStageObject *obj)	//向场景中增加一个物件，返回添加是否成功
{
	if(obj == NULL) return false;
	if(haveObject(obj)) return true;
	//这里先不排序
	m_objects.push_back(obj);
	return true;
}
bool XStageManager::removeAObject(XStageObject *obj)	//从场景中移除一个物件
{
	if(obj == NULL) return false;
	for(int i = 0;i < m_objects.size();++ i)
	{
		if(m_objects[i] == obj)
		{
			m_objects.erase(m_objects.begin() + i);
			return true;
		}
	}
	return false;
}
bool XStageManager::haveObject(const XStageObject *obj)	//获取场景中指定物体的编号，如果不存在则返回-1
{
	if(obj == NULL) return false;
	for(int i = 0;i < m_objects.size();++ i)
	{
		if(m_objects[i] == obj) return true;
	}
	return false;
}
}