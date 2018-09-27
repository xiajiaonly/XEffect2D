//INLINE 函数的定义
INLINE void XCColor::setColor(unsigned char cr,unsigned char cg,unsigned char cb,unsigned char ca)
{
	 r = cr;
	g = cg;
	b = cb;
	a = ca;
}
INLINE void XCColor::randColor()
{
	r = XRand::randomf(127);
	g = XRand::randomf(127);
	b = XRand::randomf(127);
	a = XRand::randomf(127);
}
INLINE void XCColor::getHsb(float& hue, float& saturation, float& brightness) const 
{
	float tempMax = (std::max)(r,(std::max)(g,b));
	float tempMin = (std::min)(r,(std::min)(g,b));
	
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
INLINE float XCColor::getHue() const 
{
	float hue,saturation,brightness;
	getHsb(hue,saturation,brightness);
	return hue;
}