#ifndef _JIA_XSTATISTICS_
#define _JIA_XSTATISTICS_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.3
//--------------------------------
#include <deque>
namespace XE{
namespace XMath{
//这边定义的都是一些统计学方面的方法
//求数组v中各元素的和
extern double getSum(const double *v,int sum);
extern float getSum(const float *v,int sum);
//求平均数
extern double getAverageValue(const double *v,int sum);
extern float getAverageValue(const float *v,int sum);
extern float getAverageValue(const std::deque<float> &data);
extern XVec2 getAverageValue(const std::deque<XVec2> &data);
//求标准差(均方差)，用于描述样本的离散性
extern double getStandardDeviation(const double *v,int sum);
extern float getStandardDeviation(const float *v,int sum);
extern float getStandardDeviation(const std::deque<float> &data);
extern float getStandardDeviation(const std::deque<XVec2> &data);
}
}
#endif