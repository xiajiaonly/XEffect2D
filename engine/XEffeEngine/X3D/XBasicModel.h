#ifndef _JIA_XBASICMODEL_
#define _JIA_XBASICMODEL_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.10.1
//--------------------------------
#include "X3DWorld.h"
//这是一个基础模型的类，就是简单的顶点和引用的类，完善之后加入V N C T
class _XBasicModel
{
private:
	bool m_isInited;	//是否已经初始化
	int m_vSum;	//顶点数量
	float *m_v;	//顶点数据
	float *m_c;	//顏色

	int m_iSum;	//引用数量
	unsigned int *m_i;	//引用数据
	_XVector3 m_position;	//位置
	_XVector3 m_size;		//尺寸
	_XVector3 m_angle;		//角度

	_XVBO m_vbo;
public:
	bool init(int vSum,float *v,float *c,int iSum,int *i)
	{
		if(m_isInited) return false;
		if(vSum <= 0 || iSum <= 0) return false;
		m_vSum = vSum;
		m_iSum = iSum;

		m_v = createArrayMem<float>(m_vSum * 3);
		memcpy(m_v,v,m_vSum * 3 * sizeof(float));	//每个顶点有三个数据
		m_c = createArrayMem<float>(m_vSum * 4);
		memcpy(m_c,c,m_vSum * 4 * sizeof(float));	//每个顶点有三个数据
		m_i = createArrayMem<unsigned int>(m_iSum * 3);
		memcpy(m_i,i,m_iSum * 3 * sizeof(float));	//每个三角面有三个顶点

		m_vbo.init(m_vSum,m_v,NULL,NULL,m_c,m_iSum,m_i);

		m_isInited = true;
		return true;
	}
	void draw();
	void setPosition(float x,float y,float z) {m_position.set(x,y,z);}
	void setPosition(const _XVector3 &pos) {m_position = pos;}
	void setSize(float x,float y,float z) {m_size.set(x,y,z);}
	void setSize(const _XVector3 &size) {m_size = size;}
	void setAngle(float x,float y,float z) {m_angle.set(x,y,z);}
	void setAngle(const _XVector3 &angle) {m_angle = angle;}

	void updateV(float *v)	//更新顶点数据
	{
		if(!m_isInited) return;
		memcpy(m_v,v,m_vSum * 3 * sizeof(float));
		m_vbo.updateDataV(m_vSum,m_v);
	}
	void updateC(float *c)	//更新顶点数据
	{
		if(!m_isInited) return;
		memcpy(m_c,c,m_vSum * 4 * sizeof(float));
		m_vbo.updateDataC(m_vSum,m_c);
	}
	void updateI(int *i)	//更新引用数据
	{
		if(!m_isInited) return;
		memcpy(m_i,i,m_iSum * 3 * sizeof(float));
		m_vbo.updateDataI(m_iSum,m_i);
	}
	void release()
	{
		if(!m_isInited) return;
		XDELETE_ARRAY(m_v);
		XDELETE_ARRAY(m_i);
		m_isInited = false;
	}
	_XBasicModel()
		:m_isInited(false)
		,m_vSum(0)
		,m_v(NULL)
		,m_iSum(0)
		,m_i(NULL)
		,m_position(0.0f,0.0f,0.0f)
		,m_size(1.0f,1.0f,1.0f)
		,m_angle(0.0f,0.0f,0.0f)
	{
	}
	~_XBasicModel() {release();}
};
#endif