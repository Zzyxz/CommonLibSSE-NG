#include "RE/B/BGSDefaultObjectManager.h"

using namespace REL;

namespace RE
{
	namespace
	{
		constexpr auto kInvalid = (std::numeric_limits<std::size_t>::max)();

		inline std::size_t MapIndex(std::underlying_type_t<DefaultObjectID> a_idx) noexcept
		{
			if (a_idx <= std::to_underlying(DefaultObjectID::kKeywordActivatorFurnitureNoPlayer)) {
				return a_idx;
			}
			std::size_t result;
			if SKYRIM_REL_CONSTEXPR (Module::IsVR()) {
				result = (0xFFFF0000 & a_idx) >> 16;
			} else {
				result = 0x0000FFFF & a_idx;
			}
			return result ? result : kInvalid;
		}
	}

	namespace
	{
		// DefaultObjectID carries the SE index and the VR index; the SE index is translated for AE.
		std::size_t GameIndexFromID(DefaultObjectID a_object) noexcept
		{
			const auto idx = MapIndex(std::to_underlying(a_object));
			if (idx == kInvalid) {
				return kInvalid;
			}
			return Module::IsVR() ? idx : BGSDefaultObjectManager::ToGameIndex(idx);
		}
	}

	TESForm** BGSDefaultObjectManager::GetObject(DefaultObjectID a_object) noexcept
	{
		const auto idx = GameIndexFromID(a_object);
		// NG returned &RelocateMember<TESForm**>(this, 0x20)[idx], which indexed the first object pointer.
		return idx != kInvalid && IsGameIndexInitialized(idx) ? &GetObjectArray()[idx] : nullptr;
	}

	bool BGSDefaultObjectManager::IsObjectInitialized(DefaultObjectID a_object) const noexcept
	{
		const auto idx = GameIndexFromID(a_object);
		return idx != kInvalid && IsGameIndexInitialized(idx);
	}

	bool BGSDefaultObjectManager::SupportsVR(DefaultObjectID a_object) noexcept
	{
		auto idx = std::to_underlying(a_object);
		return idx <= std::to_underlying(DefaultObjectID::kKeywordActivatorFurnitureNoPlayer) || idx & 0xFFFF0000;
	}

	bool BGSDefaultObjectManager::SupportsSE(DefaultObjectID a_object) noexcept
	{
		return (std::to_underlying(a_object) & 0x0000FFFF) || a_object != DefaultObjectID::kWerewolfSpell;
	}

	bool BGSDefaultObjectManager::SupportsCurrentRuntime(DefaultObjectID a_object) noexcept
	{
		return MapIndex(std::to_underlying(a_object)) != kInvalid;
	}
}
