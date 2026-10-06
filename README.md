# CommonLibSSE V2/V5 Template

MIT-licensed starting point for SKSE plugins that can read both the legacy Address Library V2 format and the current V5 format.

SE and AE builds select relocation IDs and Address Library formats 1, 2, or 5 from the running game version.

## Address Library location

The vendored CommonLibSSE selects the normal Address Library filename from the running game version:

```text
Data\\SKSE\\Plugins\\version-<major>-<minor>-<patch>-<revision>.bin
Data\\SKSE\\Plugins\\versionlib-<major>-<minor>-<patch>-<revision>.bin
```

Skyrim 1.5 uses `version-...bin` and format 1. Skyrim 1.6 and newer use `versionlib-...bin` and accept format 2 or 5. If the normal file is absent, the corresponding `Data\\SKSE\\Plugins\\AddressLibV2\\...` path remains available as a fallback.

The V5 reader is implemented in `extern/CommonLibSSE/include/REL/AddressLibraryV5.h`.

## Skyrim 1.7 support

The library detects the running game version. Skyrim 1.5 selects SE relocation IDs, while Skyrim 1.6 and 1.7 select AE relocation IDs. The vendored runtime constants include `SKSE::RUNTIME_1_7_99`, which is also exposed as `SKSE::RUNTIME_LATEST` for SSE/AE builds.

The starter plugin declares `UsesNoStructs()` and exports the legacy query entry point, so its AE build can also load on Skyrim 1.5.97. Plugins that access class layouts which differ between SE and AE need separate SE and AE builds.

## Create a plugin

Configure the name and author, then build the AE variant out of tree:

```powershell
cmake -S . -B build-ae -DSKYRIM_VARIANT=AE -DPLUGIN_NAME=MyPlugin -DPLUGIN_AUTHOR=YourName -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build-ae --config Release
```

Build the SE variant for structure-dependent plugins that support Skyrim 1.5.97:

```powershell
cmake -S . -B build-se -DSKYRIM_VARIANT=SE -DPLUGIN_NAME=MyPlugin -DPLUGIN_AUTHOR=YourName -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build-se --config Release
```

`extern/CommonLibSSE` and `extern/CommonLibVR` are MIT-licensed vendored dependencies. The starter plugin is `src/TemplatePlugin.cpp`; add source files and targets as required.

To build for VR instead of SSE/AE:

```powershell
cmake -S . -B build-vr -DBUILD_SKYRIMVR=ON -DPLUGIN_NAME=MyVRPlugin -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build-vr --config Release
```

To copy artifacts into a game installation during a local build:

```powershell
cmake -S . -B build-ae -DSKYRIM_VARIANT=AE -DCOPY_BUILD=ON -DSKYRIM_PATH='C:/path/to/Skyrim Special Edition'
```
