//inline 函数的定义
inline void _XCColor::setColor(unsigned char cr,unsigned char cg,unsigned char cb,unsigned char ca)
{
	 r = cr;
	g = cg;
	b = cb;
	a = ca;
}
inline void _XCColor::randColor()
{
	r = randomf(127);
	g = randomf(127);
	b = randomf(127);
	a = randomf(127);
}
inline void _XCColor::getHsb(float& hue, float& saturation, float& brightness) const 
{
	float tempMax = XEE_Max(r,XEE_Max(g,b));
	float tempMin = XEE_Min(r,XEE_Min(g,b));
	
	if(tempMax == tempMin) 
	{ // grays
		hue = 0.f;
		saturation = 0.f;
		brightness = tempMax;
		return;
	}
	
	float hueSixth;
	if(r == tempMax) 
	{
		hueSixth = (g - b) / (tempMax - tempMin);
		if(hueSixth < 0.f) hueSixth += 6.f;
	}else 
	if(g == tempMax) hueSixth = 2.f + (b - r) / (tempMax - tempMin);
	else hueSixth = 4.f + (r - g) / (tempMax - tempMin);

	hue = limit() * hueSixth / 6.f;
	saturation = limit() * (tempMax - tempMin) / tempMax;
	brightness = tempMax;
}
inline float _XCColor::getHue() const 
{
	float hue,saturation,brightness;
	getHsb(hue,saturation,brightness);
	return hue;
}