//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XCheck.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

_XCheckTexture::_XCheckTexture()
:m_isInited(XFalse)
,checkChoosed(NULL)			//选择按钮选中的贴图		
,checkDischoose(NULL)			//选择按钮未选中的贴图		
,checkDisableChoosed(NULL)		//无效状态下选择按钮选中的贴图		
,checkDisableDischoose(NULL)	//无效状态下选择按钮未选中的贴图	
{}
_XBool _XCheckTexture::init(const char *choosed,const char *disChoose,const char *disableChoosed,const char *disableDischoose,_XResourcePosition resoursePosition)
{
	if(m_isInited) return XFalse;
	//注意这里四种状态的贴图都必须要有，否则将不能初始化
	if(choosed == NULL || disChoose == NULL || disableChoosed == NULL || disableDischoose == NULL) return XFalse;
	int ret = 1;
	
	if((checkChoosed = createATextureData(choosed,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(checkDischoose = createATextureData(disChoose,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(checkDisableChoosed = createATextureData(disableChoosed,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(checkDisableDischoose = createATextureData(disableDischoose,resoursePosition)) == NULL) ret = 0;
	
	if(ret == 0)
	{
		releaseTex();
		return XFalse;
	}

	m_isInited = XTrue;
	return XTrue;
}
#define CHECK_CONFIG_FILENAME ("Check.txt")
_XBool _XCheckTexture::initEx(const char *filename,_XResourcePosition resoursePosition)
{
	if(m_isInited) return XFalse;
	if(filename == NULL) return XFalse;
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf(tempFilename,"%s/%s",filename,CHECK_CONFIG_FILENAME);
	//先打开配置文件
	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = XEE::defaultResourcePosition;
	if(resoursePosition == RESOURCE_LOCAL_FOLDER)
	{//外部资源
		FILE *fp = NULL;
		if((fp = fopen(tempFilename,"r")) == NULL) return XFalse; //信息文件读取失败
		//下面开始依次读取数据
		int flag = 0;
		char resFilename[MAX_FILE_NAME_LENGTH] = "";
		//chooose
		if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
		if(flag == 0)
		{//没有普通状态是不行的
			fclose(fp);
			return XFalse;
		}
		if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((checkChoosed = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//资源读取失败
			fclose(fp);
			return XFalse;
		}
		//dischoose
		if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
		if(flag == 0)
		{//没有普通状态是不行的
			releaseTex();
			fclose(fp);
			return XFalse;
		}
		if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((checkDischoose = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//资源读取失败
			releaseTex();
			fclose(fp);
			return XFalse;
		}
		//on
		if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
		if(flag == 0)
		{//没有普通状态是不行的
			releaseTex();
			fclose(fp);
			return XFalse;
		}
		if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((checkDisableChoosed = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//资源读取失败
			releaseTex();
			fclose(fp);
			return XFalse;
		}
		//disable
		if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
		if(flag == 0)
		{//没有普通状态是不行的
			releaseTex();
			fclose(fp);
			return XFalse;
		}
		if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((checkDisableDischoose = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//资源读取失败
			releaseTex();
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
	}else
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
		if((checkChoosed = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//资源读取失败
			XDELETE_ARRAY(p);
			return XFalse;
		}
		//down
		if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag == 0)
		{//没有普通状态是不行的
			releaseTex();
			XDELETE_ARRAY(p);
			return XFalse;
		}
		if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((checkDischoose = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//资源读取失败
			releaseTex();
			XDELETE_ARRAY(p);
			return XFalse;
		}
		//on
		if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag == 0)
		{//没有普通状态是不行的
			releaseTex();
			XDELETE_ARRAY(p);
			return XFalse;
		}
		if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((checkDisableChoosed = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//资源读取失败
			releaseTex();
			XDELETE_ARRAY(p);
			return XFalse;
		}
		//disable
		if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag == 0)
		{//没有普通状态是不行的
			releaseTex();
			XDELETE_ARRAY(p);
			return XFalse;
		}
		if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((checkDisableDischoose = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//资源读取失败
			releaseTex();
			XDELETE_ARRAY(p);
			return XFalse;
		}
		//读取两组数据
		int l,t,r,b,x,y;
		if(sscanf((char *)(p + offset),"%d,%d,%d,%d,",&l,&t,&r,&b) != 4) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		m_mouseRect.set(l,t,r,b);
		if(sscanf((char *)(p + offset),"%d,%d,",&x,&y) != 2) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		m_fontPosition.set(x,y);
		//所有数据读取完成
		XDELETE_ARRAY(p);
	}
	m_isInited = XTrue;
	return XTrue;
}

_XCheck::_XCheck()
:m_isInited(XFalse)					//进度条是否被初始化
,m_checkChoosed(NULL)			//选择按钮选中的贴图
,m_checkDischoose(NULL)		//选择按钮未选中的贴图
,m_checkDisableChoosed(NULL)	//无效状态下选择按钮选中的贴图
,m_checkDisableDischoose(NULL)	//无效状态下选择按钮未选中的贴图
,m_funInit(NULL)				//控件初始化的时候调用，（这个目前没有实际生效）
,m_funRelease(NULL)				//控件注销的时候调用，（这个目前没有实际生效）
,m_funMouseOn(NULL)				//鼠标悬浮时调用
,m_funMouseDown(NULL)			//鼠标按下时调用		
,m_funMouseUp(NULL)				//鼠标弹起时调用
,m_funStateChange(NULL)			//控件状态改变时调用
,m_pClass(NULL)
,m_state(XFalse)
,m_withCaption(XTrue)
,m_pVariable(NULL)
,m_resInfo(NULL)
,m_withoutTex(XFalse)
{
	m_ctrlType = CTRL_OBJ_CHECK;
}
void _XCheck::release()
{
	_XCtrlManger.decreaseAObject(this);	//注销这个物件
	if(m_funRelease != NULL) m_funRelease(m_pClass,m_objectID);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(this);
#endif
	if(m_resInfo != NULL)
	{
		_XResourceManager::GetInstance().releaseResource(m_resInfo);
		m_resInfo = NULL;
	}
}
//注意：这里默认四个状态的贴图尺寸一致，否则将会出现意想不到的问题
_XBool _XCheck::init(const _XVector2 & position,	//控件的位置
		const _XRect &Area,					//控件的图片的响应范围
		const _XCheckTexture &tex,			//控件的贴图
		const char *caption,const _XFontUnicode &font,float captionSize,	//显示的字体的相关信息
		const _XVector2 &textPosition)		//字体的位置
{
	if(m_isInited) return XFalse;
	if(Area.getHeight() <= 0 || Area.getWidth() <= 0) return XFalse;	//空间必须要有一个响应区间，不然会出现除零错误
	m_position = position;
	m_mouseRect = Area;

	if(tex.checkChoosed == NULL || tex.checkDisableChoosed == NULL 
		|| tex.checkDisableDischoose == NULL || tex.checkDischoose == NULL) return XFalse;
	m_checkChoosed = tex.checkChoosed;			//选择按钮选中的贴图
	m_checkDischoose = tex.checkDischoose;		//选择按钮未选中的贴图
	m_checkDisableChoosed = tex.checkDisableChoosed;	//无效状态下选择按钮选中的贴图
	m_checkDisableDischoose = tex.checkDisableDischoose;	//无效状态下选择按钮未选中的贴图
	m_withoutTex = XFalse;
	m_comment.init();

	m_caption.setACopy(font);			//进度条的标题
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //设置字体居中对齐
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//设置字体的颜色为黑色

	m_size.set(1.0f,1.0f);
	m_textPosition = textPosition;		//文字的相对位置

	m_sprite.init(m_checkChoosed->texture.m_w,m_checkChoosed->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_sprite);
#endif
	m_sprite.setPosition(m_position);
	m_sprite.setSize(m_size);
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);

	m_caption.setString(caption);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	if(captionSize < 0) return XFalse;
	m_textSize.set(captionSize,captionSize);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);

	m_mouseClickArea.set(m_caption.getBox(0).x,m_caption.getBox(0).y,m_caption.getBox(2).x,m_caption.getBox(2).y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);

	m_state = XFalse;				//复选框的选择状态
	if(m_funInit != NULL) m_funInit(m_pClass,m_objectID);
	stateChange();

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XCtrlManger.addACtrl(this);
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XCheck::initWithoutTex(const char *caption,const _XFontUnicode &font,
	float captionSize,const _XRect& area,const _XVector2 &captionPosition)
{//尚未完成
	if(m_isInited) return XFalse;

	m_position.set(0.0f,0.0f);
	m_mouseRect = area;

	m_withoutTex = XTrue;
	m_comment.init();

	m_caption.setACopy(font);			//进度条的标题
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //设置字体居中对齐
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//设置字体的颜色为黑色

	m_size.set(1.0f,1.0f);
	m_textPosition = captionPosition;		//文字的相对位置

	m_caption.setString(caption);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	if(captionSize < 0) return XFalse;
	m_textSize.set(captionSize,captionSize);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);

	m_mouseClickArea.set(m_caption.getBox(0).x,m_caption.getBox(0).y,m_caption.getBox(2).x,m_caption.getBox(2).y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);

	m_state = XFalse;				//复选框的选择状态
	if(m_funInit != NULL) m_funInit(m_pClass,m_objectID);
	stateChange();

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XCtrlManger.addACtrl(this);
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
void _XCheck::draw()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	if(m_withoutTex)
	{
		if(m_state)
		{
			if(m_isEnable) 
			{//画个X
				drawFillBoxExA(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),
					_XVector2(m_mouseRect.getWidth() * m_size.x,m_mouseRect.getHeight() * m_size.y),0.75f * m_color.fR,0.75f * m_color.fG,0.75f * m_color.fB,m_color.fA,true);
				drawLine(m_position.x + m_mouseRect.left * m_size.x + 2,
					m_position.y + m_mouseRect.top * m_size.y + 2,
					m_position.x + m_mouseRect.right * m_size.x - 2,
					m_position.y + m_mouseRect.bottom * m_size.y - 2,1,0.5f * m_color.fR,0.5f * m_color.fG,0.5f * m_color.fB,m_color.fA);
				drawLine(m_position.x + m_mouseRect.left * m_size.x + 2,
					m_position.y + m_mouseRect.bottom * m_size.y - 2,
					m_position.x + m_mouseRect.right * m_size.x - 2,
					m_position.y + m_mouseRect.top * m_size.y + 2,1,0.5f * m_color.fR,0.5f * m_color.fG,0.5f * m_color.fB,m_color.fA);
			}else 
			{
				drawFillBoxExA(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),
					_XVector2(m_mouseRect.getWidth() * m_size.x,m_mouseRect.getHeight() * m_size.y),0.5f * m_color.fR,0.5f * m_color.fG,0.5f * m_color.fB,m_color.fA,true);
				drawLine(m_position.x + m_mouseRect.left * m_size.x + 2,
					m_position.y + m_mouseRect.top * m_size.y + 2,
					m_position.x + m_mouseRect.right * m_size.x - 2,
					m_position.y + m_mouseRect.bottom * m_size.y - 2,1,0.25f * m_color.fR,0.25f * m_color.fG,0.25f * m_color.fB,m_color.fA);
				drawLine(m_position.x + m_mouseRect.left * m_size.x + 2,
					m_position.y + m_mouseRect.bottom * m_size.y - 2,
					m_position.x + m_mouseRect.right * m_size.x - 2,
					m_position.y + m_mouseRect.top * m_size.y + 2,1,0.25f * m_color.fR,0.25f * m_color.fG,0.25f * m_color.fB,m_color.fA);
			}
		}else
		{
			if(m_isEnable) 
			{
				drawFillBoxExA(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),
					_XVector2(m_mouseRect.getWidth() * m_size.x,m_mouseRect.getHeight() * m_size.y),0.85f * m_color.fR,0.85f * m_color.fG,0.85f * m_color.fB,m_color.fA,true);
			}else 
			{
				drawFillBoxExA(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),
					_XVector2(m_mouseRect.getWidth() * m_size.x,m_mouseRect.getHeight() * m_size.y),0.5f * m_color.fR,0.5f * m_color.fG,0.5f * m_color.fB,m_color.fA,true);
			}
		}
	}else
	{
		if(m_state)
		{
			if(m_isEnable) m_sprite.draw(m_checkChoosed);
			else m_sprite.draw(m_checkDisableChoosed);
		}else
		{
			if(m_isEnable) m_sprite.draw(m_checkDischoose);
			else m_sprite.draw(m_checkDisableDischoose);
		}
	}
	if(m_withCaption) m_caption.draw();	//如果需要则描绘文字内容
}
void _XCheck::setPosition(float x,float y)
{
	if(!m_isInited) return;	//如果没有初始化直接退出
	m_position.set(x,y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	if(!m_withoutTex) m_sprite.setPosition(m_position);
	m_mouseClickArea.set(m_caption.getBox(0).x,m_caption.getBox(0).y,m_caption.getBox(2).x,m_caption.getBox(2).y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	updateChildPos();
}
void _XCheck::setSize(float x,float y)
{
	if(x <= 0 || y <= 0 ||
		!m_isInited) return;	//如果没有初始化直接退出
	m_size.set(x,y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	if(!m_withoutTex) m_sprite.setSize(m_size);
	m_mouseClickArea.set(m_caption.getBox(0).x,m_caption.getBox(0).y,m_caption.getBox(2).x,m_caption.getBox(2).y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	updateChildSize();
}
_XBool _XCheck::mouseProc(float x,float y,_XMouseState mouseState)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	if(m_withAction && m_isInAction) return XFalse;	//如果支持动作播放而且正在播放动画那么不接受鼠标控制

	if((m_withCaption && m_mouseClickArea.isInRect(x,y)) || m_nowMouseRect.isInRect(x,y))
	{//操作有效，执行相关操作
		if(!m_isMouseInRect)
		{
			m_isMouseInRect = XTrue;
			m_comment.setShow();
			setCommentPos(x,y + 16.0f);
		}
		if(mouseState == MOUSE_LEFT_BUTTON_UP)
		{
			m_state = !m_state;
			if(m_pVariable != NULL) *m_pVariable = m_state;
			if(m_funMouseUp != NULL) m_funMouseUp(m_pClass,m_objectID);
			if(m_funStateChange != NULL) m_funStateChange(m_pClass,m_objectID);
			m_isBeChoose = XTrue;	//控件处于焦点状态
			stateChange();
			if(m_withAction)
			{//这里测试一个动态效果
			//	m_isInAction = XTrue;
				m_lightMD.set(1.0f,2.0f,0.002f,MOVE_DATA_MODE_SIN_MULT);
				m_oldPos = m_position;
				m_oldSize = m_size;
			}
			return XTrue;
		}
	}else
	{
		if(m_isMouseInRect)
		{
			m_isMouseInRect = XFalse;
			m_comment.disShow();
		}
	}
	return XFalse;
}
_XBool _XCheck::keyboardProc(int keyOrder,_XKeyState keyState)
{//回车或者空格可以改变这个控件的选择状态
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	if(m_withAction && m_isInAction) return XFalse;	//如果支持动作播放而且正在播放动画那么不接受鼠标控制

	if(keyState == KEY_STATE_UP)
	{//按键弹起时才作相应
		if((keyOrder == XKEY_RETURN || keyOrder == XKEY_SPACE) && m_isBeChoose)
		{//按下空格键或者回车键都有效
			m_state = !m_state;
			if(m_pVariable != NULL) *m_pVariable = m_state;
			if(m_funStateChange != NULL) m_funStateChange(m_pClass,m_objectID);
			stateChange();
		}
	}
	return XTrue;
}
void _XCheck::setCallbackFun(void (* funInit)(void *,int),
							 void (* funRelease)(void *,int),
							 void (* funMouseOn)(void *,int),
							 void (* funMouseDown)(void *,int),
							 void (* funMouseUp)(void *,int),
							 void (*funStateChange)(void *,int),void *pClass)
{//实际上这里可以不用先判断，可以允许调用者注销
	m_funInit = funInit;				//控件初始化的时候调用，（这个目前没有实际生效）
	m_funRelease = funRelease;				//控件注销的时候调用，（这个目前没有实际生效）
	m_funMouseOn = funMouseOn;				//鼠标悬浮时调用
	m_funMouseDown = funMouseDown;			//鼠标按下时调用		
	m_funMouseUp = funMouseUp;				//鼠标弹起时调用
	m_funStateChange = funStateChange;			//控件状态改变时调用
	if(pClass != NULL) m_pClass = pClass;
	else m_pClass = this;
}
_XBool _XCheck::setACopy(const _XCheck &temp)
{
	if(& temp == this) return XTrue;	//防止自身赋值
	if(!temp.m_isInited) return XFalse;
	if(!_XControlBasic::setACopy(temp)) return XFalse;

	if(!m_isInited)
	{
		_XCtrlManger.addACtrl(this);
#if WITH_OBJECT_MANAGER
		_XObjManger.addAObject(this);
#endif
	}

	m_isInited = temp.m_isInited;					//进度条是否被初始化
	if(!m_caption.setACopy(temp.m_caption))	return XFalse;		//进度条的标题
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_caption);
#endif

	m_pVariable = temp.m_pVariable;
	if(m_resInfo != NULL) _XResourceManager::GetInstance().releaseResource(m_resInfo);
	m_resInfo = _XResourceMng.copyResource(temp.m_resInfo);
	m_withoutTex = temp.m_withoutTex;

	m_checkChoosed = temp.m_checkChoosed;			//选择按钮选中的贴图
	m_checkDischoose = temp.m_checkDischoose;		//选择按钮未选中的贴图
	m_checkDisableChoosed = temp.m_checkDisableChoosed;	//无效状态下选择按钮选中的贴图
	m_checkDisableDischoose = temp.m_checkDisableDischoose;	//无效状态下选择按钮未选中的贴图

	m_funInit = temp.m_funInit;				//控件初始化的时候调用，（这个目前没有实际生效）
	m_funRelease = temp.m_funRelease;				//控件注销的时候调用，（这个目前没有实际生效）
	m_funMouseOn = temp.m_funMouseOn;				//鼠标悬浮时调用，（这个目前没有实际生效）
	m_funMouseDown = temp.m_funMouseDown;			//鼠标按下时调用，（这个目前没有实际生效）		
	m_funMouseUp = temp.m_funMouseUp;				//鼠标弹起时调用
	m_funStateChange = temp.m_funStateChange;			//控件状态改变时调用
	m_pClass = temp.m_pClass;

	m_withCaption = temp.m_withCaption;
	m_sprite.setACopy(temp.m_sprite);			//用于显示贴图的精灵
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_sprite);
#endif
	m_textPosition = temp.m_textPosition;		//文字的相对位置
	m_textSize = temp.m_textSize;			//文字的尺寸

	m_state = temp.m_state;				//复选框的选择状态
	m_textColor = temp.m_textColor;				//复选框的选择状态
	m_mouseClickArea = temp.m_mouseClickArea;	//鼠标点击的响应范围

	m_oldPos = temp.m_oldPos;				//动作播放时的位置
	m_oldSize = temp.m_oldSize;			//动作播放时的大小
	m_lightMD = temp.m_lightMD;
	m_lightRect = temp.m_lightRect;

	return XTrue;
}
_XVector2 _XCheck::getBox(int order)
{
	if(!m_isInited) return _XVector2::zero;
	float left = 0.0f;
	float right = 0.0f;
	float top = 0.0f;
	float bottom = 0.0f;
	if(m_withCaption)
	{
		if(m_nowMouseRect.left <= m_mouseClickArea.left) left = m_nowMouseRect.left;
		else left = m_mouseClickArea.left;
		if(m_nowMouseRect.top <= m_mouseClickArea.top) top = m_nowMouseRect.top;
		else top = m_mouseClickArea.top;
		if(m_nowMouseRect.right <= m_mouseClickArea.right) right = m_mouseClickArea.right;
		else right = m_nowMouseRect.right;
		if(m_nowMouseRect.bottom <= m_mouseClickArea.bottom) bottom = m_mouseClickArea.bottom;
		else bottom = m_nowMouseRect.bottom;
	}else
	{
		left = m_nowMouseRect.left;
		top = m_nowMouseRect.top;
		right = m_mouseClickArea.right;
		bottom = m_mouseClickArea.bottom;
	}

	switch(order)
	{
	case 0: return _XVector2(left,top);
	case 1: return _XVector2(right,top);
	case 2: return _XVector2(right,bottom);
	case 3: return _XVector2(left,bottom);
	}

	return _XVector2::zero;
}
void _XCheck::update(int stepTime)
{
	m_comment.update(stepTime);
	//if(m_isInAction)
	//{//处于动作过程中计算动作的实施
	//	m_actionMoveData.move(stepTime);
	//	if(m_actionMoveData.getIsEnd()) m_isInAction = false;	//动作播放完成
	//	setSize(m_actionMoveData.getNowData() * m_oldSize);
	//	_XVector2 tmp(m_mouseRect.getWidth(),m_mouseRect.getHeight());
	//	tmp = tmp * (m_actionMoveData.getNowData() * m_oldSize - m_oldSize) * 0.5f;
	//	setPosition(m_oldPos - tmp);
	//}
	if(!m_lightMD.getIsEnd())
	{
		m_lightMD.move(stepTime);
		_XVector2 pos(m_oldPos.x + m_mouseRect.getWidth() * 0.5f * m_size.x,m_oldPos.y + m_mouseRect.getHeight() * 0.5f * m_size.y);
		_XVector2 size(m_mouseRect.getWidth() * m_oldSize.x * m_lightMD.getNowData() * 0.5f,
			m_mouseRect.getHeight() * m_oldSize.y * m_lightMD.getNowData() * 0.5f);
		m_lightRect.set(pos.x - size.x,pos.y - size.y,pos.x + size.x,pos.y + size.y);
	}
}