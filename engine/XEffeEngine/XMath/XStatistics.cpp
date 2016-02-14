#include "XStdHead.h"
#include "XStatistics.h"
namespace XE{
namespace XMath{
double getSum(const double *v,int sum)
{
	if(v == NULL || sum == 0) return 0.0;
	double ret = 0.0;
	for(int i = 0;i < sum;++ i)
	{
		ret += v[i];
	}
	return ret;
}
float getSum(const float *v,int sum)
{
	if(v == NULL || sum == 0) return 0.0f;
	float ret = 0.0f;
	for(int i = 0;i < sum;++ i)
	{
		ret += v[i];
	}
	return ret;
}
double getAverageValue(const double *v,int sum)
{
	if(v == NULL || sum == 0) return 0.0;
	double ret = 0.0;
	for(int i = 0;i < sum;++ i)
	{
		ret += v[i];
	}
	return ret / (double)(sum);
}
float getAverageValue(const float *v,int sum)
{
	if(v == NULL || sum == 0) return 0.0f;
	float ret = 0.0f;
	for(int i = 0;i < sum;++ i)
	{
		ret += v[i];
	}
	return ret / (float)(sum);
}
float getAverageValue(const std::deque<float> &data)
{
	if(data.size() == 0) return 0.0f;
	float ret = 0.0f;
	std::deque<float>::const_iterator it = data.begin();
	for(;it != data.end();++ it)
	{
		ret += *it;
	}
	return ret / (float)(data.size());
}
double getStandardDeviation(const double *v,int sum)
{
	if(v == NULL || sum == 0) return 0.0;
	double av = getAverageValue(v,sum);
	double ret = 0.0;
	for(int i = 0;i < sum;++ i)
	{
		ret += (v[i] - av) * (v[i] - av);
	}
	return sqrt(ret / (double)(sum));
}
float getStandardDeviation(const float *v,int sum)
{
	if(v == NULL || sum == 0) return 0.0f;
	float av = getAverageValue(v,sum);
	float ret = 0.0f;
	for(int i = 0;i < sum;++ i)
	{
		ret += (v[i] - av) * (v[i] - av);
	}
	return sqrt(ret / (float)(sum));
}
float getStandardDeviation(const std::deque<float> &data)
{
	if(data.size() == 0) return 0.0f;
	float av = getAverageValue(data);
	float ret = 0.0f;
	std::deque<float>::const_iterator it = data.begin();
	for(;it != data.end();++ it)
	{
		ret += (*it - av)*(*it - av);
	}
	return sqrt(ret / (float)(data.size()));

}
}
}