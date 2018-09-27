#ifndef _JIA_XHARDWAREID_
#define _JIA_XHARDWAREID_
//这里包含的内容非常广泛，目前只做简单的处理
#include <string>
struct IWbemLocator;
struct IWbemServices;
namespace XE{
namespace XHardware
{
extern std::string getCPUVID();		//12个有效位
extern std::string getCPUBrand();	//48个有效位
//addr必须包含6个字节的空间
extern bool getMACAddress(unsigned char *addr);	//获取网卡硬地址
};
//通过WMI来获取硬件信息的类
class XHardwareInfo
{
private:
	bool m_isInited;
	IWbemLocator *m_pLoc;
	IWbemServices *m_pSvc;
public:
	bool init();	//初始化
	void release();
	std::string getHardDiskID();	//获取硬盘ID
	std::string getMotherboardProduct();	//获取主板型号

	XHardwareInfo()
		:m_isInited(false)
		,m_pLoc(NULL)
		,m_pSvc(NULL)
	{}
	~XHardwareInfo(){release();}
};
}
#endif