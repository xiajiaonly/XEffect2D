//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "glew.h"
//#include <gl/GLAux.h>
#include "stdio.h"
#include "math.h"

#include "SDL.h"
#include "SDL_image.h"
#include "XBasicSDL.h"

#include "XBasicOpenGL.h"
#include "XBasicFun.h"

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
    char *infoLog;

    glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if(infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
		AddLogInfoNull("%s\n",infoLog);
        free(infoLog);
    }
}

void printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if(infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
		AddLogInfoNull("%s\n",infoLog);
        free(infoLog);
    }
}

int printOglError(char *file, int line)
{
    GLenum glErr;
    int    retCode = 0;

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

char *textFileRead(const char *filename,int resoursePosition) 
{
	if(resoursePosition == RESOURCE_INSIDE)
	{
		int lengthTemp = _XResourcePack::GetInstance().getFileLength(filename);
		char *content = NULL;
		content = (char *)malloc(sizeof(char) * (lengthTemp + 1));
		_XResourcePack::GetInstance().unpackResource(filename,(unsigned char *)content);
		content[lengthTemp] = '\0';
		return content;
	}else
	{
		FILE *fp;
		char *content = NULL;
		int count = 0;
		if(filename != NULL) 
		{
			if((fp = fopen(filename,"rt")) != NULL) 
			{
				fseek(fp, 0, SEEK_END);
				count = ftell(fp);
				rewind(fp);

				if(count > 0) 
				{
					content = (char *)malloc(sizeof(char) * (count + 1));
					count = fread(content,sizeof(char),count,fp);
					content[count] = '\0';
				}
				fclose(fp);
			}
		}
		return content;
	}
}

int textFileWrite(const char *filename,const char *s) 
{
	FILE *fp;
	int status = 0;
	if(filename == NULL) return 0;
	if(s == NULL) return 0;

	if((fp = fopen(filename,"w")) != NULL) 
	{	
		if(fwrite(s,sizeof(char),strlen(s),fp) == strlen(s)) status = 1;
		fclose(fp);
	}
	return(status);
}

void setShader(const char* vertFile,const char* fragFile,int &pHandle,int resoursePosition)
{
    char *vs = NULL;
	char *fs = NULL;
	GLuint v,f;

	v = glCreateShader(GL_VERTEX_SHADER);
    f = glCreateShader(GL_FRAGMENT_SHADER);

	vs = textFileRead(vertFile,resoursePosition);
    fs = textFileRead(fragFile,resoursePosition);

	const char * vv = vs;
    const char * ff = fs;

	glShaderSource(v, 1, &vv,NULL);
    glShaderSource(f, 1, &ff,NULL);

	free(vs);
	free(fs);

	glCompileShader(v);
    glCompileShader(f);

	printShaderInfoLog(v);
    printShaderInfoLog(f);

    pHandle = glCreateProgram();
	glAttachShader(pHandle,v);
    glAttachShader(pHandle,f);

    glLinkProgram(pHandle);
    printProgramInfoLog(pHandle);
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

int SDL_Texture(GLuint &texture,const char *filename,int withAlpha,int *w,int *h,int resoursePosition)
{
	if(filename == NULL || strlen(filename) <= 0) return 0;
	if(withAlpha != 0) withAlpha = 1;

	SDL_Surface *temp = NULL;
	SDL_Surface *image = NULL;

	Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000; gmask = 0x00ff0000; bmask = 0x0000ff00;	amask = 0x000000ff;
#else
	rmask = 0x000000ff;	gmask = 0x0000ff00;	bmask = 0x00ff0000;	amask = 0xff000000;
#endif
	
	if((temp = (SDL_Surface *) loadImage(filename,withAlpha,resoursePosition)) != NULL) 
	{
		if((temp->w > XEE_MaxTetureSize) || (temp->h > XEE_MaxTetureSize)) 
		{
			fprintf(stderr, "Image size exceeds max texture size, which is %d pixels for each side\n",XEE_MaxTetureSize);
			AddLogInfoNull("Image size exceeds max texture size, which is %d pixels for each side\n",XEE_MaxTetureSize);
			SDL_FreeSurface(temp);
			temp = NULL;
			return 0;
			//exit(-1);
		}
		//printf("Texture Max Size:%d x %d\n",max_size,max_size);

		if(w != NULL && h != NULL)
		{
			(* w) = temp->w;
			(* h) = temp->h;
		}

		// Check for NPOT textures 
		if (isNPOT(temp->w, temp->h)) //检查图片的尺寸是否符合2的n次方
		{//如果不符合2的n次方的图片下面进行特殊的处理(这个处理目前还存在问题,对于不符合2的n次方的透明通道贴图)
			GLint wreal = 0, hreal = 0;
			GLint xpad = 0, ypad = 0;
			
			wreal = (int)powf(2.0, ceilf(logf((float)temp->w)/logf(2.0f)));
			hreal = (int)powf(2.0, ceilf(logf((float)temp->h)/logf(2.0f)));
			xpad = (wreal - temp->w)/2;
			ypad = (hreal - temp->h)/2;

			if(withAlpha != 0)
			{
				image = SDL_CreateRGBSurface(SDL_SWSURFACE, wreal, hreal, 32, rmask, gmask, bmask, amask);
			}else
			{
				image = SDL_CreateRGBSurface(SDL_SWSURFACE, wreal, hreal, 24, rmask, gmask, bmask, 0);
			}
			if(image == NULL)
			{
				SDL_FreeSurface(temp);
				temp = NULL;
				return 0;
			}
			SDL_Rect dst = {xpad, ypad, temp->w, temp->h};

			SDL_BlitSurface(temp, 0, image, &dst);
			SDL_FreeSurface(temp);
			temp = NULL;

			// OpenGL Texture creation	
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

		//	if(withAlpha != 0)
			if(image->format->BitsPerPixel == 32)
			{
			//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA4, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
			}else
			if(image->format->BitsPerPixel == 24)
			{
			//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB4, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
			}

			SDL_FreeSurface(image);
			image = NULL;
		}else
		{//符合2的n次方的图片
			// OpenGL Texture creation	
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
		//	glPixelStorei(GL_UNPACK_ALIGNMENT,1);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

			if(temp->format->BitsPerPixel == 32)
			{
			//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA4, temp->w, temp->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, temp->pixels);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, temp->w, temp->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, temp->pixels);
			}else
			if(temp->format->BitsPerPixel == 24)
			{
			//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB4, temp->w, temp->h, 0, GL_BGR, GL_UNSIGNED_BYTE, temp->pixels);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, temp->w, temp->h, 0, GL_BGR, GL_UNSIGNED_BYTE, temp->pixels);
			}

			SDL_FreeSurface(temp);
			temp = NULL;
		}
		
	/*	// OpenGL Texture creation	
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

		if(withAlpha != 0)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, image->w, image->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, image->pixels);
		}else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
		}

		SDL_FreeSurface(image);
		image = NULL;*/
		return 1;
	}else
	{
		return 0;
	}
	return 1;
}

//测试发现这个函数中的SDL_FreeSurface会存在内存IPR和FIM错误，具体需要SDL的代码实现来检测
int SDL_TextureEx(GLuint &texture,const char *filename,int *w,int *h,int resoursePosition)
{
	if(filename == NULL || strlen(filename) <= 0) return 0;
	SDL_Surface *temp = NULL;
	SDL_Surface *image = NULL;

	Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000; gmask = 0x00ff0000; bmask = 0x0000ff00;	amask = 0x000000ff;
#else
	rmask = 0x000000ff;	gmask = 0x0000ff00;	bmask = 0x00ff0000;	amask = 0xff000000;
#endif
	
	if((temp = loadImageEx(filename,resoursePosition)) != NULL) 
	{
		if((temp->w > XEE_MaxTetureSize || temp->h > XEE_MaxTetureSize) && XEE_MaxTetureSize > 0) 	//贴图最大尺寸的检测
		{
			fprintf(stderr, "Image size exceeds max texture size, which is %d pixels for each side\n", XEE_MaxTetureSize);
			AddLogInfoNull("Image size exceeds max texture size, which is %d pixels for each side\n", XEE_MaxTetureSize);
			SDL_FreeSurface(temp);
			temp = NULL;
			return 0;
			//exit(-1);
		}
		//printf("Texture Max Size:%d x %d\n",max_size,max_size);

		if(w != NULL && h != NULL)
		{
			(* w) = temp->w;
			(* h) = temp->h;
		}

		// Check for NPOT textures 
		if (isNPOT(temp->w, temp->h)) //检查图片的尺寸是否符合2的n次方
		{//如果不符合2的n次方的图片下面进行特殊的处理(这个处理目前还存在问题,对于不符合2的n次方的透明通道贴图)
			GLint wreal = 0, hreal = 0;
			GLint xpad = 0, ypad = 0;
			
			wreal = (int)powf(2.0, ceilf(logf((float)temp->w)/logf(2.0f)));
			hreal = (int)powf(2.0, ceilf(logf((float)temp->h)/logf(2.0f)));
			xpad = (wreal - temp->w)/2;
			ypad = (hreal - temp->h)/2;

			if(temp->format->BitsPerPixel == 32)
			{
				image = SDL_CreateRGBSurface(SDL_SWSURFACE, wreal, hreal, 32, rmask, gmask, bmask, amask);
				printf("It will take some mistake by use this texture format!\n");
			}else
			if(temp->format->BitsPerPixel == 24)
			{
				image = SDL_CreateRGBSurface(SDL_SWSURFACE, wreal, hreal, 24, rmask, gmask, bmask, 0);
			}
			if(image == NULL) 
			{
				SDL_FreeSurface(temp);
				temp = NULL;
				return 0;
			}
			
			SDL_Rect dst = {xpad, ypad, temp->w, temp->h};

			SDL_BlitSurface(temp, 0, image, &dst);
			SDL_FreeSurface(temp);
			temp = NULL;

			// OpenGL Texture creation	
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

			if(image->format->BitsPerPixel == 32)
			{
			//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA4, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
			}else
			if(image->format->BitsPerPixel == 24)
			{
			//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB4, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
			}

			SDL_FreeSurface(image);
			image = NULL;
		}else
		{//符合2的n次方的图片
			// OpenGL Texture creation	
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
		//	glPixelStorei(GL_UNPACK_ALIGNMENT,1);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

			if(temp->format->BitsPerPixel == 32)
			{
			//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA4, temp->w, temp->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, temp->pixels);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, temp->w, temp->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, temp->pixels);
			}else
			if(temp->format->BitsPerPixel == 24)
			{
			//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB4, temp->w, temp->h, 0, GL_BGR, GL_UNSIGNED_BYTE, temp->pixels);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, temp->w, temp->h, 0, GL_BGR, GL_UNSIGNED_BYTE, temp->pixels);
			}

			SDL_FreeSurface(temp);
			temp = NULL;
		}
		return 1;
	}else
	{
		return 0;
	}
	return 1;
}

int SDL_TextureQuarter(GLuint &texture,const char *filename)
{
	Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000; gmask = 0x00ff0000; bmask = 0x0000ff00;	amask = 0x000000ff;
#else
	rmask = 0x000000ff;	gmask = 0x0000ff00;	bmask = 0x00ff0000;	amask = 0xff000000;
#endif	
	SDL_Surface *tempSur = loadImageEx(filename);
	if(tempSur == NULL) return 0;
	SDL_Surface *tempSur1 = SDL_CreateRGBSurface(0,tempSur->w >> 2,tempSur->h >> 2,32,rmask,gmask,bmask,amask);
	if(tempSur1 == NULL) return 0;
	if(tempSur->format->BitsPerPixel == 32)
	{
		int *p = (int *)tempSur->pixels;
		int *p1 = (int *)tempSur1->pixels;
		for(int i = 0; i < tempSur->h;i += 4)
			for(int j = 0;j < tempSur->w;j += 4)
				*(p1 + (i >> 2) * tempSur1->w + (j >> 2)) = *(p + i * tempSur->w + j);
	}else
	if(tempSur->format->BitsPerPixel == 24)
	{
		unsigned char *p = (unsigned char *)tempSur->pixels;
		unsigned char *p1 = (unsigned char *)tempSur1->pixels;
		for(int i = 0; i < tempSur->w;i += 4)
			for(int j = 0;j < tempSur->h;j += 4)
			{
				*(p1 + i + j * tempSur1->w + 0) = *(p + (i << 2) + (j << 2) * tempSur->w + 0);
				*(p1 + i + j * tempSur1->w + 1) = *(p + (i << 2) + (j << 2) * tempSur->w + 1);
				*(p1 + i + j * tempSur1->w + 2) = *(p + (i << 2) + (j << 2) * tempSur->w + 2);
				*(p1 + i + j * tempSur1->w + 3) = 255;
			}
	}
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, tempSur1->pixels);
	SDL_FreeSurface(tempSur);
	SDL_FreeSurface(tempSur1);
	return 1;
}

char XEE_isOpenGL2Support = 0;
int XEE_MaxTetureSize = 1024;	
int initOpenGL(int width,int height)
{
	glShadeModel(GL_SMOOTH);						// Enable Smooth Shading
	glMatrixMode(GL_PROJECTION);					//设置当前矩阵模式 （对投影矩阵应用之后的矩阵操作）
	glLoadIdentity();								//变换坐标系函数
	glOrtho(0, width, height, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);			//清除颜色
	//glDisable(GL_DEPTH);							//2D项目不需要深度测试
	glDisable(GL_DEPTH_TEST);						//2D项目不需要深度测试
	if(glewInit() != 0)
	{//初始化失败
		AddLogInfoStr("Glew init error!\n");
		XEE_isOpenGL2Support = 0;
	}else
	{
		//记录显卡的最大贴图尺寸
		glGetIntegerv(GL_MAX_TEXTURE_SIZE,&XEE_MaxTetureSize);	
		//检查硬件是否支持OpenGL2.0
		if(glewIsSupported("GL_VERSION_2_0"))
		{
			AddLogInfoStr("Ready for OpenGL 2.0\n");
			XEE_isOpenGL2Support = 1;
		}else
		{
			AddLogInfoStr("OpenGL 2.0 not supported\n");
			XEE_isOpenGL2Support = 0;
		}
	}
	return 1;
}

void drawLine(int ax, int ay, int bx, int by,int width, float r, float g, float b, float a,int type)
{
    glDisable(GL_TEXTURE_2D);
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
void drawRect(const _XRect& rect,int width,float r,float g,float b,float a,int type)
{
	drawLine(rect.left,rect.top,rect.right,rect.top,width,r,g,b,a,type);
	drawLine(rect.right,rect.top,rect.right,rect.bottom,width,r,g,b,a,type);
	drawLine(rect.right,rect.bottom,rect.left,rect.bottom,width,r,g,b,a,type);
	drawLine(rect.left,rect.bottom,rect.left,rect.top,width,r,g,b,a,type);
}
void drawBox(float x,float y,float sizeX,float sizeY)
{
	drawLine(x - sizeX,y + sizeY,x - sizeX,y - sizeY);
	drawLine(x - sizeX,y + sizeY,x + sizeX,y + sizeY);
	drawLine(x + sizeX,y + sizeY,x + sizeX,y - sizeY);
	drawLine(x - sizeX,y - sizeY,x + sizeX,y - sizeY);
}
void drawBox(float x,float y,float sizeX,float sizeY,float angle)
{
	_XVector2 P1(x - (sizeX * cos(angle) - sizeY * sin(angle)),y + (sizeX * sin(angle) + sizeY * cos(angle)));
	_XVector2 P2(x - (sizeX * cos(angle) + sizeY * sin(angle)),y + (sizeX * sin(angle) - sizeY * cos(angle)));

	_XVector2 P3(x + (sizeX * cos(angle) + sizeY * sin(angle)),y - (sizeX * sin(angle) - sizeY * cos(angle)));
	_XVector2 P4(x + (sizeX * cos(angle) - sizeY * sin(angle)),y - (sizeX * sin(angle) + sizeY * cos(angle)));
	drawLine(P1,P2);
	drawLine(P2,P4);
	drawLine(P3,P4);
	drawLine(P3,P1);
}

//建立一张空白贴图
GLuint EmptyTexture(int x,int y)
{
	GLuint txtnumber;
	//unsigned int* data;	

	//try
	//{
	//	data = new unsigned int[x * y];
	//	if(data == NULL) return NULL;
	//}catch(...)
	//{
	//	return NULL;
	//}

	//memset(data,0,(x * y * sizeof(unsigned int)));

	glGenTextures(1, &txtnumber);
	glBindTexture(GL_TEXTURE_2D, txtnumber);					
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	//glTexImage2D(GL_TEXTURE_2D, 0, 4, x, y, 0,GL_RGBA, GL_UNSIGNED_BYTE, data);						
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0,GL_RGBA, GL_UNSIGNED_BYTE,0);						

//	XDELETE_ARRAY(data);

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

#ifdef OS_WINDOWS
DWORD WINAPI saveDataToPNG(void * pParam)
#endif
#ifdef OS_LINUX
static void *saveDataToPNG(void * pParam)
#endif
{
	//将数据保存成图片
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	SDL_Surface * picArm = SDL_CreateRGBSurface(SDL_SWSURFACE,screenShotData->w,screenShotData->h,32,0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
#else
	SDL_Surface * picArm = SDL_CreateRGBSurface(SDL_SWSURFACE,screenShotData->w,screenShotData->h,32,0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
#endif
	unsigned int *pData1 = (unsigned int *)picArm->pixels;
	int tempData1;
	int tempData2;
	if(screenShotData == NULL || screenShotData->data == NULL) 
	{
#ifdef OS_WINDOWS
		return 1;
#endif
#ifdef OS_LINUX
		return;
#endif
	}
	for(int i = 0;i < screenShotData->h;++ i)
	{
		tempData1 = (screenShotData->h - i - 1) * screenShotData->w;
		tempData2 = i * screenShotData->w;
		for(int j = 0;j < screenShotData->w;++ j)
		{
			*(pData1 + tempData1 + j) = (*(screenShotData->data + tempData2 + j)) << 1;
		}
	}
	printf("%d\n",SDL_GetTicks());
	savePNG(screenShotData->fileName,picArm,Z_BEST_COMPRESSION);
	printf("%d\n",SDL_GetTicks());
	//释放资源
	SDL_FreeSurface(picArm);
	picArm = NULL;
	XDELETE_ARRAY(screenShotData->data);
	XDELETE(screenShotData);
#ifdef OS_WINDOWS
	return 1;
#endif
}
//由于这里使用了多线程，这里需要进行数据互斥锁，尚未加入
int gameShot(const char *fileName,int w,int h)
{
	if(fileName == NULL) return 0;
	if(w <= 0 || h <= 0) return 0;
	if(screenShotData != NULL) return 0;
	try
	{
		screenShotData = new _XScreenShotData;
		if(screenShotData == NULL) return 0;
	}catch(...)
	{
		return 0;
	}
	screenShotData->h = h;
	screenShotData->w = w;
	strcpy(screenShotData->fileName,fileName);
	try
	{
		screenShotData->data = new unsigned int[w * h];
		if(screenShotData->data == NULL)
		{
			XDELETE(screenShotData);
			return 0;
		}
	}catch(...)
	{
		XDELETE(screenShotData);
		return 0;
	}
	//读取数据
	glReadPixels(0,0,w,h,GL_RGBA,GL_BYTE,screenShotData->data);

	//优化之后直接开启一个线程来保存图片
#ifdef OS_WINDOWS
	DWORD tempThreadP;
	if(CreateThread(0,0,saveDataToPNG,NULL,0,&tempThreadP) == 0)
	{
		return 0;
	}
#endif
#ifdef OS_LINUX
	pthread_t tempThreadP;
        if(pthread_create(&tempThreadP, NULL, &(saveDataToPNG), NULL) != 0)
        {
		return 0;
	}
#endif
	return 1;
}

#if WITH_OBJECT_MANAGER
#include "XObjectManager.h"
#endif
#include "XControl/XControlManager.h"

void clearScreen()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void updateScreen()
{
	_XControlManager::GetInstance().draw();
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().draw();
#endif
	
//	glFlush();
	SDL_GL_SwapBuffers();

#ifdef GET_ALL_PIXEL
	static int maxPixel = 0;
	if(XEE_AllPixel > maxPixel)
	{
		maxPixel = XEE_AllPixel;
		printf("All pixel is %d\n",XEE_AllPixel);
	}
#endif

}

//_XTexture::_XTexture(GLuint tex,int w,int h)
//:m_texture(tex)
//,m_isLoaded(0)
//,m_w(w)
//,m_h(h)
//{
//}

int isFBOSupported()
{
	//检查FBO是否被显卡支持
	if(glewIsSupported("GL_EXT_framebuffer_object"))
    {
		printf("FBO is supported.\n");
		return 1;
    }else
	{
        printf("FBO is not supported.");
		return 0;
	}
	return 0;
}

int isFramebufferReady()
{
    //检查FBO状态
    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    switch(status)
    {
    case GL_FRAMEBUFFER_COMPLETE_EXT:
        std::cout << "Framebuffer complete." << std::endl;
        return 1;

    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
        std::cout << "[ERROR] Framebuffer incomplete: Attachment is NOT complete." << std::endl;
        return 0;

    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
        std::cout << "[ERROR] Framebuffer incomplete: No image is attached to FBO." << std::endl;
        return 0;

    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
        std::cout << "[ERROR] Framebuffer incomplete: Attached images have different dimensions." << std::endl;
        return 0;

    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
        std::cout << "[ERROR] Framebuffer incomplete: Color attached images have different internal formats." << std::endl;
        return 0;

    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
        std::cout << "[ERROR] Framebuffer incomplete: Draw buffer." << std::endl;
        return 0;

    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
        std::cout << "[ERROR] Framebuffer incomplete: Read buffer." << std::endl;
        return 0;

    case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
        std::cout << "[ERROR] Unsupported by FBO implementation." << std::endl;
        return 0;

    default:
        std::cout << "[ERROR] Unknow error." << std::endl;
        return 0;
    }
}

int _XFBO::init(int w,int h)
{
	if(m_nowFBOTextureSum != 0) return 1;
	//检查是否支持FBO
	if(isFBOSupported() == 0) 
	{
		printf("FBO is not supported!\n");
		return 0;
	}
	//建立贴图
	addOneTexture(w,h);
	//建立一个framebuffer object，当退出时需要删除
    glGenFramebuffersEXT(1, &m_fboId);					//建立FBO
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fboId);	//绑定FBO
	//使用浮点的GL_RGBA32F_ARB，可以避免残影不消退的问题，尚未搞好
	//glRenderbufferStorageMultisample(GL_RENDERBUFFER,GL_RGBA32F_ARB,GL_DEPTH_STENCIL,w,h);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,m_fboId);

	//建立深度渲染buff，不建立对于不需要深度计算的应用貌似也没有问题
//	glGenRenderbuffersEXT(1, &m_rboId);					
//	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_rboId);
//	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, w, h);
//	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

    //挂接深度渲染点
//	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, rboId);
    //挂接贴图
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_textureId[0], 0);

    //如果你没有挂接贴图的话就取消颜色缓存
    //例如当你只需要对贴图进行深度渲染的时候
    //否则glCheckFramebufferStatusEXT将不会准备就绪.
    //glDrawBuffer(GL_NONE);
    //glReadBuffer(GL_NONE);

	if(isFramebufferReady() == 0) return 0;

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	return 1;
}

void _XFBO::useFBO(int windowsW,int windowsH)
{
	glMatrixMode(GL_PROJECTION);					//设置当前矩阵模式 （对投影矩阵应用之后的矩阵操作）
	glPushMatrix();
	glLoadIdentity();								//变换坐标系函数
	glOrtho(0,windowsW,0,windowsH, -1, 1);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,m_fboId);
}

void _XFBO::attachTex(int order)
{
	if(m_nowFBOTextureSum > 1 && order >= 0 && order < m_nowFBOTextureSum)
	{//挂接贴图
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_textureId[order], 0);
	}
}

void _XFBO::removeFBO()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // unbind
	glMatrixMode(GL_PROJECTION);					//设置当前矩阵模式 （对投影矩阵应用之后的矩阵操作）
	glPopMatrix();
}

void _XFBO::addOneTexture(int w,int h)
{
	glGenTextures(1, &m_textureId[m_nowFBOTextureSum]);
	glBindTexture(GL_TEXTURE_2D, m_textureId[m_nowFBOTextureSum]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	m_nowFBOTextureSum ++;
}

void _XFBO::release()
{
	if(m_nowFBOTextureSum == 0) return;
	for(int i = 0;i < m_nowFBOTextureSum;++ i)
	{
		if(glIsTexture(m_textureId[i])) glDeleteTextures(1, &m_textureId[i]);
	}

	glDeleteFramebuffersEXT(1, &m_fboId);
	//glDeleteRenderbuffersEXT(1, &m_rboId);
	m_nowFBOTextureSum = 0;
}
