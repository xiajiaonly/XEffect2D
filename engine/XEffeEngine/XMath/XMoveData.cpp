#include "XStdHead.h"
#include "XMoveData.h"
#include "XMath.h"
namespace XE{
inline float doubleExponentialSigmoid (float x, float a)
{
	float epsilon = 0.00001f;
	float min_param_a = 0.0f + epsilon;
	float max_param_a = 1.0f - epsilon;
	a = (std::min)(max_param_a, (std::max)(min_param_a, a));
	a = 1.0f - a; // for sensible results

	if ( x <= 0.5f )
		return pow( 2.0f * x, 1.0f / a) * 0.5f;
	else
		return 1.0f - ( pow( 2.0f * (1.0f-x), 1.0f / a) ) * 0.5f;
}
#define B0F(t) ((1.0f - t) * (1.0f - t) * (1.0f - t))
#define B1F(t) (3.0f * t * ( 1.0f - t) * (1.0f - t))
#define B2F(t) (3.0f * t * t * (1.0f - t))
#define B3F(t) (t * t * t)
#define slopeFromT(t,A,B,C)  (1.0f / (3.0f * A * t * t + 2.0f * B * t + C))
#define xFromT(t, A, B, C, D) (A * (t * t * t) + B * (t * t) + C * t + D)
#define yFromT(t, E, F, G, H) (E * (t * t * t) + F * (t * t) + G * t + H)
inline float cubicBezier(float x, float a, float b, float c, float d)
{
	float y0a = 0.00f; // initial y
	float x0a = 0.00f; // initial x
	float y1a = b;    // 1st influence y
	float x1a = a;    // 1st influence x
	float y2a = d;    // 2nd influence y
	float x2a = c;    // 2nd influence x
	float y3a = 1.00f; // final y
	float x3a = 1.00f; // final x

	float A = x3a - 3.0f * x2a + 3.0f * x1a - x0a;
	float B = 3.0f * x2a - 6.0f * x1a + 3.0f * x0a;
	float C = 3.0f * x1a - 3.0f * x0a;
	float D = x0a;

	float E = y3a - 3.0f * y2a + 3.0f * y1a - y0a;
	float F = 3.0f * y2a - 6.0f * y1a + 3.0f * y0a;
	float G = 3.0f * y1a - 3.0f * y0a;
	float H = y0a;

	// Solve for t given x (using Newton-Raphelson), then solve for y given t.
	// Assume for the first guess that t = x.
	float currentt = x;
	int nRefinementIterations = 5;
	for (int i = 0; i < nRefinementIterations;++ i)
	{
		float currentx = xFromT (currentt,A,B,C,D);
		float currentslope = slopeFromT(currentt,A,B,C);
		currentt -= (currentx - x)*(currentslope);
		currentt = XMath::clamp(currentt, 0.0f, 1.0f);
	}

	return yFromT(currentt,E,F,G,H);
}
inline float cubicBezierNearlyThroughTwoPoints(float x, float a, float b, float c, float d)
{
	float y = 0.0f;
	float epsilon = 0.00001f;
	float min_param_a = 0.0f + epsilon;
	float max_param_a = 1.0f - epsilon;
	float min_param_b = 0.0f + epsilon;
	float max_param_b = 1.0f - epsilon;
	a = max(min_param_a, min(max_param_a, a));
	b = max(min_param_b, min(max_param_b, b));

	float x0 = 0.0f;
	float y0 = 0.0f;
	float x4 = a;
	float y4 = b;
	float x5 = c;
	float y5 = d;
	float x3 = 1.0f;
	float y3 = 1.0f;
	float x1,y1,x2,y2; // to be solved.

	// arbitrary but reasonable
	// t-values for interior control points
	float t1 = 0.3f;
	float t2 = 0.7f;

	float B0t1 = B0F(t1);
	float B1t1 = B1F(t1);
	float B2t1 = B2F(t1);
	float B3t1 = B3F(t1);
	float B0t2 = B0F(t2);
	float B1t2 = B1F(t2);
	float B2t2 = B2F(t2);
	float B3t2 = B3F(t2);

	float ccx = x4 - x0*B0t1 - x3*B3t1;
	float ccy = y4 - y0*B0t1 - y3*B3t1;
	float ffx = x5 - x0*B0t2 - x3*B3t2;
	float ffy = y5 - y0*B0t2 - y3*B3t2;

	x2 = (ccx - (ffx*B1t1)/B1t2) / (B2t1 - (B1t1*B2t2)/B1t2);
	y2 = (ccy - (ffy*B1t1)/B1t2) / (B2t1 - (B1t1*B2t2)/B1t2);
	x1 = (ccx - x2*B2t1) / B1t1;
	y1 = (ccy - y2*B2t1) / B1t1;

	x1 = max(0.0f + epsilon, min(1.0f - epsilon, x1));
	x2 = max(0.0f + epsilon, min(1.0f - epsilon, x2));

	// Note that this function also requires cubicBezier()!
	y = cubicBezier(x, x1, y1, x2, y2);
	y = (std::max)(0.0f, (std::min)(1.0f, y));
	return y;
}
inline float objectDrop(float percent,float p1)
{
	float r;
	if ( percent < 0.75f )
	{
		r = cosf( (2.0f * PI / 3.0f) * percent );
	}else
	{
		float range = 0.125f;
		float diffRange = 0.125f;
		float amp = p1;
		float freq = 8.0f;

		if ( percent < 0.75f + range )
		{
			r = amp * sinf( freq * PI * percent );
		}else{
			diffRange *= 0.5f; range += diffRange; amp *= 0.5f; freq *= 2.0f;
			if ( percent < 0.75f + range )
			{
				r = amp * sinf( freq * PI * percent );
			}else
			{
				diffRange *= 0.5f; range += diffRange; amp *= 0.5f; freq *= 2.0f;
				if ( percent < 0.75f + range )
				{
					r = amp * sinf( freq * PI * percent );
				}else
				{
					diffRange *= 0.5f; range += diffRange; amp *= 0.5f; freq *= 2.0f;
					if ( percent < 0.75f + range )
					{
						r = amp * sinf( freq * PI * percent );
					}else
					{
						diffRange *= 0.5f; range += diffRange; amp *= 0.5f; freq *= 2.0f;
						if ( percent < 0.75f + range )
						{
							r = amp * sinf( freq * PI * percent );
						}else
						{
							diffRange *= 0.5f; range += diffRange; amp *= 0.5f; freq *= 2.0f;
							if ( percent < 0.75f + range )
							{
								r = amp * sinf( freq * PI * percent );
							}else
							{
								r = 0.0f;
							}
						}
					}
				}
			}
		}
	}
	return 1.0f - r;
}
inline float easeInBack(float ratio, float offset = 0.0f)
{
	float s = 1.70158f + offset;
	return powf(ratio, 2.0f) * ((s + 1.0f) * ratio - s);
}
inline float easeOutBack(float ratio, float offset = 0.0f)
{
	float invRatio = ratio - 1.0f;
	float s = 1.70158f + offset;
	return powf(invRatio, 2.0f) * ((s + 1.0f) * invRatio + s) + 1.0f;
}
inline float easeInOutBack(float ratio, float offset = 0.0f)
{
	if (ratio < 0.5f) return 0.5f * easeInBack (ratio * 2.0f, offset);
	else              return 0.5f * easeOutBack((ratio - 0.5f) * 2.0f, offset) + 0.5f;
}
inline float easeOutInBack(float ratio, float offset = 0.0f)
{
	if (ratio < 0.5f) return 0.5f * easeOutBack(ratio * 2.0f, offset);
	else              return 0.5f * easeInBack((ratio - 0.5f) * 2.0f, offset) + 0.5f;
}
inline float easeInElastic(float ratio, float param1 = 1.0f, float param2 = 1.0f, float param3 = 1.0f)
{
	if (ratio == 0.0f || ratio == 1.0f) return ratio;
	else
	{
		float p = 0.3f * param2;
		float s = p * 0.25f;
		float invRatio = ratio - 1.0f;
		return -1.0f * powf(2.0f + param3, param1 * 10.0f * invRatio) * sinf((invRatio*invRatio-s) * PI2 / p);
	}
}
inline float easeOutElastic(float ratio, float param1 = 1.0f, float param2 = 1.0f, float param3 = 1.0f)
{
	if (ratio == 0.0f || ratio == 1.0f) return ratio;
	else
	{
		float p = 0.3f * param2;
		float s = p * 0.25f;
		return powf(2.0f + param3, -param1 * 10.0f * ratio) * sinf((ratio*ratio-s) * PI2 / p) + 1.0f;
	}
}
inline float easeInOutElastic(float ratio, float param1 = 1.0f, float param2 = 1.0f, float param3 = 1.0f)
{
	if (ratio < 0.5f) return 0.5f * easeInElastic( ratio * 2.0f, param1, param2, param3);
	else              return 0.5f * easeOutElastic((ratio -0.5f ) * 2.0f, param1, param2, param3) + 0.5f;
}
inline float easeOutInElastic(float ratio, float param1 = 1.0f, float param2 = 1.0f, float param3 = 1.0f)
{
	if (ratio < 0.5f) return 0.5f * easeOutElastic(ratio * 2.0f, param1, param2, param3);
	else              return 0.5f * easeInElastic((ratio - 0.5f) * 2.0f, param1, param2, param3) + 0.5f;
}
float easeOutBounce(float ratio)
{
	float s = 7.5625f;
	float p = 2.75f;
	float pow = 2.0f;
	float l;
	if (ratio < (1.0f/p))
	{
		l = s * powf(ratio, pow);
	}else
	{
		if (ratio < (2.0f/p))
		{
			ratio -= 1.5f/p;
			l = s * powf(ratio, pow) + 0.75f;
		}else
		{
			if (ratio < 2.5f/p)
			{
				ratio -= 2.25f/p;
				l = s * powf(ratio, pow) + 0.9375f;
			}else
			{
				ratio -= 2.625f/p;
				l = s * powf(ratio, pow) + 0.984375f;
			}
		}
	}
	return l;
}
inline float easeInBounce(float ratio)
{
	return 1.0f - easeOutBounce(1.0f - ratio);
}
inline float easeInOutBounce(float ratio)
{
	if (ratio < 0.5f) return 0.5f * easeInBounce(ratio * 2.0f);
	else              return 0.5f * easeOutBounce((ratio - 0.5f) * 2.0f) + 0.5f;
}
inline float easeOutInBounce(float ratio)
{
	if (ratio < 0.5f) return 0.5f * easeOutBounce(ratio * 2.0f);
	else              return 0.5f * easeInBounce((ratio - 0.5f) * 2.0f) + 0.5f;
}
float customBounce(float t,
				   float numBm, float acc,
				   float* bDurations, float* bVels )
{

	if(bDurations == NULL || bVels == NULL) return 0.0f;

	t = XMath::clamp(t, 0.0f, 1.0f);
	int index = 0;
	float totalDuration = 0.0f;

	// get index to particular bounce
	while(index < numBm && t > totalDuration + bDurations[index])
	{
		totalDuration += bDurations[index];
		index++;
	}

	// get duration within bounce (if not the first one)
	t = t - totalDuration;

	if (index == 0)
		return 1.0f + acc * (t * t) * 0.5f;
	else
		return t * (bVels[index] + (acc * t) * 0.5f);
}
inline float smoothStep(float x)
{
	return x * x * (3.0f - 2.0f * x);
}
inline float smootherStep(float x)
{
	return x * x * x * ( x * ( x * 6.0f - 15.0f) + 10.0f);
}
float XMoveData::getValueEx(float percentage)
{
	if(percentage < 0.0f) percentage = 0.0f;
	if(percentage > 1.0f) percentage = 1.0f;

	if(percentage >= 1.0f)
	{
		return m_endData;
	}else
	{
		float r;
		switch(m_mode)
		{
		case MD_MODE_LINE:
			return m_startData + (m_endData - m_startData) * percentage;
		case MD_MODE_SIN:
			return m_startData + (m_endData - m_startData) * sin(percentage * PI_HALF);
		case MD_MODE_SIN_MULT:
			return m_startData + (m_endData - m_startData) * (1.0f - (percentage - 1.0f) * (percentage - 1.0f));
		case MD_MODE_SQRT2:
			return m_startData + (m_endData - m_startData) * pow(percentage,0.25f);
		case MD_MODE_SQRT2_MULT:
			return m_startData + (m_endData - m_startData) * (1.0f - (percentage - 1.0f) * (percentage - 1.0f) * (percentage - 1.0f) * (percentage - 1.0f));
		case MD_MODE_COS:
			return m_startData + (m_endData - m_startData) * (1.0f - cos(percentage * PI_HALF));
		case MD_MODE_COS_MULT:
			return m_startData + (m_endData - m_startData) * percentage * percentage;
		case MD_MODE_DSIN:
			if(percentage < 0.5f) return m_startData + (m_endData - m_startData) * sin(percentage * PI) * 0.5f;
			else return m_startData + (m_endData - m_startData) * (2.0f - sin(percentage * PI)) * 0.5f;
		case MD_MODE_DCOS:
			return m_startData + (m_endData - m_startData) * (1.0f - cos(percentage * PI)) * 0.5f;
		case MD_MODE_DSIN_MULT:
			if(percentage < 0.5f) return m_startData + (m_endData - m_startData) * (0.25f - (percentage - 0.5f) * (percentage - 0.5f)) * 2.0f;
			else return m_startData + (m_endData - m_startData) * (0.5f + (percentage - 0.5f) * (percentage - 0.5f) * 2.0f);
		case MD_MODE_DCOS_MULT:
			if(percentage < 0.5f)return m_startData + (m_endData - m_startData) * percentage * percentage * 2.0f;
			else return m_startData + (m_endData - m_startData) * (1.0f - (percentage - 1.0f) * (percentage - 1.0f) * 2.0f);
		case MD_MODE_SHAKE:
			//return (m_startData - m_endData) * (1.0f - percentage) * cos(percentage * PI * 2.5f) + m_endData;	//这里需要重新设计
			return (m_startData - m_endData) * (1.0f - percentage) * (1.0f - percentage) * (1.0f - percentage) * cos(percentage * PI * 2.5f) + m_endData;	//这里需要重新设计
		case MD_MODE_SHAKE_EX:
			return (m_startData - m_endData) * sin(percentage * PI2 * 2.0f) * (1.0f - sin(percentage * PI_HALF)) + m_endData;	//这里需要重新设计
		case MD_MODE_EIEO:
			return m_startData + (m_endData - m_startData) * (0.5f - 0.5f * cosf(PI * percentage));
		case MD_MODE_EI:
			return m_startData + (m_endData - m_startData) * (1.0f + sinf(PI_HALF * percentage - PI_HALF));
		case MD_MODE_EO:
			return m_startData + (m_endData - m_startData) * sinf(PI_HALF * percentage);
		case MD_MODE_EL:
			return m_startData + (m_endData - m_startData) * ((percentage < 0.25f) ? 0.0f : 1.33333333f * (percentage - 0.25f));
		case MD_MODE_LL:
			return m_startData + (m_endData - m_startData) * ((percentage < 0.5f) ? 0.0f : 2.0f * percentage - 1.0f);
		case MD_MODE_VLL:
			return m_startData + (m_endData - m_startData) * ((percentage < 0.75f) ? 0.0f : 4.0f * percentage - 3.0f);
		case MD_MODE_TANH:
			return m_startData + (m_endData - m_startData) * (0.5f + 0.5f * tanh(2.0f * PI * percentage - PI) * 1.00374187319732);
		case MD_MODE_SINH:
			return m_startData + (m_endData - m_startData) * (0.5f + 0.23482f * sinh(3.0f * percentage - 1.5f));
		case MD_MODE_SQUARE:
			return m_startData + (m_endData - m_startData) * ((percentage < 0.5f) ? 0.0f : 1.0f);
		case MD_MODE_B5:
			return m_startData + (m_endData - m_startData) * ((fmod(percentage * 5.0f, 1.01f) < 0.5f ? 0.0f : 1.0f));
		case MD_MODE_B3:
			return m_startData + (m_endData - m_startData) * ((fmod(percentage * 3.0f, 1.01f) < 0.5f ? 0.0f : 1.0f));
		case MD_MODE_B2:
			return m_startData + (m_endData - m_startData) * ((fmod(percentage * 2.0f, 1.01f) < 0.5f ? 0.0f : 1.0f));
		case MD_MODE_BAF1:
			r = (fmod(percentage * 2.0f, 1.01f) < 0.5f ? 0.0f : 1.0f);
			if (percentage >= 0.75) r = 4 * percentage - 4 * 0.75f ;
			return m_startData + (m_endData - m_startData) * r;
		case MD_MODE_BAF2:
			r = (fmod(percentage * 3.0f, 1.01f) < 0.5f ? 0.0f : 1.0f);
			if (percentage >= 0.833333f) r = 6 * percentage - 6 * 0.833333f ;
			return m_startData + (m_endData - m_startData) * r;
		case MD_MODE_BAF3:
			r = (fmod(percentage * 4.0f, 1.01f) < 0.5f ? 0.0f : 1.0f);
			if (percentage >= 0.875f) r = 8 * percentage - 8 * 0.875f ;
			return m_startData + (m_endData - m_startData) * r;
		case MD_MODE_LS:
			return m_startData + (m_endData - m_startData) * ((percentage < 0.75f) ? 0.0f : 1.0f);
		case MD_MODE_ES:
			return m_startData + (m_endData - m_startData) * ((percentage < 0.25f) ? 0.0f : 1.0f);
		case MD_MODE_LEIEO:
			return m_startData + (m_endData - m_startData) * ((percentage < 0.5f) ? 0.0f : 0.5f + 0.5f * cosf(PI2 * percentage));
		case MD_MODE_VLEIEO:
			return m_startData + (m_endData - m_startData) * ((percentage < 0.75f) ? 0.0f : 0.5f + 0.5f * cosf(4.0f * PI * percentage));
		case MD_MODE_QEI:
			return m_startData + (m_endData - m_startData) * percentage * percentage;
		case MD_MODE_QEO:
			return m_startData + (m_endData - m_startData) * (1.0f - (percentage - 1.0f) * (percentage - 1.0f));
		case MD_MODE_EQEO:
			r = (percentage - 0.25f) * 1.333333333f;
			return m_startData + (m_endData - m_startData) * ((percentage < 0.25f) ? 0.0f : 1.0f - (r - 1.0f) * (r - 1.0f));
		case MD_MODE_CEI:
			return m_startData + (m_endData - m_startData) * percentage * percentage * percentage;
		case MD_MODE_CEO:
			r = (1.0f - percentage);
			return m_startData + (m_endData - m_startData) * (1.0f - r * r * r);
		case MD_MODE_QUEI:
			return m_startData + (m_endData - m_startData) * percentage * percentage * percentage * percentage;
		case MD_MODE_QUEO:
			r = (1.0f - percentage);
			return m_startData + (m_endData - m_startData) * (1.0f - r * r * r * r);
		case MD_MODE_BOUNCY:
			r = 0.5f;
			return m_startData + (m_endData - m_startData) * (0.5f - 0.51f * cosf(PI * percentage + r * percentage - r * 0.5f ));
		case MD_MODE_QBP:
			return m_startData + (m_endData - m_startData) * XMath::quadraticBezier(percentage,m_param.p1,m_param.p2);
		case MD_MODE_ESP:
			return m_startData + (m_endData - m_startData) * doubleExponentialSigmoid(percentage,m_param.p1);
		case MD_MODE_CBP:
			return m_startData + (m_endData - m_startData) * cubicBezier(percentage,m_param.p1,m_param.p2,m_param.p3,m_param.p4);
		case MD_MODE_CB2P:
			return m_startData + (m_endData - m_startData) * cubicBezierNearlyThroughTwoPoints(percentage,m_param.p1,m_param.p2,m_param.p3,m_param.p4);
		case MD_MODE_SG:
			return m_startData + (m_endData - m_startData) * cubicBezier(percentage, 0.444f, 0.013f, 0.188f, 0.956f);
		case MD_MODE_OD:
			return m_startData + (m_endData - m_startData) * objectDrop(percentage, m_param.p1);
		case MD_MODE_EIB:
			return m_startData + (m_endData - m_startData) * easeInBack(percentage, m_param.p1);
		case MD_MODE_EOB:
			return m_startData + (m_endData - m_startData) * easeOutBack(percentage, m_param.p1);
		case MD_MODE_EOIB:
			return m_startData + (m_endData - m_startData) * easeOutInBack(percentage, m_param.p1);
		case MD_MODE_EIOB:
			return m_startData + (m_endData - m_startData) * easeInOutBack(percentage, m_param.p1);
		case MD_MODE_EIBo:
			return m_startData + (m_endData - m_startData) * easeInBounce(percentage);
		case MD_MODE_EOBo:
			return m_startData + (m_endData - m_startData) * easeOutBounce(percentage);
		case MD_MODE_EIOBo:
			return m_startData + (m_endData - m_startData) * easeInOutBounce(percentage);
		case MD_MODE_EOIBo:
			return m_startData + (m_endData - m_startData) * easeOutInBounce(percentage);
		case MD_MODE_EIE:
			return m_startData + (m_endData - m_startData) * easeInElastic(percentage,m_param.p1,m_param.p2,m_param.p3);
		case MD_MODE_EOE:
			return m_startData + (m_endData - m_startData) * easeOutElastic(percentage,m_param.p1,m_param.p2,m_param.p3);
		case MD_MODE_EIOE:
			return m_startData + (m_endData - m_startData) * easeInOutElastic(percentage,m_param.p1,m_param.p2,m_param.p3);
		case MD_MODE_EOIE:
			return m_startData + (m_endData - m_startData) * easeOutInElastic(percentage,m_param.p1,m_param.p2,m_param.p3);
		case MD_MODE_BIC:
			return m_startData + (m_endData - m_startData) * customBounce(1.0f - percentage,m_param.p1,m_param.p2,bounceDuration,bounceVelocity);
		case MD_MODE_BOC:
			return m_startData + (m_endData - m_startData) * (1.0f - customBounce(percentage,m_param.p1,m_param.p2,bounceDuration,bounceVelocity));
		case MD_MODE_SS:
			return m_startData + (m_endData - m_startData) * smoothStep(percentage);
		case MD_MODE_SES:
			return m_startData + (m_endData - m_startData) * smootherStep(percentage);
		}
	}
	return m_startData;
}
void XMoveData::setCurTimer(float t)
{
	if(m_isEnd) return;
	m_timer = t;
	if (m_isLoop == 0)
	{
		if (m_timer < 0.0f)
		{
			m_timer = static_cast<int>(m_timer) - m_timer;
		}
		else
		if(m_timer > 1.0f)
		{
			m_timer -= static_cast<int>(m_timer);
		}
	}
	else
	{
		if (m_timer < 0.0f)
		{
			m_timer = 2.0f + fmod(m_timer,2.0f);
		}
		else
		if (m_timer > 2.0f)
		{
			m_timer = fmod(m_timer, 2.0f);
		}
	}
	move(0.0f);
}
void XMoveData::move(float delay)
{
	if(m_isEnd) return;
	m_timer += delay * m_speed;
	if(m_isLoop == 0)
	{//如果不循环
		if(m_timer >= 1.0f)
		{
			m_isEnd = XTrue;
			m_timer = 1.0f;
		}
		m_curData = getValueEx(m_timer);
	}else
	{//如果循环
		if(m_timer >= 2.0f)
		{
			if(m_loopTimer > 0)	//小于零则无线循环
			{
				m_loopTimer -= m_timer * 0.5f;
				if(m_loopTimer <= 0) 
				{
					m_isEnd = XTrue;
					m_timer = 0.0f;
				}else
				{
					m_timer = fmod(m_timer,2.0f);
				}
			}else
			{
				m_timer = fmod(m_timer,2.0f);
			}
		}
		if(m_timer > 1.0f) m_curData = getValueEx(2.0f - m_timer);
		else m_curData = getValueEx(m_timer);
	}
}
std::string XMoveData::getModeStr()
{
	switch (m_mode)
	{
	case MD_MODE_NULL:
		return "NULL";
	case MD_MODE_LINE:
		return "LINE";
	case MD_MODE_SIN:
		return "SIN";
	case MD_MODE_SIN_MULT:
		return "SIN_MULT";
	case MD_MODE_SQRT2:
		return "SQRT2";
	case MD_MODE_SQRT2_MULT:
		return "SQRT2_MULT";
	case MD_MODE_COS:
		return "COS";
	case MD_MODE_COS_MULT:
		return "COS_MULT";
	case MD_MODE_DSIN:
		return "DSIN";
	case MD_MODE_DCOS:
		return "DCOS";
	case MD_MODE_DSIN_MULT:
		return "DSIN_MULT";
	case MD_MODE_DCOS_MULT:
		return "DCOS_MULT";
	case MD_MODE_SHAKE:
		return "SHAKE";
	case MD_MODE_SHAKE_EX:
		return "SHAKE_EX";
	case MD_MODE_EIEO:
		return "EIEO";
	case MD_MODE_EI:
		return "EI";
	case MD_MODE_EO:
		return "EO";
	case MD_MODE_EL:
		return "EL";
	case MD_MODE_LL:
		return "LL";
	case MD_MODE_VLL:
		return "VLL";
	case MD_MODE_TANH:
		return "TANH";
	case MD_MODE_SINH:
		return "SINH";
	case MD_MODE_SQUARE:
		return "SQUARE";
	case MD_MODE_B5:
		return "B5";
	case MD_MODE_B3:
		return "B3";
	case MD_MODE_B2:
		return "B2";
	case MD_MODE_BAF1:
		return "BAF1";
	case MD_MODE_BAF2:
		return "BAF2";
	case MD_MODE_BAF3:
		return "BAF3";
	case MD_MODE_LS:
		return "LS";
	case MD_MODE_ES:
		return "ES";
	case MD_MODE_LEIEO:
		return "LEIEO";
	case MD_MODE_VLEIEO:
		return "VLEIEO";
	case MD_MODE_QEI:
		return "QEI";
	case MD_MODE_QEO:
		return "QEO";
	case MD_MODE_EQEO:
		return "EQEO";
	case MD_MODE_CEI:
		return "CEI";
	case MD_MODE_CEO:
		return "CEO";
	case MD_MODE_QUEI:
		return "QUEI";
	case MD_MODE_QUEO:
		return "QUEO";
	case MD_MODE_BOUNCY:
		return "BOUNCY";
	case MD_MODE_QBP:
		return "QBP";
	case MD_MODE_ESP:
		return "ESP";
	case MD_MODE_CBP:
		return "CBP";
	case MD_MODE_CB2P:
		return "CB2P";
	case MD_MODE_SG:
		return "SG";
	case MD_MODE_OD:
		return "OD";
	case MD_MODE_EIB:
		return "EIB";
	case MD_MODE_EOB:
		return "EOB";
	case MD_MODE_EOIB:
		return "EOIB";
	case MD_MODE_EIOB:
		return "EIOB";
	case MD_MODE_EIBo:
		return "EIBo";
	case MD_MODE_EOBo:
		return "EOBo";
	case MD_MODE_EIOBo:
		return "EOIBo";
	case MD_MODE_EOIBo:
		return "EIOBo";
	case MD_MODE_EIE:
		return "EIE";
	case MD_MODE_EOE:
		return "EOE";
	case MD_MODE_EIOE:
		return "EIOE";
	case MD_MODE_EOIE:
		return "EOIE";
	case MD_MODE_BIC:
		return "BIC";
	case MD_MODE_BOC:
		return "BOC";
	case MD_MODE_SS:
		return "SS";
	case MD_MODE_SES:
		return "SES";
	case MD_MODE_MAX:
		return "MAX";
	}
	return "NULL";
}
void XMoveData::set(float start,float end,float speed,XMoveDataMode mode,int isLoop)
{
	m_isEnd = XFalse;
	m_startData = start;
	m_endData = end;
	m_speed = speed;
	m_curData = start;
	m_timer = 0.0f;
	m_mode = mode;
	m_isLoop = isLoop;
	if(isLoop >= 1) m_loopTimer = isLoop;
	else m_loopTimer = -1;	//无限循环
	initParam();
}
void XMoveData::initParam()
{
	//下面初始化参数为典型值
	switch (m_mode)
	{
	case XE::MD_MODE_QBP:
		m_param.p1 = 0.25f;
		m_param.p2 = 0.75f;
		break;
	case XE::MD_MODE_ESP:
		m_param.p1 = 0.5f;	//0 - 1
		break;
	case XE::MD_MODE_CBP:
	case XE::MD_MODE_CB2P:
		m_param.p1 = 0.033f;
		m_param.p2 = 0.973f;
		m_param.p3 = 0.250f;
		m_param.p4 = 0.750f;
		break;
	case XE::MD_MODE_OD:
		m_param.p1 = 0.05f;
		break;
	case XE::MD_MODE_EIB:
	case XE::MD_MODE_EOB:
	case XE::MD_MODE_EOIB:
	case XE::MD_MODE_EIOB:
		m_param.p1 = 1.0f;
		break;
	case XE::MD_MODE_EIBo:
	case XE::MD_MODE_EOBo:
	case XE::MD_MODE_EIOBo:
	case XE::MD_MODE_EOIBo:
		break;
	case XE::MD_MODE_EIE:
	case XE::MD_MODE_EOE:
	case XE::MD_MODE_EIOE:
	case XE::MD_MODE_EOIE:
		m_param.p1 = 0.5f;
		m_param.p2 = 1.0f;
		m_param.p3 = 0.3f;
		break;
	case XE::MD_MODE_BIC:
	case XE::MD_MODE_BOC:
		m_param.p1 = 7.0f;	//1 - 7 ，否则会超出范围曲线不理想
		m_param.p2 = 0.0f;
		bounceElast = 0.5f;
		initCustomBounce();
		break;
	default:
		break;
	}
}
// taken from http://sbcgamesdev.blogspot.com/2013/05/bounce-timing-function_8.html and moded heavily
void XMoveData::initCustomBounce()
{
	int bounceNumB = m_param.p1;
	// get "some" acceleration and calculate time for bounces
	float acceleration = 0.001f;
	float totalDuration = 0.0f;
	float height = 1.0f;
	for (int i = 0; i < bounceNumB; i++)
	{
		// s = 1/2 a * t^2 ... 2s / a = t^2 ... sqrt(2s / a) = t
		float duration = sqrtf(2.0f * height / acceleration) * 2.0f;
		if (i == 0) duration *= 0.5f;
		bounceDuration[i] = duration;
		totalDuration += duration;
		height *= bounceElast;
	}

	// adjust total duration to fit requested duration
	for (int i = 0; i < bounceNumB; i++)
	{
		bounceDuration[i] = bounceDuration[i] / totalDuration;
	}

	// s = 1/2 a * t^2 ... 2s / t^2 = a
	m_param.p2 = 2.0f / (bounceDuration[0] * bounceDuration[0]);

	// calculate initial bounce velocities
	for (int i = 0; i < bounceNumB; i++)
	{
		if (i == 0) bounceVelocity[i] = 0.0f;
		else bounceVelocity[i] = bounceDuration[i] / 2.0f * m_param.p2;
	}
	// change the sign of acceleration to point downwards
	m_param.p2 = -m_param.p2;
}
#if !WITH_INLINE_FILE
#include "XMoveData.inl"
#endif
}