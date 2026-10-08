#pragma once

#include "RE/N/NiPoint3.h"
#include "RE/P/PlayerInputHandler.h"
#include "RE/T/TESCameraState.h"

namespace RE
{
	class NiNode;

	class FirstPersonState :
		public TESCameraState,     // 00
		public PlayerInputHandler  // 20
	{
	public:
		inline static constexpr auto RTTI = RTTI_FirstPersonState;
		inline static constexpr auto VTABLE = VTABLE_FirstPersonState;

		~FirstPersonState() override;  // 00

		// override (TESCameraState)
		void Begin() override;                                               // 01
		void End() override;                                                 // 02
		void Update(BSTSmartPointer<TESCameraState>& a_nextState) override;  // 03
		void GetRotation(NiQuaternion& a_rotation) override;                 // 04
		void GetTranslation(NiPoint3& a_translation) override;               // 05
		void SaveGame(BGSSaveFormBuffer* a_buf) override;                    // 06
		void LoadGame(BGSLoadFormBuffer* a_buf) override;                    // 07
		void Revert(BGSLoadFormBuffer* a_buf) override;                      // 08

		// override (PlayerInputHandler)
		bool CanProcess(InputEvent* a_event) override;                                          // 01
		void ProcessButton(ButtonEvent* a_event, PlayerControlsData* a_movementData) override;  // 04

		// AE 1.7 inserts 4 bytes at 0x7C (IDA, 1.6.1170 vs 1.7.104; size stays 0x90), so the members from unk7C on
		// are reached through GetOverrideData() in builds with AE. VR has the SE layout.
		// On AE 1.7 and VR the PlayerInputHandler functions from slot 02 on are two slots later (GameSlot).
		struct OVERRIDE_DATA
		{
#define OVERRIDE_DATA_CONTENT                                        \
	float         unk7C;               /* 00 */                         \
	std::uint32_t unk80;               /* 04 */                         \
	bool          cameraOverride;      /* 08 */                         \
	bool          cameraPitchOverride; /* 09 */                         \
	std::uint16_t unk86;               /* 0A */                         \
	std::uint8_t  unk88;               /* 0C - set by ProcessButton */  \
	std::uint8_t  pad89[3];            /* 0D */

			OVERRIDE_DATA_CONTENT
		};
		static_assert(sizeof(OVERRIDE_DATA) == 0x10);

		// SE, AE 1.6 and VR 0x7C; AE 1.7 0x80.
		[[nodiscard]] inline OVERRIDE_DATA& GetOverrideData() noexcept
		{
			return REL::RuntimeMember<OVERRIDE_DATA>(this, 0x7C, 0x7C, 0x80, 0x7C);
		}

		[[nodiscard]] inline const OVERRIDE_DATA& GetOverrideData() const noexcept
		{
			return REL::RuntimeMember<OVERRIDE_DATA>(this, 0x7C, 0x7C, 0x80, 0x7C);
		}

		// members
		NiPoint3      lastPosition;             // 30
		NiPoint3      lastFrameSpringVelocity;  // 3C
		NiPoint3      dampeningOffset;          // 48
		std::uint32_t pad54;                    // 54
		NiAVObject*   firstPersonCameraObj;     // 58
		NiNode*       firstPersonFOVControl;    // 60
		float         sittingRotation;          // 68
		float         unk6C;                    // 6C
		float         unk70;                    // 70
		float         currentPitchOffset;       // 74 - [-100, 100]
		float         targetPitchOffset;        // 78 - [-100, 100]
#ifndef ENABLE_SKYRIM_AE
		OVERRIDE_DATA_CONTENT  // 7C
#else
		std::uint8_t pad7C[0x14];  // 7C - see GetOverrideData()
#endif
	};
	static_assert(sizeof(FirstPersonState) == 0x90);
}
#undef OVERRIDE_DATA_CONTENT
