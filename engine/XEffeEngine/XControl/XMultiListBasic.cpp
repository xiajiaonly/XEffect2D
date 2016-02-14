#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XMultiListBasic.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
namespace XE{
void XMultiListBasic::ctrlProc(void *pClass,int id,int eventID)
{
	XMultiListBasic &pPar = *(XMultiListBasic *)pClass;
	if(id ==  pPar.m_verticalSlider.getControlID())
	{
		if(eventID == XSlider::SLD_MOUSE_MOVE)
		{
			int temp = pPar.m_verticalSlider.getCurValue();
			if(temp != pPar.m_showStartLine)
			{
				//需要更新垂直片选的情况
				pPar.m_showStartLine = temp;
				pPar.updateSelectLine();
				pPar.updateShowChange();
			}
		}else
		if(eventID == XSlider::SLD_VALUE_CHANGE)
		{
			int temp = pPar.m_verticalSlider.getCurValue();
			if(temp != pPar.m_showStartLine)
			{
				pPar.m_showStartLine = temp;
				pPar.updateSelectLine();
				pPar.updateShowChange();
				pPar.m_verticalSlider.setCurValue(temp);
			}else
			{//微量移动时候的处理
				if(pPar.m_verticalSlider.getCurValue() > pPar.m_showStartLine)
				{
					++temp;
					pPar.m_showStartLine = temp;
					pPar.updateSelectLine();
					pPar.updateShowChange();
					pPar.m_verticalSlider.setCurValue(temp);
				}else
				if(pPar.m_verticalSlider.getCurValue() < pPar.m_showStartLine)
				{
					--temp;
					pPar.m_showStartLine = temp;
					pPar.updateSelectLine();
					pPar.updateShowChange();
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
			int temp = pPar.m_horizontalSlider.getCurValue();
			if(temp != pPar.m_showStartRow)
			{
				pPar.m_showStartRow = temp;
				pPar.updateShowChange();
			}
		}else
		if(eventID == XSlider::SLD_VALUE_CHANGE)
		{
			int temp = pPar.m_horizontalSlider.getCurValue();
			if(temp != pPar.m_showStartRow)
			{
				pPar.m_showStartRow = temp;
				pPar.updateShowChange();
				pPar.m_horizontalSlider.setCurValue(temp);
			}else
			{
				if(pPar.m_horizontalSlider.getCurValue() > pPar.m_showStartRow)
				{
					++temp;
					pPar.m_showStartRow = temp;
					pPar.updateShowChange();
					pPar.m_horizontalSlider.setCurValue(temp);
				}else
				if(pPar.m_horizontalSlider.getCurValue() < pPar.m_showStartRow)
				{
					--temp;
					pPar.m_showStartRow = temp;
					pPar.updateShowChange();
					pPar.m_horizontalSlider.setCurValue(temp);
				}
			}
		}
		return;
	}
	if(eventID == XCheck::CHK_STATE_CHANGE)
	{
		//将值更新上去
		int row = 0;		//那一列发生了变化
		XBool state = XFalse;	//发生了怎么样的变化
		int index = 0;
		for(int i = 0;i < min(pPar.m_canShowLineSum,pPar.m_tableLineSum - pPar.m_showStartLine);++ i)
		{
			if(pPar.m_check0State[i + pPar.m_showStartLine] != pPar.m_check0[i].getState()) 
			{
				state = pPar.m_check0[i].getState();
				row = 0;
				index = i + pPar.m_showStartLine;
			}
			if(pPar.m_check1State[i + pPar.m_showStartLine] != pPar.m_check1[i].getState())
			{
				state = pPar.m_check1[i].getState();
				row = 1;
				index = i + pPar.m_showStartLine;
			}

			pPar.m_check0State[i + pPar.m_showStartLine] = pPar.m_check0[i].getState();
			pPar.m_check1State[i + pPar.m_showStartLine] = pPar.m_check1[i].getState();
		}
		if(pPar.m_haveSelectFlag[index])	//这一行必须处于选中状态
		{
			for(int i = 0;i < pPar.m_tableLineSum;++ i)
			{
				if(pPar.m_haveSelectFlag[i])
				{//被选中的状态统一发生变化
					if(row == 0 && pPar.m_check0State[i] != state)
					{
						pPar.m_check0State[i] = state;
						if(i >= pPar.m_showStartLine && i < pPar.m_showStartLine + pPar.m_canShowLineSum)
						{
							pPar.m_check0[i - pPar.m_showStartLine].setState(state);
						}
					}else
					if(row == 1 && pPar.m_check1State[i] != state)
					{
						pPar.m_check1State[i] = state;
						if(i >= pPar.m_showStartLine && i < pPar.m_showStartLine + pPar.m_canShowLineSum)
						{
							pPar.m_check1[i - pPar.m_showStartLine].setState(state);
						}
					}
				}
			}
		}
		if(pPar.m_eventProc != NULL) pPar.m_eventProc(pPar.m_pClass,pPar.m_objectID,MLTLSTB_CHK_STATE_CHANGE);
		else XCtrlManager.eventProc(pPar.m_objectID,MLTLSTB_CHK_STATE_CHANGE);
		return;
	}
}
XMultiListBasic::XMultiListBasic()
:m_isInited(XFalse)					//是否初始化
,m_mutiListNormal(NULL)			//多列列表的普通状态
,m_mutiListDisable(NULL)		//多列列表的无效状态
,m_mutiListSelect(NULL)			//多列列表中被选中行的背景
,m_mutiListMove(NULL)			//移动标题时的对齐标签
,m_mutiListTitle(NULL)			//多列列表的标题背景
,m_mutiListTitleEnd(NULL)		//多列列表的标题分隔符
,m_needShowVSlider(XFalse)			//是否需要显示垂直滑动条
,m_needShowHSlider(XFalse)			//是否需要显示水平滑动条
,m_tableRowSum(0)				//表格中的列数
,m_tableRow(NULL)				//列的链表
,m_tableLineSum(0)				//表格中的行数
,m_tableBox(NULL)				//表中元素的链表
,m_mouseLeftButtonDown(XFalse)
,m_mouseMoveDown(XFalse)
,m_haveSelectFlag(NULL)
,m_check0(NULL)
,m_check1(NULL)
,m_check0State(NULL)
,m_check1State(NULL)
//,m_funSelectChange(NULL)
//,m_funCheckStateChange(NULL)
,m_resInfo(NULL)
,m_withoutTex(XFalse)
{
	m_ctrlType = CTRL_OBJ_MUTILIST;
}
void XMultiListBasic::releaseTempMemory()
{
	XMem::XDELETE_ARRAY(m_check0);
	XMem::XDELETE_ARRAY(m_check1);
	XMem::XDELETE_ARRAY(m_check0State);
	XMem::XDELETE_ARRAY(m_check1State);
	if(m_tableRow != NULL)
	{
		//删除所有的列信息
		XMultiListOneRow *tempRow = m_tableRow;
		XMultiListOneRow *temp;
		for(int i = 0;i < m_tableRowSum;++ i)
		{
			temp = tempRow;
			tempRow = tempRow->nextRow;
			//XMem::XDELETE_ARRAY(temp->title);
			XMem::XDELETE(temp);
		}
		m_tableRow = NULL;
	}
	if(m_tableBox != NULL)
	{
		//删除所有表格中的元素
		XMultiListOneBox *tempBox = m_tableBox;
		XMultiListOneBox *temp1;
		for(int i = 0;i < m_tableLineSum;++ i)
		{
			for(int j = 0;j < m_tableRowSum;++ j)
			{
				temp1 = tempBox;
				tempBox = tempBox->nextBox;
				//XMem::XDELETE_ARRAY(temp1->string);
				XMem::XDELETE(temp1);
			}
		}
		m_tableBox = NULL;
	}
	m_tableRowSum = 0;
	m_tableLineSum = 0;
	XMem::XDELETE_ARRAY(m_haveSelectFlag);
}
void XMultiListBasic::release()
{
	if(!m_isInited) return;
	releaseTempMemory();

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
XBool XMultiListBasic::init(const XVector2& position,		//空间所在的位置
		const XRect& Area,					//控件的实际显示区域
		const XMultiListSkin &tex,		//控件的贴图
		const XCheckSkin &checktex0,		//单选框的贴图
		const XCheckSkin &checktex1,		//单选框的贴图
		const XFontUnicode &font,			//控件中使用的字体
		float strSize,						//字体的缩放大小
		int rowSum,					//控件中的列数
		int lineSum,				//控件中的行数
		//const XMouseRightButtonMenu& mouseMenu,	//控件中使用的右键菜单(目前无效)
		const XSlider &vSlider,	//垂直滑动条
		const XSlider &hSlider)	//水平滑动条
{
	if(m_isInited) return XFalse;	//重复初始化
	//下面是参数检查
	if(Area.getWidth() <= 0 || Area.getHeight() <= 0) return XFalse;	//输入范围不能为空
	if(tex.mutiListDisable == NULL || tex.mutiListMove == NULL || tex.mutiListNormal == NULL
		|| tex.mutiListSelect == NULL || tex.mutiListTitle == NULL || tex.mutiListTitleEnd == NULL)
	{//需要的贴图不完整
		return XFalse;
	}
	if(rowSum < 0 || lineSum < 0 || (rowSum == 0 && lineSum > 0))
	{//不能为负数
		return XFalse;
	}
	//下面开始赋值
	m_position = position;
	m_mouseRect = Area;

	m_mutiListNormal = tex.mutiListNormal;			//多列列表的普通状态
	m_mutiListDisable = tex.mutiListDisable;			//多列列表的无效状态
	m_mutiListSelect = tex.mutiListSelect;			//多列列表中被选中行的背景
	m_mutiListMove = tex.mutiListMove;			//移动标题时的对齐标签
	m_mutiListTitle = tex.mutiListTitle;			//多列列表的标题背景
	m_mutiListTitleEnd = tex.mutiListTitleEnd;		//多列列表的标题分隔符
	m_withoutTex = XFalse;

	m_scale.set(1.0f,1.0f);
	if(!m_caption.setACopy(font)) XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);	 //设置字体上对齐
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);
	m_fontSize.set(strSize,strSize);
	m_caption.setScale(m_fontSize);
	m_curTextWidth = m_caption.getTextSize().x * m_caption.getScale().x * 0.5f;
	m_curTextHeight = m_caption.getTextSize().y * m_caption.getScale().y;

	m_spriteBackGround.init(m_mutiListNormal->texture.m_w,m_mutiListNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteBackGround);
#endif
	m_spriteBackGround.setPosition(m_position);
	m_spriteBackGround.setScale(m_scale);
	m_spriteBackGround.setIsTransformCenter(POINT_LEFT_TOP);

	m_spriteSelect.init(m_mutiListSelect->texture.m_w,m_mutiListSelect->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteSelect);
#endif
	m_spriteSelect.setScale(m_scale);
	m_spriteSelect.setIsTransformCenter(POINT_LEFT_TOP);

	m_spriteMove.init(m_mutiListMove->texture.m_w,m_mutiListMove->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteMove);
#endif
	m_spriteMove.setScale(m_scale);
	m_spriteMove.setIsTransformCenter(POINT_LEFT_TOP);

	m_haveSelect = XFalse;		//是否有选择
	m_selectLineOrder = -1;	//选择的是哪一行
	m_showStartLine = 0;	//显示的是从第几行开始的
	m_showStartRow = 0;		//显示从第几列开始的
	m_needShowMove = XFalse;		//是否需要显示移动对齐条

	m_needShowVSlider = XFalse;			//是否需要显示垂直滑动条
	m_verticalSlider.setACopy(vSlider);		//垂直滑动条
	XCtrlManager.decreaseAObject(&m_verticalSlider);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_verticalSlider);
#endif
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
		m_position.y + m_mouseRect.top * m_scale.y);
	m_verticalSlider.setScale(m_scale);
	m_verticalSlider.setEventProc(ctrlProc,this);
	m_verticalSlider.setWithAction(XFalse);

	m_needShowHSlider = XFalse;			//是否需要显示水平滑动条
	m_horizontalSlider.setACopy(hSlider);	//水平滑动条
	XCtrlManager.decreaseAObject(&m_horizontalSlider);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_horizontalSlider);
#endif
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_scale.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale.y);
	m_horizontalSlider.setScale(m_scale);
	m_horizontalSlider.setEventProc(ctrlProc,this);
	m_horizontalSlider.setWithAction(XFalse);

	if(rowSum > MUTILIST_MAX_ROW_SUM) return XFalse;
	m_tableRowSum = rowSum;		//表格中的列数
	//为列的链表分配内存空间
	if(m_tableRowSum == 0)
	{
		m_tableRow = NULL;
	}else
	{
		m_tableRow = XMem::createMem<XMultiListOneRow>();
		if(m_tableRow == NULL) 
		{
			releaseTempMemory();
			return XFalse;
		}		
		m_tableRow->isEnable = XTrue;

		m_tableRow->order = 0;
		m_tableRow->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableRow->text));
#endif
		m_tableRow->text.setScale(m_scale * m_fontSize);
		m_tableRow->pixSize.set(MUTILIST_MIN_WIDTH,m_curTextHeight);
		m_tableRow->position.set(m_position.x + (m_mouseRect.left + MUTILIST_MIN_WIDTH * 0) * m_scale.x,
			m_position.y + m_mouseRect.top * m_scale.y);
		m_tableRow->text.setPosition(m_tableRow->position.x,m_tableRow->position.y);
		m_tableRow->m_spriteTitle.init(m_mutiListTitle->texture.m_w,m_mutiListTitle->texture.m_h,1);	//设置标题背景的精灵
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableRow->m_spriteTitle));
#endif
		m_tableRow->m_spriteTitle.setPosition(m_tableRow->position);
		m_tableRow->m_spriteTitle.setScale(m_tableRow->pixSize.x * m_scale.x / m_mutiListTitle->textureSize.x,m_scale.y);
		m_tableRow->m_spriteTitle.setIsTransformCenter(POINT_LEFT_TOP);	//设置背景精灵的缩放为左上角对齐
		m_tableRow->m_spriteTitleEnd.init(m_mutiListTitleEnd->texture.m_w,m_mutiListTitleEnd->texture.m_h,1);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableRow->m_spriteTitleEnd));
#endif
		m_tableRow->m_spriteTitleEnd.setPosition(m_tableRow->position.x + m_tableRow->pixSize.x * m_scale.x - 
			m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
			m_tableRow->position.y);
		m_tableRow->m_spriteTitleEnd.setScale(m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
			m_tableRow->pixSize.y * m_scale.y/m_mutiListTitle->textureSize.y);
		m_tableRow->m_spriteTitleEnd.setIsTransformCenter(POINT_LEFT_TOP);

		m_tableRow->stringShowWidth = m_tableRow->pixSize.x * m_scale.x / m_curTextWidth;
		m_tableRow->title = "";
		setTitleStr("Title",0);	//设置默认显示
		m_tableRow->isShow = 1;

		m_tableRow->needChip = 0;					//是否需要切割
		m_tableRow->left = 0;						//左边的切割位置
		m_tableRow->right = 0;					//右边的切割位置

		XMultiListOneRow *tempRow = m_tableRow;
		for(int i = 1;i < m_tableRowSum;++ i)
		{
			tempRow->nextRow = XMem::createMem<XMultiListOneRow>();
			if(tempRow->nextRow == NULL)
			{
				releaseTempMemory();
				return XFalse;
			}
			tempRow->nextRow->isEnable = XTrue;
			tempRow->nextRow->order = i;
			tempRow->nextRow->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(tempRow->nextRow->text));
#endif
			tempRow->nextRow->text.setScale(m_scale * m_fontSize);
			tempRow->nextRow->pixSize.set(MUTILIST_MIN_WIDTH,m_curTextHeight);
			tempRow->nextRow->position.set(tempRow->position.x + tempRow->pixSize.x * m_scale.x,
				m_position.y + m_mouseRect.top * m_scale.y);
			tempRow->nextRow->text.setPosition(tempRow->nextRow->position.x,tempRow->nextRow->position.y);
			tempRow->nextRow->m_spriteTitle.init(m_mutiListTitle->texture.m_w,m_mutiListTitle->texture.m_h,1);	//设置标题背景的精灵
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(tempRow->nextRow->m_spriteTitle));
#endif
			tempRow->nextRow->m_spriteTitle.setPosition(tempRow->nextRow->position);
			tempRow->nextRow->m_spriteTitle.setScale(m_tableRow->pixSize.x * m_scale.x / m_mutiListTitle->textureSize.x,m_scale.y);
			tempRow->nextRow->m_spriteTitle.setIsTransformCenter(POINT_LEFT_TOP);	//设置背景精灵的缩放为左上角对齐
			tempRow->nextRow->m_spriteTitleEnd.init(m_mutiListTitleEnd->texture.m_w,m_mutiListTitleEnd->texture.m_h,1);
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(tempRow->nextRow->m_spriteTitleEnd));
#endif
			tempRow->nextRow->m_spriteTitleEnd.setPosition(tempRow->nextRow->position.x + m_tableRow->pixSize.x * m_scale.x - 
				m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
				m_tableRow->position.y);
			tempRow->nextRow->m_spriteTitleEnd.setScale(m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
				m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y);
			tempRow->nextRow->m_spriteTitleEnd.setIsTransformCenter(POINT_LEFT_TOP);

			tempRow->nextRow->stringShowWidth = m_tableRow->pixSize.x * m_scale.x / m_curTextWidth;
			tempRow->nextRow->title = "";
			setTitleStr("Title",i);	//设置默认显示
			tempRow->nextRow->isShow = 1;
			tempRow->needChip = 0;					//是否需要切割
			tempRow->left = 0;						//左边的切割位置
			tempRow->right = 0;					//右边的切割位置

			tempRow = tempRow->nextRow;
			//注意这里没有检查内存分配失败
		}
		tempRow->nextRow = NULL;	//最后一个结尾
	}
	m_tableLineSum = lineSum;		//表格中的行数
	if(m_tableLineSum > 0)
	{
		m_haveSelectFlag = XMem::createArrayMem<XBool>(m_tableLineSum);
		m_check0State = XMem::createArrayMem<XBool>(m_tableLineSum);
		m_check1State = XMem::createArrayMem<XBool>(m_tableLineSum);
		if(m_haveSelectFlag == NULL || m_check0State == NULL || m_check1State == NULL) 
		{
			releaseTempMemory();
			return XFalse;
		}
		for(int i = 0;i < m_tableLineSum;++ i)
		{
			m_haveSelectFlag[i] = XFalse;
			m_check0State[i] = XFalse;
			m_check1State[i] = XFalse;
		}
	}else
	{
		m_haveSelectFlag = NULL;
		m_check0State = NULL;
		m_check1State = NULL;
	}
	m_stateOfShiftKey = KEY_STATE_UP;
	//计算控件中能显示的行数
	m_showPixWidth = (m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_scale.x;
	m_showPixHight = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight() - m_mutiListTitle->textureSize.y) * m_scale.y;
	m_canShowLineSum = m_showPixHight / m_curTextHeight;
	m_curMouseRect.set(m_position.x + m_mouseRect.left * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y,
		m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale.y);	//当前的鼠标响应范围
	m_spriteSelect.setScale((float)m_showPixWidth / m_mutiListSelect->textureSize.x,m_curTextHeight / m_mutiListSelect->textureSize.y);
	m_check0 = XMem::createArrayMem<XCheck>(m_canShowLineSum);
	m_check1 = XMem::createArrayMem<XCheck>(m_canShowLineSum);
	if(m_check0 == NULL || m_check1 == NULL) 
	{
		releaseTempMemory();
		return XFalse;
	}
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		if(!m_check0[i].init(XVector2(m_position.x + (m_mouseRect.left - DEFAULT_CHECK_SIZE * 2.0f) * m_scale.x,
			m_position.y + ((m_mouseRect.left + DEFAULT_TITLE_HEIGHT) * m_scale.y + m_curTextHeight * i)),XRect(4.0f,4.0f,28.0f,28.0f),checktex0,
			" ",m_caption,1.0f,XVector2::zero)) 
		{
			releaseTempMemory();
			return XFalse;
		}
		m_check0[i].setScale(m_scale);
		m_check0[i].setWithCaption(XFalse);
		if(!m_check1[i].init(XVector2(m_position.x + (m_mouseRect.left - DEFAULT_CHECK_SIZE) * m_scale.x,
			m_position.y + ((m_mouseRect.left + DEFAULT_TITLE_HEIGHT) * m_scale.y + m_curTextHeight * i)),XRect(4.0f,4.0f,28.0f,28.0f),checktex1,
			" ",m_caption,1.0f,XVector2::zero)) 
		{
			releaseTempMemory();
			return XFalse;
		}
		m_check1[i].setScale(m_scale);
		m_check1[i].setWithCaption(XFalse);
		if(m_tableLineSum < m_canShowLineSum && i >= m_tableLineSum)
		{
			m_check0[i].disVisible();
			m_check1[i].disVisible();
		}else
		{
			m_check0[i].setVisible();
			m_check1[i].setVisible();
		}
		XCtrlManager.decreaseAObject(&(m_check0[i]));
		XCtrlManager.decreaseAObject(&(m_check1[i]));
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&m_check0[i]);
		XObjManager.decreaseAObject(&m_check1[i]);
#endif

		m_check0[i].setEventProc(ctrlProc,this);
		m_check1[i].setEventProc(ctrlProc,this);
	}
	//为元素分配内存空间
	if(m_tableLineSum == 0)
	{
		m_tableBox = NULL;
	}else
	{
		m_tableBox = XMem::createMem<XMultiListOneBox>();
		if(m_tableBox == NULL) 
		{
			releaseTempMemory();
			return XFalse;
		}
		m_tableBox->isEnable = XTrue;
		m_tableBox->isShow = XTrue;
		m_tableBox->order.set(0,0);
		m_tableBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableBox->text));
#endif
		m_tableBox->text.setScale(m_scale * m_fontSize);
		m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_curTextHeight * 1);
		m_tableBox->textStr = "";
		setBoxStr("Line",0,0);

		XMultiListOneBox *tempBox = m_tableBox;
		int i,j;
		XMultiListOneRow *tempRow;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(j == 0 && i == 0) 
				{
					tempRow = tempRow->nextRow;
					//tempBox = tempBox->nextBox;
					continue;
				}
				tempBox->nextBox = XMem::createMem<XMultiListOneBox>();
				if(tempBox->nextBox == NULL) 
				{
					releaseTempMemory();
					return XFalse;
				}
				tempBox = tempBox->nextBox;
				tempBox->isEnable = XTrue;
				
				tempBox->isShow = XTrue;
				tempBox->order.set(j,i);
				tempBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
				XObjManager.decreaseAObject(&(tempBox->text));
#endif
				tempBox->text.setScale(m_scale * m_fontSize);
				tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
				tempBox->textStr = "";
				setBoxStr("Line",i,j);
				
				tempRow = tempRow->nextRow;
				//注意这里没有检查内存分配失败
			}
		}
		tempBox->nextBox = NULL;	//最后一个结尾
	}
	m_mouseLeftButtonDown = XFalse;
	m_mouseMoveDown = XFalse;

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	updateSliderState();
	updateShowChange();
	return XTrue;
}
XBool XMultiListBasic::initPlus(const char *path,		//单选框的贴图
		const XFontUnicode &font,			//控件中使用的字体
		float strSize,						//字体的缩放大小
		int rowSum,					//控件中的列数
		int lineSum,				//控件中的行数
		//const XMouseRightButtonMenu& mouseMenu,	//控件中使用的右键菜单(目前无效)
		XResourcePosition resoursePosition)
{
	if(m_isInited || path == NULL) return XFalse;
	m_resInfo = XResManager.loadResource(path,RESOURCE_TYPEXMULTILIST_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	XMultiListSkin * tex = (XMultiListSkin *)m_resInfo->m_pointer;
	//下面是参数检查
	if(tex->m_mouseRect.getWidth() <= 0 || tex->m_mouseRect.getHeight() <= 0) return XFalse;	//输入范围不能为空
	if(tex->mutiListDisable == NULL || tex->mutiListMove == NULL || tex->mutiListNormal == NULL
		|| tex->mutiListSelect == NULL || tex->mutiListTitle == NULL || tex->mutiListTitleEnd == NULL)
	{//需要的贴图不完整
		return XFalse;
	}
	if(rowSum < 0 || lineSum < 0 || (rowSum == 0 && lineSum > 0)) return XFalse;//不能为负数
	//下面开始赋值
	m_position.set(0.0f,0.0f);
	m_mouseRect = tex->m_mouseRect;

	m_mutiListNormal = tex->mutiListNormal;			//多列列表的普通状态
	m_mutiListDisable = tex->mutiListDisable;			//多列列表的无效状态
	m_mutiListSelect = tex->mutiListSelect;			//多列列表中被选中行的背景
	m_mutiListMove = tex->mutiListMove;			//移动标题时的对齐标签
	m_mutiListTitle = tex->mutiListTitle;			//多列列表的标题背景
	m_mutiListTitleEnd = tex->mutiListTitleEnd;		//多列列表的标题分隔符
	m_withoutTex = XFalse;

	m_scale.set(1.0f,1.0f);
	if(!m_caption.setACopy(font)) XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);	 //设置字体上对齐
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);
	m_fontSize.set(strSize,strSize);
	m_caption.setScale(m_fontSize);
	m_curTextWidth = m_caption.getTextSize().x * m_caption.getScale().x * 0.5f;
	m_curTextHeight = m_caption.getTextSize().y * m_caption.getScale().y;

	m_spriteBackGround.init(m_mutiListNormal->texture.m_w,m_mutiListNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteBackGround);
#endif
	m_spriteBackGround.setPosition(m_position);
	m_spriteBackGround.setScale(m_scale);
	m_spriteBackGround.setIsTransformCenter(POINT_LEFT_TOP);

	m_spriteSelect.init(m_mutiListSelect->texture.m_w,m_mutiListSelect->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteSelect);
#endif
	m_spriteSelect.setScale(m_scale);
	m_spriteSelect.setIsTransformCenter(POINT_LEFT_TOP);

	m_spriteMove.init(m_mutiListMove->texture.m_w,m_mutiListMove->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteMove);
#endif
	m_spriteMove.setScale(m_scale);
	m_spriteMove.setIsTransformCenter(POINT_LEFT_TOP);

	m_haveSelect = XFalse;		//是否有选择
	m_selectLineOrder = -1;	//选择的是哪一行
	m_showStartLine = 0;	//显示的是从第几行开始的
	m_showStartRow = 0;		//显示从第几列开始的
	m_needShowMove = XFalse;		//是否需要显示移动对齐条

	m_needShowVSlider = XFalse;			//是否需要显示垂直滑动条
	//m_verticalSlider.setACopy(vSlider);		//垂直滑动条
	char tempPath[MAX_FILE_NAME_LENGTH];
	sprintf(tempPath,"%s/SliderV",path);
	m_verticalSlider.initPlus(tempPath,100.0f,0.0f,SLIDER_TYPE_VERTICAL,resoursePosition);
	XCtrlManager.decreaseAObject(&m_verticalSlider);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_verticalSlider);
#endif
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
		m_position.y + m_mouseRect.top * m_scale.y);
	m_verticalSlider.setScale(m_scale);
	m_verticalSlider.setEventProc(ctrlProc,this);
	m_verticalSlider.setWithAction(XFalse);

	m_needShowHSlider = XFalse;			//是否需要显示水平滑动条
	//m_horizontalSlider.setACopy(hSlider);	//水平滑动条
	sprintf(tempPath,"%s/SliderH",path);
	m_horizontalSlider.initPlus(tempPath,100.0f,0.0f,SLIDER_TYPE_HORIZONTAL,resoursePosition);
	XCtrlManager.decreaseAObject(&m_horizontalSlider);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_horizontalSlider);
#endif
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_scale.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale.y);
	m_horizontalSlider.setScale(m_scale);
	m_horizontalSlider.setEventProc(ctrlProc,this);
	m_horizontalSlider.setWithAction(XFalse);

	if(rowSum > MUTILIST_MAX_ROW_SUM) return XFalse;
	m_tableRowSum = rowSum;		//表格中的列数
	//为列的链表分配内存空间
	if(m_tableRowSum == 0)
	{
		m_tableRow = NULL;
	}else
	{
		m_tableRow = XMem::createMem<XMultiListOneRow>();
		if(m_tableRow == NULL) 
		{
			releaseTempMemory();
			return XFalse;
		}		
		m_tableRow->isEnable = XTrue;

		m_tableRow->order = 0;
		m_tableRow->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableRow->text));
#endif
		m_tableRow->text.setScale(m_scale * m_fontSize);
		m_tableRow->pixSize.set(MUTILIST_MIN_WIDTH,m_curTextHeight);
		m_tableRow->position.set(m_position.x + (m_mouseRect.left + MUTILIST_MIN_WIDTH * 0) * m_scale.x,
			m_position.y + m_mouseRect.top * m_scale.y);
		m_tableRow->text.setPosition(m_tableRow->position.x,m_tableRow->position.y);
		m_tableRow->m_spriteTitle.init(m_mutiListTitle->texture.m_w,m_mutiListTitle->texture.m_h,1);	//设置标题背景的精灵
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableRow->m_spriteTitle));
#endif
		m_tableRow->m_spriteTitle.setPosition(m_tableRow->position);
		m_tableRow->m_spriteTitle.setScale(m_tableRow->pixSize.x * m_scale.x / m_mutiListTitle->textureSize.x,m_scale.y);
		m_tableRow->m_spriteTitle.setIsTransformCenter(POINT_LEFT_TOP);	//设置背景精灵的缩放为左上角对齐
		m_tableRow->m_spriteTitleEnd.init(m_mutiListTitleEnd->texture.m_w,m_mutiListTitleEnd->texture.m_h,1);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableRow->m_spriteTitleEnd));
#endif
		m_tableRow->m_spriteTitleEnd.setPosition(m_tableRow->position.x + m_tableRow->pixSize.x * m_scale.x - 
			m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
			m_tableRow->position.y);
		m_tableRow->m_spriteTitleEnd.setScale(m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
			m_tableRow->pixSize.y * m_scale.y/m_mutiListTitle->textureSize.y);
		m_tableRow->m_spriteTitleEnd.setIsTransformCenter(POINT_LEFT_TOP);

		m_tableRow->stringShowWidth = m_tableRow->pixSize.x * m_scale.x / m_curTextWidth;
		m_tableRow->title = "";
		setTitleStr("Title",0);	//设置默认显示
		m_tableRow->isShow = 1;

		m_tableRow->needChip = 0;					//是否需要切割
		m_tableRow->left = 0;						//左边的切割位置
		m_tableRow->right = 0;					//右边的切割位置

		XMultiListOneRow *tempRow = m_tableRow;
		for(int i = 1;i < m_tableRowSum;++ i)
		{
			tempRow->nextRow = XMem::createMem<XMultiListOneRow>();
			if(tempRow->nextRow == NULL)
			{
				releaseTempMemory();
				return XFalse;
			}
			tempRow->nextRow->isEnable = XTrue;
			tempRow->nextRow->order = i;
			tempRow->nextRow->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(tempRow->nextRow->text));
#endif
			tempRow->nextRow->text.setScale(m_scale * m_fontSize);
			tempRow->nextRow->pixSize.set(MUTILIST_MIN_WIDTH,m_curTextHeight);
			tempRow->nextRow->position.set(tempRow->position.x + tempRow->pixSize.x * m_scale.x,
				m_position.y + m_mouseRect.top * m_scale.y);
			tempRow->nextRow->text.setPosition(tempRow->nextRow->position.x,tempRow->nextRow->position.y);
			tempRow->nextRow->m_spriteTitle.init(m_mutiListTitle->texture.m_w,m_mutiListTitle->texture.m_h,1);	//设置标题背景的精灵
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(tempRow->nextRow->m_spriteTitle));
#endif
			tempRow->nextRow->m_spriteTitle.setPosition(tempRow->nextRow->position);
			tempRow->nextRow->m_spriteTitle.setScale(m_tableRow->pixSize.x * m_scale.x / m_mutiListTitle->textureSize.x,m_scale.y);
			tempRow->nextRow->m_spriteTitle.setIsTransformCenter(POINT_LEFT_TOP);	//设置背景精灵的缩放为左上角对齐
			tempRow->nextRow->m_spriteTitleEnd.init(m_mutiListTitleEnd->texture.m_w,m_mutiListTitleEnd->texture.m_h,1);
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(tempRow->nextRow->m_spriteTitleEnd));
#endif
			tempRow->nextRow->m_spriteTitleEnd.setPosition(tempRow->nextRow->position.x + m_tableRow->pixSize.x * m_scale.x - 
				m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
				m_tableRow->position.y);
			tempRow->nextRow->m_spriteTitleEnd.setScale(m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
				m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y);
			tempRow->nextRow->m_spriteTitleEnd.setIsTransformCenter(POINT_LEFT_TOP);

			tempRow->nextRow->stringShowWidth = m_tableRow->pixSize.x * m_scale.x / m_curTextWidth;
			tempRow->nextRow->title = "";
			setTitleStr("Title",i);	//设置默认显示
			tempRow->nextRow->isShow = 1;
			tempRow->needChip = 0;					//是否需要切割
			tempRow->left = 0;						//左边的切割位置
			tempRow->right = 0;					//右边的切割位置

			tempRow = tempRow->nextRow;
			//注意这里没有检查内存分配失败
		}
		tempRow->nextRow = NULL;	//最后一个结尾
	}
	m_tableLineSum = lineSum;		//表格中的行数
	if(m_tableLineSum > 0)
	{
		m_haveSelectFlag = XMem::createArrayMem<XBool>(m_tableLineSum);
		m_check0State = XMem::createArrayMem<XBool>(m_tableLineSum);
		m_check1State = XMem::createArrayMem<XBool>(m_tableLineSum);
		if(m_haveSelectFlag == NULL || m_check0State == NULL || m_check1State == NULL) 
		{
			releaseTempMemory();
			return XFalse;
		}
		for(int i = 0;i < m_tableLineSum;++ i)
		{
			m_haveSelectFlag[i] = XFalse;
			m_check0State[i] = XFalse;
			m_check1State[i] = XFalse;
		}
	}else
	{
		m_haveSelectFlag = NULL;
		m_check0State = NULL;
		m_check1State = NULL;
	}
	m_stateOfShiftKey = KEY_STATE_UP;
	//计算控件中能显示的行数
	m_showPixWidth = (m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_scale.x;
	m_showPixHight = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight() - m_mutiListTitle->textureSize.y) * m_scale.y;
	m_canShowLineSum = m_showPixHight / m_curTextHeight;
	m_curMouseRect.set(m_position.x + m_mouseRect.left * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y,
		m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale.y);	//当前的鼠标响应范围
	m_spriteSelect.setScale((float)m_showPixWidth / m_mutiListSelect->textureSize.x,m_curTextHeight / m_mutiListSelect->textureSize.y);
	m_check0 = XMem::createArrayMem<XCheck>(m_canShowLineSum);
	m_check1 = XMem::createArrayMem<XCheck>(m_canShowLineSum);
	if(m_check0 == NULL || m_check1 == NULL) 
	{
		releaseTempMemory();
		return XFalse;
	}
	char tempPath0[MAX_FILE_NAME_LENGTH];
	char tempPath1[MAX_FILE_NAME_LENGTH];
	sprintf(tempPath0,"%s/checkC",path);
	sprintf(tempPath1,"%s/checkE",path);
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
	//	if(!m_check0[i].init(XVector2(m_position.x + 4.0f * m_scale.x,m_position.y + (36.0f * m_scale.y + m_curTextHeight * i)),XRect(4.0f,4.0f,28.0f,28.0f),checktex0,
	//		" ",m_caption,1.0f,XVector2::zero)) 
		if(!m_check0[i].initPlus(tempPath0," ",m_caption,1.0f,resoursePosition)) 
		{
			releaseTempMemory();
			return XFalse;
		}
		m_check0[i].setPosition(m_position.x + (m_mouseRect.left - DEFAULT_CHECK_SIZE * 2.0f) * m_scale.x,
			m_position.y + ((m_mouseRect.left + DEFAULT_TITLE_HEIGHT) * m_scale.y + m_curTextHeight * i));
		m_check0[i].setScale(m_scale);
		m_check0[i].setWithCaption(XFalse);
	//	if(!m_check1[i].init(XVector2(m_position.x + 4.0f * m_scale.x + 32.0f * m_scale.x,m_position.y + (36.0f * m_scale.y + m_curTextHeight * i)),XRect(4.0f,4.0f,28.0f,28.0f),checktex1,
	//		" ",m_caption,1.0f,XVector2::zero)) 
		if(!m_check1[i].initPlus(tempPath1," ",m_caption,1.0f,resoursePosition)) 
		{
			releaseTempMemory();
			return XFalse;
		}
		m_check1[i].setPosition(m_position.x + (m_mouseRect.left - DEFAULT_CHECK_SIZE) * m_scale.x,
			m_position.y + ((m_mouseRect.left + DEFAULT_TITLE_HEIGHT) * m_scale.y + m_curTextHeight * i));
		m_check1[i].setScale(m_scale);
		m_check1[i].setWithCaption(XFalse);
		if(m_tableLineSum < m_canShowLineSum && i >= m_tableLineSum)
		{
			m_check0[i].disVisible();
			m_check1[i].disVisible();
		}else
		{
			m_check0[i].setVisible();
			m_check1[i].setVisible();
		}
		XCtrlManager.decreaseAObject(&(m_check0[i]));
		XCtrlManager.decreaseAObject(&(m_check1[i]));
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&m_check0[i]);
		XObjManager.decreaseAObject(&m_check1[i]);
#endif

		m_check0[i].setEventProc(ctrlProc,this);
		m_check1[i].setEventProc(ctrlProc,this);
	}
	//为元素分配内存空间
	if(m_tableLineSum == 0)
	{
		m_tableBox = NULL;
	}else
	{
		m_tableBox = XMem::createMem<XMultiListOneBox>();
		if(m_tableBox == NULL) 
		{
			releaseTempMemory();
			return XFalse;
		}
		m_tableBox->isEnable = XTrue;
		m_tableBox->isShow = XTrue;
		m_tableBox->order.set(0,0);
		m_tableBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableBox->text));
#endif
		m_tableBox->text.setScale(m_scale * m_fontSize);
		m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_curTextHeight * 1);
		m_tableBox->textStr = "";
		setBoxStr("Line",0,0);

		XMultiListOneBox *tempBox = m_tableBox;
		int i,j;
		XMultiListOneRow *tempRow;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(j == 0 && i == 0) 
				{
					tempRow = tempRow->nextRow;
					//tempBox = tempBox->nextBox;
					continue;
				}
				tempBox->nextBox = XMem::createMem<XMultiListOneBox>();
				if(tempBox->nextBox == NULL) 
				{
					releaseTempMemory();
					return XFalse;
				}
				tempBox = tempBox->nextBox;
				tempBox->isEnable = XTrue;
				
				tempBox->isShow = XTrue;
				tempBox->order.set(j,i);
				tempBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
				XObjManager.decreaseAObject(&(tempBox->text));
#endif
				tempBox->text.setScale(m_scale * m_fontSize);
				tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
				tempBox->textStr = "";
				setBoxStr("Line",i,j);
				
				tempRow = tempRow->nextRow;
				//注意这里没有检查内存分配失败
			}
		}
		tempBox->nextBox = NULL;	//最后一个结尾
	}
	m_mouseLeftButtonDown = XFalse;
	m_mouseMoveDown = XFalse;

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	updateSliderState();
	updateShowChange();
	return XTrue;
}
XBool XMultiListBasic::initWithoutSkin(const XRect& area,
		const XFontUnicode &font,			//控件中使用的字体
		float strSize,						//字体的缩放大小
		int rowSum,					//控件中的列数
		int lineSum)
{
	if(m_isInited) return XFalse;	//重复初始化
	if(rowSum < 0 || lineSum < 0 || (rowSum == 0 && lineSum > 0)) return XFalse;//不能为负数
	//下面开始赋值
	m_position.set(0.0f,0.0f);
	m_mouseRect = area;

	m_withoutTex = XTrue;

	m_scale.set(1.0f,1.0f);
	if(!m_caption.setACopy(font)) XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);	 //设置字体上对齐
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);
	m_fontSize.set(strSize,strSize);
	m_caption.setScale(m_fontSize);
	m_curTextWidth = m_caption.getTextSize().x * m_caption.getScale().x * 0.5f;
	m_curTextHeight = m_caption.getTextSize().y * m_caption.getScale().y;

	m_haveSelect = XFalse;		//是否有选择
	m_selectLineOrder = -1;	//选择的是哪一行
	m_showStartLine = 0;	//显示的是从第几行开始的
	m_showStartRow = 0;		//显示从第几列开始的
	m_needShowMove = XFalse;		//是否需要显示移动对齐条

	m_needShowVSlider = XFalse;			//是否需要显示垂直滑动条
//	m_verticalSlider.initWithoutSkin(XRect(0,0,DEFAULT_SLIDER_WIDTH,m_mouseRect.getHeight() - DEFAULT_SLIDER_WIDTH),XRect(0,0,DEFAULT_SLIDER_WIDTH,DEFAULT_SLIDER_WIDTH),
//		SLIDER_TYPE_VERTICAL,100.0f,0.0f,XVector2::zero);
	m_verticalSlider.initWithoutSkin(XRect(0,0,DEFAULT_SLIDER_WIDTH,m_mouseRect.getHeight() - DEFAULT_SLIDER_WIDTH),100.0f,0.0f,SLIDER_TYPE_VERTICAL);
	XCtrlManager.decreaseAObject(&m_verticalSlider);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_verticalSlider);
#endif
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
		m_position.y + m_mouseRect.top * m_scale.y);
	m_verticalSlider.setScale(m_scale);
	m_verticalSlider.setEventProc(ctrlProc,this);
	m_verticalSlider.setWithAction(XFalse);

	m_needShowHSlider = XFalse;			//是否需要显示水平滑动条
//	m_horizontalSlider.initWithoutSkin(XRect(0,0,m_mouseRect.getWidth() - DEFAULT_SLIDER_WIDTH,DEFAULT_SLIDER_WIDTH),XRect(0,0,DEFAULT_SLIDER_WIDTH,DEFAULT_SLIDER_WIDTH),
//		SLIDER_TYPE_HORIZONTAL,100.0f,0.0f,XVector2::zero);
	m_horizontalSlider.initWithoutSkin(XRect(0,0,m_mouseRect.getWidth() - DEFAULT_SLIDER_WIDTH,DEFAULT_SLIDER_WIDTH));
	XCtrlManager.decreaseAObject(&m_horizontalSlider);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_horizontalSlider);
#endif
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_scale.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale.y);
	m_horizontalSlider.setScale(m_scale);
	m_horizontalSlider.setEventProc(ctrlProc,this);
	m_horizontalSlider.setWithAction(XFalse);

	if(rowSum > MUTILIST_MAX_ROW_SUM) return XFalse;
	m_tableRowSum = rowSum;		//表格中的列数
	//为列的链表分配内存空间
	if(m_tableRowSum == 0)
	{
		m_tableRow = NULL;
	}else
	{
		m_tableRow = XMem::createMem<XMultiListOneRow>();
		if(m_tableRow == NULL) 
		{
			releaseTempMemory();
			return XFalse;
		}		
		m_tableRow->isEnable = XTrue;

		m_tableRow->order = 0;
		m_tableRow->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableRow->text));
#endif
		m_tableRow->text.setScale(m_scale * m_fontSize);
		m_tableRow->pixSize.set(MUTILIST_MIN_WIDTH,m_curTextHeight);
		m_tableRow->position.set(m_position.x + (m_mouseRect.left + MUTILIST_MIN_WIDTH * 0) * m_scale.x,
			m_position.y + m_mouseRect.top * m_scale.y);
		m_tableRow->text.setPosition(m_tableRow->position.x,m_tableRow->position.y);

		m_tableRow->stringShowWidth = m_tableRow->pixSize.x * m_scale.x / m_curTextWidth;
		m_tableRow->title = "";
		setTitleStr("Title",0);	//设置默认显示
		m_tableRow->isShow = 1;

		m_tableRow->needChip = 0;					//是否需要切割
		m_tableRow->left = 0;						//左边的切割位置
		m_tableRow->right = 0;					//右边的切割位置

		XMultiListOneRow *tempRow = m_tableRow;
		for(int i = 1;i < m_tableRowSum;++ i)
		{
			tempRow->nextRow = XMem::createMem<XMultiListOneRow>();
			if(tempRow->nextRow == NULL)
			{
				releaseTempMemory();
				return XFalse;
			}
			tempRow->nextRow->isEnable = XTrue;
			tempRow->nextRow->order = i;
			tempRow->nextRow->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(tempRow->nextRow->text));
#endif
			tempRow->nextRow->text.setScale(m_scale * m_fontSize);
			tempRow->nextRow->pixSize.set(MUTILIST_MIN_WIDTH,m_curTextHeight);
			tempRow->nextRow->position.set(tempRow->position.x + tempRow->pixSize.x * m_scale.x,
				m_position.y + m_mouseRect.top * m_scale.y);
			tempRow->nextRow->text.setPosition(tempRow->nextRow->position.x,tempRow->nextRow->position.y);

			tempRow->nextRow->stringShowWidth = m_tableRow->pixSize.x * m_scale.x / m_curTextWidth;
			tempRow->nextRow->title = "";
			setTitleStr("Title",i);	//设置默认显示
			tempRow->nextRow->isShow = 1;
			tempRow->needChip = 0;					//是否需要切割
			tempRow->left = 0;						//左边的切割位置
			tempRow->right = 0;					//右边的切割位置

			tempRow = tempRow->nextRow;
			//注意这里没有检查内存分配失败
		}
		tempRow->nextRow = NULL;	//最后一个结尾
	}
	m_tableLineSum = lineSum;		//表格中的行数
	if(m_tableLineSum > 0)
	{
		m_haveSelectFlag = XMem::createArrayMem<XBool>(m_tableLineSum);
		m_check0State = XMem::createArrayMem<XBool>(m_tableLineSum);
		m_check1State = XMem::createArrayMem<XBool>(m_tableLineSum);
		if(m_haveSelectFlag == NULL || m_check0State == NULL || m_check1State == NULL) 
		{
			releaseTempMemory();
			return XFalse;
		}
		for(int i = 0;i < m_tableLineSum;++ i)
		{
			m_haveSelectFlag[i] = XFalse;
			m_check0State[i] = XFalse;
			m_check1State[i] = XFalse;
		}
	}else
	{
		m_haveSelectFlag = NULL;
		m_check0State = NULL;
		m_check1State = NULL;
	}
	m_stateOfShiftKey = KEY_STATE_UP;
	//计算控件中能显示的行数
	m_showPixWidth = (m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_scale.x;
//	m_showPixHight = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight() - m_mutiListTitle->textureSize.y) * m_scale.y;
	m_showPixHight = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight() - DEFAULT_TITLE_HEIGHT) * m_scale.y;
	m_canShowLineSum = m_showPixHight / m_curTextHeight;
	m_curMouseRect.set(m_position.x + m_mouseRect.left * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y,
		m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale.y);	//当前的鼠标响应范围
//	m_spriteSelect.setScale((float)m_showPixWidth / m_mutiListSelect->textureSize.x,m_curTextHeight / m_mutiListSelect->textureSize.y);
	m_check0 = XMem::createArrayMem<XCheck>(m_canShowLineSum);
	m_check1 = XMem::createArrayMem<XCheck>(m_canShowLineSum);
	if(m_check0 == NULL || m_check1 == NULL) 
	{
		releaseTempMemory();
		return XFalse;
	}
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		if(!m_check0[i].initWithoutSkin(" ",m_caption,1.0f,XRect(0,0,DEFAULT_CHECK_SIZE,DEFAULT_CHECK_SIZE),XVector2(DEFAULT_CHECK_SIZE,DEFAULT_CHECK_SIZE))) 
		{
			releaseTempMemory();
			return XFalse;
		}
		m_check0[i].setPosition(m_position.x + (m_mouseRect.left - DEFAULT_CHECK_SIZE * 2.0f) * m_scale.x,
			m_position.y + ((DEFAULT_TITLE_HEIGHT + m_mouseRect.top) * m_scale.y + m_curTextHeight * i));
		m_check0[i].setScale(m_scale);
		m_check0[i].setWithCaption(XFalse);
		if(!m_check1[i].initWithoutSkin(" ",m_caption,1.0f,XRect(0,0,DEFAULT_CHECK_SIZE,DEFAULT_CHECK_SIZE),XVector2(DEFAULT_CHECK_SIZE,DEFAULT_CHECK_SIZE))) 
		{
			releaseTempMemory();
			return XFalse;
		}
		m_check1[i].setPosition(m_position.x + (m_mouseRect.left - DEFAULT_CHECK_SIZE) * m_scale.x,
			m_position.y + ((DEFAULT_TITLE_HEIGHT + m_mouseRect.top) * m_scale.y + m_curTextHeight * i));
		m_check1[i].setScale(m_scale);
		m_check1[i].setWithCaption(XFalse);
		if(m_tableLineSum < m_canShowLineSum && i >= m_tableLineSum)
		{
			m_check0[i].disVisible();
			m_check1[i].disVisible();
		}else
		{
			m_check0[i].setVisible();
			m_check1[i].setVisible();
		}
		XCtrlManager.decreaseAObject(&(m_check0[i]));
		XCtrlManager.decreaseAObject(&(m_check1[i]));
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&m_check0[i]);
		XObjManager.decreaseAObject(&m_check1[i]);
#endif

		m_check0[i].setEventProc(ctrlProc,this);
		m_check1[i].setEventProc(ctrlProc,this);
	}
	//为元素分配内存空间
	if(m_tableLineSum == 0)
	{
		m_tableBox = NULL;
	}else
	{
		m_tableBox = XMem::createMem<XMultiListOneBox>();
		if(m_tableBox == NULL) 
		{
			releaseTempMemory();
			return XFalse;
		}
		m_tableBox->isEnable = XTrue;
		m_tableBox->isShow = XTrue;
		m_tableBox->order.set(0,0);
		m_tableBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableBox->text));
#endif
		m_tableBox->text.setScale(m_scale * m_fontSize);
		m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_curTextHeight * 1);
		m_tableBox->textStr = "";
		setBoxStr("Line",0,0);

		XMultiListOneBox *tempBox = m_tableBox;
		int i,j;
		XMultiListOneRow *tempRow;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(j == 0 && i == 0) 
				{
					tempRow = tempRow->nextRow;
					//tempBox = tempBox->nextBox;
					continue;
				}
				tempBox->nextBox = XMem::createMem<XMultiListOneBox>();
				if(tempBox->nextBox == NULL) 
				{
					releaseTempMemory();
					return XFalse;
				}
				tempBox = tempBox->nextBox;
				tempBox->isEnable = XTrue;
				
				tempBox->isShow = XTrue;
				tempBox->order.set(j,i);
				tempBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
				XObjManager.decreaseAObject(&(tempBox->text));
#endif
				tempBox->text.setScale(m_scale * m_fontSize);
				tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
				tempBox->textStr = "";
				setBoxStr("Line",i,j);
				
				tempRow = tempRow->nextRow;
				//注意这里没有检查内存分配失败
			}
		}
		tempBox->nextBox = NULL;	//最后一个结尾
	}
	m_mouseLeftButtonDown = XFalse;
	m_mouseMoveDown = XFalse;

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	updateSliderState();
	updateShowChange();
	return XTrue;
}
void XMultiListBasic::updateSliderState()				//根据表格的情况，跟新滑动条的状态
{
	if(!m_isInited) return;		//如果没有初始化直接退出
	//垂直滑动条
	if(m_canShowLineSum < m_tableLineSum)
	{//如果垂直超过显示范围，则需要启动垂直显示条，并设置选择范围
		m_needShowVSlider = XTrue;			
		m_verticalSlider.setRange(m_tableLineSum - m_canShowLineSum,0.0f);		//垂直滑动条
	}else
	{
		m_showStartLine = 0;
		m_needShowVSlider = XFalse;			
		m_verticalSlider.setRange(m_tableLineSum - m_canShowLineSum,0.0f);		//垂直滑动条
	}
	//水平滑动条
	if(m_tableRowSum <= 0)
	{//如果没有列则不需要显示水平滑动条
		m_needShowHSlider = XFalse;
	}else
	{
		int width = 0;
		float rowWidth[MUTILIST_MAX_ROW_SUM];
		memset(rowWidth,0,MUTILIST_MAX_ROW_SUM * sizeof(float));
		//方案1:只要多余1列则显示水平滑动条
	/*	if(m_tableRowSum > 1)
		{
			m_needShowHSlider = 1;
			m_horizontalSlider.setRange(m_tableRowSum - 1,0);
		}*/
	/*	//方案2:需要超出显示范围才显示水平滑动条
		XMultiListOneRow *tempRow = m_tableRow;
		int i = 0;
		while(true)
		{
			rowWidth[i] = tempRow->pixSize.x * m_scale.x;
			if(tempRow->nextRow == NULL)
			{
				break;
			}else
			{
				tempRow = tempRow->nextRow;
			}
			++ i
		}
		int flag = 0;
		for(i = 0;i < m_tableRowSum;++ i)
		{
			width += rowWidth[m_tableRowSum - i -1];
			if(width > m_showPixWidth && i < m_tableRowSum)
			{
				flag = 1;
				break;
			}
		}
		if(flag == 1)
		{//超出水平显示宽度则需要显示水平滑动条
			m_needShowHSlider = 1;
			m_horizontalSlider.setRange(m_tableRowSum - i,0);
		//	m_horizontalSlider.setRange(m_tableRowSum - 1,0);
		}else
		{
			if(m_needShowHSlider)
			{//从越界到不越界，需要重新初始化偏移量
				m_rowDx = 0;
			}
			m_needShowHSlider = 0;
			m_showStartRow = 0;
			m_horizontalSlider.setCurValue(0);
		}*/
		//方案2:需要超出显示范围才显示水平滑动条
		XMultiListOneRow *tempRow = m_tableRow;
		int i = 0;
		while(true)
		{
			rowWidth[i] = tempRow->pixSize.x * m_scale.x;
			if(tempRow->nextRow == NULL) break;
			else tempRow = tempRow->nextRow;
			++ i;
		}
		int flag = 0;
		for(i = 0;i < m_tableRowSum;++ i)
		{
			width += rowWidth[m_tableRowSum - i -1];
			if(width > m_showPixWidth && i < m_tableRowSum && flag == 0)
			{
				flag = i;
				//break;
			}
		}
		if(flag != 0)
		{//超出水平显示宽度则需要显示水平滑动条
			m_needShowHSlider = XTrue;
			//m_horizontalSlider.setRange(m_tableRowSum - i,0);
			m_horizontalSlider.setRange(width - m_showPixWidth + MUTILIST_TITLE_EXPAND_LENGTH * m_scale.x,0);	//注意这里的56是用于扩展的
		}else
		{
		//	if(m_needShowHSlider)
		//	{//从越界到不越界，需要重新初始化偏移量
		//		m_rowDx = 0;
		//	}
			m_needShowHSlider = XFalse;
			m_showStartRow = 0.0f;
			m_horizontalSlider.setCurValue(0.0f);
		}
	}
}
void XMultiListBasic::clearAllSelect()
{
	if(!m_isInited) return;
	for(int i = 0;i < m_tableLineSum;++ i)
	{
		m_haveSelectFlag[i] = XFalse;
	}
	//m_haveSelectSum = 0;
	m_haveSelect = XFalse;	//清除选择的标记
	m_selectLineOrder = 0;
}
void XMultiListBasic::setTitleStr(const char *str,int order)
{
	if(str == NULL) return;
	if(order < 0 || order >= m_tableRowSum) return;
	//寻找到指定的列
	XMultiListOneRow *tempRow;
	if(order == 0)
	{
		tempRow = m_tableRow;
	}else
	{
		tempRow = m_tableRow;
		for(int i = 1;i <= order;++ i)
		{
			tempRow = tempRow->nextRow;
		}
	}
	//修改指定列的字符串
//	char *tempTitle = XMem::createArrayMem<char>(strlen(str) + 1);
//	if(tempTitle == NULL) return;
//	strcpy(tempTitle,str);
//	XMem::XDELETE_ARRAY(tempRow->title);
//	tempRow->title = tempTitle;
	tempRow->title = str;
	//修改指定列的显示字符串
	if((int)(strlen(tempRow->title.c_str())) >= tempRow->stringShowWidth)
	{//如果超出显示长度
		if(tempRow->stringShowWidth >= 2)
		{//大于3个字符可以显示...否则显示..
			if(XString::isAtUnicodeEnd(tempRow->title.c_str(),tempRow->stringShowWidth - 2))
			{
				tempRow->title[tempRow->stringShowWidth] = '\0';
				tempRow->title[tempRow->stringShowWidth - 1] = '.';
				tempRow->title[tempRow->stringShowWidth - 2] = '.';
			}else
			{
				tempRow->title[tempRow->stringShowWidth] = '\0';
				tempRow->title[tempRow->stringShowWidth - 1] = '.';
				tempRow->title[tempRow->stringShowWidth - 2] = '.';
				tempRow->title[tempRow->stringShowWidth - 3] = '.';
			}
		}else
		{
			tempRow->title[tempRow->stringShowWidth] = '\0';
		}
		tempRow->text.setString(tempRow->title.c_str());
		//strcpy(tempRow->title,str);
		tempRow->title = str;
	}else
	{
		tempRow->text.setString(tempRow->title.c_str());
	}
}
void XMultiListBasic::setBoxStr(const char *str,int line,int row)	//设置某一个单元格的文字
{
	//输入参数合理性判断
	if(str == NULL) return;
	if(line < 0 || line >= m_tableLineSum) return;
	if(row < 0 || row >= m_tableRowSum) return;
	//寻找到指定的表格
	XMultiListOneBox *tempBox = m_tableBox;
	for(int i = 0;i < line * m_tableRowSum + row;++ i)
	{
		tempBox = tempBox->nextBox;
	}
	XMultiListOneRow *tempRow = m_tableRow;
	for(int i = 0;i < row;++ i)
	{
		tempRow = tempRow->nextRow;
	}
	//分配内存空间
	//char * tempStr = XMem::createArrayMem<char>(strlen(str) + 1);
	//if(tempStr == NULL) return;
	//strcpy(tempStr,str);//赋值
	//XMem::XDELETE_ARRAY(tempBox->string);
	//tempBox->string = tempStr;
	tempBox->textStr = str;
	//设置显示字符
	if((int)(strlen(tempBox->textStr.c_str())) >= tempRow->stringShowWidth)
	{//如果超出显示长度
		if(tempRow->stringShowWidth >= 2)
		{//大于3个字符可以显示...否则显示..
			if(XString::isAtUnicodeEnd(tempBox->textStr.c_str(),tempRow->stringShowWidth - 2))
			{
				tempBox->textStr[tempRow->stringShowWidth] = '\0';
				tempBox->textStr[tempRow->stringShowWidth - 1] = '.';
				tempBox->textStr[tempRow->stringShowWidth - 2] = '.';
			}else
			{
				tempBox->textStr[tempRow->stringShowWidth] = '\0';
				tempBox->textStr[tempRow->stringShowWidth - 1] = '.';
				tempBox->textStr[tempRow->stringShowWidth - 2] = '.';
				tempBox->textStr[tempRow->stringShowWidth - 3] = '.';
			}
		}else
		{
			tempBox->textStr[tempRow->stringShowWidth] = '\0';
		}
		tempBox->text.setString(tempBox->textStr.c_str());
		//strcpy(tempBox->string,str);
		tempBox->textStr = str;
	}else
	{
		tempBox->text.setString(tempBox->textStr.c_str());
	}
}
void XMultiListBasic::updateShowChange()				//根据列表框的变化信息更新列表框中的表格的显示情况
{
	if(!m_isInited) return;		//如果没有初始化直接退出
	if(m_tableRowSum <= 0) return;	//如果没有表格直接返回
	//标题的显示
	XMultiListOneRow *tempRow = m_tableRow;
	float curWidthPix = 0.0f;
	int row = 0;
	int showMaxRow = 0;
//	int showDx = m_showStartRow; //显示的数据表格的水平偏移	m_showStartRow(水平偏移量)
	while(true)
	{
		if(curWidthPix + tempRow->pixSize.x * m_scale.x > m_showStartRow && curWidthPix - m_showStartRow <= m_showPixWidth)
		{
			if(curWidthPix < m_showStartRow && curWidthPix + tempRow->pixSize.x * m_scale.x - m_showStartRow <= m_showPixWidth)
			{//显示右边部分
				tempRow->isShow = 1;
				if(row > showMaxRow) showMaxRow = row;
				//改变需要显示的标题的位置
				tempRow->needChip = 1;					//是否需要切割
				tempRow->left = (m_showStartRow - curWidthPix) / m_scale.x;						//左边的切割位置
				tempRow->right = tempRow->pixSize.x;					//右边的切割位置
				tempRow->text.setClipRect(tempRow->left,0.0f,tempRow->right,tempRow->text.getTextSize().y);
				if(m_withoutTex)
				{
					tempRow->posX = tempRow->position.x - curWidthPix;
					tempRow->pixLen = tempRow->pixSize.x * m_scale.x - (m_showStartRow - curWidthPix);
					if(tempRow->pixSize.x * m_scale.x - DEFAULT_END_WIDTH * m_tableRow->pixSize.y * m_scale.y / DEFAULT_TITLE_HEIGHT < 
						m_showStartRow - curWidthPix)
					{
						tempRow->isShow = 2;
					}
				}else
				{
					if(tempRow->pixSize.x * m_scale.x - m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / 
						m_mutiListTitle->textureSize.y < m_showStartRow - curWidthPix)
					{
						tempRow->isShow = 2;
					}
					tempRow->m_spriteTitle.setPosition(tempRow->position.x - curWidthPix,tempRow->position.y);
					tempRow->m_spriteTitle.setScale((float)(tempRow->pixSize.x * m_scale.x - (m_showStartRow - curWidthPix)) / m_mutiListTitle->textureSize.x,m_scale.y);
					tempRow->m_spriteTitleEnd.setPosition(tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_scale.x - 
						m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
						m_tableRow->position.y);
				}
				tempRow->text.setPosition(tempRow->position.x - m_showStartRow,tempRow->position.y);
				curWidthPix += tempRow->pixSize.x * m_scale.x;
			}else
			if(curWidthPix + tempRow->pixSize.x * m_scale.x - m_showStartRow <= m_showPixWidth && curWidthPix >= m_showStartRow)
			{//可以完整显示
				tempRow->isShow = 1;
				curWidthPix += tempRow->pixSize.x * m_scale.x;
				if(row > showMaxRow) showMaxRow = row;
				//改变需要显示的标题的位置
				if(m_withoutTex)
				{
					tempRow->posX = tempRow->position.x - m_showStartRow;
					tempRow->pixLen = tempRow->pixSize.x * m_scale.x;
				}else
				{
					tempRow->m_spriteTitle.setPosition(tempRow->position.x - m_showStartRow,tempRow->position.y);
					tempRow->m_spriteTitle.setScale(tempRow->pixSize.x * m_scale.x / m_mutiListTitle->textureSize.x,m_scale.y);
					tempRow->m_spriteTitleEnd.setPosition(tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_scale.x - 
						m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
						m_tableRow->position.y);
				}
				tempRow->text.disClip();
				tempRow->text.setPosition(tempRow->position.x - m_showStartRow,tempRow->position.y);
				tempRow->needChip = 0;					//是否需要切割
				tempRow->left = 0.0f;						//左边的切割位置
				tempRow->right = 0.0f;					//右边的切割位置
			}else
			if(curWidthPix >= m_showStartRow && curWidthPix + tempRow->pixSize.x * m_scale.x - m_showStartRow > m_showPixWidth)
			{//显示后面一部分
				tempRow->isShow = 1;
				if(row > showMaxRow) showMaxRow = row;
				//改变需要显示的标题的位置
				tempRow->needChip = 2;					//是否需要切割
				tempRow->left = 0.0f;						//左边的切割位置
				tempRow->right = (m_showPixWidth - (curWidthPix - m_showStartRow)) / m_scale.x;					//右边的切割位置
				tempRow->text.setClipRect(0.0f,0.0f,tempRow->right,tempRow->text.getTextSize().y);
				if(m_withoutTex)
				{
					tempRow->posX = tempRow->position.x - m_showStartRow;
					tempRow->pixLen = m_showPixWidth - (curWidthPix - m_showStartRow);
					if(tempRow->pixSize.x * m_scale.x - DEFAULT_END_WIDTH * m_tableRow->pixSize.y * m_scale.y / DEFAULT_TITLE_HEIGHT > 
						m_showPixWidth - (curWidthPix - m_showStartRow))
					{
						tempRow->isShow = 2;
					}
				}else
				{
					if(tempRow->pixSize.x * m_scale.x - m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y > 
						m_showPixWidth - (curWidthPix - m_showStartRow))
					{
						tempRow->isShow = 2;
					}
					tempRow->m_spriteTitle.setPosition(tempRow->position.x - m_showStartRow,tempRow->position.y);
					tempRow->m_spriteTitle.setScale((float)(m_showPixWidth - (curWidthPix - m_showStartRow)) / m_mutiListTitle->textureSize.x,m_scale.y);
					tempRow->m_spriteTitleEnd.setPosition(tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_scale.x - 
						m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
						m_tableRow->position.y);
				}
				tempRow->text.setPosition(tempRow->position.x - m_showStartRow,tempRow->position.y);
				curWidthPix += tempRow->pixSize.x * m_scale.x;
			}else
			if(curWidthPix < m_showStartRow && curWidthPix + tempRow->pixSize.x * m_scale.x - m_showStartRow > m_showPixWidth)
			{//这是需要掐头去尾，目前没有处理
				tempRow->isShow = 1;
				if(row > showMaxRow) showMaxRow = row;
				//改变需要显示的标题的位置
				tempRow->needChip = 2;					//是否需要切割
				tempRow->left = (m_showStartRow - curWidthPix) / m_scale.x;						//左边的切割位置
				tempRow->right = (m_showPixWidth - (curWidthPix - m_showStartRow)) / m_scale.x;					//右边的切割位置
				tempRow->text.setClipRect(tempRow->left,0.0f,tempRow->right,tempRow->text.getTextSize().y);
				if(m_withoutTex)
				{
					tempRow->posX = tempRow->position.x - curWidthPix;
					tempRow->pixLen = m_showPixWidth;
					if(tempRow->pixSize.x * m_scale.x - DEFAULT_END_WIDTH * m_tableRow->pixSize.y * m_scale.y / DEFAULT_TITLE_HEIGHT > 
						m_showPixWidth - (curWidthPix - m_showStartRow))
					{
						tempRow->isShow = 2;
					}
				}else
				{
					if(tempRow->pixSize.x * m_scale.x - m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y > 
						m_showPixWidth - (curWidthPix - m_showStartRow))
					{
						tempRow->isShow = 2;
					}
					tempRow->m_spriteTitle.setPosition(tempRow->position.x - curWidthPix,tempRow->position.y);
					tempRow->m_spriteTitle.setScale((float)(m_showPixWidth) / m_mutiListTitle->textureSize.x,m_scale.y);
					tempRow->m_spriteTitleEnd.setPosition(tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_scale.x - 
						m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
						m_tableRow->position.y);
				}
				tempRow->text.setPosition(tempRow->position.x - m_showStartRow,tempRow->position.y);
				curWidthPix += tempRow->pixSize.x * m_scale.x;
			}else
			{
				printf("haha\n");
			}
		}else
		{
			curWidthPix += tempRow->pixSize.x * m_scale.x;
			tempRow->isShow = 0;
		}
	//	if(row < m_showStartRow)
	//	{//计算水平偏移
	//		showDx += tempRow->pixSize.x * m_scale.x;
	//	}
		if(tempRow->nextRow == NULL) break;
		else
		{
			++ row;
			tempRow = tempRow->nextRow;
		}
	}
	//下面修改左边状态栏的显示
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		if(m_tableLineSum < m_canShowLineSum && i >= m_tableLineSum)
		{
			m_check0[i].disVisible();
			m_check1[i].disVisible();
		}else
		{
			m_check0[i].setVisible();
			m_check1[i].setVisible();
		}
	}
	//将所有表格数据全部隐藏
	if(m_tableLineSum <= 0) return;	//如果没有行直接返回
	XMultiListOneBox *tempBox = m_tableBox;
	tempRow = m_tableRow;
	row = 0;
	int line = 0;
	while(true)
	{
		if(line >= m_showStartLine && line < m_showStartLine + m_canShowLineSum)
		{
			//if(row >= m_showStartRow && row <= showMaxRow)
			if(tempRow->isShow != 0)
			{
				tempBox->isShow = XTrue;
				tempBox->text.setPosition(tempRow->position.x - m_showStartRow,tempRow->position.y + m_curTextHeight * (line - m_showStartLine + 1));
				if(tempRow->needChip != 0) tempBox->text.setClipRect(tempRow->left,0.0f,tempRow->right,tempBox->text.getTextSize().y);
				else tempBox->text.disClip();
			}else
			{
				tempBox->isShow = XFalse;
			}
		}else
		{
			tempBox->isShow = XFalse;
		}
		if(tempBox->nextBox == NULL) break;
		else tempBox = tempBox->nextBox;
		++row;
		if(row >= m_tableRowSum)
		{
			row = 0;
			++ line;
			tempRow = m_tableRow;
		}else
		{
			assert(tempRow != NULL);
			tempRow = tempRow->nextRow;
		}
	}
	//下面更新列表框的选择状态的信息
	for(int i = m_showStartLine;i < min(m_showStartLine + m_canShowLineSum,m_tableLineSum);++ i)
	{
		if(m_check0State[i]) m_check0[i - m_showStartLine].setState(XTrue);
		else m_check0[i - m_showStartLine].setState(XFalse);
		if(m_check1State[i]) m_check1[i - m_showStartLine].setState(XTrue);
		else m_check1[i - m_showStartLine].setState(XFalse);
	}
}
//注意这里传入的尺寸为缩放之前的尺寸，也就是是空间缩放比例为1的时候得尺寸
XBool XMultiListBasic::setRowWidth(int temp,int order)	//设置其中一行的宽度
{
	if(temp < MUTILIST_MIN_WIDTH || temp > m_showPixWidth / m_scale.x) return XFalse;
	if(order < 0 || order >= m_tableRowSum) return XFalse;
	//找到对应的一列
	XMultiListOneRow *tempRow = m_tableRow;
	for(int i = 0;i < order;++ i)
	{
		tempRow = tempRow->nextRow;
	}
	
	tempRow->pixSize.set(temp,tempRow->pixSize.y);	//改变当前列的列宽
	tempRow->stringShowWidth = tempRow->pixSize.x * m_scale.x / m_curTextWidth;	//改变当前列的显示字符宽度
	if(!m_withoutTex)
	{
		tempRow->m_spriteTitleEnd.setPosition(tempRow->position.x + tempRow->pixSize.x * m_scale.x - 
			m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
			m_tableRow->position.y);
		tempRow->m_spriteTitle.setScale(tempRow->pixSize.x * m_scale.x / m_mutiListTitle->textureSize.x,m_scale.y);
	}
	tempRow->text.setPosition(tempRow->position.x,tempRow->position.y);
	//改变当前列的显示字符
	//char * str = XMem::createArrayMem<char>(strlen(tempRow->title) + 1);
	//if(str == NULL) return XFalse;
	//strcpy(str,tempRow->title);
	std::string str = tempRow->title;
	//修改指定列的显示字符串
	if((int)(strlen(tempRow->title.c_str())) >= tempRow->stringShowWidth)
	{//如果超出显示长度
		if(tempRow->stringShowWidth >= 2)
		{//大于3个字符可以显示...否则显示..
			//这里没有考虑汉子的情况
			if(XString::isAtUnicodeEnd(tempRow->title.c_str(),tempRow->stringShowWidth - 2))
			{
				tempRow->title[tempRow->stringShowWidth] = '\0';
				tempRow->title[tempRow->stringShowWidth - 1] = '.';
				tempRow->title[tempRow->stringShowWidth - 2] = '.';
			}else
			{
				tempRow->title[tempRow->stringShowWidth] = '\0';
				tempRow->title[tempRow->stringShowWidth - 1] = '.';
				tempRow->title[tempRow->stringShowWidth - 2] = '.';
				tempRow->title[tempRow->stringShowWidth - 3] = '.';
			}
		}else
		{
			tempRow->title[tempRow->stringShowWidth] = '\0';
		}
		tempRow->text.setString(tempRow->title.c_str());
		//strcpy(tempRow->title,str);
		tempRow->title = str;
	}else
	{
		tempRow->text.setString(tempRow->title.c_str());
	}
	//XMem::XDELETE_ARRAY(str);
	//修改后面的所有列的位置
	while(true)
	{
		if(tempRow->nextRow == NULL)
		{
			break;
		}else
		{
			tempRow->nextRow->position.set(tempRow->position.x + tempRow->pixSize.x * m_scale.x,tempRow->nextRow->position.y);
			if(!m_withoutTex)
			{
				tempRow->nextRow->m_spriteTitle.setPosition(tempRow->nextRow->position);
				tempRow->nextRow->m_spriteTitleEnd.setPosition(tempRow->nextRow->position.x + tempRow->nextRow->pixSize.x * m_scale.x - 
					m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
					m_tableRow->position.y);
			}
			tempRow->nextRow->text.setPosition(tempRow->nextRow->position.x,tempRow->nextRow->position.y);
			tempRow = tempRow->nextRow;
		}
	}
	//改变所有单元格的情况
	int i,j;
	std::string tmpStr;
	XMultiListOneBox *tempBox = m_tableBox;
	for(i = 0;i < m_tableLineSum;++ i)
	{
		tempRow = m_tableRow;
		for(j = 0;j < m_tableRowSum;++ j)
		{
			if(j == order)
			{//改变字符串
				//tmpStr = XMem::createArrayMem<char>(strlen(tempBox->string) + 1);
				//if(tmpStr == NULL) return XFalse;
				//strcpy(tmpStr,tempBox->string);
				tmpStr = tempBox->textStr;
				if((int)(strlen(tempBox->textStr.c_str())) >= tempRow->stringShowWidth)
				{//如果超出显示长度
					if(tempRow->stringShowWidth >= 2)
					{//大于3个字符可以显示...否则显示..
						if(XString::isAtUnicodeEnd(tempBox->textStr.c_str(),tempRow->stringShowWidth - 2))
						{
							tempBox->textStr[tempRow->stringShowWidth] = '\0';
							tempBox->textStr[tempRow->stringShowWidth - 1] = '.';
							tempBox->textStr[tempRow->stringShowWidth - 2] = '.';
						}else
						{
							tempBox->textStr[tempRow->stringShowWidth] = '\0';
							tempBox->textStr[tempRow->stringShowWidth - 1] = '.';
							tempBox->textStr[tempRow->stringShowWidth - 2] = '.';
							tempBox->textStr[tempRow->stringShowWidth - 3] = '.';
						}
					}else
					{
						tempBox->textStr[tempRow->stringShowWidth] = '\0';
					}
					tempBox->text.setString(tempBox->textStr.c_str());
					//strcpy(tempBox->string,tmpStr);
					tempBox->textStr = tmpStr;
				}else
				{
					tempBox->text.setString(tempBox->textStr.c_str());
				}
				//XMem::XDELETE_ARRAY(tmpStr);
			}
			if(j > order)
			{//改变位置
				tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
			}

			tempRow = tempRow->nextRow;
			tempBox = tempBox->nextBox;
		}
	}
	//跟新显示设置
	updateSliderState();
	updateShowChange();
	return XTrue;
}
void XMultiListBasic::draw()	//描绘函数
{
	if(!m_isInited ||
		!m_isVisible) return;	//如果不可见直接退出

	if(m_withoutTex)
	{
		if(!m_isEnable) 
		{
			XRender::drawFillBoxA(m_position + XVector2(0.0f,m_mouseRect.top * m_scale.y),
				XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.getHeight() * m_scale.y),
				XCCS::mouseColor * m_color,true);
			XRender::drawFillBoxExA(m_position + XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y),
				XVector2((m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
				(m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight()) * m_scale.y),
				XCCS::specialColor * m_color,true);
		}else
		{
			XRender::drawFillBoxA(m_position + XVector2(0.0f,m_mouseRect.top * m_scale.y),
				XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.getHeight() * m_scale.y),
				XCCS::downColor * m_color,true);
			XRender::drawFillBoxExA(m_position + XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y),
				XVector2((m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
				(m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight()) * m_scale.y),
				XCCS::normalColor * m_color,true);
		}
		//这里显示列表的文字
		//显示标题
		if(m_tableRowSum > 0)
		{
			XMultiListOneRow *tempRow = m_tableRow;
			while(true)
			{
				if(tempRow->isEnable && tempRow->isShow != 0)
				{
					XRender::drawFillBoxExA(XVector2(tempRow->posX,tempRow->position.y),XVector2(tempRow->pixLen,DEFAULT_TITLE_HEIGHT * m_scale.y),
						XCCS::blackOnColor * m_color);
		//			tempRow->m_spriteTitle.draw(m_mutiListTitle);	//显示标题背景
					tempRow->text.draw();//显示标题文字
					if(tempRow->isShow != 0 && tempRow->needChip != 2)
					{
						int x = tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_scale.x - 
							DEFAULT_END_WIDTH * tempRow->pixSize.y * m_scale.y / DEFAULT_TITLE_HEIGHT;
						//if(x >= m_curMouseRect.left)
						{
							XRender::drawLine(x,m_tableRow->position.y,x,m_tableRow->position.y + DEFAULT_TITLE_HEIGHT * m_scale.y,1,
								XCCS::mouseColor * m_color);
							//描绘列分界线
							XRender::drawLine(x,m_tableRow->position.y + DEFAULT_TITLE_HEIGHT * m_scale.y,x,
								m_tableRow->position.y + (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight()) * m_scale.y,1,
								XCCS::blackNormalColor * m_color);
							//tempRow->m_spriteTitleEnd.draw(m_mutiListTitleEnd);	//显示标题结束符
						}
					}
				}
				if(tempRow->nextRow == NULL) break;
				else tempRow = tempRow->nextRow;
			}
			if(m_tableLineSum > 0)
			{
				//下面显示选择的结果
				for(int i = 0;i < min(m_canShowLineSum,m_tableLineSum - m_showStartLine);++ i)
				{
					if(m_haveSelectFlag[m_showStartLine + i])
					{//显示选择了
				//		m_spriteSelect.setPosition(m_position.x + m_mouseRect.left * m_scale.x,
				//			m_position.y + (m_mouseRect.top + m_mutiListTitle->textureSize.y) * m_scale.y + 
				//			i * m_curTextHeight);

				//		m_spriteSelect.draw(m_mutiListSelect);
						XRender::drawFillBoxExA(XVector2(m_position.x + m_mouseRect.left * m_scale.x,
							m_position.y + (m_mouseRect.top + DEFAULT_TITLE_HEIGHT) * m_scale.y + i * m_curTextHeight),
							XVector2(m_showPixWidth,m_curTextHeight),XCCS::onColor * m_color);
					}
				}
				if(m_haveSelect && m_selectLineOrder >= m_showStartLine && m_selectLineOrder < m_showStartLine + m_canShowLineSum)
				{
					XRender::drawRect(m_selectRect,1.0f,XCCS::darkColor * m_color);
				}
				XMultiListOneBox *tempBox = m_tableBox;
				while(true)
				{
					if(tempBox->isEnable && tempBox->isShow) tempBox->text.draw();//显示标题文字
					if(tempBox->nextBox == NULL) break;
					else tempBox = tempBox->nextBox;
				}
			}
		}
		if(m_needShowMove) 
		{
			int x = m_changeRow->position.x - m_showStartRow + m_changeRow->pixSize.x * m_scale.x - 
								DEFAULT_END_WIDTH * m_tableRow->pixSize.y * m_scale.y / DEFAULT_TITLE_HEIGHT;
			XRender::drawLine(x,m_changeRow->position.y,x,m_changeRow->position.y + (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight()) * m_scale.y,1,
				XCCS::blackDownColor * m_color);
			//m_spriteMove.draw(m_mutiListMove);
		}
	}else
	{
		if(!m_isEnable) m_spriteBackGround.draw(m_mutiListDisable);
		else m_spriteBackGround.draw(m_mutiListNormal);
		//这里显示列表的文字
		//显示标题
		if(m_tableRowSum > 0)
		{
			XMultiListOneRow *tempRow = m_tableRow;
			while(true)
			{
				if(tempRow->isEnable && tempRow->isShow != 0)
				{
					tempRow->m_spriteTitle.draw(m_mutiListTitle);	//显示标题背景
					tempRow->text.draw();//显示标题文字
					if(tempRow->isShow != 0 && tempRow->needChip != 2)
					{
						tempRow->m_spriteTitleEnd.draw(m_mutiListTitleEnd);	//显示标题结束符
					}
				}
				if(tempRow->nextRow == NULL) break;
				else tempRow = tempRow->nextRow;
			}
			if(m_tableLineSum > 0)
			{
				//下面显示选择的结果
				for(int i = 0;i < min(m_canShowLineSum,m_tableLineSum - m_showStartLine);++ i)
				{
					if(m_haveSelectFlag[m_showStartLine + i])
					{//显示选择了
						m_spriteSelect.setPosition(m_position.x + m_mouseRect.left * m_scale.x,
							m_position.y + (m_mouseRect.top + m_mutiListTitle->textureSize.y) * m_scale.y + 
							i * m_curTextHeight);

						m_spriteSelect.draw(m_mutiListSelect);
					}
				}
				if(m_haveSelect && m_selectLineOrder >= m_showStartLine && m_selectLineOrder < m_showStartLine + m_canShowLineSum)
				{
					XRender::drawRect(m_selectRect,1.0f,XCCS::darkColor * m_color);
				}
				XMultiListOneBox *tempBox = m_tableBox;
				while(true)
				{
					if(tempBox->isEnable && tempBox->isShow) tempBox->text.draw();//显示标题文字
					if(tempBox->nextBox == NULL) break;
					else tempBox = tempBox->nextBox;
				}
			}
		}
		if(m_needShowMove) m_spriteMove.draw(m_mutiListMove);
	}
	if(m_withoutTex)
	{
		if(!m_isEnable) 
		{
			XRender::drawFillBoxExA(m_position + XVector2((m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
				m_mouseRect.top * m_scale.y),
				XVector2(m_verticalSlider.getMouseRectWidth() * m_scale.x,
				(m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight()) * m_scale.y),
				XCCS::downColor * m_color,true);
			XRender::drawFillBoxExA(m_position + XVector2(m_mouseRect.left * m_scale.x,
				(m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale.y),
				XVector2((m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
				m_horizontalSlider.getMouseRectHeight() * m_scale.y),XCCS::downColor * m_color,true);
		}else
		{
			XRender::drawFillBoxExA(m_position + XVector2((m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
				m_mouseRect.top * m_scale.y),
				XVector2(m_verticalSlider.getMouseRectWidth() * m_scale.x,
				(m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight()) * m_scale.y),
				XCCS::lightSpecialColor * m_color,true);
			XRender::drawFillBoxExA(m_position + XVector2(m_mouseRect.left * m_scale.x,
				(m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale.y),
				XVector2((m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
				m_horizontalSlider.getMouseRectHeight() * m_scale.y),
				XCCS::lightSpecialColor * m_color,true);
		}
	}
	if(m_needShowVSlider) m_verticalSlider.draw();
	if(m_needShowHSlider) m_horizontalSlider.draw();
	if(m_withoutTex)
	{
		if(!m_isEnable) 
		{
			XRender::drawFillBoxExA(m_position + XVector2((m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
				(m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale.y),
				XVector2(m_verticalSlider.getMouseRectWidth() * m_scale.x,
				m_horizontalSlider.getMouseRectHeight() * m_scale.y),
				XCCS::blackDownColor * m_color,true);
		}else
		{
			XRender::drawFillBoxExA(m_position + XVector2((m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
				(m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale.y),
				XVector2(m_verticalSlider.getMouseRectWidth() * m_scale.x,
				m_horizontalSlider.getMouseRectHeight() * m_scale.y),
				XCCS::lightMouseColor * m_color,true);
		}
	}
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_check0[i].draw();
		m_check1[i].draw();
	}
}
XBool XMultiListBasic::mouseProc(float x,float y,XMouseState mouseState)					//对于鼠标动作的响应函数
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出

	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_check0[i].mouseProc(x,y,mouseState);
		m_check1[i].mouseProc(x,y,mouseState);
	}
	if(m_needShowVSlider)
	{
		m_verticalSlider.mouseProc(x,y,mouseState);
		if(isInRect(x,y))
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
	if(m_needShowHSlider) m_horizontalSlider.mouseProc(x,y,mouseState);
	//下面判断鼠标修改标题栏宽度判断
	if(m_tableRowSum > 0)
	{
		if(mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_LEFT_BUTTON_DCLICK)
		{//左键按下
			XRect tempRect;
			XMultiListOneRow *tempRow = m_tableRow;
			int order = 0;
		//	int showDx = m_showStartRow;	//这个需要计算列相对于开始显示的那一列的偏移量，因为列的位置是先对于第一列的绝对位置，所以需要更具实际的显示计算相对的显示位置
			while(true)
			{
				if(tempRow->isShow != 0)
				{//显示出来的才能修改
					if(m_withoutTex)
					{
						tempRect.set(tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_scale.x - 
							DEFAULT_END_WIDTH * m_tableRow->pixSize.y * m_scale.y / DEFAULT_TITLE_HEIGHT,
							tempRow->position.y,
							tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_scale.x,
							tempRow->position.y + tempRow->pixSize.y * m_scale.y);
					}else
					{
						tempRect.set(tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_scale.x - 
							m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
							tempRow->position.y,
							tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_scale.x,
							tempRow->position.y + tempRow->pixSize.y * m_scale.y);
					}
					if(tempRect.isInRect(x,y))
					{//标记拖动有效
						m_mouseLeftButtonDown = XTrue;
						m_startX = x;
					//	m_rowDx = m_showStartRow;
						m_changeRowOrder = order;
						m_changeRow = tempRow;
						//设置开始显示移动图标
						m_needShowMove = XTrue;
						if(!m_withoutTex)
						{
							m_spriteMove.setPosition(m_changeRow->position.x - m_showStartRow + m_changeRow->pixSize.x * m_scale.x - 
								m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
								m_changeRow->position.y);
						}
						break;
					}
				}
			//	if(order < m_showStartRow)
			//	{
			///		showDx += tempRow->pixSize.x * m_scale.x;
			//	}
				if(tempRow->nextRow == NULL) break;
				else tempRow = tempRow->nextRow;
				++ order;
			}
		}
		if(mouseState == MOUSE_MOVE && m_mouseLeftButtonDown)
		{//拖动有效
			//计算拖动的结果
			int dx = x - m_startX;
			m_startX = x;
			if(m_changeRow->position.x - m_showStartRow - m_tableRow->position.x + 
				m_changeRow->pixSize.x * m_scale.x + dx > m_showPixWidth ||
				x < m_curMouseRect.left)
			{//如果移动出界则取消移动，并将扩展到最大
				setRowWidth((m_showPixWidth - m_changeRow->position.x + m_showStartRow + m_tableRow->position.x) / m_scale.x,m_changeRowOrder);
				m_needShowMove = XFalse;			//取消移动图标的显示
				m_mouseLeftButtonDown = XFalse;
			}else
			if(m_changeRow->pixSize.x *m_scale.x + dx < MUTILIST_MIN_WIDTH * m_scale.x)
			{//如果单元格宽度小于最小值，则使用最小值，并取消移动
				setRowWidth(MUTILIST_MIN_WIDTH,m_changeRowOrder);
				m_needShowMove = XFalse;			//取消移动图标的显示
				m_mouseLeftButtonDown = XFalse;
			}else
			{
				if(!setRowWidth(m_changeRow->pixSize.x + dx / m_scale.x,m_changeRowOrder))
				{//拖动失败则去掉焦点
					m_needShowMove = XFalse;			//取消移动图标的显示
					m_mouseLeftButtonDown = XFalse;
				}
				if(!m_withoutTex)
				{
					m_spriteMove.setPosition(m_changeRow->position.x - m_showStartRow + m_changeRow->pixSize.x * m_scale.x - 
						m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
						m_changeRow->position.y);
				}
			}
		}
	}
	if(mouseState == MOUSE_LEFT_BUTTON_UP)
	{//左键谈起则去处焦点
		if(m_mouseLeftButtonDown)
		{
			m_needShowMove = XFalse;			//取消移动图标的显示
			m_mouseLeftButtonDown = XFalse;
		}
		if(isInRect(x,y)) m_isBeChoose = XTrue;
	}
	//下面判断行选
	if(m_tableLineSum > 0)
	{
		if(mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_LEFT_BUTTON_DCLICK)
		{//左键按下
			//计算点选范围
			XRect tempRect;
			if(m_withoutTex)
			{
				tempRect.set(m_position.x + m_mouseRect.left * m_scale.x,
					m_position.y + m_mouseRect.top * m_scale.x + DEFAULT_TITLE_HEIGHT * m_scale.y,
					m_position.x + m_mouseRect.left * m_scale.x + m_showPixWidth ,
					m_position.y + m_mouseRect.top * m_scale.x + DEFAULT_TITLE_HEIGHT * m_scale.y + m_showPixHight);
			}else
			{
				tempRect.set(m_position.x + m_mouseRect.left * m_scale.x,
					m_position.y + m_mouseRect.top * m_scale.x + m_mutiListTitle->textureSize.y * m_scale.y,
					m_position.x + m_mouseRect.left * m_scale.x + m_showPixWidth ,
					m_position.y + m_mouseRect.top * m_scale.x + m_mutiListTitle->textureSize.y * m_scale.y + m_showPixHight);
			}
			if(tempRect.isInRect(x,y))
			{//计算点选得是哪一行
				int DLine = (y - tempRect.top) / m_curTextHeight;
				if(DLine + m_showStartLine < m_tableLineSum)
				{//点选有效
					if(m_haveSelect && m_selectLineOrder == DLine + m_showStartLine)
					{//重复选择表示取消选择
						m_haveSelect = XFalse;
						m_selectLineOrder = DLine + m_showStartLine;
						if(m_haveSelectFlag[m_selectLineOrder])
						{
							m_haveSelectFlag[m_selectLineOrder] = XFalse;
							if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,MLTLST_SELECT);
							else XCtrlManager.eventProc(m_objectID,MLTLST_SELECT);;
						}
					}else
					{
						//这里做片选判断并处理
						if(m_stateOfShiftKey == KEY_STATE_DOWN && m_haveSelect)
						{//片选
							if(m_selectLineOrder < DLine + m_showStartLine)
							{
								for(int i = m_selectLineOrder;i < DLine + m_showStartLine;++ i)
								{
									m_haveSelectFlag[i] = XTrue;
								}
							}else
							{
								for(int i = DLine + m_showStartLine;i < m_selectLineOrder;++ i)
								{
									m_haveSelectFlag[i] = XTrue;
								}
							}
						}
						m_haveSelect = XTrue;
						m_selectLineOrder = DLine + m_showStartLine;
						if(!m_haveSelectFlag[m_selectLineOrder])
						{
							m_haveSelectFlag[m_selectLineOrder] = XTrue;
							if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,MLTLST_SELECT);
							else XCtrlManager.eventProc(m_objectID,MLTLST_SELECT);;
						}
					}

					updateSelectLine();
					//标记鼠标按下有效
					m_mouseMoveDown = XTrue;
					m_oldLineOrder = m_selectLineOrder;
				}
			}
		}
		if(mouseState == MOUSE_MOVE && m_mouseMoveDown)
		{//拖动有效
			//计算点选范围(注意这里扩展的20是为了实现拖动超出范围的处理)
			XRect tempRect;
			if(m_withoutTex)
			{
				tempRect.set(m_position.x + m_mouseRect.left * m_scale.x,
					m_position.y + m_mouseRect.top * m_scale.x + DEFAULT_TITLE_HEIGHT * m_scale.y - 100,
					m_position.x + m_mouseRect.left * m_scale.x + m_showPixWidth,
					m_position.y + m_mouseRect.top * m_scale.x + DEFAULT_TITLE_HEIGHT * m_scale.y + m_showPixHight + 100);
			}else
			{
				tempRect.set(m_position.x + m_mouseRect.left * m_scale.x,
					m_position.y + m_mouseRect.top * m_scale.x + m_mutiListTitle->textureSize.y * m_scale.y - 100,
					m_position.x + m_mouseRect.left * m_scale.x + m_showPixWidth,
					m_position.y + m_mouseRect.top * m_scale.x + m_mutiListTitle->textureSize.y * m_scale.y + m_showPixHight + 100);
			}
			if(tempRect.isInRect(x,y))
			{
				int DLine = (y - tempRect.top - 100) / m_curTextHeight;
				if(DLine < 0 && m_showStartLine > 0)
				{//超过上限,上移一格并将表的显示向上移动
					if(moveUpLine(m_oldLineOrder))
					{
						-- m_oldLineOrder;
						-- m_showStartLine;
						m_verticalSlider.setCurValue(m_showStartLine);
						updateSliderState();
						updateShowChange();
					}
				}else
				if(DLine >= m_canShowLineSum && m_showStartLine < m_tableLineSum - m_canShowLineSum)
				{//超过下限,下移一格并将标的显示向下移动
					if(moveDownLine(m_oldLineOrder))
					{
						++ m_oldLineOrder;
						++ m_showStartLine;
						m_verticalSlider.setCurValue(m_showStartLine);
						updateSliderState();
						updateShowChange();
					}
				}else
				if(DLine + m_showStartLine < m_oldLineOrder)
				{//向上移动
					if(moveUpLine(m_oldLineOrder))
					{
						-- m_oldLineOrder;
					}
				}else
				if(DLine + m_showStartLine > m_oldLineOrder)
				{//向下移动
					if(moveDownLine(m_oldLineOrder))
					{
						++ m_oldLineOrder;
					}
				}
			}
		}
	}
	if(mouseState == MOUSE_LEFT_BUTTON_UP && m_mouseMoveDown)
	{
		m_mouseMoveDown = XFalse;
		m_oldLineOrder = m_selectLineOrder;
	}
	return XTrue;
}
void XMultiListBasic::updateSelectLine()
{
	if(!m_isInited ||		//如果没有初始化直接退出
		!m_haveSelect) return;
	if(m_selectLineOrder >= m_showStartLine && m_selectLineOrder < m_showStartLine + m_canShowLineSum)
	{//设置选择的位置
		XVector2 temp;
		if(m_withoutTex)
		{
			temp.set(m_position.x + m_mouseRect.left * m_scale.x,
				m_position.y + (m_mouseRect.top + DEFAULT_TITLE_HEIGHT) * m_scale.y + 
				(m_selectLineOrder - m_showStartLine) * m_curTextHeight);
		}else
		{
			temp.set(m_position.x + m_mouseRect.left * m_scale.x,
				m_position.y + (m_mouseRect.top + m_mutiListTitle->textureSize.y) * m_scale.y + 
				(m_selectLineOrder - m_showStartLine) * m_curTextHeight);
		}
		m_selectRect.set(temp.x,temp.y,temp.x + m_showPixWidth,temp.y + m_curTextHeight);
	}
}
//文件的命名格式为XMultiList_ID.dat
XBool XMultiListBasic::exportData(const char *fileName)	//数据导出
{//数据导出到文件中
	if(!m_isInited) return XFalse;		//如果没有初始化直接退出
	FILE *fp;
	if(fileName == NULL)
	{
		char fileNameT[] = "XMultiListBasic_xxx.dat";
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
	//写入列信息
	XMultiListOneRow *tempRow = m_tableRow;
	int strLength = 0;
	float tempWidth = 0;
	fwrite(&m_tableRowSum,sizeof(int),1,fp);
	for(int i = 0;i < m_tableRowSum;++ i)
	{
		tempWidth = tempRow->pixSize.x;
		fwrite(&(tempWidth),sizeof(float),1,fp);
		strLength = strlen(tempRow->title.c_str());
		fwrite(&(strLength),sizeof(int),1,fp);
		fwrite(tempRow->title.c_str(),strLength + 1,1,fp);
		tempRow = tempRow->nextRow;
	}
	//写入表格信息
	XMultiListOneBox *tempBox = m_tableBox;
	fwrite(&m_tableLineSum,sizeof(int),1,fp);
	//注意这里需要写入check的状态
	char tmp = 0;
	for(int i = 0;i < m_tableLineSum;++ i)
	{
		if(m_check0State[i]) tmp = 0;
		else tmp = 1;
		fwrite(&(tmp),sizeof(char),1,fp);
		if(m_check1State[i]) tmp = 0;
		else tmp = 1;
		fwrite(&(tmp),sizeof(char),1,fp);
	}
	for(int i = 0;i < m_tableLineSum * m_tableRowSum;++ i)
	{
		strLength = strlen(tempBox->textStr.c_str());
		fwrite(&(strLength),sizeof(int),1,fp);
		fwrite(tempBox->textStr.c_str(),strLength + 1,1,fp);
		tempBox = tempBox->nextBox;
	}
	fclose(fp);
	return XTrue;
}
XBool XMultiListBasic::importData(const char *fileName)	//数据导入
{//从文件中将数据导入
	if(!m_isInited) return XFalse;		//如果没有初始化直接退出
	FILE *fp;
	if(fileName == NULL)
	{
		char fileNameT[] = "XMultiListBasic_xxx.dat";
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
	//读取列信息
	int strLength = 0;
	int tempSum = 0;
	float tempWidth = 0;
	char *str;
	fread(&tempSum,sizeof(int),1,fp);
	if(tempSum != m_tableRowSum)
	{//表格不符合
		setRowSum(tempSum);
	}
	for(int i = 0;i < m_tableRowSum;++ i)
	{
		fread(&tempWidth,sizeof(float),1,fp);
		setRowWidth(tempWidth,i);
		fread(&(strLength),sizeof(int),1,fp);
		str = XMem::createArrayMem<char>(strLength + 1);
		if(str == NULL) return XFalse;

		fread(str,strLength + 1,1,fp);
		setTitleStr(str,i);
		XMem::XDELETE_ARRAY(str);
	}
	//读取表格信息
	fread(&tempSum,sizeof(int),1,fp);
	if(tempSum != m_tableLineSum)
	{//表格不符合
		setLineSum(tempSum);
	}
	//注意这里需要读取check的状态
	char tmp = 0;
	for(int i = 0;i < m_tableLineSum;++ i)
	{
		fread(&(tmp),sizeof(char),1,fp);
		if(tmp == 0) m_check0State[i] = XFalse;
		else m_check0State[i] = XTrue;
		fread(&(tmp),sizeof(char),1,fp);
		if(tmp == 0) m_check1State[i] = XFalse;
		else m_check1State[i] = XTrue;
	}
	for(int i = 0;i < m_tableLineSum * m_tableRowSum;++ i)
	{
		fread(&(strLength),sizeof(int),1,fp);
		str = XMem::createArrayMem<char>(strLength + 1);
		if(str == NULL) return XFalse;

		fread(str,strLength + 1,1,fp);
		setBoxStr(str,i / m_tableRowSum,i % m_tableRowSum);
		XMem::XDELETE_ARRAY(str);
	}
	fclose(fp);
	return XTrue;
}
XBool XMultiListBasic::saveState(TiXmlNode &e)
{
	if(!m_needSaveAndLoad) return XTrue;	//如果不需要保存则直接返回
	TiXmlElement elmNode(m_ctrlName.c_str());
	XBool flag = XTrue;
	//写入标题
	XMultiListOneRow *tempRow = m_tableRow;
	if(!XXml::addLeafNode(elmNode,"RowSum",XString::toString(m_tableRowSum))) flag = XFalse;
	for(int i = 0;i < m_tableRowSum;++ i)
	{
		if(!XXml::addLeafNode(elmNode,"Row",tempRow->title)) flag = XFalse;
		tempRow = tempRow->nextRow;
	}
	XMultiListOneBox *tempBox = m_tableBox;
	if(!XXml::addLeafNode(elmNode,"LineSum",XString::toString(m_tableLineSum))) flag = XFalse;
	//注意这里需要读取check的状态(尚未完成)
	for(int i = 0;i < m_tableLineSum;++ i)
	{
		if(m_check0State[i]) 
		{
			if(!XXml::addLeafNode(elmNode,"State0",true)) flag = XFalse;
		}else
		{
			if(!XXml::addLeafNode(elmNode,"State0",false)) flag = XFalse;
		}
	}
	for(int i = 0;i < m_tableLineSum;++ i)
	{
		if(m_check1State[i]) 
		{
			if(!XXml::addLeafNode(elmNode,"State1",true)) flag = XFalse;
		}else
		{
			if(!XXml::addLeafNode(elmNode,"State1",false)) flag = XFalse;
		}
	}
	for(int i = 0;i < m_tableLineSum * m_tableRowSum;++ i)
	{
		if(!XXml::addLeafNode(elmNode,"Box",tempBox->textStr)) flag = XFalse;
		tempBox = tempBox->nextBox;
	}
	if(e.InsertEndChild(elmNode) == NULL) flag = XFalse;
	return flag;
}
XBool XMultiListBasic::loadState(TiXmlNode *e)
{
	if(!m_needSaveAndLoad) return XTrue;	//如果不需要保存则直接返回
	TiXmlNode * keyNode = e->FirstChildElement(m_ctrlName.c_str());
	if(keyNode == NULL) return XFalse;
	//下面读取
	XBool flag = XTrue;
	int tmpI = 0;
	std::string tmpStr = "";
	if(!XXml::getXmlAsInt(keyNode,"RowSum",tmpI)) return XFalse;
	if(tmpI != m_tableRowSum) setRowSum(tmpI);//表格不符合
	int index = 0;
	TiXmlNode *tmpNode = XXml::getXmlAsString(keyNode,"Row",tmpStr);
	while(tmpNode != NULL)
	{
	//	if(tmpNode != NULL)
	//	{
			setTitleStr(tmpStr.c_str(),index);
			++ index;
	//	}else
	//		break;
		tmpNode = XXml::getNextXmlAsString(tmpNode,"Row",tmpStr);
	}

	if(!XXml::getXmlAsInt(keyNode,"LineSum",tmpI)) return XFalse;
	if(tmpI != m_tableLineSum) setLineSum(tmpI);//表格不符合
	//注意这里需要读取check的状态
	bool tmpB = true;
	index = 0;
	tmpNode = XXml::getXmlAsBool(keyNode,"State0",tmpB);
	while(tmpNode != NULL)
	{
	//	if(tmpNode != NULL)
	//	{
			m_check0State[index] = tmpB;
			++ index;
	//	}else
	//		break;
		tmpNode = XXml::getNextXmlAsBool(tmpNode,"State0",tmpB);
	}
	index = 0;
	tmpNode = XXml::getXmlAsBool(keyNode,"State1",tmpB);
	while(tmpNode != NULL)
	{
	//	if(tmpNode != NULL)
	//	{
			m_check1State[index] = tmpB;
			++ index;
	//	}else
	//		break;
		tmpNode = XXml::getNextXmlAsBool(tmpNode,"State1",tmpB);
	}
	index = 0;
	tmpNode = XXml::getXmlAsString(keyNode,"Box",tmpStr);
	while(tmpNode != NULL)
	{
	//	if(tmpNode != NULL)
	//	{
			setBoxStr(tmpStr.c_str(),index / m_tableRowSum,index % m_tableRowSum);
			++ index;
	//	}else
	//		break;
		tmpNode = XXml::getNextXmlAsString(tmpNode,"Box",tmpStr);
	}
	return flag;
}
void XMultiListBasic::initANewRowData(XMultiListOneRow * upRow,int i)
{
	if(i == 0)
	{//第一个元素
		m_tableRow->isEnable = XTrue;

		m_tableRow->order = 0;
		m_tableRow->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableRow->text));
#endif
		m_tableRow->text.setScale(m_scale * m_fontSize);
		m_tableRow->pixSize.set(MUTILIST_MIN_WIDTH,m_caption.getTextSize().y * m_caption.getScale().x);
		m_tableRow->position.set(m_position.x + (m_mouseRect.left + MUTILIST_MIN_WIDTH * 0) * m_scale.x,
			m_position.y + m_mouseRect.top * m_scale.y);
		m_tableRow->text.setPosition(m_tableRow->position.x,m_tableRow->position.y);
		if(!m_withoutTex)
		{
			m_tableRow->m_spriteTitle.init(m_mutiListTitle->texture.m_w,m_mutiListTitle->texture.m_h,1);	//设置标题背景的精灵
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(m_tableRow->m_spriteTitle));
#endif
			m_tableRow->m_spriteTitle.setPosition(m_tableRow->position);
			m_tableRow->m_spriteTitle.setScale(m_tableRow->pixSize.x * m_scale.x / m_mutiListTitle->textureSize.x,m_scale.y);
			m_tableRow->m_spriteTitle.setIsTransformCenter(POINT_LEFT_TOP);	//设置背景精灵的缩放为左上角对齐
			m_tableRow->m_spriteTitleEnd.init(m_mutiListTitleEnd->texture.m_w,m_mutiListTitleEnd->texture.m_h,1);
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(m_tableRow->m_spriteTitleEnd));
#endif
			m_tableRow->m_spriteTitleEnd.setPosition(m_tableRow->position.x + m_tableRow->pixSize.x * m_scale.x - 
				m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,m_tableRow->position.y);
			m_tableRow->m_spriteTitleEnd.setScale(m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
				m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y);
			m_tableRow->m_spriteTitleEnd.setIsTransformCenter(POINT_LEFT_TOP);
		}

		m_tableRow->stringShowWidth = m_tableRow->pixSize.x * m_scale.x / m_curTextWidth;
		m_tableRow->title = "";
		setTitleStr("Title",0);	//设置默认显示
		m_tableRow->isShow = 1;
	}else
	{
		upRow->nextRow->isEnable = XTrue;
		upRow->nextRow->order = i;
		upRow->nextRow->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(upRow->nextRow->text));
#endif
		upRow->nextRow->text.setScale(m_scale * m_fontSize);
		upRow->nextRow->pixSize.set(MUTILIST_MIN_WIDTH,m_caption.getTextSize().y * m_caption.getScale().x);
		upRow->nextRow->position.set(upRow->position.x + upRow->pixSize.x * m_scale.x,
			m_position.y + m_mouseRect.top * m_scale.y);
		upRow->nextRow->text.setPosition(upRow->nextRow->position.x,upRow->nextRow->position.y);
		if(!m_withoutTex)
		{
			upRow->nextRow->m_spriteTitle.init(m_mutiListTitle->texture.m_w,m_mutiListTitle->texture.m_h,1);	//设置标题背景的精灵
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(upRow->nextRow->m_spriteTitle));
#endif
			upRow->nextRow->m_spriteTitle.setPosition(upRow->nextRow->position);
			upRow->nextRow->m_spriteTitle.setScale(m_tableRow->pixSize.x * m_scale.x / m_mutiListTitle->textureSize.x,m_scale.y);
			upRow->nextRow->m_spriteTitle.setIsTransformCenter(POINT_LEFT_TOP);	//设置背景精灵的缩放为左上角对齐
			upRow->nextRow->m_spriteTitleEnd.init(m_mutiListTitleEnd->texture.m_w,m_mutiListTitleEnd->texture.m_h,1);
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(upRow->nextRow->m_spriteTitleEnd));
#endif
			upRow->nextRow->m_spriteTitleEnd.setPosition(upRow->nextRow->position.x + m_tableRow->pixSize.x * m_scale.x - 
				m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
				m_tableRow->position.y);
			upRow->nextRow->m_spriteTitleEnd.setScale(m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
				m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y);
			upRow->nextRow->m_spriteTitleEnd.setIsTransformCenter(POINT_LEFT_TOP);
		}

		upRow->nextRow->stringShowWidth = m_tableRow->pixSize.x * m_scale.x / m_curTextWidth;
		upRow->nextRow->title = "";
		setTitleStr("Title",i);	//设置默认显示
		upRow->nextRow->isShow = 1;
	}
}
XBool XMultiListBasic::setRowSum(int rowSum)		//设置列数，超出部分填空白，或者删除
{
	if(!m_isInited ||	//如果没有初始化直接退出
		rowSum < 0 ||			//非法数据直接范围失败
		rowSum > MUTILIST_MAX_ROW_SUM) return XFalse;	//超过列的最大值，则退出
	if(rowSum == m_tableRowSum) return XTrue;	//如果数据没有改变这里直接返回成功
	//下面根据实际情况修改控件中的数据
	if(rowSum > m_tableRowSum)
	{//添加，这样子添加的话，如果中途因为错误退出将会影响到逻辑的完整性
		int i,j;
		int oldRowSum = m_tableRowSum;
		m_tableRowSum = rowSum;
		//添加标题数据
		XMultiListOneRow *tempRow = m_tableRow;
		XMultiListOneRow *temp = NULL;
		if(oldRowSum == 0)
		{//全新添加
			m_tableRow = XMem::createMem<XMultiListOneRow>();
			if(m_tableRow == NULL) return XFalse;

			initANewRowData(NULL,0);

			tempRow = m_tableRow;
			for(i = 1;i < m_tableRowSum;++ i)
			{
				tempRow->nextRow = XMem::createMem<XMultiListOneRow>();
				if(tempRow->nextRow == NULL) return XFalse;

				initANewRowData(tempRow,i);

				tempRow = tempRow->nextRow;
				//注意这里没有检查内存分配失败
			}
			tempRow->nextRow = NULL;	//最后一个结尾

		}else
		{
			for(i = 0;i < m_tableRowSum;++ i)
			{
				if(i >= oldRowSum)
				{//插入新的列
					assert(temp != NULL);
					temp->nextRow = XMem::createMem<XMultiListOneRow>();
					if(temp->nextRow == NULL) return XFalse;

					initANewRowData(temp,i);

					temp = temp->nextRow;
					temp->nextRow = tempRow;
				}else
				{
					temp = tempRow;
					tempRow = tempRow->nextRow;
				}
			}
		}
		//添加表格数据
		XMultiListOneBox *tempBox = m_tableBox;
		XMultiListOneBox *temp1 = NULL;
		if(oldRowSum == 0)
		{//全新添加
			m_tableLineSum = 0;
			m_tableBox = NULL;
		}else
		{
			if(m_tableLineSum > 0)
			{
				for(i = 0;i < m_tableLineSum;++ i)
				{
					tempRow = m_tableRow;
					for(j = 0;j < m_tableRowSum;++ j)
					{
						if(j >= oldRowSum)
						{
							assert(temp1 != NULL);
							temp1->nextBox = XMem::createMem<XMultiListOneBox>();
							if(temp1->nextBox == NULL) return XFalse;

							temp1 = temp1->nextBox;
							temp1->isEnable = XTrue;

							temp1->isShow = XTrue;
							temp1->order.set(j,i);
							temp1->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
							XObjManager.decreaseAObject(&(temp1->text));
#endif
							temp1->text.setScale(m_scale * m_fontSize);
							temp1->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
							temp1->textStr = "";
							setBoxStr("Line",i,j);

							temp1->nextBox = tempBox;
							tempRow = tempRow->nextRow;
						}else
						{
							temp1 = tempBox;
							tempBox = tempBox->nextBox;
							tempRow = tempRow->nextRow;
						}
					}
				}
			}
		}
	}else
	{//删除
		int i,j;
		//删除标题数据
		XMultiListOneRow *tempRow = m_tableRow;
		XMultiListOneRow *temp = NULL;
		if(rowSum > 0)
		{
			for(i = 0;i < m_tableRowSum;++ i)
			{
				if(i >= rowSum)
				{//删除后面元素的内容
					temp->nextRow = tempRow->nextRow;
					//XMem::XDELETE_ARRAY(tempRow->title);
					XMem::XDELETE(tempRow);
					tempRow = temp->nextRow;
				}else
				{
					temp = tempRow;
					tempRow = tempRow->nextRow;
				}
			}
		}else
		{//全部删除
			tempRow = m_tableRow;
			for(i = 0;i < m_tableRowSum;++ i)
			{
				temp = tempRow;
				tempRow = tempRow->nextRow;
				//XMem::XDELETE_ARRAY(temp->title);
				XMem::XDELETE(temp);
			}
			m_tableRowSum = 0;
			m_tableRow = NULL;
		}
		//删除表格数据
		XMultiListOneBox *tempBox = m_tableBox;
		XMultiListOneBox *temp1 = NULL;
		if(rowSum > 0)
		{
			for(i = 0;i < m_tableLineSum;++ i)
			{
				for(j = 0;j < m_tableRowSum;++ j)
				{
					if(j >= rowSum)
					{//删除对于的元素
						//释放资源
						temp1->nextBox = tempBox->nextBox;
						//XMem::XDELETE_ARRAY(tempBox->string);
						XMem::XDELETE(tempBox);
						tempBox = temp1->nextBox;
					}else
					{
						temp1 = tempBox;
						tempBox = tempBox->nextBox;
					}
				}
			}
			//跟新相关的数值
			m_tableRowSum = rowSum;
		}else
		{
			tempBox = m_tableBox;
			for(i = 0;i < m_tableLineSum;++ i)
			{
				for(j = 0;j < m_tableRowSum;++ j)
				{
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					//XMem::XDELETE_ARRAY(temp1->string);
					XMem::XDELETE(temp1);
				}
			}
			m_tableLineSum = 0;
			m_tableBox = NULL;
		}
	}
	updateSliderState();
	updateShowChange();
	return XTrue;
}
void XMultiListBasic::initANewBoxData(XMultiListOneBox * curBox,XMultiListOneRow * curRow,int i,int j)
{
	curBox->isEnable = XTrue;

	curBox->isShow = XTrue;
	curBox->order.set(j,i);
	curBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&(curBox->text));
#endif
	curBox->text.setScale(m_scale * m_fontSize);
	curBox->text.setPosition(curRow->position.x,curRow->position.y + m_curTextHeight * (i + 1));
	curBox->textStr = "";
	setBoxStr("Line",i,j);
}
//在这个函数中需要设置进度条的值和选择的行值
XBool XMultiListBasic::setLineSum(int lineSum)	//设置行数，超出的部分填空白，或者删除
{
	if(!m_isInited) return XFalse;		//如果没有初始化直接退出
	if(lineSum < 0) return XFalse;			//非法数据直接范围失败
	if(m_tableRowSum <= 0) return XFalse;	//非法数据直接范围失败
	if(lineSum == m_tableLineSum) return XTrue;	//如果数据没有改变这里直接返回成功
	//对选择标记进行更新
	if(lineSum > 0)
	{
		XBool *tempFlag = XMem::createArrayMem<XBool>(lineSum);
		XBool *tempFlag1 = XMem::createArrayMem<XBool>(lineSum);
		XBool *tempFlag2 = XMem::createArrayMem<XBool>(lineSum);
		if(tempFlag == NULL || tempFlag1 == NULL || tempFlag2 == NULL) 
		{
			XMem::XDELETE_ARRAY(tempFlag);
			XMem::XDELETE_ARRAY(tempFlag1);
			XMem::XDELETE_ARRAY(tempFlag2);
			return XFalse;
		}

		for(int i = 0;i < min(lineSum,m_tableLineSum);++ i)
		{
			tempFlag[i] = m_haveSelectFlag[i];
			tempFlag1[i] = m_check0State[i];
			tempFlag2[i] = m_check1State[i];
		}
		for(int i = min(lineSum,m_tableLineSum);i < max(lineSum,m_tableLineSum);++ i)
		{
			tempFlag[i] = XFalse;
			tempFlag1[i] = XFalse;
			tempFlag2[i] = XFalse;
		}
		XMem::XDELETE_ARRAY(m_haveSelectFlag);
		XMem::XDELETE_ARRAY(m_check0State);
		XMem::XDELETE_ARRAY(m_check1State);
		m_haveSelectFlag = tempFlag;
		m_check0State = tempFlag1;
		m_check1State = tempFlag2;
	}else
	{
		XMem::XDELETE_ARRAY(m_haveSelectFlag);
		XMem::XDELETE_ARRAY(m_check0State);
		XMem::XDELETE_ARRAY(m_check1State);
		m_haveSelectFlag = NULL;
		m_check0State = NULL;
		m_check1State = NULL;
	}

	int i,j;
	//下面根据实际情况修改控件中的数据
	if(lineSum > m_tableLineSum)
	{//添加
		int oldLine = m_tableLineSum;
		m_tableLineSum = lineSum;
		XMultiListOneBox *tempBox = m_tableBox;
		XMultiListOneRow *tempRow;
		if(oldLine == 0)
		{//全新添加
			m_tableBox = XMem::createMem<XMultiListOneBox>();
			if(m_tableBox == NULL) return XFalse;

			initANewBoxData(m_tableBox,m_tableRow,0,0);

			tempBox = m_tableBox;
			for(i = 0;i < m_tableLineSum;++ i)
			{
				tempRow = m_tableRow;
				for(j = 0;j < m_tableRowSum;++ j)
				{
					if(j == 0 && i == 0) 
					{
						tempRow = tempRow->nextRow;
						continue;
					}
					tempBox->nextBox = XMem::createMem<XMultiListOneBox>();
					if(tempBox->nextBox == NULL) return XFalse;

					tempBox = tempBox->nextBox;
					initANewBoxData(tempBox,tempRow,i,j);

					tempRow = tempRow->nextRow;
					//注意这里没有检查内存分配失败
				}
			}
			tempBox->nextBox = NULL;	//最后一个结尾
		}else
		{//添加
			for(i = 0;i < m_tableLineSum;++ i)
			{
				tempRow = m_tableRow;
				for(j = 0;j < m_tableRowSum;++ j)
				{
					if(i >= oldLine || (i == oldLine - 1 && j == m_tableRowSum - 1))	//最后一个元素也要这么处理
					{
						if(i == m_tableLineSum - 1 && j == m_tableRowSum - 1)
						{//最后一个元素
							continue;
						}
						tempBox->nextBox = XMem::createMem<XMultiListOneBox>();
						if(tempBox->nextBox == NULL) return XFalse;

						tempBox = tempBox->nextBox;
						tempBox->isEnable = XTrue;

						tempBox->isShow = XTrue;
						tempBox->order.set(j,i);
						tempBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
						XObjManager.decreaseAObject(&(tempBox->text));
#endif
						tempBox->text.setScale(m_scale * m_fontSize);
						tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
						tempBox->textStr = "";
						if(j + 1 >= m_tableRowSum)
						{
							setBoxStr("Line",i + 1,0);
						}else
						{
							setBoxStr("Line",i,j + 1);
						}

						tempRow = tempRow->nextRow;
						//注意这里没有检查内存分配失败
					}else
					{
						tempBox = tempBox->nextBox;
						tempRow = tempRow->nextRow;
					}
				}
			}
			tempBox->nextBox = NULL;	//最后一个结尾
		}
	}else
	{//删除
		XMultiListOneBox *tempBox = m_tableBox;
		XMultiListOneBox *temp1 = NULL;
		if(lineSum == 0)
		{//全部删除
			for(i = 0;i < m_tableLineSum;++ i)
			{
				for(j = 0;j < m_tableRowSum;++ j)
				{//删除这个元素
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					//XMem::XDELETE_ARRAY(temp1->string);
					XMem::XDELETE(temp1);
				}
			}
			m_tableLineSum = lineSum;
			m_tableBox = NULL;
		}else
		{
			for(i = 0;i < m_tableLineSum;++ i)
			{
				for(j = 0;j < m_tableRowSum;++ j)
				{
					if(i >= lineSum)
					{//删除这个元素
						temp1->nextBox = tempBox->nextBox;
						//XMem::XDELETE_ARRAY(tempBox->string);
						XMem::XDELETE(tempBox);
						tempBox = temp1->nextBox;
					}else
					{
						temp1 = tempBox;
						tempBox = tempBox->nextBox;
					}
				}
			}
			m_tableLineSum = lineSum;
		}
	}
	updateSliderState();
	updateShowChange();
	return XTrue;
}
XBool XMultiListBasic::deleteLine(int order)		//删除某一行
{
	if(!m_isInited) return XFalse;							//如果没有初始化直接退出
	if(order < 0 || order >= m_tableLineSum) return XFalse;		//非法的数据
	//对选择标记进行更新
	if(m_tableLineSum > 1)
	{
		XBool *tempFlag = XMem::createArrayMem<XBool>(m_tableLineSum - 1);
		XBool *tempFlag1 = XMem::createArrayMem<XBool>(m_tableLineSum - 1);
		XBool *tempFlag2 = XMem::createArrayMem<XBool>(m_tableLineSum - 1);
		if(tempFlag == NULL || tempFlag1 == NULL || tempFlag2 == NULL) 
		{
			XMem::XDELETE_ARRAY(tempFlag);
			XMem::XDELETE_ARRAY(tempFlag1);
			XMem::XDELETE_ARRAY(tempFlag2);
			return XFalse;
		}
		for(int i = 0;i < m_tableLineSum - 1;++ i)
		{
			if(i < order) 
			{
				tempFlag[i] = m_haveSelectFlag[i];
				tempFlag1[i] = m_check0State[i];
				tempFlag2[i] = m_check1State[i];
			}else 
			{
				tempFlag[i] = m_haveSelectFlag[i + 1];
				tempFlag1[i] = m_check0State[i + 1];
				tempFlag2[i] = m_check1State[i + 1];
			}
		}
		XMem::XDELETE_ARRAY(m_haveSelectFlag);
		XMem::XDELETE_ARRAY(m_check0State);
		XMem::XDELETE_ARRAY(m_check1State);
		m_haveSelectFlag = tempFlag;
		m_check0State = tempFlag1;
		m_check1State = tempFlag2;
	}else
	{//全部删除完儿了
		XMem::XDELETE_ARRAY(m_haveSelectFlag);
		XMem::XDELETE_ARRAY(m_check0State);
		XMem::XDELETE_ARRAY(m_check1State);
		m_haveSelectFlag = NULL;
		m_check0State = NULL;
		m_check1State = NULL;
	}
	int i,j;
	XMultiListOneRow *tempRow = m_tableRow;
	XMultiListOneBox *tempBox = m_tableBox;
	XMultiListOneBox *temp1 = NULL;
	if(order == 0)
	{//行头,删除第一行
		for(i = 0;i < m_tableRowSum;++ i)
		{
			temp1 = tempBox->nextBox;
			//XMem::XDELETE_ARRAY(tempBox->string);
			XMem::XDELETE(tempBox);
			tempBox = temp1;
		}
		m_tableBox = tempBox;
		//改变后面单元格的位置
		if(m_tableLineSum > 1)
		{
			tempBox = m_tableBox;
			m_tableBox->order.set(0,0);
			m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_curTextHeight * 1);
			for(i = 0;i < m_tableLineSum - 1;++ i)
			{
				tempRow = m_tableRow;
				for(j = 0;j < m_tableRowSum;++ j)
				{
					if(i == 0 && j == 0)
					{
						tempBox = tempBox->nextBox;
						tempRow = tempRow->nextRow;
						continue;
					}
					tempBox->order.set(i,j);
					tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * 1);
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}
			}
		}
	}else
	{
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(i == order)
				{//符合删除要求
					temp1->nextBox = tempBox->nextBox;
					//XMem::XDELETE_ARRAY(tempBox->string);
					XMem::XDELETE(tempBox);
					tempBox = temp1->nextBox;
				}else
				if(i > order)
				{//设置后面单元格的位置
					if(temp1->nextBox != NULL)
					{
						temp1->nextBox->order.set(j,i);
						temp1->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
					}
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}else
				{
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}
			}
		}
	}
	-- m_tableLineSum;
	if(m_haveSelect)
	{//存在选择
		if(m_selectLineOrder == order)
		{//正好删掉的是选择的那一行，则取消选择
			m_haveSelect = XFalse;
			updateSelectLine();
		}else
		if(m_selectLineOrder > order)
		{
			-- m_selectLineOrder;
			updateSelectLine();
		}
	}
	updateSliderState();
	updateShowChange();
	return XTrue;
}
XBool XMultiListBasic::deleteRow(int order)		//删除某一列
{
	if(!m_isInited) return XFalse;							//如果没有初始化直接退出
	if(order < 0 || order >= m_tableRowSum) return XFalse;		//非法的数据
	if(order == 0)
	{
		int i,j;
		//删除表头
		XMultiListOneRow *tempRow = m_tableRow;
		XMultiListOneRow *temp = tempRow->nextRow;
		//XMem::XDELETE_ARRAY(tempRow->title);
		XMem::XDELETE(tempRow);
		if(m_tableRowSum == 1)
		{//如果只有一列
			m_tableRow = NULL;
		}else
		{
			m_tableRow = temp;
			//设置标题的位置
			m_tableRow->order = 0;
			m_tableRow->position.set(m_position.x + (m_mouseRect.left + MUTILIST_MIN_WIDTH * 0) * m_scale.x + MUTILIST_MIN_WIDTH * m_scale.x * 0,
				m_position.y + m_mouseRect.top * m_scale.y);
			m_tableRow->text.setPosition(m_tableRow->position.x,m_tableRow->position.y);
			if(!m_withoutTex)
			{
				m_tableRow->m_spriteTitle.setPosition(m_tableRow->position);
				m_tableRow->m_spriteTitleEnd.setPosition(m_tableRow->position.x + m_tableRow->pixSize.x * m_scale.x - 
					m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
					m_tableRow->position.y);
			}
			for(i = 1;i < m_tableRowSum - 1;++ i)	//已经删除了一个元素
			{
				temp->nextRow->order = i;
				temp->nextRow->position.set(temp->position.x + temp->pixSize.x * m_scale.x,
					m_position.y + m_mouseRect.top * m_scale.y);
				temp->nextRow->text.setPosition(temp->nextRow->position.x,temp->nextRow->position.y);
				if(!m_withoutTex)
				{
					temp->nextRow->m_spriteTitleEnd.setPosition(temp->nextRow->position.x + m_tableRow->pixSize.x * m_scale.x - 
						m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
						m_tableRow->position.y);
				}
				temp = temp->nextRow;
			}
		}
		//删除表格元素
		XMultiListOneBox *tempBox = m_tableBox;
		XMultiListOneBox *temp1 = NULL;

		tempBox = m_tableBox;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(i == 0 && j == 0)
				{//第一个元素的删除额外处理，其它按规则处理
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
				//	tempRow = tempRow->nextRow;
					continue;
				}else
				if(j == order)
				{
					temp1->nextBox = tempBox->nextBox;
					//XMem::XDELETE_ARRAY(tempBox->string);
					XMem::XDELETE(tempBox);
					tempBox = temp1->nextBox;
				//	tempRow = tempRow->nextRow;
				}else
				{
				//	if(i == m_tableLineSum - 1 && j == m_tableRowSum - 1)
				//	{//最后一个元素退出
				//		break;
				//	}
					if(tempBox != NULL)
					{
						if(m_tableRowSum > 1)
						{
							tempBox->order.set(j,i);
							tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
							tempRow = tempRow->nextRow;
						}
						temp1 = tempBox;
						tempBox = tempBox->nextBox;
					}
				}
			}
		}
		//删除第一个元素
		tempBox = m_tableBox;
		temp1 = tempBox->nextBox;
		//XMem::XDELETE_ARRAY(tempBox->string);
		XMem::XDELETE(tempBox);
		if(m_tableRowSum == 1)
		{//如果只有一列，则删除之后没有单位表格
			m_tableBox = NULL;
			m_tableLineSum = 0;
		}else
		{
			assert(temp1 != NULL);
			tempBox = temp1;
			m_tableBox = tempBox;
			m_tableBox->order.set(0,0);
			m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_curTextHeight * 0);
		}
	}else
	{
		int i,j;
		//删除表头
		XMultiListOneRow *tempRow = m_tableRow;
		XMultiListOneRow *temp = tempRow->nextRow;
		for(i = 0;i < m_tableRowSum;++ i)
		{
			if(i == order)
			{
				temp->nextRow = tempRow->nextRow;
				//XMem::XDELETE_ARRAY(tempRow->title);
				XMem::XDELETE(tempRow);
				tempRow = temp->nextRow;
				if(tempRow != NULL)
				{
					tempRow->order = i;
					tempRow->position.set(temp->position.x + temp->pixSize.x * m_scale.x,
						m_position.y + m_mouseRect.top * m_scale.y);
					tempRow->text.setPosition(temp->nextRow->position.x,temp->nextRow->position.y);
					if(!m_withoutTex)
					{
						tempRow->m_spriteTitleEnd.setPosition(temp->nextRow->position.x + m_tableRow->pixSize.x * m_scale.x - 
							m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
							m_tableRow->position.y);
					}
				}
			}else
			if(i > order)
			{
				tempRow->order = i;
				tempRow->position.set(temp->position.x + temp->pixSize.x * m_scale.x,
					m_position.y + m_mouseRect.top * m_scale.y);
				tempRow->text.setPosition(temp->nextRow->position.x,temp->nextRow->position.y);
				if(!m_withoutTex)
				{
					tempRow->m_spriteTitleEnd.setPosition(temp->nextRow->position.x + m_tableRow->pixSize.x * m_scale.x - 
						m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
						m_tableRow->position.y);
				}
				temp = tempRow;
				tempRow = tempRow->nextRow;
			}else
			{
				temp = tempRow;
				tempRow = tempRow->nextRow;
			}
		}
		//删除表格元素
		XMultiListOneBox *tempBox = m_tableBox;
		XMultiListOneBox *temp1 = NULL;

		tempBox = m_tableBox;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(j == order)
				{
					assert(temp1 != NULL);
					temp1->nextBox = tempBox->nextBox;
					//XMem::XDELETE_ARRAY(tempBox->string);
					XMem::XDELETE(tempBox);
					tempBox = temp1->nextBox;
					//	tempRow = tempRow->nextRow;
				}else
				{
				//	if(i == m_tableLineSum - 1 && j == m_tableRowSum - 1)
				//	{//最后一个元素退出
				//		break;
				//	}
					if(tempBox != NULL)
					{
						tempBox->order.set(j,i);
						tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
						temp1 = tempBox;
						tempBox = tempBox->nextBox;
						tempRow = tempRow->nextRow;
					}
				}
			}
		}
	}
	m_tableRowSum--;
	updateSliderState();
	updateShowChange();
	return XTrue;
}
XBool XMultiListBasic::insertALine(int order)		//在order处插入一行
{
	if(!m_isInited) return XFalse;
	if(order < 0 || order >= m_tableLineSum) return XFalse;
	//对选择标记进行更新
	XBool *tempFlag = XMem::createArrayMem<XBool>(m_tableLineSum + 1);
	XBool *tempFlag1 = XMem::createArrayMem<XBool>(m_tableLineSum + 1);
	XBool *tempFlag2 = XMem::createArrayMem<XBool>(m_tableLineSum + 1);

	if(tempFlag == NULL || tempFlag1 == NULL || tempFlag2 == NULL)
	{
		XMem::XDELETE_ARRAY(tempFlag);
		XMem::XDELETE_ARRAY(tempFlag1);
		XMem::XDELETE_ARRAY(tempFlag2);
		return XFalse;
	}
	for(int i = 0;i < m_tableLineSum + 1;++ i)
	{
		if(i < order) 
		{
			tempFlag[i] = m_haveSelectFlag[i]; 
			tempFlag1[i] = m_check0State[i]; 
			tempFlag2[i] = m_check1State[i]; 
		}else
		if(i == order) 
		{
			tempFlag[i] = XFalse; 
			tempFlag1[i] = XFalse; 
			tempFlag2[i] = XFalse; 
		}else
		{
			tempFlag[i] = m_haveSelectFlag[i + 1];
			tempFlag1[i] = m_check0State[i + 1];
			tempFlag2[i] = m_check1State[i + 1];
		}
	}
	XMem::XDELETE_ARRAY(m_haveSelectFlag);
	XMem::XDELETE_ARRAY(m_check0State);
	XMem::XDELETE_ARRAY(m_check1State);
	m_haveSelectFlag = tempFlag;
	m_check0State = tempFlag1;
	m_check1State = tempFlag2;

	int i,j;
	if(order == 0)
	{//向头部插入
		XMultiListOneBox *tempBox = m_tableBox;
		XMultiListOneRow *tempRow = m_tableRow;
		XMultiListOneBox *temp1 = m_tableBox;
		m_tableBox = XMem::createMem<XMultiListOneBox>();
		if(m_tableBox == NULL) return XFalse;

		m_tableBox->isEnable = XTrue;
		m_tableBox->isShow = XTrue;
		m_tableBox->order.set(0,0);
		m_tableBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableBox->text));
#endif
		m_tableBox->text.setScale(m_scale * m_fontSize);
		m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_curTextHeight * 1);
		m_tableBox->textStr = "";
		setBoxStr("Line",0,0);
		tempBox = m_tableBox;
		for(i = 0;i < m_tableRowSum - 1;++ i)
		{
			tempBox->nextBox = XMem::createMem<XMultiListOneBox>();
			if(tempBox->nextBox == NULL) return XFalse;

			tempBox->nextBox->isEnable = XTrue;
			tempBox->nextBox->isShow = XTrue;
			tempBox->nextBox->order.set(i,0);
			tempBox->nextBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(tempBox->nextBox->text));
#endif
			tempBox->nextBox->text.setScale(m_scale * m_fontSize);
			tempBox->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * 1);
			tempBox->nextBox->textStr = "";
			setBoxStr("Line",0,i + 1);
			tempBox = tempBox->nextBox;
			tempRow = tempRow->nextRow;
		}
		tempBox->nextBox = temp1;
		//将后面的所有元素向下移动
		tempBox = m_tableBox;

		++ m_tableLineSum;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(i == 0 && j == 0)
				{
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
					continue;
				}
				tempBox->order.set(j,i);
				tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
				tempBox = tempBox->nextBox;
				tempRow = tempRow->nextRow;
			}
		}

		if(m_haveSelect &&m_selectLineOrder >= order)
		{
			++ m_selectLineOrder;
			updateSelectLine();
		}
	}else
	{
		XMultiListOneRow *tempRow = m_tableRow;
		XMultiListOneBox *tempBox = m_tableBox;
		XMultiListOneBox *temp1 = NULL;

		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(i == order)
				{
					temp1->nextBox = XMem::createMem<XMultiListOneBox>();
					if(temp1->nextBox == NULL) return XFalse;

					temp1->nextBox->isEnable = XTrue;
					temp1->nextBox->isShow = XTrue;
					temp1->nextBox->order.set(j,i);
					temp1->nextBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
					XObjManager.decreaseAObject(&(temp1->nextBox->text));
#endif
					temp1->nextBox->text.setScale(m_scale * m_fontSize);
					temp1->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
					temp1->nextBox->textStr = "";
					setBoxStr("Line",i,j);
					temp1 = temp1->nextBox;
					temp1->nextBox = tempBox;
				//	if(j == m_tableRowSum - 1)
				//	{//最后一个添加的元素
				//		temp1->nextBox = tempBox;
				//	}
					tempRow = tempRow->nextRow;
				}else
				if(i > order)
				{//改变位置
					if(temp1->nextBox != NULL)
					{
						temp1->nextBox->order.set(j,i);
						temp1->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
					}
					temp1 = tempBox;
					tempBox = tempBox->nextBox;	
					tempRow = tempRow->nextRow;
				}else
				{
					temp1 = tempBox;
					tempBox = tempBox->nextBox;	
					tempRow = tempRow->nextRow;
				}
			}
		}
		++ m_tableLineSum;
		if(m_haveSelect)
		{
			if(m_selectLineOrder >= order)
			{
				++ m_selectLineOrder;
				updateSelectLine();
			}
		}
	}
	updateSliderState();
	updateShowChange();
	return XTrue;
}
XBool XMultiListBasic::insertARow(int order)		//在order处插入一列
{
	if(!m_isInited) return XFalse;
	if(order < 0 || order >= m_tableRowSum) return XFalse;
	if(m_tableRowSum >= MUTILIST_MAX_ROW_SUM) return XFalse;	//超过列的最大值，则退出
	int i,j;
	XMultiListOneRow *tempRow = m_tableRow;
	XMultiListOneRow *temp = NULL;
	XMultiListOneBox *tempBox = m_tableBox;
	XMultiListOneBox *temp1 = NULL;
	if(order == 0)
	{//列头
		//改变标题栏
		tempRow = m_tableRow;
		m_tableRow = XMem::createMem<XMultiListOneRow>();
		if(m_tableRow == NULL) return XFalse;

		initANewRowData(NULL,0);
/*		m_tableRow->isEnable = 1;

		m_tableRow->order = 0;
		m_tableRow->text.setACopy(m_caption);
		m_tableRow->text.setScale(m_scale * m_fontSize);
		m_tableRow->pixSize.set(MUTILIST_MIN_WIDTH,m_caption.getTextSize().y * m_caption.getSize().x);
		m_tableRow->position.set(m_objRect.left + m_edgeDistance.left * m_scale.x + MUTILIST_MIN_WIDTH * m_scale.x * 0,
			m_objRect.top + m_edgeDistance.top * m_scale.y);
		m_tableRow->text.setPosition(m_tableRow->position.x,m_tableRow->position.y);
		m_tableRow->m_spriteTitle.init(1,m_mutiListTitle->m_w,m_mutiListTitle->m_h,1);	//设置标题背景的精灵
		m_tableRow->m_spriteTitle.setPosition(m_tableRow->position);
		m_tableRow->m_spriteTitle.setScale(m_tableRow->pixSize.x / m_mutiListTitle->m_w,1.0);
		m_tableRow->m_spriteTitle.setIsTransformCenter(POINT_LEFT_TOP);	//设置背景精灵的缩放为左上角对齐
		m_tableRow->m_spriteTitleEnd.init(1,m_mutiListTitleEnd->m_w,m_mutiListTitleEnd->m_h,1);
		m_tableRow->m_spriteTitleEnd.setPosition(m_tableRow->position.x + m_tableRow->pixSize.x - m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y,m_tableRow->position.y);
		m_tableRow->m_spriteTitleEnd.setScale(m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y,m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y);
		m_tableRow->m_spriteTitleEnd.setIsTransformCenter(POINT_LEFT_TOP);

		m_tableRow->stringShowWidth = m_tableRow->pixSize.x / (m_tableRow->text.m_scale.x * m_tableRow->text.m_showSize.x * 0.5);
		m_tableRow->title = NULL;
		setTileStr("Title",0);	//设置默认显示
		m_tableRow->isShow = 1;*/

		m_tableRow->nextRow = tempRow;
		tempRow = m_tableRow;
		for(i = 0;i < m_tableRowSum;++ i)
		{
			tempRow->nextRow->order = i + 1;
			tempRow->nextRow->position.set(tempRow->position.x + tempRow->pixSize.x * m_scale.x,
				m_position.y + m_mouseRect.top * m_scale.y);
			tempRow->nextRow->text.setPosition(tempRow->nextRow->position.x,tempRow->nextRow->position.y);
			if(!m_withoutTex)
			{
				tempRow->nextRow->m_spriteTitle.setPosition(tempRow->nextRow->position);
				tempRow->nextRow->m_spriteTitleEnd.setPosition(tempRow->nextRow->position.x + m_tableRow->pixSize.x * m_scale.x - 
					m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
					m_tableRow->position.y);
			}
			tempRow = tempRow->nextRow;
		}
		//改变表格内容
		++ m_tableRowSum;
		//插入第一个元素
		tempBox = m_tableBox;
		m_tableBox = XMem::createMem<XMultiListOneBox>();
		if(m_tableBox == NULL) return XFalse;

		m_tableBox->isEnable = XTrue;
		m_tableBox->isShow = XTrue;
		m_tableBox->order.set(0,0);
		m_tableBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableBox->text));
#endif
		m_tableBox->text.setScale(m_scale * m_fontSize);
		m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_curTextHeight * 1);
		m_tableBox->textStr = "";
		setBoxStr("Line",0,0);
		m_tableBox->nextBox = tempBox;
		tempBox = m_tableBox;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;j++)
			{
				if(i == 0 && j == 0)
				{//第一个元素暂时不处理
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}else
				if(j == order)
				{//插入
					temp1->nextBox = XMem::createMem<XMultiListOneBox>();
					if(temp1->nextBox == NULL) return XFalse;

					temp1->nextBox->isEnable = XTrue;
					temp1->nextBox->isShow = XTrue;
					temp1->nextBox->order.set(j,i);
					temp1->nextBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
					XObjManager.decreaseAObject(&(temp1->nextBox->text));
#endif
					temp1->nextBox->text.setScale(m_scale * m_fontSize);
					temp1->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
					temp1->nextBox->textStr = "";
					setBoxStr("Line",i,j);
					temp1 = temp1->nextBox;
					temp1->nextBox = tempBox;
					tempRow = tempRow->nextRow;
				}else
				if(j > order)
				{//移动
					temp1->nextBox->order.set(j,i);
					temp1->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}else
				{
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}
			}
		}
	}else
	{
		//改变标题栏
		tempRow = m_tableRow;
		for(i = 0;i < m_tableRowSum + 1;++ i)
		{
			if(i == order)
			{//插入
				temp->nextRow = XMem::createMem<XMultiListOneRow>();
				if(temp->nextRow == NULL) return XFalse;

				initANewRowData(temp,i);
			/*	temp->nextRow->isEnable = 1;
				temp->nextRow->order = i;
				temp->nextRow->text.setACopy(m_caption);
				temp->nextRow->pixSize.set(MUTILIST_MIN_WIDTH * m_scale.x,m_tableRow->text.m_scale.y * m_tableRow->text.m_showSize.x);
				temp->nextRow->position.set(temp->position.x + temp->pixSize.x,
					m_objRect.top + m_edgeDistance.top * m_scale.y);
				temp->nextRow->text.setPosition(temp->nextRow->position.x,temp->nextRow->position.y);
				temp->nextRow->m_spriteTitle.init(1,m_mutiListTitle->m_w,m_mutiListTitle->m_h,1);	//设置标题背景的精灵
				temp->nextRow->m_spriteTitle.setPosition(temp->nextRow->position);
				temp->nextRow->m_spriteTitle.setScale(m_tableRow->pixSize.x / m_mutiListTitle->m_w,1.0);
				temp->nextRow->m_spriteTitle.setIsTransformCenter(POINT_LEFT_TOP);	//设置背景精灵的缩放为左上角对齐
				temp->nextRow->m_spriteTitleEnd.init(1,m_mutiListTitleEnd->m_w,m_mutiListTitleEnd->m_h,1);
				temp->nextRow->m_spriteTitleEnd.setPosition(temp->nextRow->position.x + m_tableRow->pixSize.x - m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y,
					m_tableRow->position.y);
				temp->nextRow->m_spriteTitleEnd.setScale(m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y,m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y);
				temp->nextRow->m_spriteTitleEnd.setIsTransformCenter(POINT_LEFT_TOP);

				temp->nextRow->stringShowWidth = m_tableRow->pixSize.x / (m_tableRow->text.m_scale.x * m_tableRow->text.m_showSize.x * 0.5);
				temp->nextRow->title = NULL;
				setTileStr("Title",i);	//设置默认显示
				temp->nextRow->isShow = 1;*/

				temp = temp->nextRow;
				temp->nextRow = tempRow;
			}else
			if(i > order)
			{
				temp->nextRow->order = i;
				temp->nextRow->position.set(temp->position.x + temp->pixSize.x * m_scale.x,
					m_position.y + m_mouseRect.top * m_scale.y);
				temp->nextRow->text.setPosition(temp->nextRow->position.x,temp->nextRow->position.y);
				if(!m_withoutTex)
				{
					temp->nextRow->m_spriteTitle.setPosition(temp->nextRow->position);
					temp->nextRow->m_spriteTitleEnd.setPosition(temp->nextRow->position.x + m_tableRow->pixSize.x * m_scale.x -
						m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
						m_tableRow->position.y);
				}
				temp = tempRow;
				tempRow = tempRow->nextRow;
			}else
			{
				temp = tempRow;
				tempRow = tempRow->nextRow;
			}
		}
		//改变表格内容
		++ m_tableRowSum;
		tempBox = m_tableBox;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;j++)
			{
				if(i == 0 && j == 0)
				{//第一个元素暂时不处理
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}else
				if(j == order)
				{//插入
					temp1->nextBox = XMem::createMem<XMultiListOneBox>();
					if(temp1->nextBox == NULL) return XFalse;

					temp1->nextBox->isEnable = XTrue;
					temp1->nextBox->isShow = XTrue;
					temp1->nextBox->order.set(j,i);
					temp1->nextBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
					XObjManager.decreaseAObject(&(temp1->nextBox->text));
#endif
					temp1->nextBox->text.setScale(m_scale * m_fontSize);
					temp1->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
					temp1->nextBox->textStr = "";
					setBoxStr("Line",i,j);
					temp1 = temp1->nextBox;
					temp1->nextBox = tempBox;
					tempRow = tempRow->nextRow;
				}else
				if(j > order)
				{//移动
					temp1->nextBox->order.set(j,i);
					temp1->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}else
				{
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}
			}
		}
	}
	updateSliderState();
	updateShowChange();
	return XTrue;
}
XBool XMultiListBasic::moveDownLine(int order)	//将order行下移
{
	if(!m_isInited) return XFalse;
	if(order < 0 || order >= m_tableLineSum - 1) return XFalse;	//只有一行不能移动，数据超出范围也不能移动
	XMath::swapTwoSum(m_haveSelectFlag[order],m_haveSelectFlag[order + 1]);
	XMath::swapTwoSum(m_check0State[order],m_check0State[order + 1]);
	XMath::swapTwoSum(m_check1State[order],m_check1State[order + 1]);
	//下面开始移动
	XMultiListOneBox *tempBox = m_tableBox;
	XMultiListOneBox *temp1 = NULL;
	XMultiListOneBox temp2;
	//2行的6个端点的节点
	XMultiListOneBox * point0 = NULL;
	XMultiListOneBox * point1 = NULL;
	XMultiListOneBox * point2 = NULL;
	XMultiListOneBox * point3 = NULL;
	XMultiListOneBox * point4 = NULL;
	XMultiListOneBox * point5 = NULL;
	int i,j;
	if(order == 0)
	{//移动的是第一行
		temp1 = m_tableBox;
		point1 = m_tableBox;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(i == order)
				{
					if(j == m_tableRowSum - 1)
					{
						point2 = tempBox;
						point3 = tempBox->nextBox;
					}
					tempBox = tempBox->nextBox;
				}else
				if(i == order + 1)
				{
					temp2.order = tempBox->order;
					temp2.text.setPosition(tempBox->text.getPosition());
					tempBox->order = temp1->order;
					tempBox->text.setPosition(temp1->text.getPosition());
					temp1->order = temp2.order;
					temp1->text.setPosition(temp2.text.getPosition());
					if(j == m_tableRowSum - 1)
					{
						point4 = tempBox;
						point5 = tempBox->nextBox;
					}
					temp1 = temp1->nextBox;
					tempBox = tempBox->nextBox;
				}else
				if(i > order + 1)
				{//数据交换完成
					break;
				}else
				{
					tempBox = tempBox->nextBox;
				}
			}
		}
		//point0->nextBox = point3;
		assert(point2 != NULL);
		point2->nextBox = point5;
		assert(point4 != NULL);
		point4->nextBox = point1;
		m_tableBox = point3;
	}else
	{
		for(i = 0;i < m_tableLineSum;++ i)
		{
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(i == order - 1 && j == m_tableRowSum - 1)
				{
					point0 = tempBox;
					point1 = tempBox->nextBox;
				}
				if(i == order)
				{
					temp1 = tempBox;
					if(j == m_tableRowSum - 1)
					{
						point2 = tempBox;
						point3 = tempBox->nextBox;
					}
					tempBox = tempBox->nextBox;
				}else
				if(i == order + 1)
				{
					temp2.order = tempBox->order;
					temp2.text.setPosition(tempBox->text.getPosition());
					tempBox->order = temp1->order;
					tempBox->text.setPosition(temp1->text.getPosition());
					temp1->order = temp2.order;
					temp1->text.setPosition(temp2.text.getPosition());
					if(j == m_tableRowSum - 1)
					{
						point4 = tempBox;
						point5 = tempBox->nextBox;
					}
					temp1 = temp1->nextBox;
					tempBox = tempBox->nextBox;
				}else
				if(i > order + 1)
				{//数据交换完成
					break;
				}else
				{
					tempBox = tempBox->nextBox;
				}
			}
		}
		assert(point0 != NULL);
		assert(point2 != NULL);
		assert(point4 != NULL);
		point0->nextBox = point3;
		point2->nextBox = point5;
		point4->nextBox = point1;
	}
	if(m_haveSelect)
	{
		if(m_selectLineOrder == order)
		{
			++ m_selectLineOrder;
			updateSelectLine();
		}else
		if(m_selectLineOrder == order + 1)
		{
			-- m_selectLineOrder;
			updateSelectLine();
		}
	}
	updateSliderState();
	updateShowChange();
	return XTrue;
}
XBool XMultiListBasic::moveRightRow(int order)	//将order列右移
{
	if(!m_isInited) return XFalse;
	if(order < 0 || order >= m_tableRowSum - 1) return XFalse;	//只有一列不能移动，数据超出范围也不能移动
	int i = 0,j = 0;
	XMultiListOneRow *tempRow = m_tableRow;
	XMultiListOneRow *temp1 = NULL;
	XMultiListOneRow *point0 = NULL;
	XMultiListOneRow *point1 = NULL;

	XMultiListOneBox *tempBox = m_tableBox;
	XMultiListOneBox *temp2 = NULL;
	XMultiListOneBox *point2 = NULL;
	XMultiListOneBox *point3 = NULL;
	if(order == 0)
	{
		//移动表头
	//	point0 = tempRow;
	//	tempRow = tempRow->nextRow;
		temp1 = tempRow->nextRow;
		point1 = temp1->nextRow;

	//	point0->nextRow = temp1;
		m_tableRow = temp1;
		temp1->order = tempRow->order;
		temp1->position = tempRow->position;
		temp1->text.setPosition(temp1->position.x,temp1->position.y);
		temp1->nextRow = tempRow;
		temp1->nextRow->order = temp1->order + 1;
		temp1->nextRow->position.set(temp1->position.x + temp1->pixSize.x * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y);			
		temp1->nextRow->text.setPosition(temp1->nextRow->position.x,temp1->nextRow->position.y);
		if(!m_withoutTex)
		{
			temp1->m_spriteTitle.setPosition(temp1->position);
			temp1->m_spriteTitleEnd.setPosition(temp1->position.x + temp1->pixSize.x * m_scale.x - 
				m_mutiListTitleEnd->textureSize.x * temp1->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,temp1->position.y);
			temp1->nextRow->m_spriteTitle.setPosition(temp1->nextRow->position);
			temp1->nextRow->m_spriteTitleEnd.setPosition(temp1->position.x + temp1->nextRow->pixSize.x * m_scale.x - 
				m_mutiListTitleEnd->textureSize.x * temp1->nextRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
				temp1->nextRow->position.y);
		}
		temp1->nextRow->nextRow = point1;

		//移动表体
		if(m_tableLineSum > 1)
		{
			for(i = 0;i < m_tableLineSum;++ i)
			{
				tempRow = m_tableRow;
				for(j = 0;j < m_tableRowSum;++ j)
				{
					if(j == m_tableRowSum - 1)
					{
						point2 = tempBox;
						tempBox = tempBox->nextBox;
						temp2 = tempBox->nextBox;
						point3 = temp2->nextBox;

						point2->nextBox = temp2;
						temp2->order = tempBox->order;
						temp2->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 2));
						temp2->nextBox = tempBox;
						if(j == m_tableRowSum -1)
						{
							tempRow = m_tableRow;
							temp2->nextBox->order.set(0,temp2->order.y);
							j = 0;
							++ i;
						}else
						{//这种情况是不会出现的
							tempRow = tempRow->nextRow;
							temp2->nextBox->order.set(temp2->order.x + 1,temp2->order.y);
							++ j;
						}
						temp2->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 2));
						temp2->nextBox->nextBox = point3;
						if(i >= m_tableLineSum - 1)
						{
							break;
						}
					}else
					{
						tempRow = tempRow->nextRow;
						tempBox = tempBox->nextBox;
					}
				}
			}
		}
		//换第一个元素
	//	point2 = tempBox;
		tempBox = m_tableBox;
		temp2 = tempBox->nextBox;
		point3 = temp2->nextBox;

		m_tableBox = temp2;
		temp2->order = tempBox->order;
		temp2->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
		temp2->nextBox = tempBox;

		tempRow = tempRow->nextRow;
		temp2->nextBox->order.set(temp2->order.x + 1,temp2->order.y);

		temp2->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
		temp2->nextBox->nextBox = point3;
	}else
	{
		//移动表头
		for(i = 0;i < m_tableRowSum;++ i)
		{
			if(i == order - 1)
			{
				point0 = tempRow;
				tempRow = tempRow->nextRow;
				temp1 = tempRow->nextRow;
				point1 = temp1->nextRow;

				point0->nextRow = temp1;
				temp1->order = tempRow->order;
				temp1->position = tempRow->position;
				temp1->text.setPosition(temp1->position.x,temp1->position.y);
				temp1->nextRow = tempRow;
				temp1->nextRow->order = temp1->order + 1;
				temp1->nextRow->position.set(temp1->position.x + temp1->pixSize.x * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y);			
				temp1->nextRow->text.setPosition(temp1->nextRow->position.x,temp1->nextRow->position.y);
				if(!m_withoutTex)
				{
					temp1->m_spriteTitle.setPosition(temp1->position);
					temp1->m_spriteTitleEnd.setPosition(temp1->position.x + temp1->pixSize.x * m_scale.x - 
						m_mutiListTitleEnd->textureSize.x * temp1->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,temp1->position.y);
					temp1->nextRow->m_spriteTitle.setPosition(temp1->nextRow->position);
					temp1->nextRow->m_spriteTitleEnd.setPosition(temp1->position.x + temp1->nextRow->pixSize.x * m_scale.x - 
						m_mutiListTitleEnd->textureSize.x * temp1->nextRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
						temp1->nextRow->position.y);
				}
				temp1->nextRow->nextRow = point1;
				break;
			}
			tempRow = tempRow->nextRow;
		}
		//移动表体
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(j == order - 1)
				{
					point2 = tempBox;
					tempBox = tempBox->nextBox;
					temp2 = tempBox->nextBox;
					point3 = temp2->nextBox;

					point2->nextBox = temp2;
					temp2->order = tempBox->order;
					temp2->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
					temp2->nextBox = tempBox;
					if(j == m_tableRowSum -1)
					{//这种情况是不会出现的
						tempRow = tempRow->nextRow;
						temp2->nextBox->order.set(0,temp2->order.y);
						j = 0;
						++ i;
					}else
					{
						tempRow = tempRow->nextRow;
						temp2->nextBox->order.set(temp2->order.x + 1,temp2->order.y);
						++ j;
					}
					temp2->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
					temp2->nextBox->nextBox = point3;
					if(i >= m_tableLineSum - 1)
					{
						break;
					}
				}else
				{
					tempRow = tempRow->nextRow;
					tempBox = tempBox->nextBox;
				}
			}
		}
	}
	updateSliderState();
	updateShowChange();
	return XTrue;
}
void XMultiListBasic::setPosition(float x,float y)
{
	if(!m_isInited) return;
	m_position.set(x,y);
	m_curMouseRect.set(m_position.x + (m_mouseRect.left - 64.0f) * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y,
		m_position.x + m_mouseRect.right * m_scale.x,m_position.y + m_mouseRect.bottom * m_scale.y);
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		//m_check0[i].setScale(m_scale);
		m_check0[i].setPosition(m_position.x + (m_mouseRect.left - DEFAULT_CHECK_SIZE * 2.0f) * m_scale.x,
			m_position.y + ((DEFAULT_CHECK_SIZE + m_mouseRect.top) * m_scale.y + m_curTextHeight * i));
		//m_check1[i].setScale(m_scale);
		m_check1[i].setPosition(m_position.x + (m_mouseRect.left - DEFAULT_CHECK_SIZE) * m_scale.x,
			m_position.y + ((DEFAULT_CHECK_SIZE + m_mouseRect.top) * m_scale.y + m_curTextHeight * i));
	}
	if(!m_withoutTex) m_spriteBackGround.setPosition(m_position);
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
		m_position.y + m_mouseRect.top * m_scale.y);
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_scale.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale.y);
	//改变标题栏的位置
	if(m_tableRowSum > 0)
	{
		m_tableRow->position.set(m_position.x + (m_mouseRect.left + MUTILIST_MIN_WIDTH * 0) * m_scale.x,
			m_position.y + m_mouseRect.top * m_scale.y);
		m_tableRow->text.setPosition(m_tableRow->position.x,m_tableRow->position.y);
		if(!m_withoutTex)
		{
			m_tableRow->m_spriteTitle.setPosition(m_tableRow->position);
			m_tableRow->m_spriteTitleEnd.setPosition(m_tableRow->position.x + m_tableRow->pixSize.x - 
				m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y,m_tableRow->position.y);
		}
		XMultiListOneRow *tempRow = m_tableRow;
		int i,j;
		for(i = 1;i < m_tableRowSum;++ i)
		{
			tempRow->nextRow->position.set(tempRow->position.x + tempRow->pixSize.x * m_scale.x,
				m_position.y + m_mouseRect.top * m_scale.y);
			tempRow->nextRow->text.setPosition(tempRow->nextRow->position.x,tempRow->nextRow->position.y);
			if(!m_withoutTex)
			{
				tempRow->nextRow->m_spriteTitle.setPosition(tempRow->nextRow->position);
				tempRow->nextRow->m_spriteTitleEnd.setPosition(tempRow->nextRow->position.x + tempRow->nextRow->pixSize.x * m_scale.x - 
					m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
					m_tableRow->position.y);
			}
			tempRow = tempRow->nextRow;
		}
		//改变表格的位置
		if(m_tableLineSum > 0)
		{
			m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_curTextHeight * 1);

			XMultiListOneBox *tempBox = m_tableBox;
			for(i = 0;i < m_tableLineSum;++ i)
			{
				tempRow = m_tableRow;
				for(j = 0;j < m_tableRowSum;++ j)
				{
					if(j == 0 && i == 0) 
					{
						tempBox = tempBox->nextBox;
						tempRow = tempRow->nextRow;
						continue;
					}
					tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}
			}
		}
	}
	if(m_haveSelect) updateSelectLine();
	//updateSliderState();
	updateShowChange();	//如果不调用这个会造成移动的时候的bug
	updateChildPos();
}
void XMultiListBasic::setScale(float x,float y)	//设置尺寸
{
	if(!m_isInited ||
		x <= 0 || y <= 0) return;
	m_scale.set(x,y);
	m_curMouseRect.set(m_position.x + (m_mouseRect.left - 64.0f) * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y,
		m_position.x + m_mouseRect.right * m_scale.x,m_position.y + m_mouseRect.bottom * m_scale.y);
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
		m_position.y + m_mouseRect.top * m_scale.y);
	m_verticalSlider.setScale(m_scale);
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_scale.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale.y);
	m_horizontalSlider.setScale(m_scale);
	m_spriteMove.setScale(m_scale);
	m_caption.setScale(m_scale * m_fontSize);
	m_curTextWidth = m_caption.getTextSize().x * m_caption.getScale().x * 0.5f;
	m_curTextHeight = m_caption.getTextSize().y * m_caption.getScale().y;
	m_showPixWidth = (m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_scale.x;
	if(m_withoutTex)
	{
		m_showPixHight = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight() - DEFAULT_TITLE_HEIGHT) * m_scale.y;
	}else
	{
		m_showPixHight = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight() - m_mutiListTitle->textureSize.y) * m_scale.y;
		m_spriteBackGround.setPosition(m_position);
		m_spriteBackGround.setScale(m_scale);
		m_spriteSelect.setScale((float)m_showPixWidth / m_mutiListSelect->textureSize.x,m_curTextHeight / m_mutiListSelect->textureSize.y);
	}
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_check0[i].setScale(m_scale);
		m_check0[i].setPosition(m_position.x + (m_mouseRect.left - DEFAULT_CHECK_SIZE * 2.0f) * m_scale.x,
			m_position.y + ((m_mouseRect.top + DEFAULT_TITLE_HEIGHT) * m_scale.y + m_curTextHeight * i));
		m_check1[i].setScale(m_scale);
		m_check1[i].setPosition(m_position.x + (m_mouseRect.left - DEFAULT_CHECK_SIZE) * m_scale.x,
			m_position.y + ((m_mouseRect.top + DEFAULT_TITLE_HEIGHT) * m_scale.y + m_curTextHeight * i));
	}
	//改变标题栏的位置
	if(m_tableRowSum > 0)
	{
		m_tableRow->position.set(m_position.x + (m_mouseRect.left + MUTILIST_MIN_WIDTH * 0) * m_scale.x,
			m_position.y + m_mouseRect.top * m_scale.y);
		m_tableRow->text.setPosition(m_tableRow->position.x,m_tableRow->position.y);
		m_tableRow->text.setScale(m_scale * m_fontSize);
		if(!m_withoutTex)
		{
			m_tableRow->m_spriteTitle.setPosition(m_tableRow->position);
			m_tableRow->m_spriteTitle.setScale(m_tableRow->pixSize.x * m_scale.x / m_mutiListTitle->textureSize.x,m_scale.y);
			m_tableRow->m_spriteTitleEnd.setPosition(m_tableRow->position.x + m_tableRow->pixSize.x - 
				m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y,m_tableRow->position.y);
			m_tableRow->m_spriteTitleEnd.setScale(m_tableRow->pixSize.y * m_scale.y/ m_mutiListTitle->textureSize.y,
				m_tableRow->pixSize.y * m_scale.y/m_mutiListTitle->textureSize.y);
		}
		XMultiListOneRow *tempRow = m_tableRow;

		int i,j;
		for(i = 1;i < m_tableRowSum;++ i)
		{
			tempRow->nextRow->position.set(tempRow->position.x + tempRow->pixSize.x * m_scale.x,
				m_position.y + m_mouseRect.top * m_scale.y);
			tempRow->nextRow->text.setPosition(tempRow->nextRow->position.x,tempRow->nextRow->position.y);
			tempRow->nextRow->text.setScale(m_scale * m_fontSize);
			if(!m_withoutTex)
			{
				tempRow->nextRow->m_spriteTitle.setPosition(tempRow->nextRow->position);
				tempRow->nextRow->m_spriteTitle.setScale(tempRow->nextRow->pixSize.x * m_scale.x / m_mutiListTitle->textureSize.x,m_scale.y);
				tempRow->nextRow->m_spriteTitleEnd.setPosition(tempRow->nextRow->position.x + m_tableRow->pixSize.x - 
					m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y,
					m_tableRow->position.y);
				tempRow->nextRow->m_spriteTitleEnd.setScale(tempRow->nextRow->pixSize.y * m_scale.y/ m_mutiListTitle->textureSize.y,
					tempRow->nextRow->pixSize.y * m_scale.y/m_mutiListTitle->textureSize.y);
			}
			tempRow = tempRow->nextRow;
		}
		//改变表格的位置
		if(m_tableLineSum > 0)
		{
			m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_curTextHeight * 1);
			m_tableBox->text.setScale(m_scale * m_fontSize);

			XMultiListOneBox *tempBox = m_tableBox;
			for(i = 0;i < m_tableLineSum;++ i)
			{
				tempRow = m_tableRow;
				for(j = 0;j < m_tableRowSum;++ j)
				{
					if(j == 0 && i == 0) 
					{
						tempBox = tempBox->nextBox;
						tempRow = tempRow->nextRow;
						continue;
					}
					tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
					tempBox->text.setScale(m_scale * m_fontSize);
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}
			}
		}
	}
	if(m_haveSelect)
	{
		updateSelectLine();
	}
	updateSliderState();
	updateShowChange();
	updateChildScale();
}
#if !WITH_INLINE_FILE
#include "XMultiListBasic.inl"
#endif
}