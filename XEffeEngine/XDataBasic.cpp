//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XDataBasic.h"
#include "string.h"
#include "XBasicFun.h"
#include "XOSDefine.h"

#define DEBUG_XDB (0)

_XDataBasic::_XDataBasic()
:m_isInited(0)
,m_dataLength(0)
,m_pData(NULL)
,m_checkData(0)
,m_secretKeyLength(0)
,m_secretKey(NULL)
,m_nowBackOrder(0)
,m_ID(0)
,m_versionsOrder(0)
,m_isSaveAsynchronous(false)
,m_needSave(false)
,m_fileName(NULL)
,m_saveMode(ANALYZING_BUFFERING)
{
}
_XDataBasic::~_XDataBasic()
{
	release();
}

bool _XDataBasic::EncryptData()            //加密数据
{
    if(m_isInited == 0) return false;
    if(m_secretKeyLength <= 0) return false;
    if(m_secretKey == NULL) return false;
    if(m_dataLength <= 0) return false;
    if(m_pData == NULL) return false;
    //生成随机的密码
    for(int i = 0;i < m_secretKeyLength;++ i)
    {
        m_secretKey[i] = random(256);
    }
    //加密数据
    unsigned char temp;
    for(int i = 0,j = 0;i < m_dataLength;++ i,++j)
    {
        if(j >= m_secretKeyLength) j = 0;
        temp = m_secretKey[j] + (i%256);
        m_pData[i] = temp ^ m_pData[i];
    }
    return true;
}

bool _XDataBasic::DeCryptData()            //解密数据
{//同样的加密操作再进行一次就是解密
    if(m_isInited == 0) return false;
    if(m_secretKeyLength <= 0) return false;
    if(m_secretKey == NULL) return false;
    if(m_dataLength <= 0) return false;
    if(m_pData == NULL) return false;
    unsigned char temp;
    for(int i = 0,j = 0;i < m_dataLength;++ i,++j)
    {
        if(j >= m_secretKeyLength) j = 0;
        temp = m_secretKey[j] + (i%256);
        m_pData[i] = temp ^ m_pData[i];
    }
    return true;
}

unsigned char _XDataBasic::getCheckData()        //计算校验码
{
    if(m_isInited == 0) return 0;
    if(m_dataLength <= 0) return 0;
    if(m_pData == NULL) return 0;
    unsigned char temp = 0;
    for(int i = 0;i < m_dataLength;++ i)
    {
        temp += m_pData[i];
    }
    return temp;
}

bool _XDataBasic::checkDataIsRigth()    //检查数据是否符合校验
{
    if(m_isInited == 0) return false;
    if(m_dataLength <= 0) return false;
    if(m_pData == NULL) return false;
    if(getCheckData() != m_checkData)
    {
        return false;
    }else
    {
        return true;
    }
}

bool _XDataBasic::init(int ID,bool isSaveAsynchronous,ANALYZING_FILE_MODE saveMode)
{
    if(m_isInited != 0) return false;
    m_ID = ID;
    m_secretKeyLength = DATABASIC_KEY_LENGTH;
	m_isSaveAsynchronous = isSaveAsynchronous;
	m_saveMode = saveMode;
    try
    {
        m_secretKey = new unsigned char[m_secretKeyLength];
        if(m_secretKey == NULL)
        {
            return false;
        }
        for(int i = 0;i < m_secretKeyLength;++ i)
        {
            m_secretKey[i] = random(256);
        }
    }catch(...)
    {
        return false;
    }

	if(m_isSaveAsynchronous)
	{//如果使用异步数据保存方式这里开启保存函数
		m_isEnd = 0;
#ifdef OS_LINUX
		if(pthread_create(&m_saveDataThreadP, NULL, &saveDataThread, (void*) this) != 0)
#endif
#ifdef OS_WINDOWS
		if(CreateThread(0,0,saveDataThread,this,0,&m_saveDataThreadP) == 0)
#endif
		{//开启线程失败
			XDELETE_ARRAY(m_secretKey);
			return false;
		}
	}

    m_isInited = 1;
	return true;
}

void _XDataBasic::release()
{
	if(m_isInited == 0) return;
	if(m_isSaveAsynchronous)
	{//设置线程结束
		m_isEnd = 1;
		while(1)
		{//等待线程结束
			if(m_isEnd == 2) break;
			Sleep(1);
		}
	}
	XDELETE_ARRAY(m_secretKey);
	m_isInited = 0;
}

//内存与数据间的转换
bool _XDataBasic::mergeData(const unsigned char *pData,int dataLength)        //从外部合并数据到内部
{
    if(m_isInited == 0) return false;
    if(pData == NULL) return false;
    if(dataLength <= 0) return false;

    if(m_pData == NULL)
    {//第一次传入数据
        m_dataLength = dataLength;
        try
        {
            m_pData = new unsigned char[m_dataLength]; 
            if(m_pData == NULL) return false;
            memcpy(m_pData,pData,m_dataLength);
        }catch(...)
        {
            return false;
        }
    }else
    {
        if(m_dataLength == dataLength)
        {//不需要重新分配内存空间
            memcpy(m_pData,pData,m_dataLength);
        }else
        {
			XDELETE_ARRAY(m_pData);
            m_dataLength = dataLength;
            try
            {
                m_pData = new unsigned char[m_dataLength]; 
                if(m_pData == NULL) return false;
                memcpy(m_pData,pData,m_dataLength);
            }catch(...)
            {
                return false;
            }
        }
    }
    return true;
}

int _XDataBasic::analyzingData(unsigned char *pData,int dataLength)    //将内部数据解析到外部
{
    if(m_isInited == 0) return -1;
    if(dataLength <= 0) return -1;
    if(m_pData == NULL || m_dataLength <= 0) return 0;
    if(dataLength <= m_dataLength)
    {
        memcpy(pData,m_pData,dataLength);
    }else
    {
        memcpy(pData,m_pData,m_dataLength);
    }
    return m_dataLength;
}

bool _XDataBasic::mergeDataBack(const char *fileName)
{
    if(m_isInited == 0) return false;
    if(fileName == NULL || strlen(fileName) <= 0) return false;
    FILE *fp;
    if((fp = fopen(fileName,"rb")) == NULL)
    {//file open error!
        return false;
    }
    if(fread(&m_versionsOrder,sizeof(int),1,fp) < 1)
    {
        fclose(fp);
        return false;
    }
#if DEBUG_XDB
	printf("read:%s - %d\n",fileName,m_versionsOrder);
#endif
    if(fread(&m_ID,sizeof(int),1,fp) < 1)
    {
        fclose(fp);
        return false;
    }
    if(fread(&m_dataLength,sizeof(int),1,fp) < 1)
    {
        fclose(fp);
        return false;
    }
    if(m_dataLength > 0)
    {//需要提取数据部分
		XDELETE_ARRAY(m_pData);
        try
        {
            m_pData = new unsigned char[m_dataLength];
            if(m_pData == NULL)
            {
                fclose(fp);
                return false;    
            }
        }catch(...)
        {
            fclose(fp);
            return false;    
        }
    }
    if(fread(m_pData,m_dataLength,1,fp) < 1)
    {
        fclose(fp);
        return false;
    }
    if(fread(&m_secretKeyLength,sizeof(int),1,fp) < 1)
    {
        fclose(fp);
        return false;    
    }
    if(m_secretKeyLength > 0)
    {//需要提取数据部分
		XDELETE_ARRAY(m_secretKey);
        try
        {
            m_secretKey = new unsigned char[m_secretKeyLength];
            if(m_secretKey == NULL)
            {
                fclose(fp);
                return false;    
            }
        }catch(...)
        {
            fclose(fp);
            return false;    
        }
    }
    if(fread(m_secretKey,m_secretKeyLength,1,fp) < 1)
    {
        fclose(fp);
        return false;    
    }
    if(fread(&m_checkData,sizeof(unsigned char),1,fp) < 0)
    {
        fclose(fp);
        return false;    
    }
    if(m_checkData != getCheckData())
    {
        fclose(fp);
        return false;    
    }
    if(!DeCryptData())
    {
        fclose(fp);
        return false;    
    }
    fclose(fp);
    return true;
}

bool _XDataBasic::mergeDataBack(const char *fileNameA,const char *fileNameB)    //这个函数用于内部调用，有助于清晰化代码逻辑
{
    if(m_isInited == 0) return false;
	if(fileNameA == NULL || fileNameB == NULL) return false;
    int version0 = 0;
    int version1 = 0;
    int stateFile0 = 1;
    int stateFile1 = 1;
    FILE *fp;
    //提取第一个文件的版本号
    if((fp = fopen(fileNameA,"rb")) == NULL)
    {//file open error!
		//printf("%s Open Error!\n",fileNameA);
        stateFile0 = 0;
    }
    if(stateFile0 == 1)
    {
        if(fread(&version0,sizeof(int),1,fp) < 1)
        {
            stateFile0 = 0;
        }
        fclose(fp);
    }
#if DEBUG_XDB
	printf("%s - %d\n",fileNameA,version0);
#endif
    //提取第二个文件的版本号
    if((fp = fopen(fileNameB,"rb")) == NULL)
    {//file open error!
		//printf("%s Open Error!\n",fileNameB);
        stateFile1 = 0;
    }
    if(stateFile1 == 1)
    {
        if(fread(&version1,sizeof(int),1,fp) < 1)
        {
            stateFile1 = 0;
        }
        fclose(fp);
    }
#if DEBUG_XDB
	printf("%s - %d\n",fileNameB,version1);
#endif
    
    if(stateFile0 == 0 && stateFile1 == 0)
    {//如果两个文件都不能打开，则直接返回失败
        return false;
    }

    if(stateFile0 == 0 || stateFile1 == 0)
    {//有一个文件已经损坏，则从没有损坏的文件中提取数据
        if(stateFile0 == 1)
        {//从第一个文件中提取数据
            return mergeDataBack(fileNameA);
        }else
        {
            return mergeDataBack(fileNameB);        
        }
    }

    if(stateFile0 != 0 && stateFile1 != 0)
    {//如果暂时两个文件都没有损坏则先提取最近的版本
//        int tempResult = true;
        if(version0 > version1)
        {//提取第一个文件
            if(mergeDataBack(fileNameA))
            {
                return true;
            }
        }else
        {//提取第二个文件
            if(mergeDataBack(fileNameB))
            {
                return true;
            }
        }

        //如果提取失败则从另一个文件中提取
        if(version0 > version1)
        {//提取第一个文件
            if(mergeDataBack(fileNameB))
            {
                return true;
            }
        }else
        {//提取第二个文件
            if(mergeDataBack(fileNameA))
            {
                return true;
            }
        }
        //如果全部提取失败则返回失败
        return false;
    }
	return true;
}

//文件与数据间的转换
bool _XDataBasic::mergeData(const char *fileName)            //从文件中提取数据
{//从两个文件中分别提取数据，如果全部错误，则返回错误，如果全部成功，则取最近的版本，否则提取成功的版本
    if(m_isInited == 0) return false;

    if(fileName != NULL)
    {
        char *fileRealNameA = NULL;
        char *fileRealNameB = NULL;
        int len = strlen(fileName);
        if(len <= 0) return 0;
        try
        {
            fileRealNameA = new char[len + 2];
            if(fileRealNameA == NULL) return false;
            fileRealNameB = new char[len + 2];
            if(fileRealNameB == NULL) return false;
        }catch(...)
        {
            return false;
        }
        //确定文件名
        int dotPoint = -1;
        for(int i = 0;i < len;++ i)
        {
            if(fileName[i] == '.')
            {
                dotPoint = i;
            }
        }
        if(dotPoint == -1)
        {
            memcpy(fileRealNameA,fileName,len);
            fileRealNameA[len] = 'A';
            fileRealNameA[len + 1] = '\0';
            memcpy(fileRealNameB,fileName,len);
            fileRealNameB[len] = 'B';
            fileRealNameB[len + 1] = '\0';
        }else
        {
            memcpy(fileRealNameA,fileName,dotPoint);
            fileRealNameA[dotPoint] = 'A';
            memcpy(fileRealNameA + dotPoint + 1,fileName + dotPoint,len - dotPoint);
            fileRealNameA[len + 1] = '\0';
            memcpy(fileRealNameB,fileName,dotPoint);
            fileRealNameB[dotPoint] = 'B';
            memcpy(fileRealNameB + dotPoint + 1,fileName + dotPoint,len - dotPoint);
            fileRealNameB[len + 1] = '\0';
        }
        if(mergeDataBack(fileRealNameA,fileRealNameB))
        {
			XDELETE_ARRAY(fileRealNameA);
			XDELETE_ARRAY(fileRealNameB);
            return true;
        }else
        {
			XDELETE_ARRAY(fileRealNameA);
			XDELETE_ARRAY(fileRealNameB);
            return false;
        }
    }else
    {
        return mergeDataBack(DATABASIC_DEFAULT_FILE_NAME_A,DATABASIC_DEFAULT_FILE_NAME_B);
    }
}

bool _XDataBasic::analyzingDataBack(const char *fileName)    //这个函数用于内部调用，有助于清晰化代码逻辑
{
    if(m_isInited == 0) return false;
    if(fileName == NULL || strlen(fileName) <= 0) return false;
	if(m_saveMode == ANALYZING_BUFFERING)
	{
		FILE *fp;
		if((fp = fopen(fileName,"wb")) == NULL)
		{//file open error!
			return false;
		}
		//将数据写入到文件
		if(!EncryptData())
		{
			fclose(fp);
			return false;
		}
#if DEBUG_XDB
		printf("write:%s - %d\n",m_fileName,m_versionsOrder);
#endif	    
		if(fwrite(&m_versionsOrder,sizeof(int),1,fp) < 1) 
		{
			fclose(fp);
			return false;
		}
		if(fwrite(&m_ID,sizeof(int),1,fp) < 1) 
		{
			fclose(fp);
			return false;
		}
		if(m_dataLength < 0) m_dataLength = 0;
		if(fwrite(&m_dataLength,sizeof(int),1,fp) < 1) 
		{
			fclose(fp);
			return false;
		}

		if(m_pData != NULL && m_dataLength > 0)
		{
			if(fwrite(m_pData,m_dataLength,1,fp) < 1)  
			{
				fclose(fp);
				return false;
			}
		}
		if(m_secretKeyLength < 0) m_secretKeyLength = 0;
		if(fwrite(&m_secretKeyLength,sizeof(int),1,fp) < 1) 
		{
			fclose(fp);
			return false;
		}

		if(m_secretKey != NULL && m_secretKeyLength > 0)
		{
			if(fwrite(m_secretKey,m_secretKeyLength,1,fp) < 1) 
			{
				fclose(fp);
				return false;
			}
		}
		m_checkData = getCheckData();    //计算校验数据
		if(fwrite(&m_checkData,sizeof(unsigned char),1,fp) < 1)  
		{
			fclose(fp);
			return false;
		}
		if(!DeCryptData())
		{//这是一个非常严重的错误，所有的数据在还原的过程中造成了严重的破坏，无法再还原
		}

		fclose(fp);
	//	printf("%s:Write well!\n",fileName);
	}else
	{
#ifdef OS_WINDOWS
		if(m_fileName == NULL || strlen(m_fileName) != strlen(fileName))
		{
			XDELETE_ARRAY(m_fileName);
			try
			{
				m_fileName = new char[strlen(fileName) + 1];
				if(m_fileName == NULL) return false;
			}catch(...)
			{
				return false;
			}
		}
		strcpy(m_fileName,fileName);
		HANDLE tempFile;
		DWORD tempWriteLength;
	/*		wchar_t *wText = NULL;
		try
		{
			int len = strlen(m_fileName)/2 + 1;
			wText = new wchar_t[len];
			if(wText == NULL) return false;
		}catch(...)
		{
			return false;
		}

		MultiByteToWideChar(CP_ACP,0,m_fileName,strlen(m_fileName),wText,strlen(m_fileName)); 
		tempFile = CreateFileW(wText,
								GENERIC_WRITE,
								FILE_SHARE_WRITE | FILE_SHARE_READ,
								NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN | FILE_FLAG_WRITE_THROUGH,NULL);*/
		tempFile = CreateFile(m_fileName,
								GENERIC_WRITE,
								FILE_SHARE_WRITE | FILE_SHARE_READ,
								NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN | FILE_FLAG_WRITE_THROUGH,NULL);
		if(INVALID_HANDLE_VALUE == tempFile) return false;
		tempWriteLength = 0;

		//将数据写入到文件
		if(!EncryptData())
		{
			CloseHandle(tempFile);
			return false;
		}
#if DEBUG_XDB
		printf("write:%s - %d\n",m_fileName,m_versionsOrder);
#endif
		if(WriteFile(tempFile,&m_versionsOrder,sizeof(int),&tempWriteLength,NULL) == FALSE)
		{
			CloseHandle(tempFile);
			return false;
		}
		if(WriteFile(tempFile,&m_ID,sizeof(int),&tempWriteLength,NULL) == FALSE)
		{
			CloseHandle(tempFile);
			return false;
		}
		if(m_dataLength < 0) m_dataLength = 0;
		if(WriteFile(tempFile,&m_dataLength,sizeof(int),&tempWriteLength,NULL) == FALSE)
		{
			CloseHandle(tempFile);
			return false;
		}
		if(m_pData != NULL && m_dataLength > 0)
		{
			if(WriteFile(tempFile,m_pData,m_dataLength,&tempWriteLength,NULL) == FALSE)
			{
				CloseHandle(tempFile);
				return false;
			}
		}
		if(m_secretKeyLength < 0) m_secretKeyLength = 0;
		if(WriteFile(tempFile,&m_secretKeyLength,sizeof(int),&tempWriteLength,NULL) == FALSE)
		{
			CloseHandle(tempFile);
			return false;
		}

		if(m_secretKey != NULL && m_secretKeyLength > 0)
		{
			if(WriteFile(tempFile,m_secretKey,m_secretKeyLength,&tempWriteLength,NULL) == FALSE)
			{
				CloseHandle(tempFile);
				return false;
			}
		}
		m_checkData = getCheckData();    //计算校验数据
		if(WriteFile(tempFile,&m_checkData,sizeof(unsigned char),&tempWriteLength,NULL) == FALSE)
		{
			CloseHandle(tempFile);
			return false;
		}

		if(!DeCryptData())
		{//这是一个非常严重的错误，所有的数据在还原的过程中造成了严重的破坏，无法再还原
		}

		CloseHandle(tempFile);
		//XDELETE_ARRAY(wText);
#endif
#ifdef OS_LINUX
		//Linux下的数据立即保存还没有完成
		if(m_fileName == NULL || strlen(m_fileName) != strlen(fileName))
		{
			XDELETE_ARRAY(m_fileName);
			try
			{
				m_fileName = new char[strlen(fileName) + 1];
				if(m_fileName == NULL) return false;
			}catch(...)
			{
				return false;
			}
		}
		strcpy(m_fileName,fileName);
		FILE *fp;
		if((fp = fopen(m_fileName,"wb")) == NULL)
		{
			return false;
		}
		//将数据写入到文件
		if(!EncryptData())
		{
			CloseHandle(tempFile);
			return false;
		}
		fwrite(&m_versionsOrder,sizeof(int),1,fp);
		fwrite(&m_ID,sizeof(int),1,fp);
		if(m_dataLength < 0) m_dataLength = 0;
		fwrite(&m_dataLength,sizeof(int),1,fp);
		if(m_pData != NULL && m_dataLength > 0)
		{
			fwrite(m_pData,m_dataLength,1,fp);
		}
		if(m_secretKeyLength < 0) m_secretKeyLength = 0;
		fwrite(&m_secretKeyLength,sizeof(int),1,fp);
		if(m_secretKey != NULL && m_secretKeyLength > 0)
		{
			fwrite(m_secretKey,m_secretKeyLength,1,fp);
		}
		m_checkData = getCheckData();    //计算校验数据
		fwrite(&m_checkData,sizeof(unsigned char),1,fp);

		if(!DeCryptData())
		{//这是一个非常严重的错误，所有的数据在还原的过程中造成了严重的破坏，无法再还原
		}

		fclose(fp);
#endif
	//	printf("%s:Write OK!\n",m_fileName);
	//	system("pause");
	}
    return true;
}

bool _XDataBasic::analyzingDataManager(const char *fileName)        //将数据保存到文件，这个函数会根据之前用户的设置对保存的具体操作进行调度
{
	if(m_isInited == 0) return false;
	if(m_isSaveAsynchronous)
	{
		if(!m_needSave)
		{
			if(fileName != NULL)
			{
				if(m_fileName == NULL || strlen(fileName) != strlen(m_fileName))
				{
					XDELETE_ARRAY(m_fileName);
					int len = strlen(fileName);
					try
					{
						m_fileName = new char[len + 1];
						if(m_fileName == NULL) return false;
					}catch(...)
					{
						return false;
					}
				}
				strcpy(m_fileName,fileName);
			}else
			{
				XDELETE_ARRAY(m_fileName);
			}
			m_needSave = true;
			return true;
		}else
		{//上次标记的数据尚未保存
			return false;
		}
	}else
	{
		return analyzingData(fileName);
	}
}

bool _XDataBasic::analyzingData(const char *fileName)        //将数据保存到文件
{
    if(m_isInited == 0) return false;
    char *fileRealName = NULL;
    bool result = true;
    ++ m_versionsOrder;
    if(fileName != NULL)
    {
        int len = strlen(fileName);
        if(len <= 0) return 0;
        try
        {
            fileRealName = new char[len + 2];
            if(fileRealName == NULL) return false;
        }catch(...)
        {
            return false;
        }
        //确定文件名
        int dotPoint = -1;
        for(int i = 0;i < len;++ i)
        {
            if(fileName[i] == '.')
            {
                dotPoint = i;
            }
        }
        if(dotPoint == -1)
        {
            memcpy(fileRealName,fileName,len);
            if(m_nowBackOrder == 0)
            {
                fileRealName[len] = 'A';
            }else
            {
                fileRealName[len] = 'B';
            }
            fileRealName[len + 1] = '\0';
        }else
        {
            memcpy(fileRealName,fileName,dotPoint);
            if(m_nowBackOrder == 0)
            {
                fileRealName[dotPoint] = 'A';
            }else
            {
                fileRealName[dotPoint] = 'B';
            }
            memcpy(fileRealName + dotPoint + 1,fileName + dotPoint,len - dotPoint);
            fileRealName[len + 1] = '\0';
        }
        result = analyzingDataBack(fileRealName);
		XDELETE_ARRAY(fileRealName);
    }else
    {//使用默认的文件名提取数据
        if(m_nowBackOrder == 0)
        {
            result = analyzingDataBack(DATABASIC_DEFAULT_FILE_NAME_A);
        }else
        {
            result = analyzingDataBack(DATABASIC_DEFAULT_FILE_NAME_B);
        }
    }

    if(m_nowBackOrder == 0) m_nowBackOrder = 1;
    else m_nowBackOrder = 0;
    return result;
}
//网络与数据间的转换(目前暂时并不提供这个支持)

//用于异步数据保存的函数
DWORD WINAPI _XDataBasic::saveDataThread(void * pParam)
{
	_XDataBasic *pPar = (_XDataBasic *) pParam;
	
	while(1)
	{
		if((*pPar).m_needSave)
		{//下面进行数据保存
			(*pPar).analyzingData((*pPar).m_fileName);
		}
		if((*pPar).m_isEnd != 0) break;
		mySleep(1);
	}
	(*pPar).m_isEnd = 2;
	return 1;
}