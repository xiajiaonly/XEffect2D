//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "glew.h"	//由于需要使用shader所以需要这个文件，具体需要磋商
#include "XSprite.h"
#include "XBasicSDL.h"
#include "XBasicOpenGL.h"
#include "XResourcePack.h"
#include "XLogBook.h"
#include "XTextureInformation.h"
#include "XObjectManager.h"

int _XSprite::init(const char *filename,					//图片的名称
		int resoursePosition,						//资源的位置
		_XTransformMode isTransformCenter)					//是否使用中心旋转	
{
	if(m_isInited != 0) 
	{
		AddLogInfoStr("The action have initted!\n");
		return 1;
	}

	m_positionX = 0.0f;//精灵的位置
	m_positionY = 0.0f;
	angle = 0.0f;	//精灵的角度
	sinAngle = 0.0f;
	cosAngle = 1.0f;
	xsize = 1.0f;		//精灵的缩放比例
	ysize = 1.0f;
	//设置资源的位置
	if(resoursePosition != RESOURCE_OUTSIDE)
	{
		resoursePosition = RESOURCE_INSIDE;
	}
	m_resoursePosition = resoursePosition;
	setIsTransformCenter(isTransformCenter);

	if(m_textureData.load(filename,m_resoursePosition) == 0) return 0;
	//方案3
	updateClipAndRotatoData();
	//方案2
	//if(m_isEnableOutsideChip == 0)
	//{//如果没有进行外部裁剪，则这里进行计算
	//	setClipRect(0.0f,0.0f,m_textureData.textureSize.x,m_textureData.textureSize.y);
	//	m_isEnableOutsideChip = 0;
	//}else
	//{//如果已经进行了外部裁剪，则这里更新数据
	//	setClipRect(m_clipOutsideRect);
	//}
	//方案1
	//if(m_textureData.isEnableInsideClip == 1)
	//{
	////	init(withAlpha,m_texInfo.texture.m_w,m_texInfo.texture.m_h,0);
	//	m_clipRect = m_textureData.clipInsideRect;

	//	if(m_isTransformCenter == POINT_CENTER)
	//	{
	//		m_transformCenter.set((m_clipRect.getWidth() + m_textureData.textureMove.x + m_textureData.textureMove2.x) * 0.5f - m_textureData.textureMove.x,
	//			(m_clipRect.getHeight() + m_textureData.textureMove.y + m_textureData.textureMove2.y) * 0.5f - m_textureData.textureMove.y);
	//	}else
	//	{
	//		m_transformCenter.set(-m_textureData.textureMove.x,-m_textureData.textureMove.y);
	//	}
	////	return 1;
	//}else
	//{
	//	if(m_isTransformCenter == POINT_CENTER)
	//	{
	//		m_transformCenter.set(m_textureData.texture.m_w * 0.5f,m_textureData.texture.m_h * 0.5f);
	//	}else
	//	{
	//		m_transformCenter.set(0.0f,0.0f);
	//	}
	//}

	//载入完成之后进行相关的计算
	if(isNPOT(m_textureData.texture.m_w, m_textureData.texture.m_h)) 
	{
		wr = (int)powf(2.0, ceilf(logf((float)m_textureData.texture.m_w)/logf(2.0f)));
		hr = (int)powf(2.0, ceilf(logf((float)m_textureData.texture.m_h)/logf(2.0f)));
		xp = (wr - m_textureData.texture.m_w)/2;
		yp = (hr - m_textureData.texture.m_h)/2;
	}else 
	{
		wr = m_textureData.texture.m_w;
		hr = m_textureData.texture.m_h;
		xp= 0;
		yp= 0;
	}
#if IS_USE_SOLIDIFY
	m_needSolidify = 1;
#endif
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_SPRITE);
#endif

	m_isInited = 1;
	return 1;
}
//needSizeCheck 是否需要对尺寸进行2的n次访的检查，如果本身的贴图没有经过2的N次访的检查这里就不需要进行2的n次访的检查
//否则需要进行这个检查
int _XSprite::init(int tempW,int tempH,int needSizeCheck,_XTransformMode isTransformCenter)
{
	if(m_isInited != 0) 
	{
		AddLogInfoStr("The action have initted!\n");
		return 1;
	}

//	x = 0;			//精灵的位置
//	y = 0;
	m_positionX = 0.0f;//精灵的位置
	m_positionY = 0.0f;
	angle = 0.0f;	//精灵的角度
	sinAngle = 0.0f;
	cosAngle = 1.0f;
	xsize = 1.0f;		//精灵的缩放比例
	ysize = 1.0f;
	setIsTransformCenter(isTransformCenter);
	
	if(m_textureData.loadEmpty() == 0) return 0;

	m_textureData.texture.m_w = tempW;
	m_textureData.texture.m_h = tempH;
	//载入完成之后进行相关的计算
	if(needSizeCheck == 1 && isNPOT(m_textureData.texture.m_w, m_textureData.texture.m_h)) 
	{
		wr = (int)powf(2.0, ceilf(logf((float)m_textureData.texture.m_w)/logf(2.0f)));
		hr = (int)powf(2.0, ceilf(logf((float)m_textureData.texture.m_h)/logf(2.0f)));
		xp = (wr - m_textureData.texture.m_w)/2;
		yp = (hr - m_textureData.texture.m_h)/2;
	}else 
	{
		wr = m_textureData.texture.m_w;
		hr = m_textureData.texture.m_h;
		xp= 0;
		yp= 0;
	}
	if(m_isTransformCenter == POINT_CENTER)
	{
		m_transformCenter.set(m_textureData.texture.m_w * 0.5f,m_textureData.texture.m_h * 0.5f);
	}else
	{
		m_transformCenter.set(0.0f,0.0f);
	}
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_SPRITE);
#endif
	m_isInited = 1;
#if IS_USE_SOLIDIFY
	m_needSolidify = 1;
	m_glListOrder = glGenLists(1); //获取显示列表的编号
	if(m_glListOrder < 0) return 0;
#endif
	return 1;
}

//unsigned int srcBlendMode[9] = {GL_ZERO,GL_ONE,GL_DST_COLOR,GL_ONE_MINUS_DST_COLOR,GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,
//		GL_DST_ALPHA,GL_ONE_MINUS_DST_ALPHA,GL_SRC_ALPHA_SATURATE};
//unsigned int dstBlendMode[8] = {GL_ZERO,GL_ONE,GL_SRC_COLOR,GL_ONE_MINUS_SRC_COLOR,GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,	
//		GL_DST_ALPHA,GL_ONE_MINUS_DST_ALPHA};

void _XSprite::drawBasic(const GLuint *pTexnum)
{
	if(m_isVisiable == 0) return;	//不显示的时候直接返回
	if(xsize <= 0 || ysize <= 0 || alpha <= 0) return;

	if(m_needUpdateInsideData != 0)
	{
		m_needUpdateInsideData = 0;
		m_needUpdatePointArray = 0;
		updateInsideData();
		updatePointArray();
	}else
	if(m_needUpdatePointArray != 0)
	{
		m_needUpdatePointArray = 0;
		updatePointArray();
	}

	if(m_pShaderProc != NULL) glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_TEXTURE_2D);

	glBlendFunc(srcBlendMode[m_blendTypeScr],dstBlendMode[m_blendTypeDst]);

	glEnable(GL_BLEND);

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0,wr << 1, 0,hr << 1, -1, 1);	//设置贴图的大小

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glTranslatef(m_positionX + m_transformCenter.x,m_positionY + m_transformCenter.y, 0);

	if(angle != 0) glRotatef(angle, 0, 0, 1);	//设置未指定的角度
	if(xsize != 1.0f || ysize != 1.0f) glScalef(xsize, ysize, 0);	//设置缩放的比例
	
	if(m_pShaderProc == NULL)
	{
		if(pTexnum != NULL) glBindTexture(GL_TEXTURE_2D, (* pTexnum));
		else glBindTexture(GL_TEXTURE_2D, m_textureData.texture.m_texture);
	}else
	{
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		if(pTexnum != NULL) glBindTexture(GL_TEXTURE_2D, (* pTexnum));
		else glBindTexture(GL_TEXTURE_2D, m_textureData.texture.m_texture);

		(* m_pShaderProc)();

		glActiveTexture(GL_TEXTURE0);
	}
	glColor4f(colorRed, colorGreen, colorBlue,alpha);	//设置为指定的颜色
#ifdef GET_ALL_PIXEL
	XEE_AllPixel += m_thisDrawArea;
#endif
	//glBegin(GL_QUADS);
	glBegin(GL_POLYGON);
	for(int i = 0; i < m_pointArraySize;++ i)
	{
		glTexCoord2d(m_pointArray[i << 1].x,m_pointArray[i << 1].y);
		glVertex2d	(m_pointArray[(i << 1) + 1].x,m_pointArray[(i << 1) + 1].y);
	}
	glEnd();	

	if(m_pShaderProc != NULL) glUseProgram(0);				//停止使用shader

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
//	glMatrixMode(GL_TEXTURE);
	glDisable(GL_BLEND);
	if(m_pShaderProc != NULL) glPopAttrib();
}

void _XSprite::updatePointArray()
{
	m_pointArraySize = 4;	//普通情况下4组点
	if(m_turnOverMode != _TURN_OVER_MODE_NULL)
	{
		if(m_isEnableOutsideChip == 0 && m_textureData.isEnableInsideClip == 0)
		{//不需要裁减
#ifdef GET_ALL_PIXEL
			m_thisDrawArea += m_textureData.texture.m_w * m_textureData.texture.m_h;
#endif
			m_pointArray[0].set(xp + m_textureData.texture.m_w * m_shapeData[0],yp + m_textureData.texture.m_h * m_shapeData[1]);
			m_pointArray[1].set(m_textureData.texture.m_w * m_shapeData[2] - m_transformCenter.x,m_textureData.texture.m_h * m_shapeData[3] -m_transformCenter.y);

			m_pointArray[2].set(xp + m_textureData.texture.m_w * m_shapeData[4],yp + m_textureData.texture.m_h * m_shapeData[5]);
			m_pointArray[3].set(m_textureData.texture.m_w * m_shapeData[6] - m_transformCenter.x,m_textureData.texture.m_h * m_shapeData[7] -m_transformCenter.y);

			m_pointArray[4].set(xp + m_textureData.texture.m_w * m_shapeData[8],yp + m_textureData.texture.m_h * m_shapeData[9]);
			m_pointArray[5].set(m_textureData.texture.m_w * m_shapeData[10] - m_transformCenter.x,m_textureData.texture.m_h * m_shapeData[11] -m_transformCenter.y);

			m_pointArray[6].set(xp + m_textureData.texture.m_w * m_shapeData[12],yp + m_textureData.texture.m_h * m_shapeData[13]);
			m_pointArray[7].set(m_textureData.texture.m_w * m_shapeData[14] - m_transformCenter.x,m_textureData.texture.m_h * m_shapeData[15] -m_transformCenter.y);
		}else
		{//需要裁剪(有问题)
#ifdef GET_ALL_PIXEL
			m_thisDrawArea += m_clipRect.getWidth() * m_clipRect.getHeight();
#endif
			m_pointArray[0].set(xp + m_clipRect.left + m_clipRect.getWidth() * m_shapeData[0],yp + m_clipRect.top + m_clipRect.getHeight() * m_shapeData[1]);
			m_pointArray[1].set(m_clipRect.getWidth() * m_shapeData[2] - m_transformCenter.x,m_clipRect.getHeight() * m_shapeData[3] - m_transformCenter.y);

			m_pointArray[2].set(xp + m_clipRect.left + m_clipRect.getWidth() * m_shapeData[4],yp + m_clipRect.top + m_clipRect.getHeight() * m_shapeData[5]);
			m_pointArray[3].set(m_clipRect.getWidth() * m_shapeData[6] - m_transformCenter.x,m_clipRect.getHeight() * m_shapeData[7] - m_transformCenter.y);

			m_pointArray[4].set(xp + m_clipRect.left + m_clipRect.getWidth() * m_shapeData[8],yp + m_clipRect.top + m_clipRect.getHeight() * m_shapeData[9]);
			m_pointArray[5].set(m_clipRect.getWidth() * m_shapeData[10] - m_transformCenter.x,m_clipRect.getHeight() * m_shapeData[11] - m_transformCenter.y);

			m_pointArray[6].set(xp + m_clipRect.left + m_clipRect.getWidth() * m_shapeData[12],yp + m_clipRect.top + m_clipRect.getHeight() * m_shapeData[13]);
			m_pointArray[7].set(m_clipRect.getWidth() * m_shapeData[14] - m_transformCenter.x,m_clipRect.getHeight() * m_shapeData[15] - m_transformCenter.y);
		}
	}else
	{
		if(m_isEnableOutsideChip == 0 && m_textureData.isEnableInsideClip == 0)
		{
#ifdef GET_ALL_PIXEL
			m_thisDrawArea += m_textureData.texture.m_w * m_textureData.texture.m_h;
#endif
			m_pointArray[0].set(xp,yp);
			m_pointArray[1].set(-m_transformCenter.x, -m_transformCenter.y);

			m_pointArray[2].set(xp, yp + m_textureData.texture.m_h);
			m_pointArray[3].set(-m_transformCenter.x,m_textureData.texture.m_h - m_transformCenter.y);

			m_pointArray[4].set(xp + m_textureData.texture.m_w, yp + m_textureData.texture.m_h);
			m_pointArray[5].set(m_textureData.texture.m_w - m_transformCenter.x,m_textureData.texture.m_h - m_transformCenter.y);

			m_pointArray[6].set(xp + m_textureData.texture.m_w, yp);
			m_pointArray[7].set(m_textureData.texture.m_w - m_transformCenter.x,-m_transformCenter.y);
		}else
		{
#ifdef GET_ALL_PIXEL
			m_thisDrawArea += m_clipRect.getWidth() * m_clipRect.getHeight();
#endif
			m_pointArray[0].set(xp + m_clipRect.left,yp + m_clipRect.top);
			m_pointArray[1].set(-m_transformCenter.x, -m_transformCenter.y);

			m_pointArray[2].set(xp + m_clipRect.left, yp + m_clipRect.bottom);
			m_pointArray[3].set(-m_transformCenter.x, m_clipRect.getHeight() - m_transformCenter.y);

			m_pointArray[4].set(xp + m_clipRect.right, yp + m_clipRect.bottom);
			m_pointArray[5].set(m_clipRect.getWidth() - m_transformCenter.x,m_clipRect.getHeight() - m_transformCenter.y);

			m_pointArray[6].set(xp + m_clipRect.right, yp + m_clipRect.top);
			m_pointArray[7].set(m_clipRect.getWidth() - m_transformCenter.x, -m_transformCenter.y);
		}
	}
	if(m_needAngleClip != 0)
	{//这里先不考虑翻转以及裁剪造成的影响，也就是说翻转和裁剪将会造成这里出现逻辑的不统一
		_XVector2 tempArrayV[4];
		_XVector2 tempArrayU[4];
		for(int i = 0;i < 4;++ i)
		{
			tempArrayV[i] = m_pointArray[i << 1];
			tempArrayU[i] = m_pointArray[(i << 1) + 1];
		}
		vector4Sort(tempArrayV,tempArrayU);
		m_pointArraySize = getEdgePoint(m_clipAngle * ANGLE_TO_RADIAN,m_pointArray,tempArrayV,tempArrayU);
	}
}

void _XSprite::drawBasic(const _XTextureData *pTexData)
{
	if(m_isVisiable == 0) return;	//不显示的时候直接返回
	if(xsize <= 0 || ysize <= 0 || alpha <= 0) return;
	_XTextureData tempText;
	if(pTexData != NULL) 
	{//备份现场，这里备份的数据不是所有数据都备份，所以比较悲催
		tempText = m_textureData;
		m_textureData = *pTexData;
		//更新剪切和旋转
		updateClipAndRotatoData();
		//setPosition(m_setPosition);	//这里逻辑上存在重复调用的问题，需要从逻辑上理清楚，防止重复的计算反复进行
	}
	if(m_needUpdateInsideData != 0)
	{
		m_needUpdateInsideData = 0;
		m_needUpdatePointArray = 0;
		updateInsideData();
		updatePointArray();
	}else
	if(m_needUpdatePointArray != 0)
	{
		m_needUpdatePointArray = 0;
		updatePointArray();
	}

	if(m_pShaderProc != NULL) glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_TEXTURE_2D);

	glBlendFunc(srcBlendMode[m_blendTypeScr],dstBlendMode[m_blendTypeDst]);

	glEnable(GL_BLEND);

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0, wr << 1, 0,hr << 1, -1, 1);	//设置贴图的大小

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glTranslatef(m_positionX + m_transformCenter.x,m_positionY + m_transformCenter.y, 0);

	if(angle != 0.0f) glRotatef(angle, 0, 0, 1);	//设置未指定的角度
	if(xsize != 1.0f || ysize != 1.0f) glScalef(xsize, ysize, 0);	//设置缩放的比例

	if(m_pShaderProc == NULL)
	{
		glBindTexture(GL_TEXTURE_2D, m_textureData.texture.m_texture);
	}else
	{
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_textureData.texture.m_texture);

		(* m_pShaderProc)();

		glActiveTexture(GL_TEXTURE0);
	}
	glColor4f(colorRed, colorGreen, colorBlue,alpha);	//设置为指定的颜色

#ifdef GET_ALL_PIXEL
	XEE_AllPixel += m_thisDrawArea;
#endif
	//glBegin(GL_QUADS);
	glBegin(GL_POLYGON);
	for(int i = 0; i < m_pointArraySize;++ i)
	{
		glTexCoord2d(m_pointArray[i << 1].x,m_pointArray[i << 1].y);
		glVertex2d	(m_pointArray[(i << 1) + 1].x,m_pointArray[(i << 1) + 1].y);
	}
	glEnd();																		
	if(m_pShaderProc != NULL) glUseProgram(0);				//停止使用shader

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_TEXTURE);
	glDisable(GL_BLEND);
	if(m_pShaderProc != NULL) glPopAttrib();

	if(pTexData != NULL) 
	{//备份现场
		m_textureData = tempText;
		tempText.setNoInited();
	}
}

void _XSprite::drawWithoutBlend(const GLuint *pTexnum)
{
	if(m_isVisiable == 0) return;	//不显示的时候直接返回
	if(xsize <= 0 || ysize <= 0 || alpha <= 0) return;
	if(m_needUpdateInsideData != 0)
	{
		m_needUpdateInsideData = 0;
		m_needUpdatePointArray = 0;
		updateInsideData();
		updatePointArray();
	}else
	if(m_needUpdatePointArray != 0)
	{
		m_needUpdatePointArray = 0;
		updatePointArray();
	}

	if(m_pShaderProc != NULL) glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_TEXTURE_2D);

//	glBlendFunc(srcBlendMode[m_blendTypeScr],dstBlendMode[m_blendTypeDst]);

//	glEnable(GL_BLEND);

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0, wr << 1, 0, hr << 1, -1, 1);	//设置贴图的大小

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glTranslatef(m_positionX + m_transformCenter.x,m_positionY + m_transformCenter.y, 0);

	if(angle != 0) glRotatef(angle, 0, 0, 1);	//设置未指定的角度
	if(xsize != 1.0f || ysize != 1.0f) glScalef(xsize, ysize, 0);	//设置缩放的比例
	
	if(m_pShaderProc == NULL)
	{
		if(pTexnum != NULL) glBindTexture(GL_TEXTURE_2D, (* pTexnum));
		else glBindTexture(GL_TEXTURE_2D, m_textureData.texture.m_texture);
	}else
	{
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		if(pTexnum != NULL) glBindTexture(GL_TEXTURE_2D, (* pTexnum));
		else glBindTexture(GL_TEXTURE_2D, m_textureData.texture.m_texture);

		(* m_pShaderProc)();

		glActiveTexture(GL_TEXTURE0);
	}
	glColor4f(colorRed, colorGreen, colorBlue,alpha);	//设置为指定的颜色

#ifdef GET_ALL_PIXEL
	XEE_AllPixel += m_thisDrawArea;
#endif
	//glBegin(GL_QUADS);
	glBegin(GL_POLYGON);
	for(int i = 0; i < m_pointArraySize;++ i)
	{
		glTexCoord2d(m_pointArray[i << 1].x,m_pointArray[i << 1].y);
		glVertex2d	(m_pointArray[(i << 1) + 1].x,m_pointArray[(i << 1) + 1].y);
	}
	glEnd();	

	if(m_pShaderProc != NULL) glUseProgram(0);				//停止使用shader

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
//	glMatrixMode(GL_TEXTURE);
//	glDisable(GL_BLEND);
	if(m_pShaderProc != NULL) glPopAttrib();
}

void _XSprite::drawWithoutBlend(const _XTextureData *pTexData)
{
	if(m_isVisiable == 0) return;	//不显示的时候直接返回
	if(xsize <= 0 || ysize <= 0 || alpha <= 0) return;
	_XTextureData tempText;
	if(pTexData != NULL) 
	{//备份现场，这里备份的数据不是所有数据都备份，所以比较悲催
		tempText = m_textureData;
		m_textureData = *pTexData;
		//更新剪切和旋转
		updateClipAndRotatoData();
		//setPosition(m_setPosition);	//这里逻辑上存在重复调用的问题，需要从逻辑上理清楚，防止重复的计算反复进行
	}
	if(m_needUpdateInsideData != 0)
	{
		m_needUpdateInsideData = 0;
		m_needUpdatePointArray = 0;
		updateInsideData();
		updatePointArray();
	}else
	if(m_needUpdatePointArray != 0)
	{
		m_needUpdatePointArray = 0;
		updatePointArray();
	}

	if(m_pShaderProc != NULL) glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_TEXTURE_2D);

//	glBlendFunc(srcBlendMode[m_blendTypeScr],dstBlendMode[m_blendTypeDst]);

//	glEnable(GL_BLEND);

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0,wr << 1, 0,hr << 1, -1, 1);	//设置贴图的大小

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glTranslatef(m_positionX + m_transformCenter.x,m_positionY + m_transformCenter.y, 0);

	if(angle != 0.0f) glRotatef(angle, 0, 0, 1);	//设置未指定的角度
	if(xsize != 1.0f || ysize != 1.0f) glScalef(xsize, ysize, 0);	//设置缩放的比例

	if(m_pShaderProc == NULL)
	{
		glBindTexture(GL_TEXTURE_2D, m_textureData.texture.m_texture);
	}else
	{
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_textureData.texture.m_texture);

		(* m_pShaderProc)();

		glActiveTexture(GL_TEXTURE0);
	}
	glColor4f(colorRed, colorGreen, colorBlue,alpha);	//设置为指定的颜色

#ifdef GET_ALL_PIXEL
	XEE_AllPixel += m_thisDrawArea;
#endif
	//glBegin(GL_QUADS);
	glBegin(GL_POLYGON);
	for(int i = 0; i < m_pointArraySize;++ i)
	{
		glTexCoord2d(m_pointArray[i << 1].x,m_pointArray[i << 1].y);
		glVertex2d	(m_pointArray[(i << 1) + 1].x,m_pointArray[(i << 1) + 1].y);
	}
	glEnd();																		
	if(m_pShaderProc != NULL) glUseProgram(0);				//停止使用shader

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_TEXTURE);
//	glDisable(GL_BLEND);
	if(m_pShaderProc != NULL) glPopAttrib();

	if(pTexData != NULL) 
	{//备份现场
		m_textureData = tempText;
		tempText.setNoInited();
	}
}


void _XSprite::draw(const GLuint *pTexnum)
{//图片精灵	
	if(m_isVisiable == 0) return;	//不显示的时候直接返回
#if IS_USE_SOLIDIFY
	if(m_needSolidify != 0)
	{//需要固化
		glNewList(m_glListOrder,GL_COMPILE_AND_EXECUTE);
		drawBasic(pTexnum);
		glEndList();
		m_needSolidify = 0;
	}else
	{//不需要固化
		glCallList(m_glListOrder);	
	}
#else
	drawBasic(pTexnum);
#endif
}

void _XSprite::draw(const _XTextureData *pTexData)
{//图片精灵	
	if(m_isVisiable == 0) return;	//不显示的时候直接返回
#if IS_USE_SOLIDIFY
	if(m_isUseSolidify != 0)
	{//使用固化
		if(m_needSolidify != 0)
		{//需要固化
			glNewList(m_glListOrder,GL_COMPILE_AND_EXECUTE);
			drawBasic(pTexData);
			glEndList();
			m_needSolidify = 0;
		}else
		{//不需要固化
			glCallList(m_glListOrder);	
		}
	}else
	{
		drawBasic(pTexData);
	}
#else
	drawBasic(pTexData);
#endif
}

void _XSprite::drawEx(const GLuint *pTexnum)
{//图片精灵	
	if(m_isVisiable == 0) return;	//不显示的时候直接返回
	if(xsize <= 0 || ysize <= 0 || alpha <= 0) return;
	if(m_needUpdateInsideData != 0)
	{
		m_needUpdateInsideData = 0;
		m_needUpdatePointArray = 0;
		updateInsideData();
		updatePointArray();
	}else
	if(m_needUpdatePointArray != 0)
	{
		m_needUpdatePointArray = 0;
		updatePointArray();
	}

	if(m_pShaderProc != NULL) glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_TEXTURE_2D);
		
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glEnable(GL_BLEND);
	
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0,wr << 1, 0,hr << 1, -1, 1);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	int halfW;
	int halfH;
	halfW = m_textureData.texture.m_w >> 1;
	halfH = m_textureData.texture.m_h >> 1;
	glTranslatef(m_positionX + halfW, m_positionY + halfH, 0);

	glRotatef(angle, 0, 0, 1);
	glScalef(xsize, ysize, 0);

	if(m_pShaderProc == NULL)
	{
		if(pTexnum != NULL) glBindTexture(GL_TEXTURE_2D, (* pTexnum));
		else glBindTexture(GL_TEXTURE_2D, m_textureData.texture.m_texture);
	}else
	{
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		if(pTexnum != NULL) glBindTexture(GL_TEXTURE_2D, (* pTexnum));
		else glBindTexture(GL_TEXTURE_2D, m_textureData.texture.m_texture);

		(* m_pShaderProc)();

		glActiveTexture(GL_TEXTURE0);
	}
	glColor4f(colorRed, colorGreen, colorBlue,alpha);

#ifdef GET_ALL_PIXEL
	XEE_AllPixel += m_textureData.texture.m_w * m_textureData.texture.m_h;
#endif
	//glBegin(GL_QUADS);
	glBegin(GL_POLYGON);
		glTexCoord2d(xp, yp);
		glVertex2d(-halfW, -halfH);
		glTexCoord2d(xp + m_textureData.texture.m_w, yp);
		glVertex2d(halfW, -halfH);
		glTexCoord2d(xp + m_textureData.texture.m_w, yp + m_textureData.texture.m_h);
		glVertex2d(halfW, halfH);
		glTexCoord2d(xp, yp + m_textureData.texture.m_h);
		glVertex2d(-halfW, halfH);
	glEnd();																		
	if(m_pShaderProc != NULL) glUseProgram(0);				//停止使用shader

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_TEXTURE);

//	glDisable(GL_BLEND);
	if(m_pShaderProc != NULL) glPopAttrib();
}

void _XSprite::drawEx(const _XTextureData *pTexData)
{//图片精灵
	if(m_isVisiable == 0) return;	//不显示的时候直接返回
	if(xsize <= 0 || ysize <= 0 || alpha <= 0) return;
	_XTextureData tempText;
	if(pTexData != NULL) 
	{//备份现场，这里备份的数据不是所有数据都备份，所以比较悲催
		tempText = m_textureData;
		m_textureData = *pTexData;
		//更新剪切和旋转
		updateClipAndRotatoData();
		//setPosition(m_setPosition);	//这里逻辑上存在重复调用的问题，需要从逻辑上理清楚，防止重复的计算反复进行
	}
	if(m_needUpdateInsideData != 0)
	{
		m_needUpdateInsideData = 0;
		m_needUpdatePointArray = 0;
		updateInsideData();
		updatePointArray();
	}else
	if(m_needUpdatePointArray != 0)
	{
		m_needUpdatePointArray = 0;
		updatePointArray();
	}

	if(m_pShaderProc != NULL) glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_TEXTURE_2D);
		
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glEnable(GL_BLEND);
	
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0,wr << 1, 0,hr << 1, -1, 1);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	int halfW;
	int halfH;
	halfW = m_textureData.texture.m_w >> 1;
	halfH = m_textureData.texture.m_h >> 1;
	glTranslatef(m_positionX + halfW, m_positionY + halfH, 0);

	glRotatef(angle, 0, 0, 1);
	glScalef(xsize, ysize, 0);

	if(m_pShaderProc == NULL)
	{
		glBindTexture(GL_TEXTURE_2D, m_textureData.texture.m_texture);
	}else
	{
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_textureData.texture.m_texture);

		(* m_pShaderProc)();

		glActiveTexture(GL_TEXTURE0);
	}

	glColor4f(colorRed, colorGreen, colorBlue,alpha);

#ifdef GET_ALL_PIXEL
	XEE_AllPixel += m_textureData.texture.m_w * m_textureData.texture.m_h;
#endif
	//glBegin(GL_QUADS);
	glBegin(GL_POLYGON);
		glTexCoord2d(xp, yp);
		glVertex2d(-halfW, -halfH);
		glTexCoord2d(xp + m_textureData.texture.m_w, yp);
		glVertex2d(halfW, -halfH);
		glTexCoord2d(xp + m_textureData.texture.m_w, yp + m_textureData.texture.m_h);
		glVertex2d(halfW, halfH);
		glTexCoord2d(xp, yp + m_textureData.texture.m_h);
		glVertex2d(-halfW, halfH);
	glEnd();																		
	if(m_pShaderProc != NULL) glUseProgram(0);				//停止使用shader

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_TEXTURE);

	if(m_pShaderProc != NULL) glPopAttrib();
//	glDisable(GL_BLEND);
	if(pTexData != NULL) 
	{//备份现场
		m_textureData = tempText;
		tempText.setNoInited();
	}
}

void _XSprite::setLeft2Right()
{
	if(m_turnOverMode == _TURN_OVER_MODE_LEFT_TO_RIGHT) return;	//防止重复的计算
	m_turnOverMode = _TURN_OVER_MODE_LEFT_TO_RIGHT;
	//左右翻转
	m_shapeData[0] = 0.0f;m_shapeData[1] = 0.0f;
	m_shapeData[2] = 1.0f;m_shapeData[3] = 0.0f;

	m_shapeData[4] = 1.0f;m_shapeData[5] = 0.0f;
	m_shapeData[6] = 0.0f;m_shapeData[7] = 0.0f;

	m_shapeData[8] = 1.0f;m_shapeData[9] = 1.0f;
	m_shapeData[10] = 0.0f;m_shapeData[11] = 1.0f;

	m_shapeData[12] = 0.0f;m_shapeData[13] = 1.0f;
	m_shapeData[14] = 1.0f;m_shapeData[15] = 1.0f;
	//setPosition(m_setPosition);
	m_needUpdateInsideData = 1;
}

void _XSprite::setUp2Down()
{
	if(m_turnOverMode == _TURN_OVER_MODE_UP_TO_DOWN) return;	//防止重复的计算
	m_turnOverMode = _TURN_OVER_MODE_UP_TO_DOWN;
	//上下翻转
	m_shapeData[0] = 0.0f;m_shapeData[1] = 0.0f;
	m_shapeData[2] = 0.0f;m_shapeData[3] = 1.0f;

	m_shapeData[4] = 1.0f;m_shapeData[5] = 0.0f;
	m_shapeData[6] = 1.0f;m_shapeData[7] = 1.0f;

	m_shapeData[8] = 1.0f;m_shapeData[9] = 1.0f;
	m_shapeData[10] = 1.0f;m_shapeData[11] = 0.0f;

	m_shapeData[12] = 0.0f;m_shapeData[13] = 1.0f;
	m_shapeData[14] = 0.0f;m_shapeData[15] = 0.0f;
	//setPosition(m_setPosition);
	m_needUpdateInsideData = 1;
}

_XSprite::_XSprite()
:alpha(1.0f)
,m_isInited(0)
//,m_isOverturn(0)	//默认情况下不翻转
,colorRed(1.0f)
,colorGreen(1.0f)
,colorBlue(1.0f)
,m_resoursePosition(RESOURCE_OUTSIDE)
#if IS_USE_SOLIDIFY
,m_glListOrder(-1)
,m_needSolidify(0)		//默认情况不需要固化
#endif
,m_isEnableOutsideChip(0)
,m_isACopy(0)
,m_transformCenter(0.0f,0.0f)
,m_changeTransformCenterManual(0)
,m_clipOutsideRect(0,0,100,100)
,m_blendTypeScr(4)
,m_blendTypeDst(5)
,m_pShaderProc(NULL)
,m_turnOverMode(_TURN_OVER_MODE_NULL)
,m_needUpdateInsideData(0)
,m_needUpdatePointArray(0)
,m_needAngleClip(0)
,m_isVisiable(1)
,m_isTransformCenter(POINT_LEFT_TOP)
{
	m_shapeData[0] = 0.0f; m_shapeData[1] = 0.0f;	//具体的形状描述的数据
	m_shapeData[2] = 0.0f; m_shapeData[3] = 0.0f;	//具体的形状描述的数据
	m_shapeData[4] = 1.0f; m_shapeData[5] = 0.0f;	//具体的形状描述的数据
	m_shapeData[6] = 1.0f; m_shapeData[7] = 0.0f;	//具体的形状描述的数据
	m_shapeData[8] = 1.0f; m_shapeData[9] = 1.0f;	//具体的形状描述的数据
	m_shapeData[10] = 1.0f; m_shapeData[11] = 1.0f;	//具体的形状描述的数据
	m_shapeData[12] = 0.0f; m_shapeData[13] = 1.0f;	//具体的形状描述的数据
	m_shapeData[14] = 0.0f; m_shapeData[15] = 1.0f;	//具体的形状描述的数据

//	m_frameName = new char[MAX_SPRITE_FILENAME_LENGTH];
	setIsTransformCenter(POINT_CENTER);
//	m_isTransformCenter = POINT_CENTER;
}

void _XSprite::setIsTransformCenter(_XTransformMode temp)	//设置为中心旋转
{
	if(m_isTransformCenter == temp) return;	//防止重复的计算
	if(temp != POINT_CENTER)
	{
		m_isTransformCenter = POINT_LEFT_TOP;
		if(m_textureData.isEnableInsideClip == 0)
		{
			m_transformCenter.set(0.0f,0.0f);
		}else
		{//这一段有问题
			m_transformCenter.set(-m_textureData.textureMove.x,-m_textureData.textureMove.y);
		}
	}else
	{
		m_isTransformCenter = POINT_CENTER;
		if(m_textureData.isEnableInsideClip == 0)
		{
			if(m_textureData.texture.getIsEnable() == 0)
			{
				m_transformCenter.set(0.0f,0.0f);
			}else
			{
				m_transformCenter.set(m_textureData.texture.m_w * 0.5f,m_textureData.texture.m_h * 0.5f);
			}
		}else
		{//这一段貌似有问题
			m_transformCenter.set(m_clipOutsideRect.getWidth() * 0.5f + m_textureData.clipInsideRect.left - m_textureData.textureMove.x - m_clipRect.left,
				m_clipOutsideRect.getHeight() * 0.5f + m_textureData.clipInsideRect.top - m_textureData.textureMove.y - m_clipRect.top);
		}
	}
	m_needUpdateInsideData = 1;
}

void _XSprite::setACopy(const _XSprite& temp)
{
	if(& temp == this) return;	//防止自身赋值
	angle = temp.angle;					//精灵的角度
	sinAngle = temp.sinAngle;
	cosAngle = temp.cosAngle;
	m_isVisiable = temp.m_isVisiable;

	m_blendTypeScr = temp.m_blendTypeScr;
	m_blendTypeDst = temp.m_blendTypeDst;
	m_turnOverMode = temp.m_turnOverMode;
	for(int i = 0;i < 16;++ i)
	{
		m_shapeData[i] = temp.m_shapeData[i];
	}
	m_pShaderProc = temp.m_pShaderProc;
	m_textureData = temp.m_textureData;
	m_positionX = temp.m_positionX;
	m_positionX = temp.m_positionX;							//精灵的位置
	m_isTransformCenter = temp.m_isTransformCenter;
	m_transformCenter = temp.m_transformCenter;
	m_changeTransformCenterManual = temp.m_changeTransformCenterManual;
	m_clipRect = temp.m_clipRect;
	m_isEnableOutsideChip = temp.m_isEnableOutsideChip;
	m_clipOutsideRect = temp.m_clipOutsideRect;
	m_setPosition = temp.m_setPosition;
	m_setTransformCenter = temp.m_setTransformCenter;

	xsize = temp.xsize;
	ysize = temp.ysize;					//精灵的缩放尺寸
	alpha = temp.alpha;					//透明度
	colorRed = temp.colorRed;			//红色
	colorGreen = temp.colorGreen;		//绿色
	colorBlue = temp.colorBlue;			//蓝色
//	m_isOverturn = temp.m_isOverturn;	//是否x方向左右翻转
	wr = temp.wr;
	hr = temp.hr;
	xp = temp.xp;
	yp = temp.yp;
//	memcpy(m_frameName,temp.m_frameName,sizeof(char) * MAX_SPRITE_FILENAME_LENGTH);
#if IS_USE_SOLIDIFY
	if(m_isACopy == 0)
	{
		if(m_glListOrder >=0) glDeleteLists(m_glListOrder,1);
		m_glListOrder = -1;
	}
	m_glListOrder = temp.m_glListOrder;		//显示列表编号
	m_needSolidify = temp.m_needSolidify;		//是否需要固化
#endif

	m_isInited = temp.m_isInited;
	m_needUpdateInsideData = temp.m_needUpdateInsideData;
#if WITH_OBJECT_MANAGER
	if(m_isACopy == 0) _XObjectManager::GetInstance().addAObject(this,OBJ_SPRITE);
#endif
	m_isACopy = 1;
}

_XSprite& _XSprite::operator = (const _XSprite& temp)
{
	if(& temp == this) return *this;	//防止自身赋值
	angle = temp.angle;					//精灵的角度
	sinAngle = temp.sinAngle;					//精灵的角度
	cosAngle = temp.cosAngle;					//精灵的角度
	m_isVisiable = temp.m_isVisiable;					//精灵的角度

	m_blendTypeScr = temp.m_blendTypeScr;
	m_blendTypeDst = temp.m_blendTypeDst;
	m_turnOverMode = temp.m_turnOverMode;
	for(int i = 0;i < 16;++ i)
	{
		m_shapeData[i] = temp.m_shapeData[i];
	}
	m_pShaderProc = temp.m_pShaderProc;
	m_textureData = temp.m_textureData;
	m_positionX = temp.m_positionX;
	m_positionX = temp.m_positionX;							//精灵的位置
	m_isTransformCenter = temp.m_isTransformCenter;
	m_transformCenter = temp.m_transformCenter;
	m_changeTransformCenterManual = temp.m_changeTransformCenterManual;
	m_clipRect = temp.m_clipRect;
	m_isEnableOutsideChip = temp.m_isEnableOutsideChip;
	m_clipOutsideRect = temp.m_clipOutsideRect;
	m_setPosition = temp.m_setPosition;
	m_setTransformCenter = temp.m_setTransformCenter;

	xsize = temp.xsize;
	ysize = temp.ysize;					//精灵的缩放尺寸
	alpha = temp.alpha;					//透明度
	colorRed = temp.colorRed;			//红色
	colorGreen = temp.colorGreen;		//绿色
	colorBlue = temp.colorBlue;			//蓝色
//	m_isOverturn = temp.m_isOverturn;	//是否x方向左右翻转
	wr = temp.wr;
	hr = temp.hr;
	xp = temp.xp;
	yp = temp.yp;
//	memcpy(m_frameName,temp.m_frameName,sizeof(char) * MAX_SPRITE_FILENAME_LENGTH);

#if IS_USE_SOLIDIFY
	if(m_isACopy == 0)
	{
		if(m_glListOrder >=0) glDeleteLists(m_glListOrder,1);
		m_glListOrder = -1;
	}
	m_glListOrder = temp.m_glListOrder;		//显示列表编号
	m_needSolidify = temp.m_needSolidify;		//是否需要固化
#endif
	m_isInited = temp.m_isInited;
	m_needUpdateInsideData = temp.m_needUpdateInsideData;
#if WITH_OBJECT_MANAGER
	if(m_isACopy == 0) _XObjectManager::GetInstance().addAObject(this,OBJ_SPRITE);
#endif
	m_isACopy = 1;
	return *this;
}

_XSprite::_XSprite(const _XSprite& temp)
{
	if(& temp == this) return;	//防止自身赋值
}

void _XSprite::updateInsideData()
{
	if(m_textureData.isEnableInsideClip == 0)
	{
		m_positionX = m_setPosition.x;
		m_positionY = m_setPosition.y;
	}else
	{
		//计算x部分
		if(m_clipOutsideRect.left <= m_textureData.textureMove.x)
		{
			if(m_turnOverMode == _TURN_OVER_MODE_NULL)
			{
				if(m_isTransformCenter == POINT_LEFT_TOP) 
				{
					m_positionX = m_setPosition.x + m_textureData.textureMove.x - m_clipOutsideRect.left;
				}else
				{
					m_positionX = m_setPosition.x + m_textureData.textureMove.x - m_clipOutsideRect.left * xsize
						+ m_clipOutsideRect.left * xsize * (1.0f - cosAngle)
						+ m_clipOutsideRect.top * ysize * sinAngle;
				}
			}else
			{//存在翻转的旋转都会有问题
				if(m_isTransformCenter == POINT_LEFT_TOP)
				{
					if(m_turnOverMode == _TURN_OVER_MODE_LEFT_TO_RIGHT)
					{
						if(m_textureData.textureSize.x - m_clipOutsideRect.right >= m_textureData.textureMove2.x)
						{//ok
							m_positionX = m_setPosition.x + (m_textureData.textureMove.x - m_clipOutsideRect.left) * (1.0f - xsize)
								+ (m_textureData.textureMove.x - m_clipOutsideRect.left) * xsize * (1.0f - cosAngle)
								+ 0.0f * ysize * sinAngle;
						}else
						{//ok	//上边界是否越界应该会有问题
							m_positionX = m_setPosition.x + (m_textureData.textureMove.x - m_clipOutsideRect.left) * (1.0f - xsize)
								+ (m_textureData.textureMove2.x - (m_textureData.textureSize.x - m_clipOutsideRect.right)) * xsize
								+ (m_textureData.textureMove.x - m_clipOutsideRect.left - (m_textureData.textureMove2.x - (m_textureData.textureSize.x - m_clipOutsideRect.right))) 
									* xsize * (1.0f - cosAngle)
								+ 0.0f * ysize * sinAngle;
						}
					}else
					if(m_turnOverMode == _TURN_OVER_MODE_UP_TO_DOWN) 
					{
						if(m_textureData.textureSize.y - m_clipOutsideRect.bottom >= m_textureData.textureMove2.y)
						{
							if(m_clipOutsideRect.top >= m_textureData.textureMove.y)
							{
								m_positionX = m_setPosition.x + m_textureData.textureMove.x - m_clipOutsideRect.left;
							}else
							{
								m_positionX = m_setPosition.x + m_textureData.textureMove.x - m_clipOutsideRect.left	//T2D ok
									+ 0.0f * xsize * (1.0f - cosAngle)
									+ (m_textureData.textureMove.y - m_clipOutsideRect.top) * ysize * sinAngle;
							}
						}else
						{
							if(m_clipOutsideRect.top >= m_textureData.textureMove.y)
							{
								m_positionX = m_setPosition.x + m_textureData.textureMove.x - m_clipOutsideRect.left	//T2D ok
									+ 0.0f * xsize * (1.0f - cosAngle)
									- (m_textureData.textureMove2.y - (m_textureData.textureSize.y - m_clipOutsideRect.bottom)) 
										* ysize * sinAngle;
							}else
							{
								m_positionX = m_setPosition.x + m_textureData.textureMove.x - m_clipOutsideRect.left	//T2D ok
									+ 0.0f * xsize * (1.0f - cosAngle)
									+ (m_textureData.textureMove.y - m_clipOutsideRect.top - (m_textureData.textureMove2.y - (m_textureData.textureSize.y - m_clipOutsideRect.bottom))) 
										* ysize * sinAngle;
							}
						}
					}
				}else
				{
					if(m_turnOverMode == _TURN_OVER_MODE_LEFT_TO_RIGHT)
					{//ok !
						if(m_textureData.textureSize.x - m_clipOutsideRect.right >= m_textureData.textureMove2.x)
						{//ok
							m_positionX = m_setPosition.x + m_textureData.textureMove.x * (1.0f - xsize)
								+ m_clipOutsideRect.top * ysize * sinAngle
								+ m_textureData.textureMove.x * xsize * (1.0f - cosAngle);
						}else
						{//ok
							m_positionX = m_setPosition.x + (m_textureData.textureMove2.x - (m_textureData.textureSize.x - m_clipOutsideRect.right)) * xsize
								+ m_textureData.textureMove.x * (1.0f - xsize)
								+ m_clipOutsideRect.top * ysize * sinAngle
								- (m_textureData.textureMove2.x - m_textureData.textureMove.x - (m_textureData.textureSize.x - m_clipOutsideRect.right)) 
									* xsize * (1.0f - cosAngle);
						}
					}else//L2R 
					if(m_turnOverMode == _TURN_OVER_MODE_UP_TO_DOWN)	//T2D ok
					{//ok !
						if(m_textureData.textureSize.y - m_clipOutsideRect.bottom >= m_textureData.textureMove2.y)
						{
							m_positionX = m_setPosition.x + m_textureData.textureMove.x - m_clipOutsideRect.left * xsize
								+ m_clipOutsideRect.top * ysize * sinAngle
								+ m_clipOutsideRect.left * xsize * (1.0f - cosAngle);
						}else
						{
							m_positionX = m_setPosition.x + m_textureData.textureMove.x - m_clipOutsideRect.left * xsize
								+ (m_clipOutsideRect.top - (m_textureData.textureMove2.y - (m_textureData.textureSize.y - m_clipOutsideRect.bottom))) 
									* ysize * sinAngle
								+ m_clipOutsideRect.left * xsize * (1.0f - cosAngle);
						}
					}
				}
			}
		}else
		{
			if(m_turnOverMode == _TURN_OVER_MODE_NULL)
			{
				if(m_isTransformCenter == POINT_LEFT_TOP) 
				{
					m_positionX = m_setPosition.x;
				}else
				{
					m_positionX = m_setPosition.x + m_clipOutsideRect.left * (1.0f - xsize)
						+ m_clipOutsideRect.left * xsize * (1.0f - cosAngle)
						+ m_clipOutsideRect.top * ysize * sinAngle;
				}
			}else
			{//存在翻转的旋转都会有问题
				if(m_isTransformCenter == POINT_LEFT_TOP) 
				{
					if(m_turnOverMode == _TURN_OVER_MODE_LEFT_TO_RIGHT)
					{
						if(m_textureData.textureSize.y - m_clipOutsideRect.right >= m_textureData.textureMove2.x)
						{//ok
							m_positionX = m_setPosition.x;
						}else
						{//ok
							m_positionX = m_setPosition.x + (m_textureData.textureMove2.x - (m_textureData.textureSize.x - m_clipOutsideRect.right))  * xsize
								- (m_textureData.textureMove2.x - (m_textureData.textureSize.x - m_clipOutsideRect.right)) * xsize * (1.0f - cosAngle)
								+ 0.0f * ysize * sinAngle;
						}
					}else
					if(m_turnOverMode == _TURN_OVER_MODE_UP_TO_DOWN) //T2D ok
					{
						if(m_textureData.textureSize.y - m_clipOutsideRect.bottom >= m_textureData.textureMove2.y)
						{
							if(m_clipOutsideRect.top >= m_textureData.textureMove.y)
							{
								m_positionX = m_setPosition.x;
							}else
							{
								m_positionX = m_setPosition.x
									+ 0.0f * xsize * (1.0f - cosAngle)
									+ (m_textureData.textureMove.y - m_clipOutsideRect.top)* ysize * sinAngle;
							}
						}else
						{
							if(m_clipOutsideRect.top >= m_textureData.textureMove.y)
							{
								m_positionX = m_setPosition.x
									+ 0.0f * xsize * (1.0f - cosAngle)
									- (m_textureData.textureMove2.y - (m_textureData.textureSize.y - m_clipOutsideRect.bottom))
										* ysize * sinAngle;
							}else
							{
								m_positionX = m_setPosition.x
									+ 0.0f * xsize * (1.0f - cosAngle)
									- (m_textureData.textureMove2.y - (m_textureData.textureSize.y - m_clipOutsideRect.bottom) - (m_textureData.textureMove.y - m_clipOutsideRect.top))
										* ysize * sinAngle;
							}
						}
					}
				}else 
				{
					if(m_turnOverMode == _TURN_OVER_MODE_LEFT_TO_RIGHT)
					{
						if(m_textureData.textureSize.x - m_clipOutsideRect.right >= m_textureData.textureMove2.x)
						{//ok
							m_positionX = m_setPosition.x + m_clipOutsideRect.left * (1.0f - xsize)
								+ m_clipOutsideRect.left * xsize * (1.0f - cosAngle)
								+ m_clipOutsideRect.top * ysize * sinAngle;
						}else
						{//ok +
							m_positionX = m_setPosition.x + (m_textureData.textureMove2.x - (m_textureData.textureSize.x - m_clipOutsideRect.right)) * xsize
								+ m_clipOutsideRect.left * (1.0f - xsize) 
								+ (m_textureData.textureSize.x - m_clipOutsideRect.right - (m_textureData.textureMove2.x - m_clipOutsideRect.left)) 
								* xsize * (1.0f - cosAngle)
								+ m_clipOutsideRect.top * ysize * sinAngle;
						}
					}else
					if(m_turnOverMode == _TURN_OVER_MODE_UP_TO_DOWN)	//T2D ok
					{//ok
						if(m_clipOutsideRect.top >= m_textureData.textureMove.y)
						{
							m_positionX = m_setPosition.x + m_clipOutsideRect.left * (1.0f - xsize)
								+ m_clipOutsideRect.left * xsize * (1.0f - cosAngle)
								+ (m_clipOutsideRect.top - (m_textureData.textureMove2.y - (m_textureData.textureSize.y - m_clipOutsideRect.bottom)))
									* ysize * sinAngle;
						}else
						{
							if(m_textureData.textureSize.y - m_clipOutsideRect.bottom >= m_textureData.textureMove2.y)
							{
								m_positionX = m_setPosition.x + m_clipOutsideRect.left * (1.0f - xsize)
									+ m_clipOutsideRect.left * xsize * (1.0f - cosAngle)
									+ m_textureData.textureMove.y * ysize * sinAngle;
							}else
							{
								m_positionX = m_setPosition.x + m_clipOutsideRect.left * (1.0f - xsize)
									+ m_clipOutsideRect.left * xsize * (1.0f - cosAngle)
									+ (m_textureData.textureMove.y - (m_textureData.textureMove2.y - (m_textureData.textureSize.y - m_clipOutsideRect.bottom))) 
										* ysize * sinAngle;
							}
						}
					}
				}
			}
		}
		//计算y部分
		if(m_clipOutsideRect.top <= m_textureData.textureMove.y)
		{
			if(m_turnOverMode == _TURN_OVER_MODE_NULL)
			{
				if(m_isTransformCenter == POINT_LEFT_TOP) 
				{
					m_positionY = m_setPosition.y + m_textureData.textureMove.y - m_clipOutsideRect.top;
				}else 
				{
					m_positionY = m_setPosition.y + m_textureData.textureMove.y - m_clipOutsideRect.top * ysize
						- m_clipOutsideRect.left * xsize * sinAngle
						+ m_clipOutsideRect.top * ysize * (1.0f - cosAngle);
				}
			}else
			{//存在翻转的旋转都会有问题
				if(m_isTransformCenter == POINT_LEFT_TOP)
				{
					if(m_turnOverMode == _TURN_OVER_MODE_UP_TO_DOWN) 
					{
						if(m_textureData.textureSize.y - m_clipOutsideRect.bottom >= m_textureData.textureMove2.y)
						{//ok
							m_positionY = m_setPosition.y + (m_textureData.textureMove.y - m_clipOutsideRect.top) * (1.0f - ysize)
								+ 0.0f * xsize * sinAngle
								+ (m_textureData.textureMove.y - m_clipOutsideRect.top) * ysize * (1.0f - cosAngle);
						}else
						{//ok
							if(m_clipOutsideRect.left >= m_textureData.textureMove.x)
							{
								m_positionY = m_setPosition.y + (m_textureData.textureMove.y - m_clipOutsideRect.top) * (1.0f - ysize) 
									+ (m_textureData.textureMove2.y - (m_textureData.textureSize.y - m_clipOutsideRect.bottom)) * ysize
									+ 0.0f * xsize * sinAngle
									+ (m_textureData.textureMove.y - m_clipOutsideRect.top - (m_textureData.textureMove2.y - (m_textureData.textureSize.y - m_clipOutsideRect.bottom)))
										* ysize * (1.0f - cosAngle);
							}else
							{
								m_positionY = m_setPosition.y + (m_textureData.textureMove.y - m_clipOutsideRect.top) * (1.0f - ysize) 
									+ (m_textureData.textureMove2.y - (m_textureData.textureSize.y - m_clipOutsideRect.bottom)) * ysize
									+ 0.0f * xsize * sinAngle
									+ (m_textureData.textureMove.y - m_clipOutsideRect.top - (m_textureData.textureMove2.y - (m_textureData.textureSize.y - m_clipOutsideRect.bottom)))
										* ysize * (1.0f - cosAngle);
							}
						}
					}else//T2D时用这个
					if(m_turnOverMode == _TURN_OVER_MODE_LEFT_TO_RIGHT)	//L2F ok 
					{//2k
						if(m_clipOutsideRect.left >= m_textureData.textureMove.x)
						{
							m_positionY = m_setPosition.y + m_textureData.textureMove.y - m_clipOutsideRect.top
								+ (m_textureData.textureMove2.x - (m_textureData.textureSize.x - m_clipOutsideRect.right)) * xsize * sinAngle
								+ 0.0f * ysize * (1.0f - cosAngle);
						}else
						{
							if(m_textureData.textureSize.x - m_clipOutsideRect.right >= m_textureData.textureMove2.x)
							{
								m_positionY = m_setPosition.y + m_textureData.textureMove.y - m_clipOutsideRect.top
									- (m_textureData.textureMove.x - m_clipOutsideRect.left) * xsize * sinAngle
									+ 0.0f * ysize * (1.0f - cosAngle);
							}else
							{
								m_positionY = m_setPosition.y + m_textureData.textureMove.y - m_clipOutsideRect.top
									- (m_textureData.textureMove.x - m_clipOutsideRect.left - (m_textureData.textureMove2.x - (m_textureData.textureSize.x - m_clipOutsideRect.right)))
										* xsize * sinAngle
									+ 0.0f * ysize * (1.0f - cosAngle);
							}
						}
					}
				}else 
				{
					if(m_turnOverMode == _TURN_OVER_MODE_UP_TO_DOWN)	//T2D ok
					{
						if(m_textureData.textureSize.y - m_clipOutsideRect.bottom >= m_textureData.textureMove2.y)
						{//ok
							m_positionY = m_setPosition.y + m_textureData.textureMove.y * (1.0f - ysize)
								+ m_textureData.textureMove.y * ysize * (1.0f - cosAngle)
								- m_clipOutsideRect.left * xsize * sinAngle;
						}else
						{//ok
							m_positionY = m_setPosition.y + (m_textureData.textureMove2.y - (m_textureData.textureSize.y - m_clipOutsideRect.bottom)) * ysize
								+ m_textureData.textureMove.y * (1.0f - ysize)
								+ (m_textureData.textureMove.y - (m_textureData.textureMove2.y - (m_textureData.textureSize.y - m_clipOutsideRect.bottom)))
									* ysize * (1.0f - cosAngle)
								- m_clipOutsideRect.left * xsize * sinAngle;
						}
					}else
					if(m_turnOverMode == _TURN_OVER_MODE_LEFT_TO_RIGHT)	//L2F ok
					{
					//	if(m_textureData.textureSize.y - m_clipOutsideRect.bottom >= m_textureData.textureMove2.y)
					//	{//ok
							m_positionY = m_setPosition.y + m_textureData.textureMove.y - m_clipOutsideRect.top * ysize
								+ m_clipOutsideRect.top * ysize * (1.0f - cosAngle)
								- m_clipOutsideRect.left * ysize * sinAngle;
					//	}else
					//	{
					//		m_positionY = m_setPosition.y + m_textureData.textureMove.y - m_clipOutsideRect.top * ysize
					//			+ m_clipOutsideRect.top * ysize * (1.0f - cosAngle)
					//			- m_clipOutsideRect.left * ysize * sinAngle;
					//	}
					}
				}
			}
		}else
		{
			if(m_turnOverMode == _TURN_OVER_MODE_NULL)
			{//没有翻转
				if(m_isTransformCenter == POINT_LEFT_TOP) 
				{
					m_positionY = m_setPosition.y;
				}else 
				{
					m_positionY = m_setPosition.y + m_clipOutsideRect.top *(1.0f - ysize)
						- m_clipOutsideRect.left * xsize * sinAngle
						+ m_clipOutsideRect.top * ysize * (1.0f - cosAngle);
				}
			}else
			{//存在翻转的旋转都是有问题的
				if(m_isTransformCenter == POINT_LEFT_TOP) 
				{
					if(m_turnOverMode == _TURN_OVER_MODE_LEFT_TO_RIGHT)	//L2F
					{//这里存在旋转角度时出错的问题
						if(m_clipOutsideRect.left >= m_textureData.textureMove.x)
						{
							if(m_textureData.textureSize.x - m_clipOutsideRect.right >= m_textureData.textureMove2.x)
							{
								m_positionY = m_setPosition.y;
							}else
							{
								m_positionY = m_setPosition.y
									+ (m_textureData.textureMove2.x - (m_textureData.textureSize.x - m_clipOutsideRect.right)) * xsize * sinAngle
									+ 0.0f * ysize * (1.0f - cosAngle);
							}
						}else
						{
							if(m_textureData.textureSize.x - m_clipOutsideRect.right >= m_textureData.textureMove2.x)
							{
								m_positionY = m_setPosition.y
									- (m_textureData.textureMove.x - m_clipOutsideRect.left) * xsize * sinAngle
									+ 0.0f * ysize * (1.0f - cosAngle);
							}else
							{
								m_positionY = m_setPosition.y
									+ (m_textureData.textureMove2.x - (m_textureData.textureSize.x - m_clipOutsideRect.right) - (m_textureData.textureMove.x - m_clipOutsideRect.left)) 
										* xsize * sinAngle
									+ 0.0f * ysize * (1.0f - cosAngle);
							}
						}
					}else
					if(m_turnOverMode == _TURN_OVER_MODE_UP_TO_DOWN)	//T2D ok
					{
						if(m_textureData.textureSize.y - m_clipOutsideRect.bottom >= m_textureData.textureMove2.y)
						{//ok
							m_positionY = m_setPosition.y;// + m_textureData.textureMove2.y * ysize;
						}else
						{//ok
							m_positionY = m_setPosition.y + (m_textureData.textureMove2.y - (m_textureData.textureSize.y - m_clipOutsideRect.bottom))  * ysize
									+ 0.0f * xsize * sinAngle
									- (m_textureData.textureMove2.y - (m_textureData.textureSize.y - m_clipOutsideRect.bottom)) * ysize * (1.0f - cosAngle);
						}
					}
				}else 
				{
					if(m_turnOverMode == _TURN_OVER_MODE_LEFT_TO_RIGHT)	//L2F
					{//ok!
						if(m_clipOutsideRect.left >= m_textureData.textureMove.x)
						{//ok +
							if(m_textureData.textureSize.x - m_clipOutsideRect.right >= m_textureData.textureMove2.x)
							{//ok
								m_positionY = m_setPosition.y + m_clipOutsideRect.top *(1.0f - ysize)
									- m_clipOutsideRect.left * xsize * sinAngle
									+ m_clipOutsideRect.top * ysize * (1.0f - cosAngle);
							}else
							{//ok
								m_positionY = m_setPosition.y + m_clipOutsideRect.top *(1.0f - ysize)
									- (m_textureData.textureSize.x - m_clipOutsideRect.right - (m_textureData.textureMove2.x - m_clipOutsideRect.left)) 
										 * xsize * sinAngle
									+ m_clipOutsideRect.top * ysize * (1.0f - cosAngle);
							}
						}else
						{
							if(m_textureData.textureSize.x - m_clipOutsideRect.right >= m_textureData.textureMove2.x)
							{//ok
								m_positionY = m_setPosition.y + m_clipOutsideRect.top *(1.0f - ysize)
									- m_textureData.textureMove.x * xsize * sinAngle
									+ m_clipOutsideRect.top * ysize * (1.0f - cosAngle);
							}else
							{//ok
								m_positionY = m_setPosition.y + m_clipOutsideRect.top *(1.0f - ysize)
									+ (m_textureData.textureMove2.x - m_textureData.textureMove.x - (m_textureData.textureSize.x - m_clipOutsideRect.right)) 
										* xsize * sinAngle
									+ m_clipOutsideRect.top * ysize * (1.0f - cosAngle);
							}
						}
					}else
					if(m_turnOverMode == _TURN_OVER_MODE_UP_TO_DOWN)	//T2D ok
					{//ok !
 						if(m_textureData.textureSize.y - m_clipOutsideRect.bottom >= m_textureData.textureMove2.y)
						{//ok
							m_positionY = m_setPosition.y + m_clipOutsideRect.top * (1.0f - ysize) 
								- m_clipOutsideRect.left * xsize * sinAngle
								+ m_clipOutsideRect.top * ysize * (1.0f - cosAngle);
						}else	//当使用m_textureData.textureMove2.y作为裁剪时
						{ // ok
							m_positionY = m_setPosition.y + (m_textureData.textureMove2.y - (m_textureData.textureSize.y - m_clipOutsideRect.bottom)) * ysize
								+ m_clipOutsideRect.top * (1.0f - ysize) 
								+ (m_clipOutsideRect.top - (m_textureData.textureMove2.y - (m_textureData.textureSize.y - m_clipOutsideRect.bottom))) 
									* ysize * (1.0f - cosAngle)
								- m_clipOutsideRect.left * xsize * sinAngle;
						}
					}
				}
			}
		}
	}
}

void _XSprite::updateClipAndRotatoData()
{
	if(m_isEnableOutsideChip != 0)
	{
		setClipRect(m_clipOutsideRect);
	}else
	{
		setClipRect(0.0f,0.0f,m_textureData.textureSize.x,m_textureData.textureSize.y);
		m_isEnableOutsideChip = 0;
		m_needUpdateInsideData = 1;
	}
}

void _XSprite::setClipRect(float left,float top,float right,float bottom)
{
	m_isEnableOutsideChip = 1;
	//合理性判断
	if(left < 0) left = 0;
	if(top < 0) top = 0;
	if(right > m_textureData.texture.m_w) right = m_textureData.texture.m_w;
	if(bottom > m_textureData.texture.m_h) bottom = m_textureData.texture.m_h;

	if(left > m_textureData.texture.m_w)
	{
		left = m_textureData.texture.m_w;
		right = m_textureData.texture.m_w;
	}

	if(bottom > m_textureData.texture.m_h)
	{
		top = m_textureData.texture.m_h;
		bottom = m_textureData.texture.m_h;
	}

	m_clipOutsideRect.set(left,top,right,bottom);
	if(m_textureData.isEnableInsideClip != 0)
	{//如果内部已经对数据进行了裁剪，则这里进行拼合
		if(left < m_textureData.textureMove.x) left = m_textureData.clipInsideRect.left;
		else left += m_textureData.clipInsideRect.left - m_textureData.textureMove.x;
		if(top < m_textureData.textureMove.y) top = m_textureData.clipInsideRect.top;
		else top += m_textureData.clipInsideRect.top - m_textureData.textureMove.y;
		if(right > m_textureData.clipInsideRect.getWidth() + m_textureData.textureMove.x) right = m_textureData.clipInsideRect.right;
		else right += m_textureData.clipInsideRect.left - m_textureData.textureMove.x;
		if(bottom > m_textureData.clipInsideRect.getHeight() + m_textureData.textureMove.y) bottom = m_textureData.clipInsideRect.bottom;
		else bottom += m_textureData.clipInsideRect.top - m_textureData.textureMove.y;
	}

	m_clipRect.set(left,top,right,bottom);	
	if(m_changeTransformCenterManual == 0)
	{
		if(m_isTransformCenter == POINT_CENTER)
		{
			if(m_textureData.isEnableInsideClip != 0)
			{
				m_transformCenter.set(m_clipOutsideRect.getWidth() * 0.5f + m_textureData.clipInsideRect.left - m_textureData.textureMove.x - m_clipRect.left,
					m_clipOutsideRect.getHeight() * 0.5f + m_textureData.clipInsideRect.top - m_textureData.textureMove.y - m_clipRect.top);
				//setPosition(m_setPosition);
			}else
			{
				m_transformCenter.set(m_clipRect.getWidth() * 0.5f,m_clipRect.getHeight() * 0.5f);
			}
		}else
		{
			if(m_textureData.isEnableInsideClip != 0)
			{
				if(m_clipOutsideRect.left < m_textureData.textureMove.x) m_transformCenter.x = m_clipOutsideRect.left - m_textureData.textureMove.x;
				else m_transformCenter.x = 0;
				if(m_clipOutsideRect.top < m_textureData.textureMove.y) m_transformCenter.y = m_clipOutsideRect.top - m_textureData.textureMove.y;
				else m_transformCenter.y = 0; 
				//setPosition(m_setPosition);
		//		m_transformCenter.set(-m_textureData.textureMove.x,-m_textureData.textureMove.y);
			}else
			{
				m_transformCenter.set(0.0f,0.0f);
			}
		}
	}else
	{
		if(m_textureData.isEnableInsideClip != 0)
		{
			if(m_clipOutsideRect.left < m_textureData.textureMove.x) m_transformCenter.x = m_clipOutsideRect.left - m_textureData.textureMove.x + m_setTransformCenter.x;
			else m_transformCenter.x = m_setTransformCenter.x;
			if(m_clipOutsideRect.top < m_textureData.textureMove.y) m_transformCenter.y = m_clipOutsideRect.top - m_textureData.textureMove.y + m_setTransformCenter.y;
			else m_transformCenter.y = m_setTransformCenter.y; 
			//setPosition(m_setPosition);
		}
	}
	m_needUpdateInsideData = 1;
}


int _XSprite::release()
{
	if(m_isInited == 0) return 1;
#if IS_USE_SOLIDIFY
	if(m_isACopy == 0)
	{
		if(m_glListOrder >=0) glDeleteLists(m_glListOrder,1);
		m_glListOrder = -1;
	}
#endif
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(this);
#endif
	m_isACopy = 0;
	m_isInited = 0;
	return 1;
}

_XSprite::~_XSprite()
{
	release();
//	XDELETE_ARRAY(m_frameName);
	//m_isInited = 0;
}

_XVector2 _XSprite::getBox(int order)
{
	_XVector2 ret;
	ret.set(0.0f,0.0f);
	if(order < 0 || order >= 4) return ret;
	if(m_isTransformCenter == POINT_LEFT_TOP)
	{//左上角对其
		if(m_isEnableOutsideChip == 0 && m_textureData.isEnableInsideClip == 0)
		{//不需要裁减
			if(order == 0) ret.set(xp + m_setPosition.x,yp + m_setPosition.y); else
			if(order == 1) ret.set(xp + m_setPosition.x + m_textureData.texture.m_w * xsize * cosAngle, 
				yp + m_setPosition.y + m_textureData.texture.m_w * xsize * sinAngle); else
			if(order == 2) ret.set(xp + m_setPosition.x + m_textureData.texture.m_w * xsize * cosAngle + m_textureData.texture.m_h * ysize * (- sinAngle), 
				yp + m_setPosition.y + m_textureData.texture.m_w * xsize * sinAngle + m_textureData.texture.m_h * ysize * cosAngle); else
			if(order == 3) ret.set(xp + m_setPosition.x + m_textureData.texture.m_h * ysize * (- sinAngle), 
				yp + m_setPosition.y + m_textureData.texture.m_h * ysize * cosAngle);
		}else
		{//需要裁剪
			if(m_isEnableOutsideChip == 0)
			{
				if(order == 0) ret.set(xp + m_setPosition.x,
					yp + m_setPosition.y); else
				if(order == 1) ret.set(xp + m_setPosition.x + m_textureData.textureSize.x * xsize * cosAngle, 
					yp + m_setPosition.y + m_textureData.textureSize.x * xsize * sinAngle); else
				if(order == 2) ret.set(xp + m_setPosition.x + m_textureData.textureSize.x * xsize * cosAngle + m_textureData.textureSize.y * ysize * (- sinAngle), 
					yp + m_setPosition.y + m_textureData.textureSize.x * xsize * sinAngle + m_textureData.textureSize.y * ysize * cosAngle); else
				if(order == 3) ret.set(xp + m_setPosition.x + m_textureData.textureSize.y * ysize * (- sinAngle), 
					yp + m_setPosition.y + m_textureData.textureSize.y * ysize * cosAngle);
			}else
			{
				if(order == 0) ret.set(xp + m_setPosition.x,
					yp + m_setPosition.y); else
				if(order == 1) ret.set(xp + m_setPosition.x + m_clipOutsideRect.getWidth() * xsize * cosAngle, 
					yp + m_setPosition.y + m_clipOutsideRect.getWidth() * xsize * sinAngle); else
				if(order == 2) ret.set(xp + m_setPosition.x + m_clipOutsideRect.getWidth() * xsize * cosAngle + m_clipOutsideRect.getHeight() * ysize * (- sinAngle), 
					yp + m_setPosition.y + m_clipOutsideRect.getHeight() * ysize * cosAngle + m_clipOutsideRect.getWidth() * xsize * sinAngle); else
				if(order == 3) ret.set(xp + m_setPosition.x + m_clipOutsideRect.getHeight() * ysize * (- sinAngle), 
					yp + m_setPosition.y + m_clipOutsideRect.getHeight() * ysize * cosAngle);
			}
		}
	}else
	{//中心对其，旋转问题没有搞定
		if(m_isEnableOutsideChip == 0 && m_textureData.isEnableInsideClip == 0)
		{//不需要裁减
			if(order == 0) ret.set(xp + m_setPosition.x + m_textureData.texture.m_w * (0.5f - 0.5f * xsize * cosAngle) + m_textureData.texture.m_h * 0.5f * ysize * sinAngle,
				yp + m_setPosition.y + m_textureData.texture.m_h * (0.5f - 0.5f * ysize * cosAngle) - m_textureData.texture.m_w * 0.5f * xsize * sinAngle); else
			if(order == 1) ret.set(xp + m_setPosition.x + m_textureData.texture.m_w * (0.5f + 0.5f * xsize * cosAngle) + m_textureData.texture.m_h * 0.5f * ysize * sinAngle,
				yp + m_setPosition.y + m_textureData.texture.m_h * (0.5f - 0.5f * ysize * cosAngle) + m_textureData.texture.m_w * 0.5f * xsize * sinAngle); else
			if(order == 2) ret.set(xp + m_setPosition.x + m_textureData.texture.m_w * (0.5f + 0.5f * xsize * cosAngle) - m_textureData.texture.m_h * 0.5f * ysize * sinAngle,
				yp + m_setPosition.y + m_textureData.texture.m_h * (0.5f + 0.5f * ysize * cosAngle) + m_textureData.texture.m_w * 0.5f * xsize * sinAngle); else
			if(order == 3) ret.set(xp + m_setPosition.x + m_textureData.texture.m_w * (0.5f - 0.5f * xsize * cosAngle) - m_textureData.texture.m_h * 0.5f * ysize * sinAngle,
				yp + m_setPosition.y + m_textureData.texture.m_h * (0.5f + 0.5f * ysize * cosAngle) - m_textureData.texture.m_w * 0.5f * xsize * sinAngle);
		}else
		{//需要裁剪
			if(m_isEnableOutsideChip == 0)
			{
				if(order == 0) ret.set(xp + m_setPosition.x + m_textureData.textureSize.x * (0.5f - 0.5f * xsize * cosAngle) + m_textureData.textureSize.y * 0.5f * ysize * sinAngle,
					yp + m_setPosition.y + m_textureData.textureSize.y * (0.5f - 0.5f * ysize * cosAngle) - m_textureData.textureSize.x * 0.5f * xsize * sinAngle); else
				if(order == 1) ret.set(xp + m_setPosition.x + m_textureData.textureSize.x * (0.5f + 0.5f * xsize * cosAngle) + m_textureData.textureSize.y * 0.5f * ysize * sinAngle,
					yp + m_setPosition.y + m_textureData.textureSize.y * (0.5f - 0.5f * ysize * cosAngle) + m_textureData.textureSize.x * 0.5f * xsize * sinAngle); else
				if(order == 2) ret.set(xp + m_setPosition.x + m_textureData.textureSize.x * (0.5f + 0.5f * xsize * cosAngle) - m_textureData.textureSize.y * 0.5f * ysize * sinAngle,
					yp + m_setPosition.y + m_textureData.textureSize.y * (0.5f + 0.5f * ysize * cosAngle) + m_textureData.textureSize.x * 0.5f * xsize * sinAngle); else
				if(order == 3) ret.set(xp + m_setPosition.x + m_textureData.textureSize.x * (0.5f - 0.5f * xsize * cosAngle) - m_textureData.textureSize.y * 0.5f * ysize * sinAngle, 
					yp + m_setPosition.y + m_textureData.textureSize.y * (0.5f + 0.5f * ysize * cosAngle) - m_textureData.textureSize.x * 0.5f * xsize * sinAngle);
			}else
			{
				if(order == 0) ret.set(xp + m_setPosition.x + m_clipOutsideRect.getWidth() * (0.5f - 0.5f * xsize * cosAngle) + m_clipOutsideRect.getHeight() * 0.5f * ysize * sinAngle,
					yp + m_setPosition.y + m_clipOutsideRect.getHeight() * (0.5f - 0.5f * ysize * cosAngle) - m_clipOutsideRect.getWidth() * 0.5f * xsize * sinAngle); else
				if(order == 1) ret.set(xp + m_setPosition.x + m_clipOutsideRect.getWidth() * (0.5f + 0.5f * xsize * cosAngle) + m_clipOutsideRect.getHeight() * 0.5f * ysize * sinAngle, 
					yp + m_setPosition.y + m_clipOutsideRect.getHeight() * (0.5f - 0.5f * ysize * cosAngle) + m_clipOutsideRect.getWidth() * 0.5f * xsize * sinAngle); else
				if(order == 2) ret.set(xp + m_setPosition.x + m_clipOutsideRect.getWidth() * (0.5f + 0.5f * xsize * cosAngle) - m_clipOutsideRect.getHeight() * 0.5f * ysize * sinAngle,
					yp + m_setPosition.y + m_clipOutsideRect.getHeight() * (0.5f + 0.5f * ysize * cosAngle) + m_clipOutsideRect.getWidth() * 0.5f * xsize * sinAngle); else
				if(order == 3) ret.set(xp + m_setPosition.x + m_clipOutsideRect.getWidth() * (0.5f - 0.5f * xsize * cosAngle) - m_clipOutsideRect.getHeight() * 0.5f * ysize * sinAngle,
					yp + m_setPosition.y + m_clipOutsideRect.getHeight() * (0.5f + 0.5f * ysize * cosAngle) - m_clipOutsideRect.getWidth() * 0.5f * xsize * sinAngle);
			}
		}
	}
	return ret;
}