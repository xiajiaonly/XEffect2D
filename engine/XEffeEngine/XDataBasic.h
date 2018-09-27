#ifndef _JIA_XDATABASIC_
#define _JIA_XDATABASIC_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------
//#include "stdio.h" 
//#include "XBasicClass.h"
#include "XThread.h"
namespace XE{
//这是一个数据转换、存储、格式化、加密、传输的类
//为了数据的安全，数据使用AB轮流保存的方式，可以较大范围的确保数据有一个正切的备档
#define DATABASIC_DEFAULT_FILE_NAME_A "databasicA.dat"
#define DATABASIC_DEFAULT_FILE_NAME_B "databasicB.dat"

enum XAnalyzingFileMode
{
    ANALYZING_BUFFERING,    //缓冲保存    //数据不完全，硬件安全高效
    ANALYZING_IMMEDIATELY    //即时保存    //数据安全，硬件损耗
};
class XDataBasic
{
private:
	static const int m_databasicKeyLength = 128;
    int m_ID;                    //这个数据的ID，用于大量数据管理
    int m_versionsOrder;        //用于记录数据的版本，每次数据的改变都会改变其版本，以便于确定其文件的先后顺序
    int m_dataLength;            //数据长度
    unsigned char *m_pData;        //数据
    unsigned char m_checkData;    //校验字符
    char m_curBackOrder;        //当前操作的备档编号
    XBool m_isInited;            //是否初始化
	XBool m_isSaveAsynchronous;	//是否使用异步数据保存
	XBool m_needSave;			//数据是否需要保存
    int m_secretKeyLength;        //密钥长度
    unsigned char *m_secretKey;    //加密使用的密钥（这里可以进行简单的加密和校验）

    XBool EncryptData();            //加密数据
    XBool DeCryptData();            //解密数据
    unsigned char getCheckData();        //计算校验码
    XBool checkDataIsRigth();    //检查数据是否符合校验

    XBool mergeDataBack(const char *fileName);    //这个函数用于内部调用，有助于清晰化代码逻辑
    XBool mergeDataBack(const char *fileNameA,const char *fileNameB);    //这个函数用于内部调用，有助于清晰化代码逻辑
    XBool analyzingDataBack(const char *fileName);    //这个函数用于内部调用，有助于清晰化代码逻辑
    XBool analyzingData(const char *fileName);    //将数据保存到文件

	char *m_fileName;			//用于保存数据的文件名
	
	XAnalyzingFileMode m_saveMode;	//文件的存储模式

	XThreadState m_isEnd;	//是否结束
	
	XDataBasic(const XDataBasic &)	//重载拷贝构造函数为私有防止不必要的错误
	{}
public:
    XDataBasic();
	~XDataBasic(){release();}
	void release();

    XBool init(int ID,XBool isSaveAsynchronous = XFalse,XAnalyzingFileMode saveMode = ANALYZING_BUFFERING);
    //内存与数据间的转换
    XBool mergeData(const void *pData,int dataLength);        //从外部合并数据到内部
    int analyzingData(void *pData,int dataLength);			//将内部数据解析到外部,返回内部数据的大小，-1为错误
    //文件与数据间的转换
    XBool mergeData(const char *fileName = NULL);            //从文件中提取数据
    XBool analyzingDataManager(const char *fileName = NULL);        //将数据保存到文件，这个函数会根据之前用户的设置对保存的具体操作进行调度
    //网络与数据间的转换(目前暂时并不提供这个支持)

	//异步数据保存线程
#ifdef XEE_OS_WINDOWS
	static DWORD WINAPI saveDataThread(void * pParam);
#endif
#ifdef XEE_OS_LINUX
	static void *saveDataThread(void * pParam); 
#endif 
	pthread_t m_saveDataThreadP;

	void endDataBasic();	//退出时关闭线程
};
inline void XDataBasic::endDataBasic()
{
    if(!m_isInited) return;
	if(m_isSaveAsynchronous) m_isEnd = STATE_SET_TO_END;
}
inline XBool XDataBasic::checkDataIsRigth()    //检查数据是否符合校验
{
    if(!m_isInited ||
		m_dataLength <= 0 ||
		m_pData == NULL) return XFalse;
    return getCheckData() == m_checkData;
}
inline void XDataBasic::release()
{
	if(!m_isInited) return;
	if(m_isSaveAsynchronous) waitThreadEnd(m_isEnd);//等待线程结束
	XMem::XDELETE_ARRAY(m_secretKey);
	m_isInited = XFalse;
}
inline int XDataBasic::analyzingData(void *pData,int dataLength)    //将内部数据解析到外部
{
    if(!m_isInited ||
		dataLength <= 0) return -1;
    if(m_pData == NULL || m_dataLength <= 0) return 0;
    if(dataLength <= m_dataLength) memcpy(pData,m_pData,dataLength);
    else memcpy(pData,m_pData,m_dataLength);
    return m_dataLength;
}
}
#endif