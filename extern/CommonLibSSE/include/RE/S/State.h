#pragma once

#include "RE/N/NiSmartPointer.h"
#include "RE/N/NiTexture.h"

namespace RE
{
	namespace BSGraphics
	{
		class State
		{
		public:
			[[nodiscard]] static State* GetSingleton()
			{
				REL::Relocation<State*> singleton{ RELOCATION_ID(524998, 411479) };
				return singleton.get();
			}

			// The frame flags and the default textures lie at different offsets per game version (IDA, SE 1.5.97,
			// AE 1.6.1170, AE 1.7.104, VR 1.4.15): insideFrame SE/VR 0x50, AE 1.6 0x54, AE 1.7 0x60; the default
			// textures start at SE 0x58, AE 1.6 and VR 0x60, AE 1.7 0x70. NG used the SE offsets on every version.
			struct FRAME_DATA
			{
				bool          insideFrame;  // 00 - set when a frame starts, cleared in Present
				bool          letterbox;    // 01
				std::uint16_t unk02;        // 02
			};
			static_assert(sizeof(FRAME_DATA) == 0x4);

			struct TEXTURE_DATA
			{
				NiPointer<NiTexture> unk00;                     // 00 - same texture as defaultHeightMap
				NiPointer<NiTexture> defaultTextureWhite;       // 08
				NiPointer<NiTexture> defaultTextureGrey;        // 10
				NiPointer<NiTexture> defaultHeightMap;          // 18
				NiPointer<NiTexture> defaultReflectionCubeMap;  // 20
				NiPointer<NiTexture> defaultFaceDetailMap;      // 28
				NiPointer<NiTexture> defaultTexEffectMap;       // 30
				NiPointer<NiTexture> defaultTextureNormalMap;   // 38
				NiPointer<NiTexture> defaultDitheringNoise;     // 40
			};
			static_assert(sizeof(TEXTURE_DATA) == 0x48);

			[[nodiscard]] FRAME_DATA& GetFrameData() noexcept
			{
				return REL::RuntimeMember<FRAME_DATA>(this, 0x50, 0x54, 0x60, 0x50);
			}

			[[nodiscard]] const FRAME_DATA& GetFrameData() const noexcept
			{
				return REL::RuntimeMember<FRAME_DATA>(this, 0x50, 0x54, 0x60, 0x50);
			}

			[[nodiscard]] TEXTURE_DATA& GetTextureData() noexcept
			{
				return REL::RuntimeMember<TEXTURE_DATA>(this, 0x58, 0x60, 0x70, 0x60);
			}

			[[nodiscard]] const TEXTURE_DATA& GetTextureData() const noexcept
			{
				return REL::RuntimeMember<TEXTURE_DATA>(this, 0x58, 0x60, 0x70, 0x60);
			}

			// members
			std::uint64_t unk00;                   // 000
			std::uint64_t unk08;                   // 008
			std::uint64_t unk10;                   // 010
			std::uint64_t unk18;                   // 018
			std::uint32_t unk20;                   // 020
			std::uint32_t screenWidth;             // 024
			std::uint32_t screenHeight;            // 028
			std::uint32_t frameBufferViewport[2];  // 02C
			std::uint32_t unk34;                   // 034
			std::uint64_t unk38;                   // 038
			std::uint64_t unk40;                   // 040
#if !defined(ENABLE_SKYRIM_AE) && !defined(ENABLE_SKYRIM_VR)
			std::uint64_t        unk48;                     // 048
			bool                 insideFrame;               // 050
			bool                 letterbox;                 // 051
			std::uint16_t        unk52;                     // 052
			std::uint32_t        unk54;                     // 054
			NiPointer<NiTexture> unk058;                    // 058
			NiPointer<NiTexture> defaultTextureWhite;       // 060
			NiPointer<NiTexture> defaultTextureGrey;        // 068
			NiPointer<NiTexture> defaultHeightMap;          // 070
			NiPointer<NiTexture> defaultReflectionCubeMap;  // 078
			NiPointer<NiTexture> defaultFaceDetailMap;      // 080
			NiPointer<NiTexture> defaultTexEffectMap;       // 088
			NiPointer<NiTexture> defaultTextureNormalMap;   // 090
#endif
		};
		static_assert(offsetof(State, screenWidth) == 0x24);
		static_assert(offsetof(State, frameBufferViewport) == 0x2C);
#if !defined(ENABLE_SKYRIM_AE) && !defined(ENABLE_SKYRIM_VR)
		static_assert(offsetof(State, letterbox) == 0x51);
#endif
	}
}
