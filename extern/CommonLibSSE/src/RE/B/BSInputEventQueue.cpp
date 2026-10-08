#include "RE/B/BSInputEventQueue.h"

namespace RE
{
	BSInputEventQueue* BSInputEventQueue::GetSingleton()
	{
		REL::Relocation<BSInputEventQueue**> singleton{ RELOCATION_ID(520856, 407374) };
		return *singleton;
	}

	void* BSInputEventQueue::GetCachedEvent(EventKind a_kind, std::uint32_t a_index) noexcept
	{
		struct Layout
		{
			std::uint32_t offset[6];
			std::uint32_t stride[6];
		};
		// button, char, mouse move, thumbstick, connect, kinect
		static constexpr Layout kSE{ { 0x20, 0x200, 0x2A0, 0x2D0, 0x330, 0x350 }, { 0x30, 0x20, 0x30, 0x30, 0x20, 0x30 } };
		static constexpr Layout kAE17{ { 0x28, 0x208, 0x2A8, 0x2D8, 0x338, 0x358 }, { 0x30, 0x20, 0x30, 0x30, 0x20, 0x30 } };
		static constexpr Layout kVR{ { 0x28, 0x258, 0x2F8, 0x328, 0x388, 0x3A8 }, { 0x38, 0x20, 0x30, 0x30, 0x20, 0x30 } };

		const auto& layout = REL::Module::IsVR() ? kVR : (REL::IsAE17() ? kAE17 : kSE);
		const auto  kind = static_cast<std::uint32_t>(a_kind);
		return reinterpret_cast<std::byte*>(this) + layout.offset[kind] + layout.stride[kind] * a_index;
	}

	void BSInputEventQueue::AddButtonEvent(INPUT_DEVICE a_device, std::int32_t a_id, float a_value, float a_duration)
	{
		if (buttonEventCount < MAX_BUTTON_EVENTS) {
			auto& cachedEvent = *GetButtonEvent(buttonEventCount);
			cachedEvent.value = a_value;
			cachedEvent.heldDownSecs = a_duration;
			cachedEvent.device = a_device;
			cachedEvent.idCode = a_id;
			cachedEvent.userEvent = {};

			PushOntoInputQueue(&cachedEvent);
			++buttonEventCount;
		}
	}

	void BSInputEventQueue::AddCharEvent(std::uint32_t a_keyCode)
	{
		if (charEventCount < MAX_CHAR_EVENTS) {
			auto& cachedEvent = *GetCharEvent(charEventCount);
			cachedEvent.keycode = a_keyCode;

			PushOntoInputQueue(&cachedEvent);
			++charEventCount;
		}
	}

	void BSInputEventQueue::AddMouseMoveEvent(std::int32_t a_mouseInputX, std::int32_t a_mouseInputY)
	{
		if (mouseEventCount < MAX_MOUSE_EVENTS) {
			auto& cachedEvent = *GetMouseMoveEvent(mouseEventCount);
			cachedEvent.mouseInputX = a_mouseInputX;
			cachedEvent.mouseInputY = a_mouseInputY;
			cachedEvent.userEvent = {};

			PushOntoInputQueue(&cachedEvent);
			++mouseEventCount;
		}
	}

	void BSInputEventQueue::AddThumbstickEvent(ThumbstickEvent::InputType a_id, float a_xValue, float a_yValue)
	{
		if (thumbstickEventCount < MAX_THUMBSTICK_EVENTS) {
			auto& cachedEvent = *GetThumbstickEvent(thumbstickEventCount);
			cachedEvent.idCode = a_id;
			cachedEvent.xValue = a_xValue;
			cachedEvent.yValue = a_yValue;
			cachedEvent.userEvent = {};

			PushOntoInputQueue(&cachedEvent);
			++thumbstickEventCount;
		}
	}

	void BSInputEventQueue::AddConnectEvent(INPUT_DEVICE a_device, bool a_connected)
	{
		if (connectEventCount < MAX_CONNECT_EVENTS) {
			auto& cachedEvent = *GetConnectEvent(connectEventCount);
			cachedEvent.device = a_device;
			cachedEvent.connected = a_connected;

			PushOntoInputQueue(&cachedEvent);
			++connectEventCount;
		}
	}

	void BSInputEventQueue::AddKinectEvent(const BSFixedString& a_userEvent, const BSFixedString& a_heard)
	{
		if (kinectEventCount < MAX_KINECT_EVENTS) {
			auto& cachedEvent = *GetKinectEvent(kinectEventCount);
			cachedEvent.userEvent = a_userEvent;
			cachedEvent.heard = a_heard;

			PushOntoInputQueue(&cachedEvent);
			++kinectEventCount;
		}
	}

	void BSInputEventQueue::PushOntoInputQueue(InputEvent* a_event)
	{
		auto& queue = GetQueueData();
		if (!queue.queueHead) {
			queue.queueHead = a_event;
		}

		if (queue.queueTail) {
			queue.queueTail->next = a_event;
		}

		queue.queueTail = a_event;
		queue.queueTail->next = nullptr;
	}

	void BSInputEventQueue::ClearInputQueue()
	{
		kinectEventCount = 0;
		connectEventCount = 0;
		thumbstickEventCount = 0;
		mouseEventCount = 0;
		charEventCount = 0;
		buttonEventCount = 0;
		if (REL::IsAE17()) {
			// the three event counts AE 1.7 added at 0x1C, 0x20 and 0x24
			std::memset(&pad01C, 0, 3 * sizeof(std::uint32_t));
		}
		auto& queue = GetQueueData();
		queue.queueTail = nullptr;
		queue.queueHead = nullptr;
	}
}
