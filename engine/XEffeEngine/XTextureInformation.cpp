//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XTextureInformation.h"
#include "XResourcePack.h"
#include "XResourceOptimize.h"
#include "XBasicWindow.h"
//#include "png.h"
#include "XSavePNG.h"

#if TEX_INFO_DEBUG
int texInfoSum = 0;
#endif

_XAllTexture::_XAllTexture()
:m_isInited(XFalse)
,m_isEnable(XFalse)
,m_textureSum(0)
,m_targetTextureSum(0)
,m_texInfo(NULL)
{
}
_XAllTexture::~_XAllTexture()
{
	release();
}
_XAllTexture& _XAllTexture::GetInstance()
{
	static _XAllTexture m_instance;
	return m_instance;
}
_XBool _XAllTexture::init(_XResourcePosition resoursePosition)	//这里读取经过压缩的资源文件
{
	if(m_isInited) return XTrue;
	//设置资源的位置
	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = XEE::defaultResourcePosition;
	m_resoursePosition = resoursePosition;
	
	if(m_resoursePosition == RESOURCE_LOCAL_PACK)
	{//如果资源已经封包
		//从资源包中提取出资源文件的内容
		char tempFilename[MAX_FILE_NAME_LENGTH] = "";
		sprintf(tempFilename,"%s%s",BASE_RESOURCE_PATH,PNG_INFORMATION_FILE_NAME);
		unsigned char *p = _XResourcePack::GetInstance().getFileData(tempFilename);
		if(p == NULL) 
		{//没有优化的资源文件
			m_textureSum = 0;
			m_targetTextureSum = 0;
		}else
		{
			//解析一下内容
			int offset = 0;
			if(sscanf((char *)(p + offset),"%d,",&m_textureSum) != 1) {XDELETE_ARRAY(p);return XFalse;}
			offset += getCharPosition((char *)(p + offset),',') + 1;
			if(sscanf((char *)(p + offset),"%d;",&m_targetTextureSum) != 1) {XDELETE_ARRAY(p);return XFalse;}
			offset += getCharPosition((char *)(p + offset),';') + 1;
			if(m_textureSum <= 0 || m_targetTextureSum <= 0)
			{
				XDELETE_ARRAY(p);
				m_isEnable = XFalse;
				return XFalse;
			}
			//分配内存空间
			m_pTexture = createArrayMem<_XTexture>(m_targetTextureSum);
			if(m_pTexture == NULL)
			{
				XDELETE_ARRAY(p);
				m_isEnable = XFalse;
				return XFalse;
			}
			m_texInfo = createArrayMem<_XTextureInfo>(m_textureSum);
			if(m_texInfo == NULL)
			{
				XDELETE_ARRAY(p);
				XDELETE_ARRAY(m_pTexture);
				m_isEnable = XFalse;
				return XFalse;
			}
			//继续读取下面的资源，如果需要将资源载入
			char tempChar = ' ';
			int tempFlag = 0;
			char *targetTextureFlag = NULL;
			char targetFileName[MAX_FILE_NAME_LENGTH] = "";
			targetTextureFlag = createArrayMem<char>(m_targetTextureSum);
			if(targetTextureFlag == NULL) 
			{
				XDELETE_ARRAY(p);
				XDELETE_ARRAY(m_pTexture);
				XDELETE_ARRAY(m_texInfo);
				m_isEnable = XFalse;
				return XFalse;
			}
			for(int i = 0;i < m_targetTextureSum;++ i)
			{//标记所有的资源尚未载入
				targetTextureFlag[i] = 0;
			}
			for(int i = 0;i < m_textureSum;++ i)
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
						tempFilename[j] = '\0';
						sprintf(m_texInfo[i].textureName,"%s%s",BASE_RESOURCE_PATH,tempFilename);	//为了封装前后的路径统一这里修改相对路径
						//m_texInfo[i].textureName[j] = '\0';
						tempFlag = 1;
						break;
					}else
					{
						//m_texInfo[i].textureName[j] = tempChar;
						tempFilename[j] = tempChar;
					}
				}
				if(tempFlag == 0)
				{//文件数据发生错误，这里直接返回
					m_isEnable = XFalse;
					XDELETE_ARRAY(p);
					XDELETE_ARRAY(m_pTexture);
					XDELETE_ARRAY(m_texInfo);
					XDELETE_ARRAY(targetTextureFlag);
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
					m_isEnable = XFalse;
					XDELETE_ARRAY(p);
					XDELETE_ARRAY(m_pTexture);
					XDELETE_ARRAY(m_texInfo);
					XDELETE_ARRAY(targetTextureFlag);
					return XFalse;
				}
				if(sscanf((char *)(p + offset),"%d:",&tempFlag) != 1) {XDELETE_ARRAY(p);return XFalse;}
				offset += getCharPosition((char *)(p + offset),':') + 1;
				if(tempFlag < 0 || tempFlag >= m_targetTextureSum)
				{//文件数据发生错误，这里直接返回
					m_isEnable = XFalse;
					XDELETE_ARRAY(p);
					XDELETE_ARRAY(m_pTexture);
					XDELETE_ARRAY(m_texInfo);
					XDELETE_ARRAY(targetTextureFlag);
					return XFalse;
				}
				if(targetTextureFlag[tempFlag] == 0)
				{//如果发现当前资源尚未载入，则下面载入资源
					sprintf(tempFilename,"%s%s",BASE_RESOURCE_PATH,targetFileName);
					if(!m_pTexture[tempFlag].load(tempFilename,m_resoursePosition))
					{
						m_isEnable = XFalse;
						XDELETE_ARRAY(p);
						XDELETE_ARRAY(m_pTexture);
						XDELETE_ARRAY(m_texInfo);
						XDELETE_ARRAY(targetTextureFlag);
						return XFalse;
					}
					targetTextureFlag[tempFlag] = 1;
				}
				m_texInfo[i].texture = m_pTexture[tempFlag];
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
			XDELETE_ARRAY(targetTextureFlag);
			//所有数据读取完成
			XDELETE_ARRAY(p);
		}
	}else
	if(m_resoursePosition == RESOURCE_LOCAL_FOLDER)
	{
		FILE *fp;
		//打开文件
		if((fp = fopen(PNG_INFORMATION_FILE_NAME,"r")) == NULL)
		{
			m_isEnable = XFalse;
			return XFalse;
		}
		//读取文件内容
		if(fscanf(fp,"%d,",&m_textureSum) != 1) {fclose(fp);return XFalse;}
		if(fscanf(fp,"%d;",&m_targetTextureSum) != 1) {fclose(fp);return XFalse;}
		if(m_textureSum <= 0 || m_targetTextureSum <= 0)
		{
			fclose(fp);
			m_isEnable = XFalse;
			return XFalse;
		}
		//分配内存空间
		m_pTexture = createArrayMem<_XTexture>(m_targetTextureSum);
		if(m_pTexture == NULL)
		{
			fclose(fp);
			m_isEnable = XFalse;
			return XFalse;
		}
		m_texInfo = createArrayMem<_XTextureInfo>(m_textureSum);
		if(m_texInfo == NULL)
		{
			fclose(fp);
			XDELETE_ARRAY(m_pTexture);
			m_isEnable = XFalse;
			return XFalse;
		}
		//继续读取下面的资源，如果需要将资源载入
		char tempChar;
		int tempFlag;
		char *targetTextureFlag = NULL;
		char targetFileName[MAX_FILE_NAME_LENGTH] = "";
		targetTextureFlag = createArrayMem<char>(m_targetTextureSum);
		if(targetTextureFlag == NULL) 
		{
			fclose(fp);
			XDELETE_ARRAY(m_pTexture);
			XDELETE_ARRAY(m_texInfo);
			m_isEnable = XFalse;
			return XFalse;
		}
		for(int i = 0;i < m_targetTextureSum;++ i)
		{
			targetTextureFlag[i] = 0;
		}
		for(int i = 0;i < m_textureSum;++ i)
		{
			//这里有一个回车位
			if(fscanf(fp,"%c",&tempChar) != 1) {fclose(fp);return XFalse;}

			tempFlag = 0;
			for(int j = 0;j < MAX_FILE_NAME_LENGTH;++ j)
			{
				if(fscanf(fp,"%c",&tempChar) != 1) {fclose(fp);return XFalse;}
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
				fclose(fp);
				m_isEnable = XFalse;
				XDELETE_ARRAY(targetTextureFlag);
				XDELETE_ARRAY(m_pTexture);
				XDELETE_ARRAY(m_texInfo);
				return XFalse;
			}
			tempFlag = 0;
			for(int j = 0;j < MAX_FILE_NAME_LENGTH;++ j)
			{
				if(fscanf(fp,"%c",&tempChar) != 1) {fclose(fp);return XFalse;}
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
				fclose(fp);
				m_isEnable = XFalse;
				XDELETE_ARRAY(targetTextureFlag);
				XDELETE_ARRAY(m_pTexture);
				XDELETE_ARRAY(m_texInfo);
				return XFalse;
			}
			if(fscanf(fp,"%d:",&tempFlag) != 1) {fclose(fp);return XFalse;}
			if(tempFlag < 0 || tempFlag >= m_targetTextureSum)
			{//文件数据发生错误，这里直接返回
				fclose(fp);
				m_isEnable = XFalse;
				XDELETE_ARRAY(targetTextureFlag);
				XDELETE_ARRAY(m_pTexture);
				XDELETE_ARRAY(m_texInfo);
				return XFalse;
			}
			if(tempFlag >= m_targetTextureSum) return XFalse;
			if(targetTextureFlag[tempFlag] == 0)
			{
				if(!m_pTexture[tempFlag].load(targetFileName,m_resoursePosition))
				{
					fclose(fp);
					m_isEnable = XFalse;
					XDELETE_ARRAY(targetTextureFlag);
					XDELETE_ARRAY(m_pTexture);
					XDELETE_ARRAY(m_texInfo);
					return XFalse;
				}
				targetTextureFlag[tempFlag] = 1;
			}
			m_texInfo[i].texture = m_pTexture[tempFlag];
			//读取剩下的信息
			if(fscanf(fp,"%f,",&m_texInfo[i].textureOffset.x) != 1) {fclose(fp);return XFalse;}
			if(fscanf(fp,"%f:",&m_texInfo[i].textureOffset.y) != 1) {fclose(fp);return XFalse;}
			if(fscanf(fp,"%f,",&m_texInfo[i].textureMove.x) != 1) {fclose(fp);return XFalse;}
			if(fscanf(fp,"%f,",&m_texInfo[i].textureMove.y) != 1) {fclose(fp);return XFalse;}
			if(fscanf(fp,"%f,",&m_texInfo[i].textureMove2.x) != 1) {fclose(fp);return XFalse;}
			if(fscanf(fp,"%f:",&m_texInfo[i].textureMove2.y) != 1) {fclose(fp);return XFalse;}
			if(fscanf(fp,"%f,",&m_texInfo[i].textureSize.x) != 1) {fclose(fp);return XFalse;}
			if(fscanf(fp,"%f;",&m_texInfo[i].textureSize.y) != 1) {fclose(fp);return XFalse;}
		}
		XDELETE_ARRAY(targetTextureFlag);
		//所有数据读取完成
		fclose(fp);
	}
	m_isEnable = XTrue;

	m_isInited = XTrue;
	return XTrue;
}
_XBool _XAllTexture::getTexture(const char *temp,_XTextureInfo &texInfo)
{
	if(!m_isInited ||
		!m_isEnable ||
		temp == NULL) return XFalse;
	for(int i = 0;i < m_textureSum;++ i)
	{
		if(fileNameCompare(temp, m_texInfo[i].textureName))
		{
			texInfo = m_texInfo[i];
			//texInfo.pTexture = m_texInfo[i].pTexture;
			//texInfo.textureMove = m_texInfo[i].textureMove;
			//texInfo.textureMove2 = m_texInfo[i].textureMove2;
			//texInfo.textureOffset = m_texInfo[i].textureOffset;
			//texInfo.textureSize = m_texInfo[i].textureSize;
			//strcpy(texInfo.targetFileName,m_texInfo[i].targetFileName);
			//strcpy(texInfo.textureName,m_texInfo[i].textureName);
			return XTrue;
		}
	}
	return XFalse;
}
void _XAllTexture::release()
{
	if(!m_isInited) return;

	XDELETE_ARRAY(m_pTexture);
	XDELETE_ARRAY(m_texInfo);

	m_isInited = XFalse;
	m_isEnable = XFalse;
}
_XBool _XTextureData::load(const char * filename,_XResourcePosition resoursePosition)
{
	if(m_isInited) return XTrue;

	_XTextureInfo texInfo;
	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = XEE::defaultResourcePosition;
	if(resoursePosition  == RESOURCE_LOCAL_PACK && _XAllTexture::GetInstance().getTexture(filename,texInfo))
	{//本地压缩文件
		texture = texInfo.texture;
		textureMove = texInfo.textureMove;
		textureMove2 = texInfo.textureMove2;
		clipInsideRect.set(texInfo.textureOffset.x,texInfo.textureOffset.y,
			texInfo.textureOffset.x + texInfo.textureSize.x,texInfo.textureOffset.y + texInfo.textureSize.y);
		textureSize.set(clipInsideRect.getWidth() + textureMove.x + textureMove2.x,clipInsideRect.getHeight() + textureMove.y + textureMove2.y);
		isEnableInsideClip = 1;

		m_isInited = XTrue;
		return XTrue;
	}else
	{//下面这一部分资源没有进行管理，将会导致资源重复载入，也就是同一个资源多次载入。（这里需要修正）
		if(!texture.load(filename,resoursePosition)) return XFalse;
		//由于程序内部会对数据进行2的n次方扩展所以这里需要衔接数据。
		if(!isNPOT(texture.m_w,texture.m_h))
		{//符合2的n次方没有进行扩展
			textureSize.set(texture.m_w,texture.m_h);
			isEnableInsideClip = 0;
		}else
		{//由于进行了可扩展所以需要进行裁剪
			int w = getMinWellSize2n(texture.m_w);
			int h = getMinWellSize2n(texture.m_h);
			textureMove.set(0.0f,0.0f);
			textureMove2.set(0.0f,0.0f);
			clipInsideRect.left = (w - texture.m_w) * 0.5f;
			clipInsideRect.top = (h - texture.m_h) * 0.5f;
			clipInsideRect.right = clipInsideRect.left + texture.m_w;
			clipInsideRect.bottom = clipInsideRect.top + texture.m_h;
			textureSize.set(texture.m_w,texture.m_h);
			texture.m_w = w;
			texture.m_h = h;

			isEnableInsideClip = 1;
		}

		m_isInited = XTrue;
		return XTrue;
	}
//	return XFalse;
}
_XBool _XTextureData::loadEmpty()
{
	if(m_isInited) return XTrue;

	isEnableInsideClip = 0;

	m_isInited = XTrue;
	return XTrue;
}
_XBool _XTextureData::createWithTexture(int w,int h,_XColorMode colorMode,unsigned int tex)
{
	if(m_isInited) return XTrue;
	isEnableInsideClip = 0;
	texture.createWithTexture(w,h,colorMode,tex);
	textureSize.set(w,h);

	m_isInited = XTrue;
	return XTrue;
}
_XBool _XTextureData::createTexture(int w,int h,_XColorMode colorMode)
{
	if(m_isInited) return XTrue;
	isEnableInsideClip = 0;
	texture.createTexture(w,h,colorMode);
	textureSize.set(w,h);

	m_isInited = XTrue;
	return XTrue;
}
_XBool _XTextureData::setACopy(const _XTextureData& temp)
{
	if(& temp == this) return XFalse;	//防止自身赋值
	m_isInited = temp.m_isInited;

	m_ID = temp.m_ID;
	texture = temp.texture;		//指向贴图的指针
	textureMove = temp.textureMove;		//资源文件在裁剪时的移动位置：左上角
	textureMove2 = temp.textureMove2;		//资源文件在裁剪时的移动位置：右下角
	isEnableInsideClip = temp.isEnableInsideClip;			//贴图模式：1：内部裁剪模式 0：没有经过内部裁剪
	clipInsideRect = temp.clipInsideRect;		//内部剪切的数据
	textureSize = temp.textureSize;		//贴图的尺寸优化之前的贴图尺寸

	return XTrue;
}
_XTextureData& _XTextureData::operator = (const _XTextureData& temp)
{
	if(& temp == this) return *this;	//防止自身赋值
	m_isInited = temp.m_isInited;

	m_ID = temp.m_ID;
	texture = temp.texture;		//指向贴图的指针
	textureMove = temp.textureMove;		//资源文件在裁剪时的移动位置：左上角
	textureMove2 = temp.textureMove2;		//资源文件在裁剪时的移动位置：右下角
	isEnableInsideClip = temp.isEnableInsideClip;			//贴图模式：1：内部裁剪模式 0：没有经过内部裁剪
	clipInsideRect = temp.clipInsideRect;		//内部剪切的数据
	textureSize = temp.textureSize;		//贴图的尺寸优化之前的贴图尺寸

	return * this;
}
void _XTextureData::release()
{
	if(!m_isInited) return;
	m_isInited = XFalse;
}
//将资源还原
_XBool reductionTexture(const unsigned char *p,int length,const _XTextureInfo &texInfo)
{
	if(p == NULL) return XFalse;
	if(length <= 0) return XFalse;
//	SDL_Surface *temp = NULL;
	SDL_Surface *temp_back = NULL;
	SDL_RWops *fileData = SDL_RWFromMem((void *)p,length);
	if(fileData == 0) return XFalse;
	temp_back = IMG_Load_RW(fileData,1);
	if(temp_back == 0) return XFalse;
//	SDL_SetAlpha(temp, SDL_RLEACCEL | SDL_SRCALPHA, 255);
//	temp_back = SDL_DisplayFormatAlpha(temp);
	//根据需要重新建立贴图
	int texW = texInfo.textureSize.x + texInfo.textureMove.x + texInfo.textureMove2.x;
	int texH = texInfo.textureSize.y + texInfo.textureMove.y + texInfo.textureMove2.y;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	SDL_Surface * picArm = SDL_CreateRGBSurface(SDL_SWSURFACE,texW,texH,32,0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
#else
	SDL_Surface * picArm = SDL_CreateRGBSurface(SDL_SWSURFACE,texW,texH,32,0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
#endif
	//初始化贴图数据
	unsigned char *pData = (unsigned char *)picArm->pixels;
	unsigned char *pData1 = (unsigned char *)temp_back->pixels;
	memset(pData,0,(texW * texH) << 2);
	//将需要的数据存储到贴图当中
	for(int i = 0;i < texInfo.textureSize.y;++ i)
	{
		memcpy(pData + (int)((texInfo.textureMove.y + i) * texW * 4.0f) + (int)(texInfo.textureMove.x * 4.0f),
			pData1 + (int)((texInfo.textureOffset.y + i) * temp_back->w * 4.0f) + (int)(texInfo.textureOffset.x * 4.0f),
			texInfo.textureSize.x * 4.0f);
	}
	//保存需要的贴图数据
	savePNG(texInfo.textureName,picArm,Z_BEST_COMPRESSION);
	//释放资源
	SDL_FreeSurface(picArm);
	picArm = NULL;
//	SDL_FreeSurface(temp);
//	temp = NULL;
	SDL_FreeSurface(temp_back);
	temp_back = NULL;
	//SDL_FreeRW(fileData);
	return XTrue;
}