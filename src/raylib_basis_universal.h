/**
 * @file raylib_basis_universal.h  Load Basis Universal encoded .basis or .ktx2 textures using raylib
 *
 * @see https://github.com/raysan5/raylib
 * @see https://github.com/BinomialLLC/basis_universal
 */
#ifndef __RAYLIB_BASIS_UNIVERSAL_H__
#define __RAYLIB_BASIS_UNIVERSAL_H__

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

/**
 * Configure which pixel format will be used for RGB (opaque) textures when loading Basis Universal images.
 *
 * By default, PIXELFORMAT_COMPRESSED_DXT1_RGB is used on Desktop, PIXELFORMAT_COMPRESSED_ETC1_RGB on Android and PIXELFORMAT_UNCOMPRESSED_R5G6B5 on Web.
 *
 * @note Only supported formats are:
 * - PIXELFORMAT_COMPRESSED_ETC1_RGB
 * - PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA
 * - PIXELFORMAT_COMPRESSED_DXT1_RGB
 * - PIXELFORMAT_COMPRESSED_DXT5_RGBA
 * - PIXELFORMAT_COMPRESSED_PVRT_RGB
 * - PIXELFORMAT_COMPRESSED_PVRT_RGBA
 * - PIXELFORMAT_COMPRESSED_ASTC_4x4_RGBA
 * - PIXELFORMAT_UNCOMPRESSED_R32G32B32A32
 * - PIXELFORMAT_UNCOMPRESSED_R5G6B5
 * - PIXELFORMAT_UNCOMPRESSED_R4G4B4A4
 * - PIXELFORMAT_UNCOMPRESSED_R16G16B16
 * - PIXELFORMAT_UNCOMPRESSED_R16G16B16A16
 */
void SetBasisUniversalRgbPixelFormat(PixelFormat pixelFormat);

/**
 * Configure which pixel format will be used for RGBA (transparent) textures when loading Basis Universal images.
 *
 * By default, PIXELFORMAT_COMPRESSED_DXT5_RGBA is used on Desktop, PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA on Android and PIXELFORMAT_UNCOMPRESSED_R4G4B4A4 on Web.
 *
 * @note Only supported formats are:
 * - PIXELFORMAT_COMPRESSED_ETC1_RGB
 * - PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA
 * - PIXELFORMAT_COMPRESSED_DXT1_RGB
 * - PIXELFORMAT_COMPRESSED_DXT5_RGBA
 * - PIXELFORMAT_COMPRESSED_PVRT_RGB
 * - PIXELFORMAT_COMPRESSED_PVRT_RGBA
 * - PIXELFORMAT_COMPRESSED_ASTC_4x4_RGBA
 * - PIXELFORMAT_UNCOMPRESSED_R32G32B32A32
 * - PIXELFORMAT_UNCOMPRESSED_R5G6B5
 * - PIXELFORMAT_UNCOMPRESSED_R4G4B4A4
 * - PIXELFORMAT_UNCOMPRESSED_R16G16B16
 * - PIXELFORMAT_UNCOMPRESSED_R16G16B16A16
 */
void SetBasisUniversalRgbaPixelFormat(PixelFormat pixelFormat);

#ifdef __cplusplus
}
#endif

#endif  // __RAYLIB_BASIS_UNIVERSAL_H__
