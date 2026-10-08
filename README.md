# CommonLibSSE NG Template

MIT-licensed starting point for SKSE plugins. One DLL runs on Skyrim SE 1.5.97, AE 1.6 and 1.7, and Skyrim VR 1.4.15.

The template builds against `extern/CommonLibSSE`, which is CommonLibSSE NG 3.7.0 (MIT). It adds Address Library V5 support, Skyrim 1.7 detection, VR layouts for classes NG did not cover, and a set of fixes; see [Changes to CommonLibSSE NG](#changes-to-commonlibsse-ng).

## Requirements

- Visual Studio 2022 (C++23)
- CMake 3.21 or newer
- [vcpkg](https://github.com/microsoft/vcpkg) with `VCPKG_ROOT` set; the dependencies are in `vcpkg.json`

## Create a plugin

The example plugin is `src/TemplatePlugin.cpp`. It exports the entry points for SE, AE and VR SKSE, checks for the Address Library before `SKSE::Init`, and writes a log. Set the name and author, then build out of tree:

```powershell
cmake -S . -B build -DPLUGIN_NAME=MyPlugin -DPLUGIN_AUTHOR=YourName -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"
cmake --build build --config Release
```

The build links the MSVC runtime and the vcpkg dependencies statically (`x64-windows-static`), so the DLL has no extra runtime dependencies. Override `VCPKG_TARGET_TRIPLET` and `CMAKE_MSVC_RUNTIME_LIBRARY` together if you need the DLL runtime.

`SKYRIM_VARIANT` selects the runtimes:

| Value | Runs on |
|---|---|
| `UNIVERSAL` (default) | SE 1.5.97, AE 1.6/1.7 and VR 1.4.15, one DLL |
| `SE` | SE 1.5.97 only |
| `AE` | AE 1.6/1.7 only |
| `VR` | Skyrim VR 1.4.15 only |

To copy the DLL and PDB into the game after each build, add `-DCOPY_BUILD=ON -DSKYRIM_PATH="C:/path/to/Skyrim Special Edition"`.

## One DLL for SE, AE and VR

Some engine classes have a different layout on SE, AE and VR. In a universal build these classes do not expose the fields that move; use the runtime accessors instead:

- `GetRuntimeData()` (or `GetActorRuntimeData()` on `Actor`), for example `cell->GetRuntimeData().worldSpace` or `RE::TES::GetSingleton()->GetRuntimeData().worldSpace`
- `actor->AsMagicTarget()`, `actor->AsActorValueOwner()`, `actor->AsActorState()` for Actor's base classes
- `REL::RelocateMember<T>(obj, seAndAE, vr)` or `REL::RuntimeMember<T>(obj, se, ae, vr)` for fields you reverse-engineered yourself

Never `static_cast` an `Actor*` to one of these bases and never hard-code a field offset in a universal build: the compiled offset is right on one runtime only.

Function and global addresses use `REL::RelocationID(seID, aeID)`; a plain `REL::ID` is valid on one runtime only. VR resolves the SE id through the VR Address Library, which does not contain every SE id: for a function missing there use `REL::VariantID(seID, aeID, vrOffset)` with the VR offset from a disassembler.

Virtual function slots can differ too (VR and AE 1.7 insert virtual functions in some classes); find a hooked slot by comparing the vtable entry with the expected function instead of hard-coding the index.

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
- `BGSDefaultObjectManager`: SE has 364 default objects, AE 1.6 366 and AE 1.7 372 (entries inserted before the end and, in 1.7, at 188 and 264). `GetObject` and `IsObjectInitialized` translate the SE-numbered index and read the init flags at the right offset (checked in IDA on 1.5.97, 1.6.1170 and 1.7.104). `GetObject(DefaultObjectID)` returned a pointer into the first object instead of the array slot.
- Runtime constants for 1.6.1130, 1.6.1170, 1.6.1179, 1.7.99 and 1.7.104. `RUNTIME_SSE_1_6_1330` (value 1.5.1330, no such game version) is deprecated.
- VR (checked in IDA against 1.4.15): `ControlMap` (21 contexts, fields +0x20), `InterfaceStrings` (no "Creation Club Menu", seven VR strings), `PlayerCamera` (VR camera state 9, fields after `tempReturnStates` moved; `GetCameraState`, `GetCurrentState`, `IsInVRCameraMode`), `BSThread` (thread name at 0x48) and `BGSSaveLoadManager` (`GetThread`, `GetRequestQueue`), `BGSDefaultObjectManager` (369 objects; SE index translated, `kHelp272` fixed). NG had no VR layout for these.
- Layouts that differ on AE 1.7 and VR (checked in IDA on 1.5.97, 1.6.1170, 1.7.104 and VR 1.4.15). In builds with AE (or VR, where VR differs) the moved members are reached through the accessors named here; builds for SE only keep the plain members.
  - `PlayerCharacter`: AE 1.7 has a new base class at 0x2D8, so every member and event source after it is 8 bytes later. The SE values for the event sources were wrong in NG as well.
  - `SkyrimVM`: AE 1.7 has two new event sink bases at 0x180, so all members are 0x10 later; VR inserts 0x20 bytes at 0x760. `GetRuntimeData()` (up to 0x75F) and `GetEventRuntimeData()` (from `registeredSleepEvents`). `BSScript::Internal::VirtualMachine::GetSingleton()` read the VM pointer at the 1.6 offset and returned garbage on 1.7.
  - `BSInputEventQueue`: AE 1.7 adds three event counts and arrays, VR has a larger `ButtonEvent` and touchpad events. `GetButtonEvent()` and the other `Get...Event()` functions and `GetQueueData()` return the right address; `AddButtonEvent` and the other `Add...` functions use them.
  - `BSInputDeviceManager`: six device slots on AE 1.7, runtime data 0x10 later. `GetNumDeviceSlots()` and `GetDevice()` cover all versions; the device loops no longer run past the array (NG looped over the VR device count on SE and AE too).
  - `LockpickingMenu`: AE 1.7 inserts 0x14 bytes before `pickTensionSound`; those members are in `GetPickRuntimeData()`.
  - `BSGraphics::State`: the frame flags and the default textures were at the SE offsets on every version (wrong on AE 1.6, AE 1.7 and VR). `GetFrameData()` and `GetTextureData()`; `NiAVObject::TintScenegraph` uses them.
  - `AttackBlockHandler`: AE 1.7 adds gesture data, VR 0x48 bytes before the members. Accessors `GetHeldTime()`, `GetControlID()`, `GetAttackType()` and so on. The power attack delays are pointers to the INI settings, not floats.
  - `FirstPersonState`: AE 1.7 inserts 4 bytes at 0x7C; `cameraOverride` and the members around it are in `GetOverrideData()`.
  - `bhkWorld`: AE 1.7 inserts 0x108 bytes at 0xC5D8; `tau`, `damping`, `toggleCollision` and the other members from there are in `GetStepData()`.
  - `PlayerInputHandler` and `MenuEventHandler`: AE 1.7 and VR have more virtual functions at slot 02 (PlayerInputHandler: two on both; MenuEventHandler: two on AE 1.7, three on VR). `GameSlot()`, the `CallProcess...()` functions and `UseGameVTableLayout()` (for handler classes written in a plugin) handle the shift. This affects every player input handler, camera state and menu input handler.
  - `BSSaveDataSystemUtility`: AE 1.7 moved the members and added six virtual functions at slot 05; the members are only declared in builds without AE.
- Additions: `REL/Callsite.h`, `REL::RuntimeMember` (also with separate AE 1.6 and AE 1.7 offsets), `REL::IsAE17`, `REL::RelocateAE17`, `REL::InsertVTableSlots`, `REL::Module::RuntimeFor` and `AddressLibraryFileName`, `PluginVersionData::UsesAddressLibraryV5` and `UsesUpdatedStructs`.

## Known limits

- On VR the plugin needs the VR Address Library (`version-1-4-15-0.csv`, "VR Address Library for SKSEVR"). It holds a subset of the SE ids (14,554 in v0.276.1); NG's own ids missing there: `Actor.cpp:638` (25834), `InventoryChanges.cpp:58` (15873).
- VR camera: `PlayerCamera::GetCurrentState()` has no value in VR's own camera state (id 9); use `IsInVRCameraMode()`.
- Some AE ids from NG 3.7.0 are missing from newer Address Libraries. A plugin that calls one of these functions stops with an error on those versions:
  - `TES::GetWaterHeight` (13358) on 1.7
  - `Renderer::RequestWindowResize` (77235) on 1.6.1179 and 1.7
  - `BSScaleformManager::IsValidName` (82331) on 1.7
  - `BSScaleformExternalTexture::ReleaseTexture` (82317) from 1.6.1130 on
  - many vtables of Creation Club, ModManager and BSPlatform classes from 1.6.1130 on
- The AE layouts behind `GetRuntimeData()` of `ControlMap`, `TES`, `InterfaceStrings`, `CombatController` and `BGSSaveLoadManager` were checked in IDA on 1.6.1170 and 1.7.104, not on AE versions before 1.6.629.
- `IMenu::inputContext` and `ControlMap`'s `contextPriorityStack` hold the game's context index. On AE every context from `kFavor` on is one higher than `UserEvents::INPUT_CONTEXT_ID`; use `ControlMap::ToGameContext` when you compare or set them. VR keeps the SE indices and adds Crafting 17, Barter 18, RaceSex 19 and Dialogue 20; those menus use these contexts on VR instead of ItemMenu/MenuMode.
- C++ virtual calls and overrides of `PlayerInputHandler` and `MenuEventHandler` functions (and of the classes derived from them) use the SE slots. On AE 1.7 and VR use `GameSlot()`, the `CallProcess...()` functions and `UseGameVTableLayout()`.
- Casts from `SkyrimVM` to its event sink bases from 0x180 on are only correct on SE and AE 1.6.
- Not verified in IDA: `AttackBlockHandler::GetAttackCount()` on VR, the meaning of the new MenuEventHandler slot 02 on AE 1.7 and slot 04 on VR, and the `BSSaveDataSystemUtility` members on AE 1.7. Other classes may have changed in AE 1.7 in ways the comparison did not catch (it compared allocation sizes, base classes, vtables and the field offsets in unchanged functions).
- `PluginVersionData::UsesAddressLibraryV5` sets bit 1 of `versionIndependenceEx`, as the previous template did. It has not been checked against SKSE's `PluginAPI.h`.

## License

MIT, see `LICENSE` and `NOTICE.md`. Only MIT-compatible code and dependencies may be added.
