#ifndef XRADAR_DATA
#define XRADAR_DATA
namespace XE {
enum XRadarDataType
{
	RD_TYPE_NULL,		//无效的数据
	RD_TYPE_BLOB,		//有效的数据
};
struct XRadarData
{
	float angle;		//角度，单位：度
	float distance;		//距离，单位：毫米
	XVec2 pos;		//当前的位置,单位：毫米
	XRadarDataType type;
	float r;			//有效半径
	float intensity;	//数据的可靠性
	XRadarData()
		:angle(0.0f)
		, distance(0.0f)
		, type(RD_TYPE_NULL)
		, pos(0.0f)
		, r(1.0)
		, intensity(0.0f)
	{}
	void set(float a, float d)
	{
		angle = a;
		distance = d;
		type = RD_TYPE_BLOB;
	}
};
enum XRadarType
{
	TYPE_SILAN,		//思岚雷达
	TYPE_BEIYANG,	//北阳雷达
	TYPE_FASHI,		//砝石雷达
};
class XRadarBase
{
protected:
public:
	virtual void getRadarData(std::vector<XRadarData> &data) = 0;
	virtual bool update() = 0;
	virtual ~XRadarBase() {}
};

}
#endif