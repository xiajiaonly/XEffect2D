#ifndef _JIA_XRESOURCEPACK_
#define _JIA_XRESOURCEPACK_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//目前这里个类里面存在问题
//--------------------------------
#include "XCommonDefine.h"
#include "XMemory.h"

#include "stdio.h"
#include <memory>
#include "string.h"
namespace XE{
class XTextureInfo;

#pragma pack(push)
#pragma pack(1)
struct XResourceFileStruct
{
	char fileName[MAX_FILE_NAME_LENGTH];		//文件名称
	int fileLength;								//文件长度
	int filePosition;							//文件偏移坐标
};
#pragma pack(pop)

class XResourcePack
{
private:
	static const int m_maxFileSum = 3000;	//资源包中最多能包含的资源数量
	static const int m_lockCodeLength = 32;	//密钥的长度，密钥分成四个部分，每个部分32个bytes
	static const int m_allLockCodeLength = 128;	//密钥总长度

	static const int m_memoryChangeTimes = 36;
	static const int m_md5CodeSize = 16;
public:
	XResourcePack();
	virtual ~XResourcePack();
protected:
	XResourcePack(const XResourcePack&);
	XResourcePack &operator= (const XResourcePack&);
	//+++++++++++++++++++++++++++++++++++++++++++
	//2011年这里添加内存混乱技术
private:
	unsigned char *m_fileData;
	int m_fileDataLength;
	int m_fileDataPoint;
	//-------------------------------------------
public:
	XResourceFileStruct *m_fileInfo;						//最多能打包m_maxFileSum个文件
	int m_fileSum;											//资源包中的文件数量
	int m_normalFileSum;									//普通文件的数量
	XBool m_haveReadedFileList;								//是否已经读取索引信息
	XBool m_haveReadedNormalFileList;						//是否已经读取了普通文件索引信息
	long m_headLength;										//资源包中文件头的长度
	unsigned char tempCode[m_lockCodeLength];				//资源包中的加密码
	unsigned char m_lockKey[m_allLockCodeLength];			//掩码
	unsigned char m_hardLockKey[m_lockCodeLength];			//密钥部分中来自硬件的加密密钥

	char m_outFileName[MAX_FILE_NAME_LENGTH];				//输出文件的名字
	void setOutFileName(const char *temp = NULL);			//设置输出文件的名字
private:
	XBool m_isGetOptimizeInfo;
	XBool getOptimizeInfo();
	XBool releaseOptimizeInfo();
	XTextureInfo *m_texInfo;
	int *m_targetOrder;	//贴图与目标贴图之间的对应关系
	char *m_targetName;	//资源优化之后的目标名称
	int m_texInfoSum;
	int m_targetTextureSum;
public:
	int makeFileList();										//产生清单文件
	int makeResourcePack(int packMode = 0);					//打包资源，当packMode = 0时封装的是不需要经过加密和校验的资源。
															//			当packMode = 1时封装的是经过加密和校验的资源
	//目前下面这个函数对于压缩的资源还存在问题
	int unpackAllResource();								//解包所有的资源	//由于没有目录建立的处理，所以这里只能解压本地目录的文件
	int unpackResource(const char *fileName);				//解包某一个资源	//由于没有目录建立的处理，所以这里只能解压本地目录的文件
	int unpackResource(const char *fileName, unsigned char *p);	//解包某一个资源到一段内存空间
	int getStringLength(const char *p) const;				//获得字符串的长度
	void setHardLockKey(const void *p);						//设置硬件段加密密钥

	int checkFileCode(const char *p) const;					//检查文件特征码是否正确
	//int xFileNameCMP(char *p1,char *p2);					//比较两个文件路径是否相同

	//int getPathDeep(char *p);								//获取路径深度
	int readFileListFromResouce();							//从压缩包中读取文件索引信息		//这个函数需要重构
	int readNormalFileListFromResouce();					//从压缩包中读取普通文件的索引信息	//这个函数需要重构
	int getFileLength(const char *fileName);				//资源包中资源文件的大小
	unsigned char *getFileData(const char *filename);		//获取指定文件的文件内容

	int writeCheckData();	//向资源文件中写入校验和
	XBool checkCheckData();	//校验资源文件的校验和

	void lockOrUnlockProc(unsigned char *p, int startPoint, int length) const;	//对子数据p进行加密或者解密 startPoint为起始位置 length为数据长度
	void getlockKey();		//计算掩码

	int isOptimized(const char *p) const;	//判断这个文件是否被优化 0:没有参与优化 1:已经参与优化
	void release();	//释放资源
};
inline int XResourcePack::getStringLength(const char *p) const
{
	if(strlen(p) >= MAX_FILE_NAME_LENGTH) return 0;
	else return (int)(strlen(p));
//	for(int i=0;i < MAX_FILE_NAME_LENGTH;++ i)
//	{
//		if(p[i] == '\0') return i;
//	}
//	return 0;
}
inline void XResourcePack::setHardLockKey(const void *p)
{
	memcpy(m_hardLockKey,p,32);
}
inline XResourcePack::~XResourcePack()
{
	release();
}
inline void XResourcePack::release()
{
	XMem::XDELETE_ARRAY(m_fileInfo);
	XMem::XDELETE_ARRAY(m_fileData);
	m_haveReadedFileList = XFalse;
}
}
#endif