#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "SDL_image.h"
#include "XMath/X2DPacker.h"
#include "XSavePNG.h"
#include "XResourceOptimize.h"
#include "XFile.h"
namespace XE{
//非常遗憾的是图片资源移动加容差比较的方式没有完成，由于移动比较完成之后需要对图像进行裁剪，裁剪会造成逻辑的错误，
//也就是后比较的图片修改裁剪数据之后对之前比较的数据造成错误。

//这里使用新的方法代替旧的方法，理论上效果一样，而且速度快很多
//旧的方法：给定颜色容差，给定移动比较的范围。然后在给点范围内移动图片，比较是否误差在允许的范围内。由于图片比较本
//来就需要小号比较大的计算量，再加上在一定范围内移动，多次比较，如果范围为5则需要比较5*5=25次。而且比较完成之后需要
//更具移动比较的具体数值，来修改本图和目标图的裁剪范围。这将会造成后面比较图片的修改影响到之前修改的图片的裁剪数据，
//造成严重的bug
//新的方法：在裁剪图片的时候就是用颜色容差对多出来的容差以内的颜色就裁减掉。然后在比较的时候就不需要在移动范围比较了，
//而是直接比较裁剪后的有小部分是否符合颜色容差的限制。
//两种方法效果相同，效率后面明显要高，而且不会造成bug。
struct XPicProcData
{
	XVec2 leftTopPoint;			//图片裁剪的左上角裁剪距离
	XVec2 rightBottomPoint;		//图片裁剪的右下角裁剪距离
	XVec2 picTureSize;			//图片的原始尺寸
	XVec2 picOutSize;			//图片裁剪之后的尺寸
	XVec2 picOffsetPosition;		//图片放置的偏移坐标 在新图片中的位置
	int textureOrder;				//图片的总编号
	int sameTexOrder;				//相同图片的编号
	SDL_Surface *pPic;				//图像的指针
	//char fileName[MAX_FILE_NAME_LENGTH];			//图片的名称
	//char outFileName[MAX_FILE_NAME_LENGTH];			//所在贴图的名称
	std::string fileName;
	std::string outFileName;
	
	XPicProcData()
		:leftTopPoint(0.0f)
		,rightBottomPoint(0.0f)
		,picTureSize(0.0f)
		,picOutSize(0.0f)
		,pPic(NULL)
		,picOffsetPosition(0.0f)
		,textureOrder(0)
		,sameTexOrder(-1)
	{}
	~XPicProcData()	{/*releaseImage();*/}	//不在这里释放资源，否则会造成内存问题
	bool loadImage()
	{
		if(pPic != NULL) return true;	//已经载入，则不重新载入
		char *tmp = XString::ANSIToUTF8(fileName.c_str());
		pPic = IMG_Load(tmp);
		XMem::XDELETE_ARRAY(tmp);
		if(pPic == NULL) return false;
		return true;
	}
	void releaseImage()
	{
		if(pPic == NULL) return;
		SDL_FreeSurface(pPic);
		pPic = NULL;
	}
};

#define TEXTURE_FILE_NAME "Texture_000.png"
#define MAX_BASICBOXSUM (1024)	//最多使用到1024张标准贴图的
#define MAX_BOXSIZE (1024)		//标准贴图的尺寸为1024
//从像素数据中找到图像非透明部分的上边界
int getPixelsTop(const unsigned char *pixels,
				 int width,int height,int maxPixelsOffset)	//透明度大于maxPixelsOffset这个阈值时认为是不可切割的部分
{
	if(pixels == NULL) return 0;
	int size = width * height * 4;
	for(int i = 3;i < size;i += 4)
	{
		if(pixels[i] > maxPixelsOffset)
			return i / (width << 2);
	}
	return 0;
}
//从像素数据中找到图像非透明部分的下边界
int getPixelsBottom(const unsigned char *pixels,
				 int width,int height,int maxPixelsOffset)
{
	if(pixels == NULL) return 0;
	int size = width * height * 4;
	for(int i = size - 1;i >= 0;i -= 4)
	{
		if(pixels[i] > maxPixelsOffset)
			return i / (width << 2);
	}
	return 0;
}
//从像素数据中找到图像非透明部分的左边界
int getPixelsLeft(const unsigned char *pixels,
				 int width,int height,int maxPixelsOffset)
{
	if(pixels == NULL) return 0;
	bool flag = false;
	int left = 0;
	const unsigned char *tempC;
	int tmpWidth = width << 2;	//一排像素的数量
	for(int w = 0;w < width;++ w)	//这里可以尝试进行内存的横纵优化
	{
		flag = false;
		tempC = pixels + (w << 2) + 3;
		for(int h = 0;h < height;++ h,tempC += tmpWidth)
		{
			if(* tempC > maxPixelsOffset)
			{
				flag = true;
				break;
			}
		}
		if(flag) break;
		else left = w + 1;
	}
	return left;
}
//从像素数据中找到图像非透明部分的右边界
int getPixelsRight(const unsigned char *pixels,
				 int width,int height,int maxPixelsOffset)
{
	if(pixels == NULL) return 0;
	bool flag = false;
	int right = width;
	const unsigned char * tempC;
	int tmpWidth = width << 2;	//一排像素的数量
	for(int w = width - 1;w >= 0;-- w)	//这里可以尝试进行内存的横纵优化	
	{
		flag = false;
		tempC = pixels + (w << 2) + 3;
		for(int h = 0;h < height;++ h,tempC += tmpWidth)
		{
			if(*tempC > maxPixelsOffset)
			{
				flag = true;
				break;
			}
		}
		if(flag) break;
		else right = w;
	}
	return right;
}
//确定图片有效部分的大小
bool XResourceOptimize::getPictureRect(XPicProcData &result,int maxPixelsOffset)
{
	//读取图片
	if(!result.loadImage()) return false;
	//寻找矩形
	int top = 0,left = 0,bottom = result.pPic->h - 1,right = result.pPic->w - 1;
	if(result.pPic->format->BytesPerPixel == 4)
	{
		top = getPixelsTop((unsigned char *)result.pPic->pixels,result.pPic->w,result.pPic->h,maxPixelsOffset);
		left = getPixelsLeft((unsigned char *)result.pPic->pixels,result.pPic->w,result.pPic->h,maxPixelsOffset);
		bottom = getPixelsBottom((unsigned char *)result.pPic->pixels,result.pPic->w,result.pPic->h,maxPixelsOffset);
		right = getPixelsRight((unsigned char *)result.pPic->pixels,result.pPic->w,result.pPic->h,maxPixelsOffset);
	}

//	if(top >= bottom) return 0;
//	if(left >= right) return 0;
	//这里会出现问题就是当为空图片的时候，也就是整个图片都是透明的时候，这里会出现问题
	//下面的代码就是为了处理这种情况而写的
	if(top >= bottom)
	{
		top = 0;
		bottom = result.pPic->h;
	}
	if(left >= right)
	{
		left = 0;
		right = result.pPic->w;
	}

	result.leftTopPoint.set(left,top);
	result.picTureSize.set(result.pPic->w,result.pPic->h);
	result.picOutSize.set(right - left,bottom - top);
	result.rightBottomPoint.set(result.pPic->w - right,result.pPic->h - bottom);
	
	if(!m_isOptimizeInMem)
		result.releaseImage();
	return true;
}
int XResourceOptimize::imagePack(std::vector<XPicProcData> &picData,bool withOpti)
{
	int allBasicBoxSum = 0;	//装箱时使用到的箱子的总数
	//寻找最大的包围合
	int maxW = MAX_BOXSIZE;
	int maxH = MAX_BOXSIZE;
	for(auto it = picData.begin();it != picData.end();++ it)
	{
		if(XMath::getMinWellSize2n(it->picOutSize.x + 1) > maxW)
		{
			maxW = XMath::getMinWellSize2n(it->picOutSize.x + 1);
		}
		if(XMath::getMinWellSize2n(it->picOutSize.y + 1) > maxH)
		{
			maxH = XMath::getMinWellSize2n(it->picOutSize.y + 1);
		}
	}
	//拼合图片
	X2DPacker tempPacker;
	//设置拼合的参数
	//tempPacker.setOptimizeLevel(true);
	//tempPacker.setCanRotate(false);	//不能旋转
	if(!withOpti)
	{
		//最后一个盒子没有优化
		tempPacker.m_basicBox = XMem::createArrayMem<XBasicBox>(MAX_BASICBOXSUM);	//定义100个盒子
		for(int i = 0;i < MAX_BASICBOXSUM;i ++)
		{
			tempPacker.m_basicBox[i].m_beUsed = 0;
			tempPacker.m_basicBox[i].m_objectBoxesOrder = XMem::createArrayMem<int>(picData.size());
			tempPacker.m_basicBox[i].m_order = i;
			tempPacker.m_basicBox[i].m_size.set(maxW,maxH);
			tempPacker.m_basicBox[i].m_releaseAcreage = tempPacker.m_basicBox[i].m_size.x * tempPacker.m_basicBox[i].m_size.y;

			tempPacker.m_basicBox[i].m_mayPosition = XMem::createArrayMem<XVec2>(4 * picData.size());
			tempPacker.m_basicBox[i].m_mayPositionSum = 1;
			tempPacker.m_basicBox[i].m_mayPosition[0].set(0.0f);

			tempPacker.m_basicBox[i].m_mayPositionX = XMem::createArrayMem<int>(2 * picData.size());
			tempPacker.m_basicBox[i].m_mayPositionXSum = 1;
			tempPacker.m_basicBox[i].m_mayPositionX[0] = 0;
			tempPacker.m_basicBox[i].m_mayPositionY = XMem::createArrayMem<int>(2 * picData.size());
			tempPacker.m_basicBox[i].m_mayPositionYSum = 1;
			tempPacker.m_basicBox[i].m_mayPositionY[0] = 0;
		}
		tempPacker.m_basicBoxSum = MAX_BASICBOXSUM;

		tempPacker.m_objectBox = XMem::createArrayMem<XObjectBox>(picData.size());	//定义了MAX_OBJECTSUM个物件
		int truePicSum = 0;	//由于相同的资源会被优化掉，所以这里需要重新统计实际使用到的资源数量
		for(auto it = picData.begin();it != picData.end();++ it)
		{
			if (it->sameTexOrder >= 0) continue;
			tempPacker.m_objectBox[truePicSum].m_order = truePicSum;
			tempPacker.m_objectBox[truePicSum].m_setOrder = it - picData.begin();
			tempPacker.m_objectBox[truePicSum].m_position.reset();
			tempPacker.m_objectBox[truePicSum].m_basicBoxOrder = -1;
			//这里为了防止图像裁剪缩放造成的杂色，需要每个图片之间间隔一个像素
		//	tempPacker.m_objectBox[truePicSum].m_size = it->picOutSize;
			tempPacker.m_objectBox[truePicSum].m_size.set(it->picOutSize.x + 1,it->picOutSize.y + 1);
			tempPacker.m_objectBox[truePicSum].m_acreage = tempPacker.m_objectBox[truePicSum].m_size.x * tempPacker.m_objectBox[truePicSum].m_size.y;
			++ truePicSum;
		}
		//tempPacker.m_objectBoxSum = picSum;
		tempPacker.m_objectBoxSum = truePicSum;

		tempPacker.setOptimizeLevel(true);
		if(tempPacker.findResult() == 0)
		{
			printf("Picture optimize Error!\n");
			return 0;
		}
	}else
	{
		//下面是优化了最后一个盒子
		int tryTimer = 0;
		int needBoxSumTemp[MAX_BASICBOXSUM];
		int finishedFlag = 0;
		int curOrderMode = 0;	//排序的方式
		//下面的变量用于记录最好的结果
		int bestTryTimer = -1;	//无效
		int bestTryMode = 0;
		int bestNeedBoxSum[MAX_BASICBOXSUM];
		float bestRate = 0.0f;
		while(true)
		{
			//初始化所有盒子的信息
			tempPacker.m_basicBox = XMem::createArrayMem<XBasicBox>(MAX_BASICBOXSUM);	//定义100个盒子
			for(int i = 0;i < MAX_BASICBOXSUM;++ i)
			{
				tempPacker.m_basicBox[i].m_beUsed = 0;
				tempPacker.m_basicBox[i].m_objectBoxesOrder = XMem::createArrayMem<int>(picData.size());
				tempPacker.m_basicBox[i].m_order = i;
				if(tryTimer == 0)
				{
					tempPacker.m_basicBox[i].m_size.set(maxW,maxH);
				}else
				{
					int w = maxW;
					int h = maxH;
					for(int j = 0;j < tryTimer;++ j)
					{
						if(i < needBoxSumTemp[j] - 1) break;
						if(maxW > maxH)
						{
							if(j % 2 == 0) w = w >> 1;
							else h = h >> 1;
						}else
						{
							if(j % 2 == 0) h = h >> 1;
							else w = w >> 1;
						}
					}
					tempPacker.m_basicBox[i].m_size.set(w,h);
					//if(w <= 64 && h <= 64) 
					//{
					//	finishedFlag = 1;
					//	printf("Error!\n");
					//}
				}
				tempPacker.m_basicBox[i].m_releaseAcreage = tempPacker.m_basicBox[i].m_size.x * tempPacker.m_basicBox[i].m_size.y;

				tempPacker.m_basicBox[i].m_mayPosition = XMem::createArrayMem<XVec2>(4 * picData.size());
				tempPacker.m_basicBox[i].m_mayPositionSum = 1;
				tempPacker.m_basicBox[i].m_mayPosition[0].set(0.0f);

				tempPacker.m_basicBox[i].m_mayPositionX = XMem::createArrayMem<int>(2 * picData.size());
				tempPacker.m_basicBox[i].m_mayPositionXSum = 1;
				tempPacker.m_basicBox[i].m_mayPositionX[0] = 0;
				tempPacker.m_basicBox[i].m_mayPositionY = XMem::createArrayMem<int>(2 * picData.size());
				tempPacker.m_basicBox[i].m_mayPositionYSum = 1;
				tempPacker.m_basicBox[i].m_mayPositionY[0] = 0;
			}
			tempPacker.m_basicBoxSum = MAX_BASICBOXSUM;

			tempPacker.m_objectBox = XMem::createArrayMem<XObjectBox>(picData.size());	//定义了MAX_OBJECTSUM个物件
			int truePicSum = 0;
			for(auto it = picData.begin();it != picData.end();++ it)
			{//如果存在相同的贴图，则不需要重复放置
				if(it->sameTexOrder >= 0) continue;
				tempPacker.m_objectBox[truePicSum].m_order = truePicSum;
				tempPacker.m_objectBox[truePicSum].m_setOrder = it - picData.begin();
				tempPacker.m_objectBox[truePicSum].m_position.reset();
				tempPacker.m_objectBox[truePicSum].m_basicBoxOrder = -1;
				//这里为了防止图像裁剪缩放造成的杂色，需要每个图片之间间隔一个像素
			//	tempPacker.m_objectBox[truePicSum].m_size = it->picOutSize;
				tempPacker.m_objectBox[truePicSum].m_size.set(it->picOutSize.x + 1, it->picOutSize.y + 1);
				tempPacker.m_objectBox[truePicSum].m_acreage = tempPacker.m_objectBox[truePicSum].m_size.x * tempPacker.m_objectBox[truePicSum].m_size.y;
				++ truePicSum;
			}
			//tempPacker.m_objectBoxSum = picSum;
			tempPacker.m_objectBoxSum = truePicSum;

			tempPacker.setOptimizeLevel(true);
			//下面是多次寻找，来找到更好的解，现在修改为只寻找一次。
			if(tempPacker.findResult(curOrderMode) == 0)
			{
				for(int i = 0;i < MAX_BASICBOXSUM;++ i)
				{
					XMem::XDELETE_ARRAY(tempPacker.m_basicBox[i].m_objectBoxesOrder);
					XMem::XDELETE_ARRAY(tempPacker.m_basicBox[i].m_mayPosition);
					XMem::XDELETE_ARRAY(tempPacker.m_basicBox[i].m_mayPositionX);
					XMem::XDELETE_ARRAY(tempPacker.m_basicBox[i].m_mayPositionY);
				}
				XMem::XDELETE_ARRAY(tempPacker.m_basicBox);
				XMem::XDELETE_ARRAY(tempPacker.m_objectBox);
				++ curOrderMode;
				tryTimer = 0;
				if(curOrderMode == 3)
				{//所有尝试完成
					if(bestTryTimer < 0)
					{
						printf("Picture optimize Error!\n");
						return -1;
					}else
					{
						tryTimer = bestTryTimer;
						curOrderMode = bestTryMode;
						memcpy(needBoxSumTemp,bestNeedBoxSum,MAX_BASICBOXSUM * sizeof(int));
						finishedFlag = 1;	//最后的结果
					}
				}
			}else
			{
				if(finishedFlag != 0)
				{//找到最终的结果
					break;
				}else
				{
					needBoxSumTemp[tryTimer] = tempPacker.getNeedBoxSum();
					float tmpRate = tempPacker.getUseRate();
					if(tmpRate > bestRate)
					{//如果结果更好，则记录
						bestTryTimer = tryTimer;
						bestTryMode = curOrderMode;
						memcpy(bestNeedBoxSum,needBoxSumTemp,MAX_BASICBOXSUM * sizeof(int));
						bestRate = tmpRate;
						printf("T:%d,O:%d,R:%f\n",tryTimer,curOrderMode,tmpRate);
					}
					++ tryTimer;
					for(int i = 0;i < MAX_BASICBOXSUM;++ i)
					{
						XMem::XDELETE_ARRAY(tempPacker.m_basicBox[i].m_objectBoxesOrder);
						XMem::XDELETE_ARRAY(tempPacker.m_basicBox[i].m_mayPosition);
						XMem::XDELETE_ARRAY(tempPacker.m_basicBox[i].m_mayPositionX);
						XMem::XDELETE_ARRAY(tempPacker.m_basicBox[i].m_mayPositionY);
					}
					XMem::XDELETE_ARRAY(tempPacker.m_basicBox);
					XMem::XDELETE_ARRAY(tempPacker.m_objectBox);
				}
			}
		}
	}
	printf("All picture files optimization.!\n");
	
	//下面这一段是性能消耗最大的，需要优化
	//拼合完成之后开始放图
	char fileName[MAX_FILE_NAME_LENGTH];
	strcpy_s(fileName,MAX_FILE_NAME_LENGTH,TEXTURE_FILE_NAME);
	for(allBasicBoxSum = 0;allBasicBoxSum < MAX_BASICBOXSUM;++ allBasicBoxSum)
	{
		if(tempPacker.m_basicBox[allBasicBoxSum].m_beUsed <= 0) break;
		//对图形进行处理
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		SDL_Surface * picArm = SDL_CreateRGBSurface(SDL_SWSURFACE,tempPacker.m_basicBox[allBasicBoxSum].m_size.x,tempPacker.m_basicBox[allBasicBoxSum].m_size.y,
			32,0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
#else
		SDL_Surface * picArm = SDL_CreateRGBSurface(SDL_SWSURFACE,tempPacker.m_basicBox[allBasicBoxSum].m_size.x,tempPacker.m_basicBox[allBasicBoxSum].m_size.y,
			32,0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
#endif
		fileName[strlen(TEXTURE_FILE_NAME) - 5] = (allBasicBoxSum % 10) + '0';
		fileName[strlen(TEXTURE_FILE_NAME) - 6] = ((allBasicBoxSum/10) % 10) + '0';
		fileName[strlen(TEXTURE_FILE_NAME) - 7] = ((allBasicBoxSum/100) % 10) + '0';

		unsigned int *pA = (unsigned int *)picArm->pixels;
		int objBoxOrder;
		int picOrder;
		unsigned int *p;
		int h;
		unsigned int *pATemp;
		unsigned int *pTemp;
		int sizeTemp;
		for(int i = 0;i < tempPacker.m_basicBox[allBasicBoxSum].m_beUsed;++ i)
		{
			objBoxOrder = tempPacker.m_basicBox[allBasicBoxSum].m_objectBoxesOrder[i];
			picOrder = tempPacker.m_objectBox[objBoxOrder].m_setOrder;
			
			if(!m_isOptimizeInMem)
				if(!picData[picOrder].loadImage()) return 0;
			//如果PNG不带透明通道，这里会存在问题
			if(picData[picOrder].pPic->format->BytesPerPixel == 4)
			{
				p = (unsigned int *)picData[picOrder].pPic->pixels;

				pATemp = pA + (int)(tempPacker.m_objectBox[objBoxOrder].m_position.y) * picArm->w
					+ (int)(tempPacker.m_objectBox[objBoxOrder].m_position.x);
				pTemp = p + (int)(picData[picOrder].leftTopPoint.y) * picData[picOrder].pPic->w
					+ (int)(picData[picOrder].leftTopPoint.x);
				sizeTemp = (int)(picData[picOrder].picOutSize.x) << 2;

				for(h = 0;h < picData[picOrder].picOutSize.y;++ h)
				{
					memcpy(pATemp + h * picArm->w,pTemp + h * picData[picOrder].pPic->w,sizeTemp);
				}
			}else
			{
				printf("不带透明通道的PNG图片会造成问题，请修改后再进行优化!\n");
			}
		/*	for(h = tempPacker.m_objectBox[objBoxOrder].m_position.y;
				h < tempPacker.m_objectBox[objBoxOrder].m_position.y + picData[picOrder].picOutSize.y;
				++ h)
			{
				memcpy(pA + h * picArm->w + (int)(tempPacker.m_objectBox[objBoxOrder].m_position.x),
					p + ((int)(picData[picOrder].leftTopPoint.y) + (h - (int)(tempPacker.m_objectBox[objBoxOrder].m_position.y))) * picData[picOrder].pPic->w
					+ (int)(picData[picOrder].leftTopPoint.x),
					(int)(picData[picOrder].picOutSize.x) << 2);
			}*/
			picData[picOrder].outFileName = fileName;
			picData[picOrder].textureOrder = allBasicBoxSum;
			picData[picOrder].picOffsetPosition = tempPacker.m_objectBox[objBoxOrder].m_position;
			
			if(!m_isOptimizeInMem)
				picData[picOrder].releaseImage();
		}
//		if(savePNG(fileName,picArm,Z_BEST_COMPRESSION) == 0) return 0;
		XPng::savePNG(fileName,picArm,Z_BEST_COMPRESSION);
		//释放资源
		SDL_FreeSurface(picArm);
		printf("the %d registration completed!\n",allBasicBoxSum);
	}
	//下面处理相同图片资源的信息
	for(auto it = picData.begin();it != picData.end();++ it)
	{
		if(it->sameTexOrder >= 0 && it->sameTexOrder < picData.size())
		{
			it->outFileName = picData[it->sameTexOrder].outFileName;
			it->textureOrder = picData[it->sameTexOrder].textureOrder;
			it->picOffsetPosition = picData[it->sameTexOrder].picOffsetPosition;
		}
	}
	//下面开始释放资源
	for(int i = 0;i < MAX_BASICBOXSUM;++ i)
	{
		XMem::XDELETE_ARRAY(tempPacker.m_basicBox[i].m_objectBoxesOrder);
		XMem::XDELETE_ARRAY(tempPacker.m_basicBox[i].m_mayPosition);
		XMem::XDELETE_ARRAY(tempPacker.m_basicBox[i].m_mayPositionX);
		XMem::XDELETE_ARRAY(tempPacker.m_basicBox[i].m_mayPositionY);
	}
	XMem::XDELETE_ARRAY(tempPacker.m_basicBox);
	XMem::XDELETE_ARRAY(tempPacker.m_objectBox);

	printf("All pictures and optimal output complete!\n");

	return allBasicBoxSum;
}
//返回值1：两个像素完成相同 0：不符合要求 2：两个像素不同，但在容差范围内
inline int pixelCompare(const unsigned int *p0,const unsigned int *p1,int maxPixelsOffset)
{
	unsigned char *pc0 = (unsigned char *)p0;
	unsigned char *pc1 = (unsigned char *)p1;
	if(pc0[3] == 0 && pc0[3] == pc1[3]) return 1;	//透明通道相同且全透明，则不需要比较其他颜色
	if(* p0 == * p1) return 1;
	if(maxPixelsOffset != 0)
	{
		if(pc0[3] < pc1[3] - maxPixelsOffset || pc0[3] > pc1[3] + maxPixelsOffset) return 0;	//判断透明通道是否符合要求
		else if(pc0[3] < maxPixelsOffset || pc1[3] < maxPixelsOffset) return 2;	//如果透明通道几乎透明也不考虑其他颜色的分量

		if(pc0[0] < pc1[0] - maxPixelsOffset || pc0[0] > pc1[0] + maxPixelsOffset) return 0;	//不符合要求
		if(pc0[1] < pc1[1] - maxPixelsOffset || pc0[1] > pc1[1] + maxPixelsOffset) return 0;	//不符合要求
		if(pc0[2] < pc1[2] - maxPixelsOffset || pc0[2] > pc1[2] + maxPixelsOffset) return 0;	//不符合要求

		return 2;
	}
	return 0;
}
//返回值1：两个像素完成相同 0：不符合要求 2：两个像素不同，但是在容差范围内
inline int pixelCompare(const unsigned int *p,int maxPixelsOffset)
{
	unsigned char *pc = (unsigned char *)p;
	if(pc[3] == 0) return 1;
	if(pc[3] > maxPixelsOffset) return 0;	//不符合要求
	return 2;
}
//maxPixelsOffset为容差，在容差范围内的图片则认为相同
//返回两张图片是否相同
bool XResourceOptimize::imageCompare(XPicProcData &tex0,XPicProcData &tex1,int maxPixelsOffset)
{
	//两个贴图的有效尺寸必须相同
	if(tex0.picOutSize.x != tex0.picOutSize.x || tex0.picOutSize.y != tex0.picOutSize.y) return false;
	//比较中间的5个特殊点
	if(!m_isOptimizeInMem)
		if(!tex1.loadImage()) return false;
	unsigned int *p0 = (unsigned int *)tex0.pPic->pixels;
	unsigned int *p1 = (unsigned int *)tex1.pPic->pixels;
	int x0 = tex0.leftTopPoint.x + tex0.picOutSize.x / 4.0f;
	int y0 = tex0.leftTopPoint.y + tex0.picOutSize.y / 4.0f;
	int x1 = tex1.leftTopPoint.x + tex1.picOutSize.x / 4.0f;
	int y1 = tex1.leftTopPoint.y + tex1.picOutSize.y / 4.0f;
	if(pixelCompare(p0 + y0 * tex0.pPic->w + x0,p1 + y1 * tex1.pPic->w + x1,maxPixelsOffset) == 0)
	{
		if(!m_isOptimizeInMem)
			tex1.releaseImage();
		return false;
	}
	x0 = tex0.leftTopPoint.x + (tex0.picOutSize.x / 4.0f) * 3;
	//y0 = tex0.leftTopPoint.y + tex0.picOutSize.y / 4.0f;
	x1 = tex1.leftTopPoint.x + (tex1.picOutSize.x / 4.0f) * 3;
	//y1 = tex1.leftTopPoint.y + tex1.picOutSize.y / 4.0f;
	if(pixelCompare(p0 + y0 * tex0.pPic->w + x0,p1 + y1 * tex1.pPic->w + x1,maxPixelsOffset) == 0)
	{
		if(!m_isOptimizeInMem)
			tex1.releaseImage();
		return false;
	}
	//x0 = tex0.leftTopPoint.x + (tex0.picOutSize.x / 4.0f) * 3;
	y0 = tex0.leftTopPoint.y + (tex0.picOutSize.y / 4.0f) * 3;
	//x1 = tex1.leftTopPoint.x + (tex1.picOutSize.x / 4.0f) * 3;
	y1 = tex1.leftTopPoint.y + (tex1.picOutSize.y / 4.0f) * 3;
	if(pixelCompare(p0 + y0 * tex0.pPic->w + x0,p1 + y1 * tex1.pPic->w + x1,maxPixelsOffset) == 0)
	{
		if(!m_isOptimizeInMem)
			tex1.releaseImage();
		return false;
	}
	x0 = tex0.leftTopPoint.x + tex0.picOutSize.x / 4.0f;
	//y0 = tex0.leftTopPoint.y + (tex0.picOutSize.y / 4.0f) * 3;
	x1 = tex1.leftTopPoint.x + tex1.picOutSize.x / 4.0f;
	//y1 = tex1.leftTopPoint.y + (tex1.picOutSize.y / 4.0f) * 3;
	if(pixelCompare(p0 + y0 * tex0.pPic->w + x0,
		p1 + y1 * tex1.pPic->w + x1,maxPixelsOffset) == 0)
	{
		if(!m_isOptimizeInMem)
			tex1.releaseImage();
		return false;
	}
	x0 = tex0.leftTopPoint.x + tex0.picOutSize.x / 2.0f;
	y0 = tex0.leftTopPoint.y + tex0.picOutSize.y / 2.0f;
	x1 = tex1.leftTopPoint.x + tex1.picOutSize.x / 2.0f;
	y1 = tex1.leftTopPoint.y + tex1.picOutSize.y / 2.0f;
	if(pixelCompare(p0 + y0 * tex0.pPic->w + x0,
		p1 + y1 * tex1.pPic->w + x1,maxPixelsOffset) == 0)
	{
		if(!m_isOptimizeInMem)
			tex1.releaseImage();
		return false;
	}
	//如果上面的5个特殊点全部相同，则比较全部的像素
	for(int i = 0;i < tex0.picOutSize.y;++ i)
	{
		y0 = (i + tex0.leftTopPoint.y) * tex0.pPic->w + tex0.leftTopPoint.x;
		y1 = (i + tex1.leftTopPoint.y) * tex1.pPic->w + tex1.leftTopPoint.x;
		for(int j = 0;j < tex0.picOutSize.x;++ j)
		{
			if(pixelCompare(p0 + y0 + j,
				p1 + y1 + j,maxPixelsOffset) == 0) 
			{
				if(!m_isOptimizeInMem)
					tex1.releaseImage();
				return false;
			}
		}
	}
	//所有像素相同，则两个图片相同
	if(!m_isOptimizeInMem)
		tex1.releaseImage();
	return true;
}

//强化版的贴图比较函数，允许有容差
//maxOffset 允许的最大图像比较位置偏差，这个数值越大，花费的时间越多
//maxPixelsOffset 允许的颜色偏差，这个数值越大，允许的颜色偏差越大
//maxDiffRate (0 - 1) 这个数值是允许出现颜色偏差的点的数量
bool XResourceOptimize::imageCompareEx(XPicProcData &tex0,XPicProcData &tex1,int maxOffset,int maxPixelsOffset,float maxDiffRate)
{//尚未完成，记下来就是要完成像素矩阵移动的比较，由于存在图片移动之后的超出问题，所以这里边的比较麻烦
	//目前做到，比较完成之后使用最小的剪裁面积，来重新写入剪裁信息。
	if(maxDiffRate < 0) maxDiffRate = 0.0f;
	if(maxDiffRate > 1.0f) maxDiffRate = 1.0f;
	//两个贴图的有效尺寸必须小于等于最小偏差
	if(tex0.picOutSize.x < tex1.picOutSize.x - maxOffset
		|| tex0.picOutSize.x > tex1.picOutSize.x + maxOffset
		|| tex0.picOutSize.y < tex1.picOutSize.y - maxOffset
		|| tex0.picOutSize.y > tex1.picOutSize.y + maxOffset) return false;
	//比较中间的5个特殊点
	int maxX,maxY;	//需要比较两个图片有小部分最大的那个尺寸
	if(tex0.picOutSize.x < tex1.picOutSize.x) maxX = tex0.picOutSize.x;
	else maxX = tex1.picOutSize.x;
	if(tex0.picOutSize.y < tex1.picOutSize.y) maxY = tex0.picOutSize.y;
	else maxY = tex1.picOutSize.y;
	int dx = 0,dy = 0;
	int i = 0,j = 0,ret = 0;
	int x0 = 0,y0 = 0,x1 = 0,y1 = 0;
	float diffPixSum = 0;
	
	if(!m_isOptimizeInMem)
		if(!tex1.loadImage()) return false;

	unsigned int *p0 = (unsigned int *)tex0.pPic->pixels;
	unsigned int *p1 = (unsigned int *)tex1.pPic->pixels;
	int maxDiffPixelsSum = tex0.picOutSize.x * tex0.picOutSize.y * maxDiffRate * maxDiffRate;
	for(dx = -maxOffset;dx < maxOffset;++ dx)
	{
		for(dy = -maxOffset;dy < maxOffset;++ dy)
		{
			x0 = tex0.leftTopPoint.x + dx + tex0.picOutSize.x / 4.0f;
			y0 = tex0.leftTopPoint.y + dy + tex0.picOutSize.y / 4.0f;
			x1 = tex1.leftTopPoint.x + tex0.picOutSize.x / 4.0f;
			y1 = tex1.leftTopPoint.y + tex0.picOutSize.y / 4.0f;
			if((x0 < 0 || y0 < 0 || x0 >= tex0.pPic->w || y0 >= tex0.pPic->h)
				&& !(x1 < 0 || y1 < 0 || x1 > tex1.pPic->w || y1 > tex1.pPic->h)
				&& pixelCompare(p1 + y1 * tex1.pPic->w + x1,maxPixelsOffset) == 0) continue;
			else if((x1 < 0 || y1 < 0 || x1 > tex1.pPic->w || y1 > tex1.pPic->h)
				&& pixelCompare(p0 + y0 * tex0.pPic->w + x0,maxPixelsOffset) == 0) continue;
			else if(pixelCompare(p0 + y0 * tex0.pPic->w + x0,p1 + y1 * tex1.pPic->w + x1,maxPixelsOffset) == 0) continue;

			x0 = tex0.leftTopPoint.x + dx + (tex0.picOutSize.x / 4.0f) * 3;
		//	y0 = tex0.leftTopPoint.y + dy + tex0.picOutSize.y / 4.0f;
			x1 = tex1.leftTopPoint.x + (tex0.picOutSize.x / 4.0f) * 3;
		//	y1 = tex1.leftTopPoint.y + tex0.picOutSize.y / 4.0f;
			if((x0 < 0 || y0 < 0 || x0 >= tex0.pPic->w || y0 >= tex0.pPic->h)
				&& !(x1 < 0 || y1 < 0 || x1 > tex1.pPic->w || y1 > tex1.pPic->h)
				&& pixelCompare(p1 + y1 * tex1.pPic->w + x1,maxPixelsOffset) == 0) continue;
			else if((x1 < 0 || y1 < 0 || x1 > tex1.pPic->w || y1 > tex1.pPic->h)
				&& pixelCompare(p0 + y0 * tex0.pPic->w + x0,maxPixelsOffset) == 0) continue;
			else if(pixelCompare(p0 + y0 * tex0.pPic->w + x0,p1 + y1 * tex1.pPic->w + x1,maxPixelsOffset) == 0) continue;

		//	x0 = tex0.leftTopPoint.x + dx + (tex0.picOutSize.x / 4.0f) * 3;
			y0 = tex0.leftTopPoint.y + dy + (tex0.picOutSize.y / 4.0f) * 3;
		//	x1 = tex1.leftTopPoint.x + (tex0.picOutSize.x / 4.0f) * 3;
			y1 = tex1.leftTopPoint.y + (tex0.picOutSize.y / 4.0f) * 3;
			if((x0 < 0 || y0 < 0 || x0 >= tex0.pPic->w || y0 >= tex0.pPic->h)
				&& !(x1 < 0 || y1 < 0 || x1 > tex1.pPic->w || y1 > tex1.pPic->h)
				&& pixelCompare(p1 + y1 * tex1.pPic->w + x1,maxPixelsOffset) == 0) continue;
			else if((x1 < 0 || y1 < 0 || x1 > tex1.pPic->w || y1 > tex1.pPic->h)
				&& pixelCompare(p0 + y0 * tex0.pPic->w + x0,maxPixelsOffset) == 0) continue;
			else if(pixelCompare(p0 + y0 * tex0.pPic->w + x0,p1 + y1 * tex1.pPic->w + x1,maxPixelsOffset) == 0) continue;

			x0 = tex0.leftTopPoint.x + dx + tex0.picOutSize.x / 4.0f;
		//	y0 = tex0.leftTopPoint.y + dy + (tex0.picOutSize.y / 4.0f) * 3;
			x1 = tex1.leftTopPoint.x + tex0.picOutSize.x / 4.0f;
			//y1 = tex1.leftTopPoint.y + (tex0.picOutSize.y / 4.0f) * 3;
			if((x0 < 0 || y0 < 0 || x0 >= tex0.pPic->w || y0 >= tex0.pPic->h)
				&& !(x1 < 0 || y1 < 0 || x1 > tex1.pPic->w || y1 > tex1.pPic->h)
				&& pixelCompare(p1 + y1 * tex1.pPic->w + x1,maxPixelsOffset) == 0) continue;
			else if((x1 < 0 || y1 < 0 || x1 > tex1.pPic->w || y1 > tex1.pPic->h)
				&& pixelCompare(p0 + y0 * tex0.pPic->w + x0,maxPixelsOffset) == 0) continue;
			else if(pixelCompare(p0 + y0 * tex0.pPic->w + x0,p1 + y1 * tex1.pPic->w + x1,maxPixelsOffset) == 0) continue;

			x0 = tex0.leftTopPoint.x + dx + tex0.picOutSize.x / 2.0f;
			y0 = tex0.leftTopPoint.y + dy + tex0.picOutSize.y / 2.0f;
			x1 = tex1.leftTopPoint.x + tex0.picOutSize.x / 2.0f;
			y1 = tex1.leftTopPoint.y + tex0.picOutSize.y / 2.0f;
			if((x0 < 0 || y0 < 0 || x0 >= tex0.pPic->w || y0 >= tex0.pPic->h)
				&& !(x1 < 0 || y1 < 0 || x1 > tex1.pPic->w || y1 > tex1.pPic->h)
				&& pixelCompare(p1 + y1 * tex1.pPic->w + x1,maxPixelsOffset) == 0) continue;
			else if((x1 < 0 || y1 < 0 || x1 > tex1.pPic->w || y1 > tex1.pPic->h)
				&& pixelCompare(p0 + y0 * tex0.pPic->w + x0,maxPixelsOffset) == 0) continue;
			else if(pixelCompare(p0 + y0 * tex0.pPic->w + x0,p1 + y1 * tex1.pPic->w + x1,maxPixelsOffset) == 0) continue;
			//如果上面的5个特殊点全部相同，则比较全部的像素
			diffPixSum = 0;
			for(i = 0;i < maxY;++ i)
			{
				y0 = i + tex0.leftTopPoint.y + dy;
				y1 = i + tex1.leftTopPoint.y;
				for(j = 0;j < maxX;++ j)
				{
					x0 = j + tex0.leftTopPoint.x + dx;
					x1 = j + tex1.leftTopPoint.x;
					if(x0 < 0 || y0 < 0 || x0 >= tex0.pPic->w || y0 >= tex0.pPic->h)
					{//图片0超出范围
						if(x1 < 0 || y1 < 0 || x1 > tex1.pPic->w || y1 > tex1.pPic->h)
						{//图片1也超出范围，则认为相同
							ret = 1;
						}else
						{
							ret = pixelCompare(p1 + y1 * tex1.pPic->w + x1,maxPixelsOffset);
						}
					}else
					if(x1 < 0 || y1 < 0 || x1 > tex1.pPic->w || y1 > tex1.pPic->h)
					{//图片一超出范围
						ret = pixelCompare(p0 + y0 * tex0.pPic->w + x0,maxPixelsOffset);
					}else
					{//两个都不超出范围
						ret = pixelCompare(p0 + y0 * tex0.pPic->w + x0,p1 + y1 * tex1.pPic->w + x1,maxPixelsOffset);
					}
					if(ret == 0) break;
					if(ret == 2) 
					{
						++ diffPixSum;
						//这里需要比较不同率然后做出处理
						if(diffPixSum > maxDiffPixelsSum)
						{
							ret = 0;
							break;
						}
					}
				}
				if(ret == 0) break;
			}
			if(ret == 0) continue;
			//所有比较完成之后，图片是满足要求的,这里需要处理一些图像的信息。取最小的贴图面积可以保证不会越界。
			if(dx == 0)
			{//x方向没有移动
				if(tex0.picOutSize.x < tex1.picOutSize.x)	//有损裁剪
				{//取小的	
					tex1.picOutSize.x = tex0.picOutSize.x;
					tex1.rightBottomPoint.x = tex1.pPic->w - tex1.leftTopPoint.x - tex1.picOutSize.x;
				}else
				if(tex0.picOutSize.x > tex1.picOutSize.x)	//有损裁剪
				{
					tex0.picOutSize.x = tex1.picOutSize.x;
					tex0.rightBottomPoint.x = tex0.pPic->w - tex0.leftTopPoint.x - tex0.picOutSize.x;
				}
			}else
			if(dx < 0)
			{//有损裁剪
				if(tex0.picOutSize.x - dx > tex1.picOutSize.x)
				{
					tex0.leftTopPoint.x += dx;
					tex0.picOutSize.x = tex1.picOutSize.x;
					tex0.rightBottomPoint.x = tex0.pPic->w - tex0.leftTopPoint.x - tex0.picOutSize.x;
				}else
				{
					tex0.leftTopPoint.x += dx;
					tex0.picOutSize.x -= dx;
					tex1.picOutSize.x = tex0.picOutSize.x;
					tex1.rightBottomPoint.x = tex1.pPic->w - tex1.leftTopPoint.x - tex1.picOutSize.x;
				}
			}else
			{//有损裁剪
				if(tex0.picOutSize.x + dx > tex1.picOutSize.x)
				{
					tex1.leftTopPoint.x += dx;
					tex1.picOutSize.x -= dx;
					tex0.picOutSize.x = tex1.picOutSize.x;
					tex0.rightBottomPoint.x = tex0.pPic->w - tex0.leftTopPoint.x - tex0.picOutSize.x;
				}else
				{
					tex1.leftTopPoint.x += dx;
					tex1.picOutSize.x -= tex0.picOutSize.x;
					tex1.rightBottomPoint.x = tex1.pPic->w - tex1.leftTopPoint.x - tex1.picOutSize.x;
				}
			}

			if(dy == 0)
			{//x方向没有移动
				if(tex0.picOutSize.y < tex1.picOutSize.y)	//有损裁剪
				{//取小的	
					tex1.picOutSize.y = tex0.picOutSize.y;
					tex1.rightBottomPoint.y = tex1.pPic->h - tex1.leftTopPoint.y - tex1.picOutSize.y;
				}else
				if(tex0.picOutSize.y > tex1.picOutSize.y)	//有损裁剪
				{
					tex0.picOutSize.y = tex1.picOutSize.y;
					tex0.rightBottomPoint.y = tex0.pPic->w - tex0.leftTopPoint.y - tex0.picOutSize.y;
				}
			}else
			if(dy < 0)
			{//有损裁剪
				if(tex0.picOutSize.y - dy > tex1.picOutSize.y)
				{
					tex0.leftTopPoint.y += dy;
					tex0.picOutSize.y = tex1.picOutSize.y;
					tex0.rightBottomPoint.y = tex0.pPic->w - tex0.leftTopPoint.y - tex0.picOutSize.y;
				}else
				{
					tex0.leftTopPoint.y += dy;
					tex0.picOutSize.y -= dy;
					tex1.picOutSize.y = tex0.picOutSize.y;
					tex1.rightBottomPoint.y = tex1.pPic->w - tex1.leftTopPoint.y - tex1.picOutSize.y;
				}
			}else
			{//有损裁剪
				if(tex0.picOutSize.y + dy > tex1.picOutSize.y)
				{
					tex1.leftTopPoint.y += dy;
					tex1.picOutSize.y -= dy;
					tex0.picOutSize.y = tex1.picOutSize.y;
					tex0.rightBottomPoint.y = tex0.pPic->w - tex0.leftTopPoint.y - tex0.picOutSize.y;
				}else
				{
					tex1.leftTopPoint.y += dy;
					tex1.picOutSize.y -= tex0.picOutSize.y;
					tex1.rightBottomPoint.y = tex1.pPic->w - tex1.leftTopPoint.y - tex1.picOutSize.y;
				}
			}
			if(!m_isOptimizeInMem)
				tex1.releaseImage();
			return true;	//图像相同
		}
	}
	//所有比较完成都不满足需求
	if(!m_isOptimizeInMem)
		tex1.releaseImage();
	return false;
}
bool XResourceOptimize::optimize(bool optInMem,bool withOpti)		//优化
{
	m_isOptimizeInMem = optInMem;
	clearTempFile();
	int allPicSum = makePNGFileList();//总共有需要处理的序列帧的数量
	if(allPicSum <= 0) 
	{
#ifdef UNICODE
		DeleteFileA(PNG_FILE_LIST_NAME);
#else
		DeleteFile(PNG_FILE_LIST_NAME);
#endif
		return XTrue;	//没有资源需要优化，直接返回优化完成
	}

	std::vector<XPicProcData> picDatas;

	FILE *fp;
	if((fp = fopen(PNG_FILE_LIST_NAME,"r")) == NULL)
	{
		printf("frame.txt Open error!\n");
		return XFalse;
	}

	//读取图片名称和路径,以及处理图片
	char tempFileName[MAX_FILE_NAME_LENGTH] = "";
	char tempFileName1[MAX_FILE_NAME_LENGTH] = "";
	int len = 0;
	for(int i = 0;i < allPicSum;++ i)
	{
		XPicProcData tmp;
		if(fscanf(fp,"%s",tempFileName) != 1) return XFalse;
		//这里需要处理掉文件夹：pictureResource,与normalResource
		strcpy_s(tempFileName1,MAX_FILE_NAME_LENGTH,tempFileName);
		len = strlen(tempFileName1);
		for(int j = 0;j < len;++j)
		{
			if(tempFileName1[j] == '/' || tempFileName1[j] == '\\')
			{
				tempFileName1[j] = '\0';
			}
		}
		if(m_needOptimizeFolder.size() > 0)
		{
			bool tmpFlag = false;
			for(auto kt = m_needOptimizeFolder.begin();kt != m_needOptimizeFolder.end();++ kt)
			{
				if (XFile::fileNameCompare(tempFileName1, (*kt).c_str())) continue;		//只处理筛选目录的文件夹
				tmpFlag = true;
				break;
			}
			if(tmpFlag) continue;
		}else
		if(m_needNotOptimizeFolder.size() > 0)
		{
			bool tmpFlag = false;
			for (auto kt = m_needNotOptimizeFolder.begin(); kt != m_needNotOptimizeFolder.end(); ++kt)
			{
				if (!XFile::fileNameCompare(tempFileName1, (*kt).c_str())) continue;		//不处理该筛选目录下的图片
				tmpFlag = true;
				break;
			}
			if(tmpFlag) continue;
		}
		tmp.fileName = tempFileName;
		if(!getPictureRect(tmp,MAX_PIXELS_OFFSET))
		{
			printf("Picture resize error:%s!\n",tempFileName);
			fclose(fp);
			return XFalse;
		}
		picDatas.push_back(tmp);
	}	
#ifdef _WIN64
	printf("All files resizing completed! %zd\n",picDatas.size());
#else
	printf("All files resizing completed! %d\n",picDatas.size());
#endif
	if(picDatas.size() <= 0)
	{
		fclose(fp);
#ifdef UNICODE
		DeleteFileA(PNG_FILE_LIST_NAME);
#else
		DeleteFile(PNG_FILE_LIST_NAME);
#endif
		return XTrue;
	}
	//下面开始比较图片是否有相同的
	int sameTexSum = 0;
	for(int i = int(picDatas.size()) - 1;i > 0;-- i)
	{
		picDatas[i].sameTexOrder = -1;	//初始化为么有相同的资源
		if(!m_isOptimizeInMem)
			if(!picDatas[i].loadImage()) return false;
		printf("%d / %d\n",i,int(picDatas.size()) - 1);
		for(int j = 0;j < i;++ j)
		{
			//注意：imageCompareEx会修改两张图片的信息，这样:后比较的图片可能会修改之前比较的图片的尺寸造成问题
			//修正这个问题会造成问题复杂化，由于上面问题的存在，所以不使用这种比较方式
			//if(imageCompareEx(&(picData[i]),&(picData[j]),5,10,1.0f))	//现在可以通过开启这一行来测试图片资源中近似图像的信息
			if(imageCompare(picDatas[i],picDatas[j],MAX_PIXELS_OFFSET))
			{//找到相同的图片资源
				++sameTexSum;
				printf("+:%s\n-:%s\n",picDatas[i].fileName.c_str(),picDatas[j].fileName.c_str());
				picDatas[i].sameTexOrder = j;
				break;
			}
		}
		if(!m_isOptimizeInMem)
			picDatas[i].releaseImage();
	}
	printf("The same pictures sum:%d\n",sameTexSum);
	//下面开始拼合图片
	int allOutputPicSum = imagePack(picDatas,withOpti);
	if(allOutputPicSum == 0)
	{
		fclose(fp);
		return XFalse;
	}

	//将所有的数据写入到序列帧信息文件中
	printf("Writing file's infomation to infomationfile!\n");
	if(!writeFileData(picDatas,allOutputPicSum))
	{
		fclose(fp);
		return XFalse;
	}
	
	fclose(fp);
	//释放资源
	for(auto it = picDatas.begin();it != picDatas.end();++ it)
	{
		it->releaseImage();
	}
	return XTrue;
}
void XResourceOptimize::clearTempFile()	//清除临时文件
{
#ifdef UNICODE
	DeleteFileA(PNG_FILE_LIST_NAME);
	DeleteFileA(PNG_INFORMATION_FILE_NAME);

	char fileName[MAX_FILE_NAME_LENGTH];
	strcpy_s(fileName,MAX_FILE_NAME_LENGTH,TEXTURE_FILE_NAME);
	for(int i = 0;i < MAX_BASICBOXSUM;++ i)
	{
		fileName[strlen(TEXTURE_FILE_NAME) - 5] = (i % 10) + '0';
		fileName[strlen(TEXTURE_FILE_NAME) - 6] = ((i/10) % 10) + '0';
		fileName[strlen(TEXTURE_FILE_NAME) - 7] = ((i/100) % 10) + '0';
		DeleteFileA(fileName);
	}
#else
	DeleteFile(PNG_FILE_LIST_NAME);
	DeleteFile(PNG_INFORMATION_FILE_NAME);

	char fileName[MAX_FILE_NAME_LENGTH];
	strcpy_s(fileName,MAX_FILE_NAME_LENGTH,TEXTURE_FILE_NAME);
	for(int i = 0;i < MAX_BASICBOXSUM;++ i)
	{
		fileName[strlen(TEXTURE_FILE_NAME) - 5] = (i % 10) + '0';
		fileName[strlen(TEXTURE_FILE_NAME) - 6] = ((i/10) % 10) + '0';
		fileName[strlen(TEXTURE_FILE_NAME) - 7] = ((i/100) % 10) + '0';
		DeleteFile(fileName);
	}
#endif
}
//将优化数据写入到数据文件中以便于调用
bool XResourceOptimize::writeFileData(std::vector<XPicProcData> &picData,int allOutputSum)
{
	FILE *fp;
	if((fp = fopen(PNG_INFORMATION_FILE_NAME,"w")) == NULL)
	{
		printf("frame.txt Open error!\n");
		return false;
	}

	fprintf(fp,"%d,%d;\n",picData.size(),allOutputSum);
	//写入具体的某一帧的信息
	for(auto it = picData.begin();it != picData.end();++ it)
	{
		fprintf(fp,"%s:%s:%d:%d,%d:%d,%d,%d,%d:%d,%d;\n",it->fileName.c_str(), it->outFileName.c_str(),
			it->textureOrder,
			(int)(it->picOffsetPosition.x),(int)(it->picOffsetPosition.y),
			(int)(it->leftTopPoint.x),(int)(it->leftTopPoint.y),
			(int)(it->rightBottomPoint.x),(int)(it->rightBottomPoint.y)
			,(int)(it->picOutSize.x),(int)(it->picOutSize.y));
	}
	fclose(fp);

	if((fp = fopen(PNG_FILE_LIST_NAME,"w")) == NULL)
	{
		printf("frame.txt Open error!\n");
		return false;
	}
	//写入具体的某一帧的信息
	for(auto it = picData.begin();it != picData.end();++ it)
	{
		fprintf(fp,"%s\n",it->fileName.c_str());
	}
	fclose(fp);
	return true;
}
//将目录以及子目录下的所有PNG文件全部枚举出来，并输出到指定文件
int XResourceOptimize::makePNGFileList()
{
	char temp[512];
	//产生所有png文件的列表
	sprintf_s(temp,512,"dir *.png /s/b>%s",PNG_FILE_LIST_NAME);
	system(temp);
	//处理输出文件为相对路径
	//打开文件列表
	FILE *fp = NULL;
	if((fp = fopen(PNG_FILE_LIST_NAME,"r")) == NULL)
	{
		printf("Files list file open error!\n");
		return -1;
	}
	//完成文件列表的信息
	int pathBaseDeep = XFile::getPathDeepByChar(XFile::getCurrentExeFileFullPath().c_str());
	char fileNameTemp[MAX_FILE_NAME_LENGTH] = "";
	std::vector<std::string> pngFiles;	//所有的png文件
	while(!feof(fp))
	{
		if(fscanf(fp,"%s",fileNameTemp) != 1) break;
		if(strlen(fileNameTemp) < 4 || fileNameTemp[strlen(fileNameTemp) - 4]  != '.') continue;
		
		int len = (int)(strlen(fileNameTemp));
		for(int j = 0;j < len;++ j)
		{
			if(fileNameTemp[j] == '\\') fileNameTemp[j] = '/';
		}
		pngFiles.push_back(fileNameTemp);
	}
	fclose(fp);
	//处理成相对路径
	if((fp = fopen(PNG_FILE_LIST_NAME,"w")) == NULL)
	{
		printf("Files list file open error!\n");
		return -1;
	}
	for(auto it = pngFiles.begin();it != pngFiles.end();++ it)
	{
		fprintf(fp,"%s\n",&((*it).c_str()[pathBaseDeep + 1]));
	}
	fclose(fp);

	return pngFiles.size();
}
void XResourceOptimize::addNeedFolder(const std::string& folder)		//增加需要处理的文件夹
{
	for(unsigned int i = 0;i < m_needOptimizeFolder.size(); ++ i)
	{
		if(XFile::fileNameCompare(m_needOptimizeFolder[i].c_str(),folder.c_str())) return;
	}
	m_needOptimizeFolder.push_back(folder);
}
void XResourceOptimize::addNeedNotFolder(const std::string& folder)	//增加不需要处理的文件夹
{
	for(unsigned int i = 0;i < m_needNotOptimizeFolder.size(); ++ i)
	{
		if(XFile::fileNameCompare(m_needNotOptimizeFolder[i].c_str(),folder.c_str())) return;
	}
	m_needNotOptimizeFolder.push_back(folder);
}
}