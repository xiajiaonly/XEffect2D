#ifndef _JIA_XBABYBASE_
#define _JIA_XBABYBASE_
namespace XE{
class XBabyBase
{
protected:
public:
	XBabyBase(){}
	virtual ~XBabyBase(){}
	virtual bool reset() = 0;	//复位
	virtual bool needReset() = 0;	//是否需要启用复位
};
}
#endif