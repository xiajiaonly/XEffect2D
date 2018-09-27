#ifndef _JIA_XFILTERVEC2_
#define _JIA_XFILTERVEC2_
//++++++++++++++++++++++++++++++++
//Author:	¼ÖÊ¤»ª(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.3
//--------------------------------
#include "XKalman.h"
#include "XVector3.h"
namespace XE{
class XFilterVec3 :public XKalmanFilter
{
public:
	virtual double getQ() const { return m_fx.getQ(); }
	virtual void setQ(double q) { m_fx.setQ(q); m_fy.setQ(q); m_fz.setQ(q); }
	virtual double getR() const { return m_fx.getR(); }
	virtual void setR(double r) { m_fx.setR(r); m_fy.setR(r); m_fz.setR(r); }
	XVec3 filter(XVec3 data) { return XVec3(m_fx.filter(data.x), m_fy.filter(data.y), m_fz.reset(data.z)); }
	XVec3 reset(XVec3 data) { return XVec3(m_fx.reset(data.x), m_fy.reset(data.y), m_fz.reset(data.z)); }
	virtual void init(double Q = 0.03f, double R = 1.0f)	//£¬double p
	{
		m_fx.init(Q, R);
		m_fy.init(Q, R);
	}
	virtual ~XFilterVec3() {}
private:
	XKalmanFilter m_fx;
	XKalmanFilter m_fy;
	XKalmanFilter m_fz;
};
}
#endif