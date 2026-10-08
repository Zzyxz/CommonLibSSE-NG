#pragma once

#include "RE/B/BSAtomic.h"
#include "RE/B/bhkSerializable.h"

namespace RE
{
	struct bhkPickData;
	class BGSAcousticSpaceListener;
	class hkpSuspendInactiveAgentsUtil;
	class NiAVObject;

	class bhkWorld : public bhkSerializable
	{
	public:
		inline static constexpr auto RTTI = RTTI_bhkWorld;
		inline static auto           Ni_RTTI = NiRTTI_bhkWorld;
		inline static constexpr auto VTABLE = VTABLE_bhkWorld;

		class bhkConstraintProjector;

		~bhkWorld() override;  // 00

		// override (bhkSerializable)
		const NiRTTI* GetRTTI() const override;                                    // 02
		void          SetReferencedObject(hkReferencedObject* a_object) override;  // 25
		void          AdjustRefCount(bool a_increment) override;                   // 26
		hkpWorld*     GetWorld1() override;                                        // 27 - { return referencedObject.ptr; }
		ahkpWorld*    GetWorld2() override;                                        // 28 - { return referencedObject.ptr; }
		void          Unk_2B(void) override;                                       // 2B
		void          Unk_2C(void) override;                                       // 2C - { return 1; }
		void          Unk_2E(void) override;                                       // 2E
		void          Unk_2F(void) override;                                       // 2F

		// add
		virtual void Unk_32(void);                                              // 32
		virtual bool PickObject(bhkPickData& a_pickData);                       // 33
		virtual void Unk_34(void);                                              // 34
		virtual void Unk_35(void);                                              // 35
		virtual void InitHavok(NiAVObject* a_sceneObject, NiAVObject* a_root);  // 36

		static float GetWorldScale()
		{
			REL::Relocation<float*> worldScale{ RELOCATION_ID(231896, 188105) };
			return *worldScale;
		}

		static float GetWorldScaleInverse()
		{
			REL::Relocation<float*> worldScaleInverse{ RELOCATION_ID(230692, 187407) };
			return *worldScaleInverse;
		}

		// AE 1.7 inserts 0x108 bytes at 0xC5D8 (a new 0x100-byte sub-object at 0xC5E0), so the members from unkC5D8
		// on lie 0x108 later (IDA, 1.6.1170 vs 1.7.104; size 0xC600 -> 0xC710). Builds with AE reach them through
		// GetStepData(). VR has the SE layout.
		struct STEP_DATA
		{
#define STEP_DATA_CONTENT                                          \
	std::uint32_t unkC5D8;          /* 00 - incremented per frame */  \
	std::uint32_t unkC5DC;          /* 04 */                          \
	std::uint32_t unkC5E0;          /* 08 */                          \
	std::uint32_t unkC5E4;          /* 0C */                          \
	std::uint32_t unkC5E8;          /* 10 */                          \
	std::uint32_t unkC5EC;          /* 14 */                          \
	float         tau;              /* 18 */                          \
	float         damping;          /* 1C */                          \
	std::uint8_t  unkC5F8;          /* 20 */                          \
	bool          toggleCollision;  /* 21 */                          \
	std::uint16_t unkC5FA;          /* 22 */                          \
	std::uint16_t unkC5FC;          /* 24 */                          \
	std::uint16_t unkC5FE;          /* 26 */

			STEP_DATA_CONTENT
		};
		static_assert(sizeof(STEP_DATA) == 0x28);

		// SE, AE 1.6 and VR 0xC5D8; AE 1.7 0xC6E0.
		[[nodiscard]] inline STEP_DATA& GetStepData() noexcept
		{
			return REL::RuntimeMember<STEP_DATA>(this, 0xC5D8, 0xC5D8, 0xC6E0, 0xC5D8);
		}

		[[nodiscard]] inline const STEP_DATA& GetStepData() const noexcept
		{
			return REL::RuntimeMember<STEP_DATA>(this, 0xC5D8, 0xC5D8, 0xC6E0, 0xC5D8);
		}

		// members
		std::uint8_t                  unk0020[0x320];             // 0020
		std::uint8_t                  unk0340[0x6400];            // 0340
		std::uint8_t                  unk6740[0x5DC0];            // 6740
		BSTArray<void*>               unkC500;                    // C500
		BSTArray<void*>               unkC518;                    // C518
		BSTArray<void*>               unkC530;                    // C530
		BSTArray<void*>               unkC548;                    // C548
		std::uint64_t                 unkC560;                    // C560
		std::uint32_t                 unkC568;                    // C568
		float                         unkC56C;                    // C56C
		bhkConstraintProjector*       constraintProjector;        // C570
		std::uint64_t                 unkC578;                    // C578
		std::uint32_t                 unkC580;                    // C580
		float                         unkC584;                    // C584
		std::uint64_t                 unkC588;                    // C588
		std::uint64_t                 unkC590;                    // C590
		mutable BSReadWriteLock       worldLock;                  // C598
		mutable BSReadWriteLock       unkC5A0;                    // C5A0
		std::uint64_t                 unkC5A8;                    // C5A8
		hkVector4                     unkC5B0;                    // C5B0
		std::uint64_t                 unkC5C0;                    // C5C0
		BGSAcousticSpaceListener*     acousticSpaceListener;      // C5C8
		hkpSuspendInactiveAgentsUtil* suspendInactiveAgentsUtil;  // C5D0
#ifndef ENABLE_SKYRIM_AE
		STEP_DATA_CONTENT  // C5D8
#endif
	};
#ifndef ENABLE_SKYRIM_AE
	static_assert(sizeof(bhkWorld) == 0xC600);
#endif
}
#undef STEP_DATA_CONTENT
