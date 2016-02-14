namespace XString
{
	inline int hexCharToInt(char p)	//16进制字符转int的数值
	{
		if(p >= '0' && p <= '9') return p - '0';
		if(p >= 'a' && p <= 'f') return p - 'a' + 10;
		if(p >= 'A' && p <= 'F') return p - 'A' + 10;
		return -1;
	}
	inline XBool hexToBin(const char *in,unsigned char *out)
	{
		if(in == NULL || out == NULL) return XFalse;
		int len = 0;
		if((len = strlen(in)) %2 != 0) return XFalse;
		int tmp0;
		int tmp1;
		for(int i = 0;i < len;i += 2)
		{
			tmp0 = hexCharToInt(in[i]);
			if(tmp0 < 0) return XFalse;
			tmp1 = hexCharToInt(in[i + 1]);
			if(tmp1 < 0) return XFalse;
			out[i >> 1] = (tmp0 << 4) + tmp1;
		}
		return XTrue;
	}
	inline std::string intToStr(int temp,int radix)
	{
		char str[128];
		itoa(temp,str,radix);	//itoa
		return str;
	}
	inline std::string floatToStr(float temp,int len)
	{
		char str[128];
		gcvt(temp,len,str);	//gcvt
		return str;
	}
	inline wchar_t oneCharToWchar(const char* src)
	{
		if(src == NULL) return NULL;
		wchar_t ret = 0;
		MultiByteToWideChar(CP_ACP, 0, src, 2,&ret, 1);
		return ret;
	}
	inline std::string Wchar2ANSI(const wchar_t* src)
	{
		if(src == NULL) return NULL;
		int nRetLen = WideCharToMultiByte(CP_ACP, 0,src, -1, NULL, 0, NULL, NULL);
 		std::string ret;
		ret.resize(nRetLen + 1);
		WideCharToMultiByte(CP_ACP, 0, src, -1, &ret[0], nRetLen, NULL, NULL);
		return ret;
	}
}