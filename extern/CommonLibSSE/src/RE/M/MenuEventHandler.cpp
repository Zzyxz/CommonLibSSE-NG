#include "RE/M/MenuEventHandler.h"

namespace RE
{
	bool MenuEventHandler::ProcessKinect(KinectEvent*)
	{
		return false;
	}

	bool MenuEventHandler::ProcessThumbstick(ThumbstickEvent*)
	{
		return false;
	}

	bool MenuEventHandler::ProcessMouseMove(MouseMoveEvent*)
	{
		return false;
	}

	bool MenuEventHandler::ProcessButton(RE::ButtonEvent*)
	{
		return false;
	}

	namespace
	{
		template <class Event>
		bool CallSlot(MenuEventHandler* a_handler, std::size_t a_index, Event* a_event)
		{
			using func_t = bool (*)(MenuEventHandler*, Event*);
			const auto* vtable = *reinterpret_cast<const std::uintptr_t* const*>(a_handler);
			return reinterpret_cast<func_t>(vtable[MenuEventHandler::GameSlot(a_index)])(a_handler, a_event);
		}
	}

	bool MenuEventHandler::CallProcessKinect(KinectEvent* a_event)
	{
		return CallSlot(this, 2, a_event);
	}

	bool MenuEventHandler::CallProcessThumbstick(ThumbstickEvent* a_event)
	{
		return CallSlot(this, 3, a_event);
	}

	bool MenuEventHandler::CallProcessMouseMove(MouseMoveEvent* a_event)
	{
		return CallSlot(this, 4, a_event);
	}

	bool MenuEventHandler::CallProcessButton(ButtonEvent* a_event)
	{
		return CallSlot(this, 5, a_event);
	}

	void MenuEventHandler::UseGameVTableLayout(MenuEventHandler* a_handler, std::size_t a_slotCount)
	{
		const auto inserted = GameSlot(2) - 2;
		if (!a_handler || inserted == 0) {
			return;
		}
		REL::Relocation<const std::uintptr_t*> gameBase{ VTABLE_MenuEventHandler[0] };
		auto& vtable = *reinterpret_cast<const std::uintptr_t**>(a_handler);
		vtable = REL::InsertVTableSlots(vtable, a_slotCount, 2, inserted, gameBase.get());
	}
}
