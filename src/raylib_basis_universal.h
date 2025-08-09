#pragma once

#include <raylib.h>

#ifdef __cplusplus
extern "C" {
#endif

Image LoadBasisUniversalImage(const char *fileName);
Image LoadBasisUniversalImageFromMemory(const char *fileType, const unsigned char *fileData, int dataSize);

Texture LoadBasisUniversalTexture(const char *fileName);

#ifdef __cplusplus
}
#endif
