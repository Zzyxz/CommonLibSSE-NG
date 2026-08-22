#pragma once

#include "RE/B/BSExtraData.h"
#include "RE/E/ExtraDataTypes.h"
#include "RE/F/FormTypes.h"  // für BGSMusicType

namespace RE
{
	class ExtraCellMusicType : public BSExtraData
	{
	public:
		inline static constexpr auto RTTI = RTTI_ExtraCellMusicType;
		inline static constexpr auto VTABLE = VTABLE_ExtraCellMusicType;
		inline static constexpr auto EXTRADATATYPE = ExtraDataType::kCellMusicType;

		~ExtraCellMusicType() override = default;

		ExtraDataType GetType() const override
		{
			return EXTRADATATYPE;
		}

		bool IsNotEqual(const BSExtraData* a_rhs) const override
		{
			auto* other = static_cast<const ExtraCellMusicType*>(a_rhs);
			return type != other->type;
		}

		// members
		BGSMusicType* type;  // 10
	};
	static_assert(sizeof(ExtraCellMusicType) == 0x18);
}
