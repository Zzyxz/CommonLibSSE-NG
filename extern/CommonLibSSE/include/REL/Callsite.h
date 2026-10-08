#pragma once

#include "REL/ID.h"
#include "REL/Version.h"

// Ported from the previous template (commit 5624d5c1) onto CommonLibSSE NG.
namespace REL
{
	// Semantic callsites: find a hook site by what it does instead of by a fixed byte offset.
	//
	// The owner function is located through the Address Library, its code range comes from the
	// game's .pdata (including split parts linked through chained unwind info), and only that range
	// is searched. Most AE ids stay valid across AE versions, but some were renumbered in 1.6.1130
	// (use REL::AESplitID for those); RelocationID adds the SE id.
	//
	// The search is a byte scan for E8/E9 rel32 whose destination is the target; it does not decode
	// instructions. Prefer the UNIQUE form, which fails on more than one hit, and check the found
	// call in IDA: a match does not prove that registers and arguments fit the hook.

	// An owner or target: an ID valid on the running game, or an SE/AE RelocationID.
	class CallsiteID
	{
	public:
		constexpr CallsiteID(ID a_id) noexcept :
			_id(a_id.id(), a_id.id())
		{}

		constexpr CallsiteID(RelocationID a_id) noexcept :
			_id(a_id)
		{}

		constexpr CallsiteID(AESplitID a_id) noexcept :
			_split(a_id)
		{}

		// SE/AE ids plus a fixed VR offset, for functions missing from the VR Address Library.
		constexpr CallsiteID(RelocationID a_id, std::uint32_t a_vrOffset) noexcept :
			_id(a_id),
			_vrOffset(a_vrOffset)
		{}

		[[nodiscard]] std::uint64_t id() const noexcept { return _split ? _split->id() : _id.id(); }

		// The function's offset in the running game, or no value if it is unknown there.
		[[nodiscard]] std::optional<std::uint32_t> rva() const noexcept;

	private:
		RelocationID             _id;
		std::optional<AESplitID> _split;
		std::uint32_t            _vrOffset{ 0 };
	};

	enum class AutoCallsiteBranch : std::uint8_t
	{
		kCall,
		kJump,
		kCallOrJump
	};

	enum class CallsiteStatus : std::uint8_t
	{
		kResolved,
		kResolvedKnownOffset,
		kNotFound,
		kAmbiguous,
		kUnknownID,
		kNoFunctionRange,
		kInvalidArgument
	};

	[[nodiscard]] std::string_view callsite_status_text(CallsiteStatus a_status) noexcept;

	class AutoCallsite
	{
	public:
		static constexpr std::uint16_t LAST = std::numeric_limits<std::uint16_t>::max() - 1;
		static constexpr std::uint16_t UNIQUE = std::numeric_limits<std::uint16_t>::max();

		explicit constexpr AutoCallsite(
			CallsiteID         a_target,
			AutoCallsiteBranch a_branch = AutoCallsiteBranch::kCall,
			std::uint16_t      a_occurrence = UNIQUE) noexcept :
			_target(a_target),
			_branch(a_branch),
			_occurrence(a_occurrence)
		{}

		// A known owner-relative offset for the listed game versions, for a call that another plugin may
		// already have redirected. On a listed version it is used first, but only if the live instruction
		// there is a call (jump for kJump) to the target or to code outside the game executable;
		// otherwise the automatic search decides. Take the offsets from the unmodified executable.
		//   REL::AUTO_CALLSITE(kTarget).or_offset(0x85, REL::Version{ 1, 6, 1170, 0 })
		[[nodiscard]] constexpr AutoCallsite or_offset(
			std::ptrdiff_t a_offset,
			Version        a_version1,
			Version        a_version2 = Version{},
			Version        a_version3 = Version{},
			Version        a_version4 = Version{}) const noexcept
		{
			auto copy = *this;
			copy._knownOffset = a_offset;
			copy._knownVersions = { a_version1, a_version2, a_version3, a_version4 };
			return copy;
		}

		[[nodiscard]] constexpr const CallsiteID&  target() const noexcept { return _target; }
		[[nodiscard]] constexpr AutoCallsiteBranch branch() const noexcept { return _branch; }
		[[nodiscard]] constexpr std::uint16_t      occurrence() const noexcept { return _occurrence; }

		[[nodiscard]] constexpr std::optional<std::ptrdiff_t> known_offset(const Version& a_version) const noexcept
		{
			if (_knownOffset) {
				for (const auto& version : _knownVersions) {
					if (version != Version{} && version == a_version) {
						return _knownOffset;
					}
				}
			}
			return std::nullopt;
		}

	private:
		CallsiteID                    _target;
		AutoCallsiteBranch            _branch{ AutoCallsiteBranch::kCall };
		std::uint16_t                 _occurrence{ UNIQUE };
		std::optional<std::ptrdiff_t> _knownOffset;
		std::array<Version, 4>        _knownVersions{};
	};

	// Exactly one call to the target (the default).
	[[nodiscard]] constexpr AutoCallsite AUTO_CALLSITE(
		CallsiteID         a_target,
		AutoCallsiteBranch a_branch = AutoCallsiteBranch::kCall) noexcept
	{
		return AutoCallsite{ a_target, a_branch, AutoCallsite::UNIQUE };
	}

	[[nodiscard]] constexpr AutoCallsite AUTO_CALLSITE_FIRST(
		CallsiteID         a_target,
		AutoCallsiteBranch a_branch = AutoCallsiteBranch::kCall) noexcept
	{
		return AutoCallsite{ a_target, a_branch, 0 };
	}

	[[nodiscard]] constexpr AutoCallsite AUTO_CALLSITE_LAST(
		CallsiteID         a_target,
		AutoCallsiteBranch a_branch = AutoCallsiteBranch::kCall) noexcept
	{
		return AutoCallsite{ a_target, a_branch, AutoCallsite::LAST };
	}

	// a_index is zero-based: AUTO_CALLSITE_NTH(kTarget, 1) is the second call in address order.
	[[nodiscard]] constexpr AutoCallsite AUTO_CALLSITE_NTH(
		CallsiteID         a_target,
		std::uint16_t      a_index,
		AutoCallsiteBranch a_branch = AutoCallsiteBranch::kCall) noexcept
	{
		return AutoCallsite{ a_target, a_branch, a_index };
	}

	struct CallsiteResult
	{
		std::vector<std::uintptr_t> addresses;  // absolute, ascending
		std::vector<std::ptrdiff_t> offsets;    // relative to the owner start
		CallsiteStatus              status{ CallsiteStatus::kInvalidArgument };

		[[nodiscard]] explicit operator bool() const noexcept { return !addresses.empty(); }
	};

	struct CallsiteLookup
	{
		std::optional<std::uintptr_t> address;
		std::ptrdiff_t                offset{ 0 };   // relative to the owner start
		std::size_t                   matches{ 0 };  // hits of the automatic search
		CallsiteStatus                status{ CallsiteStatus::kInvalidArgument };
		std::string                   note;

		[[nodiscard]] explicit operator bool() const noexcept { return address.has_value(); }
	};

	// Every direct call/jump from the owner to the target.
	[[nodiscard]] CallsiteResult resolve_callsites(
		CallsiteID         a_owner,
		CallsiteID         a_target,
		AutoCallsiteBranch a_branch = AutoCallsiteBranch::kCall);

	// One callsite, without report_and_fail: on failure the address is empty and status/note say why.
	//   const auto site = REL::try_resolve_callsite(kRenderFrame, REL::AUTO_CALLSITE(kDrawWorld));
	//   if (!site) { logger::warn("feature off: {}", REL::callsite_status_text(site.status)); return; }
	//   trampoline.write_call<5>(*site.address, Hook);
	[[nodiscard]] CallsiteLookup try_resolve_callsite(CallsiteID a_owner, const AutoCallsite& a_callsite);

	// A byte pattern ("C7 44 24 20 00 00 40 00", "FF 15 ?? ?? ?? ??") inside the owner's code range.
	// Exactly one hit is required; none or several leave the address empty.
	[[nodiscard]] CallsiteLookup try_resolve_pattern(CallsiteID a_owner, std::string_view a_pattern);
}
