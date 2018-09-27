INLINE void XSimpleBezierSpline::init(const XVec2& pStart,const XVec2& pSP,
	const XVec2& pEP,const XVec2& pEnd, bool withOpti)
{
	m_pS = pStart;
	m_pSP = pSP;
	m_pEP = pEP;
	m_pE = pEnd;
	m_withOpti = withOpti;
}
INLINE void XSimpleBezierSpline::init(float pSx,float pSy,float pSPx,float pSPy,
	float pEPx,float pEPy,float pEx,float pEy,bool withOpti)
{
	m_pS.set(pSx,pSy);
	m_pSP.set(pSPx,pSPy);
	m_pEP.set(pEPx,pEPy);
	m_pE.set(pEx,pEy);
	m_withOpti = withOpti;
}
INLINE XVec2 XSimpleBezierSpline::getBezierSplineValue(float temp)const	//temp属于(0,1)
{
	if(temp <= 0.0f) return m_pS;
	if(temp >= 1.0f) return m_pE;
	float Ftemp = 1.0f - temp;
	//二次贝塞尔曲线(临时测试用，或者封装2次贝塞尔曲线的类)
	//return XVec2(m_pS.x * Ftemp * Ftemp + 2.0f * m_pSP.x * Ftemp * temp + m_pE.x * temp * temp,
	//	m_pS.y * Ftemp * Ftemp + 2.0f * m_pSP.y * Ftemp * temp + m_pE.y * temp * temp);
	//return XVec2(m_pS.x * Ftemp * Ftemp * Ftemp + 3.0f * m_pSP.x * Ftemp * Ftemp * temp + 3.0f * m_pEP.x * temp * temp * Ftemp + m_pE.x * temp * temp * temp,
	//	m_pS.y * Ftemp * Ftemp * Ftemp + 3.0f * m_pSP.y * Ftemp * Ftemp * temp + 3.0f * m_pEP.y * temp * temp * Ftemp + m_pE.y * temp * temp * temp);
	return Ftemp * Ftemp * Ftemp * m_pS + Ftemp * Ftemp * temp * 3.0 * m_pSP + 
		temp * temp * Ftemp * 3.0 * m_pEP + temp * temp * temp * m_pE;
	//求导公式如下
	//XVec2(3.0f * m_pS.x * Ftemp * Ftemp + 3.0f * m_pSP.x * Ftemp * (2.0f * temp - Ftemp) + 3.0f * m_pEP.x * temp * (temp - 2.0f * Ftemp) + 3.0f * m_pE.x * temp * temp,
	//	3.0f * m_pS.y * Ftemp * Ftemp + 3.0f * m_pSP.y * Ftemp * (2.0f * temp - Ftemp) + 3.0f * m_pEP.y * temp * (temp - 2.0f * Ftemp) + 3.0f * m_pE.y * temp * temp);
}
INLINE void XBezierSpline::calculate()
{
//		m_ts = (m_pSP - m_pS) * 3.0f;
//		m_tp = (m_pEP - m_pSP) * 6.0f;
//		m_te = (m_pE - m_pEP) * 3.0f;
	m_startSpeed = speed(0.0f);
	m_endSpeed = speed(1.0f);
	m_length = getLength(1.0f);
	if (m_withOpti)
	{
		double step = 1.0 / 128;
		m_tmpData.clear();
		m_tmpData.push_back(0.0);
		for (double i = step; i < 1.0; i += step)
		{
			m_tmpData.push_back(getT(i));
		}
		m_tmpData.push_back(m_length);
	}
}
INLINE double XBezierSpline::speedX(double t)  
{  
	double it = 1.0 - t;  
	//return -3.0*m_pS.x*it*it + 3.0*m_pSP.x*it*it - 6.0*m_pSP.x*it*t + 6.0*m_pEP.x*it*t - 3.0*m_pEP.x*t*t + 3.0*m_pE.x*t*t;  
	return 3.0 * ((m_pSP.x - m_pS.x) * it * it + 2.0 * (m_pEP.x - m_pSP.x) * it * t + (m_pE.x - m_pEP.x) * t * t);  
}
INLINE double XBezierSpline::speedY(double t)  
{  
	double it = 1.0 - t;  
	return 3.0 * ((m_pSP.y - m_pS.y) * it * it + 2.0 * (m_pEP.y - m_pSP.y) * it * t + (m_pE.y - m_pEP.y) * t * t);  
} 
INLINE double XBezierSpline::speed(double t)  
{   
	//方案1：非常消耗效率
	//return _hypot(speedX(t), speedY(t));
	//对上面的算法进行优化
//		double it = 1.0 - t;  
//		return (m_ts * (it * it) + m_tp * (it * t) + m_te * (t * t)).getLength();  
	//方案2：效率较高
	double it = 1.0 - t;    
	return 3.0 * (it * it * (m_pSP - m_pS) + 2.0 * it * t * (m_pEP - m_pSP) + t * t * (m_pE - m_pEP)).getLength();
}
INLINE void XBezierSpline::init(const XVec2& pStart,const XVec2& pSP,
	const XVec2& pEP,const XVec2& pEnd, bool withOpti)
{
	XSimpleBezierSpline::init(pStart, pSP, pEP, pEnd, withOpti);
	calculate();
}
INLINE void XBezierSpline::init(float pSx,float pSy,float pSPx,float pSPy,
	float pEPx,float pEPy,float pEx,float pEy, bool withOpti)
{
	XSimpleBezierSpline::init(pSx, pSy, pSPx, pSPy, pEPx, pEPy, pEx, pEy, withOpti);
	calculate();
}
INLINE void XBezierSpline::reset()				//重置曲线移动点
{
	m_curPosition = 0.0f;
	m_isEnd = XFalse;
	m_curValue = m_pS;
}
INLINE void XBezierSpline2::reset()				//重置曲线移动点
{
	m_curPosition = 0.0f;
	m_isEnd = XFalse;
	m_curValue = m_pS;
}
INLINE XVec2 XSimpleBezierSpline2::getBezierSplineValue(float temp)const	//temp属于(0,1)
{
	if(temp <= 0.0f) return m_pS;
	if(temp >= 1.0f) return m_pE;
	float Ftemp = 1.0f - temp;
	//二次贝塞尔曲线(临时测试用，或者封装2次贝塞尔曲线的类)
	return Ftemp * Ftemp * m_pS + Ftemp * temp * 2.0f * m_pP + temp * temp * m_pE;
	//求导公式如下
	//XVec2(3.0f * m_pS.x * Ftemp * Ftemp + 3.0f * m_pSP.x * Ftemp * (2.0f * temp - Ftemp) + 3.0f * m_pEP.x * temp * (temp - 2.0f * Ftemp) + 3.0f * m_pE.x * temp * temp,
	//	3.0f * m_pS.y * Ftemp * Ftemp + 3.0f * m_pSP.y * Ftemp * (2.0f * temp - Ftemp) + 3.0f * m_pEP.y * temp * (temp - 2.0f * Ftemp) + 3.0f * m_pE.y * temp * temp);
}
INLINE void XSimpleBezierSpline2::init(const XVec2& pStart,const XVec2& pP,const XVec2& pEnd)
{
	m_pS = pStart;
	m_pP = pP;
	m_pE = pEnd;
}
INLINE void XSimpleBezierSpline2::init(float pSx,float pSy,float pPx,float pPy,float pEx,float pEy)
{
	m_pS.set(pSx,pSy);
	m_pP.set(pPx,pPy);
	m_pE.set(pEx,pEy);
}
INLINE double XBezierSpline2::getLength(double t)  //计算[0 - 1]之间的长度
{  
	double temp1 = sqrt(C + t * (B + A * t));  
	double temp2 = 2.0 * sqrtA * (2.0 * A * t * temp1 + B * (temp1 - sqrtC));  
	double temp3 = D - E * log(B + 2.0 * A * t + 2.0 * sqrtA * temp1);  
	return (temp2 + temp3) * powA;  
} 