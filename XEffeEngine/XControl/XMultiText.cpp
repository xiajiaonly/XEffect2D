//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XMultiText.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

void funMutiTextValueChangeV(void *pClass,int objectID)
{
	_XMultiText *pPar = (_XMultiText *)pClass;
//	printf("V ID:%d\n",pPar->m_objectID);
//	printf("V Now value:%f\n",pPar->m_verticalSlider.getNowValue());
	//相应数值的改变
	int temp = pPar->m_verticalSlider.getNowValue();
	if(temp != pPar->m_nowStartLine)
	{
		pPar->setStartLine(temp);
		pPar->updateLineStr();
		pPar->m_verticalSlider.setNowValue(temp);
	}else
	{//微量移动时候的处理
		if(pPar->m_verticalSlider.getNowValue() > pPar->m_nowStartLine)
		{
			temp += 1;
			pPar->setStartLine(temp);
			pPar->updateLineStr();
			pPar->m_verticalSlider.setNowValue(temp);
		}else
		if(pPar->m_verticalSlider.getNowValue() < pPar->m_nowStartLine)
		{
			temp -= 1;
			pPar->setStartLine(temp);
			pPar->updateLineStr();
			pPar->m_verticalSlider.setNowValue(temp);
		}
	}
}

void funMutiTextValueChangeMoveV(void *pClass,int objectID)
{
	_XMultiText *pPar = (_XMultiText *)pClass;
//	printf("V ID:%d\n",pPar->m_objectID);
//	printf("V Now value:%f\n",pPar->m_verticalSlider.getNowValue());
	//相应数值的改变
	int temp = pPar->m_verticalSlider.getNowValue();
	if(temp != pPar->m_nowStartLine)
	{
		//需要更新垂直片选的情况
		pPar->setStartLine(temp);
		pPar->updateLineStr();
	}
}

void funMutiTextValueChangeH(void *pClass,int objectID)
{
	_XMultiText *pPar = (_XMultiText *)pClass;
//	printf("H ID:%d\n",pPar->m_objectID);
//	printf("H Now value:%f\n",pPar->m_horizontalSlider.getNowValue());
	//相应数值的改变
	int temp = pPar->m_horizontalSlider.getNowValue();
	if(temp != pPar->m_lineStr[0].showStart)
	{
		for(int i = 0;i < pPar->m_nowShowLineSum;++ i)
		{
			pPar->m_lineStr[i].showStart = temp;
		}
		pPar->updateLineStr();
		pPar->m_horizontalSlider.setNowValue(temp);
	}else
	{
		if(pPar->m_horizontalSlider.getNowValue() > pPar->m_lineStr[0].showStart)
		{
			temp += 1;
			for(int i = 0;i < pPar->m_nowShowLineSum;++ i)
			{
				pPar->m_lineStr[i].showStart = temp;
			}
			pPar->updateLineStr();
			pPar->m_horizontalSlider.setNowValue(temp);
		}else
		if(pPar->m_horizontalSlider.getNowValue() < pPar->m_lineStr[0].showStart)
		{
			temp -= 1;
			for(int i = 0;i < pPar->m_nowShowLineSum;++ i)
			{
				pPar->m_lineStr[i].showStart = temp;
			}
			pPar->updateLineStr();
			pPar->m_horizontalSlider.setNowValue(temp);
		}
	}
}

void funMutiTextValueChangeMoveH(void *pClass,int objectID)
{
	_XMultiText *pPar = (_XMultiText *)pClass;
//	printf("H ID:%d\n",pPar->m_objectID);
//	printf("H Now value:%f\n",pPar->m_horizontalSlider.getNowValue());
	//相应数值的改变
	int temp = pPar->m_horizontalSlider.getNowValue();
	if(temp != pPar->m_lineStr[0].showStart)
	{
		for(int i = 0;i < pPar->m_nowShowLineSum;++ i)
		{
			pPar->m_lineStr[i].showStart = temp;
		}
		pPar->updateLineStr();
	}
}

_XMultiText::_XMultiText()
:m_isInited(0)
,m_nowString(NULL)				//当前输入的字符串，整个
,m_mutiEditNormal(NULL)			//输入框普通状态
,m_mutiEditDisable(NULL)		//输入框无效状态
,m_mutiEditSelect(NULL)			//输入框片选颜色
//,m_funInputChenge(NULL)			//输入内容发生改变的时候调用
//,m_funInputOver(NULL)			//确认输入结束之后的时候调用
,m_lineStr(NULL)
{
}

_XMultiText::~_XMultiText()
{
	release();
}

void _XMultiText::release()
{
	if(m_isInited == 0) return ;	//如果没有初始化直接退出
	if(m_lineStr != NULL)
	{
		for(int i = 0;i < m_nowShowLineSum;++ i)
		{
			XDELETE_ARRAY(m_lineStr[i].showString);
		}
	}
	XDELETE_ARRAY(m_lineStr);
	XDELETE_ARRAY(m_nowString);
	_XControlManager::GetInstance().decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(this);
#endif
	m_isInited = 0;
}

void _XMultiText::setStartLine(int temp)
{
	if(m_haveSelect != 0)
	{
		m_lineStr[m_nowSelectLineOrder - m_nowStartLine].haveAllSelect = 0;
	}
	m_nowStartLine = temp;
	if(m_haveSelect != 0)
	{
		m_lineStr[m_nowSelectLineOrder - m_nowStartLine].haveAllSelect = 1;
		m_lineStr[m_nowSelectLineOrder - m_nowStartLine].spriteSelect.setPosition(m_nowMouseRect.left,
			m_nowMouseRect.top + m_textHeight * (m_nowSelectLineOrder - m_nowStartLine));
		m_lineStr[m_nowSelectLineOrder - m_nowStartLine].spriteSelect.setSize(m_nowMouseRect.getWidth() / m_mutiEditSelect->textureSize.x,
						m_textHeight / m_mutiEditSelect->textureSize.y);
	}
}

int _XMultiText::init(const _XVector2& position,	//控件所在的位置
		const _XRect& Area,	//控件的范围
		const _XMultiTextTexture& tex,	//控件的贴图
		const char *str,const _XFontUnicode& font,float strSize,		//控件的字体及相关信息
		//const _XVector2& strPosition,
		//const _XMouseRightButtonMenu &mouseMenu,
		const _XSlider &vSlider,const _XSlider &hSlider)	//控件的其他附属依赖控件
{
	if(m_isInited != 0) return 0;	//防止重复初始化
	if(tex.editNormal == NULL || tex.editInsert == NULL || tex.editDisable == NULL || tex.editSelect == NULL) return 0;

	//需要注意的是输入范围必须要能显示最少一个字符，否则将会造成问题，目前这里并没有代码去判断，但是实际使用中需要注意这个问题
	if(Area.getWidth() <= 0 || Area.getHeight() <= 0) return 0;	//输入范围不能为空
	if(strSize <= 0) return 0;		//字符串的大小不能为非法值
	if(str != NULL && strlen(str) >= MAX_STRING_LENGTH) return 0;

	//下面开始赋值
	m_position = position;
	m_mouseRect = Area;

	m_mutiEditNormal = tex.editNormal;			//输入框普通状态
	m_mutiEditDisable = tex.editDisable;		//输入框无效状态
	m_mutiEditSelect = tex.editSelect;			//输入框片选颜色
	m_mutiEditUpon = tex.editUpon;
	m_size.set(1.0f,1.0f);

	//m_textPosition = strPosition;			//文字显示的位置，是相对于控件的位置来定的
	m_textSize.set(strSize,strSize);				//文字显示的尺寸，这个尺寸会与空间的缩放尺寸叠加

	m_spriteBackGround.init(m_mutiEditNormal->texture.m_w,m_mutiEditNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteBackGround);
#endif
	m_spriteBackGround.setPosition(m_position);
	m_spriteBackGround.setSize(m_size);
	m_spriteBackGround.setIsTransformCenter(POINT_LEFT_TOP);

	m_verticalSlider.setACopy(vSlider);
	_XControlManager::GetInstance().decreaseAObject(&m_verticalSlider);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_verticalSlider);
#endif
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_size.x,
		m_position.y + m_mouseRect.top * m_size.y);
	m_verticalSlider.setSize(m_size);
	m_verticalSlider.setCallbackFun(NULL,NULL,NULL,NULL,NULL,funMutiTextValueChangeV,funMutiTextValueChangeMoveV,this);
	m_needShowVSlider = 0;			//是否需要显示垂直滑动条

	m_horizontalSlider.setACopy(hSlider);
	_XControlManager::GetInstance().decreaseAObject(&m_horizontalSlider);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_horizontalSlider);
#endif
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_size.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_size.y);
	m_horizontalSlider.setSize(m_size);
	m_horizontalSlider.setCallbackFun(NULL,NULL,NULL,NULL,NULL,funMutiTextValueChangeH,funMutiTextValueChangeMoveH,this);
	m_needShowHSlider = 0;			//是否需要显示水平滑动条

	m_textWidth = font.getTextSize().x * font.getSize().x * 0.5f;
	m_textHeight = font.getTextSize().y * font.getSize().y;
	//计算当前可以显示多少行
	m_nowShowLineSum = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight())/m_textHeight;	//不应该进去高度，而应该减去buttom ，以后在修正
	//计算当前一行可以显示多少个字符(这个0.5是因为显示英文字母却使用中文字库)
	m_nowShowLineWidth = (m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth())/m_textWidth;	//不应该减去宽度，而应该减去right，以后在修正

	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_size.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_size.y);	//当前的鼠标响应范围
	//为每行分配内存空间
	try
	{
		m_lineStr = new _XMultiTextLineString[m_nowShowLineSum];
		if(m_lineStr == NULL)return 0;
	}catch(...)
	{
		return 0;
	}
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	for(int i = 0;i < m_nowShowLineSum;++ i)
	{
		m_lineStr[i].showString = NULL;
		try
		{
			m_lineStr[i].showString = new char[m_nowShowLineWidth + 1];
			if(m_lineStr[i].showString == NULL)
			{
				for(int j = 0;j < i;++ j)
				{
					XDELETE_ARRAY(m_lineStr[j].showString);
				}
				XDELETE_ARRAY(m_lineStr);
				return 0;
			}
		}catch(...)
		{
			for(int j = 0;j < i;++ j)
			{
				XDELETE_ARRAY(m_lineStr[j].showString);
			}
			XDELETE_ARRAY(m_lineStr);
			return 0;
		}
		m_lineStr[i].isEnable = 0;
		m_lineStr[i].nowText.setACopy(font);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_lineStr[i].nowText));
#endif
		m_lineStr[i].nowText.setAlignmentModeX(_FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
		m_lineStr[i].nowText.setAlignmentModeY(_FONT_ALIGNMENT_MODE_Y_UP);	 //设置字体上对齐
		m_lineStr[i].nowText.setColor(m_textColor);
		m_lineStr[i].nowText.setPosition(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y + m_textHeight * i);
		m_lineStr[i].nowText.setSize(m_size);
		m_lineStr[i].showLength = m_nowShowLineWidth;
		m_lineStr[i].showStart = 0;
		m_lineStr[i].haveAllSelect = 0;
		m_lineStr[i].spriteSelect.init(m_mutiEditSelect->texture.m_w,m_mutiEditSelect->texture.m_h,1);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_lineStr[i].spriteSelect));
#endif
		m_lineStr[i].spriteSelect.setIsTransformCenter(POINT_LEFT_TOP);
		m_lineStr[i].showString[0] = '\0';
	}
	//为整个字符串分配内存空间
	try
	{
		m_nowString = new char[MAX_STRING_LENGTH];
		if(m_nowString == NULL) 
		{
			for(int i = 0;i < m_nowShowLineSum;++ i)
			{
				XDELETE_ARRAY(m_lineStr[i].showString);
			}
			XDELETE_ARRAY(m_lineStr);
			return 0;
		}
	}catch(...)
	{
		for(int i = 0;i < m_nowShowLineSum;++ i)
		{
			XDELETE_ARRAY(m_lineStr[i].showString);
		}
		XDELETE_ARRAY(m_lineStr);
		return 0;
	}
	m_nowString[0] = '\0';
	//拷贝字符串
	strcpy(m_nowString,str);
	//处理字符串
	m_nowStartLine = 0;
	updateLineStr();

	m_haveSelect = 0;		//是否在字符串中有选择
	m_nowSelectLineOrder = -1;

	m_isVisiable = 1;
	m_isEnable = 1;
	m_isActive = 1;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_MUTITEXT);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = 1;
	return 1;
}

void _XMultiText::updateLineStr()
{//将实际的字符串分配到各行去
	//if(m_isInited == 0) return;	//如果没有初始化直接退出
	if(m_nowString == NULL) return;
	int lineLength = 0;
	int maxLineLength = 0;
	int lineSum = 0;
	char tempChar[MAX_LINE_STRING_LENGTH];
	for(int i = 0;i < m_nowShowLineSum;++ i)
	{
		m_lineStr[i].isEnable = 0;
	}
	if(m_nowStartLine == 0)
	{//如果是从字符串开头显示，则需要初始化第一行的头位字符串开始
		m_lineStr[0].lineHeadorder = 0;
	}
	for(int i = 0;;++ i)
	{		
		if(m_nowString[i] == '\0')
		{
			if(i == 0)
			{//空字符串(什么也不显示，直接退出)
				break;
			}else
			{//最后一行
				if(lineLength == 0)
				{//如果最后一行的长度为零也什么都不处理
					break;
				}else
				{
					if(lineSum >= m_nowStartLine && lineSum < m_nowStartLine + m_nowShowLineSum)
					{//拷贝字符串到行显示内存
						if(lineLength >= m_lineStr[lineSum - m_nowStartLine].showStart)
						{
							m_lineStr[lineSum - m_nowStartLine].isEnable = 1;
							if(lineLength - m_lineStr[lineSum - m_nowStartLine].showStart > m_lineStr[lineSum - m_nowStartLine].showLength)
							{//实际字符串的长度超过显示长度
								memcpy(m_lineStr[lineSum - m_nowStartLine].showString,tempChar + m_lineStr[lineSum - m_nowStartLine].showStart,
									m_lineStr[lineSum - m_nowStartLine].showLength);
								m_lineStr[lineSum - m_nowStartLine].showString[m_lineStr[lineSum - m_nowStartLine].showLength] = '\0';
							}else
							{//实际字符串的长度小于显示长度
								memcpy(m_lineStr[lineSum - m_nowStartLine].showString,tempChar + m_lineStr[lineSum - m_nowStartLine].showStart,
									lineLength - m_lineStr[lineSum - m_nowStartLine].showStart);
								m_lineStr[lineSum - m_nowStartLine].showString[lineLength - m_lineStr[lineSum - m_nowStartLine].showStart] = '\0';
							}
							m_lineStr[lineSum - m_nowStartLine].nowText.setString(m_lineStr[lineSum - m_nowStartLine].showString);
						}else
						{
							//m_lineStr[lineSum - m_nowStartLine].isEnable = 0;
							m_lineStr[lineSum - m_nowStartLine].isEnable = 1;
							m_lineStr[lineSum - m_nowStartLine].nowText.setString(" ");
						}
						m_lineStr[lineSum - m_nowStartLine].lineEndOrder = i;
						m_lineStr[lineSum - m_nowStartLine].lineLength = m_lineStr[lineSum - m_nowStartLine].lineEndOrder - 
							m_lineStr[lineSum - m_nowStartLine].lineHeadorder + 1;
					}
					if(lineSum + 1 >= m_nowStartLine && lineSum < m_nowStartLine + m_nowShowLineSum - 1)
					{
						m_lineStr[lineSum - m_nowStartLine].lineHeadorder = i + 1;
					}
					lineSum ++;
					if(lineLength > maxLineLength)
					{//记录最大长度
						maxLineLength = lineLength;
					}
					break;
				}
			}
		}else
		if(m_nowString[i] == '\n')
		{//换行
			tempChar[lineLength] = m_nowString[i];	//记录换行符
			if(lineSum >= m_nowStartLine && lineSum < m_nowStartLine + m_nowShowLineSum)
			{//拷贝字符串到行显示内存
				if(lineLength >= m_lineStr[lineSum - m_nowStartLine].showStart)
				{
					m_lineStr[lineSum - m_nowStartLine].isEnable = 1;
					if(lineLength - m_lineStr[lineSum - m_nowStartLine].showStart > m_lineStr[lineSum - m_nowStartLine].showLength)
					{//实际字符串的长度超过显示长度
						memcpy(m_lineStr[lineSum - m_nowStartLine].showString,tempChar + m_lineStr[lineSum - m_nowStartLine].showStart,
							m_lineStr[lineSum - m_nowStartLine].showLength);
						m_lineStr[lineSum - m_nowStartLine].showString[m_lineStr[lineSum - m_nowStartLine].showLength] = '\0';
					}else
					{//实际字符串的长度小于显示长度
						memcpy(m_lineStr[lineSum - m_nowStartLine].showString,tempChar + m_lineStr[lineSum - m_nowStartLine].showStart,
							lineLength - m_lineStr[lineSum - m_nowStartLine].showStart);
						m_lineStr[lineSum - m_nowStartLine].showString[lineLength - m_lineStr[lineSum - m_nowStartLine].showStart] = '\0';
					}
					m_lineStr[lineSum - m_nowStartLine].nowText.setString(m_lineStr[lineSum - m_nowStartLine].showString);
				}else
				{//如果没有需要显示的字符，则标记没有字符需要选择
					//m_lineStr[lineSum - m_nowStartLine].isEnable = 0;
					m_lineStr[lineSum - m_nowStartLine].isEnable = 1;
					m_lineStr[lineSum - m_nowStartLine].nowText.setString(" ");
				}
				m_lineStr[lineSum - m_nowStartLine].lineEndOrder = i;
				m_lineStr[lineSum - m_nowStartLine].lineLength = m_lineStr[lineSum - m_nowStartLine].lineEndOrder - 
					m_lineStr[lineSum - m_nowStartLine].lineHeadorder + 1;
				m_lineStr[lineSum - m_nowStartLine].isEnterEnd = 1;
			}
			if(lineSum + 1 >= m_nowStartLine && lineSum < m_nowStartLine + m_nowShowLineSum - 1)
			{
				m_lineStr[lineSum - m_nowStartLine + 1].lineHeadorder = i + 1;
			}
			lineSum ++;
			lineLength ++;
			if(lineLength > maxLineLength)
			{//记录最大长度
				maxLineLength = lineLength;
			}
			lineLength = 0;
		}else
		{//如果是合法的字符则记录
			tempChar[lineLength] = m_nowString[i];	//记录换行符
			lineLength ++;
		/*	if(lineLength >= MAX_LINE_STRING_LENGTH - 1)
			{//如果超过最大显示字符，也要换行
				if(lineSum >= m_nowStartLine && lineSum < m_nowStartLine + m_nowShowLineSum)
				{//拷贝字符串到行显示内存
					if(lineLength >= m_lineStr[lineSum - m_nowStartLine].showStart)
					{
						m_lineStr[lineSum - m_nowStartLine].isEnable = 1;
						if(lineLength - m_lineStr[lineSum - m_nowStartLine].showStart > m_lineStr[lineSum - m_nowStartLine].showLength)
						{//实际字符串的长度超过显示长度
							memcpy(m_lineStr[lineSum - m_nowStartLine].showString,tempChar + m_lineStr[lineSum - m_nowStartLine].showStart,
								m_lineStr[lineSum - m_nowStartLine].showLength);
							m_lineStr[lineSum - m_nowStartLine].showString[m_lineStr[lineSum - m_nowStartLine].showLength] = '\0';
						}else
						{//实际字符串的长度小于显示长度
							memcpy(m_lineStr[lineSum - m_nowStartLine].showString,tempChar + m_lineStr[lineSum - m_nowStartLine].showStart,
								lineLength - m_lineStr[lineSum - m_nowStartLine].showStart);
							m_lineStr[lineSum - m_nowStartLine].showString[lineLength - m_lineStr[lineSum - m_nowStartLine].showStart] = '\0';
						}
						m_lineStr[lineSum - m_nowStartLine].nowText.setString(m_lineStr[lineSum - m_nowStartLine].showString);
					}else
					{
						m_lineStr[lineSum - m_nowStartLine].isEnable = 0;
					}
					m_lineStr[lineSum - m_nowStartLine].lineEndOrder = i;
					m_lineStr[lineSum - m_nowStartLine].lineLength = m_lineStr[lineSum - m_nowStartLine].lineEndOrder - 
						m_lineStr[lineSum - m_nowStartLine].lineHeadorder + 1;
					m_lineStr[lineSum - m_nowStartLine].isEnterEnd = 0;
				}
				if(lineSum + 1 >= m_nowStartLine && lineSum < m_nowStartLine + m_nowShowLineSum - 1)
				{
					m_lineStr[lineSum - m_nowStartLine + 1].lineHeadorder = i + 1;
				}
				lineSum ++;
				lineLength ++;
				if(lineLength > maxLineLength)
				{//记录最大长度
					maxLineLength = lineLength;
				}
				lineLength = 0;
			}*/
		}
	}
	m_nowMaxLineSum = lineSum;
	m_nowMaxLineLength = maxLineLength;
	if(m_nowMaxLineSum > m_nowShowLineSum)
	{//如果实际的行数大于显示的最大行数，则需要显示纵向拖动条
		m_needShowVSlider = 1;
		m_verticalSlider.setNowValue(m_nowStartLine);
		m_verticalSlider.setRange(m_nowMaxLineSum - m_nowShowLineSum,0.0f);
	}else
	{
		m_needShowVSlider = 0;
	}
	if(m_nowMaxLineLength > m_nowShowLineWidth)
	{//如果实际的行数大于显示的最大行数，则需要显示纵向拖动条
		m_needShowHSlider = 1;
		m_horizontalSlider.setNowValue(m_lineStr[0].showStart);
		m_horizontalSlider.setRange(m_nowMaxLineLength - m_nowShowLineWidth,0.0f);
	}else
	{
		m_needShowHSlider = 0;
	}
}

void _XMultiText::draw()
{
	if(m_isInited == 0) return ;	//如果没有初始化直接退出
	if(m_isVisiable == 0) return;	//如果不可见直接退出
	if(m_isEnable == 0)
	{//如果无效则显示无效,不显示插入符号
		m_spriteBackGround.draw(m_mutiEditDisable);
	}else
	{//如果有效则显示有效,显示插入符号
		m_spriteBackGround.draw(m_mutiEditNormal);
	}
	if(m_haveSelect != 0)
	{//显示选择的范围
		//m_spriteSelect.draw(&(m_mutiEditSelect->m_texture));
	}
	//显示当前输入的字符串
	for(int i = 0;i < m_nowShowLineSum;++ i)
	{
		if(m_lineStr[i].isEnable != 0)
		{
			if(m_lineStr[i].haveAllSelect != 0)
			{
				m_lineStr[i].spriteSelect.draw(m_mutiEditSelect);
			}
			m_lineStr[i].nowText.draw();
		}
	}
	//显示滑动条
	if(m_needShowVSlider != 0) m_verticalSlider.draw();
	if(m_needShowHSlider != 0) m_horizontalSlider.draw();
	if(m_mutiEditUpon != NULL) m_spriteBackGround.draw(m_mutiEditUpon);
}

int _XMultiText::canGetFocus(float x,float y)
{
	if(m_isInited == 0) return 0;	//如果没有初始化直接退出
	if(m_isActive == 0) return 0;		//没有激活的控件不接收控制
	if(m_isVisiable == 0) return 0;	//如果不可见直接退出
	if(m_isEnable == 0) return 0;		//如果无效则直接退出
	return isInRect(x,y);
}

int _XMultiText::canLostFocus(float x,float y)
{
	return 1;
}

int _XMultiText::mouseProc(float x,float y,_XMouseState mouseState)
{
	if(m_isInited == 0) return 0;		//如果没有初始化直接退出
	if(m_isActive == 0) return 0;	//如果不可见直接退出
	if(m_isVisiable == 0) return 0;	//如果不可见直接退出
	if(m_isEnable == 0) return 0;		//如果无效则直接退出
	
	if(m_needShowVSlider != 0) m_verticalSlider.mouseProc(x,y,mouseState);
	if(m_needShowHSlider != 0) m_horizontalSlider.mouseProc(x,y,mouseState);
	//判断鼠标点击是否在范围内
	if(mouseState == _XMOUSE_LEFT_BUTTON_UP && m_nowMouseRect.isInRect(x,y))
	{//点选有效，计算点选的行值
		int temp = (y - m_nowMouseRect.top) / m_textHeight;	//计算当前选择的行数
		if(temp < 0) temp = 0;
		if(temp >= m_nowShowLineSum) temp = m_nowShowLineSum - 1;
		if(temp + m_nowStartLine < m_nowMaxLineSum)
		{
			if(m_haveSelect != 0 && m_nowSelectLineOrder != temp)
			{//这样则需要剔除原有的选择，整个只能有一个处于选择状态
				m_lineStr[m_nowSelectLineOrder - m_nowStartLine].haveAllSelect = 0;	//注意这里是显示一整行被选中，所以，不需要标记头和尾
			}
			if(m_haveSelect != 0 && m_nowSelectLineOrder == temp)
			{//如果重复选择则不作标记
			}else
			{
				m_haveSelect = 1;
				m_nowSelectLineOrder = temp + m_nowStartLine;
				//标记所选的行
				m_lineStr[temp].haveAllSelect = 1;	//注意这里是显示一整行被选中，所以，不需要标记头和尾
				//计算选取的位置
				m_lineStr[temp].spriteSelect.setPosition(m_nowMouseRect.left, m_nowMouseRect.top + m_textHeight * temp);

				m_lineStr[temp].spriteSelect.setSize(m_nowMouseRect.getWidth() / m_mutiEditSelect->textureSize.x,
					m_textHeight / m_mutiEditSelect->textureSize.y);
			}
		}
	}
	return 1;
}

int _XMultiText::keyboardProc(int keyOrder,_XKeyState keyState)
{
	return 0;
}

void _XMultiText::setPosition(float x,float y)
{
	m_position.set(x,y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_size.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_size.y);	//当前的鼠标响应范围

	m_spriteBackGround.setPosition(m_position);
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_size.x,
		m_position.y + m_mouseRect.top * m_size.y);
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_size.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_size.y);
	for(int i = 0;i < m_nowShowLineSum;++ i)
	{
		m_lineStr[i].nowText.setPosition(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y + m_textHeight * i);
	}
	if(m_haveSelect != 0)
	{//存在选取
		int temp = m_nowSelectLineOrder - m_nowStartLine;
		m_lineStr[temp].spriteSelect.setPosition(m_nowMouseRect.left,m_nowMouseRect.top + m_textHeight * temp);
	}
}

void _XMultiText::setSize(float x,float y)
{
	if(x <= 0 && y <= 0) return;
	m_size.set(x,y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_size.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_size.y);	//当前的鼠标响应范围
	m_spriteBackGround.setPosition(m_position);
	m_spriteBackGround.setSize(m_size);
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_size.x,
		m_position.y + m_mouseRect.top * m_size.y);
	m_verticalSlider.setSize(m_size);
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_size.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_size.y);
	m_horizontalSlider.setSize(m_size);

	m_lineStr[0].nowText.setSize(m_size);
	m_textWidth = m_lineStr[0].nowText.getTextSize().x * m_lineStr[0].nowText.getSize().x * 0.5f;
	m_textHeight = m_lineStr[0].nowText.getTextSize().y * m_lineStr[0].nowText.getSize().y;
	for(int i = 0;i < m_nowShowLineSum;++ i)
	{
		m_lineStr[i].nowText.setPosition(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y + m_textHeight * i);
		m_lineStr[i].nowText.setSize(m_size);
	}
	if(m_haveSelect != 0)
	{//存在选取
		int temp = m_nowSelectLineOrder - m_nowStartLine;
		m_lineStr[temp].spriteSelect.setPosition(m_nowMouseRect.left,m_nowMouseRect.top + m_textHeight * temp);
		m_lineStr[temp].spriteSelect.setSize(m_nowMouseRect.getWidth() / m_mutiEditSelect->textureSize.x,
						m_textHeight / m_mutiEditSelect->textureSize.y);
	}
}

void _XMultiText::checkStr()			//检查字符串的单行宽度是否大于限制
{
	int lineLength = 0;		//当前的行宽
	int strLength = strlen(m_nowString);
	char tempStr[MAX_STRING_LENGTH];
	for(int i = 0;;++ i)
	{
		if(m_nowString[i] == '\0')
		{
			lineLength = 0;
			break;
		}else
		if(m_nowString[i] == '\n')
		{
			lineLength = 0;
		}else
		{
			lineLength ++;
			if(lineLength >= MAX_LINE_STRING_LENGTH - 2)
			{//超出长度的显示，这里直接插入换行符
				strLength++;
				if(strLength >= MAX_STRING_LENGTH)
				{//字符串的长度超过显示上限
					strcpy(tempStr,m_nowString + i);
					tempStr[MAX_STRING_LENGTH - i -1] = '\0';
				}else
				{
					strcpy(tempStr,m_nowString + i);
				}
				m_nowString[i] = '\n';
				strcpy(m_nowString + i + 1,tempStr);
				lineLength = 0;
			}
		}
	}
}

void _XMultiText::setString(const char *str)
{
	if(m_isInited == 0) return;	//必须要初始化
	if(str == NULL) return;		//空指针需要退出
	//拷贝字符串
	strcpy(m_nowString,str);
	//这里最好能整理字符串，不要超出一行的最大显示，这样子有利于后面的统一处理
	checkStr();

	//处理字符串
	m_nowStartLine = 0;
	updateLineStr();

	m_haveSelect = 0;		//是否在字符串中有选择
	m_nowSelectLineOrder = -1;
	for(int i = 0;i < m_nowShowLineSum;++ i)
	{
		m_lineStr[i].haveAllSelect = 0;
	}
}

void _XMultiText::addString(const char *str)
{
	if(m_isInited == 0) return;		//如果没有初始化直接退出
	if(str == NULL) return;
	if(strlen(str) == 0) return;
	if(strlen(str) + strlen(m_nowString) >= MAX_STRING_LENGTH) return;
	strcat(m_nowString,str);
	checkStr();
	updateLineStr();
}

void _XMultiText::addALine(const char *str)		//向字符串中添加字符串
{
	if(m_isInited == 0) return;		//如果没有初始化直接退出
	if(str == NULL) return;
	if(strlen(str) + strlen(m_nowString) + 1 >= MAX_STRING_LENGTH) return;
	if(strlen(str) == 0) return;
	if(m_nowString[strlen(m_nowString) - 1] != '\n')
	{//如果加入的字符串不是以回车结束，则这里加入换行符
		if(strlen(str) + strlen(m_nowString) + 2 >= MAX_STRING_LENGTH) return;
		strcat(m_nowString,"\n");
	}
	strcat(m_nowString,str);

	checkStr();
	if(m_needShowVSlider != 0 && m_verticalSlider.getNowValue() == m_verticalSlider.getMaxValue())
	{
		updateLineStr();
		m_verticalSlider.setNowValue(m_verticalSlider.getMaxValue());
	}else
	{
		updateLineStr();
	}
}

int _XMultiText::setACopy(const _XMultiText &temp)
{
	if(& temp == this) return 1;	//防止自身赋值
	if(temp.m_isInited == 0) return 0;
	
	//not finished
	if(m_isInited != 0) release();
	if(_XControlBasic::setACopy(temp) == 0) return 0;
	if(m_isInited == 0)
	{
		_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_MUTITEXT);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	}

	m_isInited = temp.m_isInited;				//是否初始化

	m_needShowVSlider = temp.m_needShowVSlider;			//是否需要显示垂直滑动条
	m_verticalSlider.setACopy(temp.m_verticalSlider);		//垂直滑动条
	_XControlManager::GetInstance().decreaseAObject(&m_verticalSlider);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_verticalSlider);
#endif
	m_verticalSlider.setCallbackFun(NULL,NULL,NULL,NULL,NULL,funMutiTextValueChangeV,funMutiTextValueChangeMoveV,this);
	m_needShowHSlider = temp.m_needShowHSlider;			//是否需要显示水平滑动条
	m_horizontalSlider.setACopy(temp.m_horizontalSlider);	//水平滑动条
	_XControlManager::GetInstance().decreaseAObject(&m_horizontalSlider);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_horizontalSlider);
#endif
	m_horizontalSlider.setCallbackFun(NULL,NULL,NULL,NULL,NULL,funMutiTextValueChangeH,funMutiTextValueChangeMoveH,this);

	try
	{
		m_nowString = new char[MAX_STRING_LENGTH];
		if(m_nowString == NULL) 
		{
			return 0;
		}
	}catch(...)
	{
		return 0;
	}
	strcpy(m_nowString,temp.m_nowString);

	m_mutiEditNormal = temp.m_mutiEditNormal;		//输入框普通状态
	m_mutiEditDisable = temp.m_mutiEditDisable;		//输入框无效状态
	m_mutiEditSelect = temp.m_mutiEditSelect;		//输入框片选颜色
	m_mutiEditUpon = temp.m_mutiEditUpon;

	m_spriteBackGround.setACopy(temp.m_spriteBackGround);	//用于显示输入框的背景贴图
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteBackGround);
#endif
	//m_textPosition = temp.m_textPosition;			//文字显示的位置，是相对于控件的位置来定的
	m_textSize = temp.m_textSize;				//文字显示的尺寸，这个尺寸会与空间的缩放尺寸叠加

	m_haveSelect = temp.m_haveSelect;		//是否在字符串中有选择
	m_nowSelectLineOrder = temp.m_nowSelectLineOrder;	//当前文档中被选择的是哪一行

	m_nowStartLine = temp.m_nowStartLine;		//当前显示的第一行是总的第几行
	m_nowShowLineSum = temp.m_nowShowLineSum;		//当前可以显示多少行
	m_nowShowLineWidth = temp.m_nowShowLineWidth;		//当前一行可以显示的宽度
	m_nowMaxLineSum = temp.m_nowMaxLineSum;	//当前字符串中实际的行数
	m_nowMaxLineLength = temp.m_nowMaxLineLength;	//当前字符串中实际的行宽

	try
	{
		m_lineStr = new _XMultiTextLineString[m_nowShowLineSum];
		if(m_lineStr == NULL)
		{
			return 0;
		}
	}catch(...)
	{
		return 0;
	}
	for(int i = 0;i < m_nowShowLineSum;++ i)
	{
		m_lineStr[i].showString = NULL;
		try
		{
			m_lineStr[i].showString = new char[m_nowShowLineWidth + 1];
			if(m_lineStr[i].showString == NULL)
			{
				return 0;
			}
		}catch(...)
		{
			return 0;
		}
		strcpy(m_lineStr[i].showString,temp.m_lineStr[i].showString);
		m_lineStr[i].isEnable = temp.m_lineStr[i].isEnable;				//这一行是否要显示，是否有效
		m_lineStr[i].showStart = temp.m_lineStr[i].showStart;				//这行字符串显示部分的头在当前行的偏移
		m_lineStr[i].showLength = temp.m_lineStr[i].showLength;				//显示字符串的长度
		m_lineStr[i].nowText.setACopy(temp.m_lineStr[i].nowText);		//用于显示这一行字符串的字体
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_lineStr[i].nowText));
#endif
		m_lineStr[i].lineHeadorder = temp.m_lineStr[i].lineHeadorder;			//这一行字符在全文中的头位置
		m_lineStr[i].lineEndOrder = temp.m_lineStr[i].lineEndOrder;			//这一行字符在全文中的尾位置
		m_lineStr[i].lineLength = temp.m_lineStr[i].lineLength;				//这一行字符串的长度
		m_lineStr[i].isEnterEnd = temp.m_lineStr[i].isEnterEnd;			//是否因为换行而换行显示1，或者是因为显示字符串的长度限制而换行0
		m_lineStr[i].spriteSelect.setACopy(temp.m_lineStr[i].spriteSelect);		//字符串显示选择背景颜色的精灵
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_lineStr[i].spriteSelect));
#endif
		m_lineStr[i].haveAllSelect = temp.m_lineStr[i].haveAllSelect;			//该行中是否存被整体选择
	}

//	m_funInputChenge = temp.m_funInputChenge;		//输入内容发生改变的时候调用
//	m_funInputOver = temp.m_funInputOver;		//确认输入结束之后的时候调用
//	m_pClass = temp.m_pClass;

	m_timer = temp.m_timer;	//这是插入符号闪烁时使用的时间标记，这个时间使用不准确的计时方式
	m_textColor = temp.m_textColor;
	m_textWidth = temp.m_textWidth;	//这是插入符号闪烁时使用的时间标记，这个时间使用不准确的计时方式
	m_textHeight = temp.m_textHeight;	//这是插入符号闪烁时使用的时间标记，这个时间使用不准确的计时方式
	return 1;
}

int _XMultiText::exportData(const char *fileName)		//数据导出
{
	if(m_isInited == 0) return 0;		//如果没有初始化直接退出
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
			return 0;
		}
	}else
	{
		if((fp = fopen(fileName,"wb")) == NULL)
		{
			printf("File open error!\n");
			return 0;
		}
	}
	if(m_nowString != NULL)
	{
		//fprintf(fp,"%s",m_nowString);
		fwrite(m_nowString,sizeof(char),strlen(m_nowString) + 1,fp);
	}
	fclose(fp);
	return 1;
}

int _XMultiText::importData(const char *fileName)		//数据导入
{
	if(m_isInited == 0) return 0;		//如果没有初始化直接退出
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
			return 0;
		}
	}else
	{
		if((fp = fopen(fileName,"rb")) == NULL)
		{
			printf("File open error!\n");
			return 0;
		}
	}
	if(m_nowString != NULL)
	{
		//fscanf(fp,"%s",m_nowString);
		//strcpy(m_nowString,fp->_base);
		fseek(fp, 0, SEEK_END);
		int count = ftell(fp);
		rewind(fp);
		fread(m_nowString,sizeof(char),count,fp);
		setString(m_nowString);
		fclose(fp);
		return 1;
	}else
	{
		fclose(fp);
		return 0;
	}
}

int _XMultiText::deleteSelectLine()		//删除当前选择的一行
{
	if(m_isInited == 0) return 0;		//如果没有初始化直接退出
	if(m_haveSelect == 0) return 0;
	//找到指定的一行
	int lineSum = 0;
	int lineStart = 0;
	int lineEnd = 0;
	char tempStr[MAX_STRING_LENGTH] = {0};
	memset(tempStr,0,MAX_STRING_LENGTH);
	if(m_nowSelectLineOrder == 0)
	{//第一行
		for(int i = 0;;++ i)
		{		
			if(m_nowString[i] == '\0')
			{//只有一行
				setString("");
				break;
			}
			if(m_nowString[i] == '\n')
			{
				lineEnd = i;
				strcpy(tempStr,m_nowString + i + 1);
				setString(tempStr);
				break;
			}
		/*	if(i >= MAX_LINE_STRING_LENGTH - 1)
			{//超过一行的长度也需要算作换行(这样的结束，没有行结束符'\0')
				lineEnd = i;
				strcpy(tempStr,m_nowString + i);
				setString(tempStr);
				break;
			}*/
		}
	}else
	if(m_nowSelectLineOrder == m_nowMaxLineSum - 1)
	{//最后一行
	//	int lineLength = 0;
		for(int i = 0;;++ i)
		{		
			if(m_nowString[i] == '\0')
			{
				break;
			}else
			if(m_nowString[i] == '\n')
			{
				lineSum++;
	//			lineLength = 0;
				if(lineSum == m_nowSelectLineOrder)
				{
					m_nowString[i] = '\0';
					strcpy(tempStr,m_nowString);
					setString(tempStr);
					if(m_nowMaxLineSum - m_nowShowLineSum < 0)
					{
						setStartLine(0);
						updateLineStr();
					}else
					{
						setStartLine(m_nowMaxLineSum - m_nowShowLineSum);
						updateLineStr();
					}
					break;
				}
			}
	/*		else
			{
				lineLength ++;
				if(lineLength >= MAX_LINE_STRING_LENGTH - 1)
				{//超过一行显示的最大值也需要换行
					lineSum++;
					lineLength = 0;
					if(lineSum == m_nowSelectLineOrder)
					{
						m_nowString[i + 1] = '\0';
						strcpy(tempStr,m_nowString);
						setString(tempStr);
						if(m_nowMaxLineSum - m_nowShowLineSum < 0)
						{
							setStartLine(0);
							updateLineStr();
						}else
						{
							setStartLine(m_nowMaxLineSum - m_nowShowLineSum);
							updateLineStr();
						}
						break;
					}
				}
			}*/
		}
	}else
	{
	//	int lineLength = 0;
		for(int i = 0;;++ i)
		{		
			if(m_nowString[i] == '\0')
			{
				//结束符
				if(lineSum == m_nowSelectLineOrder)
				{
					m_nowString[lineStart] = '\0';
					strcpy(tempStr,m_nowString);
					setString(tempStr);
					if(m_nowMaxLineSum - m_nowShowLineSum < 0)
					{
						setStartLine(0);
						updateLineStr();
					}else
					{
						setStartLine(m_nowMaxLineSum - m_nowShowLineSum);
						updateLineStr();
					}
					break;
				}
			}else
			if(m_nowString[i] == '\n')
			{
				lineSum ++;
	//			lineLength = 0;
				if(lineSum == m_nowSelectLineOrder)
				{//行头
					lineStart = i + 1;
				}
				if(lineSum == m_nowSelectLineOrder + 1)
				{
					m_nowString[lineStart] = '\0';
					int tempStartLine = m_nowStartLine;
					strcpy(tempStr,m_nowString);
					strcpy(tempStr + strlen(m_nowString),&m_nowString[i] + 1);
					setString(tempStr);
					if(m_nowMaxLineSum - m_nowShowLineSum < 0)
					{
						setStartLine(0);
						updateLineStr();
					}else
					if(tempStartLine + m_nowShowLineSum <= m_nowMaxLineSum)
					{
						setStartLine(tempStartLine);
						updateLineStr();
					}else
					{
						setStartLine(m_nowMaxLineSum - m_nowShowLineSum);
						updateLineStr();
					}
					break;
				}
			}
	/*		else
			{
				lineLength ++;
				if(lineLength >= MAX_LINE_STRING_LENGTH - 1)
				{//超过一行显示的最大值也需要换行
					lineSum ++;
					lineLength = 0;
					if(lineSum == m_nowSelectLineOrder)
					{//行头
						lineStart = i + 1;
					}
					if(lineSum == m_nowSelectLineOrder + 1)
					{
						m_nowString[lineStart] = '\0';
						int tempStartLine = m_nowStartLine;
						strcpy(tempStr,m_nowString);
						//因为是超长字符串结束，所以这里不需要将‘\n’计算在内
						//strcpy(tempStr + strlen(m_nowString),&m_nowString[i] + 1);
						strcpy(tempStr + strlen(m_nowString),m_nowString + lineStart + MAX_LINE_STRING_LENGTH - 1);
						setString(tempStr);
						if(m_nowMaxLineSum - m_nowShowLineSum < 0)
						{
							setStartLine(0);
							updateLineStr();
						}else
						if(tempStartLine + m_nowShowLineSum <= m_nowMaxLineSum)
						{
							setStartLine(tempStartLine);
							updateLineStr();
						}else
						{
							setStartLine(m_nowMaxLineSum - m_nowShowLineSum);
							updateLineStr();
						}
						break;
					}
				}
			}*/
		}
	}
	return 1;
}

int _XMultiText::moveUpSelectLine()	//将选择的一行上移
{
	if(m_isInited == 0) return 0;		//如果没有初始化直接退出
	if(m_haveSelect == 0) return 0;
	if(moveUpLine(m_nowSelectLineOrder) == 0)
	{//移动失败
		return 0;
	}else
	{
		m_lineStr[m_nowSelectLineOrder - m_nowStartLine].haveAllSelect = 0;
		m_nowSelectLineOrder --;
		if(m_nowSelectLineOrder < m_nowStartLine)
		{
			m_nowStartLine = m_nowSelectLineOrder;
			updateLineStr();
		}
		m_lineStr[m_nowSelectLineOrder - m_nowStartLine].haveAllSelect = 1;
		m_lineStr[m_nowSelectLineOrder - m_nowStartLine].spriteSelect.setPosition(m_nowMouseRect.left,
			m_nowMouseRect.top + m_textHeight * (m_nowSelectLineOrder - m_nowStartLine));

		m_lineStr[m_nowSelectLineOrder - m_nowStartLine].spriteSelect.setSize(m_nowMouseRect.getWidth() / m_mutiEditSelect->textureSize.x,
						m_textHeight / m_mutiEditSelect->textureSize.y);
		return 1;
	}
}

int _XMultiText::moveUpLine(int order)	//将某一行上移
{
	if(m_isInited == 0) return 0;		//如果没有初始化直接退出
	if(m_haveSelect == 0) return 0;
	if(order <= 0 || order >= m_nowMaxLineSum) return 0;	//非法的值则退出
	int upLineStart = 0;	//上一行开始的位置
	int thisLineStart = 0;	//这一行开始的位置
	int nextLineStart = 0;	//下一行结束的位置
	int lineSum = 0;		//当前的行标
//	int lineLength = 0;		//当前的行宽
	char tempChar0[MAX_LINE_STRING_LENGTH];
	char tempChar1[MAX_LINE_STRING_LENGTH];
	for(int i = 0;;++ i)
	{
		if(m_nowString[i] == '\0')
		{
			lineSum++;
			if(lineSum == order - 1)
			{
				upLineStart = i + 1;	//上一行的行头
			}else
			if(lineSum == order)
			{
				thisLineStart = i + 1;	//当前行的行头
			}else
			if(lineSum == order + 1)
			{//这里不需要复制\0这个字节不然就会出错拉
				//nextLineEnd = i + 1;		//当前行的行尾
				nextLineStart = i;		//当前行的行尾
				//进行字符操作并退出
				memcpy(tempChar0,m_nowString + upLineStart,thisLineStart - upLineStart);
				tempChar0[thisLineStart - upLineStart - 1] = '\0';	//将他的换行符改称结束符
				memcpy(tempChar1,m_nowString + thisLineStart,nextLineStart - thisLineStart);
				tempChar1[nextLineStart - thisLineStart] = '\n';	//给他加一个换行符
				memcpy(m_nowString + upLineStart,tempChar1,nextLineStart - thisLineStart + 1);
				memcpy(m_nowString + upLineStart + nextLineStart - thisLineStart + 1,tempChar0,thisLineStart - upLineStart);
				updateLineStr();
				break;
			}
	//		lineLength = 0;
		}else
		if(m_nowString[i] == '\n')
		{
			lineSum++;
			if(lineSum == order - 1)
			{
				upLineStart = i + 1;	//上一行的行头
			}else
			if(lineSum == order)
			{
				thisLineStart = i + 1;	//当前行的行头
			}else
			if(lineSum == order + 1)
			{
				nextLineStart = i + 1;		//当前行的行尾
				//进行字符操作并退出
				memcpy(tempChar0,m_nowString + upLineStart,thisLineStart - upLineStart);
				tempChar0[thisLineStart - upLineStart] = '\0';
				memcpy(tempChar1,m_nowString + thisLineStart,nextLineStart - thisLineStart);
				tempChar1[nextLineStart - thisLineStart] = '\0';
				memcpy(m_nowString + upLineStart,tempChar1,nextLineStart - thisLineStart);
				memcpy(m_nowString + upLineStart + nextLineStart - thisLineStart,tempChar0,thisLineStart - upLineStart);
				updateLineStr();
				break;
			}
	//		lineLength = 0;
		}
	/*	else
		{
			lineLength ++;
			if(lineLength >= MAX_LINE_STRING_LENGTH - 1)
			{
				lineSum ++;
				if(lineSum == order - 1)
				{
					upLineStart = i + 1;	//上一行的行头
				}else
				if(lineSum == order)
				{
					thisLineStart = i + 1;	//当前行的行头
				}else
				if(lineSum == order + 1)
				{
					nextLineStart = i + 1;		//当前行的行尾
					//进行字符操作并退出
					memcpy(tempChar0,m_nowString + upLineStart,thisLineStart - upLineStart);
					tempChar0[thisLineStart - upLineStart] = '\0';
					memcpy(tempChar1,m_nowString + thisLineStart,nextLineStart - thisLineStart);
					tempChar1[nextLineStart - thisLineStart] = '\0';
					memcpy(m_nowString + upLineStart,tempChar1,nextLineStart - thisLineStart);
					memcpy(m_nowString + upLineStart + nextLineStart - thisLineStart,tempChar0,thisLineStart - upLineStart);
					updateLineStr();
					break;
				}
				lineLength = 0;
			}
		}*/
	}
	return 1;
}

int _XMultiText::moveDownSelectLine()	//将选择的一行下移
{
	if(m_isInited == 0) return 0;		//如果没有初始化直接退出
	if(m_haveSelect == 0) return 0;
	if(moveDownLine(m_nowSelectLineOrder) == 0)
	{//移动失败
		return 0;
	}else
	{
		m_lineStr[m_nowSelectLineOrder - m_nowStartLine].haveAllSelect = 0;
		m_nowSelectLineOrder ++;
		if(m_nowSelectLineOrder >= m_nowStartLine + m_nowShowLineSum)
		{
			m_nowStartLine ++;
			updateLineStr();
		}
		m_lineStr[m_nowSelectLineOrder - m_nowStartLine].haveAllSelect = 1;
		m_lineStr[m_nowSelectLineOrder - m_nowStartLine].spriteSelect.setPosition(m_nowMouseRect.left,
			m_nowMouseRect.top + m_textHeight * (m_nowSelectLineOrder - m_nowStartLine));

		m_lineStr[m_nowSelectLineOrder - m_nowStartLine].spriteSelect.setSize(m_nowMouseRect.getWidth() / m_mutiEditSelect->textureSize.x,
						m_textHeight / m_mutiEditSelect->textureSize.y);
		return 1;
	}
}

int _XMultiText::moveDownLine(int order)	//将某一行下移
{
	if(m_isInited == 0) return 0;		//如果没有初始化直接退出
	if(m_haveSelect == 0) return 0;
	if(order < 0 || order >= m_nowMaxLineSum - 1) return 0;	//非法的值则退出
	int thisLineStart = 0;	//这一行开始的位置
	int nextLineStart = 0;	//下一行开始的位置
	int next2LineStart = 0;	//下一行结束的位置
	int lineSum = 0;		//当前的行标
//	int lineLength = 0;		//当前的行宽
	char tempChar0[MAX_LINE_STRING_LENGTH];
	char tempChar1[MAX_LINE_STRING_LENGTH];
	for(int i = 0;;++ i)
	{
		if(m_nowString[i] == '\0')
		{
			lineSum++;
			if(lineSum == order)
			{
				thisLineStart = i + 1;	//当前行的行头
			}else
			if(lineSum == order + 1)
			{
				nextLineStart = i + 1;		//当前行的行尾
			}else
			if(lineSum == order + 2)
			{//这里不需要复制\0这个字节不然就会出错拉
				//next2LineStart = i + 1;		//当前行的行尾
				next2LineStart = i;		//当前行的行尾
				//进行字符操作并退出
			/*	if(nextLineStart - thisLineStart >= MAX_LINE_STRING_LENGTH - 1)
				{
					memcpy(tempChar0,m_nowString + thisLineStart,nextLineStart - thisLineStart);
					tempChar0[nextLineStart - thisLineStart] = '\0';
					memcpy(tempChar1,m_nowString + nextLineStart,next2LineStart - nextLineStart);
					tempChar1[next2LineStart - nextLineStart] = '\n';
					memcpy(m_nowString + thisLineStart,tempChar1,next2LineStart - nextLineStart + 1);
					memcpy(m_nowString + thisLineStart + next2LineStart - nextLineStart + 1,tempChar0,nextLineStart - thisLineStart + 1);
				}else*/
				{
					memcpy(tempChar0,m_nowString + thisLineStart,nextLineStart - thisLineStart);
					tempChar0[nextLineStart - thisLineStart - 1] = '\0';
					memcpy(tempChar1,m_nowString + nextLineStart,next2LineStart - nextLineStart);
					tempChar1[next2LineStart - nextLineStart] = '\n';
					memcpy(m_nowString + thisLineStart,tempChar1,next2LineStart - nextLineStart + 1);
					memcpy(m_nowString + thisLineStart + next2LineStart - nextLineStart + 1,tempChar0,nextLineStart - thisLineStart);
				}
				updateLineStr();
				break;
			}
	//		lineLength = 0;
		}else
		if(m_nowString[i] == '\n')
		{
			lineSum++;
			if(lineSum == order)
			{
				thisLineStart = i + 1;	//当前行的行头
			}else
			if(lineSum == order + 1)
			{
				nextLineStart = i + 1;		//当前行的行尾
			}else
			if(lineSum == order + 2)
			{
				next2LineStart = i + 1;		//当前行的行尾
				//进行字符操作并退出
				memcpy(tempChar0,m_nowString + thisLineStart,nextLineStart - thisLineStart);
				tempChar0[nextLineStart - thisLineStart] = '\0';
				memcpy(tempChar1,m_nowString + nextLineStart,next2LineStart - nextLineStart);
				tempChar1[next2LineStart - nextLineStart] = '\0';
				memcpy(m_nowString + thisLineStart,tempChar1,next2LineStart - nextLineStart);
				memcpy(m_nowString + thisLineStart + next2LineStart - nextLineStart,tempChar0,nextLineStart - thisLineStart);
				updateLineStr();
				break;
			}
	//		lineLength = 0;
		}
	/*	else
		{
			lineLength ++;
			if(lineLength >= MAX_LINE_STRING_LENGTH - 1)
			{
				lineSum++;
				if(lineSum == order)
				{
					thisLineStart = i + 1;	//当前行的行头
				}else
				if(lineSum == order + 1)
				{
					nextLineStart = i + 1;		//当前行的行尾
				}else
				if(lineSum == order + 2)
				{
					next2LineStart = i + 1;		//当前行的行尾
					//进行字符操作并退出
					memcpy(tempChar0,m_nowString + thisLineStart,nextLineStart - thisLineStart);
					tempChar0[nextLineStart - thisLineStart] = '\0';
					memcpy(tempChar1,m_nowString + nextLineStart,next2LineStart - nextLineStart);
					tempChar1[next2LineStart - nextLineStart] = '\0';
					memcpy(m_nowString + thisLineStart,tempChar1,next2LineStart - nextLineStart);
					memcpy(m_nowString + thisLineStart + next2LineStart - nextLineStart,tempChar0,nextLineStart - thisLineStart);
					updateLineStr();
					break;
				}
				lineLength = 0;
			}
		}*/
	}
	return 1;
}

int _XMultiText::insertALine(const char *str,int lineOrder)	//向文本的指定行插入一行
{
	if(m_isInited == 0) return 0;		//如果没有初始化直接退出
	if(m_haveSelect == 0) return 0;
	if(lineOrder < 0 || lineOrder >= m_nowMaxLineSum) return 0;	//非法的值则退出
	if(str == NULL || strlen(str) == 0) return 0;
	if(strlen(str) + strlen(m_nowString) + 1 >= MAX_STRING_LENGTH) return 0;
	int needAddEnter = 0;
	if(str[strlen(str) - 1] != '\n')
	{//如果加入的字符串不是以回车结束，则这里加入换行符
		if(strlen(str) + strlen(m_nowString) + 2 >= MAX_STRING_LENGTH) return 0;
		needAddEnter = 1;
	}
	int thisLineStart = 0;	//这一行开始的位置
	//int nextLineStart = 0;	//下一行开始的位置
	int lineSum = 0;		//当前的行标
	char tempStr[MAX_STRING_LENGTH];
	if(lineOrder == 0)
	{
		strcpy(tempStr,m_nowString);
		strcpy(m_nowString,str);
		if(needAddEnter == 1)
		{
			strcat(m_nowString,"\n");
		}
		strcat(m_nowString,tempStr);
		checkStr();
		updateLineStr();

	}else
	{
		for(int i = 0;;++ i)
		{
			if(m_nowString[i] == '\0')
			{//如果到这一行还没有进行，那是不可能的，逻辑上
				return 0;
			}else
			if(m_nowString[i] == '\n')
			{
				lineSum ++;
				if(lineSum == lineOrder)
				{
					thisLineStart = i + 1;
					strcpy(tempStr,m_nowString + thisLineStart);
					m_nowString[thisLineStart] = '\0';
					strcat(m_nowString,str);
					if(needAddEnter == 1)
					{
						strcat(m_nowString,"\n");
					}
					strcat(m_nowString,tempStr);
					checkStr();
					updateLineStr();
					break;
				}
			}
		}
	}
	return 1;
}

int _XMultiText::isInRect(float x,float y)
{
	if(m_isInited == 0) return 0;
	return getIsInRect(_XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
}

_XVector2 _XMultiText::getBox(int order)
{
	_XVector2 ret;
	ret.set(0.0f,0.0f);
	if(m_isInited == 0) return ret;
	if(order == 0) ret.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y); else
	if(order == 1) ret.set(m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.top * m_size.y); else
	if(order == 2) ret.set(m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y); else
	if(order == 3) ret.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y); 

	return ret;
}
