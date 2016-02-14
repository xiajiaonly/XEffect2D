#include "XStdHead.h"
#include "XPixelSDL.h"
#include "XResourcePack.h"
#include "XNetWork/XHTTPRequest.h"
#include "SDL_image.h"
namespace XE{
inline SDL_Surface * loadImageMem(const void *data,int len)	//从内存中载入图片数据
{
	if(data == NULL || len <= 0) return NULL;
	SDL_RWops *fileData = SDL_RWFromConstMem(data,len);
	if(fileData == NULL) return NULL;
	SDL_Surface *img = IMG_Load_RW(fileData,1);
	return img;
}
SDL_Surface * loadImageEx(const char *pFileName,XResourcePosition resoursePosition)
{
	SDL_Surface *temp_back = NULL;
	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = getDefResPos();
	switch(resoursePosition)
	{
	case RESOURCE_LOCAL_FOLDER://这里是没有经过资源管理器的，所以会出现资源管理器没有管理的资源
		temp_back = IMG_Load(XString::ANSI2UTF8(pFileName).c_str());
		break;
	case RESOURCE_LOCAL_PACK:
		{//对于内部资源的读取
			//写入数据
			int length = XResPack.getFileLength(pFileName);
			if(length < 0) return NULL;
			unsigned char *p = XMem::createArrayMem<unsigned char>(length + 1);
			if(p == NULL) return NULL;
			//printf("%d\n",length);
			if(XResPack.unpackResource(pFileName,p) != 1)
			{
				//printf("LoadIMG Error!\n");
				LogStr("LoadIMG Error!");
				return NULL;
			}
			temp_back = loadImageMem(p,length);
			XMem::XDELETE_ARRAY(p);
		}
		break;
	case RESOURCE_WEB:
		{
			XHTTPRequest tempRequest;
			XHTTPRequestStruct tempGet;
			char fileName[MAX_FILE_NAME_LENGTH];
			strcpy(fileName,pFileName);
			if(tempRequest.sendRequest(XFalse,fileName,tempGet) != 0)	//获取网络数据
			{//读取到网络数据
				temp_back = loadImageMem(tempGet.message,tempGet.messageLength);
				XMem::XDELETE_ARRAY(tempGet.headerReceive);
				XMem::XDELETE_ARRAY(tempGet.headerSend);
				XMem::XDELETE_ARRAY(tempGet.message);
			}
		}
		break; 
	case RESOURCE_AUTO:
		{
		//packer
			int length = XResPack.getFileLength(pFileName);
			if(length >= 0)
			{
				unsigned char *p = XMem::createArrayMem<unsigned char>(length + 1);
				if(p != NULL && XResPack.unpackResource(pFileName,p) == 1)
				{
					temp_back = loadImageMem(p,length);
					XMem::XDELETE_ARRAY(p);
					if(temp_back != NULL) break;
				}
			}
		//folder
			temp_back = IMG_Load(XString::ANSI2UTF8(pFileName).c_str());
			if(temp_back != NULL) break;
		//web
			XHTTPRequest tempRequest;
			XHTTPRequestStruct tempGet;
			char fileName[MAX_FILE_NAME_LENGTH];
			strcpy(fileName,pFileName);
			if(tempRequest.sendRequest(XFalse,fileName,tempGet) != 0)	//获取网络数据
			{//读取到网络数据
				temp_back = loadImageMem(tempGet.message,tempGet.messageLength);
				XMem::XDELETE_ARRAY(tempGet.headerReceive);
				XMem::XDELETE_ARRAY(tempGet.headerSend);
				XMem::XDELETE_ARRAY(tempGet.message);
			}
		}
		break;
	}
    if(temp_back == NULL) LogNull("%s file load error!\n",pFileName);
    return temp_back;
}
bool XPixelSDL::load(const XBuffer &buff)
{
	if(isInited) return false;
	SDL_Surface * tmp = loadImageMem(buff.getBuffer(),buff.getUsage());
	if(tmp == NULL) return false;
	w = tmp->w;
	h = tmp->h;
	switch(tmp->format->BytesPerPixel)
	{
	case 4: 
		if(tmp->format->Rshift == 0) m_colorMode = COLOR_RGBA;
		else m_colorMode = COLOR_BGRA;
		break;
	case 3: 
		if(tmp->format->Rshift == 0) m_colorMode = COLOR_RGB;
		else m_colorMode = COLOR_BGR;
		break;
	case 1: m_colorMode = COLOR_GRAY;break;
	default: return false; break;
	}
	int size = w * h * tmp->format->BytesPerPixel;
	m_pPixels = XMem::createArrayMem<unsigned char>(size);
	if(m_pPixels == NULL) return false;
	memcpy(m_pPixels,tmp->pixels,size);
	SDL_FreeSurface(tmp);

	isInited = true;
	return true;
}
bool XPixelSDL::load(const std::string &filename,XResourcePosition resPos)
{
	if(isInited) return false;
	SDL_Surface * tmp = loadImageEx(filename.c_str(),resPos);
	if(tmp == NULL) return false;
	w = tmp->w;
	h = tmp->h;
	switch(tmp->format->BytesPerPixel)
	{
	case 4: 
		if(tmp->format->Rshift == 0) m_colorMode = COLOR_RGBA;
		else m_colorMode = COLOR_BGRA;
		break;
	case 3: 
		if(tmp->format->Rshift == 0) m_colorMode = COLOR_RGB;
		else m_colorMode = COLOR_BGR;
		break;
	case 1: m_colorMode = COLOR_GRAY;break;
	default: return false; break;
	}
	int size = w * h * tmp->format->BytesPerPixel;
	m_pPixels = XMem::createArrayMem<unsigned char>(size);
	if(m_pPixels == NULL) return false;
	memcpy(m_pPixels,tmp->pixels,size);
	SDL_FreeSurface(tmp);

	isInited = true;
	return true;
}
bool XPixelSDL::fitNPot()
{
	if(!isInited ||
		!XMath::isNPOT(w,h)) return true;
	int ow = w;
	int oh = h;
	unsigned char *p = m_pPixels;
	//下面处理
	w = XMath::getMinWellSize2n(ow);
	h = XMath::getMinWellSize2n(oh);
	int pW = 0,poW = 0;
	int offsetH = (h - oh) >> 1;
	int offsetW = (w - ow) >> 1;
	switch(m_colorMode)
	{
	case COLOR_RGBA:
	case COLOR_BGRA:
		pW = w << 2;
		poW = ow << 2;
		offsetW = offsetW << 2;
		break;
	case COLOR_RGB:
	case COLOR_BGR:
		pW = w * 3;
		poW = ow * 3;
		offsetW = offsetW * 3;
		break;
	case COLOR_GRAY:
		pW = w;
		poW = ow;
		break;
	default:	//其他格式不支持该操作
		return false;
	}
	m_pPixels = XMem::createArrayMem<unsigned char>(pW * h);
	if(m_pPixels == NULL)
	{
		m_pPixels = p;
		w = ow;
		h = oh;
		return false;
	}
	//下面拷贝数据
	memset(m_pPixels,0,pW * h);
	for(int i = offsetH;i < oh + offsetH;++ i)
	{
		memcpy(m_pPixels + i * pW + offsetW,p + (i - offsetH) * poW,poW);
	}
	XMem::XDELETE_ARRAY(p);
	return true;
}
}