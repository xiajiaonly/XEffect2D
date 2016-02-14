#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XShareMem.h"
namespace XE{
#ifdef XEE_OS_WINDOWS
bool XShareMem::init(const char *name, int size)
{
	if (m_isInited ||
		size <= 0) return false;
	m_size = size;

	m_hMapFile = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, name);
	if (m_hMapFile == NULL)
	{
		if (!XEE::getSysPrivilege(SE_CREATE_GLOBAL_NAME)) return false;
		SECURITY_ATTRIBUTES sa;
		SECURITY_DESCRIPTOR sd;
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = &sd,
		sa.bInheritHandle = FALSE;
		InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
//		DWORD sidBufferSize = 100;
//		char domainBuffer[80];
//		DWORD domainBufferSize = 80;
//		SID_NAME_USE snu;
//		BYTE aclBuffer[1024];
//		PACL pacl=(PACL)&aclBuffer; //声明一个ACL，长度是1024
//		InitializeAcl(pacl, 1024, ACL_REVISION);//初始化一个ACL
//		LookupAccountName(0, "everyone", psid,
//			&sidBufferSize, domainBuffer,
//			&domainBufferSize, &snu);//查找一个用户everyone，并取该用户的SID
//		AddAccessAllowedAce(pacl, ACL_REVISION, GENERIC_ALL, psid);
//		SetSecurityDescriptorDacl(&sd, TRUE, pacl, FALSE);
		SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);
		//设置该用户的Access-Allowed的ACE，其权限为“所有权限”
		m_hMapFile = CreateFileMappingA(
			INVALID_HANDLE_VALUE,    // use paging file
			&sa,                    // default security
			PAGE_READWRITE,          // read/write access
			0,                       // maximum object size (high-order DWORD)
			size,                // maximum object size (low-order DWORD)
			name);                 // name of mapping object
		if (m_hMapFile == NULL) return false;
	}

	m_pAddress = (unsigned char *)MapViewOfFile(m_hMapFile, //映射内存地址到指针
		FILE_MAP_ALL_ACCESS,0,0,size);
	if(m_pAddress == NULL)
	{//映射失败
		CloseHandle(m_hMapFile);
		return false;
	}

	m_isInited = true;
	return true;
}
void XShareMem::release()
{
	if(!m_isInited) return;
	UnmapViewOfFile(m_pAddress);
	CloseHandle(m_hMapFile);
	m_isInited = false;
}
#endif

#ifdef XEE_OS_LINUX
bool XShareMem::init(const char *name,int size)
{
	if(m_isInited) return false;
	if(size <= 0) return false;
	m_size = size;
	void *shmaddr = NULL;
	if((m_shmid = shmget(IPC_PRIVATE,size,0666 | IPC_CREAT)) < 0)				//创建共享内存
//	if((m_prsem = sem_open(name,O_CREAT,0666,0)) == SEM_FAILED) return false;	//信号量用于控制读写	//读
//	//if((m_prsem = sem_open(name,O_CREAT,0666,1)) == SEM_FAILED) return false;	//信号量用于控制读写	//写
	if((shmaddr = shmat(m_shmid,NULL,0)) == (void *)-1) return false;			//映射内存地址
	m_pAddress = (unsigned char *)shmaddr;

	m_isInited = true;
	return true;
}
void XShareMem::release()
{
	if(!m_isInited) return;
	shmctl(m_shmid,IPC_RMID,NULL);	//删除共享内存
	m_isInited = false;
}
#endif
}