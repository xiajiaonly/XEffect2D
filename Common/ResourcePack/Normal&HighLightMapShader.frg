/*uniform sampler2D Texture_00;			//主贴图
uniform sampler2D normalTexture;		//法线贴图
uniform sampler2D highLightMapTexture;	//高光贴图

varying vec2 TexCoord;			//当前ps的位置

void main()
{
	vec3 highLightColor = texture2D(highLightMapTexture,TexCoord).rgb;
	vec3 normal = normalize(texture2D(normalTexture,TexCoord).rgb * 2.0 - 1.0);
	vec3 lightPos = normalize(gl_LightSource[1].position.xyz);
	float diffuse = max(dot(normal,lightPos),0.0);

	//vec3 color = highLightColor.r * diffuse * texture2D(Texture_00,TexCoord).rgb;
	vec3 color = diffuse * texture2D(Texture_00,TexCoord).rgb;
	vec4 color1 = texture2D(Texture_00,TexCoord).rgba;

	gl_FragColor = vec4(color,color1.a);
}
*/
uniform sampler2D Texture_00;	
uniform sampler2D normalTexture;		//法线贴图
//uniform sampler2D highLightMapTexture;	//高光贴图

varying vec2 TexCoord;			//当前ps的位置
varying vec3 lightDir;
varying vec3 viewDir;

void main()
{
	vec3 l = lightDir;	//=0
	float atten = max(0.0, 1.0 - dot(l, l));
	atten = 1.0;

	l = normalize(l);

	vec3 n = normalize(texture2D(normalTexture,TexCoord).rgb * 2.0 - 1.0);
	vec3 v = normalize(viewDir);	//=0
	vec3 h = normalize(l + v);		//=0

	float nDotL = max(0.0, dot(n, l));
	float nDotH = max(0.0, dot(n, h));
	//float power = (nDotL == 0.0) ? 0.0 : pow(nDotH, gl_FrontMaterial.shininess);
	float power = (nDotL == 0.0) ? 0.0 : nDotH;

	vec4 ambient = gl_FrontLightProduct[1].ambient * atten;
	vec4 diffuse = gl_FrontLightProduct[1].diffuse * nDotL * atten;
	vec4 specular = gl_FrontLightProduct[1].specular * power * atten;
	vec4 color = gl_FrontLightModelProduct.sceneColor + ambient + diffuse + specular;

	gl_FragColor = color * texture2D(Texture_00,TexCoord);
}