uniform sampler2D Texture_00;	//原始贴图	
uniform sampler2D Texture_01;	//光贴图
uniform sampler2D Texture_02;	//使能贴图

uniform float power;	//强度
uniform float size;

varying vec2 TexCoord;

void main()
{
	vec4 color0 = texture2D(Texture_00,TexCoord).rgba;
	vec4 color2 = texture2D(Texture_02,TexCoord).rgba;
	if(color2.a > 0.01)	//防止光斑上岸
	{
		vec2 temp = TexCoord * size;
		temp = fract(temp);
	//	temp.x = fract(temp.x);
	//	temp.y = fract(temp.y);
		vec4 color1 = texture2D(Texture_01,temp).rgba;
		float tempAlpha = color2.a * 10.0;
		if(tempAlpha > 1.0) tempAlpha = 1.0;
		color0 = color0 * (color1.r * 0.4 + power) * tempAlpha + 
			color0 * (1.0 - tempAlpha);	// 0.8
		//color0 = color0 * (color1.r * 0.8 + 0.8);
		color0.a = 1.0;
	}

	gl_FragColor = color0;
}