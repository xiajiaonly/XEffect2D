#version 130
uniform sampler2D Texture_00;	
varying vec2 TexCoord;

uniform vec2 tcOffset[25];	//取点的点阵

void main()
{
	vec2 size = textureSize(Texture_00,0);
	vec2 temp = TexCoord;
//	float dx = 1.0 / 2048.0;
//	float dy = 1.0 / 1024.0;
	vec3 sample[25];
	 
	for (int i = 0; i < 25; i++)
	{//一圈采样
		//Sample a grid around and including our texel
		//sample[i] = texture2D(Texture_00,temp + tcOffset[i] * vec2(dx,dy)).stp;
		sample[i] = texture2D(Texture_00,temp + tcOffset[i]).stp;
	}
 
	// Gaussian weighting:
	// 1  4  7  4 1
	// 4 16 26 16 4
	// 7 26 41 26 7 / 273 (i.e. divide by total of weightings)
	// 4 16 26 16 4
	// 1  4  7  4 1
 
	vec3 color = (
		(1.0  * (sample[0] + sample[4]  + sample[20] + sample[24])) +
		(4.0  * (sample[1] + sample[3]  + sample[5]  + sample[9] + sample[15] + sample[19] + sample[21] + sample[23])) +
		(7.0  * (sample[2] + sample[10] + sample[14] + sample[22])) +
		(16.0 * (sample[6] + sample[8]  + sample[16] + sample[18])) +
		(26.0 * (sample[7] + sample[11] + sample[13] + sample[17])) +
		(41.0 * sample[12])
		) / 273.0;

	
	gl_FragColor = vec4(color,1.0);
}