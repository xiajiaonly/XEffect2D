//////////////////////////////////////////////////////////////////////////////
// (c) Copyright AVerMedia Technologies, Inc. 2012. All Rights Reserved.
//
//  Module:
//
//    AVerCapAPI.H
//
//  Date:
//
//	 9/13/2012
//
//  Abstract:
//
//    Header file for AVerCapAPI.DLL
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _AVERHDCapAPI_H_
#define _AVERHDCapAPI_H_

// error code
enum
{
    CAP_EC_SUCCESS = 0,
    CAP_EC_INIT_DEVICE_FAILED = -1,
    CAP_EC_DEVICE_IN_USE = -2,
    CAP_EC_NOT_SUPPORTED = -3,
    CAP_EC_INVALID_PARAM = -4,
    CAP_EC_TIMEOUT = -5,
    CAP_EC_NOT_ENOUGH_MEMORY = -6,
    CAP_EC_UNKNOWN_ERROR = -7,
	CAP_EC_ERROR_STATE = -8
};

enum
{
    DEVICETYPE_SD = 1,
    DEVICETYPE_HD = 2,
    DEVICETYPE_ALL = 3
};

// video input source
enum
{
    VIDEOSOURCE_COMPOSITE = 0,
    VIDEOSOURCE_SVIDEO = 1,
    VIDEOSOURCE_COMPONENT = 2,
    VIDEOSOURCE_HDMI = 3,
    VIDEOSOURCE_VGA = 4 
};

// video format
enum
{										
    VIDEOFORMAT_NTSC = 0,
    VIDEOFORMAT_PAL = 1			
};

// video resolution
enum
{											
    VIDEORESOLUTION_640X480 = 0,				
	VIDEORESOLUTION_704X576 = 1,				
	VIDEORESOLUTION_720X480 = 2,				
	VIDEORESOLUTION_720X480P = 3,				
	VIDEORESOLUTION_720X576 = 4,				
	VIDEORESOLUTION_720X576P = 5,	
	VIDEORESOLUTION_1280X720P = 6,
	VIDEORESOLUTION_1920X1080 = 7,
	VIDEORESOLUTION_1024X768P = 8,
	VIDEORESOLUTION_1280X800P = 9,
	VIDEORESOLUTION_1280X1024P = 10,
	VIDEORESOLUTION_1440X900P = 11,
	VIDEORESOLUTION_1600X1200P = 12,
	VIDEORESOLUTION_1680X1050P = 13,    				
	VIDEORESOLUTION_1920X1080P = 14,
	VIDEORESOLUTION_1920X1080P_24FPS = 15,	
	VIDEORESOLUTION_640X480P = 16,
	VIDEORESOLUTION_800X600P = 17,
	VIDEORESOLUTION_1280X768P = 18,
	VIDEORESOLUTION_1360X768P = 19,
	VIDEORESOLUTION_160X120 = 20,   // SD
	VIDEORESOLUTION_176X144 = 21,
	VIDEORESOLUTION_240X176 = 22,
	VIDEORESOLUTION_240X180 = 23,
	VIDEORESOLUTION_320X240 = 24,
	VIDEORESOLUTION_352X240 = 25,
	VIDEORESOLUTION_352X288 = 26,
	VIDEORESOLUTION_640X240 = 27,
	VIDEORESOLUTION_640X288 = 28,
	VIDEORESOLUTION_720X240 = 29,
	VIDEORESOLUTION_720X288 = 30,
	VIDEORESOLUTION_80X60   = 31,
	VIDEORESOLUTION_88X72   = 32,
	VIDEORESOLUTION_128X96  = 33,
	VIDEORESOLUTION_640X576 = 34,
	VIDEORESOLUTION_1152X864P = 35,
	VIDEORESOLUTION_1280X960P = 36,
	VIDEORESOLUTION_180X120 = 37,
	VIDEORESOLUTION_180X144 = 38,
	VIDEORESOLUTION_360X240 = 39,
	VIDEORESOLUTION_360X288 = 40
	
};

// video adjustment property
enum
{
    VIDEOPROCAMPPROPERTY_BRIGHTNESS = 0,
    VIDEOPROCAMPPROPERTY_CONTRAST = 1,
    VIDEOPROCAMPPROPERTY_HUE = 2,
    VIDEOPROCAMPPROPERTY_SATURATION = 3
};

// deinterlace mode
enum
{
    DEINTERLACE_NONE = 0,
    DEINTERLACE_WEAVE = 1,
    DEINTERLACE_BOB = 2,
    DEINTERLACE_BLEND = 3
};

// downscale mode
enum
{
    DSMODE_NONE = 0,
    DSMODE_HALFHEIGHT = 1,
    DSMODE_HALFWIDTH = 2,
    DSMODE_HALFBOTH	=3,
    DSMODE_CUSTOM = 5
};

// overlay settings
enum
{
    OVERLAY_TEXT = 0,
    OVERLAY_TIME = 1,
    OVERLAY_IMAGE = 2
};

enum
{
    FONTSIZE_SMALL = 0,
    FONTSIZE_BIG = 1
};

enum
{
    FORMAT_TIMEONLY = 0,
    FORMAT_DATEANDTIME = 1
};

enum
{
    ALIGNMENT_LEFT = 0,
    ALIGNMENT_CENTER = 1,
    ALIGNMENT_RIGHT = 2
};

// video frame/field capture settings
enum
{
	CT_SEQUENCE_FIELD = 0,
	CT_SEQUENCE_FRAME = 1
};

enum
{
	ST_BMP = 0,
	ST_JPG = 1,
	ST_AVI = 2,
	ST_CALLBACK = 3,
	ST_WAV = 4,
	ST_WMV = 5,
	ST_PNG = 6,
    ST_MPG = 7,
    ST_MP4 = 8,
    ST_TIFF = 9,
	ST_CALLBACK_RGB24 = 10,
	ST_CALLBACK_ARGB = 11
};

enum 
{
	DURATION_TIME = 1,
	DURATION_COUNT = 2
};

enum
{
    VIDEOENHANCE_NONE = 0, 
    VIDEOENHANCE_NORMAL = 1,  
    VIDEOENHANCE_SPLIT = 2, 
    VIDEOENHANCE_COMPARE = 3
};

enum
{
	VIDEOMIRROR_NONE = 0,
	VIDEOMIRROR_HORIZONTAL = 1,
	VIDEOMIRROR_VERTICAL = 2,
	VIDEOMIRROR_BOTH = 3					
};

enum
{
    AUDIOBITRATE_96 = 0,
    AUDIOBITRATE_112 = 1,
    AUDIOBITRATE_128 = 2,
    AUDIOBITRATE_144 = 3,
    AUDIOBITRATE_160 = 4,
    AUDIOBITRATE_176 = 5,
    AUDIOBITRATE_192 = 6,
    AUDIOBITRATE_224 = 7,
    AUDIOBITRATE_256 = 8,
    AUDIOBITRATE_288 = 9,
    AUDIOBITRATE_320 = 10,
    AUDIOBITRATE_352 = 11,
    AUDIOBITRATE_384 = 12,
	AUDIOBITRATE_64 = 13
};

enum
{
    VIDEOROTATE_NONE = 0,
    VIDEOROTATE_CW90 = 1,	
    VIDEOROTATE_CCW90 = 2
};

enum
{
	ENCODERTYPE_MPEGAUDIO = 0x00000001,
	ENCODERTYPE_H264 = 0x00000002
};

enum
{
	RCMODE_CBR = 0x00000001,
	RCMODE_VBR = 0x00000002
};

// Capture type
enum 
{	
	CT_CALLBACK_MPEGAUDIO = 0x00000001,
	CT_CALLBACK_H264 = 0x00000002,
	CT_CALLBACK_TS = 0x00000004,
	CT_FILE_TS= 0x01000000,
	CT_FILE_MP4 = 0x02000000
}; 

// Sample type
enum
{
	SAMPLETYPE_NULL = 0,
	SAMPLETYPE_RAWVIDEO = 0x01, 
	SAMPLETYPE_PCMAUDIO = 0x02, 
	SAMPLETYPE_TS = 0x10, 
	SAMPLETYPE_ES_H264 = 0x20, 
	SAMPLETYPE_ES_MPEG4AAC =0x30, 
	SAMPLETYPE_ES_MPEGAUDIO =0x31
};

// video renderer
enum
{
    VIDEORENDERER_DEFAULT = 0,
    VIDEORENDERER_VMR7 = 1,
    VIDEORENDERER_VMR9 = 2,
    VIDEORENDERER_EVR = 3
};

//HWVA
enum
{
	HWVA_ENCODER_H264 = 0x01
};

enum
{
	IMAGEQUALITY_BEST = 0,
	IMAGEQUALITY_NORMAL = 1,
	IMAGEQUALITY_LOW = 2
};

typedef struct _OVERLAY_POSITION
{
    DWORD dwXPos;
    DWORD dwYPos;
    DWORD dwAlignment;
}OVERLAY_POSITION;
 
typedef struct _OVERLAY_IMAGE_INFO
{
    LPWSTR lpFileName;
    DWORD  dwImageType;
}OVERLAY_IMAGE_INFO;

typedef struct _OVERLAY_INFO
{
    BOOL  bEnableOverlay;
    DWORD dwFontSize;
    DWORD dwFontColor;
    DWORD dwTransparency;
    OVERLAY_POSITION WindowPosition;
} OVERLAY_INFO;

typedef struct _VIDEO_SAMPLE_INFO
{
    DWORD dwWidth;
    DWORD dwHeight;
    DWORD dwStride;
    DWORD dwPixelFormat;
}VIDEO_SAMPLE_INFO;

typedef BOOL (WINAPI *VIDEOCAPTURECALLBACK)(VIDEO_SAMPLE_INFO VideoInfo, BYTE *pbData, LONG lLength, __int64 tRefTime, LONG lUserData); 

typedef struct _VIDEO_CAPTURE_INFO	
{
    DWORD  dwCaptureType;
    DWORD  dwSaveType;
    BOOL   bOverlayMix;
    DWORD  dwDurationMode; 
    DWORD  dwDuration;
    LPWSTR lpFileName;
    VIDEOCAPTURECALLBACK lpCallback;
    LONG   lCallbackUserData;
}VIDEO_CAPTURE_INFO;

typedef struct _AUDIO_SAMPLE_INFO
{ 
    DWORD dwChannels; 				
    DWORD dwBitsPerSample;			 
    DWORD dwSamplingRate; 
}AUDIO_SAMPLE_INFO;

typedef BOOL (WINAPI *AUDIOCAPTURECALLBACK)(AUDIO_SAMPLE_INFO AudioInfo, BYTE *pbData, LONG lLenght, __int64 tRefTime, LONG lUserData);

typedef struct _VIDEO_COMPRESSOR_INFO
{	
    WCHAR szName[128];
    BOOL  bPropertyPageSupported;
    BOOL  bQualitySupported;
    BOOL  bPropertyPageVisible;
    DWORD dwQuality;
}VIDEO_COMPRESSOR_INFO;

typedef struct _AUDIO_SAMPLE_FORMAT
{
    DWORD dwChannels; 				
    DWORD dwBitsPerSample;			 
    DWORD dwSamplingRate; 
    DWORD dwAvgBytesPerSec;	
}AUDIO_SAMPLE_FORMAT;

typedef struct _AUDIO_COMPRESSOR_INFO
{
    WCHAR szName[128];
    AUDIO_SAMPLE_FORMAT AudioSampleFormat;
}AUDIO_COMPRESSOR_INFO;

typedef struct _AUDIO_CAPTURE_INFO	
{
    DWORD  dwSaveType;
    LPWSTR lpFileName;
    AUDIOCAPTURECALLBACK lpCallback;
    LONG   lCallbackUserData;
    DWORD  dwReserved; 
} AUDIO_CAPTURE_INFO;

typedef struct _VIDEO_STREAM_INFO
{
    BOOL  bEnableMix;		
    DWORD dwWidth;
    DWORD dwHeight;
    DWORD dwPixelFormat;
    RECT  rcMixPosition;	
    DWORD dwTransparency;  
    DWORD dwReserved1;
    DWORD dwReserved2;
} VIDEO_STREAM_INFO;

typedef struct _WMV_VIDEOENCODER_INFO
{
    DWORD dwBitrate;
    DWORD dwQuality;
    DWORD dwReserved;
}WMV_VIDEOENCODER_INFO;

typedef struct _WMV_AUDIOENCODER_INFO
{
    DWORD dwChannels; 				
    DWORD dwBitsPerSample;			 
    DWORD dwSamplingRate; 
    DWORD dwBitrate;	
    DWORD dwReserved;
}WMV_AUDIOENCODER_INFO;

typedef struct _WAKEUP_TIME
{
    DWORD dwYear;
    DWORD dwMonth;
    DWORD dwDay;
    DWORD dwHour;
    DWORD dwMinute;
    DWORD dwSecond;
}WAKEUP_TIME;

typedef struct _MPEG2_VIDEOENCODER_INFO
{
    DWORD dwVersion;
    DWORD dwBitrate;
}MPEG2_VIDEOENCODER_INFO;

typedef struct _MPEG2_AUDIOENCODER_INFO
{
	DWORD dwVersion;
    DWORD dwBitrate;
}MPEG2_AUDIOENCODER_INFO;

typedef struct _MPEG4_VIDEOENCODER_INFO
{
	DWORD dwVersion;
	DWORD dwBitrate;
	DWORD dwGOPLength;
	BOOL  bHardwareAcceleration;
	DWORD dwQuality;
}MPEG4_VIDEOENCODER_INFO;

typedef struct _MPEG4_AUDIOENCODER_INFO
{
    DWORD dwVersion;
    DWORD dwBitrate;
}MPEG4_AUDIOENCODER_INFO;

typedef struct _INPUT_VIDEO_INFO
{
	DWORD dwVersion;   
	DWORD dwWidth;
	DWORD dwHeight;
	BOOL  bProgressive;
	DWORD dwFormat;
	DWORD dwFramerate;
}INPUT_VIDEO_INFO;

typedef struct _HW_VIDEOENCODER_INFO
{
	DWORD dwVersion;
	DWORD dwEncoderType;
	DWORD dwRcMode;
	DWORD dwBitrate;
	DWORD dwMinBitrate;
	DWORD dwMaxBitrate;
}HW_VIDEOENCODER_INFO;

typedef struct _HW_AUDIOENCODER_INFO
{
	DWORD dwVersion;
	DWORD dwEncoderType;
	DWORD dwBitrate;
	DWORD dwSamplingRate; 
}HW_AUDIOENCODER_INFO;

typedef struct _SAMPLE_INFO
{
	DWORD dwSampleType;
	LPVOID lpSampleInfo;	 
}SAMPLE_INFO;

typedef BOOL (WINAPI *CAPTURECALLBACK)(SAMPLE_INFO SampleInfo, BYTE *pbData, LONG lLength, __int64 tRefTime, LPVOID lpUserData); 

typedef struct _HW_STREAM_CAPTURE_INFO     
{
	DWORD dwVersion;
	DWORD dwCaptureType;
	CAPTURECALLBACK lpMainCallback; 
	CAPTURECALLBACK lpSecondaryCallback;
	LPWSTR lpFileName;
	LPVOID lpMainCallbackUserData;
	LPVOID lpSecondaryCallbackUserData;
}HW_STREAM_CAPTURE_INFO;


//third audio capture
typedef struct _INFO_DESCRIPTION
{   
	DWORD dwVersion; 
	WCHAR szName[256]; 
	DWORD dwIndex;
}AUDIOCAPTURESOURCE_INFO, AUDIOCAPTURESOURCE_INPUTTYPE_INFO, AUDIOCAPTURESOURCE_FORMAT_INFO;

typedef struct _AUDIOCAPTURESOURCE_SETTING
{
	DWORD dwVersion;
	DWORD dwCapSourceIndex;
	DWORD dwInputTypeIndex;
	DWORD dwFormatIndex;
}AUDIOCAPTURESOURCE_SETTING;

typedef struct _OVERLAY_CONTENT_INFO
{
	DWORD dwVersion;
	DWORD dwContentType;
	LPVOID lpContent;
	DWORD dwDuration;
	DWORD dwID;
	DWORD dwPriority;
	OVERLAY_INFO OverlayInfo;
}OVERLAY_CONTENT_INFO;

typedef struct _OVERLAY_DC_INFO 
{
	DWORD dwVersion;
	BOOL bClear; 
	HDC  hDC;
	DWORD dwDCWidth;
	DWORD dwDCHeight;
	DWORD dwBkColor; 
	DWORD dwBkTransparency;
	OVERLAY_POSITION WindowPosition;
}OVERLAY_DC_INFO;

#ifdef __cplusplus
extern "C"
{
#endif

LONG WINAPI AVerGetDeviceNum(DWORD *pdwDeviceNum);
LONG WINAPI AVerGetDeviceName(DWORD dwDeviceIndex, LPWSTR szDeviceName);

LONG WINAPI AVerCreateCaptureObject(DWORD dwDeviceIndex, HWND hWnd, HANDLE *phCaptureObject);
LONG WINAPI AVerCreateCaptureObjectEx(DWORD dwDeviceIndex, DWORD dwType, HWND hWnd, HANDLE *phCaptureObject);
LONG WINAPI AVerDeleteCaptureObject(HANDLE hCaptureObject);

LONG WINAPI AVerSetVideoWindowPosition(HANDLE hCaptureObject, RECT rectVideoWnd);
LONG WINAPI AVerRepaintVideo(HANDLE hCaptureObject);

LONG WINAPI AVerSetVideoRenderer(HANDLE hCaptureObject, DWORD dwVideoRenderer);
LONG WINAPI AVerGetVideoRenderer(HANDLE hCaptureObject, DWORD *pdwVideoRenderer);

LONG WINAPI AVerSetMaintainAspectRatioEnabled(HANDLE hCaptureObject, BOOL bEnabled);
LONG WINAPI AVerGetMaintainAspectRatioEnabled(HANDLE hCaptureObject, BOOL *pbEnabled);
LONG WINAPI AVerGetAspectRatio(HANDLE hCaptureObject, DWORD *pdwAspectRatioX, DWORD *pdwAspectRatioY);

LONG WINAPI AVerSetVideoSource(HANDLE hCaptureObject, DWORD dwVideoSource);
LONG WINAPI AVerGetVideoSource(HANDLE hCaptureObject, DWORD *pdwVideoSource);

LONG WINAPI AVerSetVideoFormat(HANDLE hCaptureObject, DWORD dwVideoFormat);
LONG WINAPI AVerGetVideoFormat(HANDLE hCaptureObject, DWORD *pdwVideoFormat);

LONG WINAPI AVerSetVideoResolution(HANDLE hCaptureObject, DWORD dwVideoResolution);
LONG WINAPI AVerGetVideoResolution(HANDLE hCaptureObject, DWORD *pdwVideoResolution);

LONG WINAPI AVerSetVideoInputFrameRate(HANDLE hCaptureObject, DWORD dwFrameRate);
LONG WINAPI AVerGetVideoInputFrameRate(HANDLE hCaptureObject, DWORD *pdwFrameRate);

LONG WINAPI AVerSetAudioSamplingRate(HANDLE hCaptureObject, DWORD dwSamplingRate);
LONG WINAPI AVerGetAudioSamplingRate(HANDLE hCaptureObject, DWORD *pdwSamplingRate);

LONG WINAPI AVerStartStreaming(HANDLE hCaptureObject);
LONG WINAPI AVerStopStreaming(HANDLE hCaptureObject);

LONG WINAPI AVerSetVideoProcAmp(HANDLE hCaptureObject, DWORD dwVideoProcAmpProperty, DWORD dwPropertyValue);
LONG WINAPI AVerGetVideoProcAmp(HANDLE hCaptureObject, DWORD dwVideoProcAmpProperty, DWORD *pdwPropertyValue);

LONG WINAPI AVerSetDeinterlaceMode(HANDLE hCaptureObject, DWORD dwMode);
LONG WINAPI AVerGetDeinterlaceMode(HANDLE hCaptureObject, DWORD *pdwMode);

LONG WINAPI AVerSetVideoDownscaleMode(HANDLE hCaptureObject, DWORD dwMode, DWORD dwWidth, DWORD dwHeight);
LONG WINAPI AVerGetVideoDownscaleMode(HANDLE hCaptureObject, DWORD *pdwMode, DWORD *pdwWidth, DWORD *pdwHeight);

LONG WINAPI AVerCaptureAudioSampleStart(HANDLE hCaptureObject, AUDIOCAPTURECALLBACK lpCallback, LONG lCallbackUserData);
LONG WINAPI AVerCaptureAudioSampleStartEx(HANDLE hCaptureObject, AUDIO_CAPTURE_INFO CaptureInfo);
LONG WINAPI AVerCaptureAudioSampleStop(HANDLE hCaptureObject);

LONG WINAPI AVerCaptureSingleImageToBuffer(HANDLE hCaptureObject, BYTE *lpBmpData, LONG *plBufferSize, BOOL bOverlayMix, DWORD dwTimeout);

LONG WINAPI AVerSetOverlayProperty(HANDLE hCaptureObject, DWORD dwContentType, OVERLAY_INFO OverlayInfo);
LONG WINAPI AVerGetOverlayProperty(HANDLE hCaptureObject, DWORD dwContentType, OVERLAY_INFO *pOverlayInfo);

LONG WINAPI AVerOverlayText(HANDLE hCaptureObject, LPWSTR lpText, DWORD dwDuration);
LONG WINAPI AVerOverlayTime(HANDLE hCaptureObject, DWORD dwFormat, DWORD dwDuration); 
LONG WINAPI AVerOverlayImage(HANDLE hCaptureObject, OVERLAY_IMAGE_INFO ImageInfo, DWORD dwDuration);

LONG WINAPI AVerOverlayMediaContent(HANDLE hCaptureObject, OVERLAY_CONTENT_INFO* pContentInfo);
LONG WINAPI AVerOverlayDC(HANDLE hCaptureObject, OVERLAY_DC_INFO* pOverlayDCInfo);

LONG WINAPI AVerCaptureVideoSequenceStart(HANDLE hCaptureObject, VIDEO_CAPTURE_INFO CaptureInfo);
LONG WINAPI AVerCaptureVideoSequenceStop(HANDLE hCaptureObject);

LONG WINAPI AVerGetHDMIVideoInfo(HANDLE hCaptureObject, DWORD *pdwWidth, DWORD *pdwHeight, BOOL *pbProgressive, DWORD *pdwVideoFormat);
LONG WINAPI AVerGetHDMIAudioInfo(HANDLE hCaptureObject, DWORD *pdwSamplingRate);
LONG WINAPI AVerGetVideoInfo(HANDLE hCaptureObject, INPUT_VIDEO_INFO *pVideoInfo);
LONG WINAPI AVerGetComponentVideoInfo(HANDLE hCaptureObject, DWORD *pdwWidth, DWORD *pdwHeight, BOOL *pbProgressive, DWORD *pdwVideoFormat);
LONG WINAPI AVerGetMacroVisionMode(HANDLE hCaptureObject, DWORD *pdwMode);
LONG WINAPI AVerGetSignalPresence(HANDLE hCaptureObject, BOOL *pbSignalPresence);

LONG WINAPI AVerSetNoiseReductionEnabled(HANDLE hCaptureObject, BOOL bEnabled);
LONG WINAPI AVerGetNoiseReductionEnabled(HANDLE hCaptureObject, BOOL *pbEnabled);

LONG WINAPI AVerSetVideoOutputFrameRate(HANDLE hCaptureObject, DWORD dwFrameRate);
LONG WINAPI AVerGetVideoOutputFrameRate(HANDLE hCaptureObject, DWORD *pdwFrameRate);

LONG WINAPI AVerSetVideoPreviewEnabled(HANDLE hCaptureObject, BOOL bEnabled);
LONG WINAPI AVerGetVideoPreviewEnabled(HANDLE hCaptureObject, BOOL *pbEnabled);

LONG WINAPI AVerSetAudioPreviewEnabled(HANDLE hCaptureObject, BOOL bEnabled);
LONG WINAPI AVerGetAudioPreviewEnabled(HANDLE hCaptureObject, BOOL *pbEnabled);

LONG WINAPI AVerSetAudioRecordEnabled(HANDLE hCaptureObject, BOOL bEnabled);
LONG WINAPI AVerGetAudioRecordEnabled(HANDLE hCaptureObject, BOOL *pbEnabled);

LONG WINAPI AVerEnumVideoCompressor(HANDLE hCaptureObject, VIDEO_COMPRESSOR_INFO *pVideoCompressorInfo, DWORD *pdwNum);
LONG WINAPI AVerSetVideoCompressorInfo(HANDLE hCaptureObject, VIDEO_COMPRESSOR_INFO *pVideoCompressorInfo);
LONG WINAPI AVerGetVideoCompressorInfo(HANDLE hCaptureObject, VIDEO_COMPRESSOR_INFO *pVideoCompressorInfo);

LONG WINAPI AVerEnumAudioCompressor(HANDLE hCaptureObject, AUDIO_COMPRESSOR_INFO *pAudioCompressorInfo, DWORD *pdwNum);
LONG WINAPI AVerSetAudioCompressorInfo(HANDLE hCaptureObject, AUDIO_COMPRESSOR_INFO *pAudioCompressorInfo);
LONG WINAPI AVerGetAudioCompressorInfo(HANDLE hCaptureObject, AUDIO_COMPRESSOR_INFO *pAudioCompressorInfo);
LONG WINAPI AVerEnumSupportedAudioSampleFormat(HANDLE hCaptureObject, AUDIO_COMPRESSOR_INFO *pAudioCompressorInfo, AUDIO_SAMPLE_FORMAT *pAudioSampleFormat, DWORD *pdwNum);

LONG WINAPI AVerSetVideoStreamMixingProperty(HANDLE hCaptureObject, DWORD dwStreamID, VIDEO_STREAM_INFO VideoStreamInfo);
LONG WINAPI AVerGetVideoStreamMixingProperty(HANDLE hCaptureObject, DWORD dwStreamID, VIDEO_STREAM_INFO *pVideoStreamInfo);
LONG WINAPI AVerMixVideoStream(HANDLE hCaptureObject, DWORD dwStreamID, BYTE *pData, DWORD dwStride, LONG lLength, __int64 tRefTime, DWORD dwOptions);

LONG WINAPI AVerSetWMVVideoEncoderInfo(HANDLE hCaptureObject, WMV_VIDEOENCODER_INFO VideoEncoderInfo);
LONG WINAPI AVerGetWMVVideoEncoderInfo(HANDLE hCaptureObject, WMV_VIDEOENCODER_INFO *pVideoEncoderInfo);

LONG WINAPI AVerSetWMVAudioEncoderInfo(HANDLE hCaptureObject, WMV_AUDIOENCODER_INFO AudioEncoderInfo);
LONG WINAPI AVerGetWMVAudioEncoderInfo(HANDLE hCaptureObject, WMV_AUDIOENCODER_INFO *pAudioEncoderInfo);

LONG WINAPI AVerSetVideoEnhanceMode(HANDLE hCaptureObject, DWORD dwMode);
LONG WINAPI AVerGetVideoEnhanceMode(HANDLE hCaptureObject, DWORD *pdwMode);

LONG WINAPI AVerSetVideoClippingRect(HANDLE hCaptureObject, RECT rcClippingRect);
LONG WINAPI AVerGetVideoClippingRect(HANDLE hCaptureObject, RECT *prcClippingRect);

LONG WINAPI AVerSetVideoMirrorMode(HANDLE hCaptureObject, DWORD dwMode);
LONG WINAPI AVerGetVideoMirrorMode(HANDLE hCaptureObject, DWORD *pdwMode);

LONG WINAPI AVerSetS5WakeUpTime(HANDLE hCaptureObject, WAKEUP_TIME WakeUpTime);
LONG WINAPI AVerGetS5WakeUpTime(HANDLE hCaptureObject, WAKEUP_TIME *pWakeUpTime);

LONG WINAPI AVerSetMpeg2VideoEncoderInfo(HANDLE hCaptureObject, MPEG2_VIDEOENCODER_INFO *pVideoEncoderInfo);
LONG WINAPI AVerGetMpeg2VideoEncoderInfo(HANDLE hCaptureObject, MPEG2_VIDEOENCODER_INFO *pVideoEncoderInfo);

LONG WINAPI AVerSetMpeg2AudioEncoderInfo(HANDLE hCaptureObject, MPEG2_AUDIOENCODER_INFO *pAudioEncoderInfo);
LONG WINAPI AVerGetMpeg2AudioEncoderInfo(HANDLE hCaptureObject, MPEG2_AUDIOENCODER_INFO *pAudioEncoderInfo);

LONG WINAPI AVerGetHardwareVideoAccelerationCapabilities(HANDLE hCaptureObject, DWORD *pdwCapabilities);
LONG WINAPI AVerSetMpeg4VideoEncoderInfo(HANDLE hCaptureObject, MPEG4_VIDEOENCODER_INFO *pVideoEncoderInfo);
LONG WINAPI AVerGetMpeg4VideoEncoderInfo(HANDLE hCaptureObject, MPEG4_VIDEOENCODER_INFO *pVideoEncoderInfo);

LONG WINAPI AVerSetMpeg4AudioEncoderInfo(HANDLE hCaptureObject, MPEG4_AUDIOENCODER_INFO *pAudioEncoderInfo);
LONG WINAPI AVerGetMpeg4AudioEncoderInfo(HANDLE hCaptureObject, MPEG4_AUDIOENCODER_INFO *pAudioEncoderInfo);

LONG WINAPI AVerSetVideoRotateMode(HANDLE hCaptureObject, DWORD dwMode);
LONG WINAPI AVerGetVideoRotateMode(HANDLE hCaptureObject, DWORD *pdwMode);

LONG WINAPI AVerSetVideoUpscaleBlackRect(HANDLE hCaptureObject, RECT rcUpscaleRect);
LONG WINAPI AVerGetVideoUpscaleBlackRect(HANDLE hCaptureObject, RECT *prcUpscaleRect);

LONG WINAPI AVerHwSetVideoEncoderInfo(HANDLE hCaptureObject, HW_VIDEOENCODER_INFO *pVideoEncoderInfo);
LONG WINAPI AVerHwGetVideoEncoderInfo(HANDLE hCaptureObject, HW_VIDEOENCODER_INFO *pVideoEncoderInfo);

LONG WINAPI AVerHwSetAudioEncoderInfo(HANDLE hCaptureObject, HW_AUDIOENCODER_INFO *pAudioEncoderInfo);
LONG WINAPI AVerHwGetAudioEncoderInfo(HANDLE hCaptureObject, HW_AUDIOENCODER_INFO *pAudioEncoderInfo);

LONG WINAPI AVerHwCaptureStreamStart(HANDLE hCaptureObject, HW_STREAM_CAPTURE_INFO *pCaptureInfo);
LONG WINAPI AVerHwCaptureStreamStop(HANDLE hCaptureObject);

LONG WINAPI AVerHwSetVolume(HANDLE hCaptureObject, DWORD dwVolume);
LONG WINAPI AVerHwGetVolume(HANDLE hCaptureObject, DWORD *pdwVolume);
LONG WINAPI AVerHwGetVolumeRange(HANDLE hCaptureObject, DWORD *pdwMinVolume, DWORD *pdwMaxVolume, DWORD *pdwDefVolume);

LONG WINAPI AVerEnumThirdPartyAudioCapSource(HANDLE hCaptureObject, AUDIOCAPTURESOURCE_INFO *pAudioCapSourceInfo, DWORD *pdwNum);
LONG WINAPI AVerEnumThirdPartyAudioCapSourceInputType(HANDLE hCaptureObject, DWORD dwCapIndex, AUDIOCAPTURESOURCE_INPUTTYPE_INFO *pInputTypeInfo, DWORD *pdwNum);
LONG WINAPI AVerEnumThirdPartyAudioCapSourceSampleFormat(HANDLE hCaptureObject, DWORD dwCapIndex, AUDIOCAPTURESOURCE_FORMAT_INFO *pFormatInfo, DWORD *pdwNum);

LONG WINAPI AVerSetThirdPartyAudioCapSource(HANDLE hCaptureObject, AUDIOCAPTURESOURCE_SETTING *pAudioCapSourceSetting);
LONG WINAPI AVerGetThirdPartyAudioCapSource(HANDLE hCaptureObject, AUDIOCAPTURESOURCE_SETTING *pAudioCapSourceSetting);

#ifdef __cplusplus
}
#endif

#endif