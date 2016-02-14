#ifndef _JIA_XFILTERVEC2_
#define _JIA_XFILTERVEC2_
//++++++++++++++++++++++++++++++++
//Author:	¼ÖÊ¤»ª(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.3
//--------------------------------
#include "XKalman.h"
#include "XVector2.h"
namespace XE{
class XFilterVec2:public XKalmanFilter
{
public:
	virtual double getQ() const{return m_fx.getQ();}
	virtual void setQ(double q){m_fx.setQ(q);m_fy.setQ(q);}
	virtual double getR() const{return m_fx.getR();}
	virtual void setR(double r){m_fx.setR(r);m_fy.setR(r);}
	XVector2 filter(XVector2 data){return XVector2(m_fx.filter(data.x),m_fy.filter(data.y));}
	virtual void init(double Q,double R)	//£¬double p
	{
		m_fx.init(Q,R);
		m_fy.init(Q,R);
	}
private:
	XKalmanFilter m_fx;
	XKalmanFilter m_fy;
};
}
#endif