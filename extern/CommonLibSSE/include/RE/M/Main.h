#pragma once

#include "RE/B/BSTEvent.h"
#include "RE/B/BSTMessageQueue.h"
#include "RE/S/ScrapHeap.h"

#include "REX/W32/BASE.h"

namespace RE
{
	class NiNode;
	class NiCamera;
	class Scenegraph;
	class ScrapHeap;
	struct BSGamerProfileEvent;
	struct BSPackedTask;
	struct PositionPlayerEvent;

	struct BSPackedTaskQueue
	{
	public:
		using UnpackFunc_t = void(const BSPackedTask*);

		struct Semaphore
		{
		public:
			// members
			void*         handle;    // 00
			std::uint32_t size;      // 08
			std::uint32_t capacity;  // 0C
		};
		static_assert(sizeof(Semaphore) == 0x10);

		// members
		BSTCommonScrapHeapMessageQueue<BSPackedTask> queue;       // 00
		mutable Semaphore                            semaphore;   // 28
		UnpackFunc_t*                                unpackFunc;  // 38
	};
	static_assert(sizeof(BSPackedTaskQueue) == 0x40);

	struct BSSaveDataSystemUtilityImage
	{
	public:
		// members
		std::uint32_t size;    // 00
		std::uint32_t width;   // 04
		std::uint32_t height;  // 08
		std::uint32_t pad0C;   // 0C
		char*         buffer;  // 10
	};
	static_assert(sizeof(BSSaveDataSystemUtilityImage) == 0x18);

	// Layout verified in IDA (constructors, SetActive, Main::Update) on SE 1.5.97, AE 1.6.1170, AE 1.7.104 and
	// VR 1.4.15. AE matches SE. VR has no BSTEventSink<BSGamerProfileEvent> base, so quitGame .. secondaryTaskQueue
	// lie 8 bytes earlier; VR then adds two refcounted pointers at 0x1D0, so unk1D8 .. saveDataIconImages lie
	// 8 bytes later (size 0x278 instead of 0x270). Builds that include VR reach the members through
	// GetRuntimeData() / GetRuntimeData2().
	class Main :
		public BSTEventSink<PositionPlayerEvent>  // 00
#if !defined(ENABLE_SKYRIM_VR) || defined(ENABLE_SKYRIM_SE) || defined(ENABLE_SKYRIM_AE)
		,
		public BSTEventSink<BSGamerProfileEvent>  // 08 - SE/AE only
#endif
	{
	public:
		inline static constexpr auto RTTI = RTTI_Main;

		~Main() override;  // 00

		// override (BSTEventSink<PositionPlayerEvent>)
		BSEventNotifyControl ProcessEvent(const PositionPlayerEvent* a_event, BSTEventSource<PositionPlayerEvent>* a_eventSource) override;  // 01 - { return BSEventNotifyControl::kContinue; }

#if !defined(ENABLE_SKYRIM_VR) || defined(ENABLE_SKYRIM_SE) || defined(ENABLE_SKYRIM_AE)
		// override (BSTEventSink<BSGamerProfileEvent>)
		BSEventNotifyControl ProcessEvent(const BSGamerProfileEvent* a_event, BSTEventSource<BSGamerProfileEvent>* a_eventSource) override;  // 01
#endif

		static Main* GetSingleton();

		static float       QFrameAnimTime();
		static NiCamera*   WorldRootCamera();
		static Scenegraph* WorldRootNode();

		void SetActive(bool a_active);

#define RUNTIME_DATA_CONTENT                                                      \
	bool                quitGame;                /* 000 */                       \
	bool                resetGame;               /* 001 */                       \
	bool                fullReset;               /* 002 */                       \
	bool                gameActive;              /* 003 */                       \
	bool                onIdle;                  /* 004 */                       \
	bool                reloadContent;           /* 005 */                       \
	bool                freezeTime;              /* 006 */                       \
	bool                freezeNextFrame;         /* 007 */                       \
	REX::W32::HWND      wnd;                     /* 008 */                       \
	REX::W32::HINSTANCE instance;                /* 010 */                       \
	std::uint32_t       threadID;                /* 018 */                       \
	std::uint32_t       unk02C;                  /* 01C */                       \
	std::uint64_t       unk030;                  /* 020 */                       \
	ScrapHeap           packedTaskHeap;          /* 028 */                       \
	BSPackedTaskQueue   taskQueue;               /* 0B8 */                       \
	ScrapHeap           secondaryPackedTaskHeap; /* 0F8 */                       \
	BSPackedTaskQueue   secondaryTaskQueue;      /* 188 */

#define RUNTIME_DATA2_CONTENT                                                      \
	std::uint8_t                 unk1D8;                      /* 00 */            \
	std::uint8_t                 unk1D9;                      /* 01 */            \
	std::uint16_t                unk1DA;                      /* 02 */            \
	std::uint32_t                unk1DC;                      /* 04 */            \
	BSSaveDataSystemUtilityImage saveDataBackgroundImages[3]; /* 08 */            \
	BSSaveDataSystemUtilityImage saveDataIconImages[3];       /* 50 */

		// SE/AE 0x10, VR 0x08.
		struct RUNTIME_DATA
		{
			RUNTIME_DATA_CONTENT
		};
		static_assert(sizeof(RUNTIME_DATA) == 0x1C8);

		// SE/AE 0x1D8, VR 0x1E0.
		struct RUNTIME_DATA2
		{
			RUNTIME_DATA2_CONTENT
		};
		static_assert(sizeof(RUNTIME_DATA2) == 0x98);

		[[nodiscard]] inline RUNTIME_DATA& GetRuntimeData() noexcept
		{
			return REL::RelocateMember<RUNTIME_DATA>(this, 0x10, 0x08);
		}

		[[nodiscard]] inline const RUNTIME_DATA& GetRuntimeData() const noexcept
		{
			return REL::RelocateMember<RUNTIME_DATA>(this, 0x10, 0x08);
		}

		[[nodiscard]] inline RUNTIME_DATA2& GetRuntimeData2() noexcept
		{
			return REL::RelocateMember<RUNTIME_DATA2>(this, 0x1D8, 0x1E0);
		}

		[[nodiscard]] inline const RUNTIME_DATA2& GetRuntimeData2() const noexcept
		{
			return REL::RelocateMember<RUNTIME_DATA2>(this, 0x1D8, 0x1E0);
		}

		// members
#ifndef ENABLE_SKYRIM_VR
		RUNTIME_DATA_CONTENT   // 010
		RUNTIME_DATA2_CONTENT  // 1D8
#elif !defined(ENABLE_SKYRIM_SE) && !defined(ENABLE_SKYRIM_AE)
		RUNTIME_DATA_CONTENT     // 008
		void* unkVR1D0[2];       // 1D0 - VR only: two refcounted pointers
		RUNTIME_DATA2_CONTENT    // 1E0
#endif
	};
#undef RUNTIME_DATA_CONTENT
#undef RUNTIME_DATA2_CONTENT
#ifndef ENABLE_SKYRIM_VR
	static_assert(sizeof(Main) == 0x270);
#elif !defined(ENABLE_SKYRIM_SE) && !defined(ENABLE_SKYRIM_AE)
	static_assert(sizeof(Main) == 0x278);
#else
	static_assert(sizeof(Main) == 0x10);  // compiled size; 0x270 on SE/AE and 0x278 on VR at runtime
#endif
}
