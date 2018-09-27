#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XMultiText.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
namespace XE{
void XMultiText::ctrlProc(void*pClass,int id,int eventID)
{
	XMultiText &pPar = *(XMultiText *)pClass;
	if(id == pPar.m_verticalSlider.getControlID())
	{
		if(eventID == XSlider::SLD_MOUSE_MOVE)
		{
		//	printf("V ID:%d\n",pPar.m_objectID);
		//	printf("V Cur value:%f\n",pPar.m_verticalSlider.getCurValue());
			//相应数值的改变
			int temp = pPar.m_verticalSlider.getCurValue();
			if(temp != pPar.m_showStartLine)
			{
				//需要更新垂直片选的情况
				pPar.setStartLine(temp);
				pPar.updateLineStr();
			}
		}else
		if(eventID == XSlider::SLD_VALUE_CHANGE)
		{
		//	printf("V ID:%d\n",pPar.m_objectID);
		//	printf("V Cur value:%f\n",pPar.m_verticalSlider.getCurValue());
			//相应数值的改变
			int temp = pPar.m_verticalSlider.getCurValue();
			if(temp != pPar.m_showStartLine)
			{
				pPar.setStartLine(temp);
				pPar.updateLineStr();
				pPar.m_verticalSlider.setCurValue(temp);
			}else
			{//微量移动时候的处理
				if(pPar.m_verticalSlider.getCurValue() > pPar.m_showStartLine)
				{
					++ temp;
					pPar.setStartLine(temp);
					pPar.updateLineStr();
					pPar.m_verticalSlider.setCurValue(temp);
				}else
				if(pPar.m_verticalSlider.getCurValue() < pPar.m_showStartLine)
				{
					-- temp;
					pPar.setStartLine(temp);
					pPar.updateLineStr();
					pPar.m_verticalSlider.setCurValue(temp);
				}
			}
		}
		return;
	}
	if(id == pPar.m_horizontalSlider.getControlID())
	{
		if(eventID == XSlider::SLD_MOUSE_MOVE)
		{
		//	printf("H ID:%d\n",pPar.m_objectID);
		//	printf("H Cur value:%f\n",pPar.m_horizontalSlider.getCurValue());
			//相应数值的改变
			int temp = pPar.m_horizontalSlider.getCurValue();
			if(temp != pPar.m_curStartPixels)
			{
				pPar.m_curStartPixels = temp;
				pPar.updateLineStr();
			}
			//if(temp != pPar.m_lineStr[0].showStart)
			//{
			//	for(int i = 0;i < pPar.m_canShowLineSum;++ i)
			//	{
			//		pPar.m_lineStr[i].showStart = temp;
			//	}
			//	pPar.updateLineStr();
			//}
		}else
		if(eventID == XSlider::SLD_VALUE_CHANGE)
		{
		//	printf("H ID:%d\n",pPar.m_objectID);
		//	printf("H Cur value:%f\n",pPar.m_horizontalSlider.getCurValue());
			//相应数值的改变
			int temp = pPar.m_horizontalSlider.getCurValue();
			if(temp != pPar.m_curStartPixels)
			{
				//printf("%d\n",temp);
				pPar.m_curStartPixels = temp;
				pPar.updateLineStr();
				pPar.m_horizontalSlider.setCurValue(temp);
			}
			//if(temp != pPar.m_lineStr[0].showStart)
			//{
			//	for(int i = 0;i < pPar.m_canShowLineSum;++ i)
			//	{
			//		pPar.m_lineStr[i].showStart = temp;
			//	}
			//	pPar.updateLineStr();
			//	pPar.m_horizontalSlider.setCurValue(temp);
			//}else
			//{
			//	if(pPar.m_horizontalSlider.getCurValue() > pPar.m_lineStr[0].showStart)
			//	{
			//		++ temp;
			//		for(int i = 0;i < pPar.m_canShowLineSum;++ i)
			//		{
			//			pPar.m_lineStr[i].showStart = temp;
			//		}
			//		pPar.updateLineStr();
			//		pPar.m_horizontalSlider.setCurValue(temp);
			//	}else
			//	if(pPar.m_horizontalSlider.getCurValue() < pPar.m_lineStr[0].showStart)
			//	{
			//		-- temp;
			//		for(int i = 0;i < pPar.m_canShowLineSum;++ i)
			//		{
			//			pPar.m_lineStr[i].showStart = temp;
			//		}
			//		pPar.updateLineStr();
			//		pPar.m_horizontalSlider.setCurValue(temp);
			//	}
			//}
		}
		return;
	}
}
XMultiText::XMultiText()
	:m_isInited(XFalse)
	, m_multiEditNormal(NULL)			//输入框普通状态
	, m_multiEditDisable(NULL)		//输入框无效状态
	, m_multiEditSelect(NULL)			//输入框片选颜色
	//,m_funInputChenge(NULL)			//输入内容发生改变的时候调用
	//,m_funInputOver(NULL)			//确认输入结束之后的时候调用
	, m_lineStr(NULL)
	, m_resInfo(NULL)
	, m_withoutTex(XFalse)
	//,m_funSelect(NULL)
	//,m_pClass(NULL)
	, m_curMaxLineSum(0)
{
	m_ctrlType = CTRL_OBJ_MUTITEXT;
}
void XMultiText::release()
{
	if(!m_isInited) return ;	//如果没有初始化直接退出
	XMem::XDELETE_ARRAY(m_lineStr);
	for(auto it = m_curStr.begin();it != m_curStr.end();++ it)
	{
		XMem::XDELETE(*it);
	}
	m_curStr.clear();
	for (auto it = m_freeStr.begin(); it != m_freeStr.end(); ++it)
	{
		XMem::XDELETE(*it);
	}
	m_freeStr.clear();
	XCtrlManager.decreaseAObject(this);	//注销这个物件
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
void XMultiText::setStartLine(int temp)
{
	if(m_haveSelect && m_selectLineOrder >= m_showStartLine
		&& m_selectLineOrder < m_showStartLine + m_canShowLineSum)
	{//回复原有的设置
		m_lineStr[m_selectLineOrder - m_showStartLine].haveAllSelect = XFalse;
	}
	m_showStartLine = temp;
	if(m_haveSelect && 
		(m_selectLineOrder >= m_showStartLine && m_selectLineOrder < m_canShowLineSum + m_showStartLine))
	{//修改新的设置
		m_lineStr[m_selectLineOrder - m_showStartLine].haveAllSelect = XTrue;
		if(!m_withoutTex)
		{
			m_lineStr[m_selectLineOrder - m_showStartLine].spriteSelect.setPosition(m_curMouseRect.left,
				m_curMouseRect.top + m_curTextHeight * (m_selectLineOrder - m_showStartLine));
			m_lineStr[m_selectLineOrder - m_showStartLine].spriteSelect.setScale(m_curMouseRect.getWidth() / m_multiEditSelect->textureSize.x,
							m_curTextHeight / m_multiEditSelect->textureSize.y);
		}
	}
}
XBool XMultiText::init(const XVec2& position,	//控件所在的位置
		const XRect& area,	//控件的范围
		const XMultiTextSkin& tex,	//控件的贴图
		const char *str,const XFontUnicode& font,float strSize,		//控件的字体及相关信息
		//const XVec2& strPosition,
		//const XMouseRightButtonMenu &mouseMenu,
		const XSlider &vSlider,const XSlider &hSlider)	//控件的其他附属依赖控件
{
	if(m_isInited) return XFalse;	//防止重复初始化
	if(tex.editNormal == NULL || tex.editInsert == NULL || tex.editDisable == NULL || tex.editSelect == NULL) return XFalse;

	//需要注意的是输入范围必须要能显示最少一个字符，否则将会造成问题，目前这里并没有代码去判断，但是实际使用中需要注意这个问题
	if(area.getWidth() <= 0 || area.getHeight() <= 0) return XFalse;	//输入范围不能为空
	if(strSize <= 0) return XFalse;		//字符串的大小不能为非法值
//	if(str != NULL && strlen(str) >= MAX_STRING_LENGTH) return XFalse;

	//下面开始赋值
	m_position = position;
	m_mouseRect = area;
	m_withoutTex = XFalse;

	m_multiEditNormal = tex.editNormal;			//输入框普通状态
	m_multiEditDisable = tex.editDisable;		//输入框无效状态
	m_multiEditSelect = tex.editSelect;			//输入框片选颜色
	m_multiEditUpon = tex.editUpon;
	m_scale.set(1.0f);

	//m_textPosition = strPosition;			//文字显示的位置，是相对于控件的位置来定的
	m_textSize.set(strSize);				//文字显示的尺寸，这个尺寸会与空间的缩放尺寸叠加

	m_spriteBackGround.init(m_multiEditNormal->texture.m_w,m_multiEditNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteBackGround);
#endif
	m_spriteBackGround.setPosition(m_position);
	m_spriteBackGround.setScale(m_scale);
	m_spriteBackGround.setIsTransformCenter(POINT_LEFT_TOP);

	m_verticalSlider.setACopy(vSlider);
	XCtrlManager.decreaseAObject(&m_verticalSlider);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_verticalSlider);
#endif
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
		m_position.y + m_mouseRect.top * m_scale.y);
	m_verticalSlider.setScale(m_scale);
	m_verticalSlider.setEventProc(ctrlProc,this);
	m_verticalSlider.setWithAction(XFalse);
	m_needShowVSlider = XFalse;			//是否需要显示垂直滑动条

	m_horizontalSlider.setACopy(hSlider);
	XCtrlManager.decreaseAObject(&m_horizontalSlider);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_horizontalSlider);
#endif
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_scale.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale.y);
	m_horizontalSlider.setScale(m_scale);
	m_horizontalSlider.setEventProc(ctrlProc,this);
	m_horizontalSlider.setWithAction(XFalse);
	m_needShowHSlider = XFalse;			//是否需要显示水平滑动条

	m_textWidth = font.getTextSize().x * font.getScale().x * 0.5f;
	m_curTextHeight = font.getTextSize().y * font.getScale().y;
	//计算当前可以显示多少行
	m_canShowLineSum = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight())/m_curTextHeight;	//不应该进去高度，而应该减去buttom ，以后在修正
	//计算当前一行可以显示多少个字符(这个0.5是因为显示英文字母却使用中文字库)
	//m_curShowLineWidth = (m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth())/m_textWidth;	//不应该减去宽度，而应该减去right，以后在修正

	m_curStartPixels = 0;
	m_curMaxLineWidthPixels = 0;
	m_maxLineWidthPixels = m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth();

	m_curMouseRect.set(m_position + m_mouseRect.getLT() * m_scale,
		m_position + (m_mouseRect.getRB() -
			XVec2(m_verticalSlider.getMouseRectWidth(), m_horizontalSlider.getMouseRectHeight())) * m_scale);	//当前的鼠标响应范围
	//为每行分配内存空间
	m_lineStr = XMem::createArrayMem<XMultiTextLineString>(m_canShowLineSum);
	if(m_lineStr == NULL) return XFalse;

	m_textColor.set(0.0f,1.0f);
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_lineStr[i].isEnable = XFalse;
		if(!m_lineStr[i].curText.setACopy(font)) return XFalse;
		m_lineStr[i].curText.setMaxStrLen(MAX_LINE_STRING_LENGTH);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_lineStr[i].curText));
#endif
		m_lineStr[i].curText.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
		m_lineStr[i].curText.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);	 //设置字体上对齐
		m_lineStr[i].curText.setColor(m_textColor);
		m_lineStr[i].curText.setPosition(m_position.x + (m_mouseRect.left - m_curStartPixels) * m_scale.x,
			m_position.y + m_mouseRect.top * m_scale.y + m_curTextHeight * i);
		m_lineStr[i].curText.setScale(m_scale);
	//	m_lineStr[i].showLength = m_curShowLineWidth;
	//	m_lineStr[i].showStart = 0;
		m_lineStr[i].haveAllSelect = XFalse;
		m_lineStr[i].spriteSelect.init(m_multiEditSelect->texture.m_w,m_multiEditSelect->texture.m_h,1);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_lineStr[i].spriteSelect));
#endif
		m_lineStr[i].spriteSelect.setIsTransformCenter(POINT_LEFT_TOP);
		//m_lineStr[i].showString = "";
	}
	//为整个字符串分配内存空间
	std::string *tmpStr = NULL;
	for(int i = 0;i < MAX_LINE_SUM;++ i)
	{
		tmpStr = XMem::createMem<std::string>();
		if(tmpStr == NULL)
		{
			XMem::XDELETE_ARRAY(m_lineStr);
			return XFalse;
		}
		m_freeStr.push_back(tmpStr);
	}

	m_haveSelect = XFalse;		//是否在字符串中有选择
	m_selectLineOrder = -1;

	m_isVisible = m_isEnable = m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	setString(str);
	return XTrue;
}
XBool XMultiText::initPlus(const char *path,
		const char *str,const XFontUnicode& font, float strSize,
		XResPos resPos)
{
	if(m_isInited) return XFalse;	//防止重复初始化
	m_resInfo = XResManager.loadResource(path,RESOURCE_TYPEXEDIT_TEX,resPos);
	if(m_resInfo == NULL) return XFalse;
	XEditSkin * tex = (XEditSkin *)m_resInfo->m_pointer;
	if(tex->editNormal == NULL || tex->editInsert == NULL || tex->editDisable == NULL || tex->editSelect == NULL) return XFalse;

	//需要注意的是输入范围必须要能显示最少一个字符，否则将会造成问题，目前这里并没有代码去判断，但是实际使用中需要注意这个问题
	if(tex->m_mouseRect.getWidth() <= 0 || tex->m_mouseRect.getHeight() <= 0) return XFalse;	//输入范围不能为空
	if(strSize <= 0) return XFalse;		//字符串的大小不能为非法值
//	if(str != NULL && strlen(str) >= MAX_STRING_LENGTH) return XFalse;

	//下面开始赋值
	m_position.reset();
	m_mouseRect = tex->m_mouseRect;
	m_withoutTex = XFalse;

	m_multiEditNormal = tex->editNormal;			//输入框普通状态
	m_multiEditDisable = tex->editDisable;		//输入框无效状态
	m_multiEditSelect = tex->editSelect;			//输入框片选颜色
	m_multiEditUpon = tex->editUpon;
	m_scale.set(1.0f);

	//m_textPosition = strPosition;			//文字显示的位置，是相对于控件的位置来定的
	m_textSize.set(strSize);				//文字显示的尺寸，这个尺寸会与空间的缩放尺寸叠加

	m_spriteBackGround.init(m_multiEditNormal->texture.m_w,m_multiEditNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteBackGround);
#endif
	m_spriteBackGround.setPosition(m_position);
	m_spriteBackGround.setScale(m_scale);
	m_spriteBackGround.setIsTransformCenter(POINT_LEFT_TOP);

	//m_verticalSlider.setACopy(vSlider);
	char tempPath[MAX_FILE_NAME_LENGTH];
	sprintf_s(tempPath,MAX_FILE_NAME_LENGTH,"%s/SliderV",path);
	m_verticalSlider.initPlus(tempPath,100.0f,0.0f,SLIDER_TYPE_VERTICAL,resPos);
	XCtrlManager.decreaseAObject(&m_verticalSlider);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_verticalSlider);
#endif
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
		m_position.y + m_mouseRect.top * m_scale.y);
	m_verticalSlider.setScale(m_scale);
	m_verticalSlider.setEventProc(ctrlProc,this);
	m_verticalSlider.setWithAction(XFalse);
	m_needShowVSlider = XFalse;			//是否需要显示垂直滑动条

	//m_horizontalSlider.setACopy(hSlider);
	sprintf_s(tempPath,MAX_FILE_NAME_LENGTH,"%s/SliderH",path);
	m_horizontalSlider.initPlus(tempPath,100.0f,0.0f,SLIDER_TYPE_HORIZONTAL,resPos);
	XCtrlManager.decreaseAObject(&m_horizontalSlider);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_horizontalSlider);
#endif
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_scale.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale.y);
	m_horizontalSlider.setScale(m_scale);
	m_horizontalSlider.setEventProc(ctrlProc,this);
	m_horizontalSlider.setWithAction(XFalse);
	m_needShowHSlider = XFalse;			//是否需要显示水平滑动条

	m_textWidth = font.getTextSize().x * font.getScale().x * 0.5f;
	m_curTextHeight = font.getTextSize().y * font.getScale().y;
	//计算当前可以显示多少行
	m_canShowLineSum = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight())/m_curTextHeight;	//不应该进去高度，而应该减去buttom ，以后在修正
	//计算当前一行可以显示多少个字符(这个0.5是因为显示英文字母却使用中文字库)
	//m_curShowLineWidth = (m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth())/m_textWidth;	//不应该减去宽度，而应该减去right，以后在修正

	m_curStartPixels = 0;
	m_curMaxLineWidthPixels = 0;
	m_maxLineWidthPixels = m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth();

	m_curMouseRect.set(m_position + m_mouseRect.getLT() * m_scale,
		m_position + (m_mouseRect.getRB() -
			XVec2(m_verticalSlider.getMouseRectWidth(), m_horizontalSlider.getMouseRectHeight())) * m_scale);	//当前的鼠标响应范围
	//为每行分配内存空间
	m_lineStr = XMem::createArrayMem<XMultiTextLineString>(m_canShowLineSum);
	if(m_lineStr == NULL) return XFalse;

	m_textColor.set(0.0f,1.0f);
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_lineStr[i].isEnable = XFalse;
		if(!m_lineStr[i].curText.setACopy(font)) return XFalse;
		m_lineStr[i].curText.setMaxStrLen(MAX_LINE_STRING_LENGTH);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_lineStr[i].curText));
#endif
		m_lineStr[i].curText.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
		m_lineStr[i].curText.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);	 //设置字体上对齐
		m_lineStr[i].curText.setColor(m_textColor);
		m_lineStr[i].curText.setPosition(m_position.x + (m_mouseRect.left - m_curStartPixels) * m_scale.x,
			m_position.y + m_mouseRect.top * m_scale.y + m_curTextHeight * i);
		m_lineStr[i].curText.setScale(m_scale);
	//	m_lineStr[i].showLength = m_curShowLineWidth;
	//	m_lineStr[i].showStart = 0;
		m_lineStr[i].haveAllSelect = XFalse;
		m_lineStr[i].spriteSelect.init(m_multiEditSelect->texture.m_w,m_multiEditSelect->texture.m_h,1);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_lineStr[i].spriteSelect));
#endif
		m_lineStr[i].spriteSelect.setIsTransformCenter(POINT_LEFT_TOP);
		//m_lineStr[i].showString = "";
	}
	//为整个字符串分配内存空间
	std::string *tmpStr = NULL;
	for(int i = 0;i < MAX_LINE_SUM;++ i)
	{
		tmpStr = XMem::createMem<std::string>();
		if(tmpStr == NULL)
		{
			XMem::XDELETE_ARRAY(m_lineStr);
			return XFalse;
		}
		m_freeStr.push_back(tmpStr);
	}

	m_haveSelect = XFalse;		//是否在字符串中有选择
	m_selectLineOrder = -1;

	m_isVisible = m_isEnable = m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	setString(str);
	return XTrue;
}
XBool XMultiText::initWithoutSkin(const XRect& area,
		const char *str,const XFontUnicode& font, float strSize)
{
	if(m_isInited ||	//防止重复初始化
		//需要注意的是输入范围必须要能显示最少一个字符，否则将会造成问题，目前这里并没有代码去判断，但是实际使用中需要注意这个问题
		strSize <= 0) return XFalse;		//字符串的大小不能为非法值
//	if(str != NULL && strlen(str) >= MAX_STRING_LENGTH) return XFalse;

	//下面开始赋值
	m_position.reset();
	m_mouseRect = area;
	m_withoutTex = XTrue;

	m_scale.set(1.0f);
	m_textSize.set(strSize);				//文字显示的尺寸，这个尺寸会与空间的缩放尺寸叠加

//	m_verticalSlider.initWithoutSkin(XRect(0,0,DEFAULT_SLIDER_WIDTH,m_mouseRect.getHeight() - DEFAULT_SLIDER_WIDTH),
//		XRect(0,DEFAULT_SLIDER_WIDTH),SLIDER_TYPE_VERTICAL,100.0f,0.0f,XVec2::zero);
	m_verticalSlider.initWithoutSkin(XRect(0,0,DEFAULT_SLIDER_WIDTH,m_mouseRect.getHeight() - DEFAULT_SLIDER_WIDTH),
		100.0f,0.0f,SLIDER_TYPE_VERTICAL);
	XCtrlManager.decreaseAObject(&m_verticalSlider);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_verticalSlider);
#endif
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
		m_position.y + m_mouseRect.top * m_scale.y);
	m_verticalSlider.setScale(m_scale);
	m_verticalSlider.setEventProc(ctrlProc,this);
	m_verticalSlider.setWithAction(XFalse);
	m_needShowVSlider = XFalse;			//是否需要显示垂直滑动条

//	m_horizontalSlider.initWithoutSkin(XRect(0,0,m_mouseRect.getWidth() - DEFAULT_SLIDER_WIDTH,DEFAULT_SLIDER_WIDTH),
//		XRect(0,DEFAULT_SLIDER_WIDTH),SLIDER_TYPE_HORIZONTAL,100.0f,0.0f,XVec2::zero);
	m_horizontalSlider.initWithoutSkin(XRect(0,0,m_mouseRect.getWidth() - DEFAULT_SLIDER_WIDTH,DEFAULT_SLIDER_WIDTH),
		100.0f,0.0f,SLIDER_TYPE_HORIZONTAL);
	XCtrlManager.decreaseAObject(&m_horizontalSlider);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_horizontalSlider);
#endif
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_scale.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale.y);
	m_horizontalSlider.setScale(m_scale);
	m_horizontalSlider.setEventProc(ctrlProc,this);
	m_horizontalSlider.setWithAction(XFalse);
	m_needShowHSlider = XFalse;			//是否需要显示水平滑动条

	m_textWidth = font.getTextSize().x * font.getScale().x * 0.5f;
	m_curTextHeight = font.getTextSize().y * font.getScale().y;
	//计算当前可以显示多少行
	m_canShowLineSum = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight())/m_curTextHeight;	//不应该进去高度，而应该减去buttom ，以后在修正
	//计算当前一行可以显示多少个字符(这个0.5是因为显示英文字母却使用中文字库)
	//m_curShowLineWidth = (m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth())/m_textWidth;	//不应该减去宽度，而应该减去right，以后在修正

	m_curStartPixels = 0;
	m_curMaxLineWidthPixels = 0;
	m_maxLineWidthPixels = m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth();

	m_curMouseRect.set(m_position + m_mouseRect.getLT() * m_scale,
		m_position + (m_mouseRect.getRB() - 
			XVec2(m_verticalSlider.getMouseRectWidth(),m_horizontalSlider.getMouseRectHeight())) * m_scale);	//当前的鼠标响应范围
	//为每行分配内存空间
	m_lineStr = XMem::createArrayMem<XMultiTextLineString>(m_canShowLineSum);
	if(m_lineStr == NULL) return XFalse;

	m_textColor.set(0.0f,1.0f);
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_lineStr[i].isEnable = XFalse;
		if(!m_lineStr[i].curText.setACopy(font)) return XFalse;
		m_lineStr[i].curText.setMaxStrLen(MAX_LINE_STRING_LENGTH);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_lineStr[i].curText));
#endif
		m_lineStr[i].curText.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
		m_lineStr[i].curText.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);	 //设置字体上对齐
		m_lineStr[i].curText.setColor(m_textColor);
		m_lineStr[i].curText.setPosition(m_position.x + (m_mouseRect.left - m_curStartPixels) * m_scale.x,
			m_position.y + m_mouseRect.top * m_scale.y + m_curTextHeight * i);
		m_lineStr[i].curText.setScale(m_scale);
	//	m_lineStr[i].showLength = m_curShowLineWidth;
	//	m_lineStr[i].showStart = 0;
		m_lineStr[i].haveAllSelect = XFalse;
		//m_lineStr[i].showString = "";
	}
	//为整个字符串分配内存空间
	std::string *tmpStr = NULL;
	for(int i = 0;i < MAX_LINE_SUM;++ i)
	{
		tmpStr = XMem::createMem<std::string>();
		if(tmpStr == NULL)
		{
			XMem::XDELETE_ARRAY(m_lineStr);
			return XFalse;
		}
		m_freeStr.push_back(tmpStr);
	}

	m_haveSelect = XFalse;		//是否在字符串中有选择
	m_selectLineOrder = -1;

	m_isVisible = m_isEnable = m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	setString(str);
	return XTrue;
}
void XMultiText::updateLineStr()
{//将实际的字符串分配到各行去
	//if(!m_isInited) return;	//如果没有初始化直接退出
	//下面对这个方法进行修改
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_lineStr[i].isEnable = XFalse;
		m_lineStr[i].curText.disClip();
	}
//	if(m_showStartLine == 0) m_lineStr[0].lineHeadorder = 0;//如果是从字符串开头显示，则需要初始化第一行的头位字符串开始

	int maxlineWidth = 0;
	//机械的翻译
	int i = 0;
	for(auto it = m_curStr.begin();it != m_curStr.end();++ it, ++ i)
	{
		if(i >= m_showStartLine && i < m_showStartLine + m_canShowLineSum)
		{//需要显示的行;
			m_lineStr[i - m_showStartLine].isEnable = XTrue;
			m_lineStr[i - m_showStartLine].curText.setString((*it)->c_str());
			m_lineStr[i - m_showStartLine].lineWidthPixel = (*it)->length() * m_lineStr[i - m_showStartLine].curText.getTextSize().x * 0.5f;
			if(m_lineStr[i - m_showStartLine].lineWidthPixel > maxlineWidth)
			{
				maxlineWidth = m_lineStr[i - m_showStartLine].lineWidthPixel;
			}
			m_lineStr[i - m_showStartLine].curText.setClipRect(m_curStartPixels,0.0f,m_maxLineWidthPixels + m_curStartPixels,
				m_lineStr[i - m_showStartLine].curText.getTextSize().y);//设置字符串裁剪
			//这里位置也需要做相应的改变
			//（尚未完成）
			m_lineStr[i - m_showStartLine].curText.setPosition(m_position.x + (m_mouseRect.left - m_curStartPixels) * m_scale.x,
				m_position.y + m_mouseRect.top * m_scale.y + m_curTextHeight * (i - m_showStartLine));

//			m_lineStr[i - m_showStartLine].lineHeadorder = index;
//			m_lineStr[i - m_showStartLine].lineEndOrder = i;
		}else
		{//不需要显示的行
			int w = (*it)->length() * m_lineStr[0].curText.getTextSize().x * 0.5f;
			if(maxlineWidth < w) maxlineWidth = w;
		}
	}
	m_curMaxLineSum = m_curStr.size();
	if(m_curMaxLineSum > m_canShowLineSum)
	{//如果实际的行数大于显示的最大行数，则需要显示纵向拖动条
		m_needShowVSlider = XTrue;
		m_verticalSlider.setCurValue(m_showStartLine);
		m_verticalSlider.setRange(m_curMaxLineSum - m_canShowLineSum,0.0f);
	}else
	{
		m_needShowVSlider = XFalse;
	}
	//判断是否需要显示水平拖动条
	m_curMaxLineWidthPixels = maxlineWidth;
	if(m_curMaxLineWidthPixels > m_maxLineWidthPixels)
	{//如果实际的行数大于显示的最大行数，则需要显示纵向拖动条
		m_needShowHSlider = XTrue;
		m_horizontalSlider.setCurValue(m_curStartPixels);
		m_horizontalSlider.setRange(m_curMaxLineWidthPixels - m_maxLineWidthPixels,0.0f);
	}else
	{
		m_needShowHSlider = XFalse;
		if(m_curStartPixels != 0)
		{//重新刷新一次显示，以便于保证显示的正确性
			m_curStartPixels = 0;
			m_horizontalSlider.setCurValue(m_curStartPixels);
			updateLineStr();
		}else
		{
			m_curStartPixels = 0;
			m_horizontalSlider.setCurValue(m_curStartPixels);
		}
	}
}
void XMultiText::draw()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	if(m_withoutTex)
	{
		if(!m_isEnable) 
		{
			XRender::drawFillRectA(m_position + m_mouseRect.getLT() * m_scale,
				(m_mouseRect.getSize() - XVec2(m_verticalSlider.getMouseRectWidth(),m_horizontalSlider.getMouseRectHeight())) * m_scale, 
				XCCS::specialColor * m_color,true);
		}else 
		{
			XRender::drawFillRectA(m_position + m_mouseRect.getLT() * m_scale,
				(m_mouseRect.getSize() - XVec2(m_verticalSlider.getMouseRectWidth(),m_horizontalSlider.getMouseRectHeight())) * m_scale, 
				XCCS::normalColor * m_color,true);
		}
		//if(m_haveSelect) m_spriteSelect.draw(&(m_mutiEditSelect->m_texture));	//显示选择的范围
		if(m_actionMD.getIsEnd())
		{
			if(m_haveSelect && m_selectLineOrder >= m_showStartLine && m_selectLineOrder < m_showStartLine + m_canShowLineSum)
			{//下面显示选择的标签
				int index = m_selectLineOrder - m_showStartLine;
				XRender::drawFillRectExA(m_position + XVec2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y + index * m_curTextHeight),
					XVec2((m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
					m_curTextHeight),XCCS::onColor * m_color);
			}
		}else
		{
			switch(m_actionType)
			{
			case MLTTXT_ACTION_TYPE_IN:		//选项出现
				if(m_actionPosition >= m_showStartLine && m_actionPosition < m_showStartLine + m_canShowLineSum)
				{
					XRender::drawFillRectExA(m_position + XVec2(m_mouseRect.left * m_scale.x, m_mouseRect.top * m_scale.y +
						(m_actionPosition - m_showStartLine) * m_curTextHeight + m_curTextHeight * (1.0f - m_actionMD.getCurData()) * 0.5f),
						XVec2(m_maxLineWidthPixels * m_scale.x,m_curTextHeight * m_actionMD.getCurData()),
						XCCS::onColor * m_color);
				}
				break;
			case MLTTXT_ACTION_TYPE_MOVE:	//选项移动
				if(m_actionPosition >= m_showStartLine && m_actionPosition < m_showStartLine + m_canShowLineSum)
				{//正常情况
					float pos = XMath::lineSlerp<float>((m_actionPosition - m_showStartLine) * m_curTextHeight,
						(m_selectLineOrder - m_showStartLine) * m_curTextHeight,m_actionMD.getCurData());
					XRender::drawFillRectExA(m_position + m_mouseRect.getLT() * m_scale + XVec2(0,pos),
						XVec2(m_maxLineWidthPixels * m_scale.x,m_curTextHeight),
						XCCS::onColor * m_color);
				}else
				if(m_actionPosition < m_showStartLine)
				{//上边越界
					float pos = XMath::lineSlerp<float>(-1.0f * m_curTextHeight,
						(m_selectLineOrder - m_showStartLine) * m_curTextHeight,m_actionMD.getCurData());
					float h = m_curTextHeight;
					if(pos < 0.0f)
					{
						h += pos;
						pos = 0.0f;
					}
					if(h > 0.0f)
					{
						XRender::drawFillRectExA(m_position + m_mouseRect.getLT() * m_scale + XVec2(0, pos),
							XVec2(m_maxLineWidthPixels * m_scale.x,h),
							XCCS::onColor * m_color);
					}
				}else
				if(m_actionPosition >= m_showStartLine + m_canShowLineSum)
				{//下边越界
					float pos = XMath::lineSlerp<float>(m_canShowLineSum * m_curTextHeight,
						(m_selectLineOrder - m_showStartLine) * m_curTextHeight,m_actionMD.getCurData());
					float h = m_curTextHeight;
					if(pos + h > m_canShowLineSum * m_curTextHeight)
					{
						h += (m_canShowLineSum * m_curTextHeight - pos - h);
					}
					if(h > 0.0f)
					{
						XRender::drawFillRectExA(m_position + m_mouseRect.getLT() * m_scale + XVec2(0.0f, pos),
							XVec2(m_maxLineWidthPixels * m_scale.x,h),
							XCCS::onColor * m_color);
					}
				}
				break;
			case MLTTXT_ACTION_TYPE_DCLICK:	//双击
			case MLTTXT_ACTION_TYPE_OUT:	//取消选择
				if(m_actionPosition >= m_showStartLine && m_actionPosition < m_showStartLine + m_canShowLineSum)
				{
					XRender::drawFillRectExA(m_position + m_mouseRect.getLT() * m_scale + XVec2(0.0f,
						(m_actionPosition - m_showStartLine) * m_curTextHeight + m_curTextHeight * (1.0f - m_actionMD.getCurData()) * 0.5f),
						XVec2(m_maxLineWidthPixels * m_scale.x,m_curTextHeight * m_actionMD.getCurData()),
						XCCS::onColor * m_color);
				}
				break;
			}
		}
		//显示当前输入的字符串
		for(int i = 0;i < m_canShowLineSum;++ i)
		{
			if(m_lineStr[i].isEnable)
			{
				//if(m_lineStr[i].haveAllSelect)
				//{//显示这一行被选中
				//	drawFillRectExA(m_position + XVec2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y + i * m_curTextHeight),
				//		XVec2((m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
				//		m_curTextHeight),0.85f * m_color.r,0.85f * m_color.g,0.85f * m_color.b,m_color.a);
				//}
				m_lineStr[i].curText.draw();
			}
		}
		if(!m_isEnable) 
		{
			XRender::drawFillRectExA(m_position + XVec2(m_mouseRect.right - m_verticalSlider.getMouseRectWidth(), m_mouseRect.top) * m_scale,
				XVec2(m_verticalSlider.getMouseRectWidth(), m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight()) * m_scale,
				XCCS::downColor * m_color, true);
			XRender::drawFillRectExA(m_position + XVec2(m_mouseRect.left, m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale,
				XVec2(m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth(), m_horizontalSlider.getMouseRectHeight()) * m_scale,
				XCCS::downColor * m_color, true);
		}else 
		{
			XRender::drawFillRectExA(m_position + XVec2(m_mouseRect.right - m_verticalSlider.getMouseRectWidth(), m_mouseRect.top) * m_scale,
				XVec2(m_verticalSlider.getMouseRectWidth(), m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight()) * m_scale,
				XCCS::lightSpecialColor * m_color, true);
			XRender::drawFillRectExA(m_position + XVec2(m_mouseRect.left, m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale,
				XVec2(m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth(), m_horizontalSlider.getMouseRectHeight()) * m_scale,
				XCCS::lightSpecialColor * m_color, true);
		}
		//显示滑动条
		if(m_needShowVSlider) m_verticalSlider.draw();
		if(m_needShowHSlider) m_horizontalSlider.draw();
		//if(m_mutiEditUpon != NULL) m_spriteBackGround.draw(m_mutiEditUpon);
		if(!m_isEnable) 
		{
			XRender::drawFillRectExA(m_position + XVec2(m_mouseRect.right - m_verticalSlider.getMouseRectWidth(),
				m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale,
				XVec2(m_verticalSlider.getMouseRectWidth(), m_horizontalSlider.getMouseRectHeight()) * m_scale,
				XCCS::blackDownColor * m_color,true);
			//m_spriteBackGround.draw(m_mutiEditDisable);	//如果无效则显示无效,不显示插入符号
		}else 
		{
			XRender::drawFillRectExA(m_position + XVec2(m_mouseRect.right - m_verticalSlider.getMouseRectWidth(),
				m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale,
				XVec2(m_verticalSlider.getMouseRectWidth(), m_horizontalSlider.getMouseRectHeight()) * m_scale,
				XCCS::lightMouseColor * m_color, true);
			//m_spriteBackGround.draw(m_mutiEditNormal); //如果有效则显示有效,显示插入符号
		}
	}else
	{
		if(!m_isEnable) m_spriteBackGround.draw(m_multiEditDisable);	//如果无效则显示无效,不显示插入符号
		else m_spriteBackGround.draw(m_multiEditNormal); //如果有效则显示有效,显示插入符号
		//if(m_haveSelect) m_spriteSelect.draw(&(m_mutiEditSelect->m_texture));	//显示选择的范围
		//显示当前输入的字符串
		for(int i = 0;i < m_canShowLineSum;++ i)
		{
			if (!m_lineStr[i].isEnable) continue;
			if(m_lineStr[i].haveAllSelect)
				m_lineStr[i].spriteSelect.draw(m_multiEditSelect);
			m_lineStr[i].curText.draw();
		}
		//显示滑动条
		if(m_needShowVSlider) m_verticalSlider.draw();
		if(m_needShowHSlider) m_horizontalSlider.draw();
		if(m_multiEditUpon != NULL) m_spriteBackGround.draw(m_multiEditUpon);
	}
}
XBool XMultiText::mouseProc(const XVec2& p,XMouseState mouseState)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	if(m_isSilent) return XFalse;
	
	if(m_needShowVSlider) 
	{
		m_verticalSlider.mouseProc(p,mouseState);
		if(m_curMouseRect.isInRect(p))
		{//这里响应滚轮事件
			switch(mouseState)
			{
			case MOUSE_WHEEL_UP_DOWN:
				m_verticalSlider.setCurValue(m_verticalSlider.getCurValue() - 1.0f);
				break;
			case MOUSE_WHEEL_DOWN_DOWN:
				m_verticalSlider.setCurValue(m_verticalSlider.getCurValue() + 1.0f);
				break;
			}
		}
	}
	if(m_needShowHSlider) m_horizontalSlider.mouseProc(p,mouseState);
	//判断鼠标点击是否在范围内
	if(mouseState == MOUSE_LEFT_BUTTON_UP && m_curMouseRect.isInRect(p))
	{//点选有效，计算点选的行值
		int tmp = (p.y - m_curMouseRect.top) / m_curTextHeight;	//计算当前选择的行数
		tmp = XMath::clamp(tmp,0, m_canShowLineSum - 1);
		if(tmp + m_showStartLine < m_curMaxLineSum)
		{
			if(m_haveSelect && m_selectLineOrder != tmp + m_showStartLine)
			{//这样则需要剔除原有的选择，整个只能有一个处于选择状态
				if(m_selectLineOrder - m_showStartLine >= 0 && m_selectLineOrder - m_showStartLine < m_canShowLineSum)
					m_lineStr[m_selectLineOrder - m_showStartLine].haveAllSelect = XFalse;	//注意这里是显示一整行被选中，所以，不需要标记头和尾
				setAction(MLTTXT_ACTION_TYPE_MOVE,m_selectLineOrder);	//标签移动
			}
			if(!m_haveSelect || m_selectLineOrder != tmp + m_showStartLine)
			{
				if(!m_haveSelect) setAction(MLTTXT_ACTION_TYPE_IN, tmp + m_showStartLine);	//有选择
				m_haveSelect = XTrue;
				m_selectLineOrder = tmp + m_showStartLine;
				if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,MLTTXT_SELECT);
				else XCtrlManager.eventProc(m_objectID,MLTTXT_SELECT);
				//标记所选的行
				m_lineStr[tmp].haveAllSelect = XTrue;	//注意这里是显示一整行被选中，所以，不需要标记头和尾
				//计算选取的位置
				if(!m_withoutTex)
				{
					m_lineStr[tmp].spriteSelect.setPosition(m_curMouseRect.left, m_curMouseRect.top + m_curTextHeight * tmp);
					m_lineStr[tmp].spriteSelect.setScale(m_curMouseRect.getWidth() / m_multiEditSelect->textureSize.x,
						m_curTextHeight / m_multiEditSelect->textureSize.y);
				}
			}
		}
		m_isBeChoose = XTrue;
	}
	return XTrue;
}
void XMultiText::setPosition(const XVec2& p)
{
	if(!m_isInited) return;
	m_position = p;
	m_curMouseRect.set(m_position + m_mouseRect.getLT() * m_scale,
		m_position + (m_mouseRect.getRB() - 
			XVec2(m_verticalSlider.getMouseRectWidth(), m_horizontalSlider.getMouseRectHeight())) * m_scale);	//当前的鼠标响应范围

	if(!m_withoutTex) m_spriteBackGround.setPosition(m_position);
	m_verticalSlider.setPosition(m_position + XVec2(m_mouseRect.right - m_verticalSlider.getMouseRectWidth(), m_mouseRect.top) * m_scale);
	m_horizontalSlider.setPosition(m_position + XVec2(m_mouseRect.left, m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale);
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_lineStr[i].curText.setPosition(m_position.x + (m_mouseRect.left - m_curStartPixels) * m_scale.x,
			m_position.y + m_mouseRect.top * m_scale.y + m_curTextHeight * i);
	}
	if(m_haveSelect && !m_withoutTex)
	{//存在选取
		int tmp = m_selectLineOrder - m_showStartLine;
		m_lineStr[tmp].spriteSelect.setPosition(m_curMouseRect.left,m_curMouseRect.top + m_curTextHeight * tmp);
	}
}
void XMultiText::setScale(const XVec2& s)
{
	if(!m_isInited || s.x <= 0 || s.y <= 0) return;
	m_scale = s;
	m_curMouseRect.set(m_position + m_mouseRect.getLT() * m_scale,
		m_position + (m_mouseRect.getRB() -
			XVec2(m_verticalSlider.getMouseRectWidth(), m_horizontalSlider.getMouseRectHeight())) * m_scale);	//当前的鼠标响应范围
	if(!m_withoutTex)
	{
		m_spriteBackGround.setPosition(m_position);
		m_spriteBackGround.setScale(m_scale);
	}
	m_verticalSlider.setPosition(m_position + XVec2(m_mouseRect.right - m_verticalSlider.getMouseRectWidth(), m_mouseRect.top) * m_scale);
	m_verticalSlider.setScale(m_scale);
	m_horizontalSlider.setPosition(m_position + XVec2(m_mouseRect.left, m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale);
	m_horizontalSlider.setScale(m_scale);

	m_lineStr[0].curText.setScale(m_scale);
	m_textWidth = m_lineStr[0].curText.getTextSize().x * m_lineStr[0].curText.getScale().x * 0.5f;
	m_curTextHeight = m_lineStr[0].curText.getTextSize().y * m_lineStr[0].curText.getScale().y;
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_lineStr[i].curText.setPosition(m_position.x + (m_mouseRect.left - m_curStartPixels) * m_scale.x,
			m_position.y + m_mouseRect.top * m_scale.y + m_curTextHeight * i);
		m_lineStr[i].curText.setScale(m_scale);
	}
	if(m_haveSelect && !m_withoutTex)
	{//存在选取
		int tmp = m_selectLineOrder - m_showStartLine;
		m_lineStr[tmp].spriteSelect.setPosition(m_curMouseRect.left,m_curMouseRect.top + m_curTextHeight * tmp);
		m_lineStr[tmp].spriteSelect.setScale(m_curMouseRect.getWidth() / m_multiEditSelect->textureSize.x,
			m_curTextHeight / m_multiEditSelect->textureSize.y);
	}
}
void XMultiText::checkStr()			//检查字符串的单行宽度是否大于限制
{
	for(auto it = m_curStr.begin();it != m_curStr.end();++ it)
	{
		if ((*it)->length() < MAX_LINE_STRING_LENGTH - 2) continue;
		//长度超过限制，这里进行截断
		*(*it) = (*it)->substr(0,MAX_LINE_STRING_LENGTH - 2);
	}
}
void XMultiText::clearAllStrData()
{//将所有现在使用的数据回收
	for (auto it = m_curStr.begin(); it != m_curStr.end(); ++it)
	{
		m_freeStr.push_back(*it);
	}
	m_curStr.clear();
}
bool XMultiText::addAStrData(const char * str)
{
	bool ret = false;
	std::string *tmp = NULL;
	if (m_freeStr.size() > 0)
	{//足够
		tmp = m_freeStr[0];
		m_freeStr.pop_front();
	}
	else
	{//不足
		tmp = m_curStr[0];
		m_curStr.pop_front();
		ret = true;
	}
	*tmp = str;
	m_curStr.push_back(tmp);
	return ret;
}
void XMultiText::setString(const char *str)
{
	if (!m_isInited ||	//必须要初始化
		str == NULL) return;		//空指针需要退出
	//这里需要将字符串分段放入
	clearAllStrData();
	int len = strlen(str) + 1;	//需要加上结束符
	if (len > 0)
	{
		char tmpStr[MAX_LINE_STRING_LENGTH];
		int start = 0;
		for (int i = 0; i < len; ++i)
		{
			if (str[i] < 0)
			{//中文
				++i;
			}
			else
				if (str[i] == '\0' || str[i] == '\n')
				{//一行的结束
					if (i - start >= MAX_LINE_STRING_LENGTH - 2)
					{//超过长度
						memcpy(tmpStr, str + start, MAX_LINE_STRING_LENGTH - 2);
						tmpStr[MAX_LINE_STRING_LENGTH - 1] = '\0';
					}
					else
					{
						memcpy(tmpStr, str + start, i - start);
						tmpStr[i - start] = '\0';
					}
					addAStrData(tmpStr);
					start = i + 1;
				}
		}
	}
	//这里最好能整理字符串，不要超出一行的最大显示，这样子有利于后面的统一处理
	checkStr();

	//处理字符串
	m_showStartLine = 0;
	updateLineStr();

	if (m_haveSelect) setAction(MLTTXT_ACTION_TYPE_OUT, m_selectLineOrder);	//取消选择
	m_haveSelect = XFalse;		//是否在字符串中有选择
	m_selectLineOrder = -1;
	for (int i = 0; i < m_canShowLineSum; ++i)
	{
		m_lineStr[i].haveAllSelect = XFalse;
	}
}
void XMultiText::addALine(const char *str)		//向字符串中添加字符串
{
	if (!m_isInited ||	//如果没有初始化直接退出
		str == NULL) return;
	char tmpStr[MAX_LINE_STRING_LENGTH];
	if (strlen(str) >= MAX_LINE_STRING_LENGTH - 2)
	{
		memcpy(tmpStr, str, MAX_LINE_STRING_LENGTH - 2);
		tmpStr[MAX_LINE_STRING_LENGTH - 1] = '\0';
	}
	else
	{
		strcpy_s(tmpStr, MAX_LINE_STRING_LENGTH, str);
	}
	int index = XString::getCharPosition(tmpStr, '\n');
	if (index >= 0) tmpStr[index] = '\0';	//丢弃多余的部分

	m_mutex.Lock();
	//将这一行插入
	if (addAStrData(tmpStr))
	{//注意这里没有处理翻滚造成的很多状态变更(尚未完成)
		//选择的行需要上移
		if (m_haveSelect)
		{
			--m_selectLineOrder;
			if (m_selectLineOrder < 0)
			{//超出范围
				m_selectLineOrder = 0;
				m_haveSelect = XFalse;
			}
		}
		//目前显示范围需要改变
	}

	checkStr();
	if (m_needShowVSlider && m_verticalSlider.getCurValue() == m_verticalSlider.getMaxValue())
	{
		updateLineStr();
		m_verticalSlider.setCurValue(m_verticalSlider.getMaxValue());
	}
	else
	{
		updateLineStr();
	}
	m_mutex.Unlock();
}
void XMultiText::addString(const char *str)			//向字符串中添加字符串
{
	if (!m_isInited ||		//如果没有初始化直接退出
		str == NULL) return;
	int len = strlen(str) + 1;
	if (len <= 0)
	{
		addALine("");
		return;
	}
	char tmpStr[MAX_LINE_STRING_LENGTH];
	int start = 0;
	for (int i = 0; i < len; ++i)
	{
		if (str[i] < 0)
		{//中文
			++i;
		}
		else
			if (str[i] == '\0' || str[i] == '\n')
			{//一行的结束
				if (i - start >= MAX_LINE_STRING_LENGTH - 2)
				{//超过长度
					memcpy(tmpStr, str + start, MAX_LINE_STRING_LENGTH - 2);
					tmpStr[MAX_LINE_STRING_LENGTH - 1] = '\0';
				}
				else
				{
					memcpy(tmpStr, str + start, i - start);
					tmpStr[i - start] = '\0';
				}
				addALine(tmpStr);
				start = i + 1;
			}
	}
}
XBool XMultiText::setACopy(const XMultiText &temp)
{
	if(& temp == this) return XTrue;	//防止自身赋值
	if(!temp.m_isInited) return XFalse;
	
	//not finished
	if(m_isInited) release();
	if(!XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
		XObjManager.addAObject(this);
#endif
	}

	m_isInited = temp.m_isInited;				//是否初始化
	if(m_resInfo != NULL) XResManager.releaseResource(m_resInfo);
	m_resInfo = XResManager.copyResource(temp.m_resInfo);
	m_withoutTex = temp.m_withoutTex;
	
	m_needShowVSlider = temp.m_needShowVSlider;			//是否需要显示垂直滑动条
	m_verticalSlider.setACopy(temp.m_verticalSlider);		//垂直滑动条
	XCtrlManager.decreaseAObject(&m_verticalSlider);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_verticalSlider);
#endif
	m_verticalSlider.setEventProc(ctrlProc,this);
	m_needShowHSlider = temp.m_needShowHSlider;			//是否需要显示水平滑动条
	m_horizontalSlider.setACopy(temp.m_horizontalSlider);	//水平滑动条
	XCtrlManager.decreaseAObject(&m_horizontalSlider);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_horizontalSlider);
#endif
	m_horizontalSlider.setEventProc(ctrlProc,this);

	std::string *tmpStr = NULL;
	for(unsigned int i = 0;i < temp.m_freeStr.size();++ i)
	{
		tmpStr = XMem::createMem<std::string>();
		if(tmpStr == NULL) return XFalse;
		m_freeStr.push_back(tmpStr);
	}
	for(auto it = temp.m_curStr.begin();it != temp.m_curStr.end();++ it)
	{
		tmpStr = XMem::createMem<std::string>();
		if(tmpStr == NULL) return XFalse;
		*tmpStr = *(*it);
		m_curStr.push_back(tmpStr);
	}

	m_multiEditNormal = temp.m_multiEditNormal;		//输入框普通状态
	m_multiEditDisable = temp.m_multiEditDisable;		//输入框无效状态
	m_multiEditSelect = temp.m_multiEditSelect;		//输入框片选颜色
	m_multiEditUpon = temp.m_multiEditUpon;

	m_spriteBackGround.setACopy(temp.m_spriteBackGround);	//用于显示输入框的背景贴图
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteBackGround);
#endif
	//m_textPosition = temp.m_textPosition;			//文字显示的位置，是相对于控件的位置来定的
	m_textSize = temp.m_textSize;				//文字显示的尺寸，这个尺寸会与空间的缩放尺寸叠加

	m_haveSelect = temp.m_haveSelect;		//是否在字符串中有选择
	m_selectLineOrder = temp.m_selectLineOrder;	//当前文档中被选择的是哪一行

	m_showStartLine = temp.m_showStartLine;		//当前显示的第一行是总的第几行
	m_canShowLineSum = temp.m_canShowLineSum;		//当前可以显示多少行
	//m_curShowLineWidth = temp.m_curShowLineWidth;		//当前一行可以显示的宽度
	m_curMaxLineSum = temp.m_curMaxLineSum;	//当前字符串中实际的行数
	//m_curMaxLineLength = temp.m_curMaxLineLength;	//当前字符串中实际的行宽

	m_curStartPixels = temp.m_curStartPixels;
	m_curMaxLineWidthPixels = temp.m_curMaxLineWidthPixels;
	m_maxLineWidthPixels = temp.m_maxLineWidthPixels;

	m_lineStr = XMem::createArrayMem<XMultiTextLineString>(m_canShowLineSum);
	if(m_lineStr == NULL) return XFalse;

	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		//m_lineStr[i].showString = temp.m_lineStr[i].showString;
		m_lineStr[i].isEnable = temp.m_lineStr[i].isEnable;				//这一行是否要显示，是否有效
	//	m_lineStr[i].showStart = temp.m_lineStr[i].showStart;				//这行字符串显示部分的头在当前行的偏移
	//	m_lineStr[i].showLength = temp.m_lineStr[i].showLength;				//显示字符串的长度
		m_lineStr[i].curText.setACopy(temp.m_lineStr[i].curText);		//用于显示这一行字符串的字体
		//m_lineStr[i].curText.setMaxStrLen(MAX_LINE_STRING_LENGTH);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_lineStr[i].curText));
#endif
//		m_lineStr[i].lineHeadorder = temp.m_lineStr[i].lineHeadorder;			//这一行字符在全文中的头位置
//		m_lineStr[i].lineEndOrder = temp.m_lineStr[i].lineEndOrder;			//这一行字符在全文中的尾位置
		//m_lineStr[i].lineLength = temp.m_lineStr[i].lineLength;				//这一行字符串的长度
		//m_lineStr[i].isEnterEnd = temp.m_lineStr[i].isEnterEnd;			//是否因为换行而换行显示1，或者是因为显示字符串的长度限制而换行0
		m_lineStr[i].spriteSelect.setACopy(temp.m_lineStr[i].spriteSelect);		//字符串显示选择背景颜色的精灵
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_lineStr[i].spriteSelect));
#endif
		m_lineStr[i].haveAllSelect = temp.m_lineStr[i].haveAllSelect;			//该行中是否存被整体选择
	}

//	m_funInputChenge = temp.m_funInputChenge;		//输入内容发生改变的时候调用
//	m_funInputOver = temp.m_funInputOver;		//确认输入结束之后的时候调用
//	m_pClass = temp.m_pClass;

	m_timer = temp.m_timer;	//这是插入符号闪烁时使用的时间标记，这个时间使用不准确的计时方式
	m_textColor = temp.m_textColor;
	m_textWidth = temp.m_textWidth;	//这是插入符号闪烁时使用的时间标记，这个时间使用不准确的计时方式
	m_curTextHeight = temp.m_curTextHeight;	//这是插入符号闪烁时使用的时间标记，这个时间使用不准确的计时方式
	return XTrue;
}
XBool XMultiText::exportData(const char *fileName)		//数据导出
{
	if(!m_isInited) return XFalse;		//如果没有初始化直接退出
	FILE *fp;
	if(fileName == NULL)
	{
		char fileNameT[] = "XMultiText_xxx.txt";
		fileNameT[10] = (m_objectID /100) % 10 + '0';
		fileNameT[11] = (m_objectID /10) % 10 + '0';
		fileNameT[12] = m_objectID % 10 + '0';	 
		if((fp = fopen(fileNameT,"wb")) == NULL)
		{
			printf("File open error!\n");
			return XFalse;
		}
	}else
	{
		if((fp = fopen(fileName,"wb")) == NULL)
		{
			printf("File open error!\n");
			return XFalse;
		}
	}
	for(auto it = m_curStr.begin();it != m_curStr.end();++ it)
	{
		fprintf(fp,"%s\n",(*it)->c_str());
	}
	fclose(fp);
	return XTrue;
}
XBool XMultiText::importData(const char *fileName)		//数据导入
{
	if(!m_isInited) return XFalse;		//如果没有初始化直接退出
	FILE *fp;
	if(fileName == NULL)
	{
		char fileNameT[] = "XMultiText_xxx.txt";
		fileNameT[10] = (m_objectID /100) % 10 + '0';
		fileNameT[11] = (m_objectID /10) % 10 + '0';
		fileNameT[12] = m_objectID % 10 + '0';	 
		if((fp = fopen(fileNameT,"rb")) == NULL)
		{
			printf("File open error!\n");
			return XFalse;
		}
	}else
	{
		if((fp = fopen(fileName,"rb")) == NULL)
		{
			printf("File open error!\n");
			return XFalse;
		}
	}
	//逐行读取数据并推入
	char tempStr[1024];
	if(fscanf(fp,"%s\n",tempStr) == 1)
		addALine(tempStr);
	fclose(fp);
	return XTrue;
}
XBool XMultiText::saveState(TiXmlNode &e)
{
	if(!m_needSaveAndLoad) return XTrue;	//如果不需要保存则直接返回
	std::string tmpStr = "";
	for (auto it = m_curStr.begin(); it != m_curStr.end(); ++it)
	{
		tmpStr += (*(*it)) + "\n";
	}
	if(!XXml::addLeafNode(e,m_ctrlName.c_str(),tmpStr)) return XFalse;
	return XTrue;
}
XBool XMultiText::loadState(TiXmlNode *e)
{
	if(!m_needSaveAndLoad) return XTrue;	//如果不需要保存则直接返回
	std::string tmpStr;
	if(XXml::getXmlAsString(e,m_ctrlName.c_str(),tmpStr) == NULL) return XFalse;
	setString(tmpStr.c_str());
	return XTrue;
}
XBool XMultiText::deleteSelectLine()		//删除当前选择的一行
{
	if(!m_isInited ||		//如果没有初始化直接退出
		!m_haveSelect) return XFalse;
	std::string tmpStr = "";
	auto sT = m_curStr.begin() + m_selectLineOrder;
	for (auto it = m_curStr.begin(); it != m_curStr.end(); ++it)
	{
		if(it == sT) continue;
		tmpStr += (*(*it)) + "\n";
	}
	setString(tmpStr.c_str());
	//setString已经进行了取消选择的操作
	//m_haveSelect = false;	//取消选择
	return XTrue;
}
XBool XMultiText::moveUpSelectLine()	//将选择的一行上移
{
	if(!m_isInited ||		//如果没有初始化直接退出
		!m_haveSelect) return XFalse;
	if(!moveUpLine(m_selectLineOrder))
	{//移动失败
		return XFalse;
	}else
	{
		m_lineStr[m_selectLineOrder - m_showStartLine].haveAllSelect = XFalse;
		-- m_selectLineOrder;
		if(m_selectLineOrder < m_showStartLine)
		{
			m_showStartLine = m_selectLineOrder;
			updateLineStr();
		}
		m_lineStr[m_selectLineOrder - m_showStartLine].haveAllSelect = XTrue;
		if(!m_withoutTex)
		{
			m_lineStr[m_selectLineOrder - m_showStartLine].spriteSelect.setPosition(m_curMouseRect.left,
				m_curMouseRect.top + m_curTextHeight * (m_selectLineOrder - m_showStartLine));
			m_lineStr[m_selectLineOrder - m_showStartLine].spriteSelect.setScale(m_curMouseRect.getWidth() / m_multiEditSelect->textureSize.x,
							m_curTextHeight / m_multiEditSelect->textureSize.y);
		}
		return XTrue;
	}
}
XBool XMultiText::moveDownSelectLine()	//将选择的一行下移
{
	if(!m_isInited ||		//如果没有初始化直接退出
		!m_haveSelect) return XFalse;
	if(!moveDownLine(m_selectLineOrder))
	{//移动失败
		return XFalse;
	}else
	{
		m_lineStr[m_selectLineOrder - m_showStartLine].haveAllSelect = XFalse;
		++ m_selectLineOrder;
		if(m_selectLineOrder >= m_showStartLine + m_canShowLineSum)
		{
			++ m_showStartLine;
			updateLineStr();
		}
		m_lineStr[m_selectLineOrder - m_showStartLine].haveAllSelect = XTrue;
		if(!m_withoutTex)
		{
			m_lineStr[m_selectLineOrder - m_showStartLine].spriteSelect.setPosition(m_curMouseRect.left,
				m_curMouseRect.top + m_curTextHeight * (m_selectLineOrder - m_showStartLine));
			m_lineStr[m_selectLineOrder - m_showStartLine].spriteSelect.setScale(m_curMouseRect.getWidth() / m_multiEditSelect->textureSize.x,
							m_curTextHeight / m_multiEditSelect->textureSize.y);
		}
		return XTrue;
	}
}
XBool XMultiText::insertALine(const char *str,int lineOrder)	//向文本的指定行插入一行
{
	if(!m_isInited ||		//如果没有初始化直接退出
		!m_haveSelect) return XFalse;
	if(lineOrder < 0 || lineOrder >= m_curMaxLineSum) return XFalse;	//非法的值则退出
	if(str == NULL || strlen(str) == 0) return XFalse;
	char tmpStr[MAX_LINE_STRING_LENGTH];
	if(strlen(str) >= MAX_LINE_STRING_LENGTH - 2)
	{
		memcpy(tmpStr,str,MAX_LINE_STRING_LENGTH - 2);
		tmpStr[MAX_LINE_STRING_LENGTH - 1] = '\0';
	}else
	{
		strcpy_s(tmpStr,MAX_LINE_STRING_LENGTH,str);
	}
	int index = XString::getCharPosition(tmpStr,'\n');
	if(index >= 0) tmpStr[index] = '\0';	//丢弃多余的部分

	if(m_freeStr.size() == 0)
	{
		if(lineOrder == 0) return XFalse;	//已经满了，不能再添加了
		//丢弃第一行再进行添加
		std::string *tmp = m_curStr[0];
		m_curStr.pop_back();
		*tmp = tmpStr;
		m_curStr.insert(m_curStr.begin() + lineOrder - 1,tmp);
		if(m_haveSelect && m_selectLineOrder < lineOrder) 
		{
			-- m_selectLineOrder;
			if(m_selectLineOrder < 0)
			{//超出范围
				m_selectLineOrder = 0;
				m_haveSelect = XFalse;
			}
		}
	}else
	{
		std::string *tmp = m_freeStr[0];
		m_freeStr.pop_back();
		*tmp = tmpStr;
		m_curStr.insert(m_curStr.begin() + lineOrder,tmp);
		if(m_haveSelect && m_selectLineOrder >= lineOrder) ++ m_selectLineOrder;
	}
	checkStr();
	updateLineStr();
	return XTrue;
}
XBool XMultiText::keyboardProc(int keyOrder,XKeyState keyState)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable ||
		!m_isBeChoose) return XFalse;		//如果无效则直接退出
	if(m_isSilent) return XFalse;
	if(keyState == KEY_STATE_UP)
	{
		switch(keyOrder)
		{
		case XKEY_UP:
			if(m_needShowVSlider) 
			{
				m_verticalSlider.m_isBeChoose = XTrue;
				m_verticalSlider.keyboardProc(XKEY_UP,KEY_STATE_UP);
				m_verticalSlider.m_isBeChoose = XFalse;
			}
			break;
		case XKEY_DOWN:
			if(m_needShowVSlider) 
			{
				m_verticalSlider.m_isBeChoose = XTrue;
				m_verticalSlider.keyboardProc(XKEY_DOWN,KEY_STATE_UP);
				m_verticalSlider.m_isBeChoose = XFalse;
			}
			break;
		case XKEY_LEFT:		//左右选择(尚未完成)
			if(m_needShowHSlider) 
			{
				m_horizontalSlider.m_isBeChoose = XTrue;
				m_horizontalSlider.keyboardProc(XKEY_LEFT,KEY_STATE_UP);
				m_horizontalSlider.m_isBeChoose = XFalse;
			}
			break;
		case XKEY_RIGHT:	//左右选择(尚未完成)
			if(m_needShowHSlider) 
			{
				m_horizontalSlider.m_isBeChoose = XTrue;
				m_horizontalSlider.keyboardProc(XKEY_RIGHT,KEY_STATE_UP);
				m_horizontalSlider.m_isBeChoose = XFalse;
			}
			break;
		}
	}
	return XTrue;
}
#if !WITH_INLINE_FILE
#include "XMultiText.inl"
#endif
}