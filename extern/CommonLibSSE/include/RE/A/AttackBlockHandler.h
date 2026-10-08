#pragma once

#include "RE/H/HeldStateHandler.h"

namespace RE
{
	class Setting;

	struct AttackBlockHandler : public HeldStateHandler
	{
	public:
		inline static constexpr auto RTTI = RTTI_AttackBlockHandler;

		enum class AttackType : std::uint8_t
		{
			kRight = 0,
			kLeft = 1,
			kDual = 2
		};

		~AttackBlockHandler() override;  // 00

		// override (PlayerInputHandler)
		bool CanProcess(InputEvent* a_event) override;                                  // 01
		void ProcessButton(ButtonEvent* a_event, PlayerControlsData* a_data) override;  // 04
		void UpdateHeldStateActive(const ButtonEvent* a_event) override;                // 05
		void SetHeldStateActive(bool a_flag) override;                                  // 06

		// The members lie at different offsets per game version (IDA, SE 1.5.97, AE 1.6.1170, AE 1.7.104, VR 1.4.15):
		// AE 1.7 adds eight gesture names and gesture state (size 0xC0), VR adds 0x48 bytes before the members
		// (size 0x90). Builds with AE or VR use these accessors; SE and AE 1.6 share one layout.
		// On AE 1.7 and VR the virtual functions from slot 02 on are two slots later, see PlayerInputHandler::GameSlot.
		[[nodiscard]] std::uint64_t& GetHeldTime() noexcept { return REL::RuntimeMember<std::uint64_t>(this, 0x18, 0x18, 0x58, 0x60); }  // GetTickCount64 value
		[[nodiscard]] BSFixedString& GetControlID() noexcept { return REL::RuntimeMember<BSFixedString>(this, 0x20, 0x20, 0x60, 0x68); }
		[[nodiscard]] AttackType&    GetAttackType() noexcept { return REL::RuntimeMember<AttackType>(this, 0x28, 0x28, 0x9C, 0x70); }
		[[nodiscard]] std::uint8_t&  GetAttackCount() noexcept { return REL::RuntimeMember<std::uint8_t>(this, 0x2C, 0x2C, 0xA0, 0x74); }  // VR offset not verified
		[[nodiscard]] Setting*&      GetInitialPowerAttackDelay() noexcept { return REL::RuntimeMember<Setting*>(this, 0x30, 0x30, 0xA8, 0x78); }
		[[nodiscard]] Setting*&      GetSubsequentPowerAttackDelay() noexcept { return REL::RuntimeMember<Setting*>(this, 0x38, 0x38, 0xB0, 0x80); }
		[[nodiscard]] bool&          GetIgnore() noexcept { return REL::RuntimeMember<bool>(this, 0x40, 0x40, 0xB8, 0x88); }
		[[nodiscard]] bool&          GetHeldLeft() noexcept { return REL::RuntimeMember<bool>(this, 0x42, 0x42, 0xBA, 0x8A); }
		[[nodiscard]] bool&          GetHeldRight() noexcept { return REL::RuntimeMember<bool>(this, 0x43, 0x43, 0xBB, 0x8B); }

		// members
#if !defined(ENABLE_SKYRIM_AE) && !defined(ENABLE_SKYRIM_VR)
		std::uint32_t heldTimeMs;                  // 18 - low half of a GetTickCount64 value
		std::uint32_t unk1C;                       // 1C
		BSFixedString controlID;                   // 20
		AttackType    attackType;                  // 28
		std::uint8_t  pad29;                       // 29
		std::uint16_t pad2A;                       // 2A
		std::uint8_t  attackCount;                 // 2C
		Setting*      initialPowerAttackDelay;     // 30 - the INI setting, not its value
		Setting*      subsequentPowerAttackDelay;  // 38 - the INI setting, not its value
		bool          ignore;                      // 40
		bool          unk41;                       // 41
		bool          heldLeft;                    // 42
		bool          heldRight;                   // 43
		std::uint32_t unk44;                       // 44
#endif
	};
#if !defined(ENABLE_SKYRIM_AE) && !defined(ENABLE_SKYRIM_VR)
	static_assert(sizeof(AttackBlockHandler) == 0x48);
#endif
}
