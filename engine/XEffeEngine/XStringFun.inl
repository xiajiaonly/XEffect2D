inline int hexCharToInt(char p)	//16进制字符转int的数值
{
	if (p >= '0' && p <= '9') return p - '0';
	if (p >= 'a' && p <= 'f') return p - 'a' + 10;
	if (p >= 'A' && p <= 'F') return p - 'A' + 10;
	return -1;
}
inline XBool hexToBin(const char *in, unsigned char *out)
{
	if (in == NULL || out == NULL) return XFalse;
	int len = strlen(in);
	//if(len % 2 != 0) return XFalse;
	//int tmp0;
	//int tmp1;
	//for(int i = 0;i < len;i += 2)
	//{
	//	tmp0 = hexCharToInt(in[i]);
	//	if(tmp0 < 0) return XFalse;
	//	tmp1 = hexCharToInt(in[i + 1]);
	//	if(tmp1 < 0) return XFalse;
	//	out[i >> 1] = (tmp0 << 4) + tmp1;
	//}
	for (int i = 0; i < len; ++i)
	{
		if (i % 2 == 0)
			out[i >> 1] = hexCharToInt(in[i]) << 4;
		else
			out[i >> 1] += hexCharToInt(in[i]);
	}
	return XTrue;
}
inline std::string intToStr(int temp, int radix)
{
	char str[128];
	_itoa_s(temp, str, radix);	//itoa
	return str;
}
inline std::string floatToStr(float temp, int len)
{
	char str[128];
	_gcvt_s(str, temp, len);	//gcvt
	return str;
}