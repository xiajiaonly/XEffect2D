//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XTextureInformation.h"
#include "XResourcePack.h"
#include "XResourceOptimize.h"
//#include "png.h"
#include "XSavePNG.h"

#if TEX_INFO_DEBUG
int texInfoSum = 0;
#endif

_XAllTexture::_XAllTexture()
:m_isInited(0)
,m_isEnable(0)
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

int _XAllTexture::init(int resoursePosition)
{
	if(m_isInited != 0) return 1;

	//设置资源的位置
	if(resoursePosition != RESOURCE_OUTSIDE)
	{
		resoursePosition = RESOURCE_INSIDE;
	}
	m_resoursePosition = resoursePosition;
	
	if(m_resoursePosition == RESOURCE_INSIDE)
	{//如果资源已经封包
		int lengthTemp = _XResourcePack::GetInstance().getFileLength(PNG_INFORMATION_FILE_NAME);
		if(lengthTemp <= 0) return 0;
		unsigned char *p = NULL;
		try
		{
			p = new unsigned char[lengthTemp + 1];
			if(p == NULL) return 0;
		}catch(...)
		{
			return 0;
		}
		//从资源包中提取出资源文件的内容
		_XResourcePack::GetInstance().unpackResource(PNG_INFORMATION_FILE_NAME,p);
		*(p + lengthTemp) = '\0';
		//解析一下内容
		int offset = 0;
		sscanf((char *)(p + offset),"%d,",&m_textureSum);
		offset += getCharPosition((char *)(p + offset),',') + 1;
		sscanf((char *)(p + offset),"%d;",&m_targetTextureSum);
		offset += getCharPosition((char *)(p + offset),';') + 1;
		if(m_textureSum <= 0 || m_targetTextureSum <= 0)
		{
			XDELETE_ARRAY(p);
			m_isEnable = 0;
			return 0;
		}
		//分配内存空间
		try
		{
			m_pTexture = new _XTexture[m_targetTextureSum];
			if(m_pTexture == NULL)
			{
				XDELETE_ARRAY(p);
				m_isEnable = 0;
				return 0;
			}
		}catch(...)
		{
			XDELETE_ARRAY(p);
			m_isEnable = 0;
			return 0;
		}
		try
		{
			m_texInfo = new _XTextureInfo[m_textureSum];
			if(m_texInfo == NULL)
			{
				XDELETE_ARRAY(p);
				XDELETE_ARRAY(m_pTexture);
				m_isEnable = 0;
				return 0;
			}
		}catch(...)
		{
			XDELETE_ARRAY(p);
			XDELETE_ARRAY(m_pTexture);
			m_isEnable = 0;
			return 0;
		}
		//继续读取下面的资源，如果需要将资源载入
		char tempChar = ' ';
		int tempFlag = 0;
		char *targetTextureFlag = NULL;
		char targetFileName[MAX_FILE_NAME_LENGTH] = "";
		try
		{
			targetTextureFlag = new char[m_targetTextureSum];
			if(targetTextureFlag == NULL) 
			{
				XDELETE_ARRAY(p);
				XDELETE_ARRAY(m_pTexture);
				XDELETE_ARRAY(m_texInfo);
				m_isEnable = 0;
				return 0;
			}
		}catch(...)
		{
			XDELETE_ARRAY(p);
			XDELETE_ARRAY(m_pTexture);
			XDELETE_ARRAY(m_texInfo);
			m_isEnable = 0;
			return 0;
		}
		for(int i = 0;i < m_targetTextureSum;++ i)
		{//标记所有的资源尚未载入
			targetTextureFlag[i] = 0;
		}
		for(int i = 0;i < m_textureSum;++ i)
		{//这里有一个回车位
			sscanf((char *)(p + offset),"%c",&tempChar);
			offset ++;//读取文本的回车位
			sscanf((char *)(p + offset),"%c",&tempChar);
			offset ++;//读取文本的回车位

			tempFlag = 0;
			for(int j = 0;j < MAX_FILE_NAME_LENGTH;++ j)
			{
				sscanf((char *)(p + offset),"%c",&tempChar);
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
				m_isEnable = 0;
				XDELETE_ARRAY(p);
				XDELETE_ARRAY(m_pTexture);
				XDELETE_ARRAY(m_texInfo);
				XDELETE_ARRAY(targetTextureFlag);
				return 0;
			}
			tempFlag = 0;
			for(int j = 0;j < MAX_FILE_NAME_LENGTH;++ j)
			{
				sscanf((char *)(p + offset),"%c",&tempChar);
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
				m_isEnable = 0;
				XDELETE_ARRAY(p);
				XDELETE_ARRAY(m_pTexture);
				XDELETE_ARRAY(m_texInfo);
				XDELETE_ARRAY(targetTextureFlag);
				return 0;
			}
			sscanf((char *)(p + offset),"%d:",&tempFlag);
			offset += getCharPosition((char *)(p + offset),':') + 1;
			if(tempFlag < 0 || tempFlag >= m_targetTextureSum)
			{//文件数据发生错误，这里直接返回
				m_isEnable = 0;
				XDELETE_ARRAY(p);
				XDELETE_ARRAY(m_pTexture);
				XDELETE_ARRAY(m_texInfo);
				XDELETE_ARRAY(targetTextureFlag);
				return 0;
			}
			if(targetTextureFlag[tempFlag] == 0)
			{//如果发现当前资源尚未载入，则下面载入资源
				if(m_pTexture[tempFlag].load(targetFileName,m_resoursePosition) == 0)
				{
					m_isEnable = 0;
					XDELETE_ARRAY(p);
					XDELETE_ARRAY(m_pTexture);
					XDELETE_ARRAY(m_texInfo);
					XDELETE_ARRAY(targetTextureFlag);
					return 0;
				}
				targetTextureFlag[tempFlag] = 1;
			}
			m_texInfo[i].texture = m_pTexture[tempFlag];
			//读取剩下的信息
			sscanf((char *)(p + offset),"%f,",&m_texInfo[i].textureOffset.x);
			offset += getCharPosition((char *)(p + offset),',') + 1;
			sscanf((char *)(p + offset),"%f:",&m_texInfo[i].textureOffset.y);
			offset += getCharPosition((char *)(p + offset),':') + 1;

			sscanf((char *)(p + offset),"%f,",&m_texInfo[i].textureMove.x);
			offset += getCharPosition((char *)(p + offset),',') + 1;
			sscanf((char *)(p + offset),"%f,",&m_texInfo[i].textureMove.y);
			offset += getCharPosition((char *)(p + offset),',') + 1;
			sscanf((char *)(p + offset),"%f,",&m_texInfo[i].textureMove2.x);
			offset += getCharPosition((char *)(p + offset),',') + 1;
			sscanf((char *)(p + offset),"%f:",&m_texInfo[i].textureMove2.y);
			offset += getCharPosition((char *)(p + offset),':') + 1;
			sscanf((char *)(p + offset),"%f,",&m_texInfo[i].textureSize.x);
			offset += getCharPosition((char *)(p + offset),',') + 1;
			sscanf((char *)(p + offset),"%f;",&m_texInfo[i].textureSize.y);
			offset += getCharPosition((char *)(p + offset),';') + 1;
		}
		XDELETE_ARRAY(targetTextureFlag);
		//所有数据读取完成
		XDELETE_ARRAY(p);
	}else
	{
		FILE *fp;
		//打开文件
		if((fp = fopen(PNG_INFORMATION_FILE_NAME,"r")) == NULL)
		{
			m_isEnable = 0;
			return 0;
		}
		//读取文件内容
		fscanf(fp,"%d,",&m_textureSum);
		fscanf(fp,"%d;",&m_targetTextureSum);
		if(m_textureSum <= 0 || m_targetTextureSum <= 0)
		{
			fclose(fp);
			m_isEnable = 0;
			return 0;
		}
		//分配内存空间
		try
		{
			m_pTexture = new _XTexture[m_targetTextureSum];
			if(m_pTexture == NULL)
			{
				fclose(fp);
				m_isEnable = 0;
				return 0;
			}
		}catch(...)
		{
			fclose(fp);
			m_isEnable = 0;
			return 0;
		}

		try
		{
			m_texInfo = new _XTextureInfo[m_textureSum];
			if(m_texInfo == NULL)
			{
				fclose(fp);
				XDELETE_ARRAY(m_pTexture);
				m_isEnable = 0;
				return 0;
			}
		}catch(...)
		{
			fclose(fp);
			XDELETE_ARRAY(m_pTexture);
			m_isEnable = 0;
			return 0;
		}
		//继续读取下面的资源，如果需要将资源载入
		char tempChar;
		int tempFlag;
		char *targetTextureFlag = NULL;
		char targetFileName[MAX_FILE_NAME_LENGTH] = "";
		try
		{
			targetTextureFlag = new char[m_targetTextureSum];
			if(targetTextureFlag == NULL) 
			{
				fclose(fp);
				XDELETE_ARRAY(m_pTexture);
				XDELETE_ARRAY(m_texInfo);
				m_isEnable = 0;
				return 0;
			}
		}catch(...)
		{
			fclose(fp);
			XDELETE_ARRAY(m_pTexture);
			XDELETE_ARRAY(m_texInfo);
			m_isEnable = 0;
			return 0;
		}
		for(int i = 0;i < m_targetTextureSum;++ i)
		{
			targetTextureFlag[i] = 0;
		}
		for(int i = 0;i < m_textureSum;++ i)
		{
			//这里有一个回车位
			fscanf(fp,"%c",&tempChar);

			tempFlag = 0;
			for(int j = 0;j < MAX_FILE_NAME_LENGTH;++ j)
			{
				fscanf(fp,"%c",&tempChar);
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
				m_isEnable = 0;
				XDELETE_ARRAY(targetTextureFlag);
				XDELETE_ARRAY(m_pTexture);
				XDELETE_ARRAY(m_texInfo);
				return 0;
			}
			tempFlag = 0;
			for(int j = 0;j < MAX_FILE_NAME_LENGTH;++ j)
			{
				fscanf(fp,"%c",&tempChar);
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
				m_isEnable = 0;
				XDELETE_ARRAY(targetTextureFlag);
				XDELETE_ARRAY(m_pTexture);
				XDELETE_ARRAY(m_texInfo);
				return 0;
			}
			fscanf(fp,"%d:",&tempFlag);
			if(tempFlag < 0 || tempFlag >= m_targetTextureSum)
			{//文件数据发生错误，这里直接返回
				fclose(fp);
				m_isEnable = 0;
				XDELETE_ARRAY(targetTextureFlag);
				XDELETE_ARRAY(m_pTexture);
				XDELETE_ARRAY(m_texInfo);
				return 0;
			}
			if(tempFlag >= m_targetTextureSum) return 0;
			if(targetTextureFlag[tempFlag] == 0)
			{
				if(m_pTexture[tempFlag].load(targetFileName,m_resoursePosition) == 0)
				{
					fclose(fp);
					m_isEnable = 0;
					XDELETE_ARRAY(targetTextureFlag);
					XDELETE_ARRAY(m_pTexture);
					XDELETE_ARRAY(m_texInfo);
					return 0;
				}
				targetTextureFlag[tempFlag] = 1;
			}
			m_texInfo[i].texture = m_pTexture[tempFlag];
			//读取剩下的信息
			fscanf(fp,"%f,",&m_texInfo[i].textureOffset.x);
			fscanf(fp,"%f:",&m_texInfo[i].textureOffset.y);
			fscanf(fp,"%f,",&m_texInfo[i].textureMove.x);
			fscanf(fp,"%f,",&m_texInfo[i].textureMove.y);
			fscanf(fp,"%f,",&m_texInfo[i].textureMove2.x);
			fscanf(fp,"%f:",&m_texInfo[i].textureMove2.y);
			fscanf(fp,"%f,",&m_texInfo[i].textureSize.x);
			fscanf(fp,"%f;",&m_texInfo[i].textureSize.y);
		}
		XDELETE_ARRAY(targetTextureFlag);
		//所有数据读取完成
		fclose(fp);
	}
	m_isEnable = 1;

	m_isInited = 1;
	return 1;
}

int _XAllTexture::getTexture(const char *temp,_XTextureInfo &texInfo)
{
	if(m_isInited == 0) return 0;
	if(m_isEnable == 0) return 0;
	if(temp == NULL) return 0;
	for(int i = 0;i < m_textureSum;++ i)
	{
		if(fileNameCompare(temp, m_texInfo[i].textureName) == 1)
		{
			texInfo = m_texInfo[i];
			//texInfo.pTexture = m_texInfo[i].pTexture;
			//texInfo.textureMove = m_texInfo[i].textureMove;
			//texInfo.textureMove2 = m_texInfo[i].textureMove2;
			//texInfo.textureOffset = m_texInfo[i].textureOffset;
			//texInfo.textureSize = m_texInfo[i].textureSize;
			//strcpy(texInfo.targetFileName,m_texInfo[i].targetFileName);
			//strcpy(texInfo.textureName,m_texInfo[i].textureName);
			return 1;
		}
	}
	return 0;
}

void _XAllTexture::release()
{
	if(m_isInited == 0) return;

	XDELETE_ARRAY(m_pTexture);
	XDELETE_ARRAY(m_texInfo);

	m_isInited = 0;
	m_isEnable = 0;
}

int _XTextureData::load(const char * filename,int resoursePosition)
{
	if(m_isInited != 0) return 1;

	_XTextureInfo texInfo;
	if(resoursePosition != 0 && _XAllTexture::GetInstance().getTexture(filename,texInfo) != 0)
	{
		texture = texInfo.texture;
		textureMove = texInfo.textureMove;
		textureMove2 = texInfo.textureMove2;
		clipInsideRect.set(texInfo.textureOffset.x,texInfo.textureOffset.y,
			texInfo.textureOffset.x + texInfo.textureSize.x,texInfo.textureOffset.y + texInfo.textureSize.y);
		textureSize.set(clipInsideRect.getWidth() + textureMove.x + textureMove2.x,clipInsideRect.getHeight() + textureMove.y + textureMove2.y);
		isEnableInsideClip = 1;
	}else
	{//下面这一部分资源没有进行管理，将会导致资源重复载入，也就是同一个资源多次载入。（这里需要修正）
		if(texture.load(filename,resoursePosition) == 0) 
		{
			return 0;
		}
		textureSize.set(texture.m_w,texture.m_h);
		isEnableInsideClip = 0;
	}

	m_isInited = 1;
	return 1;
}

int _XTextureData::loadEmpty()
{
	if(m_isInited != 0) return 1;

	isEnableInsideClip = 0;

	m_isInited = 1;
	return 1;
}

int _XTextureData::setACopy(const _XTextureData& temp)
{
	if(& temp == this) return 0;	//防止自身赋值
	m_isInited = temp.m_isInited;

	texture = temp.texture;		//指向贴图的指针
	textureMove = temp.textureMove;		//资源文件在裁剪时的移动位置：左上角
	textureMove2 = temp.textureMove2;		//资源文件在裁剪时的移动位置：右下角
	isEnableInsideClip = temp.isEnableInsideClip;			//贴图模式：1：内部裁剪模式 0：没有经过内部裁剪
	clipInsideRect = temp.clipInsideRect;		//内部剪切的数据
	textureSize = temp.textureSize;		//贴图的尺寸优化之前的贴图尺寸

	return 1;
}

_XTextureData& _XTextureData::operator = (const _XTextureData& temp)
{
	if(& temp == this) return *this;	//防止自身赋值
	m_isInited = temp.m_isInited;

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
	if(m_isInited == 0) return;
	m_isInited = 0;
}

//将资源还原
int reductionTexture(const unsigned char *p,int length,const _XTextureInfo &texInfo)
{
	if(p == NULL) return 0;
	if(length <= 0) return 0;
//	SDL_Surface *temp = NULL;
	SDL_Surface *temp_back = NULL;
	SDL_RWops *fileData = SDL_RWFromMem((void *)p,length);
	if(fileData == 0) return 0;
	temp_back = IMG_Load_RW(fileData,1);
	if(temp_back == 0) return 0;
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
	return 1;
}