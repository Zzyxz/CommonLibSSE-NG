#pragma once

#include "REX/W32/BASE.h"

namespace RE
{
	class BSThread
	{
	public:
		inline static constexpr auto RTTI = RTTI_BSThread;

		virtual ~BSThread();  // 00

		// add
		virtual void Unk_01(void);  // 01 - { return 0; }
		virtual void Unk_02(void);  // 02 - { return; }

		// VR adds the thread name at 0x48 and moves `initialized` to 0x50 (size 0x58 instead of 0x50); verified in
		// IDA (BSThread ctor SE 0xC07D60 / VR 0xC42BE0, Create SE 0xC07DE0 / VR 0xC42C70).
		[[nodiscard]] bool IsInitialized() const noexcept
		{
			return REL::RuntimeMember<bool>(this, 0x48, 0x48, 0x50);
		}

		// VR only (nullptr on SE and AE).
		[[nodiscard]] const char* GetName() const noexcept
		{
			return REL::Module::IsVR() ? REL::RelocateMember<const char*>(this, 0x48) : nullptr;
		}

		// members
		REX::W32::CRITICAL_SECTION lock;           // 08
		void*                      thread;         // 30
		void*                      ownerThread;    // 38
		std::uint32_t              threadID;       // 40
		std::uint32_t              ownerThreadID;  // 44
#ifndef ENABLE_SKYRIM_VR
		bool          initialized;  // 48
		std::uint8_t  pad49;        // 49
		std::uint16_t pad4A;        // 4A
		std::uint32_t pad4C;        // 4C
#elif !defined(ENABLE_SKYRIM_SE) && !defined(ENABLE_SKYRIM_AE)
		const char*   name;         // 48
		bool          initialized;  // 50
		std::uint8_t  pad51;        // 51
		std::uint16_t pad52;        // 52
		std::uint32_t pad54;        // 54
#else
		std::uint64_t unk48;  // 48 - SE/AE: initialized, VR: name; use IsInitialized() / GetName()
#endif
	};
#if defined(ENABLE_SKYRIM_VR) && !defined(ENABLE_SKYRIM_SE) && !defined(ENABLE_SKYRIM_AE)
	static_assert(sizeof(BSThread) == 0x58);
#else
	static_assert(sizeof(BSThread) == 0x50);  // compiled size; 0x58 at runtime on VR
#endif
}
