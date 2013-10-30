//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XLogBook.h"
#include "assert.h"

#ifdef OS_LINUX
#include <stdarg.h>
#endif

#define _DEFAULT_LOG_FILE_NAME ("logBook.txt")

int _XLogbook::initLog(const char *fileName)
{
    if(m_isInited != 0) return 0;    //如果之前已经初始化过，则这里直接返回
    if(fileName == NULL || strlen(fileName) <= 0) return 0;
	int ret = 1;
    try
    {
        m_LogInfo = new _LogbookInfo[MAX_RECORD_SUM];
        if(m_LogInfo == NULL) ret = 0;
    }catch(...)
    {
        ret = 0;
    }
	//for(int i = 0;i < MAX_RECORD_SUM;++ i)
	//{
	//	m_LogInfo[i].isEnable = 0;
	//	m_LogInfo[i].logMessage = NULL;
	//}

    m_logInfoPoint = 0;
    //memset(m_LogInfo,0,sizeof(m_LogInfo));

	if(ret == 0) return 0;
    if(m_workMode == XLOGBOOK_MODE_SERVER)
    {//连接服务器
	//	if(m_socket.xCreateClient(LOG_SERVER_IP,LOG_SERVER_PORT) == 0)
	//	{
	//		m_workMode = XLOGBOOK_MODE_SIMPLE;
	//	}
        if(m_socket.connectToNet(LOG_SERVER_PORT,LOG_SERVER_NAME) == 0)
        {//连接服务器失败，使用默认的简单模式(目前的网络模块，这里如果连接不到服务器
			//，他会自动建立服务器，所以比较需要进一步的处理)
            m_workMode = XLOGBOOK_MODE_SIMPLE;
        }else
        {//连接服务器成功之后向服务器发送初始化信息
			if(m_socket.getIsServer() == 2)
			{
				int len = strlen(fileName) + 1;
				char *tempMessage = NULL;
				try
				{
					tempMessage = new char[len + 6];
					if(tempMessage == NULL) ret = 0;
				}catch(...)
				{
					ret = 0;
				}
				if(ret != 0)
				{
					strcpy(tempMessage,"sys00:");
					strcat(tempMessage,fileName);
					m_socket.sendDataToNet(0,tempMessage,strlen(tempMessage) + 1);
					XDELETE_ARRAY(tempMessage);
				}else
				{
					XDELETE_ARRAY(m_LogInfo);
					return 0;
				}
			}else
			{//连接服务器失败，则转成非服务器模式
				m_socket.disconnect();	//断开网络连接
				m_workMode = XLOGBOOK_MODE_SIMPLE;
			}
        }
    }
    if(m_workMode == XLOGBOOK_MODE_SIMPLE)
    {
        if((m_logBookFile = fopen(fileName,"w")) == NULL)
        {
            if((m_logBookFile = fopen(fileName,"a")) == NULL)
            {
				XDELETE_ARRAY(m_LogInfo);
                return 0;
            }
        }
    }

    m_logInfoPushPoint = 0;
    m_logInfoPopPoint = 0;
    //开启信息输出线程
#ifdef OS_LINUX
    if(pthread_create(&m_outputLogMessageThreadP, NULL, &outputLogMessageThread, (void*) this) != 0) 
#endif
#ifdef OS_WINDOWS
    if(CreateThread(0,0,outputLogMessageThread,this,0,&m_outputLogMessageThreadP) == 0) 
#endif
    {//线程建立失败，直接返回
		XDELETE_ARRAY(m_LogInfo);
        return 0;
    }

    m_isInited = 1;    //标记初始化已经完成
	m_isReleased = 0;
    return 1;
}

void _XLogbook::releaseLog()
{//将没有保存的记录,保存到文件中,并关闭文件
    if(m_isInited == 0) return;
    if(m_isReleased == 0)
    {
        m_isReleased = 1;
        while(1)
        {//等待多线程结束
			if(m_isReleased == 2) break;
            mySleep(1000);
        }

        if(m_workMode == XLOGBOOK_MODE_SIMPLE)
        {//简单模式才需要向日志文件中写入这个信息
            fprintf(m_logBookFile,"------ End Of File! ------");
            fclose(m_logBookFile);
        }else
		{//关闭网络连接
			m_socket.disconnect();
			
		}
		XDELETE_ARRAY(m_LogInfo);
		m_isInited = 0;
        m_isReleased = 1;    //标记资源已经结束
    }
}

void _XLogbook::popAllLogMessage()
{//record the date to file
	if(m_isInited == 0 || m_logBookFile == NULL) return;
    for(int i = 0;i < MAX_RECORD_SUM;i ++)
    {
        if(m_LogInfo[i].isEnable != 0)
        {
            fprintf(m_logBookFile,"%s",m_LogInfo[i].logMessage);
			XDELETE_ARRAY(m_LogInfo[i].logMessage);
            m_LogInfo[i].isEnable = 0;
        }
    }
}

int _XLogbook::doubleStringLength(char ** message,int messageSize)
{
	if(messageSize <= 0 || message == NULL) return 0;
    int len = messageSize;
    char * tempData = (*message);
    (*message) = NULL;

    messageSize = messageSize << 1;
    try
    {
        (*message) = new char [messageSize];
        if((*message) == NULL)
        {
			(* message) = tempData;	//不改变原来的数据
            printf("Memory Error:File:%s,Line:%d\n",__FILE__,__LINE__);
            return 0;
        }
    }catch(...)
    {
		(* message) = tempData;	//不改变原来的数据
        printf("Memory Error:File:%s,Line:%d\n",__FILE__,__LINE__);
        return 0;
    }
    memcpy((*message),tempData,len);
	XDELETE_ARRAY(tempData);
    return 1;
}

void _XLogbook::addLogInfoNull(const char *p,...)
{
    if(m_isInited == 0) initLog(_DEFAULT_LOG_FILE_NAME);

    int tempMessageSize = MAX_RECORD_LENGTH;
    char * tempMessage;
    try
    {
        tempMessage = new char [tempMessageSize];
        if(tempMessage == NULL) return;
    }catch(...)
    {
        return;
    }

    va_list arg_ptr;
    char c,nc;
    int charPoint = 0;

    va_start(arg_ptr,p);
    while(c = * p++) 
    {
        if(c == '%'&& (nc = *p) != '\0')
        {
            p++;
            switch(nc) 
            {
            case 'c': //%c
                {
                    /*为了内存对齐，所以要写int*/
                    char ch = va_arg(arg_ptr, int);
                    if(charPoint < (int)(tempMessageSize - 1 - sizeof(char)))
                    {
                        tempMessage[charPoint] = ch;
                    }else
                    {
                        if(doubleStringLength(&tempMessage,tempMessageSize) == 0) 
                        {
							XDELETE_ARRAY(tempMessage);
                            return;
                        }
                        tempMessageSize = tempMessageSize << 1;

                        tempMessage[charPoint] = ch;
                    }
                    ++ charPoint;
                    break;
                }
            case 's': //%s
                {
                    char *str = va_arg(arg_ptr, char *);

                    int len = tempMessageSize - 1 - strlen(str);
                    if(charPoint < len)
                    {
                        memcpy(&(tempMessage[charPoint]),str,strlen(str) + 1);
                    }else
                    {
                        while(1)
                        {
                            if(doubleStringLength(&tempMessage,tempMessageSize) == 0) 
                            {
								XDELETE_ARRAY(tempMessage);
                                return;
                            }
                            tempMessageSize = tempMessageSize << 1;
                            int len = tempMessageSize - 1 - strlen(str);
                            if(charPoint < len) break;
                        }

                        memcpy(&(tempMessage[charPoint]),str,strlen(str) + 1);
                    }

                    charPoint += strlen(str);
                    break;
                }
            case 'd'://%d
                {
                    int data = va_arg(arg_ptr,int);
                    char buf[16];
                    sprintf(buf,"%d",data);
                    if(charPoint < (int)(tempMessageSize - 1 - strlen(buf)))
                    {
                        memcpy(&(tempMessage[charPoint]),buf,strlen(buf));
                    }else
                    {
                        if(doubleStringLength(&tempMessage,tempMessageSize) == 0) 
                        {
							XDELETE_ARRAY(tempMessage);
                            return;
                        }
                        tempMessageSize = tempMessageSize << 1;
                        memcpy(&(tempMessage[charPoint]),buf,strlen(buf));
                    }
                    charPoint += strlen(buf);
                    break;
                }
            case 'f'://%f
                {
                    double data = va_arg(arg_ptr,double);    //注意这里的double 如果使用float 是会出错的
                    char buf[16];
                    sprintf(buf,"%f",data);
                    if(charPoint < (int)(tempMessageSize - 1 - strlen(buf)))
                    {
                        memcpy(&(tempMessage[charPoint]),buf,strlen(buf));
                    }else
                    {
                        if(doubleStringLength(&tempMessage,tempMessageSize) == 0) 
                        {
							XDELETE_ARRAY(tempMessage);
                            return;
                        }
                        tempMessageSize = tempMessageSize << 1;
                        memcpy(&(tempMessage[charPoint]),buf,strlen(buf));
                    }
                    charPoint += strlen(buf);
                    break;
                }
            default:
                {
                    if(charPoint < (int)(tempMessageSize - 1 - sizeof(char)))
                    {
                        tempMessage[charPoint] = '%';
                    }else
                    {
                        if(doubleStringLength(&tempMessage,tempMessageSize) == 0) 
                        {
							XDELETE_ARRAY(tempMessage);
                            return;
                        }
                        tempMessageSize = tempMessageSize << 1;

                        tempMessage[charPoint] = '%';
                    }
                    ++ charPoint;
                    if(charPoint < (int)(tempMessageSize - 1 - sizeof(char)))
                    {
                        tempMessage[charPoint] = nc;
                    }else
                    {
                        if(doubleStringLength(&tempMessage,tempMessageSize) == 0) 
                        {
							XDELETE_ARRAY(tempMessage);
                            return;
                        }
                        tempMessageSize = tempMessageSize << 1;

                        tempMessage[charPoint] = nc;
                    }
                    ++ charPoint;
                }
            }
        }else
        {
            if(charPoint < (int)(tempMessageSize - 1 - sizeof(char)))
            {
                tempMessage[charPoint] = c;
            }else
            {
                if(doubleStringLength(&tempMessage,tempMessageSize) == 0) 
                {
					XDELETE_ARRAY(tempMessage);
                    return;
                }
                tempMessageSize = tempMessageSize << 1;

                tempMessage[charPoint] = c;
            }
            ++ charPoint;
        }
        if((*p) == '\0') break;
    }
    va_end(arg_ptr);
    tempMessage[charPoint] = '\0';

    pushAMessage(tempMessage);
	XDELETE_ARRAY(tempMessage);
}

void _XLogbook::addLogInfoExp(bool exp,const char *p,...)
{
    if(exp) return;//满足则不需要打印调试信息
    if(m_isInited == 0)  initLog(_DEFAULT_LOG_FILE_NAME);

    int tempMessageSize = MAX_RECORD_LENGTH;
    char * tempMessage;
    try
    {
        tempMessage = new char [tempMessageSize];
        if(tempMessage == NULL) return;
    }catch(...)
    {
        return;
    }

    va_list arg_ptr;
    char c,nc;
    int charPoint = 0;

    va_start(arg_ptr,p);
    while(c = * p++) 
    {
        if(c == '%'&& (nc = *p) != '\0')
        {
            p++;
            switch(nc) 
            {
            case 'c': //%c
                {
                    /*为了内存对齐，所以要写int*/
                    char ch = va_arg(arg_ptr, int);
                    if(charPoint < (int)(tempMessageSize - 1 - sizeof(char)))
                    {
                        tempMessage[charPoint] = ch;
                    }else
                    {
                        if(doubleStringLength(&tempMessage,tempMessageSize) == 0) 
                        {
							XDELETE_ARRAY(tempMessage);
                            return;
                        }
                        tempMessageSize = tempMessageSize << 1;

                        tempMessage[charPoint] = ch;
                    }
                    ++ charPoint;
                    break;
                }
            case 's': //%s
                {
                    char *str = va_arg(arg_ptr, char *);
                    int len = tempMessageSize - 1 - strlen(str);
                    if(charPoint < len)
                    {
                        memcpy(&(tempMessage[charPoint]),str,strlen(str) + 1);
                    }else
                    {
                        while(1)
                        {
                            if(doubleStringLength(&tempMessage,tempMessageSize) == 0) 
                            {
								XDELETE_ARRAY(tempMessage);
                                return;
                            }
                            tempMessageSize = tempMessageSize << 1;
                            int len = tempMessageSize - 1 - strlen(str);
                            if(charPoint < len) break;
                        }

                        memcpy(&(tempMessage[charPoint]),str,strlen(str) + 1);
                    }
                    charPoint += strlen(str);
                    break;
                }
            case 'd'://%d
                {
                    int data = va_arg(arg_ptr,int);
                    char buf[16];
                    sprintf(buf,"%d",data);
                    if(charPoint < (int)(tempMessageSize - 1 - strlen(buf)))
                    {
                        memcpy(&(tempMessage[charPoint]),buf,strlen(buf));
                    }else
                    {
                        if(doubleStringLength(&tempMessage,tempMessageSize) == 0) 
                        {
							XDELETE_ARRAY(tempMessage);
                            return;
                        }
                        tempMessageSize = tempMessageSize << 1;
                        memcpy(&(tempMessage[charPoint]),buf,strlen(buf));
                    }
                    charPoint += strlen(buf);
                    break;
                }
            case 'f'://%f 
                {
                    double data = va_arg(arg_ptr,double);    //注意这里的double 如果使用float 是会出错的
                    char buf[16];
                    sprintf(buf,"%f",data);
                    if(charPoint < (int)(tempMessageSize - 1 - strlen(buf)))
                    {
                        memcpy(&(tempMessage[charPoint]),buf,strlen(buf));
                    }else
                    {
                        if(doubleStringLength(&tempMessage,tempMessageSize) == 0) 
                        {
							XDELETE_ARRAY(tempMessage);
                            return;
                        }
                        tempMessageSize = tempMessageSize << 1;
                        memcpy(&(tempMessage[charPoint]),buf,strlen(buf));
                    }
                    charPoint += strlen(buf);
                    break;
                }
            default:
                {
                    if(charPoint < (int)(tempMessageSize - 1 - sizeof(char)))
                    {
                        tempMessage[charPoint] = '%';
                    }else
                    {
                        if(doubleStringLength(&tempMessage,tempMessageSize) == 0) 
                        {
							XDELETE_ARRAY(tempMessage);
                            return;
                        }
                        tempMessageSize = tempMessageSize << 1;

                        tempMessage[charPoint] = '%';
                    }
                    ++ charPoint;
                    if(charPoint < (int)(tempMessageSize - 1 - sizeof(char)))
                    {
                        tempMessage[charPoint] = nc;
                    }else
                    {
                        if(doubleStringLength(&tempMessage,tempMessageSize) == 0) 
                        {
							XDELETE_ARRAY(tempMessage);
                            return;
                        }
                        tempMessageSize = tempMessageSize << 1;

                        tempMessage[charPoint] = nc;
                    }
                    ++ charPoint;
                }
            }
        }else
        {
            if(charPoint < (int)(tempMessageSize - 1 - sizeof(char)))
            {
                tempMessage[charPoint] = c;
            }else
            {
                if(doubleStringLength(&tempMessage,tempMessageSize) == 0) 
                {
					XDELETE_ARRAY(tempMessage);
                    return;
                }
                tempMessageSize = tempMessageSize << 1;

                tempMessage[charPoint] = c;
            }
            ++ charPoint;
        }
        if((*p) == '\0') break;
    }
    va_end(arg_ptr);
    tempMessage[charPoint] = '\0';

    pushAMessage(tempMessage);
	XDELETE_ARRAY(tempMessage);
}

int _XLogbook::pushAMessage(const char * message)    //向队列中推入一条信息    
{
    if(m_isInited == 0) initLog(_DEFAULT_LOG_FILE_NAME);

    int len = strlen(message) + 1;
    if(len <= 0) return 0;    //no message
    //++++++++++++++++++++++++++++++++++++++++
    //为每个日志加入时间戳
    _XSystemTime tempTime = getTimeMs();
    char *tempMessage = NULL;
    try
    {
        tempMessage = new char[len + RECORD_HEAD_LENGTH];
        if(tempMessage == NULL) return 0;
    }catch(...)
    {
        return 0;
    }
    sprintf(tempMessage,"%4d%2d%2d-%2d%2d%2d-%3d:",tempTime.year,tempTime.month,tempTime.day,
        tempTime.hour,tempTime.minute,tempTime.second,tempTime.millisecond);
    for(int i = 0 ;i < RECORD_HEAD_LENGTH;++ i)
    {
        if(tempMessage[i] == ' ') tempMessage[i] = '0';
    }
    strcat(tempMessage,message);
    len += RECORD_HEAD_LENGTH;
    //----------------------------------------
    //push message
    if(m_LogInfo[m_logInfoPushPoint].isEnable == 0)
    {//can input
		m_LogInfo[m_logInfoPushPoint].logMessage = tempMessage;
        m_LogInfo[m_logInfoPushPoint].isEnable = 1;
    }else
    {//full
		//delete now message
        m_LogInfo[m_logInfoPushPoint].isEnable = 0;
		XDELETE_ARRAY(m_LogInfo[m_logInfoPushPoint].logMessage);
		m_LogInfo[m_logInfoPushPoint].logMessage = tempMessage;
        m_LogInfo[m_logInfoPushPoint].isEnable = 1;
    }
    //move point
    ++ m_logInfoPushPoint;
    if(m_logInfoPushPoint >= MAX_RECORD_SUM)  m_logInfoPushPoint = 0;

	return 1;
}

int _XLogbook::popAMessage(char ** message)    //从队列中取出一条信息
{
    //if(m_isInited == 0) initLog(_DEFAULT_LOG_FILE_NAME);
	if(m_isInited == 0) return 0;

	if(m_LogInfo[m_logInfoPopPoint].isEnable != 0)
    {//have message
        //pop message
		XDELETE_ARRAY((* message));	//删除原有的内容
        (* message) = m_LogInfo[m_logInfoPopPoint].logMessage;
        m_LogInfo[m_logInfoPopPoint].isEnable = 0;
        m_LogInfo[m_logInfoPopPoint].logMessage = NULL;
        //move point
        ++ m_logInfoPopPoint;
        if(m_logInfoPopPoint >= MAX_RECORD_SUM) m_logInfoPopPoint = 0;
        return 1;
    }else
    {//empty
        return 0;
    }
}

#ifdef OS_WINDOWS
DWORD WINAPI _XLogbook::outputLogMessageThread(void * pParam)
#endif
#ifdef OS_LINUX
void *_XLogbook::outputLogMessageThread(void * pParam)
#endif
{
    _XLogbook *pPar = (_XLogbook *) pParam;

    char *tempMessage = NULL;
    int len = 0;
    char *sendMessage = NULL;

    int recvOrder = 0;
    int recvID = 0;
    int recvLength = 0;
    char *recvMessage = NULL;
    while(1)
    {
        //++++++++++++++++++++++++++++++++++++++++++++
        //处理从服务器接收到的信息
        if(pPar->m_workMode == XLOGBOOK_MODE_SERVER && pPar->m_socket.getDataFromNet(&recvOrder,&recvID,&recvMessage,&recvLength) != 0)
        {//接受到数据
            if(recvOrder == 0)
            {//从服务器接收到的消息，否则直接丢弃
                if(recvMessage[0] == 's' &&
                    recvMessage[1] == 'y' &&
                    recvMessage[2] == 's' &&
                    recvMessage[3] == '0' &&
                    recvMessage[4] == '1' &&
                    recvMessage[5] == ':')
                {//服务器根据自己的实际情况修改了日志的保存文件，并将文件名发回来，这里将服务器实际使用的日志文件名保存到本地
                    FILE *fp;
                    if((fp = fopen(_DEFAULT_LOG_FILE_NAME,"a")) == NULL)
                    {
                        if((fp = fopen(_DEFAULT_LOG_FILE_NAME,"w")) == NULL)
                        {//文件打开失败，直接抛弃这个信息
                            continue;
                        }
                    }
                    fprintf(fp,"%s",recvMessage + 6);
                    fclose(fp);
                }
            }
        }
        //--------------------------------------------
        if(pPar->popAMessage(&tempMessage) != 0)
        {//如果推出消息成功
            if(pPar->m_workMode == XLOGBOOK_MODE_SERVER)
            {//将数据发送到服务器
                len = strlen(tempMessage) + 1;
                try
                {
                    sendMessage = new char[len + 6];
                    if(sendMessage == NULL)
                    {
						XDELETE_ARRAY(tempMessage);
                        continue;
                    }
                }catch(...)
                {
					XDELETE_ARRAY(tempMessage);
                    continue;
                }
                strcpy(sendMessage,"msg00:");
                strcat(sendMessage,tempMessage);
                pPar->m_socket.sendDataToNet(0,sendMessage,strlen(sendMessage) + 1);

				XDELETE_ARRAY(sendMessage);
				XDELETE_ARRAY(tempMessage);
            }
            if(pPar->m_workMode == XLOGBOOK_MODE_SIMPLE)
            {//简单模式直接将数据写入到文件
                fprintf(pPar->m_logBookFile,"%s",tempMessage);
				XDELETE_ARRAY(tempMessage);
            }
        }else
        {//如果队列中的数据已经全部处理完成，则判断是否应该结束这个线程
            if(pPar->m_isReleased != 0)
            {
                //向服务器发送结束信息
                if(pPar->m_workMode == XLOGBOOK_MODE_SERVER)
                {
                    char tempMessage[] = "sys02:";
                    pPar->m_socket.sendDataToNet(0,tempMessage,strlen(tempMessage) + 1);
                }
                pPar->m_isReleased = 2;
                break;
            }
        }
        mySleep(1000);
    }

#ifdef OS_WINDOWS
    return 1;
#endif
}

_XLogbook::_XLogbook()
:m_isInited(0)
,m_isReleased(0)
,m_workMode(XLOGBOOK_MODE_SIMPLE)
,m_logBookFile(NULL)
,m_LogInfo(NULL)
,m_logInfoPoint(0)
,m_logInfoPushPoint(0)
,m_logInfoPopPoint(0)
{
}

_XLogbook::~_XLogbook()
{
    releaseLog();
}

_XLogbook& _XLogbook::GetInstance()
{
	static _XLogbook m_instance;
    return m_instance;
}

