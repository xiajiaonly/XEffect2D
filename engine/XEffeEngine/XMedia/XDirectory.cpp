//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XDirectory.h"
#include "XBasicFun.h"

void _XFile::release()
{
	if(!isEnable) return;
	XDELETE_ARRAY(allPath);
	XDELETE_ARRAY(filename);
	if(directory != NULL) directory->release();
	XDELETE(directory);
	isEnable = XFalse;
}
void _XDir::release()
{
	if(!isEnable) return;
	XDELETE_ARRAY(directoryName);
	int size = files.size();
	for(int i = 0;i < size;++ i)
	{
		files[i]->release();
		XDELETE(files[i]);
	}
	files.clear();	
	files.swap(std::vector<_XFile *>());//释放内存空间
	isEnable = XFalse;
}
_XBool _XDirectory::init(const char *directoryName)
{
	if(m_isInited) return XTrue;
	if(directoryName == NULL) return XFalse;

	m_nowDirectory.level = 0;
	if(!enumerationFiles(directoryName,&m_nowDirectory)) return 0;

	m_isInited = XTrue;
	return XTrue;
}
_XBool _XDirectory::initEx(const char *directoryName)
{
	if(m_isInited) return XTrue;
	if(directoryName == NULL) return XFalse;

	m_nowDirectory.level = 0;
	if(!enumerationFilesEx(directoryName,&m_nowDirectory)) return 0;

	m_isInited = XTrue;
	return XTrue;
}
void infoConvert(WIN32_FIND_DATA &fileInfo,_XFile * file)
{
	file->allPath = createArrayMem<char>(MAX_FILE_NAME_LENGTH);
	file->filename = createArrayMem<char>(MAX_FILE_NAME_LENGTH);
	strcpy(file->filename,fileInfo.cFileName);
	if(fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) file->isDirectory = XTrue; 
	else file->isDirectory = XFalse;
	file->isEnable = XTrue;
}
_XBool enumerationFiles(const char *path,_XDir * dir)
{
	if(path == NULL) return XFalse;
	if(dir == NULL) return XFalse;
	
	WIN32_FIND_DATA fileInfo;
	HANDLE fileHandle;

	dir->directoryName = createArrayMem<char>(MAX_FILE_NAME_LENGTH);
	if(dir->directoryName == NULL) return XFalse;
	char tempDirectoryName[MAX_FILE_NAME_LENGTH];
	sprintf(tempDirectoryName,"%s/*.*",path);
	strcpy(dir->directoryName,path);
	//查找目录下的第一个文件
	fileHandle = FindFirstFile(tempDirectoryName,&fileInfo);
	if(fileHandle == INVALID_HANDLE_VALUE)
	{//错误：空的文件夹或者目录不存在?
		printf("error:%d",GetLastError());
		dir->release();
		return XFalse;
	}
	_XFile *tempFile = NULL;
	if(strcmp(fileInfo.cFileName,".") != 0 && strcmp(fileInfo.cFileName,"..") != 0)
	{
		tempFile = createMem<_XFile>();
		if(tempFile == NULL) 
		{
			dir->release();
			FindClose(fileHandle);
			return XFalse;
		}
		dir->isEnable = XTrue;
		infoConvert(fileInfo,tempFile);
		sprintf(tempFile->allPath,"%s/%s",dir->directoryName,tempFile->filename);
	//	printf("%s\n",tempFile->allPath);
		dir->files.push_back(tempFile);
		if(tempFile->isDirectory)
		{
			tempFile->directory = createMem<_XDir>();
			tempFile->directory->level = dir->level + 1;
			enumerationFiles(tempFile->allPath,tempFile->directory);
		}
	}
	//下面开始遍历整个文件夹
	while(1)
	{
		if(!FindNextFile(fileHandle,&fileInfo)) break;
		if(strcmp(fileInfo.cFileName,".") != 0 && strcmp(fileInfo.cFileName,"..") != 0)
		{
			tempFile = createMem<_XFile>();
			if(tempFile == NULL) 
			{//这里需要释放资源
				dir->release();
				FindClose(fileHandle);
				return XFalse;
			}
			infoConvert(fileInfo,tempFile);
			sprintf(tempFile->allPath,"%s/%s",dir->directoryName,tempFile->filename);
		//	printf("%s\n",tempFile->allPath);
			dir->files.push_back(tempFile);
			if(tempFile->isDirectory)
			{
				tempFile->directory = createMem<_XDir>();
				tempFile->directory->level = dir->level + 1;
				enumerationFiles(tempFile->allPath,tempFile->directory);
			}
		}
	}
	FindClose(fileHandle);
	dir->isEnable = XTrue;

	return XTrue;
}
_XBool enumerationFilesEx(const char *path,_XDir * dir)	//使用相对路径
{
	if(path == NULL) return XFalse;
	if(dir == NULL) return XFalse;

	WIN32_FIND_DATA fileInfo;
	HANDLE fileHandle;

	dir->directoryName = createArrayMem<char>(MAX_FILE_NAME_LENGTH);
	if(dir->directoryName == NULL) return XFalse;
	char tempDirectoryName[MAX_FILE_NAME_LENGTH];
	GetCurrentDirectory(MAX_FILE_NAME_LENGTH,dir->directoryName);	//获取当前路径
	if(strcmp(path,".") == 0)
	{
		sprintf(tempDirectoryName,"%s/*.*",dir->directoryName);
	}else
	{
		sprintf(tempDirectoryName,"%s/%s/*.*",dir->directoryName,path);
	}
	strcpy(dir->directoryName,path);
	//查找目录下的第一个文件
	fileHandle = FindFirstFile(tempDirectoryName,&fileInfo);
	if(fileHandle == INVALID_HANDLE_VALUE)
	{//错误：空的文件夹或者目录不存在?
		printf("error:%d",GetLastError());
		dir->release();
		return XFalse;
	}
	_XFile *tempFile = NULL;
	if(strcmp(fileInfo.cFileName,".") != 0 && strcmp(fileInfo.cFileName,"..") != 0)
	{
		tempFile = createMem<_XFile>();
		if(tempFile == NULL) 
		{
			dir->release();
			FindClose(fileHandle);
			return XFalse;
		}
		dir->isEnable = XTrue;
		infoConvert(fileInfo,tempFile);
		sprintf(tempFile->allPath,"%s/%s",dir->directoryName,tempFile->filename);
	//	printf("%s\n",tempFile->allPath);
		dir->files.push_back(tempFile);
		if(tempFile->isDirectory)
		{
			tempFile->directory = createMem<_XDir>();
			tempFile->directory->level = dir->level + 1;
			enumerationFilesEx(tempFile->allPath,tempFile->directory);
		}
	}
	//下面开始遍历整个文件夹
	while(1)
	{
		if(!FindNextFile(fileHandle,&fileInfo)) break;
		if(strcmp(fileInfo.cFileName,".") != 0 && strcmp(fileInfo.cFileName,"..") != 0)
		{
			tempFile = createMem<_XFile>();
			if(tempFile == NULL) 
			{//这里需要释放资源
				dir->release();
				FindClose(fileHandle);
				return XFalse;
			}
			infoConvert(fileInfo,tempFile);
			sprintf(tempFile->allPath,"%s/%s",dir->directoryName,tempFile->filename);
		//	printf("%s\n",tempFile->allPath);
			dir->files.push_back(tempFile);
			if(tempFile->isDirectory)
			{
				tempFile->directory = createMem<_XDir>();
				tempFile->directory->level = dir->level + 1;
				enumerationFilesEx(tempFile->allPath,tempFile->directory);
			}
		}
	}
	FindClose(fileHandle);
	dir->isEnable = XTrue;
	return XTrue;
}
#include "Shlwapi.h"
_XBool isExistFile(const char *path)
{
	if(path == NULL) return XFalse;
	return PathFileExists(path);
}
_XBool isExistFileEx(const char *path)
{
	if(path == NULL) return XFalse;
	char nowDirectory[MAX_FILE_NAME_LENGTH];
	GetCurrentDirectory(MAX_FILE_NAME_LENGTH,nowDirectory);
	char armPath[MAX_FILE_NAME_LENGTH];
	sprintf(armPath,"%s/%s",nowDirectory,path);
	return PathFileExists(armPath);
}
_XBool isFolder(const char * path)
{
	if(path == NULL) return XFalse;
	return (GetFileAttributes(path) & FILE_ATTRIBUTE_DIRECTORY);
}
_XBool isFolderEx(const char * path)
{
	if(path == NULL) return XFalse;
	char nowDirectory[MAX_FILE_NAME_LENGTH];
	GetCurrentDirectory(MAX_FILE_NAME_LENGTH,nowDirectory);
	char armPath[MAX_FILE_NAME_LENGTH];
	sprintf(armPath,"%s/%s",nowDirectory,path);
	return (GetFileAttributes(armPath) & FILE_ATTRIBUTE_DIRECTORY);
}
_XBool deleteFile(const char *filename)
{
	if(filename == NULL) return XFalse;
#ifdef UNICODE
	return DeleteFileA(filename);
#else
	return DeleteFile(filename);
#endif
}