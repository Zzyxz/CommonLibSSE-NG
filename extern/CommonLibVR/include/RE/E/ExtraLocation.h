#pragma once

#include "RE/B/BSExtraData.h"
#include "RE/E/ExtraDataTypes.h"

namespace RE
{
	class BGSLocation;

	class ExtraLocation : public BSExtraData
	{
	public:
		inline static constexpr auto RTTI = RTTI_ExtraLocation;
		inline static constexpr auto VTABLE = VTABLE_ExtraLocation;
		inline static constexpr auto EXTRADATATYPE = ExtraDataType::kLocation;

		~ExtraLocation() override = default;  // 00

		// override (BSExtraData)
		ExtraDataType GetType() const override
		{
			return EXTRADATATYPE;
		}

		bool IsNotEqual(const BSExtraData* a_rhs) const override
		{
			auto* other = static_cast<const ExtraLocation*>(a_rhs);
			return location != other->location;
		}

		// members
		BGSLocation* location;  // 10
	private:
		KEEP_FOR_RE()
	};
	static_assert(sizeof(ExtraLocation) == 0x18);
}
