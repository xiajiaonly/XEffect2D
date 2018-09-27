#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XColorSpace.h"
#include "XCColor.h"
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
		if (angle <= 120.0f)
		{
			if (angle <= 60.0f)
			{
				angle = angle / 60.0f;
				return XFColor(1.0f, angle, 0.0f);
			}
			else
			{
				angle = (angle - 60.0f) / 60.0f;
				return XFColor(1.0f - angle, 1.0f, 0.0f);
			}
		}else
		if(angle <= 240.0f)
		{
			if (angle < 180.0f)
			{
				angle = (angle - 120.0f) / 60.0f;
				return XFColor(0.0f, 1.0f, angle);
			}
			else
			{
				angle = (angle - 180.0f) / 60.0f;
				return XFColor(0.0f, 1.0f - angle, 1.0f);
			}
		}else
		{
			if (angle < 300.0f)
			{
				angle = (angle - 240.0f) / 60.0f;
				return XFColor(angle, 0.0f, 1.0f);
			}
			else
			{
				angle = (angle - 300.0f) / 60.0f;
				return XFColor(1.0, 0.0f, 1.0f - angle);
			}
		}
	}
	XFColor getWheelColor(float angle, float rate)
	{
		XFColor ret = getWheelColor(angle);
		if (rate <= 0.5)
		{
			ret.r *= rate * 2.0f;
			ret.g *= rate * 2.0f;
			ret.b *= rate * 2.0f;
		}
		else
		{
			ret.r = XMath::mapping1D(rate * 2.0f, 1.0f, 2.0f, ret.r, 1.0f);
			ret.g = XMath::mapping1D(rate * 2.0f, 1.0f, 2.0f, ret.g, 1.0f);
			ret.b = XMath::mapping1D(rate * 2.0f, 1.0f, 2.0f, ret.b, 1.0f);
		}
		return ret;
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
		//unsigned char R,G,B;
		//double    var_h  = 0.0;
		//int        var_i  = 0;
		//double    var_1 = 0.0,var_2 = 0.0,var_3 = 0.0;
		//double    var_r = 0.0,var_g = 0.0,var_b = 0.0;
		//if ( 0 < S && S < 0.0000001 )        //HSV from 0 to 1
		//{
		//	R = V * 255.0;
		//	G = V * 255.0;
		//	B = V * 255.0;
		//}else
		//{
		//	var_h = H;
		//	if(var_h == 6.0 ) var_h = 0.0;      //H must be < 1
		//	var_i = int( var_h );           //Or ... var_i = floor( var_h )
		//	var_1 = V * ( 1.0 - S );
		//	var_2 = V * ( 1.0 - S * ( var_h - var_i ) );
		//	var_3 = V * ( 1.0 - S * ( 1.0 - ( var_h - var_i ) ) );
 
		//	if( var_i == 0 ) { var_r = V     ; var_g = var_3 ; var_b = var_1;    }
		//	else if ( var_i == 1 ) { var_r = var_2 ; var_g = V; var_b = var_1;    }
		//	else if ( var_i == 2 ) { var_r = var_1 ; var_g = V; var_b = var_3;    }
		//	else if ( var_i == 3 ) { var_r = var_1 ; var_g = var_2; var_b = V;    }
		//	else if ( var_i == 4 ) { var_r = var_3 ; var_g = var_1; var_b = V;    }
		//	else                   { var_r = V ; var_g = var_1; var_b = var_2;    }
 
		//	R = var_r * 255.0;                  //RGB results from 0 to 255
		//	G = var_g * 255.0;
		//	B = var_b * 255.0;
		//}
		//color[0] = R;
		//color[1] = G;
		//color[2] = B;
	}
	void RGB2HSV(unsigned char *color,float& H,float& S,float&V)
	{
		//r,g,b values are from 0 to 1
		//h = [0,6], s = [0,1], v = [0,1]
		//if s == 0, then h = -1 (undefined)
		float R = color[0];
		float G = color[1];
		float B = color[2];
		float min = (std::min)((std::min)(R,G),B);
		float max = (std::max)((std::max)(R,G),B);
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
	std::vector<int> tableY;	//此处初始化有无
	int* tableVR = nullptr;	//此处初始化有无
	int* tableVG = nullptr;	//此处初始化有无
	int* tableUG = nullptr;	//此处初始化有无
	int* tableUB = nullptr;	//此处初始化有无

	inline bool initYUVTable()
	{
		if (tableY.size() != 0) return true;
		tableY.resize(TABLE_SIZE * 5);
		int *y = &tableY[0];
		tableVR = y + TABLE_SIZE;
		tableVG = tableVR + TABLE_SIZE;
		tableUG = tableVG + TABLE_SIZE;
		tableUB = tableUG + TABLE_SIZE;
		//for(int i = 0;i < TABLE_SIZE;++ i) 
		//{
		//	y[i] = ((i - 16) * 298 + 128) >> 8;
		//	tableVR[i] = (409 * (i - 128)) >> 8; 
		//	tableVG[i] = (208 * (i - 128)) >> 8; 
		//	tableUG[i] = (100 * (i - 128)) >> 8; 
		//	tableUB[i] = (516 * (i - 128)) >> 8;
		//}
		for (int i = 0, 
			ty = -16 * 298 + 128, 
			tvr = -128 * 409,
			tvg = -128 * 208,
			tur = -128 * 100,
			tug = -128 * 516;
			i < TABLE_SIZE;
			++i, 
			ty += 298,
			tvr += 409,
			tvg += 208,
			tur += 100,
			tug += 516)
		{
			y[i] = ty >> 8;
			tableVR[i] = tvr >> 8; 
			tableVG[i] = tvg >> 8; 
			tableUG[i] = tur >> 8; 
			tableUB[i] = tug >> 8;
		}
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
		if (NULL == pYUV || pRGB == NULL ||
			!XYUVTable::initYUVTable()) return;	//初始化失败
		//Get Y/U/V Pointer from YUV frame data
		int tmpW = width * height;
		unsigned char* pYbuf = pYUV;					//Pointer to Y
		unsigned char* pVbuf = pYUV + tmpW;			//Pointer to U
		unsigned char* pUbuf = pVbuf + (tmpW >> 2);	//Pointer to V

		int Y,U,V,i,j;
		tmpW = width >> 1;
		for(i = 0;i < height;++ i)
		{
			if (i & 1)
			{
				pVbuf -= tmpW;
				pUbuf -= tmpW;
			}
			for(j = 0;j < tmpW;++j)
			{
				Y = *(pYbuf++);
				U = *(pUbuf);
				V = *(pVbuf);
				*(pRGB++) = CLIP(XYUVTable::tableY[Y] + XYUVTable::tableVR[V]);
				*(pRGB++) = CLIP(XYUVTable::tableY[Y] - XYUVTable::tableVG[V] - XYUVTable::tableUG[U]);
				*(pRGB++) = CLIP(XYUVTable::tableY[Y] + XYUVTable::tableUB[U]);
				Y = *(pYbuf++);
				U = *(pUbuf++);
				V = *(pVbuf++);
				*(pRGB++) = CLIP(XYUVTable::tableY[Y] + XYUVTable::tableVR[V]);
				*(pRGB++) = CLIP(XYUVTable::tableY[Y] - XYUVTable::tableVG[V] - XYUVTable::tableUG[U]);
				*(pRGB++) = CLIP(XYUVTable::tableY[Y] + XYUVTable::tableUB[U]);
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
	void XYZ2RGB(unsigned char *color, float X, float Y, float Z)
	{
		X = X * 0.01f;        //X from 0 to  95.047      (Observer = 2°, Illuminant = D65)
		Y = Y * 0.01f;        //Y from 0 to 100.000
		Z = Z * 0.01f;        //Z from 0 to 108.883

		float R = X *  3.2406 + Y * -1.5372 + Z * -0.4986;
		float G = X * -0.9689 + Y *  1.8758 + Z *  0.0415;
		float B = X *  0.0557 + Y * -0.2040 + Z *  1.0570;

		if (R > 0.0031308) R = 1.055 * pow(static_cast<double>(R), (1.0 / 2.4)) - 0.055;
		else R = 12.92 * R;
		if (G > 0.0031308) G = 1.055 * pow(static_cast<double>(G), (1.0 / 2.4)) - 0.055;
		else G = 12.92 * G;
		if (B > 0.0031308) B = 1.055 * pow(static_cast<double>(B), (1.0 / 2.4)) - 0.055;
		else B = 12.92 * B;

		color[0] = R * 255.0f;
		color[1] = G * 255.0f;
		color[2] = B * 255.0f;
	}
	void RGB2XYZ(unsigned char *color, float &X, float &Y, float &Z)
	{
		float R = color[0] / 255.0f;        //R from 0 to 255
		float G = color[1] / 255.0f;        //G from 0 to 255
		float B = color[2] / 255.0f;        //B from 0 to 255

		if (R > 0.04045) R = pow((R + 0.055) / 1.055, 2.4);
		else R = R / 12.92;
		if (G > 0.04045) G = pow((G + 0.055) / 1.055, 2.4);
		else G = G / 12.92;
		if (B > 0.04045) B = pow((B + 0.055) / 1.055, 2.4);
		else B = B / 12.92;

		R = R * 100.0;
		G = G * 100.0;
		B = B * 100.0;

		//Observer. = 2°, Illuminant = D65
		X = R * 0.4124 + G * 0.3576 + B * 0.1805;
		Y = R * 0.2126 + G * 0.7152 + B * 0.0722;
		Z = R * 0.0193 + G * 0.1192 + B * 0.9505;
	}
	void XYZ2Yxy(float X, float Y, float Z, float &oY, float &ox, float &oy)
	{
		//X from 0 to 95.047       Observer. = 2°, Illuminant = D65
		//Y from 0 to 100.000
		//Z from 0 to 108.883

		oY = Y;
		ox = X / (X + Y + Z);
		oy = Y / (X + Y + Z);
	}
	void Yxy2XYZ(float &X, float &Y, float &Z, float iY, float ix, float iy)
	{
		//Y from 0 to 100
		//x from 0 to 1
		//y from 0 to 1

		X = ix * (iY / iy);
		Y = iY;
		Z = (1 - ix - iy) * (iY / iy);
	}
	void XYZ2HLab(float X, float Y, float Z, float &l, float &a, float &b)
	{
		l = 10.0 * sqrt(Y);
		a = 17.5 * (1.02 * X - Y) / sqrt(Y);
		b = 7.0 * (Y - 0.847 * Z) / sqrt(Y);
	}
	void HLab2XYZ(float &X, float &Y, float &Z, float l, float a, float b)
	{
		Y = l * 0.1;
		X = a / 17.5 * l * 0.1;
		Z = b / 7.0 * l * 0.1;

		Y = Y * Y;
		X = (X + Y) / 1.02;
		Z = -(Z - Y) / 0.847;
	}
	void XYZ2CLab(float X, float Y, float Z, float &l, float &a, float &b)
	{
		float ref_X = 95.047, ref_Y = 100.0, ref_Z = 108.883;
		X = X / ref_X;          //ref_X =  95.047   Observer= 2°, Illuminant= D65
		Y = Y / ref_Y;         //ref_Y = 100.000
		Z = Z / ref_Z;          //ref_Z = 108.883

		if (X > 0.008856) X = pow(static_cast<double>(X),1.0 / 3.0);
		else X = 7.787 * X + 16.0 / 116.0;
		if (Y > 0.008856) Y = pow(static_cast<double>(Y),1.0 / 3.0);
		else Y = 7.787 * Y + 16.0 / 116.0;
		if (Z > 0.008856) Z = pow(static_cast<double>(Z),1.0 / 3.0);
		else Z = 7.787 * Z + 16.0 / 116.0;

		l = 116.0 * Y - 16.0;
		a = 500.0 * (X - Y);
		b = 200.0 * (Y - Z);
		if (l < 0.0) l = 0.0;
	}
	void CLab2XYZ(float &X, float &Y, float &Z, float l, float a, float b)
	{
		Y = (l + 16.0) / 116.0;
		X = a / 500.0 + Y;
		Z = b / 200.0;

		if (Y * Y * Y > 0.008856) Y = Y * Y * Y;
		else Y = (Y - 16.0 / 116.0) / 7.787;
		if (X * X * X > 0.008856) X = X * X * X;
		else X = (X - 16.0 / 116.0) / 7.787;
		if (Z * Z * Z > 0.008856) Z = Z * Z * Z;
		else Z = (Z - 16.0 / 116.0) / 7.787;

		X = 95.047 * X;     //ref_X =  95.047     Observer= 2°, Illuminant= D65
		Y = 100.000 * Y;     //ref_Y = 100.000
		Z = 108.883 * Z;     //ref_Z = 108.883
	}
	void CLab2CLCH(float &L, float &C, float &H, float l, float a, float b)
	{
		H = atan2(b, a);  //Quadrant by signs

		if (H > 0.0) H = (H / PI) * 180.0;
		else H = 360.0 - (abs(H) / PI) * 180.0;

		L = l;
		C = _hypot(a,b);
	}
	void CLCH2CLab(float L, float C, float H, float &l, float &a, float &b)
	{
		//CIE-H° from 0 to 360°
		l = L;
		a = cos(H * DEGREE2RADIAN) * C;
		b = sin(H * DEGREE2RADIAN) * C;
	}
	void XYZ2CLuv(float X, float Y, float Z, float &l, float &u, float &v)
	{
		u = (4.0 * X) / (X + 15.0 * Y + 3.0 * Z);
		v = (9.0 * Y) / (X + 15.0 * Y + 3.0 * Z);

		Y = Y * 0.01f;
		if (Y > 0.008856) Y = pow(static_cast<double>(Y), 1.0 / 3.0);
		else Y = (7.787 * Y) + (16.0 / 116.0);

		float ref_X = 95.047;        //Observer= 2°, Illuminant= D65
		float ref_Y = 100.000;
		float ref_Z = 108.883;

		float ref_U = (4.0 * ref_X) / (ref_X + 15.0 * ref_Y + 3.0 * ref_Z);
		float ref_V = (9.0 * ref_Y) / (ref_X + 15.0 * ref_Y + 3.0 * ref_Z);

		l = (116.0 * Y) - 16.0;
		u = 13.0 * l * (u - ref_U);
		v = 13.0 * l * (v - ref_V);
	}
	void CLuv2XYZ(float &X, float &Y, float &Z, float l, float u, float v)
	{
		Y = (l + 16.0) / 116.0;
		if (Y * Y * Y > 0.008856) Y = Y * Y * Y;
		else Y = (Y - 16.0 / 116.0) / 7.787;

		float ref_X = 95.047;      //Observer= 2°, Illuminant= D65
		float ref_Y = 100.000;
		float ref_Z = 108.883;

		float ref_U = (4.0 * ref_X) / (ref_X + 15.0 * ref_Y + 3.0 * ref_Z);
		float ref_V = (9.0 * ref_Y) / (ref_X + 15.0 * ref_Y + 3.0 * ref_Z);

		u = u / (13.0 * l) + ref_U;
		v = v / (13.0 * l) + ref_V;

		Y = Y * 100.0;
		X = -(9.0 * Y * u) / ((u - 4.0) * v - u * v);
		Z = (9.0 * Y - 15.0 * v * Y - v * X) / (3.0 * v);
	}
	void RGB2HSL(float R, float G, float B, float &H, float &S, float &L)
	{
		R = (R / 255.0);                     //RGB from 0 to 255
		G = (G / 255.0);
		B = (B / 255.0);

		float var_Min = (std::min)(R, (std::min)(G, B));    //Min. value of RGB
		float var_Max = (std::max)(R, (std::max)(G, B));    //Max. value of RGB
		float del_Max = var_Max - var_Min;             //Delta RGB value

		L = (var_Max + var_Min) / 2.0;

		if (del_Max == 0.0)                     //This is a gray, no chroma...
		{
			H = 0.0;                                //HSL results from 0 to 1
			S = 0.0;
		}else                                    //Chromatic data...
		{
			if (L < 0.5) S = del_Max / (var_Max + var_Min);
			else S = del_Max / (2.0 - var_Max - var_Min);

			float del_R = ((var_Max - R) / 6.0 + del_Max * 0.5) / del_Max;
			float del_G = ((var_Max - G) / 6.0 + del_Max * 0.5) / del_Max;
			float del_B = ((var_Max - B) / 6.0 + del_Max * 0.5) / del_Max;

			if (R == var_Max) H = del_B - del_G;
			else if (G == var_Max) H = (1.0 / 3.0) + del_R - del_B;
			else if (B == var_Max) H = (2.0 / 3.0) + del_G - del_R;

			if (H < 0.0) H += 1.0;
			if (H > 1.0) H -= 1.0;
		}
	}
	float Hue_2_RGB(float v1,float v2,float vH)             //Function Hue_2_RGB
	{
		if (vH < 0.0) vH += 1.0;
		if (vH > 1.0) vH -= 1.0;
		if (6.0 * vH < 1.0) return (v1 + (v2 - v1) * 6.0 * vH);
		if (2.0 * vH < 1.0) return (v2);
		if (3.0 * vH < 2.0) return (v1 + (v2 - v1) * (2.0 / 3.0 - vH) * 6.0);
		return (v1);
	}
	void HSL2RGB(float &R, float &G, float &B, float H, float S, float L)
	{
		if (S == 0.0)                       //HSL from 0 to 1
		{
			R = L * 255.0;                      //RGB results from 0 to 255
			G = L * 255.0;
			B = L * 255.0;
		}else
		{
			float var_2;
			if (L < 0.5) var_2 = L * (1.0 + S);
			else var_2 = (L + S) - (S * L);

			float var_1 = 2.0 * L - var_2;

			R = 255.0 * Hue_2_RGB(var_1, var_2, H + (1.0 / 3.0));
			G = 255.0 * Hue_2_RGB(var_1, var_2, H);
			B = 255.0 * Hue_2_RGB(var_1, var_2, H - (1.0 / 3.0));
		}
	}
	void RGB2HSV(float R, float G, float B, float &H, float &S, float &V)
	{
		R = (R / 255.0);                     //RGB from 0 to 255
		G = (G / 255.0);
		B = (B / 255.0);

		float var_Min = (std::min)(R, (std::min)(G, B));    //Min. value of RGB
		float var_Max = (std::max)(R, (std::max)(G, B));    //Max. value of RGB
		float del_Max = var_Max - var_Min;             //Delta RGB value 

		V = var_Max;

		if (del_Max == 0.0)                     //This is a gray, no chroma...
		{
			H = 0.0;                                //HSV results from 0 to 1
			S = 0.0;
		}else                                    //Chromatic data...
		{
			S = del_Max / var_Max;

			float del_R = ((var_Max - R) / 6.0 + del_Max * 0.5) / del_Max;
			float del_G = ((var_Max - G) / 6.0 + del_Max * 0.5) / del_Max;
			float del_B = ((var_Max - B) / 6.0 + del_Max * 0.5) / del_Max;

			if (R == var_Max) H = del_B - del_G;
			else if (G == var_Max) H = (1.0 / 3.0) + del_R - del_B;
			else if (B == var_Max) H = (2.0 / 3.0) + del_G - del_R;

			if (H < 0.0) H += 1.0;
			if (H > 1.0) H -= 1.0;
		}
	}
	void HSV2RGB(float &R, float &G, float &B, float H, float S, float V)
	{
		if (S == 0.0)                       //HSV from 0 to 1
		{
			R = V * 255.0;
			G = V * 255.0;
			B = V * 255.0;
		}
		else
		{
			H = H * 6.0;
			if (H == 6.0) H = 0.0;      //H must be < 1
			int var_i = int(H);             //Or ... var_i = floor( var_h )
			float var_1 = V * (1.0 - S);
			float var_2 = V * (1.0 - S * (H - var_i));
			float var_3 = V * (1.0 - S * (1.0 - (H - var_i)));

			switch(var_i)
			{
			case 0: R = V; G = var_3; B = var_1; break;
			case 1: R = var_2; G = V; B = var_1; break;
			case 2: R = var_1; G = V; B = var_3; break;
			case 3: R = var_1; G = var_2; B = V; break;
			case 4: R = var_3; G = var_1; B = V; break;
			default: R = V; G = var_1; B = var_2; break;
			}

			R = R * 255.0;                  //RGB results from 0 to 255
			G = G * 255.0;
			B = B * 255.0;
		}
	}
	void RGB2CMY(float R, float G, float B, float &C, float &M, float &Y)
	{
		//RGB values from 0 to 255
		//CMY results from 0 to 1
		C = 1.0 - (R / 255.0);
		M = 1.0 - (G / 255.0);
		Y = 1.0 - (B / 255.0);
	}
	void CMY2RGB(float &R, float &G, float &B, float C, float M, float Y)
	{
		//CMY values from 0 to 1
		//RGB results from 0 to 255

		R = (1.0 - C) * 255.0;
		G = (1.0 - M) * 255.0;
		B = (1.0 - Y) * 255.0;
	}
	void CMY2CMYK(float C, float M, float Y, float &c, float &m, float &y, float &k)
	{
		//CMYK and CMY values from 0 to 1
		k = 1.0;

		if (C < k)   k = C;
		if (M < k)   k = M;
		if (Y < k)   k = Y;
		if (k == 1.0) 
		{ //Black
			c = m = y = 0.0;
		}
		else 
		{
			c = (C - k) / (1.0 - k);
			m = (M - k) / (1.0 - k);
			y = (Y - k) / (1.0 - k);
		}
	}
	void CMYK2CMY(float &C, float &M, float &Y, float c, float m, float y, float k)
	{
		//CMYK and CMY values from 0 to 1
		C = (c * (1.0 - k) + k);
		M = (m * (1.0 - k) + k);
		Y = (y * (1.0 - k) + k);
	}
	//void rgb2lab(int R, int G, int B, int &oL, int &oA, int &oB)
	//{//http://www.brucelindbloom.com
	//	float r, g, b, X, Y, Z, fx, fy, fz, xr, yr, zr;
	//	float Ls, as, bs;
	//	float eps = 216.f / 24389.f;
	//	float k = 24389.f / 27.f;

	//	float Xr = 0.964221f;  // reference white D50
	//	float Yr = 1.0f;
	//	float Zr = 0.825211f;

	//	// RGB to XYZ
	//	r = R / 255.f; //R 0..1
	//	g = G / 255.f; //G 0..1
	//	b = B / 255.f; //B 0..1

	//	// assuming sRGB (D65)
	//	if (r <= 0.04045)
	//		r = r / 12.0;
	//	else
	//		r = (float)pow((r + 0.055) / 1.055, 2.4);

	//	if (g <= 0.04045)
	//		g = g / 12.0;
	//	else
	//		g = (float)pow((g + 0.055) / 1.055, 2.4);

	//	if (b <= 0.04045)
	//		b = b / 12.0;
	//	else
	//		b = (float)pow((b + 0.055) / 1.055, 2.4);


	//	X = 0.436052025f * r + 0.385081593f * g + 0.143087414f * b;
	//	Y = 0.222491598f * r + 0.71688606f * g + 0.060621486f * b;
	//	Z = 0.013929122f * r + 0.097097002f * g + 0.71418547f * b;

	//	// XYZ to Lab
	//	xr = X / Xr;
	//	yr = Y / Yr;
	//	zr = Z / Zr;

	//	if (xr > eps)
	//		fx = (float)pow(xr, 1.0 / 3.0);
	//	else
	//		fx = (float)((k * xr + 16.0) / 116.0);

	//	if (yr > eps)
	//		fy = (float)pow(yr, 1.0 / 3.0);
	//	else
	//		fy = (float)((k * yr + 16.0) / 116.0);

	//	if (zr > eps)
	//		fz = (float)pow(zr, 1.0 / 3.0);
	//	else
	//		fz = (float)((k * zr + 16.0) / 116.0);

	//	Ls = (116 * fy) - 16;
	//	as = 500 * (fx - fy);
	//	bs = 200 * (fy - fz);

	//	oL = (int)(2.55 * Ls + .5);
	//	oA = (int)(as + .5);
	//	oB = (int)(bs + .5);
	//}
	double getColorDifferenceLab(const XCColor &c0, const XCColor &c1)
	{
		float a[3], b[3];
		float a0[3], b0[3];
		RGB2XYZ(c0, a0[0], a0[1], a0[2]);
		RGB2XYZ(c1, b0[0], b0[1], b0[2]);
		XYZ2CLab(a0[0], a0[1], a0[2], a[0], a[1], a[2]);
		XYZ2CLab(b0[0], b0[1], b0[2], b[0], b[1], b[2]);
	//	rgb2lab(c0.r, c0.g, c0.b, a[0], a[1], a[2]);
	//	rgb2lab(c1.r, c1.g, c1.b, b[0], b[1], b[2]);
		return sqrt(powf(b[0] - a[0], 2.0) + powf(b[1] - a[1], 2.0) + powf(b[2] - a[2], 2.0));
	}
	double getColorDifferenceHSL(const XCColor &c0, const XCColor &c1)
	{
		float a[3], b[3];
		RGB2HSL(c0.r, c0.g, c0.b, a[0], a[1], a[2]);
		RGB2HSL(c1.r, c1.g, c1.b, b[0], b[1], b[2]);
		float tmp = abs(a[0] - b[0]);
		if(tmp > 0.5f) tmp = 1.0f - tmp;
		return sqrt(powf(tmp, 2.0) + powf(b[1] - a[1], 2.0) + powf(b[2] - a[2], 2.0));
	}
	double getColorDifferenceHSV(const XCColor &c0, const XCColor &c1)
	{
		float a[3], b[3];
		RGB2HSV(c0.r, c0.g, c0.b, a[0], a[1], a[2]);
		RGB2HSV(c1.r, c1.g, c1.b, b[0], b[1], b[2]);
		float tmp = abs(a[0] - b[0]);
		if(tmp > 0.5f) tmp = 1.0f - tmp;
	//	return sqrt(pow(tmp, 2.0) + pow(b[1] - a[1], 2.0) + pow(b[2] - a[2], 2.0));
		return tmp;
	}
	double getColorDifferenceLuv(const XCColor &c0, const XCColor &c1)
	{
		float a[3], b[3];
		float a0[3], b0[3];
		RGB2XYZ(c0, a0[0], a0[1], a0[2]);
		RGB2XYZ(c1, b0[0], b0[1], b0[2]);
		XYZ2CLuv(a0[0], a0[1], a0[2], a[0], a[1], a[2]);
		XYZ2CLuv(b0[0], b0[1], b0[2], b[0], b[1], b[2]);
		return sqrt(powf(b[0] - a[0], 2.0) + powf(b[1] - a[1], 2.0) + powf(b[2] - a[2], 2.0));
	}
	double getColorDifferenceRGB(const XCColor &c0, const XCColor &c1)
	{
		return sqrt(pow(c0.r - c1.r, 2.0) + pow(c0.g - c1.g, 2.0) + pow(c0.b - c1.b, 2.0));
	}
	double getColorDifferenceDeltaE1994(const XCColor &c0, const XCColor &c1)
	{//http://www.easyrgb.com/index.php?X=DELT&H=04#text4 //这个网站的是错误的
		//http://www.brucelindbloom.com/index.html?Eqn_DeltaE_CIE94.html 根据官方的修改
		double WHT_L = 1.0, WHT_C = 1.0, WHT_H = 1.0;
		float a[3], b[3];
		float a0[3], b0[3];
		RGB2XYZ(c0, a0[0], a0[1], a0[2]);
		RGB2XYZ(c1, b0[0], b0[1], b0[2]);
		XYZ2CLab(a0[0], a0[1], a0[2], a[0], a[1], a[2]);
		XYZ2CLab(b0[0], b0[1], b0[2], b[0], b[1], b[2]);

		double xC1 = _hypot(a[1],a[2]);
		double xC2 = _hypot(b[1],b[2]);
		double xDL = a[0] - b[0];
		double xDa = a[1] - b[1];
		double xDb = a[2] - b[2];

		double xDC = xC1 - xC2;
		double xDH;
		if(xDa * xDa + xDb * xDb >= xDC * xDC) 
		   xDH = sqrt(xDa * xDa + xDb * xDb - xDC * xDC);
		else 
		   xDH = 0.0;
		double xSC = 1.0 + 0.045 * xC1;
		double xSH = 1.0 + 0.015 * xC1;
		xDL /= WHT_L;
		xDC /= WHT_C * xSC;
		xDH /= WHT_H * xSH;
		return sqrt(xDL * xDL + xDC * xDC + xDH * xDH);
	}
	double getColorDifferenceCiede2000(const XCColor &c0, const XCColor &c1)	//比较两个颜色的区别，返回比较系数
	{//https://github.com/gfiumara/CIEDE2000/blob/master/CIEDE2000.cpp
		//int a[3], b[3];
		//rgb2lab(c0.r, c0.g, c0.b, a[0], a[1], a[2]);
		//rgb2lab(c1.r, c1.g, c1.b, b[0], b[1], b[2]);
		float a[3], b[3];
		float a0[3], b0[3];
		RGB2XYZ(c0, a0[0], a0[1], a0[2]);
		RGB2XYZ(c1, b0[0], b0[1], b0[2]);
		XYZ2CLab(a0[0], a0[1], a0[2], a[0], a[1], a[2]);
		XYZ2CLab(b0[0], b0[1], b0[2], b[0], b[1], b[2]);

		//"For these and all other numerical/graphical 􏰀delta E00 values
		//reported in this article, we set the parametric weighting factors
		//to unity(i.e., k_L = k_C = k_H = 1.0)." (Page 27).
		const double k_L = 1.0, k_C = 1.0, k_H = 1.0;
		const double deg360InRad = DEGREE2RADIAN * 360.0;
		const double deg180InRad = DEGREE2RADIAN * 180.0;
		const double pow25To7 = 6103515625.0; // pow(25, 7)

		//Step 1
		//Equation 2 
		double C1 = _hypot(a[1],a[2]);
		double C2 = _hypot(b[1],b[2]);
		//Equation 3 
		double barC = (C1 + C2) * 0.5;
		//Equation 4 
		double G = 0.5 * (1.0 - sqrt(pow(barC, 7.0) / (pow(barC, 7.0) + pow25To7)));
		//Equation 5 
		double a1Prime = (1.0 + G) * a[1];
		double a2Prime = (1.0 + G) * b[1];
		//Equation 6 
		double CPrime1 = _hypot(a1Prime,a[2]);
		double CPrime2 = _hypot(a2Prime,b[2]);
		//Equation 7 
		double hPrime1;
		if (a[2] == 0.0 && a1Prime == 0.0)
			hPrime1 = 0.0;
		else 
		{
			hPrime1 = atan2(static_cast<double>(a[2]), a1Prime);
			//This must be converted to a hue angle in degrees between 0
			//and 360 by addition of 2􏰏 to negative hue angles.
			if (hPrime1 < 0.0)
				hPrime1 += deg360InRad;
		}
		double hPrime2;
		if (b[2] == 0.0 && a2Prime == 0.0)
			hPrime2 = 0.0;
		else 
		{
			hPrime2 = atan2(static_cast<double>(b[2]), a2Prime);
			//This must be converted to a hue angle in degrees between 0
			//and 360 by addition of 2􏰏 to negative hue angles.
			if (hPrime2 < 0.0)
				hPrime2 += deg360InRad;
		}

		
		//Step 2
		//Equation 8 
		double deltaLPrime = b[0] - a[0];
		//Equation 9 
		double deltaCPrime = CPrime2 - CPrime1;
		//Equation 10 
		double deltahPrime;
		double CPrimeProduct = CPrime1 * CPrime2;
		if (CPrimeProduct == 0.0)
			deltahPrime = 0.0;
		else 
		{
			//Avoid the fabs() call
			deltahPrime = hPrime2 - hPrime1;
			if (deltahPrime < -deg180InRad)
				deltahPrime += deg360InRad;
			else if (deltahPrime > deg180InRad)
				deltahPrime -= deg360InRad;
		}
		//Equation 11 
		double deltaHPrime = 2.0 * sqrt(CPrimeProduct) *
			sin(deltahPrime * 0.5);

		//Step 3
		//Equation 12
		double barLPrime = (a[0] + b[0]) * 0.5;
		//Equation 13
		double barCPrime = (CPrime1 + CPrime2) * 0.5;
		//Equation 14 
		double barhPrime, hPrimeSum = hPrime1 + hPrime2;
		if (CPrime1 * CPrime2 == 0) 
		{
			barhPrime = hPrimeSum;
		}else 
		{
			if (fabs(hPrime1 - hPrime2) <= deg180InRad)
				barhPrime = hPrimeSum * 0.5;
			else 
			{
				if (hPrimeSum < deg360InRad)
					barhPrime = (hPrimeSum + deg360InRad) * 0.5;
				else
					barhPrime = (hPrimeSum - deg360InRad) * 0.5;
			}
		}
		//Equation 15 
		double T = 1.0 - (0.17 * cos(barhPrime - 30.0 * DEGREE2RADIAN)) +
			(0.24 * cos(2.0 * barhPrime)) +
			(0.32 * cos((3.0 * barhPrime) + 6.0 * DEGREE2RADIAN)) -
			(0.20 * cos((4.0 * barhPrime) - 63.0 * DEGREE2RADIAN));
		//Equation 16 
		double deltaTheta = 30.0 * DEGREE2RADIAN *
			exp(-pow((barhPrime - 275.0 * DEGREE2RADIAN) / (25.0 * DEGREE2RADIAN), 2.0));
		//Equation 17 
		double R_C = 2.0 * sqrt(pow(barCPrime, 7.0) /
			(pow(barCPrime, 7.0) + pow25To7));
		//Equation 18 
		double S_L = 1.0 + ((0.015 * pow(barLPrime - 50.0, 2.0)) /
			sqrt(20.0 + pow(barLPrime - 50.0, 2.0)));
		//Equation 19 
		double S_C = 1.0 + (0.045 * barCPrime);
		//Equation 20 
		double S_H = 1.0 + (0.015 * barCPrime * T);
		//Equation 21 
		double R_T = (-sin(2.0 * deltaTheta)) * R_C;

		//Equation 22 
		return sqrt(
			pow(deltaLPrime / (k_L * S_L), 2.0) +
			pow(deltaCPrime / (k_C * S_C), 2.0) +
			pow(deltaHPrime / (k_H * S_H), 2.0) +
			(R_T * (deltaCPrime / (k_C * S_C)) * (deltaHPrime / (k_H * S_H))));
	}
	double CieLab2Hue(double var_a,double  var_b)          //Function returns CIE-H° value
	{
	   double var_bias = 0.0;
	   if (var_a >= 0.0 && var_b == 0.0) return 0.0;
	   if (var_a <  0.0 && var_b == 0.0) return 180.0;
	   if (var_a == 0.0 && var_b >  0.0) return 90.0;
	   if (var_a == 0.0 && var_b <  0.0) return 270.0;
	   if (var_a >  0.0 && var_b >  0.0) var_bias = 0.0;
	   if (var_a <  0.0               ) var_bias = 180.0;
	   if (var_a >  0.0 && var_b <  0.0) var_bias = 360.0;
	   return atan2(var_b , var_a) * RADIAN2DEGREE + var_bias;
	}
	double getColorDifferenceDeltaCMC(const XCColor &c0, const XCColor &c1)
	{//http://www.easyrgb.com/index.php?X=DELT&H=06#text6
		float a[3], b[3];
		float a0[3], b0[3];
		RGB2XYZ(c0, a0[0], a0[1], a0[2]);
		RGB2XYZ(c1, b0[0], b0[1], b0[2]);
		XYZ2CLab(a0[0], a0[1], a0[2], a[0], a[1], a[2]);
		XYZ2CLab(b0[0], b0[1], b0[2], b[0], b[1], b[2]);

		double WHT_L = 1.0, WHT_C = 1.0f;                       //Wheight factor

		double xC1 = _hypot(a[1],a[2]);
		double xC2 = _hypot(b[1],b[2]);
		double xff = sqrt(pow(xC1,4.0) / (pow(xC1,4.0) + 1900.0));
		double xH1 = CieLab2Hue(a[1],a[2]);
		double xTT;
		if(xH1 < 164.0 || xH1 > 345.0) xTT = 0.36 + abs(0.4 * cos(( 35.0 + xH1) * DEGREE2RADIAN));
		else                          xTT = 0.56 + abs( 0.2 * cos((168.0 + xH1) * DEGREE2RADIAN));
 
		double xSL;
		if(a[0] < 16.0) xSL = 0.511;
		else          xSL = (0.040975 * a[0]) / (1.0 + 0.01765 * a[0]);
 
		double xSC = (0.0638 * xC1) / (1.0 + 0.0131 * xC1) + 0.638;
		double xSH = (xff * xTT + 1.0 - xff) * xSC;
		double xDH = sqrt((b[1] - a[1]) * (b[1] - a[1]) + (b[2] - a[2]) * (b[2] - a[2]) - (xC2 - xC1) * (xC2 - xC1));
		xSL = (b[0] - a[0]) / (WHT_L * xSL);	//这里与原版不符，原版有bug
		xSC = (xC2 - xC1) / (WHT_C * xSC);		//这里与原版不符，原版有bug
		xSH = xDH / xSH;
		return sqrt(xSL * xSL + xSC * xSC + xSH * xSH);
	}
	double getColorDifferenceDeltaE2000(const XCColor &c0, const XCColor &c1)
	{//http://www.easyrgb.com/index.php?X=DELT&H=05#text5
		float a[3], b[3];
		float a0[3], b0[3];
		RGB2XYZ(c0, a0[0], a0[1], a0[2]);
		RGB2XYZ(c1, b0[0], b0[1], b0[2]);
		XYZ2CLab(a0[0], a0[1], a0[2], a[0], a[1], a[2]);
		XYZ2CLab(b0[0], b0[1], b0[2], b[0], b[1], b[2]);

		double WHT_L = 1.0,WHT_C = 1.0,WHT_H = 1.0;                //Wheight factor

		double xC1 = _hypot(a[1],a[2]);
		double xC2 = _hypot(b[1],b[2]);
		double xCX = (xC1 + xC2) * 0.5;
		double xGX = 0.5 * (1.0 - sqrt(pow(xCX,7.0) / (pow(xCX,7.0) + pow(25.0,7.0))));
		double xNN = (1.0 + xGX) * a[1];
		xC1 = _hypot(xNN,a[2]);
		double xH1 = CieLab2Hue(xNN, a[2]);
		xNN = (1.0 + xGX) * b[1];
		xC2 = _hypot(xNN,b[2]);
		double xH2 = CieLab2Hue(xNN, b[2]);
		double xDL = b[0] - a[0];
		double xDC = xC2 - xC1;
		double xDH;
		if(xC1 * xC2 == 0.0)
		{
		   xDH = 0.0;
		}else 
		{
		   //xNN = round(xH2 - xH1, 12);
			xNN = xH2 - xH1;
		   if (abs(xNN) <= 180.0) 
		   {
			  xDH = xH2 - xH1;
		   }else 
		   {
			  if (xNN > 180.0) xDH = xH2 - xH1 - 360.0;
			  else             xDH = xH2 - xH1 + 360.0;
		   }
		}
		xDH = 2.0 * sqrt(xC1 * xC2) * sin(xDH * 0.5 * DEGREE2RADIAN);
		double xLX = (a[0] + b[0]) * 0.5;
		double xCY = (xC1 + xC2) * 0.5;
		double xHX;
		if(xC1 * xC2 == 0.0)
		{
		   xHX = xH1 + xH2;
		}else 
		{
		   //xNN = abs(round(xH1 - xH2,12));
		   xNN = abs(xH1 - xH2);
		   if (xNN > 180.0) 
		   {
			  if ((xH2 + xH1) < 360.0) xHX = xH1 + xH2 + 360.0;
			  else                     xHX = xH1 + xH2 - 360.0;
		   }else 
		   {
			  xHX = xH1 + xH2;
		   }
		   xHX *= 0.5;
		}
		double xTX = 1.0 - 0.17 * cos((xHX - 30) * DEGREE2RADIAN) + 0.24
					   * cos(2.0 * xHX * DEGREE2RADIAN) + 0.32
					   * cos((3.0 * xHX + 6.0) * DEGREE2RADIAN) - 0.20
					   * cos((4.0 * xHX - 63.0)* DEGREE2RADIAN);
		double xPH = 30.0 * exp( -((xHX - 275.0) / 25.0) * ((xHX - 275.0) / 25.0));
		double xRC = 2.0 * sqrt(pow(xCY,7.0) / (pow(xCY,7.0) + pow(25.0,7.0)));
		double xSL = 1.0 + ((0.015 * ((xLX - 50.0) * (xLX - 50.0)))
				/ sqrt(20.0 + ((xLX - 50.0) * (xLX - 50.0))));
		double xSC = 1.0 + 0.045 * xCY;
		double xSH = 1.0 + 0.015 * xCY * xTX;
		double xRT = - sin(2.0 * xPH * DEGREE2RADIAN) * xRC;
		xDL /= WHT_L * xSL;
		xDC /= WHT_C * xSC;
		xDH /= WHT_H * xSH;
		return sqrt(xDL * xDL + xDC * xDC + xDH * xDH + xRT * xDC * xDH);
	}
	void RGB2Gray(const unsigned char *rgb,char unsigned *gray,int pixelsSum)
	{
		if(rgb == NULL || gray == NULL || pixelsSum <= 0) return;
		for(int i = 0,index = 0;i < pixelsSum;++ i,index += 3)
		{
			gray[i] = (rgb[index] + rgb[index + 1] + rgb[index + 2]) / 3;
		}
	}
	float getColorLuminance(const XFColor& color)
	{
		return 0.299f * color.r + 0.587f * color.g + 0.114f * color.b;
	}
}
}