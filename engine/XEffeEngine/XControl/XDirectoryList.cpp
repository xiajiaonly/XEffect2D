#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XDirectoryList.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "XResourcePack.h"
namespace XE{
XDirListSkin::XDirListSkin()
	:m_isInited(XFalse)
	, dirListNormal(NULL)
	, dirListDisable(NULL)
{}
XBool XDirListSkin::init(const char *normal,const char *disable,XResPos resPos)
{
	if(m_isInited ||
		normal == NULL || disable == NULL) return XFalse;
	
	if((dirListNormal = createATextureData(normal,resPos)) == NULL) return XFalse;
	if((dirListDisable = createATextureData(disable,resPos)) == NULL)
	{
		XMem::XDELETE(dirListNormal);
		return XFalse;
	}

	m_isInited = XTrue;
	return XTrue;
}
#define DIRLIST_CONFIG_FILENAME "DirevtoryList.txt"
bool XDirListSkin::loadFromFolder(const char *filename,XResPos resPos)	//从文件夹中载入资源
{
	//下面从配置文件中读取数据
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,DIRLIST_CONFIG_FILENAME);
	FILE *fp = NULL;
	if((fp = fopen(tempFilename,"r")) == NULL) return XFalse; //信息文件读取失败
	//下面开始依次读取数据
	int flag = 0;
	char resFilename[MAX_FILE_NAME_LENGTH] = "";
	//normal
	if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
	if(flag == 0)
	{//没有普通状态是不行的
		fclose(fp);
		return XFalse;
	}
	if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
	if((dirListNormal = createATextureData(tempFilename,resPos)) == NULL)
	{//资源读取失败
		fclose(fp);
		return XFalse;
	}
	//disable
	if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
	if(flag == 0)
	{//没有普通状态是不行的
		fclose(fp);
		return XFalse;
	}
	if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
	if((dirListDisable = createATextureData(tempFilename,resPos)) == NULL)
	{//资源读取失败
		fclose(fp);
		return XFalse;
	}
	//读取两组数据
	int l,t,r,b;
	if(fscanf(fp,"%d,%d,%d,%d,",&l,&t,&r,&b) != 4) {fclose(fp);return XFalse;}
	m_mouseRect.set(l,t,r,b);
	//所有数据读取完成
	fclose(fp);
	return true;
}
bool XDirListSkin::loadFromPacker(const char *filename,XResPos resPos)	//从压缩包中载入资源
{
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,DIRLIST_CONFIG_FILENAME);
	unsigned char *p = XResPack.getFileData(tempFilename);
	if(p == NULL) return XFalse;

	//下面开始依次读取数据
	int flag = 0;
	char resFilename[MAX_FILE_NAME_LENGTH] = "";
	int offset = 0;
	//normal
	if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),':') + 1;
	if(flag == 0)
	{//没有普通状态是不行的
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
	if((dirListNormal = createATextureData(tempFilename,resPos)) == NULL)
	{//资源读取失败
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),':') + 1;
	if(flag == 0)
	{//没有普通状态是不行的
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
	if((dirListDisable = createATextureData(tempFilename,resPos)) == NULL)
	{//资源读取失败
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	//读取两组数据
	int l,t,r,b;
	if(sscanf((char *)(p + offset),"%d,%d,%d,%d,",&l,&t,&r,&b) != 4) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
	m_mouseRect.set(l,t,r,b);
	//所有数据读取完成
	XMem::XDELETE_ARRAY(p);
	return true;
}
bool XDirListSkin::loadFromWeb(const char *filename,XResPos resPos)		//从网页中读取资源
{
	return false;
}
XBool XDirListSkin::initEx(const char *filename,XResPos resPos)
{
	if(m_isInited ||
		filename == NULL) return XFalse;
	//下面从配置文件中读取数据
	if(resPos == RES_SYS_DEF) resPos = getDefResPos();
	switch(resPos)
	{
	case RES_LOCAL_PACK:
		if(!loadFromPacker(filename,resPos)) return false;
		break;
	case RES_LOCAL_FOLDER:
		if(!loadFromFolder(filename,resPos)) return false;
		break;
	case RES_WEB:
		if(!loadFromWeb(filename,resPos)) return false;
		break;
	case RES_AUTO:
		if(!loadFromPacker(filename,resPos) && !loadFromFolder(filename,resPos) &&
			!loadFromWeb(filename,resPos)) return false;
		break;
	}
	m_isInited = XTrue;
	return XTrue;
}
void XDirectoryList::ctrlProc(void *pClass,int id,int eventID)
{
	XDirectoryList &pPar = *(XDirectoryList *)pClass;
	if(id == pPar.m_button.getControlID())
	{
		if(eventID != XButton::BTN_MOUSE_UP) return;
	//	printf("输入字符串为：%s\n",pPar.m_edit.getString());
		//根据用户的输入刷新列表
		//方案1：如果路径相同则不刷新数据，这样的话会造成无法刷新数据
		//if(pPar.m_dir.m_curDirectory.directoryName == NULL
		//	|| !fileNameCompare(pPar.m_dir.m_curDirectory.directoryName,pPar.m_edit.getString()))
		//{
		//	pPar.m_dir.release();
		//	pPar.m_dir.init(pPar.m_edit.getString());
		//}else
		//{
		//	return;	//相同则不更新信息
		//}
		//方案2：不论路径是否相同都刷新数据
		pPar.m_dir.release();
		pPar.m_dir.init(pPar.m_edit.getString());

		//初始化所有相应的显示信息
		unsigned int size = pPar.m_lineData.size();
		for(unsigned int i = 0;i < size;++ i)
		{
			pPar.m_lineData[i]->release();
			XMem::XDELETE(pPar.m_lineData[i]);
		}
		pPar.m_lineData.clear();
		pPar.m_showStartLine = 0;
		pPar.m_curLineLeft = 0;
		//下面定义所有显示的行数信息
		XDirListOneLine * tempLine = NULL;
		pPar.m_curLineSum = pPar.m_dir.m_curDirectory.files.size();
		for(int i = 0;i < pPar.m_curLineSum;++ i)
		{
			tempLine = XMem::createMem<XDirListOneLine>();
			tempLine->m_font.setACopy(pPar.m_caption);
			tempLine->m_font.setColor(pPar.m_color);
	#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(tempLine->m_font));
	#endif
			if(pPar.m_dir.m_curDirectory.files[i]->isDirectory)
			{//是目录
				tempLine->m_check = XMem::createMem<XCheck>();
				tempLine->m_check->setACopy(pPar.m_check);
				tempLine->m_check->setColor(pPar.m_color);
				tempLine->m_needCheck = XTrue;
		//		tempLine->m_check->setPosition(pPar.m_position + XVec2(0.0f,42.0f + pPar.m_curTextHeight * i));
				XCtrlManager.decreaseAObject(tempLine->m_check);
				tempLine->m_check->setEventProc(ctrlProc,&pPar);
	#if WITH_OBJECT_MANAGER
				XObjManager.decreaseAObject(tempLine->m_check);
	#endif
			}else
			{
				tempLine->m_needCheck = XFalse;
			}
		//	tempLine->m_pos.set(pPar.m_position + XVec2(0.0f,42.0f + pPar.m_curTextHeight * i));
		//	tempLine->m_font.setPosition(tempLine->m_pos);
			tempLine->m_font.setScale(pPar.m_fontSize * pPar.m_scale);
			tempLine->m_string = XMem::createArrayMem<char>(MAX_FILE_NAME_LENGTH);
			sprintf_s(tempLine->m_string,MAX_FILE_NAME_LENGTH,"|-%s",pPar.m_dir.m_curDirectory.files[i]->filename.c_str());
			tempLine->m_font.setString(tempLine->m_string);
			tempLine->m_isEnable = XTrue;
			tempLine->m_file = pPar.m_dir.m_curDirectory.files[i];
			pPar.m_lineData.push_back(tempLine);
		}
		if(pPar.m_curLineSum > pPar.m_canShowLineSum) 
		{
			pPar.m_needShowVSlider = XTrue;
			pPar.m_verticalSlider.setCurValue(0.0f, true);
			pPar.m_verticalSlider.setRange(pPar.m_curLineSum - pPar.m_canShowLineSum,0.0f);
		}else 
		{
			pPar.m_needShowVSlider = XFalse;
			pPar.m_showStartLine = 0;	//初始化数据
			pPar.m_verticalSlider.setCurValue(0.0f, true);
		}
		pPar.updateHSliderState();
		//更新内容之后取消选择设置
		pPar.m_haveSelect = XFalse;
		return;
	}
	if(id == pPar.m_verticalSlider.getControlID())
	{
		if(eventID == XSlider::SLD_MOUSE_MOVE || eventID == XSlider::SLD_VALUE_CHANGE)
		{
			pPar.m_showStartLine = pPar.m_verticalSlider.getCurValue();
			pPar.updateShowPosition();//更新位置
		}
		return;
	}
	if(id == pPar.m_horizontalSlider.getControlID())
	{
		if(eventID == XSlider::SLD_MOUSE_MOVE || eventID == XSlider::SLD_VALUE_CHANGE)
		{
			pPar.m_curLineLeft = (int)(pPar.m_horizontalSlider.getCurValue()) * pPar.m_curTextWidth * 2.0f;
			pPar.updateShowPosition();//更新位置
		}
		return;
	}
	if(eventID == XCheck::CHK_STATE_CHANGE)
	{
		int size = 0;
		if(pPar.m_curLineSum - pPar.m_showStartLine > pPar.m_canShowLineSum) size = pPar.m_canShowLineSum;
		else size = pPar.m_curLineSum - pPar.m_showStartLine;
		XDirListOneLine *tempLineData = NULL;
		for(int i = 0;i < size;++ i)
		{
			tempLineData = pPar.m_lineData[i + pPar.m_showStartLine];
			if(tempLineData->m_needCheck && tempLineData->m_check->getControlID() == id)
			{//确认点击之后，这里需要展开内部结构
				if(tempLineData->m_check->getState())
				{//展开
				//	printf("展开%s\n",tempLineData->m_string);
					int insertSum = tempLineData->m_file->directory->files.size();	//要插入的数量
					int sum = pPar.m_curLineSum - i - pPar.m_showStartLine - 1;
					std::vector<XDirListOneLine *> tempBuff;		//用于保存临时的数据
					for(int j = 0;j < sum;++ j)
					{//保存临时数据
						tempBuff.push_back(pPar.m_lineData[pPar.m_curLineSum - 1 - j]);
						pPar.m_lineData.pop_back();
					}	
					//插入新数据
					XDirListOneLine * tempLine = NULL;
					//XVec2 pos = tempLineData->m_pos;
					for(int j = 0;j < insertSum;++ j)
					{
						tempLine = XMem::createMem<XDirListOneLine>();
						tempLine->m_font.setACopy(pPar.m_caption);
						tempLine->m_font.setColor(pPar.m_color);
	#if WITH_OBJECT_MANAGER
						XObjManager.decreaseAObject(&(tempLine->m_font));
	#endif
					//	tempLine->m_pos.set(pos.x,pos.y + pPar.m_curTextHeight * (j + 1));
						if(tempLineData->m_file->directory->files[j]->isDirectory)
						{//是目录
							tempLine->m_check = XMem::createMem<XCheck>();
							tempLine->m_check->setACopy(pPar.m_check);
							tempLine->m_check->setColor(pPar.m_color);
							tempLine->m_needCheck = XTrue;
					//		tempLine->m_check->setPosition(pos.x + (float)(tempLineData->m_file->directory->level) * pPar.m_curTextWidth * 2.0f,
					//			tempLine->m_pos.y);
							XCtrlManager.decreaseAObject(tempLine->m_check);
							tempLine->m_check->setEventProc(ctrlProc,&pPar);
	#if WITH_OBJECT_MANAGER
							XObjManager.decreaseAObject(tempLine->m_check);
	#endif
						}else
						{
							tempLine->m_needCheck = XFalse;
						}
					//	tempLine->m_font.setPosition(tempLine->m_pos);
						tempLine->m_font.setScale(pPar.m_fontSize * pPar.m_scale);
						tempLine->m_string = XMem::createArrayMem<char>(MAX_FILE_NAME_LENGTH);
						char tempStr[MAX_FILE_NAME_LENGTH] = "";
						for(int k = 0;k < tempLineData->m_file->directory->level;++ k)
						{
							memcpy(tempStr + (k << 1), &"|,", 3);
							//tempStr[2 * k] = '|';
							//tempStr[2 * k + 1] = ' ';
							//tempStr[2 * k + 2] = '\0';
						}
						sprintf_s(tempLine->m_string,MAX_FILE_NAME_LENGTH,"%s|-%s",tempStr,tempLineData->m_file->directory->files[j]->filename.c_str());
						tempLine->m_font.setString(tempLine->m_string);
						tempLine->m_isEnable = XTrue;
						tempLine->m_file = tempLineData->m_file->directory->files[j];
						pPar.m_lineData.push_back(tempLine);
					}
					//插入老数据
					for(int j = 0;j < sum;++ j)
					{
				//		tempBuff[sum - 1 - j]->m_pos.set(pos.x,pos.y + pPar.m_curTextHeight * (j + insertSum + 1));
				//		if(tempBuff[sum - 1 - j]->m_needCheck) tempBuff[sum - 1 - j]->m_check->setPosition(tempBuff[sum - 1 - j]->m_check->getPosition().x,
				//			tempBuff[sum - 1 - j]->m_pos.y);
				//		tempBuff[sum - 1 - j]->m_font.setPosition(tempBuff[sum - 1 - j]->m_pos);
						pPar.m_lineData.push_back(tempBuff[sum - 1 - j]);
						tempBuff.pop_back();
					}
					pPar.m_curLineSum += insertSum;
					pPar.m_verticalSlider.setRange(pPar.m_curLineSum - pPar.m_canShowLineSum,0.0f);
					if(pPar.m_curLineSum > pPar.m_canShowLineSum) pPar.m_needShowVSlider = XTrue;
					else 
					{
						pPar.m_needShowVSlider = XFalse;
						pPar.m_showStartLine = 0;	//初始化数据
						pPar.m_verticalSlider.setCurValue(0.0f, true);
					}
					pPar.updateHSliderState();
				}else
				{//折叠
					pPar.furlFolder(i + pPar.m_showStartLine);
				}
				break;
			}
		}
	}
}
XBool XDirectoryList::init(const XVec2& position,
	const XRect& Area,
	XDirListSkin &tex,
	const XFontUnicode& font,
	float fontSize,
	const XCheck &check,
	const XButton &button,
	const XEdit &edit,
	const XSlider &vSlider,	//垂直滑动条
	const XSlider &hSlider)
{
	if(m_isInited) return XTrue;
	if(tex.dirListNormal == NULL || tex.dirListDisable == NULL) return XFalse;
	if(fontSize <= 0) return XFalse;
	m_position = position;
	m_dirListNormal = tex.dirListNormal;
	m_dirListDisable = tex.dirListDisable;
	m_spriteBackGround.init(m_dirListNormal->texture.m_w,m_dirListNormal->texture.m_h,1);
	m_spriteBackGround.setIsTransformCenter(POINT_LEFT_TOP);

	m_needShowVSlider = XFalse;			
	m_verticalSlider.setACopy(vSlider);	
	m_needShowHSlider = XFalse;		
	m_horizontalSlider.setACopy(hSlider);
	m_check.setACopy(check);
	m_button.setACopy(button);
	m_edit.setACopy(edit);
	m_withoutTex = XFalse;
	//设置回调函数
	m_button.setEventProc(ctrlProc,this);
	m_verticalSlider.setEventProc(ctrlProc,this);
	m_horizontalSlider.setEventProc(ctrlProc,this);

	if(!m_caption.setACopy(font)) return XFalse;
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);	 //设置字体上对齐
	m_textColor.set(0.0f);
	m_caption.setColor(m_textColor);
	m_fontSize.set(fontSize);
	m_caption.setScale(m_fontSize);
	m_curTextWidth = m_caption.getTextSize().x * m_caption.getScale().x * 0.5f;
	m_curTextHeight = m_caption.getTextSize().y * m_caption.getScale().y;

	m_scale.set(1.0f);
	//m_mouseRect.set(1.0f,1.0f,351.0f,361.0f);
	m_mouseRect = Area;
	m_showPixWidth = m_mouseRect.getWidth() * m_scale.x;
	m_showPixHight = m_mouseRect.getHeight() * m_scale.y;

	m_haveSelect = XFalse;
	m_selectLineOrder = 0;
	m_showStartLine = 0;
	m_canShowLineSum = m_showPixHight/m_curTextHeight;

	m_curLineLeft = 0;
	m_maxLineWidth = 0;
	m_canShowMaxLineWidth = m_mouseRect.getWidth();

	XCtrlManager.decreaseAObject(&m_check);
	XCtrlManager.decreaseAObject(&m_verticalSlider);
	XCtrlManager.decreaseAObject(&m_horizontalSlider);
	XCtrlManager.decreaseAObject(&m_button);
	XCtrlManager.decreaseAObject(&m_edit);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteBackGround);
	XObjManager.decreaseAObject(&m_verticalSlider);
	XObjManager.decreaseAObject(&m_horizontalSlider);
	XObjManager.decreaseAObject(&m_check);
	XObjManager.decreaseAObject(&m_button);
	XObjManager.decreaseAObject(&m_edit);
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_spriteBackGround.setPosition(m_position + XVec2(0.0f,m_edit.getMouseRect().getHeight()));
	m_edit.setPosition(m_position);
	char tempDirectoryName[MAX_FILE_NAME_LENGTH];
	GetCurrentDirectory(MAX_FILE_NAME_LENGTH,tempDirectoryName);	//获取当前路径
	m_edit.setString(tempDirectoryName);//初始化为当前路径

	m_button.setPosition(m_position + XVec2(m_mouseRect.getWidth(),0.0f) * m_scale);
	m_verticalSlider.setPosition(m_position + XVec2(m_mouseRect.getWidth(),m_edit.getMouseRect().getHeight()));
	m_horizontalSlider.setPosition(m_position + XVec2(0.0f,m_edit.getMouseRect().getHeight() + m_mouseRect.getHeight()));

	m_isVisible = m_isEnable = m_isActive = XTrue;
	setPosition(position);

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
XBool XDirectoryList::initPlus(const char * path,
		const XFontUnicode& font,
		float fontSize,
		XResPos resPos)
{
	if(m_isInited ||
		path == NULL) return XFalse;
	m_resInfo = XResManager.loadResource(path,RESOURCE_TYPEXDIRLIST_TEX,resPos);
	if(m_resInfo == NULL) return XFalse;
	XDirListSkin * tex = (XDirListSkin *)m_resInfo->m_pointer;
	if(tex->dirListNormal == NULL || tex->dirListDisable == NULL) return XFalse;
	if(fontSize <= 0) return XFalse;
	m_position.reset();
	m_dirListNormal = tex->dirListNormal;
	m_dirListDisable = tex->dirListDisable;
	m_spriteBackGround.init(m_dirListNormal->texture.m_w,m_dirListNormal->texture.m_h,1);
	m_spriteBackGround.setIsTransformCenter(POINT_LEFT_TOP);

	m_needShowVSlider = XFalse;			
	char tempPath[MAX_FILE_NAME_LENGTH];
	sprintf_s(tempPath,MAX_FILE_NAME_LENGTH,"%s/SliderV",path);
	m_verticalSlider.initPlus(tempPath,100.0f,0.0f,SLIDER_TYPE_VERTICAL,resPos);
	m_needShowHSlider = XFalse;		
	sprintf_s(tempPath,MAX_FILE_NAME_LENGTH,"%s/SliderH",path);
	m_horizontalSlider.initPlus(tempPath,100.0f,0.0f,SLIDER_TYPE_HORIZONTAL,resPos);
	sprintf_s(tempPath,MAX_FILE_NAME_LENGTH,"%s/Check",path);
	m_check.initPlus(tempPath," ",font,1.0f,resPos);
	sprintf_s(tempPath,MAX_FILE_NAME_LENGTH,"%s/Button",path);
	m_button.initPlus(tempPath," ",font,1.0f,resPos);
	sprintf_s(tempPath,MAX_FILE_NAME_LENGTH,"%s/Edit",path);
	m_edit.initPlus(tempPath," ",font,1.0f,NULL,resPos);
	m_withoutTex = XFalse;
	//设置回调函数
	m_button.setEventProc(ctrlProc,this);
	m_verticalSlider.setEventProc(ctrlProc,this);
	m_horizontalSlider.setEventProc(ctrlProc,this);

	if(!m_caption.setACopy(font)) return XFalse;
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);	 //设置字体上对齐
	m_textColor.set(0.0f,1.0f);
	m_caption.setColor(m_textColor);
	m_fontSize.set(fontSize);
	m_caption.setScale(m_fontSize);
	m_curTextWidth = m_caption.getTextSize().x * m_caption.getScale().x * 0.5f;
	m_curTextHeight = m_caption.getTextSize().y * m_caption.getScale().y;

	m_scale.set(1.0f);
	m_mouseRect = tex->m_mouseRect;
	m_showPixWidth = m_mouseRect.getWidth() * m_scale.x;
	m_showPixHight = m_mouseRect.getHeight() * m_scale.y;

	m_haveSelect = XFalse;
	m_selectLineOrder = 0;
	m_showStartLine = 0;
	m_canShowLineSum = m_showPixHight/m_curTextHeight;

	m_curLineLeft = 0;
	m_maxLineWidth = 0;
	m_canShowMaxLineWidth = m_mouseRect.getWidth();

	XCtrlManager.decreaseAObject(&m_check);
	XCtrlManager.decreaseAObject(&m_verticalSlider);
	XCtrlManager.decreaseAObject(&m_horizontalSlider);
	XCtrlManager.decreaseAObject(&m_button);
	XCtrlManager.decreaseAObject(&m_edit);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteBackGround);
	XObjManager.decreaseAObject(&m_verticalSlider);
	XObjManager.decreaseAObject(&m_horizontalSlider);
	XObjManager.decreaseAObject(&m_check);
	XObjManager.decreaseAObject(&m_button);
	XObjManager.decreaseAObject(&m_edit);
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_spriteBackGround.setPosition(m_position + XVec2(0.0f,m_edit.getMouseRect().getHeight()));
	m_edit.setPosition(m_position);
	char tempDirectoryName[MAX_FILE_NAME_LENGTH];
	GetCurrentDirectory(MAX_FILE_NAME_LENGTH,tempDirectoryName);	//获取当前路径
	m_edit.setString(tempDirectoryName);//初始化为当前路径

	m_button.setPosition(m_position + XVec2(m_mouseRect.getWidth(),0.0f) * m_scale);
	m_verticalSlider.setPosition(m_position + XVec2(m_mouseRect.getWidth(),m_edit.getMouseRect().getHeight()));
	m_horizontalSlider.setPosition(m_position + XVec2(0.0f,m_edit.getMouseRect().getHeight() + m_mouseRect.getHeight()));

	m_isVisible = m_isEnable = m_isActive = XTrue;
	setPosition(XVec2::zero);

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
XBool XDirectoryList::initWithoutSkin(const XRect& area,	
	const XFontUnicode& font,
	float fontSize)
{
	if(m_isInited) return XTrue;
	if(fontSize <= 0) return XFalse;
	m_position.reset();

	m_mouseRect = area;

	m_needShowVSlider = XFalse;			
//	m_verticalSlider.initWithoutSkin(XRect(0,0,DEFAULT_SLIDER_WIDTH,m_mouseRect.getHeight()),
//		XRect(0,DEFAULT_SLIDER_WIDTH),SLIDER_TYPE_VERTICAL,100.0f,0.0f,XVec2::zero);
	m_verticalSlider.initWithoutSkin(XRect(0,0,DEFAULT_SLIDER_WIDTH,m_mouseRect.getHeight()),100.0f,0.0f,SLIDER_TYPE_VERTICAL);
	m_needShowHSlider = XFalse;		
//	m_horizontalSlider.initWithoutSkin(XRect(0,0,m_mouseRect.getWidth(),DEFAULT_SLIDER_WIDTH),
//		XRect(0,DEFAULT_SLIDER_WIDTH),SLIDER_TYPE_HORIZONTAL,100.0f,0.0f,XVec2::zero);
	m_horizontalSlider.initWithoutSkin(XRect(0,0,m_mouseRect.getWidth(),DEFAULT_SLIDER_WIDTH));
	m_check.initWithoutSkin(" ",font,0.5f,XRect(0,DEFAULT_DIRLIST_CK_SIZE),
		XVec2(DEFAULT_DIRLIST_CK_SIZE));
	m_button.initWithoutSkin(" ",font,1.0f,XRect(0,0,DEFAULT_SLIDER_WIDTH,DEFAULT_DIRLIST_BT_SIZE),
		XVec2((int)(DEFAULT_SLIDER_WIDTH) >> 1,(int)(DEFAULT_DIRLIST_BT_SIZE) >> 1));
	m_edit.initWithoutSkin(XRect(0,0,m_mouseRect.getWidth(),DEFAULT_DIRLIST_BT_SIZE)," ",font,1.0f,NULL);
	m_withoutTex = XTrue;
	//设置回调函数
	m_button.setEventProc(ctrlProc,this);
	m_verticalSlider.setEventProc(ctrlProc,this);
	m_horizontalSlider.setEventProc(ctrlProc,this);
	m_verticalSlider.setWithAction(false);
	m_horizontalSlider.setWithAction(false);

	if(!m_caption.setACopy(font)) return XFalse;
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);	 //设置字体上对齐
	m_textColor.set(0.0f,1.0f);
	m_caption.setColor(m_textColor);
	m_fontSize.set(fontSize);
	m_caption.setScale(m_fontSize);
	m_curTextWidth = m_caption.getTextSize().x * m_caption.getScale().x * 0.5f;
	m_curTextHeight = m_caption.getTextSize().y * m_caption.getScale().y;

	m_scale.set(1.0f);
	m_showPixWidth = m_mouseRect.getWidth() * m_scale.x;
	m_showPixHight = m_mouseRect.getHeight() * m_scale.y;

	m_haveSelect = XFalse;
	m_selectLineOrder = 0;
	m_showStartLine = 0;
	m_canShowLineSum = m_showPixHight/m_curTextHeight;

	m_curLineLeft = 0;
	m_maxLineWidth = 0;
	m_canShowMaxLineWidth = m_mouseRect.getWidth();

	XCtrlManager.decreaseAObject(&m_check);
	XCtrlManager.decreaseAObject(&m_verticalSlider);
	XCtrlManager.decreaseAObject(&m_horizontalSlider);
	XCtrlManager.decreaseAObject(&m_button);
	XCtrlManager.decreaseAObject(&m_edit);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteBackGround);
	XObjManager.decreaseAObject(&m_verticalSlider);
	XObjManager.decreaseAObject(&m_horizontalSlider);
	XObjManager.decreaseAObject(&m_check);
	XObjManager.decreaseAObject(&m_button);
	XObjManager.decreaseAObject(&m_edit);
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_edit.setPosition(m_position);
	char tempDirectoryName[MAX_FILE_NAME_LENGTH];
	GetCurrentDirectory(MAX_FILE_NAME_LENGTH,tempDirectoryName);	//获取当前路径
	m_edit.setString(tempDirectoryName);//初始化为当前路径

	m_button.setPosition(m_position + XVec2(m_mouseRect.getWidth(),0.0f) * m_scale);
	m_verticalSlider.setPosition(m_position + XVec2(m_mouseRect.getWidth(),m_edit.getMouseRect().getHeight()));
	m_horizontalSlider.setPosition(m_position + XVec2(0.0f,m_edit.getMouseRect().getHeight() + m_mouseRect.getHeight()));

	m_isVisible = m_isEnable = m_isActive = XTrue;
	setPosition(XVec2::zero);

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
void XDirectoryList::draw()
{
	if (!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出

	if (m_withoutTex)
	{
		if (!m_isEnable)
		{
			XRender::drawFillRectExA(m_position + XVec2(m_mouseRect.left, m_mouseRect.top + m_edit.getMouseRectHeight()) * m_scale,
				m_mouseRect.getSize() * m_scale, XCCS::blackOnColor * m_color);
			XRender::drawFillRectExA(m_position + XVec2(m_mouseRect.right, m_mouseRect.top + m_edit.getMouseRectHeight()) * m_scale,
				m_verticalSlider.getMouseRect().getSize() * m_scale, XCCS::downColor * m_color);
			XRender::drawFillRectExA(m_position + XVec2(m_mouseRect.left, m_mouseRect.bottom + m_edit.getMouseRectHeight()) * m_scale,
				m_horizontalSlider.getMouseRect().getSize() * m_scale, XCCS::downColor * m_color);
			XRender::drawFillRectExA(m_position + XVec2(m_mouseRect.right, m_mouseRect.bottom + m_edit.getMouseRectHeight()) * m_scale,
				XVec2(m_verticalSlider.getMouseRectWidth(),
					m_horizontalSlider.getMouseRectHeight()) * m_scale, XCCS::lightBlackColor * m_color);
		}
		else
		{
			XRender::drawFillRectExA(m_position + XVec2(m_mouseRect.left, m_mouseRect.top + m_edit.getMouseRectHeight()) * m_scale,
				m_mouseRect.getSize() * m_scale, XCCS::specialColor * m_color);
			XRender::drawFillRectExA(m_position + XVec2(m_mouseRect.right, m_mouseRect.top + m_edit.getMouseRectHeight()) * m_scale,
				m_verticalSlider.getMouseRect().getSize() * m_scale, XCCS::lightSpecialColor * m_color);
			XRender::drawFillRectExA(m_position + XVec2(m_mouseRect.left, m_mouseRect.bottom + m_edit.getMouseRectHeight()) * m_scale,
				m_horizontalSlider.getMouseRect().getSize() * m_scale, XCCS::lightSpecialColor * m_color);
			XRender::drawFillRectExA(m_position + XVec2(m_mouseRect.right, m_mouseRect.bottom + m_edit.getMouseRectHeight()) * m_scale,
				XVec2(m_verticalSlider.getMouseRectWidth(),
					m_horizontalSlider.getMouseRectHeight()) * m_scale, XCCS::lightMouseColor * m_color);
		}
	}
	else
	{
		if (!m_isEnable) m_spriteBackGround.draw(m_dirListDisable);
		else m_spriteBackGround.draw(m_dirListNormal);
	}
	//显示选择的状态
	if (m_haveSelect &&
		m_selectLineOrder >= m_showStartLine && m_selectLineOrder < m_showStartLine + m_canShowLineSum)
	{//需要显示
		XRender::drawRect(m_position + XVec2(m_mouseRect.getXCenter(),
			m_mouseRect.top + m_edit.getMouseRect().getHeight() + (m_selectLineOrder - m_showStartLine + 0.5f) * m_curTextHeight) * m_scale,	//42.0f为输入框的高
			XVec2(m_mouseRect.getWidth(), m_curTextHeight) * m_scale * 0.5f, 0.5f, XCCS::darkColor * m_color);
	}
	if (m_needShowVSlider) m_verticalSlider.draw();
	if (m_needShowHSlider) m_horizontalSlider.draw();
	m_button.draw();
	m_edit.draw();
	//显示所有行的信息
	int lineSum = 0;
	if (m_curLineSum - m_showStartLine >= m_canShowLineSum) lineSum = m_canShowLineSum;
	else lineSum = m_curLineSum - m_showStartLine;
	XDirListOneLine * tempLine = NULL;
	for (int i = 0; i < lineSum; ++i)
	{
		tempLine = m_lineData[i + m_showStartLine];
		tempLine->m_font.draw();
		if (tempLine->m_needCheck && tempLine->m_check != NULL)
		{
			if (tempLine->m_check->getPosition().x >= m_position.x
				&& tempLine->m_check->getPosition().x < m_position.x + m_canShowMaxLineWidth - tempLine->m_check->getMouseRect().getWidth())
				tempLine->m_check->draw();
		}
	}
}
void XDirectoryList::drawUp()
{
	if (!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	if (m_needShowVSlider) m_verticalSlider.drawUp();
	if (m_needShowHSlider) m_horizontalSlider.drawUp();
	m_button.drawUp();
	m_edit.drawUp();
	//显示所有行的信息
	int lineSum = 0;
	if (m_curLineSum - m_showStartLine >= m_canShowLineSum) lineSum = m_canShowLineSum;
	else lineSum = m_curLineSum - m_showStartLine;
	XDirListOneLine * tempLine = NULL;
	for (int i = 0; i < lineSum; ++i)
	{
		tempLine = m_lineData[i + m_showStartLine];
		if (tempLine->m_needCheck && tempLine->m_check != NULL)
		{
			if (tempLine->m_check->getPosition().x >= m_position.x
				&& tempLine->m_check->getPosition().x < m_position.x + m_canShowMaxLineWidth - tempLine->m_check->getMouseRect().getWidth())
				tempLine->m_check->drawUp();
		}
	}
}
void XDirectoryList::update(float stepTime)
{
	if (m_needShowVSlider) m_verticalSlider.update(stepTime);
	if (m_needShowHSlider) m_horizontalSlider.update(stepTime);
	m_button.update(stepTime);
	m_edit.update(stepTime);
	//显示所有行的信息
	int lineSum = 0;
	if (m_curLineSum - m_showStartLine >= m_canShowLineSum) lineSum = m_canShowLineSum;
	else lineSum = m_curLineSum - m_showStartLine;
	XDirListOneLine * tempLine = NULL;
	for (int i = 0; i < lineSum; ++i)
	{
		tempLine = m_lineData[i + m_showStartLine];
		if (tempLine->m_needCheck && tempLine->m_check != NULL)
		{
			if (tempLine->m_check->getPosition().x >= m_position.x
				&& tempLine->m_check->getPosition().x < m_position.x + m_canShowMaxLineWidth - tempLine->m_check->getMouseRect().getWidth())
				tempLine->m_check->update(stepTime);
		}
	}
	//鼠标双击计时
	m_mouseTime += stepTime;
}
#pragma comment(lib, "shell32.lib")	//for ShellExecute()!
XBool XDirectoryList::mouseProc(const XVec2& p,XMouseState mouseState)
{
	if(!m_isInited || 	//如果没有初始化直接退出
		!m_isVisible) return XTrue;	//如果不可见直接退出
	if(m_isSilent) return XFalse;
	if(m_needShowVSlider) m_verticalSlider.mouseProc(p,mouseState);
	if(m_needShowHSlider) m_horizontalSlider.mouseProc(p,mouseState);
	m_edit.mouseProc(p,mouseState);
	m_button.mouseProc(p,mouseState);
	//所有文件夹的点接受鼠标控制
	int lineSum = 0;
	if(m_curLineSum - m_showStartLine >= m_canShowLineSum) lineSum = m_canShowLineSum;
	else lineSum = m_curLineSum - m_showStartLine;
	XDirListOneLine * tempLine = NULL;
	for(int i = m_showStartLine;i < lineSum + m_showStartLine;++ i)
	{
		tempLine = m_lineData[i];
		if(tempLine->m_needCheck && tempLine->m_check != NULL
			&& tempLine->m_check->getPosition().x >= m_position.x
			&& tempLine->m_check->getPosition().x < m_position.x + m_canShowMaxLineWidth - tempLine->m_check->getMouseRect().getWidth()) 
		{
			if(tempLine->m_check->mouseProc(p,mouseState))
			{//如果鼠标动作产生了变化，则在这里更新状态信息
				if(m_curLineSum - m_showStartLine >= m_canShowLineSum) lineSum = m_canShowLineSum;
				else lineSum = m_curLineSum - m_showStartLine;
			}
		}
	}
	//下面计算点选的情况
	if(mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_LEFT_BUTTON_DCLICK)
	{//计算点击是否在范围内
		XRect tempRect;
		tempRect.set(m_mouseRect.left * m_scale.x + m_position.x,
			(m_mouseRect.top + m_edit.getMouseRect().getHeight()) * m_scale.y + m_position.y,
			m_mouseRect.right * m_scale.x + m_position.x,
			(m_mouseRect.bottom + m_edit.getMouseRect().getHeight()) * m_scale.y + m_position.y);
		if(tempRect.isInRect(p))
		{//处于点击范围之内
			int lineOrder = (p.y - (m_mouseRect.top + m_edit.getMouseRect().getHeight()) * m_scale.y - m_position.y) / (m_curTextHeight * m_scale.y);
			if(lineOrder >= 0 && lineOrder < m_canShowLineSum)
			{
				if(lineOrder + m_showStartLine < m_curLineSum)
				{
					m_haveSelect = XTrue;
					m_selectLineOrder = lineOrder + m_showStartLine;
				}
			}
			m_isBeChoose = XTrue;
		}
		if(m_mouseTime < 500)
		{//双击打开指定文件
			//printf("双击事件\n");
			//char tempStr[MAX_FILE_NAME_LENGTH];
			//sprintf_s(tempStr,MAX_FILE_NAME_LENGTH,"explorer %s",getSelectFileName());
			//system(tempStr);
			if(m_haveSelect)
			{
				if(m_lineData[m_selectLineOrder]->m_needCheck)
				{//打开文件夹
					if(m_lineData[m_selectLineOrder]->m_check->getState()) m_lineData[m_selectLineOrder]->m_check->setState(XFalse);
					else m_lineData[m_selectLineOrder]->m_check->setState(XTrue);
					ctrlProc(this,m_lineData[m_selectLineOrder]->m_check->getControlID(),XCheck::CHK_STATE_CHANGE);
				}else
				{
					ShellExecute(NULL,"open",m_lineData[m_selectLineOrder]->m_file->allPath.c_str(),NULL,NULL,SW_SHOW);
				}
			}
			m_mouseTime = 1000;
		}else
		{
			m_mouseTime = 0;
		}
	}else
	if(mouseState == MOUSE_MOVE || mouseState == MOUSE_RIGHT_BUTTON_DOWN || mouseState == MOUSE_MIDDLE_BUTTON_DOWN)
	{
		m_mouseTime = 1000;
	}
	return XTrue;
}
void XDirectoryList::furlFolder(int index,XBool flag)
{//需要遍历其下的所有文件夹
	//注意这里没有进行输入变量合法性价差，内部调用函数，需要保证函数的输入参数正确
	if(!m_lineData[index]->m_needCheck) return;			//如果不是文件夹直接返回
	if((flag && m_lineData[index]->m_check->getState())
		|| (!flag && !m_lineData[index]->m_check->getState())) return;	//已经是折叠好的
	int insert = m_lineData[index]->m_file->directory->files.size();
	//折叠所有子文件夹
	for(int i = 0;i < insert;++ i)
	{
		furlFolder(index + i + 1,XFalse);
	}
	//折叠自身
	XDirListOneLine *tempLineData = m_lineData[index];
//	printf("折叠%s\n",tempLineData->m_string);
	int insertSum = tempLineData->m_file->directory->files.size();	//要插入的数量
	int sum = m_curLineSum - index - insertSum - 1;
	for(int j = 0;j < insertSum;++ j)
	{//释放多余的资源
		m_lineData[j + index + 1]->release();
	}
	//移动
	//XVec2 pos = tempLineData->m_pos;
	int tmpIndex = index + 1;
	for(int j = 0;j < sum;++ j,++tmpIndex)
	{
		m_lineData[tmpIndex] = m_lineData[tmpIndex + insertSum];
		tempLineData = m_lineData[tmpIndex];
		//改变位置
	//	tempLineData->m_pos.set(pos.x,pos.y + m_curTextHeight * (j + 1));
	//	tempLineData->m_font.setPosition(tempLineData->m_pos);
	//	if(tempLineData->m_needCheck)
	//	{
	//		tempLineData->m_check->setPosition(tempLineData->m_check->getPosition().x,pos.y);
	//	}
	}
	//删除结尾的
	for(int j = 0;j < insertSum;++ j)
	{
		m_lineData.pop_back();
	}
	m_curLineSum -= insertSum;
	m_verticalSlider.setRange(m_curLineSum - m_canShowLineSum,0.0f);
	if(m_curLineSum > m_canShowLineSum) m_needShowVSlider = XTrue;
	else 
	{
		m_needShowVSlider = XFalse;
		m_showStartLine = 0;	//初始化数据
		m_verticalSlider.setCurValue(0.0f, true);
	}
	updateHSliderState();
}
void XDirectoryList::updateHSliderState()
{
	//遍历所有的条目，寻找到长的条目，然后进行裁剪或者复位
	m_maxLineWidth = 0;				//当前的最大行宽[像素]

	for(unsigned int i = 0;i < m_lineData.size();++ i)
	{
		m_lineData[i]->m_font.disClip();
		if(m_lineData[i]->m_font.getMaxPixelWidth() > m_maxLineWidth)
		{
			m_maxLineWidth = m_lineData[i]->m_font.getMaxPixelWidth();	//寻找最宽的
		}
	}
	if(m_maxLineWidth > m_canShowMaxLineWidth * m_scale.x)
	{//宽度越界
		if(!m_needShowHSlider)
		{//尚未进行裁剪
			m_needShowHSlider = XTrue;
			m_curLineLeft = 0;
			m_horizontalSlider.setRange((m_maxLineWidth - m_canShowMaxLineWidth * m_scale.x)/(m_curTextWidth * m_scale.x * 2.0f) + 1.0f,0.0f);
			m_horizontalSlider.setCurValue(0.0f, true);
		}else
		{//已经进行各裁剪
			float tempValue = (m_maxLineWidth - m_canShowMaxLineWidth * m_scale.x)/(m_curTextWidth * m_scale.x * 2.0f) + 1.0f;
			if(m_horizontalSlider.getCurValue() >= tempValue)
			{
				m_horizontalSlider.setCurValue(tempValue, true);
			}
			m_horizontalSlider.setRange(tempValue,0.0f);
		}
	}else
	{//不越界没有必要进行裁剪
		if(m_needShowHSlider)
		{//取消所有裁剪设置
			m_needShowHSlider = XFalse;
			m_curLineLeft = 0;
			m_horizontalSlider.setCurValue(0.0f, true);
			for(unsigned int i = 0;i < m_lineData.size();++ i)
			{
				m_lineData[i]->m_font.disClip();	//取消所有裁剪
			}
		}
	}
}
void XDirectoryList::updateShowPosition()
{
	int size = 0;
	if(m_curLineSum - m_showStartLine > m_canShowLineSum) size = m_canShowLineSum;
	else size = m_curLineSum - m_showStartLine;
	XDirListOneLine * tempLine = NULL;
	for(int i = 0;i < size;++ i)
	{
		tempLine = m_lineData[i + m_showStartLine];
		tempLine->m_font.setScale(m_scale);
		tempLine->m_font.setClipRect(m_curLineLeft,0.0f,m_curLineLeft + m_canShowMaxLineWidth,32.0f);	//字体的高度
		tempLine->m_font.setPosition(m_position + XVec2(0.0f - m_curLineLeft,m_edit.getMouseRect().getHeight() + m_curTextHeight * i) * m_scale);
		if(tempLine->m_needCheck)
		{
			tempLine->m_check->setScale(m_scale);
			tempLine->m_check->setPosition(m_position.x + 
				((tempLine->m_file->directory->level - 1) * m_curTextWidth * 2.0f - m_curLineLeft) * m_scale.x,
				m_position.y + (m_edit.getMouseRect().getHeight() + m_curTextHeight * i) * m_scale.y);
		}
	}
}
void XDirectoryList::release()
{
	if(!m_isInited) return;
	//下面释放所有的资源
	for(int i = 0;i < m_curLineSum;++ i)
	{
		m_lineData[i]->release();
		XMem::XDELETE(m_lineData[i]);
	}
	m_lineData.clear();
	//m_lineData.swap(std::vector<XDirListOneLine *>());

	XCtrlManager.decreaseAObject(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	if(m_resInfo != NULL)
	{
		XResManager.releaseResource(m_resInfo);
		m_resInfo = NULL;
	}
	m_isInited = XFalse;
}
#if !WITH_INLINE_FILE
#include "XDirectoryList.inl"
#endif
}