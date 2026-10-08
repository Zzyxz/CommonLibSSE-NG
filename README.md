# CommonLibSSE NG Template

MIT-licensed starting point for SKSE plugins. One DLL runs on Skyrim SE 1.5.97 and on AE 1.6 and 1.7.

The template builds against `extern/CommonLibSSE`, which is CommonLibSSE NG 3.7.0 (MIT). It adds Address Library V5 support, Skyrim 1.7 detection and a set of fixes; see [Changes to CommonLibSSE NG](#changes-to-commonlibsse-ng).

## Requirements

- Visual Studio 2022 (C++23)
- CMake 3.21 or newer
- [vcpkg](https://github.com/microsoft/vcpkg) with `VCPKG_ROOT` set; the dependencies are in `vcpkg.json`

## Create a plugin

The example plugin is `src/TemplatePlugin.cpp`. It exports the SE and AE entry points, checks for the Address Library before `SKSE::Init`, and writes a log. Set the name and author, then build out of tree:

```powershell
cmake -S . -B build -DPLUGIN_NAME=MyPlugin -DPLUGIN_AUTHOR=YourName -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"
cmake --build build --config Release
```

The build links the MSVC runtime and the vcpkg dependencies statically (`x64-windows-static`), so the DLL has no extra runtime dependencies. Override `VCPKG_TARGET_TRIPLET` and `CMAKE_MSVC_RUNTIME_LIBRARY` together if you need the DLL runtime.

`SKYRIM_VARIANT` selects the runtimes:

| Value | Runs on |
|---|---|
| `UNIVERSAL` (default) | SE 1.5.97 and AE 1.6/1.7, one DLL |
| `SE` | SE 1.5.97 only |
| `AE` | AE 1.6/1.7 only |
| `VR` | Skyrim VR, built against `extern/CommonLibVR` |

To copy the DLL and PDB into the game after each build, add `-DCOPY_BUILD=ON -DSKYRIM_PATH="C:/path/to/Skyrim Special Edition"`.

## One DLL for SE and AE

Some engine classes have a different layout on SE and AE. In a universal build these classes do not expose the fields that move; use the runtime accessors instead:

- `GetRuntimeData()` (or `GetActorRuntimeData()` on `Actor`), for example `cell->GetRuntimeData().worldSpace` or `RE::TES::GetSingleton()->GetRuntimeData().worldSpace`
- `actor->AsMagicTarget()`, `actor->AsActorValueOwner()`, `actor->AsActorState()` for Actor's base classes
- `REL::RelocateMember<T>(obj, seAndAE, vr)` or `REL::RuntimeMember<T>(obj, seOffset, aeOffset)` for fields you reverse-engineered yourself

Never `static_cast` an `Actor*` to one of these bases and never hard-code a field offset in a universal build: the compiled offset is right on one runtime only.

Function and global addresses use `REL::RelocationID(seID, aeID)`; a plain `REL::ID` is valid on one runtime only.

## Address Library

The Address Library file is chosen from the running game version:

```text
Data/SKSE/Plugins/version-<version>.bin      SE 1.5.97 (format 1)
Data/SKSE/Plugins/versionlib-<version>.bin   AE 1.6 (format 2), 1.6.1130 and later / 1.7 (format 5)
```

Formats 1, 2 and 5 are read. If the normal file is missing, `Data/SKSE/Plugins/AddressLibV2/<same name>` is used instead.

## Finding hook sites without byte offsets

`REL/Callsite.h` finds a call by its owner and target function instead of a fixed offset:

```cpp
constexpr REL::RelocationID kOwner{ /* SE id */, /* AE id */ };
constexpr REL::RelocationID kTarget{ /* SE id */, /* AE id */ };

const auto site = REL::try_resolve_callsite(kOwner, REL::AUTO_CALLSITE(kTarget));
if (site) {
    SKSE::GetTrampoline().write_call<5>(*site.address, Hook);
} else {
    SKSE::log::warn("hook site not found: {}", REL::callsite_status_text(site.status));
}
```

The owner's code range comes from the game's exception data (`.pdata`). `AUTO_CALLSITE` requires exactly one match; `AUTO_CALLSITE_FIRST`, `_LAST` and `_NTH` pick one of several. `or_offset(...)` accepts a known offset for listed game versions, also when another plugin has already hooked that call. Check every found site in a disassembler: a matching call does not prove that the arguments fit your hook.

## Changes to CommonLibSSE NG

Compared with CommonLibSSE NG 3.7.0 (commit `b93280e8`):

- Address Library V5 (format 5) and the `AddressLibV2` fallback folder.
- Skyrim 1.7 is detected as AE. NG treated every version other than 1.4 and 1.6 as SE.
- `IDDatabase::id2offset` requires an exact id match. Before, an id missing from the library silently returned the next id's address on SE and AE. `try_id2offset` returns no value instead of failing.
- `ControlMap`, `TES`, `InterfaceStrings`, `CombatController` and `BGSSaveLoadManager` select their AE layout at runtime (`GetRuntimeData()`). NG checked a macro it never defines, so these classes always had the SE layout. `ControlMap` also maps `kFavor` to the game's index on AE.
- Corrected Address Library ids, checked in IDA against 1.5.97 and 1.6.1170: the `BShkbAnimationGraph` variable setters, the SE id of `BSShaderTextureSet::Create`, `InventoryChanges::SetUniqueID`, `ObjectTypeInfo::ReleaseData` (which also takes a flag), two `MovementMessageFreezeDirection` vtables and `FxResponseArgs<12>`. Five functions got new AE ids in 1.6.1130 (`GetCachedString`, `Set_CStr`, `Console::SelectedRef`, `Script::CompileAndRun`, `InventoryChanges::RemoveAllItems`); `REL::AESplitID` picks the id for the running version.
- Runtime constants for 1.6.1130, 1.6.1170, 1.6.1179, 1.7.99 and 1.7.104. `RUNTIME_SSE_1_6_1330` (value 1.5.1330, no such game version) is deprecated.
- Additions: `REL/Callsite.h`, `REL::RuntimeMember`, `REL::Module::RuntimeFor` and `AddressLibraryFileName`, `PluginVersionData::UsesAddressLibraryV5` and `UsesUpdatedStructs`.

## Known limits

- Universal builds cover SE and AE. VR is a separate build (`SKYRIM_VARIANT=VR`).
- Some AE ids from NG 3.7.0 are missing from newer Address Libraries. A plugin that calls one of these functions stops with an error on those versions:
  - `TES::GetWaterHeight` (13358) on 1.7
  - `Renderer::RequestWindowResize` (77235) on 1.6.1179 and 1.7
  - `BSScaleformManager::IsValidName` (82331) on 1.7
  - `BSScaleformExternalTexture::ReleaseTexture` (82317) from 1.6.1130 on
  - many vtables of Creation Club, ModManager and BSPlatform classes from 1.6.1130 on
- The AE layouts behind `GetRuntimeData()` of `ControlMap`, `TES`, `InterfaceStrings`, `CombatController` and `BGSSaveLoadManager` were checked in IDA on 1.6.1170 and 1.7.104, not on AE versions before 1.6.629.
- `IMenu::inputContext` and `ControlMap`'s `contextPriorityStack` hold the game's context index. On AE every context from `kFavor` on is one higher than `UserEvents::INPUT_CONTEXT_ID`; use `ControlMap::ToGameContext` when you compare or set them.
- `PluginVersionData::UsesAddressLibraryV5` sets bit 1 of `versionIndependenceEx`, as the previous template did. It has not been checked against SKSE's `PluginAPI.h`.

## License

MIT, see `LICENSE` and `NOTICE.md`. Only MIT-compatible code and dependencies may be added.
