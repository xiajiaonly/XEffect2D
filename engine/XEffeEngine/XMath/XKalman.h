#ifndef _JIA_XKALMAN_
#define _JIA_XKALMAN_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.8.19
//--------------------------------
//这是一个关于卡尔曼滤波的类
namespace XE{
class XKalmanFilter
{
protected:
	double m_processNioseQ;	//Q:过程噪声，Q增大，动态响应变快，收敛稳定性变坏
	double m_measureNoiseR;	//R:测量噪声，R增大，动态响应变慢，收敛稳定性变好  
//	double m_initialPrediction;	//参数
	//中间参数
	double m_xLast;
	double m_pLast;
	bool m_isFirst;
public:
	virtual double getQ() const{return m_processNioseQ;}
	virtual void setQ(double q){m_processNioseQ = q;}
	virtual double getR() const{return m_measureNoiseR;}
	virtual void setR(double r){m_measureNoiseR = r;}
//	virtual double getP() const{return m_initialPrediction;}
//	virtual void setP(double p){m_initialPrediction = p;}
	double filter(double data)
	{
		if(m_isFirst)
		{
			m_xLast = data;
			m_isFirst = false;
		}
		double x_mid = m_xLast;								//x_last=x(k-1|k-1),x_mid=x(k|k-1)
		double p_mid = m_pLast + m_processNioseQ;			//p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=噪声
		double kg = p_mid / (p_mid + m_measureNoiseR);		//kg为kalman filter，R为噪声
		m_xLast = x_mid + kg * (data - x_mid);			//估计出的最优值
		m_pLast = (1.0 - kg) * p_mid;						//最优值对应的covariance

		return m_xLast;                
	}
	virtual void init(double Q,double R)	//，double p
	{
		m_processNioseQ = Q;
		m_measureNoiseR = R;
//		m_initialPrediction = P;
	}
	XKalmanFilter()
		:m_processNioseQ(0.0000001)
		,m_measureNoiseR(10.0)
//		,m_initialPrediction(2210.0)
		,m_xLast(0.0)
		,m_pLast(0.0)
		,m_isFirst(true)
	{}
};
////下面是卡尔曼滤波的网络资料
////http://www.amobbs.com/thread-5559754-1-1.html
////http://www.amobbs.com/thread-5571611-1-1.html
////下面是关于卡尔曼滤波的一些算法
//#define KALMAN_Q        0.0000001
//#define KALMAN_R        10.0000
//#define KALMAN_P0        2210.0000
////Q:过程噪声，Q增大，动态响应变快，收敛稳定性变坏
////R:测量噪声，R增大，动态响应变慢，收敛稳定性变好        
//inline double KalmanFilter(const double ResrcData,	//输入数据
//					double ProcessNiose_Q,	//参数
//					double MeasureNoise_R,	//参数
//					double InitialPrediction)	//参数
//{
//	double R = MeasureNoise_R;
//	double Q = ProcessNiose_Q;
//
//	static double x_last = 0.0;
//	static double p_last = 0.0;      
//
//	double x_mid = x_last; //x_last=x(k-1|k-1),x_mid=x(k|k-1)
//	double p_mid = p_last+Q; //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=噪声
//	double kg = p_mid/(p_mid+R); //kg为kalman filter，R为噪声
//	double x_now = x_mid+kg*(ResrcData-x_mid);//估计出的最优值
//                
//	double p_now = (1-kg)*p_mid;//最优值对应的covariance        
//
//	p_last = p_now; //更新covariance值
//	x_last = x_now; //更新系统状态值
//
//	return x_now;                
//}
}
#endif