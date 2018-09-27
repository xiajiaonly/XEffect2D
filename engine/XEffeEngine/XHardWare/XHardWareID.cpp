#include "XStdHead.h"
#include "XHardWareID.h"
#include <IPHlpApi.h>
#include <winioctl.h>
#pragma comment(lib, "iphlpapi.lib")
#define _WIN32_DCOM
#include <iostream>
using namespace std;
#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")
#include <algorithm>
#include <functional>
#include <cctype>
//#ifndef _WIN64
#include <intrin.h>
//#endif
//去掉行尾的空格
static inline std::string &ltrim(std::string &s)
{ 
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace)))); 
	return s; 
} 
//去掉行尾的空格
static inline std::string &rtrim(std::string &s) 
{ 
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end()); 
	return s; 
} 
//去掉行头行尾的空格
static inline std::string &trim(std::string &s) 
{ 
	return ltrim(rtrim(s)); 
}

namespace XE
{
namespace XHardware
{
void executecpuid(DWORD veax,DWORD data[4])
{// The following 4 DWORD variable store the four registers values after cpuid
//#ifndef _WIN64
	__cpuid((int *)data,veax);
//#endif
	//x64编译时不能直接使用汇编__asm
	//DWORD deax,debx,decx,dedx;
	//__asm
	//{
	//	mov eax, veax
	//	cpuid
	//	mov deax, eax
	//	mov debx, ebx
	//	mov decx, ecx
	//	mov dedx, edx
	//}
	//data[0] = deax;	// store in class member
	//data[1] = debx;
	//data[2] = decx;
	//data[3] = dedx;
}
std::string getCPUVID()
{
	DWORD data[4];
	char cVID[13];				// Store Vender ID String, contain 12 chars
	memset(cVID, 0, 13);		// Initialize all chars to 0
	executecpuid(0,data);			// Execute cpuid instruction with eax = 0
	memcpy(cVID, &data[1], 4);	// ebx contains the first 4 chars
	memcpy(cVID+4, &data[3], 4);	// edx contains the middle 4 chars
	memcpy(cVID+8, &data[2], 4);	// ecx contains the last 4 chars;

	return cVID;		// convert to string and return
}
std::string getCPUBrand()
{
	DWORD data[4];
	const DWORD BRANDID = 0x80000002;		// brand parameter start at 0x80000002
	char cBrand[49];						// Store Brand String, contain 48 chars
	memset(cBrand, 0, 49);					// Initialize all chars to 0

	for (DWORD i = 0; i < 3; i++)			// Execute cpuid instruction with 
	{										// eax = 0x80000002 to 0x80000004
		executecpuid(BRANDID + i,data);			
		memcpy(cBrand + i*16, &data[0], 16);	// eax, ebx, ecx, edx contain the brand string
	}
	std::string tmpStr = cBrand;
	return trim(tmpStr);		// convert to string and return
}
bool getMACAddress(unsigned char *addr)	//获取网卡硬地址
{
	if(addr == NULL) return false;
    DWORD nRet;
    //只查询物理地址
    DWORD nFlags = GAA_FLAG_SKIP_UNICAST 
        | GAA_FLAG_SKIP_ANYCAST
        | GAA_FLAG_SKIP_FRIENDLY_NAME
        | GAA_FLAG_SKIP_MULTICAST
        | GAA_FLAG_SKIP_DNS_SERVER;

    ULONG bufLen = 1024;
    PIP_ADAPTER_ADDRESSES pAdapterAddr = (PIP_ADAPTER_ADDRESSES)malloc(bufLen);
    if(pAdapterAddr == NULL)
        return false;

    //AF_INET: return only IPv4 addresses.
    nRet = GetAdaptersAddresses(AF_INET, nFlags,  NULL, pAdapterAddr, &bufLen);
    if(nRet == ERROR_BUFFER_OVERFLOW)
    {
        pAdapterAddr = (PIP_ADAPTER_ADDRESSES)realloc(pAdapterAddr, bufLen);
        if(pAdapterAddr == NULL)
            return false;

        nRet = GetAdaptersAddresses(AF_INET, nFlags,  NULL, pAdapterAddr, &bufLen);
    }

    if(nRet == ERROR_SUCCESS)
    {
        memcpy(addr, &pAdapterAddr->PhysicalAddress, pAdapterAddr->PhysicalAddressLength);
        free(pAdapterAddr);
        return true;
    }else
    {
        //ff-ff-ff-ff-ff-ff: 表示获取失败（未知）
        memset(addr, 0xff, 6);
        free(pAdapterAddr);
        return false;
    }
//	return true;
}
}
bool XHardwareInfo::init()
{
	if(m_isInited) return false;
#if _WIN32_WINNT >= 0x0501
	// Step 1:Initialize COM. --------------------------------------------------
	HRESULT hres =  CoInitializeEx(0, COINIT_MULTITHREADED); 
	if (FAILED(hres))
	{
		std::cout << "Failed to initialize COM library. Error code = 0x" 
			<< hex << hres << std::endl;
		return false;                  // Program has failed.
	}
	// Step 2:Set general COM security levels --------------------------------------------------
	hres =  CoInitializeSecurity(
		NULL, 
		-1,                          // COM authentication
		NULL,                        // Authentication services
		NULL,                        // Reserved
		RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
		RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
		NULL,                        // Authentication info
		EOAC_NONE,                   // Additional capabilities 
		NULL                         // Reserved
		);    
	if (FAILED(hres))
	{
		std::cout << "Failed to initialize security. Error code = 0x" 
			<< hex << hres << std::endl;
		CoUninitialize();
		return false;                    // Program has failed.
	}
	// Step 3: Obtain the initial locator to WMI ---------------------------------------------------
	hres = CoCreateInstance(
		CLSID_WbemLocator,             
		0, 
		CLSCTX_INPROC_SERVER, 
		IID_IWbemLocator, (LPVOID *) &m_pLoc); 
	if (FAILED(hres))
	{
		std::cout << "Failed to create IWbemLocator object."
			<< " Err code = 0x"
			<< hex << hres << std::endl;
		CoUninitialize();
		return false;                 // Program has failed.
	}
	// Step 4:Connect to WMI through the IWbemLocator::ConnectServer method -----------------------------------------------------
	// Connect to the root\cimv2 namespace with
	// the current user and obtain pointer pSvc
	// to make IWbemServices calls.
	hres = m_pLoc->ConnectServer(
			_bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
			NULL,                    // User name. NULL = current user
			NULL,                    // User password. NULL = current
			0,                       // Locale. NULL indicates current
			NULL,                    // Security flags.
			0,                       // Authority (for example, Kerberos)
			0,                       // Context object 
			&m_pSvc                    // pointer to IWbemServices proxy
			);
    
	if (FAILED(hres))
	{
		std::cout << "Could not connect. Error code = 0x" 
				<< hex << hres << std::endl;
		m_pLoc->Release();     
		CoUninitialize();
		return false;                // Program has failed.
	}
	// Step 5:Set security levels on the proxy --------------------------------------------------
	hres = CoSetProxyBlanket(
		m_pSvc,                        // Indicates the proxy to set
		RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
		RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
		NULL,                        // Server principal name 
		RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
		RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
		NULL,                        // client identity
		EOAC_NONE                    // proxy capabilities 
	);
	if (FAILED(hres))
	{
		std::cout << "Could not set proxy blanket. Error code = 0x" 
			<< hex << hres << std::endl;
		m_pSvc->Release();
		m_pLoc->Release();     
		CoUninitialize();
		return false;               // Program has failed.
	}
#else
	return false;
#endif
	m_isInited = true;
	return true;
}
std::string XHardwareInfo::getHardDiskID()	//获取硬盘ID
{
	if(!m_isInited) return XString::gNullStr;
	std::string retString = XString::gNullStr;
	IEnumWbemClassObject* pEnumerator = NULL;
	//HRESULT hres = 
		m_pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t("SELECT SerialNumber FROM Win32_PhysicalMedia"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, 
		NULL,&pEnumerator);
	IWbemClassObject *pclsObj = NULL;
	ULONG uReturn = 0;
	while (pEnumerator)
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, 
				&pclsObj, &uReturn);
		if(0 == uReturn) break;
		VARIANT vtProp;
		hr = pclsObj->Get(L"SerialNumber", 0, &vtProp, 0, 0);
		if (SUCCEEDED(hr) && vtProp.bstrVal != NULL) 
			retString = (const char*) _bstr_t(vtProp.bstrVal);
		
		VariantClear(&vtProp);

        pclsObj->Release();
	}
    pEnumerator->Release();
	return trim(retString);
}
std::string XHardwareInfo::getMotherboardProduct()	//获取主板型号
{
	if(!m_isInited) return XString::gNullStr;
	std::string retString = XString::gNullStr;
	IEnumWbemClassObject* pEnumerator = NULL;
	//HRESULT hres = 
		m_pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t("SELECT Product from Win32_BaseBoard"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, 
		NULL,&pEnumerator);
	IWbemClassObject *pclsObj = NULL;
	ULONG uReturn = 0;
	while (pEnumerator)
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, 
				&pclsObj, &uReturn);
		if(0 == uReturn) break;
		VARIANT vtProp;
		hr = pclsObj->Get(L"Product", 0, &vtProp, 0, 0);
		if (SUCCEEDED(hr) && vtProp.bstrVal != NULL) 
			retString = (const char*) _bstr_t(vtProp.bstrVal);
		
		VariantClear(&vtProp);

        pclsObj->Release();
	}
    pEnumerator->Release();
	return trim(retString);
}
void XHardwareInfo::release()
{
	if(!m_isInited) return;
	m_pSvc->Release();
	m_pLoc->Release();
	CoUninitialize();
	m_isInited = false;
}
}