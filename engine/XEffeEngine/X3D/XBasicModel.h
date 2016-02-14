#ifndef _JIA_XBASICMODEL_
#define _JIA_XBASICMODEL_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.10.1
//--------------------------------
#include "X3DWorld.h"
namespace XE{
//这是一个基础模型的类，就是简单的顶点和引用的类，完善之后加入V N C T
class XBasicModel
{
private:
	bool m_isInited;	//是否已经初始化
	int m_vSum;	//顶点数量
	float *m_v;	//顶点数据
	float *m_c;	//顏色

	int m_iSum;	//引用数量
	unsigned int *m_i;	//引用数据
	XVector3 m_position;	//位置
	XVector3 m_scale;		//尺寸
	XVector3 m_angle;		//角度

	XVBO m_vbo;
public:
	//iSum为索引数量，索引数量为绘图元素数量 * 每个元素需要的顶点数
	bool init(int vSum,float *v,float *c,int iSum,int *i)
	{
		if(m_isInited) return false;
		if(vSum <= 0 || iSum <= 0) return false;
		m_vSum = vSum;
		m_iSum = iSum;

		m_v = XMem::createArrayMem<float>(m_vSum * 3);
		memcpy(m_v,v,m_vSum * 3 * sizeof(float));	//每个顶点有三个数据
		m_c = XMem::createArrayMem<float>(m_vSum * 4);
		memcpy(m_c,c,m_vSum * 4 * sizeof(float));	//每个顶点有三个数据
		m_i = XMem::createArrayMem<unsigned int>(m_iSum * 3);
		memcpy(m_i,i,m_iSum * sizeof(int));

		m_vbo.init(m_vSum,m_v,NULL,NULL,m_c,m_iSum,m_i);

		m_isInited = true;
		return true;
	}
	void draw();
	void setPosition(float x,float y,float z) {m_position.set(x,y,z);}
	void setPosition(const XVector3 &pos) {m_position = pos;}
	void setScale(float x,float y,float z) {m_scale.set(x,y,z);}
	void setScale(const XVector3 &scale) {m_scale = scale;}
	void setAngle(float x,float y,float z) {m_angle.set(x,y,z);}
	void setAngle(const XVector3 &angle) {m_angle = angle;}

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
		memcpy(m_i,i,m_iSum * sizeof(int));
		m_vbo.updateDataI(m_iSum,m_i);
	}
	void release()
	{
		if(!m_isInited) return;
		XMem::XDELETE_ARRAY(m_v);
		XMem::XDELETE_ARRAY(m_i);
		m_isInited = false;
	}
	XBasicModel()
		:m_isInited(false)
		,m_vSum(0)
		,m_v(NULL)
		,m_iSum(0)
		,m_i(NULL)
		,m_position(0.0f,0.0f,0.0f)
		,m_scale(1.0f,1.0f,1.0f)
		,m_angle(0.0f,0.0f,0.0f)
	{}
	~XBasicModel() {release();}
};
}
#endif