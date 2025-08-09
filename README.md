# Raylib Basis Universal
Load [raylib](https://github.com/raysan5/raylib) Images and Textures from `.basis` or `.ktx2` files encoded using [Basis Universal](https://github.com/BinomialLLC/basis_universal).


## Features
- Simple API: just call `LoadBasisUniversalImage` instead of `LoadImage`, `LoadBasisUniversalImageFromMemory` instead of `LoadImageFromMemory` and `LoadBasisUniversalTexture` instead of `LoadTexture`
- Sane defaults for which texture format to use
  + On Desktop, uses DXT1 (BC1) for opaque textures and DXT5 (BC3) for textures with alpha
  + On Android, uses ETC1 for opaque textures and ETC2 for textures with alpha
  + On Web, uses RGB565 for opaque textures and RGBA4444 for textures with alpha.
    We use uncompressed formats here because it could be running on mobile as well as on desktop.
- CMake build script with comprehensive set of options


## TODO
- [ ] Add support for setting which texture formats to use instead of forcing the defaults
