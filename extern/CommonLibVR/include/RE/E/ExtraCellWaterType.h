#pragma once

#include "RE/B/BSExtraData.h"
#include "RE/E/ExtraDataTypes.h"
#include "RE/F/FormTypes.h"  // für TESWaterForm

namespace RE
{
	class ExtraCellWaterType : public BSExtraData
	{
	public:
		inline static constexpr auto RTTI = RTTI_ExtraCellWaterType;
		inline static constexpr auto VTABLE = VTABLE_ExtraCellWaterType;
		inline static constexpr auto EXTRADATATYPE = ExtraDataType::kCellWaterType;

		~ExtraCellWaterType() override = default;

		ExtraDataType GetType() const override
		{
			return EXTRADATATYPE;
		}

		bool IsNotEqual(const BSExtraData* a_rhs) const override
		{
			auto* other = static_cast<const ExtraCellWaterType*>(a_rhs);
			return water != other->water;
		}

		// members
		TESWaterForm* water;  // 10
	};
	static_assert(sizeof(ExtraCellWaterType) == 0x18);
}
