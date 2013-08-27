//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XProgress.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

_XProgressTexture::_XProgressTexture()
:m_isInited(0)
,progressBackGround(NULL)			//进度条的背景贴图
,progressMove(NULL)				//进度条用移动变化的贴图
,progressUpon(NULL)				//进度条中上层的遮罩或者立体光效
{
}

_XProgressTexture::~_XProgressTexture()
{
	release();
}

int _XProgressTexture::init(const char *backgound,const char *move,const char *upon,int resoursePosition)
{
	if(m_isInited != 0) return 0;
	if(move == NULL) return 0;	//进度条最少必须要有中间移动的贴图
	int ret = 1;
	if(backgound != NULL && 
		(progressBackGround = createATextureData(backgound,resoursePosition)) == NULL) ret = 0;
	if(move != NULL && ret != 0 && 
		(progressMove = createATextureData(move,resoursePosition)) == NULL) ret = 0;
	if(upon != NULL && ret != 0 &&
		(progressUpon = createATextureData(upon,resoursePosition)) == NULL) ret = 0;

	if(ret == 0)
	{
		XDELETE(progressBackGround);
		XDELETE(progressMove);
		XDELETE(progressUpon);
		return 0;
	}
	m_isInited = 1;
	return 1;
}

void _XProgressTexture::release()
{
	if(m_isInited == 0) return;
	XDELETE(progressBackGround);
	XDELETE(progressMove);
	XDELETE(progressUpon);
	m_isInited = 0;
}

int _XProgress::init(const _XVector2& position,//控件所在的位置
		const _XRect &Area,	//进度条区域的范围
		const _XProgressTexture &tex,	//控件的贴图
		_XNumber *font,float captionSize,const _XVector2& textPosition,	//控件的文字
		int mode)
{
	if(m_isInited != 0) return 0;
	if(Area.getHeight() <= 0 || Area.getWidth() <= 0) return 0;	//空间必须要有一个响应区间，不然会出现除零错误
	if(tex.progressMove == NULL) return 0;//按键的普通状态的贴图不能为空，否则直接返回错误
	if(captionSize < 0) return 0;
	m_mouseRect = Area;
	m_position = position;

	if(mode != 0) m_mode = 1;
	else m_mode = 0;

	m_progressBackGround = tex.progressBackGround;		
	m_progressMove = tex.progressMove;				
	m_progressUpon = tex.progressUpon;		

	m_nowValue = 0.0f;
	if(font == NULL) m_isShowFont = 0;
	else
	{
		m_caption.setACopy(* font);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
		m_isShowFont = 1;
	}
	m_size.set(1.0f,1.0f);
	if(m_mode == 0)
	{
		m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
			m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	}else
	{
		m_nowMouseRect.set(m_position.x - m_mouseRect.top * m_size.y,m_position.y + m_mouseRect.left * m_size.x,
			m_position.x - m_mouseRect.bottom * m_size.y,m_position.y + m_mouseRect.right * m_size.x);
	}

	//初始化数字显示的信息
	m_caption.setNumber("0%");
	m_caption.setAlignmentMode(_NUMBER_ALIGNMENT_MODE_MIDDLE);
	m_textPosition = textPosition;
	if(m_mode != 0)
	{
		m_caption.setAngle(90);
		m_caption.setPosition(m_position.x - m_textPosition.y * m_size.y,m_position.y + m_textPosition.x * m_size.x);
	}else m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);

	m_textSize.set(captionSize,captionSize);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//设置字体的颜色为黑色
	//初始化显示的精灵
	m_spriteBackground.init(m_progressBackGround->texture.m_w,m_progressBackGround->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteBackground);
#endif
	m_spriteBackground.setPosition(m_position);
	m_spriteBackground.setSize(m_size);
	m_spriteBackground.setIsTransformCenter(POINT_LEFT_TOP);

	m_spriteMove.init(m_progressMove->texture.m_w,m_progressMove->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteMove);
#endif
	m_spriteMove.setPosition(m_position);
	m_spriteMove.setSize(m_size);
	m_spriteMove.setIsTransformCenter(POINT_LEFT_TOP);
	m_spriteMove.setClipRect(m_mouseRect.left,m_mouseRect.top,m_mouseRect.left + 1.0f,m_mouseRect.bottom);

	m_spriteUpon.init(m_progressUpon->texture.m_w,m_progressUpon->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteUpon);
#endif
	m_spriteUpon.setPosition(m_position);
	m_spriteUpon.setSize(m_size);
	m_spriteUpon.setIsTransformCenter(POINT_LEFT_TOP);
	if(m_mode != 0)
	{
		m_caption.setAngle(90);
		m_spriteBackground.setAngle(90);
		m_spriteMove.setAngle(90);
		m_spriteUpon.setAngle(90);
	}

	m_isVisiable = 1;
	m_isEnable = 1;
	m_isActive = 1;
	//注册这个控件
	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_PROGRESS);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif

	m_isInited = 1;
	return 1;
}

void _XProgress::setSize(float x,float y)
{
	if(x <= 0 && y <= 0) return;
	m_size.set(x,y);
	m_spriteBackground.setSize(m_size);
	m_spriteMove.setSize(m_size);
	m_spriteUpon.setSize(m_size);
	if(m_mode == 0)
	{
		m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
		m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
			m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	}else
	{
		m_caption.setPosition(m_position.x - m_textPosition.y * m_size.y,m_position.y + m_textPosition.x * m_size.x);
		m_nowMouseRect.set(m_position.x - m_mouseRect.top * m_size.y,m_position.y + m_mouseRect.left * m_size.x,
			m_position.x - m_mouseRect.bottom * m_size.y,m_position.y + m_mouseRect.right * m_size.x);
	}
	m_caption.setSize(m_size);
}

void _XProgress::setPosition(float x,float y)
{
	m_position.set(x,y);
	m_spriteBackground.setPosition(m_position);
	m_spriteMove.setPosition(m_position);
	m_spriteUpon.setPosition(m_position);
	if(m_mode == 0)
	{
		m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
		m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
			m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	}else
	{
		m_caption.setPosition(m_position.x - m_textPosition.y * m_size.y,m_position.y + m_textPosition.x * m_size.x);
		m_nowMouseRect.set(m_position.x - m_mouseRect.top * m_size.y,m_position.y + m_mouseRect.left * m_size.x,
			m_position.x - m_mouseRect.bottom * m_size.y,m_position.y + m_mouseRect.right * m_size.x);
	}
}

void _XProgress::draw()
{
	if(m_isInited == 0) return ;	//如果没有初始化直接退出
	if(m_isVisiable == 0) return;	//如果不可见直接退出
	if(m_progressBackGround != NULL) m_spriteBackground.draw(m_progressBackGround);
	if(m_progressMove != NULL) m_spriteMove.draw(m_progressMove);
	if(m_isShowFont != 0) m_caption.draw();	//显示数字
	if(m_progressUpon != NULL) m_spriteUpon.draw(m_progressUpon);
}

void _XProgress::setValue(float temp)
{
	if(m_isInited == 0) return;
	if(temp < 0.0f) temp = 0.0f;
	if(temp > 100.0f) temp = 100.0f;
	int tempI = temp;
	if(tempI == (int)m_nowValue) return;
	m_nowValue = temp;
	char tempC[20] = "100%";
	sprintf(tempC,"%d%%",tempI);
	m_caption.setNumber(tempC);

	m_spriteMove.setClipRect(m_mouseRect.left,m_mouseRect.top,m_mouseRect.left + m_mouseRect.getWidth() * m_nowValue/100.0f,m_mouseRect.bottom);
}

_XProgress::_XProgress()
:m_isInited(0)
,m_progressBackGround(NULL)		
,m_progressMove(NULL)			
,m_progressUpon(NULL)		
,m_nowValue(0.0f)
,m_isShowFont(1)
,m_isACopy(0)
{
}

_XProgress::~_XProgress()
{
	release();
}

void _XProgress::release()
{
	if(m_isInited == 0) return;
	_XControlManager::GetInstance().decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(this);
#endif
	m_isInited = 0;
}

int _XProgress::setACopy(const _XProgress &temp)		//建立一个副本
{
	if(&temp == this) return 0;
	if(temp.m_isInited == 0) return 0;
	if(_XControlBasic::setACopy(temp) == 0) return 0;

	m_nowValue = temp.m_nowValue;					//进度条的当前值
	if(m_isInited == 0)
	{//如果没有经过初始化就进行副本的话这里需要注册控件
		_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_PROGRESS);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	}
	m_isInited = temp.m_isInited;					//进度条是否被初始化
	m_mode = temp.m_mode;					//进度条的当前值
	if(temp.m_isShowFont != 0)
	{
		if(m_caption.setACopy(temp.m_caption) == 0)	return 0;				//进度条的标题
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	}

	m_progressBackGround = temp.m_progressBackGround;	//进度条的背景贴图
	m_progressMove = temp.m_progressMove;			//进度条用移动变化的贴图
	m_progressUpon = temp.m_progressUpon;			//进度条中上层的遮罩或者立体光效

	m_spriteBackground.setACopy(temp.m_spriteBackground);	//用于显示背景的精灵
	m_spriteMove.setACopy(temp.m_spriteMove);			//用于显示贴图的精灵
	//m_movePosition = temp.m_movePosition;			//动作条相对于背景的位置
	m_spriteUpon.setACopy(temp.m_spriteUpon);			//用于显示上层遮罩的精灵
	//m_uponPosition = temp.m_uponPosition;			//遮罩相对于背景的位置
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteBackground);
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteMove);
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteUpon);
#endif
	m_textPosition = temp.m_textPosition;			//文字显示的位置
	m_textSize = temp.m_textSize;				//文字的尺寸
	m_textColor = temp.m_textColor;				//文字的尺寸

	m_isShowFont = temp.m_isShowFont;				//是否显示数字文字
	m_isACopy = 1;
	return 1;
}

int _XProgress::isInRect(float x,float y)
{
	if(m_isInited == 0) return 0;
	return getIsInRect(_XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
}

_XVector2 _XProgress::getBox(int order)
{
	_XVector2 ret;
	ret.set(0.0f,0.0f);
	if(m_isInited == 0) return ret;
	if(order == 0) ret.set(m_nowMouseRect.left,m_nowMouseRect.top);else
	if(order == 1) ret.set(m_nowMouseRect.right,m_nowMouseRect.top);else
	if(order == 2) ret.set(m_nowMouseRect.right,m_nowMouseRect.bottom);else
	if(order == 3) ret.set(m_nowMouseRect.left,m_nowMouseRect.bottom);
	return ret;
}
