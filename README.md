# CommonLibSSE V2/V5 Template

MIT-licensed starting point for SKSE plugins that can read both the legacy Address Library V2 format and the current V5 format.

## Address Library location

The vendored CommonLibSSE first reads the normal Address Library file:

```text
Data\\SKSE\\Plugins\\versionlib-<major>-<minor>-<patch>-<revision>.bin
```

That file may use format 2 or format 5. If the normal file is absent, the legacy `Data\\SKSE\\Plugins\\AddressLibV2\\versionlib-...bin` path remains available as a fallback.

The V5 reader is implemented in `extern/CommonLibSSE/include/REL/AddressLibraryV5.h`.

## Skyrim 1.7 support

The SSE/AE target is compiled with `SKYRIM_SUPPORT_AE`, so Skyrim `1.7.x` uses AE relocation IDs and the matching V5 `versionlib-...bin` file. The vendored runtime constants include `SKSE::RUNTIME_1_7_99`, which is also exposed as `SKSE::RUNTIME_LATEST` for AE builds.

## Create a plugin

Configure the name and author, then build out of tree:

```powershell
cmake -S . -B build -DPLUGIN_NAME=MyPlugin -DPLUGIN_AUTHOR=YourName -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
```

`extern/CommonLibSSE` and `extern/CommonLibVR` are MIT-licensed vendored dependencies. The starter plugin is `src/TemplatePlugin.cpp`; add source files and targets as required.

To build for VR instead of SSE/AE:

```powershell
cmake -S . -B build-vr -DBUILD_SKYRIMVR=ON -DPLUGIN_NAME=MyVRPlugin -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build-vr --config Release
```

To copy artifacts into a game installation during a local build:

```powershell
cmake -S . -B build -DCOPY_BUILD=ON -DSKYRIM_PATH='C:/path/to/Skyrim Special Edition'
```
