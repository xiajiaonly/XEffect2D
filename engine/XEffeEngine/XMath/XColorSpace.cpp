#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XColorSpace.h"
#include "XMath.h"
namespace XE{
#define max3v(a,b,c)    ((a > ((b>c)?b:c)) ? a : ((b>c)?b:c))
#define min3v(a,b,c)    ((a < ((b<c)?b:c)) ? a : ((b<c)?b:c))
#define HSLMAX         240
#define RGBMAX         255
#define UNDEFINED    (HSLMAX*2/3)
namespace XColorSpace
{
	XFColor getWheelColor(float angle)
	{ 
		angle = XMath::angleReduce(angle);
		if(angle <= 120)
		{
			angle = angle / 120.0f;
			return XFColor(1.0f - angle,angle,0.0f,1.0f);
			//return XMath::lineSlerp(XFColor::red,XFColor::green,angle / 120.0f);
		}else
		if(angle <= 240)
		{
			angle = (angle - 120.0f) / 120.0f;
			return XFColor(0.0f,1.0f - angle,angle,1.0f);
			//return XMath::lineSlerp(XFColor::green,XFColor::blue,(angle - 120.0f) / 120.0f);
		}else
		{
			angle = (angle - 240.0f) / 120.0f;
			return XFColor(angle,0.0f,1.0f - angle,1.0f);
			//return XMath::lineSlerp(XFColor::blue,XFColor::red,(angle - 240.0f) / 120.0f);
		}
	}
	void HSV2RGB(unsigned char *color,float H,float S,float V)
	{
		if(S == 0.0f) 
		{// achromatic (grey)
			color[0] = color[1] = color[2] = (unsigned char)(V * 255.0f);
			return;
		}

		int i = floor(H);
		float f = H - i; // factorial part of h
		//if(f < 0.0f) f = 0.0f;
		float p = V * (1 - S);
		float q = V * (1 - S * f);
		float t = V * (1 - S * (1 - f));
		float R,G,B;
		switch(i) 
		{
		case 0: R = V;G = t;B = p;break;
		case 1: R = q;G = V;B = p;break;
		case 2: R = p;G = V;B = t;break;
		case 3: R = p;G = q;B = V;break;
		case 4: R = t;G = p;B = V;break;
		default: R = V;G = p;B = q;break;
		}
		color[0] = (unsigned char)(R * 255.0f);
		color[1] = (unsigned char)(G * 255.0f);
		color[2] = (unsigned char)(B * 255.0f);
	/*	unsigned char R,G,B;
		double    var_h  = 0.0;
		int        var_i  = 0;
		double    var_1 = 0.0,var_2 = 0.0,var_3 = 0.0;
		double    var_r = 0.0,var_g = 0.0,var_b = 0.0;
		if ( 0 < S && S < 0.0000001 )        //HSV from 0 to 1
		{
			R = V * 255.0;
			G = V * 255.0;
			B = V * 255.0;
		}else
		{
			var_h = H;
			if(var_h == 6.0 ) var_h = 0.0;      //H must be < 1
			var_i = int( var_h );           //Or ... var_i = floor( var_h )
			var_1 = V * ( 1.0 - S );
			var_2 = V * ( 1.0 - S * ( var_h - var_i ) );
			var_3 = V * ( 1.0 - S * ( 1.0 - ( var_h - var_i ) ) );
 
			if( var_i == 0 ) { var_r = V     ; var_g = var_3 ; var_b = var_1;    }
			else if ( var_i == 1 ) { var_r = var_2 ; var_g = V; var_b = var_1;    }
			else if ( var_i == 2 ) { var_r = var_1 ; var_g = V; var_b = var_3;    }
			else if ( var_i == 3 ) { var_r = var_1 ; var_g = var_2; var_b = V;    }
			else if ( var_i == 4 ) { var_r = var_3 ; var_g = var_1; var_b = V;    }
			else                   { var_r = V ; var_g = var_1; var_b = var_2;    }
 
			R = var_r * 255.0;                  //RGB results from 0 to 255
			G = var_g * 255.0;
			B = var_b * 255.0;
		}
		color[0] = R;
		color[1] = G;
		color[2] = B;*/
	}
	void RGB2HSV(unsigned char *color,float& H,float& S,float&V)
	{
		//r,g,b values are from 0 to 1
		//h = [0,6], s = [0,1], v = [0,1]
		//if s == 0, then h = -1 (undefined)
		float R = color[0];
		float G = color[1];
		float B = color[2];
		float min = XEE_Min(XEE_Min(R,G),B);
		float max = XEE_Max(XEE_Max(R,G),B);
		V = max / 255.0f;
		float delta = max - min;
		if(delta == 0){H = 0;S = 0;return;}	//gray

		if(max == 0)
		{// r = g = b = 0 // s = 0, v is undefined
			S = 0;
			H = UNDEFINED;
			return;
		}else
		S = delta / max;

		if(R == max) H = (G - B) / delta; else		// between yellow & magenta
		if(G == max) H = 2 + (B - R) / delta; else	// between cyan & yellow
		H = 4 + (R - G) / delta; // between magenta & cyan

		if(H < 0) H += 6;
		if(H > 6) H -= 6;
	}
	float HueToRGB (float n1,float n2,float hue)
	{
		if(hue < 0) hue += HSLMAX;
		if(hue > HSLMAX) hue -= HSLMAX;
 
		if(hue < (HSLMAX / 6)) return (n1 + (((n2 - n1) * hue + (HSLMAX/12)) / (HSLMAX / 6)));
		if(hue < (HSLMAX / 2)) return (n2);
		if(hue < ((HSLMAX * 2) / 3)) return (n1 + (((n2 - n1) * (((HSLMAX * 2) / 3) - hue) + (HSLMAX / 12)) / (HSLMAX / 6)));
		else return (n1);
	}
	namespace XYUVTable{
	#define TABLE_SIZE  256
	int *tableY = NULL;	//此处初始化有无
	int *tableVR = NULL;	//此处初始化有无
	int *tableVG = NULL;	//此处初始化有无
	int *tableUG = NULL;	//此处初始化有无
	int *tableUB = NULL;	//此处初始化有无

	#define ORIGXLAT 1
	inline bool initYUVTable()
	{
		static bool flag = false;
		if(flag) return true;
		tableY = XMem::createArrayMem<int>(TABLE_SIZE * 5);	//注意这个内存并没有释放掉
		if(tableY == NULL) return false;
		tableVR = tableY + TABLE_SIZE;
		tableVG = tableVR + TABLE_SIZE;
		tableUG = tableVG + TABLE_SIZE;
		tableUB = tableUG + TABLE_SIZE;
		for(int i = 0;i < TABLE_SIZE;++ i) 
		{
			tableY[i] = ((i - 16) * 298 + 128) >> 8;
			tableVR[i] = (409 * (i - 128)) >> 8; 
			tableVG[i] = (208 * (i - 128)) >> 8; 
			tableUG[i] = (100 * (i - 128)) >> 8; 
			tableUB[i] = (516 * (i - 128)) >> 8;
		}
		flag = true;	//
		return true;
	}
	}
	#define GET_CR(sy,su,sv) ((298*((sy)-16) + 409*((sv)-128) + 128)>>8)// Get R Value
	#define GET_CG(sy,su,sv) ((298*((sy)-16) - 100*((su)-128) - 208*((sv)-128)+128)>> 8)// Get G Value
	#define GET_CB(sy,su,sv) ((298*((sy)-16) + 516*((su)-128) + 128)>> 8)// Get B Value
	// Check RGB value, if overflow then correct it
	#define CLIP(value)  (value)<0 ? 0 : ((value)>255 ? 255 : (value))
	//YUV转RGB
	void YUV2RGB(unsigned char *color,unsigned char Y,unsigned char U,unsigned char V)
	{
		color[0] = CLIP(GET_CB(Y, U, V));	
		color[1] = CLIP(GET_CG(Y, U, V));
		color[2] = CLIP(GET_CR(Y, U, V));
	}

	#define GET_R(sy,su,sv) ((sy + 409 * sv + 128)>>8)				// Get R Value
	#define GET_G(sy,su,sv) ((sy - 100 * su - 208 * sv + 128)>> 8)	// Get G Value
	#define GET_B(sy,su,sv) ((sy + 516 * su + 128)>> 8)				// Get B Value
	//YUV转RGB
	void YUVToRGB(unsigned char *pYUV, unsigned char *pRGB,int width,int height)
	{
		if (NULL == pYUV || pRGB == NULL) return;
		if(!XYUVTable::initYUVTable()) return;	//初始化失败
		//Get Y/U/V Pointer from YUV frame data
		int tmpW = width * height;
		unsigned char * pYbuf = pYUV;					//Pointer to Y
		unsigned char * pVbuf = pYUV + tmpW;			//Pointer to U
		unsigned char * pVbufT;
		unsigned char * pUbuf = pVbuf + (tmpW >> 2);	//Pointer to V
		unsigned char * pUbufT;

		int k = -1;
		int offset = 0;
		int Y,U,V,i,j;
		tmpW = width >> 1;
		for(i = 0;i < height;++ i)
		{
			offset = (i >> 1) * tmpW;
			//if(i&1) offset += tmpW;
			pUbufT = pUbuf + offset;
			pVbufT = pVbuf + offset;
			for(j = 0;j < tmpW;++j)
			{
				Y = *(pYbuf++);
				U = *(pUbufT);
				V = *(pVbufT);
				pRGB[++k] = CLIP(XYUVTable::tableY[Y] + XYUVTable::tableVR[V]);
				pRGB[++k] = CLIP(XYUVTable::tableY[Y] - XYUVTable::tableVG[V] - XYUVTable::tableUG[U]);
				pRGB[++k] = CLIP(XYUVTable::tableY[Y] + XYUVTable::tableUB[U]);
				Y = *(pYbuf++);
				U = *(pUbufT++);
				V = *(pVbufT++);
				pRGB[++k] = CLIP(XYUVTable::tableY[Y] + XYUVTable::tableVR[V]);
				pRGB[++k] = CLIP(XYUVTable::tableY[Y] - XYUVTable::tableVG[V] - XYUVTable::tableUG[U]);
				pRGB[++k] = CLIP(XYUVTable::tableY[Y] + XYUVTable::tableUB[U]);
			}
		}
	}
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//
	//#define MORE_QUALITY 1
	//void __inline yuv_convert_rgb16(unsigned char *rawY, unsigned char *rawU,
	//    unsigned char *rawV, unsigned char *rgb, int size)
	//{
	//	unsigned short  buf1, buf3;
	//	int   red;
	//	int   blue;
	//	int   green;
	//	unsigned long   cnt;
	//	int    Y, U, V;
	//	unsigned short  data;
	//	unsigned short  data2;
	//
	//	for(cnt = 0;cnt < size;cnt +=2)
	//	{
	//		buf1 = *(rawY+cnt) & 0xff;  // Y data
	//		buf3 = *(rawY+cnt+1) & 0xff;  // Y data
	//
	//		U = *(rawV+cnt/2) & 0xff;
	//		V = *(rawU+cnt/2) & 0xff;
	//
	//#if MORE_QUALITY
	//		Y = buf1;
	//#else
	//		Y = ((buf1+buf3)/2);
	//#endif
	//
	//		red = XlatY[Y] + XlatU_R[U];
	//		CLIP(red);
	//		green = XlatY[Y] - XlatV_G[V] - XlatU_G[U];
	//		CLIP(green);
	//		blue = XlatY[Y] + XlatV_B[V];
	//		CLIP(blue);
	//
	//		data = ((red << 8) & RED_REGION)
	//		| ((green << 3) & GREEN_REGION)
	//		| (blue >> 3);
	//
	//#if MORE_QUALITY
	//		Y = buf3;
	//		red = XlatY[Y] + XlatU_R[U];
	//		CLIP(red);
	//		green = XlatY[Y] - XlatV_G[V] - XlatU_G[U];
	//		CLIP(green);
	//		blue = XlatY[Y] + XlatV_B[V];
	//		CLIP(blue);
	//
	//		data2 = ((red << 8) & RED_REGION)
	//			| ((green << 3) & GREEN_REGION)
	//			| (blue >> 3);
	//#else
	//		data2 = data;
	//#endif
	//
	//		*(unsigned short *)(rgb + 2 * cnt) = data;
	//		*(unsigned short *)(rgb + 2 * (cnt + 1))= data2;
	//	}
	//}

	#define SCALEBITS 8
	#define ONE_HALF  (1 << (SCALEBITS - 1))
	#define FIX(x)    ((int) ((x) * (1L<<SCALEBITS) + 0.5))
	typedef unsigned char uint8_t;

	void RGB2YUV420(unsigned char *Y,unsigned char *U,unsigned char *V, unsigned char *pRGB,int width,int height)
	//static void rgb24_to_yuv420p(uint8_t *lum, uint8_t *cb, uint8_t *cr,uint8_t *src, int width, int height)
	{
		int wrap, wrap3, x, y;
		int r, g, b, r1, g1, b1;
		unsigned char *p;

		wrap = width;
		wrap3 = width * 3;
		p = pRGB;
		for(y = 0;y < height;y += 2) 
		{
			for(x = 0;x < width;x += 2) 
			{
				r = p[0];g = p[1];b = p[2];
				r1 = r;g1 = g; b1 = b;
				Y[0] = (FIX(0.29900) * r + FIX(0.58700) * g +
						  FIX(0.11400) * b + ONE_HALF) >> SCALEBITS;
				r = p[3];g = p[4];b = p[5];
				r1 += r;g1 += g;b1 += b;
				Y[1] = (FIX(0.29900) * r + FIX(0.58700) * g +
						  FIX(0.11400) * b + ONE_HALF) >> SCALEBITS;
				p += wrap3;
				Y += wrap;

				r = p[0];g = p[1];b = p[2];
				r1 += r;g1 += g;b1 += b;
				Y[0] = (FIX(0.29900) * r + FIX(0.58700) * g +
						  FIX(0.11400) * b + ONE_HALF) >> SCALEBITS;
				r = p[3];g = p[4]; b = p[5];
				r1 += r;g1 += g;b1 += b;
				Y[1] = (FIX(0.29900) * r + FIX(0.58700) * g +
						  FIX(0.11400) * b + ONE_HALF) >> SCALEBITS;

				U[0] = ((- FIX(0.16874) * r1 - FIX(0.33126) * g1 +
						  FIX(0.50000) * b1 + 4 * ONE_HALF - 1) >> (SCALEBITS + 2)) + 128;
				V[0] = ((FIX(0.50000) * r1 - FIX(0.41869) * g1 -
						 FIX(0.08131) * b1 + 4 * ONE_HALF - 1) >> (SCALEBITS + 2)) + 128;

				++ U;
				++ V;
				p += -wrap3 + 2 * 3;
				Y += -wrap + 2;
			}
			p += wrap3;
			Y += wrap;
		}
	}
	//inline byte ADJUST(double tmp)
	//{
	//	return (byte)((tmp >= 0 && tmp <= 255)?tmp:(tmp < 0 ? 0 : 255));
	//}
	void YUV420P2RGB(unsigned char *Y,unsigned char *U,unsigned char *V, unsigned char *pRGB,int width,int height)
	//void YUV420P_TO_RGB24(unsigned char* yuv_src,unsigned char* rgb_dst,int nWidth,int nHeight)
	{
		if (NULL == Y || NULL == U || NULL == V  || pRGB == NULL) return;
		if(!XYUVTable::initYUVTable()) return;	//初始化失败
		//Get Y/U/V Pointer from YUV frame data
		int tmpW = width * height;
		unsigned char * pYbuf = Y;					//Pointer to Y
		unsigned char * pVbuf = U;			//Pointer to U
		unsigned char * pVbufT;
		unsigned char * pUbuf = V;	//Pointer to V
		unsigned char * pUbufT;

		int k = -1;
		int offset = 0;
		int Yt,Ut,Vt,i,j;
		tmpW = width >> 1;
		for(i = 0;i < height;++ i)
		{
			offset = (i >> 1) * tmpW;
			//if(i&1) offset += tmpW;
			pUbufT = pUbuf + offset;
			pVbufT = pVbuf + offset;
			for(j = 0;j < tmpW;++j)
			{
				Yt = *(pYbuf++);
				Ut = *(pUbufT);
				Vt = *(pVbufT);
				pRGB[++k] = CLIP(XYUVTable::tableY[Yt] + XYUVTable::tableVR[Vt]);
				pRGB[++k] = CLIP(XYUVTable::tableY[Yt] - XYUVTable::tableVG[Vt] - XYUVTable::tableUG[Ut]);
				pRGB[++k] = CLIP(XYUVTable::tableY[Yt] + XYUVTable::tableUB[Ut]);
				Yt = *(pYbuf++);
				Ut = *(pUbufT++);
				Vt = *(pVbufT++);
				pRGB[++k] = CLIP(XYUVTable::tableY[Yt] + XYUVTable::tableVR[Vt]);
				pRGB[++k] = CLIP(XYUVTable::tableY[Yt] - XYUVTable::tableVG[Vt] - XYUVTable::tableUG[Ut]);
				pRGB[++k] = CLIP(XYUVTable::tableY[Yt] + XYUVTable::tableUB[Ut]);
			}
		}
	//	unsigned char y,u,v,r,g,b;
	//	int C,D,E;
	//	int rgb_width , u_width;
	//	rgb_width = width * 3;
	//	u_width = (width >> 1);
	////	int ypSize = width * height;
	////    int upSize = (ypSize>>2);
	//	int offSet = 0;

	//	for(int i = 0; i < height;++ i)
	//	{
	//		for(int j = 0; j < width;++ j)
	//		{
	//			// Get the Y value from the y planar
	//			y = *(Y + width * i + j);
	//			// Get the V value from the u planar
	//			offSet = (i>>1) * (u_width) + (j>>1);
	//			v = *(U + offSet);
	//			// Get the U value from the v planar
	//			u = *(V + offSet);
 //           
	//			// Cacular the R,G,B values
	//			/*R = ADJUST((Y + (1.4075 * (V - 128))));
	//			G = ADJUST((Y - (0.3455 * (U - 128) - 0.7169 * (V - 128))));
	//			B = ADJUST((Y + (1.7790 * (U - 128))));*/
 //           
	//			// The following formulas are from MicroSoft' MSDN
	//			// the result is better than the former
	//			//
	//			C = y - 16;
	//			D = u - 128;
	//			E = v - 128;
 //           
	//			r = ADJUST(( 298 * C           + 409 * E + 128) >> 8);
	//			g = ADJUST(( 298 * C - 100 * D - 208 * E + 128) >> 8);
	//			b = ADJUST(( 298 * C + 516 * D           + 128) >> 8);
	//			r = ((r - 128) * 0.6 + 128) > 255 ? 255 : (r - 128) * 0.6 + 128;  
	//			g = ((g - 128) * 0.6 + 128) > 255 ? 255 : (g - 128) * 0.6 + 128;  
	//			b = ((b - 128) * 0.6 + 128) > 255 ? 255 : (b - 128) * 0.6 + 128;  
	//			//COLOR_RGB rgb={R,G,B};
	//			//COLOR_HSL hsl;
	//			//RGBtoHSL((const COLOR_RGB *)&rgb,&hsl);
	//			//hsl.saturation =hsl.saturation>90?100:hsl.saturation+10;
	//			//hsl.luminance=hsl.luminance>90?100:hsl.luminance+10;
	//			//HSLtoRGB((const COLOR_HSL *)&hsl,&rgb);
	//			// Set the values

	//			//ns_r为R分量的统计记数
	////            ++ ns_r[R];
	//			//ns_g为G分量的统计记数
	////            ++ ns_g[G];
	//			//ns_b为B分量的统计记数
	////            ++ ns_b[B];
	//			offSet = rgb_width * i + j * 3;
	//			//tmpbuf[offSet] = R;
	//			//tmpbuf[offSet + 1] = G;
	//			//tmpbuf[offSet + 2] = B;

	//			pRGB[offSet] = b;
	//			pRGB[offSet + 1] = g;
	//			pRGB[offSet + 2] = r;
	//		}
	//	}
	//	//for(i=0;i<256;++i) //计算R、G、B三分量的直方图分布
	//	//{
	//	//	//ps_r[i]为R分量中i灰度级出现的概率
	//	//	ps_r[i]=ns_r[i]/((nWidth*nHeight)/1.0f);
	//	//	//ps_b[i]为G分量中i灰度级出现的概率
	//	//	ps_g[i]=ns_g[i]/((nWidth*nHeight)/1.0f);
	//	//	//ps_b[i]为B分量中i灰度级出现的概率
	//	//	ps_b[i]=ns_b[i]/((nWidth*nHeight)/1.0f);
	//	//}
	//	//for(i=0;i<256;++i)
	//	//{
	//	//	//计算累计直方图分布
	//	//	temp_r[i]=(i>0?temp_r[i-1]:0)+ps_r[i];
	//	//	temp_g[i]=(i>0?temp_g[i-1]:0)+ps_g[i];
	//	//	temp_b[i]=(i>0?temp_b[i-1]:0)+ps_b[i];
	//	//	//累计分布取整，ns_r[]、ns_g[]、ns_b[]保存有计算出来的灰度映射关系
	//	//	ns_r[i]=(int)(255.0f*temp_r[i]+0.5f);
	//	//	ns_g[i]=(int)(255.0f*temp_g[i]+0.5f);
	//	//	ns_b[i]=(int)(255.0f*temp_b[i]+0.5f);
	//	//}
	//	//for(i = 0; i < nHeight; ++i)
	//	//{
	//	//	for(int j = 0; j < nWidth; ++j)
	//	//	{
	//	//		offSet = rgb_width * i + j * 3;
	//	//		rgb_dst[offSet]=ns_r[tmpbuf[offSet]] ;
	//	//		rgb_dst[offSet+ 1]=ns_r[tmpbuf[offSet + 1] ];
	//	//		rgb_dst[offSet+ 2]=ns_r[tmpbuf[offSet + 2]];
	//	//	}
	//	//}
	}
}
}