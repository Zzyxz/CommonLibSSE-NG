# CommonLibSSE V2 Template

MIT-licensed starting point for SKSE plugins that use the Address Library V2 format.

## V2 Address Library location

This template's vendored CommonLibSSE first checks whether the Address Library
file in the normal location uses the compatible format.

If the normal location contains another format, it checks the isolated location:

```text
Data\\SKSE\\Plugins\\AddressLibV2\\versionlib-<major>-<minor>-<patch>-<revision>.bin
```

Keep compatible libraries under `AddressLibV2` to avoid conflicts with other
Address Library files. If neither location has an appropriate file, the normal
path produces the usual missing-library error.

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
