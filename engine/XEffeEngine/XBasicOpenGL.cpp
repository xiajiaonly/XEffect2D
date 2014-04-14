//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "glew.h"
//#include <gl/GLAux.h>
#include "stdio.h"
#include "math.h"

#include "XBasicOpenGL.h"
#include "XBasicFun.h"
#include "XPixelCommon.h"

#include "XResourcePack.h"
#include "XLogBook.h"
#include "XSavePNG.h"
#if TEX_DEBUG
int XTSum = 0;
#endif
#if TEX_DEBUG1
int CPSum = 0;
#endif

unsigned int srcBlendMode[9] = {GL_ZERO,GL_ONE,GL_DST_COLOR,GL_ONE_MINUS_DST_COLOR,GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,
		GL_DST_ALPHA,GL_ONE_MINUS_DST_ALPHA,GL_SRC_ALPHA_SATURATE};
unsigned int dstBlendMode[8] = {GL_ZERO,GL_ONE,GL_SRC_COLOR,GL_ONE_MINUS_SRC_COLOR,GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,	
		GL_DST_ALPHA,GL_ONE_MINUS_DST_ALPHA};

void printShaderInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    static char *infoLog = NULL;

    glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if(infologLength > 0)
    {
        infoLog = createArrayMem<char>(infologLength);
		if(infoLog == NULL) return;
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
		AddLogInfoNull("%s\n",infoLog);
        XDELETE_ARRAY(infoLog);
    }
}
void printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    static char *infoLog = NULL;

    glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if(infologLength > 0)
    {
        infoLog = createArrayMem<char>(infologLength);
		if(infoLog == NULL) return;
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
		AddLogInfoNull("%s\n",infoLog);
        XDELETE_ARRAY(infoLog);
    }
}
int printOglError(const char *file,int line)
{
    static GLenum glErr;
    int retCode = 0;

    glErr = glGetError();
    while(glErr != GL_NO_ERROR)
    {
	//	AddLogInfoNull("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
 		AddLogInfoNull("glError in file %s @ line %d\n",file,line);
		printf("glError: 0x%x in file %s @ line %d\n",glErr,file,line);
        retCode = 1;
        glErr = glGetError();
    }
    return retCode;
}
char *textFileRead(const char *filename,_XResourcePosition resoursePosition) 
{
	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = XEE::defaultResourcePosition;
	if(resoursePosition == RESOURCE_LOCAL_PACK)
	{
	//	unsigned char *content = _XResourcePack::GetInstance().getFileData(filename);
	//	return (char *)content;
		return (char *)_XResourcePack::GetInstance().getFileData(filename);
	}else
	if(resoursePosition == RESOURCE_LOCAL_FOLDER)
	{
		FILE *fp;
		char *content = NULL;
		int count = 0;
		if(filename != NULL && (fp = fopen(filename,"rt")) != NULL) 
		{
			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);

			if(count > 0) 
			{
				//content = (char *)malloc(sizeof(char) * (count + 1));
				content = createArrayMem<char>(sizeof(char) * (count + 1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
		return content;
	}
	return NULL;
}

int textFileWrite(const char *filename,const char *s) 
{
	if(filename == NULL) return 0;
	if(s == NULL) return 0;
	FILE *fp = NULL;
	int status = 0;

	if((fp = fopen(filename,"w")) != NULL) 
	{	
		if(fwrite(s,sizeof(char),strlen(s),fp) == strlen(s)) status = 1;
		fclose(fp);
	}
	return(status);
}

_XBool setShader(const char* vertFile,const char* fragFile,int &pHandle,_XResourcePosition resoursePosition)
{
	if(vertFile == NULL || fragFile == NULL) return XFalse;
    char *vs = NULL;
	char *fs = NULL;
	GLuint v,f;

	v = glCreateShader(GL_VERTEX_SHADER);
    f = glCreateShader(GL_FRAGMENT_SHADER);
	//v = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
    //f = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

	vs = textFileRead(vertFile,resoursePosition);
    fs = textFileRead(fragFile,resoursePosition);
	if(vs == NULL || fs == NULL) return XFalse;

	const char * vv = vs;
    const char * ff = fs;

	glShaderSource(v, 1, &vv,NULL);
    glShaderSource(f, 1, &ff,NULL);
	//glShaderSourceARB(v, 1, &vv,NULL);
    //glShaderSourceARB(f, 1, &ff,NULL);

	XDELETE_ARRAY(vs);
	XDELETE_ARRAY(fs);

	glCompileShader(v);
	glCompileShader(f);
	//glCompileShaderARB(v);
	//glCompileShaderARB(f);

	printShaderInfoLog(v);
    printShaderInfoLog(f);

    pHandle = glCreateProgram();
	glAttachShader(pHandle,v);
    glAttachShader(pHandle,f);
    //pHandle = glCreateProgramObjectARB();
	//glAttachObjectARB(pHandle,v);
    //glAttachObjectARB(pHandle,f);

    glLinkProgram(pHandle);
    //glLinkProgramARB(pHandle);
    printProgramInfoLog(pHandle);
	return XTrue;
}
void changeSize(int h, int w)
{/*
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
	*/
}
//_XBool TextureLoad(GLuint &texture,const char *filename,int withAlpha,int *w,int *h,_XResourcePosition resoursePosition)
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
//	if((temp = (SDL_Surface *) loadImage(filename,withAlpha,resoursePosition)) != NULL) 
//	{
//		if((temp->w > XEE::maxTetureSize) || (temp->h > XEE::maxTetureSize)) 
//		{
//			fprintf(stderr, "Image size exceeds max texture size, which is %d pixels for each side\n",XEE::maxTetureSize);
//			AddLogInfoNull("Image size exceeds max texture size, which is %d pixels for each side\n",XEE::maxTetureSize);
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
//			glBindTexture(GL_TEXTURE_2D, texture);
//			//效果好
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//#if WITH_XSPRITE_EX
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
//			glBindTexture(GL_TEXTURE_2D, texture);
//		//	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
//
//			//效果好
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//#if WITH_XSPRITE_EX
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
////		glBindTexture(GL_TEXTURE_2D, texture);
////
////		//效果好
////	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
////	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
////#if WITH_XSPRITE_EX
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
////	return 1;
//}
//测试发现这个函数中的SDL_FreeSurface会存在内存IPR和FIM错误，具体需要SDL的代码实现来检测
_XBool TextureLoadEx(GLuint &texture,const char *filename,int *w,int *h,_XResourcePosition resoursePosition)
{
	if(filename == NULL || strlen(filename) <= 0) return XFalse;

	_XPixels<_XCurPixel> tmp;
	if(!tmp.load(filename,resoursePosition)) return XFalse;
	if((tmp.getWidth() > XEE::maxTetureSize || tmp.getHeight() > XEE::maxTetureSize) && XEE::maxTetureSize > 0) 	//贴图最大尺寸的检测
	{
		fprintf(stderr, "Image size exceeds max texture size, which is %d pixels for each side\n", XEE::maxTetureSize);
		AddLogInfoNull("Image size exceeds max texture size, which is %d pixels for each side\n", XEE::maxTetureSize);
		tmp.release();
		return XFalse;
	}
	if(w != NULL) (* w) = tmp.getWidth();
	if(h != NULL) (* h) = tmp.getHeight();
	tmp.fitNPot();	//适应2的n次方
	// OpenGL Texture creation	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
//	glPixelStorei(GL_UNPACK_ALIGNMENT,1);

	//效果好
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#if WITH_XSPRITE_EX
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

	switch(tmp.getColorMode())
	{
	case COLOR_RGBA:
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,tmp.getWidth(),tmp.getHeight(),0,GL_RGBA,GL_UNSIGNED_BYTE,tmp.getPixel());
		break;
	case COLOR_BGRA:
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,tmp.getWidth(),tmp.getHeight(),0,GL_BGRA,GL_UNSIGNED_BYTE,tmp.getPixel());
		break;
	case COLOR_RGB:
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,tmp.getWidth(),tmp.getHeight(),0,GL_RGB,GL_UNSIGNED_BYTE,tmp.getPixel());
		break;
	case COLOR_BGR:
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,tmp.getWidth(),tmp.getHeight(),0,GL_BGR,GL_UNSIGNED_BYTE,tmp.getPixel());
		break;
	case COLOR_GRAY:
		glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,tmp.getWidth(),tmp.getHeight(),0,GL_LUMINANCE,GL_UNSIGNED_BYTE,tmp.getPixel());
		break;
	}

	tmp.release();
	return true;
}
_XBool TextureLoadQuarter(GLuint &texture,const char *filename)
{
	_XPixels<_XCurPixel> tmp;
	if(!tmp.load(filename)) return XFalse;
	int w = tmp.getWidth() >> 2;
	int h = tmp.getHeight() >> 2;
	unsigned char * tempPixel = createArrayMem<unsigned char>(w * h * 4);
	if(tempPixel == NULL)
	{
		tmp.release();
		return XFalse;
	}
	switch(tmp.getColorMode())
	{
	case COLOR_RGBA:
	case COLOR_BGRA:
		{
			int *p = (int *)tmp.getPixel();
			int *p1 = (int *)tempPixel;
			for(int i = 0; i < tmp.getHeight();i += 4)
				for(int j = 0;j < tmp.getWidth();j += 4)
					*(p1 + (i >> 2) * w + (j >> 2)) = *(p + i * tmp.getWidth() + j);
		}
		break;
	case COLOR_RGB:
	case COLOR_BGR:
		{
			unsigned char *p = tmp.getPixel();
			unsigned char *p1 = tempPixel;
			for(int i = 0; i < tmp.getWidth();i += 4)
				for(int j = 0;j < tmp.getHeight();j += 4)
				{
					*(p1 + i + j * w + 0) = *(p + (i << 2) + (j << 2) * tmp.getWidth() + 0);
					*(p1 + i + j * w + 1) = *(p + (i << 2) + (j << 2) * tmp.getWidth() + 1);
					*(p1 + i + j * w + 2) = *(p + (i << 2) + (j << 2) * tmp.getWidth() + 2);
					*(p1 + i + j * w + 3) = 255;
				}
		}
		break;
	case COLOR_GRAY:
		{//尚未实现
		}
		break;
	}
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	//效果好
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#if WITH_XSPRITE_EX
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#endif
	//速度快
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	switch(tmp.getColorMode())
	{
	case COLOR_RGBA:
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,tempPixel);
		break;
	case COLOR_BGRA:
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w,h,0,GL_BGRA,GL_UNSIGNED_BYTE,tempPixel);
		break;
	case COLOR_RGB:
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,w,h,0,GL_RGB,GL_UNSIGNED_BYTE,tempPixel);
		break;
	case COLOR_BGR:
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,w,h,0,GL_BGR,GL_UNSIGNED_BYTE,tempPixel);
		break;
	case COLOR_GRAY:
		//尚未实现
		break;
	}
	tmp.release();
	XDELETE_ARRAY(tempPixel);
	return XTrue;
}
_XBool initOpenGL(int width,int height)
{
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
	glEnable(GL_POINT_SMOOTH);		//开启各种抗锯齿功能
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	//glEnable(GL_POLYGON_SMOOTH);

	glShadeModel(GL_SMOOTH);						//启动反走样
	glMatrixMode(GL_PROJECTION);					//设置当前矩阵模式 （对投影矩阵应用之后的矩阵操作）
	glLoadIdentity();								//变换坐标系函数
	glOrtho(0,width,height,0,-1,1);					//only for 2D的原始数据
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(XEE::defaultBGColor.fR,XEE::defaultBGColor.fG,XEE::defaultBGColor.fB,XEE::defaultBGColor.fA);			//清除颜色
	//glDisable(GL_DEPTH);						//
	glDisable(GL_DEPTH_TEST);						//2D项目不需要深度测试

	if(glewInit() != 0)
	{//初始化失败
		AddLogInfoStr("Glew init error!\n");
		XEE::isOpenGL2Support = 0;
	}else
	{
		//记录显卡的最大贴图尺寸
		glGetIntegerv(GL_MAX_TEXTURE_SIZE,&XEE::maxTetureSize);	
		//检查硬件是否支持OpenGL2.0
		if(glewIsSupported("GL_VERSION_2_0"))
		{
			AddLogInfoStr("Ready for OpenGL 2.0\n");
			XEE::isOpenGL2Support = 1;
		}else
		{
			AddLogInfoStr("OpenGL 2.0 not supported\n");
			XEE::isOpenGL2Support = 0;
		}
		//开启多重采样抗锯齿
		//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB |GLUT_MULTISAMPLE);//，初始化的时候开启；2）
		GLint bufSum = 0;
		GLint samplesSum = 0;
		glGetIntegerv(GL_SAMPLE_BUFFERS, &bufSum);
		glGetIntegerv(GL_SAMPLES, &samplesSum);//可以检查多重采样是否可用，前者返回1，后者返回大于1表示可用; 3）
		if(bufSum == 1 && samplesSum > 1) glEnable(GL_MULTISAMPLE);
	}
	XEE::sceneX = 0;
	XEE::sceneY = 0;
	XEE::sceneWidth = width;
	XEE::sceneHeight = height;
	XEE::sceneSizeX = 1.0f;
	XEE::sceneSizeY = 1.0f;
	return XTrue;
}
#pragma comment(lib,"glu32.lib")
_XBool initOpenGL3D(int width,int height)
{
	if(glewInit() != 0)
	{//初始化失败
		AddLogInfoStr("Glew init error!\n");
		XEE::isOpenGL2Support = 0;
	}else
	{
		//记录显卡的最大贴图尺寸
		glGetIntegerv(GL_MAX_TEXTURE_SIZE,&XEE::maxTetureSize);	
		//检查硬件是否支持OpenGL2.0
		if(glewIsSupported("GL_VERSION_2_0"))
		{
			AddLogInfoStr("Ready for OpenGL 2.0\n");
			XEE::isOpenGL2Support = 1;
		}else
		{
			AddLogInfoStr("OpenGL 2.0 not supported\n");
			XEE::isOpenGL2Support = 0;
		}
		//开启多重采样抗锯齿
		//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB |GLUT_MULTISAMPLE);//，初始化的时候开启；2）
		GLint bufSum = 0;
		GLint samplesSum = 0;
		glGetIntegerv(GL_SAMPLE_BUFFERS, &bufSum);
		glGetIntegerv(GL_SAMPLES, &samplesSum);//可以检查多重采样是否可用，前者返回1，后者返回大于1表示可用; 3）
		if(bufSum == 1 && samplesSum > 1) glEnable(GL_MULTISAMPLE);
	}
	glMatrixMode(GL_PROJECTION);					//设置当前矩阵模式 （对投影矩阵应用之后的矩阵操作）
	glLoadIdentity();								//变换坐标系函数

	gluPerspective(45.0f,(float)(width) / (float)(height),1.0f,10000.0f);		//最后两个数值需要与之前的吻合，不然会出现模型破损的问题

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//3D
	glShadeModel(GL_SMOOTH);		//启动反走样
	glClearColor(XEE::defaultBGColor.fR,XEE::defaultBGColor.fG,XEE::defaultBGColor.fB,XEE::defaultBGColor.fA);				//清除颜色
	glClearDepth(1.0f);									//Depth Buffer Setup
	glClearStencil(0);
//	glEnable(GL_DEPTH);	
	glEnable(GL_DEPTH_TEST);							//Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								//The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);	//Really Nice Perspective Calculations
	glEnable(GL_TEXTURE_2D);							// Enable 2D Texture Mapping
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);	// Set Up Sphere Mapping
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);	// Set Up Sphere Mapping

	glAlphaFunc(GL_GREATER,0.1f);						//允许进行alpha测试
	glEnable(GL_ALPHA_TEST); 
	//下面的代码是为了解决背光面的问题
	//glEnable(GL_POLYGON_SMOOTH);			//开启这里会造成贴图破损
	//glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
	glEnable(GL_POINT_SMOOTH);		//开启各种抗锯齿功能
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_CULL_FACE);	//只允许渲染正面

	glEnable(GL_NORMALIZE);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	//+++++++++++++++++++++++++++++
	//注意这里，颜色与法线贴图有冲突
	glEnable(GL_COLOR_MATERIAL);	//开启这里会造成法线贴图颜色太暗的问题,不开启这个会造成颜色不生效
	//-----------------------------
	glMaterialfv(GL_FRONT,GL_SPECULAR,_XVector4(1.0f,1.0f,1.0f,1.0f));
	glMaterialf(GL_FRONT,GL_SHININESS,128.0f);

	//glDisable(GL_LINE_SMOOTH);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	XEE::sceneX = 0;
	XEE::sceneY = 0;
	XEE::sceneWidth = width;
	XEE::sceneHeight = height;
	XEE::sceneSizeX = 1.0f;
	XEE::sceneSizeY = 1.0f;
	return XTrue;
}
_XBool initOpenGLEx(const _XWindowData &windowData)
{
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
	glEnable(GL_POINT_SMOOTH);		//开启各种抗锯齿功能
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	//glEnable(GL_POLYGON_SMOOTH);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glShadeModel(GL_SMOOTH);						// Enable Smooth Shading
	glMatrixMode(GL_PROJECTION);					//设置当前矩阵模式 （对投影矩阵应用之后的矩阵操作）
	glLoadIdentity();								//变换坐标系函数
	switch(windowData.mode)
	{
	case WINDOW_SIZE_MODE_CLIP_LP:
		switch(windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0:
			switch(windowData.turnOverMode)
			{
			case WINDOW_TURNOVER_MODE_NULL:
				glOrtho(windowData.positionX,windowData.w + windowData.positionX,
					windowData.h + windowData.positionY,windowData.positionY,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_LTR:
				glOrtho(windowData.w + windowData.positionX,windowData.positionX,
					windowData.h + windowData.positionY,windowData.positionY,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_TTD:
				glOrtho(windowData.positionX,windowData.w + windowData.positionX,
					windowData.positionY,windowData.h + windowData.positionY,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_LTR_TTD:
				glOrtho(windowData.w + windowData.positionX,windowData.positionX,
					windowData.positionY,windowData.h + windowData.positionY,-1,1);
				break;
			}
			break;
		case WINDOW_ROTATE_MODE_90:
			switch(windowData.turnOverMode)
			{
			case WINDOW_TURNOVER_MODE_NULL:
				glOrtho(windowData.h + windowData.positionY,windowData.positionY,
					windowData.positionX,windowData.w + windowData.positionX,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_LTR:
				glOrtho(windowData.h + windowData.positionY,windowData.positionY,
					windowData.w + windowData.positionX,windowData.positionX,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_TTD:
				glOrtho(windowData.positionY,windowData.h + windowData.positionY,
					windowData.positionX,windowData.w + windowData.positionX,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_LTR_TTD:
				glOrtho(windowData.positionY,windowData.h + windowData.positionY,
					windowData.w + windowData.positionX,windowData.positionX,-1,1);
				break;
			}
			break;
		case WINDOW_ROTATE_MODE_180:
			switch(windowData.turnOverMode)
			{
			case WINDOW_TURNOVER_MODE_NULL:
				glOrtho(windowData.w + windowData.positionX,windowData.positionX,
					windowData.positionY,windowData.h + windowData.positionY,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_LTR:
				glOrtho(windowData.positionX,windowData.w + windowData.positionX,
					windowData.positionY,windowData.h + windowData.positionY,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_TTD:
				glOrtho(windowData.w + windowData.positionX,windowData.positionX,
					windowData.h + windowData.positionY,windowData.positionY,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_LTR_TTD:
				glOrtho(windowData.positionX,windowData.w + windowData.positionX,
					windowData.h + windowData.positionY,windowData.positionY,-1,1);
				break;
			}
			break;
		case WINDOW_ROTATE_MODE_270:
			switch(windowData.turnOverMode)
			{
			case WINDOW_TURNOVER_MODE_NULL:
				glOrtho(windowData.positionY,windowData.h + windowData.positionY,
					windowData.w + windowData.positionX,windowData.positionX,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_LTR:
				glOrtho(windowData.positionY,windowData.h + windowData.positionY,
					windowData.positionX,windowData.w + windowData.positionX,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_TTD:
				glOrtho(windowData.h + windowData.positionY,windowData.positionY,
					windowData.w + windowData.positionX,windowData.positionX,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_LTR_TTD:
				glOrtho(windowData.h + windowData.positionY,windowData.positionY,
					windowData.positionX,windowData.w + windowData.positionX,-1,1);
				break;
			}
			break;
		}
		XEE::sceneX = 0;
		XEE::sceneY = 0;
		XEE::sceneWidth = windowData.w;
		XEE::sceneHeight = windowData.h;
		XEE::sceneSizeX = 1.0f;
		XEE::sceneSizeY = 1.0f;
		break;
	case WINDOW_SIZE_MODE_CLIP_MID: //no pos
		{
			int left = (windowData.sceneW - windowData.w) >> 1;
			int top = (windowData.sceneH - windowData.h) >> 1;
			switch(windowData.rotateMode)
			{
			case WINDOW_ROTATE_MODE_0:
				switch(windowData.turnOverMode)
				{
				case WINDOW_TURNOVER_MODE_NULL:
					glOrtho(left,left + windowData.w,top + windowData.h,top,-1,1);
					break;
				case WINDOW_TURNOVER_MODE_LTR:
					glOrtho(left + windowData.w,left,top + windowData.h,top,-1,1);
					break;
				case WINDOW_TURNOVER_MODE_TTD:
					glOrtho(left,left + windowData.w,top,top + windowData.h,-1,1);
					break;
				case WINDOW_TURNOVER_MODE_LTR_TTD:
					glOrtho(left + windowData.w,left,top,top + windowData.h,-1,1);
					break;
				}
				break;
			case WINDOW_ROTATE_MODE_90:
				switch(windowData.turnOverMode)
				{
				case WINDOW_TURNOVER_MODE_NULL:
					glOrtho(top + windowData.h,top,left,left + windowData.w,-1,1);
					break;
				case WINDOW_TURNOVER_MODE_LTR:
					glOrtho(top + windowData.h,top,left + windowData.w,left,-1,1);
					break;
				case WINDOW_TURNOVER_MODE_TTD:
					glOrtho(top,top + windowData.h,left,left + windowData.w,-1,1);
					break;
				case WINDOW_TURNOVER_MODE_LTR_TTD:
					glOrtho(top,top + windowData.h,left + windowData.w,left,-1,1);
					break;
				}
				break;
			case WINDOW_ROTATE_MODE_180:
				switch(windowData.turnOverMode)
				{
				case WINDOW_TURNOVER_MODE_NULL:
					glOrtho(left + windowData.w,left,top,top + windowData.h,-1,1);
					break;
				case WINDOW_TURNOVER_MODE_LTR:
					glOrtho(left,left + windowData.w,top,top + windowData.h,-1,1);
					break;
				case WINDOW_TURNOVER_MODE_TTD:
					glOrtho(left + windowData.w,left,top + windowData.h,top,-1,1);
					break;
				case WINDOW_TURNOVER_MODE_LTR_TTD:
					glOrtho(left,left + windowData.w,top + windowData.h,top,-1,1);
					break;
				}
				break;
			case WINDOW_ROTATE_MODE_270:
				switch(windowData.turnOverMode)
				{
				case WINDOW_TURNOVER_MODE_NULL:
					glOrtho(top,top + windowData.h,left + windowData.w,left,-1,1);
					break;
				case WINDOW_TURNOVER_MODE_LTR:
					glOrtho(top,top + windowData.h,left,left + windowData.w,-1,1);
					break;
				case WINDOW_TURNOVER_MODE_TTD:
					glOrtho(top + windowData.h,top,left + windowData.w,left,-1,1);
					break;
				case WINDOW_TURNOVER_MODE_LTR_TTD:
					glOrtho(top + windowData.h,top,left,left + windowData.w,-1,1);
					break;
				}
				break;
			}
			XEE::sceneX = left;
			XEE::sceneY = top;
			XEE::sceneWidth = windowData.w;
			XEE::sceneHeight = windowData.h;
			XEE::sceneSizeX = 1.0f;
			XEE::sceneSizeY = 1.0f;
		}
		break;
	case WINDOW_SIZE_MODE_CLIP_RESIZE:
		{
			float sizeX = (float)(windowData.sceneW) / (float)(windowData.w);
			float sizeY = (float)(windowData.sceneH) / (float)(windowData.h);
			int resultX = 0;
			int resultY = 0;
			if(sizeX <= sizeY)
			{
				resultX = windowData.sceneW;
				resultY = windowData.h * sizeX;
				int top = (windowData.sceneH - resultY) >> 1;
				switch(windowData.rotateMode)
				{
				case WINDOW_ROTATE_MODE_0:
					switch(windowData.turnOverMode)
					{
					case WINDOW_TURNOVER_MODE_NULL:
						glOrtho(0,resultX,top + resultY,top,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_LTR:
						glOrtho(resultX,0,top + resultY,top,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_TTD:
						glOrtho(0,resultX,top,top + resultY,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_LTR_TTD:
						glOrtho(resultX,0,top,top + resultY,-1,1);
						break;
					}
					break;
				case WINDOW_ROTATE_MODE_90:
					switch(windowData.turnOverMode)
					{
					case WINDOW_TURNOVER_MODE_NULL:
						glOrtho(top + resultY,top,0,resultX,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_LTR:
						glOrtho(top + resultY,top,resultX,0,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_TTD:
						glOrtho(top,top + resultY,0,resultX,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_LTR_TTD:
						glOrtho(top,top + resultY,resultX,0,-1,1);
						break;
					}
					break;
				case WINDOW_ROTATE_MODE_180:
					switch(windowData.turnOverMode)
					{
					case WINDOW_TURNOVER_MODE_NULL:
						glOrtho(resultX,0,top,top + resultY,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_LTR:
						glOrtho(0,resultX,top,top + resultY,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_TTD:
						glOrtho(resultX,0,top + resultY,top,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_LTR_TTD:
						glOrtho(0,resultX,top + resultY,top,-1,1);
						break;
					}
					break;
				case WINDOW_ROTATE_MODE_270:
					switch(windowData.turnOverMode)
					{
					case WINDOW_TURNOVER_MODE_NULL:
						glOrtho(top,top + resultY,resultX,0,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_LTR:
						glOrtho(top,top + resultY,0,resultX,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_TTD:
						glOrtho(top + resultY,top,resultX,0,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_LTR_TTD:
						glOrtho(top + resultY,top,0,resultX,-1,1);
						break;
					}
					break;
				}
				XEE::sceneX = 0;
				XEE::sceneY = top;
				XEE::sceneWidth = resultX;
				XEE::sceneHeight = resultY;
				XEE::sceneSizeX = sizeX;
				XEE::sceneSizeY = sizeX;
			}else
			{
				resultX = windowData.w * sizeY;
				resultY = windowData.sceneH;
				int left = (windowData.sceneW - resultX) >> 1;
				switch(windowData.rotateMode)
				{
				case WINDOW_ROTATE_MODE_0:
					switch(windowData.turnOverMode)
					{
					case WINDOW_TURNOVER_MODE_NULL:
						glOrtho(left,left + resultX,resultY,0,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_LTR:
						glOrtho(left + resultX,left,resultY,0,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_TTD:
						glOrtho(left,left + resultX,0,resultY,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_LTR_TTD:
						glOrtho(left + resultX,left,0,resultY,-1,1);
						break;
					}
					break;
				case WINDOW_ROTATE_MODE_90:
					switch(windowData.turnOverMode)
					{
					case WINDOW_TURNOVER_MODE_NULL:
						glOrtho(resultY,0,left,left + resultX,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_LTR:
						glOrtho(resultY,0,left + resultX,left,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_TTD:
						glOrtho(0,resultY,left,left + resultX,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_LTR_TTD:
						glOrtho(0,resultY,left + resultX,left,-1,1);
						break;
					}
					break;
				case WINDOW_ROTATE_MODE_180:
					switch(windowData.turnOverMode)
					{
					case WINDOW_TURNOVER_MODE_NULL:
						glOrtho(left + resultX,left,0,resultY,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_LTR:
						glOrtho(left,left + resultX,0,resultY,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_TTD:
						glOrtho(left + resultX,left,resultY,0,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_LTR_TTD:
						glOrtho(left,left + resultX,resultY,0,-1,1);
						break;
					}
					break;
				case WINDOW_ROTATE_MODE_270:
					switch(windowData.turnOverMode)
					{
					case WINDOW_TURNOVER_MODE_NULL:
						glOrtho(0,resultY,left + resultX,left,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_LTR:
						glOrtho(0,resultY,left,left + resultX,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_TTD:
						glOrtho(resultY,0,left + resultX,left,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_LTR_TTD:
						glOrtho(resultY,0,left,left + resultX,-1,1);
						break;
					}
					break;
				}
				XEE::sceneX = left;
				XEE::sceneY = 0;
				XEE::sceneWidth = resultX;
				XEE::sceneHeight = resultY;
				XEE::sceneSizeX = sizeY;
				XEE::sceneSizeY = sizeY;
			}
		}
		break;
	case WINDOW_SIZE_MODE_RESIZE_CLIP:
		{
			float sizeX = (float)(windowData.sceneW) / (float)(windowData.w);
			float sizeY = (float)(windowData.sceneH) / (float)(windowData.h);
			int resultX = 0;
			int resultY = 0;
			if(sizeX >= sizeY)
			{
				resultX = windowData.sceneW;
				resultY = windowData.h * sizeX;
				int top = (windowData.sceneH - resultY) >> 1;
				switch(windowData.rotateMode)
				{
				case WINDOW_ROTATE_MODE_0:
					switch(windowData.turnOverMode)
					{
					case WINDOW_TURNOVER_MODE_NULL:
						glOrtho(0,resultX,top + resultY,top,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_LTR:
						glOrtho(resultX,0,top + resultY,top,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_TTD:
						glOrtho(0,resultX,top,top + resultY,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_LTR_TTD:
						glOrtho(resultX,top,0,top + resultY,-1,1);
						break;
					}
					break;
				case WINDOW_ROTATE_MODE_90:
					switch(windowData.turnOverMode)
					{
					case WINDOW_TURNOVER_MODE_NULL:
						glOrtho(top + resultY,top,0,resultX,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_LTR:
						glOrtho(top + resultY,top,resultX,0,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_TTD:
						glOrtho(top,top + resultY,0,resultX,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_LTR_TTD:
						glOrtho(top,top + resultY,resultX,0,-1,1);
						break;
					}
					break;
				case WINDOW_ROTATE_MODE_180:
					switch(windowData.turnOverMode)
					{
					case WINDOW_TURNOVER_MODE_NULL:
						glOrtho(resultX,0,top,top + resultY,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_LTR:
						glOrtho(0,resultX,top,top + resultY,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_TTD:
						glOrtho(resultX,0,top + resultY,top,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_LTR_TTD:
						glOrtho(0,resultX,top + resultY,top,-1,1);
						break;
					}
					break;
				case WINDOW_ROTATE_MODE_270:
					switch(windowData.turnOverMode)
					{
					case WINDOW_TURNOVER_MODE_NULL:
						glOrtho(top,top + resultY,resultX,0,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_LTR:
						glOrtho(top,top + resultY,0,resultX,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_TTD:
						glOrtho(top + resultY,top,resultX,0,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_LTR_TTD:
						glOrtho(top + resultY,top,0,resultX,-1,1);
						break;
					}
					break;
				}
				XEE::sceneX = 0;
				XEE::sceneY = top;
				XEE::sceneWidth = resultX;
				XEE::sceneHeight = resultY;
				XEE::sceneSizeX = sizeX;
				XEE::sceneSizeY = sizeX;
			}else
			{
				resultX = windowData.w * sizeY;
				resultY = windowData.sceneH;
				int left = (windowData.sceneW - resultX) >> 1;
				switch(windowData.rotateMode)
				{
				case WINDOW_ROTATE_MODE_0:
					switch(windowData.turnOverMode)
					{
					case WINDOW_TURNOVER_MODE_NULL:
						glOrtho(left,left + resultX,resultY,0,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_LTR:
						glOrtho(left + resultX,left,resultY,0,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_TTD:
						glOrtho(left,left + resultX,0,resultY,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_LTR_TTD:
						glOrtho(left + resultX,left,0,resultY,-1,1);
						break;
					}
					break;
				case WINDOW_ROTATE_MODE_90:
					switch(windowData.turnOverMode)
					{
					case WINDOW_TURNOVER_MODE_NULL:
						glOrtho(resultY,0,left,left + resultX,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_LTR:
						glOrtho(resultY,0,left + resultX,left,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_TTD:
						glOrtho(0,resultY,left,left + resultX,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_LTR_TTD:
						glOrtho(0,resultY,left + resultX,left,-1,1);
						break;
					}
					break;
				case WINDOW_ROTATE_MODE_180:
					switch(windowData.turnOverMode)
					{
					case WINDOW_TURNOVER_MODE_NULL:
						glOrtho(left + resultX,left,0,resultY,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_LTR:
						glOrtho(left,left + resultX,0,resultY,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_TTD:
						glOrtho(left + resultX,left,resultY,0,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_LTR_TTD:
						glOrtho(left,left + resultX,resultY,0,-1,1);
						break;
					}
					break;
				case WINDOW_ROTATE_MODE_270:
					switch(windowData.turnOverMode)
					{
					case WINDOW_TURNOVER_MODE_NULL:
						glOrtho(0,resultY,left + resultX,left,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_LTR:
						glOrtho(0,resultY,left,left + resultX,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_TTD:
						glOrtho(resultY,0,left + resultX,left,-1,1);
						break;
					case WINDOW_TURNOVER_MODE_LTR_TTD:
						glOrtho(resultY,0,left,left + resultX,-1,1);
						break;
					}
					break;
				}
				XEE::sceneX = left;
				XEE::sceneY = 0;
				XEE::sceneWidth = resultX;
				XEE::sceneHeight = resultY;
				XEE::sceneSizeX = sizeY;
				XEE::sceneSizeY = sizeY;
			}
		}
		break;
	case WINDOW_SIZE_MODE_RESIZE:
		switch(windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0:
			switch(windowData.turnOverMode)
			{
			case WINDOW_TURNOVER_MODE_NULL:
				glOrtho(0,windowData.sceneW,windowData.sceneH,0,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_LTR:
				glOrtho(windowData.sceneW,0,
					windowData.sceneH,0,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_TTD:
				glOrtho(0,windowData.sceneW,
					0,windowData.sceneH,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_LTR_TTD:
				glOrtho(windowData.sceneW,0,
					0,windowData.sceneH,-1,1);
				break;
			}
			break;
		case WINDOW_ROTATE_MODE_90:
			switch(windowData.turnOverMode)
			{
			case WINDOW_TURNOVER_MODE_NULL:
				glOrtho(windowData.sceneH,0,0,windowData.sceneW,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_LTR:
				glOrtho(windowData.sceneH,0,windowData.sceneW,0,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_TTD:
				glOrtho(0,windowData.sceneH,0,windowData.sceneW,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_LTR_TTD:
				glOrtho(0,windowData.sceneH,windowData.sceneW,0,-1,1);
				break;
			}
			break;
		case WINDOW_ROTATE_MODE_180:
			switch(windowData.turnOverMode)
			{
			case WINDOW_TURNOVER_MODE_NULL:
				glOrtho(windowData.sceneW,0,0,windowData.sceneH,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_LTR:
				glOrtho(0,windowData.sceneW,0,windowData.sceneH,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_TTD:
				glOrtho(windowData.sceneW,0,windowData.sceneH,0,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_LTR_TTD:
				glOrtho(0,windowData.sceneW,windowData.sceneH,0,-1,1);
				break;
			}
			break;
		case WINDOW_ROTATE_MODE_270:
			switch(windowData.turnOverMode)
			{
			case WINDOW_TURNOVER_MODE_NULL:
				glOrtho(0,windowData.sceneH,windowData.sceneW,0,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_LTR:
				glOrtho(0,windowData.sceneH,0,windowData.sceneW,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_TTD:
				glOrtho(windowData.sceneH,0,windowData.sceneW,0,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_LTR_TTD:
				glOrtho(windowData.sceneH,0,0,windowData.sceneW,-1,1);
				break;
			}
			break;
		}
		XEE::sceneX = 0;
		XEE::sceneY = 0;
		XEE::sceneWidth = windowData.sceneW;
		XEE::sceneHeight = windowData.sceneH;
		XEE::sceneSizeX = (float)(windowData.sceneW) / (float)(windowData.w);
		XEE::sceneSizeY = (float)(windowData.sceneH) / (float)(windowData.h);
		break;
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(XEE::defaultBGColor.fR,XEE::defaultBGColor.fG,XEE::defaultBGColor.fB,XEE::defaultBGColor.fA);			//清除颜色
	//glDisable(GL_DEPTH);							//2D项目不需要深度测试
	glDisable(GL_DEPTH_TEST);						//2D项目不需要深度测试
	//glEnalbe(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	switch(windowData.rotateMode)
	{
	case WINDOW_ROTATE_MODE_0: break;	//do nothing
	case WINDOW_ROTATE_MODE_90: 
		switch(windowData.mode)
		{
		case WINDOW_SIZE_MODE_CLIP_LP:
			glTranslatef(windowData.h,0,0);
			glRotatef(90,0,0,1);
			break;
		case WINDOW_SIZE_MODE_CLIP_MID:
		case WINDOW_SIZE_MODE_CLIP_RESIZE:
		case WINDOW_SIZE_MODE_RESIZE_CLIP:
		case WINDOW_SIZE_MODE_RESIZE:
			glTranslatef(windowData.sceneH,0,0);
			glRotatef(90,0,0,1);
			break;
		}
		break;
	case WINDOW_ROTATE_MODE_180: break;	//do nothing
	case WINDOW_ROTATE_MODE_270: 
		switch(windowData.mode)
		{
		case WINDOW_SIZE_MODE_CLIP_LP:
			glTranslatef(windowData.h,0,0);
			glRotatef(90,0,0,1);
			break;
		case WINDOW_SIZE_MODE_CLIP_MID:
		case WINDOW_SIZE_MODE_CLIP_RESIZE:
		case WINDOW_SIZE_MODE_RESIZE_CLIP:
		case WINDOW_SIZE_MODE_RESIZE:
			glTranslatef(windowData.sceneH,0,0);
			glRotatef(90,0,0,1);
			break;
		}
		break;
	}
	if(glewInit() != 0)
	{//初始化失败
		AddLogInfoStr("Glew init error!\n");
		XEE::isOpenGL2Support = 0;
	}else
	{
		//记录显卡的最大贴图尺寸
		glGetIntegerv(GL_MAX_TEXTURE_SIZE,&XEE::maxTetureSize);	
		//检查硬件是否支持OpenGL2.0
		if(glewIsSupported("GL_VERSION_2_0"))
		{
			AddLogInfoStr("Ready for OpenGL 2.0\n");
			XEE::isOpenGL2Support = 1;
		}else
		{
			AddLogInfoStr("OpenGL 2.0 not supported\n");
			XEE::isOpenGL2Support = 0;
		}
		//开启多重采样抗锯齿
		//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB |GLUT_MULTISAMPLE);//，初始化的时候开启；2）
		GLint bufSum = 0;
		GLint samplesSum = 0;
		glGetIntegerv(GL_SAMPLE_BUFFERS, &bufSum);
		glGetIntegerv(GL_SAMPLES, &samplesSum);//可以检查多重采样是否可用，前者返回1，后者返回大于1表示可用; 3）
		if(bufSum == 1 && samplesSum > 1) glEnable(GL_MULTISAMPLE);
	}
	return XTrue;
}
void drawLine(int ax,int ay,int bx,int by,int width,float r,float g,float b)
{
	glDisable(GL_TEXTURE_2D);
	if(XEE::isLineSmooth != 0)
	{
		glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glColor3f(r,g,b);
		glLineWidth(width);
		glBegin(GL_LINES);
		glVertex2i(ax,ay);
		glVertex2i(bx,by);
		glEnd();
		glDisable(GL_BLEND);
	}else
	{
		glDisable(GL_TEXTURE_2D);
		glColor3f(r,g,b);
		glLineWidth(width);
		glBegin(GL_LINES);
		glVertex2i(ax,ay);
		glVertex2i(bx,by);
		glEnd();
	}
	glEnable(GL_TEXTURE_2D);
}
void drawLine(int ax, int ay, int bx, int by,int width, float r, float g, float b, float a,int type)
{
    glDisable(GL_TEXTURE_2D);
	if(XEE::isLineSmooth != 0)
	{
		glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
		glEnable(GL_LINE_SMOOTH);
	}

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if(type == 0)
	{//实线
		glColor4f( r, g, b, a);
		glLineWidth(width);
		glBegin(GL_LINES);
		glVertex2i( ax, ay);
		glVertex2i( bx, by);
		glEnd();
	}else
	{//虚线
		glColor4f( r, g, b, a);
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(1,0x00ff);
		glLineWidth(width);
		glBegin(GL_LINES);
		glVertex2i( ax, ay);
		glVertex2i( bx, by);
		glEnd();
		glDisable(GL_LINE_STIPPLE);
	}

    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
} 
void drawLine(const _XVector2& p0, const _XVector2& p1, int width,float r,float g,float b,float a,int type)
{
	drawLine(p0.x,p0.y,p1.x,p1.y,width,r,g,b,a,type);
}
void drawLine(const _XVector2& p,float angle,float length)
{
	_XVector2 temp(p.x + length * cos(angle * ANGLE_TO_RADIAN),p.y + length * sin(angle * ANGLE_TO_RADIAN));
	drawLine(p,temp);
}
void drawRect(const _XRect& rect,int width,float r,float g,float b,float a,int type)
{
	glDisable(GL_TEXTURE_2D);
	if(XEE::isLineSmooth != 0)
	{
		glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
		glEnable(GL_LINE_SMOOTH);
	}

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if(type == 0)
	{//实线
		glColor4f( r, g, b, a);
		glLineWidth(width);
		glBegin(GL_LINE_STRIP);
		glVertex2i(rect.left,rect.top);
		glVertex2i(rect.right,rect.top);
		glVertex2i(rect.right,rect.bottom);
		glVertex2i(rect.left,rect.bottom);
		glVertex2i(rect.left,rect.top);
		glEnd();
	}else
	{//虚线
		glColor4f( r, g, b, a);
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(1,0x00ff);
		glLineWidth(width);
		glBegin(GL_LINE_STRIP);
		glVertex2i(rect.left,rect.top);
		glVertex2i(rect.right,rect.top);
		glVertex2i(rect.right,rect.bottom);
		glVertex2i(rect.left,rect.bottom);
		glVertex2i(rect.left,rect.top);
		glEnd();
		glDisable(GL_LINE_STIPPLE);
	}

    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
}
void drawBox(float x,float y,float sizeX,float sizeY)
{
	glDisable(GL_TEXTURE_2D);
	if(XEE::isLineSmooth != 0)
	{
		glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1.0f,1.0f,1.0f,1.0f);
		glLineWidth(1);
		glBegin(GL_LINE_STRIP);
		glVertex2i(x - sizeX,y + sizeY);
		glVertex2i(x - sizeX,y - sizeY);
		glVertex2i(x + sizeX,y - sizeY);
		glVertex2i(x + sizeX,y + sizeY);
		glVertex2i(x - sizeX,y + sizeY);
		glEnd();
		glDisable(GL_BLEND);
	}else
	{
		glColor4f(1.0f,1.0f,1.0f,1.0f);
		glLineWidth(1);
		glBegin(GL_LINE_STRIP);
		glVertex2i(x - sizeX,y + sizeY);
		glVertex2i(x - sizeX,y - sizeY);
		glVertex2i(x + sizeX,y - sizeY);
		glVertex2i(x + sizeX,y + sizeY);
		glVertex2i(x - sizeX,y + sizeY);
		glEnd();
	}
	glEnable(GL_TEXTURE_2D);
}
void drawBox(float x,float y,float sizeX,float sizeY,int w,float r,float g,float b)
{
	glDisable(GL_TEXTURE_2D);
	if(XEE::isLineSmooth != 0)
	{
		glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor3f(r,g,b);
		glLineWidth(w);
		glBegin(GL_LINE_STRIP);
		glVertex2i(x - sizeX,y + sizeY);
		glVertex2i(x - sizeX,y - sizeY);
		glVertex2i(x + sizeX,y - sizeY);
		glVertex2i(x + sizeX,y + sizeY);
		glVertex2i(x - sizeX,y + sizeY);
		glEnd();
		glDisable(GL_BLEND);
	}else
	{
		glColor3f(r,g,b);
		glLineWidth(w);
		glBegin(GL_LINE_STRIP);
		glVertex2i(x - sizeX,y + sizeY);
		glVertex2i(x - sizeX,y - sizeY);
		glVertex2i(x + sizeX,y - sizeY);
		glVertex2i(x + sizeX,y + sizeY);
		glVertex2i(x - sizeX,y + sizeY);
		glEnd();
	}
	glEnable(GL_TEXTURE_2D);
}
void drawBox(float x,float y,float sizeX,float sizeY,int w,float r,float g,float b,float a)
{
	glDisable(GL_TEXTURE_2D);
	if(XEE::isLineSmooth != 0)
	{
		glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
		glEnable(GL_LINE_SMOOTH);
	}
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(r,g,b,a);
	glLineWidth(w);
	glBegin(GL_LINE_STRIP);
	glVertex2i(x - sizeX,y + sizeY);
	glVertex2i(x - sizeX,y - sizeY);
	glVertex2i(x + sizeX,y - sizeY);
	glVertex2i(x + sizeX,y + sizeY);
	glVertex2i(x - sizeX,y + sizeY);
	glEnd();
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
//	drawLine(x - sizeX,y + sizeY,x - sizeX,y - sizeY,w,r,g,b,a);
//	drawLine(x - sizeX,y + sizeY,x + sizeX,y + sizeY,w,r,g,b,a);
//	drawLine(x + sizeX,y + sizeY,x + sizeX,y - sizeY,w,r,g,b,a);
//	drawLine(x - sizeX,y - sizeY,x + sizeX,y - sizeY,w,r,g,b,a);
}
void drawBox(float x,float y,float sizeX,float sizeY,float angle)
{
	_XVector2 P1(x - (sizeX * cos(angle) - sizeY * sin(angle)),y + (sizeX * sin(angle) + sizeY * cos(angle)));
	_XVector2 P2(x - (sizeX * cos(angle) + sizeY * sin(angle)),y + (sizeX * sin(angle) - sizeY * cos(angle)));

	_XVector2 P3(x + (sizeX * cos(angle) + sizeY * sin(angle)),y - (sizeX * sin(angle) - sizeY * cos(angle)));
	_XVector2 P4(x + (sizeX * cos(angle) - sizeY * sin(angle)),y - (sizeX * sin(angle) + sizeY * cos(angle)));

	glDisable(GL_TEXTURE_2D);
	if(XEE::isLineSmooth != 0)
	{
		glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor3f(1.0f,1.0f,1.0f);
		glLineWidth(1);
		glBegin(GL_LINE_STRIP);
		glVertex2i(P1.x,P1.y);
		glVertex2i(P2.x,P2.y);
		glVertex2i(P4.x,P4.y);
		glVertex2i(P3.x,P3.y);
		glVertex2i(P1.x,P1.y);
		glEnd();
		glDisable(GL_BLEND);
	}else
	{
		glColor3f(1.0f,1.0f,1.0f);
		glLineWidth(1);
		glBegin(GL_LINE_STRIP);
		glVertex2i(P1.x,P1.y);
		glVertex2i(P2.x,P2.y);
		glVertex2i(P4.x,P4.y);
		glVertex2i(P3.x,P3.y);
		glVertex2i(P1.x,P1.y);
		glEnd();
	}
	glEnable(GL_TEXTURE_2D);
}
void drawBox(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3,int width,float r,float g,float b)
{
	glDisable(GL_TEXTURE_2D);
	if(XEE::isLineSmooth != 0)
	{
		glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor3f(r,g,b);
		glLineWidth(width);
		glBegin(GL_LINE_STRIP);
		glVertex2i(p0.x,p0.y);
		glVertex2i(p1.x,p1.y);
		glVertex2i(p2.x,p2.y);
		glVertex2i(p3.x,p3.y);
		glVertex2i(p0.x,p0.y);
		glEnd();
		glDisable(GL_BLEND);
	}else
	{
		glColor3f(r,g,b);
		glLineWidth(width);
		glBegin(GL_LINE_STRIP);
		glVertex2i(p0.x,p0.y);
		glVertex2i(p1.x,p1.y);
		glVertex2i(p2.x,p2.y);
		glVertex2i(p3.x,p3.y);
		glVertex2i(p0.x,p0.y);
		glEnd();
		glDisable(GL_BLEND);
	}
	glEnable(GL_TEXTURE_2D);
}
void drawBox(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3,int width,float r,float g,float b,float a)
{
	glDisable(GL_TEXTURE_2D);
	if(XEE::isLineSmooth != 0)
	{
		glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
		glEnable(GL_LINE_SMOOTH);
	}
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(r,g,b,a);
	glLineWidth(width);
	glBegin(GL_LINE_STRIP);
	glVertex2i(p0.x,p0.y);
	glVertex2i(p1.x,p1.y);
	glVertex2i(p2.x,p2.y);
	glVertex2i(p3.x,p3.y);
	glVertex2i(p0.x,p0.y);
	glEnd();
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
}
void drawPoint(int x,int y,float r,float g,float b,float a)
{
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(r,g,b,a);
	glPointSize(1.0f);
	glBegin(GL_POINTS);
		glVertex2i(x,y);
	glEnd();

    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
}
void drawPoint(int x,int y,float size,float r,float g,float b,float a)
{
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(r,g,b,a);
	glPointSize(size);
	glBegin(GL_POINTS);
		glVertex2i(x,y);
	glEnd();

    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
}
void drawCircle(float px, float py, float sr, int an,float r,float g,float b,float a)
{
	if(an <= 0 || sr <= 0.0f) return;
	glColor4f(r,g,b,a);
	glBegin(GL_POLYGON);
		for(float i = 0;i < PI2; i +=(PI2)/an)
		{
			glVertex2i(px + cosf(i) * sr,py + sinf(i) * sr);
		}
	glEnd();
}
//这里是关于圆角的定义
bool isRoundCornerInit = false;
#define ROUND_CORNER_DENSITY (5)
_XVector2 roundCornerData[ROUND_CORNER_DENSITY * 4];	//10度一个弧
float roundCornerR = 8.0f;	//圆角的半径
float roundCornerD = 16.0f;	//圆角的直径
inline void roundCornerInit()
{
	if(!isRoundCornerInit)
	{
		float angle;
		for(int i = 0;i < ROUND_CORNER_DENSITY * 4;++ i)
		{
			angle = i * 360.0f / (ROUND_CORNER_DENSITY * 4.0f) * ANGLE_TO_RADIAN;
			roundCornerData[i].set(roundCornerR * cos(angle),
				roundCornerR * sin(angle));
		}
		isRoundCornerInit = true;
	}
}
void drawFillBox(const _XVector2& pos,const _XVector2& size,
				 float r,float g,float b)	//描绘实心矩形
{
	//drawLine(pos.x,pos.y + size.y * 0.5f,pos.x + size.x,pos.y + size.y * 0.5f,size.y,r,g,b);
	glDisable(GL_TEXTURE_2D);
	glColor3f(r,g,b);
	glBegin(GL_QUADS);
	glVertex2i(pos.x,pos.y);
	glVertex2i(pos.x + size.x,pos.y);
	glVertex2i(pos.x + size.x,pos.y + size.y);
	glVertex2i(pos.x,pos.y + size.y);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}
void drawFillBoxEx(const _XVector2& pos,const _XVector2& size,
				 float r,float g,float b,
				 bool withLine,bool withRoundCorner)	//描绘实心矩形
{
	glDisable(GL_TEXTURE_2D);
	if(withRoundCorner && size.x >= roundCornerD && size.y >= roundCornerD)
	{
		roundCornerInit();
		if(withLine)
		{//描绘边框
			glColor3f(0.15f,0.15f,0.15f);
			glLineWidth(1.0f);
			glBegin(GL_LINE_STRIP);
			for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
			{
				glVertex2f(roundCornerData[i].x + size.x - roundCornerR + pos.x,
					roundCornerData[i].y + size.y - roundCornerR + pos.y);
			}
			for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
			{
				glVertex2f(roundCornerData[i + ROUND_CORNER_DENSITY].x + roundCornerR + pos.x,
					roundCornerData[i + ROUND_CORNER_DENSITY].y + size.y - roundCornerR + pos.y);
			}
			for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
			{
				glVertex2f(roundCornerData[i + ROUND_CORNER_DENSITY * 2].x + roundCornerR + pos.x,
					roundCornerData[i + ROUND_CORNER_DENSITY * 2].y + roundCornerR + pos.y);
			}
			for(int i = 0;i < ROUND_CORNER_DENSITY;++ i)
			{
				glVertex2f(roundCornerData[i + ROUND_CORNER_DENSITY * 3].x + size.x - roundCornerR + pos.x,
					roundCornerData[i + ROUND_CORNER_DENSITY * 3].y + roundCornerR + pos.y);
			}
			glVertex2i(roundCornerData[0].x + size.x - roundCornerR + pos.x,
					roundCornerData[0].y + size.y - roundCornerR + pos.y);
			glEnd();
		}
		glColor3f(r * 0.9f,g * 0.9f,b * 0.9f);
		glBegin(GL_POLYGON);
		for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
		{
			glVertex2f(roundCornerData[i].x + size.x - roundCornerR + pos.x,
				roundCornerData[i].y + size.y - roundCornerR + pos.y);
		}
		for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
		{
			glVertex2f(roundCornerData[i + ROUND_CORNER_DENSITY].x + roundCornerR + pos.x,
				roundCornerData[i + ROUND_CORNER_DENSITY].y + size.y - roundCornerR + pos.y);
		}
		glColor3f(r * 1.1f,g * 1.1f,b * 1.1f);
		for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
		{
			glVertex2f(roundCornerData[i + ROUND_CORNER_DENSITY * 2].x + roundCornerR + pos.x,
				roundCornerData[i + ROUND_CORNER_DENSITY * 2].y + roundCornerR + pos.y);
		}
		for(int i = 0;i < ROUND_CORNER_DENSITY;++ i)
		{
			glVertex2f(roundCornerData[i + ROUND_CORNER_DENSITY * 3].x + size.x - roundCornerR + pos.x,
				roundCornerData[i + ROUND_CORNER_DENSITY * 3].y + roundCornerR + pos.y);
		}
		glEnd();
	}else
	{
		//drawLine(pos.x,pos.y + size.y * 0.5f,pos.x + size.x,pos.y + size.y * 0.5f,size.y,r,g,b);
		if(withLine)
		{//描绘边框
			glColor3f(0.15f,0.15f,0.15f);
			glLineWidth(1.0f);
			glBegin(GL_LINE_STRIP);
			glVertex2i(pos.x,pos.y);
			glVertex2i(pos.x + size.x,pos.y);
			glVertex2i(pos.x + size.x,pos.y + size.y);
			glVertex2i(pos.x,pos.y + size.y);
			glVertex2i(pos.x,pos.y);
			glEnd();
		}
		glBegin(GL_QUADS);
		glColor3f(r * 1.1f,g * 1.1f,b * 1.1f);
		glVertex2i(pos.x,pos.y);
		glVertex2i(pos.x + size.x,pos.y);
		glColor3f(r * 0.9f,g * 0.9f,b * 0.9f);
		glVertex2i(pos.x + size.x,pos.y + size.y);
		glVertex2i(pos.x,pos.y + size.y);
		glEnd();
	}
	glEnable(GL_TEXTURE_2D);
}
void drawFillBox(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3,
				 float r,float g,float b)
{
	glDisable(GL_TEXTURE_2D);
	glColor3f(r,g,b);
	glBegin(GL_QUADS);
	glVertex2i(p0.x,p0.y);
	glVertex2i(p1.x,p1.y);
	glVertex2i(p2.x,p2.y);
	glVertex2i(p3.x,p3.y);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}
void drawFillBoxEx(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3,
				 float r,float g,float b,
				 bool withLine)
{
	glDisable(GL_TEXTURE_2D);
	if(withLine)
	{//描绘边框
		glColor3f(0.15f,0.15f,0.15f);
		glLineWidth(1.0f);
		glBegin(GL_LINE_STRIP);
		glVertex2i(p0.x,p0.y);
		glVertex2i(p1.x,p1.y);
		glVertex2i(p2.x,p2.y);
		glVertex2i(p3.x,p3.y);
		glVertex2i(p0.x,p0.y);
		glEnd();
	}
	glBegin(GL_QUADS);
	glColor3f(r * 1.1f,g * 1.1f,b * 1.1f);
	glVertex2i(p0.x,p0.y);
	glVertex2i(p1.x,p1.y);
	glColor3f(r * 0.9f,g * 0.9f,b * 0.9f);
	glVertex2i(p2.x,p2.y);
	glVertex2i(p3.x,p3.y);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}
void drawFillBoxBW(const _XVector2& pos,const _XVector2& size,const _XVector2& cSize)
{
	glDisable(GL_TEXTURE_2D);
	glColor3f(1.0f,1.0f,1.0f);
	//先描绘一个白色底
	glBegin(GL_QUADS);
	glVertex2i(pos.x,pos.y);
	glVertex2i(pos.x + size.x,pos.y);
	glVertex2i(pos.x + size.x,pos.y + size.y);
	glVertex2i(pos.x,pos.y + size.y);
	glEnd();
	int w = size.x / cSize.x;
	int h = size.y / cSize.y;
	_XVector2 tempPos;
	glColor3f(0.0f,0.0f,0.0f);
	//方法1
	glBegin(GL_QUADS);
	for(int i = 0;i < w;++ i)
	{
		tempPos.x = pos.x + i * cSize.x;
		for(int j = 0;j < h;++ j)
		{
			if((i + j)%2 == 0)
			{
				tempPos.y = pos.y + j * cSize.y;
				glVertex2f(tempPos.x,tempPos.y);
				glVertex2f(tempPos.x + cSize.x,tempPos.y);
				glVertex2f(tempPos.x + cSize.x,tempPos.y + cSize.y);
				glVertex2f(tempPos.x,tempPos.y + cSize.y);
			}
		}
	}
	glEnd();
	glEnable(GL_TEXTURE_2D);
	//方法2
	//int arraySize = w * (h >> 1) * 4;
	//_XVector2 *vPoint = createArrayMem<_XVector2>(arraySize);
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
	//glEnable(GL_TEXTURE_2D);
	//XDELETE_ARRAY(vPoint);
}
void drawTexture(_XTexture &tex,_XVector2 &pos,int cW,int cH,
	_XVector2 *vArray,_XVector2 *uArray,int arrayW,int arrayH)
{
	if(vArray == NULL || uArray == NULL) return;
	int tW = tex.m_w;
	int tH = tex.m_h;
	int ux = (tW - cW) >> 1,uy = (tH - cH) >> 1;
	int vx = -(cW >> 1),vy = -(cH >> 1);

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
#if WITH_XSPRITE_EX
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
#endif
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0,tW << 1, 0,tH << 1, -1, 1);
			
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(pos.x + (cW >> 1),pos.y + (cH >> 1), 0);

	glBindTexture(GL_TEXTURE_2D,tex.m_texture);
	//glActiveTexture(GL_TEXTURE0);
	glColor4f(1.0f,1.0f,1.0f,1.0f);

	for(int j = 0;j < arrayH - 1;++ j)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for(int i = 0;i < arrayW;++ i)
		{	//左上
			int offsetTemp = i + j * arrayW;
			glTexCoord2d(ux + uArray[offsetTemp].x,uy + uArray[offsetTemp].y);	//u
			glVertex2d(vx + vArray[offsetTemp].x,vy + vArray[offsetTemp].y);	//v

			glTexCoord2d(ux + uArray[offsetTemp + arrayW].x,
				uy + uArray[offsetTemp + arrayW].y);	//u
			glVertex2d(vx + vArray[offsetTemp + arrayW].x,
				vy + vArray[offsetTemp + arrayW].y);	//v
		}
		glEnd();
	}

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
#if WITH_XSPRITE_EX
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
#endif
	glDisable(GL_BLEND);
}
void drawTextureEx(unsigned int tex,const _XVector2 &pos,_XVector2 *vArray,_XVector2 *uArray,int arrayW,int arrayH,bool blend)
{
	glEnable(GL_TEXTURE_2D);
	if(blend)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}else
	{
		glDisable(GL_BLEND);
	}
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(pos.x,pos.y,0);
	//glScalef(0.5f,0.5f,1.0);

	glBindTexture(GL_TEXTURE_2D,tex);
	glColor4f(1.0f,1.0f,1.0f,1.0f);

	for(int j = 0;j < arrayH - 1;++ j)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for(int i = 0;i < arrayW;++ i)
		{	//左上
			int offsetTemp = i + j * arrayW;
			glTexCoord2d(uArray[offsetTemp].x,uArray[offsetTemp].y);	//u
			glVertex2d(vArray[offsetTemp].x,vArray[offsetTemp].y);	//v

			glTexCoord2d(uArray[offsetTemp + arrayW].x,
				uArray[offsetTemp + arrayW].y);	//u
			glVertex2d(vArray[offsetTemp + arrayW].x,
				vArray[offsetTemp + arrayW].y);	//v
		}
		glEnd();
	}

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glDisable(GL_BLEND);
}
void drawTexture(unsigned int tex,const _XVector2 &pos,int texW,int texH,bool blend)
{
	glEnable(GL_TEXTURE_2D);
	if(blend)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}else
	{
		glDisable(GL_BLEND);
	}
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(pos.x,pos.y,0);
	//glScalef(0.5f,0.5f,1.0);

	glBindTexture(GL_TEXTURE_2D,tex);
	glColor4f(1.0f,1.0f,1.0f,1.0f);

	glBegin(GL_QUADS);
		glTexCoord2d(0,0);	//u
		glVertex2d(0,0);	//v
		glTexCoord2d(1,0);	//u
		glVertex2d(texW,0);	//v
		glTexCoord2d(1,1);	//u
		glVertex2d(texW,texH);	//v
		glTexCoord2d(0,1);	//u
		glVertex2d(0,texH);	//v
	glEnd();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glDisable(GL_BLEND);
}
GLuint emptyTexture(int x,int y,_XColorMode mode)
{
	GLuint txtnumber;

	glGenTextures(1, &txtnumber);
	glBindTexture(GL_TEXTURE_2D, txtnumber);					
	//效果好
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#if WITH_XSPRITE_EX
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#endif
	//速度快
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	switch(mode)
	{
	case COLOR_RGBA:glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,x,y,0,GL_RGBA,GL_UNSIGNED_BYTE,0);break;
	case COLOR_RGB:glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,x,y,0,GL_RGB,GL_UNSIGNED_BYTE,0);break;
//	case COLOR_GRAY:glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,x,y,0,GL_RGB,GL_UNSIGNED_BYTE,0);break;	//感觉存在问题
	case COLOR_GRAY:glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,x,y,0,GL_LUMINANCE,GL_UNSIGNED_BYTE,0);break;	//修改
	case COLOR_RGBA_F:glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA32F,x,y,0,GL_RGBA,GL_FLOAT,0);break;
//	case COLOR_GRAY_F:glTexImage2D(GL_TEXTURE_2D,0,GL_RGB32F,x,y,0,GL_RGB,GL_FLOAT,0);break;	//感觉存在问题
	case COLOR_GRAY_F:glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,x,y,0,GL_LUMINANCE,GL_FLOAT,0);break;	//修改
	}
	//初始化像素
	//glClearTexImage();	//这个貌似要GL3.0才支持，目前的版本好像不支持

	return txtnumber;
}

struct _XScreenShotData
{
	char fileName[MAX_FILE_NAME_LENGTH];
	int w;
	int h;
	unsigned int *data;
	_XScreenShotData()
		:data(NULL)
	{
	}
};

_XScreenShotData *screenShotData = NULL;

//由于这里使用了多线程，这里需要进行数据互斥锁，尚未加入

#ifdef XEE_OS_WINDOWS
DWORD WINAPI saveDataToPNG(void * pParam)
#endif
#ifdef XEE_OS_LINUX
static void *saveDataToPNG(void * pParam)
#endif
{
	if(screenShotData == NULL || screenShotData->data == NULL) 
	{
#ifdef XEE_OS_WINDOWS
		return 1;
#endif
#ifdef XEE_OS_LINUX
		return;
#endif
	}
	//将数据保存成图片
	unsigned char *pData = createArrayMem<unsigned char>(screenShotData->w * screenShotData->h * 4);
	int w = screenShotData->w << 2;
	unsigned char *pd = pData + screenShotData->w * screenShotData->h * 4 - w;
	unsigned char *ps = (unsigned char *)screenShotData->data;
	for(int i = 0;i < screenShotData->h;++ i)	//上下翻转
	{
		memcpy(pd,ps,w);
		ps += w;
		pd -= w;
	}
//	printf("%d\n",getCurrentTicks());
	savePNG(screenShotData->fileName,pData,screenShotData->w,screenShotData->h,COLOR_RGBA,Z_BEST_COMPRESSION);
//	printf("%d\n",getCurrentTicks());
	//释放资源
	XDELETE_ARRAY(pData);
	XDELETE_ARRAY(screenShotData->data);
	XDELETE(screenShotData);
#ifdef XEE_OS_WINDOWS
	return 1;
#endif
}
//由于这里使用了多线程，这里需要进行数据互斥锁，尚未加入
_XBool gameShot(const char *fileName,int w,int h,bool withThread,float x,float y)
{
	if(fileName == NULL) return XFalse;
	if(x < 0.0f || y < 0.0f) return XFalse;
	if(w <= 0 || h <= 0) return XFalse;
	if(screenShotData != NULL) return XFalse;
	screenShotData = createMem<_XScreenShotData>();
	if(screenShotData == NULL) return XFalse;

	screenShotData->h = h;
	screenShotData->w = w;
	strcpy(screenShotData->fileName,fileName);
	screenShotData->data = createArrayMem<unsigned int>(w * h);
	if(screenShotData->data == NULL)
	{
		XDELETE(screenShotData);
		return XFalse;
	}

	//读取数据
	//glReadPixels(0,0,w,h,GL_RGBA,GL_BYTE,screenShotData->data);
	glReadPixels(x,y,w,h,GL_RGBA,GL_UNSIGNED_BYTE,screenShotData->data);
	if(withThread)
	{//优化之后直接开启一个线程来保存图片
#ifdef XEE_OS_WINDOWS
		DWORD tempThreadP;
		if(CreateThread(0,0,saveDataToPNG,NULL,0,&tempThreadP) == 0)
		{
			return XFalse;
		}
#endif
#ifdef XEE_OS_LINUX
		pthread_t tempThreadP;
			if(pthread_create(&tempThreadP, NULL, &(saveDataToPNG), NULL) != 0)
			{
			return XFalse;
		}
#endif
	}else
	{
		saveDataToPNG(NULL);
	}
	return XTrue;
}
#include "XShaderGLSL.h"
void _XTexture::updateTexture(const _XPBO &pbo)		//使用指定数据更新贴图
{
	pbo.bind();
	updateTexture(NULL);
	//glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB,0);
}
void _XTexture::updateTextureR2B(const _XPBO &pbo)		//使用指定数据更新贴图
{
	pbo.bind();
	updateTextureR2B(NULL);
	//glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB,0);
}
void _XTexture::updateTexture(const void *data,int w,int h,int x,int y)
{
	if(!m_isLoaded) return;
	//if(data == NULL) return;	//PBO的时候可以为 NULL
	if(w <= 0) w = m_w;
	if(h <= 0) h = m_h;
	if(glIsTexture(m_texture))
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,m_texture);
		switch(m_mode)
		{
		case COLOR_RGBA:
			glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_RGBA,GL_UNSIGNED_BYTE,data);
			break;
		case COLOR_RGB:
			glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_RGB,GL_UNSIGNED_BYTE,data);
			break;
		case COLOR_GRAY:
			glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_LUMINANCE,GL_UNSIGNED_BYTE,data);
			break;
		case COLOR_RGBA_F:
			glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_RGBA,GL_FLOAT,data);
			break;
		case COLOR_GRAY_F:
			glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_LUMINANCE,GL_FLOAT,data);
			break;
		}

		glDisable(GL_TEXTURE_2D);
	}
}
void _XTexture::updateTextureR2B(const void *data,int w,int h,int x,int y)
{
	if(!m_isLoaded) return;
	//if(data == NULL) return; //PBO时为null
	if(w <= 0) w = m_w;
	if(h <= 0) h = m_h;
	if(glIsTexture(m_texture))
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,m_texture);
		switch(m_mode)
		{
		case COLOR_RGBA:
			glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_BGRA,GL_UNSIGNED_BYTE,data);
			break;
		case COLOR_RGB:
			glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_BGR,GL_UNSIGNED_BYTE,data);
			break;
		case COLOR_GRAY:
			glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_LUMINANCE,GL_UNSIGNED_BYTE,data);
			break;
		case COLOR_RGBA_F:
			glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_RGBA,GL_FLOAT,data);
			break;
		case COLOR_GRAY_F:
			glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_LUMINANCE,GL_FLOAT,data);
			break;
		}

		glDisable(GL_TEXTURE_2D);
	}
}
//void _XTexture::updateTextureGray(const unsigned char *data)
//{
//	if(!m_isLoaded) return;
//	if(data == NULL) return;
//	if(glIsTexture(m_texture))
//	{
//		glEnable(GL_TEXTURE_2D);
//		glBindTexture(GL_TEXTURE_2D,m_texture);
//		if(m_mode == 0) glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_w, m_h, GL_R,GL_UNSIGNED_BYTE,data);
//		else glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_w, m_h, GL_R,GL_UNSIGNED_BYTE,data);
//		glDisable(GL_TEXTURE_2D);
//	}
//}
_XBool _XTexture::createTexture(int w,int h,_XColorMode mode)
{
	if(m_isLoaded) return XTrue;	//如果资源已经载入了，则不能重复载入
	if(w <= 0) return XFalse;
	if(h <= 0) return XFalse;
	try
	{
#if TEX_DEBUG1
		CPSum ++;
		printf("%d-\n",CPSum);
#endif
		m_cp = new _XSCounter;
		if(m_cp == NULL) return XFalse;
	}catch(...)
	{
		return XFalse;
	}
//	m_fileName = NULL;	//没有文件名
	m_mode = mode;

	m_w = w;
	m_h = h;
	m_texture = emptyTexture(m_w,m_h,m_mode);

	m_isLoaded = XTrue;
	return XTrue;
}
_XBool _XTexture::createWithTexture(int w,int h,_XColorMode mode,unsigned int tex)
{
	if(m_isLoaded) return XTrue;	//如果资源已经载入了，则不能重复载入
	if(w <= 0) return XFalse;
	if(h <= 0) return XFalse;
	try
	{
#if TEX_DEBUG1
		CPSum ++;
		printf("%d-\n",CPSum);
#endif
		m_cp = new _XSCounter;
		if(m_cp == NULL) return XFalse;
	}catch(...)
	{
		return XFalse;
	}
//	m_fileName = NULL;	//没有文件名
	m_mode = mode;

	m_w = w;
	m_h = h;
	m_texture = tex;

	m_isLoaded = XTrue;
	return XTrue;
}
#include "XResourceManager.h"
_XBool _XTexture::load(const char *filename,_XResourcePosition resoursePosition)//从指定文件中载入图片资源
{
	if(m_isLoaded) return XTrue;	//如果资源已经载入了，则不能重复载入
	try
	{
#if TEX_DEBUG1
		CPSum ++;
		printf("%d-\n",CPSum);
#endif
		m_cp = new _XSCounter;
		if(m_cp == NULL) return XFalse;
	}catch(...)
	{
		return XFalse;
	}
//	m_fileName = createArrayMem<char>(MAX_FILE_NAME_LENGTH);
//	if(m_fileName == NULL)
//	{
//		XDELETE(m_cp);
//		return XFalse;
//	}
//	strcpy(m_fileName,filename);
	m_resInfo = _XResourceManager::GetInstance().loadResource(filename,RESOURCE_TYPE_TEXTURE,resoursePosition);
	if(m_resInfo != NULL)
	{
		_XResourceTex *tempResTex = (_XResourceTex *)m_resInfo->m_pointer;
		m_texture = tempResTex->m_texID;
		m_w = tempResTex->m_width;
		m_h = tempResTex->m_height;
	//if(TextureLoadEx(m_texture,filename,&m_w,&m_h,resoursePosition) != 0)
	//{
	//	printf("%d\n",m_texture);
#if TEX_DEBUG
		++ XTSum;
		printf("%d:%d+%s\n",XTSum,m_texture,filename);
#endif
		m_isLoaded = XTrue;
		return XTrue;
	}
	XDELETE(m_cp);
//	XDELETE_ARRAY(m_fileName);
	m_isLoaded = XFalse;
	return XFalse;
}
void _XTexture::release()	//释放图片资源
{
	if(!m_isLoaded) return;
#if TEX_DEBUG
	-- XTSum;
	printf("%d-%d\n",XTSum,m_texture);
#endif
	//经过资源管理器之后其自身不能释放资源
	if(m_resInfo == NULL)
	{
		if(glIsTexture(m_texture)) 
		{
			printf("delete texture:%d\n",m_texture);
			glDeleteTextures(1, &m_texture);
		}
	}else
	{
		if(_XResourceManager::GetInstance().releaseResource(m_resInfo))
			m_resInfo = NULL;
	}
	m_isLoaded = XFalse;
}
_XTexture::_XTexture(const _XTexture& temp)	//拷贝构造函数
{
	if(this == &temp) return;		//防止自生拷贝
	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	m_texture = temp.m_texture;
	m_w = temp.m_w;
	m_h = temp.m_h;
	m_isLoaded = temp.m_isLoaded;
	m_cp = temp.m_cp;
//		m_fileName = temp.m_fileName;

	m_resInfo = _XResourceMng.copyResource(temp.m_resInfo);
	return;	
}
_XTexture& _XTexture::operator = (const _XTexture &temp)
{
	if(this == &temp) return *this;		//防止自生拷贝
	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{
		release();	//引用计数结束，释放资源
#if TEX_DEBUG1
		CPSum --;
		printf("%d-\n",CPSum);
#endif
		XDELETE(m_cp);
//		XDELETE_ARRAY(m_fileName);
	}
	m_texture = temp.m_texture;
	m_w = temp.m_w;
	m_h = temp.m_h;
	m_isLoaded = temp.m_isLoaded;
	m_cp = temp.m_cp;
//	m_fileName = temp.m_fileName;
	m_resInfo = _XResourceMng.copyResource(temp.m_resInfo);
	return *this;	
}