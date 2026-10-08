#pragma once

#include "RE/A/AITimer.h"
#include "RE/B/BSAtomic.h"
#include "RE/B/BSPointerHandle.h"
#include "RE/B/BSTArray.h"
#include "RE/C/CombatState.h"
#include "RE/N/NiSmartPointer.h"

namespace RE
{
	class CombatAimController;
	class CombatAreaStandard;
	class CombatBehaviorController;
	class CombatBlackboard;
	class CombatGroup;
	class CombatInventory;
	class CombatTargetSelectorStandard;
	class CombatState;
	class TESCombatStyle;

	class CombatController
	{
	public:
		[[nodiscard]] bool IsFleeing() const
		{
			return state->isFleeing;
		}

		// members
		CombatGroup*                   combatGroup;           // 00
		CombatState*                   state;                 // 08
		CombatInventory*               inventory;             // 10
		CombatBlackboard*              blackboard;            // 18
		CombatBehaviorController*      behaviorController;    // 20
		ActorHandle                    attackerHandle;        // 28
		ActorHandle                    targetHandle;          // 2C
		ActorHandle                    previousTargetHandle;  // 30
		std::uint8_t                   unk34;                 // 34
		bool                           startedCombat;         // 35
		std::uint8_t                   unk36;                 // 36
		std::uint8_t                   unk37;                 // 37
		TESCombatStyle*                combatStyle;           // 38
		bool                           stoppedCombat;         // 40
		bool                           unk41;                 // 41 - isbeingMeleeAttacked?
		bool                           ignoringCombat;        // 42
		bool                           inactive;              // 43
		AITimer                        unk44;                 // 44
		float                          unk4C;                 // 4C
		BSTArray<CombatAimController*> aimControllers;        // 50

		// AE adds a spin lock for aimControllers at 0x68, so the fields after it lie 8 bytes later.
		struct RUNTIME_DATA
		{
#define RUNTIME_DATA_CONTENT                                                                         \
	CombatAimController*                    currentAimController;   /* 68, 70 */                    \
	CombatAimController*                    previousAimController;  /* 70, 78 */                    \
	BSTArray<CombatAreaStandard*>           areas;                  /* 78, 80 */                    \
	CombatAreaStandard*                     currentArea;            /* 90, 98 */                    \
	BSTArray<CombatTargetSelectorStandard*> targetSelectors;        /* 98, A0 */                    \
	CombatTargetSelectorStandard*           currentTargetSelector;  /* B0, B8 */                    \
	CombatTargetSelectorStandard*           previousTargetSelector; /* B8, C0 */                    \
	std::uint32_t                           handleCount;            /* C0, C8 */                    \
	std::int32_t                            unkC4;                  /* C4, CC */                    \
	NiPointer<Actor>                        cachedAttacker;         /* C8, D0 - attackerHandle */   \
	NiPointer<Actor>                        cachedTarget;           /* D0, D8 - targetHandle */

			RUNTIME_DATA_CONTENT
		};
		static_assert(sizeof(RUNTIME_DATA) == 0x70);

		[[nodiscard]] RUNTIME_DATA& GetRuntimeData() noexcept
		{
			return REL::RuntimeMember<RUNTIME_DATA>(this, 0x68, 0x70);
		}

		[[nodiscard]] const RUNTIME_DATA& GetRuntimeData() const noexcept
		{
			return REL::RuntimeMember<RUNTIME_DATA>(this, 0x68, 0x70);
		}

		// AE only (nullptr on SE).
		[[nodiscard]] BSSpinLock* GetAimControllerLock() const noexcept
		{
			return REL::Module::IsAE() ? std::addressof(REL::RelocateMember<BSSpinLock>(const_cast<CombatController*>(this), 0x68)) : nullptr;
		}

#ifndef ENABLE_SKYRIM_AE
		RUNTIME_DATA_CONTENT
#endif
	};
#ifndef ENABLE_SKYRIM_AE
	static_assert(sizeof(CombatController) == 0xD8);
#endif
}
#undef RUNTIME_DATA_CONTENT
