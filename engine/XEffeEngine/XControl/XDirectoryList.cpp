//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XDirectoryList.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

_XDirListTexture::_XDirListTexture()
:m_isInited(XFalse)
,dirListNormal(NULL)
,dirListDisable(NULL)
{}
_XBool _XDirListTexture::init(const char *normal,const char *disable,_XResourcePosition resoursePosition)
{
	if(m_isInited ||
		normal == NULL || disable == NULL) return XFalse;
	
	if((dirListNormal = createATextureData(normal,resoursePosition)) == NULL) return XFalse;
	if((dirListDisable = createATextureData(disable,resoursePosition)) == NULL)
	{
		XDELETE(dirListNormal);
		return XFalse;
	}

	m_isInited = XTrue;
	return XTrue;
}
#define DIRLIST_CONFIG_FILENAME ("DirevtoryList.txt")
_XBool _XDirListTexture::initEx(const char *filename,_XResourcePosition resoursePosition)
{
	if(m_isInited ||
		filename == NULL) return XFalse;
	//下面从配置文件中读取数据
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf(tempFilename,"%s/%s",filename,DIRLIST_CONFIG_FILENAME);
	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = XEE::defaultResourcePosition;
	if(resoursePosition == RESOURCE_LOCAL_FOLDER)
	{//从文件中读取资源
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
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((dirListNormal = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((dirListDisable = createATextureData(tempFilename,resoursePosition)) == NULL)
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
	}else
	if(resoursePosition == RESOURCE_LOCAL_PACK)
	{
		unsigned char *p = _XResourcePack::GetInstance().getFileData(tempFilename);
		if(p == NULL) return XFalse;

		//下面开始依次读取数据
		int flag = 0;
		char resFilename[MAX_FILE_NAME_LENGTH] = "";
		int offset = 0;
		//normal
		if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag == 0)
		{//没有普通状态是不行的
			XDELETE_ARRAY(p);
			return XFalse;
		}
		if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((dirListNormal = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//资源读取失败
			XDELETE_ARRAY(p);
			return XFalse;
		}
		if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag == 0)
		{//没有普通状态是不行的
			XDELETE_ARRAY(p);
			return XFalse;
		}
		if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((dirListDisable = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//资源读取失败
			XDELETE_ARRAY(p);
			return XFalse;
		}
		//读取两组数据
		int l,t,r,b;
		if(sscanf((char *)(p + offset),"%d,%d,%d,%d,",&l,&t,&r,&b) != 4) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		m_mouseRect.set(l,t,r,b);
		//所有数据读取完成
		XDELETE_ARRAY(p);
	}
	m_isInited = XTrue;
	return XTrue;
}
void funDirListButton(void *pClass,int)
{
	_XDirectoryList &pPar = *(_XDirectoryList *)pClass;
//	printf("输入字符串为：%s\n",pPar.m_edit.getString());
	//根据用户的输入刷新列表
	//方案1：如果路径相同则不刷新数据，这样的话会造成无法刷新数据
	//if(pPar.m_dir.m_nowDirectory.directoryName == NULL
	//	|| !fileNameCompare(pPar.m_dir.m_nowDirectory.directoryName,pPar.m_edit.getString()))
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
	int size = pPar.m_lineData.size();
	for(int i = 0;i < size;++ i)
	{
		pPar.m_lineData[i]->release();
		XDELETE(pPar.m_lineData[i]);
	}
	pPar.m_lineData.clear();
	pPar.m_showStartLine = 0;
	pPar.m_nowLineLeft = 0;
	//下面定义所有显示的行数信息
	_XDirListOneLine * tempLine = NULL;
	pPar.m_nowLineSum = pPar.m_dir.m_nowDirectory.files.size();
	for(int i = 0;i < pPar.m_nowLineSum;++ i)
	{
		tempLine = createMem<_XDirListOneLine>();
		tempLine->m_font.setACopy(pPar.m_caption);
		tempLine->m_font.setColor(pPar.m_color);
#if WITH_OBJECT_MANAGER
		_XObjManger.decreaseAObject(&(tempLine->m_font));
#endif
		if(pPar.m_dir.m_nowDirectory.files[i]->isDirectory)
		{//是目录
			tempLine->m_check = createMem<_XCheck>();
			tempLine->m_check->setACopy(pPar.m_check);
			tempLine->m_check->setColor(pPar.m_color);
			tempLine->m_needCheck = XTrue;
	//		tempLine->m_check->setPosition(pPar.m_position + _XVector2(0.0f,42.0f + pPar.m_nowTextHeight * i));
			_XCtrlManger.decreaseAObject(tempLine->m_check);
			tempLine->m_check->setStateChangeCB(funDirListCheck,&pPar);
#if WITH_OBJECT_MANAGER
			_XObjManger.decreaseAObject(tempLine->m_check);
#endif
		}else
		{
			tempLine->m_needCheck = XFalse;
		}
	//	tempLine->m_pos.set(pPar.m_position + _XVector2(0.0f,42.0f + pPar.m_nowTextHeight * i));
	//	tempLine->m_font.setPosition(tempLine->m_pos);
		tempLine->m_font.setSize(pPar.m_fontSize * pPar.m_size);
		tempLine->m_string = createArrayMem<char>(MAX_FILE_NAME_LENGTH);
		sprintf(tempLine->m_string,"|-%s",pPar.m_dir.m_nowDirectory.files[i]->filename);
		tempLine->m_font.setString(tempLine->m_string);
		tempLine->m_isEnable = XTrue;
		tempLine->m_file = pPar.m_dir.m_nowDirectory.files[i];
		pPar.m_lineData.push_back(tempLine);
	}
	if(pPar.m_nowLineSum > pPar.m_canShowLineSum) 
	{
		pPar.m_needShowVSlider = XTrue;
		pPar.m_verticalSlider.setNowValue(0.0f);
		pPar.m_verticalSlider.setRange(pPar.m_nowLineSum - pPar.m_canShowLineSum,0.0f);
	}else 
	{
		pPar.m_needShowVSlider = XFalse;
		pPar.m_showStartLine = 0;	//初始化数据
		pPar.m_verticalSlider.setNowValue(0.0f);
	}
	pPar.updateHSliderState();
	//更新内容之后取消选择设置
	pPar.m_haveSelect = XFalse;
}
void funDirListValueChangeV(void *pClass,int)
{
	_XDirectoryList &pPar = *(_XDirectoryList *)pClass;
	pPar.m_showStartLine = pPar.m_verticalSlider.getNowValue();
	//更新位置
	pPar.updateShowPosition();
}
void funDirListValueChangeH(void *pClass,int)
{
	_XDirectoryList &pPar = *(_XDirectoryList *)pClass;
	pPar.m_nowLineLeft = (int)(pPar.m_horizontalSlider.getNowValue()) * pPar.m_nowTextWidth * 2.0f;
	//更新位置
	pPar.updateShowPosition();
}
void funDirListCheck(void *pClass,int objectID)
{
	_XDirectoryList &pPar = *(_XDirectoryList *)pClass;
	int size = 0;
	if(pPar.m_nowLineSum - pPar.m_showStartLine > pPar.m_canShowLineSum) size = pPar.m_canShowLineSum;
	else size = pPar.m_nowLineSum - pPar.m_showStartLine;
	for(int i = 0;i < size;++ i)
	{
		_XDirListOneLine *tempLineData = pPar.m_lineData[i + pPar.m_showStartLine];
		if(tempLineData->m_needCheck && tempLineData->m_check->getControlID() == objectID)
		{//确认点击之后，这里需要展开内部结构
			if(tempLineData->m_check->getState())
			{//展开
			//	printf("展开%s\n",tempLineData->m_string);
				int insertSum = tempLineData->m_file->directory->files.size();	//要插入的数量
				int sum = pPar.m_nowLineSum - i - pPar.m_showStartLine - 1;
				std::vector<_XDirListOneLine *> tempBuff;		//用于保存临时的数据
				for(int j = 0;j < sum;++ j)
				{//保存临时数据
					tempBuff.push_back(pPar.m_lineData[pPar.m_nowLineSum - 1 - j]);
					pPar.m_lineData.pop_back();
				}	
				//插入新数据
				_XDirListOneLine * tempLine = NULL;
				//_XVector2 pos = tempLineData->m_pos;
				for(int j = 0;j < insertSum;++ j)
				{
					tempLine = createMem<_XDirListOneLine>();
					tempLine->m_font.setACopy(pPar.m_caption);
					tempLine->m_font.setColor(pPar.m_color);
#if WITH_OBJECT_MANAGER
					_XObjManger.decreaseAObject(&(tempLine->m_font));
#endif
				//	tempLine->m_pos.set(pos.x,pos.y + pPar.m_nowTextHeight * (j + 1));
					if(tempLineData->m_file->directory->files[j]->isDirectory)
					{//是目录
						tempLine->m_check = createMem<_XCheck>();
						tempLine->m_check->setACopy(pPar.m_check);
						tempLine->m_check->setColor(pPar.m_color);
						tempLine->m_needCheck = XTrue;
				//		tempLine->m_check->setPosition(pos.x + (float)(tempLineData->m_file->directory->level) * pPar.m_nowTextWidth * 2.0f,
				//			tempLine->m_pos.y);
						_XCtrlManger.decreaseAObject(tempLine->m_check);
						tempLine->m_check->setStateChangeCB(funDirListCheck,&pPar);
#if WITH_OBJECT_MANAGER
						_XObjManger.decreaseAObject(tempLine->m_check);
#endif
					}else
					{
						tempLine->m_needCheck = XFalse;
					}
				//	tempLine->m_font.setPosition(tempLine->m_pos);
					tempLine->m_font.setSize(pPar.m_fontSize * pPar.m_size);
					tempLine->m_string = createArrayMem<char>(MAX_FILE_NAME_LENGTH);
					char tempStr[MAX_FILE_NAME_LENGTH] = "";
					for(int k = 0;k < tempLineData->m_file->directory->level;++ k)
					{
						tempStr[2 * k] = '|';
						tempStr[2 * k + 1] = ' ';
						tempStr[2 * k + 2] = '\0';
					}
					sprintf(tempLine->m_string,"%s|-%s",tempStr,tempLineData->m_file->directory->files[j]->filename);
					tempLine->m_font.setString(tempLine->m_string);
					tempLine->m_isEnable = XTrue;
					tempLine->m_file = tempLineData->m_file->directory->files[j];
					pPar.m_lineData.push_back(tempLine);
				}
				//插入老数据
				for(int j = 0;j < sum;++ j)
				{
			//		tempBuff[sum - 1 - j]->m_pos.set(pos.x,pos.y + pPar.m_nowTextHeight * (j + insertSum + 1));
			//		if(tempBuff[sum - 1 - j]->m_needCheck) tempBuff[sum - 1 - j]->m_check->setPosition(tempBuff[sum - 1 - j]->m_check->getPosition().x,
			//			tempBuff[sum - 1 - j]->m_pos.y);
			//		tempBuff[sum - 1 - j]->m_font.setPosition(tempBuff[sum - 1 - j]->m_pos);
					pPar.m_lineData.push_back(tempBuff[sum - 1 - j]);
					tempBuff.pop_back();
				}
				pPar.m_nowLineSum += insertSum;
				pPar.m_verticalSlider.setRange(pPar.m_nowLineSum - pPar.m_canShowLineSum,0.0f);
				if(pPar.m_nowLineSum > pPar.m_canShowLineSum) pPar.m_needShowVSlider = XTrue;
				else 
				{
					pPar.m_needShowVSlider = XFalse;
					pPar.m_showStartLine = 0;	//初始化数据
					pPar.m_verticalSlider.setNowValue(0.0f);
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
_XBool _XDirectoryList::init(const _XVector2& position,
	const _XRect& Area,
	_XDirListTexture &tex,
	_XFontUnicode &font,
	float fontSize,
	const _XCheck &check,
	const _XButton &button,
	const _XEdit &edit,
	const _XSlider &vSlider,	//垂直滑动条
	const _XSlider &hSlider)
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
	m_button.setMouseUpCB(funDirListButton,this);
	m_verticalSlider.setDataChangeCB(funDirListValueChangeV,funDirListValueChangeV,this);
	m_horizontalSlider.setDataChangeCB(funDirListValueChangeH,funDirListValueChangeH,this);

	m_caption.setACopy(font);
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);	 //设置字体上对齐
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);
	m_fontSize.set(fontSize,fontSize);
	m_caption.setSize(m_fontSize);
	m_nowTextWidth = m_caption.getTextSize().x * m_caption.getSize().x * 0.5f;
	m_nowTextHeight = m_caption.getTextSize().y * m_caption.getSize().y;

	m_size.set(1.0f,1.0f);
	//m_mouseRect.set(1.0f,1.0f,351.0f,361.0f);
	m_mouseRect = Area;
	m_showPixWidth = m_mouseRect.getWidth() * m_size.x;
	m_showPixHight = m_mouseRect.getHeight() * m_size.y;

	m_haveSelect = XFalse;
	m_selectLineOrder = 0;
	m_showStartLine = 0;
	m_canShowLineSum = m_showPixHight/m_nowTextHeight;

	m_nowLineLeft = 0;
	m_maxLineWidth = 0;
	m_canShowMaxLineWidth = m_mouseRect.getWidth();

	_XCtrlManger.decreaseAObject(&m_check);
	_XCtrlManger.decreaseAObject(&m_verticalSlider);
	_XCtrlManger.decreaseAObject(&m_horizontalSlider);
	_XCtrlManger.decreaseAObject(&m_button);
	_XCtrlManger.decreaseAObject(&m_edit);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_spriteBackGround);
	_XObjManger.decreaseAObject(&m_verticalSlider);
	_XObjManger.decreaseAObject(&m_horizontalSlider);
	_XObjManger.decreaseAObject(&m_check);
	_XObjManger.decreaseAObject(&m_button);
	_XObjManger.decreaseAObject(&m_edit);
	_XObjManger.decreaseAObject(&m_caption);
#endif
	m_position = position;
	m_spriteBackGround.setPosition(m_position + _XVector2(0.0f,m_edit.getMouseRect().getHeight()));
	m_edit.setPosition(m_position);
	char tempDirectoryName[MAX_FILE_NAME_LENGTH];
	GetCurrentDirectory(MAX_FILE_NAME_LENGTH,tempDirectoryName);	//获取当前路径
	m_edit.setString(tempDirectoryName);//初始化为当前路径

	m_button.setPosition(m_position + _XVector2(m_mouseRect.getWidth(),0.0f) * m_size);
	m_verticalSlider.setPosition(m_position + _XVector2(m_mouseRect.getWidth(),m_edit.getMouseRect().getHeight()));
	m_horizontalSlider.setPosition(m_position + _XVector2(0.0f,m_edit.getMouseRect().getHeight() + m_mouseRect.getHeight()));

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XCtrlManger.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XDirectoryList::initPlus(const char * path,
		_XFontUnicode &font,
		float fontSize,
		_XResourcePosition resoursePosition)
{
	if(m_isInited ||
		path == NULL) return XFalse;
	m_resInfo = _XResourceManager::GetInstance().loadResource(path,RESOURCE_TYPE_XDIRLIST_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	_XDirListTexture * tex = (_XDirListTexture *)m_resInfo->m_pointer;
	if(tex->dirListNormal == NULL || tex->dirListDisable == NULL) return XFalse;
	if(fontSize <= 0) return XFalse;
	m_position.set(0.0f,0.0f);
	m_dirListNormal = tex->dirListNormal;
	m_dirListDisable = tex->dirListDisable;
	m_spriteBackGround.init(m_dirListNormal->texture.m_w,m_dirListNormal->texture.m_h,1);
	m_spriteBackGround.setIsTransformCenter(POINT_LEFT_TOP);

	m_needShowVSlider = XFalse;			
	char tempPath[MAX_FILE_NAME_LENGTH];
	sprintf(tempPath,"%s/SliderV",path);
	m_verticalSlider.initPlus(tempPath,100.0f,0.0f,SLIDER_TYPE_VERTICAL,resoursePosition);
	m_needShowHSlider = XFalse;		
	sprintf(tempPath,"%s/SliderH",path);
	m_horizontalSlider.initPlus(tempPath,100.0f,0.0f,SLIDER_TYPE_HORIZONTAL,resoursePosition);
	sprintf(tempPath,"%s/Check",path);
	m_check.initPlus(tempPath," ",font,1.0f,resoursePosition);
	sprintf(tempPath,"%s/Button",path);
	m_button.initPlus(tempPath," ",font,1.0f,resoursePosition);
	sprintf(tempPath,"%s/Edit",path);
	m_edit.initPlus(tempPath," ",font,1.0f,NULL,resoursePosition);
	m_withoutTex = XFalse;
	//设置回调函数
	m_button.setMouseUpCB(funDirListButton,this);
	m_verticalSlider.setDataChangeCB(funDirListValueChangeV,funDirListValueChangeV,this);
	m_horizontalSlider.setDataChangeCB(funDirListValueChangeH,funDirListValueChangeH,this);

	m_caption.setACopy(font);
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);	 //设置字体上对齐
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);
	m_fontSize.set(fontSize,fontSize);
	m_caption.setSize(m_fontSize);
	m_nowTextWidth = m_caption.getTextSize().x * m_caption.getSize().x * 0.5f;
	m_nowTextHeight = m_caption.getTextSize().y * m_caption.getSize().y;

	m_size.set(1.0f,1.0f);
	m_mouseRect = tex->m_mouseRect;
	m_showPixWidth = m_mouseRect.getWidth() * m_size.x;
	m_showPixHight = m_mouseRect.getHeight() * m_size.y;

	m_haveSelect = XFalse;
	m_selectLineOrder = 0;
	m_showStartLine = 0;
	m_canShowLineSum = m_showPixHight/m_nowTextHeight;

	m_nowLineLeft = 0;
	m_maxLineWidth = 0;
	m_canShowMaxLineWidth = m_mouseRect.getWidth();

	_XCtrlManger.decreaseAObject(&m_check);
	_XCtrlManger.decreaseAObject(&m_verticalSlider);
	_XCtrlManger.decreaseAObject(&m_horizontalSlider);
	_XCtrlManger.decreaseAObject(&m_button);
	_XCtrlManger.decreaseAObject(&m_edit);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_spriteBackGround);
	_XObjManger.decreaseAObject(&m_verticalSlider);
	_XObjManger.decreaseAObject(&m_horizontalSlider);
	_XObjManger.decreaseAObject(&m_check);
	_XObjManger.decreaseAObject(&m_button);
	_XObjManger.decreaseAObject(&m_edit);
	_XObjManger.decreaseAObject(&m_caption);
#endif
	m_position.set(0.0f,0.0f);
	m_spriteBackGround.setPosition(m_position + _XVector2(0.0f,m_edit.getMouseRect().getHeight()));
	m_edit.setPosition(m_position);
	char tempDirectoryName[MAX_FILE_NAME_LENGTH];
	GetCurrentDirectory(MAX_FILE_NAME_LENGTH,tempDirectoryName);	//获取当前路径
	m_edit.setString(tempDirectoryName);//初始化为当前路径

	m_button.setPosition(m_position + _XVector2(m_mouseRect.getWidth(),0.0f) * m_size);
	m_verticalSlider.setPosition(m_position + _XVector2(m_mouseRect.getWidth(),m_edit.getMouseRect().getHeight()));
	m_horizontalSlider.setPosition(m_position + _XVector2(0.0f,m_edit.getMouseRect().getHeight() + m_mouseRect.getHeight()));

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XCtrlManger.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XDirectoryList::initWithoutTex(const _XRect& area,	
	_XFontUnicode &font,
	float fontSize)
{
	if(m_isInited) return XTrue;
	if(fontSize <= 0) return XFalse;
	m_position.set(0.0f,0.0f);

	m_mouseRect = area;

	m_needShowVSlider = XFalse;			
//	m_verticalSlider.initWithoutTex(_XRect(0,0,DEFAULT_SLIDER_WIDTH,m_mouseRect.getHeight()),
//		_XRect(0,0,DEFAULT_SLIDER_WIDTH,DEFAULT_SLIDER_WIDTH),SLIDER_TYPE_VERTICAL,100.0f,0.0f,_XVector2::zero);
	m_verticalSlider.initWithoutTex(_XRect(0,0,DEFAULT_SLIDER_WIDTH,m_mouseRect.getHeight()),100.0f,0.0f,SLIDER_TYPE_VERTICAL);
	m_needShowHSlider = XFalse;		
//	m_horizontalSlider.initWithoutTex(_XRect(0,0,m_mouseRect.getWidth(),DEFAULT_SLIDER_WIDTH),
//		_XRect(0,0,DEFAULT_SLIDER_WIDTH,DEFAULT_SLIDER_WIDTH),SLIDER_TYPE_HORIZONTAL,100.0f,0.0f,_XVector2::zero);
	m_horizontalSlider.initWithoutTex(_XRect(0,0,m_mouseRect.getWidth(),DEFAULT_SLIDER_WIDTH));
	m_check.initWithoutTex(" ",font,0.5f,_XRect(0,0,DEFAULT_DIRLIST_CK_SIZE,DEFAULT_DIRLIST_CK_SIZE),
		_XVector2(DEFAULT_DIRLIST_CK_SIZE,DEFAULT_DIRLIST_CK_SIZE));
	m_button.initWithoutTex(" ",font,1.0f,_XRect(0,0,DEFAULT_SLIDER_WIDTH,DEFAULT_DIRLIST_BT_SIZE),
		_XVector2(DEFAULT_SLIDER_WIDTH >> 1,DEFAULT_DIRLIST_BT_SIZE >> 1));
	m_edit.initWithoutTex(_XRect(0,0,m_mouseRect.getWidth(),DEFAULT_DIRLIST_BT_SIZE)," ",font,1.0f,NULL);
	m_withoutTex = XTrue;
	//设置回调函数
	m_button.setMouseUpCB(funDirListButton,this);
	m_verticalSlider.setDataChangeCB(funDirListValueChangeV,funDirListValueChangeV,this);
	m_horizontalSlider.setDataChangeCB(funDirListValueChangeH,funDirListValueChangeH,this);

	m_caption.setACopy(font);
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);	 //设置字体上对齐
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);
	m_fontSize.set(fontSize,fontSize);
	m_caption.setSize(m_fontSize);
	m_nowTextWidth = m_caption.getTextSize().x * m_caption.getSize().x * 0.5f;
	m_nowTextHeight = m_caption.getTextSize().y * m_caption.getSize().y;

	m_size.set(1.0f,1.0f);
	m_showPixWidth = m_mouseRect.getWidth() * m_size.x;
	m_showPixHight = m_mouseRect.getHeight() * m_size.y;

	m_haveSelect = XFalse;
	m_selectLineOrder = 0;
	m_showStartLine = 0;
	m_canShowLineSum = m_showPixHight/m_nowTextHeight;

	m_nowLineLeft = 0;
	m_maxLineWidth = 0;
	m_canShowMaxLineWidth = m_mouseRect.getWidth();

	_XCtrlManger.decreaseAObject(&m_check);
	_XCtrlManger.decreaseAObject(&m_verticalSlider);
	_XCtrlManger.decreaseAObject(&m_horizontalSlider);
	_XCtrlManger.decreaseAObject(&m_button);
	_XCtrlManger.decreaseAObject(&m_edit);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_spriteBackGround);
	_XObjManger.decreaseAObject(&m_verticalSlider);
	_XObjManger.decreaseAObject(&m_horizontalSlider);
	_XObjManger.decreaseAObject(&m_check);
	_XObjManger.decreaseAObject(&m_button);
	_XObjManger.decreaseAObject(&m_edit);
	_XObjManger.decreaseAObject(&m_caption);
#endif
	m_position.set(0.0f,0.0f);
	m_edit.setPosition(m_position);
	char tempDirectoryName[MAX_FILE_NAME_LENGTH];
	GetCurrentDirectory(MAX_FILE_NAME_LENGTH,tempDirectoryName);	//获取当前路径
	m_edit.setString(tempDirectoryName);//初始化为当前路径

	m_button.setPosition(m_position + _XVector2(m_mouseRect.getWidth(),0.0f) * m_size);
	m_verticalSlider.setPosition(m_position + _XVector2(m_mouseRect.getWidth(),m_edit.getMouseRect().getHeight()));
	m_horizontalSlider.setPosition(m_position + _XVector2(0.0f,m_edit.getMouseRect().getHeight() + m_mouseRect.getHeight()));

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XCtrlManger.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
void _XDirectoryList::draw()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出

	if(m_withoutTex)
	{
		if(!m_isEnable) 
		{
			drawFillBoxExA(m_position + _XVector2(m_mouseRect.left * m_size.x,(m_mouseRect.top + m_edit.getMouseRect().getHeight()) * m_size.y),
				_XVector2(m_mouseRect.getWidth() * m_size.x,
				m_mouseRect.getHeight() * m_size.y),0.8f * m_color.fR,0.8f * m_color.fG,0.8f * m_color.fB,m_color.fA); 

			drawFillBoxExA(m_position + _XVector2(m_mouseRect.right * m_size.x,
				(m_mouseRect.top + m_edit.getMouseRect().getHeight()) * m_size.y),
				_XVector2(m_verticalSlider.getMouseRectWidth() * m_size.x,
				m_verticalSlider.getMouseRectHeight() * m_size.y),0.5f * m_color.fR,0.5f * m_color.fG,0.5f * m_color.fB,m_color.fA);
			drawFillBoxExA(m_position + _XVector2(m_mouseRect.left * m_size.x,
				(m_mouseRect.bottom + m_edit.getMouseRect().getHeight()) * m_size.y),
				_XVector2(m_horizontalSlider.getMouseRectWidth() * m_size.x,
				m_horizontalSlider.getMouseRectHeight() * m_size.y),0.5f * m_color.fR,0.5f * m_color.fG,0.5f * m_color.fB,m_color.fA);
			drawFillBoxExA(m_position + _XVector2(m_mouseRect.right * m_size.x,
				(m_mouseRect.bottom + m_edit.getMouseRect().getHeight()) * m_size.y),
				_XVector2(m_verticalSlider.getMouseRectWidth() * m_size.x,
				m_horizontalSlider.getMouseRectHeight() * m_size.y),0.2f * m_color.fR,0.2f * m_color.fG,0.2f * m_color.fB,m_color.fA);
		}else
		{
			drawFillBoxExA(m_position + _XVector2(m_mouseRect.left * m_size.x,(m_mouseRect.top + m_edit.getMouseRect().getHeight()) * m_size.y),
				_XVector2(m_mouseRect.getWidth() * m_size.x,
				m_mouseRect.getHeight() * m_size.y),0.6f * m_color.fR,0.6f * m_color.fG,0.6f * m_color.fB,m_color.fA); 

			drawFillBoxExA(m_position + _XVector2(m_mouseRect.right * m_size.x,
				(m_mouseRect.top + m_edit.getMouseRect().getHeight()) * m_size.y),
				_XVector2(m_verticalSlider.getMouseRectWidth() * m_size.x,
				m_verticalSlider.getMouseRectHeight() * m_size.y),0.65f * m_color.fR,0.65f * m_color.fG,0.65f * m_color.fB,m_color.fA);
			drawFillBoxExA(m_position + _XVector2(m_mouseRect.left * m_size.x,
				(m_mouseRect.bottom + m_edit.getMouseRect().getHeight()) * m_size.y),
				_XVector2(m_horizontalSlider.getMouseRectWidth() * m_size.x,
				m_horizontalSlider.getMouseRectHeight() * m_size.y),0.65f * m_color.fR,0.65f * m_color.fG,0.65f * m_color.fB,m_color.fA);
			drawFillBoxExA(m_position + _XVector2(m_mouseRect.right * m_size.x,
				(m_mouseRect.bottom + m_edit.getMouseRect().getHeight()) * m_size.y),
				_XVector2(m_verticalSlider.getMouseRectWidth() * m_size.x,
				m_horizontalSlider.getMouseRectHeight() * m_size.y),0.45f * m_color.fR,0.45f * m_color.fG,0.45f * m_color.fB,m_color.fA);
		}
	}else
	{
		if(!m_isEnable) m_spriteBackGround.draw(m_dirListDisable);
		else m_spriteBackGround.draw(m_dirListNormal);
	}
	//显示选择的状态
	if(m_haveSelect)
	{
		if(m_selectLineOrder >= m_showStartLine && m_selectLineOrder < m_showStartLine + m_canShowLineSum)
		{//需要显示
			drawBox(m_position.x + m_mouseRect.left * m_size.x + m_mouseRect.getWidth() * m_size.x * 0.5f,
				m_position.y + (m_mouseRect.top + m_edit.getMouseRect().getHeight() + (m_selectLineOrder - m_showStartLine + 0.5f) * m_nowTextHeight) * m_size.y,	//42.0f为输入框的高
				m_mouseRect.getWidth() * m_size.x * 0.5f,
				m_nowTextHeight * m_size.y * 0.5f,0.5f,0.0f,0.0f,0.0f,m_color.fA);
		}
	}
	if(m_needShowVSlider) m_verticalSlider.draw();
	if(m_needShowHSlider) m_horizontalSlider.draw();
	m_button.draw();
	m_edit.draw();
	//显示所有行的信息
	int lineSum = 0;
	if(m_nowLineSum - m_showStartLine >= m_canShowLineSum) lineSum = m_canShowLineSum;
	else lineSum = m_nowLineSum - m_showStartLine;
	_XDirListOneLine * tempLine;
	for(int i = 0;i < lineSum;++ i)
	{
		tempLine = m_lineData[i + m_showStartLine];
		tempLine->m_font.draw();
		if(tempLine->m_needCheck && tempLine->m_check != NULL) 
		{
			if(tempLine->m_check->getPosition().x >= m_position.x
				&& tempLine->m_check->getPosition().x < m_position.x + m_canShowMaxLineWidth - tempLine->m_check->getMouseRect().getWidth())
				tempLine->m_check->draw();
		}
	}
}
void _XDirectoryList::drawUp()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	if(m_needShowVSlider) m_verticalSlider.drawUp();
	if(m_needShowHSlider) m_horizontalSlider.drawUp();
	m_button.drawUp();
	m_edit.drawUp();
	//显示所有行的信息
	int lineSum = 0;
	if(m_nowLineSum - m_showStartLine >= m_canShowLineSum) lineSum = m_canShowLineSum;
	else lineSum = m_nowLineSum - m_showStartLine;
	_XDirListOneLine * tempLine;
	for(int i = 0;i < lineSum;++ i)
	{
		tempLine = m_lineData[i + m_showStartLine];
		if(tempLine->m_needCheck && tempLine->m_check != NULL) 
		{
			if(tempLine->m_check->getPosition().x >= m_position.x
				&& tempLine->m_check->getPosition().x < m_position.x + m_canShowMaxLineWidth - tempLine->m_check->getMouseRect().getWidth())
				tempLine->m_check->drawUp();
		}
	}
}
void _XDirectoryList::update(int stepTime)
{
	if(m_needShowVSlider) m_verticalSlider.update(stepTime);
	if(m_needShowHSlider) m_horizontalSlider.update(stepTime);
	m_button.update(stepTime);
	m_edit.update(stepTime);
	//显示所有行的信息
	int lineSum = 0;
	if(m_nowLineSum - m_showStartLine >= m_canShowLineSum) lineSum = m_canShowLineSum;
	else lineSum = m_nowLineSum - m_showStartLine;
	_XDirListOneLine * tempLine;
	for(int i = 0;i < lineSum;++ i)
	{
		tempLine = m_lineData[i + m_showStartLine];
		if(tempLine->m_needCheck && tempLine->m_check != NULL) 
		{
			if(tempLine->m_check->getPosition().x >= m_position.x
				&& tempLine->m_check->getPosition().x < m_position.x + m_canShowMaxLineWidth - tempLine->m_check->getMouseRect().getWidth())
				tempLine->m_check->update(stepTime);
		}
	}
	//鼠标双击计时
	m_mouseTime += stepTime;
}
#pragma comment(lib, "shell32.lib")	//for ShellExecute()!
_XBool _XDirectoryList::mouseProc(float x,float y,_XMouseState mouseState)
{
	if(!m_isInited || 	//如果没有初始化直接退出
		!m_isVisible) return XTrue;	//如果不可见直接退出
	if(m_needShowVSlider) m_verticalSlider.mouseProc(x,y,mouseState);
	if(m_needShowHSlider) m_horizontalSlider.mouseProc(x,y,mouseState);
	m_edit.mouseProc(x,y,mouseState);
	m_button.mouseProc(x,y,mouseState);
	//所有文件夹的点接受鼠标控制
	int lineSum = 0;
	if(m_nowLineSum - m_showStartLine >= m_canShowLineSum) lineSum = m_canShowLineSum;
	else lineSum = m_nowLineSum - m_showStartLine;
	_XDirListOneLine * tempLine;
	for(int i = m_showStartLine;i < lineSum + m_showStartLine;++ i)
	{
		tempLine = m_lineData[i];
		if(tempLine->m_needCheck && tempLine->m_check != NULL
			&& tempLine->m_check->getPosition().x >= m_position.x
			&& tempLine->m_check->getPosition().x < m_position.x + m_canShowMaxLineWidth - tempLine->m_check->getMouseRect().getWidth()) 
		{
			if(tempLine->m_check->mouseProc(x,y,mouseState))
			{//如果鼠标动作产生了变化，则在这里更新状态信息
				if(m_nowLineSum - m_showStartLine >= m_canShowLineSum) lineSum = m_canShowLineSum;
				else lineSum = m_nowLineSum - m_showStartLine;
			}
		}
	}
	//下面计算点选的情况
	if(mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_LEFT_BUTTON_DCLICK)
	{//计算点击是否在范围内
		_XRect tempRect;
		tempRect.set(m_mouseRect.left * m_size.x + m_position.x,
			(m_mouseRect.top + m_edit.getMouseRect().getHeight()) * m_size.y + m_position.y,
			m_mouseRect.right * m_size.x + m_position.x,
			(m_mouseRect.bottom + m_edit.getMouseRect().getHeight()) * m_size.y + m_position.y);
		if(tempRect.isInRect(x,y))
		{//处于点击范围之内
			int lineOrder = (y - (m_mouseRect.top + m_edit.getMouseRect().getHeight()) * m_size.y - m_position.y) / (m_nowTextHeight * m_size.y);
			if(lineOrder >= 0 && lineOrder < m_canShowLineSum)
			{
				if(lineOrder + m_showStartLine < m_nowLineSum)
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
			//sprintf(tempStr,"explorer %s",getSelectFileName());
			//system(tempStr);
			if(m_haveSelect)
			{
				if(m_lineData[m_selectLineOrder]->m_needCheck)
				{//打开文件夹
					if(m_lineData[m_selectLineOrder]->m_check->getState()) m_lineData[m_selectLineOrder]->m_check->setState(XFalse);
					else m_lineData[m_selectLineOrder]->m_check->setState(XTrue);
					funDirListCheck(this,m_lineData[m_selectLineOrder]->m_check->getControlID());
				}else
				{
					ShellExecute(NULL,"open",m_lineData[m_selectLineOrder]->m_file->allPath,NULL,NULL,SW_SHOW);
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
void _XDirectoryList::furlFolder(int index,_XBool flag)
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
	_XDirListOneLine *tempLineData = m_lineData[index];
//	printf("折叠%s\n",tempLineData->m_string);
	int insertSum = tempLineData->m_file->directory->files.size();	//要插入的数量
	int sum = m_nowLineSum - index - insertSum - 1;
	for(int j = 0;j < insertSum;++ j)
	{//释放多余的资源
		m_lineData[j + index + 1]->release();
	}
	//移动
	//_XVector2 pos = tempLineData->m_pos;
	for(int j = 0;j < sum;++ j)
	{
		m_lineData[j + index + 1] = m_lineData[j + index + 1 + insertSum];
		tempLineData = m_lineData[j + index + 1];
		//改变位置
	//	tempLineData->m_pos.set(pos.x,pos.y + m_nowTextHeight * (j + 1));
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
	m_nowLineSum -= insertSum;
	m_verticalSlider.setRange(m_nowLineSum - m_canShowLineSum,0.0f);
	if(m_nowLineSum > m_canShowLineSum) m_needShowVSlider = XTrue;
	else 
	{
		m_needShowVSlider = XFalse;
		m_showStartLine = 0;	//初始化数据
		m_verticalSlider.setNowValue(0.0f);
	}
	updateHSliderState();
}
void _XDirectoryList::updateHSliderState()
{
	//遍历所有的条目，寻找到长的条目，然后进行裁剪或者复位
	m_maxLineWidth = 0;				//当前的最大行宽[像素]

	for(int i = 0;i < m_lineData.size();++ i)
	{
		m_lineData[i]->m_font.disClip();
		if(m_lineData[i]->m_font.getMaxPixelWidth() > m_maxLineWidth)
		{
			m_maxLineWidth = m_lineData[i]->m_font.getMaxPixelWidth();	//寻找最宽的
		}
	}
	if(m_maxLineWidth > m_canShowMaxLineWidth * m_size.x)
	{//宽度越界
		if(!m_needShowHSlider)
		{//尚未进行裁剪
			m_needShowHSlider = XTrue;
			m_nowLineLeft = 0;
			m_horizontalSlider.setRange((m_maxLineWidth - m_canShowMaxLineWidth * m_size.x)/(m_nowTextWidth * m_size.x * 2.0f) + 1.0f,0.0f);
			m_horizontalSlider.setNowValue(0.0f);
		}else
		{//已经进行各裁剪
			float tempValue = (m_maxLineWidth - m_canShowMaxLineWidth * m_size.x)/(m_nowTextWidth * m_size.x * 2.0f) + 1.0f;
			if(m_horizontalSlider.getNowValue() >= tempValue)
			{
				m_horizontalSlider.setNowValue(tempValue);
			}
			m_horizontalSlider.setRange(tempValue,0.0f);
		}
	}else
	{//不越界没有必要进行裁剪
		if(m_needShowHSlider)
		{//取消所有裁剪设置
			m_needShowHSlider = XFalse;
			m_nowLineLeft = 0;
			m_horizontalSlider.setNowValue(0.0f);
			for(int i = 0;i < m_lineData.size();++ i)
			{
				m_lineData[i]->m_font.disClip();	//取消所有裁剪
			}
		}
	}
}
void _XDirectoryList::updateShowPosition()
{
	int size = 0;
	if(m_nowLineSum - m_showStartLine > m_canShowLineSum) size = m_canShowLineSum;
	else size = m_nowLineSum - m_showStartLine;
	for(int i = 0;i < size;++ i)
	{
		m_lineData[i + m_showStartLine]->m_font.setSize(m_size);
		m_lineData[i + m_showStartLine]->m_font.setClipRect(m_nowLineLeft,0.0f,m_nowLineLeft + m_canShowMaxLineWidth,32.0f);	//字体的高度
		m_lineData[i + m_showStartLine]->m_font.setPosition(m_position + _XVector2(0.0f - m_nowLineLeft,m_edit.getMouseRect().getHeight() + m_nowTextHeight * i) * m_size);
		if(m_lineData[i + m_showStartLine]->m_needCheck)
		{
			m_lineData[i + m_showStartLine]->m_check->setSize(m_size);
			m_lineData[i + m_showStartLine]->m_check->setPosition(m_position.x + 
				((m_lineData[i + m_showStartLine]->m_file->directory->level - 1) * m_nowTextWidth * 2.0f - m_nowLineLeft) * m_size.x,
				m_position.y + (m_edit.getMouseRect().getHeight() + m_nowTextHeight * i) * m_size.y);
		}
	}
}
void _XDirectoryList::release()
{
	if(!m_isInited) return;
	//下面释放所有的资源
	for(int i = 0;i < m_nowLineSum;++ i)
	{
		m_lineData[i]->release();
		XDELETE(m_lineData[i]);
	}
	m_lineData.clear();
	m_lineData.swap(std::vector<_XDirListOneLine *>());

	_XCtrlManger.decreaseAObject(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(this);
#endif
	if(m_resInfo != NULL)
	{
		_XResourceManager::GetInstance().releaseResource(m_resInfo);
		m_resInfo = NULL;
	}
	m_isInited = XFalse;
}