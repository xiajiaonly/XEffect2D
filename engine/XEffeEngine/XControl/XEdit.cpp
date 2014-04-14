//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XEdit.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "XResourcePack.h"
#include "XResourceManager.h"

//这一段是在Edit内部公用的内存空间，主要用于类实体之间的复制粘贴时的内存调用问题
char copyString[MAX_INPUT_STRING_LENGTH] = "";

_XEditTexture::_XEditTexture()
:m_isInited(XFalse)
,editNormal(NULL)			//输入框普通状态
,editDisable(NULL)			//输入框无效状态
,editSelect(NULL)			//输入框片选颜色
,editInsert(NULL)			//输入框插入标记
,editUpon(NULL)
{
}
_XEditTexture::~_XEditTexture()
{
	release();
}
_XBool _XEditTexture::init(const char *normal,const char *disable,const char *select,const char *insert,const char *upon,_XResourcePosition resoursePosition)
{
	if(m_isInited) return XFalse;	//防止重复初始化
	if(normal == NULL || disable == NULL || insert == NULL || select == NULL) return XFalse;//这些关键的资源不能为空
	int ret = 1;
	if((editNormal = createATextureData(normal,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(editDisable = createATextureData(disable,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(editInsert = createATextureData(insert,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(editSelect = createATextureData(select,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(editUpon = createATextureData(upon,resoursePosition)) == NULL) ret = 0;

	if(ret == 0)
	{
		releaseTex();
		return XFalse;
	}

	m_isInited = XTrue;
	return XTrue;
}
#define EDIT_CONFIG_FILENAME ("Edit.txt")
_XBool _XEditTexture::initEx(const char *filename,_XResourcePosition resoursePosition)
{
	if(m_isInited) return XFalse;
	if(filename == NULL) return XFalse;
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf(tempFilename,"%s/%s",filename,EDIT_CONFIG_FILENAME);
	//先打开配置文件
	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = XEE::defaultResourcePosition;
	if(resoursePosition == RESOURCE_LOCAL_FOLDER)
	{//外部资源
		FILE *fp = NULL;
		if((fp = fopen(tempFilename,"r")) == 0) return XFalse; //信息文件读取失败
		//下面开始依次读取数据
		int flag = 0;
		char resFilename[MAX_FILE_NAME_LENGTH] = "";
		//normal
		fscanf(fp,"%d:",&flag);
		if(flag == 0)
		{//没有普通状态是不行的
			fclose(fp);
			return XFalse;
		}
		fscanf(fp,"%s",resFilename);
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((editNormal = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//资源读取失败
			fclose(fp);
			return XFalse;
		}
		//down
		fscanf(fp,"%d:",&flag);
		if(flag == 0)
		{//没有普通状态是不行的
			releaseTex();
			fclose(fp);
			return XFalse;
		}
		fscanf(fp,"%s",resFilename);
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((editDisable = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//资源读取失败
			releaseTex();
			fclose(fp);
			return XFalse;
		}
		//on
		fscanf(fp,"%d:",&flag);
		if(flag == 0)
		{//没有普通状态是不行的
			releaseTex();
			fclose(fp);
			return XFalse;
		}
		fscanf(fp,"%s",resFilename);
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((editSelect = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//资源读取失败
			releaseTex();
			fclose(fp);
			return XFalse;
		}
		//disable
		fscanf(fp,"%d:",&flag);
		if(flag == 0)
		{//没有普通状态是不行的
			releaseTex();
			fclose(fp);
			return XFalse;
		}
		fscanf(fp,"%s",resFilename);
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((editInsert = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//资源读取失败
			releaseTex();
			fclose(fp);
			return XFalse;
		}
		//upon
		fscanf(fp,"%d:",&flag);
		if(flag != 0)
		{
			fscanf(fp,"%s",resFilename);
			sprintf(tempFilename,"%s/%s",filename,resFilename);
			if((editUpon = createATextureData(tempFilename,resoursePosition)) == NULL)
			{//资源读取失败
				releaseTex();
				fclose(fp);
				return XFalse;
			}
		}
		//读取两组数据
		int l,t,r,b;
		fscanf(fp,"%d,%d,%d,%d,",&l,&t,&r,&b);
		m_mouseRect.set(l,t,r,b);
		//所有数据读取完成
		fclose(fp);
	}else
	{
		unsigned char *p = _XResourcePack::GetInstance().getFileData(tempFilename);
		if(p == NULL) return XFalse;

		//下面开始依次读取数据
		int flag = 0;
		char resFilename[MAX_FILE_NAME_LENGTH] = "";
		int offset = 0;
		//normal
		sscanf((char *)(p + offset),"%d:",&flag);
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag == 0)
		{//没有普通状态是不行的
			XDELETE_ARRAY(p);
			return XFalse;
		}
		sscanf((char *)(p + offset),"%s",resFilename);
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((editNormal = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//资源读取失败
			XDELETE_ARRAY(p);
			return XFalse;
		}
		//down
		sscanf((char *)(p + offset),"%d:",&flag);
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag == 0)
		{//没有普通状态是不行的
			releaseTex();
			XDELETE_ARRAY(p);
			return XFalse;
		}
		sscanf((char *)(p + offset),"%s",resFilename);
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((editDisable = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//资源读取失败
			releaseTex();
			XDELETE_ARRAY(p);
			return XFalse;
		}
		//on
		sscanf((char *)(p + offset),"%d:",&flag);
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag == 0)
		{//没有普通状态是不行的
			releaseTex();
			XDELETE_ARRAY(p);
			return XFalse;
		}
		sscanf((char *)(p + offset),"%s",resFilename);
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((editSelect = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//资源读取失败
			releaseTex();
			XDELETE_ARRAY(p);
			return XFalse;
		}
		//disable
		sscanf((char *)(p + offset),"%d:",&flag);
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag == 0)
		{//没有普通状态是不行的
			releaseTex();
			XDELETE_ARRAY(p);
			return XFalse;
		}
		sscanf((char *)(p + offset),"%s",resFilename);
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((editInsert = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//资源读取失败
			releaseTex();
			XDELETE_ARRAY(p);
			return XFalse;
		}
		//upon
		sscanf((char *)(p + offset),"%d:",&flag);
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag != 0)
		{
			sscanf((char *)(p + offset),"%s",resFilename);
			offset += getCharPosition((char *)(p + offset),'\n') + 1;
			sprintf(tempFilename,"%s/%s",filename,resFilename);
			if((editUpon = createATextureData(tempFilename,resoursePosition)) == NULL)
			{//资源读取失败
				releaseTex();
				XDELETE_ARRAY(p);
				return XFalse;
			}
		}
		//读取两组数据
		int l,t,r,b;
		sscanf((char *)(p + offset),"%d,%d,%d,%d,",&l,&t,&r,&b);
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		m_mouseRect.set(l,t,r,b);
		//所有数据读取完成
		XDELETE_ARRAY(p);
	}
	m_isInited = XTrue;
	return XTrue;
}
void _XEditTexture::releaseTex()
{
	XDELETE(editNormal);
	XDELETE(editDisable);
	XDELETE(editSelect);
	XDELETE(editInsert);
	XDELETE(editUpon);
}
void _XEditTexture::release()
{
	if(!m_isInited) return;
	releaseTex();
	m_isInited = XFalse;
}
_XEdit::_XEdit()
:m_isInited(XFalse)					//是否初始化
,m_mouseRightButtonMenu(NULL)	//鼠标右键菜单
,m_nowString(NULL)				//当前输入的字符串
,m_tempNowString(NULL)
//,m_insertString(NULL)			//插入的字符串
//,m_selectString(NULL)			//选取的字符串
,m_haveSelect(XFalse)
,m_editNormal(NULL)				//输入框普通状态
,m_editDisable(NULL)			//输入框无效状态
,m_editSelect(NULL)				//输入框片选颜色
,m_editInsert(NULL)				//输入框插入标记
,m_funInputChenge(NULL)			//输入内容发生改变的时候调用
,m_funInputOver(NULL)			//确认输入结束之后的时候调用
,m_resInfo(NULL)
,m_withoutTex(XFalse)
{
}
_XEdit::~_XEdit()
{
	release();
}
_XBool _XEdit::init(const _XVector2& position,		//控件的位置
		const _XRect& Area,					//控件的鼠标响应区间
		const _XEditTexture &tex,			//控件的贴图
		const char *str,					//控件的初始化字符串
		const _XFontUnicode &font,			//控件的字体
		float strSize,	//控件的字体信息
		_XMouseRightButtonMenu * mouseMenu)		//控件的右键菜单
{
	if(m_isInited) return XFalse;
	if(tex.editNormal == NULL || tex.editInsert == NULL || tex.editDisable == NULL || tex.editSelect == NULL) return XFalse;

	//需要注意的是输入范围必须要能显示最少一个字符，否则将会造成问题，目前这里并没有代码去判断，但是实际使用中需要注意这个问题
	if(Area.getWidth() <= 0 || Area.getHeight() <= 0) return XFalse;	//输入范围不能为空
	if(strSize <= 0) return XFalse;		//字符串的大小不能为非法值
	if(str != NULL && strlen(str) >= MAX_INPUT_STRING_LENGTH) return XFalse;
	if(mouseMenu != NULL)
	{//这里需要改进，这里使用指针，会受到别处修改的影响
		m_mouseRightButtonMenu = createMem<_XMouseRightButtonMenu>();
		if(m_mouseRightButtonMenu == NULL) return XFalse;	
		m_mouseRightButtonMenu->setACopy(* mouseMenu);
		//从控件管理器中将这个控件注销掉
		_XControlManager::GetInstance().decreaseAObject(m_mouseRightButtonMenu);	//注销这个物件
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(m_mouseRightButtonMenu);
#endif
	}

	//下面开始付值
	m_mouseRect = Area;	//记录鼠标的响应范围
	m_position = position;
	m_withoutTex = XFalse;

	m_editNormal = tex.editNormal;			//输入框普通状态
	m_editDisable = tex.editDisable;		//输入框无效状态
	m_editSelect = tex.editSelect;			//输入框片选颜色
	m_editInsert = tex.editInsert;			//输入框插入标记
	m_editUpon = tex.editUpon;			//输入框插入标记
	m_size.set(1.0f,1.0f);
	m_nowString = createArrayMem<char>(MAX_INPUT_STRING_LENGTH);
	if(m_nowString == NULL) 
	{
		XDELETE(m_mouseRightButtonMenu);
		return XFalse;
	}		
	m_nowString[0] = '\0';
	m_tempNowString = createArrayMem<char>(MAX_INPUT_STRING_LENGTH);
	if(m_tempNowString == NULL) 
	{
		XDELETE_ARRAY(m_nowString);
		XDELETE(m_mouseRightButtonMenu);
		return XFalse;
	}		
	m_tempNowString[0] = '\0';
//	m_insertString = createArrayMem<char>(MAX_INPUT_STRING_LENGTH);	//当前输入的字符串
//	if(m_insertString == NULL) return 0;
//	m_insertString[0] = '\0';
//	m_selectString = createArrayMem<char>(MAX_INPUT_STRING_LENGTH);	//当前输入的字符串
//	if(m_selectString == NULL) return 0;
//	m_selectString[0] = '\0';
	m_nowShowString = createArrayMem<char>(MAX_INPUT_STRING_LENGTH);
	if(m_nowShowString == NULL) 
	{
		XDELETE_ARRAY(m_tempNowString);
		XDELETE_ARRAY(m_nowString);
		XDELETE(m_mouseRightButtonMenu);
		return XFalse;
	}		
	m_nowShowString[0] = '\0';

	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //设置字体居中对齐
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//设置字体的颜色为黑色

	m_textPosition.set(m_mouseRect.left + TEXT_EDGE_WIDTH,m_mouseRect.top + m_mouseRect.getHeight() * 0.5f);			//文字显示的位置，是相对于控件的位置来定的
	m_textSize.set(strSize,strSize);				//文字显示的尺寸，这个尺寸会与空间的缩放尺寸叠加
	if(str != NULL)
	{
		m_caption.setString(str);
		strcpy(m_nowString,str);
		m_nowInsertPoint = strlen(m_nowString);	//当前选择输入的位置
		m_nowStringLength = strlen(m_nowString);	//当前输入字符串的长度
	}else
	{
		m_caption.setString(" ");
		m_nowString[0] = '\0';
		m_nowInsertPoint = 0;	//当前选择输入的位置
		m_nowStringLength = 0;	//当前输入字符串的长度
	}
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	//m_caption.m_isPassWord = 1;
//	m_selectText.setACopy(* font);
//	m_selectText.setString(" ");
//	m_selectText.setPosition(m_objRect.left + (m_edgeDistance.left + m_textPosition.x) * m_size.x,m_objRect.top + (m_edgeDistance.top + m_textPosition.y) * m_size.y);
//	m_selectText.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);

	m_haveSelect = XFalse;		//是否在字符串中有选择
	m_selectMouseDown = XFalse;
	m_selectStart = -1;		//选择的起始位置
	m_selectEnd = -1;		//选择的结束位置

	m_spriteBackGround.init(m_editNormal->texture.m_w,m_editNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteBackGround);
#endif
	m_spriteBackGround.setPosition(m_position);
	m_spriteBackGround.setSize(m_size);
	m_spriteBackGround.setIsTransformCenter(POINT_LEFT_TOP);

	m_spriteSelect.init(m_editSelect->texture.m_w,m_editSelect->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteSelect);
#endif
	m_spriteSelect.setPosition(m_position);
	m_spriteSelect.setSize(m_size);
	m_spriteSelect.setIsTransformCenter(POINT_LEFT_TOP);

	//计算输入框中可以显示的字符串的长度(这里取0.5，是因为显示英文字符但是却是使用的中文字体库)
	m_nowTextWidth = m_caption.getTextSize().x * m_caption.getSize().x * 0.5f;
	m_nowTextHeight = m_caption.getTextSize().y * m_caption.getSize().y;
	m_canShowLength = (m_mouseRect.getWidth() - 2.0f * TEXT_EDGE_WIDTH) * m_size.x / m_nowTextWidth;
	m_nowShowStart = 0;	//初始化当前显示的起始位置
	upDataShowStr();

	m_spriteInsert.init(m_editInsert->texture.m_w,m_editInsert->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteInsert);
#endif
	upDateInsertShowPosition();
	m_spriteInsert.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	m_spriteInsert.setIsTransformCenter(POINT_LEFT_TOP);

	//printf("Can Show String Length:%d\n",m_canShowLength);

	m_timer = 0;

	m_keyShiftState = 0;
	m_keyCapsLockState = 0;
	m_keyNumLockState = 1;
	m_keyCtrlState = 0;

	m_isVisiable = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;
	m_isBeChoose = XFalse; 

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_EDIT);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XEdit::initEx(const _XVector2& position,		//控件的位置
	const _XEditTexture &tex,			//控件的贴图
	const char *str,					//控件的初始化字符串
	const _XFontUnicode &font,			//控件的字体
	float strSize,	//控件的字体信息
	_XMouseRightButtonMenu * mouseMenu)		//控件的右键菜单
{
	if(m_isInited) return XFalse;
	if(tex.editNormal == NULL || tex.editInsert == NULL || tex.editDisable == NULL || tex.editSelect == NULL) return XFalse;

	//需要注意的是输入范围必须要能显示最少一个字符，否则将会造成问题，目前这里并没有代码去判断，但是实际使用中需要注意这个问题
	if(tex.m_mouseRect.getWidth() <= 0 || tex.m_mouseRect.getHeight() <= 0) return XFalse;	//输入范围不能为空
	if(strSize <= 0) return XFalse;		//字符串的大小不能为非法值
	if(str != NULL && strlen(str) >= MAX_INPUT_STRING_LENGTH) return XFalse;
	if(mouseMenu != NULL)
	{//这里需要改进，这里使用指针，会受到别处修改的影响
		m_mouseRightButtonMenu = createMem<_XMouseRightButtonMenu>();
		if(m_mouseRightButtonMenu == NULL) return XFalse;	
		m_mouseRightButtonMenu->setACopy(* mouseMenu);
		//从控件管理器中将这个控件注销掉
		_XControlManager::GetInstance().decreaseAObject(m_mouseRightButtonMenu);	//注销这个物件
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(m_mouseRightButtonMenu);
#endif
	}

	//下面开始付值
	m_mouseRect = tex.m_mouseRect;	//记录鼠标的响应范围
	m_position = position;
	m_withoutTex = XFalse;

	m_editNormal = tex.editNormal;			//输入框普通状态
	m_editDisable = tex.editDisable;		//输入框无效状态
	m_editSelect = tex.editSelect;			//输入框片选颜色
	m_editInsert = tex.editInsert;			//输入框插入标记
	m_editUpon = tex.editUpon;			//输入框插入标记
	m_size.set(1.0f,1.0f);
	m_nowString = createArrayMem<char>(MAX_INPUT_STRING_LENGTH);
	if(m_nowString == NULL) 
	{
		XDELETE(m_mouseRightButtonMenu);
		return XFalse;
	}		
	m_nowString[0] = '\0';
	m_tempNowString = createArrayMem<char>(MAX_INPUT_STRING_LENGTH);
	if(m_tempNowString == NULL) 
	{
		XDELETE_ARRAY(m_nowString);
		XDELETE(m_mouseRightButtonMenu);
		return XFalse;
	}		
	m_tempNowString[0] = '\0';
//	m_insertString = createArrayMem<char>(MAX_INPUT_STRING_LENGTH);	//当前输入的字符串
//	if(m_insertString == NULL) return 0;
//	m_insertString[0] = '\0';
//	m_selectString = createArrayMem<char>(MAX_INPUT_STRING_LENGTH);	//当前输入的字符串
//	if(m_selectString == NULL) return 0;
//	m_selectString[0] = '\0';
	m_nowShowString = createArrayMem<char>(MAX_INPUT_STRING_LENGTH);
	if(m_nowShowString == NULL) 
	{
		XDELETE_ARRAY(m_tempNowString);
		XDELETE_ARRAY(m_nowString);
		XDELETE(m_mouseRightButtonMenu);
		return XFalse;
	}		
	m_nowShowString[0] = '\0';

	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //设置字体居中对齐
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//设置字体的颜色为黑色

	m_textPosition.set(m_mouseRect.left + TEXT_EDGE_WIDTH,m_mouseRect.top + m_mouseRect.getHeight() * 0.5f);			//文字显示的位置，是相对于控件的位置来定的
	m_textSize.set(strSize,strSize);				//文字显示的尺寸，这个尺寸会与空间的缩放尺寸叠加
	if(str != NULL)
	{
		m_caption.setString(str);
		strcpy(m_nowString,str);
		m_nowInsertPoint = strlen(m_nowString);	//当前选择输入的位置
		m_nowStringLength = strlen(m_nowString);	//当前输入字符串的长度
	}else
	{
		m_caption.setString(" ");
		m_nowString[0] = '\0';
		m_nowInsertPoint = 0;	//当前选择输入的位置
		m_nowStringLength = 0;	//当前输入字符串的长度
	}
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	//m_caption.m_isPassWord = 1;
//	m_selectText.setACopy(* font);
//	m_selectText.setString(" ");
//	m_selectText.setPosition(m_objRect.left + (m_edgeDistance.left + m_textPosition.x) * m_size.x,m_objRect.top + (m_edgeDistance.top + m_textPosition.y) * m_size.y);
//	m_selectText.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);

	m_haveSelect = XFalse;		//是否在字符串中有选择
	m_selectMouseDown = XFalse;
	m_selectStart = -1;		//选择的起始位置
	m_selectEnd = -1;		//选择的结束位置

	m_spriteBackGround.init(m_editNormal->texture.m_w,m_editNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteBackGround);
#endif
	m_spriteBackGround.setPosition(m_position);
	m_spriteBackGround.setSize(m_size);
	m_spriteBackGround.setIsTransformCenter(POINT_LEFT_TOP);

	m_spriteSelect.init(m_editSelect->texture.m_w,m_editSelect->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteSelect);
#endif
	m_spriteSelect.setPosition(m_position);
	m_spriteSelect.setSize(m_size);
	m_spriteSelect.setIsTransformCenter(POINT_LEFT_TOP);

	//计算输入框中可以显示的字符串的长度(这里取0.5，是因为显示英文字符但是却是使用的中文字体库)
	m_nowTextWidth = m_caption.getTextSize().x * m_caption.getSize().x * 0.5f;
	m_nowTextHeight = m_caption.getTextSize().y * m_caption.getSize().y;
	m_canShowLength = (m_mouseRect.getWidth() - 2.0f * TEXT_EDGE_WIDTH) * m_size.x / m_nowTextWidth;
	m_nowShowStart = 0;	//初始化当前显示的起始位置
	upDataShowStr();

	m_spriteInsert.init(m_editInsert->texture.m_w,m_editInsert->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteInsert);
#endif
	upDateInsertShowPosition();
	m_spriteInsert.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	m_spriteInsert.setIsTransformCenter(POINT_LEFT_TOP);

	//printf("Can Show String Length:%d\n",m_canShowLength);

	m_timer = 0;

	m_keyShiftState = 0;
	m_keyCapsLockState = 0;
	m_keyNumLockState = 1;
	m_keyCtrlState = 0;

	m_isVisiable = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;
	m_isBeChoose = XFalse; 

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_EDIT);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XEdit::initPlus(const char * path,			//控件的贴图
	const char *str,					//控件的初始化字符串
	const _XFontUnicode &font,			//控件的字体
	float strSize,	//控件的字体信息
	_XMouseRightButtonMenu * mouseMenu,
	_XResourcePosition resoursePosition)		//控件的右键菜单
{
	if(m_isInited) return XFalse;
	if(path == NULL) return XFalse;
	m_resInfo = _XResourceManager::GetInstance().loadResource(path,RESOURCE_TYPE_XEDIT_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	_XEditTexture * tex = (_XEditTexture *)m_resInfo->m_pointer;
	if(tex->editNormal == NULL || tex->editInsert == NULL || tex->editDisable == NULL || tex->editSelect == NULL) return XFalse;

	//需要注意的是输入范围必须要能显示最少一个字符，否则将会造成问题，目前这里并没有代码去判断，但是实际使用中需要注意这个问题
	if(tex->m_mouseRect.getWidth() <= 0 || tex->m_mouseRect.getHeight() <= 0) return XFalse;	//输入范围不能为空
	if(strSize <= 0) return XFalse;		//字符串的大小不能为非法值
	if(str != NULL && strlen(str) >= MAX_INPUT_STRING_LENGTH) return XFalse;
	if(mouseMenu != NULL)
	{//这里需要改进，这里使用指针，会受到别处修改的影响
		m_mouseRightButtonMenu = createMem<_XMouseRightButtonMenu>();
		if(m_mouseRightButtonMenu == NULL) return XFalse;	
		m_mouseRightButtonMenu->setACopy(* mouseMenu);
		//从控件管理器中将这个控件注销掉
		_XControlManager::GetInstance().decreaseAObject(m_mouseRightButtonMenu);	//注销这个物件
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(m_mouseRightButtonMenu);
#endif
	}

	//下面开始付值
	m_mouseRect = tex->m_mouseRect;	//记录鼠标的响应范围
	m_position.set(0.0f,0.0f);
	m_withoutTex = XFalse;

	m_editNormal = tex->editNormal;			//输入框普通状态
	m_editDisable = tex->editDisable;		//输入框无效状态
	m_editSelect = tex->editSelect;			//输入框片选颜色
	m_editInsert = tex->editInsert;			//输入框插入标记
	m_editUpon = tex->editUpon;			//输入框插入标记
	m_size.set(1.0f,1.0f);
	m_nowString = createArrayMem<char>(MAX_INPUT_STRING_LENGTH);
	if(m_nowString == NULL) 
	{
		XDELETE(m_mouseRightButtonMenu);
		return XFalse;
	}		
	m_nowString[0] = '\0';
	m_tempNowString = createArrayMem<char>(MAX_INPUT_STRING_LENGTH);
	if(m_tempNowString == NULL) 
	{
		XDELETE_ARRAY(m_nowString);
		XDELETE(m_mouseRightButtonMenu);
		return XFalse;
	}		
	m_tempNowString[0] = '\0';
//	m_insertString = createArrayMem<char>(MAX_INPUT_STRING_LENGTH);	//当前输入的字符串
//	if(m_insertString == NULL) return 0;
//	m_insertString[0] = '\0';
//	m_selectString = createArrayMem<char>(MAX_INPUT_STRING_LENGTH);	//当前输入的字符串
//	if(m_selectString == NULL) return 0;
//	m_selectString[0] = '\0';
	m_nowShowString = createArrayMem<char>(MAX_INPUT_STRING_LENGTH);
	if(m_nowShowString == NULL) 
	{
		XDELETE_ARRAY(m_tempNowString);
		XDELETE_ARRAY(m_nowString);
		XDELETE(m_mouseRightButtonMenu);
		return XFalse;
	}		
	m_nowShowString[0] = '\0';

	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //设置字体居中对齐
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//设置字体的颜色为黑色

	m_textPosition.set(m_mouseRect.left + TEXT_EDGE_WIDTH,m_mouseRect.top + m_mouseRect.getHeight() * 0.5f);			//文字显示的位置，是相对于控件的位置来定的
	m_textSize.set(strSize,strSize);				//文字显示的尺寸，这个尺寸会与空间的缩放尺寸叠加
	if(str != NULL)
	{
		m_caption.setString(str);
		strcpy(m_nowString,str);
		m_nowInsertPoint = strlen(m_nowString);	//当前选择输入的位置
		m_nowStringLength = strlen(m_nowString);	//当前输入字符串的长度
	}else
	{
		m_caption.setString(" ");
		m_nowString[0] = '\0';
		m_nowInsertPoint = 0;	//当前选择输入的位置
		m_nowStringLength = 0;	//当前输入字符串的长度
	}
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	//m_caption.m_isPassWord = 1;
//	m_selectText.setACopy(* font);
//	m_selectText.setString(" ");
//	m_selectText.setPosition(m_objRect.left + (m_edgeDistance.left + m_textPosition.x) * m_size.x,m_objRect.top + (m_edgeDistance.top + m_textPosition.y) * m_size.y);
//	m_selectText.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);

	m_haveSelect = XFalse;		//是否在字符串中有选择
	m_selectMouseDown = XFalse;
	m_selectStart = -1;		//选择的起始位置
	m_selectEnd = -1;		//选择的结束位置

	m_spriteBackGround.init(m_editNormal->texture.m_w,m_editNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteBackGround);
#endif
	m_spriteBackGround.setPosition(m_position);
	m_spriteBackGround.setSize(m_size);
	m_spriteBackGround.setIsTransformCenter(POINT_LEFT_TOP);

	m_spriteSelect.init(m_editSelect->texture.m_w,m_editSelect->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteSelect);
#endif
	m_spriteSelect.setPosition(m_position);
	m_spriteSelect.setSize(m_size);
	m_spriteSelect.setIsTransformCenter(POINT_LEFT_TOP);

	//计算输入框中可以显示的字符串的长度(这里取0.5，是因为显示英文字符但是却是使用的中文字体库)
	m_nowTextWidth = m_caption.getTextSize().x * m_caption.getSize().x * 0.5f;
	m_nowTextHeight = m_caption.getTextSize().y * m_caption.getSize().y;
	m_canShowLength = (m_mouseRect.getWidth() - 2.0f * TEXT_EDGE_WIDTH) * m_size.x / m_nowTextWidth;
	m_nowShowStart = 0;	//初始化当前显示的起始位置
	upDataShowStr();

	m_spriteInsert.init(m_editInsert->texture.m_w,m_editInsert->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteInsert);
#endif
	upDateInsertShowPosition();
	m_spriteInsert.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	m_spriteInsert.setIsTransformCenter(POINT_LEFT_TOP);

	//printf("Can Show String Length:%d\n",m_canShowLength);

	m_timer = 0;

	m_keyShiftState = 0;
	m_keyCapsLockState = 0;
	m_keyNumLockState = 1;
	m_keyCtrlState = 0;

	m_isVisiable = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;
	m_isBeChoose = XFalse; 

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_EDIT);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XEdit::initWithoutTex(const _XRect& area,
		const char *str,					//控件的初始化字符串
		const _XFontUnicode &font,			//控件的字体
		float strSize,	//控件的字体信息
		_XMouseRightButtonMenu * mouseMenu)
{
	if(m_isInited) return XFalse;

	//需要注意的是输入范围必须要能显示最少一个字符，否则将会造成问题，目前这里并没有代码去判断，但是实际使用中需要注意这个问题
	if(strSize <= 0) return XFalse;		//字符串的大小不能为非法值
	if(str != NULL && strlen(str) >= MAX_INPUT_STRING_LENGTH) return XFalse;
	if(mouseMenu != NULL)
	{//这里需要改进，这里使用指针，会受到别处修改的影响
		m_mouseRightButtonMenu = createMem<_XMouseRightButtonMenu>();
		if(m_mouseRightButtonMenu == NULL) return XFalse;	
		m_mouseRightButtonMenu->setACopy(* mouseMenu);
		//从控件管理器中将这个控件注销掉
		_XControlManager::GetInstance().decreaseAObject(m_mouseRightButtonMenu);	//注销这个物件
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(m_mouseRightButtonMenu);
#endif
	}

	//下面开始付值
	m_mouseRect = area;	//记录鼠标的响应范围
	m_position.set(0.0f,0.0f);
	m_withoutTex = XTrue;

	m_size.set(1.0f,1.0f);
	m_nowString = createArrayMem<char>(MAX_INPUT_STRING_LENGTH);
	if(m_nowString == NULL) 
	{
		XDELETE(m_mouseRightButtonMenu);
		return XFalse;
	}		
	m_nowString[0] = '\0';
	m_tempNowString = createArrayMem<char>(MAX_INPUT_STRING_LENGTH);
	if(m_tempNowString == NULL) 
	{
		XDELETE_ARRAY(m_nowString);
		XDELETE(m_mouseRightButtonMenu);
		return XFalse;
	}		
	m_tempNowString[0] = '\0';
	m_nowShowString = createArrayMem<char>(MAX_INPUT_STRING_LENGTH);
	if(m_nowShowString == NULL) 
	{
		XDELETE_ARRAY(m_tempNowString);
		XDELETE_ARRAY(m_nowString);
		XDELETE(m_mouseRightButtonMenu);
		return XFalse;
	}		
	m_nowShowString[0] = '\0';

	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //设置字体居中对齐
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//设置字体的颜色为黑色

	m_textPosition.set(m_mouseRect.left + TEXT_EDGE_WIDTH,m_mouseRect.top + m_mouseRect.getHeight() * 0.5f);			//文字显示的位置，是相对于控件的位置来定的
	m_textSize.set(strSize,strSize);				//文字显示的尺寸，这个尺寸会与空间的缩放尺寸叠加
	if(str != NULL)
	{
		m_caption.setString(str);
		strcpy(m_nowString,str);
		m_nowInsertPoint = strlen(m_nowString);	//当前选择输入的位置
		m_nowStringLength = strlen(m_nowString);	//当前输入字符串的长度
	}else
	{
		m_caption.setString(" ");
		m_nowString[0] = '\0';
		m_nowInsertPoint = 0;	//当前选择输入的位置
		m_nowStringLength = 0;	//当前输入字符串的长度
	}
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);

	m_haveSelect = 0;		//是否在字符串中有选择
	m_selectMouseDown = 0;
	m_selectStart = -1;		//选择的起始位置
	m_selectEnd = -1;		//选择的结束位置

	//计算输入框中可以显示的字符串的长度(这里取0.5，是因为显示英文字符但是却是使用的中文字体库)
	m_nowTextWidth = m_caption.getTextSize().x * m_caption.getSize().x * 0.5f;
	m_nowTextHeight = m_caption.getTextSize().y * m_caption.getSize().y;
	m_canShowLength = (m_mouseRect.getWidth() - 2.0f * TEXT_EDGE_WIDTH) * m_size.x / m_nowTextWidth;
	m_nowShowStart = 0;	//初始化当前显示的起始位置
	upDataShowStr();

	upDateInsertShowPosition();

	//printf("Can Show String Length:%d\n",m_canShowLength);
	m_timer = 0;

	m_keyShiftState = 0;
	m_keyCapsLockState = 0;
	m_keyNumLockState = 1;
	m_keyCtrlState = 0;

	m_isVisiable = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;
	m_isBeChoose = XFalse; 

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_EDIT);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = XTrue;
	return XTrue;
}
void _XEdit::upDataShowStr()
{//还没有实现
	if(m_nowStringLength <= m_nowShowStart + m_canShowLength) strcpy(m_nowShowString,m_nowString + m_nowShowStart);
	else
	{
		if(isAtUnicodeEnd(m_nowString,m_nowShowStart + m_canShowLength)) 
		{
			memcpy(m_nowShowString,m_nowString + m_nowShowStart,m_canShowLength);
			m_nowShowString[m_canShowLength] = '\0';
		}else 
		{
			memcpy(m_nowShowString,m_nowString + m_nowShowStart,m_canShowLength - 1);
			m_nowShowString[m_canShowLength - 1] = '\0';
		}
	}
	//跟新到显示中
	m_caption.setString(m_nowShowString);
}
void _XEdit::drawUp()				//描绘小菜单
{
	if(!m_isInited) return ;	//如果没有初始化直接退出
	if(!m_isVisiable) return;	//如果不可见直接退出
	//显示鼠标右键菜单
	if(m_mouseRightButtonMenu != NULL) m_mouseRightButtonMenu->draw();
}
void _XEdit::update(int stepTime)
{
	if(!m_isInited) return ;	//如果没有初始化直接退出
	if(!m_isVisiable) return;	//如果不可见直接退出
	if(m_isEnable && m_isBeChoose)
	{//输入光标闪烁的计时
		m_timer += XEE::frameTime;
		if(m_timer > 500) m_timer -= 500;
	}
}
void _XEdit::draw()
{
	if(!m_isInited) return ;	//如果没有初始化直接退出
	if(!m_isVisiable) return;	//如果不可见直接退出
	
	if(m_withoutTex)
	{
		if(!m_isEnable) 
		{
			drawFillBoxEx(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),_XVector2(m_mouseRect.getWidth() * m_size.x,m_mouseRect.getHeight() * m_size.y),0.75f,0.75f,0.75f);
		}else 
		{
			drawFillBoxEx(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),_XVector2(m_mouseRect.getWidth() * m_size.x,m_mouseRect.getHeight() * m_size.y),0.5f,0.5f,0.5f);
		}

		if(m_haveSelect) 
		{
			drawFillBoxEx(_XVector2(m_selectRect.left,m_selectRect.top),_XVector2(m_selectRect.right,m_selectRect.bottom),0.85f,0.85f,0.85f);;
		}

		//显示当前输入的字符串
		m_caption.draw();
		if(m_isEnable && m_isBeChoose)
		{
			if(m_timer > 250) 
			{
				float x = m_position.x + m_textPosition.x * m_size.x + (m_nowInsertPoint - m_nowShowStart) * m_nowTextWidth;
				float y = m_position.y + m_textPosition.y * m_size.y - m_nowTextHeight * 0.5f;
				drawLine(x,y,x,y + m_nowTextHeight,1,0.25f,0.25f,0.25f);
				//m_spriteInsert.draw(m_editInsert);
			}
		}
		//不画上层
	//	if(m_editUpon != NULL) 
	//	{
	//		m_spriteBackGround.draw(m_editUpon);
	//	}
	}else
	{
		if(!m_isEnable) m_spriteBackGround.draw(m_editDisable);	//如果无效则显示无效,不显示插入符号
		else m_spriteBackGround.draw(m_editNormal);//如果有效则显示有效,显示插入符号

		if(m_haveSelect) m_spriteSelect.draw(m_editSelect);//显示选择的范围

		//显示当前输入的字符串
		m_caption.draw();
		if(m_isEnable && m_isBeChoose && m_timer > 250)
			m_spriteInsert.draw(m_editInsert);

		if(m_editUpon != NULL) m_spriteBackGround.draw(m_editUpon);	
	}
}
void _XEdit::release()
{
	if(!m_isInited) return;
	XDELETE_ARRAY(m_nowString);
	XDELETE_ARRAY(m_tempNowString);
	//XDELETE_ARRAY(m_insertString);
	//XDELETE_ARRAY(m_selectString);
	XDELETE_ARRAY(m_nowShowString);
	XDELETE(m_mouseRightButtonMenu);
	_XControlManager::GetInstance().decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(this);
#endif
	if(m_resInfo != NULL)
	{
		_XResourceManager::GetInstance().releaseResource(m_resInfo);
		m_resInfo = NULL;
	}
	m_isInited = XFalse;
}
void _XEdit::setString(const char *str)
{//这个函数面临很复杂的逻辑环境，如果用户正在处于输入阶段或者正在处于片选阶段的时候这个修改将会造成较为大的影响，所以这里可能会做一些不人性化的处理
	if(str == NULL) return;
	m_caption.setString(str);
	strcpy(m_nowString,str);
	m_nowInsertPoint = strlen(m_nowString);	//当前选择输入的位置
	m_nowStringLength = strlen(m_nowString);	//当前输入字符串的长度
	//取消所有片选的信息
	m_haveSelect = XFalse;		//是否在字符串中有选择
	m_selectMouseDown = XFalse;
	m_selectStart = -1;		//选择的起始位置
	m_selectEnd = -1;		//选择的结束位置
	//计算输入框中可以显示的字符串的长度(这里取0.5，是因为显示英文字符但是却是使用的中文字体库)
	m_canShowLength = (m_mouseRect.getWidth() - 2.0f * TEXT_EDGE_WIDTH) * m_size.x / m_nowTextWidth;
	m_nowShowStart = 0;	//初始化当前显示的起始位置
	upDataShowStr();

	upDateInsertShowPosition();
}
void _XEdit::upDateInsertShowPosition()
{//下面*0.5是因为只显示英文，但是却使用中文字体库，所以宽度要缩小到0.5
	if(!m_withoutTex)
	{
		m_spriteInsert.setPosition(m_position.x + m_textPosition.x * m_size.x 
			+ (m_nowInsertPoint - m_nowShowStart) * m_nowTextWidth,
			m_position.y + m_textPosition.y * m_size.y - m_nowTextHeight * 0.5f);
	}
}
void _XEdit::deleteSelectStr()
{
	if(getSelectEnd() == m_nowStringLength)
	{//片选到结束
		m_nowStringLength -= getSelectLength();
		m_nowString[getSelectHead()] = '\0';
		changeInsertPoint(getSelectHead() - m_nowInsertPoint);
	}else
	{
		strcpy(m_nowString + getSelectHead(),m_nowString + getSelectEnd());
		m_nowStringLength -= getSelectLength();
		changeInsertPoint(getSelectHead() - m_nowInsertPoint);
	}
}
void _XEdit::mouseRightMenuProc()
{
	int TextChange = 0;
	if(m_mouseRightButtonMenu->getLastChoose() == 0)
	{//剪切
		if(m_haveSelect)
		{//存在片选，没有片选则什么也不作
			//1、拷贝片选的内容
			memcpy(copyString,m_nowString + getSelectHead(),getSelectLength());
			copyString[getSelectLength()] = '\0';
			//2、删除片选的内容
			deleteSelectStr();
			m_haveSelect = XFalse;	//片选结束
			TextChange = 1;
		}
	}else
	if(m_mouseRightButtonMenu->getLastChoose() == 1)
	{//复制
		if(m_haveSelect)
		{//存在片选，没有片选则什么也不作
			//拷贝片选的内容
			memcpy(copyString,m_nowString + getSelectHead(),getSelectLength());
			copyString[getSelectLength()] = '\0';
		}
	}else
	if(m_mouseRightButtonMenu->getLastChoose() == 2)
	{//粘贴
		//判断公用内存空间中是否有内容
		if(copyString[0] != '\0')
		{
			char haveEnd = 0;
			for(int i = 0;i < MAX_INPUT_STRING_LENGTH;++ i)
			{
				if(copyString[i] == '\0')
				{
					haveEnd = 1;
					break;
				}
			}
			if(haveEnd != 0)
			{
				if(m_haveSelect)
				{//存在片选(这里实际是替换动作)
					if(m_nowStringLength + strlen(copyString) - getSelectLength() > MAX_INPUT_STRING_LENGTH - 2)
					{//如果超出总长度，则这里需要谨慎处理，目前定义为不作处理
					}else
					{							
						//1、删除片选内容
						deleteSelectStr();
						m_haveSelect = XFalse;	//片选结束
						//2、插入复制内容
						if(m_nowInsertPoint == m_nowStringLength)
						{//如果是在字符串的末尾添加
							strcat(m_nowString,copyString);
							m_nowStringLength = strlen(m_nowString);
							//m_nowInsertPoint = m_nowStringLength;
							changeInsertPoint(m_nowStringLength - m_nowInsertPoint);
							TextChange = 1;
						}else
						{
							strcpy(m_tempNowString,m_nowString + m_nowInsertPoint);
							strcpy(m_nowString + m_nowInsertPoint,copyString);
							strcat(m_nowString,m_tempNowString);
							m_nowStringLength = strlen(m_nowString);
							//m_nowInsertPoint += strlen(copyString);
							changeInsertPoint(strlen(copyString));
							TextChange = 1;
						}
					}
				}else
				{//没有片选
					//判断粘贴的内容是否会超出字符串的总长度
					if(m_nowStringLength + strlen(copyString) > MAX_INPUT_STRING_LENGTH - 2)
					{//如果超出总长度，则这里需要谨慎处理，目前定义为不作处理
					}else
					{
						if(m_nowInsertPoint == m_nowStringLength)
						{//如果是在字符串的末尾添加
							strcat(m_nowString,copyString);
							m_nowStringLength = strlen(m_nowString);
						//	m_nowInsertPoint = m_nowStringLength;
							changeInsertPoint(m_nowStringLength - m_nowInsertPoint);
							TextChange = 1;
						}else
						{
							strcpy(m_tempNowString,m_nowString + m_nowInsertPoint);
							strcpy(m_nowString + m_nowInsertPoint,copyString);
							strcat(m_nowString,m_tempNowString);
							m_nowStringLength = strlen(m_nowString);
							//m_nowInsertPoint += strlen(copyString);
							changeInsertPoint(strlen(copyString));
							TextChange = 1;
						}
					}
				}
			}
		}
	}else
	if(m_mouseRightButtonMenu->getLastChoose() == 3)
	{//撤销(如果存在片选择取消片选)
		if(m_haveSelect) m_haveSelect = XFalse;
	}
	printf("%d\n",m_mouseRightButtonMenu->getLastChoose());
	if(TextChange != 0)
	{
		upDataShowStr();	//因为字符串被改变，所以这里需要更新字符的显示
	//	m_caption.setString(m_nowString);
	//	m_spriteInsert.setPosition(m_objRect.left + (m_edgeDistance.left + m_textPosition.x) * m_size.x + m_nowInsertPoint * m_caption.m_size.x * m_caption.m_showSize.x * 0.5,
	//		m_objRect.top + (m_edgeDistance.top + m_textPosition.y) * m_size.y);
		if(m_funInputChenge != NULL) m_funInputChenge(m_pClass,m_objectID);
	}
}
void _XEdit::changeInsertPoint(int sum)
{
	if(sum == 0) return;
	int tempNewPosition = m_nowInsertPoint + sum;
	if(tempNewPosition < 0) return;
	if(tempNewPosition > m_nowStringLength) return;
	//这里需要检查中文的问题
	if(!isAtUnicodeEnd(m_nowString,tempNewPosition)) 
	{//如果遇到中文临界就多移动一位,(这个方法不对)
		if(sum < 0) changeInsertPoint(sum - 1);//后退
		else changeInsertPoint(sum + 1);//前进
		return;
	}
	//可以移动
	int tempOldPosition = m_nowInsertPoint;		//旧光标的位置
	m_nowInsertPoint = tempNewPosition;
	//检查是否要移动显示数据
	if(m_nowInsertPoint < m_nowShowStart || m_nowInsertPoint > m_nowShowStart + m_canShowLength)
	{//超出显示范围需要修改显示范围
		if(m_nowInsertPoint < m_nowShowStart)
		{//向前移动
			if(tempOldPosition != m_nowShowStart)
			{//如果之前光标没有在输入框的开头，则需要跟新到开头
				m_nowShowStart = m_nowInsertPoint;
				upDateInsertShowPosition();
			}else
			{
				m_nowShowStart = m_nowInsertPoint;
			}
		}else
		if(m_nowInsertPoint > m_nowShowStart + m_canShowLength)
		{//向后移动
			if(tempOldPosition != m_nowShowStart + m_canShowLength)
			{//如果之前的光标不在输入框结尾则需要更新光标到结尾
				m_nowShowStart = m_nowInsertPoint - m_canShowLength;
				if(!isAtUnicodeEnd(m_nowString,m_nowShowStart)) ++m_nowShowStart; //如果头临界、则多移动一位
				upDateInsertShowPosition();
			}else
			{
				m_nowShowStart = m_nowInsertPoint - m_canShowLength;
				if(!isAtUnicodeEnd(m_nowString,m_nowShowStart)) ++m_nowShowStart; //如果头临界、则多移动一位
				upDateInsertShowPosition();
			}
		}
		upDataShowStr();
	}else
	{//不需要修改显示范围
		upDateInsertShowPosition();
	}
}
void _XEdit::upDataSelectShow()
{
	int tempHead = getSelectHead();
	int tempEnd = getSelectEnd();
	if(tempHead < m_nowShowStart) tempHead = m_nowShowStart;
	if(tempHead >= m_nowShowStart + m_canShowLength)
	{//这个情况不需要显示
		tempHead = m_nowShowStart + m_canShowLength;
	}
	if(tempEnd <= m_nowShowStart)
	{//这种情况也不需要显示
		tempEnd = m_nowShowStart;
	}
	if(tempEnd > m_nowShowStart + m_canShowLength) tempEnd = m_nowShowStart + m_canShowLength;
	//更具实际的情况计算选区位置和选区图片需要放置的尺寸
	if(m_withoutTex)
	{
		if(tempEnd - tempHead < m_canShowLength)
		{
			m_selectRect.set(m_position.x + m_textPosition.x * m_size.x + (tempHead - m_nowShowStart) * m_nowTextWidth,
				m_position.y + m_textPosition.y * m_size.y - m_nowTextHeight * 0.5f,(tempEnd - tempHead) * m_nowTextWidth,m_nowTextHeight);
		}else
		{
			m_selectRect.set(m_position.x + m_textPosition.x * m_size.x + (tempHead - m_nowShowStart) * m_nowTextWidth,
				m_position.y + m_textPosition.y * m_size.y - m_nowTextHeight * 0.5f,m_canShowLength * m_nowTextWidth,m_nowTextHeight);
		}
	}else
	{
		m_spriteSelect.setPosition(m_position.x + m_textPosition.x * m_size.x + (tempHead - m_nowShowStart) * m_nowTextWidth,
			m_position.y + m_textPosition.y * m_size.y - m_nowTextHeight * 0.5f);
		if(tempEnd - tempHead < m_canShowLength)
		{
			m_spriteSelect.setSize((tempEnd - tempHead) * m_nowTextWidth /m_editSelect->textureSize.x,
				m_nowTextHeight / m_editSelect->textureSize.y);
		}else
		{
			m_spriteSelect.setSize(m_canShowLength * m_nowTextWidth /m_editSelect->textureSize.x,
				m_nowTextHeight / m_editSelect->textureSize.y);
		}
	}
}
_XBool _XEdit::canGetFocus(float x,float y)
{
	if(!m_isInited) return XFalse;	//如果没有初始化直接退出
	if(!m_isActive) return XFalse;		//没有激活的控件不接收控制
	if(!m_isVisiable) return XFalse;	//如果不可见直接退出
	if(!m_isEnable) return XFalse;		//如果无效则直接退出
	return isInRect(x,y);
}
_XBool _XEdit::canLostFocus(float x,float y)
{
	if(!m_isInited) return XTrue;	//如果没有初始化直接退出
	if(!m_isActive) return XTrue;		//没有激活的控件不接收控制
	if(!m_isVisiable) return XTrue;	//如果不可见直接退出
	if(!m_isEnable) return XTrue;		//如果无效则直接退出
	if(m_mouseRightButtonMenu != NULL && m_mouseRightButtonMenu->getVisiable() != 0) return 0;
	//if(m_isBeChoose != 0) return 0;
	return 1;
}
_XBool _XEdit::mouseProc(float x,float y,_XMouseState mouseState)
{
	if(!m_isInited) return XFalse;	//如果没有初始化直接退出
	if(!m_isActive) return XFalse;		//没有激活的控件不接收控制
	if(!m_isVisiable) return XFalse;	//如果不可见直接退出
	if(!m_isEnable) return XFalse;		//如果无效则直接退出
	//计算鼠标响应区域的位置
//	_XRect temp;
//	temp.set(m_objRect.left,m_objRect.top,m_objRect.left + m_objRect.getWidth() * m_size.x,m_objRect.top + m_objRect.getHeight() * m_size.y);
	if(m_mouseRightButtonMenu != NULL)
	{//右键菜单处理在这里进行
		int  flagMouseMenu = 0;
		if(m_mouseRightButtonMenu->getVisiable()) flagMouseMenu = 1;
		if(mouseState == MOUSE_RIGHT_BUTTON_UP)
		{//如果是右键单击产生动作需要在空间范围内
			if(m_nowMouseRect.isInRect(x,y) && m_isBeChoose)
			{
				m_mouseRightButtonMenu->mouseProc(x,y,mouseState);
			}
		}else
		{
			m_mouseRightButtonMenu->mouseProc(x,y,mouseState);
		}
		if(flagMouseMenu == 1 && m_mouseRightButtonMenu->getVisiable() == 0)
		{//按下了快捷键(这里实现对应菜单项的功能)
			mouseRightMenuProc();//右键菜单的响应
			if(m_mouseRightButtonMenu->getLastChoose() !=  -1)
			{//如果菜单有操作，则这里的消息不后传
				return 1;	//对右键菜单的操作生效的话，则这个鼠标消息不会左重复处理
			}
		}
	} 
	if(m_nowMouseRect.isInRect(x,y))
	{//鼠标在范围内点击
		//下面是左右键都可以出发
		if(mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_RIGHT_BUTTON_DOWN)
		{//点击	
			if(!m_isBeChoose)	m_isBeChoose = XTrue;//如果控件没有处于激活状态则激活

			if(m_nowStringLength > 0)
			{//字符串中有字符时，可以同通过鼠标选择插入位置(这里的常数10是为了方便选择头或者尾)
				if(m_nowMouseRect.isInRect(x,y))
				{//鼠标按下动作有效
					//计算插入位置
					if(x <= m_position.x + m_textPosition.x * m_size.x)
					{//头
						if(m_nowInsertPoint != m_nowShowStart) changeInsertPoint(m_nowShowStart - m_nowInsertPoint);
					}else
					if(x >= m_position.x + (m_mouseRect.right - TEXT_EDGE_WIDTH) * m_size.x)
					{//尾
						if(m_nowStringLength <= m_canShowLength)
						{
							if(m_nowInsertPoint != m_nowStringLength) changeInsertPoint(m_nowStringLength - m_nowInsertPoint);
						}else
						{//则到显示的结束
							if(m_nowInsertPoint != m_nowShowStart + m_canShowLength) changeInsertPoint(m_nowShowStart + m_canShowLength - m_nowInsertPoint);
						}
					}else
					{//范围内
						//这里使用0.5的常数是因为目前使用的是英文输入但是使用的字体是中文字体库，所以要加少一半的占用像素
						//为了提高选择时的感觉，这里错位半个字符的宽度，使选择的时候更具有感觉
						int tempInsertPoint = (x + m_nowTextWidth * 0.5f - m_position.x - m_textPosition.x * m_size.x) / m_nowTextWidth;
						if(tempInsertPoint + m_nowShowStart > m_nowStringLength)
						{
							if(m_nowInsertPoint != m_nowStringLength) changeInsertPoint(m_nowStringLength - m_nowInsertPoint);
						}else
						{
							if(m_nowInsertPoint != tempInsertPoint + m_nowShowStart) changeInsertPoint(tempInsertPoint + m_nowShowStart - m_nowInsertPoint);
						}
					}
					if(mouseState == MOUSE_LEFT_BUTTON_DOWN)
					{//如果是左键按下，则可能是片选属性
						m_haveSelect = XFalse;
						m_selectEnd = m_nowInsertPoint;
						m_selectStart = m_nowInsertPoint;
						m_selectMouseDown = XTrue;
					}else
					if(mouseState == MOUSE_RIGHT_BUTTON_DOWN)
					{//如果是右键按下操作不再范围内也将会造成片选结束
						if(m_nowInsertPoint > getSelectEnd() || m_nowInsertPoint < getSelectHead())
						{
							m_haveSelect = XFalse;
						}
					}
				}
			}
		}
	}else
	{//如果在范围外点击则去激活
		if(mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_RIGHT_BUTTON_DOWN)
		{//点击
			if(m_isBeChoose) m_isBeChoose = XFalse;//如果控件没有处于激活状态则激活
		}
	}

	if(mouseState == MOUSE_LEFT_BUTTON_DOWN)
	{//任何鼠标左键按下操作都回造成片选结束
		if(m_haveSelect) m_haveSelect = XFalse;
	}

	if(mouseState == MOUSE_MOVE)
	{
		if(m_selectMouseDown)
		{//如果鼠标按下，并且拖动，则作为片选动作处理
			//计算片选的位置
			if(x <= m_position.x + m_textPosition.x * m_size.x)
			{//头
				if(m_nowShowStart > 0 && x <= m_position.x + m_textPosition.x * m_size.x - TEXT_EDGE_WIDTH) 
				{//如果超过头，可以向前移动
					m_selectEnd = m_nowShowStart - 1;
					if(!isAtUnicodeEnd(m_nowString,m_selectEnd)) --m_selectEnd;
				}else 
				{
					m_selectEnd = m_nowShowStart;
				}
			}else
			if(x >= m_position.x + (m_mouseRect.right - TEXT_EDGE_WIDTH) * m_size.x)
			{//尾
				if(m_nowShowStart + m_canShowLength >= m_nowStringLength)
				{//如果字符串没有显示满
					m_selectEnd = m_nowStringLength;
				}else
				{
					if(x >= m_position.x + (m_mouseRect.right - TEXT_EDGE_WIDTH) * m_size.x + TEXT_EDGE_WIDTH)
					{//如果超过尾可以向后移动
						m_selectEnd = m_nowShowStart + m_canShowLength + 1;
						if(!isAtUnicodeEnd(m_nowString,m_selectEnd)) ++m_selectEnd;
					}else
					{
						m_selectEnd = m_nowShowStart + m_canShowLength;
					}
				}
			}else
			{//这里使用0.5的常数是因为目前使用的是英文输入但是使用的字体是中文字体库，所以要加少一半的占用像素
				int tempEnd = m_selectEnd;
				int tempPosition = (x - m_position.x - m_textPosition.x * m_size.x) / m_nowTextWidth;
				if(tempPosition + m_nowShowStart >= m_nowStringLength) m_selectEnd = m_nowStringLength;
				else m_selectEnd = m_nowShowStart + tempPosition;

				if(!isAtUnicodeEnd(m_nowString,m_selectEnd))
				{
					if(m_selectEnd > tempEnd) ++m_selectEnd;
					else --m_selectEnd;
				}
			}
			if(m_selectEnd != m_nowInsertPoint) changeInsertPoint(m_selectEnd - m_nowInsertPoint);	//插入符号也要跟随变化
			if(m_selectEnd != m_selectStart)
			{//片选有效
				m_haveSelect = XTrue;
				upDataSelectShow();
			}else m_haveSelect = XFalse;	//片选无效
		}
	}	
	if(m_isBeChoose && (mouseState == MOUSE_LEFT_BUTTON_UP || mouseState == MOUSE_RIGHT_BUTTON_UP))
	{//如果是弹起事件，则不论是否在范围内，只要空间处于激活状态，则会做出反应
		if(m_selectMouseDown)
		{//片选结束判断
			if(m_selectEnd != m_selectStart) m_haveSelect = XTrue;	//片选有效
			else m_haveSelect = XFalse;	//片选无效
			m_selectMouseDown = XFalse;
		}
	}
	return XTrue;
}
_XBool _XEdit::keyboardProc(int keyOrder,_XKeyState keyState)
{
	if(!m_isInited) return XFalse;	//如果没有初始化直接退出
	if(!m_isActive) return XFalse;		//没有激活的控件不接收控制
	if(!m_isVisiable) return XFalse;	//如果不可见直接退出
	if(!m_isEnable) return XFalse;		//如果无效则直接退出

	if(m_mouseRightButtonMenu != NULL)
	{
		if(m_mouseRightButtonMenu->getVisiable())
		{//如果右键菜单可见,则所有按键都不会对输入框造成影响
			m_mouseRightButtonMenu->keyboardProc(keyOrder,keyState);
			if(!m_mouseRightButtonMenu->getVisiable())
			{//按下了快捷键(这里实现对应菜单项的功能)
				mouseRightMenuProc();//右键菜单的响应
			}
			return XFalse;
		}
	}
	
	if(keyState == KEY_STATE_DOWN && m_isBeChoose)
	{
	//	printf("down %d\n",keyOrder);
		if(keyOrder ==  XKEY_RSHIFT || keyOrder ==  XKEY_LSHIFT)
		{//shift键按下
			m_keyShiftState = 1;
		}else
		if(keyOrder ==  XKEY_CAPSLOCK)
		{//CapsLock键按下
			m_keyCapsLockState = 1;
		}else
		if(keyOrder ==  XKEY_NUMLOCK)
		{//CapsLock键按下
			m_keyNumLockState = 1;
		}else
		if(keyOrder ==  XKEY_RCTRL || keyOrder ==  XKEY_LCTRL)
		{//Ctrl键按下
			m_keyCtrlState = 1;
		}
	}

	int TextChange = 0;		//文本内容是否发生变化
	if(keyState == KEY_STATE_UP && m_isBeChoose)
	{//按键弹起时才作相应
		if(m_keyCtrlState == 0)
		{//ctrl没有按下的所有输入才能处理
			if(m_haveSelect == 0)
			{//没有片选的时候
				if(m_nowStringLength < MAX_INPUT_STRING_LENGTH - 2)
				{//如果输入长度小于最大长度，则可以继续输入，否则只能输入一些功能
					//这里是可打印字符的处理			
					TextChange = keyJudgement(keyOrder);
				}
			}else
			{//存在片选的时候
				if(m_nowStringLength - getSelectLength() < MAX_INPUT_STRING_LENGTH - 2)
				{//如果输入长度小于最大长度，则可以继续输入，否则只能输入一些功能
					//这里是可打印字符的处理			
					TextChange = keyJudgement(keyOrder);
				}
			}
		}else
		{//ctrl按下的所有输入作为功能键处理
			if(keyOrder ==  XKEY_C)
			{//ctrl + c 复制
				if(m_haveSelect)
				{//存在片选，没有片选则什么也不作
					//拷贝片选的内容
					memcpy(copyString,m_nowString + getSelectHead(),getSelectLength());
					copyString[getSelectLength()] = '\0';
				}
			}else
			if(keyOrder ==  XKEY_V)
			{//ctrl + v 粘贴
				//判断公用内存空间中是否有内容
				if(copyString[0] != '\0')
				{
					char haveEnd = 0;
					for(int i = 0;i < MAX_INPUT_STRING_LENGTH;++ i)
					{
						if(copyString[i] == '\0')
						{
							haveEnd = 1;
							break;
						}
					}
					if(haveEnd != 0)
					{
						if(m_haveSelect)
						{//存在片选(这里实际是替换动作)
							if(m_nowStringLength + strlen(copyString) - getSelectLength() > MAX_INPUT_STRING_LENGTH - 2)
							{//如果超出总长度，则这里需要谨慎处理，目前定义为不作处理
							}else
							{							
								//1、删除片选内容
								deleteSelectStr();
								m_haveSelect = XFalse;	//片选结束
								//2、插入复制内容
								if(m_nowInsertPoint == m_nowStringLength)
								{//如果是在字符串的末尾添加
									strcat(m_nowString,copyString);
									m_nowStringLength = strlen(m_nowString);
									//m_nowInsertPoint = m_nowStringLength;
									changeInsertPoint(m_nowStringLength - m_nowInsertPoint);
									TextChange = 1;
								}else
								{
									strcpy(m_tempNowString,m_nowString + m_nowInsertPoint);
									strcpy(m_nowString + m_nowInsertPoint,copyString);
									strcat(m_nowString,m_tempNowString);
									m_nowStringLength = strlen(m_nowString);
									//m_nowInsertPoint += strlen(copyString);
									changeInsertPoint(strlen(copyString));
									TextChange = 1;
								}
							}
						}else
						{//没有片选
							//判断粘贴的内容是否会超出字符串的总长度
							if(m_nowStringLength + strlen(copyString) > MAX_INPUT_STRING_LENGTH - 2)
							{//如果超出总长度，则这里需要谨慎处理，目前定义为不作处理
							}else
							{
								if(m_nowInsertPoint == m_nowStringLength)
								{//如果是在字符串的末尾添加
									strcat(m_nowString,copyString);
									m_nowStringLength = strlen(m_nowString);
								//	m_nowInsertPoint = m_nowStringLength;
									changeInsertPoint(m_nowStringLength - m_nowInsertPoint);
									TextChange = 1;
								}else
								{
									strcpy(m_tempNowString,m_nowString + m_nowInsertPoint);
									strcpy(m_nowString + m_nowInsertPoint,copyString);
									strcat(m_nowString,m_tempNowString);
									m_nowStringLength = strlen(m_nowString);
									//m_nowInsertPoint += strlen(copyString);
									changeInsertPoint(strlen(copyString));
									TextChange = 1;
								}
							}
						}
					}
				}			
			}
		}
		if(keyOrder ==  XKEY_BACKSPACE)
		{//回退键
			if(!m_haveSelect)
			{//没有片选的时候
				if(m_nowInsertPoint > 0)
				{
					if(!isAtUnicodeEnd(m_nowString,m_nowInsertPoint - 1))
					{
						m_nowStringLength -= 2;
						changeInsertPoint(-2);
						if(m_nowStringLength == m_nowInsertPoint) m_nowString[m_nowInsertPoint] = '\0';	//字符串末尾
						else strcpy(m_nowString + m_nowInsertPoint,m_nowString + m_nowInsertPoint + 2);
					}else
					{
						-- m_nowStringLength;
						changeInsertPoint(-1);
						if(m_nowStringLength == m_nowInsertPoint) m_nowString[m_nowInsertPoint] = '\0';	//字符串末尾
						else strcpy(m_nowString + m_nowInsertPoint,m_nowString + m_nowInsertPoint + 1);
					}
					TextChange = 1;
				}
			}else
			{//存在片选的时候
				if(!m_selectMouseDown)
				{//片选已经结束
					//1、删除片选内容
					deleteSelectStr();
					m_haveSelect = XFalse;	//片选结束
					TextChange = 1;
				}else
				{//片选尚未结束
					//作结束动作
					if(m_selectEnd != m_selectStart)
					{//片选有效
						m_haveSelect = XTrue;
						//1、删除片选内容
						deleteSelectStr();
					}else m_haveSelect = XFalse;	//片选无效

					m_haveSelect = XFalse;	//片选结束
					TextChange = 1;
					//然后再作重新开始动作
					m_selectEnd = m_nowInsertPoint;
					m_selectStart = m_nowInsertPoint;
					m_selectMouseDown = XTrue;
				}
			}
		}else
		if(keyOrder ==  XKEY_DELETE || (m_keyShiftState == m_keyNumLockState && keyOrder ==  XKEY_N_PERIOD))
		{//Delete按键
			if(!m_haveSelect)
			{//没有片选的时候
				if(m_nowStringLength == m_nowInsertPoint)
				{//字符串末尾不作处理
				}else
				{
					//这里需要判断后面那个字符是否为中文，如果为中文的话，则需要删除两个字符
					if(m_nowString[m_nowInsertPoint] < 0)
					{
						strcpy(m_nowString + m_nowInsertPoint,m_nowString + m_nowInsertPoint + 2);
						m_nowStringLength -= 2;
					}else
					{
						strcpy(m_nowString + m_nowInsertPoint,m_nowString + m_nowInsertPoint + 1);
						-- m_nowStringLength;
					}
					TextChange = 1;
				}
			}else
			{//存在片选的时候
				if(!m_selectMouseDown)
				{//片选已经结束
					//1、删除片选内容
					deleteSelectStr();
					m_haveSelect = XFalse;	//片选结束
					TextChange = 1;
				}else
				{//片选尚未结束
					//作结束动作
					if(m_selectEnd != m_selectStart)
					{//片选有效
						m_haveSelect = XTrue;
						//1、删除片选内容
						deleteSelectStr();
					}else m_haveSelect = XFalse;	//片选无效

					TextChange = 1;
					//然后再作重新开始动作
					m_haveSelect = XFalse;
					m_selectEnd = m_nowInsertPoint;
					m_selectStart = m_nowInsertPoint;
					m_selectMouseDown = XTrue;
				}
			}
		}else
		if(keyOrder ==  XKEY_RSHIFT || keyOrder ==  XKEY_LSHIFT)
		{//shift键弹起
			m_keyShiftState = 0;
		}else
		if(keyOrder ==  XKEY_CAPSLOCK)
		{//CapsLock键弹起
			m_keyCapsLockState = 0;
		}else
		if(keyOrder ==  XKEY_NUMLOCK)
		{//NumLock键弹起
			m_keyNumLockState = 0;
		}else
		if(keyOrder ==  XKEY_RCTRL || keyOrder ==  XKEY_LCTRL)
		{//Ctrl键弹起
			m_keyCtrlState = 0;
		}else
		if(keyOrder ==  XKEY_MENU)
		{//菜单键被按下，则相当于是鼠标右击
			//相当于是鼠标右击事件
			m_mouseRightButtonMenu->mouseProc(m_position.x + m_textPosition.x * m_size.x + (m_nowInsertPoint - m_nowShowStart) * m_nowTextWidth,
				m_position.y + m_textPosition.y * m_size.y,MOUSE_RIGHT_BUTTON_UP);
		}else
		if(keyOrder ==  XKEY_LEFT || (m_keyShiftState == m_keyNumLockState && keyOrder ==  XKEY_N4))
		{//向左案件弹起(字符串不会改变，但是会改变插入的位置)
			if(m_keyShiftState == 0)
			{//如果没有按下shift键
				if(!m_selectMouseDown)
				{
					if(!m_haveSelect) changeInsertPoint(-1);	//没有片选,移动光标的位置
					else
					{//存在片选
					//	m_nowInsertPoint = m_selectStart;
					//	m_spriteInsert.setPosition(m_objRect.left + (m_edgeDistance.left + m_textPosition.x) * m_size.x + m_nowInsertPoint * m_caption.m_size.x * m_caption.m_showSize.x * 0.5,
					//		m_objRect.top + (m_edgeDistance.top + m_textPosition.y) * m_size.y);
						changeInsertPoint(m_selectStart - m_nowInsertPoint);
						m_haveSelect = XFalse;
					}
				}
			}else
			{//如果按下shift键则为片选动作
				if(!m_haveSelect)
				{//标记开始片选
					m_haveSelect = XFalse;
					m_selectEnd = m_nowInsertPoint;
					m_selectStart = m_nowInsertPoint;
					//然后移动光标，并修改片选状态
					if(m_nowInsertPoint > 0)
					{
						changeInsertPoint(-1);
						m_selectEnd = m_nowInsertPoint;
					}
					if(m_selectEnd != m_selectStart)
					{//片选有效
						m_haveSelect = XTrue;
						upDataSelectShow();
					}else m_haveSelect = XFalse;//片选无效
				}else
				{//修改片选结果
					//然后移动光标，并修改片选状态
					if(m_nowInsertPoint > 0)
					{
						changeInsertPoint(-1);
						m_selectEnd = m_nowInsertPoint;
					}
					if(m_selectEnd != m_selectStart)
					{//片选有效
						m_haveSelect = XTrue;
						upDataSelectShow();
					}else m_haveSelect = XFalse;//片选无效
				}
			}
		}else
		if(keyOrder ==  XKEY_RIGHT || (m_keyShiftState == m_keyNumLockState && keyOrder ==  XKEY_N6))
		{//向左案件弹起(字符串不会改变，但是会改变插入的位置)
			if(m_keyShiftState == 0)
			{//如果没有按下shift键
				if(!m_selectMouseDown)
				{
					if(!m_haveSelect) changeInsertPoint(1);	//没有片选,移动光标的位置
					else
					{//存在片选
						changeInsertPoint(m_selectEnd - m_nowInsertPoint);
						m_haveSelect = XFalse;
					}
				}
			}else
			{//如果按下shift键则为片选动作
				if(!m_haveSelect)
				{//标记开始片选
					m_haveSelect = XFalse;
					m_selectEnd = m_nowInsertPoint;
					m_selectStart = m_nowInsertPoint;
					//然后移动光标，并修改片选状态
					if(m_nowInsertPoint < m_nowStringLength)
					{
						changeInsertPoint(1);
						m_selectEnd = m_nowInsertPoint;
					}
					if(m_selectEnd != m_selectStart)
					{//片选有效
						m_haveSelect = XTrue;
						upDataSelectShow();
					}else m_haveSelect = XFalse;//片选无效
				}else
				{//修改片选结果
					//然后移动光标，并修改片选状态
					if(m_nowInsertPoint < m_nowStringLength)
					{
						changeInsertPoint(1);
						m_selectEnd = m_nowInsertPoint;
					}
					if(m_selectEnd != m_selectStart)
					{//片选有效
						m_haveSelect = XTrue;
						upDataSelectShow();
					}else m_haveSelect = XFalse;	//片选无效
				}
			}
		}else
		if(keyOrder ==  XKEY_HOME || (m_keyShiftState == m_keyNumLockState && keyOrder ==  XKEY_N7))
		{//home
			if(m_keyShiftState == 0)
			{//如果shift没有按下
				if(m_selectMouseDown == XFalse)
				{//如果没有鼠标选择(如果有鼠标选择则按键无效)
					if(m_nowInsertPoint != 0) changeInsertPoint(-m_nowInsertPoint);
					if(m_haveSelect) m_haveSelect = XFalse;
				}
			}else
			{//如果shift按下了
				if(!m_haveSelect)
				{//标记开始片选
					m_haveSelect = XFalse;
					m_selectEnd = m_nowInsertPoint;
					m_selectStart = m_nowInsertPoint;
					//然后移动光标，并修改片选状态
					if(m_nowInsertPoint > 0)
					{//将选择移动到头
						changeInsertPoint(-m_nowInsertPoint);
						m_selectEnd = m_nowInsertPoint;
					}
					if(m_selectEnd != m_selectStart)
					{//片选有效
						m_haveSelect = XTrue;
						upDataSelectShow();
					}else m_haveSelect = XFalse;	//片选无效
				}else
				{//修改片选结果
					//然后移动光标，并修改片选状态
					if(m_nowInsertPoint > 0)
					{//将选择移动到头
						changeInsertPoint(-m_nowInsertPoint);
						m_selectEnd = m_nowInsertPoint;
					}
					if(m_selectEnd != m_selectStart)
					{//片选有效
						m_haveSelect = XTrue;
						upDataSelectShow();
					}else m_haveSelect = XFalse;	//片选无效
				}
			}
		}else
		if(keyOrder ==  XKEY_END || (m_keyShiftState == m_keyNumLockState && keyOrder ==  XKEY_N1))
		{//end(鼠标没有片选动作时才能生效)
			if(m_keyShiftState == 0)
			{//如果shift没有按下
				if(!m_selectMouseDown)
				{//如果没有鼠标选择(如果有鼠标选择则按键无效)
					if(m_nowInsertPoint != m_nowStringLength) changeInsertPoint(m_nowStringLength - m_nowInsertPoint);
					if(m_haveSelect) m_haveSelect = XFalse;
				}
			}else
			{//如果shift按下了
				if(!m_haveSelect)
				{//标记开始片选
					m_haveSelect = XFalse;
					m_selectEnd = m_nowInsertPoint;
					m_selectStart = m_nowInsertPoint;
					//然后移动光标，并修改片选状态
					if(m_nowInsertPoint < m_nowStringLength)
					{//将选择移动到头
						changeInsertPoint(m_nowStringLength - m_nowInsertPoint);
						m_selectEnd = m_nowInsertPoint;
					}
					if(m_selectEnd != m_selectStart)
					{//片选有效
						m_haveSelect = XTrue;
						upDataSelectShow();
					}else m_haveSelect = XFalse;	//片选无效
				}else
				{//修改片选结果
					//然后移动光标，并修改片选状态
					if(m_nowInsertPoint < m_nowStringLength)
					{//将选择移动到头
						changeInsertPoint(m_nowStringLength - m_nowInsertPoint);
						m_selectEnd = m_nowInsertPoint;
					}
					if(m_selectEnd != m_selectStart)
					{//片选有效
						m_haveSelect = XTrue;
						upDataSelectShow();
					}else m_haveSelect = XFalse;	//片选无效
				}
			}
		}

		if(TextChange != 0)
		{
			//如果字符串发生改变，需要更新字符串的内容
			upDataShowStr();	//跟新字符串的显示
			if(m_funInputChenge != NULL) m_funInputChenge(m_pClass,m_objectID);
			return XTrue;
		}
		if(keyOrder ==  XKEY_RETURN || keyOrder == XKEY_N_ENTER)
		{//回车键
			if(m_haveSelect) m_haveSelect = XFalse;
			if(m_funInputOver != NULL) m_funInputOver(m_pClass,m_objectID);
			return XTrue;
		}
	}
	return XFalse;
}
void _XEdit::setPosition(float x,float y)
{
	if(!m_isInited) return;	//如果没有初始化直接退出
	m_position.set(x,y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	if(!m_withoutTex) m_spriteBackGround.setPosition(m_position);
	upDateInsertShowPosition();
	upDataSelectShow();
	updateChildPos();
}
void _XEdit::setSize(float x,float y)
{
	if(x <= 0 && y <= 0) return;
	if(!m_isInited) return;	//如果没有初始化直接退出
	m_size.set(x,y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	m_nowTextWidth = m_caption.getTextSize().x * m_caption.getSize().x * 0.5f;
	m_nowTextHeight = m_caption.getTextSize().y * m_caption.getSize().y;
//	m_selectText.setPosition(m_objRect.left + (m_edgeDistance.left + m_textPosition.x) * m_size.x,m_objRect.top + (m_edgeDistance.top + m_textPosition.y) * m_size.y);
//	m_selectText.setSize(m_size);
	//m_spriteBackGround.setPosition(m_position);
	if(!m_withoutTex) m_spriteBackGround.setSize(m_size);
	upDateInsertShowPosition();
	//m_spriteSelect.setSize(m_size);
	upDataSelectShow();
	if(!m_withoutTex) m_spriteInsert.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);

	updateChildSize();
}
_XBool _XEdit::setACopy(const _XEdit &temp)			//设置为一个拷贝公用部分资源		
{
	if(& temp == this) return XTrue;	//防止自身赋值
	if(!temp.m_isInited) return XFalse;
	if(m_isInited) release();//如果自身已经初始化，则需要释放资源
	if(!_XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_EDIT);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	}

	m_isInited = temp.m_isInited;	//是否初始化
	if(m_resInfo != NULL) _XResourceManager::GetInstance().releaseResource(m_resInfo);
	m_resInfo = _XResourceMng.copyResource(temp.m_resInfo);
	m_withoutTex = temp.m_withoutTex;
	m_selectRect = temp.m_selectRect;

	if(temp.m_mouseRightButtonMenu != NULL)
	{
		m_mouseRightButtonMenu = createMem<_XMouseRightButtonMenu>();
		if(m_mouseRightButtonMenu == NULL) return XFalse;

		m_mouseRightButtonMenu->setACopy(* (temp.m_mouseRightButtonMenu));	//鼠标右键菜单
		//从控件管理器中将这个控件注销掉
		_XControlManager::GetInstance().decreaseAObject(m_mouseRightButtonMenu);	//注销这个物件
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(m_mouseRightButtonMenu);
#endif
	}

	if(m_caption.setACopy(temp.m_caption) == 0)	return XFalse;		//显示当前输入的字符串
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_nowString = createArrayMem<char>(MAX_INPUT_STRING_LENGTH);
	strcpy(m_nowString,temp.m_nowString);
	m_tempNowString = createArrayMem<char>(MAX_INPUT_STRING_LENGTH);
	if(m_tempNowString == NULL) 
	{
		XDELETE_ARRAY(m_nowString);
		return XFalse;
	}
	strcpy(m_tempNowString,temp.m_tempNowString);
	m_nowShowString	= createArrayMem<char>(MAX_INPUT_STRING_LENGTH);
	if(m_nowShowString == NULL)
	{
		XDELETE_ARRAY(m_nowString);
		XDELETE_ARRAY(m_tempNowString);
		return XFalse;
	}
	strcpy(m_nowShowString,temp.m_nowShowString);

	m_nowInsertPoint = temp.m_nowInsertPoint;	//当前选择输入的位置,整个字符串的整体位置
	m_nowStringLength = temp.m_nowStringLength;	//当前输入字符串的长度
	m_haveSelect = temp.m_haveSelect;		//是否在字符串中有选择
	m_selectMouseDown = temp.m_selectMouseDown;	//鼠标是否按下准备片选
	m_selectStart = temp.m_selectStart;		//选择的起始位置
	m_selectEnd = temp.m_selectEnd;		//选择的结束位置

	m_editNormal = temp.m_editNormal;		//输入框普通状态
	m_editDisable = temp.m_editDisable;		//输入框无效状态
	m_editSelect = temp.m_editSelect;		//输入框片选颜色
	m_editInsert = temp.m_editInsert;		//输入框插入标记
	m_editUpon = temp.m_editUpon;		//输入框插入标记

	m_spriteBackGround.setACopy(temp.m_spriteBackGround);	//用于显示输入框的背景贴图
	m_spriteSelect.setACopy(temp.m_spriteSelect);		//用于显示选择内容的背景颜色
	m_spriteInsert.setACopy(temp.m_spriteInsert);		//用于显示插入符号
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteBackGround);
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteSelect);
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteInsert);
#endif
	m_textPosition = temp.m_textPosition;			//文字显示的位置，是相对于控件的位置来定的
	m_textSize = temp.m_textSize;				//文字显示的尺寸，这个尺寸会与空间的缩放尺寸叠加

	m_funInputChenge = temp.m_funInputChenge;		//输入内容发生改变的时候调用
	m_funInputOver = temp.m_funInputOver;		//确认输入结束之后的时候调用

	m_timer = temp.m_timer;	//这是插入符号闪烁时使用的时间标记，这个时间使用不准确的计时方式
	//下面是用于按键判断的中间变量
	m_keyShiftState = temp.m_keyShiftState;		//shift按键的状态	0弹起，1按下
	m_keyCapsLockState = temp.m_keyCapsLockState;	//CapsLock按键的状态	1有效，0无效
	m_keyNumLockState = temp.m_keyNumLockState;		//NumLock按键的状态	1有效，0无效
	m_keyCtrlState = temp.m_keyCtrlState;		//Ctrl按键的状态 0弹起，1按下

	m_canShowLength = temp.m_canShowLength;		//输入框中可以显示的字符串的长度
	m_nowShowStart = temp.m_nowShowStart;			//当前输入框中显示的字符串的起始位置
	m_pClass = temp.m_pClass;

	m_textColor = temp.m_textColor;
	m_nowTextWidth = temp.m_nowTextWidth;
	m_nowTextHeight = temp.m_nowTextHeight;
	return XTrue;
}
void _XEdit::insertChar(const char * ch,int len)
{
	if(ch == NULL) return;
	if(len <= 0) return;
	if(m_nowStringLength + len >= MAX_INPUT_STRING_LENGTH - 1) return;
	if(m_haveSelect)
	{//存在片选状态
		if(!m_selectMouseDown)
		{//片选已经结束
			//1、删除片选内容
			deleteSelectStr();
			m_haveSelect = XFalse;	//片选结束

			if(m_nowInsertPoint == m_nowStringLength)
			{//末尾
				memcpy(m_nowString + m_nowInsertPoint,ch,len);
			//	++ m_nowInsertPoint;
				m_nowStringLength += len;
				m_nowString[m_nowStringLength] = '\0';
				changeInsertPoint(len);
			}else
			{//中间插入
				//strcpy(m_nowString + m_nowInsertPoint + 1,m_nowString + m_nowInsertPoint);
				strcpy(m_tempNowString,m_nowString + m_nowInsertPoint);
				memcpy(m_nowString + m_nowInsertPoint,ch,len);
				strcpy(m_nowString + m_nowInsertPoint + len,m_tempNowString);
			//	++ m_nowInsertPoint;
				m_nowStringLength += len;
				changeInsertPoint(len);
			}
		}else
		{//片选尚未结束
			//作结束动作
			if(m_selectEnd != m_selectStart)
			{//片选有效
				//插入按键
				//1、删除片选内容
				deleteSelectStr();
				m_haveSelect = XFalse;	//片选结束
				if(m_nowInsertPoint == m_nowStringLength)
				{//末尾
					memcpy(m_nowString + m_nowInsertPoint,ch,len);
					//++ m_nowInsertPoint;
					m_nowStringLength += len;
					m_nowString[m_nowStringLength] = '\0';
					changeInsertPoint(len);
				}else
				{//中间插入
					//strcpy(m_nowString + m_nowInsertPoint + 1,m_nowString + m_nowInsertPoint);
					strcpy(m_tempNowString,m_nowString + m_nowInsertPoint);
					memcpy(m_nowString + m_nowInsertPoint,ch,len);
					strcpy(m_nowString + m_nowInsertPoint + len,m_tempNowString);
					//++ m_nowInsertPoint;
					m_nowStringLength += len;
					changeInsertPoint(len);
				}
			}
			//然后再作重新开始动作
			m_haveSelect = XFalse;
			m_selectEnd = m_nowInsertPoint;
			m_selectStart = m_nowInsertPoint;
			m_selectMouseDown = XTrue;
		}
	}else
	{//不存在片选
		if(m_nowInsertPoint == m_nowStringLength)
		{//末尾
			memcpy(m_nowString + m_nowInsertPoint,ch,len);
			//++ m_nowInsertPoint;
			m_nowStringLength += len;
			m_nowString[m_nowStringLength] = '\0';
			changeInsertPoint(len);
		}else
		{//中间插入
			//strcpy(m_nowString + m_nowInsertPoint + 1,m_nowString + m_nowInsertPoint);
			strcpy(m_tempNowString,m_nowString + m_nowInsertPoint);
			//m_nowString[m_nowInsertPoint] = tempChar;
			memcpy(m_nowString + m_nowInsertPoint,ch,len);
			strcpy(m_nowString + m_nowInsertPoint + len,m_tempNowString);
			//++ m_nowInsertPoint;
			m_nowStringLength += len;
			changeInsertPoint(len);
		}
		if(m_selectMouseDown != 0)
		{//如果按下了片选鼠标，则光标要随着走
			m_selectEnd = m_nowInsertPoint;
			m_selectStart = m_nowInsertPoint;
		}
	}

	//如果字符串发生改变，需要更新字符串的内容
	upDataShowStr();	//跟新字符串的显示
	if(m_funInputChenge != NULL) m_funInputChenge(m_pClass,m_objectID);

	return;
}
int _XEdit::keyJudgement(int keyOrder)
{
#ifdef XEE_OS_WINDOWS	//windows系统下通过输入法来输入字符
	return 0;
#endif
#ifdef XEE_OS_LINUX
	int TextChange = 0;
	char tempChar = 0;
//	printf("%d\n",keyOrder);
	if(keyOrder == XKEY_SPACE)
	{
		tempChar = ' ';
		TextChange = 1;
	}else
	if(m_keyShiftState == 0)
	{
		TextChange = 1;
		switch(keyOrder)
		{
		case XKEY_0:tempChar = '0';break;
		case XKEY_1:tempChar = '1';break;
		case XKEY_2:tempChar = '2';break;
		case XKEY_3:tempChar = '3';break;
		case XKEY_4:tempChar = '4';break;
		case XKEY_5:tempChar = '5';break;
		case XKEY_6:tempChar = '6';break;
		case XKEY_7:tempChar = '7';break;
		case XKEY_8:tempChar = '8';break;
		case XKEY_9:tempChar = '9';break;
		//符号按键
		case XKEY_BACKQUOTE:tempChar = '`';break;
		case XKEY_MINUS:tempChar = '-';break;
		case XKEY_EQUALS:tempChar = '=';break;
		case XKEY_LEFTBRACKET:tempChar = '[';break;
		case XKEY_RIGHTBRACKET:tempChar = ']';break;
		case XKEY_SEMICOLON:tempChar = ';';break;
		case XKEY_QUOTE:tempChar = '\'';break;
		case XKEY_BACKSLASH:tempChar = '\\';break;
		case XKEY_COMMA:tempChar = ',';break;
		case XKEY_PERIOD:tempChar = '.';break;
		case XKEY_SLASH:tempChar = '/';break;
		default:TextChange = 0;break;
		}
	}else
	{
		TextChange = 1;
		switch(keyOrder)
		{
		case XKEY_0:tempChar = ')';break;
		case XKEY_1:tempChar = '!';break;
		case XKEY_2:tempChar = '@';break;
		case XKEY_3:tempChar = '#';break;
		case XKEY_4:tempChar = '$';break;
		case XKEY_5:tempChar = '%';break;
		case XKEY_6:tempChar = '^';break;
		case XKEY_7:tempChar = '&';break;
		case XKEY_8:tempChar = '*';break;
		case XKEY_9:tempChar = '(';break;
			//符号按键
		case XKEY_BACKQUOTE:tempChar = '~';break;
		case XKEY_MINUS:tempChar = '_';break;
		case XKEY_EQUALS:tempChar = '+';break;
		case XKEY_LEFTBRACKET:tempChar = '{';break;
		case XKEY_RIGHTBRACKET:tempChar = '}';break;
		case XKEY_SEMICOLON:tempChar = ':';break;
		case XKEY_QUOTE:tempChar = '\"';break;
		case XKEY_BACKSLASH:tempChar = '|';break;
		case XKEY_COMMA:tempChar = '<';break;
		case XKEY_PERIOD:tempChar = '>';break;
		case XKEY_SLASH:tempChar = '?';break;
		default:TextChange = 0;break;
		}
	}
	//字母键
	if(TextChange == 0)
	{
	//	if((m_keyShiftState == 0 && m_keyCapsLockState == 0) || (m_keyShiftState == 1 || m_keyCapsLockState == 1))
		if(m_keyShiftState == m_keyCapsLockState)
		{
			TextChange = 1;
			switch(keyOrder)
			{
			case XKEY_a:tempChar = 'a';break;
			case XKEY_b:tempChar = 'b';break;
			case XKEY_c:tempChar = 'c';break;
			case XKEY_d:tempChar = 'd';break;
			case XKEY_e:tempChar = 'e';break;
			case XKEY_f:tempChar = 'f';break;
			case XKEY_g:tempChar = 'g';break;
			case XKEY_h:tempChar = 'h';break;
			case XKEY_i:tempChar = 'i';break;
			case XKEY_j:tempChar = 'j';break;
			case XKEY_k:tempChar = 'k';break;
			case XKEY_l:tempChar = 'l';break;
			case XKEY_m:tempChar = 'm';break;
			case XKEY_n:tempChar = 'n';break;
			case XKEY_o:tempChar = 'o';break;
			case XKEY_p:tempChar = 'p';break;
			case XKEY_q:tempChar = 'q';break;
			case XKEY_r:tempChar = 'r';break;
			case XKEY_s:tempChar = 's';break;
			case XKEY_t:tempChar = 't';break;
			case XKEY_u:tempChar = 'u';break;
			case XKEY_v:tempChar = 'v';break;
			case XKEY_w:tempChar = 'w';break;
			case XKEY_x:tempChar = 'x';break;
			case XKEY_y:tempChar = 'y';break;
			case XKEY_z:tempChar = 'z';break;
			default:TextChange = 0;break;
			}
		}else
		{
			TextChange = 1;
			switch(keyOrder)
			{
			case XKEY_a:tempChar = 'A';break;
			case XKEY_b:tempChar = 'B';break;
			case XKEY_c:tempChar = 'C';break;
			case XKEY_d:tempChar = 'D';break;
			case XKEY_e:tempChar = 'E';break;
			case XKEY_f:tempChar = 'F';break;
			case XKEY_g:tempChar = 'G';break;
			case XKEY_h:tempChar = 'H';break;
			case XKEY_i:tempChar = 'I';break;
			case XKEY_j:tempChar = 'J';break;
			case XKEY_k:tempChar = 'K';break;
			case XKEY_l:tempChar = 'L';break;
			case XKEY_m:tempChar = 'M';break;
			case XKEY_n:tempChar = 'N';break;
			case XKEY_o:tempChar = 'O';break;
			case XKEY_p:tempChar = 'P';break;
			case XKEY_q:tempChar = 'Q';break;
			case XKEY_r:tempChar = 'R';break;
			case XKEY_s:tempChar = 'S';break;
			case XKEY_t:tempChar = 'T';break;
			case XKEY_u:tempChar = 'U';break;
			case XKEY_v:tempChar = 'V';break;
			case XKEY_w:tempChar = 'W';break;
			case XKEY_x:tempChar = 'X';break;
			case XKEY_y:tempChar = 'Y';break;
			case XKEY_z:tempChar = 'Z';break;
			default:TextChange = 0;break;
			}
		}
	}
	//小键盘
	if(TextChange == 0)
	{
		if(m_keyShiftState != m_keyNumLockState)
		{
			TextChange = 1;
			switch(keyOrder)
			{
			case XKEY_KP0:tempChar = '0';break;
			case XKEY_KP1:tempChar = '1';break;
			case XKEY_KP2:tempChar = '2';break;
			case XKEY_KP3:tempChar = '3';break;
			case XKEY_KP4:tempChar = '4';break;
			case XKEY_KP5:tempChar = '5';break;
			case XKEY_KP6:tempChar = '6';break;
			case XKEY_KP7:tempChar = '7';break;
			case XKEY_KP8:tempChar = '8';break;
			case XKEY_KP9:tempChar = '9';break;
			case XKEY_KP_PERIOD:tempChar = '.';break;
			default:TextChange = 0;break;
			}
		}
		if(TextChange == 0)
		{
			TextChange = 1;
			switch(keyOrder)
			{
			case XKEY_KP_DIVIDE:tempChar = '/';break;
			case XKEY_KP_MULTIPLY:tempChar = '*';break;
			case XKEY_KP_MINUS:tempChar = '-';break;
			case XKEY_KP_PLUS:tempChar = '+';break;
			default:TextChange = 0;break;
			}
		}
	}
	if(TextChange != 0) insertChar(&tempChar,1);
	return TextChange;
#endif
}
_XBool _XEdit::isInRect(float x,float y)
{
	if(!m_isInited) return XFalse;
	return getIsInRect(_XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
}
_XVector2 _XEdit::getBox(int order)
{
	_XVector2 ret;
	ret.set(0.0f,0.0f);
	if(!m_isInited) return ret;
	if(order == 0) ret.set(m_nowMouseRect.left,m_nowMouseRect.top);else
	if(order == 1) ret.set(m_nowMouseRect.right,m_nowMouseRect.top);else
	if(order == 2) ret.set(m_nowMouseRect.right,m_nowMouseRect.bottom);else
	if(order == 3) ret.set(m_nowMouseRect.left,m_nowMouseRect.bottom);
	return ret;
}