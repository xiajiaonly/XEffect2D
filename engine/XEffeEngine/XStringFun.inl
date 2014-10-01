inline int hexCharToInt(char p)	//16进制字符转int的数值
{
	if(p >= '0' && p <= '9') return p - '0';
	if(p >= 'a' && p <= 'f') return p - 'a' + 10;
	if(p >= 'A' && p <= 'F') return p - 'A' + 10;
	return '0';
}
inline _XBool hexToBin(const char *in,unsigned char *out)
{
	if(in == NULL || out == NULL) return XFalse;
	int len = 0;
	if((len = strlen(in)) %2 != 0) return XFalse;
	int tmp0;
	int tmp1;
	for(int i = 0;i < len;i += 2)
	{
		tmp0 = hexCharToInt(in[i]);
		if(tmp0 == '0') return XFalse;
		tmp1 = hexCharToInt(in[i + 1]);
		if(tmp1 == '0') return XFalse;
		out[i >> 1] = (tmp0 << 4) + tmp1;
	}
	return XTrue;
}
inline std::string intToStr(int temp,int radix)
{
	static char str[128];
	itoa(temp,str,radix);
	return str;
}
inline std::string floatToStr(float temp,int len)
{
	static char str[128];
	gcvt(temp,len,str);
	return str;
}
inline std::string getWorkPath()
{//获取大当前工作路径
	static char path[512];
	GetCurrentDirectory(512,path);
	return path;
}
inline std::string getFileExtension(const char * filename)
{
	if(filename == NULL) return " ";
	int len = getCharPosition(filename,'.',1);
	if(len < 0) return " ";
	return filename + len + 1;
}
inline void setWorkPath(const std::string &path)
{//设置当前工作路径
	SetCurrentDirectory(path.c_str());
}
inline std::string getCurrentExeFileFullPath()
{//获取当前exe文件的全路径文件名
	static char lpFileName[MAX_PATH];
	GetModuleFileName(NULL,lpFileName,MAX_PATH);
	return lpFileName;
}
inline std::string getFullPath(const std::string &filename)
{//相对路径转换成绝对路径
	//方案1、自己分析，当相对路径不存在时这里会存在问题
	//static char currentPath[MAX_PATH + 1];
	//GetCurrentDirectory(MAX_PATH,currentPath);	//记录当前的路径
	//int len = getCharPosition(filename.c_str(),'\\',1);
	//int len1 = getCharPosition(filename.c_str(),'/',1);
	//if(len < 0 && len1 < 0) return getWorkPath() + "/" + filename;
	//
	//static char path[MAX_PATH + 1];
	//static char path1[MAX_PATH + 1];
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
	static char currentPath[MAX_PATH + 1];
	if(!GetFullPathName(filename.c_str(),MAX_PATH + 1,currentPath,NULL)) return "";
	return currentPath;
}
inline _XBool isAbsolutePath(const char * path)
{
	if(path == NULL) return XTrue;
	if(strlen(path) > 2 && path[1] == ':') return XTrue;
	return XFalse;
}
inline std::string mergeFilename(const std::string &base,const std::string &offset)
{//这里假设base和offset都是合法的路径
	if(isAbsolutePath(offset.c_str())) return offset;
	int depth,lastPos;
	calPathDepth(offset.c_str(),depth,lastPos);
	std::string tmp = getBackPath(base.c_str(),depth);
	return tmp + (offset.c_str() + lastPos);
}
inline void setCapsLockState(_XBool state)
{
	BYTE btKeyState[256];
	if(!GetKeyboardState((LPBYTE)&btKeyState)) return;
	 
	if((!(btKeyState[VK_CAPITAL] & 1) && state) ||
		((btKeyState[VK_CAPITAL] & 1) && !state))
	{//如果按键没有亮，则设置为亮
		keybd_event(VK_CAPITAL, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
		keybd_event(VK_CAPITAL, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
	}
}
inline void setNumLockState(_XBool state)
{
	BYTE btKeyState[256];
	if(!GetKeyboardState((LPBYTE)&btKeyState)) return;
	 
	if((!(btKeyState[VK_NUMLOCK] & 1) && state) ||
		((btKeyState[VK_NUMLOCK] & 1) && !state))
	{//如果按键没有亮，则设置为亮
		keybd_event(VK_NUMLOCK, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
		keybd_event(VK_NUMLOCK, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
	}
}
inline std::string getChooseFilename(char * defName,char * title,bool isOpen)
{
	char OFN_Direct[MAX_PATH];	//全路径
	char OFN_Name[MAX_PATH];	//文件名
	OFN_Name[0] ='\0';
	//strcpy(OFN_Name,"c:config.xml");
	if(defName == NULL) OFN_Direct[0] ='\0';
	else strcpy(OFN_Direct,defName);
	if(!popFileOpenDlg(NULL,OFN_Direct,OFN_Name,title,isOpen) || strlen(OFN_Name) <= 0) return "";
	return OFN_Direct;
}
inline std::string getWindowsSysFontPath()
{
	std::string fontsDir = getenv ("windir");
	fontsDir += "\\Fonts\\";
	return fontsDir;
}
inline std::string getFilenameFormPath(const char *path)
{
	if(path == NULL) return "";
	int ret = getPathDeepByChar(path);
	if(path[ret] == '/' || path[ret] == '\\') return path + ret + 1;
	return path + ret;
}
inline wchar_t oneCharToWchar(const char* src)
{
	if(src == NULL) return NULL;
	wchar_t ret;
	MultiByteToWideChar(CP_ACP, 0, src, 2,&ret, 1);
	return ret;
}
inline std::string Wchar2ANSI(const wchar_t* src)
{
	if(src == NULL) return NULL;
    int nRetLen = WideCharToMultiByte(CP_ACP, 0,src, -1, NULL, 0, NULL, NULL);
 	std::string ret;
	ret.resize(nRetLen + 1);
    WideCharToMultiByte(CP_ACP, 0, src, -1, &ret[0], nRetLen, NULL, NULL);
    return ret;
}