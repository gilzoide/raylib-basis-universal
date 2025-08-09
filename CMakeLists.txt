cmake_minimum_required(VERSION 3.18)

project(raylib_basis_universal C CXX)

option(RAYLIB_BASIS_UNIVERSAL_SHARED "Build raylib_basis_universal as a shared library" ${BUILD_SHARED_LIBS})

option(BASISU_FORCE_DEVEL_MESSAGES "Enable debug printf()'s whenever an error occurs, for easier debugging during development" OFF)
option(BASISD_SUPPORT_KTX2 "Enable KTX2 support as well as Zstd/miniz usage which is only required for UASTC supercompression in KTX2 files" ON)
option(BASISD_SUPPORT_KTX2_ZSTD "Enable KTX2 UASTC Zstd supercompression support " ON)

option(BASISD_SUPPORT_DXT1 "Enable support for DXT1 (BC1) texture format" ON)
option(BASISD_SUPPORT_DXT5A "Enable support for DXT5A (BC3/4/5) texture format" ON)
option(BASISD_SUPPORT_BC7 "Enable support for BC7 texture format" OFF)
option(BASISD_SUPPORT_BC7_MODE5 "Enable support for BC7 mode 5 texture format. Supports both opaque and opaque+alpha textures, and uses less memory than BC1." OFF)
option(BASISD_SUPPORT_PVRTC1 "Enable support for PVRTC1 texture format" OFF)
option(BASISD_SUPPORT_ETC2_EAC_A8 "Enable support for ETC2_EAC_A8 texture format" ON)
option(BASISD_SUPPORT_ASTC "Enable support for ASTC texture format" OFF)
option(BASISD_SUPPORT_ATC "Enable support for ATC texture format" OFF)
option(BASISD_SUPPORT_ASTC_HIGHER_OPAQUE_QUALITY "Enable support for ASTC where opaque blocks will be transcoded at slightly higher quality (higher than BC1), but the transcoder tables will be 2x as large. This impacts grayscale and grayscale+alpha textures the most." ON)
option(BASISD_SUPPORT_ETC2_EAC_RG11 "Enable support for ETC2 EAC R11 and ETC2 EAC RG11 texture formats" OFF)
option(BASISD_SUPPORT_FXT1 "Enable support for FXT1 texture format" OFF)
option(BASISD_SUPPORT_PVRTC2 "Enable support for PVRTC2 texture format" OFF)
option(BASISD_SUPPORT_UASTC "Enable support for UASTC texture format" OFF)
option(BASISD_SUPPORT_UASTC_HDR "Enable support for UASTC HDR texture format" OFF)

if(RAYLIB_BASIS_UNIVERSAL_SHARED)
  set(RAYLIB_BASIS_UNIVERSAL_LIBRARY_TYPE SHARED)
else()
  set(RAYLIB_BASIS_UNIVERSAL_LIBRARY_TYPE STATIC)
endif()

set(RAYLIB_BASIS_UNIVERSAL_SOURCES "src/raylib_basis_universal.cpp" "src/external/basis_universal/transcoder/basisu_transcoder.cpp")
if(BASISD_SUPPORT_KTX2 AND BASISD_SUPPORT_KTX2_ZSTD)
  set(RAYLIB_BASIS_UNIVERSAL_SOURCES ${RAYLIB_BASIS_UNIVERSAL_SOURCES} "src/external/basis_universal/zstd/zstddeclib.c")
endif()
add_library(raylib_basis_universal ${RAYLIB_BASIS_UNIVERSAL_LIBRARY_TYPE} ${RAYLIB_BASIS_UNIVERSAL_SOURCES})
target_compile_features(raylib_basis_universal PRIVATE cxx_std_17)
target_include_directories(raylib_basis_universal PUBLIC src)
# Avoid "unknown warning group '-Wclass-memaccess', ignored" warnings on clang
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  target_compile_options(raylib_basis_universal PRIVATE -Wno-unknown-warning-option)
endif()
target_compile_definitions(raylib_basis_universal
  PRIVATE
    -DBASISU_FORCE_DEVEL_MESSAGES=$<BOOL:${BASISU_FORCE_DEVEL_MESSAGES}>
    -DBASISD_SUPPORT_KTX2=$<BOOL:${BASISD_SUPPORT_KTX2}>
    -DBASISD_SUPPORT_KTX2_ZSTD=$<BOOL:${BASISD_SUPPORT_KTX2_ZSTD}>

    -DBASISD_SUPPORT_DXT1=$<BOOL:${BASISD_SUPPORT_DXT1}>
    -DBASISD_SUPPORT_DXT5A=$<BOOL:${BASISD_SUPPORT_DXT5A}>
    -DBASISD_SUPPORT_BC7=$<BOOL:${BASISD_SUPPORT_BC7}>
    -DBASISD_SUPPORT_BC7_MODE5=$<BOOL:${BASISD_SUPPORT_BC7_MODE5}>
    -DBASISD_SUPPORT_PVRTC1=$<BOOL:${BASISD_SUPPORT_PVRTC1}>
    -DBASISD_SUPPORT_ETC2_EAC_A8=$<BOOL:${BASISD_SUPPORT_ETC2_EAC_A8}>
    -DBASISD_SUPPORT_ASTC=$<BOOL:${BASISD_SUPPORT_ASTC}>
    -DBASISD_SUPPORT_ATC=$<BOOL:${BASISD_SUPPORT_ATC}>
    -DBASISD_SUPPORT_ASTC_HIGHER_OPAQUE_QUALITY=$<BOOL:${BASISD_SUPPORT_ASTC_HIGHER_OPAQUE_QUALITY}>
    -DBASISD_SUPPORT_ETC2_EAC_RG11=$<BOOL:${BASISD_SUPPORT_ETC2_EAC_RG11}>
    -DBASISD_SUPPORT_FXT1=$<BOOL:${BASISD_SUPPORT_FXT1}>
    -DBASISD_SUPPORT_PVRTC2=$<BOOL:${BASISD_SUPPORT_PVRTC2}>
    -DBASISD_SUPPORT_UASTC=$<BOOL:${BASISD_SUPPORT_UASTC}>
    -DBASISD_SUPPORT_UASTC_HDR=$<BOOL:${BASISD_SUPPORT_UASTC_HDR}>
)

# In case Raylib is being compiled via CMake, link it directly
if(TARGET raylib)
  target_link_libraries(raylib_basis_universal PUBLIC raylib)
else()
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(RAYLIB REQUIRED raylib)
  target_include_directories(raylib_basis_universal PUBLIC ${RAYLIB_INCLUDE_DIRS})
  target_link_libraries(raylib_basis_universal PRIVATE ${RAYLIB_LINK_LIBRARIES})
endif()
