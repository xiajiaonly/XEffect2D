#ifndef _JIA_XRESOURCEOPTIMIZE_
#define _JIA_XRESOURCEOPTIMIZE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------
//#include "XTextureInformation.h"
#include "XCommonDefine.h"
#include <vector>
namespace XE{
#define PNG_FILE_LIST_NAME "pngFileList.txt"
#define PNG_INFORMATION_FILE_NAME "textureInformation.txt"

//资源优化和打包统一处理之后造成优化的最后一张图不够优化，这个需要后面处理
#define MAX_PIXELS_OFFSET (0)	//图片优化时允许的图片像素的容差

struct XPicProcData;
class XResourceOptimize
{
public:
	XResourceOptimize()
		:m_isOptimizeInMem(true)
	{}
	virtual ~XResourceOptimize() {/*clearTempFile();*/ }
protected:
	XResourceOptimize(const XResourceOptimize&);
	XResourceOptimize &operator= (const XResourceOptimize&);
public:
	//下面是外部调用的接口
	//optInMem是否在内存中进行优化,这样速度较快,但是如果资源较多时会造成内存耗尽
	//withOpti是否使用统一的贴图大小。如果不使用统一的题图大小则优化效果更好
	bool optimize(bool optInMem = false, bool withOpti = false);		//优化
	void clearFolderSet()		//删除所有文件夹筛选设定
	{
		m_needOptimizeFolder.clear();
		m_needNotOptimizeFolder.clear();
	}
	void addNeedFolder(const std::string& folder);		//增加需要处理的文件夹
	void addNeedNotFolder(const std::string& folder);	//增加不需要处理的文件夹
	void clearTempFile();	//清除临时文件
private:
	bool m_isOptimizeInMem;	//是否在内存中进行优化，当图片数量较少时，则个速度很快，当图片数量较多时，这里会造成内存不足而退出	(未生效)
	//m_needOptimizeFolder和m_needNotOptimizeFolder只取其一，且m_needOptimizeFolder有较高优先级	
	std::vector<std::string> m_needOptimizeFolder;		//需要处理的文件夹，只处理这里的文件夹	(未生效)
	std::vector<std::string> m_needNotOptimizeFolder;	//不需要处理的文件夹，全部都处理不处理指定的文件夹	(未生效)

	bool writeFileData(std::vector<XPicProcData> &picData, int allOutputSum);	//将优化之后的数据写入到数据文件中
	int makePNGFileList();
	bool getPictureRect(XPicProcData& result, int maxPixelsOffset);				//根据容差计算图片有效区域的范围
	int imagePack(std::vector<XPicProcData>& picData, bool withOpti = false);							//将图片数据进行拼合
	bool imageCompare(XPicProcData& tex0, XPicProcData& tex1, int maxPixelsOffset);	//比较两张照片是否相同或者差异是否在maxPixelsOffset约束的容差范围内
	bool imageCompareEx(XPicProcData& tex0, XPicProcData& tex1, int maxOffset, 
		int maxPixelsOffset, float maxDiffRate);
};
}
#endif