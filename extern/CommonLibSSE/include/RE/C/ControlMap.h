#pragma once

#include "RE/B/BSFixedString.h"
#include "RE/B/BSInputDevice.h"
#include "RE/B/BSTArray.h"
#include "RE/B/BSTEvent.h"
#include "RE/B/BSTSingleton.h"
#include "RE/I/InputDevices.h"
#include "RE/P/PCGamepadType.h"
#include "RE/U/UserEvents.h"

namespace RE
{
	class UserEventEnabled;

	class ControlMap :
		public BSTSingletonSDM<ControlMap>,      // 00
		public BSTEventSource<UserEventEnabled>  // 08
	{
	public:
		using InputContextID = UserEvents::INPUT_CONTEXT_ID;
		using UEFlag = UserEvents::USER_EVENT_FLAG;

		enum : std::uint32_t
		{
			kInvalid = static_cast<std::uint8_t>(-1)
		};

		struct UserEventMapping
		{
		public:
			// members
			BSFixedString                           eventID;             // 00
			std::uint16_t                           inputKey;            // 08
			std::uint16_t                           modifier;            // 08
			std::int8_t                             indexInContext;      // 0C
			bool                                    remappable;          // 0D
			bool                                    linked;              // 0E
			stl::enumeration<UEFlag, std::uint32_t> userEventGroupFlag;  // 10
			std::uint32_t                           pad14;               // 14
		};
		static_assert(sizeof(UserEventMapping) == 0x18);

		struct InputContext
		{
		public:
			[[nodiscard]] static SKYRIM_REL_VR std::size_t GetNumDeviceMappings() noexcept
			{
#ifndef SKYRIM_CROSS_VR
				return INPUT_DEVICES::kTotal;
#else
				if SKYRIM_REL_VR_CONSTEXPR (REL::Module::IsVR()) {
					return INPUT_DEVICES::kTotal;
				} else {
					return static_cast<std::size_t>(INPUT_DEVICES::kVirtualKeyboard) + 1;
				}
#endif
			}

			// members
			BSTArray<UserEventMapping> deviceMappings[INPUT_DEVICES::kTotal];  // 00
		};
#ifdef ENABLE_SKYRIM_VR
		static_assert(sizeof(InputContext) == 0xA8);
#else
		static_assert(sizeof(InputContext) == 0x60);
#endif

		struct LinkedMapping
		{
		public:
			// members
			BSFixedString  linkedMappingName;     // 00
			InputContextID linkedMappingContext;  // 08
			INPUT_DEVICE   device;                // 0C
			InputContextID linkFromContext;       // 10
			std::uint32_t  pad14;                 // 14
			BSFixedString  linkFromName;          // 18
		};
		static_assert(sizeof(LinkedMapping) == 0x20);

		static ControlMap* GetSingleton();

		// AE (1.6+) has one input context more than SE: Marketplace is inserted at game index 16, so kFavor is
		// 16 on SE and 17 on AE, and everything after controlMap[] lies 8 bytes later (verified in IDA for
		// 1.5.97, 1.6.1170 and 1.7.104). InputContextID keeps the SE numbering; the methods below translate.
		struct RUNTIME_DATA
		{
#define RUNTIME_DATA_CONTENT                                                                                       \
	BSTArray<LinkedMapping>                          linkedMappings;               /* 0E8, 0F0 */                  \
	BSTArray<InputContextID>                         contextPriorityStack;         /* 100, 108 - game indices */   \
	stl::enumeration<UEFlag, std::uint32_t>          enabledControls;              /* 118, 120 */                  \
	stl::enumeration<UEFlag, std::uint32_t>          unk11C;                       /* 11C, 124 - saved controls */ \
	std::int8_t                                      textEntryCount;               /* 120, 128 */                  \
	bool                                             ignoreKeyboardMouse;          /* 121, 129 */                  \
	bool                                             ignoreActivateDisabledEvents; /* 122, 12A */                  \
	std::uint8_t                                     pad123;                       /* 123, 12B */                  \
	stl::enumeration<PC_GAMEPAD_TYPE, std::uint32_t> gamePadMapType;               /* 124, 12C */

			RUNTIME_DATA_CONTENT
		};
		static_assert(sizeof(RUNTIME_DATA) == 0x40);

		[[nodiscard]] RUNTIME_DATA& GetRuntimeData() noexcept
		{
			return REL::RuntimeMember<RUNTIME_DATA>(this, 0xE8, 0xF0);
		}

		[[nodiscard]] const RUNTIME_DATA& GetRuntimeData() const noexcept
		{
			return REL::RuntimeMember<RUNTIME_DATA>(this, 0xE8, 0xF0);
		}

		// The game's index for a context: kFavor and later move up by one on AE.
		[[nodiscard]] static std::uint32_t ToGameContext(InputContextID a_context) noexcept
		{
			const auto index = static_cast<std::uint32_t>(a_context);
			return REL::Module::IsAE() && index >= static_cast<std::uint32_t>(InputContextID::kFavor) ? index + 1 : index;
		}

		// The input context for a context id (nullptr if none), read from the game's array.
		[[nodiscard]] InputContext* GetInputContext(InputContextID a_context) const noexcept
		{
			if (a_context >= InputContextID::kTotal) {
				return nullptr;
			}
			const auto* contexts = reinterpret_cast<InputContext* const*>(reinterpret_cast<std::uintptr_t>(this) + 0x60);
			return contexts[ToGameContext(a_context)];
		}

		std::int8_t      AllowTextInput(bool a_allow);
		bool             AreControlsEnabled(UEFlag a_flags) const noexcept { return GetRuntimeData().enabledControls.all(a_flags); }
		std::uint32_t    GetMappedKey(std::string_view a_eventID, INPUT_DEVICE a_device, InputContextID a_context = InputContextID::kGameplay) const;
		std::string_view GetUserEventName(std::uint32_t a_buttonID, INPUT_DEVICE a_device, InputContextID a_context = InputContextID::kGameplay) const;
		PC_GAMEPAD_TYPE  GetGamePadType() const noexcept { return GetRuntimeData().gamePadMapType.get(); }
		bool             IsActivateControlsEnabled() const noexcept { return AreControlsEnabled(UEFlag::kActivate); }
		bool             IsConsoleControlsEnabled() const noexcept { return AreControlsEnabled(UEFlag::kConsole); }
		bool             IsFightingControlsEnabled() const noexcept { return AreControlsEnabled(UEFlag::kFighting); }
		bool             IsLookingControlsEnabled() const noexcept { return AreControlsEnabled(UEFlag::kLooking); }
		bool             IsMenuControlsEnabled() const noexcept { return AreControlsEnabled(UEFlag::kMenu); }
		bool             IsMainFourControlsEnabled() const noexcept { return AreControlsEnabled(UEFlag::kMainFour); }
		bool             IsMovementControlsEnabled() const noexcept { return AreControlsEnabled(UEFlag::kMovement); }
		bool             IsPOVSwitchControlsEnabled() const noexcept { return AreControlsEnabled(UEFlag::kPOVSwitch); }
		bool             IsSneakingControlsEnabled() const noexcept { return AreControlsEnabled(UEFlag::kSneaking); }
		bool             IsVATSControlsEnabled() const noexcept { return AreControlsEnabled(UEFlag::kVATS); }
		bool             IsWheelZoomControlsEnabled() const noexcept { return AreControlsEnabled(UEFlag::kWheelZoom); }
		void             PopInputContext(InputContextID a_context);
		void             PushInputContext(InputContextID a_context);
		void             ToggleControls(UEFlag a_flags, bool a_enable);

		// members
#ifndef ENABLE_SKYRIM_AE
		InputContext* controlMap[InputContextID::kTotal];  // 060
		RUNTIME_DATA_CONTENT
#endif
	};
#ifndef ENABLE_SKYRIM_AE
	static_assert(sizeof(ControlMap) == 0x128);
#endif
}
#undef RUNTIME_DATA_CONTENT
