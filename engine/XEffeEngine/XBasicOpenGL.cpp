#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
//#include "stdio.h"
//#include "math.h"

#include "XBasicOpenGL.h"
//#include "XBasicFun.h"
#include "XPixelCommon.h"

//#include "XResourcePack.h"
//#include "XLogBook.h"
#include "XShaderGLSL.h"
#include "XResourceManager.h"
#include "XFile.h"
//#include "XMath\XVector4.h"
//#include "XMath\XVector3.h"

namespace XE{
#if TEX_DEBUG
int XTSum = 0;
#endif
#if TEX_DEBUG1
int CPSum = 0;
#endif
namespace XGL
{
//	const int srcBlendMode[9] = { GL_ZERO,GL_ONE,GL_DST_COLOR,GL_ONE_MINUS_DST_COLOR,GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,
//			GL_DST_ALPHA,GL_ONE_MINUS_DST_ALPHA,GL_SRC_ALPHA_SATURATE };
//	const int dstBlendMode[8] = { GL_ZERO,GL_ONE,GL_SRC_COLOR,GL_ONE_MINUS_SRC_COLOR,GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,
//			GL_DST_ALPHA,GL_ONE_MINUS_DST_ALPHA };
	//	bool blendFlag = false;
	//	int blendSrc = GL_ZERO;
	//	int blendDst = GL_ONE;
	//	bool texture2DFlag = false;
	//	unsigned int curTextureID[32];

	XShaderResult printShaderInfoLog(GLuint obj)
	{
		int infologLength = 0;
		glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
		if (infologLength <= 0) return CMP_SUCCESS;
		int charsWritten = 0;
		std::string infoLog;
		infoLog.resize(infologLength);
		glGetShaderInfoLog(obj, infologLength, &charsWritten, &(infoLog[0]));
		LogNull(infoLog.c_str());
		if (infoLog.find("error") != std::string::npos) return CMP_ERROR;
		return CMP_WARNING;
	}
	XShaderResult printProgramInfoLog(GLuint obj)
	{
		int infologLength = 0;
		glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
		if (infologLength <= 0) return CMP_SUCCESS;

		int charsWritten = 0;
		std::string infoLog;
		infoLog.resize(infologLength);
		glGetProgramInfoLog(obj, infologLength, &charsWritten, &(infoLog[0]));
		LogNull(infoLog.c_str());
		if (infoLog.find("error") != std::string::npos) return CMP_ERROR;
		return CMP_WARNING;
	}
	int printOglError(const char *file, int line)
	{
		int retCode = 0;
		while (glGetError() != GL_NO_ERROR)
		{
			//	LogNull("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
			LogNull("glError in file %s @ line %d", file, line);
			//printf("glError: 0x%x in file %s @ line %d\n",glErr,file,line);
			retCode = 1;
		}
		return retCode;
	}
	XBool setShader(const char* vertFile, const char* fragFile, int &pHandle, XResPos resPos)
	{
		if (vertFile == NULL || fragFile == NULL) return XFalse;
		char *vs = XFile::readFileToStr(vertFile, resPos);
		char *fs = XFile::readFileToStr(fragFile, resPos);
		if (vs == NULL || fs == NULL) return XFalse;

		GLuint v = glCreateShader(GL_VERTEX_SHADER);
		GLuint f = glCreateShader(GL_FRAGMENT_SHADER);
		//v = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
		//f = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

		glShaderSource(v, 1, &vs, NULL);
		glShaderSource(f, 1, &fs, NULL);
		//glShaderSourceARB(v, 1, &vv,NULL);
		//glShaderSourceARB(f, 1, &ff,NULL);

		XMem::XDELETE_ARRAY(vs);
		XMem::XDELETE_ARRAY(fs);

		glCompileShader(v);
		glCompileShader(f);
		//glCompileShaderARB(v);
		//glCompileShaderARB(f);

		if (printShaderInfoLog(v) == CMP_ERROR) return false;
		if (printShaderInfoLog(f) == CMP_ERROR) return false;

		if (pHandle == 0) pHandle = glCreateProgram();	//如果没有建立则这里建立

		glAttachShader(pHandle, v);
		glAttachShader(pHandle, f);
		//pHandle = glCreateProgramObjectARB();
		//glAttachObjectARB(pHandle,v);
		//glAttachObjectARB(pHandle,f);

		glLinkProgram(pHandle);
		//glLinkProgramARB(pHandle);
		if (printProgramInfoLog(pHandle) == CMP_ERROR) return false;
		return XTrue;
	}
	XBool setShaderFromStr(const char* vStr, const char *fStr, const char *gStr, XShaderHandle &pHandle)
	{
		if (vStr == NULL) return XFalse;
		if (pHandle.shaderV == 0) pHandle.shaderV = glCreateShader(GL_VERTEX_SHADER);
		if (fStr != NULL && pHandle.shaderF == 0)
			pHandle.shaderF = glCreateShader(GL_FRAGMENT_SHADER);
		if (gStr != NULL && pHandle.shaderG == 0)
			pHandle.shaderG = glCreateShader(GL_GEOMETRY_SHADER_EXT);

		glShaderSource(pHandle.shaderV, 1, &vStr, NULL);
		if (fStr != NULL)
			glShaderSource(pHandle.shaderF, 1, &fStr, NULL);
		if (gStr != NULL)
			glShaderSource(pHandle.shaderG, 1, &gStr, NULL);

		glCompileShader(pHandle.shaderV);
		if (fStr != NULL) glCompileShader(pHandle.shaderF);
		if (gStr != NULL) glCompileShader(pHandle.shaderG);

		//检查编译是否成功
		GLint status = GL_FALSE;
		glGetShaderiv(pHandle.shaderV, GL_COMPILE_STATUS, &status);
		if(!status) return false;
	//	GLuint err = glGetError();
	//	if(err != GL_NO_ERROR)	return false;
	//	//if(status == GL_TRUE)

		LogStr("vStr");
		printShaderInfoLog(pHandle.shaderV);
		if (fStr != NULL)
		{
			LogStr("fStr");
			printShaderInfoLog(pHandle.shaderF);
			glGetShaderiv(pHandle.shaderF, GL_COMPILE_STATUS, &status);
			if (!status) return false;
		}
		if (gStr != NULL)
		{
			LogStr("gStr");
			printShaderInfoLog(pHandle.shaderG);
			glGetShaderiv(pHandle.shaderG, GL_COMPILE_STATUS, &status);
			if (!status) return false;
		}

		if (pHandle.shaderHandle == 0) pHandle.shaderHandle = glCreateProgram();	//如果没有建立则这里建立
		glAttachShader(pHandle.shaderHandle, pHandle.shaderV);
		if (fStr != NULL) glAttachShader(pHandle.shaderHandle, pHandle.shaderF);
		if (gStr != NULL) glAttachShader(pHandle.shaderHandle, pHandle.shaderG);

		glLinkProgram(pHandle.shaderHandle);
		printProgramInfoLog(pHandle.shaderHandle);
		glGetShaderiv(pHandle.shaderHandle, GL_LINK_STATUS, &status);
		return status;
	}
	XBool setShader(const char* vertFile, const char* fragFile, const char* geomFile, XShaderHandle &pHandle,
		XResPos resPos)
	{
		if (vertFile == NULL) return XFalse;
		char *vs = XFile::readFileToStr(vertFile, resPos);
		if (vs == NULL) return false;
		char *fs = NULL;
		if (fragFile != NULL)
		{
			fs = XFile::readFileToStr(fragFile, resPos);
			if (fs == NULL) return false;
		}
		char *gs = NULL;
		if (geomFile != NULL)
		{
			gs = XFile::readFileToStr(geomFile, resPos);
			if (gs == NULL) return XFalse;
		}
		//1、简化版本
		XBool ret = setShaderFromStr(vs, fs, gs, pHandle);
		XMem::XDELETE_ARRAY(vs);
		XMem::XDELETE_ARRAY(fs);
		XMem::XDELETE_ARRAY(gs);
		return ret;
		//2、原始版本
		//if(pHandle.shaderV == 0) pHandle.shaderV = glCreateShader(GL_VERTEX_SHADER);
		//if(fragFile != NULL && pHandle.shaderF == 0) 
		//	pHandle.shaderF = glCreateShader(GL_FRAGMENT_SHADER);
		//if(geomFile != NULL && pHandle.shaderG == 0)
		//	pHandle.shaderG = glCreateShader(GL_GEOMETRY_SHADER_EXT);
		////v = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
		////f = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

		//glShaderSource(pHandle.shaderV, 1, &vs,NULL);
		//if(fragFile != NULL)
		//	glShaderSource(pHandle.shaderF, 1, &fs,NULL);
		//if(geomFile != NULL)
		//	 glShaderSource(pHandle.shaderG, 1, &gs,NULL);
		////glShaderSourceARB(v, 1, &vv,NULL);
		////glShaderSourceARB(f, 1, &ff,NULL);

		//XMem::XDELETE_ARRAY(vs);
		//XMem::XDELETE_ARRAY(fs);
		//XMem::XDELETE_ARRAY(gs);

		//glCompileShader(pHandle.shaderV);
		//if(fragFile != NULL) glCompileShader(pHandle.shaderF);
		//if(geomFile != NULL) glCompileShader(pHandle.shaderG);
		////glCompileShaderARB(v);
		////glCompileShaderARB(f);

		//LogStr(vertFile);
		//if(printShaderInfoLog(pHandle.shaderV) == CMP_ERROR) return false;
		//if(fragFile != NULL)
		//{
		//	LogStr(fragFile);
		//	if(printShaderInfoLog(pHandle.shaderF) == CMP_ERROR) return false;
		//}
		//if(geomFile != NULL)
		//{
		//	LogStr(geomFile);
		//	if(printShaderInfoLog(pHandle.shaderG) == CMP_ERROR) return false;
		//}

		//if(pHandle.shaderHandle == 0) pHandle.shaderHandle = glCreateProgram();	//如果没有建立则这里建立

		//glAttachShader(pHandle.shaderHandle,pHandle.shaderV);
		//if(fragFile != NULL) glAttachShader(pHandle.shaderHandle,pHandle.shaderF);
		//if(geomFile != NULL) glAttachShader(pHandle.shaderHandle,pHandle.shaderG);
		////pHandle = glCreateProgramObjectARB();
		////glAttachObjectARB(pHandle,v);
		////glAttachObjectARB(pHandle,f);

		//glLinkProgram(pHandle.shaderHandle);
		////glLinkProgramARB(pHandle);
		//if(printProgramInfoLog(pHandle.shaderHandle) == CMP_ERROR) return false;
		//return XTrue;
	}
	/*void changeSize(int h, int w)
	{
		// Prevent a divide by zero, when window is too short
		// (you cant make a window of zero width).
		if(h == 0) h = 1;

		float ratio = 1.0f* w / h;

		// Reset the coordinate system before modifying
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		// Set the viewport to be the entire window
		glViewport(0, 0, w, h);

		// Set the correct perspective.
		//gluPerspective(90,ratio,1,1000);
		glMatrixMode(GL_MODELVIEW);

	}*/
	//XBool TextureLoad(GLuint &texture,const char *filename,int withAlpha,int *w,int *h,XResPos resPos)
	//{
	//	if(filename == NULL || strlen(filename) <= 0) return XFalse;
	//	if(withAlpha != 0) withAlpha = 1;
	//
	//	SDL_Surface *temp = NULL;
	//	SDL_Surface *image = NULL;
	//
	//	Uint32 rmask, gmask, bmask, amask;
	//
	//#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	//	rmask = 0xff000000; gmask = 0x00ff0000; bmask = 0x0000ff00;	amask = 0x000000ff;
	//#else
	//	rmask = 0x000000ff;	gmask = 0x0000ff00;	bmask = 0x00ff0000;	amask = 0xff000000;
	//#endif
	//	
	//	if((temp = (SDL_Surface *) loadImage(filename,withAlpha,resPos)) != NULL) 
	//	{
	//		if((temp->w > XEE::maxTetureSize) || (temp->h > XEE::maxTetureSize)) 
	//		{
	//			fprintf(stderr, "Image size exceeds max texture size, which is %d pixels for each side\n",XEE::maxTetureSize);
	//			LogNull("Image size exceeds max texture size, which is %d pixels for each side\n",XEE::maxTetureSize);
	//			SDL_FreeSurface(temp);
	//			temp = NULL;
	//			return 0;
	//			//exit(-1);
	//		}
	//		//printf("Texture Max Size:%d x %d\n",max_size,max_size);
	//
	//		if(w != NULL && h != NULL)
	//		{
	//			(* w) = temp->w;
	//			(* h) = temp->h;
	//		}
	//
	//		// Check for NPOT textures 
	//		if (isNPOT(temp->w, temp->h)) //检查图片的尺寸是否符合2的n次方
	//		{//如果不符合2的n次方的图片下面进行特殊的处理(这个处理目前还存在问题,对于不符合2的n次方的透明通道贴图)
	//			GLint wreal = 0, hreal = 0;
	//			GLint xpad = 0, ypad = 0;
	//			
	//			wreal = (int)powf(2.0, ceilf(logf((float)temp->w)/logf(2.0f)));
	//			hreal = (int)powf(2.0, ceilf(logf((float)temp->h)/logf(2.0f)));
	//			xpad = (wreal - temp->w)/2;
	//			ypad = (hreal - temp->h)/2;
	//
	//			if(withAlpha != 0)
	//			{
	//				image = SDL_CreateRGBSurface(SDL_SWSURFACE, wreal, hreal, 32, rmask, gmask, bmask, amask);
	//			}else
	//			{
	//				image = SDL_CreateRGBSurface(SDL_SWSURFACE, wreal, hreal, 24, rmask, gmask, bmask, 0);
	//			}
	//			if(image == NULL)
	//			{
	//				SDL_FreeSurface(temp);
	//				temp = NULL;
	//				return XFalse;
	//			}
	//			SDL_Rect dst = {xpad, ypad, temp->w, temp->h};
	//			//当存在alpha通道时，这里这是不进行混合直接覆盖
	//			SDL_SetAlpha(temp,0,255);
	//			SDL_SetAlpha(image,0,255);
	//			SDL_BlitSurface(temp, 0, image, &dst);
	//			SDL_FreeSurface(temp);
	//			temp = NULL;
	//
	//			// OpenGL Texture creation	
	//			glGenTextures(1, &texture);
	//			XGL::BindTexture2D(texture);
	//			//效果好
	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//#if WITHXSPRITE_EX
	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//#endif			//速度快
	//			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//		//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	//		//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	//
	//		//	if(withAlpha != 0)
	//			if(image->format->BitsPerPixel == 32)
	//			{
	//			//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA4, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
	//				if(image->format->Rshift == 0)
	//					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
	//				else
	//					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, image->pixels);
	//			}else
	//			if(image->format->BitsPerPixel == 24)
	//			{
	//			//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB4, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	//				if(image->format->Rshift == 0)
	//					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	//				else
	//					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_BGR, GL_UNSIGNED_BYTE, image->pixels);
	//			}
	//
	//			SDL_FreeSurface(image);
	//			image = NULL;
	//		}else
	//		{//符合2的n次方的图片
	//			// OpenGL Texture creation	
	//			glGenTextures(1, &texture);
	//			XGL::BindTexture2D(texture);
	//		//	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	//
	//			//效果好
	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//#if WITHXSPRITE_EX
	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//#endif			//速度快
	//			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//		//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	//		//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	//
	//			if(temp->format->BitsPerPixel == 32)
	//			{
	//			//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA4, temp->w, temp->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, temp->pixels);
	//				if(temp->format->Rshift == 0)
	//					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, temp->w, temp->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, temp->pixels);
	//				else
	//					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, temp->w, temp->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, temp->pixels);
	//			}else
	//			if(temp->format->BitsPerPixel == 24)
	//			{
	//			//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB4, temp->w, temp->h, 0, GL_BGR, GL_UNSIGNED_BYTE, temp->pixels);
	//				if(temp->format->Rshift == 0)
	//					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, temp->w, temp->h, 0, GL_RGB, GL_UNSIGNED_BYTE, temp->pixels);
	//				else
	//					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, temp->w, temp->h, 0, GL_BGR, GL_UNSIGNED_BYTE, temp->pixels);
	//			}
	//
	//			SDL_FreeSurface(temp);
	//			temp = NULL;
	//		}
	//		
	////		// OpenGL Texture creation	
	////		glGenTextures(1, &texture);
	////		XGL::BindTexture2D(texture);
	////
	////		//效果好
	////	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	////	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	////#if WITHXSPRITE_EX
	////	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	////	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	////#endif
	////		//速度快
	////		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	////		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	////	//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	////	//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	////
	////		if(withAlpha != 0)
	////		{
	////			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
	////			//glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, image->w, image->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, image->pixels);
	////		}else
	////		{
	////			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	////		}
	////
	////		SDL_FreeSurface(image);
	////		image = NULL;
	//		return XTrue;
	//	}else
	//	{
	//		return XFalse;
	//	}
	////	return XTrue;
	//}
	//测试发现这个函数中的SDL_FreeSurface会存在内存IPR和FIM错误，具体需要SDL的代码实现来检测
	XBool TextureLoadEx(GLuint &texture, const char *filename, int *w, int *h, 
		XResPos resPos, bool isInThread)
	{
		if (filename == NULL || strlen(filename) <= 0) return XFalse;

		XPixels<XCurPixel> tmp;
		if (!tmp.load(filename, resPos))
			return XFalse;
		if ((tmp.getWidth() > XEG.getMaxTextureSize() || tmp.getHeight() > XEG.getMaxTextureSize()) && XEG.getMaxTextureSize() > 0) 	//贴图最大尺寸的检测
		{
			fprintf(stderr, "Image size exceeds max texture size, which is %d pixels for each side\n", XEG.getMaxTextureSize());
			LogNull("Image size exceeds max texture size, which is %d pixels for each side\n", XEG.getMaxTextureSize());
			tmp.release();
			return XFalse;
		}		
		//记录原始的大小
		int sw = tmp.getWidth();
		int sh = tmp.getHeight();
		//做像素适应变换
		if (XEG.getTexNeedFit2N()) tmp.fitNPot();	//适应2的n次方
		if (isInThread)
		{
			XEG.hdcLock();
			wglMakeCurrent(XEG.getHDC(), XEG.getCopyHGLRC());
		}
		if (glIsTexture(texture))
		{
			if (w != nullptr && h != nullptr && *w == sw && *h == sh)
			{//只有相同尺寸才进行更新，注意这里是由于考虑到之上面的逻辑才加的这个限制，否则会造成问题
				//XGL::EnableTexture2D();
				XGL::BindTexture2D(texture);
				//glTexSubImage2D glTexImage2D
				switch (tmp.getColorMode())
				{
				case COLOR_RGBA:
					//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tmp.getWidth(), tmp.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tmp.getPixel());
					glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, tmp.getWidth(), tmp.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, tmp.getPixel());
					break;
				case COLOR_BGRA:
					//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tmp.getWidth(), tmp.getHeight(), 0, GL_BGRA, GL_UNSIGNED_BYTE, tmp.getPixel());
					glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, tmp.getWidth(), tmp.getHeight(), GL_BGRA, GL_UNSIGNED_BYTE, tmp.getPixel());
					break;
				case COLOR_RGB:
					//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tmp.getWidth(), tmp.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, tmp.getPixel());
					glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, tmp.getWidth(), tmp.getHeight(), GL_RGB, GL_UNSIGNED_BYTE, tmp.getPixel());
					break;
				case COLOR_BGR:
					//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tmp.getWidth(), tmp.getHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, tmp.getPixel());
					glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, tmp.getWidth(), tmp.getHeight(), GL_BGR, GL_UNSIGNED_BYTE, tmp.getPixel());
					break;
				case COLOR_GRAY:
					//glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, tmp.getWidth(), tmp.getHeight(), 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, tmp.getPixel());
					glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, tmp.getWidth(), tmp.getHeight(), GL_LUMINANCE, GL_UNSIGNED_BYTE, tmp.getPixel());
					break;
				}
			}
			else
			{
				switch (tmp.getColorMode())
				{
				case COLOR_RGBA:
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tmp.getWidth(), tmp.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tmp.getPixel());
					break;
				case COLOR_BGRA:
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tmp.getWidth(), tmp.getHeight(), 0, GL_BGRA, GL_UNSIGNED_BYTE, tmp.getPixel());
					break;
				case COLOR_RGB:
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tmp.getWidth(), tmp.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, tmp.getPixel());
					break;
				case COLOR_BGR:
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tmp.getWidth(), tmp.getHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, tmp.getPixel());
					break;
				case COLOR_GRAY:
					glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, tmp.getWidth(), tmp.getHeight(), 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, tmp.getPixel());
					break;
				}
			}
		}
		else
		{
			if (w != nullptr) *w = sw;
			if (h != nullptr) *h = sh;
			// OpenGL Texture creation	
			glGenTextures(1, &texture);
			XGL::BindTexture2D(texture);
			//	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
				//效果好
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#if WITHXSPRITE_EX
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#endif
			//速度快
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
		//	float largest_supported_anisotropy;
		//	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT,&largest_supported_anisotropy);	//各异向性纹理过滤
		//	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,largest_supported_anisotropy);

			switch (tmp.getColorMode())
			{
			case COLOR_RGBA:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tmp.getWidth(), tmp.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tmp.getPixel());
				break;
			case COLOR_BGRA:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tmp.getWidth(), tmp.getHeight(), 0, GL_BGRA, GL_UNSIGNED_BYTE, tmp.getPixel());
				break;
			case COLOR_RGB:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tmp.getWidth(), tmp.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, tmp.getPixel());
				break;
			case COLOR_BGR:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tmp.getWidth(), tmp.getHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, tmp.getPixel());
				break;
			case COLOR_GRAY:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, tmp.getWidth(), tmp.getHeight(), 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, tmp.getPixel());
				break;
			}
		}
		if (isInThread)
		{
			wglMakeCurrent(NULL, NULL);//nullptr
			XEG.hdcUnlock();
		}
#if WITH_ENGINE_STATISTICS
		XEG.addStaticsticsTexInfo(texture);
#endif

		tmp.release();
		return XTrue;
	}
	XBool TextureLoadQuarter(GLuint &texture, const char *filename, XResPos resPos)
	{
		XPixels<XCurPixel> tmp;
		if (!tmp.load(filename, resPos)) return XFalse;
		int w = tmp.getWidth() >> 2;
		int h = tmp.getHeight() >> 2;
		unsigned char * tempPixel = XMem::createArrayMem<unsigned char>(w * h * 4);
		if (tempPixel == NULL)
		{
			tmp.release();
			return XFalse;
		}
		switch (tmp.getColorMode())
		{
		case COLOR_RGBA:
		case COLOR_BGRA:
		{
			int *p = (int *)tmp.getPixel();
			int *p1 = (int *)tempPixel;
			int indexS, indexD;
			for (int i = 0; i < tmp.getHeight(); i += 4)
			{
				indexS = i * tmp.getWidth();
				indexD = (i >> 2) * w;
				for (int j = 0; j < tmp.getWidth(); j += 4)
				{
					*(p1 + indexD + (j >> 2)) = *(p + indexS + j);
				}
			}
		}
		break;
		case COLOR_RGB:
		case COLOR_BGR:
		{
			unsigned char *p = tmp.getPixel();
			unsigned char *p1 = tempPixel;		//这里可以进行优化
			int indexD = 0, indexS;
			for (int j = 0; j < tmp.getHeight(); j += 4)
			{
				indexS = j * tmp.getWidth() * 3;
				indexD = (j >> 2) * w * 4;
				for (int i = 0; i < tmp.getWidth(); i += 4, indexD += 4, indexS += 12)
				{
					*(p1 + indexD + 0) = *(p + indexS + 0);
					*(p1 + indexD + 1) = *(p + indexS + 1);
					*(p1 + indexD + 2) = *(p + indexS + 2);
					*(p1 + indexD + 3) = 255;
				}
			}
		}
		break;
		default:	//其他格式不支持
			tmp.release();
			XMem::XDELETE_ARRAY(tempPixel);
			return XFalse;
		}
		glGenTextures(1, &texture);
		XGL::BindTexture2D(texture);
		//效果好
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#if WITHXSPRITE_EX
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#endif
		//速度快
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		switch (tmp.getColorMode())
		{
		case COLOR_RGBA:
		case COLOR_RGB:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tempPixel);
			break;
		case COLOR_BGRA:
		case COLOR_BGR:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, tempPixel);
			break;
		default:	//其他格式不支持
			tmp.release();
			XMem::XDELETE_ARRAY(tempPixel);
			return XFalse;
		}
#if WITH_ENGINE_STATISTICS
		XEG.addStaticsticsTexInfo(texture);
#endif
		tmp.release();
		XMem::XDELETE_ARRAY(tempPixel);
		return XTrue;
	}
	void showGLInfoToLog()
	{
		GLint major, minor;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		LogNull("GL Vendor    :%s", glGetString(GL_VENDOR));
		LogNull("GL Renderer  :%s", glGetString(GL_RENDERER));
		LogNull("GL Version (string)  :%s", glGetString(GL_VERSION));
		LogNull("GL Version (integer) :%d.%d", major, minor);
		LogNull("GLSL Version :%s", glGetString(GL_SHADING_LANGUAGE_VERSION));
	}
}
namespace XRender
{
	void drawLineAntiColor(const XVec2& a, const XVec2& b, float width, XLineStyle type)
	{
		XGL::setBlendInverse();	//反色效果
		glColor4fv(XFColor::white);
		glLineWidth(width);
		XGL::DisableTexture2D();

		if (type == LS_SOLID)
		{//实线
			glBegin(GL_LINES);
			glVertex2fv(a);
			glVertex2fv(b);
			glEnd();
		}
		else
		{//虚线
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(1, type);
			glBegin(GL_LINES);
			glVertex2fv(a);
			glVertex2fv(b);
			glEnd();
			glDisable(GL_LINE_STIPPLE);
		}
		XGL::DisableBlend();
	}
	void drawLine(const XVec2&a, const XVec2&b, float width,
		const XFColor& c)
	{
		XGL::setBlendAlpha();
		glColor4fv(c);
		glLineWidth(width);
		XGL::DisableTexture2D();
		glBegin(GL_LINES);
		glVertex2fv(a);
		glVertex2fv(b);
		glEnd();
		XGL::DisableBlend();
	}
	void drawLine(const XVec2& p0, const XVec2& p1, float width,
		const XFColor& c, XLineStyle type)
	{
		XGL::setBlendAlpha();
		glColor4fv(c);
		glLineWidth(width);
		XGL::DisableTexture2D();
		if (type == LS_SOLID)
		{//实线
			glBegin(GL_LINES);
			glVertex2fv(p0);
			glVertex2fv(p1);
			glEnd();
		}
		else
		{//虚线
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(1, type);
			glBegin(GL_LINES);
			glVertex2fv(p0);
			glVertex2fv(p1);
			glEnd();
			glDisable(GL_LINE_STIPPLE);
		}
		XGL::DisableBlend();
	}
	void drawLines(const std::vector<XVec2>& points, float width, const XFColor& c)
	{
		XGL::setBlendAlpha();
		glColor4fv(c);
		glLineWidth(width);
		XGL::DisableTexture2D();
		glBegin(GL_LINE_STRIP);
		for (auto it = points.begin(); it != points.end(); ++it)
		{
			glVertex2fv(*it);
		}
		glEnd();
		XGL::DisableBlend();
	}
	void drawLinesVbo(unsigned int v,int pointSum,float w,float r,float g,float b)
	{
		glLineWidth(w);
		glColor3f(r,g,b);
		XGL::DisableTexture2D();
		glEnableClientState(GL_VERTEX_ARRAY);	
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,v);
		glVertexPointer(2,GL_FLOAT,0,NULL);
		glDrawArrays(GL_LINE_STRIP,0,pointSum);
	
		glDisableClientState(GL_VERTEX_ARRAY);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);
	}
	void drawLinesVbo(unsigned int v,int pointSum,float w,const XFColor& c)
	{
		XGL::setBlendAlpha();
		glLineWidth(w);
		glColor4fv(c);
		XGL::DisableTexture2D();
		glEnableClientState(GL_VERTEX_ARRAY);	
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,v);
		glVertexPointer(2,GL_FLOAT,0,NULL);
		glDrawArrays(GL_LINE_STRIP,0,pointSum);
		XGL::DisableBlend();
		glDisableClientState(GL_VERTEX_ARRAY);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);
		//if(isMultiSampleSupport && isLineSmooth) XGL::DisableBlend();
	}
	//void drawLine(const XVec2& p,float angle,float length)
	//{
	//	drawLine(p,p + XMath::getRotateRate(angle * DEGREE2RADIAN) * length);
	//}
	void drawRect(const XRect& rect, float width, const XFColor& c, XLineStyle type)
	{
		XGL::setBlendAlpha();
		glColor4fv(c);
		glLineWidth(width);
		XGL::DisableTexture2D();

		if (type == LS_SOLID)
		{//实线
			drawRectBase(rect);
		}else
		{//虚线
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(1, type);
			drawRectBase(rect);
			glDisable(GL_LINE_STIPPLE);
		}
		XGL::DisableBlend();
	}
	void drawRect(const XVec2& pos,const XVec2& size,float width, const XFColor& c)
	{
		XGL::setBlendAlpha();
		glColor4fv(c);
		glLineWidth(width);
		XGL::DisableTexture2D();
		drawQuadBase(pos - size, XVec2(pos.x + size.x, pos.y - size.y),
			pos + size, XVec2(pos.x - size.x, pos.y + size.y));
		XGL::DisableBlend();
	}
	void drawRectAntiColor(const XRect& rect, float width, XLineStyle type)
	{
		XGL::setBlendInverse();//反色效果
		glColor4fv(XFColor::white);
		glLineWidth(width);
		XGL::DisableTexture2D();
		if (type == LS_SOLID)
		{//实线
			drawRectBase(rect);
		}else
		{//虚线
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(1, type);
			drawRectBase(rect);
			glDisable(GL_LINE_STIPPLE);
		}
		XGL::DisableBlend();
	}
	void drawRectEx(float x,float y,float sizeX,float sizeY,float angle,const XFColor& cl)
	{
		float c = cos(angle);
		float s = sin(angle);
		glColor3fv(cl);
		glLineWidth(1);
		XGL::DisableTexture2D();
		drawQuadBase(XVec2(x - (sizeX * c - sizeY * s), y + (sizeX * s + sizeY * c)),
			XVec2(x - (sizeX * c + sizeY * s), y + (sizeX * s - sizeY * c)),
			XVec2(x + (sizeX * c - sizeY * s), y - (sizeX * s + sizeY * c)),
			XVec2(x + (sizeX * c + sizeY * s), y - (sizeX * s - sizeY * c)));
	}
	void drawQuad(const XVec2& p0,const XVec2& p1,const XVec2& p2,const XVec2& p3,
		float width,const XFColor& c)
	{
		XGL::setBlendAlpha();
		glColor4fv(c);
		glLineWidth(width);
		XGL::DisableTexture2D();
		drawQuadBase(p0,p1,p2,p3);
		XGL::DisableBlend();
	}
	void drawTriangle(const XVec2& p0,const XVec2& p1,const XVec2& p2,float width,float r,float g,float b)
	{
		glColor3f(r,g,b);
		glLineWidth(width);
		XGL::DisableTexture2D();
		glBegin(GL_LINE_LOOP);
		glVertex2fv(p0);
		glVertex2fv(p1);
		glVertex2fv(p2);
		glEnd();
	}
	void drawTriangle(const XVec2& p,float size,float width,const XFColor& color)
	{
		XGL::setBlendAlpha();
		glColor4fv(color);
		glLineWidth(width);
		XGL::DisableTexture2D();
		glBegin(GL_LINE_LOOP);
		glVertex2f(p.x,p.y - size);
		glVertex2f(p.x - size * SQRT3 * 0.5f,p.y + size * 0.5f);
		glVertex2f(p.x + size * SQRT3 * 0.5f,p.y + size * 0.5f);
		glEnd();
		XGL::DisableBlend();
	}
	//void drawPoint(float x,float y,float r,float g,float b,float a)
	//{
	//	XGL::setBlendAlpha();
	//	glColor4f(r,g,b,a);
	//	glPointSize(1.0f);
	//	XGL::DisableTexture2D();
	//	glBegin(GL_POINTS);
	//		glVertex2f(x,y);
	//	glEnd();
	//	XGL::DisableBlend();
	//}
	void drawPoint(const XVec2& p,const XFColor& color)
	{
		XGL::setBlendAlpha();
		glColor4fv(color);
		glPointSize(1.0f);
		XGL::DisableTexture2D();
		glBegin(GL_POINTS);
			glVertex2fv(p);
		glEnd();
		XGL::DisableBlend();
	}
	//void drawPoint(float x,float y,float size,float r,float g,float b,float a)
	//{
	//	XGL::setBlendAlpha();
	//		glColor4f(r,g,b,a);
	//		glPointSize(size);
	//	XGL::DisableTexture2D();
	//	glBegin(GL_POINTS);
	//		glVertex2f(x,y);
	//	glEnd();
	//	XGL::DisableBlend();
	//}
	void drawPoint(const XVec2& p, float size, const XFColor& color)
	{
		XGL::setBlendAlpha();
		glColor4fv(color);
		glPointSize(size);
		XGL::DisableTexture2D();
		glBegin(GL_POINTS);
		glVertex2fv(p);
		glEnd();
		XGL::DisableBlend();
	}
	void drawPoints(const std::vector<XVec2>& ps, float size, const XFColor& color)
	{
		if (ps.size() <= 0) return;
		XGL::setBlendAlpha();
		glColor4fv(color);
		glPointSize(size);
		XGL::DisableTexture2D();
		glBegin(GL_POINTS);
		for(int i = 0;i < ps.size();++ i)
			glVertex2fv(ps[i]);
		glEnd();
		XGL::DisableBlend();
	}
	void drawPointsVbo(unsigned int v,int pointSum,float s,float r,float g,float b)
	{
		glPointSize(s);
		glColor3f(r,g,b);
		XGL::DisableTexture2D();
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,v);
		glVertexPointer(2,GL_FLOAT,0,NULL);
		glDrawArrays(GL_POINTS,0,pointSum);

		glDisableClientState(GL_VERTEX_ARRAY);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);
	}
	void drawPointsVbo(unsigned int v,int pointSum,float s,const XFColor& c)
	{
		XGL::setBlendAlpha();
		glPointSize(s);
		glColor4fv(c);
		XGL::DisableTexture2D();
		glEnableClientState(GL_VERTEX_ARRAY);	
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,v);
		glVertexPointer(2,GL_FLOAT,0,NULL);
		glDrawArrays(GL_POINTS,0,pointSum);

		glDisableClientState(GL_VERTEX_ARRAY);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);
		XGL::DisableBlend();
	}
	void drawCross(const XVec2& p,float size,float w,float r,float g,float b)
	{
		glColor3f(r,g,b);
		glLineWidth(w);
		XGL::DisableTexture2D();
		glBegin(GL_LINES);
		glVertex2f(p.x - size,p.y);
		glVertex2f(p.x + size,p.y);
		glVertex2f(p.x,p.y - size);
		glVertex2f(p.x,p.y + size);
		glEnd();
	}
	void drawCross(const XVec2& p,float size,float w,const XFColor& color)
	{
		XGL::setBlendAlpha();
		glColor4fv(color);
		glLineWidth(w);
		XGL::DisableTexture2D();

		glBegin(GL_LINES);
		glVertex2f(p.x - size,p.y);
		glVertex2f(p.x + size,p.y);
		glVertex2f(p.x,p.y - size);
		glVertex2f(p.x,p.y + size);
		glEnd();

		XGL::DisableBlend();
	}
	void drawMaskRight(const XVec2& p,const XVec2& size,float width,const XFColor& color)
	{
		XGL::setBlendAlpha();
		glColor4fv(color);
		glLineWidth(width);
		XGL::DisableTexture2D();
		glBegin(GL_LINE_STRIP);
		glVertex2f(p.x - size.x,p.y);
		glVertex2f(p.x - size.x * 0.5f,p.y + size.y);
		glVertex2f(p.x + size.x,p.y - size.y);
		glEnd();
		XGL::DisableBlend();
	}
	void drawMaskWrong(const XVec2& p,const XVec2& size,float width,const XFColor& color)
	{
		XGL::setBlendAlpha();
		glColor4fv(color);
		glLineWidth(width);
		XGL::DisableTexture2D();
		glBegin(GL_LINES);
		glVertex2fv(p - size);
		glVertex2fv(p + size);
		glVertex2f(p.x + size.x,p.y - size.y);
		glVertex2f(p.x - size.x,p.y + size.y);
		glEnd();
		XGL::DisableBlend();
	}
	void drawLeft(const XVec2& p,const XVec2& size,float width,const XFColor& color)
	{
		XGL::setBlendAlpha();
		glColor4fv(color);
		glLineWidth(width);
		XGL::DisableTexture2D();
		glBegin(GL_LINE_STRIP);
		glVertex2f(p.x + size.x,p.y - size.y);
		glVertex2f(p.x - size.x,p.y);
		glVertex2fv(p + size);
		glEnd();
		XGL::DisableBlend();
	}
	void drawRight(const XVec2& p,const XVec2& size,float width,const XFColor& color)
	{
		XGL::setBlendAlpha();
		glColor4fv(color);
		glLineWidth(width);
		XGL::DisableTexture2D();
		glBegin(GL_LINE_STRIP);
		glVertex2fv(p - size);
		glVertex2f(p.x + size.x,p.y);
		glVertex2f(p.x - size.x,p.y + size.y);
		glEnd();
		XGL::DisableBlend();
	}
	void drawUp(const XVec2& p,const XVec2& size,float width,const XFColor& color)
	{
		XGL::setBlendAlpha();
		glColor4fv(color);
		glLineWidth(width);
		XGL::DisableTexture2D();
		glBegin(GL_LINE_STRIP);
		glVertex2f(p.x - size.x,p.y + size.y);
		glVertex2f(p.x,p.y - size.y);
		glVertex2fv(p + size);
		glEnd();
		XGL::DisableBlend();
	}
	void drawDown(const XVec2& p,const XVec2& size,float width,const XFColor& color)
	{
		XGL::setBlendAlpha();
		glColor4fv(color);
		glLineWidth(width);
		XGL::DisableTexture2D();
		glBegin(GL_LINE_STRIP);
		glVertex2fv(p - size);
		glVertex2f(p.x,p.y + size.y);
		glVertex2f(p.x + size.x,p.y - size.y);
		glEnd();
		XGL::DisableBlend();
	}
	void drawSave(const XVec2& p, const XVec2& size, float width, const XFColor& color)
	{
		XGL::setBlendAlpha();
		glColor4fv(color);
		glLineWidth(width);
		XGL::DisableTexture2D();
		glBegin(GL_LINES);
		//下半框
		glVertex2f(p.x - size.x, p.y);
		glVertex2f(p.x - size.x, p.y + size.y);
		glVertex2f(p.x - size.x, p.y + size.y);
		glVertex2fv(p + size);
		glVertex2fv(p + size);
		glVertex2f(p.x + size.x, p.y);
		//箭头
		glVertex2f(p.x, p.y - size.y);
		glVertex2f(p.x, p.y + size.y * 0.5f);

		glVertex2f(p.x - size.x * 0.5f, p.y);
		glVertex2f(p.x, p.y + size.y * 0.5f);
		glVertex2f(p.x, p.y + size.y * 0.5f);
		glVertex2f(p.x + size.x * 0.5f, p.y);
		glEnd();
		XGL::DisableBlend();
	}
	void drawLoad(const XVec2& p, const XVec2& size, float width, const XFColor& color)
	{
		XGL::setBlendAlpha();
		glColor4fv(color);
		glLineWidth(width);
		XGL::DisableTexture2D();
		glBegin(GL_LINES);
		//下半框
		glVertex2f(p.x - size.x, p.y);
		glVertex2f(p.x - size.x, p.y + size.y);
		glVertex2f(p.x - size.x, p.y + size.y);
		glVertex2fv(p + size);
		glVertex2fv(p + size);
		glVertex2f(p.x + size.x, p.y);
		//箭头
		glVertex2f(p.x, p.y - size.y);
		glVertex2f(p.x, p.y + size.y * 0.5f);

		glVertex2f(p.x - size.x * 0.5f, p.y - size.y * 0.5f);
		glVertex2f(p.x, p.y - size.y);
		glVertex2f(p.x, p.y - size.y);
		glVertex2f(p.x + size.x * 0.5f, p.y - size.y * 0.5f);
		glEnd();
		XGL::DisableBlend();
	}
	void drawDefault(const XVec2& p, const XVec2& size, float width, const XFColor& color)
	{
		XGL::setBlendAlpha();
		glColor4fv(color);
		glLineWidth(width);
		XGL::DisableTexture2D();
		glBegin(GL_LINE_LOOP);
		for (float i = 0; i < PI2; i += PI2 * 0.02f)
		{
			glVertex2fv(p + XMath::getRotateRate(i) * size);
		}
		glEnd();
		glBegin(GL_LINES);
		//下半框
		glVertex2fv(p - size);
		glVertex2f(p.x, p.y - size.y);
		glVertex2f(p.x, p.y - size.y);
		glVertex2fv(p - size * XVec2(0.7071f, 0.2929f));

		glVertex2fv(p + size * XVec2(0.7071f, 0.2929f));
		glVertex2f(p.x, p.y + size.y);
		glVertex2f(p.x, p.y + size.y);
		glVertex2fv(p + size);
		glEnd();
		XGL::DisableBlend();
	}
	void drawDownload(const XVec2& p, const XVec2& size, float width, const XFColor& color)
	{
		XGL::setBlendAlpha();
		glColor4fv(color);
		glLineWidth(width);
		XGL::DisableTexture2D();
		glBegin(GL_LINES);
		//下半框
		glVertex2f(p.x - size.x, p.y);
		glVertex2f(p.x - size.x, p.y + size.y);
		glVertex2f(p.x - size.x, p.y + size.y);
		glVertex2fv(p + size);
		glVertex2fv(p + size);
		glVertex2f(p.x + size.x, p.y);
		//箭头
		glVertex2f(p.x - size.x * 0.5f, p.y);
		glVertex2f(p.x, p.y + size.y * 0.5f);
		glVertex2f(p.x, p.y + size.y * 0.5f);
		glVertex2f(p.x + size.x * 0.5f, p.y);
		glEnd();

		glPointSize(size.x * 0.25f);
		glBegin(GL_POINTS);
		glVertex2fv(p);
		glVertex2f(p.x, p.y - size.y);
		glVertex2f(p.x, p.y - size.y * 0.5f);
		glEnd();
		XGL::DisableBlend();
	}
	void drawUpdate(const XVec2& p, const XVec2& size, float width, const XFColor& color)
	{
		XGL::setBlendAlpha();
		glColor4fv(color);
		glLineWidth(width);
		XGL::DisableTexture2D();
		glBegin(GL_LINES);
		//下半框
		glVertex2f(p.x - size.x, p.y);
		glVertex2f(p.x - size.x, p.y + size.y);
		glVertex2f(p.x - size.x, p.y + size.y);
		glVertex2fv(p + size);
		glVertex2fv(p + size);
		glVertex2f(p.x + size.x, p.y);
		//箭头
		glVertex2fv(p - size * 0.5f);
		glVertex2f(p.x, p.y - size.y);
		glVertex2f(p.x, p.y - size.y);
		glVertex2f(p.x + size.x * 0.5f, p.y - size.y * 0.5f);
		glEnd();

		glPointSize(size.x * 0.25f);
		glBegin(GL_POINTS);
		glVertex2fv(p);
		glVertex2f(p.x, p.y + size.y * 0.5f);
		glVertex2f(p.x, p.y - size.y * 0.5f);
		glEnd();
		XGL::DisableBlend();
	}
	void drawCircle(const XVec2&pos, float sr, int an,const XFColor& color)
	{
		if(an <= 0 || sr <= 0.0f) return;
		XGL::setBlendAlpha();
		glColor4fv(color);
		float r = (PI2)/an;	//为了优化计算
		XGL::DisableTexture2D();
		glBegin(GL_POLYGON);
			for(float i = 0;i < PI2; i += r)
			{
				glVertex2fv(pos + XMath::getRotateRate(i) * sr);
			}
		glEnd();
		XGL::DisableBlend();
	}
	namespace XGLCircleData
	{
	#define CIRCLE_AN (60)
	bool isCircleInit = false;
	std::vector<XVec2> circleRate;
	inline void circleInit()
	{
		if(isCircleInit) return;
		isCircleInit = true;
		float r = PI2 / CIRCLE_AN;
		float tmp = 0.0f;
		circleRate.resize(CIRCLE_AN);
		for(int i = 0;i < CIRCLE_AN;++ i, tmp += r)
		{
			circleRate[i] = XMath::getRotateRate(tmp);
		}
	}
	}
	void drawCircleEx(const XVec2&pos, float sr,const XFColor& color)
	{
		if(sr <= 0.0f) return;
		XGLCircleData::circleInit();
		XGL::setBlendAlpha();
		glColor4fv(color);
		XGL::DisableTexture2D();
		glBegin(GL_POLYGON);
		for(int i = 0;i < CIRCLE_AN;++ i)
		{
			glVertex2fv(pos + XGLCircleData::circleRate[i] * sr);
		}
		glEnd();
		XGL::DisableBlend();
	}
	void drawEllipseLine(const XVec2&pos, float ar, float br, int an,
		const XFColor&c)
	{
		if (an <= 0) return;
		XGL::setBlendAlpha();
		glColor4fv(c);
		float r = (PI2) / an;	//为了优化计算
		XGL::DisableTexture2D();
		glBegin(GL_LINE_LOOP);
		for (float i = 0; i < PI2; i += r)
		{
			glVertex2fv(pos + XMath::getRotateRate(i) * XVec2(ar,br));
		}
		glEnd();
		XGL::DisableBlend();
	}
	void drawEllipseLineEx(const XVec2&pos, float ar, float br, int an,
		float angle, const XFColor&color)
	{
		if (an <= 0) return;
		XGL::setBlendAlpha();
		glColor4fv(color);
		float r = (PI2) / an;	//为了优化计算
		XVec2 offset;
		XGL::DisableTexture2D();
		glBegin(GL_LINE_LOOP);
		for (float i = 0; i < PI2; i += r)
		{
			offset.set(cosf(i) * ar, sinf(i) * br);
			glVertex2fv(pos + offset.rotation(angle));
		}
		glEnd();
		XGL::DisableBlend();
	}
	void drawFillEllipseEx(const XVec2&pos, float ar, float br, int an,
		float angle, const XFColor&color)
	{
		if (an <= 0) return;
		XGL::setBlendAlpha();
		glColor4fv(color);
		float r = (PI2) / an;	//为了优化计算
		XVec2 offset;
		XGL::DisableTexture2D();
		glBegin(GL_TRIANGLE_FAN);
		for (float i = 0; i < PI2; i += r)
		{
			offset.set(cosf(i) * ar, sinf(i) * br);
			glVertex2fv(pos + offset.rotation(angle));
		}
		glEnd();
		XGL::DisableBlend();
	}
	void drawCircleLine(const XVec2& pos, float sr, int an, float w,
		const XFColor& color)
	{
		if (an <= 0 || sr <= 0.0f) return;
		XGL::setBlendAlpha();
		glColor4fv(color);
		glLineWidth(w);
		float r = (PI2) / an;	//为了优化计算
		XGL::DisableTexture2D();
		glBegin(GL_LINE_LOOP);
		for (float i = 0; i < PI2; i += r)
		{
			glVertex2fv(pos + XMath::getRotateRate(i) * sr);
		}
		glEnd();
		XGL::DisableBlend();
	}
	void drawCircleLineEx(const XVec2& pos, float sr,float w,const XFColor& color)
	{
		if(sr <= 0.0f) return;
		XGLCircleData::circleInit();
		XGL::setBlendAlpha();
		glColor4fv(color);
		glLineWidth(w);
		XGL::DisableTexture2D();
		glBegin(GL_LINE_LOOP);
		for(int i = 0;i < CIRCLE_AN; ++ i)
		{
			glVertex2fv(pos + XGLCircleData::circleRate[i] * sr);
		}
		glEnd();
		XGL::DisableBlend();
	}
	void drawHexagonLine(const XVec2& pos, float sr, float w, const XFColor& color)
	{
		if (sr <= 0.0f) return;
		XGL::setBlendAlpha();
		glColor4fv(color);
		glLineWidth(w);
		XGL::DisableTexture2D();
		glBegin(GL_LINE_LOOP);		
		glVertex2f(pos.x - sr * 0.5f, pos.y - sr * 0.86603f);
		glVertex2f(pos.x + sr * 0.5f, pos.y - sr * 0.86603f);
		glVertex2f(pos.x + sr, pos.y);
		glVertex2f(pos.x + sr * 0.5f, pos.y + sr * 0.86603f);
		glVertex2f(pos.x - sr * 0.5f, pos.y + sr * 0.86603f);
		glVertex2f(pos.x - sr, pos.y);
		glEnd();
		XGL::DisableBlend();
	}
	void drawFillHexagon(const XVec2& pos, float sr, const XFColor& color)
	{
		if (sr <= 0.0f) return;
		XGL::setBlendAlpha();
		glColor4fv(color);
		XGL::DisableTexture2D();
		glBegin(GL_POLYGON);
		glVertex2f(pos.x - sr * 0.5f, pos.y - sr * 0.86603f);
		glVertex2f(pos.x + sr * 0.5f, pos.y - sr * 0.86603f);
		glVertex2f(pos.x + sr, pos.y);
		glVertex2f(pos.x + sr * 0.5f, pos.y + sr * 0.86603f);
		glVertex2f(pos.x - sr * 0.5f, pos.y + sr * 0.86603f);
		glVertex2f(pos.x - sr, pos.y);
		glEnd();
		XGL::DisableBlend();
	}
	void drawFillRing(const XVec2& pos, float r0, float r1, float angleS,
		float angleE, int an, const XFColor& c)
	{//测试通过，但是计算需要进一步优化
		if (angleE == angleS || r0 == r1 || an <= 0) return;
		//开始计算需要的点
		float perAngle = (angleE - angleS) / an * DEGREE2RADIAN;
		angleS *= DEGREE2RADIAN;
		XVec2 tmpR;
		XGL::setBlendAlpha();
		glColor4fv(c);
		XGL::DisableTexture2D();
		glBegin(GL_TRIANGLE_STRIP);
		for (int i = 0; i <= an; ++i)
		{
			tmpR = XMath::getRotateRate(angleS);
			glVertex2fv(pos - tmpR * r0);
			glVertex2fv(pos - tmpR * r1);
			angleS += perAngle;
		}
		glEnd();
		XGL::DisableBlend();
	}
	void drawFillFan(const XVec2& pos, float r, float angleS, float angleE, int an, const XFColor& c)
	{
		if (angleE == angleS || r <= 0.0f || an <= 0) return;
		//开始计算需要的点
		float perAngle = (angleE - angleS) / an * DEGREE2RADIAN;
		angleS *= DEGREE2RADIAN;
		XVec2 tmpR;
		XGL::setBlendAlpha();
		glColor4fv(c);
		XGL::DisableTexture2D();
		glBegin(GL_TRIANGLE_STRIP);
		for (int i = 0; i <= an; ++i)
		{
			tmpR = XMath::getRotateRate(angleS);
			glVertex2fv(pos);
			glVertex2fv(pos - tmpR * r);
			angleS += perAngle;
		}
		glEnd();
		XGL::DisableBlend();
	}
	void drawFillRingEx(const XVec2& pos, float r0, float r1, float angleS, 
		float angleE, int an, const XFColor& c)
	{
		if (angleE == angleS || r0 == r1 || an <= 0) return;
		//下面开始计算需要的点
		float perAngle = (angleE - angleS) / an * DEGREE2RADIAN;
		angleS *= DEGREE2RADIAN;
		XGL::setBlendAlpha();
		float perAlpha = c.a / ((an + 1.0f) * 2.0f);
		XVec2 tmpV;
		XFColor tmpC = c;
		tmpC.a = 0.0f;
		XGL::DisableTexture2D();
		glBegin(GL_TRIANGLE_STRIP);
		for (int i = 0; i <= an; ++i)
		{
			tmpV = XMath::getRotateRate(angleS);
			angleS += perAngle;
			tmpC.a += perAlpha;
			glColor4fv(tmpC);
			glVertex2fv(pos - tmpV * r0);
			tmpC.a += perAlpha;
			glColor4fv(tmpC);
			glVertex2fv(pos - tmpV * r1);
		}
		glEnd();
		XGL::DisableBlend();
	}
	//这里是关于圆角的定义
	namespace XGLCornerData
	{
		bool isRoundCornerInit = false;
		#define ROUND_CORNER_DENSITY (3)
		std::vector<XVec2> roundedDatas;	//10度一个弧
		float roundCornerR = 10.0f;	//圆角的半径
		float roundCornerD = 20.0f;	//圆角的直径
		inline void roundCornerInit()
		{
			if (isRoundCornerInit) return;
			roundedDatas.resize(ROUND_CORNER_DENSITY * 4);
			float angle = 0.0f;
			for(int i = 0;i < ROUND_CORNER_DENSITY * 4;++ i)
			{
				roundedDatas[i] = XMath::getRotateRate(angle * DEGREE2RADIAN) * roundCornerR;
				angle += 360.0f / (ROUND_CORNER_DENSITY * 4.0f);
			}
			isRoundCornerInit = true;
		}
		inline void drawCornerLine(const XVec2& pos,const XVec2& size)
		{
			XVec2 tmp = pos + size - XVec2(XGLCornerData::roundCornerR);
			glBegin(GL_LINE_STRIP);
			for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
			{//右下角圆角
				glVertex2fv(XGLCornerData::roundedDatas[i] + tmp);
			}
			tmp = pos + XVec2(XGLCornerData::roundCornerR, size.y - XGLCornerData::roundCornerR);
			for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
			{//左下角圆角
				glVertex2fv(XGLCornerData::roundedDatas[i + ROUND_CORNER_DENSITY] + tmp);
			}
			tmp = pos + XVec2(XGLCornerData::roundCornerR);
			for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
			{//左上角圆角
				glVertex2fv(XGLCornerData::roundedDatas[i + (ROUND_CORNER_DENSITY << 1)] + tmp);
			}
			tmp = pos + XVec2(size.x - XGLCornerData::roundCornerR, XGLCornerData::roundCornerR);
			for(int i = 0;i < ROUND_CORNER_DENSITY;++ i)
			{//右上角圆角
				glVertex2fv(XGLCornerData::roundedDatas[i + ROUND_CORNER_DENSITY * 3] + tmp);
			}
			glVertex2fv(XGLCornerData::roundedDatas[0] + tmp);
			glVertex2fv(XGLCornerData::roundedDatas[0] + pos + size - XVec2(XGLCornerData::roundCornerR));
			glEnd();
		}
	}
	void drawFillQuad(const XVec2& p0,const XFColor& c0,		//第一个点以及颜色
							const XVec2& p1,const XFColor& c1,		//第二个点以及颜色
							const XVec2& p2,const XFColor& c2,		//第三个点以及颜色
							const XVec2& p3,const XFColor& c3)	//第四个点以及颜色
	{
		XGL::setBlendAlpha();
		XGL::DisableTexture2D();
		glBegin(GL_QUADS);
		glColor4fv(c0);
		glVertex2fv(p0);
		glColor4fv(c1);
		glVertex2fv(p1);
		glColor4fv(c2);
		glVertex2fv(p2);
		glColor4fv(c3);
		glVertex2fv(p3);
		glEnd();
		XGL::DisableBlend();
	}
	void drawFillQuad(const XVec2& p0, const XVec2& p1, const XVec2& p2, const XVec2& p3,
		const XFColor& c)
	{
		XGL::setBlendAlpha();
		XGL::DisableTexture2D();
		glColor4fv(c);
		glBegin(GL_QUADS);
		glVertex2fv(p0);
		glVertex2fv(p1);
		glVertex2fv(p2);
		glVertex2fv(p3);
		glEnd();
		XGL::DisableBlend();
	}
	void drawFillRect(const XVec2& pos,const XVec2& size,
					 float r,float g,float b,bool withLine)	//描绘实心矩形
	{
		XGL::DisableTexture2D();
		if(withLine)
		{//描绘边框
			glColor3f(0.15f, 0.15f, 0.15f);
			glLineWidth(1.0f);
			drawRect(pos,size);
		}
		glColor3f(r,g,b);
		drawFillRectBase(pos,size);
	}
	void drawFillRectA(const XVec2& pos,const XVec2& size,
		const XFColor& c,bool withLine)	//描绘实心矩形
	{
		XGL::setBlendAlpha();
		XGL::DisableTexture2D();
		if(withLine)
		{//描绘边框
			glColor4f(0.15f, 0.15f, 0.15f, c.a);
			glLineWidth(1.0f);
			drawRect(pos,size);
		}
		glColor4fv(c);
		drawFillRectBase(pos,size);
		XGL::DisableBlend();
	}
	void drawFillTriangle(const XVec2& p0,const XVec2& p1,const XVec2& p2,
							float r,float g ,float b)
	{
		glColor3f(r,g,b);
		XGL::DisableTexture2D();
		glBegin(GL_TRIANGLES);
		glVertex2fv(p0);
		glVertex2fv(p1);
		glVertex2fv(p2);
		glEnd();
	}
	void drawFillTriangle(const XVec2& p0, const XVec2& p1, const XVec2& p2,
		const XFColor&c)
	{
		XGL::setBlendAlpha();
		XGL::DisableTexture2D();
		glColor4fv(c);
		XGL::DisableTexture2D();
		glBegin(GL_TRIANGLES);
		glVertex2fv(p0);
		glVertex2fv(p1);
		glVertex2fv(p2);
		glEnd();
		XGL::DisableBlend();
	}
	void drawFillTriangleEx(const XVec2& p0,const XVec2& p1,const XVec2& p2,
							float r,float g,float b)	//描绘实心三角形
	{
		float min = p0.y;
		float max = p0.y;
		if(p1.y < min) min = p1.y;else
		if(p1.y > max) max = p1.y;
		if(p2.y < min) min = p2.y;else
		if(p2.y > max) max = p2.y;
		float rate = 1.0f;
		XGL::DisableTexture2D();
		glBegin(GL_TRIANGLES);
		rate = XMath::mapping1D(p0.y,min,max,0.9f,1.1f);
		glColor3f(r * rate,g * rate,b * rate);
		glVertex2fv(p0);
		rate = XMath::mapping1D(p1.y,min,max,0.9f,1.1f);
		glColor3f(r * rate,g * rate,b * rate);
		glVertex2fv(p1);
		rate = XMath::mapping1D(p2.y,min,max,0.9f,1.1f);
		glColor3f(r * rate,g * rate,b * rate);
		glVertex2fv(p2);
		glEnd();
	}
	void drawFillRectEx(const XVec2& pos, const XVec2& size,
		float r, float g, float b, bool withLine, bool withRoundCorner, bool down)	//描绘实心矩形
	{
		XFColor lColor(r * 1.1f, g * 1.1f, b * 1.1f, 1.0f);
		XFColor dColor(r * 0.9f, g * 0.9f, b * 0.9f, 1.0f);
		if (!down)
			std::swap(lColor, dColor);

		XGL::DisableTexture2D();
		if (withRoundCorner && size.x >= XGLCornerData::roundCornerD && size.y >= XGLCornerData::roundCornerD)
		{
			XGLCornerData::roundCornerInit();
			if (withLine)
			{//描绘边框
				glColor3f(0.15f, 0.15f, 0.15f);
				glLineWidth(1.0f);
				XGLCornerData::drawCornerLine(pos, size);
			}
			//下半部分
			glBegin(GL_POLYGON);
			glColor3fv(lColor);
			XVec2 tmp = size + pos - XVec2(XGLCornerData::roundCornerR);
			for (int i = 0; i <= ROUND_CORNER_DENSITY; ++i)
			{//右下角圆角
				glVertex2fv(XGLCornerData::roundedDatas[i] + tmp);
			}
			tmp = pos + XVec2(XGLCornerData::roundCornerR, size.y - XGLCornerData::roundCornerR);
			for (int i = 0; i <= ROUND_CORNER_DENSITY; ++i)
			{//左下角圆角
				glVertex2fv(XGLCornerData::roundedDatas[i + ROUND_CORNER_DENSITY] + tmp);
			}
			glEnd();
			//上半部分
			glBegin(GL_POLYGON);
			glColor3fv(dColor);
			tmp = pos + XVec2(XGLCornerData::roundCornerR);
			for (int i = 0; i <= ROUND_CORNER_DENSITY; ++i)
			{//左上角圆角
				glVertex2fv(XGLCornerData::roundedDatas[i + (ROUND_CORNER_DENSITY << 1)] + tmp);
			}
			tmp = pos + XVec2(size.x - XGLCornerData::roundCornerR, XGLCornerData::roundCornerR);
			for (int i = 0; i < ROUND_CORNER_DENSITY; ++i)
			{//左下角圆角
				glVertex2fv(XGLCornerData::roundedDatas[i + ROUND_CORNER_DENSITY * 3] + tmp);
			}
			glVertex2fv(XGLCornerData::roundedDatas[0] + tmp);
			glEnd();
			//中间
			glBegin(GL_POLYGON);
			glColor3fv(lColor);
			glVertex2fv(XGLCornerData::roundedDatas[0] + size + pos - XVec2(XGLCornerData::roundCornerR));
			glVertex2fv(XGLCornerData::roundedDatas[ROUND_CORNER_DENSITY << 1] + pos + XVec2(XGLCornerData::roundCornerR, size.y - XGLCornerData::roundCornerR));
			glColor3fv(dColor);
			glVertex2fv(XGLCornerData::roundedDatas[ROUND_CORNER_DENSITY << 1] + pos + XVec2(XGLCornerData::roundCornerR));
			glVertex2fv(XGLCornerData::roundedDatas[0] + pos + XVec2(size.x - XGLCornerData::roundCornerR, XGLCornerData::roundCornerR));
			glEnd();
		}
		else
		{
			if (withLine)
			{//描绘边框
				glColor3f(0.15f, 0.15f, 0.15f);
				glLineWidth(1.0f);
				drawRect(pos, size);
			}
			glBegin(GL_QUADS);
			glColor3fv(dColor);
			glVertex2fv(pos);
			glVertex2f(pos.x + size.x, pos.y);
			glColor3fv(lColor);
			glVertex2fv(pos + size);
			glVertex2f(pos.x, pos.y + size.y);
			glEnd();
		}
	}
	void drawFillRectExA(const XVec2& pos,const XVec2& size,
					 float r,float g,float b,float a,
					 bool withLine,bool withRoundCorner,bool down)	//描绘实心矩形
	{
		XFColor lColor(r * 1.1f, g * 1.1f, b * 1.1f, a);
		XFColor dColor(r * 0.9f, g * 0.9f, b * 0.9f, a);
		if (!down)
			std::swap(lColor, dColor);
	
		XGL::DisableTexture2D();
		XGL::setBlendAlpha();
		if(withRoundCorner && size.x >= XGLCornerData::roundCornerD && size.y >= XGLCornerData::roundCornerD)
		{
			XGLCornerData::roundCornerInit();
			if(withLine)
			{//描绘边框
				glColor4f(0.15f,0.15f,0.15f,a);
				glLineWidth(1.0f);
				XGLCornerData::drawCornerLine(pos,size);
			}
			//下半部分
			glBegin(GL_POLYGON);
			glColor4fv(lColor); 
			XVec2 tmp = size + pos - XVec2(XGLCornerData::roundCornerR);
			for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)	
			{//右下角圆角
				glVertex2fv(XGLCornerData::roundedDatas[i] + tmp);
			}
			tmp = pos + XVec2(XGLCornerData::roundCornerR, size.y - XGLCornerData::roundCornerR);
			for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
			{//左下角圆角
				glVertex2fv(XGLCornerData::roundedDatas[i + ROUND_CORNER_DENSITY] + tmp);
			}
			glEnd();
			//上半部分
			glBegin(GL_POLYGON);
			glColor4fv(dColor);
			tmp = pos + XVec2(XGLCornerData::roundCornerR);
			for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
			{//左上角圆角
				glVertex2fv(XGLCornerData::roundedDatas[i + (ROUND_CORNER_DENSITY << 1)] + tmp);
			}
			tmp = pos + XVec2(size.x - XGLCornerData::roundCornerR, XGLCornerData::roundCornerR);
			for(int i = 0;i < ROUND_CORNER_DENSITY;++ i)
			{//左下角圆角
				glVertex2fv(XGLCornerData::roundedDatas[i + ROUND_CORNER_DENSITY * 3] + tmp);
			}
			glVertex2fv(XGLCornerData::roundedDatas[0] + pos + XVec2(size.x - XGLCornerData::roundCornerR,XGLCornerData::roundCornerR));
			glEnd();
			//中间
			glBegin(GL_POLYGON);
			glColor4fv(lColor); 
			glVertex2fv(XGLCornerData::roundedDatas[0] + pos + size - XVec2(XGLCornerData::roundCornerR));
			glVertex2fv(XGLCornerData::roundedDatas[ROUND_CORNER_DENSITY << 1] + pos + XVec2(XGLCornerData::roundCornerR,size.y - XGLCornerData::roundCornerR));
			glColor4fv(dColor);
			glVertex2fv(XGLCornerData::roundedDatas[ROUND_CORNER_DENSITY << 1] + pos + XVec2(XGLCornerData::roundCornerR));
			glVertex2fv(XGLCornerData::roundedDatas[0] + pos + XVec2(size.x - XGLCornerData::roundCornerR,XGLCornerData::roundCornerR));
			glEnd();
		}else
		{
			//调换绘制顺序，立体感会更强。
			glBegin(GL_QUADS);
			glColor4fv(dColor); 
			glVertex2fv(pos);
			glVertex2f(pos.x + size.x,pos.y);
			glColor4fv(lColor);
			glVertex2fv(pos + size);
			glVertex2f(pos.x,pos.y + size.y);
			glEnd();
			if(withLine)
			{
				//这里描绘高光
				glLineWidth(2.0f);
				glColor4f(dColor.r + 0.2f,dColor.g + 0.2f,dColor.b + 0.2f,dColor.a); 
				glBegin(GL_LINES);
				if(down)
				{
					glVertex2f(pos.x + size.x,pos.y + size.y - 1);
					glVertex2f(pos.x,pos.y + size.y - 1);
				}else
				{
					glVertex2fv(pos);
					glVertex2f(pos.x + size.x,pos.y);
				}
				glEnd();
				//描绘边框
				glColor4f(0.15f,0.15f,0.15f,a);
				glLineWidth(1.0f);
				drawRect(pos,size);
			}
		}
		XGL::DisableBlend();
	}
	void drawArrow(const XVec2&pos,float r,float start,float end,const XFColor& color)
	{
		float step = (end - start) / 60.0f * DEGREE2RADIAN;
		float angle = start * DEGREE2RADIAN;
//		XGL::setBlendAlpha();
		glColor4fv(color);
		glLineWidth(1.0f);

		glBegin(GL_LINE_STRIP);
		for(int i = 0;i <= 60;++ i)
		{
			glVertex2fv(pos + XMath::getRotateRate(angle) * r);
			angle += step;
		}
		glEnd();
		if(start < end)
		{//正方向
			//下面描绘箭头
			XVec2 endPos = pos + XMath::getRotateRate(end * DEGREE2RADIAN) * r;
			glBegin(GL_LINE_STRIP);
			glVertex2fv(endPos + XMath::getRotateRate((end - 90.0f - 30.0f) * DEGREE2RADIAN) * 10.0f);
			glVertex2fv(endPos);
			glVertex2fv(endPos + XMath::getRotateRate((end - 90.0f + 30.0f) * DEGREE2RADIAN) * 10.0f);
			glEnd();
		}else
		{//反方向
			XVec2 endPos = pos + XMath::getRotateRate(end * DEGREE2RADIAN) * r;
			glBegin(GL_LINE_STRIP);
			glVertex2fv(endPos + XMath::getRotateRate((end + 90.0f - 30.0f) * DEGREE2RADIAN) * 10.0f);
			glVertex2fv(endPos);
			glVertex2fv(endPos + XMath::getRotateRate((end + 90.0f + 30.0f) * DEGREE2RADIAN) * 10.0f);
			glEnd();
		}
//		XGL::DisableBlend();
	}
	void drawFillRectExAS(const XVec2& pos,const XVec2& size,
					 float r,float g,float b,float a,
					 bool withLine,bool withRoundCorner,bool down)	//描绘实心矩形
	{
		XFColor lColor(r * 1.1f, g * 1.1f, b * 1.1f, a);
		XFColor dColor(r * 0.9f, g * 0.9f, b * 0.9f, a);
		if (!down)
			std::swap(lColor, dColor);

		XGL::DisableTexture2D();
		XGL::setBlendAlpha();
		if(withRoundCorner && size.x >= XGLCornerData::roundCornerD && size.y >= XGLCornerData::roundCornerD)
		{
			XGLCornerData::roundCornerInit();
			if(withLine)
			{//描绘边框
				glColor4f(0.15f,0.15f,0.15f,a);
				glLineWidth(1.0f);
				XGLCornerData::drawCornerLine(pos,size);
			}
			//下半部分
			glBegin(GL_POLYGON);
			glColor4fv(lColor); 
			XVec2 tmp = pos + size - XVec2(XGLCornerData::roundCornerR);
			for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)	
			{//右下角圆角
				glVertex2fv(XGLCornerData::roundedDatas[i] + tmp);
			}
			tmp = pos + XVec2(XGLCornerData::roundCornerR, size.y - XGLCornerData::roundCornerR);
			for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
			{//左下角圆角
				glVertex2fv(XGLCornerData::roundedDatas[i + ROUND_CORNER_DENSITY] + tmp);
			}
			glEnd();
			//上半部分
			glBegin(GL_POLYGON);
			glColor4fv(dColor);
			tmp = pos + XVec2(XGLCornerData::roundCornerR);
			for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
			{//左上角圆角
				glVertex2fv(XGLCornerData::roundedDatas[i + (ROUND_CORNER_DENSITY << 1)] + tmp);
			}
			tmp = pos + XVec2(size.x - XGLCornerData::roundCornerR, XGLCornerData::roundCornerR);
			for(int i = 0;i < ROUND_CORNER_DENSITY;++ i)
			{//左下角圆角
				glVertex2fv(XGLCornerData::roundedDatas[i + ROUND_CORNER_DENSITY * 3] + tmp);
			}
			glVertex2fv(XGLCornerData::roundedDatas[0] + pos + XVec2(size.x - XGLCornerData::roundCornerR,XGLCornerData::roundCornerR));
			glEnd();
			//中间
			glBegin(GL_POLYGON);
			glColor4fv(lColor); 
			glVertex2fv(XGLCornerData::roundedDatas[0] + pos + size - XVec2(XGLCornerData::roundCornerR));
			glVertex2fv(XGLCornerData::roundedDatas[ROUND_CORNER_DENSITY << 1] + pos + XVec2(XGLCornerData::roundCornerR,size.y - XGLCornerData::roundCornerR));
			glColor4fv(dColor);
			glVertex2fv(XGLCornerData::roundedDatas[ROUND_CORNER_DENSITY << 1] + pos + XVec2(XGLCornerData::roundCornerR));
			glVertex2fv(XGLCornerData::roundedDatas[0] + pos + XVec2(size.x - XGLCornerData::roundCornerR,XGLCornerData::roundCornerR));
			glEnd();
		}else
		{
			if(withLine)
			{//描绘边框
				glColor4f(0.15f,0.15f,0.15f,a);
				glLineWidth(1.0f);
				drawRect(pos,size);
			}
			glBegin(GL_QUADS);
			glColor4fv(dColor); 
			glVertex2fv(pos);
			glVertex2f(pos.x + size.x,pos.y);
			glColor4fv(lColor);
			glVertex2fv(pos + size);
			glVertex2f(pos.x,pos.y + size.y);
			glEnd();
		}
		XGL::DisableBlend();
	}
	inline void drawCorner(const XVec2& pos,const XVec2& size,int style,int index)
	{
		XVec2 tmp;
		switch(style)
		{
		case 2:
			glBegin(GL_POLYGON);
			switch(index)
			{
			case 0:
				tmp = pos + size;
				glVertex2fv(tmp);
				for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)	
				{
					glVertex2fv(tmp + XVec2(XGLCornerData::roundCornerR - XGLCornerData::roundedDatas[i].x,
						XGLCornerData::roundedDatas[i].y - XGLCornerData::roundCornerR));
				}
				break;
			case 1:
				tmp.set(pos.x, pos.y + size.y);
				glVertex2fv(tmp);
				for(int i = ROUND_CORNER_DENSITY;i <= (ROUND_CORNER_DENSITY << 1);++ i)	
				{
					glVertex2fv(tmp - XVec2(XGLCornerData::roundedDatas[i].x + XGLCornerData::roundCornerR,
						XGLCornerData::roundCornerR - XGLCornerData::roundedDatas[i].y));
				}
				break;
			case 2:
				glVertex2fv(pos);
				for(int i = (ROUND_CORNER_DENSITY << 1);i <= (ROUND_CORNER_DENSITY * 3);++ i)	
				{
					glVertex2fv(pos - XVec2(XGLCornerData::roundedDatas[i].x + XGLCornerData::roundCornerR,
						-XGLCornerData::roundedDatas[i].y - XGLCornerData::roundCornerR));
				}
				break;
			case 3:
				tmp.set(pos.x + size.x, pos.y);
				glVertex2fv(tmp);
				for(int i = (ROUND_CORNER_DENSITY * 3);i < (ROUND_CORNER_DENSITY << 2);++ i)	
				{
					glVertex2fv(tmp + XVec2(XGLCornerData::roundCornerR - XGLCornerData::roundedDatas[i].x,
						XGLCornerData::roundedDatas[i].y + XGLCornerData::roundCornerR));
				}
				glVertex2fv(tmp + XVec2(XGLCornerData::roundCornerR - XGLCornerData::roundedDatas[0].x,
					XGLCornerData::roundedDatas[0].y + XGLCornerData::roundCornerR));
				break;
			}
			glEnd();
			break;
		case 3:
			glBegin(GL_POLYGON);
			switch(index)
			{
			case 0:
				tmp = pos + size;
				glVertex2fv(tmp);
				for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)	
				{
					glVertex2fv(tmp + XVec2(XGLCornerData::roundedDatas[i].x - XGLCornerData::roundCornerR,
						XGLCornerData::roundCornerR - XGLCornerData::roundedDatas[i].y));
				}
				break;
			case 1:
				tmp.set(pos.x, pos.y + size.y);
				glVertex2fv(tmp);
				for(int i = ROUND_CORNER_DENSITY;i <= (ROUND_CORNER_DENSITY << 1);++ i)	
				{
					glVertex2fv(tmp + XVec2(XGLCornerData::roundedDatas[i].x + XGLCornerData::roundCornerR,
						XGLCornerData::roundCornerR - XGLCornerData::roundedDatas[i].y));
				}
				break;
			case 2:
				glVertex2fv(pos);
				for(int i = (ROUND_CORNER_DENSITY << 1);i <= (ROUND_CORNER_DENSITY * 3);++ i)	
				{
					glVertex2fv(pos + XVec2(XGLCornerData::roundedDatas[i].x + XGLCornerData::roundCornerR,
						 -(XGLCornerData::roundedDatas[i].y + XGLCornerData::roundCornerR)));
				}
				glEnd();
				break;
			case 3:
				tmp.set(pos.x + size.x, pos.y);
				glVertex2fv(tmp);
				for(int i = (ROUND_CORNER_DENSITY * 3);i < (ROUND_CORNER_DENSITY << 2);++ i)	
				{
					glVertex2fv(tmp + XVec2(XGLCornerData::roundedDatas[i].x - XGLCornerData::roundCornerR,
						-(XGLCornerData::roundedDatas[i].y + XGLCornerData::roundCornerR)));
				}
				glVertex2fv(tmp + XVec2(XGLCornerData::roundedDatas[0].x - XGLCornerData::roundCornerR,
					-(XGLCornerData::roundedDatas[0].y + XGLCornerData::roundCornerR)));
				break;
			}
			glEnd();
			break;
		}
	}
	inline void cornerData(const XVec2& pos,const XVec2& size,int style,int index)
	{
		XVec2 tmp;
		switch(style)
		{
		case 1:
			switch(index)
			{
			case 0:
				tmp = size + pos - XVec2(XGLCornerData::roundCornerR);
				for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)	
				{
					glVertex2fv(XGLCornerData::roundedDatas[i] + tmp);
				}
				break;
			case 1:
				tmp = pos + XVec2(XGLCornerData::roundCornerR, size.y - XGLCornerData::roundCornerR);
				for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
				{
					glVertex2fv(XGLCornerData::roundedDatas[i + ROUND_CORNER_DENSITY] + tmp);
				}
				break;
			case 2:
				tmp = pos + XVec2(XGLCornerData::roundCornerR);
				for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
				{//左上角圆角
					glVertex2fv(XGLCornerData::roundedDatas[i + (ROUND_CORNER_DENSITY << 1)] + tmp);
				}
				break;
			case 3:
				tmp = pos + XVec2(size.x - XGLCornerData::roundCornerR, XGLCornerData::roundCornerR);
				for(int i = 0;i < ROUND_CORNER_DENSITY;++ i)
				{//左下角圆角
					glVertex2fv(XGLCornerData::roundedDatas[i + ROUND_CORNER_DENSITY * 3] + tmp);
				}
				glVertex2fv(XGLCornerData::roundedDatas[0] + pos + XVec2(size.x - XGLCornerData::roundCornerR,XGLCornerData::roundCornerR));
				break;
			}
			break;
		default:
			switch(index)
			{
			case 0:
				tmp = pos + size;
				glVertex2fv(tmp + XGLCornerData::roundedDatas[0] - XVec2(XGLCornerData::roundCornerR));
				glVertex2fv(tmp);
				glVertex2fv(tmp + XGLCornerData::roundedDatas[ROUND_CORNER_DENSITY] - XVec2(XGLCornerData::roundCornerR));
				break;
			case 1:
				glVertex2fv(XGLCornerData::roundedDatas[ROUND_CORNER_DENSITY] + pos + XVec2(XGLCornerData::roundCornerR,size.y - XGLCornerData::roundCornerR));
				glVertex2f(pos.x,pos.y + size.y);
				glVertex2f(pos.x + XGLCornerData::roundCornerR + XGLCornerData::roundedDatas[ROUND_CORNER_DENSITY << 1].x,
					pos.y + size.y - XGLCornerData::roundCornerR + XGLCornerData::roundedDatas[ROUND_CORNER_DENSITY << 1].y);
				break;
			case 2:
				glVertex2fv(XGLCornerData::roundedDatas[ROUND_CORNER_DENSITY << 1] + pos + XVec2(XGLCornerData::roundCornerR));
				glVertex2fv(pos);
				glVertex2fv(XGLCornerData::roundedDatas[ROUND_CORNER_DENSITY * 3] + pos + XVec2(XGLCornerData::roundCornerR));
				break;
			case 3:
				glVertex2fv(XGLCornerData::roundedDatas[ROUND_CORNER_DENSITY * 3] + pos + XVec2(size.x - XGLCornerData::roundCornerR,XGLCornerData::roundCornerR));
				glVertex2f(pos.x + size.x,pos.y);
				glVertex2fv(XGLCornerData::roundedDatas[0] + pos + XVec2(size.x - XGLCornerData::roundCornerR,XGLCornerData::roundCornerR));
				break;
			}
			break;
		}
	}
	void drawFillRectExEx(const XVec2& pos, const XVec2& size,
		float r, float g, float b,
		unsigned char lineStyle, unsigned char cornerStyle, bool down)
	{
		XFColor lColor(r * 1.1f, g * 1.1f, b * 1.1f, 1.0f);
		XFColor dColor(r * 0.9f, g * 0.9f, b * 0.9f, 1.0f);
		if (!down)
			std::swap(lColor, dColor);
		XGL::DisableTexture2D();
		if (cornerStyle != 0 && size.x >= XGLCornerData::roundCornerD && size.y >= XGLCornerData::roundCornerD)
		{//需要描绘圆角
			XGLCornerData::roundCornerInit();
			if (lineStyle != 0)
			{//描绘边框
				glColor3f(0.15f, 0.15f, 0.15f);
				glLineWidth(1.0f);
				if (lineStyle & 0x01)
				{//上边需要描绘
					glBegin(GL_LINE_STRIP);
					switch ((cornerStyle & 0x30) >> 4)
					{
					case 0:glVertex2fv(pos); break;
					case 1:
						for (int i = ROUND_CORNER_DENSITY << 1; i <= ROUND_CORNER_DENSITY * 3; ++i)
						{//左上角圆角
							glVertex2fv(pos + XGLCornerData::roundedDatas[i] + XGLCornerData::roundCornerR);
						}
						break;
					case 2:glVertex2f(pos.x - XGLCornerData::roundCornerR, pos.y); break;
					case 3:
						for (int i = ROUND_CORNER_DENSITY << 1; i <= ROUND_CORNER_DENSITY * 3; ++i)
						{//左上角圆角
							glVertex2f(pos.x + XGLCornerData::roundCornerR + XGLCornerData::roundedDatas[i].x,
								pos.y - (XGLCornerData::roundCornerR + XGLCornerData::roundedDatas[i].y));
						}
						break;
					}
					switch ((cornerStyle & 0xc0) >> 6)
					{
					case 0:glVertex2f(pos.x + size.x, pos.y); break;
					case 1:glVertex2f(pos.x + size.x - XGLCornerData::roundCornerR, pos.y); break;
					case 2:glVertex2f(pos.x + size.x + XGLCornerData::roundCornerR, pos.y); break;
					case 3:
						for (int i = ROUND_CORNER_DENSITY * 3; i < ROUND_CORNER_DENSITY << 2; ++i)
						{//左上角圆角
							glVertex2f(pos.x + size.x - XGLCornerData::roundCornerR + XGLCornerData::roundedDatas[i].x,
								pos.y - (XGLCornerData::roundCornerR + XGLCornerData::roundedDatas[i].y));
						}
						glVertex2f(pos.x + size.x - XGLCornerData::roundCornerR + XGLCornerData::roundedDatas[0].x,
							pos.y - (XGLCornerData::roundCornerR + XGLCornerData::roundedDatas[0].y));
						break;
					}
					glEnd();
				}
				if (lineStyle & 0x02)
				{
					glBegin(GL_LINE_STRIP);
					switch ((cornerStyle & 0xc0) >> 6)
					{
					case 0:glVertex2f(pos.x + size.x, pos.y); break;
					case 1:
						for (int i = ROUND_CORNER_DENSITY * 3; i < ROUND_CORNER_DENSITY << 2; ++i)
						{
							glVertex2fv(pos + XGLCornerData::roundedDatas[i] + XVec2(size.x - XGLCornerData::roundCornerR, XGLCornerData::roundCornerR));
						}
						//glVertex2f(pos.x + size.x - XGLCornerData::roundCornerR + XGLCornerData::roundCornerData[0].x,
						//	pos.y + XGLCornerData::roundCornerR + XGLCornerData::roundCornerData[0].y);
						glVertex2fv(pos + XGLCornerData::roundedDatas[0] + XVec2(size.x - XGLCornerData::roundCornerR, XGLCornerData::roundCornerR));
						break;
					case 2:
						for (int i = ROUND_CORNER_DENSITY * 3; i < ROUND_CORNER_DENSITY << 2; ++i)
						{
							glVertex2f(pos.x + size.x - (XGLCornerData::roundedDatas[i].x - XGLCornerData::roundCornerR),
								pos.y + XGLCornerData::roundCornerR + XGLCornerData::roundedDatas[i].y);
						}
						glVertex2f(pos.x + size.x - (XGLCornerData::roundedDatas[0].x - XGLCornerData::roundCornerR),
							pos.y + XGLCornerData::roundCornerR + XGLCornerData::roundedDatas[0].y);
						break;
					case 3:glVertex2f(pos.x + size.x, pos.y - XGLCornerData::roundCornerR); break;
					}
					switch (cornerStyle & 0x03)
					{
					case 0:glVertex2fv(pos + size); break;
					case 1:glVertex2f(pos.x + size.x, pos.y + size.y - XGLCornerData::roundCornerR); break;
					case 2:
						for (int i = 0; i <= ROUND_CORNER_DENSITY; ++i)
						{
							glVertex2f(pos.x + size.x - (XGLCornerData::roundedDatas[i].x - XGLCornerData::roundCornerR),
								pos.y + size.y - XGLCornerData::roundCornerR + XGLCornerData::roundedDatas[i].y);
						}
						break;
					case 3:glVertex2f(pos.x + size.x, pos.y + size.y + XGLCornerData::roundCornerR); break;
					}
					glEnd();
				}
				if (lineStyle & 0x04)
				{
					glBegin(GL_LINE_STRIP);
					switch (cornerStyle & 0x03)
					{
					case 0:glVertex2fv(pos + size); break;
					case 1:
						for (int i = 0; i <= ROUND_CORNER_DENSITY; ++i)
						{
							glVertex2fv(XGLCornerData::roundedDatas[i] + pos + size - XGLCornerData::roundCornerR);
						}
						break;
					case 2:glVertex2f(pos.x + size.x + XGLCornerData::roundCornerR, pos.y + size.y); break;
					case 3:
						for (int i = 0; i <= ROUND_CORNER_DENSITY; ++i)
						{
							glVertex2f(pos.x + size.x + XGLCornerData::roundedDatas[i].x - XGLCornerData::roundCornerR,
								pos.y + size.y - (XGLCornerData::roundedDatas[i].y - XGLCornerData::roundCornerR));
						}
						break;
					}
					switch ((cornerStyle & 0x0c) >> 2)
					{
					case 0:glVertex2f(pos.x, pos.y + size.y); break;
					case 1:glVertex2f(pos.x + XGLCornerData::roundCornerR, pos.y + size.y); break;
					case 2:glVertex2f(pos.x - XGLCornerData::roundCornerR, pos.y + size.y); break;
					case 3:
						for (int i = ROUND_CORNER_DENSITY; i <= ROUND_CORNER_DENSITY << 1; ++i)
						{
							glVertex2f(pos.x + XGLCornerData::roundedDatas[i].x + XGLCornerData::roundCornerR,
								pos.y + size.y - (XGLCornerData::roundedDatas[i].y - XGLCornerData::roundCornerR));
						}
						break;
					}
					glEnd();
				}
				if (lineStyle & 0x08)
				{
					glBegin(GL_LINE_STRIP);
					switch ((cornerStyle & 0x0c) >> 2)
					{
					case 0:glVertex2f(pos.x, pos.y + size.y); break;
					case 1:
						for (int i = ROUND_CORNER_DENSITY; i <= ROUND_CORNER_DENSITY << 1; ++i)
						{
							glVertex2fv(XGLCornerData::roundedDatas[i] + pos + XVec2(XGLCornerData::roundCornerR, size.y - XGLCornerData::roundCornerR));
						}
						break;
					case 2:
						for (int i = ROUND_CORNER_DENSITY; i <= ROUND_CORNER_DENSITY << 1; ++i)
						{
							glVertex2f(pos.x - (XGLCornerData::roundedDatas[i].x + XGLCornerData::roundCornerR),
								pos.y + size.y + XGLCornerData::roundedDatas[i].y - XGLCornerData::roundCornerR);
						}
						break;
					case 3:glVertex2f(pos.x, pos.y + size.y + XGLCornerData::roundCornerR); break;
					}
					switch ((cornerStyle & 0x30) >> 4)
					{
					case 0:glVertex2fv(pos); break;
					case 1:glVertex2f(pos.x, pos.y + XGLCornerData::roundCornerR); break;
					case 2:
						for (int i = ROUND_CORNER_DENSITY << 1; i <= ROUND_CORNER_DENSITY * 3; ++i)
						{
							glVertex2f(pos.x - (XGLCornerData::roundCornerR + XGLCornerData::roundedDatas[i].x),
								pos.y + XGLCornerData::roundCornerR + XGLCornerData::roundedDatas[i].y);
						}
						break;
					case 3:glVertex2f(pos.x, pos.y - XGLCornerData::roundCornerR); break;
					}
					glEnd();
				}
			}
			glColor3fv(lColor);
			//下半部分
			glColor3fv(lColor);
			glBegin(GL_POLYGON);
			cornerData(pos, size, cornerStyle & 0x03, 0);	//右下角
			cornerData(pos, size, (cornerStyle & 0x0c) >> 2, 1);	//左下角
			glEnd();
			drawCorner(pos, size, cornerStyle & 0x03, 0);
			drawCorner(pos, size, (cornerStyle & 0x0c) >> 2, 1);
			//上半部分
			glColor3fv(dColor);
			glBegin(GL_POLYGON);
			cornerData(pos, size, (cornerStyle & 0x30) >> 4, 2);	//右下角
			cornerData(pos, size, (cornerStyle & 0xc0) >> 6, 3);	//左下角
			glEnd();
			drawCorner(pos, size, (cornerStyle & 0x30) >> 4, 2);	//左上角
			drawCorner(pos, size, (cornerStyle & 0xc0) >> 6, 3);	//右上角
			//中间
			glBegin(GL_POLYGON);
			glColor3fv(lColor);
			glVertex2fv(XGLCornerData::roundedDatas[0] + size + pos - XGLCornerData::roundCornerR);
			glVertex2fv(XGLCornerData::roundedDatas[ROUND_CORNER_DENSITY << 1] + pos + XVec2(XGLCornerData::roundCornerR, size.y - XGLCornerData::roundCornerR));
			glColor3fv(dColor);
			glVertex2fv(XGLCornerData::roundedDatas[ROUND_CORNER_DENSITY << 1] + pos + XGLCornerData::roundCornerR);
			glVertex2fv(XGLCornerData::roundedDatas[0] + pos + XVec2(size.x - XGLCornerData::roundCornerR, XGLCornerData::roundCornerR));
			glEnd();
		}
		else
		{//不需要描绘圆角
			if (lineStyle != 0)
			{//描绘边框
				glColor3f(0.15f, 0.15f, 0.15f);
				glLineWidth(1.0f);
				glBegin(GL_LINES);
				if (lineStyle & 0x01)
				{
					glVertex2fv(pos);
					glVertex2f(pos.x + size.x, pos.y);
				}
				if (lineStyle & 0x02)
				{
					glVertex2f(pos.x + size.x, pos.y);
					glVertex2fv(pos + size);
				}
				if (lineStyle & 0x04)
				{
					glVertex2fv(pos + size);
					glVertex2f(pos.x, pos.y + size.y);
				}
				if (lineStyle & 0x08)
				{
					glVertex2f(pos.x, pos.y + size.y);
					glVertex2fv(pos);
				}
				glEnd();
			}
			glBegin(GL_QUADS);
			glColor3fv(dColor);
			glVertex2f(pos.x, pos.y + XGLCornerData::roundCornerR);
			glVertex2fv(pos);
			glVertex2f(pos.x + size.x, pos.y);
			glVertex2f(pos.x + size.x, pos.y + XGLCornerData::roundCornerR);
			glEnd();
			glBegin(GL_QUADS);
			glColor3fv(lColor);
			glVertex2f(pos.x + size.x, pos.y + size.y - XGLCornerData::roundCornerR);
			glVertex2fv(pos + size);
			glVertex2f(pos.x, pos.y + size.y);
			glVertex2f(pos.x, pos.y + size.y - XGLCornerData::roundCornerR);
			glEnd();
			glBegin(GL_QUADS);
			glColor3fv(dColor);
			glVertex2f(pos.x, pos.y + XGLCornerData::roundCornerR);
			glVertex2f(pos.x + size.x, pos.y + XGLCornerData::roundCornerR);
			glColor3fv(lColor);
			glVertex2f(pos.x + size.x, pos.y + size.y - XGLCornerData::roundCornerR);
			glVertex2f(pos.x, pos.y + size.y - XGLCornerData::roundCornerR);
			glEnd();
		}
	}
	void drawFillRectExExA(const XVec2& pos, const XVec2& size,
		float r, float g, float b, float a,
		unsigned char lineStyle, unsigned char cornerStyle, bool down)
	{
		XFColor lColor(r * 1.1f, g * 1.1f, b * 1.1f, a);
		XFColor dColor(r * 0.9f, g * 0.9f, b * 0.9f, a);
		if (!down)
			std::swap(lColor, dColor);
		XGL::DisableTexture2D();
		XGL::setBlendAlpha();

		if (cornerStyle != 0 && size.x >= XGLCornerData::roundCornerD && size.y >= XGLCornerData::roundCornerD)
		{//需要描绘圆角
			XGLCornerData::roundCornerInit();
			if (lineStyle != 0)
			{//描绘边框
				glColor4f(0.15f, 0.15f, 0.15f, a);
				glLineWidth(1.0f);
				if (lineStyle & 0x01)
				{//上边需要描绘
					glBegin(GL_LINE_STRIP);
					switch ((cornerStyle & 0x30) >> 4)
					{
					case 0:glVertex2fv(pos); break;
					case 1:
						for (int i = ROUND_CORNER_DENSITY << 1; i <= ROUND_CORNER_DENSITY * 3; ++i)
						{//左上角圆角
							glVertex2fv(pos + XVec2(XGLCornerData::roundCornerR) + XGLCornerData::roundedDatas[i]);
						}
						break;
					case 2:glVertex2f(pos.x - XGLCornerData::roundCornerR, pos.y); break;
					case 3:
						for (int i = ROUND_CORNER_DENSITY << 1; i <= ROUND_CORNER_DENSITY * 3; ++i)
						{//左上角圆角
							glVertex2f(pos.x + XGLCornerData::roundCornerR + XGLCornerData::roundedDatas[i].x,
								pos.y - (XGLCornerData::roundCornerR + XGLCornerData::roundedDatas[i].y));
						}
						break;
					}
					switch ((cornerStyle & 0xc0) >> 6)
					{
					case 0:glVertex2f(pos.x + size.x, pos.y); break;
					case 1:glVertex2f(pos.x + size.x - XGLCornerData::roundCornerR, pos.y); break;
					case 2:glVertex2f(pos.x + size.x + XGLCornerData::roundCornerR, pos.y); break;
					case 3:
						for (int i = ROUND_CORNER_DENSITY * 3; i < ROUND_CORNER_DENSITY << 2; ++i)
						{//左上角圆角
							glVertex2f(pos.x + size.x - XGLCornerData::roundCornerR + XGLCornerData::roundedDatas[i].x,
								pos.y - (XGLCornerData::roundCornerR + XGLCornerData::roundedDatas[i].y));
						}
						glVertex2f(pos.x + size.x - XGLCornerData::roundCornerR + XGLCornerData::roundedDatas[0].x,
							pos.y - (XGLCornerData::roundCornerR + XGLCornerData::roundedDatas[0].y));
						break;
					}
					glEnd();
				}
				if (lineStyle & 0x02)
				{
					glBegin(GL_LINE_STRIP);
					switch ((cornerStyle & 0xc0) >> 6)
					{
					case 0:glVertex2f(pos.x + size.x, pos.y); break;
					case 1:
						for (int i = ROUND_CORNER_DENSITY * 3; i < ROUND_CORNER_DENSITY << 2; ++i)
						{
							glVertex2fv(XGLCornerData::roundedDatas[i] + pos + XVec2(size.x - XGLCornerData::roundCornerR, XGLCornerData::roundCornerR));
						}
						glVertex2fv(XGLCornerData::roundedDatas[0] + pos + XVec2(size.x - XGLCornerData::roundCornerR, XGLCornerData::roundCornerR));
						break;
					case 2:
						for (int i = ROUND_CORNER_DENSITY * 3; i < ROUND_CORNER_DENSITY << 2; ++i)
						{
							glVertex2f(pos.x + size.x - (XGLCornerData::roundedDatas[i].x - XGLCornerData::roundCornerR),
								pos.y + XGLCornerData::roundCornerR + XGLCornerData::roundedDatas[i].y);
						}
						glVertex2f(pos.x + size.x - (XGLCornerData::roundedDatas[0].x - XGLCornerData::roundCornerR),
							pos.y + XGLCornerData::roundCornerR + XGLCornerData::roundedDatas[0].y);
						break;
					case 3:glVertex2f(pos.x + size.x, pos.y - XGLCornerData::roundCornerR); break;
					}
					switch (cornerStyle & 0x03)
					{
					case 0:glVertex2fv(pos + size); break;
					case 1:glVertex2f(pos.x + size.x, pos.y + size.y - XGLCornerData::roundCornerR); break;
					case 2:
						for (int i = 0; i <= ROUND_CORNER_DENSITY; ++i)
						{
							glVertex2f(pos.x + size.x - (XGLCornerData::roundedDatas[i].x - XGLCornerData::roundCornerR),
								pos.y + size.y - XGLCornerData::roundCornerR + XGLCornerData::roundedDatas[i].y);
						}
						break;
					case 3:glVertex2f(pos.x + size.x, pos.y + size.y + XGLCornerData::roundCornerR); break;
					}
					glEnd();
				}
				if (lineStyle & 0x04)
				{
					glBegin(GL_LINE_STRIP);
					switch (cornerStyle & 0x03)
					{
					case 0:glVertex2fv(pos + size); break;
					case 1:
						for (int i = 0; i <= ROUND_CORNER_DENSITY; ++i)
						{
							glVertex2fv(XGLCornerData::roundedDatas[i] + pos + size - XGLCornerData::roundCornerR);
						}
						break;
					case 2:glVertex2f(pos.x + size.x + XGLCornerData::roundCornerR, pos.y + size.y); break;
					case 3:
						for (int i = 0; i <= ROUND_CORNER_DENSITY; ++i)
						{
							glVertex2f(pos.x + size.x + XGLCornerData::roundedDatas[i].x - XGLCornerData::roundCornerR,
								pos.y + size.y - (XGLCornerData::roundedDatas[i].y - XGLCornerData::roundCornerR));
						}
						break;
					}
					switch ((cornerStyle & 0x0c) >> 2)
					{
					case 0:glVertex2f(pos.x, pos.y + size.y); break;
					case 1:glVertex2f(pos.x + XGLCornerData::roundCornerR, pos.y + size.y); break;
					case 2:glVertex2f(pos.x - XGLCornerData::roundCornerR, pos.y + size.y); break;
					case 3:
						for (int i = ROUND_CORNER_DENSITY; i <= ROUND_CORNER_DENSITY << 1; ++i)
						{
							glVertex2f(pos.x + XGLCornerData::roundedDatas[i].x + XGLCornerData::roundCornerR,
								pos.y + size.y - (XGLCornerData::roundedDatas[i].y - XGLCornerData::roundCornerR));
						}
						break;
					}
					glEnd();
				}
				if (lineStyle & 0x08)
				{
					glBegin(GL_LINE_STRIP);
					switch ((cornerStyle & 0x0c) >> 2)
					{
					case 0:glVertex2f(pos.x, pos.y + size.y); break;
					case 1:
						for (int i = ROUND_CORNER_DENSITY; i <= ROUND_CORNER_DENSITY << 1; ++i)
						{
							glVertex2fv(XGLCornerData::roundedDatas[i] + pos +
								XVec2(XGLCornerData::roundCornerR, size.y - XGLCornerData::roundCornerR));
						}
						break;
					case 2:
						for (int i = ROUND_CORNER_DENSITY; i <= ROUND_CORNER_DENSITY << 1; ++i)
						{
							glVertex2f(pos.x - (XGLCornerData::roundedDatas[i].x + XGLCornerData::roundCornerR),
								pos.y + size.y + XGLCornerData::roundedDatas[i].y - XGLCornerData::roundCornerR);
						}
						break;
					case 3:glVertex2f(pos.x, pos.y + size.y + XGLCornerData::roundCornerR); break;
					}
					switch ((cornerStyle & 0x30) >> 4)
					{
					case 0:glVertex2fv(pos); break;
					case 1:glVertex2f(pos.x, pos.y + XGLCornerData::roundCornerR); break;
					case 2:
						for (int i = ROUND_CORNER_DENSITY << 1; i <= ROUND_CORNER_DENSITY * 3; ++i)
						{
							glVertex2f(pos.x - (XGLCornerData::roundCornerR + XGLCornerData::roundedDatas[i].x),
								pos.y + XGLCornerData::roundCornerR + XGLCornerData::roundedDatas[i].y);
						}
						break;
					case 3:glVertex2f(pos.x, pos.y - XGLCornerData::roundCornerR); break;
					}
					glEnd();
				}
			}
			//下半部分
			glColor4fv(lColor);
			glBegin(GL_POLYGON);
			cornerData(pos, size, cornerStyle & 0x03, 0);	//右下角
			cornerData(pos, size, (cornerStyle & 0x0c) >> 2, 1);	//左下角
			glEnd();
			drawCorner(pos, size, cornerStyle & 0x03, 0);
			drawCorner(pos, size, (cornerStyle & 0x0c) >> 2, 1);
			//上半部分
			glColor4fv(dColor);
			glBegin(GL_POLYGON);
			cornerData(pos, size, (cornerStyle & 0x30) >> 4, 2);	//右下角
			cornerData(pos, size, (cornerStyle & 0xc0) >> 6, 3);	//左下角
			glEnd();
			drawCorner(pos, size, (cornerStyle & 0x30) >> 4, 2);	//左上角
			drawCorner(pos, size, (cornerStyle & 0xc0) >> 6, 3);	//右上角
			//中间
			glBegin(GL_POLYGON);
			glColor4fv(lColor);
			glVertex2fv(XGLCornerData::roundedDatas[0] + size + pos - XVec2(XGLCornerData::roundCornerR));
			glVertex2fv(XGLCornerData::roundedDatas[ROUND_CORNER_DENSITY << 1] + pos + XVec2(XGLCornerData::roundCornerR, size.y - XGLCornerData::roundCornerR));
			glColor4fv(dColor);
			glVertex2fv(XGLCornerData::roundedDatas[ROUND_CORNER_DENSITY << 1] + pos + XVec2(XGLCornerData::roundCornerR));
			glVertex2fv(XGLCornerData::roundedDatas[0] + pos + XVec2(size.x - XGLCornerData::roundCornerR, XGLCornerData::roundCornerR));
			glEnd();
		}
		else
		{//不需要描绘圆角
			if (lineStyle != 0)
			{//描绘边框
				glColor4f(0.15f, 0.15f, 0.15f, a);
				glLineWidth(1.0f);
				glBegin(GL_LINES);
				if (lineStyle & 0x01)
				{
					glVertex2fv(pos);
					glVertex2f(pos.x + size.x, pos.y);
				}
				if (lineStyle & 0x02)
				{
					glVertex2f(pos.x + size.x, pos.y);
					glVertex2fv(pos + size);
				}
				if (lineStyle & 0x04)
				{
					glVertex2fv(pos + size);
					glVertex2f(pos.x, pos.y + size.y);
				}
				if (lineStyle & 0x08)
				{
					glVertex2f(pos.x, pos.y + size.y);
					glVertex2fv(pos);
				}
				glEnd();
			}
			glBegin(GL_QUADS);
			glColor4fv(dColor);
			glVertex2f(pos.x, pos.y + XGLCornerData::roundCornerR);
			glVertex2fv(pos);
			glVertex2f(pos.x + size.x, pos.y);
			glVertex2f(pos.x + size.x, pos.y + XGLCornerData::roundCornerR);
			glEnd();
			glBegin(GL_QUADS);
			glColor4fv(lColor);
			glVertex2f(pos.x + size.x, pos.y + size.y - XGLCornerData::roundCornerR);
			glVertex2fv(pos + size);
			glVertex2f(pos.x, pos.y + size.y);
			glVertex2f(pos.x, pos.y + size.y - XGLCornerData::roundCornerR);
			glEnd();
			glBegin(GL_QUADS);
			glColor4fv(dColor);
			glVertex2f(pos.x, pos.y + XGLCornerData::roundCornerR);
			glVertex2f(pos.x + size.x, pos.y + XGLCornerData::roundCornerR);
			glColor4fv(lColor);
			glVertex2f(pos.x + size.x, pos.y + size.y - XGLCornerData::roundCornerR);
			glVertex2f(pos.x, pos.y + size.y - XGLCornerData::roundCornerR);
			glEnd();
		}
		XGL::DisableBlend();
	}
	void drawFillQuadEx(const XVec2& p0, const XVec2& p1, const XVec2& p2, const XVec2& p3,
		float r, float g, float b, bool withLine)
	{
		XGL::DisableTexture2D();
		if (withLine)
		{//描绘边框
			glColor3f(0.15f, 0.15f, 0.15f);
			glLineWidth(1.0f);
			drawQuadBase(p0, p1, p2, p3);
		}
		glBegin(GL_QUADS);
		glColor3f(r * 1.1f, g * 1.1f, b * 1.1f);
		glVertex2fv(p0);
		glVertex2fv(p1);
		glColor3f(r * 0.9f, g * 0.9f, b * 0.9f);
		glVertex2fv(p2);
		glVertex2fv(p3);
		glEnd();
	}
	void drawFillPolygon(const XVec2 *p,int sum,float r,float g,float b)
	{
		glColor3f(r,g,b);
		XGL::DisableTexture2D();
		glBegin(GL_POLYGON);
		for(int i = 0;i < sum;++ i)
		{
			glVertex2fv(p[i]);
		}
		glEnd();
	}
	void drawFillPolygon(const XVec2 *p,int sum,const XVec2& pos,float angle,float r,float g,float b)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(pos.x,pos.y, 0.0f);
		glRotatef(angle,0.0f,0.0f,1.0f);

		glColor3f(r,g,b);
		XGL::DisableTexture2D();
		glBegin(GL_POLYGON);
		for(int i = 0;i < sum;++ i)
		{
			glVertex2fv(p[i]);
		}
		glEnd();
		glPopMatrix();
	}
	void drawFillPolygonEx(const XVec2 *p,const float *c,int sum,float r,float g,float b,bool withLine)
	{
		XGL::DisableTexture2D();
		if(withLine)
		{//描绘边框
			glColor3f(0.15f,0.15f,0.15f);
			glLineWidth(1.0f);
			glBegin(GL_LINE_STRIP);
			for(int i = 0;i < sum;++ i)
			{
				glVertex2fv(p[i]);
			}
			glEnd();
		}
		glBegin(GL_POLYGON);
		for(int i = 0;i < sum;++ i)
		{
			glColor3f(r * c[i],g * c[i],b * c[i]);
			glVertex2fv(p[i]);
		}
		glEnd();
	}
	void drawFillPolygonExA(const XVec2 *p, const float *c, int sum,
		float r, float g, float b, float a, bool withLine)
	{
		XGL::DisableTexture2D();
		XGL::setBlendAlpha();

		if (withLine)
		{//描绘边框
			glColor4f(0.15f, 0.15f, 0.15f, a);
			glLineWidth(1.0f);
			glBegin(GL_LINE_STRIP);
			for (int i = 0; i < sum; ++i)
			{
				glVertex2fv(p[i]);
			}
			glEnd();
		}
		glBegin(GL_POLYGON);
		for (int i = 0; i < sum; ++i)
		{
			glColor4f(r * c[i], g * c[i], b * c[i], a);
			glVertex2fv(p[i]);
		}
		glEnd();
		XGL::DisableBlend();
	}
	void drawFillRectBW(const XVec2& pos,const XVec2& size,const XVec2& cSize,bool anti)
	{
		XGL::DisableTexture2D();
		if(anti) glColor3fv(XFColor::black);
		else glColor3fv(XFColor::white);
		//先描绘一个白色底
		drawFillRectBase(pos,size);
		int w = size.x / cSize.x;
		int h = size.y / cSize.y;
		XVec2 tempPos;
		if(anti) glColor3fv(XFColor::white);
		else glColor3fv(XFColor::black);
		//方法1
		glBegin(GL_QUADS);
		tempPos.x = pos.x;
		for(int i = 0;i < w;++ i,tempPos.x += cSize.x)
		{
			tempPos.y = pos.y;
			for(int j = 0;j < h;++ j,tempPos.y += cSize.y)
			{
				if((i + j)%2 != 0) continue;
				glVertex2fv(tempPos);
				glVertex2f(tempPos.x + cSize.x,tempPos.y);
				glVertex2fv(tempPos + cSize);
				glVertex2f(tempPos.x,tempPos.y + cSize.y);
			}
		}
		glEnd();
		//方法2
		//int arraySize = w * (h >> 1) * 4;
		//XVec2 *vPoint = XMem::createArrayMem<XVec2>(arraySize);
		//int index = 0;
		//for(int i = 0;i < w;++ i)
		//{
		//	tempPos.x = pos.x + i * cSize.x;
		//	for(int j = 0;j < h;++ j)
		//	{
		//		if((i + j)%2 == 0)
		//		{
		//			tempPos.y = pos.y + j * cSize.y;
		//			vPoint[(index << 2)] = tempPos;
		//			vPoint[(index << 2) + 1].set(tempPos.x + cSize.x,tempPos.y);
		//			vPoint[(index << 2) + 2].set(tempPos.x + cSize.x,tempPos.y + cSize.y);
		//			vPoint[(index << 2) + 3].set(tempPos.x,tempPos.y + cSize.y);
		//			++ index;
		//		}
		//	}
		//}
		////下面开始描绘黑色的方块
		//glEnableClientState(GL_VERTEX_ARRAY);		//表示启用顶点数组
		//glVertexPointer(2, GL_FLOAT, 0, vPoint);	//2表示每个顶点由两个量组成，GL_FLOAT表示每个量都是	
		////glEnableClientState(GL_TEXTURE_COORD_ARRAY);//启用颜色数组
		////glTexCoordPointer(2, GL_FLOAT, 0, m_uPoint);
		//glDrawArrays(GL_QUADS,0,arraySize);
		//XGL::DisableTexture2D();
		//XMem::XDELETE_ARRAY(vPoint);
	}
	void drawFillRectBWEx(const XVec2& pos,const XVec2& cSize,const XVec2& sum,bool anti)
	{
		XGL::DisableTexture2D();
		if(anti) glColor3fv(XFColor::black);
		else glColor3fv(XFColor::white);
		//先描绘一个白色底
		drawFillRectBase(pos,sum * cSize);
		XVec2 tempPos;
		if(anti) glColor3fv(XFColor::white);
		else glColor3fv(XFColor::black);
		//方法1
		glBegin(GL_QUADS);
		tempPos.x = pos.x;
		for(int i = 0;i < sum.x;++ i,tempPos.x += cSize.x)
		{
			tempPos.y = pos.y;
			for(int j = 0;j < sum.y;++ j,tempPos.y += cSize.y)
			{
				if((i + j)%2 != 0) continue;
				glVertex2fv(tempPos);
				glVertex2f(tempPos.x + cSize.x,tempPos.y);
				glVertex2fv(tempPos + cSize);
				glVertex2f(tempPos.x,tempPos.y + cSize.y);
			}
		}
		glEnd();
	}
	void drawTexture(XTexture &tex, const XVec2& pos, int cW, int cH,
		const XVec2 *vArray, const XVec2 *uArray, int arrayW, int arrayH)
	{
		if (vArray == NULL || uArray == NULL || arrayW <= 0 || arrayH <= 0) return;
		int tW = tex.m_w;
		int tH = tex.m_h;
		XVec2 u((tW - cW) >> 1, (tH - cH) >> 1);
		XVec2 v(-(cW >> 1), -(cH >> 1));

		XGL::EnableTexture2D();
		XGL::DisableBlend();
#if WITHXSPRITE_EX
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#endif
		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0.0, (tW << 1), 0.0, (tH << 1), -1.0, 1.0);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(pos.x + (cW >> 1), pos.y + (cH >> 1), 0.0f);

		XGL::BindTexture2D(tex.m_texture);
		//glActiveTexture(GL_TEXTURE0);
		glColor4fv(XFColor::white);
		int offsetTemp = 0;
		for (int j = 0; j < arrayH - 1; ++j)
		{
			offsetTemp = j * arrayW;
			glBegin(GL_TRIANGLE_STRIP);
			for (int i = 0; i < arrayW; ++i, ++offsetTemp)
			{	//左上
				glTexCoord2fv(u + uArray[offsetTemp]);	//u
				glVertex2fv(v + vArray[offsetTemp]);	//v

				glTexCoord2fv(u + uArray[offsetTemp + arrayW]);	//u
				glVertex2fv(v + vArray[offsetTemp + arrayW]);	//v
			}
			glEnd();
		}

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glMatrixMode(GL_TEXTURE);
		glPopMatrix();
#if WITHXSPRITE_EX
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#endif
		XGL::DisableBlend();
	}
	void drawTexture(unsigned int tex, const XVec2* v, const XVec2* u, int pSum, bool blend, const XFColor& c)
	{
		if (v == nullptr || u == nullptr || pSum <= 0) return;
		XGL::EnableTexture2D();
		if (blend) XGL::setBlendAlpha();
		else XGL::DisableBlend();

		XGL::BindTexture2D(tex);
		glColor4fv(c);

		glBegin(GL_TRIANGLE_STRIP);
		for (int i = 0, index = 0; i < pSum >> 1; ++i, index += 2)
		{	//左上
			glTexCoord2fv(u[index]);	//u
			glVertex2fv(v[index]);	//v

			glTexCoord2fv(u[index + 1]);	//u
			glVertex2fv(v[index + 1]);	//v
		}
		glEnd();

		XGL::DisableBlend();
	}
	void drawTextureEx(unsigned int tex, const XVec2* v, const XVec2* u, int pSum, int subSum, bool blend)
	{//这个函数还可以深入的优化，但是为了逻辑的理解，目前只进行到这里
		if (v == nullptr || u == nullptr || pSum <= 0) return;
		if (subSum < 1 || subSum == 1) return drawTexture(tex, v, u, pSum, blend);
		XGL::EnableTexture2D();
		if (blend) XGL::setBlendAlpha();
		else XGL::DisableBlend();
		XGL::BindTexture2D(tex);
		glColor4fv(XFColor::white);

		float tmpW, tmpH = 0.0f;
		float step = 1.0f / static_cast<float>(subSum);
		XVec2 uul, uur, udl, udr, vul, vur, vdl, vdr;
		for (int h = 0; h < subSum; ++h, tmpH += step)
		{
			glBegin(GL_TRIANGLE_STRIP);
			for (int i = 0, index = 0; i < (pSum >> 1); ++i, index += 2)
			{	//左上
				if (i == (pSum >> 1) - 1)
				{//最后一列
					glTexCoord2fv(u[index] + (u[index + 1] - u[index]) * tmpH);		//u
					glVertex2fv(v[index] + (v[index + 1] - v[index]) * tmpH);		//v

					glTexCoord2fv(u[index] + (u[index + 1] - u[index]) * (tmpH + step));		//u
					glVertex2fv(v[index] + (v[index + 1] - v[index]) * (tmpH + step));		//v
				}
				else
				{
					uul = u[index] + (u[index + 1] - u[index]) * tmpH;
					udl = u[index] + (u[index + 1] - u[index]) * (tmpH + step);
					uur = u[index + 2] + (u[index + 3] - u[index + 2]) * tmpH;
					udr = u[index + 2] + (u[index + 3] - u[index + 2]) * (tmpH + step);

					vul = v[index] + (v[index + 1] - v[index]) * tmpH;
					vdl = v[index] + (v[index + 1] - v[index]) * (tmpH + step);
					vur = v[index + 2] + (v[index + 3] - v[index + 2]) * tmpH;
					vdr = v[index + 2] + (v[index + 3] - v[index + 2]) * (tmpH + step);
					tmpW = 0.0f;
					for (int w = 0; w < subSum; ++w, tmpW += step)
					{
						glTexCoord2fv(uul + (uur - uul) * tmpW);	//u
						glVertex2fv(vul + (vur - vul) * tmpW);		//v

						glTexCoord2fv(udl + (udr - udl) * tmpW);		//u
						glVertex2fv(vdl + (vdr - vdl) * tmpW);		//v
					}
				}
			}
			glEnd();
		}

		XGL::DisableBlend();
	}
	void drawTextureEx(unsigned int tex, const XVec2& pos, const XVec2& size,
		XVec2 *vArray, XVec2 *uArray, int arrayW, int arrayH, bool blend)
	{
		if (vArray == NULL || uArray == NULL || arrayW <= 0 || arrayH <= 0) return;
		XGL::EnableTexture2D();
		if (blend) XGL::setBlendAlpha();
		else XGL::DisableBlend();
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(pos.x, pos.y, 0.0f);
		glScalef(size.x, size.y, 1.0f);

		XGL::BindTexture2D(tex);
		glColor4fv(XFColor::white);
		int offsetTemp = 0;
		for (int j = 0; j < arrayH - 1; ++j)
		{
			offsetTemp = j * arrayW;
			glBegin(GL_TRIANGLE_STRIP);
			for (int i = 0; i < arrayW; ++i, ++offsetTemp)
			{	//左上
				glTexCoord2fv(uArray[offsetTemp]);	//u
				glVertex2fv(vArray[offsetTemp]);	//v

				glTexCoord2fv(uArray[offsetTemp + arrayW]);	//u
				glVertex2fv(vArray[offsetTemp + arrayW]);	//v
			}
			glEnd();
		}

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		XGL::DisableBlend();
	}
	void drawTexture(unsigned int tex, const XVec2& pos, int texW, int texH, bool blend)
	{
		XGL::EnableTexture2D();
		if (blend) XGL::setBlendAlpha();
		else XGL::DisableBlend();
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(pos.x, pos.y, 0.0f);
		//glScalef(0.5f,0.5f,1.0f);

		XGL::BindTexture2D(tex);
		glColor4fv(XFColor::white);

		glBegin(GL_QUADS);
		glTexCoord2fv(XVec2::zero);	//u
		glVertex2fv(XVec2::zero);	//v
		glTexCoord2fv(XVec2::oneZero);	//u
		glVertex2f(texW, 0);	//v
		glTexCoord2fv(XVec2::one);	//u
		glVertex2f(texW, texH);	//v
		glTexCoord2fv(XVec2::zeroOne);	//u
		glVertex2f(0, texH);	//v
		glEnd();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		XGL::DisableBlend();
	}
}
namespace XGL
{
	int getGLFormat(XColorMode mode)
	{
		switch (mode)
		{
		case COLOR_RGBA:
		case COLOR_RGBA8:
		case COLOR_RGBA16:
		case COLOR_RGBA16F:
		case COLOR_RGBA32F_ARB:
			return GL_RGBA;

		case COLOR_RGB:
		case COLOR_RGB8:
		case COLOR_RGB16:
		case COLOR_RGB16F:
		case COLOR_RGB32F_ARB:
			return GL_RGB;

		case COLOR_GRAY:
		case COLOR_GRAY8:
		case COLOR_GRAY16:
		case COLOR_GRAY32F_ARB:
			return GL_LUMINANCE;

		case COLOR_GRAY_ALPHA:
		case COLOR_GRAY8_ALPHA8:
		case COLOR_GRAY16_ALPHA16:
		case COLOR_GRAY_ALPHA32F_ARB:
			return GL_LUMINANCE_ALPHA;

		case COLOR_DEPTH:
		case COLOR_DEPTH16:
		case COLOR_DEPTH24:
		case COLOR_DEPTH32:
			return GL_DEPTH_COMPONENT;
		case COLOR_DEPTH_STENCIL:
			return GL_DEPTH_STENCIL;

		case COLOR_R8:
		case COLOR_R16:
		case COLOR_R16F:
		case COLOR_R32F:
			return GL_RED;

		case COLOR_ALPHA:
		case COLOR_ALPHA8:
			return GL_ALPHA;

		case COLOR_RG8:
		case COLOR_RG16:
		case COLOR_RG16F:
		case COLOR_RG32F:
			return GL_RG;

		case COLOR_STENCIL_INDEX:
			return GL_STENCIL_INDEX;

		case COLOR_RED:		return GL_RED;
		case COLOR_RGBA_F:	return GL_RGBA;
		case COLOR_RGB_F:	return GL_RGB;
		case COLOR_GRAY_F:	return GL_RED;
		case COLOR_BGRA:	return GL_BGRA;
		case COLOR_BGR:		return GL_BGR;
		}
		return GL_RGBA;
	}
	int getPixelsFormat(XColorMode mode)
	{
		switch (mode)
		{
		case COLOR_RGBA:
		case COLOR_RGB:
		case COLOR_GRAY:
		case COLOR_GRAY_ALPHA:
		case COLOR_ALPHA:

		case COLOR_GRAY8:
		case COLOR_GRAY8_ALPHA8:
		case COLOR_R8:
		case COLOR_RG8:
		case COLOR_RGB8:
		case COLOR_RGBA8:
		case COLOR_ALPHA8:
			return GL_UNSIGNED_BYTE;

		case COLOR_GRAY16:
		case COLOR_GRAY16_ALPHA16:
		case COLOR_R16:
		case COLOR_RG16:
		case COLOR_RGB16:
		case COLOR_RGBA16:
			return GL_UNSIGNED_SHORT;

		case COLOR_GRAY32F_ARB:
		case COLOR_GRAY_ALPHA32F_ARB:
		case COLOR_R16F:
		case COLOR_R32F:
		case COLOR_RG16F:
		case COLOR_RG32F:
		case COLOR_RGB16F:
		case COLOR_RGB32F_ARB:
		case COLOR_RGBA16F:
		case COLOR_RGBA32F_ARB:
			return GL_FLOAT;

		case COLOR_DEPTH_STENCIL:
			return GL_UNSIGNED_INT_24_8;

		case COLOR_DEPTH:
		case COLOR_DEPTH16:
			return GL_UNSIGNED_SHORT;

		case COLOR_DEPTH24:
		case COLOR_DEPTH32:
			return GL_UNSIGNED_INT;

		case COLOR_STENCIL_INDEX:
			return GL_UNSIGNED_BYTE;

		case COLOR_RGBA_F:
		case COLOR_RGB_F:
		case COLOR_GRAY_F:
			return GL_FLOAT;
		case COLOR_RED:
		case COLOR_BGRA:
		case COLOR_BGR:
			return GL_UNSIGNED_BYTE;
		}
		return GL_UNSIGNED_BYTE;
	}
	int getGLInternalFormat(XColorMode mode)
	{
		switch (mode)
		{
		case COLOR_RGBA:		return GL_RGBA;
		case COLOR_RGBA8:		return GL_RGBA8;
		case COLOR_RGBA16:		return GL_RGBA16;
		case COLOR_RGBA16F:		return GL_RGBA16F;
		case COLOR_RGBA32F_ARB:	return GL_RGBA32F_ARB;

		case COLOR_RGB:			return GL_RGB;
		case COLOR_RGB8:		return GL_RGB8;
		case COLOR_RGB16:		return GL_RGB16;
		case COLOR_RGB16F:		return GL_RGB16F;
		case COLOR_RGB32F_ARB:	return GL_RGB32F_ARB;

		case COLOR_GRAY:		return GL_LUMINANCE;
		case COLOR_GRAY8:		return GL_LUMINANCE8;
		case COLOR_GRAY16:		return GL_LUMINANCE16;
		case COLOR_GRAY32F_ARB:	return GL_LUMINANCE32F_ARB;

		case COLOR_GRAY_ALPHA:			return GL_LUMINANCE_ALPHA;
		case COLOR_GRAY8_ALPHA8:		return GL_LUMINANCE8_ALPHA8;
		case COLOR_GRAY16_ALPHA16:		return GL_LUMINANCE16_ALPHA16;
		case COLOR_GRAY_ALPHA32F_ARB:	return GL_LUMINANCE_ALPHA32F_ARB;

		case COLOR_DEPTH:		return GL_DEPTH_COMPONENT;
		case COLOR_DEPTH16:		return GL_DEPTH_COMPONENT16;
		case COLOR_DEPTH24:		return GL_DEPTH_COMPONENT24;
		case COLOR_DEPTH32:		return GL_DEPTH_COMPONENT32;

		case COLOR_DEPTH_STENCIL:	return GL_DEPTH_STENCIL;

		case COLOR_R8:		return GL_R8;
		case COLOR_R16:		return GL_R16;
		case COLOR_R16F:	return GL_R16F;
		case COLOR_R32F:	return GL_R32F;

		case COLOR_ALPHA:	return GL_ALPHA;
		case COLOR_ALPHA8:	return GL_ALPHA8;

		case COLOR_RG8:		return GL_RG;
		case COLOR_RG16:	return GL_RG16;
		case COLOR_RG16F:	return GL_RG16F;
		case COLOR_RG32F:	return GL_RG32F;

		case COLOR_STENCIL_INDEX:	return GL_STENCIL_INDEX;

		case COLOR_RED:		return GL_RED;
		case COLOR_RGBA_F:	return GL_RGBA32F;
		case COLOR_RGB_F:	return GL_RGB32F;
		case COLOR_GRAY_F:	return GL_R32F;
		case COLOR_BGRA:	return GL_BGRA;
		case COLOR_BGR:		return GL_BGR;
		}
		return GL_RGBA;
	}
	GLuint emptyTexture(int x, int y, XColorMode mode, int &internalFormat, int filter, int MSSum)
	{
		GLuint textureID;
		glGenTextures(1, &textureID);
		if (MSSum == 0)
			XGL::BindTexture2D(textureID);
		else
			XGL::BindTexture2DMS(textureID);
		//效果好
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
#if WITHXSPRITE_EX
		if (mode == COLOR_DEPTH)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
#endif
		//速度快
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		if (MSSum > 0)
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, MSSum, GL_RGBA8, x, y, false);
		else
		{
			if (internalFormat < 0)
			{
				internalFormat = getGLInternalFormat(mode);
				int glFormat = getGLFormat(mode);
				int pixelsFormat = getPixelsFormat(mode);
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, x, y, 0, glFormat, pixelsFormat, 0);
			}
			else
			{
				switch (mode)
				{
				case COLOR_RGBA:	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);		break;
				case COLOR_RGB:		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);		break;
				case COLOR_GRAY:	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, x, y, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, 0);	break;	//修改
				case COLOR_RGBA_F:	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, x, y, 0, GL_RGBA, GL_FLOAT, 0);				break;
				case COLOR_RGB_F:	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, x, y, 0, GL_RGBA, GL_FLOAT, 0);				break;
				case COLOR_GRAY_F:	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, x, y, 0, GL_RED, GL_FLOAT, 0);				break;	//修改
				case COLOR_BGRA:	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, x, y, 0, GL_BGRA, GL_UNSIGNED_BYTE, 0);		break;
				case COLOR_BGR:		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, x, y, 0, GL_BGR, GL_UNSIGNED_BYTE, 0);		break;
				case COLOR_DEPTH:	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, x, y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0); break;
				case COLOR_RED:		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, x, y, 0, GL_RED, GL_UNSIGNED_BYTE, 0);		break;	//修改
				}
			}
		}
#if WITH_ENGINE_STATISTICS
		if (gFrameworkData.pFrameWork != NULL)
			gFrameworkData.pFrameWork->addStaticsticsTexInfo(textureID);
#endif
		//glClear(GL_COLOR_BUFFER_BIT);
		//初始化像素
		//glClearTexImage();	//这个貌似要GL3.0才支持，目前的版本好像不支持
		if (MSSum == 0)
			XGL::BindTexture2D(0);
		else
			XGL::BindTexture2DMS(0);
		return textureID;
	}
	void readFrame(void* backbuffer, int width, int height)
	{
		glReadBuffer(GL_BACK);
		glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, backbuffer);
	}
	void glAssert(const char* msg, long line, const char* file)
	{
		struct glError
		{
			GLenum code;
			const char* name;
		};

		static const glError errors[] = { { GL_NO_ERROR, "No Error" },
		{ GL_INVALID_ENUM, "Invalid Enum" },
		{ GL_INVALID_VALUE, "Invalid Value" },
		{ GL_INVALID_OPERATION, "Invalid Operation" }
#if OGL1
			,{ GL_STACK_OVERFLOW, "Stack Overflow" },
			{ GL_STACK_UNDERFLOW, "Stack Underflow" },
			{ GL_OUT_OF_MEMORY, "Out Of Memory" }
#endif
		};

		GLenum e = glGetError();

		if (e == GL_NO_ERROR)
		{
			return;
		}
		else
		{
			const char* errorName = "Unknown error";

			// find error message
			for (unsigned int i = 0; i < sizeof(errors) / sizeof(glError); i++)
			{
				if (errors[i].code == e)
				{
					errorName = errors[i].name;
				}
			}

			printf("OpenGL: %s - error %s in %s at line %d\n", msg, errorName, file, int(line));
			assert(0);
		}
	}
}
void XTexture::updateTexture(const XPBO &pbo)		//使用指定数据更新贴图
{
	if (!m_isLoaded) return;
	pbo.bind();
	updateTextureX(NULL);
	//pbo.unbind();
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
}
void XTexture::updateTextureR2B(const XPBO &pbo)		//使用指定数据更新贴图
{
	if (!m_isLoaded) return;
	pbo.bind();
	updateTextureR2BX(NULL);
	//pbo.unbind();
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
}
void XTexture::updateTextureEx(const void *data)		//初步测试这个性能会比下面的性能好
{
	if (!m_isLoaded || data == NULL) return;	//PBO的时候可以为 NULL
	if (m_isThreadEvn)
	{
		XEG.hdcLock();
		wglMakeCurrent(XEG.getHDC(), XEG.getCopyHGLRC());
	}
	assert(glIsTexture(m_texture));
	//if(!glIsTexture(m_texture)) return;
	XGL::EnableTexture2D();
	XGL::BindTexture2D(m_texture);
	switch (m_mode)
	{
	case COLOR_RGBA:	glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_w, m_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);			break;
	case COLOR_RGB:		glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_w, m_h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);			break;
	case COLOR_GRAY:	glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_w, m_h, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);	break;
	case COLOR_RGBA_F:	glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_w, m_h, 0, GL_RGBA, GL_FLOAT, data);					break;
	case COLOR_RGB_F:	glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_w, m_h, 0, GL_RGBA, GL_FLOAT, data);			break;
	case COLOR_GRAY_F:	glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_w, m_h, 0, GL_RED, GL_FLOAT, data);			break;
	case COLOR_BGRA:	glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_w, m_h, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);	break;
	case COLOR_BGR:		glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_w, m_h, 0, GL_BGR, GL_UNSIGNED_BYTE, data);	break;
	case COLOR_RED:		glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_w, m_h, 0, GL_RED, GL_UNSIGNED_BYTE, data);	break;
	default:	//其他格式不支持
		break;
	}
	if (m_isThreadEvn)
	{
		wglMakeCurrent(NULL, NULL);//nullptr
		XEG.hdcUnlock();
	}
}
void XTexture::updateTextureR2BEx(const void *data)	//初步测试这个性能会比下面的性能好
{
	if (!m_isLoaded || data == NULL) return;	//PBO的时候可以为 NULL
	if (m_isThreadEvn)
	{
		XEG.hdcLock();
		wglMakeCurrent(XEG.getHDC(), XEG.getCopyHGLRC());
	}
	assert(glIsTexture(m_texture));
	//if(!glIsTexture(m_texture)) return;
	XGL::EnableTexture2D();
	XGL::BindTexture2D(m_texture);
	switch (m_mode)
	{
	case COLOR_RGBA:	glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_w, m_h, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);	break;
	case COLOR_RGB:		glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_w, m_h, 0, GL_BGR, GL_UNSIGNED_BYTE, data);	break;
	case COLOR_GRAY:	glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_w, m_h, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);	break;
	case COLOR_RGBA_F:	glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_w, m_h, 0, GL_BGRA, GL_FLOAT, data);		break;
	case COLOR_RGB_F:	glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_w, m_h, 0, GL_BGR, GL_FLOAT, data);		break;
	case COLOR_GRAY_F:	glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_w, m_h, 0, GL_RED, GL_FLOAT, data);			break;
	case COLOR_BGRA:	glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_w, m_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);	break;
	case COLOR_BGR:		glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_w, m_h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);	break;
	case COLOR_RED:		glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_w, m_h, 0, GL_RED, GL_UNSIGNED_BYTE, data);	break;
	default:	//其他格式不支持
		break;
	}
	if (m_isThreadEvn)
	{
		wglMakeCurrent(NULL, NULL);//nullptr
		XEG.hdcUnlock();
	}
}
void XTexture::updateTextureX(const void *data, int w, int h, int x, int y)
{
	if (m_isThreadEvn)
	{
		XEG.hdcLock();
		wglMakeCurrent(XEG.getHDC(), XEG.getCopyHGLRC());
	}
	assert(glIsTexture(m_texture));
	//if(!glIsTexture(m_texture)) return;
	if (w <= 0) w = m_w;
	if (h <= 0) h = m_h;
	XGL::EnableTexture2D();
	XGL::BindTexture2D(m_texture);
	switch (m_mode)
	{
	case COLOR_RGBA:	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);			break;
	case COLOR_RGB:		glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RGB, GL_UNSIGNED_BYTE, data);			break;
	case COLOR_GRAY:	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);	break;
	case COLOR_RGBA_F:	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RGBA, GL_FLOAT, data);					break;
	case COLOR_RGB_F:	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RGB, GL_FLOAT, data);					break;
	case COLOR_GRAY_F:	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RED, GL_FLOAT, data);					break;
	case COLOR_BGRA:	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_BGRA, GL_UNSIGNED_BYTE, data);			break;
	case COLOR_BGR:		glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_BGR, GL_UNSIGNED_BYTE, data);			break;
	case COLOR_RED:		glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RED, GL_UNSIGNED_BYTE, data);			break;
	default:	//其他格式不支持
		break;
	}
	if (m_isThreadEvn)
	{
		wglMakeCurrent(NULL, NULL);//nullptr
		XEG.hdcUnlock();
	}
}
void XTexture::updateTextureR2BX(const void *data, int w, int h, int x, int y)
{
	if (m_isThreadEvn)
	{
		XEG.hdcLock();
		wglMakeCurrent(XEG.getHDC(), XEG.getCopyHGLRC());
	}
	assert(glIsTexture(m_texture));
	//if(!glIsTexture(m_texture)) return;
	if (w <= 0) w = m_w;
	if (h <= 0) h = m_h;
	XGL::EnableTexture2D();
	XGL::BindTexture2D(m_texture);
	switch (m_mode)
	{
	case COLOR_RGBA:	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_BGRA, GL_UNSIGNED_BYTE, data);		break;
	case COLOR_RGB:		glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_BGR, GL_UNSIGNED_BYTE, data);		break;
	case COLOR_GRAY:	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_LUMINANCE, GL_UNSIGNED_BYTE, data); break;
	case COLOR_RGBA_F:	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RGBA, GL_FLOAT, data);				break;
	case COLOR_RGB_F:	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RGB, GL_FLOAT, data);				break;
	case COLOR_GRAY_F:	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RED, GL_FLOAT, data);				break;
	case COLOR_BGRA:	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);		break;
	case COLOR_BGR:		glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RGB, GL_UNSIGNED_BYTE, data);		break;
	case COLOR_RED:		glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RED, GL_UNSIGNED_BYTE, data);		break;
	default:	//其他格式不支持
		break;
	}
	if (m_isThreadEvn)
	{
		wglMakeCurrent(NULL, NULL);//nullptr
		XEG.hdcUnlock();
	}
}
XBool XTexture::createTexture(int w, int h, XColorMode mode, int internalFormat, int filter, int MSSum)
{
	if (m_isLoaded) return XTrue;	//如果资源已经载入了，则不能重复载入

	if ((w * getBytePerPixelByColorMode(mode)) % 4 != 0)
	{
		LogStr("图像没有四字节对齐,可能会发生异常!");
		return XFalse;
	}

	if (w <= 0 ||
		h <= 0) return XFalse;
	try
	{
#if TEX_DEBUG1
		++ CPSum;
		printf("%d-\n", CPSum);
#endif
		m_cp = new XSCounter;
		if (m_cp == NULL) return XFalse;
	}
	catch (...)
	{
		return XFalse;
	}
	m_mode = mode;

	m_w = w;
	m_h = h;
	m_MSSum = MSSum;
	m_internalFormat = internalFormat;
	if (m_isThreadEvn)
	{
		XEG.hdcLock();
		wglMakeCurrent(XEG.getHDC(), XEG.getCopyHGLRC());
		m_texture = XGL::emptyTexture(m_w, m_h, m_mode, m_internalFormat, filter, m_MSSum);
		wglMakeCurrent(NULL, NULL);//nullptr
		XEG.hdcUnlock();
	}
	else
	{
		m_texture = XGL::emptyTexture(m_w, m_h, m_mode, m_internalFormat, filter, m_MSSum);
	}
	//for test
	//	glGetTexParameterIiv(m_texture,GL_TEXTURE_INTERNAL_FORMAT,&internalFormat);
	//	if(internalFormat != m_internalFormat)
	//		printf("error\n");
	m_isLoaded = XTrue;
	return XTrue;
}
XBool XTexture::createWithTexture(int w, int h, XColorMode mode, unsigned int tex)
{
	if (m_isLoaded) return XTrue;	//如果资源已经载入了，则不能重复载入

	if ((w * getBytePerPixelByColorMode(mode)) % 4 != 0)
	{
		LogStr("图像没有四字节对齐,可能会发生异常!");
		return XFalse;
	}
	if (w <= 0 ||
		h <= 0) return XFalse;
	try
	{
#if TEX_DEBUG1
		++ CPSum;
		printf("%d-\n", CPSum);
#endif
		m_cp = new XSCounter;
		if (m_cp == NULL) return XFalse;
	}
	catch (...)
	{
		return XFalse;
	}
	m_mode = mode;

	m_w = w;
	m_h = h;
	//m_internalFormat;	//这个地方没有初始化
	m_texture = tex;
	if (m_isThreadEvn)
	{
		XEG.hdcLock();
		wglMakeCurrent(XEG.getHDC(), XEG.getCopyHGLRC());
		glGetTexParameterIiv(m_texture, GL_TEXTURE_INTERNAL_FORMAT, &m_internalFormat);
		wglMakeCurrent(NULL, NULL);//nullptr
		XEG.hdcUnlock();
	}
	else
	{
		glGetTexParameterIiv(m_texture, GL_TEXTURE_INTERNAL_FORMAT, &m_internalFormat);
	}

	m_isLoaded = XTrue;
	return XTrue;
}
XBool XTexture::load(const char *filename, XResPos resPos)//从指定文件中载入图片资源
{
	if (m_isLoaded)
	{//如果图片已经载入则，这里需要检查是否需要进行更新
		if (m_resInfo != nullptr)
		{
			//资源已经载入，不在重新载入
			if (XFile::fileNameCompare(filename, m_resInfo->m_resName.c_str()))
			{
				XResourceTex *tempResTex = (XResourceTex *)m_resInfo->m_pointer;
				m_w = tempResTex->m_width;
				m_h = tempResTex->m_height;
				return true;
			}
			if (XResManager.updateRes(m_resInfo, filename, resPos, m_isThreadEvn))
			{
				XResourceTex *tempResTex = (XResourceTex *)m_resInfo->m_pointer;
				m_w = tempResTex->m_width;
				m_h = tempResTex->m_height;
				return true;
			}
			else
			{
				XResManager.releaseResource(m_resInfo);
				m_isLoaded = false;
				m_resInfo = nullptr;
			}
		}
		else
			return true;
	}
	if (m_cp == nullptr)
	{
		try
		{
#if TEX_DEBUG1
			++ CPSum;
			printf("%d-\n", CPSum);
#endif
			m_cp = new XSCounter;
			if (m_cp == NULL) return XFalse;
		}
		catch (...)
		{
			return XFalse;
		}
	}
	m_resInfo = XResManager.loadResource(filename, RESOURCE_TYPE_TEXTURE, resPos, m_isThreadEvn);
	if (m_resInfo != NULL)
	{
		XResourceTex *tempResTex = (XResourceTex *)m_resInfo->m_pointer;
		m_texture = tempResTex->m_texID;
		m_w = tempResTex->m_width;
		m_h = tempResTex->m_height;
		//m_internalFormat;	//这个地方没有初始化
		if (m_isThreadEvn)
		{
			XEG.hdcLock();
			wglMakeCurrent(XEG.getHDC(), XEG.getCopyHGLRC());

			glGetTexParameterIiv(m_texture, GL_TEXTURE_INTERNAL_FORMAT, &m_internalFormat);

			wglMakeCurrent(NULL, NULL);//nullptr
			XEG.hdcUnlock();
		}
		else
		{
			glGetTexParameterIiv(m_texture, GL_TEXTURE_INTERNAL_FORMAT, &m_internalFormat);
		}
#if TEX_DEBUG
		++ XTSum;
		printf("%d:%d+%s\n", XTSum, m_texture, filename);
#endif
		m_isLoaded = XTrue;
		return XTrue;
	}
	XMem::XDELETE(m_cp);
	m_isLoaded = XFalse;
	return XFalse;
}
XBool XTexture::reset()
{
	int sum = m_w * m_h;
	switch (m_mode)
	{
	case COLOR_RGBA:
	case COLOR_BGRA:sum = (sum << 2);	break;
	case COLOR_RGB:
	case COLOR_BGR:	sum = sum * 3;		break;
	case COLOR_GRAY:
	case COLOR_RED:	break;
	case COLOR_RGBA_F:sum = (sum << 4); break;
	case COLOR_RGB_F:sum = (sum * 12); break;
	case COLOR_GRAY_F:sum = (sum << 2); break;
	default: return XFalse;	//其他格式不支持该操作
	}
	unsigned char *pData = XMem::createArrayMem<unsigned char>(sum);
	if (pData == NULL) return XFalse;
	memset(pData, 0, sum);
	updateTexture(pData);
	XMem::XDELETE_ARRAY(pData);
	return XTrue;
}
void XTexture::release()	//释放图片资源
{
	if (!m_isLoaded) return;
#if TEX_DEBUG
	-- XTSum;
	printf("%d-%d\n", XTSum, m_texture);
#endif
	//经过资源管理器之后其自身不能释放资源
	if (m_resInfo == NULL)
	{
		if (glIsTexture(m_texture))
		{
#if WITH_ENGINE_STATISTICS
			XEG.decStaticsticsTexInfo(m_texture);
#endif
			LogNull("delete texture:%d", m_texture);
			glDeleteTextures(1, &m_texture);
		}
	}
	else
	{
		if (XResManager.releaseResource(m_resInfo))
			m_resInfo = NULL;
	}
	m_isLoaded = XFalse;
}
XTexture::XTexture(const XTexture& temp)	//拷贝构造函数
{
	if (this == &temp) return;		//防止自生拷贝
	if (temp.m_cp != NULL) ++temp.m_cp->m_counter;
	m_texture = temp.m_texture;
	m_w = temp.m_w;
	m_h = temp.m_h;
	m_isLoaded = temp.m_isLoaded;
	m_cp = temp.m_cp;

	m_resInfo = XResManager.copyResource(temp.m_resInfo);
	return;
}
XTexture& XTexture::operator = (const XTexture &temp)
{
	if (this == &temp) return *this;		//防止自生拷贝
	if (temp.m_cp != NULL) ++temp.m_cp->m_counter;
	if (m_cp != NULL && --m_cp->m_counter <= 0)
	{
		release();	//引用计数结束，释放资源
#if TEX_DEBUG1
		-- CPSum;
		printf("%d-\n", CPSum);
#endif
		XMem::XDELETE(m_cp);
	}
	m_texture = temp.m_texture;
	m_w = temp.m_w;
	m_h = temp.m_h;
	m_isLoaded = temp.m_isLoaded;
	m_cp = temp.m_cp;
	if (m_resInfo != NULL) XResManager.releaseResource(m_resInfo);
	m_resInfo = XResManager.copyResource(temp.m_resInfo);
	return *this;
}
}