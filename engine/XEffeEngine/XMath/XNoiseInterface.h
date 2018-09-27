#ifndef _JIA_XNOISEINTERFACE_
#define _JIA_XNOISEINTERFACE_
#include "XNoise.h"
namespace XE {
//[0 - 1]
inline float noise(float x) { return _slang_library_noise1(x) * 0.5f + 0.5f; }
inline float noise(float x, float y) { return _slang_library_noise2(x, y) * 0.5f + 0.5f; }
inline float noise(float x, float y, float z) { return _slang_library_noise3(x, y, z) * 0.5f + 0.5f; }
inline float noise(float x, float y, float z, float w) { return _slang_library_noise4(x, y, z, w) * 0.5f + 0.5f; }
inline float signedNoise(float x) { return _slang_library_noise1(x); }
inline float signedNoise(float x, float y) { return _slang_library_noise2(x, y); }
inline float signedNoise(float x, float y, float z) { return _slang_library_noise3(x, y, z); }
inline float signedNoise(float x, float y, float z, float w) { return _slang_library_noise4(x, y, z, w); }
}
#endif