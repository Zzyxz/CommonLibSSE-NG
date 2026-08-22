#pragma once

#include "RE/B/BSExtraData.h"
#include "RE/E/ExtraDataTypes.h"
#include "RE/F/FormTypes.h"  // für TESImageSpace

namespace RE
{
	class ExtraCellImageSpace : public BSExtraData
	{
	public:
		inline static constexpr auto RTTI = RTTI_ExtraCellImageSpace;
		inline static constexpr auto VTABLE = VTABLE_ExtraCellImageSpace;
		inline static constexpr auto EXTRADATATYPE = ExtraDataType::kCellImageSpace;

		~ExtraCellImageSpace() override = default;

		ExtraDataType GetType() const override
		{
			return EXTRADATATYPE;
		}

		bool IsNotEqual(const BSExtraData* a_rhs) const override
		{
			auto* other = static_cast<const ExtraCellImageSpace*>(a_rhs);
			return imageSpace != other->imageSpace;
		}

		// members
		TESImageSpace* imageSpace;  // 10
	};
	static_assert(sizeof(ExtraCellImageSpace) == 0x18);
}
