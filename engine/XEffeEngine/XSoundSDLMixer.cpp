#include "XSoundSDLMixer.h"
#include "XBasicWindow.h"

bool _XSoundSDLMixer::init()
{
//初始化声音设备(Music Sound 与Movie类，这边对于声音设备存在冲突，必须要独占)
//	XEE::audioFrequency = MIX_DEFAULT_FREQUENCY;
	XEE::audioFrequency = 44100;	
	XEE::audioFormat = MIX_DEFAULT_FORMAT;
	XEE::audioChannel = 2;
	const int TMP_CHUNK_SIZE = 2048;// + 192000 * 3 /2;

	Mix_OpenAudio(XEE::audioFrequency,XEE::audioFormat,XEE::audioChannel,TMP_CHUNK_SIZE);
	Mix_AllocateChannels(128);
	return true;
}