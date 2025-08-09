#include "raylib_basis_universal.h"

#include "external/basis_universal/transcoder/basisu_transcoder.h"
#include "raylib.h"

#include <cstdint>
#include <rlgl.h>

static bool is_basisu_initialized = false;

#if defined(__EMSCRIPTEN__)  // use uncompressed format in web since we could be running on either mobile or desktop
basist::transcoder_texture_format rgba_texture_format = basist::transcoder_texture_format::cTFRGBA4444;
basist::transcoder_texture_format rgb_texture_format = basist::transcoder_texture_format::cTFRGB565;
#elif defined(__ANDROID__)
basist::transcoder_texture_format rgba_texture_format = basist::transcoder_texture_format::cTFETC2_RGBA;
basist::transcoder_texture_format rgb_texture_format = basist::transcoder_texture_format::cTFETC1_RGB;
#else
basist::transcoder_texture_format rgba_texture_format = basist::transcoder_texture_format::cTFBC3_RGBA;
basist::transcoder_texture_format rgb_texture_format = basist::transcoder_texture_format::cTFBC1_RGB;
#endif

static basist::transcoder_texture_format get_wanted_format(bool has_alpha) {
	if (has_alpha) {
		return rgba_texture_format;
	}
	else {
		return rgb_texture_format;
	}
}

static rlPixelFormat to_rlPixelFormat(basist::transcoder_texture_format transcoder_format) {
	switch (transcoder_format) {
		case basist::transcoder_texture_format::cTFETC1_RGB: return RL_PIXELFORMAT_COMPRESSED_ETC1_RGB;
		case basist::transcoder_texture_format::cTFETC2_RGBA: return RL_PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA;
		case basist::transcoder_texture_format::cTFBC1_RGB: return RL_PIXELFORMAT_COMPRESSED_DXT1_RGB;
		case basist::transcoder_texture_format::cTFBC3_RGBA: return RL_PIXELFORMAT_COMPRESSED_DXT5_RGBA;
		case basist::transcoder_texture_format::cTFPVRTC1_4_RGB: return RL_PIXELFORMAT_COMPRESSED_PVRT_RGB;
		case basist::transcoder_texture_format::cTFPVRTC1_4_RGBA: return RL_PIXELFORMAT_COMPRESSED_PVRT_RGBA;
		case basist::transcoder_texture_format::cTFASTC_4x4_RGBA: return RL_PIXELFORMAT_COMPRESSED_ASTC_4x4_RGBA;
		case basist::transcoder_texture_format::cTFRGBA32: return RL_PIXELFORMAT_UNCOMPRESSED_R32G32B32A32;
		case basist::transcoder_texture_format::cTFRGB565: return RL_PIXELFORMAT_UNCOMPRESSED_R5G6B5;
		case basist::transcoder_texture_format::cTFRGBA4444: return RL_PIXELFORMAT_UNCOMPRESSED_R4G4B4A4;
		case basist::transcoder_texture_format::cTFRGB_HALF: return RL_PIXELFORMAT_UNCOMPRESSED_R16G16B16;
		case basist::transcoder_texture_format::cTFRGBA_HALF: return RL_PIXELFORMAT_UNCOMPRESSED_R16G16B16A16;
		default: return RL_PIXELFORMAT_UNCOMPRESSED_R32G32B32A32;
	}
}

static basist::transcoder_texture_format to_transcoder_texture_format(PixelFormat format) {
	switch (format) {
		case PIXELFORMAT_COMPRESSED_ETC1_RGB: return basist::transcoder_texture_format::cTFETC1_RGB;
		case PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA: return basist::transcoder_texture_format::cTFETC2_RGBA;
		case PIXELFORMAT_COMPRESSED_DXT1_RGB: return basist::transcoder_texture_format::cTFBC1_RGB;
		case PIXELFORMAT_COMPRESSED_DXT5_RGBA: return basist::transcoder_texture_format::cTFBC3_RGBA;
		case PIXELFORMAT_COMPRESSED_PVRT_RGB: return basist::transcoder_texture_format::cTFPVRTC1_4_RGB;
		case PIXELFORMAT_COMPRESSED_PVRT_RGBA: return basist::transcoder_texture_format::cTFPVRTC1_4_RGBA;
		case PIXELFORMAT_COMPRESSED_ASTC_4x4_RGBA: return basist::transcoder_texture_format::cTFASTC_4x4_RGBA;
		case PIXELFORMAT_UNCOMPRESSED_R32G32B32A32: return basist::transcoder_texture_format::cTFRGBA32;
		case PIXELFORMAT_UNCOMPRESSED_R5G6B5: return basist::transcoder_texture_format::cTFRGB565;
		case PIXELFORMAT_UNCOMPRESSED_R4G4B4A4: return basist::transcoder_texture_format::cTFRGBA4444;
		case PIXELFORMAT_UNCOMPRESSED_R16G16B16: return basist::transcoder_texture_format::cTFRGB_HALF;
		case PIXELFORMAT_UNCOMPRESSED_R16G16B16A16: return basist::transcoder_texture_format::cTFRGBA_HALF;
		default: return basist::transcoder_texture_format::cTFRGBA32;
	}
}

static void *load_basis_from_memory(const unsigned char *data, unsigned int data_size, int *width, int *height, int *format, int *mips) {
	basist::basisu_transcoder transcoder;
	basist::basisu_image_info image_info;
	if (!transcoder.get_image_info(data, data_size, image_info, 0) || !transcoder.start_transcoding(data, data_size)) {
		return nullptr;
	}
	
	basist::transcoder_texture_format output_format = get_wanted_format(image_info.m_alpha_flag);
	bool output_format_is_uncompressed = basist::basis_transcoder_format_is_uncompressed(output_format);
	uint32_t bytes_per_block_or_pixel = basist::basis_get_bytes_per_block_or_pixel(output_format);
	uint32_t total_blocks_or_pixels = 0;
	void *image_data = nullptr;
	unsigned char *level_data = nullptr;
	for (int i = 0; i < image_info.m_total_levels; i++) {
		basist::basisu_image_level_info level_info;
		transcoder.get_image_level_info(data, data_size, level_info, 0, i);
		uint32_t blocks_or_pixels = output_format_is_uncompressed ? level_info.m_width * level_info.m_height : level_info.m_total_blocks;
		total_blocks_or_pixels += blocks_or_pixels;
		if (void *new_image_data = RL_REALLOC(image_data, total_blocks_or_pixels * bytes_per_block_or_pixel)) {
			image_data = new_image_data;
		}
		else {
			if (image_data) {
				RL_FREE(image_data);
			}
			return nullptr;
		}
		if (!level_data) {
			level_data = (unsigned char *) image_data;
		}
		transcoder.transcode_image_level(data, data_size, 0, i, level_data, blocks_or_pixels, output_format);
		level_data += blocks_or_pixels * bytes_per_block_or_pixel;
	}
	transcoder.stop_transcoding();
	
	*height = image_info.m_height;
	*width = image_info.m_width;
	*format = to_rlPixelFormat(output_format);
	*mips = image_info.m_total_levels;
	return image_data;
}

#if BASISD_SUPPORT_KTX2
static void *load_ktx2_from_memory(const unsigned char *data, unsigned int data_size, int *width, int *height, int *format, int *mips) {
	basist::ktx2_transcoder transcoder;
	if (!transcoder.init(data, data_size) || !transcoder.start_transcoding()) {
		return nullptr;
	}

	basist::transcoder_texture_format output_format = get_wanted_format(transcoder.get_has_alpha());
	bool output_format_is_uncompressed = basist::basis_transcoder_format_is_uncompressed(output_format);
	uint32_t bytes_per_block_or_pixel = basist::basis_get_bytes_per_block_or_pixel(output_format);
	uint32_t total_blocks_or_pixels = 0;
	void *image_data = nullptr;
	unsigned char *level_data = nullptr;
	for (int i = 0; i < transcoder.get_levels(); i++) {
		basist::ktx2_image_level_info level_info;
		transcoder.get_image_level_info(level_info, i, 0, 0);
		uint32_t blocks_or_pixels = output_format_is_uncompressed ? level_info.m_width * level_info.m_height : level_info.m_total_blocks;
		total_blocks_or_pixels += blocks_or_pixels;
		if (void *new_image_data = RL_REALLOC(image_data, total_blocks_or_pixels * bytes_per_block_or_pixel)) {
			image_data = new_image_data;
		}
		else {
			if (image_data) {
				RL_FREE(image_data);
			}
			return nullptr;
		}
		if (!level_data) {
			level_data = (unsigned char *) image_data;
		}
		transcoder.transcode_image_level(i,0, 0, level_data, blocks_or_pixels, output_format);
		level_data += blocks_or_pixels * bytes_per_block_or_pixel;
	}
	*height = transcoder.get_height();
	*width = transcoder.get_width();
	*format = to_rlPixelFormat(output_format);
	*mips = transcoder.get_levels();
	return image_data;
}
#endif  // BASISD_SUPPORT_KTX2

#ifdef __cplusplus
extern "C" {
#endif

Image LoadBasisUniversalImage(const char *fileName) {
	Image image = { 0 };

	// Loading file to memory
	int dataSize = 0;
	unsigned char *fileData = LoadFileData(fileName, &dataSize);

	// Loading image from memory data
	if (fileData != nullptr)
	{
		image = LoadBasisUniversalImageFromMemory(GetFileExtension(fileName), fileData, dataSize);
		UnloadFileData(fileData);
	}

	return image;
}

Image LoadBasisUniversalImageFromMemory(const char *fileType, const unsigned char *fileData, int dataSize) {
	Image image = { 0 };

	// Security check for input data
	if ((fileData == NULL) || (dataSize == 0) || (fileType == NULL))
	{
		return image;
	}

	if (!is_basisu_initialized) {
		basist::basisu_transcoder_init();
		is_basisu_initialized = true;
	}

	if ((strcmp(fileType, ".basis") == 0) || (strcmp(fileType, ".BASIS") == 0)) {
		image.data = load_basis_from_memory(fileData, dataSize, &image.width, &image.height, &image.format, &image.mipmaps);
	}
#if BASISD_SUPPORT_KTX2
	else if ((strcmp(fileType, ".ktx2") == 0) || (strcmp(fileType, ".KTX2") == 0)) {
		image.data = load_ktx2_from_memory(fileData, dataSize, &image.width, &image.height, &image.format, &image.mipmaps);
	}
#endif  // BASISD_SUPPORT_KTX2

	return image;
}

Texture LoadBasisUniversalTexture(const char *fileName) {
	Texture2D texture = { 0 };
	Image image = LoadBasisUniversalImage(fileName);
	if (image.data != nullptr)
	{
		texture = LoadTextureFromImage(image);
		UnloadImage(image);
	}
	return texture;
}

void ConfigureRgbTextureFormat(PixelFormat pixelFormat) {
	rgb_texture_format = to_transcoder_texture_format(pixelFormat);
}

void ConfigureRgbaTextureFormat(PixelFormat pixelFormat) {
	rgba_texture_format = to_transcoder_texture_format(pixelFormat);
}

#ifdef __cplusplus
}
#endif
