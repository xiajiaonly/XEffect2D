//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XConfigManager.h"
void callbackProcMD(void *pClass,int ID)
{
	_XConfigManager *pPar = (_XConfigManager *)pClass;
	for(int i = 0;i < pPar->m_pItems.size();++ i)
	{
		if(pPar->m_pItems[i]->m_pCtrl != NULL)
		{
			if(pPar->m_pItems[i]->m_pCtrl->getControlID() == ID)
			{//处理事件
				if(i == pPar->m_minuteIndex && pPar->m_minuteSld.getVisiable()) pPar->m_minuteSld.disVisiable();
				break;
			}
		}
	}
}
void callbackProc(void *pClass,int ID)
{
	_XConfigManager *pPar = (_XConfigManager *)pClass;
	//功能按键
	if(pPar->m_minuteBtn.getControlID() == ID)
	{//微调按钮
		if(pPar->m_minuteSld.getVisiable()) pPar->m_minuteSld.disVisiable();
		else 
		{
			pPar->updateMinuteSld();
			pPar->m_minuteSld.setVisiable();
		}
	}
	if(pPar->m_saveBtn.getControlID() == ID)
	{//保存
		pPar->save();
		pPar->setOperateToServer(CFG_NET_OP_SAVE);
		return;
	}
	if(pPar->m_loadBtn.getControlID() == ID)
	{//读取
		if(pPar->m_configMode == CFG_MODE_CLIENT)
		{
			pPar->setOperateToServer(CFG_NET_OP_LOAD);
		}else
		if(pPar->m_configMode == CFG_MODE_SERVER)
		{
			pPar->load();
			pPar->sendCFGInfo();
		}else
		{
			pPar->load();
		}
		return;
	}
	if(pPar->m_defaultBtn.getControlID() == ID)
	{//默认
		if(pPar->m_configMode == CFG_MODE_CLIENT)
		{
			pPar->setOperateToServer(CFG_NET_OP_DEFAULT);
		}else
		if(pPar->m_configMode == CFG_MODE_SERVER)
		{
			pPar->setDefault();
			pPar->sendCFGInfo();
		}else
		{
			pPar->setDefault();
		}
		return;
	}
	if(pPar->m_netUpdateBtn.getControlID() == ID)
	{//同步
		pPar->sendSynchToServer();
		return;
	}	
	if(pPar->m_netInjectBtn.getControlID() == ID)
	{//注入
		pPar->sendInject();
		return;
	}
	//配置项
	if(pPar->m_minuteSld.getControlID() == ID && pPar->m_minuteSld.getVisiable())
	{//微调按钮
		int i = pPar->m_minuteIndex;
		if(i < 0 || i >= pPar->m_pItems.size()) return;
		switch(pPar->m_pItems[i]->m_type)
		{
		case CFG_DATA_TYPE_INT:
			if(pPar->m_pItems[i]->m_nowValue.valueI != pPar->m_minuteSld.getNowValue())
			{
				pPar->m_pItems[i]->m_nowValue.valueI = pPar->m_minuteSld.getNowValue();
				* (int *)pPar->m_pItems[i]->m_pVariable = pPar->m_minuteSld.getNowValue();
				((_XSlider *)(pPar->m_pItems[i]->m_pCtrl))->setNowValue(pPar->m_minuteSld.getNowValue());
				if(pPar->m_pItems[i]->m_changeProc != NULL) pPar->m_pItems[i]->m_changeProc();	//在数值变化之后才调用回调函数
				pPar->sendItemValue(pPar->m_pItems[i]);	//如果是网络模式，则通知网络设备更新相应的值
			}
			break;
		case CFG_DATA_TYPE_CHAR:
			if(pPar->m_pItems[i]->m_nowValue.valueI != pPar->m_minuteSld.getNowValue())
			{
				pPar->m_pItems[i]->m_nowValue.valueI = pPar->m_minuteSld.getNowValue();
				* (char *)pPar->m_pItems[i]->m_pVariable = pPar->m_minuteSld.getNowValue();
				if(pPar->m_pItems[i]->m_changeProc != NULL) pPar->m_pItems[i]->m_changeProc();	//在数值变化之后才调用回调函数
				pPar->sendItemValue(pPar->m_pItems[i]);	//如果是网络模式，则通知网络设备更新相应的值
			}
		case CFG_DATA_TYPE_UCHAR:
			if(pPar->m_pItems[i]->m_nowValue.valueI != pPar->m_minuteSld.getNowValue())
			{
				pPar->m_pItems[i]->m_nowValue.valueI = pPar->m_minuteSld.getNowValue();
				* (unsigned char *)pPar->m_pItems[i]->m_pVariable = pPar->m_minuteSld.getNowValue();
				((_XSlider *)(pPar->m_pItems[i]->m_pCtrl))->setNowValue(pPar->m_minuteSld.getNowValue());
				if(pPar->m_pItems[i]->m_changeProc != NULL) pPar->m_pItems[i]->m_changeProc();	//在数值变化之后才调用回调函数
				pPar->sendItemValue(pPar->m_pItems[i]);	//如果是网络模式，则通知网络设备更新相应的值
			}
			break;
		case CFG_DATA_TYPE_FLOAT:
			if(pPar->m_pItems[i]->m_nowValue.valueF != pPar->m_minuteSld.getNowValue())
			{
				pPar->m_pItems[i]->m_nowValue.valueF = pPar->m_minuteSld.getNowValue();
				* (float *)pPar->m_pItems[i]->m_pVariable = pPar->m_minuteSld.getNowValue();
				((_XSlider *)(pPar->m_pItems[i]->m_pCtrl))->setNowValue(pPar->m_minuteSld.getNowValue());
				if(pPar->m_pItems[i]->m_changeProc != NULL) pPar->m_pItems[i]->m_changeProc();	//在数值变化之后才调用回调函数
				pPar->sendItemValue(pPar->m_pItems[i]);	//如果是网络模式，则通知网络设备更新相应的值
			}
			break;
		}
		return;
	}
	for(int i = 0;i < pPar->m_pItems.size();++ i)
	{
		if(pPar->m_pItems[i]->m_pCtrl != NULL)
		{
			if(pPar->m_pItems[i]->m_pCtrl->getControlID() == ID)
			{//处理事件
				switch(pPar->m_pItems[i]->m_type)
				{
				case CFG_DATA_TYPE_INT:
					if(pPar->m_pItems[i]->m_nowValue.valueI != ((_XSlider *)(pPar->m_pItems[i]->m_pCtrl))->getNowValue())
					{
						pPar->m_pItems[i]->m_nowValue.valueI = ((_XSlider *)(pPar->m_pItems[i]->m_pCtrl))->getNowValue();
						* (int *)pPar->m_pItems[i]->m_pVariable = ((_XSlider *)(pPar->m_pItems[i]->m_pCtrl))->getNowValue();
						if(pPar->m_pItems[i]->m_changeProc != NULL) pPar->m_pItems[i]->m_changeProc();	//在数值变化之后才调用回调函数
						pPar->sendItemValue(pPar->m_pItems[i]);	//如果是网络模式，则通知网络设备更新相应的值
					}
					break;
				case CFG_DATA_TYPE_CHAR:
					if(pPar->m_pItems[i]->m_nowValue.valueI != ((_XSlider *)(pPar->m_pItems[i]->m_pCtrl))->getNowValue())
					{
						pPar->m_pItems[i]->m_nowValue.valueI = ((_XSlider *)(pPar->m_pItems[i]->m_pCtrl))->getNowValue();
						* (char *)pPar->m_pItems[i]->m_pVariable = ((_XSlider *)(pPar->m_pItems[i]->m_pCtrl))->getNowValue();
						if(pPar->m_pItems[i]->m_changeProc != NULL) pPar->m_pItems[i]->m_changeProc();	//在数值变化之后才调用回调函数
						pPar->sendItemValue(pPar->m_pItems[i]);	//如果是网络模式，则通知网络设备更新相应的值
					}
				case CFG_DATA_TYPE_UCHAR:
					if(pPar->m_pItems[i]->m_nowValue.valueI != ((_XSlider *)(pPar->m_pItems[i]->m_pCtrl))->getNowValue())
					{
						pPar->m_pItems[i]->m_nowValue.valueI = ((_XSlider *)(pPar->m_pItems[i]->m_pCtrl))->getNowValue();
						* (unsigned char *)pPar->m_pItems[i]->m_pVariable = ((_XSlider *)(pPar->m_pItems[i]->m_pCtrl))->getNowValue();
						if(pPar->m_pItems[i]->m_changeProc != NULL) pPar->m_pItems[i]->m_changeProc();	//在数值变化之后才调用回调函数
						pPar->sendItemValue(pPar->m_pItems[i]);	//如果是网络模式，则通知网络设备更新相应的值
					}
					break;
				case CFG_DATA_TYPE_FLOAT:
					if(pPar->m_pItems[i]->m_nowValue.valueF != ((_XSlider *)(pPar->m_pItems[i]->m_pCtrl))->getNowValue())
					{
						pPar->m_pItems[i]->m_nowValue.valueF = ((_XSlider *)(pPar->m_pItems[i]->m_pCtrl))->getNowValue();
						* (float *)pPar->m_pItems[i]->m_pVariable = ((_XSlider *)(pPar->m_pItems[i]->m_pCtrl))->getNowValue();
						if(pPar->m_pItems[i]->m_changeProc != NULL) pPar->m_pItems[i]->m_changeProc();	//在数值变化之后才调用回调函数
						pPar->sendItemValue(pPar->m_pItems[i]);	//如果是网络模式，则通知网络设备更新相应的值
					}
					break;
				case CFG_DATA_TYPE_XBOOL:
					if(pPar->m_pItems[i]->m_nowValue.valueB != ((_XCheck *)(pPar->m_pItems[i]->m_pCtrl))->getState())
					{
						pPar->m_pItems[i]->m_nowValue.valueB = ((_XCheck *)(pPar->m_pItems[i]->m_pCtrl))->getState();
						* (_XBool *)pPar->m_pItems[i]->m_pVariable = ((_XCheck *)(pPar->m_pItems[i]->m_pCtrl))->getState();
						if(pPar->m_pItems[i]->m_changeProc != NULL) pPar->m_pItems[i]->m_changeProc();	//在数值变化之后才调用回调函数
						pPar->sendItemValue(pPar->m_pItems[i]);	//如果是网络模式，则通知网络设备更新相应的值
					}
					break;
				case CFG_DATA_TYPE_RADIOS:
					if(pPar->m_pItems[i]->m_nowValue.valueI != ((_XRadios *)(pPar->m_pItems[i]->m_pCtrl))->getNowChoose())
					{
						pPar->m_pItems[i]->m_nowValue.valueI = ((_XRadios *)(pPar->m_pItems[i]->m_pCtrl))->getNowChoose();
						* (int *)pPar->m_pItems[i]->m_pVariable = ((_XRadios *)(pPar->m_pItems[i]->m_pCtrl))->getNowChoose();
						if(pPar->m_pItems[i]->m_changeProc != NULL) pPar->m_pItems[i]->m_changeProc();	//在数值变化之后才调用回调函数
						pPar->sendItemValue(pPar->m_pItems[i]);	//如果是网络模式，则通知网络设备更新相应的值
					}
					break;
				}
				break;
			}
		}
	}
	//判断是否是群组控件的状态发生了变化
	for(int i = 0;i < pPar->m_pGroups.size();++ i)
	{
		if(pPar->m_pGroups[i]->m_group.getControlID() == ID)
		{//群组控件的状态发生变化的时候改变整体布局
			pPar->relayout();
			return;
		}
	}
}
bool _XConfigManager::save(const char *filename)
{
	FILE *fp = NULL;
	if(filename == NULL)
	{
		if((fp = fopen(CFG_DEFAULT_FILENAME,"w")) == NULL) return false;
	}else
	{
		if((fp = fopen(filename,"w")) == NULL) return false;
	}
	for(int i = 0;i < m_pItems.size();++ i)
	{
		if(m_pItems[i]->m_isActive)
		{//被动的值不需要保存
			switch(m_pItems[i]->m_type)
			{
			case CFG_DATA_TYPE_INT:
			case CFG_DATA_TYPE_CHAR:
			case CFG_DATA_TYPE_UCHAR:
				fprintf(fp,"%s:%d,\n",m_pItems[i]->m_name.c_str(),m_pItems[i]->m_nowValue.valueI);
				break;
			case CFG_DATA_TYPE_FLOAT:
				fprintf(fp,"%s:%f,\n",m_pItems[i]->m_name.c_str(),m_pItems[i]->m_nowValue.valueF);
				break;
			case CFG_DATA_TYPE_XBOOL:
				if(m_pItems[i]->m_nowValue.valueB) fprintf(fp,"%s:1,\n",m_pItems[i]->m_name.c_str());
				else fprintf(fp,"%s:0,\n",m_pItems[i]->m_name.c_str());
				break;
			case CFG_DATA_TYPE_RADIOS:
				fprintf(fp,"%s:%d,\n",m_pItems[i]->m_name.c_str(),m_pItems[i]->m_nowValue.valueI);
				break;
			case CFG_DATA_TYPE_CUSTOM:
				m_pItems[i]->m_customIt->save(fp);
				break;
			}
		}
	}
	fclose(fp);
	return true;
}
//bool _XConfigManager::load(const char *filename)	//优化:建议这里判断读取项的名字是否正确，如果不对的话直接返回读取失败
//{
//	FILE *fp = NULL;
//	if(filename == NULL)
//	{
//		if((fp = fopen(CFG_DEFAULT_FILENAME,"r")) == NULL) return false;
//	}else
//	{
//		if((fp = fopen(filename,"r")) == NULL) return false;
//	}
//	char lineData[1024];
//	int len = 0;
//	for(int i = 0;i < m_pItems.size();++ i)
//	{
//		if(!m_pItems[i]->m_isActive) continue;	//被动的值不会读取
//		//读取一行数据
//		if(m_pItems[i]->m_type == CFG_DATA_TYPE_CUSTOM)
//		{
//			if(!m_pItems[i]->m_customIt->load(fp))
//			{
//				fclose(fp);
//				return false;
//			}
//			continue;
//		}
//		if(fgets(lineData,1024,fp) == NULL) break;
//		//计算名字
//		len = getCharPosition(lineData,':') + 1;
//		if(len < 0) 
//		{
//			fclose(fp);
//			return false;
//		}
//		lineData[len - 1] = '\0';
//		if(strcmp(lineData,m_pItems[i]->m_name.c_str()) != 0)
//		{
//			fclose(fp);
//			return false;
//		}
//		if(!getItemValueFromStr(m_pItems[i],lineData + len))
//		{
//			fclose(fp);
//			return false;
//		}
//		if(m_pItems[i]->m_changeProc != NULL) m_pItems[i]->m_changeProc();	//fix bug:读取配置参数时，没有调用其附带的函数
//	}
//	fclose(fp);
//	return true;
//}
bool _XConfigManager::getItemValueFromStr(_XConfigItem *it,const char *str)
{//这里不进行输入参数合理性检查
	int temp;
	switch(it->m_type)
	{
	case CFG_DATA_TYPE_INT:
		if(sscanf(str,"%d,\n",&it->m_nowValue.valueI) == 0) return false;
		* (int *)it->m_pVariable = it->m_nowValue.valueI;
		((_XSlider *)it->m_pCtrl)->setNowValue(it->m_nowValue.valueI);
		break;
	case CFG_DATA_TYPE_CHAR:
		if(sscanf(str,"%d,\n",&it->m_nowValue.valueI) == 0)  return false;
		* (char *)it->m_pVariable = it->m_nowValue.valueI;
		((_XSlider *)it->m_pCtrl)->setNowValue(it->m_nowValue.valueI);
		break;
	case CFG_DATA_TYPE_UCHAR:
		if(sscanf(str,"%d,\n",&it->m_nowValue.valueI) == 0) return false;
		* (unsigned char *)it->m_pVariable = it->m_nowValue.valueI;
		((_XSlider *)it->m_pCtrl)->setNowValue(it->m_nowValue.valueI);
		break;
	case CFG_DATA_TYPE_FLOAT:
		if(sscanf(str,"%f,\n",&it->m_nowValue.valueF) == 0) return false;
		* (float *)it->m_pVariable = it->m_nowValue.valueF;
		((_XSlider *)it->m_pCtrl)->setNowValue(it->m_nowValue.valueF);
		break;
	case CFG_DATA_TYPE_XBOOL:
		if(sscanf(str,"%d,\n",&temp) == 0) return false;			
		if(temp == 0) it->m_nowValue.valueB = XFalse;
		else it->m_nowValue.valueB = XTrue;
		* (_XBool *)it->m_pVariable = it->m_nowValue.valueB;
		((_XCheck *)it->m_pCtrl)->setState(it->m_nowValue.valueB);
		break;
	case CFG_DATA_TYPE_RADIOS:
		if(sscanf(str,"%d,\n",&it->m_nowValue.valueI) == 0) return false;			
		* (int *)it->m_pVariable = it->m_nowValue.valueI;
		((_XRadios *)it->m_pCtrl)->setChoosed(it->m_nowValue.valueI);
		break;
	case CFG_DATA_TYPE_CUSTOM:
		break;
	}
	return true;
}
bool _XConfigManager::loadEx(const char *filename)
{
	FILE *fp = NULL;
	if(filename == NULL)
	{
		if((fp = fopen(CFG_DEFAULT_FILENAME,"r")) == NULL) return false;
	}else
	{
		if((fp = fopen(filename,"r")) == NULL) return false;
	}
	char lineData[1024];
	int len = 0;
	bool ret = true;
	for(int i = 0;i < m_pItems.size();++ i)
	{
		if(!m_pItems[i]->m_isActive) continue;	//被动的值不会读取
		if(feof(fp)) break;
		//读取一行数据
		if(m_pItems[i]->m_type == CFG_DATA_TYPE_CUSTOM)
		{//这里会存在问题
			int fpos = ftell(fp);
			if(m_pItems[i]->m_customIt->load(fp))
			{//如果读取成功，直接继续
				continue;
			}else
			{
				ret = false;
				fseek(fp,fpos,SEEK_SET);	//恢复原有的位置
			}
		}
		while(1)
		{
			if(feof(fp) || fgets(lineData,1024,fp) == NULL)
			{//数据读取失败或者文件结束
				fclose(fp);
				return ret;
			}
			//寻找配置项的名字
			len = getCharPosition(lineData,':') + 1;
			if(len < 0) 
			{//数据错误，继续读取下面的数据
				ret = false;
				continue;
			}else break;
		}
		lineData[len - 1] = '\0';
		if(m_pItems[i]->m_type == CFG_DATA_TYPE_CUSTOM || strcmp(lineData,m_pItems[i]->m_name.c_str()) != 0)
		{//名称比较失败
			_XConfigItem *it = getItemByName(lineData,i + 1);
			if(it == NULL) 
			{
				-- i;
				continue;	//找不到这个配置项
			}
			if(getItemValueFromStr(it,lineData + len))			//下面开始读取配置项的值
				if(it->m_changeProc != NULL) it->m_changeProc();	//fix bug:读取配置参数时，没有调用其附带的函数
			ret = false;	//标记数据不正确
			--i;
			continue;
		}else
		{
			if(getItemValueFromStr(m_pItems[i],lineData + len))
			{//数据读取成功
				if(m_pItems[i]->m_changeProc != NULL) m_pItems[i]->m_changeProc();	//fix bug:读取配置参数时，没有调用其附带的函数
			}else
			{
				--i;
				continue;
			}
		}
	}
	fclose(fp);
	return ret;
}
void _XConfigManager::setDefault()	//恢复默认值
{
	for(int i = 0;i < m_pItems.size();++ i)
	{
		if(!m_pItems[i]->m_isActive) continue;	//被动的值不能回复默认设置
		switch(m_pItems[i]->m_type)
		{
			case CFG_DATA_TYPE_INT:
				m_pItems[i]->m_nowValue.valueI = m_pItems[i]->m_defaultValue.valueI;
				* (int *)m_pItems[i]->m_pVariable = m_pItems[i]->m_nowValue.valueI;
				((_XSlider *)m_pItems[i]->m_pCtrl)->setNowValue(m_pItems[i]->m_nowValue.valueI);
				break;
			case CFG_DATA_TYPE_CHAR:
				m_pItems[i]->m_nowValue.valueI = m_pItems[i]->m_defaultValue.valueI;
				* (char *)m_pItems[i]->m_pVariable = m_pItems[i]->m_nowValue.valueI;
				((_XSlider *)m_pItems[i]->m_pCtrl)->setNowValue(m_pItems[i]->m_nowValue.valueI);
				break;
			case CFG_DATA_TYPE_UCHAR:
				m_pItems[i]->m_nowValue.valueI = m_pItems[i]->m_defaultValue.valueI;
				* (unsigned char *)m_pItems[i]->m_pVariable = m_pItems[i]->m_nowValue.valueI;
				((_XSlider *)m_pItems[i]->m_pCtrl)->setNowValue(m_pItems[i]->m_nowValue.valueI);
				break;
			case CFG_DATA_TYPE_FLOAT:
				m_pItems[i]->m_nowValue.valueF = m_pItems[i]->m_defaultValue.valueF;
				* (float *)m_pItems[i]->m_pVariable = m_pItems[i]->m_nowValue.valueF;
				((_XSlider *)m_pItems[i]->m_pCtrl)->setNowValue(m_pItems[i]->m_nowValue.valueF);
				break;
			case CFG_DATA_TYPE_XBOOL:
				m_pItems[i]->m_nowValue.valueB = m_pItems[i]->m_defaultValue.valueB;
				* (_XBool *)m_pItems[i]->m_pVariable = m_pItems[i]->m_nowValue.valueB;
				((_XCheck *)m_pItems[i]->m_pCtrl)->setState(m_pItems[i]->m_nowValue.valueB);
				break;
			case CFG_DATA_TYPE_RADIOS:
				m_pItems[i]->m_nowValue.valueI = m_pItems[i]->m_defaultValue.valueI;
				* (int *)m_pItems[i]->m_pVariable = m_pItems[i]->m_nowValue.valueI;
				((_XRadios *)m_pItems[i]->m_pCtrl)->setChoosed(m_pItems[i]->m_nowValue.valueI);
				break;
			case CFG_DATA_TYPE_CUSTOM:
				m_pItems[i]->m_customIt->defValue();
				break;
		}
		if(m_pItems[i]->m_changeProc != NULL) m_pItems[i]->m_changeProc();	//fix bug:读取配置参数时，没有调用其附带的函数
	}
}
bool _XConfigManager::init(_XConfigMode mode)
{
	if(m_isInited) return false;	//防止重复初始化
	m_configMode = mode;

	m_minuteBtn.initWithoutTex("+",XEE::systemFont,1.0f,_XRect(0.0f,0.0f,CFG_MNG_H_SLD,CFG_MNG_H_SLD),
		_XVector2(CFG_MNG_H_SLD * 0.5f,CFG_MNG_H_SLD * 0.5f));
	m_minuteBtn.setCallbackFun(NULL,NULL,NULL,callbackProc,NULL,this);
	m_minuteBtn.disVisiable();	//隐藏
	m_minuteTimer = 0;
	m_minuteIndex = -1;
	m_minuteNeedRemove = true;
	m_minuteSld.initWithoutTex(_XRect(0.0f,0.0f,m_width,CFG_MNG_H_SLD),
		100.0f,0.0f,SLIDER_TYPE_HORIZONTAL,_XVector2(0.0,0.0f));
	m_minuteSld.setSize(m_size);
	m_minuteSld.setCallbackFun(NULL,NULL,NULL,NULL,NULL,callbackProc,callbackProc,this);
	m_minuteSld.disVisiable();	//隐藏

	m_saveBtn.initWithoutTex("保存",XEE::systemFont,1.0f,_XRect(0.0f,0.0f,64.0f,32.0f),_XVector2(32.0f,16.0f));
	m_saveBtn.setCallbackFun(NULL,NULL,NULL,callbackProc,NULL,this);
	m_saveBtn.setPosition(m_position);
	m_loadBtn.initWithoutTex("读取",XEE::systemFont,1.0f,_XRect(0.0f,0.0f,64.0f,32.0f),_XVector2(32.0f,16.0f));
	m_loadBtn.setCallbackFun(NULL,NULL,NULL,callbackProc,NULL,this);
	m_loadBtn.setPosition(m_position + _XVector2(64.0f + CFG_MANAGER_W_SPACE,0.0f));
	m_defaultBtn.initWithoutTex("默认",XEE::systemFont,1.0f,_XRect(0.0f,0.0f,64.0f,32.0f),_XVector2(32.0f,16.0f));
	m_defaultBtn.setCallbackFun(NULL,NULL,NULL,callbackProc,NULL,this);
	m_defaultBtn.setPosition(m_position + _XVector2(128.0f + 2.0f * CFG_MANAGER_W_SPACE,0.0f));
	m_netUpdateBtn.initWithoutTex("同步",XEE::systemFont,1.0f,_XRect(0.0f,0.0f,64.0f,32.0f),_XVector2(32.0f,16.0f));
	m_netUpdateBtn.setCallbackFun(NULL,NULL,NULL,callbackProc,NULL,this);
	m_netUpdateBtn.setPosition(m_position + _XVector2(192.0f + 3.0f * CFG_MANAGER_W_SPACE,0.0f));
	m_netInjectBtn.initWithoutTex("注入",XEE::systemFont,1.0f,_XRect(0.0f,0.0f,64.0f,32.0f),_XVector2(32.0f,16.0f));
	m_netInjectBtn.setCallbackFun(NULL,NULL,NULL,callbackProc,NULL,this);
	m_netInjectBtn.setPosition(m_position + _XVector2(256.0f + 4.0f * CFG_MANAGER_W_SPACE,0.0f));
	if(m_configMode != CFG_MODE_CLIENT)
	{
		m_netUpdateBtn.disVisiable();
		m_netInjectBtn.disVisiable();
	}
	m_nowInsertPos = m_position + _XVector2(0.0f,32.0f + CFG_MANAGER_H_SPACE);
	m_maxHeight = XEE::windowData.h;	//默认使用全屏高度

	addGroup(CFG_DEFAULT_GROUPNAME);
	//_XConfigGroup *defGroup = createMem<_XConfigGroup>();
	//if(defGroup == NULL) return false;
	//defGroup->m_isEnable = true;
	//defGroup->m_position = m_position + _XVector2(0.0f,32.0f + CFG_MANAGER_H_SPACE);
	//defGroup->m_group.init(defGroup->m_position,_XRect(0.0f,0.0f,m_width,32.0f),defGroup->m_name.c_str(),XEE::systemFont,1.0f);
	//m_pGroups.push_back(defGroup);
	if(m_configMode == CFG_MODE_SERVER)
	{//建立服务器
		string tempStr = XEE::windowData.windowTitle;
		tempStr = tempStr + "_Config";
		m_netServer.setProjectStr(tempStr.c_str());
		if(!m_netServer.createServer(6868,XTrue)) return false;
	}else
	if(m_configMode == CFG_MODE_CLIENT)
	{//建立客户端
		string tempStr = XEE::windowData.windowTitle;
		tempStr = tempStr + "_Config";
		m_netClient.setProjectStr(tempStr.c_str());
		if(!m_netClient.createClient()) return false;
	}

	m_isInited = true;
	return true;
}
void _XConfigManager::draw()
{
	for(int i = 0;i < m_pItems.size();++ i)
	{
		if(m_pItems[i]->m_type == CFG_DATA_TYPE_CUSTOM) m_pItems[i]->m_customIt->draw(); else
		if(m_pItems[i]->m_type == CFG_DATA_TYPE_XSPRITE) ((_XObjectBasic *)m_pItems[i]->m_pVariable)->draw();
	}
}
void _XConfigManager::relayoutGroup(int index,bool flag)
{
	m_pGroups[index]->m_size = m_size;
	m_pGroups[index]->m_position = m_nowInsertPos; 
	m_pGroups[index]->m_maxHeight = m_maxHeight;
	m_pGroups[index]->m_group.setPosition(m_pGroups[index]->m_position);
	m_pGroups[index]->m_group.setSize(m_size);
	if(flag) m_pGroups[index]->relayout();
}
void _XConfigManager::relayout()	//重新自动布局
{
	m_saveBtn.setPosition(m_position);
	m_saveBtn.setSize(m_size);
	m_loadBtn.setPosition(m_position + _XVector2((64.0f + CFG_MANAGER_W_SPACE) * m_size.x,0.0f));
	m_loadBtn.setSize(m_size);
	m_defaultBtn.setPosition(m_position + _XVector2((128.0f + 2.0f * CFG_MANAGER_W_SPACE) * m_size.x,0.0f));
	m_defaultBtn.setSize(m_size);
	//更新组件的位置
	m_nowInsertPos = m_position + _XVector2(0.0f,(32.0f + CFG_MANAGER_H_SPACE) * m_size.x);
	m_isNewRow = true;
	for(int i = 0;i < m_pGroups.size();++ i)
	{
		if(m_pGroups[i]->m_group.getState() == STATE_NORMAL)
		{//检查当前列是否可以容下，如果容不下则处理
			if(m_maxHeight + m_position.y - m_nowInsertPos.y < m_pGroups[i]->calculateMaxSize().y)
			{//如果容不下
				if(m_isNewRow)
				{//如果已经是新的一列了则不处理
					relayoutGroup(i);
					moveDown(m_pGroups[i]->m_maxSize.x,m_pGroups[i]->m_maxSize.y);
				}else
				{//否则的话使用新的一列
					useANewRow();
					relayoutGroup(i);
					moveDown(m_pGroups[i]->m_maxSize.x,m_pGroups[i]->m_maxSize.y);
				}
			}else
			{
				relayoutGroup(i);
				moveDown(m_pGroups[i]->m_maxSize.x,m_pGroups[i]->m_maxSize.y);
			}
		}else
		{
			relayoutGroup(i,false);
			_XVector2 tempSize = m_pGroups[i]->m_group.getBox(0) - m_pGroups[i]->m_group.getBox(2);
			if(tempSize.x < 0.0f) tempSize.x = -tempSize.x;
			if(tempSize.y < 0.0f) tempSize.y = -tempSize.y;
			moveDown(tempSize.x * 1.0f / m_size.x,tempSize.y * 1.0f / m_size.x);
		}
	}
	return ;
}
void _XConfigManager::setVisible()
{//设置显示
	m_isVisble = true;
	m_saveBtn.setVisiable();
	m_loadBtn.setVisiable();
	m_defaultBtn.setVisiable();
	if(m_configMode == CFG_MODE_CLIENT)
	{
		m_netUpdateBtn.setVisiable();
		m_netInjectBtn.setVisiable();
	}
	for(int i = 0;i < m_pGroups.size();++ i)
	{
		m_pGroups[i]->m_group.setVisiable();
	}
}
void _XConfigManager::disVisible()
{//设置不显示
	m_isVisble = false;
	m_saveBtn.disVisiable();
	m_loadBtn.disVisiable();
	m_defaultBtn.disVisiable();
	if(m_configMode == CFG_MODE_CLIENT)
	{
		m_netUpdateBtn.disVisiable();
		m_netInjectBtn.disVisiable();
	}
	for(int i = 0;i < m_pGroups.size();++ i)
	{
		m_pGroups[i]->m_group.disVisiable();
	}
}
void _XConfigManager::sendSynchToServer()
{
	if(m_configMode == CFG_MODE_CLIENT)
	{
		_XNetData *tempSendData = createMem<_XNetData>();
		tempSendData->dataLen = 2;
		tempSendData->data = createArrayMem<unsigned char>(tempSendData->dataLen);
		tempSendData->isEnable = true;
		tempSendData->type = DATA_TYPE_CONFIG_INFO;
		m_netClient.sendData(tempSendData);
	}
}
//从字符串中读取Item相关的数据，返回是否需要建立
bool _XConfigManager::createAItemFromStr(const unsigned char * data,int &offset,unsigned char *groupName,
										 std::vector<_XConfigItem *> *itemsList,std::vector<int> *itemsIDListD)
{
	if(data == NULL) return false;
	int itemID,tempItemID = 0,len;
	_XConfigDataType itemType;
	offset = 0; 
	unsigned char tempNameI[4096];	//默认最大长度为4096，这里会存在问题

	memcpy(&itemID,data + offset,sizeof(int));
	offset += sizeof(int);
	memcpy(&itemType,data + offset,sizeof(itemType));
	offset += sizeof(itemType);
	_XConfigValue rangeMin,rangeMax,defValue,nowValue;
	switch(itemType)
	{
	case CFG_DATA_TYPE_INT:
	case CFG_DATA_TYPE_CHAR:
	case CFG_DATA_TYPE_UCHAR:
	case CFG_DATA_TYPE_RADIOS:
	case CFG_DATA_TYPE_FLOAT:
	case CFG_DATA_TYPE_XBOOL:
		memcpy(&rangeMin,data + offset,sizeof(rangeMin));
		offset += sizeof(rangeMin);
		memcpy(&rangeMax,data + offset,sizeof(rangeMax));
		offset += sizeof(rangeMax);
		memcpy(&defValue,data + offset,sizeof(defValue));
		offset += sizeof(defValue);
		memcpy(&nowValue.valueI,data + offset,sizeof(nowValue));
		offset += sizeof(nowValue);
		break;
	case CFG_DATA_TYPE_CUSTOM:	//工作进行中
		{
			int tempItemsSum;
			memcpy(&tempItemsSum,data + offset,sizeof(tempItemsSum));
			offset += sizeof(tempItemsSum);
			for(int i = 0;i < tempItemsSum;++ i)
			{//这里进行嵌套
				createAItemFromStr(data + offset,len,groupName,itemsList,itemsIDListD);
				offset += len;
			}
		}
		return false;
		break;
	case CFG_DATA_TYPE_XSPRITE:	//网络不支持
	case CFG_DATA_TYPE_NULL:
		return false;	//跳过下面的操作
		break;
	default:
		return false;
		break;
	}
	memcpy(&len,data + offset,sizeof(int));
	offset += sizeof(int);
	memcpy(tempNameI,data + offset,len);
	tempNameI[len] = '\0';
	offset += len;
	switch(itemType)
	{
	case CFG_DATA_TYPE_INT:
		{
			int *temp = createMem<int>();
			*temp = nowValue.valueI;
			tempItemID = addAItem<int>(temp,itemType,(char *)tempNameI,rangeMax.valueI,rangeMin.valueI,defValue.valueI,NULL,(char *)groupName);
		}
		break;
	case CFG_DATA_TYPE_CHAR:
		{
			char *temp = createMem<char>();
			*temp = nowValue.valueI;
			tempItemID = addAItem<char>(temp,itemType,(char *)tempNameI,rangeMax.valueI,rangeMin.valueI,defValue.valueI,NULL,(char *)groupName);
		}
		break;
	case CFG_DATA_TYPE_UCHAR:
		{
			unsigned char *temp = createMem<unsigned char>();
			*temp = nowValue.valueI;
			tempItemID = addAItem<unsigned char>(temp,itemType,(char *)tempNameI,rangeMax.valueI,rangeMin.valueI,defValue.valueI,NULL,(char *)groupName);
		}
		break;
	case CFG_DATA_TYPE_RADIOS:
		{
			int *temp = createMem<int>();
			*temp = nowValue.valueI;
			tempItemID = addAItem<int>(temp,itemType,(char *)tempNameI,rangeMax.valueI,rangeMin.valueI,defValue.valueI,NULL,(char *)groupName);
		}
		break;
	case CFG_DATA_TYPE_FLOAT:
		{
			float *temp = createMem<float>();
			*temp = nowValue.valueF;
			tempItemID = addAItem<float>(temp,itemType,(char *)tempNameI,rangeMax.valueF,rangeMin.valueF,defValue.valueF,NULL,(char *)groupName);
		}
		break;
	case CFG_DATA_TYPE_XBOOL:
		{
			_XBool *temp = createMem<_XBool>();
			*temp = nowValue.valueB;
			tempItemID = addAItem<_XBool>(temp,itemType,(char *)tempNameI,rangeMax.valueB,rangeMin.valueB,defValue.valueB,NULL,(char *)groupName);
		}
		break;
	case CFG_DATA_TYPE_CUSTOM:	//工作进行中
	case CFG_DATA_TYPE_XSPRITE:	//网络不支持
	case CFG_DATA_TYPE_NULL:	//网络不支持
		break;
	default:
		break;
	}
	_XConfigItem * it = getItemByID(tempItemID);
	if(it != NULL)
	{
		if(itemsList != NULL) itemsList->push_back(it);
		if(itemsIDListD != NULL) itemsIDListD->push_back(itemID);
	}
	return true;
}
void _XConfigManager::updateInfo(unsigned char *data)
{
	if(data == NULL) return;
	int groupSum = 0;
	int itemsSum = 0;
	int offset = 0;
	int len = 0;
	memcpy(&groupSum,data + offset,sizeof(int));
	offset += sizeof(int);
	unsigned char tempName[4096];	//默认最大长度为4096，这里会存在问题
	std::vector<_XConfigItem *> itemsList;
	std::vector<int> itemsIDListD;
	for(int i = 0;i < groupSum;++ i)
	{
		memcpy(&len,data + offset,sizeof(int));
		offset += sizeof(int);
		memcpy(tempName,data + offset,len);
		tempName[len] = '\0';
		offset += len;
		if(i == 0) renameGroup(CFG_DEFAULT_GROUPNAME,(char *)tempName);
		else addGroup((char *)tempName);
		memcpy(&itemsSum,data + offset,sizeof(int));
		offset += sizeof(int);
		for(int j = 0;j < itemsSum;++ j)
		{
			createAItemFromStr(data + offset,len,tempName,&itemsList,&itemsIDListD);
			offset += len;
		}
	}
	//统一改变控件的ID
	for(int i = 0;i < itemsList.size();++ i)
	{
		itemsList[i]->setID(itemsIDListD[i]);
	}
}
void _XConfigManager::sendItemValue(const _XConfigItem * it)
{
	if(m_configMode == CFG_MODE_SERVER)
	{
		if(it->m_type != CFG_DATA_TYPE_CUSTOM && it->m_type != CFG_DATA_TYPE_XSPRITE)
		{
			_XNetData *tempSendData = createMem<_XNetData>();
			unsigned char *temp = NULL;
			int size = 0;
			int offset = 0;
			int tempID = it->getID();
			temp = spliceData(temp,size,offset,(unsigned char *)&tempID,sizeof(int));
			temp = spliceData(temp,size,offset,(unsigned char *)&it->m_nowValue,sizeof(it->m_nowValue));
			tempSendData->data = temp;
			tempSendData->dataLen = offset;
			tempSendData->isEnable = true;
			tempSendData->type = DATA_TYPE_CONFIG_ITEM;
			m_netServer.sendData(tempSendData);
			//printf("配置项数据发送!\n");
		}
	}else
	if(m_configMode == CFG_MODE_CLIENT)
	{
		if(it->m_type != CFG_DATA_TYPE_CUSTOM && it->m_type != CFG_DATA_TYPE_XSPRITE)
		{
			_XNetData *tempSendData = createMem<_XNetData>();
			unsigned char *temp = NULL;
			int size = 0;
			int offset = 0;
			int tempID = it->getID();
			temp = spliceData(temp,size,offset,(unsigned char *)&tempID,sizeof(int));
			temp = spliceData(temp,size,offset,(unsigned char *)&it->m_nowValue,sizeof(it->m_nowValue));
			tempSendData->data = temp;
			tempSendData->dataLen = offset;
			tempSendData->isEnable = true;
			tempSendData->type = DATA_TYPE_CONFIG_ITEM;
			m_netClient.sendData(tempSendData);
			//printf("配置项数据发送!\n");
		}
	}
}
void _XConfigManager::updateItemFromCFG(_XConfigItem * it)
{
	if(it == NULL) return;
	switch(it->m_type)
	{
	case CFG_DATA_TYPE_INT:
		if(* (int *)it->m_pVariable != it->m_nowValue.valueI)
		{//下面更新数据
			* (int *)it->m_pVariable = it->m_nowValue.valueI;
			((_XSlider *)it->m_pCtrl)->setNowValue(it->m_nowValue.valueI);
		}
		break;
	case CFG_DATA_TYPE_CHAR:
		if(* (char *)it->m_pVariable != it->m_nowValue.valueI)
		{//下面更新数据
			* (char *)it->m_pVariable = it->m_nowValue.valueI;
			((_XSlider *)it->m_pCtrl)->setNowValue(it->m_nowValue.valueI);
		}
		break;
	case CFG_DATA_TYPE_UCHAR:
		if(* (unsigned char *)it->m_pVariable != it->m_nowValue.valueI)
		{//下面更新数据
			* (unsigned char *)it->m_pVariable = it->m_nowValue.valueI;
			((_XSlider *)it->m_pCtrl)->setNowValue(it->m_nowValue.valueI);
		}
		break;
	case CFG_DATA_TYPE_FLOAT:
		if(* (float *)it->m_pVariable != it->m_nowValue.valueF)
		{//下面更新数据
			* (float *)it->m_pVariable = it->m_nowValue.valueF;
			((_XSlider *)it->m_pCtrl)->setNowValue(it->m_nowValue.valueF);
		}
		break;
	case CFG_DATA_TYPE_XBOOL:
		if(((* (_XBool *)it->m_pVariable) && !it->m_nowValue.valueB)
			|| (!(* (_XBool *)it->m_pVariable) && it->m_nowValue.valueB))
		{//下面更新数据
			* (_XBool *)it->m_pVariable = it->m_nowValue.valueB;
			((_XCheck *)it->m_pCtrl)->setState(it->m_nowValue.valueB);
		}
		break;
	case CFG_DATA_TYPE_RADIOS:
		if(* (int *)it->m_pVariable != it->m_nowValue.valueI)
		{//下面更新数据
			* (int *)it->m_pVariable = it->m_nowValue.valueI;
			((_XRadios *)it->m_pCtrl)->setChoosed(it->m_nowValue.valueI);
		}
		break;
	case CFG_DATA_TYPE_CUSTOM:	//占位
		it->m_customIt->update();
		break;
	}
	if(it->m_changeProc != NULL) it->m_changeProc();	
}
void _XConfigManager::updateItemToCFG(_XConfigItem * it)
{
	if(it == NULL) return;
	switch(it->m_type)
	{
	case CFG_DATA_TYPE_INT:
		if(* (int *)it->m_pVariable != it->m_nowValue.valueI)
		{//下面更新数据
			it->m_nowValue.valueI = * (int *)it->m_pVariable;
			((_XSlider *)it->m_pCtrl)->setNowValue(it->m_nowValue.valueI);
		}
		break;
	case CFG_DATA_TYPE_CHAR:
		if(* (char *)it->m_pVariable != it->m_nowValue.valueI)
		{//下面更新数据
			it->m_nowValue.valueI = * (char *)it->m_pVariable;
			((_XSlider *)it->m_pCtrl)->setNowValue(it->m_nowValue.valueI);
		}
		break;
	case CFG_DATA_TYPE_UCHAR:
		if(* (unsigned char *)it->m_pVariable != it->m_nowValue.valueI)
		{//下面更新数据
			it->m_nowValue.valueI = * (unsigned char *)it->m_pVariable;
			((_XSlider *)it->m_pCtrl)->setNowValue(it->m_nowValue.valueI);
		}
		break;
	case CFG_DATA_TYPE_FLOAT:
		if(* (float *)it->m_pVariable != it->m_nowValue.valueF)
		{//下面更新数据
			it->m_nowValue.valueF = * (float *)it->m_pVariable;
			((_XSlider *)it->m_pCtrl)->setNowValue(it->m_nowValue.valueF);
		}
		break;
	case CFG_DATA_TYPE_XBOOL:
		if(((* (_XBool *)it->m_pVariable) && !it->m_nowValue.valueB)
			|| (!(* (_XBool *)it->m_pVariable) && it->m_nowValue.valueB))
		{//下面更新数据
			it->m_nowValue.valueB = * (_XBool *)it->m_pVariable;
			((_XCheck *)it->m_pCtrl)->setState(it->m_nowValue.valueB);
		}
		break;
	case CFG_DATA_TYPE_RADIOS:
		if(* (int *)it->m_pVariable != it->m_nowValue.valueI)
		{//下面更新数据
			it->m_nowValue.valueI = * (int *)it->m_pVariable;
			((_XRadios *)it->m_pCtrl)->setChoosed(it->m_nowValue.valueI);
		}
		break;
	case CFG_DATA_TYPE_CUSTOM:	//占位
		it->m_customIt->update();
		break;
	}
}
void _XConfigManager::setItemValue(int ID,const _XConfigValue value)
{
	_XConfigItem * it = getItemByID(ID);
	if(it == NULL) return;
	it->m_nowValue = value;
	updateItemFromCFG(it);
}
void _XConfigManager::sendCFGInfo()
{
	//GroupSum
	//GroupNameLen|GroupName|
		//ItemsSum
		//ItemID|ItemType|ItemRangeMin|ItemRangeMax|ItemDefault|ItemNowValue|ItemNameLen|ItemName|
	_XNetData *tempSendData = createMem<_XNetData>();
	tempSendData->data = getConfigInfo(tempSendData->dataLen);
	tempSendData->isEnable = true;
	tempSendData->type = DATA_TYPE_CONFIG_INFO;
	m_netServer.sendData(tempSendData);
	//printf("服务器发送同步信息!\n");
}
void _XConfigManager::sendInject()	//向服务器发送注入信息
{
	if(m_configMode == CFG_MODE_CLIENT)
	{//读取配置文件，将配置文件发送到服务端，服务端读取配置文件并载入，然后同步到客户端
		FILE *fp = NULL;
		if((fp = fopen(CFG_DEFAULT_FILENAME,"r")) == NULL) return;
		_XNetData *tempSendData = createMem<_XNetData>();
		tempSendData->dataLen = getFileLen(fp);
		tempSendData->data = createArrayMem<unsigned char>(tempSendData->dataLen);
		fread(tempSendData->data,1,tempSendData->dataLen,fp);
		fclose(fp);
		tempSendData->isEnable = true;
		tempSendData->type = DATA_TYPE_CONFIG_INJECT;
		m_netClient.sendData(tempSendData);
		//printf("发送数据注入命令!\n");
	}
}
void _XConfigManager::updateNet()
{
	if(m_configMode == CFG_MODE_SERVER)
	{//服务器
		_XNetData *tempData = m_netServer.getData();
		if(tempData != NULL)
		{//下面处理数据
			switch(tempData->type)
			{
			case DATA_TYPE_CONFIG_INFO:		//客户端请求发送配置的结构信息
				sendCFGInfo();
				break;
			case DATA_TYPE_CONFIG_ITEM:		//客户端发送某个配置项的值
				{//取出相应的值
					int itemID;
					_XConfigValue nowValue;
					memcpy(&itemID,tempData->data,sizeof(itemID));
					memcpy(&nowValue,tempData->data + sizeof(int),sizeof(nowValue));
					if(itemID > CFG_MAX_ITEMS_SUM)
					{//用户自定义的配置类的ID
						itemID = itemID / CFG_MAX_ITEMS_SUM;
						_XConfigItem *it = getItemByID(itemID);
						if(it != NULL && it->m_customIt != NULL)
						{
							it->m_customIt->setValueFromStr(tempData->data);	//生效相应的值
						}
					}else
					{
						setItemValue(itemID,nowValue);	//生效相应的值
					}
					//printf("收到客户端配置项数据变更!\n");
				}
				break;
			case DATA_TYPE_CONFIG_ITEMS:	//客户端发送所有配置项的值，不会发送
				break;
			case DATA_TYPE_CONFIG_INJECT:	//从客户端收到数据注入操作请求
				{
					FILE *fp = NULL;
					if((fp = fopen(CFG_INJECT_FILENAME,"w")) == NULL) return;
					fwrite(tempData->data,1,tempData->dataLen,fp);
					fclose(fp);
					load(CFG_INJECT_FILENAME);
					sendCFGInfo();
				}
				break;
			case DATA_TYPE_CONFIG_OPERATE:	//功能操作(完成)
				{
					_XConfigNetOperate op;
					memcpy(&op,tempData->data,sizeof(op));
					switch(op)
					{
					case CFG_NET_OP_SAVE:
						save();
						break;
					case CFG_NET_OP_LOAD:
						load();
						sendCFGInfo();
						break;
					case CFG_NET_OP_DEFAULT:
						setDefault();
						sendCFGInfo();
						break;
					}
				}
				break;
			}
			XDELETE(tempData);	//处理完成之后再删除数据
		}
		for(int i = 0;i < m_pItems.size();++ i)
		{
			if(m_pItems[i]->m_type == CFG_DATA_TYPE_CUSTOM && m_pItems[i]->m_customIt->needSendStr())
			{//如果自定义配置类需要发送数据，这里发送数据
				_XNetData *tempSendData = createMem<_XNetData>();
				tempSendData->data = m_pItems[i]->m_customIt->sendStr(tempSendData->dataLen,m_pItems[i]->getID());
				tempSendData->isEnable = true;
				tempSendData->type = DATA_TYPE_CONFIG_ITEMS;
				m_netServer.sendData(tempSendData);
				//printf("向客户端发送数据变更!\n");
			}
		}
	}else
	if(m_configMode == CFG_MODE_CLIENT)
	{//客户端
		_XNetData *tempData = m_netClient.getData();
		if(tempData != NULL)
		{//下面处理数据
			switch(tempData->type)
			{
			case DATA_TYPE_CONFIG_INFO:		//服务器发送的配置结构
				clear();					//清除当前所有的群组
				updateInfo(tempData->data);				//根据同步的信息建立新的群组关系
				//printf("客户端收到同步信息!\n");
				break;
			case DATA_TYPE_CONFIG_ITEM:		//服务器发送的某个配置项的值
				{//取出相应的值
					int itemID;
					_XConfigValue nowValue;
					memcpy(&itemID,tempData->data,sizeof(itemID));
					memcpy(&nowValue,tempData->data + sizeof(int),sizeof(nowValue));
					setItemValue(itemID,nowValue);	//生效相应的值
					//printf("收到服务器端配置项数据变更!\n");
				}
				break;
			case DATA_TYPE_CONFIG_ITEMS:	//服务器发送的多个配置项的值
				//服务器的自定义组件才会发送这个值(尚未完成)
				{
					int sum = 0;
					int offset = 0;
					memcpy(&sum,tempData->data + offset,sizeof(sum));
					offset += sizeof(sum);
					int itemID;
					_XConfigValue nowValue;
					for(int i = 0;i < sum;++ i)
					{
						memcpy(&itemID,tempData->data + offset,sizeof(itemID));
						offset += sizeof(itemID);
						memcpy(&nowValue,tempData->data + offset,sizeof(nowValue));
						offset += sizeof(nowValue);
						setItemValue(itemID,nowValue);	//生效相应的值
					}
					//printf("接收到服务器的数据变更!\n");
				}
				break;
			case DATA_TYPE_CONFIG_OPERATE:	//服务器不会发送这个数据
				break;
			}
			XDELETE(tempData);	//处理完成之后再删除数据
		}
	}
}
void _XConfigManager::updateMinuteSld()
{
	int i = m_minuteIndex;
	float nowValue;
	float range;
	switch(m_pItems[i]->m_type)
	{
	case CFG_DATA_TYPE_INT: 
	case CFG_DATA_TYPE_CHAR: 
	case CFG_DATA_TYPE_UCHAR: 
		nowValue = m_pItems[i]->m_nowValue.valueI;
		range = (m_pItems[i]->m_rangeMax.valueI - m_pItems[i]->m_rangeMin.valueI) * 0.05f;
		if(nowValue + range <= m_pItems[i]->m_rangeMax.valueI 
			&& nowValue - range >= m_pItems[i]->m_rangeMin.valueI)
		{//范围内
			m_minuteSld.setRange(nowValue + range,nowValue - range);
		}else
		if(nowValue + range > m_pItems[i]->m_rangeMax.valueI)
		{//上边界越界
			m_minuteSld.setRange(m_pItems[i]->m_rangeMax.valueI,m_pItems[i]->m_rangeMax.valueI - range * 2.0f);
		}else
		if(nowValue + range < m_pItems[i]->m_rangeMin.valueI)
		{//下边界越界
			m_minuteSld.setRange(m_pItems[i]->m_rangeMin.valueI + range * 2.0f,m_pItems[i]->m_rangeMin.valueI);
		}
		m_minuteSld.setNowValue(nowValue);
		break;
	case CFG_DATA_TYPE_FLOAT: 
		nowValue = m_pItems[i]->m_nowValue.valueF;
		range = (m_pItems[i]->m_rangeMax.valueF - m_pItems[i]->m_rangeMin.valueF) * 0.05f;
		if(nowValue + range <= m_pItems[i]->m_rangeMax.valueF 
			&& nowValue - range >= m_pItems[i]->m_rangeMin.valueF)
		{//范围内
			m_minuteSld.setRange(nowValue + range,nowValue - range);
		}else
		if(nowValue + range > m_pItems[i]->m_rangeMax.valueF)
		{//上边界越界
			m_minuteSld.setRange(m_pItems[i]->m_rangeMax.valueF,m_pItems[i]->m_rangeMax.valueF - range * 2.0f);
		}else
		if(nowValue - range < m_pItems[i]->m_rangeMin.valueF)
		{//下边界越界
			m_minuteSld.setRange(m_pItems[i]->m_rangeMin.valueF + range * 2.0f,m_pItems[i]->m_rangeMin.valueF);
	//		printf("%f,%f\n",m_pItems[i]->m_rangeMin.valueF + range * 2.0f,m_pItems[i]->m_rangeMin.valueF);
		}
	//	printf("%f\n",nowValue);
		m_minuteSld.setNowValue(nowValue);
		break;
	}
}
void _XConfigManager::update()	//更新状态
{
	updateNet();
	if(m_minuteTimer > 1000)
	{//按钮自动消失
		m_minuteTimer = 0;
		m_minuteBtn.disVisiable();
		m_minuteSld.disVisiable();
	}
	if(m_minuteBtn.getVisiable() && m_minuteNeedRemove) m_minuteTimer += XEE::frameTime;
	bool tempFlag = true;
	for(int i = 0;i < m_pItems.size();++ i)
	{
	//	if(!m_pItems[i]->m_isActive)
		{//这里更新被动控件的状态
			updateItemToCFG(m_pItems[i]);
		}
		//这里检查是否需要显示微调按钮
		if(tempFlag && (m_pItems[i]->m_type == CFG_DATA_TYPE_INT || m_pItems[i]->m_type == CFG_DATA_TYPE_CHAR
			|| m_pItems[i]->m_type == CFG_DATA_TYPE_UCHAR || m_pItems[i]->m_type == CFG_DATA_TYPE_FLOAT))
		{
			if(((_XSlider *)(m_pItems[i]->m_pCtrl))->getVisiable() && 
				((_XSlider *)(m_pItems[i]->m_pCtrl))->isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
			{//如果鼠标在范围内，则显示
				if(m_minuteIndex != i || !m_minuteBtn.getVisiable())// || !m_minuteSld.getVisiable())
				{//需要更新数据
				//	printf("Yes\n");
					tempFlag = false;
					m_minuteIndex = i;
					m_minuteTimer = 0;
					m_minuteNeedRemove = false;
					_XVector2 tempPos = ((_XSlider *)(m_pItems[i]->m_pCtrl))->getBox(1);
					m_minuteBtn.setPosition(tempPos.x + 5.0f * m_size.x,tempPos.y);
					m_minuteBtn.setVisiable();
					tempPos = ((_XSlider *)(m_pItems[i]->m_pCtrl))->getBox(3);
					m_minuteSld.setPosition(tempPos.x,tempPos.y + 5.0f * m_size.y);
					updateMinuteSld();	//更新微调的范围
				}else
				{//不需要更新数据
				//	printf("No\n");
					tempFlag = false;
					m_minuteTimer = 0;
					m_minuteNeedRemove = false;
				}
			}
		}
	}
	if(tempFlag)
	{
		if((m_minuteBtn.getVisiable() && m_minuteBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
			|| (m_minuteBtn.getVisiable() && m_minuteSld.isInRect(XEE::mousePosition.x,XEE::mousePosition.y)))
		{
			m_minuteNeedRemove = false;	//不需要移除微调
			m_minuteTimer = 0;
		}else
		{
			m_minuteNeedRemove = true;	//需要移除微调
		}
	}
}
void _XConfigManager::setItemActive(bool isActive,void * p)	//设置某个配置项是否为主动形式(默认全部为主动形式)
{
	if(p == NULL) return;
	_XConfigItem *it = getItemByVariable(p);
	if(it != NULL)
	{
		it->m_isActive = isActive;
		if(isActive)
		{
			switch(it->m_type)
			{
			case CFG_DATA_TYPE_INT:
			case CFG_DATA_TYPE_CHAR:
			case CFG_DATA_TYPE_UCHAR:
			case CFG_DATA_TYPE_FLOAT:
				((_XSlider *)it->m_pCtrl)->enable();
				break;
			case CFG_DATA_TYPE_XBOOL:
				((_XCheck *)it->m_pCtrl)->enable();
				break;
			case CFG_DATA_TYPE_RADIOS:
				((_XRadios *)it->m_pCtrl)->enable();
				break;
			case CFG_DATA_TYPE_CUSTOM:	//占位
				it->m_customIt->enable();
				break;
			}
		}else
		{
			switch(it->m_type)
			{
			case CFG_DATA_TYPE_INT:
			case CFG_DATA_TYPE_CHAR:
			case CFG_DATA_TYPE_UCHAR:
			case CFG_DATA_TYPE_FLOAT:
				((_XSlider *)it->m_pCtrl)->disable();
				break;
			case CFG_DATA_TYPE_XBOOL:
				((_XCheck *)it->m_pCtrl)->disable();
				break;
			case CFG_DATA_TYPE_RADIOS:
				((_XRadios *)it->m_pCtrl)->disable();
				break;
			case CFG_DATA_TYPE_CUSTOM:	//占位
				it->m_customIt->disable();
				break;
			}
		}
	}
}
bool _XConfigManager::clear()
{
	if(m_configMode == CFG_MODE_CLIENT)
	{//这里需要删除变量
		for(int i = 0;i < m_pItems.size();++ i)
		{
			XDELETE(m_pItems[i]->m_pVariable);
			XDELETE(m_pItems[i]);
		}
		m_pItems.clear();
	}else
	{
		for(int i = 0;i < m_pItems.size();++ i)
		{
			XDELETE(m_pItems[i]);
		}
		m_pItems.clear();
	}
	for(int i = 0;i < m_pGroups.size();++ i)
	{
		XDELETE(m_pGroups[i]);
	}
	m_pGroups.clear();
	addGroup(CFG_DEFAULT_GROUPNAME);	//建立一个默认组
	return true;
}
bool _XConfigManager::decreaseAItem(void *p)	//减少一个配置项
{
	if(p == NULL) return false;
	for(int i = 0;i < m_pItems.size();++ i)
	{
		if(m_pItems[i]->m_pVariable == p) 
		{
			_XConfigItem *it = m_pItems[i];
			for(int j = i;j < m_pItems.size() - 1;++ i)
			{
				m_pItems[j] = m_pItems[j + 1]; 
			}
			m_pItems.pop_back();
			//更新群组的信息
			bool flag = false;
			for(int j = 0;j < m_pGroups.size();++ j)
			{
				for(int k = 0;k < m_pGroups[j]->m_items.size();++ k)
				{
					if(m_pGroups[j]->m_items[k] == it)
					{
						for(int l = k;l < m_pGroups[j]->m_items.size() - 1;++ l)
						{
							m_pGroups[j]->m_items[l] = m_pGroups[j]->m_items[l + 1]; 
						}
						m_pGroups[j]->m_items.pop_back();
						flag = true;
						break;
					}
				}
				if(flag) break;
			}
			XDELETE(it);
			return true;
		}
	}
	return false;
}
void _XConfigGroup::moveDownPretreatment(int pixelW,int pixelH)
{
	float h = m_nowInsertPos.y + (pixelH + CFG_MANAGER_H_SPACE) * m_size.y;
	if(h > m_position.y + m_maxHeight && !m_isNewRow)
	{
		useANewRow();
		m_isMaxH = true;
	}
}
void _XConfigGroup::moveDown(int pixelW,int pixelH)	//向下移动插入点
{
	m_nowInsertPos.y += (pixelH + CFG_MANAGER_H_SPACE) * m_size.y;
	if(m_isNewRow)
	{
		m_maxRowWidth = pixelW;
		m_maxSize.x += pixelW + CFG_MANAGER_W_SPACE;	//这个数据有问题
	}else
	if(pixelW > m_maxRowWidth)
	{
		m_maxSize.x += pixelW - m_maxRowWidth;	//这个数据有问题
		m_maxRowWidth = pixelW;
	}
	if(!m_isMaxH) m_maxSize.y += pixelH + CFG_MANAGER_H_SPACE;
	else
	if((m_nowInsertPos.y - m_position.y) / m_size.y > m_maxSize.y)
	{//如果后面列的高度大于之前列的高度，则去最大值
		m_maxSize.y = (m_nowInsertPos.y - m_position.y) / m_size.y;
	}
	m_isNewRow = false;
	//这里会扩展一行，但是如果这是最后一行的话，则不需要扩展一列
	if(m_nowInsertPos.y >= m_position.y + m_maxHeight)
	{//这里需要换行
		useANewRow();
		m_isMaxH = true;
	}
}
void _XConfigGroup::relayout()
{//遍历组件，并设置组件的位置
	m_nowInsertPos = m_position + _XVector2(10.0f,32.0f) * m_size;
	m_maxSize.set(10.0f + m_width + CFG_MANAGER_W_SPACE,32.0f);
	m_maxRowWidth = m_width;
	m_isNewRow = false;
	m_isMaxH = false;
	for(int i = 0;i < m_items.size();++ i)
	{
		_XConfigItem *it = m_items[i];
		if(it == NULL) continue;
		switch(it->m_type)
		{
		case CFG_DATA_TYPE_INT:
			moveDownPretreatment(m_width,CFG_MNG_H_FONT + CFG_MNG_H_SLD);
			((_XSlider *)it->m_pCtrl)->setPosition(m_nowInsertPos);
			moveDown(m_width,CFG_MNG_H_FONT + CFG_MNG_H_SLD);
			break;
		case CFG_DATA_TYPE_CHAR:
			moveDownPretreatment(m_width,CFG_MNG_H_FONT + CFG_MNG_H_SLD);
			((_XSlider *)it->m_pCtrl)->setPosition(m_nowInsertPos);
			moveDown(m_width,CFG_MNG_H_FONT + CFG_MNG_H_SLD);
			break;
		case CFG_DATA_TYPE_UCHAR:
			moveDownPretreatment(m_width,CFG_MNG_H_FONT + CFG_MNG_H_SLD);
			((_XSlider *)it->m_pCtrl)->setPosition(m_nowInsertPos);
			moveDown(m_width,CFG_MNG_H_FONT + CFG_MNG_H_SLD);
			break;
		case CFG_DATA_TYPE_FLOAT:
			moveDownPretreatment(m_width,CFG_MNG_H_FONT + CFG_MNG_H_SLD);
			((_XSlider *)it->m_pCtrl)->setPosition(m_nowInsertPos);
			moveDown(m_width,CFG_MNG_H_FONT + CFG_MNG_H_SLD);
			break;
		case CFG_DATA_TYPE_XBOOL:
			moveDownPretreatment(m_width,CFG_MNG_H_FONT);
			it->m_pCtrl->setPosition(m_nowInsertPos);
			moveDown(m_width,CFG_MNG_H_FONT);
			break;
		case CFG_DATA_TYPE_RADIOS:
			moveDownPretreatment((((_XRadios *)it->m_pCtrl)->getBox(2) - ((_XRadios *)it->m_pCtrl)->getBox(0)) / m_size);
			((_XRadios *)it->m_pCtrl)->setPosition(m_nowInsertPos);
			moveDown((((_XRadios *)it->m_pCtrl)->getBox(2) - ((_XRadios *)it->m_pCtrl)->getBox(0)) / m_size);// / m_size.y
			break;
		case CFG_DATA_TYPE_CUSTOM:
			moveDownPretreatment((it->m_customIt->getBox(2) - it->m_customIt->getBox(0)) / m_size);
			it->m_customIt->setPosition(m_nowInsertPos);
			moveDown((it->m_customIt->getBox(2) - it->m_customIt->getBox(0)) / m_size);// / m_size.y
			break;
		case CFG_DATA_TYPE_XSPRITE:	
			moveDownPretreatment((((_XObjectBasic *)it->m_pVariable)->getBox(2) - ((_XObjectBasic *)it->m_pVariable)->getBox(0)) / m_size);
			((_XObjectBasic *)it->m_pVariable)->setPosition(m_nowInsertPos);
			moveDown((((_XObjectBasic *)it->m_pVariable)->getBox(2) - ((_XObjectBasic *)it->m_pVariable)->getBox(0)) / m_size);
			break;
		}
	}
	//重新改编组件的范围
	m_group.resetSize(m_maxSize);
}
_XVector2 _XConfigGroup::calculateMaxSize()
{
	_XVector2 ret;
	if(m_group.getState() == STATE_MINISIZE)
	{
		ret = m_group.getBox(2) - m_group.getBox(0);
	//	ret.x /= m_size.x;	//还原成像素
	//	ret.y /= m_size.y;
	}else
	{
		float maxY = 0.0f;
		bool isMaxH = false;
		ret.set(10.0f + m_width + CFG_MANAGER_W_SPACE,32.0f);
		for(int i = 0;i < m_items.size();++ i)
		{
			_XConfigItem *it = m_items[i];
			if(it == NULL) continue;
			switch(it->m_type)
			{
			case CFG_DATA_TYPE_INT:
			case CFG_DATA_TYPE_CHAR:
			case CFG_DATA_TYPE_UCHAR:
			case CFG_DATA_TYPE_FLOAT:
				ret.y += (CFG_MNG_H_FONT + CFG_MNG_H_SLD + CFG_MANAGER_H_SPACE) * m_size.y;
				if(ret.y > m_maxHeight)
				{
					ret.x += (m_width + CFG_MANAGER_W_SPACE) * m_size.x;
					isMaxH = true;
					if(maxY < ret.y) maxY = ret.y;
					ret.y = 32.0f * m_size.x;
				}
				break;
			case CFG_DATA_TYPE_XBOOL:
				ret.y += (CFG_MNG_H_FONT + CFG_MANAGER_H_SPACE) * m_size.y;
				if(ret.y > m_maxHeight)
				{
					ret.x += (m_width + CFG_MANAGER_W_SPACE) * m_size.x;
					isMaxH = true;
					if(maxY < ret.y) maxY = ret.y;
					ret.y = 32.0f * m_size.x;
				}
				break;
			case CFG_DATA_TYPE_RADIOS:
				ret.y += CFG_MANAGER_H_SPACE * m_size.y + (((_XRadios *)it->m_pCtrl)->getBox(2) - ((_XRadios *)it->m_pCtrl)->getBox(0)).y;
				if(ret.y > m_maxHeight)
				{
					ret.x += (m_width + CFG_MANAGER_W_SPACE) * m_size.x;
					isMaxH = true;
					if(maxY < ret.y) maxY = ret.y;
					ret.y = 32.0f * m_size.x;
				}
				break;
			case CFG_DATA_TYPE_CUSTOM:	//占位
				ret.y += CFG_MANAGER_H_SPACE * m_size.y + (it->m_customIt->getBox(2) - it->m_customIt->getBox(0)).y;
				if(ret.y > m_maxHeight)
				{
					ret.x += (m_width + CFG_MANAGER_W_SPACE) * m_size.x;
					isMaxH = true;
					if(maxY < ret.y) maxY = ret.y;
					ret.y = 32.0f * m_size.x;
				}
				break;
			case CFG_DATA_TYPE_XSPRITE:
				ret.y += CFG_MANAGER_H_SPACE * m_size.y + (((_XObjectBasic *)it->m_pVariable)->getBox(2) - ((_XObjectBasic *)it->m_pVariable)->getBox(0)).y;
				if(ret.y > m_maxHeight)
				{
					ret.x += (m_width + CFG_MANAGER_W_SPACE) * m_size.x;
					isMaxH = true;
					if(maxY < ret.y) maxY = ret.y;
					ret.y = 32.0f * m_size.x;
				}
				break;
			}
		}
		if(isMaxH) ret.y = maxY;
	}
	return ret;
}
bool _XConfigManager::addGroup(const char * name)
{
	if(name == NULL) return false;
	if(isGroupExist(name)) return false;	//组不能重名
	_XConfigGroup *defGroup = createMem<_XConfigGroup>();
	if(defGroup == NULL) return false;
	defGroup->m_isEnable = true;
	defGroup->m_position = m_position + _XVector2(0.0f,64.0f + CFG_MANAGER_H_SPACE);
	defGroup->m_name = name;
	defGroup->m_group.init(defGroup->m_position,_XRect(0.0f,0.0f,m_width + CFG_MANAGER_W_SPACE * 0.5f,32.0f),
		defGroup->m_name.c_str(),XEE::systemFont,1.0f);
	defGroup->m_group.setCallbackFun(callbackProc,this);
	defGroup->m_group.setState(STATE_MINISIZE);	//初始状态为最小化
	defGroup->m_width = m_width;
	defGroup->m_maxHeight = m_maxHeight;
	m_pGroups.push_back(defGroup);
	relayout();
	return true;
}
int _XConfigManager::addSpecialItem(void * it,_XConfigDataType type,const char * groupName)
{
	if(it == NULL || type <= CFG_DATA_TYPE_CUSTOM) return -1;
	if(isSpecialItemExist(it)) return -1;	//如果已经存在则不能重复加入
	_XConfigItem *pItem = createMem<_XConfigItem>();
	if(pItem == NULL) return -1;
	pItem->m_isEnable = true;
	pItem->m_pVariable = it;
	pItem->m_customIt = NULL;
	((_XObjectBasic *)pItem->m_pVariable)->setSize(((_XObjectBasic *)pItem->m_pVariable)->getSize() * m_size);
	////针对序列帧不能设置左上角对齐的问题，这里使用一种不适合的方式设置序列帧的尺寸不随之变化(注意：这是权宜之计，如果重构的话需要取消这个设计)
	//if(((_XObjectBasic *)pItem->m_pVariable)->getObjectType() == OBJ_FRAMEEX)
	//{
	//	((_XObjectBasic *)pItem->m_pVariable)->setSize(1.0f,1.0f);
	//}else
	//{
	//	((_XObjectBasic *)pItem->m_pVariable)->setSize(((_XObjectBasic *)pItem->m_pVariable)->getSize() * m_size);
	//}
//	pItem->m_size = m_size;
	pItem->m_type = type;
	pItem->m_isActive = false;
	m_pItems.push_back(pItem);
	if(groupName == NULL)
	{//使用默认组
		_XConfigGroup *gp = m_pGroups[0];
		gp->m_items.push_back(pItem);
		gp->m_group.pushChild((_XObjectBasic *)pItem->m_pVariable);
		if(gp->m_group.getState() == STATE_MINISIZE) ((_XObjectBasic *)pItem->m_pVariable)->disVisiable();

		relayout();
	}else
	{
		_XConfigGroup *gp = getGroup(groupName);
		if(gp != NULL)
		{
			gp->m_items.push_back(pItem);
			gp->m_group.pushChild((_XObjectBasic *)pItem->m_pVariable);
			if(gp->m_group.getState() == STATE_MINISIZE) ((_XObjectBasic *)pItem->m_pVariable)->disVisiable();
			relayout();
		}
	}
	return pItem->getID();
}
int _XConfigManager::addCustomItem(_XCFGItemBasic *it,const char * groupName)
{
	if(it == NULL || isCustomItemExist(it)) return -1;
	_XConfigItem *pItem = createMem<_XConfigItem>();
	if(pItem == NULL) return -1;
	pItem->m_isEnable = true;
	pItem->m_pVariable = NULL;
	pItem->m_customIt = it;
	pItem->m_customIt->setSize(m_size);
//	pItem->m_size = m_size;
	pItem->m_type = CFG_DATA_TYPE_CUSTOM;
	m_pItems.push_back(pItem);
	if(groupName == NULL)
	{//使用默认组
		_XConfigGroup *gp = m_pGroups[0];
		gp->m_items.push_back(pItem);
		gp->m_group.pushChild(pItem->m_customIt);
		if(gp->m_group.getState() == STATE_MINISIZE) pItem->m_customIt->disVisiable();

		relayout();
	}else
	{
		_XConfigGroup *gp = getGroup(groupName);
		if(gp != NULL)
		{
			gp->m_items.push_back(pItem);
			gp->m_group.pushChild(pItem->m_customIt);
			if(gp->m_group.getState() == STATE_MINISIZE) pItem->m_customIt->disVisiable();
			relayout();
		}
	}
	return pItem->getID();
}
bool _XConfigManager::isItemExist(void * p)	//检查配置项是否已经存在
{
	if(p == NULL) return false;
	for(int i = 0;i < m_pItems.size();++ i)
	{
		if(m_pItems[i]->m_pVariable == p) return true;
	}
	return false;
}
bool _XConfigManager::isCustomItemExist(_XCFGItemBasic *it)
{
	if(it == NULL) return false;
	for(int i = 0;i < m_pItems.size();++ i)
	{
		if(m_pItems[i]->m_type == CFG_DATA_TYPE_CUSTOM && 
			m_pItems[i]->m_customIt == it) return true;
	}
	return false;
}
_XConfigItem *_XConfigManager::getItemByID(int ID)			//通过ID获取配置项的指针
{
	if(ID < 0) return NULL;
	for(int i = 0;i < m_pItems.size();++ i)
	{
		if(m_pItems[i]->getID() == ID) return m_pItems[i];
	}
	return NULL;
}
_XConfigItem *_XConfigManager::getItemByVariable(void *p)	//通过变量指针获得配置项的指针
{
	for(int i = 0;i < m_pItems.size();++ i)
	{
		if(m_pItems[i]->m_pVariable == p) return m_pItems[i];
	}
	return NULL;
}
_XConfigItem *_XConfigManager::getItemByName(const char *name,int start)
{
	if(name == NULL) return NULL;
	for(int i = start;i < m_pItems.size();++ i)
	{
		if(strcmp(m_pItems[i]->m_name.c_str(),name) == 0) return m_pItems[i];
	}
	return NULL;
}
bool _XConfigManager::isGroupExist(const char * name)	//判断组件是否存在
{
	if(name == NULL) return false;
	for(int i = 0;i < m_pGroups.size();++ i)
	{
		if(strcmp(m_pGroups[i]->m_name.c_str(),name) == 0) return true;
	}
	return false;
}
_XConfigGroup *_XConfigManager::getGroup(const char * name)
{
	if(name == NULL) return NULL;
	for(int i = 0;i < m_pGroups.size();++ i)
	{
		if(strcmp(m_pGroups[i]->m_name.c_str(),name) == 0) return m_pGroups[i];
	}
	return NULL;
}
void _XConfigManager::release()
{//资源释放
	if(!m_isInited) return;
	if(m_configMode == CFG_MODE_CLIENT)
	{//这里需要删除变量
		for(int i = 0;i < m_pItems.size();++ i)
		{
			XDELETE(m_pItems[i]->m_pVariable);
			XDELETE(m_pItems[i]);
		}
		m_pItems.clear();
	}else
	{
		for(int i = 0;i < m_pItems.size();++ i)
		{
			XDELETE(m_pItems[i]);
		}
		m_pItems.clear();
	}
	for(int i = 0;i < m_pGroups.size();++ i)
	{
		XDELETE(m_pGroups[i]);
	}
	m_pGroups.clear();
	m_isInited = false;
}
void _XConfigManager::setOperateToServer(_XConfigNetOperate op)
{
	if(!m_isInited) return;
	if(m_configMode != CFG_MODE_CLIENT) return;
	_XNetData *tempData = createMem<_XNetData>();
	tempData->isEnable = true;
	tempData->type = DATA_TYPE_CONFIG_OPERATE;
	tempData->dataLen = sizeof(op);
	tempData->data = createArrayMem<unsigned char>(tempData->dataLen);
	memcpy(tempData->data,&op,tempData->dataLen);
	m_netClient.sendData(tempData);
}
unsigned char *spliceData(unsigned char * baseBuff,int &baseLen,int &offset,const unsigned char * addBuff,int len)
{
	if(addBuff == NULL) return baseBuff;
	if(offset + len > baseLen || baseBuff == NULL)
	{//数据超过长度
		if(baseLen == 0) baseLen = 1;
		while(1)
		{
			baseLen = baseLen << 1;
			if(offset + len <= baseLen) break;
		}
		unsigned char *temp = createArrayMem<unsigned char>(baseLen);
		if(baseBuff != NULL) memcpy(temp,baseBuff,offset);
		XDELETE_ARRAY(baseBuff);
		memcpy(temp + offset,addBuff,len);
		offset += len;
		return temp;
	}else
	{//数据没有超过长度
		memcpy(baseBuff + offset,addBuff,len);
		offset += len;
		return baseBuff;
	}
}
unsigned char *_XConfigManager::getConfigInfo(int &slen)
{
	unsigned char *temp = NULL;
	int size = 0;
	int offset = 0;
	_XConfigDataType nullType = CFG_DATA_TYPE_NULL;
	int len = m_pGroups.size();
	temp = spliceData(temp,size,offset,(unsigned char *)&len,sizeof(int));
	for(int i = 0;i < m_pGroups.size();++ i)
	{
		len = m_pGroups[i]->m_name.size();
		temp = spliceData(temp,size,offset,(unsigned char *)&len,sizeof(int));
		temp = spliceData(temp,size,offset,(unsigned char *)m_pGroups[i]->m_name.c_str(),len);
		len = m_pGroups[i]->m_items.size();
		temp = spliceData(temp,size,offset,(unsigned char *)&len,sizeof(int));
		for(int j = 0;j < m_pGroups[i]->m_items.size();++ j)
		{
			_XConfigItem *it = m_pGroups[i]->m_items[j];
			switch(it->m_type)
			{
			case CFG_DATA_TYPE_INT:
			case CFG_DATA_TYPE_CHAR:
			case CFG_DATA_TYPE_UCHAR:
			case CFG_DATA_TYPE_RADIOS:
			case CFG_DATA_TYPE_FLOAT:
			case CFG_DATA_TYPE_XBOOL:
				len = it->getID();
				temp = spliceData(temp,size,offset,(unsigned char *)&len,sizeof(int));	//ID
				if(!it->m_isActive)
				{
					temp = spliceData(temp,size,offset,(unsigned char *)&nullType,sizeof(it->m_type));	//TYPE
					break;
				}
				temp = spliceData(temp,size,offset,(unsigned char *)&it->m_type,sizeof(it->m_type));	//TYPE
				temp = spliceData(temp,size,offset,(unsigned char *)&it->m_rangeMin,sizeof(it->m_rangeMin));	//min
				temp = spliceData(temp,size,offset,(unsigned char *)&it->m_rangeMax,sizeof(it->m_rangeMax));	//max
				temp = spliceData(temp,size,offset,(unsigned char *)&it->m_defaultValue,sizeof(it->m_defaultValue));	//defualt
				temp = spliceData(temp,size,offset,(unsigned char *)&it->m_nowValue,sizeof(it->m_nowValue));	//nowValue
				len = it->m_name.size();
				temp = spliceData(temp,size,offset,(unsigned char *)&len,sizeof(int));	//name len
				temp = spliceData(temp,size,offset,(unsigned char *)it->m_name.c_str(),len);	//name len
				break;
			case CFG_DATA_TYPE_CUSTOM:	//工作进行中
				//扩展
				{
					len = it->getID();
					temp = spliceData(temp,size,offset,(unsigned char *)&len,sizeof(int));					//ID
					temp = spliceData(temp,size,offset,(unsigned char *)&it->m_type,sizeof(it->m_type));	//TYPE
					unsigned char *tempStr = it->m_customIt->getInfo(len,it->getID());
					temp = spliceData(temp,size,offset,tempStr,len);
					XDELETE_ARRAY(tempStr);
				}
				break;
			case CFG_DATA_TYPE_XSPRITE:	//网络不支持
			case CFG_DATA_TYPE_NULL:	//网络不支持
				len = it->getID();
				temp = spliceData(temp,size,offset,(unsigned char *)&len,sizeof(int));	//ID
				temp = spliceData(temp,size,offset,(unsigned char *)&nullType,sizeof(it->m_type));	//TYPE
				break;
			}
		}
	}
	slen = offset;
	return temp;
}