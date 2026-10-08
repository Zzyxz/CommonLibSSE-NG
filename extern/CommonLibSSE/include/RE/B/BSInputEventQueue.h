#pragma once

#include "RE/B/BSTSingleton.h"
#include "RE/B/ButtonEvent.h"
#include "RE/C/CharEvent.h"
#include "RE/D/DeviceConnectEvent.h"
#include "RE/K/KinectEvent.h"
#include "RE/M/MouseMoveEvent.h"
#include "RE/T/ThumbstickEvent.h"

namespace RE
{
	class BSInputEventQueue : public BSTSingletonSDM<BSInputEventQueue>
	{
	public:
		inline static constexpr std::uint8_t MAX_BUTTON_EVENTS = 10;
		inline static constexpr std::uint8_t MAX_CHAR_EVENTS = 5;
		inline static constexpr std::uint8_t MAX_MOUSE_EVENTS = 1;
		inline static constexpr std::uint8_t MAX_THUMBSTICK_EVENTS = 2;
		inline static constexpr std::uint8_t MAX_CONNECT_EVENTS = 1;
		inline static constexpr std::uint8_t MAX_KINECT_EVENTS = 1;

		static BSInputEventQueue* GetSingleton();

		void AddButtonEvent(INPUT_DEVICE a_device, std::int32_t a_id, float a_value, float a_duration);
		void AddCharEvent(std::uint32_t a_keyCode);
		void AddMouseMoveEvent(std::int32_t a_mouseInputX, std::int32_t a_mouseInputY);
		void AddThumbstickEvent(ThumbstickEvent::InputType a_id, float a_xValue, float a_yValue);
		void AddConnectEvent(INPUT_DEVICE a_device, bool a_connected);
		void AddKinectEvent(const BSFixedString& a_userEvent, const BSFixedString& a_heard);
		void PushOntoInputQueue(InputEvent* a_event);
		void ClearInputQueue();

		// The cached events and the queue lie at different offsets per game version (IDA, SE 1.5.97, AE 1.6.1170,
		// AE 1.7.104, VR 1.4.15):
		// - AE 1.7 adds three event counts (0x1C..0x27) and three event arrays after kinectEvents: the events start
		//   at 0x28, the queue at 0x558.
		// - VR's ButtonEvent is 0x38 bytes and VR adds touchpad events: buttons at 0x28, chars 0x258, mouse 0x2F8,
		//   thumbsticks 0x328, connect 0x388, kinect 0x3A8, queue 0x570.
		// The Get...Event() functions return the cached event for the running game.
		enum class EventKind : std::uint32_t
		{
			kButton,
			kChar,
			kMouseMove,
			kThumbstick,
			kConnect,
			kKinect
		};

		struct QUEUE_DATA
		{
			InputEvent* queueHead;  // 00
			InputEvent* queueTail;  // 08
		};
		static_assert(sizeof(QUEUE_DATA) == 0x10);

		[[nodiscard]] inline QUEUE_DATA& GetQueueData() noexcept
		{
			return REL::RuntimeMember<QUEUE_DATA>(this, 0x380, 0x380, 0x558, 0x570);
		}

		[[nodiscard]] inline const QUEUE_DATA& GetQueueData() const noexcept
		{
			return REL::RuntimeMember<QUEUE_DATA>(this, 0x380, 0x380, 0x558, 0x570);
		}

		// Address of cached event a_index of a_kind in the running game.
		[[nodiscard]] void* GetCachedEvent(EventKind a_kind, std::uint32_t a_index) noexcept;

		[[nodiscard]] ButtonEvent*        GetButtonEvent(std::uint32_t a_index) noexcept { return static_cast<ButtonEvent*>(GetCachedEvent(EventKind::kButton, a_index)); }
		[[nodiscard]] CharEvent*          GetCharEvent(std::uint32_t a_index) noexcept { return static_cast<CharEvent*>(GetCachedEvent(EventKind::kChar, a_index)); }
		[[nodiscard]] MouseMoveEvent*     GetMouseMoveEvent(std::uint32_t a_index) noexcept { return static_cast<MouseMoveEvent*>(GetCachedEvent(EventKind::kMouseMove, a_index)); }
		[[nodiscard]] ThumbstickEvent*    GetThumbstickEvent(std::uint32_t a_index) noexcept { return static_cast<ThumbstickEvent*>(GetCachedEvent(EventKind::kThumbstick, a_index)); }
		[[nodiscard]] DeviceConnectEvent* GetConnectEvent(std::uint32_t a_index) noexcept { return static_cast<DeviceConnectEvent*>(GetCachedEvent(EventKind::kConnect, a_index)); }
		[[nodiscard]] KinectEvent*        GetKinectEvent(std::uint32_t a_index) noexcept { return static_cast<KinectEvent*>(GetCachedEvent(EventKind::kKinect, a_index)); }

		// members
		std::uint8_t  pad001;                // 001
		std::uint16_t pad002;                // 002
		std::uint32_t buttonEventCount;      // 004
		std::uint32_t charEventCount;        // 008
		std::uint32_t mouseEventCount;       // 00C
		std::uint32_t thumbstickEventCount;  // 010
		std::uint32_t connectEventCount;     // 014
		std::uint32_t kinectEventCount;      // 018
		std::uint32_t pad01C;                // 01C - AE 1.7: first of three new event counts
#if !defined(ENABLE_SKYRIM_AE) && !defined(ENABLE_SKYRIM_VR)
		ButtonEvent        buttonEvents[MAX_BUTTON_EVENTS];          // 020
		CharEvent          charEvents[MAX_CHAR_EVENTS];              // 200
		MouseMoveEvent     mouseEvents[MAX_MOUSE_EVENTS];            // 2A0
		ThumbstickEvent    thumbstickEvents[MAX_THUMBSTICK_EVENTS];  // 2D0
		DeviceConnectEvent connectEvents[MAX_CONNECT_EVENTS];        // 330
		KinectEvent        kinectEvents[MAX_KINECT_EVENTS];          // 350
		InputEvent*        queueHead;                                // 380
		InputEvent*        queueTail;                                // 388
#endif
	};
#if !defined(ENABLE_SKYRIM_AE) && !defined(ENABLE_SKYRIM_VR)
	static_assert(sizeof(BSInputEventQueue) == 0x390);
#endif
}
