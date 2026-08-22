#pragma once

#include "RE/B/BSExtraData.h"
#include "RE/E/ExtraDataTypes.h"
#include "RE/F/FormTypes.h"

namespace RE
{
	class ExtraCellAcousticSpace : public BSExtraData
	{
	public:
		inline static constexpr auto RTTI = RTTI_ExtraCellAcousticSpace;
		inline static constexpr auto VTABLE = VTABLE_ExtraCellAcousticSpace;
		inline static constexpr auto EXTRADATATYPE = ExtraDataType::kCellAcousticSpace;

		~ExtraCellAcousticSpace() override = default;

		ExtraDataType GetType() const override
		{
			return EXTRADATATYPE;
		}

		bool IsNotEqual(const BSExtraData* a_rhs) const override
		{
			auto* other = static_cast<const ExtraCellAcousticSpace*>(a_rhs);
			return space != other->space;
		}

		// members
		BGSAcousticSpace* space;  // 10
	};
	static_assert(sizeof(ExtraCellAcousticSpace) == 0x18);
}
