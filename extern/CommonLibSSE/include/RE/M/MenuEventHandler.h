#pragma once

#include "RE/B/BSIntrusiveRefCounted.h"

namespace RE
{
	class ButtonEvent;
	class InputEvent;
	class KinectEvent;
	class MouseMoveEvent;
	class ThumbstickEvent;

	class MenuEventHandler : public BSIntrusiveRefCounted
	{
	public:
		inline static constexpr auto RTTI = RTTI_MenuEventHandler;

		MenuEventHandler() = default;
		virtual ~MenuEventHandler() = default;  // 00

		virtual bool CanProcess(InputEvent* a_event) = 0;          // 01
		virtual bool ProcessKinect(KinectEvent* a_event);          // 02 - { return false; }
		virtual bool ProcessThumbstick(ThumbstickEvent* a_event);  // 03 - { return false; }
		virtual bool ProcessMouseMove(MouseMoveEvent* a_event);    // 04 - { return false; }
		virtual bool ProcessButton(ButtonEvent* a_event);          // 05 - { return false; }

		/**
		 * The game's vtable slot of a function declared above. AE 1.7 has two more input functions at slot 02
		 * (03 handles sixaxis input), VR three (02 touchpad swipe, 03 touchpad position, 04 unknown), so
		 * ProcessKinect, ProcessThumbstick, ProcessMouseMove and ProcessButton are slots 04..07 on AE 1.7 and 05..08
		 * on VR (IDA, 1.7.104 and VR 1.4.15; also for the MenuEventHandler sub-object of menus). C++ calls of these
		 * virtual functions use the SE/AE 1.6 slots: call a game handler through the Call... functions instead, and
		 * hook game vtables at GameSlot(index).
		 */
		[[nodiscard]] static std::size_t GameSlot(std::size_t a_index) noexcept
		{
			if (a_index < 2) {
				return a_index;
			}
			return a_index + (REL::Module::IsVR() ? 3 : (REL::IsAE17() ? 2 : 0));
		}

		bool CallProcessKinect(KinectEvent* a_event);
		bool CallProcessThumbstick(ThumbstickEvent* a_event);
		bool CallProcessMouseMove(MouseMoveEvent* a_event);
		bool CallProcessButton(ButtonEvent* a_event);

		/**
		 * For a handler class written in a plugin: on AE 1.7 and VR its C++ vtable has the SE layout, so the game
		 * would call the wrong functions. This gives the object a vtable in the game's layout; the inserted slots
		 * use the game's MenuEventHandler defaults. a_slotCount is the number of virtual functions of the class (6
		 * for MenuEventHandler, more if the class adds its own). Call it once, after construction and before the
		 * game sees the object. Does nothing on SE and AE 1.6.
		 */
		static void UseGameVTableLayout(MenuEventHandler* a_handler, std::size_t a_slotCount = 6);

		// members
		bool          registered;  // 0C
		std::uint8_t  unk0D;       // 0D
		std::uint16_t pad0E;       // 0E
	};
	static_assert(sizeof(MenuEventHandler) == 0x10);
}
