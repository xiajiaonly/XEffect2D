#include "XStdHead.h"
#include "XFile.h"
#include "XMath\XCColor.h"
#include "XResourcePack.h"
#include "shlobj.h"
//#include "XLogBook.h"
namespace XE{
namespace XFile
{
	char *readFileToStr(const char *filename,XResourcePosition resoursePosition) 
	{
		if(resoursePosition == RESOURCE_SYSTEM_DEFINE)
			resoursePosition = getDefResPos();
		switch(resoursePosition)
		{
		case RESOURCE_LOCAL_PACK:
			return (char *)XResPack.getFileData(filename);
		case RESOURCE_LOCAL_FOLDER:
			{
				FILE *fp;
				char *content = NULL;
				if(filename != NULL && (fp = fopen(filename,"rt")) != NULL) 
				{
					fseek(fp, 0, SEEK_END);
					int count = ftell(fp);
					rewind(fp);

					if(count > 0) 
					{
						//content = (char *)malloc(sizeof(char) * (count + 1));
						content = XMem::createArrayMem<char>(sizeof(char) * (count + 1));
						count = fread(content,sizeof(char),count,fp);
						content[count] = '\0';
					}
					fclose(fp);
				}
				return content; 
			}
		case RESOURCE_WEB:
			return NULL;
		case RESOURCE_AUTO:
			{
				//packer中加载
				char * content = (char *)XResPack.getFileData(filename);
				if(content != NULL) return content;
				//folder中加载
				FILE *fp;
				if(filename != NULL && (fp = fopen(filename,"rt")) != NULL) 
				{
					fseek(fp, 0, SEEK_END);
					int count = ftell(fp);
					rewind(fp);

					if(count > 0) 
					{
						//content = (char *)malloc(sizeof(char) * (count + 1));
						content = XMem::createArrayMem<char>(sizeof(char) * (count + 1));
						count = fread(content,sizeof(char),count,fp);
						content[count] = '\0';
					}
					fclose(fp);
				}
				if(content != NULL) return content; 
				//web中加载
				return NULL;
			}
			break;
		default:
			return NULL;
		}
	}
	int writeStrToFile(const char *filename,const char *s) 
	{
		if(filename == NULL ||
			s == NULL) return 0;
		FILE *fp = NULL;
		int status = 0;

		if((fp = fopen(filename,"w")) != NULL) 
		{	
			if(fwrite(s,sizeof(char),strlen(s),fp) == strlen(s)) status = 1;
			fclose(fp);
		}
		return(status);
	}
	bool getFileString(const char *filename,std::string &outStr)	//从指定的文件中读取数据
	{
		if(filename == NULL) return false;
		FILE *fp = NULL;
		if((fp = fopen(filename,"r")) == NULL)
		{
			LogNull("打开文件失败:%s!",filename);
			return false;
		}
		fseek(fp, 0, SEEK_END);
		int len = ftell(fp);
		rewind(fp);
		if(len <= 0)
		{
			fclose(fp);
			return false;
		}
		outStr.resize(len);
		fread(&outStr[0],1,len,fp);
		fclose(fp);
		return true;
	}
	std::string addIndexToFilename(const char *filename,int index)
	{
		if(filename == NULL || strlen(filename) >= 1024) return "";	//文件名长度非法
		char tmpFilename[1024];
		strcpy(tmpFilename,filename);
		int pos = XString::getCharPosition(tmpFilename,'.',1);
		if(pos < 0)
		{
			return std::string(filename) + "(" + XString::toString(index) + ")";
		}else
		{
			tmpFilename[pos] = '\0';
			return std::string(tmpFilename) + "(" + XString::toString(index) + ")." + std::string(tmpFilename + pos + 1); 
		}
	}
	XBool fileNameCompare(const char *p1, const char *p2)
	{
		if (p1 == NULL || p2 == NULL) return XFalse;
		int len1 = strlen(p1);
		int len2 = strlen(p2);
		if (len1 != len2) return XFalse;
		//int temp = 'A' - 'a';
		for (int i = 0; i < len1; ++i)
		{
			if (p1[i] == p2[i] && p2[i] < 0)
			{//汉字
				if (i + 1 < len1)
				{
					if (p1[i + 1] == p2[i + 1])
					{
						++i;
						continue;
					}else
					{
						return XFalse;
					}
				}
			}
			if (p1[i] == p2[i]) continue;
			if (p1[i] >= 'a' && p1[i] <= 'z' && p1[i] - 'a' + 'A' == p2[i]) continue; else
			if (p1[i] >= 'A' && p1[i] <= 'Z' && p1[i] - 'A' + 'a' == p2[i]) continue; else
			if ((p1[i] == '/' && p2[i] == '\\') || (p1[i] == '\\' && p2[i] == '/')) continue;
			return XFalse;
		}
		return XTrue;
	}
	int getPathDeep(const char *p)
	{
		if (p == NULL) return 0;
		int len = strlen(p);
		if (len >= MAX_FILE_NAME_LENGTH) return 0;
		int deep = 0;
		for (int i = 0; i < len; ++i)
		{
			if (p[i] < 0)
			{//汉字
				++i;
				continue;
			}
			if (p[i] == '\\' || p[i] == '/') ++deep;
		}
		return deep;
	}
	int getPathDeepByChar(const char *p)
	{
		if (p == NULL) return -1;
		int len = strlen(p);
		if (len >= MAX_FILE_NAME_LENGTH) return -1;
		int ret = -1;
		for(int i = 0;i < len;++ i)
		{
			if(p[i] < 0)
			{
				++ i;
				continue;
			}
			if (p[i] == '\\' || p[i] == '/') ret = i;
		}
		return ret;
		//for (int i = len - 1; i >= 0; --i)
		//{
		//	if (i - 1 >= 0 && p[i - 1] < 0)
		//	{//汉字
		//		--i;
		//		continue;
		//	}
		//	if (p[i] == '\\' || p[i] == '/') return i;
		//}
		//return 0;

		//int len = strlen(p);
		//int deepByChar = 0;
		//for(int i = 0;i < len;++ i)
		//{
		//	if(p[i] == '\\' || p[i] == '/') 
		//	{
		//		deepByChar = i;
		//	}
		//}
		//return deepByChar;
	}
	void calPathDepth(const char *p, int &depth, int &lastPos)
	{//这里假设路径是合法的
		if (p == NULL) return;
		int len = strlen(p);
		int pointSum = 0;
		depth = 0;
		for (int i = 0; i < len; ++i)
		{
			if (p[i] == '.')
			{
				++ pointSum;
				continue;
			}
			if (p[i] == '\\' || p[i] == '/')
			{
				if (pointSum == 2) ++depth;
				pointSum = 0;
				continue;
			}
			lastPos = i;
			break;
		}
	}
	std::string getBackPath(const char *p, int backSum)
	{
		if (p == NULL || backSum < 0) return p;
		if (backSum == 0)
		{//整理路径而已
			char buffer[MAX_PATH + 1] = "";
			strcpy(buffer, p);
			int len = strlen(buffer);
			for (int i = len - 1; i >= 0; --i)
			{
				if (buffer[i] == '\\' || buffer[i] == '/')
				{
					buffer[i + 1] = '\0';
					return buffer;
				}
			}
			buffer[0] = '\0';
			return buffer;
		}
		int depth, lastPos;
		calPathDepth(p, depth, lastPos);
		int tmp = getPathDeep(p + lastPos);
		if (tmp == backSum)
		{//正好
			char buffer[MAX_PATH + 1] = "";
			strcpy(buffer, p);
			buffer[lastPos] = '\0';
			return buffer;
		}
		else
		if (tmp < backSum)
		{//还需要退
			char buffer[MAX_PATH + 1] = "";
			strcpy(buffer, p);
			int index = lastPos;
			for (int i = 0; i < backSum - tmp; ++i, index += 3)
			{
				buffer[index] = '.';
				buffer[index + 1] = '.';
				buffer[index + 2] = '/';
			}
			buffer[index] = '\0';
			return buffer;
		}
		else
		{//有路径
			char buffer[MAX_PATH + 1] = "";
			strcpy(buffer, p);
			int index = 0;
			int len = strlen(buffer);
			for (int i = lastPos + 1; i < len; ++i)
			{
				if (buffer[i] == '\\' || buffer[i] == '/')
				{
					++ index;
					if (index >= tmp - backSum)
					{
						buffer[i + 1] = '\0';
					}
				}
			}
			return buffer;
		}
	}
	int getCharsNumInPath(const char * path)
	{//尚未经过验证
		int cnt = 0;
		int n = strlen(path);

		for (int i = 0; i < n; ++i)
		{
			if ((path[i] < 0x80) || (path[i] > 0xbf))
				++cnt;
		}

		return cnt;
	}
	bool isPathValid(const char * path)
	{//检查路径是否合法，尚未经过验证(目前存在问题：如./../data/a.dat)
		if (path == NULL) return false;

		if (getCharsNumInPath(path) > MAX_PATH) return false;
		int n = strlen(path);

		for (int i = 0; i < n; ++i)
		{
			switch (path[i])
			{
				// ? " / < > * |
			case '?':
			case '\"':
				//case '/':
			case '<':
			case '>':
			case '*':
			case '|':
				return false;
				// for example D:\folder\file.txt
			case ':':
				if (i != 1)  return false;
				else break;
			case ' ':
			case '.':
				if (i + 1 == n || path[i + 1] == '\\' || path[i + 1] == '/') return false;
				else break;
			case '\\':
				if (i > 0 && (path[i - 1] == '\\' || path[i - 1] == '/')) return false;
				else break;
			case '/':
				if (i > 0 && (path[i - 1] == '\\' || path[i - 1] == '/')) return false;
				else break;
			}
		}
		return true;
	}
	BOOL popFileOpenDlg(HWND hwnd, char* pstrFileName, char* pstrTitleName, char* title, bool isOpen, const std::string&defExt)
	{//弹出文件打开窗口并返回选择的文件的相对路径和绝对路径
		OPENFILENAME ofn;
		char initDir[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, initDir);	//设置当前路径为默认路径
		//std::string tmpStr = defExt + " files(*." + defExt + ")\0*." + defExt + "\0";

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof (OPENFILENAME);
		ofn.hInstance = NULL;
		if (defExt == "*")
		{
			ofn.lpstrFilter = TEXT("All Files(*.*)\0*.*\0");
		}
		else
		{
			if (defExt.length() > 128) return FALSE;
			char tmpStr[1024];
			memset(tmpStr,0,1024);
			sprintf(tmpStr, "%s Files(*.%s)", defExt.c_str(), defExt.c_str());
			int len = strlen(tmpStr);
			sprintf(tmpStr + len + 1, "*.%s", defExt.c_str());
			ofn.lpstrFilter = TEXT(tmpStr);
		}
		//std::string tmpDir = initDir + std::string("\\");
		ofn.lpstrCustomFilter = NULL;
		ofn.nMaxCustFilter = 0;
		ofn.nFilterIndex = 0;
		ofn.nMaxFile = MAX_PATH - 1;
		ofn.nMaxFileTitle = MAX_PATH - 1;
		ofn.lpstrInitialDir = initDir;
		ofn.lpstrTitle = TEXT(title);
		ofn.nFileOffset = 0;
		ofn.nFileExtension = 0;
		ofn.lpstrDefExt = TEXT(defExt.c_str());
		ofn.lCustData = 0L;
		ofn.lpfnHook = NULL;
		ofn.lpTemplateName = NULL;

		ofn.hwndOwner = hwnd;
		ofn.lpstrFile = pstrFileName;
		ofn.lpstrFileTitle = pstrTitleName;
		ofn.Flags = OFN_HIDEREADONLY;// | OFN_CREATEPROMPT;	//貌似后面这个标识符是多余的
		if (isOpen) return GetOpenFileName(&ofn);
		else return GetSaveFileName(&ofn);
	}
	std::string getChooseFolderName()
	{
		//LPITEMIDLIST pidlRoot=NULL;
		//SHGetSpecialFolderLocation(XEG.getHWND(),CSIDL_DRIVES,&pidlRoot);
		char szPath[MAX_PATH];                       //保存路径变量
		memset(szPath, 0, sizeof(szPath)); 
		char szLan[32] = "Choose folder";
		BROWSEINFOA bi = {0};                              // BROWSEINFO结构变量
		bi.hwndOwner		= XEG.getHWND();         //HWND句柄
		bi.pidlRoot			= NULL;//pidlRoot;              //默认值为NULL
		bi.lpszTitle		= (LPSTR)szLan;				 //对话框标题
		bi.pszDisplayName	= (LPSTR)szPath;                //选择文件夹路径
		bi.ulFlags			= BIF_RETURNONLYFSDIRS;  //标记
		bi.lpfn				= NULL;                  //默认值为NULL
		bi.lParam			= NULL;                  //回调消息
		bi.iImage			= 0;
		LPITEMIDLIST pItemIDList = SHBrowseForFolderA(&bi);			//显示文件浏览对话框
		if(pItemIDList && SHGetPathFromIDListA(pItemIDList,szPath))	//判断是否获得文件夹路径
			return szPath; 
		return ""; //文件夹路径为空
	}
	XCColor getChooseColor(const XCColor &color)
	{
		CHOOSECOLOR tmpC;
		COLORREF colorX((color.r) + (color.g << 8) + (color.a << 16) + 0xff000000);
		ZeroMemory(&tmpC, sizeof(tmpC));
		tmpC.lStructSize = sizeof (CHOOSECOLOR);
		tmpC.rgbResult = colorX;
		tmpC.lpCustColors = &colorX;
		tmpC.hwndOwner = XEG.getHWND();
		tmpC.Flags = CC_RGBINIT | CC_FULLOPEN;
		if (!ChooseColor(&tmpC)) return color;
		return XCColor(tmpC.rgbResult % 256, (tmpC.rgbResult >> 8) % 256, (tmpC.rgbResult >> 16) % 256, 0xff);
	}
	XBool isExistFileEx(const char *path)
	{
		if(path == NULL) return XFalse;
		std::string tmp = getFullPath(path);
	//	char curDirectory[MAX_FILE_NAME_LENGTH];
	//	GetCurrentDirectory(MAX_FILE_NAME_LENGTH,curDirectory);
	//	char armPath[MAX_FILE_NAME_LENGTH];
	//	sprintf(armPath,"%s/%s",curDirectory,path);
		return PathFileExists(tmp.c_str());
	}
	XBool isFolderEx(const char * path)
	{
		if(path == NULL) return XFalse;
		std::string tmp = getFullPath(path);
	//	char curDirectory[MAX_FILE_NAME_LENGTH];
	//	GetCurrentDirectory(MAX_FILE_NAME_LENGTH,curDirectory);
	//	char armPath[MAX_FILE_NAME_LENGTH];
	//	sprintf(armPath,"%s/%s",curDirectory,path);
		return (GetFileAttributes(tmp.c_str()) & FILE_ATTRIBUTE_DIRECTORY);
	}
#if !WITH_INLINE_FILE
#include "XFile.inl"
#endif
}
}