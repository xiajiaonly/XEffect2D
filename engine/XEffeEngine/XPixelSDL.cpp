#include "XPixelSDL.h"
#include "XBasicFun.h"
#include "XResourcePack.h"
#include "XLogBook.h"
#include "XHTTPRequest.h"
#include "SDL_image.h"

SDL_Surface * loadImageEx(const char *pFileName,_XResourcePosition resoursePosition)
{
	SDL_Surface *temp_back = NULL;
	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = XEE::defaultResourcePosition;
	if(resoursePosition == RESOURCE_LOCAL_FOLDER)
	{//这里是没有经过资源管理器的，所以会出现资源管理器没有管理的资源
		char * tmp = ANSIToUTF8(pFileName);
		temp_back = IMG_Load(tmp);
		XDELETE_ARRAY(tmp);
	}else
	if(resoursePosition == RESOURCE_LOCAL_PACK)
	{//对于内部资源的读取
		//写入数据
		int length = _XResourcePack::GetInstance().getFileLength(pFileName);
		if(length < 0) return NULL;
		unsigned char *p = createArrayMem<unsigned char>(length + 1);
		if(p == NULL) return NULL;
		//printf("%d\n",length);
		if(_XResourcePack::GetInstance().unpackResource(pFileName,p) != 1)
		{
			//printf("LoadIMG Error!\n");
			LogStr("LoadIMG Error!");
			return NULL;
		}
		SDL_RWops *fileData = SDL_RWFromMem(p,length);
		if(fileData == NULL)
		{
			XDELETE_ARRAY(p);
			return NULL;
		}else
		{
			temp_back = IMG_Load_RW(fileData,1);
			//temp_back = IMG_Load(rw,1);
			XDELETE_ARRAY(p);
			//SDL_FreeRW(fileData);
		}
	}else
	if(resoursePosition == RESOURCE_WEB)
	{
		_XHTTPRequest tempRequest;
		_XHTTPRequestStruct tempGet;
		char fileName[MAX_FILE_NAME_LENGTH];
		strcpy(fileName,pFileName);
		if(tempRequest.sendRequest(XFalse,fileName,tempGet) != 0)	//获取网络数据
		{//读取到网络数据
			SDL_RWops *fileData = SDL_RWFromMem(tempGet.message,tempGet.messageLength);
			if(fileData == NULL)
			{
				XDELETE_ARRAY(tempGet.headerReceive);
				XDELETE_ARRAY(tempGet.headerSend);
				XDELETE_ARRAY(tempGet.message);
				return NULL;
			}else
			{
				temp_back = IMG_Load_RW(fileData,1);
				//temp_back = IMG_Load(rw,1);
				XDELETE_ARRAY(tempGet.headerReceive);
				XDELETE_ARRAY(tempGet.headerSend);
				XDELETE_ARRAY(tempGet.message);
				//SDL_FreeRW(fileData);
			}
		}else
		{
			return NULL;
		}
	}
    if(temp_back == NULL)
    {
		LogNull("%s file load error!\n",pFileName);
    	return NULL;
    }
    return temp_back;
}
bool _XPixelSDL::load(const std::string &filename,_XResourcePosition resPos)
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
	m_pPixels = createArrayMem<unsigned char>(w * h * tmp->format->BytesPerPixel);
	if(m_pPixels == NULL) return false;
	memcpy(m_pPixels,tmp->pixels,w * h * tmp->format->BytesPerPixel);
	SDL_FreeSurface(tmp);

	isInited = true;
	return true;
}
bool _XPixelSDL::fitNPot()
{
	if(!isInited) return false;
	if(!isNPOT(w,h)) return true;
	int ow = w;
	int oh = h;
	unsigned char *p = m_pPixels;
	//下面处理
	w = getMinWellSize2n(ow);
	h = getMinWellSize2n(oh);
	int pW = 0,poW = 0;
	int offsetH = (h - oh) >> 1;
	int offsetW = (w - ow) >> 1;
	switch(m_colorMode)
	{
	case COLOR_RGBA:
		pW = w << 2;
		poW = ow << 2;
		offsetW = offsetW << 2;
		break;
	case COLOR_RGB:
		pW = w * 3;
		poW = ow * 3;
		offsetW = offsetW * 3;
		break;
	case COLOR_GRAY:
		pW = w;
		poW = ow;
		break;
	}
	m_pPixels = createArrayMem<unsigned char>(pW * h);
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
	XDELETE_ARRAY(p);
	return true;
}