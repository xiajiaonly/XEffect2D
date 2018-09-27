#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XDirectory.h"
#include "XFile.h"
namespace XE {
void XFileInfo::release()
{
	if (!isEnable) return;
	//	XMem::XDELETE_ARRAY(allPath);
	//	XMem::XDELETE_ARRAY(filename);
	if (directory != NULL) directory->release();
	XMem::XDELETE(directory);
	isEnable = XFalse;
}
void XDir::release()
{
	if (!isEnable) return;
	//	XMem::XDELETE_ARRAY(directoryName);
	int size = files.size();
	for (int i = 0; i < size; ++i)
	{
		files[i]->release();
		XMem::XDELETE(files[i]);
	}
	files.clear();
	//files.swap(std::vector<XFileInfo *>());//释放内存空间
	isEnable = XFalse;
}
XBool XDirectory::init(const char *directoryName, bool withChildFolder)
{
	if (m_isInited) return XTrue;
	if (directoryName == NULL) return XFalse;

	m_curDirectory.level = 0;
	if (!XFile::enumerationFiles(directoryName, m_curDirectory, withChildFolder)) return XFalse;

	m_isInited = XTrue;
	return XTrue;
}
XBool XDirectory::initEx(const char *directoryName, bool withChildFolder)
{
	if (m_isInited) return XTrue;
	if (directoryName == NULL) return XFalse;

	m_curDirectory.level = 0;
	if (XFile::isAbsolutePath(directoryName))
	{
		if (!XFile::enumerationFiles(directoryName, m_curDirectory, withChildFolder)) return XFalse;
	}
	else
		if (!XFile::enumerationFilesEx(directoryName, m_curDirectory, withChildFolder)) return XFalse;

	m_isInited = XTrue;
	return XTrue;
}
void XDirectory::listFiles(const XDir&dir)	//列出路径下的所有文件，并通过过滤器进行过滤
{//遍历所有文件
	for (int i = 0; i < dir.files.size(); ++i)
	{
		if (dir.files[i]->isDirectory) listFiles(*dir.files[i]->directory);
		else
		{
			if (m_filterNames.size() > 0)
			{//过滤
				std::string tmpName = XFile::getFileExtension(dir.files[i]->filename.c_str());
				for (int j = 0; j < m_filterNames.size(); ++j)
				{
					if (XFile::fileNameCompare(m_filterNames[j].c_str(), tmpName.c_str()))
					{//扩展名符合要求
						m_sortResult.push_back(dir.files[i]);
						break;
					}
				}
			}
			else
			{//直接使用
				m_sortResult.push_back(dir.files[i]);
			}
		}
	}
}
void XDirectory::setFilterName(const std::string& name)	//设置过滤的文件扩展名
{
	for (int i = 0; i < m_filterNames.size(); ++i)
	{
		if (XFile::fileNameCompare(m_filterNames[i].c_str(), name.c_str())) return;	//排除重复的
	}
	m_filterNames.push_back(name);
}
void XDirectory::sortFiles(XSortMode mode)	//请先过滤文件再进行排序
{
	if (m_curSortMode == mode) return;	//重复的排序
	switch (mode)
	{
	case SORT_BY_SYSTEM:filterFiles(); break;
	case SORT_BY_SIZE:	//尚未完成
		for (int i = 0; i < m_sortResult.size(); ++i)
		{
			bool flag = true;
			for (int j = 0; j < (int)(m_sortResult.size()) - 1; ++j)
			{
				if (m_sortResult[j]->size > m_sortResult[j + 1]->size)
				{
					XMath::swapTwoSum(m_sortResult[j], m_sortResult[j + 1]);
					flag = false;
				}
			}
			if (flag) break;
		}
		break;
	case SORT_BY_TIME:	//尚未完成
		for (int i = 0; i < m_sortResult.size(); ++i)
		{
			bool flag = true;
			for (int j = 0; j < (int)(m_sortResult.size()) - 1; ++j)
			{
				if (m_sortResult[j]->time.dwHighDateTime > m_sortResult[j + 1]->time.dwHighDateTime ||
					(m_sortResult[j]->time.dwHighDateTime == m_sortResult[j + 1]->time.dwHighDateTime &&
						m_sortResult[j]->time.dwLowDateTime > m_sortResult[j + 1]->time.dwLowDateTime))
				{
					XMath::swapTwoSum(m_sortResult[j], m_sortResult[j + 1]);
					flag = false;
				}
			}
			if (flag) break;
		}
		break;
	}
}
namespace XFile
{
	XBool infoConvert(WIN32_FIND_DATA &fileInfo, XFileInfo &file)
	{
		file.filename = fileInfo.cFileName;
		file.isDirectory = (fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
		file.isEnable = XTrue;
		return XTrue;
	}
	XBool enumerationFiles(const char *path, XDir &dir, bool witchCilder)
	{
		if (path == NULL || strlen(path) == 0) return XFalse;

		dir.directoryName = path;
		if (dir.directoryName[int(dir.directoryName.size()) - 1] == '\\' ||
			dir.directoryName[int(dir.directoryName.size()) - 1] == '/') dir.directoryName[int(dir.directoryName.size()) - 1] = '\0';
		char tempDirectoryName[MAX_FILE_NAME_LENGTH];
		sprintf_s(tempDirectoryName, MAX_FILE_NAME_LENGTH, "%s/*.*", dir.directoryName.c_str());
		//查找目录下的第一个文件
		WIN32_FIND_DATA fileInfo;
		HANDLE fileHandle = FindFirstFile(tempDirectoryName, &fileInfo);
		if (fileHandle == INVALID_HANDLE_VALUE)
		{//错误：空的文件夹或者目录不存在?
			LogNull("error:%d", GetLastError());
			dir.release();
			return XFalse;
		}
		XFileInfo *tempFile = NULL;
		if (strcmp(fileInfo.cFileName, ".") != 0 && strcmp(fileInfo.cFileName, "..") != 0)
		{
			tempFile = XMem::createMem<XFileInfo>();
			if (tempFile == NULL)
			{
				dir.release();
				FindClose(fileHandle);
				return XFalse;
			}
			dir.isEnable = XTrue;
			infoConvert(fileInfo, *tempFile);
			tempFile->allPath = dir.directoryName + "/" + tempFile->filename;
			//	printf("%s\n",tempFile->allPath);
			tempFile->size = fileInfo.nFileSizeHigh;
			tempFile->size = (tempFile->size << 32) + fileInfo.nFileSizeLow;
			tempFile->time = fileInfo.ftLastWriteTime;

			dir.files.push_back(tempFile);
			if (tempFile->isDirectory)
			{
				tempFile->directory = XMem::createMem<XDir>();
				tempFile->directory->level = dir.level + 1;
				if (witchCilder)
					enumerationFiles(tempFile->allPath, *tempFile->directory, witchCilder);
			}
		}
		//下面开始遍历整个文件夹
		while (true)
		{
			if (!FindNextFile(fileHandle, &fileInfo)) break;
			if (strcmp(fileInfo.cFileName, ".") != 0 && strcmp(fileInfo.cFileName, "..") != 0)
			{
				tempFile = XMem::createMem<XFileInfo>();
				if (tempFile == NULL)
				{//这里需要释放资源
					dir.release();
					FindClose(fileHandle);
					return XFalse;
				}
				infoConvert(fileInfo, *tempFile);
				tempFile->allPath = dir.directoryName + "/" + tempFile->filename;
				//	printf("%s\n",tempFile->allPath);
				tempFile->size = fileInfo.nFileSizeHigh;
				tempFile->size = (tempFile->size << 32) + fileInfo.nFileSizeLow;
				tempFile->time = fileInfo.ftLastWriteTime;

				dir.files.push_back(tempFile);
				if (tempFile->isDirectory)
				{
					tempFile->directory = XMem::createMem<XDir>();
					tempFile->directory->level = dir.level + 1;
					if (witchCilder)
						enumerationFiles(tempFile->allPath, *(tempFile->directory), witchCilder);
				}
			}
		}
		FindClose(fileHandle);
		dir.isEnable = XTrue;

		return XTrue;
	}
	XBool enumerationFilesEx(const char *path, XDir &dir, bool witchCilder)	//使用相对路径
	{
		if (path == NULL || strlen(path) == 0) return XFalse;

		char tempDirectoryName[MAX_FILE_NAME_LENGTH];
		char curDirectoryName[MAX_FILE_NAME_LENGTH];
		GetCurrentDirectory(MAX_FILE_NAME_LENGTH, curDirectoryName);	//获取当前路径
		dir.directoryName = path;
		if (dir.directoryName[int(dir.directoryName.size()) - 1] == '\\' ||
			dir.directoryName[int(dir.directoryName.size()) - 1] == '/') dir.directoryName[int(dir.directoryName.size()) - 1] = '\0';
		if (dir.directoryName == ".")
			sprintf_s(tempDirectoryName, MAX_FILE_NAME_LENGTH, "%s/*.*", curDirectoryName);
		else
			sprintf_s(tempDirectoryName, MAX_FILE_NAME_LENGTH, "%s/%s/*.*", curDirectoryName, dir.directoryName.c_str());

		//查找目录下的第一个文件
		WIN32_FIND_DATA fileInfo;
		HANDLE fileHandle = FindFirstFile(tempDirectoryName, &fileInfo);
		if (fileHandle == INVALID_HANDLE_VALUE)
		{//错误：空的文件夹或者目录不存在?
			LogNull("error:%d", GetLastError());
			dir.release();
			return XFalse;
		}
		XFileInfo *tempFile = NULL;
		if (strcmp(fileInfo.cFileName, ".") != 0 && strcmp(fileInfo.cFileName, "..") != 0)
		{
			tempFile = XMem::createMem<XFileInfo>();
			if (tempFile == NULL)
			{
				dir.release();
				FindClose(fileHandle);
				return XFalse;
			}
			dir.isEnable = XTrue;
			infoConvert(fileInfo, *tempFile);
			tempFile->allPath = dir.directoryName + "/" + tempFile->filename;
			//	printf("%s\n",tempFile->allPath);
			tempFile->size = fileInfo.nFileSizeHigh;
			tempFile->size = (tempFile->size << 32) + fileInfo.nFileSizeLow;
			tempFile->time = fileInfo.ftLastWriteTime;

			dir.files.push_back(tempFile);
			if (tempFile->isDirectory)
			{
				tempFile->directory = XMem::createMem<XDir>();
				tempFile->directory->level = dir.level + 1;
				if (witchCilder)
					enumerationFilesEx(tempFile->allPath, *(tempFile->directory), witchCilder);
			}
		}
		//下面开始遍历整个文件夹
		while (true)
		{
			if (!FindNextFile(fileHandle, &fileInfo)) break;
			if (strcmp(fileInfo.cFileName, ".") != 0 && strcmp(fileInfo.cFileName, "..") != 0)
			{
				tempFile = XMem::createMem<XFileInfo>();
				if (tempFile == NULL)
				{//这里需要释放资源
					dir.release();
					FindClose(fileHandle);
					return XFalse;
				}
				infoConvert(fileInfo, *tempFile);
				tempFile->allPath = dir.directoryName + "/" + tempFile->filename;
				//	printf("%s\n",tempFile->allPath);
				tempFile->size = fileInfo.nFileSizeHigh;
				tempFile->size = (tempFile->size << 32) + fileInfo.nFileSizeLow;
				tempFile->time = fileInfo.ftLastWriteTime;

				dir.files.push_back(tempFile);
				if (tempFile->isDirectory)
				{
					tempFile->directory = XMem::createMem<XDir>();
					tempFile->directory->level = dir.level + 1;
					if (witchCilder)
						enumerationFilesEx(tempFile->allPath, *(tempFile->directory), witchCilder);
				}
			}
		}
		FindClose(fileHandle);
		dir.isEnable = XTrue;
		return XTrue;
	}
}
}