#pragma once

#include "REL/Relocation.h"

namespace RE
{
	namespace Offset
	{
#ifdef SKYRIM_SUPPORT_AE
		namespace Actor
		{
			inline constexpr REL::RelocationID AddSpell(37771, 38716);
			inline constexpr REL::RelocationID DispelWornItemEnchantments(33828, 34620);
			inline constexpr REL::RelocationID DoReset3D(39181, 40255);
			inline constexpr REL::RelocationID GetGhost(36286, 37275);
			inline constexpr REL::RelocationID GetHostileToActor(36537, 37537);
			inline constexpr REL::RelocationID GetLevel(36344, 37334);
			inline constexpr REL::RelocationID HasPerk(36690, 37698);
			inline constexpr REL::RelocationID IsRunning(36252, 37234);
			inline constexpr REL::RelocationID RequestDetectionLevel(36748, 37764);
			inline constexpr REL::RelocationID SwitchRace(36901, 37925);
			inline constexpr REL::RelocationID UpdateArmorAbility(37802, 38751);
			inline constexpr REL::RelocationID UpdateWeaponAbility(37803, 38752);
		}

		namespace ActorEquipManager
		{
			inline constexpr REL::RelocationID EquipObject(37938, 38894);
			inline constexpr REL::RelocationID Singleton(514494, 400636);
			inline constexpr REL::RelocationID UnequipObject(37945, 38901);
		}

		namespace ActorValueOwner
		{
			inline constexpr REL::RelocationID GetClampedActorValue(26616, 27284);
		}

		namespace AIProcess
		{
			inline constexpr REL::RelocationID SetBaseScale(38568, 39588);
			inline constexpr REL::RelocationID Update3DModel(38404, 39395);
		}

		namespace BGSFootstepManager
		{
			inline constexpr REL::RelocationID Singleton(517045, 403553);
		}

		namespace BGSListForm
		{
			inline constexpr REL::RelocationID AddForm(20470, 20913);
		}

		namespace BGSSaveLoadManager
		{
			inline constexpr REL::RelocationID Save(34818, 35727);
			inline constexpr REL::RelocationID Singleton(516860, 403340);
			inline constexpr REL::RelocationID Load(34819, 35728);
		}

		namespace BGSSkillPerkTreeNode
		{
			inline constexpr REL::RelocationID Ctor(26592, 27263);
		}

		namespace BGSStoryEventManager
		{
			inline constexpr REL::RelocationID AddEvent(31576, 32359);
			inline constexpr REL::RelocationID GetSingleton(22317, 22790);
		}

		namespace BGSStoryTeller
		{
			inline constexpr REL::RelocationID BeginShutDownQuest(31718, 32486);
			inline constexpr REL::RelocationID BeginStartUpQuest(31717, 32485);
			inline constexpr REL::RelocationID Singleton(514316, 400476);
		}

		namespace BipedAnim
		{
			inline constexpr REL::RelocationID Dtor(15491, 15656);
			inline constexpr REL::RelocationID RemoveAllParts(15494, 15659);
		}

		namespace BSAudioManager
		{
			inline constexpr REL::RelocationID GetSingleton(66391, 67652);
			inline constexpr REL::RelocationID BuildSoundDataFromDescriptor(66404, 67666);
		}

		namespace BSInputDeviceManager
		{
			inline constexpr REL::RelocationID Singleton(516574, 402776);
		}

		namespace BSLightingShaderMaterialBase
		{
			inline constexpr REL::RelocationID CreateMaterial(100016, 106723);
		}

		namespace BSReadWriteLock
		{
			inline constexpr REL::RelocationID LockForRead(66976, 68233);
			inline constexpr REL::RelocationID LockForWrite(66977, 68234);
			inline constexpr REL::RelocationID UnlockForRead(66982, 68239);
			inline constexpr REL::RelocationID UnlockForWrite(66983, 68240);
		}

		namespace BSResourceNiBinaryStream
		{
			inline constexpr REL::RelocationID Ctor(69636, 71014);
			inline constexpr REL::RelocationID Dtor(69638, 71016);
			inline constexpr REL::RelocationID Seek(69640, 71018);
			inline constexpr REL::RelocationID SetEndianSwap(69643, 71021);
		}

		namespace BSScaleformTranslator
		{
			inline constexpr REL::RelocationID GetCachedString(67844, 443410);
		}

		namespace BSScript
		{
			namespace ObjectBindPolicy
			{
				inline constexpr REL::RelocationID BindObject(97379, 104184);
			}

			namespace NF_util
			{
				namespace NativeFunctionBase
				{
					inline constexpr REL::RelocationID Call(97923, 104651);
				}
			}

			namespace Stack
			{
				inline constexpr REL::RelocationID Dtor(97742, 104480);
			}
		}

		namespace BSSoundHandle
		{
			inline constexpr REL::RelocationID IsValid(66360, 67621);
			inline constexpr REL::RelocationID Play(66355, 67616);
			inline constexpr REL::RelocationID SetObjectToFollow(66375, 67636);
			inline constexpr REL::RelocationID SetPosition(66370, 67631);
			inline constexpr REL::RelocationID Stop(66358, 67619);
		}

		namespace BSString
		{
			inline constexpr REL::RelocationID Set_CStr(10979, 439876);
		}

		namespace BucketTable
		{
			inline constexpr REL::RelocationID GetSingleton(67855, 69200);
		}

		namespace BSWin32SaveDataSystemUtility
		{
			inline constexpr REL::RelocationID GetSingleton(101884, 109278);
		}

		namespace Calendar
		{
			inline constexpr REL::RelocationID Singleton(514287, 400447);
		}

		namespace Console
		{
			inline constexpr REL::RelocationID SelectedRef(519394, 504099);
			inline constexpr REL::RelocationID SetSelectedRef(50164, 51093);
		}

		namespace ConsoleLog
		{
			inline constexpr REL::RelocationID Singleton(515064, 401203);
			inline constexpr REL::RelocationID VPrint(50180, 51110);
		}

		namespace ControlMap
		{
			inline constexpr REL::RelocationID Singleton(514705, 400863);
		}

		namespace CraftingSubMenus
		{
			namespace EnchantConstructMenu
			{
				inline constexpr REL::RelocationID RenameItem(50530, 51415);
				inline constexpr REL::RelocationID UpdateInterface(50567, 51459);
			}
		}

		namespace CRC32Calculator
		{
			inline constexpr REL::RelocationID SizeOf32(66963, 12141);
			inline constexpr REL::RelocationID SizeOf64(66964, 68221);
			inline constexpr REL::RelocationID SizeOfSize(66962, 68219);
		}

		namespace ExtraDataList
		{
			inline constexpr REL::RelocationID Add(12176, 12315);
			inline constexpr REL::RelocationID SetCount(11471, 11617);
			inline constexpr REL::RelocationID SetExtraFlags(11903, 12042);
			inline constexpr REL::RelocationID SetInventoryChanges(11483, 11600);
		}

		namespace GameSettingCollection
		{
			inline constexpr REL::RelocationID Singleton(514622, 400782);
		}

		namespace GASActionBufferData
		{
			inline constexpr REL::ID Vtbl(static_cast<std::uint64_t>(242366));
		}

		namespace GASDoAction
		{
			inline constexpr REL::RelocationID Vtbl(291613, 242413);
		}

		namespace GASDoInitAction
		{
			inline constexpr REL::ID Vtbl(static_cast<std::uint64_t>(242414));
		}

		namespace GFxInitImportActions
		{
			inline constexpr REL::ID Vtbl(static_cast<std::uint64_t>(244866));
		}

		namespace GFxLoader
		{
			inline constexpr REL::RelocationID CreateMovie(80620, 84640);
		}

		namespace GFxMovieView
		{
			inline constexpr REL::RelocationID InvokeNoReturn(80547, 82665);
		}

		namespace GFxPlaceObject2
		{
			inline constexpr REL::ID Vtbl(static_cast<std::uint64_t>(242592));
		}

		namespace GFxPlaceObject3
		{
			inline constexpr REL::ID Vtbl(static_cast<std::uint64_t>(242593));
		}

		namespace GFxRemoveObject
		{
			inline constexpr REL::ID Vtbl(static_cast<std::uint64_t>(244863));
		}

		namespace GFxRemoveObject2
		{
			inline constexpr REL::ID Vtbl(static_cast<std::uint64_t>(244864));
		}

		namespace GFxSetBackgroundColor
		{
			inline constexpr REL::ID Vtbl(static_cast<std::uint64_t>(244865));
		}

		namespace GFxValue
		{
			namespace ObjectInterface
			{
				inline constexpr REL::RelocationID AttachMovie(80197, 82219);
				inline constexpr REL::RelocationID DeleteMember(80207, 82230);
				inline constexpr REL::RelocationID GetArraySize(80214, 82237);
				inline constexpr REL::RelocationID GetCxform(80215, 82238);
				inline constexpr REL::RelocationID GetDisplayInfo(80216, 82239);
				inline constexpr REL::RelocationID GetDisplayMatrix(80217, 82240);
				inline constexpr REL::RelocationID GetElement(80218, 82241);
				inline constexpr REL::RelocationID GetMember(80222, 82245);
				inline constexpr REL::RelocationID GotoAndPlay(80230, 82253);
				inline constexpr REL::RelocationID HasMember(80231, 82254);
				inline constexpr REL::RelocationID Invoke(80233, 82256);
				inline constexpr REL::RelocationID ObjectAddRef(80244, 82269);
				inline constexpr REL::RelocationID ObjectRelease(80245, 82270);
				inline constexpr REL::RelocationID PushBack(80248, 82273);
				inline constexpr REL::RelocationID RemoveElements(80252, 82280);
				inline constexpr REL::RelocationID SetArraySize(80261, 82285);
				inline constexpr REL::RelocationID SetDisplayInfo(80263, 82287);
				inline constexpr REL::RelocationID SetDisplayMatrix(80264, 82288);
				inline constexpr REL::RelocationID SetCxform(80262, 82286);
				inline constexpr REL::RelocationID SetElement(80265, 82289);
				inline constexpr REL::RelocationID SetMember(80268, 82292);
				inline constexpr REL::RelocationID SetText(80270, 82293);
				inline constexpr REL::RelocationID VisitMembers(80279, 82302);
			}
		}

		namespace GMemory
		{
			inline constexpr REL::RelocationID GlobalHeap(525584, 412058);
		}

		namespace hkReferencedObject
		{
			inline constexpr REL::RelocationID AddReference(56606, 57010);
			inline constexpr REL::RelocationID RemoveReference(56607, 57011);
		}

		namespace INIPrefSettingCollection
		{
			inline constexpr REL::RelocationID Singleton(523673, 410219);
		}

		namespace INISettingCollection
		{
			inline constexpr REL::RelocationID Singleton(524557, 411155);
		}

		namespace InterfaceStrings
		{
			inline constexpr REL::RelocationID Singleton(514286, 400446);
		}

		namespace Inventory
		{
			inline constexpr REL::RelocationID GetEventSource(15980, 16225);
		}

		namespace InventoryChanges
		{
			inline constexpr REL::RelocationID GetNextUniqueID(15908, 16148);
			inline constexpr REL::RelocationID SendContainerChangedEvent(15909, 16149);
			inline constexpr REL::RelocationID SetUniqueID(15907, 16147);
			inline constexpr REL::RelocationID TransferItemUID(15909, 16149);
		}

		namespace ItemCrafted
		{
			inline constexpr REL::RelocationID GetEventSource(50515, 51403);
		}

		namespace ItemList
		{
			inline constexpr REL::RelocationID Update(50099, 51031);
		}

		namespace ItemsPickpocketed
		{
			inline constexpr REL::RelocationID GetEventSource(50258, 51183);
		}

		namespace LocalMapCamera
		{
			inline constexpr REL::RelocationID Ctor(16084, 16325);
			inline constexpr REL::RelocationID SetNorthRotation(16089, 16330);
		}

		namespace LooseFileStream
		{
			//inline constexpr REL::ID Ctor(static_cast<std::uint64_t>(0));
		}

		namespace MagicFavorites
		{
			inline constexpr REL::RelocationID Singleton(516858, 403337);
		}

		namespace MagicItem
		{
			inline constexpr REL::RelocationID CalculateCost(11213, 11321);
			inline constexpr REL::RelocationID GetCostliestEffectItem(11216, 11335);
		}

		namespace MagicTarget
		{
			inline constexpr REL::RelocationID HasMagicEffect(33733, 34517);
		}

		namespace Main
		{
			inline constexpr REL::RelocationID Singleton(516943, 403449);
		}

		namespace MenuControls
		{
			inline constexpr REL::RelocationID Singleton(515124, 401263);
		}

		namespace MessageDataFactoryManager
		{
			inline constexpr REL::RelocationID GetSingleton(22843, 52875);
		}

		namespace NiAVObject
		{
			inline constexpr REL::RelocationID SetMotionType(76033, 77866);
			inline constexpr REL::RelocationID Update(68900, 70251);
		}

		namespace NiCamera
		{
			inline constexpr REL::RelocationID WorldPtToScreenPt3(69270, 70640);
		}

		namespace NiMemManager
		{
			inline constexpr REL::RelocationID Singleton(523759, 410319);
		}

		namespace NiNode
		{
			inline constexpr REL::RelocationID Ctor(68936, 70287);
		}

		namespace NiPoint3
		{
			inline constexpr REL::RelocationID Zero(523887, 410468);
		}

		namespace NiRefObject
		{
			inline constexpr REL::RelocationID TotalObjectCount(523912, 410493);
		}

		namespace NiSkinInstance
		{
			inline constexpr REL::RelocationID Ctor(69804, 71227);
		}

		namespace PlayerCamera
		{
			inline constexpr REL::RelocationID Singleton(514642, 400802);
			inline constexpr REL::RelocationID UpdateThirdPerson(49908, 50841);
		}

		namespace PlayerCharacter
		{
			namespace PlayerSkills
			{
				inline constexpr REL::RelocationID AdvanceLevel(40560, 41567);
			}

			inline constexpr REL::RelocationID ActivatePickRef(39471, 40548);
			inline constexpr REL::RelocationID GetArmorValue(39175, 40249);
			inline constexpr REL::RelocationID GetDamage(39179, 40253);
			inline constexpr REL::RelocationID GetNumTints(39614, 40700);
			inline constexpr REL::RelocationID GetTintMask(39612, 40698);
			inline constexpr REL::RelocationID PlayPickupEvent(39384, 40456);
			inline constexpr REL::RelocationID Singleton(517014, 403521);
			inline constexpr REL::RelocationID StartGrabObject(39475, 40552);
		}

		namespace PlayerControls
		{
			inline constexpr REL::RelocationID Ctor(41257, 42336);
			inline constexpr REL::RelocationID Singleton(514706, 400864);
		}

		namespace RaceSexMenu
		{
			inline constexpr REL::RelocationID ChangeName(51540, 52415);
		}

		namespace ReferenceEffectController
		{
			inline constexpr REL::RelocationID Start(33961, 34761);
		}

		namespace Script
		{
			inline constexpr REL::RelocationID CompileAndRun(21416, 441582);
			inline constexpr REL::RelocationID GetProcessScripts(21436, 21921);
			inline constexpr REL::RelocationID SetProcessScripts(21435, 21920);
		}

		namespace SCRIPT_FUNCTION
		{
			inline constexpr REL::RelocationID FirstConsoleCommand(501797, 365650);
			inline constexpr REL::RelocationID FirstScriptCommand(501789, 361120);
		}

		namespace SkyrimVM
		{
			inline constexpr REL::RelocationID QueuePostRenderCall(53144, 53955);
			inline constexpr REL::RelocationID RelayEvent(53221, 54033);
			inline constexpr REL::RelocationID Singleton(514315, 400475);
		}

		namespace TES
		{
			inline constexpr REL::RelocationID Singleton(516923, 403450);
		}

		namespace TESCamera
		{
			inline constexpr REL::RelocationID SetState(32290, 33026);
		}

		namespace TESDataHandler
		{
			inline constexpr REL::RelocationID LoadScripts(13657, 13766);
			inline constexpr REL::RelocationID Singleton(514141, 400269);
		}

		namespace TESDescription
		{
			inline constexpr REL::RelocationID GetDescription(14399, 14552);
		}

		namespace TESFile
		{
			inline constexpr REL::RelocationID Duplicate(13923, 14018);
			inline constexpr REL::RelocationID GetCurrentSubRecordType(13902, 13988);
			inline constexpr REL::RelocationID GetFormType(13897, 13982);
			inline constexpr REL::RelocationID ReadData(13904, 13991);
			inline constexpr REL::RelocationID Seek(13898, 13984);
			inline constexpr REL::RelocationID SeekNextSubrecord(13903, 13990);
		}

		namespace TESHavokUtilities
		{
			inline constexpr REL::RelocationID FindCollidableRef(25466, 26003);
		}

		namespace TESNPC
		{
			inline constexpr REL::RelocationID ChangeHeadPart(24246, 24750);
			inline constexpr REL::RelocationID GetBaseOverlays(24275, 24791);
			inline constexpr REL::RelocationID GetNumBaseOverlays(24276, 24792);
			inline constexpr REL::RelocationID HasOverlays(24274, 24790);
			inline constexpr REL::RelocationID SetSkinFromTint(24206, 24710);
			inline constexpr REL::RelocationID UpdateNeck(24207, 24711);
		}

		namespace TESObjectREFR
		{
			inline constexpr REL::RelocationID FindReferenceFor3D(19323, 19750);
			inline constexpr REL::RelocationID GetDisplayFullName(19354, 19781);
			inline constexpr REL::RelocationID GetLock(19818, 20223);
			inline constexpr REL::RelocationID GetOwner(19789, 20194);
			inline constexpr REL::RelocationID GetStealValue(15807, 16045);
			inline constexpr REL::RelocationID InitInventoryIfRequired(15800, 16038);
			inline constexpr REL::RelocationID MoveTo(56227, 56626);
			inline constexpr REL::RelocationID PlayAnimation(14189, 14297);
		}

		namespace TESQuest
		{
			inline constexpr REL::RelocationID EnsureQuestStarted(24481, 25003);
			inline constexpr REL::RelocationID ResetQuest(24486, 25014);
		}

		namespace UI
		{
			inline constexpr REL::RelocationID Singleton(514178, 400327);
		}

		namespace UIBlurManager
		{
			inline constexpr REL::RelocationID DecrementBlurCount(51900, 52777);
			inline constexpr REL::RelocationID IncrementBlurCount(51899, 52776);
			inline constexpr REL::RelocationID Singleton(516871, 403350);
		}

		namespace UIMessageQueue
		{
			inline constexpr REL::RelocationID AddMessage(13530, 13631);
			inline constexpr REL::RelocationID CreateUIMessageData(80061, 82169);
			inline constexpr REL::RelocationID ProcessCommands(80059, 82167);
			inline constexpr REL::RelocationID Singleton(514285, 400445);
		}

		namespace UserEvents
		{
			inline constexpr REL::RelocationID Singleton(516458, 402638);
		}

		inline constexpr REL::RelocationID CreateRefHandle(12193, 12326);
		inline constexpr REL::RelocationID DebugNotification(52050, 52933);
		inline constexpr REL::RelocationID LookupReferenceByHandle(12204, 12332);
		inline constexpr REL::RelocationID PlaySound(52054, 52939);
		inline constexpr REL::RelocationID TlsIndex(528600, 415542);
		inline constexpr REL::RelocationID GlobalStateCounter(514157, 400305);
#else
		namespace Actor
		{
			inline constexpr REL::ID AddSpell(static_cast<std::uint64_t>(37771));
			inline constexpr REL::ID DispelWornItemEnchantments(static_cast<std::uint64_t>(33828));
			inline constexpr REL::ID DoReset3D(static_cast<std::uint64_t>(39181));
			inline constexpr REL::ID GetGhost(static_cast<std::uint64_t>(36286));
			inline constexpr REL::ID GetHostileToActor(static_cast<std::uint64_t>(36537));
			inline constexpr REL::ID GetLevel(static_cast<std::uint64_t>(36344));
			inline constexpr REL::ID HasPerk(static_cast<std::uint64_t>(36690));
			inline constexpr REL::ID IsRunning(static_cast<std::uint64_t>(36252));
			inline constexpr REL::ID RequestDetectionLevel(static_cast<std::uint64_t>(36748));
			inline constexpr REL::ID SwitchRace(static_cast<std::uint64_t>(36901));
			inline constexpr REL::ID UpdateArmorAbility(static_cast<std::uint64_t>(37802));
			inline constexpr REL::ID UpdateWeaponAbility(static_cast<std::uint64_t>(37803));
		}

		namespace ActorEquipManager
		{
			inline constexpr REL::ID EquipObject(static_cast<std::uint64_t>(37938));
			inline constexpr REL::ID Singleton(static_cast<std::uint64_t>(514494));
			inline constexpr REL::ID UnequipObject(static_cast<std::uint64_t>(37945));
		}

		namespace ActorValueOwner
		{
			inline constexpr REL::ID GetClampedActorValue(static_cast<std::uint64_t>(26616));
		}

		namespace AIProcess
		{
			inline constexpr REL::ID SetBaseScale(static_cast<std::uint64_t>(38568));
			inline constexpr REL::ID Update3DModel(static_cast<std::uint64_t>(38404));
		}

		namespace BGSFootstepManager
		{
			inline constexpr REL::ID Singleton(static_cast<std::uint64_t>(517045));
		}

		namespace BGSListForm
		{
			inline constexpr REL::ID AddForm(static_cast<std::uint64_t>(20470));
		}

		namespace BGSSaveLoadManager
		{
			inline constexpr REL::ID Save(static_cast<std::uint64_t>(34818));
			inline constexpr REL::ID Singleton(static_cast<std::uint64_t>(516860));
			inline constexpr REL::ID Load(static_cast<std::uint64_t>(34819));
		}

		namespace BGSSkillPerkTreeNode
		{
			inline constexpr REL::ID Ctor(static_cast<std::uint64_t>(26592));
		}

		namespace BGSStoryEventManager
		{
			inline constexpr REL::ID AddEvent(static_cast<std::uint64_t>(31576));
			inline constexpr REL::ID GetSingleton(static_cast<std::uint64_t>(22317));
		}

		namespace BGSStoryTeller
		{
			inline constexpr REL::ID BeginShutDownQuest(static_cast<std::uint64_t>(31718));
			inline constexpr REL::ID BeginStartUpQuest(static_cast<std::uint64_t>(31717));
			inline constexpr REL::ID Singleton(static_cast<std::uint64_t>(514316));
		}

		namespace BipedAnim
		{
			inline constexpr REL::ID Dtor(static_cast<std::uint64_t>(15491));
			inline constexpr REL::ID RemoveAllParts(static_cast<std::uint64_t>(15494));
		}

		namespace BSAudioManager
		{
			inline constexpr REL::ID GetSingleton(static_cast<std::uint64_t>(66391));
			inline constexpr REL::ID BuildSoundDataFromDescriptor(static_cast<std::uint64_t>(66404));
		}

		namespace BSInputDeviceManager
		{
			inline constexpr REL::ID Singleton(static_cast<std::uint64_t>(516574));
		}

		namespace BSLightingShaderMaterialBase
		{
			inline constexpr REL::ID CreateMaterial(static_cast<std::uint64_t>(100016));
		}

		namespace BSReadWriteLock
		{
			inline constexpr REL::ID LockForRead(static_cast<std::uint64_t>(66976));
			inline constexpr REL::ID LockForWrite(static_cast<std::uint64_t>(66977));
			inline constexpr REL::ID UnlockForRead(static_cast<std::uint64_t>(66982));
			inline constexpr REL::ID UnlockForWrite(static_cast<std::uint64_t>(66983));
		}

		namespace BSResourceNiBinaryStream
		{
			inline constexpr REL::ID Ctor(static_cast<std::uint64_t>(69636));
			inline constexpr REL::ID Dtor(static_cast<std::uint64_t>(69638));
			inline constexpr REL::ID Seek(static_cast<std::uint64_t>(69640));
			inline constexpr REL::ID SetEndianSwap(static_cast<std::uint64_t>(69643));
		}

		namespace BSScaleformTranslator
		{
			inline constexpr REL::ID GetCachedString(static_cast<std::uint64_t>(67844));
		}

		namespace BSScript
		{
			namespace ObjectBindPolicy
			{
				inline constexpr REL::ID BindObject(static_cast<std::uint64_t>(97379));
			}

			namespace NF_util
			{
				namespace NativeFunctionBase
				{
					inline constexpr REL::ID Call(static_cast<std::uint64_t>(97923));
				}
			}

			namespace Stack
			{
				inline constexpr REL::ID Dtor(static_cast<std::uint64_t>(97742));
			}
		}

		namespace BSSoundHandle
		{
			inline constexpr REL::ID IsValid(static_cast<std::uint64_t>(66360));
			inline constexpr REL::ID Play(static_cast<std::uint64_t>(66355));
			inline constexpr REL::ID SetObjectToFollow(static_cast<std::uint64_t>(66375));
			inline constexpr REL::ID SetPosition(static_cast<std::uint64_t>(66370));
			inline constexpr REL::ID Stop(static_cast<std::uint64_t>(66358));
		}

		namespace BSString
		{
			inline constexpr REL::ID Set_CStr(static_cast<std::uint64_t>(10979));
		}

		namespace BucketTable
		{
			inline constexpr REL::ID GetSingleton(static_cast<std::uint64_t>(67855));
		}

		namespace BSWin32SaveDataSystemUtility
		{
			inline constexpr REL::ID GetSingleton(static_cast<std::uint64_t>(101884));
		}

		namespace Calendar
		{
			inline constexpr REL::ID Singleton(static_cast<std::uint64_t>(514287));
		}

		namespace Console
		{
			inline constexpr REL::ID SelectedRef(static_cast<std::uint64_t>(519394));
			inline constexpr REL::ID SetSelectedRef(static_cast<std::uint64_t>(50164));
		}

		namespace ConsoleLog
		{
			inline constexpr REL::ID Singleton(static_cast<std::uint64_t>(515064));
			inline constexpr REL::ID VPrint(static_cast<std::uint64_t>(50180));
		}

		namespace ControlMap
		{
			inline constexpr REL::ID Singleton(static_cast<std::uint64_t>(514705));
		}

		namespace CraftingSubMenus
		{
			namespace EnchantConstructMenu
			{
				inline constexpr REL::ID RenameItem(static_cast<std::uint64_t>(50530));
				inline constexpr REL::ID UpdateInterface(static_cast<std::uint64_t>(50567));
			}
		}

		namespace CRC32Calculator
		{
			inline constexpr REL::ID SizeOf32(static_cast<std::uint64_t>(66963));
			inline constexpr REL::ID SizeOf64(static_cast<std::uint64_t>(66964));
			inline constexpr REL::ID SizeOfSize(static_cast<std::uint64_t>(66962));
		}

		namespace ExtraDataList
		{
			inline constexpr REL::ID Add(static_cast<std::uint64_t>(12176));
			inline constexpr REL::ID SetCount(static_cast<std::uint64_t>(11471));
			inline constexpr REL::ID SetExtraFlags(static_cast<std::uint64_t>(11903));
			inline constexpr REL::ID SetInventoryChanges(static_cast<std::uint64_t>(11483));
		}

		namespace GameSettingCollection
		{
			inline constexpr REL::ID Singleton(static_cast<std::uint64_t>(514622));
		}

		namespace GASActionBufferData
		{
			inline constexpr std::uint64_t Vtbl(0x17BC3F0);
		}

		namespace GASDoAction
		{
			inline constexpr REL::ID Vtbl(static_cast<std::uint64_t>(291613));
		}

		namespace GASDoInitAction
		{
			inline constexpr std::uint64_t Vtbl(0x17BCA00);
		}

		namespace GFxInitImportActions
		{
			inline constexpr std::uint64_t Vtbl(0x17DC4C8);
		}

		namespace GFxLoader
		{
			inline constexpr REL::ID CreateMovie(static_cast<std::uint64_t>(80620));
		}

		namespace GFxMovieView
		{
			inline constexpr REL::ID InvokeNoReturn(static_cast<std::uint64_t>(80547));
		}

		namespace GFxPlaceObject2
		{
			inline constexpr std::uint64_t Vtbl(0x17BE0E0);
		}

		namespace GFxPlaceObject3
		{
			inline constexpr std::uint64_t Vtbl(0x17BE138);
		}

		namespace GFxRemoveObject
		{
			inline constexpr std::uint64_t Vtbl(0x17DC408);
		}

		namespace GFxRemoveObject2
		{
			inline constexpr std::uint64_t Vtbl(0x17DC448);
		}

		namespace GFxSetBackgroundColor
		{
			inline constexpr std::uint64_t Vtbl(0x17DC488);
		}

		namespace GFxValue
		{
			namespace ObjectInterface
			{
				inline constexpr REL::ID AttachMovie(static_cast<std::uint64_t>(80197));
				inline constexpr REL::ID DeleteMember(static_cast<std::uint64_t>(80207));
				inline constexpr REL::ID GetArraySize(static_cast<std::uint64_t>(80214));
				inline constexpr REL::ID GetCxform(static_cast<std::uint64_t>(80215));
				inline constexpr REL::ID GetDisplayInfo(static_cast<std::uint64_t>(80216));
				inline constexpr REL::ID GetDisplayMatrix(static_cast<std::uint64_t>(80217));
				inline constexpr REL::ID GetElement(static_cast<std::uint64_t>(80218));
				inline constexpr REL::ID GetMember(static_cast<std::uint64_t>(80222));
				inline constexpr REL::ID GotoAndPlay(static_cast<std::uint64_t>(80230));
				inline constexpr REL::ID HasMember(static_cast<std::uint64_t>(80231));
				inline constexpr REL::ID Invoke(static_cast<std::uint64_t>(80233));
				inline constexpr REL::ID ObjectAddRef(static_cast<std::uint64_t>(80244));
				inline constexpr REL::ID ObjectRelease(static_cast<std::uint64_t>(80245));
				inline constexpr REL::ID PushBack(static_cast<std::uint64_t>(80248));
				inline constexpr REL::ID RemoveElements(static_cast<std::uint64_t>(80252));
				inline constexpr REL::ID SetArraySize(static_cast<std::uint64_t>(80261));
				inline constexpr REL::ID SetCxform(static_cast<std::uint64_t>(80262));
				inline constexpr REL::ID SetDisplayInfo(static_cast<std::uint64_t>(80263));
				inline constexpr REL::ID SetDisplayMatrix(static_cast<std::uint64_t>(80264));
				inline constexpr REL::ID SetElement(static_cast<std::uint64_t>(80265));
				inline constexpr REL::ID SetMember(static_cast<std::uint64_t>(80268));
				inline constexpr REL::ID SetText(static_cast<std::uint64_t>(80270));
				inline constexpr REL::ID VisitMembers(static_cast<std::uint64_t>(80279));
			}
		}

		namespace GMemory
		{
			inline constexpr REL::ID GlobalHeap(static_cast<std::uint64_t>(525584));
		}

		namespace hkReferencedObject
		{
			inline constexpr REL::ID AddReference(static_cast<std::uint64_t>(56606));
			inline constexpr REL::ID RemoveReference(static_cast<std::uint64_t>(56607));
		}

		namespace INIPrefSettingCollection
		{
			inline constexpr REL::ID Singleton(static_cast<std::uint64_t>(523673));
		}

		namespace INISettingCollection
		{
			inline constexpr REL::ID Singleton(static_cast<std::uint64_t>(524557));
		}

		namespace InterfaceStrings
		{
			inline constexpr REL::ID Singleton(static_cast<std::uint64_t>(514286));
		}

		namespace Inventory
		{
			inline constexpr REL::ID GetEventSource(static_cast<std::uint64_t>(15980));
		}

		namespace InventoryChanges
		{
			inline constexpr REL::ID GetNextUniqueID(static_cast<std::uint64_t>(15908));
			inline constexpr REL::ID SendContainerChangedEvent(static_cast<std::uint64_t>(15909));
			inline constexpr REL::ID SetUniqueID(static_cast<std::uint64_t>(15907));
			inline constexpr REL::ID TransferItemUID(static_cast<std::uint64_t>(15909));
		}

		namespace ItemCrafted
		{
			inline constexpr REL::ID GetEventSource(static_cast<std::uint64_t>(50515));
		}

		namespace ItemList
		{
			inline constexpr REL::ID Update(static_cast<std::uint64_t>(50099));
		}

		namespace ItemsPickpocketed
		{
			inline constexpr REL::ID GetEventSource(static_cast<std::uint64_t>(50258));
		}

		namespace LocalMapCamera
		{
			inline constexpr REL::ID Ctor(static_cast<std::uint64_t>(16084));
			inline constexpr REL::ID SetNorthRotation(static_cast<std::uint64_t>(16089));
		}

		namespace LooseFileStream
		{
			inline constexpr REL::ID Ctor(static_cast<std::uint64_t>(68663));
		}

		namespace MagicFavorites
		{
			inline constexpr REL::ID Singleton(static_cast<std::uint64_t>(516858));
		}

		namespace MagicItem
		{
			inline constexpr REL::ID CalculateCost(static_cast<std::uint64_t>(11213));
			inline constexpr REL::ID GetCostliestEffectItem(static_cast<std::uint64_t>(11216));
		}

		namespace MagicTarget
		{
			inline constexpr REL::ID HasMagicEffect(static_cast<std::uint64_t>(33733));
		}

		namespace Main
		{
			inline constexpr REL::ID Singleton(static_cast<std::uint64_t>(516943));
		}

		namespace MenuControls
		{
			inline constexpr REL::ID Singleton(static_cast<std::uint64_t>(515124));
		}

		namespace MessageDataFactoryManager
		{
			inline constexpr REL::ID GetSingleton(static_cast<std::uint64_t>(22843));
		}

		namespace NiAVObject
		{
			inline constexpr REL::ID SetMotionType(static_cast<std::uint64_t>(76033));
			inline constexpr REL::ID Update(static_cast<std::uint64_t>(68900));
		}

		namespace NiCamera
		{
			inline constexpr REL::ID WorldPtToScreenPt3(static_cast<std::uint64_t>(69270));
		}

		namespace NiMemManager
		{
			inline constexpr REL::ID Singleton(static_cast<std::uint64_t>(523759));
		}

		namespace NiNode
		{
			inline constexpr REL::ID Ctor(static_cast<std::uint64_t>(68936));
		}

		namespace NiPoint3
		{
			inline constexpr REL::ID Zero(static_cast<std::uint64_t>(523887));
		}

		namespace NiRefObject
		{
			inline constexpr REL::ID TotalObjectCount(static_cast<std::uint64_t>(523912));
		}

		namespace NiSkinInstance
		{
			inline constexpr REL::ID Ctor(static_cast<std::uint64_t>(69804));
		}

		namespace PlayerCamera
		{
			inline constexpr REL::ID Singleton(static_cast<std::uint64_t>(514642));
			inline constexpr REL::ID UpdateThirdPerson(static_cast<std::uint64_t>(49908));
		}

		namespace PlayerCharacter
		{
			namespace PlayerSkills
			{
				inline constexpr REL::ID AdvanceLevel(static_cast<std::uint64_t>(40560));
			}

			inline constexpr REL::ID ActivatePickRef(static_cast<std::uint64_t>(39471));
			inline constexpr REL::ID GetArmorValue(static_cast<std::uint64_t>(39175));
			inline constexpr REL::ID GetDamage(static_cast<std::uint64_t>(39179));
			inline constexpr REL::ID GetNumTints(static_cast<std::uint64_t>(39614));
			inline constexpr REL::ID GetTintMask(static_cast<std::uint64_t>(39612));
			inline constexpr REL::ID PlayPickupEvent(static_cast<std::uint64_t>(39384));
			inline constexpr REL::ID Singleton(static_cast<std::uint64_t>(517014));
			inline constexpr REL::ID StartGrabObject(static_cast<std::uint64_t>(39475));
		}

		namespace PlayerControls
		{
			inline constexpr REL::ID Ctor(static_cast<std::uint64_t>(41257));
			inline constexpr REL::ID Singleton(static_cast<std::uint64_t>(514706));
		}

		namespace RaceSexMenu
		{
			inline constexpr REL::ID ChangeName(static_cast<std::uint64_t>(51540));
		}

		namespace ReferenceEffectController
		{
			inline constexpr REL::ID Start(static_cast<std::uint64_t>(33961));
		}

		namespace Script
		{
			inline constexpr REL::ID CompileAndRun(static_cast<std::uint64_t>(21416));
			inline constexpr REL::ID GetProcessScripts(static_cast<std::uint64_t>(21436));
			inline constexpr REL::ID SetProcessScripts(static_cast<std::uint64_t>(21435));
		}

		namespace SCRIPT_FUNCTION
		{
			inline constexpr REL::ID FirstConsoleCommand(static_cast<std::uint64_t>(501797));
			inline constexpr REL::ID FirstScriptCommand(static_cast<std::uint64_t>(501789));
		}

		namespace SkyrimVM
		{
			inline constexpr REL::ID QueuePostRenderCall(static_cast<std::uint64_t>(53144));
			inline constexpr REL::ID RelayEvent(static_cast<std::uint64_t>(53221));
			inline constexpr REL::ID Singleton(static_cast<std::uint64_t>(514315));
		}

		namespace TES
		{
			inline constexpr REL::ID Singleton(static_cast<std::uint64_t>(516923));
		}

		namespace TESCamera
		{
			inline constexpr REL::ID SetState(static_cast<std::uint64_t>(32290));
		}

		namespace TESDataHandler
		{
			inline constexpr REL::ID LoadScripts(static_cast<std::uint64_t>(13657));
			inline constexpr REL::ID Singleton(static_cast<std::uint64_t>(514141));
		}

		namespace TESDescription
		{
			inline constexpr REL::ID GetDescription(static_cast<std::uint64_t>(14399));
		}

		namespace TESFile
		{
			inline constexpr REL::ID Duplicate(static_cast<std::uint64_t>(13923));
			inline constexpr REL::ID GetCurrentSubRecordType(static_cast<std::uint64_t>(13902));
			inline constexpr REL::ID GetFormType(static_cast<std::uint64_t>(13897));
			inline constexpr REL::ID ReadData(static_cast<std::uint64_t>(13904));
			inline constexpr REL::ID Seek(static_cast<std::uint64_t>(13898));
			inline constexpr REL::ID SeekNextSubrecord(static_cast<std::uint64_t>(13903));
		}

		namespace TESHavokUtilities
		{
			inline constexpr REL::ID FindCollidableRef(static_cast<std::uint64_t>(25466));
		}

		namespace TESNPC
		{
			inline constexpr REL::ID ChangeHeadPart(static_cast<std::uint64_t>(24246));
			inline constexpr REL::ID GetBaseOverlays(static_cast<std::uint64_t>(24275));
			inline constexpr REL::ID GetNumBaseOverlays(static_cast<std::uint64_t>(24276));
			inline constexpr REL::ID HasOverlays(static_cast<std::uint64_t>(24274));
			inline constexpr REL::ID SetSkinFromTint(static_cast<std::uint64_t>(24206));
			inline constexpr REL::ID UpdateNeck(static_cast<std::uint64_t>(24207));
		}

		namespace TESObjectREFR
		{
			inline constexpr REL::ID FindReferenceFor3D(static_cast<std::uint64_t>(19323));
			inline constexpr REL::ID GetDisplayFullName(static_cast<std::uint64_t>(19354));
			inline constexpr REL::ID GetLock(static_cast<std::uint64_t>(19818));
			inline constexpr REL::ID GetOwner(static_cast<std::uint64_t>(19789));
			inline constexpr REL::ID GetStealValue(static_cast<std::uint64_t>(15807));
			inline constexpr REL::ID InitInventoryIfRequired(static_cast<std::uint64_t>(15800));
			inline constexpr REL::ID MoveTo(static_cast<std::uint64_t>(56227));
			inline constexpr REL::ID PlayAnimation(static_cast<std::uint64_t>(14189));
		}

		namespace TESQuest
		{
			inline constexpr REL::ID EnsureQuestStarted(static_cast<std::uint64_t>(24481));
			inline constexpr REL::ID ResetQuest(static_cast<std::uint64_t>(24486));
		}

		namespace UI
		{
			inline constexpr REL::ID Singleton(static_cast<std::uint64_t>(514178));
		}

		namespace UIBlurManager
		{
			inline constexpr REL::ID DecrementBlurCount(static_cast<std::uint64_t>(51900));
			inline constexpr REL::ID IncrementBlurCount(static_cast<std::uint64_t>(51899));
			inline constexpr REL::ID Singleton(static_cast<std::uint64_t>(516871));
		}

		namespace UIMessageQueue
		{
			inline constexpr REL::ID AddMessage(static_cast<std::uint64_t>(13530));
			inline constexpr REL::ID CreateUIMessageData(static_cast<std::uint64_t>(80061));
			inline constexpr REL::ID ProcessCommands(static_cast<std::uint64_t>(80059));
			inline constexpr REL::ID Singleton(static_cast<std::uint64_t>(514285));
		}

		namespace UserEvents
		{
			inline constexpr REL::ID Singleton(static_cast<std::uint64_t>(516458));
		}

		inline constexpr REL::ID CreateRefHandle(static_cast<std::uint64_t>(12193));
		inline constexpr REL::ID DebugNotification(static_cast<std::uint64_t>(52050));
		inline constexpr REL::ID LookupReferenceByHandle(static_cast<std::uint64_t>(12204));
		inline constexpr REL::ID PlaySound(static_cast<std::uint64_t>(52054));
		inline constexpr REL::ID TlsIndex(static_cast<std::uint64_t>(528600));
		inline constexpr REL::ID GlobalStateCounter(static_cast<std::uint64_t>(514157));
#endif
	}
}
