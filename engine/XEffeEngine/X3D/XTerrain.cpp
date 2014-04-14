//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "glew.h"
#include "XTerrain.h"
#include "X3DWorld.h"
#include "XBasicWindow.h"
#include "XResourcePack.h"

_XBool _XTerrain::init(const char *mapFilename,const char *texFilename,int w,int h,
					   _XBool withColor,
					   _XResourcePosition resourcePosition)
{
	if(m_isInited) return XTrue;
	if(mapFilename == NULL) return XFalse;
	if(w <= 0 || h <= 0) return XFalse;

	m_width = w;
	m_height = h;
	m_withColor = withColor;
	unsigned char *tempTerrainData = createArrayMem<unsigned char>(m_width * m_height);
	if(tempTerrainData == NULL) 
	{
		XDELETE_ARRAY(tempTerrainData);
		return XFalse;
	}
	if(resourcePosition == RESOURCE_SYSTEM_DEFINE) resourcePosition = XEE::defaultResourcePosition;
	if(resourcePosition  == RESOURCE_LOCAL_PACK)
	{
		unsigned char *p = _XResourcePack::GetInstance().getFileData(mapFilename);
		if(p == NULL) return XFalse;
		memcpy(tempTerrainData,p,m_width * m_height);

		XDELETE_ARRAY(p);
	}else
	if(resourcePosition == RESOURCE_LOCAL_FOLDER)
	{
		FILE *fp = NULL;
		if((fp = fopen(mapFilename,"rb")) == NULL) return XFalse;	//打开数据文件失败

		fread(tempTerrainData,1,m_width * m_height,fp);
		if(ferror(fp))
		{
			fclose(fp);
			XDELETE_ARRAY(tempTerrainData);
			return XFalse;
		}
		fclose(fp);
	}
//	//写入数据 For Test
//	if((fp = fopen("Test.dat","wb")) == NULL) return XFalse;	//打开数据文件失败
//	for(int i = 0;i < h;i += 2)
//	{
//		for(int j = 0;j < w;j += 2)
//		{
//			fwrite(&tempTerrainData[i * h + j],1,sizeof(char),fp);
//		}
//	}
//	fclose(fp);
	m_terrainData = createArrayMem<float>(m_width * m_height);
	if(m_terrainData == NULL)
	{
		XDELETE_ARRAY(m_terrainData);
		return XFalse;
	}
	//数据类型转换
	for(int i = 0;i < m_width * m_height;++ i)
	{
		m_terrainData[i] = tempTerrainData[i];
	}
	XDELETE_ARRAY(tempTerrainData);	//删除临时数据
	//载入贴图数据
	if(texFilename == NULL) m_withTexture = XFalse;
	else 
	{
		m_withTexture = XTrue;
		if(!m_texture.load(texFilename,resourcePosition)) 
		{
			XDELETE_ARRAY(m_terrainData);
			return XFalse;
		}
	}

	m_position.set(0.0f,0.0f,0.0f);
	m_size.set(1.0f,1.0f,1.0f);
	m_angle.set(0.0f,0.0f,0.0f);
	//下面建立VBO
	m_allFaceSum = (m_width - 1) * (m_height - 1) * 2;
	m_allPointSum = m_width * m_height;
	m_index = createArrayMem<unsigned int>(m_allFaceSum * 3);
	if(m_index == NULL)
	{
		releaseMemory();
		return XFalse;
	}
	if(withColor)
	{
		m_c = createArrayMem<float>(m_allPointSum * 4);
		if(m_c == NULL)
		{
			releaseMemory();
			return XFalse;
		}
	}
	m_v = createArrayMem<float>(m_allPointSum * 3);	//顶点
	m_n = createArrayMem<float>(m_allPointSum * 3);	//法线
	m_faceN = createArrayMem<_XVector3>(m_allFaceSum);	//面法线
	if(m_n == NULL || m_v == NULL || m_faceN == NULL)// || m_pointN == NULL)
	{
		releaseMemory();
		return XFalse;
	}
	if(m_withTexture)
	{
		m_t = createArrayMem<float>(m_allPointSum * 2);
		if(m_t == NULL)
		{
			releaseMemory();
			return XFalse;
		}
	}
	m_coordOffsetX = 1.0f / m_width;
	m_coordOffsetY = 1.0f / m_height;
//	int tempIndex = 0;
	int indexT = 0;
	int indexV = 0;
	int indexC = 0;
	float nowCoordX = 0.0f;
	float nowCoordY = 0.0f;
	int i,j;
	
	int faceIndex = 0;
	int indexI = 0;
	for(j = 0;j < m_height;++ j,nowCoordY += m_coordOffsetY)
	{
		indexT = j * m_width * 2;
		indexV = j * m_width * 3;
		indexC = j * m_width * 4;
		indexI = j * m_width;
		faceIndex = j * (m_width - 1) * 2 * 3;
		for(i = 0,nowCoordX = 0.0f;i < m_width;++i)
		{
			if(m_withTexture)
			{
				m_t[indexT] = nowCoordX;
				m_t[indexT + 1] = nowCoordY;
				indexT += 2;
				nowCoordX += m_coordOffsetX;
			}
			m_v[indexV] = i;
			m_v[indexV + 2] = j;
			indexV += 3;
			if(m_withColor)
			{
				m_c[indexC] = 1.0f;
				m_c[indexC + 1] = 1.0f;
				m_c[indexC + 2] = 1.0f;
				m_c[indexC + 3] = m_reflectRate;
				indexC += 4;
			}
		}
		if(j < m_height - 1)
		{
			for(i = 0;i < m_width - 1;++i)
			{
				m_index[faceIndex] = indexI;
				m_index[faceIndex + 1] = indexI + m_width;
				m_index[faceIndex + 2] = indexI + 1;
				m_index[faceIndex + 3] = indexI + 1;
				m_index[faceIndex + 4] = indexI + m_width;
				m_index[faceIndex + 5] = indexI + 1 + m_width;

				faceIndex += 2 * 3;
				++indexI;
			}
		}
	}
	
	m_isInited = XTrue;
	if(!updateHeight(m_terrainData))
	{
		releaseMemory();
		m_isInited = XFalse;
		return XFalse;
	}

	return XTrue;
}
_XBool _XTerrain::init(const float *mapData,const char * texFilename,int w,int h,
		_XBool withColor,_XResourcePosition resourcePosition)	//直接从数组中读取数据
{
	if(m_isInited) return XTrue;
	if(mapData == NULL) return XFalse;
	if(w <= 0 || h <= 0) return XFalse;

	m_width = w;
	m_height = h;
	m_withColor = withColor;
	if(resourcePosition == RESOURCE_SYSTEM_DEFINE) resourcePosition = XEE::defaultResourcePosition;
//	//写入数据 For Test
//	if((fp = fopen("Test.dat","wb")) == NULL) return XFalse;	//打开数据文件失败
//	for(int i = 0;i < h;i += 2)
//	{
//		for(int j = 0;j < w;j += 2)
//		{
//			fwrite(&tempTerrainData[i * h + j],1,sizeof(char),fp);
//		}
//	}
//	fclose(fp);
	m_terrainData = createArrayMem<float>(m_width * m_height);
	if(m_terrainData == NULL)
	{
		XDELETE_ARRAY(m_terrainData);
		return XFalse;
	}
	memcpy(m_terrainData,mapData,sizeof(float) * m_width * m_height);
	//载入贴图数据
	if(texFilename == NULL) m_withTexture = XFalse;
	else 
	{
		m_withTexture = XTrue;
		if(!m_texture.load(texFilename,resourcePosition)) 
		{
			XDELETE_ARRAY(m_terrainData);
			return XFalse;
		}
	}

	m_position.set(0.0f,0.0f,0.0f);
	m_size.set(1.0f,1.0f,1.0f);
	m_angle.set(0.0f,0.0f,0.0f);
	//下面建立VBO
	m_allFaceSum = (m_width - 1) * (m_height - 1) * 2;
	m_allPointSum = m_width * m_height;
	m_index = createArrayMem<unsigned int>(m_allFaceSum * 3);
	if(m_index == NULL)
	{
		releaseMemory();
		return XFalse;
	}
	if(withColor)
	{
		m_c = createArrayMem<float>(m_allPointSum * 4);
		if(m_c == NULL)
		{
			releaseMemory();
			return XFalse;
		}
	}
	m_v = createArrayMem<float>(m_allPointSum * 3);	//顶点
	m_n = createArrayMem<float>(m_allPointSum * 3);	//法线
	m_faceN = createArrayMem<_XVector3>(m_allFaceSum);	//面法线
	if(m_n == NULL || m_v == NULL || m_faceN == NULL)// || m_pointN == NULL)
	{
		releaseMemory();
		return XFalse;
	}
	if(m_withTexture)
	{
		m_t = createArrayMem<float>(m_allPointSum * 2);
		if(m_t == NULL)
		{
			releaseMemory();
			return XFalse;
		}
	}
	m_coordOffsetX = 1.0f / m_width;
	m_coordOffsetY = 1.0f / m_height;
//	int tempIndex = 0;
	int indexT = 0;
	int indexV = 0;
	int indexC = 0;
	float nowCoordX = 0.0f;
	float nowCoordY = 0.0f;
	int i,j;

	int faceIndex = 0;
	int indexI = 0;
	for(j = 0;j < m_height;++ j,nowCoordY += m_coordOffsetY)
	{
		indexT = j * m_width * 2;
		indexV = j * m_width * 3;
		indexC = j * m_width * 4;
		indexI = j * m_width;
		faceIndex = j * (m_width - 1) * 2 * 3;
		for(i = 0,nowCoordX = 0.0f;i < m_width;++i)
		{
			if(m_withTexture)
			{
				m_t[indexT] = nowCoordX;
				m_t[indexT + 1] = nowCoordY;
				indexT += 2;
				nowCoordX += m_coordOffsetX;
			}
			m_v[indexV] = i;
			m_v[indexV + 2] = j;
			indexV += 3;
			if(m_withColor)
			{
				m_c[indexC] = 1.0f;
				m_c[indexC + 1] = 1.0f;
				m_c[indexC + 2] = 1.0f;
				m_c[indexC + 3] = m_reflectRate;
				indexC += 4;
			}
		}
		if(j < m_height - 1)
		{
			for(i = 0;i < m_width - 1;++i)
			{
				m_index[faceIndex] = indexI;
				m_index[faceIndex + 1] = indexI + m_width;
				m_index[faceIndex + 2] = indexI + 1;
				m_index[faceIndex + 3] = indexI + 1;
				m_index[faceIndex + 4] = indexI + m_width;
				m_index[faceIndex + 5] = indexI + 1 + m_width;

				faceIndex += 2 * 3;
				++indexI;
			}
		}
	}
	m_isInited = XTrue;
	if(!updateHeight(m_terrainData))
	{
		releaseMemory();
		m_isInited = XFalse;
		return XFalse;
	}

	return XTrue;
}
void _XTerrain::draw(_XBool withTex)
{
	if(!m_isInited) return;
	if(m_faceSum <= 0) return;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_CULL_FACE);
	if(m_withTexGen)
	{
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
	}
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(m_position.x,m_position.y,m_position.z);
    glRotatef(m_angle.x,1,0,0);
    glRotatef(m_angle.y,0,1,0);
    glRotatef(m_angle.z,0,0,1);
	glScalef(m_size.x,m_size.y,m_size.z);

	_X3DWorld::GetInstance().m_worldMaterial.usetMaterial();
	if(m_withTexture && withTex)
	{
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
	//	glDisable(GL_BLEND);
	//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE);	//光照与纹理进行混合
		glBindTexture(GL_TEXTURE_2D,m_texture.texture.m_texture);
	}else
	{
	//	glDisable(GL_TEXTURE_2D);
	//	glDisable(GL_BLEND);
	}
	_X3DWorld::GetInstance().useShadow(m_withTexture && withTex,SHADER_SHADOW);
	if(m_withTexGen)
	{
		glEnable(GL_BLEND);
		glDisable(GL_LIGHTING);
		glColor4f(1.0f,1.0f,1.0f,m_reflectRate);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	if(m_pShader != NULL) m_pShader->useShader();

	m_vbo.use(m_withTexture && withTex);
	glDrawElements(GL_TRIANGLES,m_vbo.getIndexSize() * 3,GL_UNSIGNED_INT,0);	//m_index
	//glDrawElements(GL_TRIANGLES,m_faceSum * 3,GL_UNSIGNED_INT,0);	//m_index
	m_vbo.disuse();

	if(m_pShader != NULL) m_pShader->disShader();
	glPopMatrix();
	if(m_withTexGen)
	{
		glDisable(GL_BLEND);
		glEnable(GL_LIGHTING);
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
	}
	glPopAttrib();
//	glEnable(GL_BLEND); 
//	glDisable(GL_TEXTURE_2D);
}
_XBool _XTerrain::updateVBO(bool v,bool n,bool c,bool t,bool i)
{
	if(!m_isInited) return XFalse;

	int sum;
	sum = m_allPointSum;
	if(v) m_vbo.updateDataV(sum,m_v);
	if(n) m_vbo.updateDataN(sum,m_n);
	if(c) m_vbo.updateDataC(sum,m_c);
	if(t) m_vbo.updateDataT(sum,m_t);
	if(i) m_vbo.updateDataI(sum,m_index);

	return true;
}
_XBool _XTerrain::updateByHeightAndNormal(const float *heights,const float *normal)
{
	if(!m_isInited) return XFalse;
	if(heights == NULL || normal == NULL) return XFalse;
	int i,j;
	for(i = 0,j = 1;i < m_allPointSum;++ i,j += 3)
	{
		m_v[j] = heights[i];
	}

	memcpy(m_n,normal,m_height * m_width * 3 * sizeof(float));
	m_faceSum = m_allFaceSum;
	//m_newH = heights;
	updateVBO(true,true);//本身不绘制，所以可以屏蔽，再3D流水项目中,消耗大
	//memcpy(m_terrainData,heights,sizeof(float) * m_width * m_height);
	return true;
}
_XBool _XTerrain::updateByFlag(const bool *flags,const float * w)
{
	if(!m_isInited) return XFalse;
	if(flags ==NULL) return XFalse;
	if(w == NULL) return XFalse;
	int cIndex = 0;
	int j,i;
	int hIndex,index;
	m_faceSum = 0;
	for(j = 0,index = 0;j < m_height - 1;++ j)
	{
		hIndex = j * m_width;
		cIndex = (hIndex << 2) + 3;
		for(i = 0;i < m_width - 1;++ i,++ hIndex,cIndex += 4)
		{
			if(flags[hIndex + 1] || flags[hIndex + m_width])
			{//上半个面
				m_index[index] = hIndex; 
				m_index[index + 1] = hIndex + 1; 
				m_index[index + 2] = hIndex + m_width; 
				m_index[index + 3] = hIndex + 1; 
				m_index[index + 4] = hIndex + m_width; 
				m_index[index + 5] = hIndex + 1 + m_width;
				if(flags[hIndex]) m_c[cIndex] = w[hIndex];//m_reflectRate * w[hIndex];
				else m_c[cIndex] = 0.0f;
				if(flags[hIndex + 1]) m_c[cIndex + 4] = w[hIndex + 1];//m_reflectRate * w[hIndex + 1];
				else m_c[cIndex + 4] = 0.0f;
				if(flags[hIndex + m_width]) m_c[cIndex + (m_width << 2)] = w[hIndex + m_width];//m_reflectRate * w[hIndex + m_width];
				else m_c[cIndex + (m_width << 2)] = 0.0f;
				if(flags[hIndex + 1 + m_width]) m_c[cIndex + 4 + (m_width << 2)] = w[hIndex + 1 + m_width];//m_reflectRate * w[hIndex + 1 + m_width];
				else m_c[cIndex + 4 + (m_width << 2)] = 0.0f;
				m_faceSum += 2;
				index += 6;
			}else
			if(flags[hIndex])
			{
				m_index[index] = hIndex; 
				m_index[index + 1] = hIndex + 1; 
				m_index[index + 2] = hIndex + m_width;
				m_c[cIndex] = w[hIndex];//m_reflectRate * w[hIndex];
				m_c[cIndex + 4] = 0.0f;
				m_c[cIndex + (m_width << 2)] = 0.0f;
				++ m_faceSum;
				index += 3;
			}else
			if(flags[hIndex + m_width + 1])
			{//下半个面
				m_index[index] = hIndex + 1; 
				m_index[index + 1] = hIndex + m_width; 
				m_index[index + 2] = hIndex + 1 + m_width; 
				m_c[cIndex + 4] = 0.0f;
				m_c[cIndex + (m_width << 2)] = 0.0f;
				m_c[cIndex + 4 + (m_width << 2)] = w[hIndex + 1 + m_width];//m_reflectRate * w[hIndex + 1 + m_width];
				++ m_faceSum;
				index += 3;
			}
		}
	}
	m_vbo.updateDataC(m_allPointSum,m_c);

	return XTrue;
}
_XBool _XTerrain::updateByStep(int step)
{
	if(!m_isInited) return XFalse;
	if(step <= 0) return XFalse;
	int i,j,index,hIndex;
	m_faceSum = 0;
	int w = step * m_width;
	for(j = 0,index = 0;j < m_height - step;j += step)
	{
		hIndex = j * m_width;
		for(i = 0;i < m_width - step;i += step,hIndex += step)
		{
			m_index[index] = hIndex; 
			m_index[index + 1] = hIndex + step; 
			m_index[index + 2] = hIndex + w; 
			m_index[index + 3] = hIndex + step; 
			m_index[index + 4] = hIndex + w; 
			m_index[index + 5] = hIndex + step + w;
			m_faceSum += 2;
			index += 6;
		}
	}
	m_vbo.updateDataI(m_faceSum,m_index);
	return true;
}
_XBool _XTerrain::updateHeight(const float *heights,const bool *flags)
{//根据高度信息更新数据
	if(!m_isInited) return XFalse;
	//下面更具实际的数值填充数据
	_XVector3 tempNormal;
	int index = 0;
	int hIndex = 0;
	int lIndex = 0;
	int i,j;
	if(flags == NULL)
	{
		if(m_terrainData == heights)
		{
			for(j = 0;j < m_height;++ j)
			{
				hIndex = j * m_width;
				index = hIndex * 3;
				for(i = 0;i < m_width;++ i)
				{
					m_v[index + 1] = heights[hIndex];
					index += 3;
					++ hIndex;
				}
			}
			//下面计算面法线
			for(i = 0,j = 0;i < m_allFaceSum;++ i,j += 3)
			{
				m_faceN[i] = getNormal(m_v + m_index[j] * 3,
					m_v + m_index[j + 1] * 3,
					m_v + m_index[j + 2] * 3);
			}
			m_faceSum = m_allFaceSum;
			updateNormal(heights);//这里考虑对法线数据进行平滑处理
			//updateVBO(true,true,true,true);
			
			if(m_withTexture) m_vbo.updateDate(m_allPointSum,m_v,m_t,m_n,m_c,m_faceSum,m_index);
			else m_vbo.updateDate(m_allPointSum,m_v,NULL,m_n,m_c,m_faceSum,m_index);
		}else
		{
			for(i = 0,j = 1;i < m_allPointSum;++ i,j += 3)
			{
				m_v[j] = heights[i];
			}
			m_newH = heights;
			for(j = 0;j < m_height - 1;++ j)
			{
				lIndex = j * m_width;
				index = (lIndex - j) * 2;
				for(i = 0;i < m_width - 1;++ i,++lIndex,index += 2)
				{
					if(m_terrainData[lIndex + 1] != heights[lIndex + 1]
						|| m_terrainData[lIndex + m_width] != heights[lIndex + m_width])
					{
						m_faceN[index] = getMyNormal(lIndex,true);
						m_faceN[index + 1] = getMyNormal(lIndex,false);
					}else
					if(m_terrainData[lIndex] != heights[lIndex])
					{
						m_faceN[index] = getMyNormal(lIndex,true);
					}else
					if(m_terrainData[lIndex + m_width + 1] != heights[lIndex + m_width + 1])
					{
						m_faceN[index] = getMyNormal(lIndex,false);
					}
				}
			}

			m_faceSum = m_allFaceSum;
			updateNormal(heights);//这里考虑对法线数据进行平滑处理,消耗大
			updateVBO(true,true);	//本身不绘制，所以可以屏蔽，再3D流水项目中,消耗大
		}
		memcpy(m_terrainData,heights,sizeof(float) * m_width * m_height);	//目前这个数据可以不用更新
	}else
	{
		//m_faceSum = 0;
//		int tIndex = 0;
		if(m_withColor)
		{
			for(j = 0;j < m_height;++ j)
			{
				hIndex = j * m_width;
				index = hIndex * 3;
				lIndex = (hIndex << 2) + 3;
				for(i = 0;i < m_width;++ i)
				{
					m_v[index + 1] = heights[hIndex];
					if(flags[hIndex]) m_c[lIndex] = m_reflectRate;
					else m_c[lIndex] = 0;
					index += 3;
					++ hIndex;
					lIndex += 4;
				}
			}
		}else
		{
			for(j = 0;j < m_height;++ j)
			{
				hIndex = j * m_width;
				index = hIndex * 3;
				for(i = 0;i < m_width;++ i)
				{
					m_v[index + 1] = heights[hIndex];
					index += 3;
					++ hIndex;
				}
			}		
		}
		//下面计算面法线
		static int p0,p1,p2;
		for(i = 0,j = 0;i < m_allFaceSum;++ i,j += 3)
		{
			p0 = m_index[j];p1 = m_index[j + 1];p2 = m_index[j + 2];	//这个数值发生了变化
			if((flags[p0] || flags[p1] || flags[p2])	//需要改变并且数值发生变化
				&& (m_terrainData[p0] != heights[p0] ||
				m_terrainData[p1] != heights[p1] ||
				m_terrainData[p2] != heights[p2]))
			{
				m_faceN[i] = getNormal(m_v + p0 * 3,m_v + p1 * 3,m_v + p2 * 3);
			}
		}
		//for(j = 0,index = 0;j < m_height - 1;++ j)
		//{
		//	hIndex = j * m_width;
		//	for(i = 0;i < m_width - 1;++ i,++ hIndex)
		//	{
		//		if(flags[hIndex] || flags[hIndex + 1] || flags[hIndex + m_width])
		//		{//上半个面
		//			m_index[index] = hIndex; 
		//			m_index[index + 1] = hIndex + 1; 
		//			m_index[index + 2] = hIndex + m_width; 

		//			++ m_faceSum;
		//			index += 3;
		//		}
		//		if(flags[hIndex + 1] || flags[hIndex + m_width] || flags[hIndex + m_width + 1])
		//		{//下半个面
		//			m_index[index] = hIndex + 1; 
		//			m_index[index + 1] = hIndex + m_width; 
		//			m_index[index + 2] = hIndex + 1 + m_width; 

		//			++ m_faceSum;
		//			index += 3;
		//		}
		//	}
		//}
		updateNormal(heights);
		if(m_withColor) updateVBO(true,true,true);
		else updateVBO(true,true);
		
		memcpy(m_terrainData,heights,sizeof(float) * m_width * m_height);	//目前这个数据可以不用更新
	}
	return XTrue;
}
void _XTerrain::updateNormal(const float *heights)
{
	static int faceSumW = (m_width - 1) * 2;
	static _XVector3 tempPointN;
	static int upIndex = 0;
	static int hIndex = 0;
	static int index = 0,i,j;
	static int lIndex = 0;
	if(heights == m_terrainData)
	{
		for(j = 0;j < m_height;++ j)
		{
			hIndex = j * m_width;
			index = (hIndex - j) << 1;
			upIndex = index - faceSumW;
			hIndex *= 3;
			for(i = 0;i < m_width;++ i,hIndex += 3,index += 2,upIndex += 2)
			{
				if(i == 0)
				{
					if(j == 0)
					{//角 1	不需要计算
						tempPointN = m_faceN[index];
						m_n[hIndex] = tempPointN.x;
						m_n[hIndex + 1] = tempPointN.y;
						m_n[hIndex + 2] = tempPointN.z;
					}else
					if(j == m_height - 1)
					{//角 2
						tempPointN = (m_faceN[upIndex] + m_faceN[upIndex + 1]) * 0.5;
						m_n[hIndex] = tempPointN.x;
						m_n[hIndex + 1] = tempPointN.y;
						m_n[hIndex + 2] = tempPointN.z;
					}else
					{//边 3
						tempPointN = (m_faceN[upIndex] + m_faceN[upIndex + 1] + m_faceN[index]) * 0.33333333333f;
						m_n[hIndex] = tempPointN.x;
						m_n[hIndex + 1] = tempPointN.y;
						m_n[hIndex + 2] = tempPointN.z;
					}
				}else
				if(i == m_width - 1)
				{
					if(j == 0)
					{//角 2
						tempPointN = (m_faceN[index - 1] + m_faceN[index - 2]) * 0.5f;
						m_n[hIndex] = tempPointN.x;
						m_n[hIndex + 1] = tempPointN.y;
						m_n[hIndex + 2] = tempPointN.z;
					}else
					if(j == m_height - 1)
					{//角 1
						tempPointN = m_faceN[upIndex - 1];
						m_n[hIndex] = tempPointN.x;
						m_n[hIndex + 1] = tempPointN.y;
						m_n[hIndex + 2] = tempPointN.z;
					}else
					{//边 3
						tempPointN = (m_faceN[upIndex - 1] + m_faceN[index - 1] + m_faceN[index - 2]) * 0.33333333333f;
						m_n[hIndex] = tempPointN.x;
						m_n[hIndex + 1] = tempPointN.y;
						m_n[hIndex + 2] = tempPointN.z;
					}
				}else
				{
					if(j == 0)
					{//边 3
						tempPointN = (m_faceN[index] + m_faceN[index - 1] + m_faceN[index - 2]) * 0.33333333333f;
						m_n[hIndex] = tempPointN.x;
						m_n[hIndex + 1] = tempPointN.y;
						m_n[hIndex + 2] = tempPointN.z;
					}else
					if(j == m_height - 1)
					{//边 3
						tempPointN = (m_faceN[upIndex] + m_faceN[upIndex - 1] + m_faceN[upIndex + 1]) * 0.33333333333f;
						m_n[hIndex] = tempPointN.x;
						m_n[hIndex + 1] = tempPointN.y;
						m_n[hIndex + 2] = tempPointN.z;
					}else
					{//体 6
						tempPointN = (m_faceN[upIndex - 1] + m_faceN[upIndex] + m_faceN[upIndex + 1] +
							m_faceN[index - 2] + m_faceN[index - 1] + m_faceN[index]) * 0.16666666666f;
						m_n[hIndex] = tempPointN.x;
						m_n[hIndex + 1] = tempPointN.y;
						m_n[hIndex + 2] = tempPointN.z;
					}
				}
			}
		}
	}else
	{
		for(j = 0;j < m_height;++ j)
		{
			lIndex = j * m_width;
			hIndex = lIndex * 3;
			index = (lIndex - j) << 1;
			upIndex = index - faceSumW;
			for(i = 0;i < m_width;++ i,++lIndex,hIndex += 3,index += 2,upIndex += 2)
			{
				if(heights[lIndex] == m_terrainData[lIndex]) continue;	//数据没有发生变化则直接返回
				if(i == 0)
				{
					if(j == 0)
					{//角 1	不需要计算
						tempPointN = m_faceN[index];
						m_n[hIndex] = tempPointN.x;
						m_n[hIndex + 1] = tempPointN.y;
						m_n[hIndex + 2] = tempPointN.z;
					}else
					if(j == m_height - 1)
					{//角 2
						tempPointN = (m_faceN[upIndex] + m_faceN[upIndex + 1]) * 0.5;
						m_n[hIndex] = tempPointN.x;
						m_n[hIndex + 1] = tempPointN.y;
						m_n[hIndex + 2] = tempPointN.z;
					}else
					{//边 3
						tempPointN = (m_faceN[upIndex] + m_faceN[upIndex + 1] + m_faceN[index]) * 0.33333333333f;
						m_n[hIndex] = tempPointN.x;
						m_n[hIndex + 1] = tempPointN.y;
						m_n[hIndex + 2] = tempPointN.z;
					}
				}else
				if(i == m_width - 1)
				{
					if(j == 0)
					{//角 2
						tempPointN = (m_faceN[index - 1] + m_faceN[index - 2]) * 0.5f;
						m_n[hIndex] = tempPointN.x;
						m_n[hIndex + 1] = tempPointN.y;
						m_n[hIndex + 2] = tempPointN.z;
					}else
					if(j == m_height - 1)
					{//角 1
						tempPointN = m_faceN[upIndex - 1];
						m_n[hIndex] = tempPointN.x;
						m_n[hIndex + 1] = tempPointN.y;
						m_n[hIndex + 2] = tempPointN.z;
					}else
					{//边 3
						tempPointN = (m_faceN[upIndex - 1] + m_faceN[index - 1] + m_faceN[index - 2]) * 0.33333333333f;
						m_n[hIndex] = tempPointN.x;
						m_n[hIndex + 1] = tempPointN.y;
						m_n[hIndex + 2] = tempPointN.z;
					}
				}else
				{
					if(j == 0)
					{//边 3
						tempPointN = (m_faceN[index] + m_faceN[index - 1] + m_faceN[index - 2]) * 0.33333333333f;
						m_n[hIndex] = tempPointN.x;
						m_n[hIndex + 1] = tempPointN.y;
						m_n[hIndex + 2] = tempPointN.z;
					}else
					if(j == m_height - 1)
					{//边 3
						tempPointN = (m_faceN[upIndex] + m_faceN[upIndex - 1] + m_faceN[upIndex + 1]) * 0.33333333333f;
						m_n[hIndex] = tempPointN.x;
						m_n[hIndex + 1] = tempPointN.y;
						m_n[hIndex + 2] = tempPointN.z;
					}else
					{//体 6
						tempPointN = (m_faceN[upIndex - 1] + m_faceN[upIndex] + m_faceN[upIndex + 1] +
							m_faceN[index - 2] + m_faceN[index - 1] + m_faceN[index]) * 0.16666666666f;
						m_n[hIndex] = tempPointN.x;
						m_n[hIndex + 1] = tempPointN.y;
						m_n[hIndex + 2] = tempPointN.z;
					}
				}
			}
		}
	}
}
bool _XTerrain::getMousePoint(_XVector2 &outPoint)
{
	if(!m_isInited) return false;
	//遍历所有的点
	float len = 10.0f;
	float tempLen;
	_XVector3 tempV3;
	bool isTrue = false;
	int index = 0;
	for(int i = 0;i < m_height;i += 2)	//这里降低精度减少计算量
	{//这样没有考虑到遮挡问题
		index = i * m_width;
		for(int j = 0;j < m_width;j += 2) 	//这里降低精度减少计算量
		{
			tempV3.set(j,m_terrainData[index + j],i);

			tempV3 = _X3DWorld::GetInstance().worldToScreen(tempV3,_XRect(0.0f,0.0f,XEE::windowData.w,XEE::windowData.h));
			if(tempV3.z < 0.0f) continue;	//摄像机后面的点不算
			tempLen = XEE::mousePosition.getLengthSqure(tempV3.x,tempV3.y);
			if(tempLen < len)
			{
				len = tempLen;
				outPoint.set(j,i);
				isTrue = true;
			}
		}
	}
	return isTrue;
}
bool _XTerrain::getMousePointEx(const _XVector3 &n,const _XVector3 &f,_XVector2 &outPoint)
{
	if(!m_isInited) return false;
	static _XVector3 out,p[3];//n;
	float tempD = -1.0f;
	float ret = 0.0f;
	//int index;
	//_XVector3 tempN(0.0f,1.0f,0.0f);
	//下面遍历所有的面
	int pIndex;
	for(int i = 0,index = 0;i < m_allFaceSum;++ i,index += 3)
	{
		pIndex = m_index[index + 0] * 3;
		p[0].set(m_v[pIndex],m_v[pIndex + 1],m_v[pIndex + 2]);
		pIndex = m_index[index + 1] * 3;
		p[1].set(m_v[pIndex],m_v[pIndex + 1],m_v[pIndex + 2]);
		pIndex = m_index[index + 2] * 3;
		p[2].set(m_v[pIndex],m_v[pIndex + 1],m_v[pIndex + 2]);
		//tempN = getNormal(p[0],p[1],p[2]);
		ret = canPickingByRay(n,f,p,m_faceN[i],out);
		if(ret >= 0.0f && (ret < tempD || tempD < 0.0f))
		{//遍历最近的点
			tempD = ret;
			outPoint.set(out.x,out.z);
		}
	}

	return (tempD >= 0.0f);
}
void _XTerrain::drawNormal(int step)
{
	if(!m_isInited) return;
	_XVector3 ps;

	for(int i = 0,index = 0;i < m_allFaceSum;i += step,index += step * 3)
	{
		ps.set(m_v[m_index[index + 0] * 3 + 0],m_v[m_index[index + 0] * 3 + 1],m_v[m_index[index + 0] * 3 + 2]);
		ps += _XVector3(m_v[m_index[index + 1] * 3 + 0],m_v[m_index[index + 1] * 3 + 1],m_v[m_index[index + 1] * 3 + 2]);
		ps += _XVector3(m_v[m_index[index + 2] * 3 + 0],m_v[m_index[index + 2] * 3 + 1],m_v[m_index[index + 2] * 3 + 2]);
		ps *= 0.333333f;
		drawLine(ps,m_faceN[i],20.0f);
	}
}
#include "XBezierInterpolation.h"
void fitTerrain(const float *inData,int inX,int inY,float *outData,int outX,int outY)//地形适应,将任意尺寸的地形按照3次贝塞尔差值成需要的地形
{
	if(inX < 2 || inY < 2) return;	//最小的差值
	float *tempData = createArrayMem<float>(outX * inY);	//中间变量
	if(tempData == NULL) return;
	_XBezierInterpolation BZInter;	//用于计算贝塞尔插值
	//水平贝塞尔插值
	float p0,p1,p2,p3;
	float tWF,tHF;
	int tW,tH;
	int tempIndex;
	float v0,v1,v2;
	for(int h = 0;h < inY;++ h)
	{
		tempIndex = h * inX;
		for(int w = 0;w < outX;++ w)
		{//计算最近的两个点
			tWF = (float)w / (float)outX * (inX - 1.0f);	//
			tW = tWF;
			if(tW == inX - 1)
			{//最后一个点之外的差值，直接用最后一个点的数值
				tempData[w + h * outX] = inData[tW + tempIndex];
			}else
			{
				p0 = inData[tW + tempIndex];
				p3 = inData[tW + 1 + tempIndex];
				v1 = (p0 + p3) * 0.5f;
				if(tW == 0) v0 = p0 - (v1 - p0);
				else v0 = (p0 + inData[tW - 1 + tempIndex]) * 0.5f;
				if(tW == inX - 2) v2 = p3 + p3 - v1;
				else v2 = (p3 + inData[tW + 2 + tempIndex]) * 0.5f;
				p1 = (v1 - v0) * 0.5f + p0;
				p2 = p3 - (v2 - v1) * 0.5f;
				BZInter.init(p0,p1,p2,p3);
				tempData[w + h * outX] = BZInter.getBezierSplineValue(tWF - tW);
			}
		}
	}
	//垂直贝塞尔插值
	for(int w = 0;w < outX;++ w)
	{
		for(int h = 0;h < outY;++ h)
		{
			tHF = (float)h / (float)outY * (inY - 1.0f);
			tH = tHF;
			if(tH == inY - 1)
			{
				outData[w + h * outX] = tempData[w + tH * outX];
			}else
			{
				p0 = tempData[w + tH * outX];
				p3 = tempData[w + (tH + 1) * outX];
				v1 = (p0 + p3) * 0.5f;
				if(tH == 0) v0 = p0 - (v1 - p0);
				else v0 = (p0 + tempData[w + (tH - 1) * outX]) * 0.5f;
				if(tH == inY - 2) v2 = p3 + p3 - v1;
				else v2 = (p3 + tempData[w + (tH + 2) * outX]) * 0.5f;
				p1 = (v1 - v0) * 0.5f + p0;
				p2 = p3 - (v2 - v1) * 0.5f;

			//	if(tH == 0) p1 = (p0 + p3) * 0.5f;
			//	else p1 = p0 + (p0 - tempData[w + (tH - 1) * outX]) * 0.5f;
			//	if(tH == inY - 2) p2 = (p0 + p3) * 0.5f;
			//	else p2 = p3 - (tempData[w + (tH + 2) * outX] - p3) * 0.5f;
				BZInter.init(p0,p1,p2,p3);
				outData[w + h * outX] = BZInter.getBezierSplineValue(tHF - tH);
			}
		}
	}
	//XDELETE_ARRAY(tempData);
}
void _XTerrain::myFitTerrain(const float *inData,int inX,int inY,float *outData)
{
	if(inX < 2 || inY < 2) return;	//最小的差值
	//3次贝塞尔差值
//	int outX = m_width;
//	int outY = m_height;
//	if(m_tempFitTerrain == NULL) 
//	{//第一次进入
//		m_tempFitTerrain = createArrayMem<float>(outX * inY);
//		if(m_tempFitTerrain == NULL) return;
//		m_oInX = inX;
//		m_oInY = inY;
//	}else
//	if(m_oInY != inY)
//	{//输入数据发生变化
//		XDELETE_ARRAY(m_tempFitTerrain);
//		m_tempFitTerrain = createArrayMem<float>(outX * inY);
//		if(m_tempFitTerrain == NULL) return;
//		m_oInY = inY;
//	}
//	_XBezierInterpolation BZInter;	//用于计算贝塞尔插值
//	//水平贝塞尔插值
//	float p0,p1,p2,p3;
//	float tWF,tHF;
//	int tW,tH;
//	int tempIndex;
//	float v0,v1,v2;
//	for(int h = 0;h < inY;++ h)
//	{
//		tempIndex = h * inX;
//		for(int w = 0;w < outX;++ w)
//		{//计算最近的两个点
//			tWF = (float)w / (float)outX * (inX - 1.0f);	//
//			tW = tWF;
//			if(tW == inX - 1)
//			{//最后一个点之外的差值，直接用最后一个点的数值
//				m_tempFitTerrain[w + h * outX] = inData[tW + tempIndex];
//			}else
//			{
//				p0 = inData[tW + tempIndex];
//				p3 = inData[tW + 1 + tempIndex];
//				v1 = (p0 + p3) * 0.5f;
//				if(tW == 0) v0 = p0 - (v1 - p0);
//				else v0 = (p0 + inData[tW - 1 + tempIndex]) * 0.5f;
//				if(tW == inX - 2) v2 = p3 + p3 - v1;
//				else v2 = (p3 + inData[tW + 2 + tempIndex]) * 0.5f;
//				p1 = (v1 - v0) * 0.5f + p0;
//				p2 = p3 - (v2 - v1) * 0.5f;
//				BZInter.init(p0,p1,p2,p3);
//				m_tempFitTerrain[w + h * outX] = BZInter.getBezierSplineValue(tWF - tW);
//			}
//		}
//	}
//	//垂直贝塞尔插值
//	for(int w = 0;w < outX;++ w)
//	{
//		for(int h = 0;h < outY;++ h)
//		{
//			tHF = (float)h / (float)outY * (inY - 1.0f);
//			tH = tHF;
//			if(tH == inY - 1)
//			{
//				outData[w + h * outX] = m_tempFitTerrain[w + tH * outX];
//			}else
//			{
//				p0 = m_tempFitTerrain[w + tH * outX];
//				p3 = m_tempFitTerrain[w + (tH + 1) * outX];
//				v1 = (p0 + p3) * 0.5f;
//				if(tH == 0) v0 = p0 - (v1 - p0);
//				else v0 = (p0 + m_tempFitTerrain[w + (tH - 1) * outX]) * 0.5f;
//				if(tH == inY - 2) v2 = p3 + p3 - v1;
//				else v2 = (p3 + m_tempFitTerrain[w + (tH + 2) * outX]) * 0.5f;
//				p1 = (v1 - v0) * 0.5f + p0;
//				p2 = p3 - (v2 - v1) * 0.5f;
//
//				BZInter.init(p0,p1,p2,p3);
//				outData[w + h * outX] = BZInter.getBezierSplineValue(tHF - tH);
//			}
//		}
//	}
	//双线性差值
	float zoom = (float)m_width / (float)inX;
	float tempSize = 1.0f / zoom;
	float xx,yy;
	int x,y,indexS,indexD,indexTS,indexTD;
	//下面根据需要对数据进行处理
	for(int h = 0;h < m_height;++ h)
	{
		yy = h * tempSize;
		y = yy;
		yy = yy - y;
		indexTD = h * m_width;
		indexTS = y * inX;
		if(y < inY - 1)
		{//最后一行
			for(int w = 0;w < m_width;++ w)
			{
				xx = w * tempSize;
				x = xx;			//整数部分
				xx = xx - x;	//小数部分
				
				indexD = w + indexTD;
				indexS = x + indexTS;
				if(x < inX - 1)
				{
					outData[indexD] = inData[indexS] * (1.0f - xx) * (1.0f - yy) + (1.0f - xx) * yy * inData[indexS + inX]
						+ xx * (1.0f - yy) * inData[indexS + 1] + xx * yy * inData[indexS + inX + 1];
				}else
				{
					outData[indexD] = inData[indexS] * (1.0f - xx) * (1.0f - yy) + (1.0f - xx) * yy * inData[indexS + inX]
						+ xx * (1.0f - yy) * inData[indexS - 1] + xx * yy * inData[indexS + inX - 1];
				}
			}
		}else
		{
			for(int w = 0;w < m_width;++ w)
			{
				xx = w * tempSize;
				x = xx;		//整数部分
				xx = xx - x;	//小数部分
				
				indexD = w + indexTD;
				indexS = x + indexTS;
				if(x < inX - 1)
				{
					outData[indexD] = inData[indexS] * (1.0f - xx) * (1.0f - yy) + (1.0f - xx) * yy * inData[indexS - inX]
						+ xx * (1.0f - yy) * inData[indexS + 1] + xx * yy * inData[indexS - inX + 1];
				}else
				{
					outData[indexD] = inData[indexS] * (1.0f - xx) * (1.0f - yy) + (1.0f - xx) * yy * inData[indexS - inX]
						+ xx * (1.0f - yy) * inData[indexS - 1] + xx * yy * inData[indexS - inX - 1];
				}
			}
		}
	}
}
