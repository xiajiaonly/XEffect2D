#include "XStdHead.h"
#include "XSimpleChart.h"
#include "XControl\XCtrlColorStyle.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "XResourcePack.h"
#include "..\XMath\XStatistics.h"
namespace XE{
void XSimpleChart::draw()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	XVector2 tmpSize = m_size * m_scale;
	//描绘背景
	XRender::drawFillBoxExA(m_position,tmpSize,XCCS::lightBGColor * m_color);
	switch (m_simpleChartType)
	{
	case XE::TYPE_0_UP:
		//描绘零线
		XRender::drawLine(XVector2(m_position.x,m_position.y),XVector2(m_position.x + tmpSize.x,m_position.y),1.0f,
			XCCS::blackColor * m_color);
		//下面描绘数据
		if(m_datas.size() > 0)
			XRender::drawLine(XVector2(m_position.x,m_position.y),XVector2(m_position.x + m_rateX * 0 * m_scale.x,(-m_datas[0]) * m_rateY * m_scale.y + m_position.y),1,
				XCCS::lightBlackColor * m_color);
		for(int i = 1;i < m_datas.size();++ i)
		{
			XRender::drawLine(XVector2(m_position.x + m_rateX * (i - 1) * m_scale.x,(-m_datas[i - 1]) * m_rateY * m_scale.y + m_position.y),
				XVector2(m_position.x + m_rateX * i * m_scale.x,(-m_datas[i]) * m_rateY * m_scale.y + m_position.y),1,
				XCCS::lightBlackColor * m_color);
		}
		break;
	case XE::TYPE_0_MIDDLE:
		//描绘零线
		XRender::drawLine(XVector2(m_position.x,m_position.y + tmpSize.y * 0.5f),XVector2(m_position.x + tmpSize.x,m_position.y + tmpSize.y * 0.5f),1.0f,
			XCCS::blackColor * m_color);
		//下面描绘数据
		if(m_datas.size() > 0)
			XRender::drawLine(XVector2(m_position.x,m_position.y + tmpSize.y * 0.5f),XVector2(m_position.x + m_rateX * 0 * m_scale.x,(-m_datas[0]) * m_rateY * m_scale.y + m_position.y + tmpSize.y * 0.5f),1,
				XCCS::lightBlackColor * m_color);
		for(int i = 1;i < m_datas.size();++ i)
		{
			XRender::drawLine(XVector2(m_position.x + m_rateX * (i - 1) * m_scale.x,(-m_datas[i - 1]) * m_rateY * m_scale.y + m_position.y + tmpSize.y * 0.5f),
				XVector2(m_position.x + m_rateX * i * m_scale.x,(-m_datas[i]) * m_rateY * m_scale.y + m_position.y + tmpSize.y * 0.5f),1,
				XCCS::lightBlackColor * m_color);
		}
		break;
	case XE::TYPE_0_BOTTOM:
		//描绘零线
		XRender::drawLine(XVector2(m_position.x,m_position.y + tmpSize.y),XVector2(m_position.x + tmpSize.x,m_position.y + tmpSize.y),1.0f,
			XCCS::blackColor * m_color);
		//下面描绘数据
		if(m_datas.size() > 0)
			XRender::drawLine(XVector2(m_position.x,m_position.y + tmpSize.y),XVector2(m_position.x + m_rateX * 0 * m_scale.x,(-m_datas[0]) * m_rateY * m_scale.y + m_position.y + tmpSize.y),1,
				XCCS::lightBlackColor * m_color);
		for(int i = 1;i < m_datas.size();++ i)
		{
			XRender::drawLine(XVector2(m_position.x + m_rateX * (i - 1) * m_scale.x,(-m_datas[i - 1]) * m_rateY * m_scale.y + m_position.y + tmpSize.y),
				XVector2(m_position.x + m_rateX * i * m_scale.x,(-m_datas[i]) * m_rateY * m_scale.y + m_position.y + tmpSize.y),1,
				XCCS::lightBlackColor * m_color);
		}
		break;
	}
	if(m_showCaption) m_caption.draw();
	if(m_showInfo != INFO_NULL)
		m_infoFont.draw();
}
bool XSimpleChart::insertAData(float data)
{
	switch (m_simpleChartType)
	{
	case XE::TYPE_0_UP:
		if(data > 0.0f) 
		{
			LogStr("插入一个不符合需求的数据类型");
			return false;	//这会是一个错误
		}
		break;
	case XE::TYPE_0_MIDDLE:
		break;
	case XE::TYPE_0_BOTTOM:
		if(data < 0.0f) 
		{
			LogStr("插入一个不符合需求的数据类型");
			return false;	//这会是一个错误
		}
		break;
	}
	m_datas.push_back(data);
	if(m_datas.size() > m_maxDataSum)
		m_datas.pop_front();
	float maxData = 0.0f;
	for(int i = 0;i < m_datas.size();++ i)
	{
		if(abs(m_datas[i]) > maxData) maxData = abs(m_datas[i]);
	}
	switch (m_simpleChartType)
	{
	case XE::TYPE_0_MIDDLE:
		if(m_isAutoRateY || m_datas.size() == 1)
			m_rateY = m_size.y * 0.5f / maxData;
		else
		{
			float rate = m_size.y * 0.5f / maxData;
			if(rate < m_rateY) m_rateY = rate;
		}
		break;
	case XE::TYPE_0_UP:	//如果出现大于0的值也会是一个错误
	case XE::TYPE_0_BOTTOM:	//如果出现小于0的值将会是一个错误
		if(m_isAutoRateY || m_datas.size() == 1)
			m_rateY = m_size.y / maxData;
		else
		{
			float rate = m_size.y / maxData;
			if(rate < m_rateY) m_rateY = rate;
		}
		break;
	}
	if(m_isFirstPush)
	{
		m_maxData = m_minData = data;
		m_localMaxData = m_localMinData = data;
		m_localAverageData = data;
		m_localSDData = 0.0f;
	}else
	{
		if(m_maxData < data || m_minData > data)
		{//范围发生改变
			if(m_maxData < data) m_maxData = data;
			if(m_minData > data) m_minData = data;
		}
		switch (m_showInfo)
		{
		case INFO_LOCAL_RANGLE:
			m_localMaxData = m_localMinData = m_datas[0];
			for(int i = 1;i < m_datas.size();++ i)
			{
				if(m_datas[i] > m_localMaxData)
				{
					m_localMaxData = m_datas[i];
					continue;
				}
				if(m_datas[i] < m_localMinData)
				{
					m_localMinData = m_datas[i];
					continue;
				}
			}
			break;
		case INFO_LOCAL_AVERAGE:
			m_localAverageData = XMath::getAverageValue(m_datas);
			break;
		case INFO_LOCAL_SD:
			m_localSDData = XMath::getStandardDeviation(m_datas);
			break;
		}
	}
	m_curData = data;
	updateInfoStr();
	return true;
}
XBool XSimpleChart::init(const XFontUnicode &font,const XVector2 &size,float xRate)
{
	if(m_isInited) return false;
	if(xRate <= 0.0f || size.x <= 0.0f || size.y <= 0.0f) return false;
	if(!m_caption.setACopy(font)) return XFalse;
	if(!m_infoFont.setACopy(font)) return XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
	XObjManager.decreaseAObject(&m_infoFont);
#endif

	m_size = size;
	m_rateX = xRate;
	m_caption.setScale(m_scale);
	m_caption.setPosition(m_position.x + 1.0f * m_scale.x,m_position.y + 1.0f * m_scale.y);
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
	m_caption.setColor(XFColor::gray);
	m_infoFont.setScale(m_scale);
	m_infoFont.setPosition(m_position.x + 1.0f * m_scale.x,m_position.y + (1.0f + 32.0f) * m_scale.y);
	m_infoFont.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
	m_infoFont.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
	m_infoFont.setColor(XFColor::gray);
	m_infoFont.setString("Range:0.0 - 0.0");

	m_maxDataSum = m_size.x / m_rateX;

	m_curMouseRect.set(m_position,m_position + m_size * m_scale);

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;
	m_isInited = true;
	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif

	return true;
}
void XSimpleChart::release()
{
	clearAll();
	XCtrlManager.decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
}
#if !WITH_INLINE_FILE
#include "XSimpleChart.inl"
#endif
}