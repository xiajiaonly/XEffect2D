//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
//#include "stdafx.h"
#include "XResourcePack.h"
#include "XResourceOptimize.h"
#include "stdlib.h"
#include "stdio.h"
#include "XBasicWindow.h"
#include "XMD5.h"
#ifdef XEE_OS_WINDOWS
#include "windows.h"
#endif

#include "XTextureInformation.h"	//为了实现资源反压缩而引入的头文件

#define LOCK_DATA_PART_1 ("~!si2e69,;1'5/1io&*%!*(&@%onjkli")
#define MEMORY_CHANGE_TIMES (36)
#define MD5_CODE_SIZE (16)

//强化建议加入加密和压缩
#define FILE_LIST_TEXT_NAME ("ResourceFileList.txt")
#define OUT_FILE_NAME ("Resource.dat")
#define FILE_CODE  ("SP:xiajia_1981@163.com")

int _XResourcePack::makeResourcePack(int packMode)
{
	FILE *fpTemp = NULL;
	FILE *fp = NULL;
	//打开文件列表
	if((fp = fopen(FILE_LIST_TEXT_NAME,"r")) == NULL)
	{
#ifdef DEBUG_MODE
		printf("File list file open error!\n");
#endif
		return -1;
	}
	//完成文件列表的信息
	m_fileSum = 0;	//实际操作的文件数量
	char fileNameTemp[MAX_FILE_NAME_LENGTH];
	char fileNameTemp1[MAX_FILE_NAME_LENGTH];
	_XBool flag = XTrue;
	for(int i = 0;i < MAX_FILE_SUM;++ i)
	{
		while(true)
		{
			if(feof(fp))
			{
				flag = XFalse;
				break;
			}

			fileNameTemp[0] = '\0';
			fscanf(fp,"%s",fileNameTemp);
			if(fileNameTemp[0]  == '\0') continue;
			int tempLen = getStringLength(fileNameTemp);
			if(tempLen < 4) continue;
			if(packMode == 0)
			{//不加密的文件
				if(fileNameTemp[tempLen - 4]  != '.') continue;
				strcpy(fileNameTemp1,fileNameTemp);
				//tempLen = strlen(fileNameTemp1);
				for(int j = 0;j < tempLen;++j)
				{
					if(fileNameTemp1[j] == '/' || fileNameTemp1[j] == '\\')
					{
						fileNameTemp1[j] = '\0';
					}
				}
				if(!fileNameCompare(fileNameTemp1,"normalResource")) continue;	//只有normalResource目录下面的文件才会经过这个处理
			}else
			{
				if(fileNameCompare(fileNameTemp,"ResourcePack.exe")) continue;			//去掉可执行文件自身
				if(fileNameCompare(fileNameTemp,"ResourceFileList.txt")) continue;		//去掉文件列表文档
				if(fileNameCompare(fileNameTemp,"Resource.dat")) continue;				//去掉资源包文件
				if(fileNameTemp[tempLen - 4]  != '.') continue;		//这个可以不必要如此处理,这一行的目的是为了去掉文件夹，保证选取的都是文件
				//下面去掉资源优化产生的几个临时文件
				if(fileNameCompare(fileNameTemp,PNG_FILE_LIST_NAME)) continue;	//去掉资源优化中间文件
				if(fileNameCompare(fileNameTemp,"!!BasePath.png")) continue;	//去掉资源优化中间文件
				if(fileNameCompare(fileNameTemp,"logBook.txt")) continue;		//去掉资源优化中间文件
				//DLL文件不需要打包
				if(fileNameTemp[tempLen - 4] == '.'
					&& (fileNameTemp[tempLen - 3] == 'D' || fileNameTemp[tempLen - 3] == 'd')
					&& (fileNameTemp[tempLen - 2] == 'L' || fileNameTemp[tempLen - 2] == 'l')
					&& (fileNameTemp[tempLen - 1] == 'L' || fileNameTemp[tempLen - 1] == 'l')) continue;
				//取消已经处理过的文件
				if(isOptimized(fileNameTemp) == 1) continue;

				strcpy(fileNameTemp1,fileNameTemp);
				//tempLen = strlen(fileNameTemp1);
				for(int j = 0;j < tempLen;++j)
				{
					if(fileNameTemp1[j] == '/' || fileNameTemp1[j] == '\\')
					{
						fileNameTemp1[j] = '\0';
					}
				}
				if(fileNameCompare(fileNameTemp1,"normalResource")) continue;	//normalResource目录下的文件不会经过这个处理
			}
			break;
		}
		if(!flag) break;
		memset(m_fileInfo[i].fileName,0,sizeof(m_fileInfo[i].fileName));	//初始化内存空间(避免UMR错误)，实际上并没有必要
		memcpy(m_fileInfo[i].fileName,fileNameTemp,sizeof(fileNameTemp));
	//	printf("%s\n",m_fileInfo[i].fileName);

		if((fpTemp = fopen(m_fileInfo[i].fileName,"r")) == NULL)
		{
#ifdef DEBUG_MODE
			printf("File list file text error!\n");
#endif
			return -1;
		}
		fseek(fpTemp,0,SEEK_END);
		m_fileInfo[i].fileLength = ftell(fpTemp);	//获取文件的长度
		fseek(fpTemp,0,SEEK_SET);
		fclose(fpTemp);
		m_fileSum ++;
	}
	fclose(fp);
	if(flag)
	{
#ifdef DEBUG_MODE
		printf("Need packed files is more than %d.Pack filed!\n",MAX_FILE_SUM);
#endif
		return 0;
	}
	//开始打包文件
	if(packMode == 0)
	{//不加密的文件放在资源文件的头部，所以需要以全新写的方式打开文件
		if((fp = fopen(m_outFileName,"wb")) == NULL)
		{
			if((fp = fopen(m_outFileName,"ab")) == NULL)
			{
#ifdef DEBUG_MODE
				printf("File list file open error!\n");
#endif
				return -1;
			}
		}
		//写入包头长度
		m_headLength = m_fileSum * sizeof(_XResourceFileStruct);
		fwrite(&m_headLength,sizeof(m_headLength),1,fp);		//4bytes
		//写入包头加上文件的总长度
		for(int i = 0;i < m_fileSum;++ i)
		{
			m_headLength += m_fileInfo[i].fileLength;
		}
		fwrite(&m_headLength,sizeof(m_headLength),1,fp);		//4bytes
		//写入包头内容
		for(int i = 0;i < m_fileSum;++ i)
		{
			if(i > 0)
			{
				m_fileInfo[i].filePosition = m_fileInfo[i-1].filePosition + m_fileInfo[i-1].fileLength;
			}else
			{
				m_fileInfo[i].filePosition = sizeof(m_headLength) + sizeof(m_headLength) + m_fileSum * sizeof(_XResourceFileStruct);
			}
			fwrite(m_fileInfo[i].fileName,sizeof(m_fileInfo[i].fileName),1,fp);	
			fwrite(&(m_fileInfo[i].fileLength),sizeof(m_fileInfo[i].fileLength),1,fp);	
			fwrite(&(m_fileInfo[i].filePosition),sizeof(m_fileInfo[i].filePosition),1,fp);	

			//printf("%s,%ld,%ld,%ld\n",m_fileInfo[i].fileName,m_fileInfo[i].fileLength,m_fileInfo[i].filePosition,ftell(fp));
#ifdef DEBUG_MODE
			printf("File %s packed.\n",m_fileInfo[i].fileName);
#endif
		}
		//写入文件内容
		unsigned char tempFileData[2048];
		int readLength = 0;
		for(int i = 0;i < m_fileSum;++ i)
		{
			if((fpTemp = fopen(m_fileInfo[i].fileName,"rb")) == NULL)
			{
#ifdef DEBUG_MODE
				printf("Resource file read error!\n");
#endif
				return -1;
			}
			readLength = 0;
			//printf"%d\n",ftell(fp));
			while(true)
			{//复制文件
				if(m_fileInfo[i].fileLength - readLength > 2048)
				{
					fread(tempFileData,2048,1,fpTemp);
					fwrite(tempFileData,2048,1,fp);		
					readLength += 2048;
				}else
				{
					fread(tempFileData,m_fileInfo[i].fileLength - readLength,1,fpTemp);
					fwrite(tempFileData,m_fileInfo[i].fileLength - readLength,1,fp);	
					fclose(fpTemp);
					break;
				}
			}
		}
		//结束
		fclose(fp);
	}else
	{//加密的文件，放在文件尾部，所以需要以追加的方式打开文件
		if((fp = fopen(m_outFileName,"ab")) == NULL)
		{
#ifdef DEBUG_MODE
			printf("File list file open error!\n");
#endif
			return -1;
		}
		//写入文件说明
		char temp[] = FILE_CODE;
		fwrite(temp,sizeof(temp),1,fp);				//23bytes
		//printf("%d\n",ftell(fp));
		//写入包头长度
		m_headLength = LOCK_CODE_LENGTH + m_fileSum * sizeof(_XResourceFileStruct);
		fwrite(&m_headLength,sizeof(m_headLength),1,fp);		//4bytes
		//printf"%d\n",ftell(fp));
		//写入32bytes的随机码
		for(int i = 0;i < LOCK_CODE_LENGTH;++ i)
		{
			tempCode[i] = rand()%256;
		}
		fwrite(tempCode,LOCK_CODE_LENGTH,1,fp);		//32bytes
		getlockKey();	//计算掩码
		//printf"%d,%d\n",ftell(fp),m_headLength);
		int tempFilePosition = sizeof(FILE_CODE) + sizeof(long) + LOCK_CODE_LENGTH;
		//写入包头内容
		for(int i = 0;i < m_fileSum;++ i)
		{
			if(i > 0)
			{
			//	m_fileInfo[i].filePosition = sizeof(FILE_CODE) + sizeof(long) + m_headLength + m_fileInfo[i-1].fileLength;
				m_fileInfo[i].filePosition = m_fileInfo[i-1].filePosition + m_fileInfo[i-1].fileLength;
			}else
			{
				m_fileInfo[i].filePosition = sizeof(FILE_CODE) + sizeof(long) + m_headLength;
			}
			lockOrUnlockProc((unsigned char *)m_fileInfo[i].fileName,tempFilePosition,sizeof(m_fileInfo[i].fileName));	//加密
			fwrite(m_fileInfo[i].fileName,sizeof(m_fileInfo[i].fileName),1,fp);	
			lockOrUnlockProc((unsigned char *)m_fileInfo[i].fileName,tempFilePosition,sizeof(m_fileInfo[i].fileName));	//还原
			tempFilePosition += sizeof(m_fileInfo[i].fileName);

			lockOrUnlockProc((unsigned char *)&(m_fileInfo[i].fileLength),tempFilePosition,sizeof(m_fileInfo[i].fileLength));	//加密
			fwrite(&(m_fileInfo[i].fileLength),sizeof(m_fileInfo[i].fileLength),1,fp);	
			lockOrUnlockProc((unsigned char *)&(m_fileInfo[i].fileLength),tempFilePosition,sizeof(m_fileInfo[i].fileLength));	//还原
			tempFilePosition += sizeof(m_fileInfo[i].fileLength);

			lockOrUnlockProc((unsigned char *)&(m_fileInfo[i].filePosition),tempFilePosition,sizeof(m_fileInfo[i].filePosition));	//加密
			fwrite(&(m_fileInfo[i].filePosition),sizeof(m_fileInfo[i].filePosition),1,fp);	
			lockOrUnlockProc((unsigned char *)&(m_fileInfo[i].filePosition),tempFilePosition,sizeof(m_fileInfo[i].filePosition));	//还原
			tempFilePosition += sizeof(m_fileInfo[i].filePosition);

			//printf("%s,%ld,%ld,%ld\n",m_fileInfo[i].fileName,m_fileInfo[i].fileLength,m_fileInfo[i].filePosition,ftell(fp));
#ifdef DEBUG_MODE
			printf("File %s packed.\n",m_fileInfo[i].fileName);
#endif
		}
		//写入文件内容
		unsigned char tempFileData[2048];
		int readLength = 0;
		for(int i = 0;i < m_fileSum;++ i)
		{
			if((fpTemp = fopen(m_fileInfo[i].fileName,"rb")) == NULL)
			{
#ifdef DEBUG_MODE
				printf("Resource file read error!\n");
#endif
				return -1;
			}
			readLength = 0;
			//printf"%d\n",ftell(fp));
			while(true)
			{//复制文件
				if(m_fileInfo[i].fileLength - readLength > 2048)
				{
					fread(tempFileData,2048,1,fpTemp);

					//lockOrUnlockProc(tempFileData,ftell(fp),2048);
					lockOrUnlockProc(tempFileData,tempFilePosition,2048);
					tempFilePosition += 2048;
					fwrite(tempFileData,2048,1,fp);		
					readLength += 2048;
				}else
				{
					fread(tempFileData,m_fileInfo[i].fileLength - readLength,1,fpTemp);

					//lockOrUnlockProc(tempFileData,ftell(fp),m_fileInfo[i].fileLength - readLength);
					lockOrUnlockProc(tempFileData,tempFilePosition,m_fileInfo[i].fileLength - readLength);
					tempFilePosition += m_fileInfo[i].fileLength - readLength;
					fwrite(tempFileData,m_fileInfo[i].fileLength - readLength,1,fp);	
					fclose(fpTemp);
					break;
				}
			}
		}
		//结束
		fclose(fp);
		writeCheckData();	
	}

#ifdef DEBUG_MODE
	if(packMode == 0) printf("Normal resource packed finished,%d file packed!\n",m_fileSum);
	else printf("Other resource packed finished,%d file packed!\n",m_fileSum);
#endif
	return 1;
}

int _XResourcePack::isOptimized(const char *p) const
{
	FILE *fp;
	if((fp = fopen(PNG_FILE_LIST_NAME,"r")) == NULL)
	{
	//	printf("Resource file open error!\n");
		return 0;
	}
	char temp[MAX_FILE_NAME_LENGTH];
	for(;;)
	{
		if(feof(fp)) break;
		if(fscanf(fp,"%s",temp) != 1) break;
		if(fileNameCompare(p,temp))
		{
			fclose(fp);
			return 1;
		}
	}
	fclose(fp);
	return 0;
}

int _XResourcePack::unpackAllResource()
{
	if(!m_haveReadedNormalFileList)
	{
		if(readNormalFileListFromResouce() != 1) 
		{
#ifdef DEBUG_MODE
			printf("Read file list from resouce error!\n");
#endif
			return -1;
		}
	}
	if(!m_haveReadedFileList) 
	{
		if(readFileListFromResouce() != 1) 
		{
#ifdef DEBUG_MODE
			printf("Read file list from resouce error!\n");
#endif
			return -1;
		}
	}
	if(!m_isGetOptimizeInfo)
	{
		if(!getOptimizeInfo())
		{
#ifdef DEBUG_MODE
			printf("Read resource optimize information error!\n");
#endif
			return 0;
		}
	}
	FILE *fpTemp;
	unsigned char * tempFileData = NULL;
	int needFileOrder = -1;
	for(int i = 0;i < m_fileSum;++ i)
	{
		//这里要处理掉优化的资源
		needFileOrder = 0;
		for(int j = 0;j < m_texInfoSum;++ j)
		{
			if(fileNameCompare(m_targetName + m_targetOrder[j] * MAX_FILE_NAME_LENGTH,m_fileInfo[i].fileName))
			{
				needFileOrder = 1;
				break;
			}
		}
		if(needFileOrder == 1) continue;
		tempFileData = createArrayMem<unsigned char>(m_fileInfo[i].fileLength);
		if(tempFileData == NULL) return -1;

		if((fpTemp = fopen(m_fileInfo[i].fileName,"wb")) == NULL)
		{
			if((fpTemp = fopen(m_fileInfo[i].fileName,"ab")) == NULL)
			{
#ifdef DEBUG_MODE
				printf("File list open error!\n");
#endif
				return -1;
			}
		}
		m_fileDataPoint = m_fileInfo[i].filePosition;
		memcpy(tempFileData,m_fileData + m_fileDataPoint,m_fileInfo[i].fileLength);
		//fread(tempFileData,m_fileInfo[i].fileLength,1,fp);
		lockOrUnlockProc(tempFileData,m_fileInfo[i].filePosition,m_fileInfo[i].fileLength);

		fwrite(tempFileData,m_fileInfo[i].fileLength,1,fpTemp);	

		//结束
		fclose(fpTemp);
		XDELETE_ARRAY(tempFileData);
	}
#ifdef DEBUG_MODE
	printf("下面进入压缩文件还原：%d\n",m_texInfoSum);
#endif
	//下面需要处理所有压缩后的资源
	for(int i = 0;i < m_texInfoSum;++ i)
	{
#ifdef DEBUG_MODE
		printf("还原：%s\n %d:%d\n",m_texInfo[i].textureName,i,m_texInfoSum);
#endif
		if(unpackResource(m_texInfo[i].textureName) == 0) return 0;
	}
#ifdef DEBUG_MODE
	printf("All file unpack finished!\n");
#endif
	return 1;
}

int _XResourcePack::checkFileCode(const char *p) const
{
	char temp[] = FILE_CODE;
	for(int i = 0;i < sizeof(temp);++ i)
	{
		if(p[i] != temp[i]) return -1;
	}
	return 1;
}
_XBool _XResourcePack::getOptimizeInfo()
{
	if(m_isGetOptimizeInfo) return XTrue;
	//从资源包中提取压缩文件信息
	int lengthTemp = getFileLength(PNG_INFORMATION_FILE_NAME);
	if(lengthTemp < 0) return XFalse;
	unsigned char *p = createArrayMem<unsigned char>(lengthTemp);
	if(p == NULL) return XFalse;

	//从资源包中提取出资源文件的内容
	unpackResource(PNG_INFORMATION_FILE_NAME,p);
	//解析一下内容
	int offset = 0;
	if(sscanf((char *)(p + offset),"%d,",&m_texInfoSum) != 1) {XDELETE_ARRAY(p);return XFalse;}
	offset += getCharPosition((char *)(p + offset),',') + 1;
	if(sscanf((char *)(p + offset),"%d;",&m_targetTextureSum) != 1) {XDELETE_ARRAY(p);return XFalse;}
	offset += getCharPosition((char *)(p + offset),';') + 1;
	if(m_texInfoSum <= 0 || m_targetTextureSum <= 0)
	{
		XDELETE_ARRAY(p);
		return XFalse;
	}
	m_texInfo = createArrayMem<_XTextureInfo>(m_texInfoSum);
	if(m_texInfo == NULL)
	{
		XDELETE_ARRAY(p);
		return XFalse;
	}
	m_targetOrder = createArrayMem<int>(m_texInfoSum);
	if(m_targetOrder == NULL)
	{
		XDELETE_ARRAY(p);
		XDELETE_ARRAY(m_texInfo);
		return XFalse;
	}
	for(int i = 0;i < m_texInfoSum;++ i)
	{
		m_targetOrder[i] = 0;
	}
	m_targetName = createArrayMem<char>(m_targetTextureSum * MAX_FILE_NAME_LENGTH);
	if(m_targetName == NULL)
	{
		XDELETE_ARRAY(p);
		XDELETE_ARRAY(m_texInfo);
		XDELETE_ARRAY(m_targetOrder);
		return XFalse;
	}
	int nowGetTargetNameSum = 0;
	//下面开始提取压缩文件信息
	char tempChar;
	int tempFlag;
	char targetFileName[MAX_FILE_NAME_LENGTH] = "";
	for(int i = 0;i < m_texInfoSum;++ i)
	{//这里有一个回车位
		if(sscanf((char *)(p + offset),"%c",&tempChar) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset ++;//读取文本的回车位
		if(sscanf((char *)(p + offset),"%c",&tempChar) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset ++;//读取文本的回车位

		tempFlag = 0;
		for(int j = 0;j < MAX_FILE_NAME_LENGTH;++ j)
		{
			if(sscanf((char *)(p + offset),"%c",&tempChar) != 1) {XDELETE_ARRAY(p);return XFalse;}
			offset ++;
			if(tempChar == ':')
			{
				m_texInfo[i].textureName[j] = '\0';
				tempFlag = 1;
				break;
			}else
			{
				m_texInfo[i].textureName[j] = tempChar;
			}
		}
		if(tempFlag == 0)
		{//文件数据发生错误，这里直接返回
			XDELETE_ARRAY(p);
			XDELETE_ARRAY(m_texInfo);
			XDELETE_ARRAY(m_targetOrder);
			XDELETE_ARRAY(m_targetName);
			return XFalse;
		}
		
		tempFlag = 0;
		for(int j = 0;j < MAX_FILE_NAME_LENGTH;++ j)
		{
			if(sscanf((char *)(p + offset),"%c",&tempChar) != 1) {XDELETE_ARRAY(p);return XFalse;}
			offset ++;
			if(tempChar == ':')
			{
				targetFileName[j] = '\0';
				tempFlag = 1;
				break;
			}else
			{
				targetFileName[j] = tempChar;
			}
		}
		if(tempFlag == 0)
		{//文件数据发生错误，这里直接返回
			XDELETE_ARRAY(p);
			XDELETE_ARRAY(m_texInfo);
			XDELETE_ARRAY(m_targetOrder);
			XDELETE_ARRAY(m_targetName);
			return XFalse;
		}
		tempFlag = 0;
		for(int j = 0;j < nowGetTargetNameSum;++ j)
		{
			if(fileNameCompare(targetFileName,m_targetName + j * MAX_FILE_NAME_LENGTH))
			{//文件名相同
				m_targetOrder[i] = j;
				tempFlag = 1;
				break;
			}
		}
		if(tempFlag == 0)
		{
			if(nowGetTargetNameSum >= m_targetTextureSum)
			{//发生数据错误
				XDELETE_ARRAY(p);
				XDELETE_ARRAY(m_texInfo);
				XDELETE_ARRAY(m_targetOrder);
				XDELETE_ARRAY(m_targetName);
				return XFalse;
			}
			m_targetOrder[i] = nowGetTargetNameSum;
			strcpy(m_targetName + nowGetTargetNameSum * MAX_FILE_NAME_LENGTH,targetFileName);
			++nowGetTargetNameSum;
		}
		if(sscanf((char *)(p + offset),"%d:",&tempFlag) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(tempFlag < 0 || tempFlag >= m_targetTextureSum)
		{//文件数据发生错误，这里直接返回
			XDELETE_ARRAY(p);
			XDELETE_ARRAY(m_texInfo);
			XDELETE_ARRAY(m_targetOrder);
			XDELETE_ARRAY(m_targetName);
			return XFalse;
		}
		//读取剩下的信息
		if(sscanf((char *)(p + offset),"%f,",&m_texInfo[i].textureOffset.x) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),',') + 1;
		if(sscanf((char *)(p + offset),"%f:",&m_texInfo[i].textureOffset.y) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;

		if(sscanf((char *)(p + offset),"%f,",&m_texInfo[i].textureMove.x) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),',') + 1;
		if(sscanf((char *)(p + offset),"%f,",&m_texInfo[i].textureMove.y) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),',') + 1;
		if(sscanf((char *)(p + offset),"%f,",&m_texInfo[i].textureMove2.x) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),',') + 1;
		if(sscanf((char *)(p + offset),"%f:",&m_texInfo[i].textureMove2.y) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(sscanf((char *)(p + offset),"%f,",&m_texInfo[i].textureSize.x) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),',') + 1;
		if(sscanf((char *)(p + offset),"%f;",&m_texInfo[i].textureSize.y) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),';') + 1;
	}
	//所有数据读取完成
	XDELETE_ARRAY(p);

	m_isGetOptimizeInfo = XTrue;
	return XTrue;
}
_XBool _XResourcePack::releaseOptimizeInfo()
{
	if(!m_isGetOptimizeInfo) return XTrue;
	
	XDELETE_ARRAY(m_texInfo);
	XDELETE_ARRAY(m_targetOrder);
	XDELETE_ARRAY(m_targetName);	

	m_isGetOptimizeInfo = XFalse;
	return XTrue;
}

int _XResourcePack::unpackResource(const char *fileName)
{
	if(!m_haveReadedNormalFileList)
	{
		if(readNormalFileListFromResouce() != 1) 
		{
#ifdef DEBUG_MODE
			printf("Read file list from resouce error!\n");
#endif
			return -1;
		}
	}
	//查找资源位置
	int needFileOrder = -1;
	for(int i = 0;i < m_fileSum;++ i)
	{
		if(fileNameCompare(m_fileInfo[i].fileName,fileName))
		{
			needFileOrder = i;
			break;
		}
	}
	if(needFileOrder == -1) 
	{//如果普通文件中找不到则看看是否需要解压高级文件列表
		if(!m_haveReadedFileList) 
		{
			if(readFileListFromResouce() != 1) 
			{
	#ifdef DEBUG_MODE
				printf("Read file list from resouce error!\n");
	#endif
				return -1;
			}
			for(int i = 0;i < m_fileSum;++ i)
			{
				if(fileNameCompare(m_fileInfo[i].fileName,fileName))
				{
					needFileOrder = i;
					break;
				}
			}
		}
	}
	if(needFileOrder == -1) 
	{//如果在文件列表中不能查找到资源则从压缩资源列表中查找资源
		if(!m_isGetOptimizeInfo)
		{
			if(!getOptimizeInfo())
			{
#ifdef DEBUG_MODE
				printf("Read resource optimize information error!\n");
#endif
				return 0;
			}
		}

		//压缩文件信息读取完成之后需要搜索压缩文件中是否有需要的信息
		needFileOrder = -1;
		for(int i = 0;i < m_texInfoSum;++ i)
		{
			if(fileNameCompare(m_texInfo[i].textureName,fileName))
			{
				needFileOrder = i;
				break;
			}
		}
		if(needFileOrder == -1)
		{
#ifdef DEBUG_MODE
			printf("Cannot find the file from the resource file :%s!\n",fileName);
#endif
			return 0;
		}else
		{//找到资源之后提取资源并还原资源
			int lengthTemp = getFileLength(m_targetName + m_targetOrder[needFileOrder] * MAX_FILE_NAME_LENGTH);
			if(lengthTemp < 0) return 0;
			unsigned char *p = createArrayMem<unsigned char>(lengthTemp + 1);
			if(p == NULL) return 0;

			if(unpackResource(m_targetName + m_targetOrder[needFileOrder] * MAX_FILE_NAME_LENGTH,p) == 0)
			{
#ifdef DEBUG_MODE
				printf("Optimize file unpack error!\n");
#endif
				XDELETE_ARRAY(p);
				return 0;
			}
			//下面根据解压出来的信息还原资源
			if(!reductionTexture(p,lengthTemp,m_texInfo[needFileOrder]))
			{
#ifdef DEBUG_MODE
				printf("Texture reduction error!\n");
#endif
				XDELETE_ARRAY(p);
				return 0;
			}
			XDELETE_ARRAY(p);
			return 1;
		}
	}
	FILE *fpTemp;

	//生成资源文件
	unsigned char * tempFileData = createArrayMem<unsigned char>(m_fileInfo[needFileOrder].fileLength);
	if(tempFileData == NULL) return -1;

	if((fpTemp = fopen(m_fileInfo[needFileOrder].fileName,"wb")) == NULL)
	{
		if((fpTemp = fopen(m_fileInfo[needFileOrder].fileName,"ab")) == NULL)
		{
#ifdef DEBUG_MODE
			printf("File list open error!\n");
#endif
			return -1;
		}
	}
	if(needFileOrder < m_normalFileSum)
	{//普通文件
		FILE *fp;
		//打开资源文件
		if((fp = fopen(m_outFileName,"rb")) == NULL)
		{
			printf("Resource file open error!\n");
			return -1;
		}		
		fseek(fp,m_fileInfo[needFileOrder].filePosition,SEEK_SET);
		fread(tempFileData,m_fileInfo[needFileOrder].fileLength,1,fp);
		fclose(fp);
	}else
	{//经过加密的文件
		m_fileDataPoint = m_fileInfo[needFileOrder].filePosition;
		memcpy(tempFileData,m_fileData + m_fileDataPoint,m_fileInfo[needFileOrder].fileLength);
		lockOrUnlockProc(tempFileData,m_fileInfo[needFileOrder].filePosition,m_fileInfo[needFileOrder].fileLength);
	}

	fwrite(tempFileData,m_fileInfo[needFileOrder].fileLength,1,fpTemp);	

	//结束
	fclose(fpTemp);
	//fclose(fp);
	XDELETE_ARRAY(tempFileData);
#ifdef DEBUG_MODE
	printf("%s Unpack finished!\n",fileName);
#endif
	return 1;
}

int _XResourcePack::unpackResource(const char *fileName,unsigned char *p)	//解包某一个资源到一段内存空间
{//这里可以优化并不需要每次都需要读取资源包中的文件索引表
	if(p == NULL) return 0;
	if(fileName == NULL) return 0;
	if(!m_haveReadedNormalFileList)
	{
		if(readNormalFileListFromResouce() != 1) 
		{
#ifdef DEBUG_MODE
			printf("Read file list from resouce error!\n");
#endif
			return -1;
		}
	}
	//查找资源位置
	int needFileOrder = -1;
	for(int i = 0;i < m_fileSum;++ i)
	{
		if(fileNameCompare(m_fileInfo[i].fileName,fileName))
		{
			needFileOrder = i;
			break;
		}
	}
	if(needFileOrder == -1) 
	{
		if(!m_haveReadedFileList) 
		{
			if(readFileListFromResouce() != 1) 
			{
#ifdef DEBUG_MODE
				printf("Read file list from resouce error!\n");
#endif
				return -1;
			}
			for(int i = 0;i < m_fileSum;++ i)
			{
				if(fileNameCompare(m_fileInfo[i].fileName,fileName))
				{
					needFileOrder = i;
					break;
				}
			}
		}
	}
	if(needFileOrder == -1) 
	{
#ifdef DEBUG_MODE
		printf("Cannot find the file from the resource file!:%s\n",fileName);
#endif
		return 0;
	}
	if(needFileOrder < m_normalFileSum)
	{//普通文件
		FILE *fp;
		//打开资源文件
		if((fp = fopen(m_outFileName,"rb")) == NULL)
		{
			printf("Resource file open error!\n");
			return -1;
		}		
		fseek(fp,m_fileInfo[needFileOrder].filePosition,SEEK_SET);
		fread(p,m_fileInfo[needFileOrder].fileLength,1,fp);
		fclose(fp);
	}else
	{
		m_fileDataPoint = m_fileInfo[needFileOrder].filePosition;
//	printf("0x%x,0x%x,%d,%d:-->%d,%d\n",p,m_fileData,m_fileDataPoint,m_fileInfo[needFileOrder].fileLength,m_fileDataLength,needFileOrder);
		memcpy(p,m_fileData + m_fileDataPoint,m_fileInfo[needFileOrder].fileLength);
		lockOrUnlockProc(p,m_fileInfo[needFileOrder].filePosition,m_fileInfo[needFileOrder].fileLength);
	}

	//结束
#ifdef DEBUG_MODE
	printf("%s Unpack finished!\n",fileName);
#endif
	return 1;
}

int _XResourcePack::makeFileList()
{
#ifdef XEE_OS_WINDOWS
	char temp[512];
	sprintf(temp,"dir /s/b>%s",FILE_LIST_TEXT_NAME);
	system(temp);
	//处理输出文件为相对路径
	FILE *fp;
	//打开文件列表
	if((fp = fopen(FILE_LIST_TEXT_NAME,"r")) == NULL)
	{
#ifdef DEBUG_MODE
		printf("File list file open error!\n");
#endif
		return -1;
	}
	//完成文件列表的信息
	int pathDeep = 1000;
	int tempPathDeep;
	char fileNameTemp[MAX_FILE_NAME_LENGTH];
	_XBool flag = XTrue;
	m_fileSum = 0;	//实际操作的文件数量
	for(int i = 0;i < MAX_FILE_SUM;++ i)
	{
		while(true)
		{
			if(feof(fp))
			{
				flag = XFalse;
				break;
			}

			fileNameTemp[0] = '\0';
			fscanf(fp,"%s",fileNameTemp);
			if(fileNameTemp[0]  == '\0') continue;
			//if(fileNameTemp[getStringLength(fileNameTemp) - 4]  != '.') continue;
			break;
		}
		if(!flag) break;
		memcpy(m_fileInfo[i].fileName,fileNameTemp,sizeof(fileNameTemp));

		tempPathDeep = getPathDeepByChar(m_fileInfo[i].fileName);
		if(tempPathDeep < pathDeep) pathDeep = tempPathDeep;
		++ m_fileSum;
	}
	fclose(fp);
	//处理成相对路径
	if((fp = fopen(FILE_LIST_TEXT_NAME,"w")) == NULL)
	{
#ifdef DEBUG_MODE
		printf("File list file open error!\n");
#endif
		return -1;
	}
	for(int i = 0;i < m_fileSum;++ i)
	{
		fprintf(fp,"%s\n",&(m_fileInfo[i].fileName[pathDeep + 1]));
	}
	fclose(fp);
#endif
#ifdef XEE_OS_LINUX
	char temp[512];
	sprintf(temp,"ls -qRF>%s",FILE_LIST_TEXT_NAME);
	system(temp);
	//处理输出文件为相对路径
	FILE *fp;
	//打开文件列表
	if((fp = fopen(FILE_LIST_TEXT_NAME,"r")) == NULL)
	{
		printf("File list file open error!\n");
		return -1;
	}
	char fileNameTemp[MAX_FILE_NAME_LENGTH];
//	int pathDeep = 1000;
//	int tempPathDeep;
	char tempStr[256];
	char tempDir[256] = "";
	_XBool firstEnterFlag = XFalse;
	m_fileSum = 0;
	int len = 0; 
	int j = 0;
	while(true)
	{
		if(feof(fp)) break;
		fscanf(fp,"%s",tempStr); 
		len = strlen(tempStr);
		if(firstEnterFlag)
		{
			if(len > 0 && tempStr[len - 1] != '/' && tempStr[len - 1] != ':')
			{ 
				if(tempStr[len - 1] == '*') tempStr[len - 1] = '\0';
				if(tempDir[0] != '\0') 
				{
					strcpy(fileNameTemp,tempDir + 2);
				}
				strcat(fileNameTemp,tempStr);
				//printf("%s\n",fileNameTemp);	//for test
				memcpy(m_fileInfo[m_fileSum].fileName,fileNameTemp,sizeof(fileNameTemp));

				//tempPathDeep = getPathDeepByChar(m_fileInfo[m_fileSum].fileName);
				//if(tempPathDeep < pathDeep) pathDeep = tempPathDeep;
				++ m_fileSum;
			}
		}
		//record the DIR
		if(tempStr[len - 1] == ':')
		{
			strcpy(tempDir,tempStr);
			tempDir[len - 1] = '/';
		}
		if(!firstEnterFlag) firstEnterFlag = XTrue;
	}
	fclose(fp);
	//处理成相对路径
	if((fp = fopen(FILE_LIST_TEXT_NAME,"w")) == NULL)
	{
		printf("File list file open error!\n");
		return -1;
	}
	for(int i = 0;i < m_fileSum;++ i)
	{
		//fprintf(fp,"%s\n",&(m_fileInfo[i].fileName[pathDeep + 1]));
		fprintf(fp,"%s\n",m_fileInfo[i].fileName);
	}
	fclose(fp);
#endif
	return 1;
}

int _XResourcePack::readNormalFileListFromResouce()
{
	printf("readNormalFileListFromResouce\n");
	if(m_fileInfo == NULL) return 0;
	FILE *fp;
	//打开资源文件
	if((fp = fopen(m_outFileName,"rb")) == NULL)
	{
#ifdef DEBUG_MODE
		printf("Resource file open error!\n");
#endif
		return -1;
	}
	long tempHeadLength;
	fread(&tempHeadLength,sizeof(long),1,fp);	//读取头的长度
	m_fileSum = tempHeadLength / sizeof(_XResourceFileStruct);
	m_normalFileSum = m_fileSum;
	if(m_normalFileSum >= MAX_FILE_SUM)
	{//文件数量超过上限
		fclose(fp);
		return 0;
	}
	fread(&tempHeadLength,sizeof(long),1,fp);	//读取头的长度
	char tempFilename[MAX_FILE_NAME_LENGTH] = "";
	for(int i = 0;i < m_fileSum;++ i)
	{//读取文件列表信息
		//fread(m_fileInfo[i].fileName,sizeof(m_fileInfo[i].fileName),1,fp);	//为了统一路径，这里进行一些修改
		fread(tempFilename,sizeof(m_fileInfo[i].fileName),1,fp);	//为了统一路径，这里进行一些修改
		sprintf(m_fileInfo[i].fileName,"%s%s",BASE_RESOURCE_PATH,tempFilename);

		fread(&(m_fileInfo[i].fileLength),sizeof(m_fileInfo[i].fileLength),1,fp);	
		fread(&(m_fileInfo[i].filePosition),sizeof(m_fileInfo[i].filePosition),1,fp);	
	}
	fclose(fp);
	m_haveReadedNormalFileList = XTrue;

	return 1;
}

int _XResourcePack::readFileListFromResouce()				//从压缩包中读取文件索引信息
{
	printf("readFileListFromResouce\n");
//	printf("开始取资源:%d\n",getCurrentTicks());
	if(m_fileInfo == NULL) return 0;
	FILE *fp;
	//打开资源文件
	if((fp = fopen(m_outFileName,"rb")) == NULL)
	{
#ifdef DEBUG_MODE
		printf("Resource file open error!\n");
#endif
		return -1;
	}
	fseek(fp,0,SEEK_END);
	m_fileDataLength = ftell(fp);	//获取文件的长度
	fseek(fp,0,SEEK_SET);
	long tempHeadLength;
	fread(&tempHeadLength,sizeof(long),1,fp);	//读取头的长度
	fread(&tempHeadLength,sizeof(long),1,fp);	//读取头的长度
	tempHeadLength += sizeof(long) + sizeof(long);
	m_fileDataLength -= tempHeadLength;
	if(m_fileDataLength <= 0)
	{//没有特殊文件
		fclose(fp);
		m_haveReadedFileList = XTrue;
		return 1;
	}
	fseek(fp,tempHeadLength,SEEK_SET);	//移动指针到指定位置
	XDELETE_ARRAY(m_fileData);
	m_fileData = createArrayMem<unsigned char>(m_fileDataLength);
	if(m_fileData == NULL) 
	{
		fclose(fp);
		return 0;
	}
	//读取文件内容
	fread(m_fileData,m_fileDataLength,1,fp);
	fclose(fp);
//	printf("取出资源:%d\n",getCurrentTicks());
	//内存混乱的逆向操作
	unsigned char *fileData = createArrayMem<unsigned char>(m_fileDataLength);
	if(fileData == NULL) 
	{
		XDELETE_ARRAY(m_fileData);
		return 0;
	}
	int coreLen;
	char coreOrder[MEMORY_CHANGE_TIMES];
	int oldDataLength = m_fileDataLength - 1 - MEMORY_CHANGE_TIMES * sizeof(char) - sizeof(int);
	memcpy(&coreLen,m_fileData + oldDataLength,sizeof(int));
	memcpy(coreOrder,m_fileData + oldDataLength + sizeof(int),MEMORY_CHANGE_TIMES * sizeof(char));
	for(int i = 0;i < MEMORY_CHANGE_TIMES;++ i)
	{
		memcpy(fileData + coreLen * coreOrder[i],m_fileData + coreLen * i,coreLen);
	}
	oldDataLength -= MD5_CODE_SIZE;
	//结余部分继续补全
	if(oldDataLength % MEMORY_CHANGE_TIMES != 0)
	{
		memcpy(fileData + coreLen * MEMORY_CHANGE_TIMES,m_fileData + coreLen * MEMORY_CHANGE_TIMES,oldDataLength - coreLen * MEMORY_CHANGE_TIMES);
	}
	//奇偶交换
	for(int i = 0;i < oldDataLength - 1;i += 2)
	{
		m_fileData[i + 1] = fileData[i];
		m_fileData[i] = fileData[i + 1];
	}
	if((oldDataLength % 2) != 0)
	{
		m_fileData[oldDataLength - 1] = fileData[oldDataLength - 1];
	}
	XDELETE_ARRAY(fileData);
	/*
	int memoryChangeData[MEMORY_CHANGE_TIMES];
	int oldDataLength = m_fileDataLength - 1 - MEMORY_CHANGE_TIMES * sizeof(int);
	memcpy(memoryChangeData,m_fileData + oldDataLength,MEMORY_CHANGE_TIMES * sizeof(int));
	oldDataLength -= MD5_CODE_SIZE;
//	for(int i = MEMORY_CHANGE_TIMES - 1;i >= 0;-- i)
	int tempI;
	for(int i = 0;i < MEMORY_CHANGE_TIMES;++ i)
	{
		tempI = MEMORY_CHANGE_TIMES - 1 - i;
		if((i % 2) == 0)
		{
			memcpy(fileData,m_fileData + (oldDataLength - memoryChangeData[tempI]),memoryChangeData[tempI]);
			memcpy(fileData + memoryChangeData[tempI],m_fileData,oldDataLength - memoryChangeData[tempI]);
		}else
		{
			memcpy(m_fileData,fileData + (oldDataLength - memoryChangeData[tempI]),memoryChangeData[tempI]);
			memcpy(m_fileData + memoryChangeData[tempI],fileData,oldDataLength - memoryChangeData[tempI]);
		}
	}
	if((MEMORY_CHANGE_TIMES % 2) != 0)
	{
		memcpy(m_fileData,fileData,oldDataLength);
	}
	XDELETE_ARRAY(fileData);
//	printf("资源混乱完成:%d\n",getCurrentTicks());
*/
	//读取文件特征码
	m_fileDataPoint = 0;
	char temp[23];
	memcpy(temp,m_fileData + m_fileDataPoint,sizeof(FILE_CODE));
	m_fileDataPoint += sizeof(FILE_CODE);
	//fread(temp,23,1,fp);
	if(checkFileCode(temp) != 1) 
	{
		XDELETE_ARRAY(m_fileData);
		return -1;
	}
	//读取包头长度
	memcpy(&m_headLength,m_fileData + m_fileDataPoint,sizeof(int));
	m_fileDataPoint += sizeof(m_headLength);
	//fread(&m_headLength,sizeof(int),1,fp);		//4bytes
	//读取加密码
	memcpy(tempCode,m_fileData + m_fileDataPoint,LOCK_CODE_LENGTH);
	m_fileDataPoint += LOCK_CODE_LENGTH;
	//fread(tempCode,LOCK_CODE_LENGTH,1,fp);		//32bytes
	getlockKey();	//计算掩码
	//读取文件列表(查找到相应的资源以及偏移地址)
	m_fileSum += (m_headLength - LOCK_CODE_LENGTH) / sizeof(_XResourceFileStruct);
	if(m_fileSum < 0 || (m_fileSum + m_normalFileSum) >= MAX_FILE_SUM)
	{//数据非法会造成错误
		XDELETE_ARRAY(m_fileData);
		return -1;
	}
	//int needFileOrder = -1;
	char tempFilename[MAX_FILE_NAME_LENGTH] = "";
	for(int i = m_normalFileSum;i < m_fileSum;++ i)
	{
		memcpy(tempFilename,m_fileData + m_fileDataPoint,sizeof(m_fileInfo[i].fileName));
		lockOrUnlockProc((unsigned char *)tempFilename,m_fileDataPoint,sizeof(m_fileInfo[i].fileName));
		sprintf(m_fileInfo[i].fileName,"%s%s",BASE_RESOURCE_PATH,tempFilename);

		m_fileDataPoint += sizeof(m_fileInfo[i].fileName);

		memcpy(&(m_fileInfo[i].fileLength),m_fileData + m_fileDataPoint,sizeof(m_fileInfo[i].fileLength));
		lockOrUnlockProc((unsigned char *)&(m_fileInfo[i].fileLength),m_fileDataPoint,sizeof(m_fileInfo[i].fileLength));
		m_fileDataPoint += sizeof(m_fileInfo[i].fileLength);

		memcpy(&(m_fileInfo[i].filePosition),m_fileData + m_fileDataPoint,sizeof(m_fileInfo[i].filePosition));
		lockOrUnlockProc((unsigned char *)&(m_fileInfo[i].filePosition),m_fileDataPoint,sizeof(m_fileInfo[i].filePosition));
		m_fileDataPoint += sizeof(m_fileInfo[i].filePosition);
	}
	//fclose(fp);
	m_haveReadedFileList = XTrue;
//	printf("资源索引取出完成:%d\n",getCurrentTicks());
	//检查文件是否被修改
	if(!checkCheckData())
	{
#ifdef DEBUG_MODE
		printf("Data Check ERROR！\n");
#endif
		m_haveReadedFileList = XFalse;
		XDELETE_ARRAY(m_fileData);
		return 0;
	}
//	printf("资源检查完成:%d\n",getCurrentTicks());
	return 1;
}

int _XResourcePack::getFileLength(const char *fileName)
{
	int i = 0;
	if(!m_haveReadedNormalFileList) 
	{
		if(readNormalFileListFromResouce() != 1) 
		{
#ifdef DEBUG_MODE
			printf("Read file list from resouce error!\n");
#endif
			return -1;
		}
	}
	//查找资源位置
	for(i = 0;i < m_fileSum;++ i)
	{
		if(fileNameCompare(m_fileInfo[i].fileName,fileName))
		{
			return m_fileInfo[i].fileLength;
		}
	}
	//如果在普通资源中找不到文件，则在特殊资源那种寻找
	if(!m_haveReadedFileList) 
	{
		if(readFileListFromResouce() != 1) 
		{
#ifdef DEBUG_MODE
			printf("Read file list from resouce error!\n");
#endif
			return -1;
		}
		//查找资源位置
		for(i = 0;i < m_fileSum;++ i)
		{
			if(fileNameCompare(m_fileInfo[i].fileName,fileName))
			{
				return m_fileInfo[i].fileLength;
			}
		}
	}
	return 0;
}

unsigned char *_XResourcePack::getFileData(const char *filename)
{
	if(filename == NULL) return NULL;
	int lengthTemp = getFileLength(filename);
	if(lengthTemp <= 0) return NULL;
	unsigned char *p = NULL;
	if((p = createArrayMem<unsigned char>(lengthTemp + 1)) == NULL) return NULL;
	_XResourcePack::GetInstance().unpackResource(filename,p);
	*(p + lengthTemp) = '\0';
	return p;
}

void _XResourcePack::lockOrUnlockProc(unsigned char *p,int startPoint,int length) const
{
	if(length < 0) return;
	int loopStart = startPoint % ALL_LOCK_CODE_LENGTH;

	for(int i = 0,j = loopStart;i < length;++ i,++ j)
	{
		if(j >= ALL_LOCK_CODE_LENGTH) j = 0;
		p[i] = p[i]^m_lockKey[j];
	}
}

int _XResourcePack::writeCheckData()
{
	FILE *fp;
	//下面加入写入MD5校验码
	int tempLength;
	if((fp = fopen(m_outFileName,"rb")) == NULL)
	{
#ifdef DEBUG_MODE
		printf("Resource file open error!\n");
#endif
		return 0;
	}

	fseek(fp,0,SEEK_END);
	tempLength = ftell(fp);	//获取文件的长度
	fseek(fp,0,SEEK_SET);
	//读取文件头部
	long tempHeadLength;
	fread(&tempHeadLength,sizeof(long),1,fp);
	fread(&tempHeadLength,sizeof(long),1,fp);
	tempHeadLength += sizeof(long) + sizeof(long);
	tempLength -= tempHeadLength;
	//printf("%d,%d\n",tempHeadLength,tempLength);
	if(tempLength <= 0) 
	{//文件没有后面的部分则直接返回
		fclose(fp);
		return 1;
	}
	unsigned char *fileData = createArrayMem<unsigned char>(tempLength);
	unsigned char *fileDataHead = createArrayMem<unsigned char>(tempHeadLength);
	if(fileData == NULL || fileDataHead == NULL) 
	{
		XDELETE_ARRAY(fileData);
		XDELETE_ARRAY(fileDataHead);
		fclose(fp);
		return 0;
	}
	//读取文件内容
	fseek(fp,0,SEEK_SET);
	fread(fileDataHead,tempHeadLength,1,fp);	//读取文件头的内容
	fread(fileData,tempLength,1,fp);			//去读文件头之后的内容
	fclose(fp);
	//计算MD5值
	unsigned char *MD5Result = md5(fileData,tempLength);
#ifdef DEBUG_MODE
	printf("MD5:");
	for(int j = 0;j < MD5_CODE_SIZE;++ j)
	{
		printf("%02x-",MD5Result[j]);
	}
	printf("%d\n",tempLength);
#endif

	//这里需要进行内存混乱操作
	unsigned char *fileData1 = createArrayMem<unsigned char>(tempLength);
	if(fileData1 == NULL) return 0;
	//++++++++++++++++++++++++++++++++++++++++++++++++
	//下面修改混乱操作的具体内容
	//第一步将奇偶位交换
	//printf("开始混乱操作！\n");
	for(int i = 0;i < tempLength - 1;i += 2)
	{
		fileData1[i + 1] = fileData[i];
		fileData1[i] = fileData[i + 1];
	}
	if((tempLength % 2) != 0)
	{
	//	fileData1[tempLength - 1] = fileData1[tempLength];
		fileData1[tempLength - 1] = fileData[tempLength - 1];
	}
	//第二步分块交换
	int coreLen = (tempLength - (tempLength % MEMORY_CHANGE_TIMES)) / MEMORY_CHANGE_TIMES;
	char coreOrder[MEMORY_CHANGE_TIMES];
	//printf("区块长度为：%d :%d:%d！\n",coreLen,MEMORY_CHANGE_TIMES,tempLength);
	for(int i = 0;i < MEMORY_CHANGE_TIMES;++ i)
	{
		coreOrder[i] = i;
	}
	char tempData;
	int tempOrder;
	for(int i = 0;i < MEMORY_CHANGE_TIMES;++ i)
	{
		tempOrder = random(MEMORY_CHANGE_TIMES);
		if(i != tempOrder)
		{
			tempData = coreOrder[i];
			coreOrder[i] = coreOrder[tempOrder];
			coreOrder[tempOrder] = tempData;
		}
	}
	//printf("开始交换数据！\n");
	for(int i = 0;i < MEMORY_CHANGE_TIMES;++ i)
	{
		memcpy(fileData + i * coreLen,fileData1 + coreOrder[i] * coreLen,coreLen);
	}
	if(tempLength % MEMORY_CHANGE_TIMES != 0)
	{
		memcpy(fileData + MEMORY_CHANGE_TIMES * coreLen,fileData1 + MEMORY_CHANGE_TIMES * coreLen,tempLength -  MEMORY_CHANGE_TIMES * coreLen);
	}
	//printf("数据交换完成，写入数据:%d.\n",tempLength);
	//下面开始文件写入操作
	if((fp = fopen(m_outFileName,"wb")) == NULL)
	{
#ifdef DEBUG_MODE
		printf("Resource file open error!\n");
#endif
		return 0;
	}
	fwrite(fileDataHead,tempHeadLength,1,fp);	//读取文件头的内容
	fwrite(fileData,tempLength,1,fp);	//写入文件内容
	fwrite(MD5Result,MD5_CODE_SIZE,1,fp);			//写入MD5校验字符串
	fwrite(&coreLen,sizeof(int),1,fp);			//写入区块长度
	fwrite(coreOrder,MEMORY_CHANGE_TIMES * sizeof(char),1,fp);			//写入区块编号
	fclose(fp);
	XDELETE_ARRAY(fileData);
	XDELETE_ARRAY(fileDataHead);
	XDELETE_ARRAY(fileData1);
	//printf("完成数据混乱操作.\n");
	//------------------------------------------------
/*	int memoryChangeData[MEMORY_CHANGE_TIMES];
	for(int i = 0;i < MEMORY_CHANGE_TIMES;++ i)
	{
		memoryChangeData[i] = tempLength *(random(75) / 100.0f + 0.125f);
	//	printf("%d\n",memoryChangeData[i]);
		if((i % 2) == 0)
		{
			memcpy(fileData1,fileData + memoryChangeData[i],tempLength - memoryChangeData[i]);
			memcpy(fileData1 + (tempLength - memoryChangeData[i]),fileData,memoryChangeData[i]);
			//memcpy(fileData,fileData1,tempLength);
		}else
		{
			memcpy(fileData,fileData1 + memoryChangeData[i],tempLength - memoryChangeData[i]);
			memcpy(fileData + (tempLength - memoryChangeData[i]),fileData1,memoryChangeData[i]);
			//memcpy(fileData,fileData1,tempLength);
		}
	}
	//printf("length:%d\n",tempLength);
	if((MEMORY_CHANGE_TIMES % 2) != 0)
	{
		fwrite(fileData1,tempLength,1,fp);	//写入文件内容
	}else
	{
		fwrite(fileData,tempLength,1,fp);	//写入文件内容
	}
	fwrite(MD5Result,MD5_CODE_SIZE,1,fp);			//写入MD5校验字符串
	fwrite(memoryChangeData,MEMORY_CHANGE_TIMES * sizeof(int),1,fp);			//写入混乱数据
	fclose(fp);
	XDELETE_ARRAY(fileData);
	XDELETE_ARRAY(fileData1);
	*/
	//打开资源文件
	if((fp = fopen(m_outFileName,"rb")) == NULL)
	{
#ifdef DEBUG_MODE
		printf("Resource file open error!\n");
#endif
		return 0;
	}
	unsigned char checkData = 0;
	unsigned char temp[1024];
	memset(temp,0,1024);

	fseek(fp,0,SEEK_END);
	tempLength = ftell(fp);	//获取文件的长度
	fseek(fp,0,SEEK_SET);

	tempLength -= tempHeadLength;
	if(tempLength <= 0)
	{
		fclose(fp);
		return 1;
	}
	//fread(fileDataHead,tempHeadLength,1,fp);	//读取文件头的内容，这部分内容不会经过字节校验处理
	fseek(fp,tempHeadLength,SEEK_SET);
	int nowPosition = 0;
	while(true)
	{
		if(tempLength <= nowPosition + 1024)
		{
			fread(temp,tempLength - nowPosition,1,fp);
			for(int i = 0;i< tempLength - nowPosition;++ i)
			{
				checkData += temp[i];
			}
			break;
		}else
		{
			fread(temp,1024,1,fp);
			for(int i = 0;i< 1024;++ i)
			{
				checkData += temp[i];
			}
			nowPosition += 1024;
		}
	}
	fclose(fp);
	//写入校验和
	if((fp = fopen(m_outFileName,"ab")) == NULL)
	{
#ifdef DEBUG_MODE
		printf("Resource file open error!\n");
#endif
		return 0;
	}
	fwrite(&checkData,1,1,fp);
	fclose(fp);
	return 1;
}

_XBool _XResourcePack::checkCheckData()
{
	if(m_haveReadedFileList)
	{
		//计算MD5值
		int tempLen = m_fileDataLength - MD5_CODE_SIZE - 1 - MEMORY_CHANGE_TIMES * sizeof(char) - sizeof(int);
		unsigned char *MD5Result = md5(m_fileData,tempLen);
		//检查计算结果
		for(int i = 0;i < MD5_CODE_SIZE;++ i)
		{
			if(MD5Result[i] != m_fileData[tempLen + i]) 
			{
				//for(int j = 0;j < MD5_CODE_SIZE;++ j)
				//{
				//	printf("%2x-",MD5Result[j]);
				//}
				//printf("\n%d\n",tempLen);
				//for(int j = 0;j < MD5_CODE_SIZE;++ j)
				//{
				//	printf("%2x-",m_fileData[tempLen + j]);
				//}
#ifdef DEBUG_MODE
				printf("MD5校验码检测错误:1!\n");
#endif
				return XFalse;
			}
		}
#ifdef DEBUG_MODE
		printf("MD5校验成功:1!\n");
#endif
		//检查校验字节
		unsigned char checkData = 0;
		for(int i = 0;i< m_fileDataLength - 1;++ i)
		{
			checkData += m_fileData[i];
		}
		if(checkData != m_fileData[m_fileDataLength - 1]) return XFalse;
		return XTrue;
	}else
	{//这后面的代码是有问题的，但是由于原程序不会运行到后面所以不会暴露问题，因为内存进行了混乱操作，但是下面的代码没有进行相应的逆操作
		FILE *fp;
		//检查MD5值
		int tempLength;
		if((fp = fopen(m_outFileName,"rb")) == NULL)
		{
#ifdef DEBUG_MODE
			printf("Resource file open error!\n");
#endif
			return XFalse;
		}

		fseek(fp,0,SEEK_END);
		tempLength = ftell(fp);	//获取文件的长度
		fseek(fp,0,SEEK_SET);
		long tempHeadLength;
		fread(&tempHeadLength,sizeof(long),1,fp);
		fread(&tempHeadLength,sizeof(long),1,fp);
		tempHeadLength += sizeof(long) + sizeof(long);
		tempLength -= tempHeadLength;
		if(tempLength <= 0)
		{
			fclose(fp);
			return XTrue;
		}
		fseek(fp,tempHeadLength,SEEK_SET);
		unsigned char *fileData = createArrayMem<unsigned char>(tempLength);
		if(fileData == NULL) 
		{
			fclose(fp);
			return XFalse;
		}
		//读取文件内容
		fread(fileData,tempLength,1,fp);
		fclose(fp);
		//计算MD5值
		int tempLen = tempLength - MD5_CODE_SIZE - 1 - MEMORY_CHANGE_TIMES * sizeof(char) - sizeof(int);
		unsigned char *MD5Result = md5(fileData,tempLen);
		//检查计算结果
		for(int i = 0;i < MD5_CODE_SIZE;++ i)
		{
			if(MD5Result[i] != fileData[tempLen + i]) 
			{
				//for(int j = 0;j < MD5_CODE_SIZE;++ j)
				//{
				//	printf("%2x-",MD5Result[j]);
				//}
				//printf("\n");
				//for(int j = 0;j < MD5_CODE_SIZE;++ j)
				//{
				//	printf("%2x-",m_fileData[tempLength - 17 - MEMORY_CHANGE_TIMES * sizeof(char) - sizeof(int) + j]);
				//}
#ifdef DEBUG_MODE
				printf("MD5校验码检测错误:2!\n");
#endif
				XDELETE_ARRAY(fileData);
				return XFalse;
			}
		}
#ifdef DEBUG_MODE
		printf("MD5校验成功:2!\n");
#endif

		unsigned char checkData = 0;
		unsigned char temp[1024];
		memset(temp,0,1024);

		int nowPosition = 0;
		while(true)
		{
			if(tempLength <= nowPosition + 1024)
			{
				//fread(temp,tempLength - nowPosition,1,fp);
				memcpy(temp,fileData + nowPosition,tempLength - nowPosition);
				for(int i = 0;i< tempLength - nowPosition;++ i)
				{
					if(i < tempLength - nowPosition - 1) checkData += temp[i];
				}
				temp[0] = temp[tempLength - nowPosition - 1];
				break;
			}else
			{
				//fread(temp,1024,1,fp);
				memcpy(temp,fileData + nowPosition,1024);
				for(int i = 0;i< 1024;++ i)
				{
					checkData += temp[i];
				}
				nowPosition += 1024;
			}
		}
		XDELETE_ARRAY(fileData);

		if(checkData != temp[0]) return XFalse;
		return XTrue;
	}
}

void _XResourcePack::getlockKey()
{
	unsigned char tempLockData[33] = LOCK_DATA_PART_1;	//这一部分是代码中的密码段
	memcpy(m_lockKey,tempCode,32);						//0  - 31个字节的密钥来自于打包程序自动生成
	memcpy(m_lockKey + 32,tempLockData,32);				//32 - 63个字节的密钥来自于打包程序中固定的字符串
	memcpy(m_lockKey + 64,m_hardLockKey,32);			//64 - 95个字及的密钥来自于用户设置，将来的扩展中将会来自于某个硬件加密设备
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//最新的修改中资源打包模块不直接与Senselock挂钩，以便于其扩展性
//	unsigned char tempHardLock[64];
//	Check128_sense4(tempHardLock);
//	memcpy(m_lockKey + 64,tempHardLock,32);	//加入密码锁之后的数据
//-------------------------------------------------------------

	//密钥中96 - 127个字节的密钥为前面密钥的校验和
	for(int i = 0;i < 32;++ i)
	{//通过计算计算出后面的key
		m_lockKey[96 + i] = (tempCode[i] + tempLockData[i] + m_hardLockKey[i])%256;
	}
}

_XResourcePack::_XResourcePack()
:m_haveReadedFileList(XFalse)
,m_haveReadedNormalFileList(XFalse)
,m_fileInfo(NULL)
,m_fileSum(0)
,m_headLength(0)
,m_fileData(NULL)
,m_fileDataLength(0)
,m_fileDataPoint(0)
,m_isGetOptimizeInfo(XFalse)
,m_texInfo(NULL)
,m_texInfoSum(0)
{
	//put single object into auto_ptr object 
	//m_auto_ptr = auto_ptr<_XResourcePack>(this);

/*	for(int i = 0;i < MAX_FILE_SUM;++ i)
	{
		m_fileInfo[i] = NULL;
		m_fileInfo[i] = (_XResourceFileStruct *)operator new(sizeof(_XResourceFileStruct));
		if(m_fileInfo[i] == NULL) 
		{
			printf("Memory Error!\n");
		}
	}*/
	m_fileInfo = createArrayMem<_XResourceFileStruct>(MAX_FILE_SUM);
	if(m_fileInfo == NULL) 
	{
#ifdef DEBUG_MODE
		printf("Memory Error!\n");
#endif
	}
	//默认的硬件密钥在这里设置，如果不变更则使用默认值
	unsigned char tempHardLock[32] =
	{0x29,0xd3,0x4e,0xf4,0xa9,0xd4,0xde,0x66,0xc2,0xd0,0xc1,0x91,0x19,0x3b,0xc1,0xeb,
	0xe3,0xae,0x3b,0xb4,0x87,0x5c,0x86,0x99,0x24,0x7e,0xa5,0xcc,0xce,0x7f,0xbf,0xd6};
	setHardLockKey(tempHardLock);

	setOutFileName(NULL);
}

void _XResourcePack::setOutFileName(const char *temp)
{
	if(temp == NULL)
	{
		strcpy(m_outFileName,OUT_FILE_NAME);
	//	printf("setOutFileName:%s\n",m_outFileName);
	}else
	{
		if(strlen(temp) >= MAX_FILE_NAME_LENGTH)
		{
			strcpy(m_outFileName,OUT_FILE_NAME);
		}else
		{
			strcpy(m_outFileName,temp);
		}
	//	printf("setOutFileName:%s\n",m_outFileName);
	}
	if(m_haveReadedNormalFileList)
	{
		if(readNormalFileListFromResouce() != 1) 
		{
#ifdef DEBUG_MODE
			printf("Read file list from resouce error!\n");
#endif
			return;
		}
	}
	if(m_haveReadedFileList) 
	{
		m_haveReadedFileList = XFalse;
		XDELETE_ARRAY(m_fileData);
	}
//	if(m_haveReadedFileList) 
//	{
//		XDELETE_ARRAY(m_fileData);
//		if(readFileListFromResouce() != 1) 
//		{
//#ifdef DEBUG_MODE
//			printf("Read file list from resouce error!\n");
//#endif
//			return;
//		}
//	}
}