//inline º¯Êý
INLINE void XFontUnicode::setRotateBasePoint(float x,float y)
{
	m_rotateBasicPoint.set(x,y);
	setPosition(m_setPosition);
}
INLINE int XFontUnicode::getStringLengthPix()
{
	return getMaxPixelWidth();
	//int stringLength = 0;
	//for(int i =0;i < m_stringMaxLength;++ i)
	//{
	//	if(m_string[i] == '\0') break;	//×Ö·û´®½áÊø
	//	if((unsigned char)m_string[i] < 128)
	//	{//ASCII
	//		stringLength += m_size.x * m_showSize.x * 0.5f;
	//		stringLength += m_distance;
	//	}else
	//	{//·ÇASCII
	//		if(m_isPassword == 0)
	//		{//·ÇÃÜÂëÄ£Ê½
	//			stringLength += m_size.x * m_showSize.x;
	//			stringLength += m_distance;
	//		}else
	//		{//ÃÜÂëÄ£Ê½
	//			stringLength += m_size.x * m_showSize.x * 0.5f;
	//			stringLength += m_distance;
	//		}
	//		++ i;
	//	}
	//}
	//return stringLength;
}
INLINE int XFontUnicode::getTextIndex(const char p[UNICODE_BYTES_WIDTH])	//ÕÒµ½Ö¸¶¨µÄ×Ö·ûËùÔÚµÄÎ»ÖÃ
{
	//·½°¸1:×îÔ­Ê¼µÄ·½·¨£¬²»ÐèÒªÊÂÏÈÅÅÐò
	//for(int i = 0;i < m_libFontSum;++ i)
	//{
	//	if(m_fontPageText[UNICODE_BYTES_WIDTH * i] == p[0]
	//		&& m_fontPageText[UNICODE_BYTES_WIDTH * i + 1] == p[1])
	//		return i;
	//}
	//·½°¸2:Õë¶ÔANSIÂë½øÐÐÓÅ»¯
	//unsigned short tmpS;
	//memcpy(&tmpS,p,2);
	//unsigned short *tmpStr = (unsigned short *)m_fontPageText;
	//for(int i = 0;i < m_libFontSum;++ i)
	//{
	//	if(tmpStr[i] == tmpS) return i;
	//}
	//·½°¸3£º2·Ö·¨²éÕÒ£¬ÐèÒª¶ÔÔ­Ê¼Êý¾Ý½øÐÐÅÅÐò
	if(m_libFontSum <= 0) return -1;
	int indexMin = 0;
	int indexMax = m_libFontSum - 1;
	unsigned short tmpS;
	memcpy(&tmpS,p,2);
	unsigned short *tmpStr = (unsigned short *)m_fontPageText;
	if(tmpStr[indexMin] == tmpS) return indexMin;
	if(tmpStr[indexMax] == tmpS) return indexMax;
	int indexTmp;
	while(true)
	{
		indexTmp = (indexMin + indexMax) >> 1;
		if(indexMin == indexMax || indexTmp == indexMin || indexTmp == indexMax) break;
		if(tmpStr[indexTmp] == tmpS) return indexTmp;
		if(tmpStr[indexTmp] > tmpS) indexMax = indexTmp;
		else indexMin = indexTmp;
	}
	//·½°¸4£¬Ö±½ÓÓ³Éä£¬²»¹ý±È½ÏÏûºÄÄÚ´æ(ÉÐÎ´½øÐÐ)
	return -1; 
}
#if WITH_FULL_ALL_CHINESE == 2
#ifndef TEXT_FONT_UNICODE_MODE
#define TEXT_FONT_UNICODE_MODE " !\"#$%&\'()*+,-./0123456789:;<=>\?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
#endif
#endif
//ÏÂÃæÊÇ³£ÓÃºº×Ö
#if WITH_FULL_ALL_CHINESE == 0
#ifndef TEXT_FONT_UNICODE_MODE
#define TEXT_FONT_UNICODE_MODE " !\"#$%&\'()*+,-./0123456789:;<=>\?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~£¡\
°¡±¡²¡³¡´¡µ¡¶¡·¡¸¡¹¡º¡»¡¼¡½¡¾¡¿¡À¡Á¡Â¡Ã¡Ä¡Å¡Æ¡Ç¡È¡É¡Ê¡Ë¡Ì¡Í¡Î¡Ï¡Ð¡Ñ¡Ò¡Ó¡Ô¡Õ¡Ö¡×¡¡¢°¢±¢²¢³¢´¢µ¢¶¢·¢¸¢¹¢º¢»¢¼¢½¢¾¢¿¢À¢Á¢Â¢Ã¢\
Ä¢Å¢Æ¢Ç¢È¢É¢Ê¢Ë¢Ì¢Í¢Î¢Ï¢Ð¢Ñ¢Ò¢Ó¢Ô¢Õ¢Ö¢×¢¡£°£±£²£³£´£µ£¶£·£¸£¹£º£»£¼£½£¾£¿£À£Á£Â£Ã£Ä£Å£Æ£Ç£È£É£Ê£Ë£Ì£Í£Î£Ï£Ð£Ñ£Ò£Ó£Ô£Õ£Ö£×£\
¡¤£¤°¤±¤²¤³¤´¤µ¤¶¤·¤¸¤¹¤º¤»¤¼¤½¤¾¤¿¤À¤Á¤Â¤Ã¤Ä¤Å¤Æ¤Ç¤È¤É¤Ê¤Ë¤Ì¤Í¤Î¤Ï¤Ð¤Ñ¤Ò¤Ó¤Ô¤Õ¤Ö¤×¤°¥±¥²¥³¥´¥µ¥¶¥·¥¸¥¹¥º¥»¥¼¥½¥¾¥¿¥À¥Á¥Â¥\
Ã¥Ä¥Å¥Æ¥Ç¥È¥É¥Ê¥Ë¥Ì¥Í¥Î¥Ï¥Ð¥Ñ¥Ò¥Ó¥Ô¥Õ¥Ö¥×¥°¦±¦²¦³¦´¦µ¦¶¦·¦¸¦¹¦º¦»¦¼¦½¦¾¦¿¦À¦Á¦Â¦Ã¦Ä¦Å¦Æ¦Ç¦È¦É¦Ê¦Ë¦Ì¦Í¦Î¦Ï¦Ð¦Ñ¦Ò¦Ó¦Ô¦Õ¦Ö¦×¦\
°§±§²§³§´§µ§¶§·§¸§¹§º§»§¼§½§¾§¿§À§Á§Â§Ã§Ä§Å§Æ§Ç§È§É§Ê§Ë§Ì§Í§Î§Ï§Ð§Ñ§Ò§Ó§Ô§Õ§Ö§×§£¨°¨±¨²¨³¨´¨µ¨¶¨·¨¸¨¹¨º¨»¨¼¨½¨¾¨¿¨À¨Á¨Â¨Ã¨\
Ä¨Å¨Æ¨Ç¨È¨É¨Ê¨Ë¨Ì¨Í¨Î¨Ï¨Ð¨Ñ¨Ò¨Ó¨Ô¨Õ¨Ö¨×¨£©°©±©²©³©´©µ©¶©·©¸©¹©º©»©¼©½©¾©¿©À©Á©Â©Ã©Ä©Å©Æ©Ç©È©É©Ê©Ë©Ì©Í©Î©Ï©Ð©Ñ©Ò©Ó©Ô©Õ©Ö©×©\
¡ª°ª±ª²ª³ª´ªµª¶ª·ª¸ª¹ªºª»ª¼ª½ª¾ª¿ªÀªÁªÂªÃªÄªÅªÆªÇªÈªÉªÊªËªÌªÍªÎªÏªÐªÑªÒªÓªÔªÕªÖª×ª°«±«²«³«´«µ«¶«·«¸«¹«º«»«¼«½«¾«¿«À«Á«Â«Ã«\
Ä«Å«Æ«Ç«È«É«Ê«Ë«Ì«Í«Î«Ï«Ð«Ñ«Ò«Ó«Ô«Õ«Ö«×«£¬°¬±¬²¬³¬´¬µ¬¶¬·¬¸¬¹¬º¬»¬¼¬½¬¾¬¿¬À¬Á¬Â¬Ã¬Ä¬Å¬Æ¬Ç¬È¬É¬Ê¬Ë¬Ì¬Í¬Î¬Ï¬Ð¬Ñ¬Ò¬Ó¬Ô¬Õ¬Ö¬×¬\
¡­°­±­²­³­´­µ­¶­·­¸­¹­º­»­¼­½­¾­¿­À­Á­Â­Ã­Ä­Å­Æ­Ç­È­É­Ê­Ë­Ì­Í­Î­Ï­Ð­Ñ­Ò­Ó­Ô­Õ­Ö­×­¡®°®±®²®³®´®µ®¶®·®¸®¹®º®»®¼®½®¾®¿®À®Á®Â®\
Ã®Ä®Å®Æ®Ç®È®É®Ê®Ë®Ì®Í®Î®Ï®Ð®Ñ®Ò®Ó®Ô®Õ®Ö®×®¡¯°¯±¯²¯³¯´¯µ¯¶¯·¯¸¯¹¯º¯»¯¼¯½¯¾¯¿¯À¯Á¯Â¯Ã¯Ä¯Å¯Æ¯Ç¯È¯É¯Ê¯Ë¯Ì¯Í¯Î¯Ï¯Ð¯Ñ¯Ò¯Ó¯Ô¯Õ¯Ö¯\
×¯¡°°°±°²°³°´°µ°¶°·°¸°¹°º°»°¼°½°¾°¿°À°Á°Â°Ã°Ä°Å°Æ°Ç°È°É°Ê°Ë°Ì°Í°Î°Ï°Ð°Ñ°Ò°Ó°Ô°Õ°Ö°×°¡±°±±±²±³±´±µ±¶±·±¸±¹±º±»±¼±½±¾±¿±À±Á±\
Â±Ã±Ä±Å±Æ±Ç±È±É±Ê±Ë±Ì±Í±Î±Ï±Ð±Ñ±Ò±Ó±Ô±Õ±Ö±×±°²±²²²³²´²µ²¶²·²¸²¹²º²»²¼²½²¾²¿²À²Á²Â²Ã²Ä²Å²Æ²Ç²È²É²Ê²Ë²Ì²Í²Î²Ï²Ð²Ñ²Ò²Ó²Ô²Õ²Ö²\
×²°³±³²³³³´³µ³¶³·³¸³¹³º³»³¼³½³¾³¿³À³Á³Â³Ã³Ä³Å³Æ³Ç³È³É³Ê³Ë³Ì³Í³Î³Ï³Ð³Ñ³Ò³Ó³Ô³Õ³Ö³×³°´±´²´³´´´µ´¶´·´¸´¹´º´»´¼´½´¾´¿´À´Á´Â´Ã´\
Ä´Å´Æ´Ç´È´É´Ê´Ë´Ì´Í´Î´Ï´Ð´Ñ´Ò´Ó´Ô´Õ´Ö´×´ß´°µ±µ²µ³µ´µµµ¶µ·µ¸µ¹µºµ»µ¼µ½µ¾µ¿µÀµÁµÂµÃµÄµÅµÆµÇµÈµÉµÊµËµÌµÍµÎµÏµÐµÑµÒµÓµÔµÕµÖµ×µ\
¡¶°¶±¶²¶³¶´¶µ¶¶¶·¶¸¶¹¶º¶»¶¼¶½¶¾¶¿¶À¶Á¶Â¶Ã¶Ä¶Å¶Æ¶Ç¶È¶É¶Ê¶Ë¶Ì¶Í¶Î¶Ï¶Ð¶Ñ¶Ò¶Ó¶Ô¶Õ¶Ö¶×¶ß¶¡·°·±·²·³·´·µ·¶···¸·¹·º·»·¼·½·¾·¿·À·Á·\
Â·Ã·Ä·Å·Æ·Ç·È·É·Ê·Ë·Ì·Í·Î·Ï·Ð·Ñ·Ò·Ó·Ô·Õ·Ö·×·°¸±¸²¸³¸´¸µ¸¶¸·¸¸¸¹¸º¸»¸¼¸½¸¾¸¿¸À¸Á¸Â¸Ã¸Ä¸Å¸Æ¸Ç¸È¸É¸Ê¸Ë¸Ì¸Í¸Î¸Ï¸Ð¸Ñ¸Ò¸Ó¸Ô¸Õ¸Ö¸\
×¸à¸°¹±¹²¹³¹´¹µ¹¶¹·¹¸¹¹¹º¹»¹¼¹½¹¾¹¿¹À¹Á¹Â¹Ã¹Ä¹Å¹Æ¹Ç¹È¹É¹Ê¹Ë¹Ì¹Í¹Î¹Ï¹Ð¹Ñ¹Ò¹Ó¹Ô¹Õ¹Ö¹×¹£º°º±º²º³º´ºµº¶º·º¸º¹ººº»º¼º½º¾º¿ºÀºÁº\
ÂºÃºÄºÅºÆºÇºÈºÉºÊºËºÌºÍºÎºÏºÐºÑºÒºÓºÔºÕºÖº×º£»°»±»²»³»´»µ»¶»·»¸»¹»º»»»¼»½»¾»¿»À»Á»Â»Ã»Ä»Å»Æ»Ç»È»É»Ê»Ë»Ì»Í»Î»Ï»Ð»Ñ»Ò»Ó»Ô»Õ»\
Ö»×»°¼±¼²¼³¼´¼µ¼¶¼·¼¸¼¹¼º¼»¼¼¼½¼¾¼¿¼À¼Á¼Â¼Ã¼Ä¼Å¼Æ¼Ç¼È¼É¼Ê¼Ë¼Ì¼Í¼Î¼Ï¼Ð¼Ñ¼Ò¼Ó¼Ô¼Õ¼Ö¼×¼°½±½²½³½´½µ½¶½·½¸½¹½º½»½¼½½½¾½¿½À½Á½Â½\
Ã½Ä½Å½Æ½Ç½È½É½Ê½Ë½Ì½Í½Î½Ï½Ð½Ñ½Ò½Ó½Ô½Õ½Ö½×½¡¾°¾±¾²¾³¾´¾µ¾¶¾·¾¸¾¹¾º¾»¾¼¾½¾¾¾¿¾À¾Á¾Â¾Ã¾Ä¾Å¾Æ¾Ç¾È¾É¾Ê¾Ë¾Ì¾Í¾Î¾Ï¾Ð¾Ñ¾Ò¾Ó¾Ô¾Õ¾Ö¾\
×¾¡¿£¿°¿±¿²¿³¿´¿µ¿¶¿·¿¸¿¹¿º¿»¿¼¿½¿¾¿¿¿À¿Á¿Â¿Ã¿Ä¿Å¿Æ¿Ç¿È¿É¿Ê¿Ë¿Ì¿Í¿Î¿Ï¿Ð¿Ñ¿Ò¿Ó¿Ô¿Õ¿Ö¿×¿°À±À²À³À´ÀµÀ¶À·À¸À¹ÀºÀ»À¼À½À¾À¿ÀÀÀÁÀ\
ÂÀÃÀÄÀÅÀÆÀÇÀÈÀÉÀÊÀËÀÌÀÍÀÎÀÏÀÐÀÑÀÒÀÓÀÔÀÕÀÖÀ×À°Á±Á²Á³Á´ÁµÁ¶Á·Á¸Á¹ÁºÁ»Á¼Á½Á¾Á¿ÁÀÁÁÁÂÁÃÁÄÁÅÁÆÁÇÁÈÁÉÁÊÁËÁÌÁÍÁÎÁÏÁÐÁÑÁÒÁÓÁÔÁÕÁÖÁ\
×Á°Â±Â²Â³Â´ÂµÂ¶Â·Â¸Â¹ÂºÂ»Â¼Â½Â¾Â¿ÂÀÂÁÂÂÂÃÂÄÂÅÂÆÂÇÂÈÂÉÂÊÂËÂÌÂÍÂÎÂÏÂÐÂÑÂÒÂÓÂÔÂÕÂÖÂ×Â°Ã±Ã²Ã³Ã´ÃµÃ¶Ã·Ã¸Ã¹ÃºÃ»Ã¼Ã½Ã¾Ã¿ÃÀÃÁÃÂÃÃÃ\
ÄÃÅÃÆÃÇÃÈÃÉÃÊÃËÃÌÃÍÃÎÃÏÃÐÃÑÃÒÃÓÃÔÃÕÃÖÃ×Ã°Ä±Ä²Ä³Ä´ÄµÄ¶Ä·Ä¸Ä¹ÄºÄ»Ä¼Ä½Ä¾Ä¿ÄÀÄÁÄÂÄÃÄÄÄÅÄÆÄÇÄÈÄÉÄÊÄËÄÌÄÍÄÎÄÏÄÐÄÑÄÒÄÓÄÔÄÕÄÖÄ×Ä°Å\
±Å²Å³Å´ÅµÅ¶Å·Å¸Å¹ÅºÅ»Å¼Å½Å¾Å¿ÅÀÅÁÅÂÅÃÅÄÅÅÅÆÅÇÅÈÅÉÅÊÅËÅÌÅÍÅÎÅÏÅÐÅÑÅÒÅÓÅÔÅÕÅÖÅ×ÅàÅ°Æ±Æ²Æ³Æ´ÆµÆ¶Æ·Æ¸Æ¹ÆºÆ»Æ¼Æ½Æ¾Æ¿ÆÀÆÁÆÂÆÃÆÄÆ\
ÅÆÆÆÇÆÈÆÉÆÊÆËÆÌÆÍÆÎÆÏÆÐÆÑÆÒÆÓÆÔÆÕÆÖÆ×Æ°Ç±Ç²Ç³Ç´ÇµÇ¶Ç·Ç¸Ç¹ÇºÇ»Ç¼Ç½Ç¾Ç¿ÇÀÇÁÇÂÇÃÇÄÇÅÇÆÇÇÇÈÇÉÇÊÇËÇÌÇÍÇÎÇÏÇÐÇÑÇÒÇÓÇÔÇÕÇÖÇ×Ç°È±È\
²È³È´ÈµÈ¶È·È¸È¹ÈºÈ»È¼È½È¾È¿ÈÀÈÁÈÂÈÃÈÄÈÅÈÆÈÇÈÈÈÉÈÊÈËÈÌÈÍÈÎÈÏÈÐÈÑÈÒÈÓÈÔÈÕÈÖÈ×È°É±É²É³É´ÉµÉ¶É·É¸É¹ÉºÉ»É¼É½É¾É¿ÉÀÉÁÉÂÉÃÉÄÉÅÉÆÉ\
ÇÉÈÉÉÉÊÉËÉÌÉÍÉÎÉÏÉÐÉÑÉÒÉÓÉÔÉÕÉÖÉ×ÉßÉ°Ê±Ê²Ê³Ê´ÊµÊ¶Ê·Ê¸Ê¹ÊºÊ»Ê¼Ê½Ê¾Ê¿ÊÀÊÁÊÂÊÃÊÄÊÅÊÆÊÇÊÈÊÉÊÊÊËÊÌÊÍÊÎÊÏÊÐÊÑÊÒÊÓÊÔÊÕÊÖÊ×Ê°Ë±Ë²Ë\
³Ë´ËµË¶Ë·Ë¸Ë¹ËºË»Ë¼Ë½Ë¾Ë¿ËÀËÁËÂËÃËÄËÅËÆËÇËÈËÉËÊËËËÌËÍËÎËÏËÐËÑËÒËÓËÔËÕËÖË×Ë°Ì±Ì²Ì³Ì´ÌµÌ¶Ì·Ì¸Ì¹ÌºÌ»Ì¼Ì½Ì¾Ì¿ÌÀÌÁÌÂÌÃÌÄÌÅÌÆÌÇÌ\
ÈÌÉÌÊÌËÌÌÌÍÌÎÌÏÌÐÌÑÌÒÌÓÌÔÌÕÌÖÌ×Ì°Í±Í²Í³Í´ÍµÍ¶Í·Í¸Í¹ÍºÍ»Í¼Í½Í¾Í¿ÍÀÍÁÍÂÍÃÍÄÍÅÍÆÍÇÍÈÍÉÍÊÍËÍÌÍÍÍÎÍÏÍÐÍÑÍÒÍÓÍÔÍÕÍÖÍ×Í°Î±Î²Î³Î´Î\
µÎ¶Î·Î¸Î¹ÎºÎ»Î¼Î½Î¾Î¿ÎÀÎÁÎÂÎÃÎÄÎÅÎÆÎÇÎÈÎÉÎÊÎËÎÌÎÍÎÎÎÏÎÐÎÑÎÒÎÓÎÔÎÕÎÖÎ×Î°Ï±Ï²Ï³Ï´ÏµÏ¶Ï·Ï¸Ï¹ÏºÏ»Ï¼Ï½Ï¾Ï¿ÏÀÏÁÏÂÏÃÏÄÏÅÏÆÏÇÏÈÏÉÏ\
ÊÏËÏÌÏÍÏÎÏÏÏÐÏÑÏÒÏÓÏÔÏÕÏÖÏ×Ï°Ð±Ð²Ð³Ð´ÐµÐ¶Ð·Ð¸Ð¹ÐºÐ»Ð¼Ð½Ð¾Ð¿ÐÀÐÁÐÂÐÃÐÄÐÅÐÆÐÇÐÈÐÉÐÊÐËÐÌÐÍÐÎÐÏÐÐÐÑÐÒÐÓÐÔÐÕÐÖÐ×Ð°Ñ±Ñ²Ñ³Ñ´ÑµÑ¶Ñ\
·Ñ¸Ñ¹ÑºÑ»Ñ¼Ñ½Ñ¾Ñ¿ÑÀÑÁÑÂÑÃÑÄÑÅÑÆÑÇÑÈÑÉÑÊÑËÑÌÑÍÑÎÑÏÑÐÑÑÑÒÑÓÑÔÑÕÑÖÑ×ÑòÑ°Ò±Ò²Ò³Ò´ÒµÒ¶Ò·Ò¸Ò¹ÒºÒ»Ò¼Ò½Ò¾Ò¿ÒÀÒÁÒÂÒÃÒÄÒÅÒÆÒÇÒÈÒÉÒÊÒ\
ËÒÌÒÍÒÎÒÏÒÐÒÑÒÒÒÓÒÔÒÕÒÖÒ×Ò°Ó±Ó²Ó³Ó´ÓµÓ¶Ó·Ó¸Ó¹ÓºÓ»Ó¼Ó½Ó¾Ó¿ÓÀÓÁÓÂÓÃÓÄÓÅÓÆÓÇÓÈÓÉÓÊÓËÓÌÓÍÓÎÓÏÓÐÓÑÓÒÓÓÓÔÓÕÓÖÓ×Ó°Ô±Ô²Ô³Ô´ÔµÔ¶Ô·Ô\
¸Ô¹ÔºÔ»Ô¼Ô½Ô¾Ô¿ÔÀÔÁÔÂÔÃÔÄÔÅÔÆÔÇÔÈÔÉÔÊÔËÔÌÔÍÔÎÔÏÔÐÔÑÔÒÔÓÔÔÔÕÔÖÔ×Ô°Õ±Õ²Õ³Õ´ÕµÕ¶Õ·Õ¸Õ¹ÕºÕ»Õ¼Õ½Õ¾Õ¿ÕÀÕÁÕÂÕÃÕÄÕÅÕÆÕÇÕÈÕÉÕÊÕËÕÌÕ\
ÍÕÎÕÏÕÐÕÑÕÒÕÓÕÔÕÕÕÖÕ×Õ°Ö±Ö²Ö³Ö´ÖµÖ¶Ö·Ö¸Ö¹ÖºÖ»Ö¼Ö½Ö¾Ö¿ÖÀÖÁÖÂÖÃÖÄÖÅÖÆÖÇÖÈÖÉÖÊÖËÖÌÖÍÖÎÖÏÖÐÖÑÖÒÖÓÖÔÖÕÖÖÖ×Ö°×±×²×³×´×µ×¶×·×¸×¹×\
º×»×¼×½×¾×¿×À×Á×Â×Ã×Ä×Å×Æ×Ç×È×É×Ê×Ë×Ì×Í×Î×Ï×Ð×Ñ×Ò×Ó×Ô×Õ×Ö×××°Ø±Ø²Ø³Ø´ØµØ¶Ø·Ø¸Ø¹ØºØ»Ø¼Ø½Ø¾Ø¿ØÀØÁØÂØÃØÄØÅØÆØÇØÈØÉØÊØËØÌØÍØÎØ\
ÏØÐØÑØÒØÓØÔØÕØÖØ×Ø°Ù±Ù²Ù³Ù´ÙµÙ¶Ù·Ù¸Ù¹ÙºÙ»Ù¼Ù½Ù¾Ù¿ÙÀÙÁÙÂÙÃÙÄÙÅÙÆÙÇÙÈÙÉÙÊÙËÙÌÙÍÙÎÙÏÙÐÙÑÙÒÙÓÙÔÙÕÙÖÙ×ÙéÙ°Ú±Ú²Ú³Ú´ÚµÚ¶Ú·Ú¸Ú¹ÚºÚ\
»Ú¼Ú½Ú¾Ú¿ÚÀÚÁÚÂÚÃÚÄÚÅÚÆÚÇÚÈÚÉÚÊÚËÚÌÚÍÚÎÚÏÚÐÚÑÚÒÚÓÚÔÚÕÚÖÚ×Ú°Û±Û²Û³Û´ÛµÛ¶Û·Û¸Û¹ÛºÛ»Û¼Û½Û¾Û¿ÛÀÛÁÛÂÛÃÛÄÛÅÛÆÛÇÛÈÛÉÛÊÛËÛÌÛÍÛÎÛÏÛ\
ÐÛÑÛÒÛÓÛÔÛÕÛÖÛ×Û°Ü±Ü²Ü³Ü´ÜµÜ¶Ü·Ü¸Ü¹ÜºÜ»Ü¼Ü½Ü¾Ü¿ÜÀÜÁÜÂÜÃÜÄÜÅÜÆÜÇÜÈÜÉÜÊÜËÜÌÜÍÜÎÜÏÜÐÜÑÜÒÜÓÜÔÜÕÜÖÜ×Ü°Ý±Ý²Ý³Ý´ÝµÝ¶Ý·Ý¸Ý¹ÝºÝ»Ý¼Ý\
½Ý¾Ý¿ÝÀÝÁÝÂÝÃÝÄÝÅÝÆÝÇÝÈÝÉÝÊÝËÝÌÝÍÝÎÝÏÝÐÝÑÝÒÝÓÝÔÝÕÝÖÝ×ÝóÝ°Þ±Þ²Þ³Þ´ÞµÞ¶Þ·Þ¸Þ¹ÞºÞ»Þ¼Þ½Þ¾Þ¿ÞÀÞÁÞÂÞÃÞÄÞÅÞÆÞÇÞÈÞÉÞÊÞËÞÌÞÍÞÎÞÏÞÐÞ\
ÑÞÒÞÓÞÔÞÕÞÖÞ×Þ°ß±ß²ß³ßµß´ß¶ß·ß¸ß¹ßºß»ß¼ß½ß¾ß¿ßÀßÁßÂßÃßÄßÅßÆßÇßÈßÉßÊßËßÌßÍßÎßÏßÐßÑßÒßÓßÔßÕßÖß×ßòß°à±à²à³à´àµà¶à·à¸à¹àºà»à¼à\
½à¾à¿àÀàÁàÂàÃàÄàÅàÆàÇàÈàÉàÊàËàÌàÍàÎàÏàÐàÑàÒàÓàÔàÕàÖà×à°á±á²á³á´áµá¶á·á¸á¹áºá»á¼á½á¾á¿áÀáÁáÂáÃáÄáÅáÆáÇáÈáÉáÊáËáÌáÍáÎáÏáÐáÑá\
ÒáÓáÔáÕáÖá×á°â±â²â³â´âµâ¶â·â¸â¹âºâ»â¼â½â¾â¿âÀâÁâÂâÃâÄâÅâÆâÇâÈâÉâÊâËâÌâÍâÎâÏâÐâÑâÒâÓâÔâÕâÖâ×â°ã±ã²ã³ã´ãµã¶ã·ã¸ã¹ãºã»ã¼ã½ã¾ã\
¿ãÀãÁãÂãÃãÄãÅãÆãÇãÈãÉãÊãËãÌãÍãÎãÏãÐãÑãÒãÓãÔãÕãÖã×ãßã°ä±ä²ä³ä´äµä¶ä·ä¸ä¹äºä»ä¼ä½ä¾ä¿äÀäÁäÂäÃäÄäÅäÆäÇäÈäÉäÊäËäÌäÍäÎäÏäÐäÑäÒä\
ÓäÔäÕäÖä×ä°å±å²å³å´åµå¶å·å¸å¹åºå»å¼å½å¾å¿åÀåÁåÂåÃåÄåÅåÆåÇåÈåÉåÊåËåÌåÍåÎåÏåÐåÑåÒåÓåÔåÕåÖå×å°æ±æ²æ³æ´æµæ¶æ·æ¸æ¹æºæ»æ¼æ½æ¾æ¿æ\
ÀæÁæÂæÃæÄæÅæÆæÇæÈæÉæÊæËæÌæÍæÎæÏæÐæÑæÒæÓæÔæÕæÖæ×æ°ç±ç²ç³ç´çµç¶ç·ç¸ç¹çºç»ç¼ç½ç¾ç¿çÀçÁçÂçÃçÄçÅçÆçÇçÈçÉçÊçËçÌçÍçÎçÏçÐçÑçÒçÓçÔç\
ÕçÖç×ç°è±è²è³è´èµè¶è·è¸è¹èºè»è¼è½è¾è¿èÀèÁèÂèÃèÄèÅèÆèÇèÈèÉèÊèËèÌèÍèÎèÏèÐèÑèÒèÓèÔèÕèÖè×èßè°é±é²é³é´éµé¶é·é¸é¹éºé»é¼é½é¾é¿éÀé\
ÁéÂéÃéÄéÅéÆéÇéÈéÉéÊéËéÌéÍéÎéÏéÐéÑéÒéÓéÔéÕéÖé×é°ê±ê²ê³ê´êµê¶ê·ê¸ê¹êºê»ê¼ê½ê¾ê¿êÀêÁêÂêÃêÄêÅêÆêÇêÈêÉêÊêËêÌêÍêÎêÏêÐêÑêÒêÓêÔêÕê\
Öê×ê°ë±ë²ë³ë´ëµë¶ë·ë¸ë¹ëºë»ë¼ë½ë¾ë¿ëÀëÁëÂëÃëÄëÅëÆëÇëÈëÉëÊëËëÌëÍëÎëÏëÐëÑëÒëÓëÔëÕëÖë×ë°ì±ì²ì³ì´ìµì¶ì·ì¸ì¹ìºì»ì¼ì½ì¾ì¿ìÀìÁìÂì\
ÃìÄìÅìÆìÇìÈìÉìÊìËìÌìÍìÎìÏìÐìÑìÒìÓìÔìÕìÖì×ì°í±í²í³í´íµí¶í·í¸í¹íºí»í¼í½í¾í¿íÀíÁíÂíÃíÄíÅíÆíÇíÈíÊíÉíËíÌíÍíÎíÏíÐíÑíÒíÓíÔíÕíÖí×í\
°î±î²î³î´îµî¶î·î¸î¹îºî»î¼î½î¾î¿îÀîÁîÂîÃîÄîÅîÆîÇîÈîÉîÊîËîÌîÍîÎîÏîÐîÑîÒîÓîÔîÕîÖî×î°ï±ï²ï³ï´ïµï¶ï·ï¸ï¹ïºï»ï½ï¾ï¿ïÀïÁïÂïÃïÄïÅï\
ÆïÇïÈïÉïÊïËïÌïÍïÎïÏïÐïÑïÒïÓïÔïÕïÖï×ï°ð±ð²ð³ð´ðµð¶ð·ð¸ð¹ðºð»ð¼ð½ð¾ð¿ðÀðÁðÂðÃðÄðÅðÆðÇðÈðÉðÊðËðÌðÍðÎðÏðÐðÑðÒðÓðÔðÕðÖð×ð°ñ±ñ²ñ\
³ñ´ñµñ¶ñ·ñ¸ñ¹ñºñ»ñ¼ñ½ñ¾ñ¿ñÀñÁñÂñÃñÄñÅñÆñÇñÈñÉñÊñËñÌñÍñÎñÏñÐñÑñÒñÓñÔñÕñÖñ×ñ°ò±ò²ò³ò´òµò¶ò·ò¸ò¹òºò»ò¼ò½ò¾ò¿òÀòÁòÂòÃòÄòÅòÆòÇò\
ÈòÉòÊòËòÌòÍòÎòÏòÐòÑòÒòÓòÔòÕòÖò×ò°ó±ó²ó³ó´óµó¶ó·ó¸ó¹óºó»ó¼ó½ó¾ó¿óÀóÁóÂóÃóÄóÅóÆóÇóÈóÉóÊóËóÌóÍóÎóÏóÐóÑóÒóÓóÔóÕóÖó×ó°ô±ô²ô³ô´ô\
µô¶ô·ô¸ô¹ôºô»ô¼ô½ô¾ô¿ôÀôÁôÂôÃôÄôÅôÆôÇôÈôÉôÊôËôÌôÍôÎôÏôÐôÑôÒôÓôÔôÕôÖô×ô°õ±õ²õ³õ´õµõ¶õ·õ¸õ¹õºõ»õ¼õ½õ¾õ¿õÀõÁõÂõÃõÄõÅõÆõÇõÈõÉõ\
ÊõËõÌõÍõÎõÏõÐõÑõÒõÓõÔõÕõÖõ×õ°ö±ö²ö³ö´öµö¶ö·ö¸ö¹öºö»ö¼ö½ö¾ö¿öÀöÁöÂöÃöÄöÅöÆöÇöÈöÉöÊöËöÌöÍöÎöÏöÐöÑöÒöÓöÔöÕöÖö×ö°÷±÷²÷³÷´÷µ÷¶÷\
·÷¸÷¹÷º÷»÷¼÷½÷¾÷¿÷À÷Á÷Â÷Ã÷Ä÷Å÷Æ÷Ç÷È÷É÷Ê÷Ë÷Ì÷Í÷Î÷Ï÷Ð÷Ñ÷Ò÷Ó÷Ô÷Õ÷Ö÷×÷ß÷°ø±ø²ø³ø´øµø¶ø·ø¸ø¹øºø»ø¼ø½ø¾ø¿øÀøÁøÂøÃøÄøÅøÆøÇøÈøÉøÊø\
ËøÌøÍøÎøÏøÐøÑøÒøÓøÔøÕøÖø×ø°ù±ù²ù³ù´ùµù¶ù·ù¸ù¹ùºù»ù¼ù½ù¾ù¿ùÀùÁùÂùÃùÄùÅùÆùÇùÈùÉùÊùËùÌùÍùÎùÏùÐùÑùÒùÓùÔùÕùÖù×ù°ú±ú²ú³ú´úµú¶ú·ú\
¸ú¹úºú»ú¼ú½ú¾ú¿úÀúÁúÂúÃúÄúÅúÆúÇúÈúÉúÊúËúÌúÍúÎúÏúÐúÑúÒúÓúÔúÕúÖú°û±û²û³û´ûµû¶û·û¸û¹ûºû»û¼û½û¾û¿ûÀûÁûÂûÃûÄûÅûÆûÇûÈûÉûÊûËûÌûÍû\
ÎûÏûÐûÑûÒûÓûÔûÕûÖû°ü±ü²ü³ü´üµü¶ü·ü¸ü¹üºü»ü¼ü½ü¾ü¿üÁüÂüÃüÅüÄüÆüÇüÈüÉüÊüËüÌüÍüÎüÏüÐüÑüÒüÓüÔüÕüÖü°ý±ý²ý³ý´ýµý¶ý·ý¸ýºý¹ý¼ý»ý½ý\
¾ý¿ýÀýÂýÁýÃýÄýÅýÆýÇýÈýÉýÊýËýÌýÍýÎýÏýÐýÑýÒýÓýÔýÕýÖý°þ±þ²þ³þ´þµþ¶þ·þ¸þ¹þºþ»þ¼þ½þ¾þ¿þÀþÂþÁþÃþÄþÅþÆþÇþÈþÉþÊþËþÌþÍþÎþÏþÐþÑþÒþÓþ\
ÔþÕþÖþ"
#endif
#endif
#if WITH_FULL_ALL_CHINESE == 1
//ÏÂÃæÊÇºº×Ö´óÈ«
#ifndef TEXT_FONT_UNICODE_MODE0
#define TEXT_FONT_UNICODE_MODE0 " !\"#$%&\'()*+,-./0123456789:;<=>\?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\
@‚@ƒ@„@…@†@‡@ˆ@‰@Š@‹@Œ@@Ž@@@‘@’@“@”@•@–@—@˜@™@š@›@œ@@ž@Ÿ@ @ª@«@¬@­@®@¯@°@±@²@³@´@µ@¶@·@¸@¹@º@»@¼@½@¾@¿@À@Á@Â@Ã@Ä@Å@Æ@\
Ç@È@É@Ê@Ë@Ì@Í@Î@Ï@Ð@Ñ@Ò@Ó@Ô@Õ@Ö@×@Ø@Ù@Ú@Û@Ü@Ý@Þ@ß@à@á@â@ã@ä@å@æ@ç@è@é@ê@ë@ì@í@î@ï@ð@ñ@ò@ó@ô@õ@ö@÷@ø@ù@ú@û@ü@ý@A‚AƒA„A…A†A\
‡AˆA‰AŠA‹AŒAAŽAAA‘A’A“A”A•A–A—A˜A™AšA›AœAAžAŸA AªA«A¬A­A®A¯A°A±A²A³A´AµA¶A·A¸A¹AºA»A¼A½A¾A¿AÀAÁAÂAÃAÄAÅAÆAÇAÈAÉAÊAËAÌA\
ÍAÎAÏAÐAÑAÒAÓAÔAÕAÖA×AØAÙAÚAÛAÜAÝAÞAßAàAáAâAãAäAåAæAçAèAéAêAëAìAíAîAïAðAñAòAóAôAõAöA÷AøAùAúAûAüAýAB‚BƒB„B…B†B‡BˆB‰BŠB‹BŒB\
BŽBBB‘B’B“B”B•B–B—B˜B™BšB›BœBBžBŸB BªB«B­B¯B°B±B²B³B´BµB¶B·B¸B¹BºB»B¼B½B¾B¿BÀBÁBÂBÃBÄBÅBÆBÇBÈBÉBÊBËBÌBÍBÎBÏBÐBÑBÒBÓBÔB\
ÕBÖB×BØBÙBÚBÛBÜBÝBÞBßBàBáBâBãBäBåBæBçBèBéBêBëBìBíBîBïBðBñBòBóBôBõBöB÷BøBùBúBûBüBýBC‚CƒC„C…C†C‡CˆC‰CŠC‹CŒCCŽCCC‘C’C“C”C\
•C–C—C˜C™CšC›CœCCžCŸC CªC«C¬C­C®C¯C°C±C²C³C´CµC¶C·C¸C¹CºC»C¼C½C¾C¿CÀCÁCÂCÃCÄCÅCÆCÇCÈCÉCÊCËCÌCÍCÎCÏCÐCÑCÒCÓCÔCÕCÖC×CØCÙCÚC\
ÛCÜCÝCÞCßCàCáCâCãCäCåCæCçCèCéCêCëCìCíCîCïCðCñCòCóCôCõCöC÷CøCùCúCûCüCýCD‚DƒD„D…D†D‡DˆD‰DŠD‹DŒDDŽDDD‘D’D“D”D•D–D—D˜D™DšD\
›DœDDžDŸD DªD«D¬D­D®D¯D°D±D²D³D´DµD¶D·D¸D¹DºD»D¼D½D¾D¿DÀDÁDÂDÃDÄDÅDÆDÇDÈDÉDÊDËDÌDÍDÎDÏDÐDÑDÒDÓDÔDÕDÖD×DØDÙDÚDÛDÜDÝDÞDßDàD\
áDâDãDäDåDæDçDèDéDêDëDìDíDîDïDðDñDòDóDôDõDöD÷DøDùDúDûDüDýDE‚EƒE„E…E†E‡EˆE‰EŠE‹EŒEEŽEEE‘E’E“E”E•E–E—E˜E™EšE›EœEEžEŸE E\
ªE«E¬E­E®E¯E°E±E²E³E´EµE¶E·E¸E¹EºE»E¼E½E¾E¿EÀEÁEÂEÃEÄEÅEÆEÇEÈEÉEÊEËEÌEÍEÎEÏEÐEÑEÒEÓEÔEÕEÖE×EØEÙEÚEÛEÜEÝEÞEßEàEáEâEãEäEåEæE\
çEèEéEêEëEìEíEîEïEðEñEòEóEôEõEöE÷EøEùEúEûEüEýEF‚FƒF„F…F†F‡FˆF‰FŠF‹FŒFFŽFFF‘F’F“F”F•F–F—F˜F™FšF›FœFFžFŸF FªF«F¬F­F¯F°F\
±F²F³F´FµF¶F·F¸F¹FºF»F¼F½F¾F¿FÀFÁFÂFÃFÄFÅFÆFÇFÈFÉFÊFËFÌFÍFÎFÏFÐFÑFÒFÓFÔFÕFÖF×FØFÙFÚFÛFÜFÝFÞFßFàFáFâFãFäFåFæFçFèFéFêFëFìFíF\
îFïFðFñFòFóFôFõFöF÷FøFùFúFûFüFýFG‚GƒG„G…G†G‡GˆG‰GŠG‹GŒGGŽGGG‘G’G“G”G•G–G—G˜G™GšG›GœGGžGŸG GªG«G¬G­G®G¯G°G±G²G³GµG¶G·G\
¸G¹GºG»G¼G½G¾G¿GÀGÁGÂGÃGÄGÅGÆGÇGÈGÉGÊGËGÌGÍGÎGÏGÐGÑGÒGÓGÔGÕGÖG×GØGÙGÚGÛGÜGÝGÞGßGàGáGâGãGäGåGæGçGèGéGêGëGìGíGîGïGðGñGòGóGôG\
õGöG÷GøGùGúGûGüGýGH‚HƒH„H…H†H‡HˆH‰HŠH‹HŒHHŽHHH‘H’H“H”H•H–H—H˜H™HšH›HœHHžHŸH HªH«H¬H­H®H¯H°H±H²H³H´HµH¶H·H¸H¹HºH»H¼H½H\
¾H¿HÀHÁHÂHÃHÄHÅHÆHÇHÈHÉHÊHËHÌHÍHÎHÏHÐHÑHÒHÓHÔHÕHÖH×HØHÙHÚHÛHÜHÝHÞHßHàHáHâHãHäHåHæHçHèHéHêHëHìHíHîHïHðHñHòHóHôHõHöH÷HøHùHúH\
ûHüHýHI‚IƒI„I…I†I‡IˆI‰IŠI‹IŒIIŽIII‘I’I“I”I•I–I—I˜I™IšI›IœIIžIŸI IªI«I¬I­I®I¯I°I±I²I³I´IµI¶I·I¸I¹IºI»I¼I½I¾I¿IÀIÁIÂIÃI\
ÄIÅIÆIÇIÈIÉIÊIËIÌIÍIÎIÏIÐIÑIÒIÓIÔIÕIÖI×IØIÙIÚIÛIÜIÝIÞIßIàIáIâIãIäIåIæIçIèIéIêIëIìIíIîIïIðIñIòIóIôIõIöI÷IøIùIúIûIüIýIJ‚JƒJ\
„J…J†J‡JˆJ‰JŠJ‹JŒJJŽJJJ‘J’J“J”J•J–J—J˜J™JšJ›JœJJžJŸJ JªJ«J¬J­J®J¯J°J±J²J³J´JµJ¶J·J¸J¹JºJ»J¼J½J¾J¿JÀJÁJÂJÃJÄJÅJÆJÇJÈJÉJ\
ÊJÌJÍJÎJÏJÐJÑJÒJÓJÔJÕJÖJ×JØJÙJÚJÛJÜJÝJÞJßJàJáJâJãJäJåJæJçJèJéJêJëJìJíJîJïJðJñJòJóJôJõJöJ÷JøJùJúJûJüJýJK‚KƒK„K…K†K‡KˆK‰KŠK\
‹KŒKKŽKKK‘K’K“K”K•K–K—K˜K™KšK›KœKKžKŸK KªK«K¬K­K®K¯K°K±K²K³K´KµK¶K·K¸K¹KºK»K¼K½K¾K¿KÀKÁKÂKÃKÄKÅKÆKÇKÈKÉKÊKËKÌKÍKÎKÏKÐK\
ÑKÒKÓKÔKÕKÖK×KØKÙKÚKÛKÜKÝKÞKßKàKáKâKãKäKåKæKçKèKéKêKëKìKíKîKïKðKñKòKóKôKõKöK÷KøKùKúKûKüKýKL‚LƒL„L…L†L‡LˆL‰LŠL‹LŒLLŽLLL\
‘L’L“L”L•L–L—L˜L™LšL›LœLLžLŸL LªL«L¬L­L®L¯L°L±L²L³L´LµL¶L·L¸L¹LºL»L¼L½L¾L¿LÀLÁLÂLÃLÄLÅLÆLÇLÈLÉLÊLËLÌLÍLÎLÏLÐLÑLÒLÓLÔLÕLÖL\
×LØLÙLÚLÛLÜLÝLÞLßLàLáLâLãLäLåLæLçLèLéLêLëLìLíLîLïLðLñLòLóLôLõLöL÷LøLùLúLûLüLýLM‚MƒM„M…M†M‡MˆM‰MŠM‹MŒMMŽMMM‘M’M“M”M•M–M\
—M˜M™MšM›MœMMžMŸM MªM«M¬M­M¯M°M±M²M³M´MµM¶M·M¸M¹MºM»M¼M½M¾M¿MÀMÁMÂMÃMÄMÅMÆMÇMÈMÉMÊMËMÌMÍMÎMÏMÐMÑMÒMÓMÔMÕMÖM×MØMÙMÚMÛMÜMÝM\
ÞMßMàMáMâMãMäMåMæMçMèMéMêMëMìMíMîMïMðMñMòMóMôMõMöM÷MøMùMúMûMüMýMN‚NƒN„N…N†N‡NˆN‰NŠN‹NŒNNŽNNN‘N’N“N”N•N–N—N˜N™NšN›NœNN\
žNŸN NªN«N¬N­N®N¯N°N±N²N³N´NµN¶N·N¸N¹NºN»N¼N½N¾N¿NÀNÁNÂNÃNÄNÅNÆNÇNÈNÉNÊNËNÌNÍNÎNÏNÐNÑNÒNÓNÔNÕNÖN×NØNÙNÚNÛNÜNÝNÞNßNàNáNâNãN\
äNåNæNçNèNéNêNëNìNíNîNïNðNñNòNóNôNõNöN÷NøNùNúNûNüNýNO‚OƒO„O…O†O‡OˆO‰OŠO‹OŒOOŽOOO‘O’O“O”O•O–O—O˜O™OšO›OœOOžOŸO OªO«O¬O\
­O®O¯O°O±O²O³O´OµO¶O·O¸O¹OºO»O¼O½O¾O¿OÀOÁOÂOÃOÄOÅOÆOÇOÈOÉOÊOËOÌOÍOÎOÏOÐOÑOÒOÓOÔOÕOÖO×OØOÙOÚOÛOÜOÝOÞOßOàOáOâOãOäOåOæOçOèOéO\
êOëOìOíOîOïOðOñOòOóOôOõOöO÷OøOùOúOûOüOýOP‚PƒP„P…P†P‡PˆP‰PŠP‹PŒPPŽPPP‘P’P“P”P•P–P—P˜P™PšP›PœPPžPŸP PªP«P¬P­P®P¯P°P±P²P\
³P´PµP¶P·P¸P¹PºP»P¼P½P¾P¿PÀPÁPÂPÃPÄPÅPÆPÇPÈPÉPÊPËPÌPÍPÎPÏPÐPÑPÒPÓPÔPÕPÖP×PØPÙPÚPÛPÜPÝPÞPßPàPáPâPãPäPåPæPçPèPéPêPëPìPíPîPïP\
ðPñPòPóPôPõPöP÷PøPùPúPûPüPýPQ‚QƒQ„Q…Q†Q‡QˆQ‰QŠQ‹QŒQQŽQQQ‘Q’Q“Q”Q•Q–Q—Q˜Q™QšQ›QœQQžQŸQ QªQ«Q¬Q­Q®Q¯Q°Q±Q²Q³Q´QµQ¶Q·Q¸Q\
¹QºQ»Q¼Q½Q¾Q¿QÀQÁQÂQÃQÄQÅQÆQÇQÈQÉQÊQËQÌQÍQÎQÏQÐQÑQÒQÓQÔQÕQÖQ×QØQÙQÚQÛQÜQÝQÞQßQàQáQâQãQäQåQæQçQèQéQêQëQìQíQîQïQðQñQòQóQôQõQ\
öQ÷QøQùQúQûQüQýQR‚RƒR„R…R†R‡RˆR‰RŠR‹RŒRRŽRRR‘R’R“R”R•R–R—R˜R™RšR›RœRRžRŸR RªR«R¬R­R®R¯R°R±R²R³R´RµR¶R·R¸R¹RºR»R¼R½R¾R\
¿RÀRÁRÂRÃRÄRÅRÆRÇRÈRÉRÊRËRÌRÍRÎRÏRÐRÑRÒRÓRÔRÕRÖR×RØRÙRÚRÛRÜRÝRÞRßRàRáRâRãRäRåRæRçRèRéRêRëRìRíRîRïRðRñRòRóRôRõRöR÷RøRùRúRûR\
üRýRS‚SƒS„S…S†S‡SˆS‰SŠS‹SŒSSŽSSS‘S’S“S”S•S–S—S˜S™SšS›SœSSžSŸS SªS«S¬S­S®S¯S°S±S²S³S´SµS¶S·S¸S¹SºS»S¼S½S¾S¿SÀSÁSÂSÃSÄS\
ÅSÆSÇSÈSÉSÊSËSÌSÍSÎSÏSÐSÑSÒSÓSÔSÕSÖS×SØSÙSÚSÛSÜSÝSÞSßSàSáSâSãSäSåSæSçSèSéSêSëSìSíSîSïSðSñSòSóSôSõSöS÷SøSùSúSûSüSýST‚TƒT„T\
…T†T‡TˆT‰TŠT‹TŒTTŽTTT‘T’T“T”T•T–T—T˜T™TšT›TœTTžTŸT TªT«T¬T­T®T¯T°T±T²T³T´TµT¶T·T¸T¹TºT»T¼T½T¾T¿TÀTÁTÂTÃTÄTÅTÆTÇTÈTÉTÊT\
ËTÌTÍTÎTÏTÐTÑTÒTÓTÔTÕTÖT×TØTÙTÚTÛTÜTÝTÞTßTàTáTâTãTäTåTæTçTèTéTêTëTìTíTîTïTðTñTòTóTôTõTöT÷TøTùTúTûTüTýTU‚UƒU„U…U†U‡UˆU‰UŠU\
‹UŒUUŽUUU‘U’U“U”U•U–U—U˜U™UšU›UœUUžUŸU UªU«U¬U­U®U¯U°U±U²U³U´UµU¶U·U¸U¹UºU»U¼U½U¾U¿UÀUÁUÂUÃUÄUÅUÆUÇUÈUÉUÊUËUÌUÍUÎUÏUÐU\
ÑUÒUÓUÔUÕUÖU×UØUÙUÚUÛUÜUÝUÞUßUàUáUâUãUäUåUæUçUèUéUêUëUìUíUîUïUðUñUòUóUôUõUöU÷UøUùUúUûUüUýUV‚VƒV„V…V†V‡VˆV‰VŠV‹VŒVVŽVVV\
‘V’V“V”V•V–V—V˜V™VšV›VœVVžVŸV VªV«V¬V­V®V¯V°V±V²V³V´VµV¶V·V¸V¹VºV»V¼V½V¾V¿VÀVÁVÂVÃVÄVÅVÆVÇVÈVÉVÊVËVÌVÍVÎVÏVÐVÑVÒVÓVÔVÕVÖV\
×VØVÙVÚVÛVÜVÝVÞVßVàVáVâVãVäVåVæVçVèVéVêVëVìVíVîVïVðVñVòVóVôVõVöV÷VøVùVúVûVüVýVW‚WƒW„W…W†W‡WˆW‰WŠW‹WŒWWŽWWW‘W’W“W”W•W–W\
—W˜W™WšW›WœWWžWŸW WªW«W¬W­W®W¯W°W±W²W³W´WµW¶W·W¸W¹WºW»W¼W½W¾W¿WÀWÁWÂWÃWÄWÅWÆWÇWÈWÉWÊWËWÌWÍWÎWÏWÐWÑWÒWÓWÔWÕWÖW×WØWÙWÚWÛWÜW\
ÝWÞWßWàWáWâWãWäWåWæWçWèWéWêWëWìWíWîWïWðWñWòWóWôWõWöW÷WøWùWúWûWüWýWX‚XƒX„X…X†X‡XˆX‰XŠX‹XŒXXŽXXX‘X’X“X”X•X–X—X˜X™XšX›XœX\
XžXŸX XªX«X¬X­X®X¯X°X±X²X³X´XµX¶X·X¸X¹XºX»X¼X½X¾X¿XÀXÁXÂXÃXÄXÅXÆXÇXÈXÉXÊXËXÌXÍXÎXÏXÐXÑXÒXÓXÔXÕXÖX×XØXÙXÚXÛXÜXÝXÞXßXàXáXâX\
ãXäXåXæXçXèXéXêXëXìXíXîXïXðXñXòXóXôXõXöX÷XøXùXúXûXüXýXY‚YƒY„Y…Y†Y‡YˆY‰YŠY‹YŒYYŽYYY‘Y’Y“Y”Y•Y—Y˜Y™YšY›YœYYžYŸY YªY«Y¬Y\
­Y®Y¯Y°Y±Y²Y³Y´YµY¶Y·Y¸Y¹YºY»Y¼Y½Y¾Y¿YÀYÁYÂYÃYÄYÅYÆYÇYÈYÉYÊYËYÌYÍYÎYÏYÐYÑYÒYÓYÔYÕYÖY×YØYÙYÚYÛYÜYÝYÞYßYàYáYâYãYäYåYæYçYèYéY\
êYëYìYíYîYïYðYñYòYóYôYõYöY÷YøYùYúYûYüYýYZ‚ZƒZ„Z…Z†Z‡ZˆZ‰ZŠZ‹ZŒZZŽZZZ‘Z’Z“Z”Z•Z–Z—Z˜Z™ZšZ›ZœZZžZŸZ ZªZ«Z¬Z­Z®Z¯Z°Z±Z²Z\
³Z´ZµZ¶Z·Z¸Z¹ZºZ»Z¼Z½Z¾Z¿ZÀZÁZÂZÃZÄZÅZÆZÇZÈZÉZÊZËZÌZÍZÎZÏZÐZÑZÒZÓZÔZÕZÖZ×ZØZÙZÚZÛZÜZÝZÞZßZàZáZâZãZäZåZæZçZèZéZêZëZìZíZîZïZ\
ðZñZòZóZôZõZöZ÷ZøZùZúZûZüZýZ[‚[ƒ[„[…[†[‡[ˆ[‰[Š[‹[Œ[[Ž[[[‘[’[“[”[•[–[—[˜[™[š[›[œ[[ž[Ÿ[ [ª[«[¬[­[®[¯[°[±[²[³[´[µ[¶[·[¸[\
¹[º[»[¼[½[¾[¿[À[Á[Â[Ã[Ä[Å[Æ[Ç[È[É[Ê[Ë[Ì[Í[Î[Ï[Ð[Ñ[Ò[Ó[Ô[Õ[Ö[×[Ø[Ù[Ú[Û[Ü[Ý[Þ[ß[à[á[â[ã[ä[å[æ[ç[è[é[ê[ë[ì[í[î[ï[ð[ñ[ò[ó[ô[õ[\
ö[÷[ø[ù[ú[û[ü[ý[\‚\ƒ\„\…\†\‡\ˆ\‰\Š\‹\Œ\\Ž\\\‘\’\“\”\•\–\—\˜\™\š\›\œ\\ž\Ÿ\ \ª\«\¬\­\®\¯\°\±\²\³\´\µ\¶\·\¸\¹\º\»\¼\½\¾\\
¿\À\Á\Â\Ã\Ä\Å\Æ\Ç\È\É\Ê\Ë\Ì\Í\Î\Ï\Ð\Ñ\Ò\Ó\Ô\Õ\Ö\×\Ø\Ù\Ú\Û\Ü\Ý\Þ\ß\à\á\â\ã\ä\å\æ\ç\è\é\ê\ë\ì\í\î\ï\ð\ñ\ò\ó\ô\õ\ö\÷\ø\ù\ú\û\\
ü\ý\]‚]ƒ]„]…]†]‡]ˆ]‰]Š]‹]Œ]]Ž]]]‘]’]“]”]•]–]—]˜]™]š]›]œ]]ž]Ÿ] ]ª]«]¬]­]®]¯]°]±]²]³]´]µ]¶]·]¸]¹]º]»]¼]½]¾]¿]À]Á]Â]Ã]Ä]\
Å]Æ]Ç]È]É]Ê]Ë]Ì]Í]Î]Ï]Ð]Ñ]Ò]Ó]Ô]Õ]Ö]×]Ø]Ù]Ú]Û]Ü]Ý]Þ]ß]à]á]â]ã]ä]å]æ]ç]è]é]ê]ë]ì]í]î]ï]ð]ñ]ò]ó]ô]õ]ö]÷]ø]ù]ú]û]ü]ý]^‚^ƒ^„^\
…^†^‡^ˆ^‰^Š^‹^Œ^^Ž^^^‘^’^“^”^•^–^—^˜^™^š^›^œ^^ž^Ÿ^ ^ª^«^¬^­^®^¯^°^±^²^³^´^µ^¶^·^¸^¹^º^»^¼^½^¾^¿^À^Á^Â^Ã^Ä^Å^Æ^Ç^È^É^Ê^\
Ë^Ì^Í^Î^Ï^Ð^Ñ^Ò^Ó^Ô^Õ^Ö^×^Ø^Ù^Ú^Û^Ü^Ý^Þ^ß^à^á^â^ã^ä^å^æ^ç^è^é^ê^ë^ì^í^î^ï^ð^ñ^ò^ó^ô^õ^ö^÷^ø^ù^ú^û^ü^ý^_‚_ƒ_„_…_†_‡_ˆ_‰_Š_\
‹_Œ__Ž___‘_’_“_”_•_–_—_˜_™_š_›_œ__ž_Ÿ_ _ª_«_¬_­_®_¯_°_±_²_³_´_µ_¶_·_¸_¹_º_»_¼_½_¾_¿_À_Á_Â_Ã_Ä_Å_Æ_Ç_È_É_Ê_Ë_Ì_Í_Î_Ï_Ð_\
Ñ_Ò_Ó_Ô_Õ_Ö_×_Ø_Ù_Ú_Û_Ü_Ý_Þ_ß_à_á_â_ã_ä_å_æ_ç_è_é_ê_ë_ì_í_î_ï_ð_ñ_ò_ó_ô_õ_ö_÷_ø_ù_ú_û_ü_ý_`‚`ƒ`„`…`†`‡`ˆ`‰`Š`‹`Œ``Ž```\
‘`’`“`”`•`–`—`˜`™`š`›`œ``ž`Ÿ` `ª`«`¬`­`®`¯`°`±`²`³`´`µ`¶`·`¸`¹`º`»`¼`½`¾`¿`À`Á`Â`Ã`Ä`Å`Æ`Ç`È`É`Ê`Ë`Ì`Í`Î`Ï`Ð`Ñ`Ò`Ó`Ô`Õ`Ö`\
×`Ø`Ù`Ú`Û`Ü`Ý`Þ`ß`à`á`â`ã`ä`å`æ`ç`è`é`ê`ë`ì`í`î`ï`ð`ñ`ò`ó`ô`õ`ö`÷`ø`ù`ú`û`ü`ý`a‚aƒa„a…a†a‡aˆa‰aŠa‹aŒaaŽaaa‘a’a“a”a•a–a\
—a˜a™aša›aœaažaŸa aªa«a¬a­a®a¯a°a±a²a³a´aµa¶a·a¸a¹aºa»a¼a½a¾a¿aÀaÁaÂaÃaÄaÅaÆaÇaÈaÉaÊaËaÌaÍaÎaÏaÐaÑaÒaÓaÔaÕaÖa×aØaÙaÚaÛaÜa\
ÝaÞaßaàaáaâaãaäaåaæaçaèaéaêaëaìaíaîaïaðañaòaóaôaõaöa÷aøaùaúaûaüaýab‚bƒb„b…b†b‡bˆb‰bŠb‹bŒbbŽbbb‘b’b“b”b•b–b—b˜b™bšb›bœb\
bžbŸb bªb«b¬b­b®b¯b°b±b²b³b´bµb¶b·b¸b¹bºb»b¼b½b¾b¿bÀbÁbÂbÃbÄbÅbÆbÇbÈbÉbÊbËbÌbÍbÎbÏbÐbÑbÒbÓbÔbÕbÖb×bØbÙbÚbÛbÜbÝbÞbßbàbábâb\
ãbäbåbæbçbèbébêbëbìbíbîbïbðbñbòbóbôbõböb÷bøbùbúbûbübýbc‚cƒc„c…c†c‡cˆc‰cŠc‹cŒccŽccc‘c’c“c”c•c–c—c˜c™cšc›cœccžcŸc cªc«c\
¬c­c®c¯c°c±c²c³c´cµc¶c·c¸c¹cºc»c¼c½c¾c¿cÀcÁcÂcÃcÄcÅcÆcÇcÈcÉcÊcËcÌcÍcÎcÐcÑcÒcÓcÔcÕcÖc×cØcÙcÚcÛcÜcÝcÞcßcàcácâcãcäcåcæcçcècéc\
êcëcìcícîcïcðcñcòcócôcõcöc÷cøcùcúcûcücýcd‚dƒd„d…d†d‡dˆd‰dŠd‹dŒddŽddd‘d’d“d”d•d–d—d˜d™dšd›dœddždŸd dªd«d¬d­d®d¯d°d±d²d\
³d´dµd¶d·d¸d¹dºd»d¼d½d¾d¿dÀdÁdÂdÃdÄdÅdÆdÇdÈdÉdÊdËdÌdÍdÎdÏdÐdÑdÒdÓdÔdÕdÖd×dØdÙdÚdÛdÜdÝdÞdßdàdádâdãdädådædçdèdédêdëdìdídîdïd\
ðdñdòdódôdõdöd÷dødùdúdûdüdýde‚eƒe„e…e†e‡eˆe‰eŠe‹eŒeeŽeee‘e’e“e”e•e–e—e˜e™eše›eœeežeŸe eªe«e¬e­e®e¯e°e±e²e³e´eµe¶e·e¸e\
¹eºe»e¼e½e¾e¿eÀeÁeÂeÃeÄeÅeÇeÈeÉeÊeËeÌeÍeÎeÏeÐeÑeÒeÓeÔeÕeÖe×eØeÙeÚeÛeÜeÝeÞeßeàeáeâeãeäeåeæeçeèeéeêeëeìeíeîeïeðeñeòeóeôeõeöe\
÷eøeùeúeûeüeýef‚fƒf„f…f†f‡fˆf‰fŠf‹fŒffŽfff‘f’f“f”f•f–f—f˜f™fšf›fœffžfŸf fªf«f¬f­f®f¯f°f±f²f³f´fµf¶f·f¸f¹fºf»f¼f½f¾f¿f\
ÀfÁfÂfÃfÄfÅfÆfÇfÈfÉfÊfËfÌfÍfÎfÏfÐfÑfÒfÓfÔfÕfÖf×fØfÙfÚfÛfÜfÝfÞfßfàfáfâfãfäfåfæfçfèféfêfëfìfífîfïfðfñfòfófôfõföf÷føfùfúfûfüf\
ýf‚gƒg„g…g†g‡gˆg‰gŠg‹gŒggŽggg‘g’g“g”g•g–g—g˜g™gšg›gœggžgŸg g«g¬g­g®g¯g°g±g²g³g´gµg¶g·g¸g¹gºg»g¼g½g¾g¿gÀgÁgÂgÃgÄgÅgÆgÇg\
ÈgÉgÊgËgÌgÍgÎgÏgÐgÑgÒgÓgÔgÕgÖg×gØgÙgÚgÛgÜgÝgÞgßgàgágâgãgägågægçgègégêgëgìgígîgïgðgñgògógôgõgög÷gøgùgúgûgügýgh‚hƒh„h…h†h‡h\
ˆh‰hŠh‹hŒhhŽhhh‘h’h“h”h•h–h—h˜h™hšh›hœhhžhŸh hªh«h¬h­h®h¯h°h±h²h³h´hµh¶h·h¸h¹hºh»h¼h½h¾h¿hÀhÁhÂhÃhÄhÅhÆhÇhÈhÉhÊhËhÌhÍh\
ÎhÏhÐhÑhÒhÓhÔhÕhÖh×hØhÙhÚhÛhÜhÝhÞhßhàháhâhãhähåhæhçhèhéhêhëhìhíhîhïhðhñhòhóhôhõhöh÷høhùhúhûhühýh‚iƒi„i…i†i‡iˆi‰iŠi‹iŒiiŽi\
ii‘i’i“i”i•i–i—i˜i™iši›iœiižiŸi iªi«i¬i­i®i¯i°i±i²i³i´i¶i·i¸i¹iºi»i¼i½i¾i¿iÀiÁiÂiÃiÄiÅiÆiÇiÈiÉiÊiËiÌiÍiÎiÏiÐiÑiÒiÓiÔiÕi\
Öi×iØiÙiÚiÛiÜiÝiÞißiàiáiâiãiäiåiæiçièiéiêiëiìiíiîiïiðiñiòióiôiõiöi÷iøiùiúiûiüiýij‚jƒj„j…j†j‡jˆj‰jŠj‹jŒjjŽjjj‘j’j“j”j•j\
–j—j˜j™jšj›jœjjžjŸj jªj«j¬j­j®j¯j°j±j²j³j´jµj¶j·j¸j¹jºj»j¼j½j¾j¿jÀjÁjÂjÃjÄjÅjÆjÇjÈjÉjÊjËjÌjÍjÎjÏjÐjÑjÒjÓjÔjÕjÖj×jØjÙjÚjÛj\
ÜjÝjÞjßjàjájâjãjäjåjæjçjèjéjêjëjìjíjîjïjðjñjòjójôjõjöj÷jøjùjújûjüjýj‚kƒk„k…k†k‡kˆk‰kŠk‹kŒkkŽkkk‘k’k“k”k•k–k—k˜k™kšk›kœk\
kžkŸk kªk«k¬k­k®k¯k°k±k²k³k´kµk¶k·k¸k¹kºk»k¼k½k¾k¿kÀkÁkÂkÃkÄkÅkÆkÇkÈkÉkÊkËkÌkÍkÎkÏkÐkÑkÒkÓkÔkÕkÖk×kØkÙkÚkÛkÜkÝkÞkßkàkákâk\
ãkäkåkækçkèkékêkëkìkíkîkïkðkñkòkókôkõkök÷køkùkúkûkükýkl‚lƒl„l…l†l‡lˆl‰lŠl‹lŒllŽlll‘l’l“l”l•l–l—l˜l™lšl›lœllžlŸl lªl«l\
¬l­l®l¯l°l±l²l³l´lµl¶l·l¸l¹lºl»l¼l½l¾l¿lÀlÁlÂlÃlÄlÅlÆlÇlÈlÉlÊlËlÌlÍlÎlÏlÐlÑlÒlÓlÔlÕlÖl×lØlÙlÚlÛlÜlÝlÞlßlàlálâlãlälålælçlèl"
#define TEXT_FONT_UNICODE_MODE1 "élêlëlìlílîlïlðlñlòlólôlõlöl÷lølùlúlûlülýlm‚mƒm„m…m†m‡mˆm‰mŠm‹mŒmmŽmmm‘m’m“m”m•m–m—m˜m™mšm›mœmmžmŸm mªm«m¬m­m®m¯m°m±m\
²m³m´mµm¶m·m¸m¹mºm»m¼m½m¾m¿mÀmÁmÂmÃmÄmÅmÆmÇmÈmÉmÊmËmÌmÍmÎmÏmÐmÑmÒmÓmÔmÕmÖm×mØmÙmÚmÛmÜmÝmÞmßmàmámâmãmämåmæmçmèmémêmëmìmímîm\
ïmðmñmòmómômõmöm÷mømùmúmûmümýmn‚nƒn„n…n†n‡nˆn‰nŠn‹nŒnnŽnnn‘n’n“n”n•n–n—n˜n™nšn›nœnnžnŸn nªn«n¬n­n®n¯n°n±n²n³n´nµn¶n·n\
¸n¹nºn»n¼n½n¾n¿nÀnÁnÂnÃnÄnÅnÆnÇnÈnÉnÊnËnÌnÍnÎnÏnÐnÑnÒnÓnÔnÕnÖn×nØnÙnÚnÛnÜnÝnÞnßnànánânãnänånænçnènénênënìnínînïnðnñnònónôn\
õnön÷nønùnúnûnünýno‚oƒo„o…o†o‡oˆo‰oŠo‹oŒooŽooo‘o’o“o”o•o–o—o˜o™ošo›oœoožoŸo oªo«o¬o­o®o¯o°o±o²o³o´oµo¶o·o¸o¹oºo»o¼o½o\
¾o¿oÀoÁoÂoÃoÄoÅoÆoÇoÈoÉoÊoËoÌoÍoÎoÏoÐoÑoÒoÓoÔoÕoÖo×oØoÙoÚoÛoÜoÝoÞoßoàoáoâoãoäoåoæoçoèoéoêoëoìoíoîoïoðoñoòoóoôoõoöo÷oøoùoúo\
ûoüoýop‚pƒp„p…p†p‡pˆp‰pŠp‹pŒppŽppp‘p’p“p”p•p–p—p˜p™pšp›pœppžpŸp pªp«p¬p­p®p¯p°p±p²p³p´pµp¶p·p¸p¹pºp»p¼p½p¾p¿pÀpÁpÂpÃp\
ÄpÅpÆpÇpÈpÉpÊpËpÌpÍpÎpÏpÐpÑpÒpÓpÔpÕpÖp×pØpÙpÚpÛpÜpÝpÞpßpàpápâpãpäpåpæpçpèpépêpëpìpípîpïpðpñpòpópôpõpöp÷pøpùpúpûpüpýpq‚qƒq\
„q…q†q‡qˆq‰qŠq‹qŒqqŽqqq‘q’q“q”q•q–q—q˜q™qšq›qœqqžqŸq qªq«q¬q­q®q¯q°q±q²q³q´qµq¶q·q¸q¹qºq»q¼q½q¾q¿qÀqÁqÂqÃqÄqÅqÆqÇqÈqÉq\
ÊqËqÌqÍqÎqÏqÐqÑqÒqÓqÔqÕqÖq×qØqÙqÚqÛqÜqÝqÞqßqàqáqâqãqäqåqæqçqèqéqêqëqìqíqîqïqðqñqòqóqôqõqöq÷qøqùqúqûqüqýqr‚rƒr„r…r†r‡rˆr‰r\
Šr‹rŒrrŽrrr‘r’r“r”r•r–r—r˜r™ršr›rœrržrŸr rªr«r¬r­r®r¯r°r±r²r³r´rµr¶r·r¸r¹rºr»r¼r½r¾r¿rÀrÁrÂrÃrÄrÅrÆrÇrÈrÉrÊrËrÌrÍrÎrÏr\
ÐrÑrÒrÓrÔrÕrÖr×rØrÙrÚrÛrÜrÝrÞrßràrárârãrärårærçrèrérêrërìrírîrïrðrñròrórôrõrör÷rørùrúrûrürýrs‚sƒs„s…s†s‡sˆs‰sŠs‹sŒssŽss\
s‘s’s“s”s•s–s—s˜s™sšs›sœssžsŸs sªs«s¬s­s®s¯s°s±s²s³s´sµs¶s·s¸s¹sºs»s¼s½s¾s¿sÀsÁsÂsÃsÄsÅsÆsÇsÈsÉsÊsËsÌsÍsÎsÏsÐsÑsÒsÓsÔsÕs\
Ös×sØsÙsÚsÛsÜsÝsÞsßsàsásâsãsäsåsæsçsèsésêsësìsísîsïsðsñsòsósôsõsös÷søsùsúsûsüsýst‚tƒt„t…t†t‡tˆt‰tŠt‹tŒttŽttt‘t’t“t”t•t\
–t—t˜t™tšt›tœttžtŸt tªt«t¬t­t®t¯t°t±t²t³t´tµt¶t·t¸t¹tºt»t¼t½t¾t¿tÀtÁtÂtÃtÄtÅtÆtÇtÈtÉtÊtËtÌtÍtÎtÏtÐtÑtÒtÓtÔtÕtÖt×tØtÙtÚtÛt\
ÜtÝtÞtßtàtátâtãtätåtætçtètétêtëtìtítîtïtðtñtòtótôtõtöt÷tøtùtútûtütýtu‚uƒu„u…u†u‡uˆu‰uŠu‹uŒuuŽuuu‘u’u“u”u•u–u—u˜u™ušu›u\
œuužuŸu uªu«u¬u­u®u¯u°u±u²u³u´uµu¶u·u¸u¹uºu»u¼u½u¾u¿uÀuÁuÂuÃuÄuÅuÆuÇuÈuÉuÊuËuÌuÍuÎuÏuÐuÑuÒuÓuÔuÕuÖu×uØuÙuÚuÛuÜuÝuÞußuàuáu\
âuãuäuåuæuçuèuéuêuëuìuíuîuïuðuñuòuóuôuõuöu÷uøuùuúuûuüuýuv‚vƒv„v…v†v‡vˆv‰vŠv‹vŒvvŽvvv‘v’v“v”v•v–v—v˜v™všv›vœvvžvŸv vªv\
«v¬v­v®v¯v°v±v²v³v´vµv¶v·v¸v¹vºv»v¼v½v¾v¿vÀvÁvÂvÃvÄvÅvÆvÇvÈvÉvÊvËvÌvÍvÎvÏvÐvÑvÒvÓvÔvÕvÖv×vØvÙvÚvÛvÜvÝvÞvßvàvávâvãvävåvævçv\
èvévêvëvìvívîvïvðvñvòvóvôvõvöv÷vøvùvúvûvüvývw‚wƒw„w…w†w‡wˆw‰wŠw‹wŒwwŽwww‘w’w“w”w•w–w—w˜w™wšw›wœwwžwŸw wªw«w¬w­w®w¯w°w\
±w²w³w´wµw¶w·w¸w¹wºw»w¼w½w¾w¿wÀwÁwÂwÃwÄwÅwÆwÇwÈwÉwÊwËwÌwÍwÎwÏwÐwÑwÒwÓwÔwÕwÖw×wØwÙwÚwÛwÜwÝwßwàwáwâwãwäwåwæwçwèwéwêwëwìwíwîw\
ïwðwñwòwówôwõwöw÷wøwùwúwûwüwýwx‚xƒx„x…x‡xˆx‰xŠx‹xŒxxŽxxx‘x’x“x”x•x–x—x˜x™xšx›xœxxžxŸx xªx«x¬x­x®x¯x°x±x²x³x´xµx¶x·x¸x\
¹xºx»x¼x½x¾x¿xÀxÁxÂxÃxÄxÅxÆxÇxÈxÉxÊxËxÌxÍxÎxÏxÐxÑxÒxÓxÔxÕxÖx×xØxÙxÚxÛxÜxÝxÞxßxàxáxâxãxäxåxæxçxèxéxêxëxìxíxîxïxðxñxòxóxôxõx\
öx÷xøxùxúxûxüxýxy‚yƒy„y…y†y‡yˆy‰yŠy‹yŒyyŽyyy‘y’y“y”y•y–y—y˜y™yšy›yœyyžyŸy yªy«y¬y­y®y¯y°y±y²y³y´yµy¶y·y¸y¹yºy»y¼y½y¾y\
¿yÀyÁyÂyÃyÄyÅyÆyÇyÈyÉyÊyËyÌyÍyÎyÏyÐyÑyÒyÓyÔyÕyÖy×yØyÙyÚyÛyÜyÝyÞyßyàyáyâyãyäyåyæyçyèyéyêyëyìyíyîyïyðyñyòyóyôyõyöy÷yøyùyúyûy\
üyýyz‚zƒz„z…z‡zˆz‰zŠz‹zŒzzŽzzz‘z’z“z”z•z–z—z˜z™zšz›zœzzžzŸz zªz«z¬z­z¯z°z±z²z³z´zµz¶z·z¸z¹zºz»z¼z½z¾z¿zÀzÁzÂzÃzÄzÅzÆz\
ÇzÈzÉzÊzËzÌzÍzÎzÏzÐzÑzÒzÓzÔzÕzÖz×zØzÙzÚzÛzÜzÝzÞzßzàzázâzãzäzåzæzçzèzézêzëzìzízîzïzðzñzòzózôzõzöz÷zøzùzúzûzüzýz{‚{ƒ{„{…{‡{\
ˆ{‰{Š{‹{Œ{{Ž{{{‘{’{“{”{•{–{—{˜{™{š{›{œ{{ž{Ÿ{ {ª{«{¬{­{®{¯{°{±{²{³{´{µ{¶{·{¸{¹{º{»{¼{½{¾{¿{À{Á{Â{Ã{Ä{Å{Æ{Ç{È{É{Ê{Ë{Ì{Í{\
Î{Ï{Ð{Ñ{Ò{Ó{Ô{Õ{Ö{×{Ø{Ù{Ú{Û{Ü{Ý{Þ{ß{à{á{â{ã{ä{å{æ{ç{è{é{ê{ë{ì{í{î{ï{ð{ñ{ò{ó{ô{õ{ö{÷{ø{ù{ú{û{ü{ý{|‚|ƒ|„|…|†|‡|ˆ|‰|Š|‹|Œ||\
||‘|’|“|”|•|–|—|˜|™|š|›|œ||ž|Ÿ| |ª|«|¬|­|®|¯|°|±|²|³|´|µ|¶|·|¸|¹|º|»|¼|½|¾|¿|À|Á|Â|Ã|Ä|Å|Æ|Ç|È|É|Ê|Ë|Ì|Í|Î|Ï|Ð|Ñ|Ò|Ó|Ô|\
Õ|Ö|×|Ø|Ù|Ú|Û|Ü|Ý|Þ|ß|à|â|ã|ä|å|æ|ç|è|é|ê|ë|ì|í|î|ï|ð|ñ|ò|ó|ô|õ|ö|÷|ø|ù|ú|û|ü|ý|}‚}ƒ}„}…}†}‡}ˆ}‰}Š}‹}Œ}}Ž}}}‘}’}“}”}•}\
–}—}˜}™}š}›}œ}}ž}Ÿ} }ª}«}¬}­}®}¯}°}±}²}³}´}µ}¶}·}¸}¹}º}»}¼}½}¾}¿}À}Á}Â}Ã}Ä}Å}Æ}Ç}È}É}Ê}Ë}Ì}Í}Î}Ï}Ð}Ñ}Ò}Ó}Ô}Õ}Ö}×}Ø}Ù}Ú}Û}\
Ü}Ý}Þ}ß}à}á}â}ã}ä}å}æ}ç}è}é}ê}ë}ì}í}î}ï}ð}ñ}ò}ó}ô}õ}ö}÷}ø}ù}ú}û}ü}ý}~‚~ƒ~„~…~†~‡~ˆ~‰~Š~‹~Œ~~Ž~~~‘~’~“~”~•~–~—~˜~™~š~›~\
œ~~ž~Ÿ~ ~ª~«~¬~­~®~¯~°~±~²~³~´~µ~¶~·~¸~¹~º~»~¼~½~¾~¿~À~Á~Â~Ã~Ä~Å~Æ~Ç~È~É~Ê~Ë~Ì~Í~Î~Ï~Ð~Ñ~Ò~Ó~Ô~Õ~Ö~×~Ø~Ù~Ú~Û~Ü~Ý~Þ~ß~à~á~\
â~ã~ä~å~æ~ç~è~é~ê~ë~ì~í~î~ï~ð~ñ~ò~ó~ô~õ~ö~÷~ø~ù~ú~û~ü~ý~€‚€ƒ€„€…€†€‡€ˆ€‰€Š€‹€Œ€€Ž€€€‘€’€“€”€•€–€—€˜€™€š€›€œ€€ž€Ÿ€ €ª€\
«€¬€­€®€¯€°€±€²€³€´€µ€¶€·€¸€¹€º€»€¼€½€¾€¿€À€Á€Â€Ã€Ä€Å€Æ€Ç€È€É€Ê€Ë€Ì€Í€Î€Ï€Ð€Ñ€Ò€Ó€Ô€Õ€Ö€×€Ø€Ù€Ú€Û€Ü€Ý€Þ€ß€à€á€â€ã€ä€å€æ€ç€\
è€é€ê€ë€ì€í€î€ï€ð€ñ€ò€ó€ô€õ€ö€÷€ø€ù€ú€û€ü€ý€‚ƒ„†‡ˆ‰Š‹ŒŽ‘’“”•–—˜™š›œžŸ ª«¬­®¯°±\
²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíî\
ïðñòóôõö÷øùúûüý‚‚‚ƒ‚„‚…‚†‚‡‚ˆ‚‰‚Š‚‹‚Œ‚‚Ž‚‚‚‘‚’‚“‚”‚•‚–‚—‚˜‚™‚š‚›‚œ‚‚ž‚Ÿ‚ ‚ª‚«‚¬‚­‚®‚¯‚°‚±‚²‚³‚´‚µ‚¶‚·‚\
¸‚¹‚º‚»‚¼‚½‚¾‚¿‚À‚Á‚Â‚Ã‚Ä‚Å‚Æ‚Ç‚È‚É‚Ê‚Ë‚Ì‚Í‚Î‚Ï‚Ð‚Ñ‚Ò‚Ó‚Ô‚Õ‚Ö‚×‚Ø‚Ù‚Ú‚Û‚Ü‚Ý‚Þ‚ß‚à‚á‚â‚ã‚ä‚å‚æ‚ç‚è‚é‚ê‚ë‚ì‚í‚î‚ï‚ð‚ñ‚ò‚ó‚ô‚\
õ‚ö‚÷‚ø‚ù‚ú‚û‚ü‚ý‚ƒ‚ƒƒƒ„ƒ…ƒ†ƒ‡ƒˆƒ‰ƒŠƒ‹ƒŒƒƒŽƒƒƒ‘ƒ’ƒ“ƒ”ƒ•ƒ–ƒ—ƒ˜ƒ™ƒšƒ›ƒœƒƒžƒŸƒ ƒªƒ«ƒ¬ƒ­ƒ®ƒ¯ƒ°ƒ±ƒ²ƒ³ƒ´ƒµƒ¶ƒ·ƒ¸ƒ¹ƒºƒ»ƒ¼ƒ½ƒ\
¾ƒ¿ƒÀƒÁƒÂƒÃƒÄƒÅƒÆƒÇƒÈƒÉƒÊƒËƒÌƒÍƒÎƒÏƒÐƒÑƒÒƒÓƒÔƒÕƒÖƒ×ƒØƒÙƒÚƒÛƒÜƒÝƒÞƒßƒàƒáƒâƒãƒäƒåƒæƒçƒèƒéƒêƒëƒìƒíƒîƒïƒðƒñƒòƒóƒôƒõƒöƒ÷ƒøƒùƒúƒ\
ûƒüƒýƒ„‚„ƒ„„„…„†„‡„ˆ„‰„Š„‹„Œ„„Ž„„„‘„’„“„”„•„–„—„˜„™„š„›„œ„„ž„Ÿ„ „ª„«„¬„­„®„¯„°„±„²„³„´„µ„¶„·„¸„¹„º„»„¼„½„¾„¿„À„Á„Â„Ã„\
Ä„Å„Æ„Ç„È„É„Ê„Ë„Ì„Í„Î„Ï„Ð„Ñ„Ò„Ó„Ô„Õ„Ö„×„Ø„Ù„Ú„Û„Ü„Ý„Þ„ß„à„â„ã„ä„å„æ„ç„è„é„ê„ë„ì„í„î„ï„ð„ñ„ò„ó„ô„õ„ö„÷„ø„ù„ú„û„ü„ý„…‚…ƒ…„…\
……†…‡…ˆ…‰…Š…‹…Œ……Ž………‘…’…“…”…•…–…—…˜…™…š…›…œ……ž…Ÿ… …ª…«…¬…­…®…¯…°…±…²…³…´…µ…¶…·…¸…¹…º…»…¼…½…¾…¿…À…Á…Â…Ã…Ä…Å…Æ…Ç…È…É…Ê…\
Ë…Ì…Í…Î…Ï…Ð…Ñ…Ò…Ó…Ô…Õ…Ö…×…Ø…Ù…Ú…Û…Ü…Ý…Þ…ß…à…á…â…ã…ä…å…æ…ç…è…é…ê…ë…ì…í…î…ï…ð…ñ…ò…ó…ô…õ…ö…÷…ø…ù…ú…û…ü…ý…†‚†ƒ†„†…†††‡†ˆ†‰†Š†\
‹†Œ††Ž†††‘†’†“†”†•†–†—†˜†™†š†›†œ††ž†Ÿ† †ª†«†¬†­†®†¯†°†±†²†³†´†µ†¶†·†¸†¹†º†»†¼†½†¾†¿†À†Á†Â†Ã†Ä†Å†Æ†Ç†È†É†Ê†Ë†Ì†Í†Î†Ï†Ð†\
Ñ†Ò†Ó†Ô†Õ†Ö†×†Ø†Ù†Ú†Û†Ü†Ý†Þ†ß†à†á†â†ã†ä†å†æ†ç†è†é†ê†ë†ì†í†î†ï†ð†ñ†ò†ó†ô†õ†ö†÷†ø†ù†ú†û†ü†ý†‡‚‡ƒ‡„‡…‡†‡‡‡ˆ‡‰‡Š‡‹‡Œ‡‡Ž‡‡‡\
‘‡’‡“‡”‡•‡–‡—‡˜‡™‡š‡›‡œ‡‡ž‡Ÿ‡ ‡ª‡«‡¬‡­‡®‡¯‡°‡±‡²‡³‡´‡µ‡¶‡·‡¸‡¹‡º‡»‡¼‡½‡¾‡¿‡À‡Á‡Â‡Ã‡Ä‡Å‡Æ‡Ç‡È‡É‡Ê‡Ë‡Ì‡Í‡Î‡Ï‡Ð‡Ñ‡Ò‡Ó‡Ô‡Õ‡Ö‡\
×‡Ø‡Ù‡Ú‡Û‡Ü‡Ý‡Þ‡ß‡à‡á‡â‡ã‡ä‡å‡æ‡ç‡è‡é‡ê‡ë‡ì‡í‡î‡ï‡ð‡ñ‡ò‡ó‡ô‡õ‡ö‡÷‡ø‡ù‡ú‡û‡ü‡ý‡ˆ‚ˆƒˆ„ˆ…ˆ†ˆ‡ˆˆˆ‰ˆŠˆ‹ˆŒˆˆŽˆˆˆ‘ˆ’ˆ“ˆ”ˆ•ˆ–ˆ\
—ˆ˜ˆ™ˆšˆ›ˆœˆˆžˆŸˆ ˆªˆ«ˆ¬ˆ­ˆ®ˆ¯ˆ°ˆ±ˆ²ˆ³ˆ´ˆµˆ¶ˆ·ˆ¸ˆ¹ˆºˆ»ˆ¼ˆ½ˆ¾ˆ¿ˆÀˆÁˆÂˆÃˆÄˆÅˆÆˆÇˆÈˆÉˆÊˆËˆÌˆÍˆÎˆÏˆÐˆÑˆÒˆÓˆÔˆÕˆÖˆ×ˆØˆÙˆÚˆÛˆÜˆ\
ÝˆÞˆßˆàˆáˆâˆãˆåˆæˆçˆèˆéˆêˆëˆìˆíˆîˆïˆðˆñˆòˆóˆôˆõˆöˆ÷ˆøˆùˆúˆûˆüˆýˆ‰‚‰ƒ‰„‰…‰†‰‡‰ˆ‰‰‰Š‰‹‰Œ‰‰Ž‰‰‰‘‰’‰“‰”‰•‰–‰—‰˜‰™‰š‰›‰œ‰‰\
ž‰Ÿ‰ ‰ª‰«‰¬‰­‰®‰¯‰°‰±‰²‰³‰´‰µ‰¶‰·‰¸‰¹‰º‰»‰¼‰½‰¾‰¿‰À‰Á‰Â‰Ã‰Ä‰Å‰Æ‰Ç‰È‰É‰Ê‰Ë‰Ì‰Í‰Î‰Ï‰Ð‰Ñ‰Ò‰Ó‰Ô‰Õ‰Ö‰×‰Ø‰Ù‰Ú‰Û‰Ü‰Ý‰Þ‰ß‰à‰á‰â‰ã‰\
ä‰å‰æ‰ç‰è‰é‰ê‰ë‰ì‰í‰î‰ï‰ð‰ñ‰ò‰ó‰ô‰õ‰ö‰÷‰ø‰ù‰ú‰û‰ü‰ý‰Š‚ŠƒŠ„Š…Š†Š‡ŠˆŠ‰ŠŠŠ‹ŠŒŠŠŽŠŠŠ‘Š’Š“Š”Š•Š–Š—Š˜Š™ŠšŠ›ŠœŠŠžŠŸŠ ŠªŠ«Š¬Š\
­Š®Š¯Š°Š±Š²Š³Š´ŠµŠ¶Š·Š¸Š¹ŠºŠ»Š¼Š½Š¾Š¿ŠÀŠÁŠÂŠÃŠÄŠÅŠÆŠÇŠÈŠÉŠÊŠËŠÌŠÍŠÎŠÏŠÐŠÑŠÒŠÓŠÔŠÕŠÖŠ×ŠØŠÙŠÚŠÛŠÜŠÝŠÞŠßŠàŠáŠâŠãŠäŠåŠæŠçŠèŠéŠ\
êŠëŠìŠíŠîŠïŠðŠñŠòŠóŠôŠõŠöŠ÷ŠøŠùŠúŠûŠüŠýŠ‹‚‹ƒ‹„‹…‹†‹‡‹ˆ‹‰‹Š‹‹‹Œ‹‹Ž‹‹‹‘‹’‹“‹”‹•‹–‹—‹˜‹™‹š‹›‹œ‹‹ž‹Ÿ‹ ‹ª‹«‹¬‹­‹®‹¯‹°‹±‹²‹\
³‹´‹µ‹¶‹·‹¸‹¹‹º‹»‹¼‹½‹¾‹¿‹À‹Á‹Â‹Ã‹Ä‹Å‹Æ‹Ç‹È‹É‹Ê‹Ë‹Ì‹Í‹Î‹Ï‹Ð‹Ñ‹Ò‹Ó‹Ô‹Õ‹Ö‹×‹Ø‹Ù‹Ú‹Û‹Ü‹Ý‹Þ‹ß‹à‹á‹â‹ã‹ä‹å‹æ‹ç‹è‹é‹ê‹ë‹ì‹í‹î‹ï‹\
ð‹ñ‹ò‹ó‹ô‹õ‹ö‹÷‹ø‹ù‹ú‹û‹ü‹ý‹Œ‚ŒƒŒ„Œ…Œ†Œ‡ŒˆŒ‰ŒŠŒ‹ŒŒŒŒŽŒŒŒ‘Œ’Œ“Œ”Œ•Œ–Œ—Œ˜Œ™ŒšŒ›ŒœŒŒžŒŸŒ ŒªŒ«Œ¬Œ­Œ®Œ¯Œ°Œ±Œ²Œ³Œ´ŒµŒ¶Œ·Œ¸Œ\
¹ŒºŒ»Œ¼Œ½Œ¾Œ¿ŒÀŒÁŒÂŒÃŒÅŒÆŒÇŒÈŒÉŒÊŒËŒÌŒÍŒÎŒÏŒÐŒÑŒÒŒÓŒÔŒÕŒÖŒ×ŒØŒÙŒÚŒÛŒÜŒÝŒÞŒßŒàŒáŒâŒãŒäŒåŒæŒçŒèŒéŒêŒëŒìŒíŒîŒïŒðŒñŒòŒóŒôŒõŒöŒ\
÷ŒøŒùŒúŒûŒüŒýŒ‚ƒ„…†‡ˆ‰Š‹ŒŽ‘’“”•–—˜™š›œžŸ ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿\
ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûü\
ýŽ‚ŽƒŽ„Ž…Ž†Ž‡ŽˆŽ‰ŽŠŽ‹ŽŒŽŽŽŽŽŽ‘Ž’Ž“Ž”Ž•Ž–Ž—Ž˜Ž™ŽšŽ›ŽœŽŽžŽŸŽ ŽªŽ«Ž¬Ž­Ž®Ž¯Ž°Ž±Ž²Ž³Ž´ŽµŽ¶Ž·Ž¸Ž¹ŽºŽ»Ž¼Ž½Ž¾Ž¿ŽÀŽÁŽÂŽÄŽÅŽÆŽ\
ÇŽÈŽÉŽÊŽËŽÌŽÍŽÎŽÏŽÐŽÑŽÒŽÓŽÔŽÕŽÖŽ×ŽØŽÙŽÚŽÛŽÜŽÝŽÞŽßŽàŽáŽâŽãŽäŽåŽæŽçŽèŽéŽêŽëŽìŽíŽîŽïŽðŽñŽòŽóŽôŽõŽöŽ÷ŽøŽùŽúŽûŽüŽýŽ‚ƒ„…†\
‡ˆ‰Š‹ŒŽ‘’“”•–—˜™š›œžŸ ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌ\
ÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüý‚ƒ„…†‡ˆ‰Š‹Œ\
Ž‘’“”•–—˜™š›œžŸ ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒ\
ÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüý‘‚‘ƒ‘„‘…‘†‘‡‘ˆ‘‰‘Š‘‹‘Œ‘‘Ž‘‘‘‘‘’‘\
“‘”‘•‘–‘—‘˜‘™‘š‘›‘œ‘‘ž‘Ÿ‘ ‘ª‘«‘¬‘­‘®‘¯‘°‘±‘²‘³‘´‘µ‘¶‘·‘¸‘¹‘º‘»‘¼‘½‘¾‘¿‘À‘Á‘Â‘Ã‘Ä‘Å‘Æ‘Ç‘È‘É‘Ê‘Ë‘Ì‘Í‘Î‘Ï‘Ð‘Ñ‘Ò‘Ó‘Ô‘Õ‘Ö‘×‘Ø‘\
Ù‘Ú‘Û‘Ü‘Ý‘Þ‘ß‘à‘á‘â‘ã‘ä‘å‘æ‘ç‘è‘é‘ê‘ë‘ì‘í‘î‘ï‘ð‘ñ‘ò‘ó‘ô‘õ‘ö‘÷‘ø‘ù‘ú‘û‘ü‘ý‘’‚’ƒ’„’…’†’‡’ˆ’‰’Š’‹’Œ’’Ž’’’‘’’’“’”’•’–’—’˜’\
™’š’›’œ’’ž’Ÿ’ ’ª’«’¬’­’®’¯’°’±’²’³’´’µ’¶’·’¸’¹’º’»’¼’½’¾’¿’À’Á’Â’Ã’Ä’Å’Æ’Ç’È’É’Ê’Ë’Ì’Í’Î’Ï’Ð’Ñ’Ò’Ó’Ô’Õ’Ö’×’Ø’Ù’Ú’Û’Ü’Ý’Þ’\
ß’à’á’â’ã’ä’å’æ’ç’è’é’ê’ë’ì’í’î’ï’ð’ñ’ò’ó’ô’õ’ö’÷’ø’ù’ú’û’ü’ý’“‚“ƒ“„“…“†“‡“ˆ“‰“Š“‹“Œ““Ž“““‘“’“““”“•“–“—“˜“™“š“›“œ““ž“\
Ÿ“ “ª“«“¬“­“®“¯“°“±“²“³“´“µ“¶“·“¸“¹“º“»“¼“½“¾“¿“À“Á“Â“Ã“Ä“Å“Æ“Ç“È“É“Ê“Ë“Ì“Í“Î“Ï“Ð“Ñ“Ò“Ó“Ô“Õ“Ö“×“Ø“Ù“Ú“Û“Ü“Ý“Þ“ß“à“á“â“ã“ä“\
å“æ“ç“è“é“ê“ë“ì“í“î“ï“ð“ñ“ò“ó“ô“õ“ö“÷“ø“ù“ú“û“ü“ý“”‚”ƒ”„”…”†”‡”ˆ”‰”Š”‹”Œ””Ž”””‘”’”“”””•”–”—”˜”™”š”›”œ””ž”Ÿ” ”ª”«”¬”­”\
®”¯”°”±”²”³”´”µ”¶”·”¸”¹”º”»”¼”½”¾”¿”À”Á”Â”Ã”Ä”Å”Æ”Ç”È”É”Ê”Ë”Ì”Í”Î”Ï”Ð”Ñ”Ò”Ó”Ô”Õ”Ö”×”Ø”Ù”Ú”Û”Ü”Ý”Þ”ß”à”á”â”ã”ä”å”æ”ç”è”é”ê”\
ë”ì”í”î”ï”ð”ñ”ò”ó”ô”õ”ö”÷”ø”ù”ú”û”ü”ý”•‚•ƒ•„•…•†•‡•ˆ•‰•Š•‹•Œ••Ž•••‘•’•“•”•••–•—•˜•™•š•›•œ••ž•Ÿ• •ª•«•¬•­•®•¯•°•±•²•³•\
´•µ•¶•·•¸•¹•º•»•¼•½•¾•¿•À•Á•Â•Ã•Ä•Å•Æ•Ç•È•É•Ê•Ë•Ì•Í•Î•Ï•Ð•Ñ•Ò•Ó•Ô•Õ•Ö•×•Ø•Ù•Ú•Û•Ü•Ý•Þ•ß•à•á•â•ã•ä•å•æ•ç•è•é•ê•ë•ì•í•î•ï•ð•\
ñ•ò•ó•ô•õ•ö•÷•ø•ù•ú•û•ü•ý•–‚–ƒ–„–…–†–‡–ˆ–‰–Š–‹–Œ––Ž–––‘–’–“–”–•–––—–˜–™–š–›–œ––ž–Ÿ– –ª–«–¬–­–®–¯–°–±–²–³–´–µ–¶–·–¸–¹–\
º–»–¼–½–¾–¿–À–Á–Â–Ã–Ä–Å–Æ–Ç–È–É–Ê–Ë–Ì–Í–Î–Ï–Ð–Ñ–Ò–Ó–Ô–Õ–Ö–×–Ù–Ú–Û–Ü–Ý–Þ–ß–à–á–â–ã–ä–å–æ–ç–è–é–ê–ë–ì–í–î–ï–ð–ñ–ò–ó–ô–õ–ö–÷–\
ø–ù–ú–û–ü–ý–—‚—ƒ—„—…—†—‡—ˆ—‰—Š—‹—Œ——Ž———‘—’—“—”—•—–———˜—™—š—›—œ——ž—Ÿ— —ª—«—¬—®—¯—°—±—²—³—´—µ—¶—·—¸—¹—º—»—¼—½—¾—¿—À—Á—\
Â—Ã—Ä—Å—Æ—Ç—È—É—Ê—Ë—Ì—Í—Î—Ï—Ð—Ñ—Ò—Ó—Ô—Õ—Ö—×—Ø—Ù—Ú—Û—Ü—Ý—Þ—ß—à—á—â—ã—ä—å—æ—ç—è—é—ê—ë—ì—í—î—ï—ð—ñ—ò—ó—ô—õ—ö—÷—ø—ù—ú—û—ü—ý—˜\
‚˜ƒ˜„˜…˜†˜‡˜ˆ˜‰˜Š˜‹˜Œ˜˜Ž˜˜˜‘˜’˜“˜”˜•˜–˜—˜˜˜™˜š˜›˜œ˜˜ž˜Ÿ˜ ˜ª˜«˜¬˜­˜®˜¯˜°˜±˜²˜³˜´˜µ˜¶˜·˜¸˜¹˜º˜»˜¼˜½˜¾˜¿˜À˜Á˜Â˜Ã˜Ä˜Å˜Æ˜Ç˜\
È˜É˜Ê˜Ë˜Ì˜Í˜Î˜Ï˜Ð˜Ñ˜Ò˜Ó˜Ô˜Õ˜Ö˜×˜Ø˜Ù˜Ú˜Û˜Ü˜Ý˜Þ˜ß˜à˜á˜â˜ã˜ä˜å˜æ˜ç˜è˜é˜ê˜ë˜ì˜í˜î˜ï˜ð˜ñ˜ò˜ó˜ô˜õ˜ö˜÷˜ø˜ù˜ú˜û˜ü˜ý˜™‚™ƒ™„™…™†™‡™\
ˆ™‰™Š™‹™Œ™™Ž™™™‘™’™“™”™•™–™—™˜™™™š™›™œ™™ž™Ÿ™ ™ª™«™¬™®™¯™°™±™²™³™´™µ™¶™·™¸™¹™º™»™¼™½™¾™¿™À™Á™Â™Ã™Ä™Å™Æ™Ç™È™É™Ê™Ë™Ì™Í™Î™\
Ï™Ð™Ñ™Ò™Ó™Ô™Õ™Ö™×™Ø™Ù™Ú™Û™Ü™Ý™Þ™ß™à™á™â™ã™ä™å™æ™ç™è™é™ê™ë™ì™í™î™ï™ð™ñ™ò™ó™ô™õ™ö™÷™ø™ù™ú™û™ü™ý™š‚šƒš„š…š†š‡šˆš‰šŠš‹šŒššŽš\
šš‘š’š“š”š•š–š—š˜š™ššš›šœššžšŸš šªš«š¬š­š®š¯š°š±š²š³š´šµš¶š·š¸š¹šºš»š¼š½š¾š¿šÀšÁšÂšÃšÄšÅšÆšÇšÈšÉšÊšËšÌšÍšÎšÏšÐšÑšÒšÓšÔš\
ÕšÖš×šØšÙšÚšÛšÜšÝšÞšßšàšášâšãšäšåšæšçšèšéšêšëšìšíšîšïšðšñšòšóšôšõšöš÷šøšùšúšûšüšýš›‚›ƒ›„›…›†›‡›ˆ›‰›Š›‹›Œ››Ž›››‘›’›“›”›"
#define TEXT_FONT_UNICODE_MODE2 "•›–›—›˜›™›š›››œ››ž›Ÿ› ›ª›«›¬›­›®›¯›°›±›²›³›´›µ›¶›·›¸›¹›º›»›¼›½›¾›¿›À›Á›Â›Ã›Ä›Å›Æ›Ç›È›É›Ê›Ë›Ì›Í›Î›Ï›Ð›Ñ›Ò›Ó›Ô›Õ›Ö›×›Ø›Ù›Ú›\
Û›Ü›Ý›Þ›ß›à›á›â›ã›ä›å›æ›ç›è›é›ê›ë›ì›í›î›ï›ð›ñ›ò›ó›ô›õ›ö›÷›ø›ù›ú›û›ü›ý›œ‚œƒœ„œ…œ†œ‡œˆœ‰œŠœ‹œŒœœŽœœœ‘œ’œ“œ”œ•œ–œ—œ˜œ™œšœ\
›œœœœžœŸœ œªœ«œ¬œ­œ®œ¯œ°œ²œ³œ´œµœ¶œ·œ¸œ¹œºœ»œ¼œ½œ¾œ¿œÀœÁœÂœÃœÄœÅœÆœÇœÈœÉœÊœËœÌœÍœÎœÏœÐœÑœÒœÓœÔœÕœÖœ×œØœÙœÚœÛœÜœÝœÞœßœàœáœ\
âœãœäœåœæœçœèœéœêœëœìœíœîœïœðœñœòœóœôœõœöœ÷œøœùœúœûœüœ‚ƒ„…†‡ˆ‰Š‹ŒŽ‘’“”•–—˜™š›œžŸ ª«\
¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçè\
éêëìíîïðñòóôõö÷øùúûüž‚žƒž„ž†ž‡žˆž‰žŠž‹žŒžžŽžžž‘ž’ž“ž”ž•ž–ž—ž˜ž™žšž›žœžžžžŸž žªž«ž¬ž®ž¯ž°ž±ž²ž³ž´ž\
µž¶ž·ž¸ž¹žºž»ž¼ž½ž¾ž¿žÀžÁžÂžÃžÄžÅžÆžÇžÈžÉžÊžËžÌžÍžÎžÏžÐžÑžÒžÓžÔžÕžÖž×žØžÙžÚžÛžÜžÝžÞžßžàžážâžãžäžåžæžçžèžéžêžëžìžížîžïžðžñž\
òžóžôžõžöž÷žøžùžúžûžüžŸ‚ŸƒŸ„Ÿ…Ÿ†Ÿ‡ŸˆŸ‰ŸŠŸ‹ŸŒŸŸŽŸŸŸ‘Ÿ’Ÿ“Ÿ”Ÿ•Ÿ–Ÿ—Ÿ˜Ÿ™ŸšŸ›ŸœŸŸžŸŸŸ ŸªŸ«Ÿ¬Ÿ®Ÿ¯Ÿ°Ÿ±Ÿ²Ÿ³Ÿ´ŸµŸ¶Ÿ·Ÿ¸Ÿ¹ŸºŸ»Ÿ¼Ÿ\
½Ÿ¾Ÿ¿ŸÀŸÁŸÂŸÃŸÄŸÅŸÆŸÇŸÈŸÉŸÊŸËŸÌŸÍŸÎŸÏŸÐŸÑŸÒŸÓŸÔŸÕŸÖŸ×ŸØŸÙŸÚŸÛŸÜŸÝŸÞŸßŸàŸáŸâŸãŸäŸåŸæŸçŸèŸéŸêŸëŸìŸíŸîŸïŸðŸñŸòŸóŸôŸõŸöŸ÷ŸøŸùŸ\
úŸûŸüŸ ‚ ƒ „ … † ‡ ˆ ‰ Š ‹ Œ  Ž   ‘ ’ “ ” • – — ˜ ™ š › œ  ž Ÿ   ª « ¬ ­ ® ¯ ° ± ² ³ ´ µ · ¸ ¹ º » ¼ ½ ¾ ¿ À Á Â Ã Ä \
Å Æ Ç È É Ê Ë Ì Í Î Ï Ð Ñ Ò Ó Ô Õ Ö × Ø Ù Ú Û Ü Ý Þ ß à á â ã ä å æ ç è é ê ë ì í î ï ð ñ ò ó ô õ ö ÷ ø ù ú û ü ¡‚¡ƒ¡„¡…¡\
†¡‡¡ˆ¡‰¡Š¡‹¡Œ¡¡Ž¡¡¡‘¡’¡“¡”¡•¡–¡—¡˜¡™¡š¡›¡œ¡¡ž¡Ÿ¡ ¡£¡°¡±¡²¡³¡´¡µ¡¶¡·¡¸¡¹¡º¡»¡¼¡½¡¾¡¿¡À¡Á¡Â¡Ã¡Ä¡Å¡Æ¡Ç¡È¡É¡Ê¡Ë¡Ì¡Í¡Î¡Ï¡Ð¡\
Ñ¡Ò¡Ó¡Ô¡Õ¡Ö¡×¡Ø¡Ù¡Ú¡Û¡Ü¡Ý¡Þ¡ß¡à¡á¡â¡ã¡ä¡å¡æ¡ç¡è¡é¡ê¡ë¡ì¡í¡î¡ï¡ð¡ñ¡ò¡ó¡ô¡õ¡ö¡÷¡¢‚¢ƒ¢„¢…¢†¢‡¢ˆ¢‰¢Š¢‹¢Œ¢¢Ž¢¢¢‘¢’¢“¢”¢•¢–¢\
—¢˜¢™¢š¢›¢œ¢¢ž¢Ÿ¢ ¢¡¢°¢±¢²¢³¢´¢µ¢¶¢·¢¸¢¹¢º¢»¢¼¢½¢¾¢¿¢À¢Á¢Â¢Ã¢Ä¢Å¢Æ¢Ç¢È¢É¢Ê¢Ë¢Ì¢Í¢Î¢Ï¢Ð¢Ñ¢Ò¢Ó¢Ô¢Õ¢Ö¢×¢Ø¢Ù¢Ú¢Û¢Ü¢Ý¢Þ¢ß¢à¢á¢\
â¢ã¢ä¢å¢æ¢ç¢è¢é¢ê¢ë¢ì¢í¢î¢ï¢ð¢ñ¢ò¢ó¢ô¢õ¢ö¢÷¢£‚£ƒ£„£…£†£‡£ˆ£‰£Š£‹£Œ££Ž£££‘£’£“£”£•£–£—£˜£™£š£›£œ££ž£Ÿ£ £¡£°£±£²£³£´£µ£\
¶£·£¸£¹£º£»£¼£½£¾£¿£À£Á£Â£Ã£Ä£Å£Æ£Ç£È£É£Ê£Ë£Ì£Í£Î£Ï£Ð£Ñ£Ò£Ó£Ô£Õ£Ö£×£Ø£Ù£Ú£Û£Ü£Ý£Þ£ß£à£á£â£ã£ä£å£æ£ç£è£é£ê£ë£ì£í£î£ï£ð£ñ£ò£\
ó£ô£õ£ö£÷£¤‚¤ƒ¤„¤…¤†¤‡¤ˆ¤‰¤Š¤‹¤Œ¤¤Ž¤¤¤‘¤’¤“¤”¤•¤–¤—¤˜¤™¤š¤›¤œ¤¤ž¤Ÿ¤ ¤¡¤£¤°¤±¤²¤³¤´¤µ¤¶¤·¤¸¤¹¤º¤»¤¼¤½¤¾¤¿¤À¤Á¤Â¤Ã¤Ä¤Å¤\
Æ¤Ç¤È¤É¤Ê¤Ë¤Ì¤Í¤Î¤Ï¤Ð¤Ñ¤Ò¤Ó¤Ô¤Õ¤Ö¤×¤Ø¤Ù¤Ú¤Û¤Ü¤Ý¤Þ¤ß¤à¤á¤â¤ã¤ä¤å¤æ¤ç¤è¤é¤ê¤ë¤ì¤í¤î¤ï¤ð¤ñ¤ò¤ó¤ô¤õ¤ö¤÷¤¥‚¥ƒ¥„¥…¥†¥‡¥ˆ¥‰¥Š¥‹¥\
Œ¥¥Ž¥¥¥‘¥’¥“¥”¥•¥–¥—¥˜¥™¥š¥›¥œ¥¥ž¥Ÿ¥ ¥°¥±¥²¥³¥´¥µ¥¶¥·¥¸¥¹¥º¥»¥¼¥½¥¾¥¿¥À¥Á¥Â¥Ã¥Ä¥Å¥Æ¥Ç¥È¥É¥Ê¥Ë¥Ì¥Í¥Î¥Ï¥Ð¥Ñ¥Ò¥Ó¥Ô¥Õ¥Ö¥×¥\
Ø¥Ù¥Ú¥Û¥Ü¥Ý¥Þ¥ß¥à¥á¥â¥ã¥ä¥å¥æ¥ç¥è¥é¥ê¥ë¥ì¥í¥î¥ï¥ð¥ñ¥ò¥ó¥ô¥õ¥ö¥÷¥¦‚¦ƒ¦„¦…¦†¦‡¦ˆ¦‰¦Š¦‹¦Œ¦¦Ž¦¦¦‘¦’¦“¦”¦•¦–¦—¦˜¦™¦š¦›¦œ¦¦\
ž¦Ÿ¦ ¦°¦±¦²¦³¦´¦µ¦¶¦·¦¸¦¹¦º¦»¦¼¦½¦¾¦¿¦À¦Á¦Â¦Ã¦Ä¦Å¦Æ¦Ç¦È¦É¦Ê¦Ë¦Ì¦Í¦Î¦Ï¦Ð¦Ñ¦Ò¦Ó¦Ô¦Õ¦Ö¦×¦Ø¦Ù¦Ú¦Û¦Ü¦Ý¦Þ¦ß¦à¦á¦â¦ã¦ä¦å¦æ¦ç¦è¦é¦\
ê¦ë¦ì¦í¦î¦ï¦ð¦ñ¦ò¦ó¦ô¦õ¦ö¦÷¦§‚§ƒ§„§…§†§‡§ˆ§‰§Š§‹§Œ§§Ž§§§‘§’§“§”§•§–§—§˜§™§š§›§œ§§ž§Ÿ§ §°§±§²§³§´§µ§¶§·§¸§¹§º§»§¼§½§¾§\
¿§À§Á§Â§Ã§Ä§Å§Æ§Ç§È§É§Ê§Ë§Ì§Í§Î§Ï§Ð§Ñ§Ò§Ó§Ô§Õ§Ö§×§Ø§Ù§Ú§Û§Ü§Ý§Þ§ß§à§á§â§ã§ä§å§æ§ç§è§é§ê§ë§ì§í§î§ï§ð§ñ§ò§ó§ô§õ§ö§÷§¨‚¨ƒ¨„¨\
…¨†¨‡¨ˆ¨‰¨Š¨‹¨Œ¨¨Ž¨¨¨‘¨’¨“¨”¨•¨–¨—¨˜¨™¨š¨›¨œ¨¨ž¨Ÿ¨ ¨£¨°¨±¨²¨³¨´¨µ¨¶¨·¨¸¨¹¨º¨»¨¼¨½¨¾¨¿¨À¨Á¨Â¨Ã¨Ä¨Å¨Æ¨Ç¨È¨É¨Ê¨Ë¨Ì¨Í¨Î¨Ï¨\
Ð¨Ñ¨Ò¨Ó¨Ô¨Õ¨Ö¨×¨Ø¨Ù¨Ú¨Û¨Ý¨Þ¨ß¨à¨á¨â¨ã¨ä¨å¨æ¨ç¨è¨é¨ê¨ë¨ì¨í¨î¨ï¨ð¨ñ¨ò¨ó¨ô¨õ¨ö¨÷¨©‚©ƒ©„©…©†©‡©ˆ©‰©Š©‹©Œ©©Ž©©©‘©’©“©”©•©–©\
—©˜©™©š©›©œ©©ž©Ÿ© ©£©°©±©²©³©´©µ©¶©·©¸©¹©º©»©¼©½©¾©¿©À©Á©Â©Ã©Ä©Å©Æ©Ç©È©É©Ê©Ë©Ì©Í©Î©Ï©Ð©Ñ©Ò©Ó©Ô©Õ©Ö©×©Ø©Ù©Ú©Û©Ü©Ý©Þ©ß©à©á©\
â©ã©ä©å©æ©ç©è©é©ê©ë©ì©í©î©ï©ð©ñ©ò©ó©ô©õ©ö©÷©ª‚ªƒª„ª…ª†ª‡ªˆª‰ªŠª‹ªŒªªŽªªª‘ª’ª“ª”ª•ª–ª—ª˜ª™ªšª›ªœªªžªŸª ª¡ª°ª±ª²ª³ª´ªµª\
¶ª·ª¸ª¹ªºª»ª¼ª½ª¾ª¿ªÀªÁªÂªÃªÄªÅªÆªÇªÈªÉªÊªËªÌªÍªÎªÏªÐªÑªÒªÓªÔªÕªÖª×ªØªÙªÚªÛªÜªÝªÞªßªàªáªâªãªäªåªæªçªèªéªêªëªìªíªîªïªðªñªòª\
óªôªõªöª÷ª«‚«ƒ«„«…«†«‡«ˆ«‰«Š«‹«Œ««Ž«««‘«’«“«”«•«–«—«˜«™«š«›«œ««ž«Ÿ« «°«±«²«³«´«µ«¶«·«¸«¹«º«»«¼«½«¾«¿«À«Á«Â«Ã«Ä«Å«Æ«Ç«\
È«É«Ê«Ë«Ì«Í«Î«Ï«Ð«Ñ«Ò«Ó«Ô«Õ«Ö«×«Ø«Ù«Ú«Û«Ü«Ý«Þ«ß«à«á«â«ã«ä«å«æ«ç«è«é«ê«ë«ì«í«î«ð«ñ«ò«ó«ô«õ«ö«÷«¬‚¬ƒ¬„¬…¬†¬‡¬ˆ¬‰¬Š¬‹¬Œ¬¬Ž¬\
¬¬‘¬’¬“¬”¬•¬–¬—¬˜¬™¬š¬›¬œ¬¬ž¬Ÿ¬ ¬£¬°¬±¬²¬³¬´¬µ¬¶¬·¬¸¬¹¬º¬»¬¼¬½¬¾¬¿¬À¬Á¬Â¬Ã¬Ä¬Å¬Æ¬Ç¬È¬É¬Ê¬Ë¬Ì¬Í¬Î¬Ï¬Ð¬Ñ¬Ò¬Ó¬Ô¬Õ¬Ö¬×¬Ø¬Ù¬\
Ú¬Û¬Ü¬Ý¬Þ¬ß¬à¬á¬â¬ã¬ä¬å¬æ¬ç¬è¬é¬ê¬ë¬ì¬í¬î¬ï¬ð¬ñ¬ò¬ó¬ô¬õ¬ö¬÷¬­‚­ƒ­„­…­†­‡­ˆ­‰­Š­‹­Œ­­Ž­­­‘­’­“­”­•­–­—­˜­™­š­›­œ­­ž­Ÿ­\
 ­¡­°­±­²­³­´­µ­¶­·­¸­¹­º­»­¼­½­¾­¿­À­Á­Â­Ã­Ä­Å­Æ­Ç­È­É­Ê­Ë­Ì­Í­Î­Ï­Ð­Ñ­Ò­Ó­Ô­Õ­Ö­×­Ø­Ù­Ú­Û­Ü­Ý­Þ­ß­à­á­â­ã­ä­å­æ­ç­è­é­ê­\
ë­ì­í­î­ï­ð­ñ­ò­ó­ô­õ­ö­÷­®‚®ƒ®„®…®†®‡®ˆ®‰®Š®‹®Œ®®Ž®®®‘®’®“®”®•®–®—®˜®™®š®›®œ®®ž®Ÿ® ®¡®°®±®²®³®´®µ®¶®·®¸®¹®º®»®¼®½®¾®\
¿®À®Á®Â®Ã®Ä®Å®Æ®Ç®È®É®Ê®Ë®Ì®Í®Î®Ï®Ð®Ñ®Ò®Ó®Ô®Õ®Ö®×®Ø®Ù®Ú®Û®Ü®Ý®Þ®ß®à®á®â®ã®ä®å®æ®ç®è®é®ê®ë®ì®í®î®ï®ð®ñ®ò®ó®ô®õ®ö®÷®¯‚¯ƒ¯„¯\
…¯‡¯ˆ¯‰¯Š¯‹¯Œ¯¯Ž¯¯¯‘¯’¯“¯”¯•¯–¯—¯˜¯™¯š¯›¯œ¯¯ž¯Ÿ¯ ¯¡¯°¯±¯²¯³¯´¯µ¯¶¯·¯¸¯¹¯º¯»¯¼¯½¯¾¯¿¯À¯Á¯Â¯Ã¯Ä¯Å¯Æ¯Ç¯È¯É¯Ê¯Ë¯Ì¯Í¯Î¯Ï¯Ð¯\
Ñ¯Ò¯Ó¯Ô¯Õ¯Ö¯×¯Ø¯Ù¯Ú¯Û¯Ü¯Ý¯Þ¯ß¯à¯á¯â¯ã¯ä¯å¯æ¯ç¯è¯é¯ê¯ë¯ì¯í¯î¯ï¯ð¯ñ¯ò¯ó¯ô¯õ¯ö¯÷¯°‚°ƒ°„°…°†°‡°ˆ°‰°Š°‹°Œ°°Ž°°°‘°’°“°”°•°–°\
—°˜°™°š°›°œ°°ž°Ÿ° °¡°°°±°²°³°´°µ°¶°·°¸°¹°º°»°¼°½°¾°¿°À°Á°Â°Ã°Ä°Å°Æ°Ç°È°É°Ê°Ë°Ì°Í°Î°Ï°Ð°Ñ°Ò°Ó°Ô°Õ°Ö°×°Ø°Ù°Ú°Û°Ü°Ý°Þ°ß°à°á°\
â°ã°ä°å°æ°ç°è°é°ê°ë°ì°í°î°ï°ð°ñ°ò°ó°ô°õ°ö°÷°±‚±ƒ±„±…±†±‡±ˆ±‰±Š±‹±Œ±±Ž±±±‘±’±“±”±•±–±—±˜±™±š±›±œ±±ž±Ÿ± ±¡±°±±±²±³±´±µ±\
¶±·±¸±¹±º±»±¼±½±¾±¿±À±Á±Â±Ã±Ä±Å±Æ±Ç±È±É±Ê±Ë±Ì±Í±Î±Ï±Ð±Ñ±Ò±Ó±Ô±Õ±Ö±×±Ø±Ù±Ú±Û±Ü±Ý±Þ±ß±à±á±â±ã±ä±å±æ±ç±è±é±ê±ë±ì±í±î±ï±ð±ñ±ò±\
ó±ô±õ±ö±÷±²‚²ƒ²„²…²†²‡²ˆ²‰²Š²‹²Œ²²Ž²²²‘²’²“²”²•²–²—²˜²™²š²›²œ²²ž²Ÿ² ²°²±²²²³²´²µ²¶²·²¸²¹²º²»²¼²½²¾²¿²À²Á²Â²Ã²Ä²Å²Æ²Ç²\
È²É²Ê²Ë²Ì²Í²Î²Ï²Ð²Ñ²Ò²Ó²Ô²Õ²Ö²×²Ø²Ù²Ú²Û²Ü²Ý²Þ²ß²à²á²â²ã²ä²å²æ²ç²è²é²ê²ë²ì²í²î²ï²ð²ñ²ò²ó²ô²õ²ö²÷²³‚³ƒ³„³…³†³‡³ˆ³‰³Š³‹³Œ³³\
Ž³³³‘³’³“³”³•³–³—³˜³™³š³›³œ³³ž³Ÿ³ ³°³±³²³³³´³µ³¶³·³¸³¹³º³»³¼³½³¾³¿³À³Á³Â³Ã³Ä³Å³Æ³Ç³È³É³Ê³Ë³Ì³Í³Î³Ï³Ð³Ñ³Ò³Ó³Ô³Õ³Ö³×³Ø³Ù³\
Ú³Û³Ü³Ý³Þ³ß³à³á³â³ã³ä³å³æ³ç³è³é³ê³ë³ì³í³î³ï³ð³ñ³ò³ó³ô³õ³ö³÷³´‚´ƒ´„´…´†´‡´ˆ´‰´Š´‹´Œ´´Ž´´´‘´’´“´”´•´–´—´˜´™´š´›´œ´´ž´Ÿ´\
 ´°´±´²´³´´´µ´¶´·´¸´¹´º´»´¼´½´¾´¿´À´Á´Â´Ã´Ä´Å´Æ´Ç´È´É´Ê´Ë´Ì´Í´Î´Ï´Ð´Ñ´Ò´Ó´Ô´Õ´Ö´×´Ø´Ù´Ú´Û´Ü´Ý´Þ´ß´à´á´â´ã´ä´å´æ´ç´è´é´ê´ë´\
ì´í´î´ï´ð´ñ´ò´ó´ô´õ´ö´÷´µ‚µƒµ„µ…µ†µ‡µˆµ‰µŠµ‹µŒµµŽµµµ‘µ’µ“µ”µ•µ–µ—µ˜µ™µšµ›µœµµžµŸµ µ°µ±µ²µ³µ´µµµ¶µ·µ¸µ¹µºµ»µ¼µ½µ¾µ¿µÀµ\
ÁµÂµÃµÄµÅµÆµÇµÈµÉµÊµËµÌµÍµÎµÏµÐµÑµÒµÓµÔµÕµÖµ×µØµÙµÚµÛµÜµÝµÞµßµàµáµâµãµäµåµæµçµèµéµêµëµìµíµîµïµðµñµòµóµôµõµöµ÷µ¶‚¶ƒ¶„¶†¶‡¶\
ˆ¶‰¶Š¶‹¶Œ¶¶Ž¶¶¶‘¶’¶“¶”¶•¶–¶—¶˜¶™¶š¶›¶œ¶¶ž¶Ÿ¶ ¶¡¶°¶±¶²¶³¶´¶µ¶¶¶·¶¸¶¹¶º¶»¶¼¶½¶¾¶¿¶À¶Á¶Â¶Ã¶Ä¶Å¶Æ¶Ç¶È¶É¶Ê¶Ë¶Ì¶Í¶Î¶Ï¶Ð¶Ñ¶Ò¶\
Ó¶Ô¶Õ¶Ö¶×¶Ø¶Ù¶Ú¶Û¶Ü¶Ý¶Þ¶ß¶à¶á¶â¶ä¶å¶æ¶ç¶è¶é¶ê¶ë¶ì¶í¶î¶ï¶ð¶ñ¶ò¶ó¶ô¶õ¶ö¶÷¶·‚·ƒ·„·…·†·‡·ˆ·‰·Š·‹·Œ··Ž···‘·’·“·”·•·–·—·˜·™·\
š·›·œ··ž·Ÿ· ·¡·°·±·²·³·´·µ·¶···¸·¹·º·»·¼·½·¾·¿·À·Á·Â·Ã·Ä·Å·Æ·Ç·È·É·Ê·Ë·Ì·Í·Î·Ï·Ð·Ñ·Ò·Ó·Ô·Õ·Ö·×·Ø·Ù·Ú·Û·Ü·Ý·Þ·ß·à·á·â·ã·ä·\
å·æ·ç·è·é·ê·ë·ì·í·î·ï·ð·ñ·ò·ó·ô·õ·ö·÷·¸‚¸ƒ¸„¸…¸†¸‡¸ˆ¸‰¸Š¸‹¸Œ¸¸Ž¸¸¸‘¸’¸“¸”¸•¸–¸—¸˜¸™¸š¸›¸œ¸¸ž¸Ÿ¸ ¸°¸±¸²¸³¸´¸µ¸¶¸·¸¸¸¹¸\
º¸»¸¼¸½¸¾¸¿¸À¸Á¸Â¸Ã¸Ä¸Å¸Æ¸Ç¸È¸É¸Ê¸Ë¸Ì¸Í¸Î¸Ï¸Ð¸Ñ¸Ò¸Ó¸Ô¸Õ¸Ö¸×¸Ø¸Ù¸Ú¸Û¸Ü¸Ý¸Þ¸ß¸à¸á¸â¸ã¸ä¸å¸æ¸ç¸è¸é¸ê¸ë¸ì¸í¸î¸ï¸ð¸ñ¸ò¸ó¸ô¸õ¸ö¸\
÷¸¹‚¹ƒ¹„¹…¹†¹‡¹ˆ¹‰¹Š¹‹¹Œ¹¹Ž¹¹¹‘¹’¹“¹”¹•¹–¹—¹˜¹™¹š¹›¹œ¹¹ž¹Ÿ¹ ¹°¹±¹²¹³¹´¹µ¹¶¹·¹¸¹¹¹º¹»¹¼¹½¹¾¹¿¹À¹Á¹Â¹Ã¹Ä¹Å¹Æ¹Ç¹È¹É¹Ê¹Ë¹\
Ì¹Í¹Î¹Ï¹Ð¹Ñ¹Ò¹Ó¹Ô¹Õ¹Ö¹×¹Ø¹Ù¹Ú¹Û¹Ü¹Ý¹Þ¹ß¹à¹á¹â¹ã¹ä¹å¹æ¹ç¹è¹é¹ê¹ë¹ì¹í¹î¹ï¹ð¹ñ¹ò¹ó¹ô¹õ¹ö¹÷¹º‚ºƒº„º…º†º‡ºˆº‰ºŠº‹ºŒººŽººº‘º\
’º“º”º•º–º—º˜º™ºšº›ºœººžºŸº º£º°º±º²º³º´ºµº¶º·º¸º¹ººº»º¼º½º¾º¿ºÀºÁºÂºÃºÄºÅºÆºÇºÈºÉºÊºËºÌºÍºÎºÏºÐºÑºÒºÓºÔºÕºÖº×ºØºÙºÚºÛºÜº\
ÝºÞºßºàºáºâºãºäºåºæºçºèºéºêºëºìºíºîºïºðºñºòºóºôºõºöº÷º»‚»ƒ»„»…»†»‡»ˆ»‰»Š»‹»Œ»»Ž»»»‘»’»“»”»•»–»—»˜»™»š»›»œ»»ž»Ÿ» »£»°»\
±»²»³»´»µ»¶»·»¸»¹»º»»»¼»½»¾»¿»À»Á»Â»Ã»Ä»Å»Æ»Ç»È»É»Ê»Ë»Ì»Í»Î»Ï»Ð»Ñ»Ò»Ó»Ô»Õ»Ö»×»Ø»Ù»Ú»Û»Ü»Ý»Þ»ß»à»á»â»ã»ä»å»æ»ç»è»é»ê»ë»ì»í»\
î»ï»ð»ñ»ò»ó»ô»õ»ö»÷»¼‚¼ƒ¼„¼…¼†¼‡¼ˆ¼‰¼Š¼‹¼Œ¼¼Ž¼¼¼‘¼’¼“¼”¼•¼–¼—¼˜¼™¼š¼›¼œ¼¼ž¼Ÿ¼ ¼°¼±¼²¼³¼´¼µ¼¶¼·¼¸¼¹¼º¼»¼¼¼½¼¾¼¿¼À¼Á¼Â¼\
Ã¼Ä¼Å¼Æ¼Ç¼È¼É¼Ê¼Ë¼Ì¼Í¼Î¼Ï¼Ð¼Ñ¼Ò¼Ó¼Ô¼Õ¼Ö¼×¼Ø¼Ù¼Ú¼Û¼Ü¼Ý¼Þ¼ß¼à¼á¼â¼ã¼ä¼å¼æ¼ç¼è¼é¼ê¼ë¼ì¼í¼î¼ï¼ð¼ñ¼ò¼ó¼ô¼õ¼ö¼÷¼½‚½ƒ½„½…½†½‡½ˆ½\
‰½Š½‹½Œ½½Ž½½½‘½’½“½”½•½–½—½˜½™½š½›½œ½½ž½Ÿ½ ½°½±½²½³½´½µ½¶½·½¸½¹½º½»½¼½½½¾½¿½À½Á½Â½Ã½Ä½Å½Æ½Ç½È½É½Ê½Ë½Ì½Í½Î½Ï½Ð½Ñ½Ò½Ó½Ô½\
Õ½Ö½×½Ø½Ù½Ú½Û½Ü½Ý½Þ½ß½à½á½â½ã½ä½å½æ½ç½è½é½ê½ë½ì½í½î½ï½ð½ñ½ò½ó½ô½õ½ö½÷½¾‚¾„¾…¾†¾‡¾ˆ¾‰¾Š¾‹¾Œ¾¾Ž¾¾¾‘¾’¾“¾”¾•¾–¾—¾˜¾™¾š¾›¾\
œ¾¾ž¾Ÿ¾ ¾¡¾°¾±¾²¾³¾´¾µ¾¶¾·¾¸¾¹¾º¾»¾¼¾½¾¾¾¿¾À¾Á¾Â¾Ã¾Ä¾Å¾Æ¾Ç¾È¾É¾Ê¾Ë¾Ì¾Í¾Î¾Ï¾Ð¾Ñ¾Ò¾Ó¾Ô¾Õ¾Ö¾×¾Ø¾Ù¾Ú¾Û¾Ü¾Ý¾Þ¾ß¾à¾á¾â¾ã¾ä¾å¾æ¾\
ç¾è¾é¾ê¾ë¾ì¾í¾î¾ï¾ð¾ñ¾ò¾ó¾ô¾õ¾ö¾÷¾¿‚¿„¿…¿†¿‡¿ˆ¿‰¿Š¿‹¿Œ¿¿Ž¿¿¿‘¿’¿“¿”¿•¿–¿—¿˜¿™¿š¿›¿œ¿¿ž¿Ÿ¿ ¿¡¿£¿°¿±¿²¿³¿´¿µ¿¶¿·¿¸¿¹¿º¿\
»¿¼¿½¿¾¿¿¿À¿Á¿Â¿Ã¿Ä¿Å¿Æ¿Ç¿È¿É¿Ê¿Ë¿Ì¿Í¿Î¿Ï¿Ð¿Ñ¿Ò¿Ó¿Ô¿Õ¿Ö¿×¿Ø¿Ù¿Ú¿Û¿Ü¿Ý¿Þ¿ß¿à¿á¿â¿ã¿ä¿å¿æ¿ç¿è¿é¿ê¿ë¿ì¿í¿î¿ï¿ð¿ñ¿ò¿ó¿ô¿õ¿ö¿÷¿\
À‚À„À…À†À‡ÀˆÀ‰ÀŠÀ‹ÀŒÀÀŽÀÀÀ‘À’À“À”À•À–À—À˜À™ÀšÀ›ÀœÀÀžÀŸÀ À°À±À²À³À´ÀµÀ¶À·À¸À¹ÀºÀ»À¼À½À¾À¿ÀÀÀÁÀÂÀÃÀÄÀÅÀÆÀÇÀÈÀÉÀÊÀËÀÌÀÍÀ\
ÎÀÏÀÐÀÑÀÒÀÓÀÔÀÕÀÖÀ×ÀØÀÙÀÚÀÛÀÜÀÝÀÞÀßÀàÀáÀâÀãÀäÀåÀæÀçÀèÀéÀêÀëÀìÀíÀîÀïÀðÀñÀòÀóÀôÀõÀöÀ÷ÀÁ‚Á„Á…Á†Á‡ÁˆÁ‰ÁŠÁ‹ÁŒÁÁŽÁÁÁ‘Á’Á“Á”Á\
•Á–Á—Á˜Á™ÁšÁ›ÁœÁÁžÁŸÁ Á°Á±Á²Á³Á´ÁµÁ¶Á·Á¸Á¹ÁºÁ»Á¼Á½Á¾Á¿ÁÀÁÁÁÂÁÃÁÄÁÅÁÆÁÇÁÈÁÉÁÊÁËÁÌÁÍÁÎÁÏÁÐÁÑÁÒÁÓÁÔÁÕÁÖÁ×ÁØÁÙÁÚÁÛÁÜÁÝÁÞÁßÁàÁ\
áÁâÁãÁäÁåÁæÁçÁèÁéÁêÁëÁìÁíÁîÁïÁðÁñÁòÁóÁôÁõÁöÁ÷ÁÂ‚ÂƒÂ„Â…Â†Â‡ÂˆÂ‰ÂŠÂ‹ÂŒÂÂŽÂÂÂ‘Â’Â“Â”Â•Â–Â—Â˜Â™ÂšÂ›ÂœÂÂžÂŸÂ Â°Â±Â²Â³Â´ÂµÂ\
¶Â·Â¸Â¹ÂºÂ»Â¼Â½Â¾Â¿ÂÀÂÁÂÂÂÃÂÄÂÅÂÆÂÇÂÈÂÉÂÊÂËÂÌÂÍÂÎÂÏÂÐÂÑÂÒÂÓÂÔÂÕÂÖÂ×ÂØÂÙÂÚÂÛÂÜÂÝÂÞÂßÂàÂáÂâÂãÂäÂåÂæÂçÂèÂéÂêÂëÂìÂíÂîÂïÂðÂñÂòÂ\
óÂôÂõÂöÂ÷ÂÃ‚ÃƒÃ„Ã…Ã†Ã‡ÃˆÃ‰ÃŠÃ‹ÃŒÃÃŽÃÃÃ‘Ã’Ã“Ã”Ã•Ã–Ã—Ã˜Ã™ÃšÃ›ÃœÃÃžÃŸÃ Ã°Ã±Ã²Ã³Ã´ÃµÃ¶Ã·Ã¸Ã¹ÃºÃ»Ã¼Ã½Ã¾Ã¿ÃÀÃÁÃÂÃÃÃÄÃÅÃÆÃÇÃ\
ÈÃÉÃÊÃËÃÌÃÍÃÎÃÏÃÐÃÑÃÒÃÓÃÔÃÕÃÖÃ×ÃØÃÙÃÚÃÛÃÜÃÝÃÞÃßÃàÃáÃâÃãÃäÃåÃæÃçÃèÃéÃêÃëÃìÃíÃîÃïÃðÃñÃòÃóÃôÃõÃöÃ÷ÃÄ‚Ä„Ä…Ä†Ä‡ÄˆÄ‰ÄŠÄ‹ÄŒÄÄŽÄ\
ÄÄ‘Ä’Ä“Ä”Ä•Ä–Ä—Ä˜Ä™ÄšÄ›ÄœÄÄžÄŸÄ Ä°Ä±Ä²Ä³Ä´ÄµÄ¶Ä·Ä¸Ä¹ÄºÄ»Ä¼Ä½Ä¾Ä¿ÄÀÄÁÄÂÄÃÄÄÄÅÄÆÄÇÄÈÄÉÄÊÄËÄÌÄÍÄÎÄÏÄÐÄÑÄÒÄÓÄÔÄÕÄÖÄ×ÄØÄÙÄÚÄ\
ÛÄÜÄÝÄÞÄßÄàÄáÄâÄãÄäÄåÄæÄçÄèÄéÄêÄëÄìÄíÄîÄïÄðÄñÄòÄóÄôÄõÄöÄ÷ÄÅ‚Å„Å…Å†Å‡ÅˆÅ‰ÅŠÅ‹ÅŒÅÅŽÅÅÅ‘Å’Å“Å”Å•Å–Å—Å˜Å™ÅšÅ›ÅœÅÅžÅŸÅ Å°Å\
±Å²Å³Å´ÅµÅ¶Å·Å¸Å¹ÅºÅ»Å¼Å½Å¾Å¿ÅÀÅÁÅÂÅÃÅÄÅÅÅÆÅÇÅÈÅÉÅÊÅËÅÌÅÍÅÎÅÏÅÐÅÑÅÒÅÓÅÔÅÕÅÖÅ×ÅØÅÙÅÚÅÛÅÜÅÝÅÞÅßÅàÅáÅâÅãÅäÅåÅæÅçÅèÅéÅêÅëÅìÅíÅ\
îÅïÅðÅñÅòÅóÅôÅõÅöÅ÷ÅÆ‚ÆƒÆ„Æ…Æ†Æ‡ÆˆÆ‰ÆŠÆ‹ÆŒÆÆŽÆÆÆ‘Æ’Æ“Æ”Æ•Æ–Æ—Æ˜Æ™ÆšÆ›ÆœÆÆžÆŸÆ Æ°Æ±Æ²Æ³Æ´ÆµÆ¶Æ·Æ¸Æ¹ÆºÆ»Æ¼Æ½Æ¾Æ¿ÆÀÆÁÆÂÆ\
ÃÆÄÆÅÆÆÆÇÆÈÆÉÆÊÆËÆÌÆÍÆÎÆÏÆÐÆÑÆÒÆÓÆÔÆÕÆÖÆ×ÆØÆÙÆÚÆÛÆÜÆÝÆÞÆßÆàÆáÆâÆãÆäÆåÆæÆçÆèÆéÆêÆëÆìÆíÆîÆïÆðÆñÆòÆóÆôÆõÆöÆ÷ÆÇ‚ÇƒÇ„Ç…Ç†Ç‡ÇˆÇ\
‰ÇŠÇ‹ÇŒÇÇŽÇÇÇ‘Ç’Ç“Ç”Ç•Ç–Ç—Ç˜Ç™ÇšÇ›ÇœÇÇžÇŸÇ Ç°Ç±Ç²Ç³Ç´ÇµÇ¶Ç·Ç¸Ç¹ÇºÇ»Ç¼Ç½Ç¾Ç¿ÇÀÇÁÇÂÇÃÇÄÇÅÇÆÇÇÇÈÇÉÇÊÇËÇÌÇÍÇÎÇÏÇÐÇÑÇÒÇÓÇÔÇ\
ÕÇÖÇ×ÇØÇÙÇÚÇÛÇÜÇÝÇÞÇßÇàÇáÇâÇãÇäÇåÇæÇçÇèÇéÇêÇëÇìÇíÇîÇïÇðÇñÇòÇóÇôÇõÇöÇ÷ÇÈ‚ÈƒÈ„È…È†È‡ÈˆÈ‰ÈŠÈ‹ÈŒÈÈŽÈÈÈ‘È’È“È”È•È–È—È˜È™ÈšÈ\
›ÈœÈÈžÈŸÈ È°È±È²È³È´ÈµÈ¶È·È¸È¹ÈºÈ»È¼È½È¾È¿ÈÀÈÁÈÂÈÃÈÄÈÅÈÆÈÇÈÈÈÉÈÊÈËÈÌÈÍÈÎÈÏÈÐÈÑÈÒÈÓÈÔÈÕÈÖÈ×ÈØÈÙÈÚÈÛÈÜÈÝÈÞÈßÈàÈáÈâÈãÈäÈåÈæÈ\
çÈèÈéÈêÈëÈìÈíÈîÈïÈðÈñÈòÈóÈôÈõÈöÈ÷ÈÉ‚ÉƒÉ„É…É†É‡ÉˆÉ‰ÉŠÉ‹ÉŒÉÉŽÉÉÉ‘É’É“É”É•É–É—É˜É™ÉšÉ›ÉœÉÉžÉŸÉ É°É±É²É³É´ÉµÉ¶É·É¸É¹ÉºÉ»É\
¼É½É¾É¿ÉÀÉÁÉÂÉÃÉÄÉÅÉÆÉÇÉÈÉÉÉÊÉËÉÌÉÍÉÎÉÏÉÐÉÑÉÒÉÓÉÔÉÕÉÖÉ×ÉØÉÙÉÚÉÛÉÜÉÝÉÞÉßÉàÉáÉâÉãÉäÉåÉæÉçÉèÉéÉêÉëÉìÉíÉîÉïÉðÉñÉòÉóÉôÉõÉöÉ÷ÉÊ\
‚ÊƒÊ„Ê…Ê†Ê‡ÊˆÊ‰ÊŠÊ‹ÊŒÊÊŽÊÊÊ‘Ê’Ê“Ê”Ê•Ê–Ê—Ê˜Ê™ÊšÊ›ÊœÊÊžÊŸÊ Ê°Ê±Ê²Ê³Ê´ÊµÊ¶Ê·Ê¸Ê¹ÊºÊ»Ê¼Ê½Ê¾Ê¿ÊÀÊÁÊÂÊÃÊÄÊÅÊÆÊÇÊÈÊÉÊÊÊËÊÌÊÍÊ\
ÎÊÏÊÐÊÑÊÒÊÓÊÔÊÕÊÖÊ×ÊØÊÙÊÚÊÛÊÜÊÝÊÞÊßÊàÊáÊâÊãÊäÊåÊæÊçÊèÊéÊêÊëÊìÊíÊîÊïÊðÊñÊòÊóÊôÊõÊöÊ÷ÊË‚ËƒË„Ë…Ë†Ë‡ËˆË‰ËŠË‹ËŒËËŽËËË‘Ë’Ë“Ë\
”Ë•Ë–Ë—Ë˜Ë™ËšË›ËœËËžËŸË Ë°Ë±Ë²Ë³Ë´ËµË¶Ë·Ë¸Ë¹ËºË»Ë¼Ë½Ë¾Ë¿ËÀËÁËÂËÃËÄËÅËÆËÇËÈËÉËÊËËËÌËÍËÎËÏËÐËÑËÒËÓËÔËÕËÖË×ËØËÙËÚËÛËÜËÝËÞËßË\
àËáËâËãËäËåËæËçËèËéËêËëËìËíËîËïËðËñËòËóËôËõËöË÷ËÌ‚ÌƒÌ„Ì…Ì†Ì‡ÌˆÌ‰ÌŠÌ‹ÌŒÌÌŽÌÌÌ‘Ì’Ì“Ì”Ì•Ì–Ì—Ì˜Ì™ÌšÌ›ÌœÌÌžÌŸÌ Ì°Ì±Ì²Ì³Ì´Ì\
µÌ¶Ì·Ì¸Ì¹ÌºÌ»Ì¼Ì½Ì¾Ì¿ÌÀÌÁÌÂÌÃÌÄÌÅÌÆÌÇÌÈÌÉÌÊÌËÌÌÌÍÌÎÌÏÌÐÌÑÌÒÌÓÌÔÌÕÌÖÌ×ÌØÌÙÌÚÌÛÌÜÌÝÌÞÌßÌàÌáÌâÌãÌäÌåÌæÌçÌèÌéÌêÌëÌìÌíÌîÌïÌðÌñÌ"
#define TEXT_FONT_UNICODE_MODE3 "òÌóÌôÌõÌöÌ÷ÌÍ‚ÍƒÍ„Í…Í†Í‡ÍˆÍ‰ÍŠÍ‹ÍŒÍÍŽÍÍÍ‘Í’Í“Í”Í•Í–Í—Í˜Í™ÍšÍ›ÍœÍÍžÍŸÍ Í°Í±Í²Í³Í´ÍµÍ¶Í·Í¸Í¹ÍºÍ»Í¼Í½Í¾Í¿ÍÀÍÁÍÂÍÃÍÄÍÅÍÆÍ\
ÇÍÈÍÉÍÊÍËÍÌÍÍÍÎÍÏÍÐÍÑÍÒÍÓÍÔÍÕÍÖÍ×ÍØÍÙÍÚÍÛÍÜÍÝÍÞÍßÍàÍáÍâÍãÍäÍåÍæÍçÍèÍéÍêÍëÍìÍíÍîÍïÍðÍñÍòÍóÍôÍõÍöÍ÷ÍÎ‚ÎƒÎ„Î…Î†Î‡ÎˆÎ‰ÎŠÎ‹ÎŒÎ\
ÎŽÎÎÎ‘Î’Î“Î”Î•Î–Î—Î˜Î™ÎšÎ›ÎœÎÎžÎŸÎ Î°Î±Î²Î³Î´ÎµÎ¶Î·Î¸Î¹ÎºÎ»Î¼Î½Î¾Î¿ÎÀÎÁÎÂÎÃÎÄÎÅÎÆÎÇÎÈÎÉÎÊÎËÎÌÎÍÎÎÎÏÎÐÎÑÎÒÎÓÎÔÎÕÎÖÎ×ÎØÎ\
ÙÎÚÎÛÎÜÎÝÎÞÎßÎàÎáÎâÎãÎäÎåÎæÎçÎèÎéÎêÎëÎìÎíÎîÎïÎðÎñÎòÎóÎôÎõÎöÎ÷ÎÏ‚ÏƒÏ„Ï…Ï†Ï‡ÏˆÏ‰ÏŠÏ‹ÏŒÏÏŽÏÏÏ‘Ï’Ï“Ï”Ï•Ï–Ï—Ï˜Ï™ÏšÏ›ÏœÏÏžÏ\
ŸÏ Ï°Ï±Ï²Ï³Ï´ÏµÏ¶Ï·Ï¸Ï¹ÏºÏ»Ï¼Ï½Ï¾Ï¿ÏÀÏÁÏÂÏÃÏÄÏÅÏÆÏÇÏÈÏÉÏÊÏËÏÌÏÍÏÎÏÏÏÐÏÑÏÒÏÓÏÔÏÕÏÖÏ×ÏØÏÙÏÚÏÛÏÜÏÝÏÞÏßÏàÏáÏâÏãÏäÏåÏæÏçÏèÏéÏêÏ\
ëÏìÏíÏîÏïÏðÏñÏòÏóÏôÏõÏöÏ÷ÏÐ‚ÐƒÐ„Ð…Ð†Ð‡ÐˆÐ‰ÐŠÐ‹ÐŒÐÐŽÐÐÐ‘Ð’Ð“Ð”Ð•Ð–Ð—Ð˜Ð™ÐšÐ›ÐœÐÐžÐŸÐ Ð°Ð±Ð²Ð³Ð´ÐµÐ¶Ð·Ð¸Ð¹ÐºÐ»Ð¼Ð½Ð¾Ð¿Ð\
ÀÐÁÐÂÐÃÐÄÐÅÐÆÐÇÐÈÐÉÐÊÐËÐÌÐÍÐÎÐÏÐÐÐÑÐÒÐÓÐÔÐÕÐÖÐ×ÐØÐÙÐÚÐÛÐÜÐÝÐÞÐßÐàÐáÐâÐãÐäÐåÐæÐçÐèÐéÐêÐëÐìÐíÐîÐïÐðÐñÐòÐóÐôÐõÐöÐ÷ÐÑ‚ÑƒÑ„Ñ…Ñ\
†Ñ‡ÑˆÑ‰ÑŠÑ‹ÑŒÑÑŽÑÑÑ‘Ñ’Ñ“Ñ”Ñ•Ñ–Ñ—Ñ˜Ñ™ÑšÑ›ÑœÑÑžÑŸÑ Ñ°Ñ±Ñ²Ñ³Ñ´ÑµÑ¶Ñ·Ñ¸Ñ¹ÑºÑ»Ñ¼Ñ½Ñ¾Ñ¿ÑÀÑÁÑÂÑÃÑÄÑÅÑÆÑÇÑÈÑÉÑÊÑËÑÌÑÍÑÎÑÏÑÐÑÑÑ\
ÒÑÓÑÔÑÕÑÖÑ×ÑØÑÙÑÚÑÛÑÜÑÝÑÞÑßÑàÑáÑâÑãÑäÑåÑæÑçÑèÑéÑêÑëÑìÑíÑîÑïÑðÑñÑòÑóÑôÑõÑöÑ÷ÑÒ‚ÒƒÒ„Ò…Ò†Ò‡ÒˆÒ‰ÒŠÒ‹ÒŒÒÒŽÒÒÒ‘Ò’Ò“Ò”Ò•Ò–Ò—Ò\
˜Ò™ÒšÒ›ÒœÒÒžÒŸÒ Ò°Ò±Ò²Ò³Ò´ÒµÒ¶Ò·Ò¸Ò¹ÒºÒ»Ò¼Ò½Ò¾Ò¿ÒÀÒÁÒÂÒÃÒÄÒÅÒÆÒÇÒÈÒÉÒÊÒËÒÌÒÍÒÎÒÏÒÐÒÑÒÒÒÓÒÔÒÕÒÖÒ×ÒØÒÙÒÚÒÛÒÜÒÝÒÞÒßÒàÒáÒâÒãÒ\
äÒåÒæÒçÒèÒéÒêÒëÒìÒíÒîÒïÒðÒñÒòÒóÒôÒõÒöÒ÷ÒÓ‚ÓƒÓ„Ó…Ó†Ó‡ÓˆÓ‰ÓŠÓ‹ÓŒÓÓŽÓÓÓ‘Ó’Ó“Ó”Ó•Ó–Ó—Ó˜Ó™ÓšÓ›ÓœÓÓžÓŸÓ Ó°Ó±Ó²Ó³Ó´ÓµÓ¶Ó·Ó¸Ó\
¹ÓºÓ»Ó¼Ó½Ó¾Ó¿ÓÀÓÁÓÂÓÃÓÄÓÅÓÆÓÇÓÉÓÊÓËÓÌÓÍÓÎÓÏÓÐÓÑÓÒÓÓÓÔÓÕÓÖÓ×ÓØÓÙÓÚÓÛÓÜÓÝÓÞÓßÓàÓáÓâÓãÓäÓåÓæÓçÓèÓéÓêÓëÓìÓíÓîÓïÓðÓñÓòÓóÓôÓõÓöÓ\
÷ÓÔ‚ÔƒÔ„Ô…Ô†Ô‡ÔˆÔ‰ÔŠÔ‹ÔŒÔÔŽÔÔÔ‘Ô’Ô“Ô”Ô•Ô–Ô—Ô˜Ô™ÔšÔ›ÔœÔÔžÔŸÔ Ô°Ô±Ô²Ô³Ô´ÔµÔ¶Ô·Ô¸Ô¹ÔºÔ»Ô¼Ô½Ô¾Ô¿ÔÀÔÁÔÂÔÃÔÄÔÅÔÆÔÇÔÉÔÊÔËÔÌÔ\
ÍÔÎÔÏÔÐÔÑÔÒÔÓÔÔÔÕÔÖÔ×ÔØÔÙÔÚÔÛÔÜÔÝÔÞÔßÔàÔáÔâÔãÔäÔåÔæÔçÔèÔéÔêÔëÔìÔíÔîÔïÔðÔñÔòÔóÔôÔõÔöÔ÷ÔÕ‚ÕƒÕ„Õ…Õ†Õ‡ÕˆÕ‰ÕŠÕ‹ÕŒÕÕŽÕÕÕ‘Õ’Õ\
“Õ”Õ•Õ–Õ—Õ˜Õ™ÕšÕ›ÕœÕÕžÕŸÕ Õ°Õ±Õ²Õ³Õ´ÕµÕ¶Õ·Õ¸Õ¹ÕºÕ»Õ¼Õ½Õ¾Õ¿ÕÀÕÁÕÂÕÃÕÄÕÅÕÆÕÇÕÈÕÉÕÊÕËÕÌÕÍÕÎÕÏÕÐÕÑÕÒÕÓÕÔÕÕÕÖÕ×ÕØÕÙÕÚÕÛÕÜÕÝÕÞÕ\
ßÕàÕáÕâÕãÕäÕåÕæÕçÕèÕéÕêÕëÕìÕíÕîÕïÕðÕñÕòÕóÕôÕõÕöÕ÷ÕÖ‚ÖƒÖ„Ö…Ö†ÖˆÖ‰ÖŠÖ‹ÖŒÖÖŽÖÖÖ‘Ö’Ö“Ö”Ö•Ö–Ö—Ö˜Ö™ÖšÖ›ÖœÖÖžÖŸÖ Ö°Ö±Ö²Ö³Ö´Ö\
µÖ¶Ö·Ö¸Ö¹ÖºÖ»Ö¼Ö½Ö¾Ö¿ÖÀÖÁÖÂÖÃÖÄÖÅÖÆÖÇÖÈÖÉÖÊÖËÖÌÖÍÖÎÖÏÖÐÖÑÖÒÖÓÖÔÖÕÖÖÖ×ÖØÖÙÖÚÖÛÖÜÖÝÖÞÖßÖàÖáÖâÖãÖäÖåÖæÖçÖèÖéÖêÖëÖìÖíÖîÖïÖðÖñÖ\
òÖóÖôÖõÖöÖ÷Ö×‚×ƒ×„×…×†×‡×ˆ×‰×Š×‹×Œ××Ž×××‘×’×“×”×•×–×—×˜×™×š×›×œ××ž×Ÿ× ×°×±×²×³×´×µ×¶×·×¸×¹×º×»×¼×½×¾×¿×À×Á×Â×Ã×Ä×Å×Æ×\
Ç×È×É×Ê×Ë×Ì×Í×Î×Ï×Ð×Ñ×Ò×Ó×Ô×Õ×Ö×××Ø×Ù×Ú×Û×Ü×Ý×Þ×ß×à×á×â×ã×ä×å×æ×ç×è×é×ê×ë×ì×í×î×ï×ð×ñ×ò×ó×ô×õ×ö×÷×Ø‚ØƒØ„Ø…Ø†Ø‡ØˆØ‰ØŠØ‹ØŒØ\
ØŽØØØ‘Ø’Ø“Ø”Ø•Ø–Ø—Ø˜Ø™ØšØ›ØœØØžØŸØ Ø°Ø±Ø²Ø³Ø´ØµØ¶Ø·Ø¸Ø¹ØºØ»Ø¼Ø½Ø¾Ø¿ØÀØÁØÂØÃØÄØÅØÆØÇØÈØÉØÊØËØÌØÍØÎØÏØÐØÑØÒØÓØÔØÕØÖØ×ØØØ\
ÙØÚØÛØÜØÝØÞØßØàØáØâØãØäØåØæØçØèØéØêØëØìØíØîØïØðØñØòØóØôØõØöØ÷ØÙ‚ÙƒÙ„Ù…Ù†Ù‡Ù‰ÙŠÙ‹ÙŒÙÙŽÙÙÙ‘Ù’Ù“Ù”Ù•Ù–Ù—Ù˜Ù™ÙšÙ›ÙœÙÙžÙŸÙ\
 Ù°Ù±Ù²Ù³Ù´ÙµÙ¶Ù·Ù¸Ù¹ÙºÙ»Ù¼Ù½Ù¾Ù¿ÙÀÙÁÙÂÙÃÙÄÙÅÙÆÙÇÙÈÙÉÙÊÙËÙÌÙÍÙÎÙÏÙÐÙÑÙÒÙÓÙÔÙÕÙÖÙ×ÙØÙÙÙÚÙÛÙÜÙÝÙÞÙßÙàÙáÙâÙãÙäÙåÙæÙçÙèÙéÙêÙëÙ\
ìÙíÙîÙïÙðÙñÙòÙóÙôÙõÙöÙ÷ÙÚ‚ÚƒÚ„Ú…Ú†Ú‡ÚˆÚ‰ÚŠÚ‹ÚŒÚÚŽÚÚÚ‘Ú’Ú“Ú”Ú•Ú–Ú—Ú˜Ú™ÚšÚ›ÚœÚÚžÚŸÚ Ú°Ú±Ú²Ú³Ú´ÚµÚ¶Ú·Ú¸Ú¹ÚºÚ»Ú¼Ú½Ú¾Ú¿ÚÀÚ\
ÁÚÂÚÃÚÄÚÅÚÆÚÇÚÈÚÉÚÊÚËÚÌÚÍÚÎÚÏÚÐÚÑÚÒÚÓÚÔÚÕÚÖÚ×ÚØÚÙÚÚÚÛÚÜÚÝÚÞÚßÚàÚáÚâÚãÚäÚåÚæÚçÚèÚéÚêÚëÚìÚíÚîÚïÚðÚñÚòÚóÚôÚõÚöÚ÷ÚÛ‚ÛƒÛ„Û…Û†Û\
‡ÛˆÛ‰ÛŠÛ‹ÛŒÛÛŽÛÛÛ‘Û’Û“Û”Û•Û–Û—Û˜Û™ÛšÛ›ÛœÛÛžÛŸÛ Û°Û±Û²Û³Û´ÛµÛ¶Û·Û¸Û¹ÛºÛ»Û¼Û½Û¾Û¿ÛÀÛÁÛÂÛÃÛÄÛÅÛÆÛÇÛÈÛÉÛÊÛËÛÌÛÍÛÎÛÏÛÐÛÑÛÒÛ\
ÓÛÔÛÕÛÖÛ×ÛØÛÙÛÚÛÛÛÜÛÝÛÞÛßÛàÛáÛâÛãÛäÛåÛæÛçÛèÛéÛêÛëÛìÛíÛîÛïÛðÛñÛòÛóÛôÛõÛöÛ÷ÛÜ‚ÜƒÜ„Ü…Ü†Ü‡ÜˆÜ‰ÜŠÜ‹ÜŒÜÜŽÜÜÜ‘Ü’Ü“Ü”Ü•Ü–Ü—Ü˜Ü\
™ÜšÜ›ÜœÜÜžÜŸÜ Ü°Ü±Ü²Ü³Ü´ÜµÜ¶Ü·Ü¸Ü¹ÜºÜ»Ü¼Ü½Ü¾Ü¿ÜÀÜÁÜÂÜÃÜÄÜÅÜÆÜÇÜÈÜÉÜÊÜËÜÌÜÍÜÎÜÏÜÐÜÑÜÒÜÓÜÔÜÕÜÖÜ×ÜØÜÙÜÚÜÛÜÜÜÝÜÞÜßÜàÜáÜâÜãÜäÜ\
åÜæÜçÜèÜéÜêÜëÜìÜíÜîÜïÜðÜñÜòÜóÜôÜõÜöÜ÷ÜÝ‚ÝƒÝ„Ý…Ý†Ý‡ÝˆÝ‰ÝŠÝŒÝÝŽÝÝÝ‘Ý’Ý“Ý”Ý•Ý–Ý—Ý˜Ý™ÝšÝ›ÝœÝÝžÝŸÝ Ý°Ý±Ý²Ý³Ý´ÝµÝ¶Ý·Ý¸Ý¹ÝºÝ\
»Ý¼Ý½Ý¾Ý¿ÝÀÝÁÝÂÝÃÝÄÝÅÝÆÝÇÝÈÝÉÝÊÝËÝÌÝÍÝÎÝÏÝÐÝÑÝÒÝÓÝÔÝÕÝÖÝ×ÝØÝÙÝÚÝÛÝÜÝÝÝÞÝßÝàÝáÝâÝãÝäÝåÝæÝçÝèÝéÝêÝëÝìÝíÝîÝïÝðÝñÝòÝóÝôÝõÝöÝ÷Ý\
Þ‚ÞƒÞ„Þ…Þ†Þ‡ÞˆÞ‰ÞŠÞ‹ÞŒÞÞŽÞÞÞ‘Þ’Þ“Þ”Þ•Þ–Þ—Þ˜Þ™ÞšÞ›ÞœÞÞžÞŸÞ Þ°Þ±Þ²Þ³Þ´ÞµÞ¶Þ·Þ¸Þ¹ÞºÞ»Þ¼Þ½Þ¾Þ¿ÞÀÞÁÞÂÞÃÞÄÞÅÞÆÞÇÞÈÞÉÞÊÞËÞÌÞ\
ÍÞÎÞÏÞÐÞÑÞÒÞÓÞÔÞÕÞÖÞ×ÞØÞÙÞÚÞÛÞÜÞÝÞÞÞßÞàÞáÞâÞãÞäÞåÞæÞçÞèÞéÞêÞëÞìÞíÞîÞïÞðÞñÞòÞóÞôÞõÞöÞ÷Þß‚ßƒß„ß…ß†ß‡ßˆß‰ßŠß‹ßŒßßŽßßß‘ß’ß\
“ß”ß•ß–ß—ß˜ß™ßšß›ßœßßžßŸß ß°ß±ß²ß³ß´ßµß¶ß·ß¸ß¹ßºß»ß¼ß½ß¾ß¿ßÀßÁßÂßÃßÄßÅßÆßÇßÈßÉßÊßËßÌßÍßÎßÏßÐßÑßÒßÓßÔßÕßÖß×ßØßÙßÚßÛßÜßÝßÞß\
ßßàßáßâßãßäßåßæßçßèßéßêßëßìßíßîßïßðßñßòßóßôßõßöß÷ßà‚àƒà„à…à†à‡àˆà‰àŠà‹àŒààŽààà‘à’à“à”à•à–à—à˜à™àšà›àœààžàŸà à°à±à²à³à\
´àµà¶à·à¸à¹àºà»à¼à½à¾à¿àÀàÁàÂàÃàÄàÅàÆàÇàÈàÉàÊàËàÌàÍàÎàÏàÐàÑàÒàÓàÔàÕàÖà×àØàÙàÚàÛàÜàÝàÞàßàààáàâàãàäàåàæàçàèàéàêàëàìàíàîàïàðà\
ñàòàóàôàõàöà÷àá‚áƒá„á…á†á‡áˆá‰áŠá‹áŒááŽááá‘á’á“á”á•á–á—á˜á™ášá›áœáážáŸá á°á±á²á³á´áµá¶á·á¸á¹áºá»á¼á½á¾á¿áÀáÁáÂáÃáÄáÅá\
ÆáÇáÈáÉáÊáËáÌáÍáÎáÏáÐáÑáÒáÓáÔáÕáÖá×áØáÙáÚáÛáÜáÝáÞáßáàáááâáãáäáåáæáçáèáéáêáëáìáíáîáïáðáñáòáóáôáõáöá÷áâ‚âƒâ„â…â†â‡âˆâ‰âŠâ‹â\
ŒââŽâââ‘â’â“â”â•â–â—â˜â™âšâ›âœââžâŸâ â°â±â²â³â´âµâ¶â·â¸â¹âºâ»â¼â½â¾â¿âÀâÁâÂâÃâÄâÅâÆâÇâÈâÉâÊâËâÌâÍâÎâÏâÐâÑâÒâÓâÔâÕâÖâ×â\
ØâÙâÚâÛâÜâÝâÞâßâàâáâââãâäâåâæâçâèâéâêâëâìâíâîâïâðâñâòâóâôâõâöâ÷âã‚ãƒã„ã…ã†ã‡ãˆã‰ãŠã‹ãŒããŽããã‘ã’ã“ã”ã•ã–ã—ã˜ã™ãšã›ãœãã\
žãŸã ã°ã±ã²ã³ã´ãµã¶ã·ã¸ã¹ãºã»ã¼ã½ã¾ã¿ãÁãÂãÃãÄãÅãÆãÇãÈãÉãÊãËãÌãÍãÎãÏãÐãÑãÒãÓãÔãÕãÖã×ãØãÙãÚãÛãÜãÝãÞãßãàãáãâãããäãåãæãçãèãéãêã\
ëãìãíãîãïãðãñãòãóãôãõãöã÷ãä‚äƒä„ä…ä†ä‡äˆä‰äŠä‹äŒääŽäää‘ä’ä“ä”ä•ä–ä—ä˜ä™äšä›äœääžäŸä ä°ä±ä²ä³ä´äµä¶ä·ä¸ä¹äºä»ä¼ä½ä¾ä¿ä\
ÀäÁäÂäÃäÄäÅäÆäÇäÈäÉäÊäËäÌäÍäÎäÏäÐäÑäÒäÓäÔäÕäÖä×äØäÙäÚäÛäÜäÝäÞäßäàäáäâäãäääåäæäçäèäéäêäëäìäíäîäïäðäñäòäóäôäõäöä÷äå‚åƒå„å…å\
†å‡åˆå‰åŠå‹åŒååŽååå‘å’å“å”å•å–å—å˜å™åšå›åœååžåŸå å°å±å²å³å´åµå¶å·å¸å¹åºå»å¼å½å¾å¿åÀåÁåÂåÃåÄåÅåÆåÇåÈåÉåÊåËåÌåÍåÎåÏåÐåÑå\
ÒåÓåÔåÕåÖå×åØåÙåÚåÛåÜåÝåÞåßåàåáåâåãåäåååæåçåèåéåêåëåìåíåîåïåðåñåòåóåôåõåöå÷åæ‚æƒæ„æ…æ†æ‡æˆæ‰æŠæ‹æŒææŽæææ‘æ’æ“æ”æ•æ–æ—æ\
˜æ™æšæ›æœææžæŸæ æ°æ±æ²æ³æ´æµæ¶æ·æ¸æ¹æºæ»æ¼æ½æ¾æ¿æÀæÁæÂæÃæÄæÅæÆæÇæÈæÉæÊæËæÌæÍæÎæÏæÐæÑæÒæÓæÔæÕæÖæ×æØæÙæÚæÛæÜæÝæÞæßæàæáæâæãæ\
äæåæææçæèæéæêæëæìæíæîæïæðæñæòæóæôæõæöæ÷æç‚çƒç„ç…ç†ç‡çˆç‰çŠç‹çŒççŽççç‘ç’ç“ç”ç•ç–ç—ç˜ç™çšç›çœççžçŸç ç°ç±ç²ç³ç´çµç¶ç·ç¸ç\
¹çºç»ç¼ç½ç¾ç¿çÀçÁçÂçÃçÄçÅçÆçÇçÈçÉçÊçËçÌçÍçÎçÏçÐçÑçÒçÓçÔçÕçÖç×çØçÙçÚçÛçÜçÝçÞçßçàçáçâçãçäçåçæçççèçéçêçëçìçíçîçïçðçñçòçóçôçõç\
öç÷çè‚èƒè„è…è†è‡èˆè‰èŠè‹èèŽèèè‘è’è“è”è•è–è—è˜è™èšè›èœèèžèŸè è°è±è²è³è´èµè¶è·è¸è¹èºè»è¼è½è¾è¿èÀèÁèÂèÃèÄèÅèÆèÇèÈèÉèÊèËè\
ÌèÍèÎèÏèÐèÑèÒèÓèÔèÕèÖè×èØèÙèÚèÛèÜèÝèÞèßèàèáèâèãèäèåèæèçèèèéèêèëèìèíèîèïèðèñèòèóèôèõèöè÷èé‚éƒé„é…é†é‡éˆé‰éŠé‹éŒééŽééé‘é\
’é“é”é•é–é—é˜é™éšé›éœééžéŸé é°é±é²é³é´éµé¶é·é¸é¹éºé»é¼é½é¾é¿éÀéÁéÂéÃéÄéÅéÆéÇéÈéÉéÊéËéÌéÍéÎéÏéÐéÑéÒéÓéÔéÕéÖé×éØéÙéÚéÛéÜéÝé\
Þéßéàéáéâéãéäéåéæéçéèéééêéëéìéíéîéïéðéñéòéóéôéõéöé÷éê‚êƒê„ê…ê†ê‡êˆê‰êŠê‹êŒêêŽêêê‘ê’ê“ê”ê•ê–ê—ê˜ê™êšê›êœêêžêŸê ê°ê±ê²ê\
³ê´êµê¶ê·ê¸ê¹êºê»ê¼ê½ê¾ê¿êÀêÁêÂêÃêÄêÅêÆêÇêÈêÉêÊêËêÌêÍêÎêÏêÐêÑêÒêÓêÔêÕêÖê×êØêÙêÚêÛêÜêÝêÞêßêàêáêâêãêäêåêæêçêèêéêêêëêìêíêîêïê\
ðêñêòêóêôêõêöê÷êë‚ëƒë„ë…ë†ë‡ëˆë‰ëŠë‹ëŒëëŽëëë‘ë’ë“ë”ë•ë–ë—ë˜ë™ëšë›ëœëëžëŸë ë°ë±ë²ë³ë´ëµë¶ë·ë¸ë¹ëºë»ë¼ë½ë¾ë¿ëÀëÁëÂëÃëÄë\
ÅëÆëÇëÈëÉëÊëËëÌëÍëÎëÏëÐëÑëÒëÓëÔëÕëÖë×ëØëÙëÚëÛëÜëÝëÞëßëàëáëâëãëäëåëæëçëèëéëêëëëìëíëîëïëðëñëòëóëôëõëöë÷ëì‚ìƒì„ì…ì†ì‡ìˆì‰ìŠì\
‹ìŒììŽììì‘ì’ì“ì”ì•ì–ì—ì˜ì™ìšì›ìœììžìŸì ì°ì±ì²ì³ì´ìµì¶ì·ì¸ì¹ìºì»ì¼ì½ì¾ì¿ìÀìÁìÂìÃìÄìÅìÆìÇìÈìÉìÊìËìÌìÍìÎìÏìÐìÑìÒìÓìÔìÕìÖì\
×ìØìÙìÚìÛìÜìÝìÞìßìàìáìâìãìäìåìæìçìèìéìêìëìììíìîìïìðìñìòìóìôìõìöì÷ìí‚íƒí„í…í‡íˆí‰íŠí‹íŒííŽííí‘í’í“í”í•í–í—í˜í™íší›íœíí\
žíŸí í°í±í²í³í´íµí¶í·í¸í¹íºí»í¼í½í¾í¿íÀíÁíÂíÃíÄíÅíÆíÇíÈíÉíÊíËíÌíÍíÎíÏíÐíÑíÒíÓíÔíÕíÖí×íØíÙíÚíÛíÜíÝíÞíßíàíáíâíãíäíåíæíçíèíéí\
êíëíìíííîíïíðíñíòíóíôíõíöí÷íî‚îƒî„î…î†î‡îˆî‰îŠî‹îŒîîŽîîî‘î’î“î”î•î–î—î˜î™îšî›îœîîžîŸî î°î±î²î³î´îµî¶î·î¸î¹îºî»î¼î½î¾î\
¿îÀîÁîÂîÃîÄîÅîÆîÇîÈîÉîÊîËîÌîÍîÎîÏîÐîÑîÒîÓîÔîÕîÖî×îØîÙîÚîÛîÜîÝîÞîßîàîáîâîãîäîåîæîçîèîéîêîëîìîíîîîïîðîñîòîóîôîõîöî÷îï‚ïƒï„ï\
…ï†ï‡ïˆï‰ïŠï‹ïŒïïŽïïï‘ï’ï“ï”ï•ï–ï—ï˜ï™ïšï›ïœïïžïŸï ï°ï±ï²ï³ï´ïµï¶ï·ï¸ï¹ïºï»ï¼ï½ï¾ï¿ïÀïÁïÂïÃïÅïÆïÇïÈïÉïÊïËïÌïÍïÎïÏïÐïÑï\
ÒïÓïÔïÕïÖï×ïØïÙïÚïÛïÜïÝïÞïßïàïáïâïãïäïåïæïçïèïéïêïëïìïíïîïïïðïñïòïóïôïõïöï÷ïð‚ðƒð„ð…ð†ð‡ðˆð‰ðŠð‹ðŒððŽððð‘ð’ð“ð”ð•ð–ð—ð\
˜ð™ðšð›ðœððžðŸð ð°ð±ð²ð³ð´ðµð¶ð·ð¸ð¹ðºð»ð¼ð½ð¾ð¿ðÀðÁðÂðÃðÅðÆðÇðÈðÉðÊðËðÌðÍðÎðÏðÐðÑðÒðÓðÔðÕðÖð×ðØðÙðÚðÛðÜðÝðÞðßðàðáðâðãðäð\
åðæðçðèðéðêðëðìðíðîðïðððñðòðóðôðõðöð÷ðñ‚ñƒñ„ñ…ñ†ñ‡ñˆñ‰ñŠñ‹ñŒññŽñññ‘ñ’ñ“ñ”ñ•ñ–ñ—ñ˜ñ™ñšñ›ñœññžñŸñ ñ°ñ±ñ²ñ³ñ´ñµñ¶ñ·ñ¸ñ¹ñ\
ºñ»ñ¼ñ½ñ¾ñ¿ñÀñÁñÂñÃñÄñÅñÆñÇñÈñÉñÊñËñÌñÍñÎñÏñÐñÑñÒñÓñÔñÕñÖñ×ñØñÙñÚñÛñÜñÝñÞñßñàñáñâñãñäñåñæñçñèñéñêñëñìñíñîñïñðñññòñóñôñõñöñ\
÷ñò‚òƒò„ò…ò†ò‡òˆò‰òŠò‹òŒòòŽòòò‘ò’ò“ò”ò•ò–ò—ò˜ò™òšò›òœòòžòŸò ò°ò±ò²ò³ò´òµò¶ò·ò¸ò¹òºò»ò¼ò½ò¾ò¿òÀòÁòÂòÃòÄòÅòÆòÇòÈòÉòÊòËò\
ÌòÍòÎòÏòÐòÑòÒòÓòÔòÕòÖò×òØòÙòÚòÛòÜòÝòÞòßòàòáòâòãòäòåòæòçòèòéòêòëòìòíòîòïòðòñòòòóòôòõòöò÷òó‚óƒó„ó…ó†ó‡óˆó‰óŠó‹óŒóóŽóóó‘ó\
’ó“ó”ó•ó–ó—ó˜ó™óšó›óœóóžóŸó ó°ó±ó²ó³ó´óµó¶ó·ó¸ó¹óºó»ó¼ó½ó¾ó¿óÀóÁóÂóÃóÄóÅóÆóÇóÈóÉóÊóËóÌóÍóÎóÏóÐóÑóÒóÓóÔóÕóÖó×óØóÙóÚóÛóÜóÝó\
Þóßóàóáóâóãóäóåóæóçóèóéóêóëóìóíóîóïóðóñóòóóóôóõóöó÷óô‚ôƒô„ô…ô†ô‡ôˆô‰ôŠô‹ôŒôôŽôôô‘ô’ô“ô”ô•ô–ô—ô˜ô™ôšô›ôœôôžôŸô ô°ô±ô²ô\
³ô´ôµô¶ô·ô¸ô¹ôºô»ô¼ô½ô¾ô¿ôÀôÁôÂôÃôÄôÅôÆôÇôÈôÉôÊôËôÌôÍôÎôÏôÐôÑôÒôÓôÔôÕôÖô×ôØôÙôÚôÛôÜôÝôÞôßôàôáôâôãôäôåôæôçôèôéôêôëôìôíôîôïô\
ðôñôòôóôôôõôöô÷ôõ‚õƒõ…õ†õ‡õˆõ‰õŠõ‹õŒõõŽõõõ‘õ’õ“õ”õ•õ–õ—õ˜õ™õšõ›õœõõžõŸõ õ°õ±õ²õ³õ´õµõ¶õ·õ¸õ¹õºõ»õ¼õ½õ¾õ¿õÀõÁõÂõÃõÄõÅõ\
ÆõÇõÈõÉõÊõËõÌõÍõÎõÏõÐõÑõÒõÓõÔõÕõÖõ×õØõÙõÚõÛõÜõÝõÞõßõàõáõâõãõäõåõæõçõèõéõêõëõìõíõîõïõðõñõòõóõôõõõöõ÷õö‚öƒö„ö…ö†ö‡öˆö‰öŠö‹ö\
ŒööŽööö‘ö’ö“ö”ö•ö–ö—ö˜ö™öšö›öœööžöŸö ö°ö±ö²ö³ö´öµö¶ö·ö¸ö¹öºö»ö¼ö½ö¾ö¿öÀöÁöÂöÃöÄöÅöÆöÇöÈöÉöÊöËöÌöÍöÎöÏöÐöÑöÒöÓöÔöÕöÖö×ö\
ØöÙöÚöÛöÜöÝöÞößöàöáöâöãöäöåöæöçöèöéöêöëöìöíöîöïöðöñöòöóöôöõööö÷ö÷‚÷ƒ÷„÷…÷†÷‡÷ˆ÷‰÷Š÷‹÷Œ÷÷Ž÷÷÷‘÷’÷“÷”÷•÷–÷—÷˜÷™÷š÷›÷œ÷÷\
ž÷Ÿ÷ ÷°÷±÷²÷³÷´÷µ÷¶÷·÷¸÷¹÷º÷»÷¼÷½÷¾÷¿÷À÷Á÷Â÷Ã÷Ä÷Å÷Æ÷Ç÷È÷É÷Ê÷Ë÷Ì÷Í÷Î÷Ï÷Ð÷Ñ÷Ò÷Ó÷Ô÷Õ÷Ö÷×÷Ø÷Ù÷Ú÷Û÷Ü÷Ý÷Þ÷ß÷à÷á÷â÷ã÷ä÷å÷æ÷ç÷è÷é÷\
ê÷ë÷ì÷í÷î÷ï÷ð÷ñ÷ò÷ó÷ô÷õ÷ö÷÷÷ø‚øƒø„ø…ø†ø‡øˆø‰øŠø‹øŒøøŽøøø‘ø’ø“ø”ø•ø–ø—ø˜ø™øšø›øœøøžøŸø ø°ø±ø²ø³ø´øµø¶ø·ø¸ø¹øºø»ø¼ø½ø¾ø\
¿øÀøÁøÂøÃøÄøÅøÆøÇøÈøÉøÊøËøÌøÍøÎøÏøÐøÑøÒøÓøÔøÕøÖø×øØøÙøÚøÛøÜøÝøÞøßøàøáøâøãøäøåøæøçøèøéøêøëøìøíøîøïøðøñøòøóøôøõøöø÷øù‚ùƒù„ù\
…ù†ù‡ùˆù‰ùŠù‹ùŒùùŽùùù‘ù’ù“ù”ù•ù–ù—ù˜ù™ùšù›ùœùùžùŸù ù°ù±ù²ù³ù´ùµù¶ù·ù¸ù¹ùºù»ù¼ù½ù¾ù¿ùÀùÁùÂùÃùÄùÅùÆùÇùÈùÉùÊùËùÌùÍùÎùÏùÐù\
ÑùÒùÓùÔùÕùÖù×ùØùÙùÚùÛùÜùÝùÞùßùàùáùâùãùäùåùæùçùèùéùêùëùìùíùîùïùðùñùòùóùôùõùöù÷ùú‚úƒú„ú…ú†ú‡úˆú‰úŠú‹úŒúúŽúúú‘ú’ú“ú”ú•ú–ú\
—ú˜ú™úšú›úœúúžúŸú ú°ú±ú²ú³ú´úµú¶ú·ú¸ú¹úºú»ú¼ú½ú¾ú¿úÀúÁúÂúÃúÄúÅúÆúÇúÈúÉúÊúËúÌúÍúÎúÏúÐúÑúÒúÓúÔúÕúÖúØúÙúÚúÛúÜúÝúÞúßúàúáúâúãú\
äúåúæúçúèúéúêúëúìúíúîúïúðúñúòúóúôúõúöú÷úû‚ûƒû„û…û†û‡ûˆû‰ûŠû‹ûŒûûŽûûû‘û’û“û”û•û–û—û˜û™ûšû›ûœûûžûŸû û°û±û²û³û´ûµû¶û·û¸û\
¹ûºû»û¼û½û¾û¿ûÀûÁûÂûÃûÄûÅûÆûÇûÈûÉûÊûËûÌûÍûÎûÏûÐûÑûÒûÓûÔûÕûÖûØûÙûÚûÛûÜûÝûÞûßûàûáûâûãûäûåûæûçûèûéûêûëûìûíûîûïûðûñûòûóûôûõûöû\
÷ûü‚üƒü„ü…ü†ü‡üˆü‰üŠü‹üŒüüŽüüü‘ü’ü“ü”ü•ü–ü—ü˜ü™üšü›üœüüžüŸü ü°ü±ü²ü³ü´üµü¶ü·ü¸ü¹üºü»ü¼ü½ü¾ü¿üÀüÁüÂüÃüÄüÅüÆüÇüÈüÉüÊüËü\
ÌüÍüÎüÏüÐüÑüÒüÓüÔüÕüÖüØüÙüÚüÛüÜüÝüÞüßüàüáüâüãüäüåüæüçüèüéüêüëüìüíüîüïüðüñüòüóüôüõüöü÷üý‚ýƒý„ý…ý†ý‡ýˆý‰ýŠý‹ýŒýýŽýýý‘ý’ý\
“ý”ý•ý–ý—ý˜ý™ýšý›ýœýýžýŸý ý°ý±ý²ý³ý´ýµý¶ý·ý¸ý¹ýºý»ý¼ý½ý¾ý¿ýÀýÁýÂýÃýÄýÅýÆýÇýÈýÉýÊýËýÌýÍýÎýÏýÐýÑýÒýÓýÔýÕýÖýØýÙýÚýÛýÜýÝýÞýßý\
àýáýâýãýäýåýæýçýèýéýêýëýìýíýîýïýðýñýòýóýôýõýöý÷ýþ‚þƒþ„þ…þ†þ‡þˆþ‰þŠþ‹þŒþþŽþþþ‘þ’þ“þ”þ•þ–þ—þ˜þ™þšþ›þœþþžþŸþ þ°þ±þ²þ³þ´þ\
µþ¶þ·þ¸þ¹þºþ»þ¼þ½þ¾þ¿þÀþÁþÂþÃþÄþÅþÆþÇþÈþÉþÊþËþÌþÍþÎþÏþÐþÑþÒþÓþÔþÕþÖþØþÙþÚþÛþÜþÝþÞþßþàþáþâþãþäþåþæþçþèþéþêþëþìþíþîþïþðþñþòþ\
óþôþõþöþ÷þ"
#endif
#endif