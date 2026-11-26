# nomlib

Cross-platform 2D game engine (C++14) built on SDL2. Developed alongside
[ttcards](https://github.com/i8degrees/ttcards), a remake of the Triple Triad
side-game from *Final Fantasy VIII*.

## Requirements

| | |
|---|---|
| CMake | 3.24 – 4.2.3 (4.4.x breaks macOS framework builds) |
| Compiler | `clang`/`clang++` on Linux and macOS; `clang-cl` on Windows |
| git | submodules: `vendor/vcpkg`, `wiki.git` |
| vcpkg | vendored as a submodule; `VCPKG_ROOT` is set by the `vcpkg` preset |
| Doxygen + Graphviz | optional, for API docs |

Dependencies are installed by vcpkg from `vcpkg.json` (SDL2, SDL2_image,
SDL2_ttf, libRocket, OpenAL Soft, libsndfile, FreeType). GoogleTest, jsoncpp,
RapidXML, TCLap and SDL_GameControllerDB are fetched by CMake at configure time
(see `vendor/CMakeLists.txt`).

GCC also works: `cmake -S . --preset debug-dynamic -DCMAKE_C_COMPILER=gcc
-DCMAKE_CXX_COMPILER=g++` (verified with GCC 15.2.1).

### Linux system packages

vcpkg builds SDL2 with its `wayland` feature and alsa-lib from source, so the
development headers and autotools are required:

```shell
# Debian / Ubuntu
sudo apt install build-essential pkg-config libx11-dev libxext-dev \
  libxrandr-dev libxcursor-dev libxi-dev libxfixes-dev libxss-dev \
  libwayland-dev libxkbcommon-dev libgl1-mesa-dev autoconf libtool
# Arch
sudo pacman -S --needed base-devel pkgconf libx11 libxext libxrandr \
  libxcursor libxi libxfixes libxss wayland wayland-protocols libxkbcommon \
  mesa autoconf automake libtool
# Fedora
sudo dnf install gcc-c++ pkgconf-pkg-config libX11-devel libXext-devel \
  libXrandr-devel libXcursor-devel libXi-devel libXfixes-devel \
  libXScrnSaver-devel wayland-devel libxkbcommon-devel mesa-libGL-devel \
  autoconf libtool
```

For the `docs` target: `doxygen` and `graphviz` from your package manager
(`texlive-latexextra` too on Arch, for the LaTeX output).

## Build

```shell
git clone https://github.com/i8degrees/nomlib.git nomlib.git
cd nomlib.git
git submodule sync
git submodule update --init
vendor/vcpkg/bootstrap-vcpkg.sh

cmake -S . --preset debug -DNOM_USE_CCACHE=on
cmake --build build/debug --target all -j"$(nproc)"
cmake --install build/debug
```

Presets are defined in `CMakePresets.json`; each configures into
`build/<preset>`. Unix presets: `debug`, `debug-dynamic`, `release`,
`release-dynamic`, `RelWithDebInfo`. macOS and Windows have their own
(`debug-osx`, `release-win`, `debug-clang-cl`, …).

| Preset | Libraries | Examples | Tests |
|---|---|---|---|
| `debug` | static, assertions on | on | on |
| `release` | static | on | off |
| `release-dynamic` | shared | on | off |

Build output: libraries in `build/<preset>/lib`, examples in
`build/<preset>/bin`, tests in `build/<preset>/opt`.

## Options

Options are passed as `-D<NAME>=<VALUE>` at configure time; presets already set
the common ones.

| Option | Default | Purpose |
|---|---|---|
| `CMAKE_INSTALL_PREFIX` | `$HOME/local`; `/usr/local` in `release` | install location |
| `BUILD_SHARED_LIBS` | `OFF` | shared instead of static library targets |
| `BUILD_EXAMPLES` | `OFF` (`ON` in most presets) | build `examples/` |
| `BUILD_TESTS` | `OFF` (`ON` in `debug`) | build GoogleTest suites |
| `BUILD_DOCS` | `ON` | configure the `docs` target |
| `DEBUG_ASSERT` | `OFF` | enable run-time assertions |
| `NOM_USE_CCACHE` | `OFF` | compile through ccache |

Deprecated aliases still accepted with a warning: `DEBUG`, `EXAMPLES`,
`NOM_BUILD_TESTS`, `NOM_BUILD_DOCS`.

## Tests and examples

```shell
cd build/debug/opt && ./ActionTest            # one test binary, from its output dir
cd build/debug && ctest                       # all registered tests
./build/debug/bin/nom_font --text "Hello, world!" build/debug/bin/OpenSans-Regular.ttf
```

Run OpenAL examples from the repository root so relative `Resources` paths
resolve. Examples read data from `build/<preset>/bin`.

## Documentation

```shell
cmake -S . --preset debug -DBUILD_DOCS=on
cmake --build build/debug --target docs
```

Output lands in `build/<preset>/docs/html`.

## Other build flavours

Windows, Visual Studio 2022 with the ClangCL toolset:

```console
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -T "ClangCL"
cmake --build . --config Release
```

Line counts (uses the checked-in `.clocsettings`):

```shell
cloc . --config=.clocsettings --yaml --report=report.yaml
```

## Asset tooling

- **BMFont** (<https://www.angelcode.com/products/bmfont/>) — `nomlib-graphics`
  implements most of the BMFont ASCII format v3; `BMFontTest` and the
  `nom_font` example exercise it. `bmGlyph` (macOS) generated the fonts used by
  ttcards; `snowb-bmf` and `fontbm` are unverified alternatives.
- **TexturePacker** (<https://www.codeandweb.com/texturepacker>) — the custom
  exporter in `Resources/SharedSupport/TexturePacker/nomlib/` (see
  `exporter.xml` and `spritesheet.json`) is implemented as part of the
  `SpriteSheet` interface.

`third-party/README.md` documents the older pre-packaged dependency bundles
(SDL 2.0.x era) for macOS and Windows. They are not required for vcpkg builds,
and the versions listed there are not current.

## License

nomlib is BSD-2-Clause; see `LICENSE.md`. Third-party licenses are collected in
`LICENSES.md`.
