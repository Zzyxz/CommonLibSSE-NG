#pragma once

#include "RE/B/BSAtomic.h"
#include "RE/B/BSPointerHandle.h"
#include "RE/B/BSTArray.h"
#include "RE/B/BSTSingleton.h"
#include "RE/B/BSTSmartPointer.h"
#include "RE/N/NiPoint3.h"
#include "RE/T/TESCamera.h"

namespace RE
{
	class bhkRigidBody;
	class bhkSimpleShapePhantom;
	class NiRefObject;
	class TESCameraState;

	struct CameraStates
	{
		enum CameraState : std::uint32_t
		{
			kFirstPerson = 0,
			kAutoVanity,
			kVATS,
			kFree,
			kIronSights,
			kFurniture,
			kPCTransition,
			kTween,
			kAnimated,
			kThirdPerson,
			kMount,
			kBleedout,
			kDragon,

			kTotal
		};
	};
	using CameraState = CameraStates::CameraState;

	class PlayerCamera :
		public TESCamera,                     // 000
		public BSTSingletonSDM<PlayerCamera>  // 038
	{
	public:
		inline static constexpr auto RTTI = RTTI_PlayerCamera;

		struct Unk120
		{
			NiPointer<bhkSimpleShapePhantom*> unk00;  // 00
			NiPointer<bhkSimpleShapePhantom*> unk08;  // 08
		};
		static_assert(sizeof(Unk120) == 0x10);

		~PlayerCamera() override;  // 00

		// override (TESCamera)
		void SetCameraRoot(NiPointer<NiNode> a_root) override;  // 01

		static PlayerCamera* GetSingleton();

		bool ForceFirstPerson();
		bool ForceThirdPerson();
		bool IsInBleedoutMode() const;
		bool IsInFirstPerson() const;
		bool IsInFreeCameraMode() const;
		bool IsInThirdPerson() const;
		void ToggleFreeCameraMode(bool a_freezeTime);
		void UpdateThirdPerson(bool a_weaponDrawn);

		// VR has 14 camera states: it inserts its own VrCameraState at id 9 (the normal VR gameplay camera), so
		// third person, mount, bleedout and dragon are 10..13 there. Verified in IDA (state factory SE 0x84A280 /
		// VR 0x875560). CameraState keeps the SE numbering; these translate.
		static constexpr std::uint32_t kVRCameraStateID = 9;

		[[nodiscard]] static std::uint32_t ToGameStateID(CameraState a_state) noexcept
		{
			const auto id = static_cast<std::uint32_t>(a_state);
			return REL::Module::IsVR() && id >= kVRCameraStateID ? id + 1 : id;
		}

		// The SE-numbered state for a game state id; no value for VR's own camera state (see IsInVRCameraMode).
		[[nodiscard]] static std::optional<CameraState> FromGameStateID(std::uint32_t a_id) noexcept
		{
			if (REL::Module::IsVR()) {
				if (a_id == kVRCameraStateID) {
					return std::nullopt;
				}
				if (a_id > kVRCameraStateID) {
					--a_id;
				}
			}
			return a_id < CameraState::kTotal ? std::optional{ static_cast<CameraState>(a_id) } : std::nullopt;
		}

		// The current state in SE numbering; no value without a state or in VR's own camera state.
		[[nodiscard]] std::optional<CameraState> GetCurrentState() const noexcept;

		// VR only: the player is in VR's normal gameplay camera (VrCameraState).
		[[nodiscard]] bool IsInVRCameraMode() const noexcept;

		// The state object for a camera state, or nullptr.
		[[nodiscard]] TESCameraState* GetCameraState(CameraState a_state) const noexcept
		{
			const auto* states = reinterpret_cast<const BSTSmartPointer<TESCameraState>*>(
				reinterpret_cast<std::uintptr_t>(this) + REL::Relocate<std::ptrdiff_t>(0xB8, 0xB8, 0xC0));
			return states[ToGameStateID(a_state)].get();
		}

		// VR moves the fields after tempReturnStates: +0x8 (14 inline states), the camera state array and the next
		// three fields +0x10, and from `lock` on +0x1C because VR inserts a BSSoundHandle at 0x144. Size SE/AE 0x168,
		// VR 0x188. Verified in IDA (ctor SE 0x849F90 / VR 0x875260, Update, dtor).
		struct PHYSICS_RUNTIME_DATA
		{
#define PHYSICS_RUNTIME_DATA_CONTENT                                       \
	Unk120*                 unk120;           /* 120, VR 130 - ? */       \
	NiPointer<bhkRigidBody> rigidBody;        /* 128, VR 138 - ? */       \
	RefHandle               objectFadeHandle; /* 130, VR 140 - ? */

			PHYSICS_RUNTIME_DATA_CONTENT
		};
		static_assert(sizeof(PHYSICS_RUNTIME_DATA) == 0x18);

		struct RUNTIME_DATA
		{
#define RUNTIME_DATA_CONTENT                                                              \
	mutable BSSpinLock lock;                /* 134, VR 150 */                          \
	float              worldFOV;            /* 13C, VR 158 */                          \
	float              firstPersonFOV;      /* 140, VR 15C */                          \
	NiPoint3           pos;                 /* 144, VR 160 - ? */                      \
	float              idleTimer;           /* 150, VR 16C - ? */                      \
	float              yaw;                 /* 154, VR 170 - ? - in radians */         \
	std::uint32_t      unk158;              /* 158, VR 174 - ? */                      \
	std::uint32_t      unk15C;              /* 15C, VR 178 - ? */                      \
	bool               allowAutoVanityMode; /* 160, VR 17C */                          \
	bool               bowZoomedIn;         /* 161, VR 17D */                          \
	bool               isWeapSheathed;      /* 162, VR 17E - ? */                      \
	bool               isProcessed;         /* 163, VR 17F - ? */                      \
	std::uint8_t       unk164;              /* 164, VR 180 */                          \
	std::uint8_t       unk165;              /* 165, VR 181 */                          \
	std::uint16_t      pad166;              /* 166, VR 182 */

			RUNTIME_DATA_CONTENT
		};
		static_assert(sizeof(RUNTIME_DATA) == 0x34);

		[[nodiscard]] PHYSICS_RUNTIME_DATA& GetPhysicsRuntimeData() noexcept
		{
			return REL::RuntimeMember<PHYSICS_RUNTIME_DATA>(this, 0x120, 0x120, 0x130);
		}

		[[nodiscard]] const PHYSICS_RUNTIME_DATA& GetPhysicsRuntimeData() const noexcept
		{
			return REL::RuntimeMember<PHYSICS_RUNTIME_DATA>(this, 0x120, 0x120, 0x130);
		}

		[[nodiscard]] RUNTIME_DATA& GetRuntimeData() noexcept
		{
			return REL::RuntimeMember<RUNTIME_DATA>(this, 0x134, 0x134, 0x150);
		}

		[[nodiscard]] const RUNTIME_DATA& GetRuntimeData() const noexcept
		{
			return REL::RuntimeMember<RUNTIME_DATA>(this, 0x134, 0x134, 0x150);
		}

		// members
		std::uint8_t  pad039;        // 039
		std::uint16_t pad03A;        // 03A
		ActorHandle   cameraTarget;  // 03C
#ifndef ENABLE_SKYRIM_VR
		BSTSmallArray<TESCameraState*, CameraStates::kTotal> tempReturnStates;                    // 040
		BSTSmartPointer<TESCameraState>                      cameraStates[CameraStates::kTotal];  // 0B8 - use GetCameraState()
		PHYSICS_RUNTIME_DATA_CONTENT
		RUNTIME_DATA_CONTENT
#endif

	private:
		bool QCameraEquals(CameraState a_cameraState) const;
	};
#ifndef ENABLE_SKYRIM_VR
	static_assert(sizeof(PlayerCamera) == 0x168);
#endif
#undef PHYSICS_RUNTIME_DATA_CONTENT
#undef RUNTIME_DATA_CONTENT
}
