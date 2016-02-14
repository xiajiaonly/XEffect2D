#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XProgress.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "XResourcePack.h"
namespace XE{
XProgressSkin::XProgressSkin()
:m_isInited(XFalse)
,progressBackGround(NULL)			//进度条的背景贴图
,progressMove(NULL)				//进度条用移动变化的贴图
,progressUpon(NULL)				//进度条中上层的遮罩或者立体光效
{}
XBool XProgressSkin::init(const char *backgound,const char *move,const char *upon,XResourcePosition resoursePosition)
{
	if(m_isInited ||
		move == NULL) return XFalse;	//进度条最少必须要有中间移动的贴图
	int ret = 1;
	if(backgound != NULL && 
		(progressBackGround = createATextureData(backgound,resoursePosition)) == NULL) ret = 0;
	if(move != NULL && ret != 0 && 
		(progressMove = createATextureData(move,resoursePosition)) == NULL) ret = 0;
	if(upon != NULL && ret != 0 &&
		(progressUpon = createATextureData(upon,resoursePosition)) == NULL) ret = 0;

	if(ret == 0)
	{
		XMem::XDELETE(progressBackGround);
		XMem::XDELETE(progressMove);
		XMem::XDELETE(progressUpon);
		return XFalse;
	}
	m_isInited = XTrue;
	return XTrue;
}
#define PROGRESS_CONFIG_FILENAME "Progress.txt"
bool XProgressSkin::loadFromFolder(const char *filename,XResourcePosition resPos)	//从文件夹中载入资源
{
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf(tempFilename,"%s/%s",filename,PROGRESS_CONFIG_FILENAME);
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
	if((progressBackGround = createATextureData(tempFilename,resPos)) == NULL)
	{//资源读取失败
		fclose(fp);
		return XFalse;
	}
	//down
	if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
	if(flag == 0)
	{//没有普通状态是不行的
		XMem::XDELETE(progressBackGround);
		fclose(fp);
		return XFalse;
	}
	if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
	sprintf(tempFilename,"%s/%s",filename,resFilename);
	if((progressMove = createATextureData(tempFilename,resPos)) == NULL)
	{//资源读取失败
		XMem::XDELETE(progressBackGround);
		fclose(fp);
		return XFalse;
	}
	//on
	if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
	if(flag == 0)
	{//没有普通状态是不行的
		XMem::XDELETE(progressBackGround);
		XMem::XDELETE(progressMove);
		fclose(fp);
		return XFalse;
	}
	if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
	sprintf(tempFilename,"%s/%s",filename,resFilename);
	if((progressUpon = createATextureData(tempFilename,resPos)) == NULL)
	{//资源读取失败
		XMem::XDELETE(progressBackGround);
		XMem::XDELETE(progressMove);
		fclose(fp);
		return XFalse;
	}
	//读取两组数据
	int l,t,r,b,x,y;
	if(fscanf(fp,"%d,%d,%d,%d,",&l,&t,&r,&b) != 4) {fclose(fp);return XFalse;}
	m_mouseRect.set(l,t,r,b);
	if(fscanf(fp,"%d,%d,",&x,&y) != 2) {fclose(fp);return XFalse;}
	m_fontPosition.set(x,y);
	//所有数据读取完成
	fclose(fp);
	return true;
}
bool XProgressSkin::loadFromPacker(const char *filename,XResourcePosition resPos)	//从压缩包中载入资源
{
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf(tempFilename,"%s/%s",filename,PROGRESS_CONFIG_FILENAME);
	unsigned char *p = XResPack.getFileData(tempFilename);
	if(p == NULL) return XFalse;
	//下面开始依次读取数据
	int flag = 0;
	char resFilename[MAX_FILE_NAME_LENGTH] = "";
	//normal
	int offset = 0;
	if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),':') + 1;
	if(flag == 0)
	{//没有普通状态是不行的
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
	sprintf(tempFilename,"%s/%s",filename,resFilename);
	if((progressBackGround = createATextureData(tempFilename,resPos)) == NULL)
	{//资源读取失败
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	//down
	if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),':') + 1;
	if(flag == 0)
	{//没有普通状态是不行的
		XMem::XDELETE(progressBackGround);
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
	sprintf(tempFilename,"%s/%s",filename,resFilename);
	if((progressMove = createATextureData(tempFilename,resPos)) == NULL)
	{//资源读取失败
		XMem::XDELETE(progressBackGround);
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	//on
	if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),':') + 1;
	if(flag == 0)
	{//没有普通状态是不行的
		XMem::XDELETE(progressBackGround);
		XMem::XDELETE(progressMove);
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
	sprintf(tempFilename,"%s/%s",filename,resFilename);
	if((progressUpon = createATextureData(tempFilename,resPos)) == NULL)
	{//资源读取失败
		XMem::XDELETE(progressBackGround);
		XMem::XDELETE(progressMove);
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	//读取两组数据
	int l,t,r,b,x,y;
	if(sscanf((char *)(p + offset),"%d,%d,%d,%d,",&l,&t,&r,&b) != 4) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
	m_mouseRect.set(l,t,r,b);
	if(sscanf((char *)(p + offset),"%d,%d,",&x,&y) != 2) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
	m_fontPosition.set(x,y);
	//所有数据读取完成
	XMem::XDELETE_ARRAY(p);
	return true;
}
bool XProgressSkin::loadFromWeb(const char *filename,XResourcePosition resPos)		//从网页中读取资源
{
	return false;
}
XBool XProgressSkin::initEx(const char *filename,XResourcePosition resoursePosition)
{
	if(m_isInited ||
		filename == NULL) return XFalse;
	//先打开配置文件
	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = getDefResPos();
	switch(resoursePosition)
	{
	case RESOURCE_LOCAL_PACK:
		if(!loadFromPacker(filename,resoursePosition)) return false;
		break;
	case RESOURCE_LOCAL_FOLDER:
		if(!loadFromFolder(filename,resoursePosition)) return false;
		break;
	case RESOURCE_WEB:
		if(!loadFromWeb(filename,resoursePosition)) return false;
		break;
	case RESOURCE_AUTO:
		if(!loadFromPacker(filename,resoursePosition) && !loadFromFolder(filename,resoursePosition) &&
			!loadFromWeb(filename,resoursePosition)) return false;
		break;
	}
	m_isInited = XTrue;
	return XTrue;
}
XBool XProgress::init(const XVector2& position,//控件所在的位置
		const XRect &Area,	//进度条区域的范围
		const XProgressSkin &tex,	//控件的贴图
		XNumber *font,float captionSize,const XVector2& textPosition,	//控件的文字
		int mode)
{
	if(m_isInited ||
		Area.getHeight() <= 0 || Area.getWidth() <= 0 ||	//空间必须要有一个响应区间，不然会出现除零错误
		tex.progressMove == NULL ||//按键的普通状态的贴图不能为空，否则直接返回错误
		captionSize < 0) return XFalse;
	m_mouseRect = Area;
	m_position = position;

	if(mode != 0) m_mode = 1;
	else m_mode = 0;

	m_progressBackGround = tex.progressBackGround;		
	m_progressMove = tex.progressMove;				
	m_progressUpon = tex.progressUpon;	
	m_withoutTex = XFalse;

	m_curValue = 0.0f;
	if(font == NULL) m_isShowFont = XFalse;
	else
	{
		m_caption.setACopy(* font);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&m_caption);
#endif
		m_isShowFont = XTrue;
	}
	m_scale.set(1.0f,1.0f);
	if(m_mode == 0)
	{
		m_curMouseRect.set(m_position.x + m_mouseRect.left * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y,
			m_position.x + m_mouseRect.right * m_scale.x,m_position.y + m_mouseRect.bottom * m_scale.y);
	}else
	{
		m_curMouseRect.set(m_position.x - m_mouseRect.top * m_scale.y,m_position.y + m_mouseRect.left * m_scale.x,
			m_position.x - m_mouseRect.bottom * m_scale.y,m_position.y + m_mouseRect.right * m_scale.x);
	}

	//初始化数字显示的信息
	m_caption.setNumber("0%");
	m_caption.setAlignmentMode(NUMBER_ALIGNMENT_MODE_MIDDLE);
	m_textPosition = textPosition;
	if(m_mode != 0)
	{
		m_caption.setAngle(90);
		m_caption.setPosition(m_position.x - m_textPosition.y * m_scale.y,m_position.y + m_textPosition.x * m_scale.x);
	}else m_caption.setPosition(m_position.x + m_textPosition.x * m_scale.x,m_position.y + m_textPosition.y * m_scale.y);

	m_textSize.set(captionSize,captionSize);
	m_caption.setScale(m_textSize.x * m_scale.x,m_textSize.y * m_scale.y);
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//设置字体的颜色为黑色
	//初始化显示的精灵
	m_spriteBackground.init(m_progressBackGround->texture.m_w,m_progressBackGround->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteBackground);
#endif
	m_spriteBackground.setPosition(m_position);
	m_spriteBackground.setScale(m_scale);
	m_spriteBackground.setIsTransformCenter(POINT_LEFT_TOP);

	m_spriteMove.init(m_progressMove->texture.m_w,m_progressMove->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteMove);
#endif
	m_spriteMove.setPosition(m_position);
	m_spriteMove.setScale(m_scale);
	m_spriteMove.setIsTransformCenter(POINT_LEFT_TOP);
	m_spriteMove.setClipRect(m_mouseRect.left,m_mouseRect.top,m_mouseRect.left + 1.0f,m_mouseRect.bottom);

	m_spriteUpon.init(m_progressUpon->texture.m_w,m_progressUpon->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteUpon);
#endif
	m_spriteUpon.setPosition(m_position);
	m_spriteUpon.setScale(m_scale);
	m_spriteUpon.setIsTransformCenter(POINT_LEFT_TOP);
	if(m_mode != 0)
	{
		m_caption.setAngle(90);
		m_spriteBackground.setAngle(90);
		m_spriteMove.setAngle(90);
		m_spriteUpon.setAngle(90);
	}

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;
	//注册这个控件
	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif

	m_isInited = XTrue;
	return XTrue;
}
XBool XProgress::initWithoutSkin(const XRect &area,
	XNumber* font,float captionSize,	//控件的文字
	const XVector2& textPosition,int mode)
{
	if(m_isInited ||
		captionSize < 0) return XFalse;
	m_mouseRect = area;
	m_position.set(0.0f,0.0f);

	if(mode != 0) m_mode = 1;
	else m_mode = 0;

	m_withoutTex = XTrue;

	m_curValue = 0.0f;
	if(font == NULL) m_isShowFont = XFalse;
	else
	{
		m_caption.setACopy(* font);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&m_caption);
#endif
		m_isShowFont = XTrue;
	}
	m_scale.set(1.0f,1.0f);
	if(m_mode == 0)
	{
		m_curMouseRect.set(m_position.x + m_mouseRect.left * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y,
			m_position.x + m_mouseRect.right * m_scale.x,m_position.y + m_mouseRect.bottom * m_scale.y);
	}else
	{
		m_curMouseRect.set(m_position.x - m_mouseRect.top * m_scale.y,m_position.y + m_mouseRect.left * m_scale.x,
			m_position.x - m_mouseRect.bottom * m_scale.y,m_position.y + m_mouseRect.right * m_scale.x);
	}

	//初始化数字显示的信息
	m_caption.setNumber("0%");
	m_caption.setAlignmentMode(NUMBER_ALIGNMENT_MODE_MIDDLE);
//	m_textPosition = tex->m_fontPosition;
	m_textPosition = textPosition;
	if(m_mode != 0)
	{
		m_caption.setAngle(90);
		m_caption.setPosition(m_position.x - m_textPosition.y * m_scale.y,m_position.y + m_textPosition.x * m_scale.x);
	}else m_caption.setPosition(m_position.x + m_textPosition.x * m_scale.x,m_position.y + m_textPosition.y * m_scale.y);

	m_textSize.set(captionSize,captionSize);
	m_caption.setScale(m_textSize.x * m_scale.x,m_textSize.y * m_scale.y);
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//设置字体的颜色为黑色
	//初始化显示的精灵
	if(m_mode != 0)
	{
		m_caption.setAngle(90);
//		m_spriteBackground.setAngle(90);
//		m_spriteMove.setAngle(90);
//		m_spriteUpon.setAngle(90);
	}

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;
	//注册这个控件
	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif

	m_isInited = XTrue;
	return XTrue;
}
void XProgress::setScale(float x,float y)
{
	if(x <= 0 || y <= 0) return;
	m_scale.set(x,y);
	if(!m_withoutTex)
	{
		m_spriteBackground.setScale(m_scale);
		m_spriteMove.setScale(m_scale);
		m_spriteUpon.setScale(m_scale);
	}
	if(m_mode == 0)
	{
		m_caption.setPosition(m_position.x + m_textPosition.x * m_scale.x,m_position.y + m_textPosition.y * m_scale.y);
		m_curMouseRect.set(m_position.x + m_mouseRect.left * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y,
			m_position.x + m_mouseRect.right * m_scale.x,m_position.y + m_mouseRect.bottom * m_scale.y);
	}else
	{
		m_caption.setPosition(m_position.x - m_textPosition.y * m_scale.y,m_position.y + m_textPosition.x * m_scale.x);
		m_curMouseRect.set(m_position.x - m_mouseRect.top * m_scale.y,m_position.y + m_mouseRect.left * m_scale.x,
			m_position.x - m_mouseRect.bottom * m_scale.y,m_position.y + m_mouseRect.right * m_scale.x);
	}
	m_caption.setScale(m_scale);
}
void XProgress::setPosition(float x,float y)
{
	m_position.set(x,y);
	if(!m_withoutTex)
	{
		m_spriteBackground.setPosition(m_position);
		m_spriteMove.setPosition(m_position);
		m_spriteUpon.setPosition(m_position);
	}
	if(m_mode == 0)
	{
		m_caption.setPosition(m_position.x + m_textPosition.x * m_scale.x,m_position.y + m_textPosition.y * m_scale.y);
		m_curMouseRect.set(m_position.x + m_mouseRect.left * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y,
			m_position.x + m_mouseRect.right * m_scale.x,m_position.y + m_mouseRect.bottom * m_scale.y);
	}else
	{
		m_caption.setPosition(m_position.x - m_textPosition.y * m_scale.y,m_position.y + m_textPosition.x * m_scale.x);
		m_curMouseRect.set(m_position.x - m_mouseRect.top * m_scale.y,m_position.y + m_mouseRect.left * m_scale.x,
			m_position.x - m_mouseRect.bottom * m_scale.y,m_position.y + m_mouseRect.right * m_scale.x);
	}
}
void XProgress::draw()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	if(m_withoutTex)
	{
		XRender::drawFillBoxExA(m_position + XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y),
			XVector2(m_mouseRect.getWidth() * m_scale.x,m_mouseRect.getHeight() * m_scale.y),
			XCCS::downColor * m_color);
		XRender::drawFillBoxExA(m_position + XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y),
			XVector2(m_mouseRect.getWidth() * m_curValue/100.0f * m_scale.x,m_mouseRect.getHeight() * m_scale.y),
			XCCS::onColor * m_color);
		if(m_isShowFont) m_caption.draw();	//显示数字
	}else
	{
		if(m_progressBackGround != NULL) m_spriteBackground.draw(m_progressBackGround);
		if(m_progressMove != NULL) m_spriteMove.draw(m_progressMove);
		if(m_isShowFont) m_caption.draw();	//显示数字
		if(m_progressUpon != NULL) m_spriteUpon.draw(m_progressUpon);
	}
}
void XProgress::setValue(float temp)
{
	if(!m_isInited) return;
	if(temp < 0.0f) temp = 0.0f;
	if(temp > 100.0f) temp = 100.0f;
//	int tempI = temp;
//	if(tempI == (int)m_curValue) return;	//好的性能
	if((int)(m_curValue) != (int)(temp)) 	//好的效果
	{
		char tempC[20] = "100%";
		sprintf(tempC,"%d%%",(int)(temp));
		m_caption.setNumber(tempC);
	}
	if(m_curValue == temp) return;
	m_curValue = temp;
	if(!m_withoutTex)
		m_spriteMove.setClipRect(m_mouseRect.left,m_mouseRect.top,m_mouseRect.left + m_mouseRect.getWidth() * m_curValue/100.0f,m_mouseRect.bottom);
}
XProgress::XProgress()
:m_isInited(XFalse)
,m_progressBackGround(NULL)		
,m_progressMove(NULL)			
,m_progressUpon(NULL)		
,m_curValue(0.0f)
,m_isShowFont(XTrue)
,m_isACopy(XFalse)
,m_resInfo(NULL)
,m_withoutTex(XFalse)
{
	m_ctrlType = CTRL_OBJ_PROGRESS;
}
void XProgress::release()
{
	if(!m_isInited) return;
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
XBool XProgress::setACopy(const XProgress &temp)		//建立一个副本
{
	if(&temp == this) return XFalse;
	if(!temp.m_isInited) return XFalse;
	if(!XControlBasic::setACopy(temp)) return XFalse;

	m_curValue = temp.m_curValue;					//进度条的当前值
	if(!m_isInited)
	{//如果没有经过初始化就进行副本的话这里需要注册控件
		XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
		XObjManager.addAObject(this);
#endif
	}
	m_isInited = temp.m_isInited;					//进度条是否被初始化
	m_mode = temp.m_mode;					//进度条的当前值
	if(temp.m_isShowFont)
	{
		if(!m_caption.setACopy(temp.m_caption))	return XFalse;				//进度条的标题
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&m_caption);
#endif
	}
	if(m_resInfo != NULL) XResManager.releaseResource(m_resInfo);
	m_resInfo = XResManager.copyResource(temp.m_resInfo);
	m_withoutTex = temp.m_withoutTex;

	m_progressBackGround = temp.m_progressBackGround;	//进度条的背景贴图
	m_progressMove = temp.m_progressMove;			//进度条用移动变化的贴图
	m_progressUpon = temp.m_progressUpon;			//进度条中上层的遮罩或者立体光效

	m_spriteBackground.setACopy(temp.m_spriteBackground);	//用于显示背景的精灵
	m_spriteMove.setACopy(temp.m_spriteMove);			//用于显示贴图的精灵
	//m_movePosition = temp.m_movePosition;			//动作条相对于背景的位置
	m_spriteUpon.setACopy(temp.m_spriteUpon);			//用于显示上层遮罩的精灵
	//m_uponPosition = temp.m_uponPosition;			//遮罩相对于背景的位置
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteBackground);
	XObjManager.decreaseAObject(&m_spriteMove);
	XObjManager.decreaseAObject(&m_spriteUpon);
#endif
	m_textPosition = temp.m_textPosition;			//文字显示的位置
	m_textSize = temp.m_textSize;				//文字的尺寸
	m_textColor = temp.m_textColor;				//文字的尺寸

	m_isShowFont = temp.m_isShowFont;				//是否显示数字文字
	m_isACopy = XTrue;
	return XTrue;
}
#if !WITH_INLINE_FILE
#include "XProgress.inl"
#endif
}