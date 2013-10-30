//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XFontUnicode.h"
#include "XBasicOpenGL.h"
#include "XResourcePack.h"
#include "XLogBook.h"
#include "XBasicSDL.h"
#include "XObjectManager.h"

//int testE = 0;
_XFontUnicode::_XFontUnicode()
:m_texture(NULL)
//,m_isAcopy(0)
,m_fontPageText(NULL)
,m_pageSum(0)
,m_isCliped(0)
,m_clipRect(0,0,0,0)
,m_isPassword(0)
,m_pageW(0)
,m_pageH(0)
,m_libFontSum(0)
,m_layout(0,0)
,m_isVisiable(1)
,m_cp(NULL)
{
	m_textPosition = new _XVector2[STRING_MAX_SIZE];
	m_textRect = new _XRect[STRING_MAX_SIZE];
	m_textPageOrder = new int[STRING_MAX_SIZE];
//	testE ++;
//	printf("%d\n",testE);
}

_XFontUnicode::~_XFontUnicode()
{
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{//自身没有引用需要释放
		for(int i = 0;i < m_pageSum;++ i)
		{
			m_texture[i].release();
		}
		XDELETE_ARRAY(m_texture);
		XDELETE_ARRAY(m_fontPageText);

		XDELETE(m_cp);
	}
//	testE --;
//	printf("%d\n",testE);
	XDELETE_ARRAY(m_textPosition);
	XDELETE_ARRAY(m_textRect);
	XDELETE_ARRAY(m_textPageOrder);

#if WITH_OBJECT_MANAGER	//在物件管理的类中注销这些物件
	_XObjectManager::GetInstance().decreaseAObject(this);
#endif

	release();
}

int _XFontUnicode::init(const char *filenameIn,_XVector2I size,_XVector2I layout,int pageSum,int resoursePosition)
{
	if(m_isInited != 0) 
	{
		AddLogInfoStr("The font have initted!\n");
		return 1;
	}
	try
	{
		m_cp = new _XSCounter;
		if(m_cp == NULL) return 0;
	}catch(...)
	{
		return 0;
	}

	if(pageSum <= 0 || pageSum >= UNICODE_FONT_PAGE_SUM || pageSum >= 100) return 0;
	m_pageSum = pageSum;
	if((m_texture = createArrayMem<_XTextureData>(m_pageSum)) == NULL)
	{
		XDELETE(m_cp);
		return 0;
	}
	if((m_fontPageText = createArrayMem<char>(MAX_UNICODE_TEXT_SUM)) == NULL)
	{
		XDELETE_ARRAY(m_texture);
		XDELETE(m_cp);
		return 0;
	}

	if(resoursePosition != RESOURCE_OUTSIDE)
	{
		resoursePosition = RESOURCE_INSIDE;
	}
	m_resoursePosition = resoursePosition;

	m_string[0] = '\0';
	//载入图片
	char *filename = NULL;
	if((filename = createArrayMem<char>(MAX_FONT_UNICODE_FILE_NAME_LENGTH)) == NULL)
	{
		XDELETE_ARRAY(m_texture);
		XDELETE_ARRAY(m_fontPageText);
		XDELETE(m_cp);
		return 0;
	}

	strcpy(filename,filenameIn);
	int fileNameLength = strlen(filename);
	for(int i = 0;i < m_pageSum;++ i)
	{
		if( i< 10)
		{
			filename[fileNameLength - 5] = i%10 + '0';
		}else
		if( i< 100)
		{
			filename[fileNameLength - 5] = i%10 + '0';
			filename[fileNameLength - 6] = (i/10)%10 + '0';
		}else
		{
			XDELETE_ARRAY(m_texture);
			XDELETE_ARRAY(m_fontPageText);
			XDELETE_ARRAY(filename);
			XDELETE(m_cp);
			return 0;
		}

	//	if(SDL_Texture(&m_texture[i],filename,1,&m_pageW,&m_pageH,m_resoursePosition) == 0)
		if(m_texture[i].load(filename,m_resoursePosition) == 0)
		{
			AddLogInfoNull("pic:%s load error!\n",filename);
			XDELETE_ARRAY(m_texture);
			XDELETE_ARRAY(m_fontPageText);
			XDELETE_ARRAY(filename);
			XDELETE(m_cp);
			return 0;
		}
	}

	m_pageW = m_texture[0].texture.m_w;
	m_pageH = m_texture[0].texture.m_h;

	//初始化精灵
	if(m_sprite.init(m_pageW,m_pageH,1,POINT_LEFT_TOP) == 0) 
	{
		XDELETE_ARRAY(m_texture);
		XDELETE_ARRAY(m_fontPageText);
		XDELETE_ARRAY(filename);
		XDELETE(m_cp);
		return 0;
	}
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);
#if WITH_OBJECT_MANAGER	//在物件管理的类中注销这些物件
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
	_XObjectManager::GetInstance().addAObject(this,OBJ_FONTUNICODE);
#endif
	//读取字体库对应的文本
	FILE *fp = NULL;
	filename[fileNameLength - 5] = '0';
	filename[fileNameLength - 6] = '0';
	filename[fileNameLength - 3] = 't';
	filename[fileNameLength - 2] = 'x';
	filename[fileNameLength - 1] = 't';
	if(m_resoursePosition == 0)
	{
		if((fp = fopen(filename,"r")) == NULL)
		{//文件打开失败
			XDELETE_ARRAY(m_texture);
			XDELETE_ARRAY(m_fontPageText);
			XDELETE_ARRAY(filename);
			XDELETE(m_cp);
			return 0;
		}
		unsigned char fileReadChar;
		m_libFontSum = 0;
		for(int i = 0;i < MAX_UNICODE_TEXT_SUM;++ i)
		{
			if(fread(&fileReadChar,1,1,fp) == 0) break;
			if(feof(fp)) break;	//文件读取失败则退出
			if(fileReadChar == '\n') continue;
			if(fileReadChar <128)
			{//ASCII
				m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum] = ' ';
				m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum + 1] = fileReadChar;
				m_libFontSum ++;
			}else
			{//非ASCII
				m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum] = fileReadChar;
				if(fread(&fileReadChar,1,1,fp) == 0) return 0;
				m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum + 1] = fileReadChar;
				m_libFontSum ++;
			}
		}
		if(m_libFontSum == 0)
		{//字符库中没有字体元素
			XDELETE_ARRAY(m_texture);
			XDELETE_ARRAY(m_fontPageText);
			XDELETE_ARRAY(filename);
			XDELETE(m_cp);
			fclose(fp);
			return 0;
		}
		fclose(fp);
	}else
	{
/*		static int checkedresource = 0;
		if(checkedresource == 0)
		{
			checkedresource = 1;
			if(ResourceTemp.checkCheckData() == 0)
			{//严重的错误，这里定掉游戏
				DebugShow("Resource Data Error!\n");
				while(1)
				{
					Sleep(100);
				}
			}
		}*/
		int lengthTemp = _XResourcePack::GetInstance().getFileLength(filename);
		unsigned char *p = NULL;
		if((p = createArrayMem<unsigned char>(lengthTemp + 1)) == NULL)
		{
			XDELETE_ARRAY(m_texture);
			XDELETE_ARRAY(m_fontPageText);
			XDELETE_ARRAY(filename);
			XDELETE(m_cp);
			return 0;
		}
		_XResourcePack::GetInstance().unpackResource(filename,p);
		*(p + lengthTemp) = '\0';

		unsigned char fileReadChar;
		m_libFontSum = 0;
		int nowPosition = 0;
		for(int i = 0;i < MAX_UNICODE_TEXT_SUM;++ i)
		{
			if(nowPosition == lengthTemp) break;
			fileReadChar = p[nowPosition];
			++ nowPosition;
			if(fileReadChar == '\n') continue;
			if(fileReadChar <128)
			{//ASCII
				m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum] = ' ';
				m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum + 1] = fileReadChar;
				m_libFontSum ++;
			}else
			{//非ASCII
				m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum] = fileReadChar;
				if(nowPosition == lengthTemp) 
				{
					XDELETE_ARRAY(m_texture);
					XDELETE_ARRAY(m_fontPageText);
					XDELETE_ARRAY(filename);
					XDELETE(m_cp);
					XDELETE_ARRAY(p);
					return 0;
				}
				fileReadChar = p[nowPosition];
				++ nowPosition;
				m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum + 1] = fileReadChar;
				m_libFontSum ++;
			}
		}
		if(m_libFontSum == 0)
		{//字符库中没有字体元素
			XDELETE_ARRAY(m_texture);
			XDELETE_ARRAY(m_fontPageText);
			XDELETE_ARRAY(filename);
			XDELETE(m_cp);
			XDELETE_ARRAY(p);
			return 0;
		}
		XDELETE_ARRAY(p);
	}
	m_position.set(0,0);	
	m_setPosition.set(0,0);	//字体被设置的位置，这个位置可能由于旋转等动作在最终被改变
	m_angle = 0;			
	m_distance = 0;;		
	m_size = size;			
	m_layout = layout;
	m_showSize.x = 1.0f;
	m_showSize.y = 1.0f;
	m_sprite.setSize(m_showSize);
	m_isPassword = 0;

	XDELETE_ARRAY(filename);
	m_isInited = 1;		

	m_isCliped = 0;
	return 1;
}

int _XFontUnicode::init(const char *filenameIn,int resoursePosition)
{
	if(m_isInited != 0) 
	{
		AddLogInfoStr("The font have initted!\n");
		return 1;
	}
	try
	{
		m_cp = new _XSCounter;
		if(m_cp == NULL) return 0;
	}catch(...)
	{
		return 0;
	}

	if(resoursePosition != RESOURCE_OUTSIDE)
		resoursePosition = RESOURCE_INSIDE;
	m_resoursePosition = resoursePosition;
	//下面读取相关格式数据
	char *filename = NULL;
	if((filename = createArrayMem<char>(MAX_FONT_UNICODE_FILE_NAME_LENGTH)) == NULL)
	{
		XDELETE(m_cp);
		return 0;
	}
	strcpy(filename,filenameIn);
	int fileNameLength = strlen(filename);

	filename[fileNameLength - 5] = '0';
	filename[fileNameLength - 6] = '0';
	filename[fileNameLength - 3] = 'd';
	filename[fileNameLength - 2] = 'a';
	filename[fileNameLength - 1] = 't';
	FILE *fp = NULL;
	_XVector2I size(0,0);
	_XVector2I layout(0,0);
	int pageSum = 0;
	if(m_resoursePosition == 0)
	{
		if((fp = fopen(filename,"r")) == NULL)
		{//文件单开失败
			XDELETE(m_cp);
			XDELETE_ARRAY(filename);
			return 0;
		}
		fscanf(fp,"%d,%d,%d,%d,%d,",&size.x,&size.y,&layout.x,&layout.y,&pageSum);
		fclose(fp);
	}else
	{
		int lengthTemp = _XResourcePack::GetInstance().getFileLength(filename);
		unsigned char *p = NULL;
		if((p = createArrayMem<unsigned char>(lengthTemp + 1)) == NULL)
		{
			XDELETE(m_cp);
			XDELETE_ARRAY(filename);
			return 0;
		}
		_XResourcePack::GetInstance().unpackResource(filename,p);
		*(p + lengthTemp) = '\0';
		sscanf((char *)p,"%d,%d,%d,%d,%d,",&size.x,&size.y,&layout.x,&layout.y,&pageSum);
		XDELETE_ARRAY(p);
	}

	if(pageSum <= 0 || pageSum >= UNICODE_FONT_PAGE_SUM || pageSum >= 100) 
	{
		XDELETE(m_cp);
		XDELETE_ARRAY(filename);
		return 0;
	}
	m_pageSum = pageSum;

	if((m_texture = createArrayMem<_XTextureData>(m_pageSum)) == NULL)
	{
		XDELETE(m_cp);
		XDELETE(filename);
		return 0;
	}
	if((m_fontPageText = createArrayMem<char>(MAX_UNICODE_TEXT_SUM)) == NULL)
	{
		XDELETE(m_cp);
		XDELETE(filename);
		XDELETE_ARRAY(m_texture);
		return 0;
	}

	filename[fileNameLength - 3] = filenameIn[fileNameLength - 3];
	filename[fileNameLength - 2] = filenameIn[fileNameLength - 2];
	filename[fileNameLength - 1] = filenameIn[fileNameLength - 1];
	m_string[0] = '\0';
	//载入图片
	for(int i = 0;i < m_pageSum;++ i)
	{
		if( i< 10)
		{
			filename[fileNameLength - 5] = i%10 + '0';
		}else
		if( i< 100)
		{
			filename[fileNameLength - 5] = i%10 + '0';
			filename[fileNameLength - 6] = (i/10)%10 + '0';
		}else
		{
			XDELETE_ARRAY(m_texture);
			XDELETE_ARRAY(m_fontPageText);
			XDELETE_ARRAY(filename);
			XDELETE(m_cp);
			return 0;
		}

	//	if(SDL_Texture(&m_texture[i],filename,1,&m_pageW,&m_pageH,m_resoursePosition) == 0)
		if(m_texture[i].load(filename,m_resoursePosition) == 0)
		{
			AddLogInfoNull("pic:%s load error!\n",filename);
			XDELETE_ARRAY(m_texture);
			XDELETE_ARRAY(m_fontPageText);
			XDELETE_ARRAY(filename);
			XDELETE(m_cp);
			return 0;
		}
	}

	m_pageW = m_texture[0].texture.m_w;
	m_pageH = m_texture[0].texture.m_h;

	//初始化精灵
	if(m_sprite.init(m_pageW,m_pageH,1,POINT_LEFT_TOP) == 0) 
	{
		XDELETE_ARRAY(m_texture);
		XDELETE_ARRAY(m_fontPageText);
		XDELETE_ARRAY(filename);
		XDELETE(m_cp);
		return 0;
	}
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);
#if WITH_OBJECT_MANAGER	//在物件管理的类中注销这些物件
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
	_XObjectManager::GetInstance().addAObject(this,OBJ_FONTUNICODE);
#endif
	//读取字体库对应的文本
	filename[fileNameLength - 5] = '0';
	filename[fileNameLength - 6] = '0';
	filename[fileNameLength - 3] = 't';
	filename[fileNameLength - 2] = 'x';
	filename[fileNameLength - 1] = 't';
	if(m_resoursePosition == 0)
	{
		if((fp = fopen(filename,"r")) == NULL)
		{//文件打开失败
			XDELETE_ARRAY(m_texture);
			XDELETE_ARRAY(m_fontPageText);
			XDELETE_ARRAY(filename);
			XDELETE(m_cp);
			return 0;
		}
		unsigned char fileReadChar;
		m_libFontSum = 0;
		for(int i = 0;i < MAX_UNICODE_TEXT_SUM;++ i)
		{
			if(fread(&fileReadChar,1,1,fp) == 0) break;
			if(feof(fp)) break;	//文件读取失败则退出
			if(fileReadChar == '\n') continue;
			if(fileReadChar <128)
			{//ASCII
				m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum] = ' ';
				m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum + 1] = fileReadChar;
				m_libFontSum ++;
			}else
			{//非ASCII
				m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum] = fileReadChar;
				if(fread(&fileReadChar,1,1,fp) == 0) return 0;
				m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum + 1] = fileReadChar;
				m_libFontSum ++;
			}
		}
		if(m_libFontSum == 0)
		{//字符库中没有字体元素
			XDELETE_ARRAY(m_texture);
			XDELETE_ARRAY(m_fontPageText);
			XDELETE_ARRAY(filename);
			XDELETE(m_cp);
			fclose(fp);
			return 0;
		}
		fclose(fp);
	}else
	{
//		static int checkedresource = 0;
//		if(checkedresource == 0)
//		{
//			checkedresource = 1;
//			if(ResourceTemp.checkCheckData() == 0)
//			{//严重的错误，这里定掉游戏
//				DebugShow("Resource Data Error!\n");
//				while(1)
//				{
//					Sleep(100);
//				}
//			}
//		}
		int lengthTemp = _XResourcePack::GetInstance().getFileLength(filename);
		unsigned char *p = NULL;
		if((p = createArrayMem<unsigned char>(lengthTemp + 1)) == NULL)
		{
			XDELETE_ARRAY(m_texture);
			XDELETE_ARRAY(m_fontPageText);
			XDELETE_ARRAY(filename);
			XDELETE(m_cp);
			return 0;
		}
		_XResourcePack::GetInstance().unpackResource(filename,p);
		*(p + lengthTemp) = '\0';

		unsigned char fileReadChar;
		m_libFontSum = 0;
		int nowPosition = 0;
		for(int i = 0;i < MAX_UNICODE_TEXT_SUM;++ i)
		{
			if(nowPosition == lengthTemp) break;
			fileReadChar = p[nowPosition];
			++ nowPosition;
			if(fileReadChar == '\n') continue;
			if(fileReadChar <128)
			{//ASCII
				m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum] = ' ';
				m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum + 1] = fileReadChar;
				m_libFontSum ++;
			}else
			{//非ASCII
				m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum] = fileReadChar;
				if(nowPosition == lengthTemp) 
				{
					XDELETE_ARRAY(m_texture);
					XDELETE_ARRAY(m_fontPageText);
					XDELETE_ARRAY(filename);
					XDELETE(m_cp);
					XDELETE_ARRAY(p);
					return 0;
				}
				fileReadChar = p[nowPosition];
				++ nowPosition;
				m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum + 1] = fileReadChar;
				m_libFontSum ++;
			}
		}
		if(m_libFontSum == 0)
		{//字符库中没有字体元素
			XDELETE_ARRAY(m_texture);
			XDELETE_ARRAY(m_fontPageText);
			XDELETE_ARRAY(filename);
			XDELETE(m_cp);
			XDELETE_ARRAY(p);
			return 0;
		}
		XDELETE_ARRAY(p);
	}
	m_position.set(0,0);	
	m_setPosition.set(0,0);	//字体被设置的位置，这个位置可能由于旋转等动作在最终被改变
	m_angle = 0;			
	m_distance = 0;;		
	m_size = size;			
	m_layout = layout;
	m_showSize.x = 1.0f;
	m_showSize.y = 1.0f;
	m_sprite.setSize(m_showSize);
	m_isPassword = 0;

	XDELETE_ARRAY(filename);
	m_isInited = 1;		

	m_isCliped = 0;
	return 1;
}

void _XFontUnicode::draw()
{
//	if((m_isCliped != 0 && m_clipRect.top >= m_size.y)
//		|| (m_isCliped != 0 && m_clipRect.bottom <= 0))
	if(m_isCliped != 0 && m_clipRect.bottom <= 0)
	{//这种情况什么也不用画
		return;
	}
	if(m_isVisiable == 0) return;
	if(m_needUpdateData != 0)
	{
		m_needUpdateData = 0;
		updateData();
	}
	for(int i = 0;i < m_needShowTextSum;++ i)
	{
		m_sprite.setClipRect(m_textRect[i]);
		m_sprite.setPosition(m_textPosition[i]);

		m_sprite.draw(&(m_texture[m_textPageOrder[i]]));
	}
}

void _XFontUnicode::updateData()
{
	m_needShowTextSum = 0;
	m_maxPixelWidth = 0;
	m_maxPixelHeight = 0;
	int now_x = 0;
	int now_y = 0;
	char tempChar[2];
	int charPoint = 0;	//字符所在位置的指针
	int pageNumble = -1;
	int wordsSumD = -1;		//双字节
	int wordsSumS = 0;		//单字节
	//char nowIsD = 0;		//当前显示的这个字体时单字节还是多字节
	//为了对齐方式的引入这里需要先做适当的处理
	//1、统计总行数
	int allLineSum = 1;
	int nowLineWidthD = 0;	//当前行中的中文字符数量
	int nowLineWidthS = 0;	//当前行中的ASCII字符数量
	for(int i =0;i < STRING_MAX_SIZE;++ i)
	{
		if(m_string[i] == '\0') break;
		if((unsigned char)m_string[i] < 128) 
		{//ASCII
			if(m_string[i] == '\n') allLineSum ++;	//这是总行数
			else 
			{
				if(allLineSum == 1) nowLineWidthS ++; //统计第一行的宽度
			}
		}else
		{//非ASCII
			if(allLineSum == 1) nowLineWidthD ++; //统计第一行的宽度
			++ i;
		}
	}

	m_lineSum = 0;	//用于统计行数
	float textPixelDX = (m_size.x + m_distance) * m_showSize.x;//双字节字符的宽度
	float textPixelSX = (m_size.x * 0.5f + m_distance) * m_showSize.x;//单字节字符的宽度
	float textPixelY = (m_size.y + m_distanceY) * m_showSize.y;//字符的宽度
	for(int i =0;i < STRING_MAX_SIZE;++ i)
	{
		if(m_string[i] == '\0') break;	//字符串结束
		if((unsigned char)m_string[i] < 128)
		{//ASCII
			if(m_isPassword == 0)
			{//非密码模式
				tempChar[0] = ' ';
				tempChar[1] = m_string[i];
			}else
			{//密码模式
				tempChar[0] = ' ';
				tempChar[1] = '*';
			}
			++ wordsSumS;
			//nowIsD = 0;
			if(m_string[i] == '\n')
			{//如果是换行符则开始换行
				m_lineSum ++;
				wordsSumS = 0;
				wordsSumD = -1;
				//遇到换行时统计当前行的字符串长度
				nowLineWidthD = 0;	//当前行中的中文字符数量
				nowLineWidthS = 0;	//当前行中的ASCII字符数量
				for(int j = i + 1;j < STRING_MAX_SIZE;++ j)
				{
					if((unsigned char)m_string[j] < 128) 
					{//ASCII
						if(m_string[j] == '\n' || m_string[j] == '\0') break;	//当前行结束
						else nowLineWidthS ++; //统计第一行的宽度
					}else
					{//非ASCII
						nowLineWidthD ++; //统计第一行的宽度
						++ j;
					}
				}
				continue;	//换行符不需要做任何处理，直接继续
			}
		}else
		{//非ASCII
			if(m_isPassword == 0)
			{//非密码模式
				tempChar[0] = m_string[i];
				tempChar[1] = m_string[i + 1];
				++ i;
				++ wordsSumD;
				//nowIsD = 1;
			}else
			{//密码模式
				tempChar[0] = ' ';
				tempChar[1] = '*';
				++ i;
				++ wordsSumS;
				//nowIsD = 0;
			}
		}
		//查找字符
		charPoint = 0;
		for(charPoint = 0;charPoint < m_libFontSum;++ charPoint)
		{
			if(m_fontPageText[UNICODE_BYTES_WIDTH * charPoint] == tempChar[0]
				&& m_fontPageText[UNICODE_BYTES_WIDTH * charPoint + 1] == tempChar[1])
			{
				break;
			}
		}
		if(charPoint == m_libFontSum)
		{//如果找不到这个字符则作为空格来处理
			//charPoint = 0;
			continue;
		}
		//查找字符的页面，横坐标和纵坐标
		pageNumble = charPoint / (m_layout.x * m_layout.y);
		now_x = (charPoint % m_layout.x) * m_size.x;

		if(m_layout.y == 0) 
		{
			now_y = 0;
		}else
		{
			now_y = ((charPoint / m_layout.x) % m_layout.y) * m_size.y;
		}
		if(m_isCliped == 0)
		{
			m_textRect[m_needShowTextSum].set(now_x,now_y,now_x + m_size.x,now_y + m_size.y);
			if(m_alignmentModeX == _FONT_ALIGNMENT_MODE_X_LEFT)
			{//左对齐，啥也不用做
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_UP)
				{
					m_textPosition[m_needShowTextSum].set(m_position.x + m_angleCos * (textPixelDX * wordsSumD + textPixelSX * wordsSumS)
						- m_angleSin * textPixelY * m_lineSum,
						m_position.y + m_angleSin * (textPixelDX * wordsSumD + textPixelSX * wordsSumS)
						+ m_angleCos * textPixelY * m_lineSum);
				}else
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_MIDDLE)
				{
					m_textPosition[m_needShowTextSum].set(m_position.x + m_angleCos * (textPixelDX * wordsSumD + textPixelSX * wordsSumS)
						- m_angleSin * textPixelY * (m_lineSum - allLineSum * 0.5f),
						m_position.y + m_angleSin * (textPixelDX * wordsSumD + textPixelSX * wordsSumS)
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum * 0.5f));
				}else
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_DOWN)
				{
					m_textPosition[m_needShowTextSum].set(m_position.x + m_angleCos * (textPixelDX * wordsSumD + textPixelSX * wordsSumS)
						- m_angleSin * textPixelY * (m_lineSum - allLineSum),
						m_position.y + m_angleSin * (textPixelDX * wordsSumD + textPixelSX * wordsSumS)
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum));
				}
			}else
			if(m_alignmentModeX == _FONT_ALIGNMENT_MODE_X_MIDDLE)
			{//左对齐，啥也不用做
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_UP)
				{
					m_textPosition[m_needShowTextSum].set(m_position.x + m_angleCos * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD * 0.5f - textPixelSX * nowLineWidthS * 0.5f)
						- m_angleSin * textPixelY * m_lineSum,
						m_position.y + m_angleSin * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD * 0.5f - textPixelSX * nowLineWidthS * 0.5f)
						+ m_angleCos * textPixelY * m_lineSum);
				}else
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_MIDDLE)
				{
					m_textPosition[m_needShowTextSum].set(m_position.x + m_angleCos * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD * 0.5f - textPixelSX * nowLineWidthS * 0.5f)
						- m_angleSin * textPixelY * (m_lineSum - allLineSum * 0.5f),
						m_position.y + m_angleSin * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD * 0.5f - textPixelSX * nowLineWidthS * 0.5f)
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum * 0.5f));
				}else
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_DOWN)
				{
					m_textPosition[m_needShowTextSum].set(m_position.x + m_angleCos * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD * 0.5f - textPixelSX * nowLineWidthS * 0.5f)
						- m_angleSin * textPixelY * (m_lineSum - allLineSum),
						m_position.y + m_angleSin * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD * 0.5f - textPixelSX * nowLineWidthS * 0.5f)
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum));
				}
			}else
			if(m_alignmentModeX == _FONT_ALIGNMENT_MODE_X_RIGHT)
			{//左对齐，啥也不用做
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_UP)
				{
					m_textPosition[m_needShowTextSum].set(m_position.x + m_angleCos * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD - textPixelSX * nowLineWidthS)
						- m_angleSin * textPixelY * m_lineSum,
						m_position.y + m_angleSin * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD - textPixelSX * nowLineWidthS)
						+ m_angleCos * textPixelY * m_lineSum);
				}else
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_MIDDLE)
				{
					m_textPosition[m_needShowTextSum].set(m_position.x + m_angleCos * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD - textPixelSX * nowLineWidthS)
						- m_angleSin * textPixelY * (m_lineSum - allLineSum * 0.5f),
						m_position.y + m_angleSin * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD - textPixelSX * nowLineWidthS)
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum * 0.5f));
				}else
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_DOWN)
				{
					m_textPosition[m_needShowTextSum].set(m_position.x + m_angleCos * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD - textPixelSX * nowLineWidthS)
						- m_angleSin * textPixelY * (m_lineSum - allLineSum),
						m_position.y + m_angleSin * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD - textPixelSX * nowLineWidthS)
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum));
				}
			}

		//	m_textPosition[m_needShowTextSum].set(m_position.x + m_angleCos * (textPixelDX * wordsSumD + textPixelSX * wordsSumS)
		//		- m_angleSin * textPixelY * m_lineSum,
		//		m_position.y + m_angleSin * (textPixelDX * wordsSumD + textPixelSX * wordsSumS)
		//		+ m_angleCos * textPixelY * m_lineSum);
			m_textPageOrder[m_needShowTextSum] = pageNumble;
			m_needShowTextSum ++;

			if(textPixelDX * (wordsSumD + 1) + textPixelSX * wordsSumS > m_maxPixelWidth)
			{
				m_maxPixelWidth = textPixelDX * (wordsSumD + 1) + textPixelSX * wordsSumS;
			}
			if(textPixelY * (m_lineSum + 1) > m_maxPixelHeight)
			{
				m_maxPixelHeight = textPixelY * (m_lineSum + 1);
			}
		}else
		{
			int left,right,top,bottom;
			//下面判断y方向是否超出范围，目前尚未实现
		//	if(m_clipRect.top <= 0)
		//	{
		//		top = 0;
		//	}else
		//	{
		//		top = m_clipRect.top;
		//	}
			float tempY = 0.0f;
			if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_UP)
			{//左对齐，啥也不用做
				tempY = (m_size.y + m_distanceY) * m_lineSum;
			}else
			if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_MIDDLE)
			{//居中，每个文字左移半个宽度
				tempY = (m_size.y + m_distanceY) * (m_lineSum - allLineSum * 0.5f);
			}else
			if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_DOWN)
			{//右对齐，每个文字左移整个宽度
				tempY = (m_size.y + m_distanceY) * (m_lineSum - allLineSum);
			}
			//float tempY = (m_size.y + m_distanceY) * m_lineSum;	//字体左边的距离
			if(m_clipRect.bottom < tempY + m_distance)
			{//下线之下不显示
				continue;
			}
			//if(m_clipRect.top >= tempY + (m_size.y + m_distanceY))
			if(m_clipRect.top >= tempY + m_size.y)
			{//上线之上不显示
				continue;
			}
			//下面判断x方向是否超出范围，如果超出范围则不显示
			float tempX = 0.0f;
			if(m_alignmentModeX == _FONT_ALIGNMENT_MODE_X_LEFT)
			{//左对齐，啥也不用做
				tempX = (m_size.x + m_distance) * wordsSumD + (m_size.x * 0.5f + m_distance) * wordsSumS;
			}else
			if(m_alignmentModeX == _FONT_ALIGNMENT_MODE_X_MIDDLE)
			{//居中，每个文字左移半个宽度
				tempX = (m_size.x + m_distance) * wordsSumD + (m_size.x * 0.5f + m_distance) * wordsSumS - (m_size.x + m_distance) * nowLineWidthD * 0.5f - (m_size.x * 0.5f + m_distance) * nowLineWidthS * 0.5f;
			}else
			if(m_alignmentModeX == _FONT_ALIGNMENT_MODE_X_RIGHT)
			{//右对齐，每个文字左移整个宽度
				tempX = (m_size.x + m_distance) * wordsSumD + (m_size.x * 0.5f + m_distance) * wordsSumS - (m_size.x + m_distance) * nowLineWidthD - (m_size.x * 0.5f + m_distance) * nowLineWidthS;
			}
			//float tempX = (m_size.x + m_distance) * wordsSumD + (m_size.x * 0.5f + m_distance) * wordsSumS;	//字体左边的距离
			if(tempX + m_distance >= m_clipRect.right) 
			{//右边之右不显示
				continue;	//字范围外面不需要画出来
			}
		//	if((nowIsD == 0 && temp + (m_size.x * 0.5 + m_distance) <= m_clipRect.left) 
			//if((nowIsD == 0 && temp + (m_size.x + m_distance) <= m_clipRect.left) //超出现实范围则直接不动作
			//	|| (nowIsD == 1 && temp + (m_size.x + m_distance) <= m_clipRect.left))	//??
			//if(tempX + (m_size.x + m_distance) <= m_clipRect.left)
			if(tempX + m_size.x <= m_clipRect.left)
			{//左边之左不显示
				continue;
			}
			if(tempY >= m_clipRect.top && tempY + (m_size.y + m_distanceY) <= m_clipRect.bottom)
			{//需要全部描画
				top = 0;
				bottom = 0;
			}else
			{
				if(tempY < m_clipRect.top)
				{
					top = m_clipRect.top - tempY;
				}else
				{
					top = 0;
				}
				if(tempY + (m_size.y + m_distanceY) > m_clipRect.bottom)
				{
					bottom = tempY + (m_size.y + m_distanceY) - m_clipRect.bottom;
				}else
				{
					bottom = 0;
				}
			}
			//当英文字符裁剪的时候会存在问题，如果是中文字符这里不会有问题
			if(tempX >= m_clipRect.left && tempX + (m_size.x + m_distance) <= m_clipRect.right)
			{//需要全部描画
				left = 0;
				right = 0;
			}else
			{
				if(tempX < m_clipRect.left)
				{
					left = m_clipRect.left - tempX;
				}else
				{
					left = 0;
				}
				if(tempX + (m_size.x + m_distance) > m_clipRect.right)
				{
					right = tempX + (m_size.x + m_distance) - m_clipRect.right;
				}else
				{
					right = 0;
				}
			}
			m_textRect[m_needShowTextSum].set(now_x + left,now_y + top,now_x + m_size.x - right,now_y + m_size.y - bottom);
			if(m_alignmentModeX == _FONT_ALIGNMENT_MODE_X_LEFT)
			{//左对齐，啥也不用做
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_UP)
				{//左对齐，啥也不用做
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_showSize.y 
						- m_angleSin * textPixelY * m_lineSum
						+ m_angleCos * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS),
						m_position.y + m_angleCos * top * m_showSize.x 
						+ m_angleCos * textPixelY * m_lineSum
						+ m_angleSin * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS));
				}else
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_MIDDLE)
				{//左对齐，啥也不用做
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_showSize.y 
						- m_angleSin * textPixelY * (m_lineSum - allLineSum * 0.5f)
						+ m_angleCos * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS),
						m_position.y + m_angleCos * top * m_showSize.x 
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum * 0.5f)
						+ m_angleSin * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS));
				}else
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_DOWN)
				{//左对齐，啥也不用做
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_showSize.y 
						- m_angleSin * textPixelY * (m_lineSum - allLineSum)
						+ m_angleCos * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS),
						m_position.y + m_angleCos * top * m_showSize.x 
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum)
						+ m_angleSin * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS));
				}
			}else
			if(m_alignmentModeX == _FONT_ALIGNMENT_MODE_X_MIDDLE)
			{//左对齐，啥也不用做
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_UP)
				{//左对齐，啥也不用做
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_showSize.y 
						- m_angleSin * textPixelY * m_lineSum
						+ m_angleCos * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD * 0.5f - textPixelSX * nowLineWidthS * 0.5f),
						m_position.y + m_angleCos * top * m_showSize.x 
						+ m_angleCos * textPixelY * m_lineSum
						+ m_angleSin * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD * 0.5f - textPixelSX * nowLineWidthS * 0.5f));
				}else
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_MIDDLE)
				{//左对齐，啥也不用做
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_showSize.y 
						- m_angleSin * textPixelY * (m_lineSum - allLineSum * 0.5f)
						+ m_angleCos * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD * 0.5f - textPixelSX * nowLineWidthS * 0.5f),
						m_position.y + m_angleCos * top * m_showSize.x 
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum * 0.5f)
						+ m_angleSin * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD * 0.5f - textPixelSX * nowLineWidthS * 0.5f));
				}else
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_DOWN)
				{//左对齐，啥也不用做
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_showSize.y 
						- m_angleSin * textPixelY * (m_lineSum - allLineSum)
						+ m_angleCos * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD * 0.5f - textPixelSX * nowLineWidthS * 0.5f),
						m_position.y + m_angleCos * top * m_showSize.x 
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum)
						+ m_angleSin * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD * 0.5f - textPixelSX * nowLineWidthS * 0.5f));
				}
			}else
			if(m_alignmentModeX == _FONT_ALIGNMENT_MODE_X_MIDDLE)
			{//左对齐，啥也不用做
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_UP)
				{//左对齐，啥也不用做
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_showSize.y 
						- m_angleSin * textPixelY * m_lineSum
						+ m_angleCos * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD - textPixelSX * nowLineWidthS),
						m_position.y + m_angleCos * top * m_showSize.x 
						+ m_angleCos * textPixelY * m_lineSum
						+ m_angleSin * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD - textPixelSX * nowLineWidthS));
				}else
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_MIDDLE)
				{//左对齐，啥也不用做
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_showSize.y 
						- m_angleSin * textPixelY * (m_lineSum - allLineSum * 0.5f)
						+ m_angleCos * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD - textPixelSX * nowLineWidthS),
						m_position.y + m_angleCos * top * m_showSize.x 
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum * 0.5f)
						+ m_angleSin * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD - textPixelSX * nowLineWidthS));
				}else
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_DOWN)
				{//左对齐，啥也不用做
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_showSize.y 
						- m_angleSin * textPixelY * (m_lineSum - allLineSum)
						+ m_angleCos * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD - textPixelSX * nowLineWidthS),
						m_position.y + m_angleCos * top * m_showSize.x 
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum)
						+ m_angleSin * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD - textPixelSX * nowLineWidthS));
				}
			}
		//	m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_showSize.y 
		//		- m_angleSin * textPixelY * m_lineSum
		//		+ m_angleCos * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS),
		//		m_position.y + m_angleCos * top * m_showSize.x 
		//		+ m_angleCos * textPixelY * m_lineSum
		//		+ m_angleSin * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS));
			m_textPageOrder[m_needShowTextSum] = pageNumble;
			m_needShowTextSum ++;
		}
	}
}

_XFontUnicode& _XFontUnicode::operator = (const _XFontUnicode& temp)
{
	if(this == &temp) return *this;		//防止自生拷贝
	if(temp.m_isInited == 0) return *this;	//如果目标没有初始化则直接退出
	
	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{//自身没有引用需要释放
		for(int i = 0;i < m_pageSum;++ i)
		{
			m_texture[i].release();
		}
		XDELETE_ARRAY(m_texture);
		XDELETE_ARRAY(m_fontPageText);

		XDELETE(m_cp);
	}
	m_cp = temp.m_cp;
	//release();

	_XFontBasic::operator =(temp);
#if WITH_OBJECT_MANAGER	//在物件管理的类中注销这些物件
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
	_XObjectManager::GetInstance().addAObject(this,OBJ_FONTUNICODE);
#endif

	//字体的副本不能有自己的资源，如果存在自己的资源则需要释放掉
//	m_isAcopy = 1;

	m_isVisiable = temp.m_isVisiable;			
	m_pageW = temp.m_pageW;			
	m_pageH = temp.m_pageH;

	m_fontPageText = temp.m_fontPageText;	//*
	m_texture = temp.m_texture;				//*						
	m_libFontSum = temp.m_libFontSum;		

	m_isCliped = temp.m_isCliped;		
	m_clipRect = temp.m_clipRect;		
	m_pageSum = temp.m_pageSum;

	for(int i = 0;i < STRING_MAX_SIZE;++ i)
	{
		m_textPosition[i] = temp.m_textPosition[i];
		m_textRect[i] = temp.m_textRect[i];
		m_textPageOrder[i] =temp.m_textPageOrder[i];
	}

	m_layout = temp.m_layout;		

	m_isPassword = temp.m_isPassword;
	return *this;
}

int _XFontUnicode::setACopy(const _XFontUnicode & temp)
{
	if(this == &temp) return 0;		//防止自生拷贝
	if(temp.m_isInited == 0) return 0;	//如果目标没有初始化则直接退出

	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{//自身没有引用需要释放
		for(int i = 0;i < m_pageSum;++ i)
		{
			m_texture[i].release();
		}
		XDELETE_ARRAY(m_texture);
		XDELETE_ARRAY(m_fontPageText);

		XDELETE(m_cp);
	}
	m_cp = temp.m_cp;

	//release();
	if(_XFontBasic::setACopy(temp) == 0) return 0;
#if WITH_OBJECT_MANAGER	//在物件管理的类中注销这些物件
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
	_XObjectManager::GetInstance().addAObject(this,OBJ_FONTUNICODE);
#endif

	//字体的副本不能有自己的资源，如果存在自己的资源则需要释放掉
//	m_isAcopy = 1;

	m_isVisiable = temp.m_isVisiable;			
	m_pageW = temp.m_pageW;			
	m_pageH = temp.m_pageH;

	m_fontPageText = temp.m_fontPageText;	//*
	m_texture = temp.m_texture;				//*						
	m_libFontSum = temp.m_libFontSum;		

	m_isCliped = temp.m_isCliped;		
	m_clipRect = temp.m_clipRect;		
	m_pageSum = temp.m_pageSum;

	for(int i = 0;i < STRING_MAX_SIZE;++ i)
	{
		m_textPosition[i] = temp.m_textPosition[i];
		m_textRect[i] = temp.m_textRect[i];
		m_textPageOrder[i] =temp.m_textPageOrder[i];
	}

	m_layout = temp.m_layout;		

	m_isPassword = temp.m_isPassword;
	return 1;
}

_XFontUnicode::_XFontUnicode(const _XFontUnicode & temp)
:_XFontBasic(temp)
{
	//if(this == &temp) return;		//防止自生拷贝
#if WITH_OBJECT_MANAGER	//在物件管理的类中注销这些物件
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
	_XObjectManager::GetInstance().addAObject(this,OBJ_FONTUNICODE);
#endif
	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;

	m_textPosition = new _XVector2[STRING_MAX_SIZE];
	m_textRect = new _XRect[STRING_MAX_SIZE];
	m_textPageOrder = new int[STRING_MAX_SIZE];

//	m_isAcopy = 1;

	m_pageW = temp.m_pageW;			
	m_pageH = temp.m_pageH;

	m_fontPageText = temp.m_fontPageText;	//*
	m_texture = temp.m_texture;				//*						
	m_libFontSum = temp.m_libFontSum;		

	m_isCliped = temp.m_isCliped;		
	m_clipRect = temp.m_clipRect;		
	m_pageSum = temp.m_pageSum;

	for(int i = 0;i < STRING_MAX_SIZE;++ i)
	{
		m_textPosition[i] = temp.m_textPosition[i];
		m_textRect[i] = temp.m_textRect[i];
		m_textPageOrder[i] =temp.m_textPageOrder[i];
	}

	m_layout = temp.m_layout;		

	m_isPassword = temp.m_isPassword;
}