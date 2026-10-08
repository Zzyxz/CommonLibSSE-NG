#pragma once

#include "RE/B/BSAtomic.h"
#include "RE/B/BSTEvent.h"
#include "RE/B/BSTFreeList.h"
#include "RE/B/BSTHashMap.h"
#include "RE/B/BSTMessageQueue.h"
#include "RE/B/BSTSingleton.h"
#include "RE/B/BSTSmartPointer.h"
#include "RE/C/CompiledScriptLoader.h"
#include "RE/D/DelayFunctor.h"
#include "RE/F/FragmentSystem.h"
#include "RE/H/HandlePolicy.h"
#include "RE/I/IFreezeQuery.h"
#include "RE/I/IStackCallbackSaveInterface.h"
#include "RE/L/Logger.h"
#include "RE/P/Profiler.h"
#include "RE/S/SavePatcher.h"
#include "RE/S/SimpleAllocMemoryPagePolicy.h"
#include "RE/S/SkyrimScriptObjectBindPolicy.h"
#include "RE/S/SkyrimScriptStore.h"

namespace RE
{
	namespace BSScript
	{
		class IFunctionArguments;
		class IVirtualMachine;
		class IVMDebugInterface;
		class IVMSaveLoadInterface;
		struct StatsEvent;
	}

	struct PositionPlayerEvent;
	struct TESActivateEvent;
	struct TESActiveEffectApplyRemoveEvent;
	struct TESActorLocationChangeEvent;
	struct TESBookReadEvent;
	struct TESCellAttachDetachEvent;
	struct TESCellFullyLoadedEvent;
	struct TESCombatEvent;
	struct TESContainerChangedEvent;
	struct TESDeathEvent;
	struct TESDestructionStageChangedEvent;
	struct TESEnterBleedoutEvent;
	struct TESEquipEvent;
	struct TESFastTravelEndEvent;
	struct TESFormDeleteEvent;
	struct TESFurnitureEvent;
	struct TESGrabReleaseEvent;
	struct TESHitEvent;
	struct TESInitScriptEvent;
	struct TESLoadGameEvent;
	struct TESLockChangedEvent;
	struct TESMagicEffectApplyEvent;
	struct TESMagicWardHitEvent;
	struct TESMoveAttachDetachEvent;
	struct TESObjectLoadedEvent;
	struct TESObjectREFRTranslationEvent;
	struct TESOpenCloseEvent;
	struct TESPackageEvent;
	struct TESPerkEntryRunEvent;
	struct TESPlayerBowShotEvent;
	struct TESQuestInitEvent;
	struct TESQuestStageEvent;
	struct TESResetEvent;
	struct TESResolveNPCTemplatesEvent;
	struct TESSceneActionEvent;
	struct TESSceneEvent;
	struct TESScenePhaseEvent;
	struct TESSellEvent;
	struct TESSleepStartEvent;
	struct TESSleepStopEvent;
	struct TESSpellCastEvent;
	struct TESSwitchRaceCompleteEvent;
	struct TESTopicInfoEvent;
	struct TESTrackedStatsEvent;
	struct TESTrapHitEvent;
	struct TESTriggerEnterEvent;
	struct TESTriggerEvent;
	struct TESTriggerLeaveEvent;
	struct TESUniqueIDChangeEvent;

	class SkyrimVM :
		public BSTSingletonSDM<SkyrimVM>,                      // 01A0
		public BSScript::IFreezeQuery,                         // 0000
		public BSScript::IStackCallbackSaveInterface,          // 0008
		public BSTEventSink<TESActivateEvent>,                 // 0010
		public BSTEventSink<TESActiveEffectApplyRemoveEvent>,  // 0018
		public BSTEventSink<TESActorLocationChangeEvent>,      // 0020
		public BSTEventSink<TESBookReadEvent>,                 // 0028
		public BSTEventSink<TESCellAttachDetachEvent>,         // 0030
		public BSTEventSink<TESCellFullyLoadedEvent>,          // 0038
		public BSTEventSink<TESCombatEvent>,                   // 0040
		public BSTEventSink<TESContainerChangedEvent>,         // 0048
		public BSTEventSink<TESDeathEvent>,                    // 0050
		public BSTEventSink<TESDestructionStageChangedEvent>,  // 0058
		public BSTEventSink<TESEnterBleedoutEvent>,            // 0060
		public BSTEventSink<TESEquipEvent>,                    // 0068
		public BSTEventSink<TESFormDeleteEvent>,               // 0070
		public BSTEventSink<TESFurnitureEvent>,                // 0078
		public BSTEventSink<TESGrabReleaseEvent>,              // 0080
		public BSTEventSink<TESHitEvent>,                      // 0088
		public BSTEventSink<TESInitScriptEvent>,               // 0090
		public BSTEventSink<TESLoadGameEvent>,                 // 0098
		public BSTEventSink<TESLockChangedEvent>,              // 00A0
		public BSTEventSink<TESMagicEffectApplyEvent>,         // 00A8
		public BSTEventSink<TESMagicWardHitEvent>,             // 00B0
		public BSTEventSink<TESMoveAttachDetachEvent>,         // 00B8
		public BSTEventSink<TESObjectLoadedEvent>,             // 00C0
		public BSTEventSink<TESObjectREFRTranslationEvent>,    // 00C8
		public BSTEventSink<TESOpenCloseEvent>,                // 00D0
		public BSTEventSink<TESPackageEvent>,                  // 00D8
		public BSTEventSink<TESPerkEntryRunEvent>,             // 00E0
		public BSTEventSink<TESQuestInitEvent>,                // 00E8
		public BSTEventSink<TESQuestStageEvent>,               // 00F0
		public BSTEventSink<TESResetEvent>,                    // 00F8
		public BSTEventSink<TESResolveNPCTemplatesEvent>,      // 0100
		public BSTEventSink<TESSceneEvent>,                    // 0108
		public BSTEventSink<TESSceneActionEvent>,              // 0110
		public BSTEventSink<TESScenePhaseEvent>,               // 0118
		public BSTEventSink<TESSellEvent>,                     // 0120
		public BSTEventSink<TESSleepStartEvent>,               // 0128
		public BSTEventSink<TESSleepStopEvent>,                // 0130
		public BSTEventSink<TESSpellCastEvent>,                // 0138
		public BSTEventSink<TESTopicInfoEvent>,                // 0140
		public BSTEventSink<TESTrackedStatsEvent>,             // 0148
		public BSTEventSink<TESTrapHitEvent>,                  // 0150
		public BSTEventSink<TESTriggerEvent>,                  // 0158
		public BSTEventSink<TESTriggerEnterEvent>,             // 0160
		public BSTEventSink<TESTriggerLeaveEvent>,             // 0168
		public BSTEventSink<TESUniqueIDChangeEvent>,           // 0170
		public BSTEventSink<TESSwitchRaceCompleteEvent>,       // 0178
		public BSTEventSink<TESPlayerBowShotEvent>,            // 0180
		public BSTEventSink<TESFastTravelEndEvent>,            // 0188
		public BSTEventSink<PositionPlayerEvent>,              // 0190
		public BSTEventSink<BSScript::StatsEvent>,             // 0198
		public BSTEventSource<BSScript::StatsEvent>            // 01A8
	{
	public:
		inline static constexpr auto RTTI = RTTI_SkyrimVM;

		struct UpdateDataEvent : public BSIntrusiveRefCounted
		{
		public:
			enum class UpdateType : bool
			{
				kRepeat = 0,    // RegisterForUpdate/RegisterForUpdateGameTime
				kNoRepeat = 1,  // RegisterForSingleUpdate/RegisterForSingleUpdateGameTime
			};

			// members
			UpdateType    updateType;       // 04
			std::uint16_t pad06;            // 06
			std::uint32_t timeToSendEvent;  // 08 - updateTime + currentVMMenuMode/currentVMDaysPassed
			std::uint32_t updateTime;       // 0C
			VMHandle      handle;           // 10
		};
		static_assert(sizeof(UpdateDataEvent) == 0x18);

		struct WaitCall
		{
		public:
			// members
			std::uint32_t              timeToSendEvent;  // 00 - Same as UpdateDataEvent, updateTime is discarded
			VMStackID                  stackID;          // 04 - used for vm->ReturnFromLatent()
			BSScript::IVirtualMachine* vm;               // 08
		};
		static_assert(sizeof(WaitCall) == 0x10);

		struct LOSDataEvent : public BSIntrusiveRefCounted
		{
		public:
			enum class LOSEventType
			{
				kGain = 0,
				kLost,
				kBoth,
			};

			enum class PreviousLOS
			{
				kLOS = 1,
				kNoLOS = 2
			};

			// members
			std::uint32_t pad04;               // 04
			VMHandle      handle;              // 08
			FormID        akViewerFormID;      // 10
			FormID        akTargetFormID;      // 14
			LOSEventType  losEventType;        // 18
			PreviousLOS   lastLOSCheckResult;  // 1C - (2 - (akViewer::HasLOS(akTarget) != 0))
		};
		static_assert(sizeof(LOSDataEvent) == 0x20);

		struct InventoryEventFilterLists : public BSIntrusiveRefCounted
		{
			// members
			BSTSet<FormID> itemsForFiltering;      // 08
			BSTSet<FormID> itemListsForFiltering;  // 38
		};
		static_assert(sizeof(InventoryEventFilterLists) == 0x68);

		struct ISendEventFilter
		{
			virtual bool matchesFilter(VMHandle handle) = 0;
		};

		~SkyrimVM() override;  // 00

		static SkyrimVM* GetSingleton();

		bool QueuePostRenderCall(const BSTSmartPointer<SkyrimScript::DelayFunctor>& a_functor);
		void RelayEvent(VMHandle handle, BSFixedString* event, BSScript::IFunctionArguments* args, ISendEventFilter* optionalFilter);
		void SendAndRelayEvent(VMHandle handle, BSFixedString* event, BSScript::IFunctionArguments* args, ISendEventFilter* optionalFilter);

		// The members are in two blocks because AE 1.7 and VR moved them differently (IDA, SE 1.5.97, AE 1.6.1170,
		// AE 1.7.104, VR 1.4.15):
		// - AE 1.7 adds two event sink bases at 0x180 (TESAmiiboTouchEvent, TESAmiiboForcedStopDetectionEvent), so
		//   every base from there and all members lie 0x10 later than on AE 1.6.
		// - VR has the same members up to 0x75F and inserts 0x20 bytes at 0x760.
		// Builds with AE or VR reach the members through GetRuntimeData() and GetEventRuntimeData(). The casts to
		// the bases from 0x180 on are only correct on SE and AE 1.6; VR has PositionPlayerEvent at 0x188, StatsEvent
		// at 0x190 and MenuOpenCloseEvent at 0x198, and no TESFastTravelEndEvent sink.
		struct RUNTIME_DATA
		{
#define RUNTIME_DATA_CONTENT                                                                                                                     \
	BSTSmartPointer<BSScript::IVirtualMachine>                            impl;                         /* 0000 */                                  \
	BSScript::IVMSaveLoadInterface*                                       saveLoadInterface;            /* 0008 */                                  \
	BSScript::IVMDebugInterface*                                          debugInterface;               /* 0010 */                                  \
	BSScript::SimpleAllocMemoryPagePolicy                                 memoryPagePolicy;             /* 0018 */                                  \
	BSScript::CompiledScriptLoader                                        scriptLoader;                 /* 0040 */                                  \
	SkyrimScript::Logger                                                  logger;                       /* 0078 */                                  \
	SkyrimScript::HandlePolicy                                            handlePolicy;                 /* 0128 */                                  \
	SkyrimScript::ObjectBindPolicy                                        objectBindPolicy;             /* 0198 */                                  \
	BSTSmartPointer<SkyrimScript::Store>                                  scriptStore;                  /* 0270 */                                  \
	SkyrimScript::FragmentSystem                                          fragmentSystem;               /* 0278 */                                  \
	SkyrimScript::Profiler                                                profiler;                     /* 0390 */                                  \
	SkyrimScript::SavePatcher                                             savePatcher;                  /* 0470 */                                  \
	mutable BSSpinLock                                                    frozenLock;                   /* 0478 */                                  \
	std::uint32_t                                                         isFrozen;                     /* 0480 */                                  \
	mutable BSSpinLock                                                    currentVMTimeLock;            /* 0484 */                                  \
	std::uint32_t                                                         currentVMTime;                /* 048C */                                  \
	std::uint32_t                                                         currentVMMenuModeTime;        /* 0490 */                                  \
	std::uint32_t                                                         currentVMGameTime;            /* 0494 */                                  \
	std::uint32_t                                                         currentVMDaysPassed;          /* 0498 Calender.GetDaysPassed() * 1000 */  \
	mutable BSSpinLock                                                    queuedWaitEventLock;          /* 049C */                                  \
	std::uint32_t                                                         pad06A4;                      /* 04A4 */                                  \
	BSTArray<WaitCall>                                                    queuedWaitCalls;              /* 04A8 Utility.Wait() calls */             \
	BSTArray<WaitCall>                                                    queuedWaitMenuModeCalls;      /* 04C0 Utility.WaitMenuMode() calls */     \
	BSTArray<WaitCall>                                                    queuedWaitGameCalls;          /* 04D8 Utility.WaitGameTime() calls */     \
	mutable BSSpinLock                                                    queuedLOSEventCheckLock;      /* 04F0 */                                  \
	BSTArray<BSTSmartPointer<LOSDataEvent>>                               queuedLOSEventChecks;         /* 04F8 OnGainLOS/OnLostLOS */              \
	std::uint32_t                                                         currentLOSEventCheckIndex;    /* 0510 */                                  \
	mutable BSSpinLock                                                    queuedOnUpdateEventLock;      /* 0514 */                                  \
	std::uint32_t                                                         pad071C;                      /* 051C */                                  \
	BSTArray<BSTSmartPointer<UpdateDataEvent>>                            queuedOnUpdateEvents;         /* 0520 */                                  \
	BSTArray<BSTSmartPointer<UpdateDataEvent>>                            queuedOnUpdateGameEvents;     /* 0538 */                                  \
	std::uint32_t                                                         unk0750;                      /* 0550 */                                  \
	mutable BSSpinLock                                                    registeredSleepEventsLock;    /* 0554 */                                  \
	std::uint32_t                                                         pad075C;                      /* 055C */

			RUNTIME_DATA_CONTENT
		};
		static_assert(sizeof(RUNTIME_DATA) == 0x560);

		struct EVENT_RUNTIME_DATA
		{
#define EVENT_RUNTIME_DATA_CONTENT                                                                                                         \
	BSTSet<VMHandle>                                                      registeredSleepEvents;        /* 0000 RegisterForSleep */           \
	mutable BSSpinLock                                                    registeredStatsEventsLock;    /* 0030 */                            \
	BSTSet<VMHandle>                                                      registeredStatsEvents;        /* 0038 RegisterForTrackedStats */    \
	BSTStaticFreeList<BSTSmartPointer<SkyrimScript::DelayFunctor>, 512>   renderSafeFunctorPool1;       /* 0068 */                            \
	BSTCommonLLMessageQueue<BSTSmartPointer<SkyrimScript::DelayFunctor>>  renderSafeFunctorQueue1;      /* 2080 */                            \
	BSTStaticFreeList<BSTSmartPointer<SkyrimScript::DelayFunctor>, 512>   renderSafeFunctorPool2;       /* 20A8 */                            \
	BSTCommonLLMessageQueue<BSTSmartPointer<SkyrimScript::DelayFunctor>>  renderSafeFunctorQueue2;      /* 40C0 */                            \
	BSTStaticFreeList<BSTSmartPointer<SkyrimScript::DelayFunctor>, 512>   postRenderFunctorPool1;       /* 40E8 */                            \
	BSTCommonLLMessageQueue<BSTSmartPointer<SkyrimScript::DelayFunctor>>  postRenderFunctorQueue1;      /* 6100 */                            \
	BSTStaticFreeList<BSTSmartPointer<SkyrimScript::DelayFunctor>, 512>   postRenderFunctorPool2;       /* 6128 */                            \
	BSTCommonLLMessageQueue<BSTSmartPointer<SkyrimScript::DelayFunctor>>  postRenderFunctorQueue2;      /* 8140 */                            \
	mutable BSSpinLock                                                    renderSafeQueueLock;          /* 8168 */                            \
	BSTCommonLLMessageQueue<BSTSmartPointer<SkyrimScript::DelayFunctor>>* renderSafeQueueToReadFrom;    /* 8170 */                            \
	BSTCommonLLMessageQueue<BSTSmartPointer<SkyrimScript::DelayFunctor>>* renderSafeQueueToWriteTo;     /* 8178 */                            \
	mutable BSSpinLock                                                    postRenderQueueLock;          /* 8180 */                            \
	BSTCommonLLMessageQueue<BSTSmartPointer<SkyrimScript::DelayFunctor>>* postRenderQueueToReadFrom;    /* 8188 */                            \
	BSTCommonLLMessageQueue<BSTSmartPointer<SkyrimScript::DelayFunctor>>* postRenderQueueToWriteTo;     /* 8190 */                            \
	mutable BSSpinLock                                                    userLogMapLock;               /* 8198 */                            \
	BSTHashMap<const char*, SkyrimScript::Logger*>                        userLogMap;                   /* 81A0 Debug.OpenUserLog() */        \
	mutable BSSpinLock                                                    currentVMOverstressTimeLock;  /* 81D0 */                            \
	std::uint32_t                                                         currentVMOverstressTime;      /* 81D8 */                            \
	std::uint32_t                                                         lastVMStackDumpTime;          /* 81DC */                            \
	mutable BSSpinLock                                                    InventoryEventFilterMapLock;  /* 81E0 */                            \
	BSTHashMap<VMHandle, InventoryEventFilterLists*>                      InventoryEventFilterMap;      /* 81E8 AddInventoryEventFilter() */

			EVENT_RUNTIME_DATA_CONTENT
		};
		static_assert(sizeof(EVENT_RUNTIME_DATA) == 0x8218);

		// impl .. pad075C: SE, AE 1.6 and VR 0x200; AE 1.7 0x210.
		[[nodiscard]] inline RUNTIME_DATA& GetRuntimeData() noexcept
		{
			return REL::RuntimeMember<RUNTIME_DATA>(this, 0x200, 0x200, 0x210, 0x200);
		}

		[[nodiscard]] inline const RUNTIME_DATA& GetRuntimeData() const noexcept
		{
			return REL::RuntimeMember<RUNTIME_DATA>(this, 0x200, 0x200, 0x210, 0x200);
		}

		// registeredSleepEvents .. InventoryEventFilterMap: SE and AE 1.6 0x760; AE 1.7 0x770; VR 0x780.
		[[nodiscard]] inline EVENT_RUNTIME_DATA& GetEventRuntimeData() noexcept
		{
			return REL::RuntimeMember<EVENT_RUNTIME_DATA>(this, 0x760, 0x760, 0x770, 0x780);
		}

		[[nodiscard]] inline const EVENT_RUNTIME_DATA& GetEventRuntimeData() const noexcept
		{
			return REL::RuntimeMember<EVENT_RUNTIME_DATA>(this, 0x760, 0x760, 0x770, 0x780);
		}

		// members
#ifndef ENABLE_SKYRIM_AE
		RUNTIME_DATA_CONTENT  // 200
#	ifndef ENABLE_SKYRIM_VR
		EVENT_RUNTIME_DATA_CONTENT  // 760
#	endif
#endif
	};
#if !defined(ENABLE_SKYRIM_AE) && !defined(ENABLE_SKYRIM_VR)
	static_assert(sizeof(SkyrimVM) == 0x8978);
#endif
}
#undef RUNTIME_DATA_CONTENT
#undef EVENT_RUNTIME_DATA_CONTENT
