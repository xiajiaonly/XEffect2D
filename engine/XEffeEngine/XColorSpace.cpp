//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XColorSpace.h"
#include "math.h"
#include "windows.h"

#define max3v(a,b,c)    ((a > ((b>c)?b:c)) ? a : ((b>c)?b:c))
#define min3v(a,b,c)    ((a < ((b<c)?b:c)) ? a : ((b<c)?b:c))
#define HSLMAX         240
#define RGBMAX         255
#define UNDEFINED    (HSLMAX*2/3)

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
    float min = min(min(R,G),B);
    float max = max(max(R,G),B);
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

#define TABLE_SIZE  256
int tableY[TABLE_SIZE] = { 0 };	//此处初始化有无
int tableVR[TABLE_SIZE] = { 0 };	//此处初始化有无
int tableVG[TABLE_SIZE] = { 0 };	//此处初始化有无
int tableUG[TABLE_SIZE] = { 0 };	//此处初始化有无
int tableUB[TABLE_SIZE] = { 0 };	//此处初始化有无

#define ORIG_XLAT 1
void initYUVTable()
{
	static int flag = 0;
	if(flag != 0) return;

	for(int i = 0;i < TABLE_SIZE;++ i) 
	{
		tableY[i] = ((i - 16) * 298 + 128) >> 8;
		tableVR[i] = (409 * (i - 128)) >> 8; 
		tableVG[i] = (208 * (i - 128)) >> 8; 
		tableUG[i] = (100 * (i - 128)) >> 8; 
		tableUB[i] = (516 * (i - 128)) >> 8;
	}
	flag = 1;	//
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
//YUV杞琑GB
void YUVToRGB(unsigned char *pYUV, unsigned char *pRGB,int width,int height)
{
	if (NULL == pYUV || pRGB == NULL) return;
	//Get Y/U/V Pointer from YUV frame data
	unsigned char * pYbuf = pYUV;							//Pointer to Y
	unsigned char * pYbufT;
	unsigned char * pVbuf = pYUV + width * height;			//Pointer to U
	unsigned char * pVbufT;
	unsigned char * pUbuf = pVbuf + (width * height >> 2);  //Pointer to V
	unsigned char * pUbufT;

	initYUVTable();
	int k = 0;
	int offset = 0;
	int Y,U,V;
	for(int i = 0;i < height;++ i)
	{
		offset = (i >> 1) * (width >> 1);
		pYbufT = pYbuf + (offset << 2);
		pUbufT = pUbuf + offset;
		pVbufT = pVbuf + offset;
		for(int j = 0;j < width;++j)
		{
			//Y = 298 * (*(pYbufT + j) - 16);
			//U = *(pUbufT + (j >> 1)) - 128;
			//V = *(pVbufT + (j >> 1)) - 128;
			//pRGB[k + 0] = CLIP(GET_R(Y, U, V));		//R
			//pRGB[k + 1] = CLIP(GET_G(Y, U, V));		//G
			//pRGB[k + 2] = CLIP(GET_B(Y, U, V));		//B
			//Y = *(pYbufT + j);
			//U = *(pUbufT + (j >> 1));
			//V = *(pVbufT + (j >> 1));
			//pRGB[k + 0] = CLIP((Y + 1.4075 * (V-128)));
			//pRGB[k + 1] = CLIP((Y - 0.3455 * (U - 128) - 0.7169 * (V - 128)));
			//pRGB[k + 2] = CLIP((Y + 1.779 * (U - 128)));
			Y = *(pYbufT + j);
			U = *(pUbufT + (j >> 1));
			V = *(pVbufT + (j >> 1));
			pRGB[k + 0] = CLIP(tableY[Y] + tableVR[V]);
			pRGB[k + 1] = CLIP(tableY[Y] - tableVG[V] - tableUG[U]);
			pRGB[k + 2] = CLIP(tableY[Y] + tableUB[U]);
			k += 3;
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

            U++;
            V++;
            p += -wrap3 + 2 * 3;
            Y += -wrap + 2;
        }
        p += wrap3;
        Y += wrap;
    }
}
inline byte ADJUST(double tmp)
{
    return (byte)((tmp >= 0 && tmp <= 255)?tmp:(tmp < 0 ? 0 : 255));
}
void YUV420P2RGB(unsigned char *Y,unsigned char *U,unsigned char *V, unsigned char *pRGB,int width,int height)
//void YUV420P_TO_RGB24(unsigned char* yuv_src,unsigned char* rgb_dst,int nWidth,int nHeight)
{
    unsigned char y,u,v,r,g,b;
    int C,D,E;
    int rgb_width , u_width;
    rgb_width = width * 3;
    u_width = (width >> 1);
//	int ypSize = width * height;
//    int upSize = (ypSize>>2);
    int offSet = 0;

    for(int i = 0; i < height;++ i)
    {
        for(int j = 0; j < width;++ j)
        {
            // Get the Y value from the y planar
            y = *(Y + width * i + j);
            // Get the V value from the u planar
            offSet = (i>>1) * (u_width) + (j>>1);
            v = *(U + offSet);
            // Get the U value from the v planar
            u = *(V + offSet);
            
            // Cacular the R,G,B values
            /*R = ADJUST((Y + (1.4075 * (V - 128))));
            G = ADJUST((Y - (0.3455 * (U - 128) - 0.7169 * (V - 128))));
            B = ADJUST((Y + (1.7790 * (U - 128))));*/
            
            // The following formulas are from MicroSoft' MSDN
            // the result is better than the former
            //
            C = y - 16;
            D = u - 128;
            E = v - 128;
            
            r = ADJUST(( 298 * C           + 409 * E + 128) >> 8);
            g = ADJUST(( 298 * C - 100 * D - 208 * E + 128) >> 8);
            b = ADJUST(( 298 * C + 516 * D           + 128) >> 8);
            r = ((r - 128) * 0.6 + 128) > 255 ? 255 : (r - 128) * 0.6 + 128;  
            g = ((g - 128) * 0.6 + 128) > 255 ? 255 : (g - 128) * 0.6 + 128;  
            b = ((b - 128) * 0.6 + 128) > 255 ? 255 : (b - 128) * 0.6 + 128;  
            //COLOR_RGB rgb={R,G,B};
            //COLOR_HSL hsl;
            //RGBtoHSL((const COLOR_RGB *)&rgb,&hsl);
            //hsl.saturation =hsl.saturation>90?100:hsl.saturation+10;
            //hsl.luminance=hsl.luminance>90?100:hsl.luminance+10;
            //HSLtoRGB((const COLOR_HSL *)&hsl,&rgb);
            // Set the values

            //ns_r为R分量的统计记数
//            ns_r[R]++;
            //ns_g为G分量的统计记数
//            ns_g[G]++;
            //ns_b为B分量的统计记数
//            ns_b[B]++;
            offSet = rgb_width * i + j * 3;
            //tmpbuf[offSet] = R;
            //tmpbuf[offSet + 1] = G;
            //tmpbuf[offSet + 2] = B;

            pRGB[offSet] = b;
            pRGB[offSet + 1] = g;
            pRGB[offSet + 2] = r;
        }
    }
    /*
    for(i=0;i<256;i++) //计算R、G、B三分量的直方图分布
    {
        //ps_r[i]为R分量中i灰度级出现的概率
        ps_r[i]=ns_r[i]/((nWidth*nHeight)/1.0f);
        //ps_b[i]为G分量中i灰度级出现的概率
        ps_g[i]=ns_g[i]/((nWidth*nHeight)/1.0f);
        //ps_b[i]为B分量中i灰度级出现的概率
        ps_b[i]=ns_b[i]/((nWidth*nHeight)/1.0f);
    }
    for(i=0;i<256;i++)
    {
        //计算累计直方图分布
        temp_r[i]=(i>0?temp_r[i-1]:0)+ps_r[i];
        temp_g[i]=(i>0?temp_g[i-1]:0)+ps_g[i];
        temp_b[i]=(i>0?temp_b[i-1]:0)+ps_b[i];
        //累计分布取整，ns_r[]、ns_g[]、ns_b[]保存有计算出来的灰度映射关系
        ns_r[i]=(int)(255.0f*temp_r[i]+0.5f);
        ns_g[i]=(int)(255.0f*temp_g[i]+0.5f);
        ns_b[i]=(int)(255.0f*temp_b[i]+0.5f);
    }
    for(i = 0; i < nHeight; i++)
    {
        for(int j = 0; j < nWidth; j ++)
        {
            offSet = rgb_width * i + j * 3;
            rgb_dst[offSet]=ns_r[tmpbuf[offSet]] ;
            rgb_dst[offSet+ 1]=ns_r[tmpbuf[offSet + 1] ];
            rgb_dst[offSet+ 2]=ns_r[tmpbuf[offSet + 2]];
        }
    }
 */
}