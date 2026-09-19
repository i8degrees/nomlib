# vcpkg - custom triplet file for building universal macOS binaries
# (arm64 + x86_64)
#

set(VCPKG_TARGET_ARCHITECTURE arm64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE static)
set(VCPKG_CMAKE_SYSTEM_NAME Darwin)
set(VCPKG_OSX_ARCHITECTURES "arm64;x86_64")

# Port-specific fixes for universal binary compilation
if(PORT STREQUAL "libpng")
  # Disable hardware-specific SIMD instructions (like ARM Neon)
  # so both the x86_64 and arm64 slices can compile successfully from a
  # single CMake config.
  list(APPEND VCPKG_CMAKE_CONFIGURE_OPTIONS "-DPNG_HARDWARE_OPTIMIZATIONS=OFF")
endif(PORT STREQUAL "libpng")

# Port-specific fixes for universal binary compilation
if(PORT STREQUAL "liblzma")
  # Disable hardware-specific SIMD instructions (like ARM Neon)
  # so both the x86_64 and arm64 slices can compile successfully from a
  # single CMake config.
  list(APPEND VCPKG_CMAKE_CONFIGURE_OPTIONS "-DPNG_HARDWARE_OPTIMIZATIONS=OFF")
endif(PORT STREQUAL "liblzma")

# Port-specific fixes for universal binary compilation
if(PORT STREQUAL "libjpeg-turbo")
  # Disable hardware-specific SIMD instructions (like ARM Neon)
  # so both the x86_64 and arm64 slices can compile successfully from a
  # single CMake config.
  set(VCPKG_CMAKE_CONFIGURE_OPTIONS "-DWITH_SIMD=OFF")
  #list(APPEND VCPKG_CMAKE_CONFIGURE_OPTIONS "-DCMAKE_ASM_FLAGS="--target=arm64-osx")
endif(PORT STREQUAL "libjpeg-turbo")
