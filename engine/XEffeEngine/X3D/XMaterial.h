#ifndef _JIA_XMATERIAL_
#define _JIA_XMATERIAL_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.6.3
//--------------------------------
namespace XE{
class XMaterial
{
private:
	float m_amblient[4];	//材质的环境颜色RGBA
	float m_diffuse[4];		//材质的散射颜色RGBA
	float m_specular[4];	//材质的镜面反射颜色RGBA
	float m_shininess;		//镜面反射指数
	float m_emission[4];	//材质的发射光颜色
	int m_colorIndex[3];	//环境颜色索引、散射颜色索引和镜面反射颜色索引
public:
	XMaterial()
		:m_shininess(128.0f)
	{
		m_amblient[0] = 0.4f;
		m_amblient[1] = 0.4f;
		m_amblient[2] = 0.4f;
		m_amblient[3] = 1.0f;

		m_diffuse[0] = 0.2f;
		m_diffuse[1] = 0.6f;
		m_diffuse[2] = 0.9f;
		m_diffuse[3] = 1.0f;

		m_specular[0] = 0.0f;
		m_specular[1] = 0.0f;
		m_specular[2] = 0.0f;
		m_specular[3] = 1.0f;

		m_emission[0] = 0.0f;
		m_emission[1] = 0.0f;
		m_emission[2] = 0.0f;
		m_emission[3] = 0.0f;
	}
	void setAmblient(float x,float y,float z,float w)
	{
		m_amblient[0] = x;
		m_amblient[1] = y;
		m_amblient[2] = z;
		m_amblient[3] = w;
	}
	void setAmblient(const XVec4& a) {setAmblient(a.x,a.y,a.z,a.w);}
	void setDiffuse(float x,float y,float z,float w)
	{
		m_diffuse[0] = x;
		m_diffuse[1] = y;
		m_diffuse[2] = z;
		m_diffuse[3] = w;
	}
	void setDiffuse(const XVec4& d) {setDiffuse(d.z,d.y,d.z,d.w);}
	void setSpecular(float x,float y,float z,float w)
	{
		m_specular[0] = x;
		m_specular[1] = y;
		m_specular[2] = z;
		m_specular[3] = w;
	}
	void setSpecular(const XVec4& s) {setSpecular(s.x,s.y,s.z,s.w);}
	void setEmission(float x,float y,float z,float w)
	{
		m_emission[0] = x;
		m_emission[1] = y;
		m_emission[2] = z;
		m_emission[3] = w;
	}
	void setEmission(const XVec4& e) {setEmission(e.x,e.y,e.z,e.w);}
	void setShininess(float shininess) {m_shininess = shininess;}
	void bindMaterial()
	{//材质正面属性的设置
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);//使材质色跟踪当前颜色
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,m_amblient);
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,m_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,m_specular);
		glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,m_shininess);
		glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,m_emission);
	}
	void unbindMaterial()
	{
		glDisable(GL_COLOR_MATERIAL);
	}
};
}
#endif //_JIA_XMATERIAL_
