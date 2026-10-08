#pragma once

namespace RE
{
	class ButtonEvent;
	class InputEvent;
	class MouseMoveEvent;
	class PlayerControlsData;
	class ThumbstickEvent;

	class PlayerInputHandler
	{
	public:
		inline static constexpr auto RTTI = RTTI_PlayerInputHandler;

		virtual ~PlayerInputHandler();  // 00

		virtual bool CanProcess(InputEvent* a_event) = 0;                                      // 01
		virtual void ProcessThumbstick(ThumbstickEvent* a_event, PlayerControlsData* a_data);  // 02 - { return; }
		virtual void ProcessMouseMove(MouseMoveEvent* a_event, PlayerControlsData* a_data);    // 03 - { return; }
		virtual void ProcessButton(ButtonEvent* a_event, PlayerControlsData* a_data);          // 04 - { return; }

		[[nodiscard]] bool IsInputEventHandlingEnabled() const;
		void               SetInputEventHandlingEnabled(bool a_enabled);

		/**
		 * The game's vtable slot of a function declared above. AE 1.7 and VR have two more input functions at
		 * slot 02, so ProcessThumbstick, ProcessMouseMove and ProcessButton are slots 04, 05 and 06 there (and the
		 * slots of derived classes move by two as well). C++ calls of these virtual functions use the SE/AE 1.6
		 * slots: call a game handler through CallProcessThumbstick/MouseMove/Button instead, and hook game
		 * vtables at GameSlot(index).
		 */
		[[nodiscard]] static std::size_t GameSlot(std::size_t a_index) noexcept
		{
			return a_index >= 2 && (REL::Module::IsVR() || REL::IsAE17()) ? a_index + 2 : a_index;
		}

		void CallProcessThumbstick(ThumbstickEvent* a_event, PlayerControlsData* a_data);
		void CallProcessMouseMove(MouseMoveEvent* a_event, PlayerControlsData* a_data);
		void CallProcessButton(ButtonEvent* a_event, PlayerControlsData* a_data);

		/**
		 * For a handler class written in a plugin: on AE 1.7 and VR its C++ vtable has the SE layout, so the game
		 * would call the wrong functions. This gives the object a vtable in the game's layout; the two inserted
		 * slots use the game's PlayerInputHandler defaults. a_slotCount is the number of virtual functions of the
		 * class (5 for PlayerInputHandler, 7 for HeldStateHandler, more if the class adds its own). Call it once,
		 * after construction and before the game sees the object. Does nothing on SE and AE 1.6.
		 */
		static void UseGameVTableLayout(PlayerInputHandler* a_handler, std::size_t a_slotCount = 5);

		// members
		bool          inputEventHandlingEnabled;  // 08
		std::uint8_t  pad09;                      // 09
		std::uint16_t pad0A;                      // 0A
		std::uint32_t pad0C;                      // 0C
	};
	static_assert(sizeof(PlayerInputHandler) == 0x10);
}
