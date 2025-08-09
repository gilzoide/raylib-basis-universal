/**
 * @file raylib_basis_universal.h  Load Basis Universal encoded .basis or .ktx2 textures using raylib
 *
 * @see https://github.com/raysan5/raylib
 * @see https://github.com/BinomialLLC/basis_universal
 */
#pragma once

#include <raylib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Load a raylib Image from Basis Universal encoded files.
 *
 * @param fileName  File name. Must have the extension ".basis" or ".ktx2"
 */
Image LoadBasisUniversalImage(const char *fileName);

/**
 * Load a raylib Image from Basis Universal encoded data.
 *
 * @param fileType  Must be either ".basis" or ".ktx2"
 * @param fileData  Raw file data. If you pass `NULL`, the returned Image will be empty.
 * @param dataSize  Size of the file data. If you pass 0, the returned Image will be empty.
 */
Image LoadBasisUniversalImageFromMemory(const char *fileType, const unsigned char *fileData, int dataSize);

/**
 * Load a raylib Texture from Basis Universal encoded files.
 *
 * @param fileName  File name. Must have the extension ".basis" or ".ktx2"
 */
Texture LoadBasisUniversalTexture(const char *fileName);

#ifdef __cplusplus
}
#endif
