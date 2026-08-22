#pragma once

#include "RE/B/BSExtraData.h"
#include "RE/E/ExtraDataTypes.h"
#include "RE/F/FormTypes.h"  // für TESRegion

namespace RE
{
	class ExtraCellSkyRegion : public BSExtraData
	{
	public:
		inline static constexpr auto RTTI = RTTI_ExtraCellSkyRegion;
		inline static constexpr auto VTABLE = VTABLE_ExtraCellSkyRegion;
		inline static constexpr auto EXTRADATATYPE = ExtraDataType::kCellSkyRegion;

		~ExtraCellSkyRegion() override = default;

		ExtraDataType GetType() const override
		{
			return EXTRADATATYPE;
		}

		bool IsNotEqual(const BSExtraData* a_rhs) const override
		{
			auto* other = static_cast<const ExtraCellSkyRegion*>(a_rhs);
			return skyRegion != other->skyRegion;
		}

		// members
		TESRegion* skyRegion;  // 10
	};
	static_assert(sizeof(ExtraCellSkyRegion) == 0x18);
}
