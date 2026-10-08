#include "RE/P/PlayerInputHandler.h"

namespace RE
{
	bool PlayerInputHandler::IsInputEventHandlingEnabled() const
	{
		return inputEventHandlingEnabled;
	}

	void PlayerInputHandler::SetInputEventHandlingEnabled(bool a_enabled)
	{
		inputEventHandlingEnabled = a_enabled;
	}

	namespace
	{
		template <class Event>
		void CallSlot(PlayerInputHandler* a_handler, std::size_t a_index, Event* a_event, PlayerControlsData* a_data)
		{
			using func_t = void (*)(PlayerInputHandler*, Event*, PlayerControlsData*);
			const auto* vtable = *reinterpret_cast<const std::uintptr_t* const*>(a_handler);
			reinterpret_cast<func_t>(vtable[PlayerInputHandler::GameSlot(a_index)])(a_handler, a_event, a_data);
		}
	}

	void PlayerInputHandler::CallProcessThumbstick(ThumbstickEvent* a_event, PlayerControlsData* a_data)
	{
		CallSlot(this, 2, a_event, a_data);
	}

	void PlayerInputHandler::CallProcessMouseMove(MouseMoveEvent* a_event, PlayerControlsData* a_data)
	{
		CallSlot(this, 3, a_event, a_data);
	}

	void PlayerInputHandler::CallProcessButton(ButtonEvent* a_event, PlayerControlsData* a_data)
	{
		CallSlot(this, 4, a_event, a_data);
	}

	void PlayerInputHandler::UseGameVTableLayout(PlayerInputHandler* a_handler, std::size_t a_slotCount)
	{
		if (!a_handler || GameSlot(2) == 2) {
			return;
		}
		REL::Relocation<const std::uintptr_t*> gameBase{ VTABLE_PlayerInputHandler[0] };
		auto& vtable = *reinterpret_cast<const std::uintptr_t**>(a_handler);
		vtable = REL::InsertVTableSlots(vtable, a_slotCount, 2, 2, gameBase.get());
	}
}
