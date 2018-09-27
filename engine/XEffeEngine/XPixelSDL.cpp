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
SDL_Surface * loadImageEx(const char *pFileName,XResPos resPos)
{
	SDL_Surface *temp_back = NULL;
	if(resPos == RES_SYS_DEF) resPos = getDefResPos();
	switch(resPos)
	{
	case RES_LOCAL_FOLDER://这里是没有经过资源管理器的，所以会出现资源管理器没有管理的资源
		temp_back = IMG_Load(XString::ANSI2UTF8(pFileName).c_str());
		break;
	case RES_LOCAL_PACK:
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
	case RES_WEB:
		{
			XHTTPRequest tempRequest;
			XHTTPRequestStruct tempGet;
			char fileName[MAX_FILE_NAME_LENGTH];
			strcpy_s(fileName,MAX_FILE_NAME_LENGTH,pFileName);
			if(tempRequest.sendRequest(XFalse,fileName,tempGet) != 0)	//获取网络数据
			{//读取到网络数据
				temp_back = loadImageMem(tempGet.message,tempGet.messageLength);
				XMem::XDELETE_ARRAY(tempGet.headerReceive);
				XMem::XDELETE_ARRAY(tempGet.headerSend);
				XMem::XDELETE_ARRAY(tempGet.message);
			}
		}
		break; 
	case RES_AUTO:
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
			strcpy_s(fileName,MAX_FILE_NAME_LENGTH,pFileName);
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
    if(temp_back == NULL) LogNull("%s file load error!",pFileName);
    return temp_back;
}
bool XPixelSDL::load_(void *_tmp)	//这个是内部调用的接口
{
	SDL_Surface *tmp = (SDL_Surface *)_tmp;
	if(tmp == NULL) return false;
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
	default: return false;
	}
	w = tmp->w;
	h = tmp->h;
	if(!m_isInited)
	{
		m_dataSize = w * h * tmp->format->BytesPerPixel;
		m_pPixels = XMem::createArrayMem<unsigned char>(m_dataSize);
		if(m_pPixels == NULL) return false;
	}else
	{
		int size = w * h * tmp->format->BytesPerPixel;
		if(size != m_dataSize)
		{//如果大小发生变化则重新分配大小
			m_dataSize = size;
			XMem::XDELETE_ARRAY(m_pPixels);
			m_pPixels = XMem::createArrayMem<unsigned char>(m_dataSize);
			if(m_pPixels == NULL) return false;
		}
	}
	if (w * tmp->format->BytesPerPixel % 4 == 0)
	{//注意这里需要4字节对齐
		memcpy(m_pPixels,tmp->pixels,m_dataSize);
	}
	else
	{
		int wOffset = w * tmp->format->BytesPerPixel;
		unsigned char *sP = (unsigned char *)tmp->pixels;
		for (int i = 0,offsetD = 0,offsetS = 0; i < h; ++i)
		{
			memcpy(m_pPixels + offsetD, sP + offsetS, wOffset);
			offsetD += wOffset;
			offsetS += tmp->pitch;
		}
	}
	SDL_FreeSurface(tmp);

	m_isInited = true;
	return true;
}
bool XPixelSDL::load(const XBuffer &buff)
{
	return load_(loadImageMem(buff.getBuffer(),buff.getUsage()));
}
bool XPixelSDL::load(const std::string& filename,XResPos resPos)
{
	return load_(loadImageEx(filename.c_str(),resPos));
}
bool XPixelSDL::fitNPot()
{
	if(!m_isInited ||
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