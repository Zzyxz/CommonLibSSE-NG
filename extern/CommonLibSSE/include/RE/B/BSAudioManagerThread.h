#pragma once

#include "RE/B/BSThread.h"

namespace RE
{
	class BSAudioManagerThread : public BSThread
	{
	public:
		inline static constexpr auto RTTI = RTTI_BSAudioManagerThread;

		~BSAudioManagerThread() override;  // 00

		// override (BSThread)
		void Unk_01(void) override;  // 01

		// members
#ifndef SKYRIM_CROSS_VR
		std::uint64_t unk50;  // 50 (VR: 58)
		std::uint64_t unk58;  // 58 (VR: 60)
		std::uint64_t unk60;  // 60 (VR: 68)
#endif
	};
#if defined(ENABLE_SKYRIM_VR) && !defined(ENABLE_SKYRIM_SE) && !defined(ENABLE_SKYRIM_AE)
	static_assert(sizeof(BSAudioManagerThread) == 0x70);
#elif !defined(SKYRIM_CROSS_VR)
	static_assert(sizeof(BSAudioManagerThread) == 0x68);
#endif
}
