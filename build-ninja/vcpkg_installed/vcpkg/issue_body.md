Package: sdl2:arm64-osx@2.32.6

**Host Environment**

- Host: arm64-osx
- Compiler: AppleClang 16.0.0.16000026
-    vcpkg-tool version: 2025-04-16-f9b6c6917b23c1ccf16c1a9f015ebabf8f615045
    vcpkg-scripts version: d43918b914 2025-05-20 (22 hours ago)

**To Reproduce**

`vcpkg install `

**Failure logs**

```
Downloading https://github.com/libsdl-org/SDL/archive/release-2.32.6.tar.gz -> libsdl-org-SDL-release-2.32.6.tar.gz
Successfully downloaded libsdl-org-SDL-release-2.32.6.tar.gz
-- Extracting source /Users/max/Documents/C++/vcpkg/downloads/libsdl-org-SDL-release-2.32.6.tar.gz
-- Applying patch deps.patch
-- Applying patch alsa-dep-fix.patch
-- Applying patch cxx-linkage-pkgconfig.diff
-- Using source at /Users/max/Documents/C++/vcpkg/buildtrees/sdl2/src/ase-2.32.6-cfd589b55d.clean
-- Found external ninja('1.12.1').
-- Configuring arm64-osx
-- Building arm64-osx-dbg
-- Building arm64-osx-rel
-- Fixing pkgconfig file: /Users/max/Documents/C++/vcpkg/packages/sdl2_arm64-osx/lib/pkgconfig/sdl2.pc
CMake Error at scripts/cmake/vcpkg_find_acquire_program.cmake:166 (message):
  Could not find pkg-config.  Please install it via your package manager:

      brew install pkg-config
Call Stack (most recent call first):
  scripts/cmake/vcpkg_fixup_pkgconfig.cmake:193 (vcpkg_find_acquire_program)
  ports/sdl2/portfile.cmake:122 (vcpkg_fixup_pkgconfig)
  scripts/ports.cmake:206 (include)



```

**Additional context**

<details><summary>vcpkg.json</summary>

```
{
  "name": "geometers-board",
  "version-string": "0.1.0",
  "dependencies": [
    "sdl2",
    "sdl2-gfx"
  ]
}

```
</details>
