inline std::string getWorkPath()
{//获取大当前工作路径
	static char path[512] = "";
#ifdef UNICODE
	if (GetCurrentDirectoryA(512, path) <= 0) return "";
#else
	if (GetCurrentDirectory(512, path) <= 0) return "";
#endif
	return path;
}
inline std::string getFileExtension(const char * filename)
{//获取文件的扩展名
	if (filename == NULL) return " ";
	int len = XString::getCharPosition(filename, '.', 1);
	if (len < 0) return " ";
	return filename + len + 1;
}
inline XBool setWorkPath(const std::string &path)
{//设置当前工作路径
#ifdef UNICODE
	return SetCurrentDirectoryA(path.c_str());
#else
	return SetCurrentDirectory(path.c_str());
#endif
}
inline std::string getCurrentExeFileFullPath()
{//获取当前exe文件的全路径文件名
	char lpFileName[MAX_PATH];
#ifdef UNICODE
	if (GetModuleFileNameA(NULL, lpFileName, MAX_PATH) <= 0) return "";
#else
	if (GetModuleFileName(NULL, lpFileName, MAX_PATH) <= 0) return "";
#endif
	return lpFileName;
}
inline std::string getFullPath(const std::string &filename)
{//相对路径转换成绝对路径
	//方案1、自己分析，当相对路径不存在时这里会存在问题
	//char currentPath[MAX_PATH + 1];
	//GetCurrentDirectory(MAX_PATH,currentPath);	//记录当前的路径
	//int len = XString::getCharPosition(filename.c_str(),'\\',1);
	//int len1 = XString::getCharPosition(filename.c_str(),'/',1);
	//if(len < 0 && len1 < 0) return getWorkPath() + "/" + filename;
	//
	//char path[MAX_PATH + 1];
	//char path1[MAX_PATH + 1];
	//strcpy(path,filename.c_str());
	//if(len >= len1) len = len + 1;
	//else len = len1 + 1;
	//path[len] = '\0';
	//SetCurrentDirectory(path);  //用得到的相对路径设置当前目录
	//GetCurrentDirectory(MAX_PATH,path1);  //得到当前目录的绝对路径
	//
	//SetCurrentDirectory(currentPath);	//恢复当前的路径
	//strcat(path1,"/");
	//strcpy(path,filename.c_str());
	//strcat(path1,path + len);
	//return  path1;
	//方案2、直接调用系统API，妥妥的
	char currentPath[MAX_PATH + 1];
#ifdef UNICODE
	if (!GetFullPathNameA(filename.c_str(), MAX_PATH + 1, currentPath, NULL)) return "";
#else
	if (!GetFullPathName(filename.c_str(), MAX_PATH + 1, currentPath, NULL)) return "";
#endif
	return currentPath;
}
inline XBool isAbsolutePath(const char * path)	//是否为绝对路径
{
	if (path == NULL) return XTrue;
	if (strlen(path) > 2 && path[1] == ':') return XTrue;
	return XFalse;
}
inline std::string mergeFilename(const std::string &base, const std::string &offset)
{//这里假设base和offset都是合法的路径
	if (isAbsolutePath(offset.c_str())) return offset;
	int depth, lastPos;
	calPathDepth(offset.c_str(), depth, lastPos);
	std::string tmp = getBackPath(base.c_str(), depth);
	return tmp + (offset.c_str() + lastPos);
}
//defName:默认的文件名
//title:窗口名称
//isOpen:按钮是否为保存
//defExt:默认扩展名
inline std::string getChooseFilename(char * defName, char * title, bool isOpen, const std::string&defExt)
{
	char OFN_Direct[MAX_PATH];	//全路径
	char OFN_Name[MAX_PATH];	//文件名
	memset(OFN_Direct,0,MAX_PATH);
	memset(OFN_Name,0,MAX_PATH);
	//strcpy(OFN_Name,"c:config.xml");
	if (defName != NULL) strcpy(OFN_Direct, defName);
	if (!popFileOpenDlg(XEG.getHWND(), OFN_Direct, OFN_Name, title, isOpen, defExt) || strlen(OFN_Name) <= 0) return "";
	return OFN_Direct;
}
inline std::string getWindowsSysFontPath()
{
	std::string fontsDir = getenv("windir");
	fontsDir += "\\Fonts\\";
	return fontsDir;
}
inline std::string getFilenameFormPath(const char *path)
{
	if (path == NULL) return "";
	int ret = getPathDeepByChar(path);
	if (path[ret] == '/' || path[ret] == '\\') return path + ret + 1;
	return path + ret;
}
inline std::string getFilePath(const char *filename)
{
	if(filename == NULL) return "";
	char tmpFilename[MAX_PATH];
	strcpy(tmpFilename,filename);
	int index = getPathDeepByChar(tmpFilename);
	if(index == 0) return "";	//路径不是绝对路径
	tmpFilename[index] = '\0';
	return tmpFilename;
}