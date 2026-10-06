#include "REL/Relocation.h"

#define WIN32_LEAN_AND_MEAN

#define NOGDICAPMASKS
#define NOVIRTUALKEYCODES
//#define NOWINMESSAGES
#define NOWINSTYLES
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOKEYSTATES
#define NOSYSCOMMANDS
#define NORASTEROPS
#define NOSHOWWINDOW
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
//#define NOCTLMGR
#define NODRAWTEXT
#define NOGDI
#define NOKERNEL
//#define NOUSER
#define NONLS
//#define NOMB
#define NOMEMMGR
#define NOMETAFILE
#define NOMINMAX
//#define NOMSG
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOWINOFFSETS
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX

#include <Windows.h>

namespace REL
{
	namespace detail
	{
		bool memory_map::open(stl::zwstring a_name, std::size_t a_size)
		{
			close();

			::ULARGE_INTEGER bytes;
			bytes.QuadPart = a_size;

			_mapping = ::OpenFileMappingW(
				FILE_MAP_READ | FILE_MAP_WRITE,
				false,
				a_name.data());
			if (!_mapping) {
				close();
				return false;
			}

			_view = ::MapViewOfFile(
				_mapping,
				FILE_MAP_READ | FILE_MAP_WRITE,
				0,
				0,
				bytes.QuadPart);
			if (!_view) {
				close();
				return false;
			}

			return true;
		}

		bool memory_map::create(stl::zwstring a_name, std::size_t a_size)
		{
			close();

			::ULARGE_INTEGER bytes;
			bytes.QuadPart = a_size;

			_mapping = ::OpenFileMappingW(
				FILE_MAP_READ | FILE_MAP_WRITE,
				false,
				a_name.data());
			if (!_mapping) {
				_mapping = ::CreateFileMappingW(
					INVALID_HANDLE_VALUE,
					nullptr,
					PAGE_READWRITE,
					bytes.HighPart,
					bytes.LowPart,
					a_name.data());
				if (!_mapping) {
					return false;
				}
			}

			_view = ::MapViewOfFile(
				_mapping,
				FILE_MAP_READ | FILE_MAP_WRITE,
				0,
				0,
				bytes.QuadPart);
			if (!_view) {
				return false;
			}

			return true;
		}

		void memory_map::close()
		{
			if (_view) {
				::UnmapViewOfFile(static_cast<const void*>(_view));
				_view = nullptr;
			}

			if (_mapping) {
				::CloseHandle(_mapping);
				_mapping = nullptr;
			}
		}
	}

	void Module::load_segments()
	{
		auto        dosHeader = reinterpret_cast<const IMAGE_DOS_HEADER*>(_base);
		auto        ntHeader = stl::adjust_pointer<IMAGE_NT_HEADERS64>(dosHeader, dosHeader->e_lfanew);
		const auto* sections = IMAGE_FIRST_SECTION(ntHeader);
		const auto  size = std::min<std::size_t>(ntHeader->FileHeader.NumberOfSections, _segments.size());
		for (std::size_t i = 0; i < size; ++i) {
			const auto& section = sections[i];
			const auto  it = std::find_if(
                SEGMENTS.begin(),
                SEGMENTS.end(),
                [&](auto&& a_elem) {
                    constexpr auto size = std::extent_v<decltype(section.Name)>;
                    const auto     len = std::min(a_elem.first.size(), size);
                    return std::memcmp(a_elem.first.data(), section.Name, len) == 0 &&
                           (section.Characteristics & a_elem.second) == a_elem.second;
                });
			if (it != SEGMENTS.end()) {
				const auto idx = static_cast<std::size_t>(std::distance(SEGMENTS.begin(), it));
				_segments[idx] = Segment{ _base, _base + section.VirtualAddress, section.Misc.VirtualSize };
			}
		}
	}

	namespace
	{
		struct RuntimeFunction
		{
			std::uint32_t begin;
			std::uint32_t end;
			std::uint32_t unwind;
		};
		static_assert(sizeof(RuntimeFunction) == 12);

		using CodeScopes = std::vector<std::pair<std::uint32_t, std::uint32_t>>;

		constexpr std::size_t   MAX_CALLSITE_FUNCTION_BYTES = 256u * 1024u;
		constexpr std::uint8_t  UNWIND_FLAG_CHAININFO = 0x4;
		constexpr std::uint32_t MAX_UNWIND_CHAIN = 32;

		struct GameImage
		{
			std::uintptr_t                   base{ 0 };
			std::uint32_t                    size{ 0 };
			std::span<const RuntimeFunction> functions;
			std::uint32_t                    textBegin{ 0 };
			std::uint32_t                    textEnd{ 0 };

			[[nodiscard]] bool contains(std::uint64_t a_rva, std::size_t a_size) const noexcept
			{
				return a_rva <= size && a_size <= size - a_rva;
			}
		};

		[[nodiscard]] GameImage game_image() noexcept
		{
			GameImage   image;
			const auto& module = Module::get();
			image.base = module.base();
			if (image.base == 0) {
				return image;
			}

			const auto* dosHeader = reinterpret_cast<const ::IMAGE_DOS_HEADER*>(image.base);
			const auto* ntHeader = reinterpret_cast<const ::IMAGE_NT_HEADERS64*>(image.base + dosHeader->e_lfanew);
			image.size = ntHeader->OptionalHeader.SizeOfImage;

			std::uint32_t pdataRVA = 0;
			std::uint32_t pdataSize = 0;
			if (ntHeader->OptionalHeader.NumberOfRvaAndSizes > IMAGE_DIRECTORY_ENTRY_EXCEPTION) {
				const auto& directory = ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION];
				pdataRVA = directory.VirtualAddress;
				pdataSize = directory.Size;
			}
			if (pdataRVA == 0 || pdataSize == 0) {
				const auto pdata = module.segment(Segment::pdata);
				pdataRVA = static_cast<std::uint32_t>(pdata.offset());
				pdataSize = static_cast<std::uint32_t>(pdata.size());
			}
			if (pdataRVA != 0 && image.contains(pdataRVA, pdataSize)) {
				image.functions = std::span(
					reinterpret_cast<const RuntimeFunction*>(image.base + pdataRVA),
					pdataSize / sizeof(RuntimeFunction));
			}

			const auto text = module.segment(Segment::textx);
			if (text.size() != 0 && image.contains(text.offset(), text.size())) {
				image.textBegin = static_cast<std::uint32_t>(text.offset());
				image.textEnd = static_cast<std::uint32_t>(text.offset() + text.size());
			}
			return image;
		}

		// The primary RUNTIME_FUNCTION of a (possibly split) function: follows chained unwind info.
		[[nodiscard]] std::optional<RuntimeFunction> root_function(const GameImage& a_image, RuntimeFunction a_function) noexcept
		{
			for (std::uint32_t hop = 0; hop < MAX_UNWIND_CHAIN; ++hop) {
				if (a_function.begin >= a_function.end || a_function.end > a_image.size) {
					return std::nullopt;
				}
				if (a_function.unwind == 0) {
					return a_function;
				}
				if ((a_function.unwind & 1u) != 0) {
					// Indirect entry: the unwind field points to another RUNTIME_FUNCTION.
					const auto next = a_function.unwind & ~1u;
					if (!a_image.contains(next, sizeof(RuntimeFunction))) {
						return std::nullopt;
					}
					std::memcpy(std::addressof(a_function), reinterpret_cast<const void*>(a_image.base + next), sizeof(RuntimeFunction));
					continue;
				}
				if (!a_image.contains(a_function.unwind, 4)) {
					return std::nullopt;
				}
				const auto* header = reinterpret_cast<const std::uint8_t*>(a_image.base + a_function.unwind);
				if (((header[0] >> 3) & UNWIND_FLAG_CHAININFO) == 0) {
					return a_function;
				}
				const auto codeCount = static_cast<std::uint32_t>(header[2]);
				const auto chained = static_cast<std::uint64_t>(a_function.unwind) + 4 + ((codeCount + 1) & ~1u) * 2;
				if (!a_image.contains(chained, sizeof(RuntimeFunction))) {
					return std::nullopt;
				}
				std::memcpy(std::addressof(a_function), reinterpret_cast<const void*>(a_image.base + chained), sizeof(RuntimeFunction));
			}
			return std::nullopt;
		}

		// The code ranges of the function that starts at (or contains) a_ownerRVA, sorted and merged.
		[[nodiscard]] CodeScopes owner_scopes(const GameImage& a_image, std::uint32_t a_ownerRVA)
		{
			const auto functions = a_image.functions;
			const auto it = std::upper_bound(
				functions.begin(),
				functions.end(),
				a_ownerRVA,
				[](std::uint32_t a_rva, const RuntimeFunction& a_function) { return a_rva < a_function.begin; });
			if (it == functions.begin()) {
				return {};
			}
			const auto& containing = *std::prev(it);
			if (a_ownerRVA < containing.begin || a_ownerRVA >= containing.end) {
				return {};
			}
			const auto root = root_function(a_image, containing);
			if (!root) {
				return {};
			}

			CodeScopes scopes;
			scopes.emplace_back(root->begin, root->end);
			for (const auto& function : functions) {
				if (function.unwind == 0 || (function.begin == root->begin && function.end == root->end)) {
					continue;
				}
				const auto candidate = root_function(a_image, function);
				if (candidate && candidate->begin == root->begin && candidate->end == root->end) {
					scopes.emplace_back(function.begin, function.end);
				}
			}
			std::ranges::sort(scopes);

			CodeScopes  merged;
			std::size_t totalBytes = 0;
			for (const auto& scope : scopes) {
				if (!merged.empty() && scope.first <= merged.back().second) {
					merged.back().second = (std::max)(merged.back().second, scope.second);
				} else {
					merged.push_back(scope);
				}
			}
			for (const auto& [begin, end] : merged) {
				if (begin >= end || begin < a_image.textBegin || end > a_image.textEnd) {
					return {};
				}
				totalBytes += end - begin;
			}
			return totalBytes <= MAX_CALLSITE_FUNCTION_BYTES ? merged : CodeScopes{};
		}

		[[nodiscard]] std::optional<std::uint32_t> try_rva(std::uint64_t a_id)
		{
			if (a_id == 0) {
				return std::nullopt;
			}
			const auto offset = IDDatabase::get().try_id2offset(a_id);
			if (!offset || *offset == 0 || *offset > (std::numeric_limits<std::uint32_t>::max)()) {
				return std::nullopt;
			}
			return static_cast<std::uint32_t>(*offset);
		}

		[[nodiscard]] bool branch_accepts(AutoCallsiteBranch a_branch, std::uint8_t a_opcode) noexcept
		{
			switch (a_branch) {
			case AutoCallsiteBranch::kCall:
				return a_opcode == 0xE8;
			case AutoCallsiteBranch::kJump:
				return a_opcode == 0xE9;
			case AutoCallsiteBranch::kCallOrJump:
				return a_opcode == 0xE8 || a_opcode == 0xE9;
			default:
				return false;
			}
		}

		[[nodiscard]] bool valid_branch(AutoCallsiteBranch a_branch) noexcept
		{
			return a_branch == AutoCallsiteBranch::kCall ||
			       a_branch == AutoCallsiteBranch::kJump ||
			       a_branch == AutoCallsiteBranch::kCallOrJump;
		}

		[[nodiscard]] std::uintptr_t branch_destination(std::uintptr_t a_site) noexcept
		{
			std::int32_t displacement = 0;
			std::memcpy(std::addressof(displacement), reinterpret_cast<const void*>(a_site + 1), sizeof(displacement));
			return a_site + 5 + static_cast<std::intptr_t>(displacement);
		}

		[[nodiscard]] bool readable_memory(std::uintptr_t a_address, std::size_t a_size) noexcept
		{
			::MEMORY_BASIC_INFORMATION info{};
			if (::VirtualQuery(reinterpret_cast<const void*>(a_address), std::addressof(info), sizeof(info)) != sizeof(info) ||
				info.State != MEM_COMMIT || (info.Protect & (PAGE_GUARD | PAGE_NOACCESS)) != 0) {
				return false;
			}
			const auto end = reinterpret_cast<std::uintptr_t>(info.BaseAddress) + info.RegionSize;
			return a_address <= end && a_size <= end - a_address;
		}

		// The module that owns the code at a_address, following up to three trampoline hops
		// (jmp [rip+x] or jmp rel32), as used by SKSE's write_call.
		[[nodiscard]] std::string owning_module_name(std::uintptr_t a_address)
		{
			auto address = a_address;
			for (int hop = 0; hop < 4; ++hop) {
				::HMODULE module = nullptr;
				if (::GetModuleHandleExW(
						GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
						reinterpret_cast<::LPCWSTR>(address),
						std::addressof(module))) {
					std::array<wchar_t, MAX_PATH> path{};
					const auto length = ::GetModuleFileNameW(module, path.data(), static_cast<::DWORD>(path.size()));
					std::wstring_view name(path.data(), length < path.size() ? length : 0);
					if (const auto slash = name.find_last_of(L"\\/"); slash != std::wstring_view::npos) {
						name.remove_prefix(slash + 1);
					}
					return name.empty() ? "an unnamed module"s : stl::utf16_to_utf8(name).value_or("a module"s);
				}
				if (hop == 3 || !readable_memory(address, 14)) {
					break;
				}
				const auto* code = reinterpret_cast<const std::uint8_t*>(address);
				std::int32_t relative = 0;
				if (code[0] == 0xFF && code[1] == 0x25) {
					std::memcpy(std::addressof(relative), code + 2, sizeof(relative));
					const auto slot = address + 6 + static_cast<std::intptr_t>(relative);
					if (!readable_memory(slot, sizeof(std::uintptr_t))) {
						break;
					}
					std::memcpy(std::addressof(address), reinterpret_cast<const void*>(slot), sizeof(address));
				} else if (code[0] == 0xE9) {
					std::memcpy(std::addressof(relative), code + 1, sizeof(relative));
					address = address + 5 + static_cast<std::intptr_t>(relative);
				} else {
					break;
				}
			}
			return "unknown code"s;
		}

		struct ResolvedOwner
		{
			GameImage     image;
			std::uint32_t ownerRVA{ 0 };
			CodeScopes    scopes;
		};

		[[nodiscard]] std::optional<ResolvedOwner> resolve_owner(CallsiteID a_owner, CallsiteStatus& a_status)
		{
			ResolvedOwner result;
			const auto    ownerRVA = try_rva(a_owner.id());
			if (!ownerRVA) {
				a_status = CallsiteStatus::kUnknownID;
				return std::nullopt;
			}
			result.ownerRVA = *ownerRVA;
			result.image = game_image();
			result.scopes = owner_scopes(result.image, result.ownerRVA);
			if (result.scopes.empty()) {
				a_status = CallsiteStatus::kNoFunctionRange;
				return std::nullopt;
			}
			return result;
		}

		[[nodiscard]] std::vector<std::uint32_t> scan_callsites(
			const ResolvedOwner& a_owner,
			std::uint32_t        a_targetRVA,
			AutoCallsiteBranch   a_branch)
		{
			std::vector<std::uint32_t> sites;
			const auto                 target = a_owner.image.base + a_targetRVA;
			for (const auto& [begin, end] : a_owner.scopes) {
				if (end - begin < 5) {
					continue;
				}
				for (auto rva = begin; rva <= end - 5; ++rva) {
					const auto site = a_owner.image.base + rva;
					if (branch_accepts(a_branch, *reinterpret_cast<const std::uint8_t*>(site)) &&
						branch_destination(site) == target) {
						sites.push_back(rva);
					}
				}
			}
			return sites;
		}

		[[nodiscard]] std::string branch_text(AutoCallsiteBranch a_branch)
		{
			return a_branch == AutoCallsiteBranch::kCall ? "call"s :
			       a_branch == AutoCallsiteBranch::kJump ? "jump"s :
			                                               "call or jump"s;
		}

		// A known offset (AutoCallsite::or_offset) is accepted only inside the owner's code, on a call/jump
		// to the target itself or to code outside the game image.
		[[nodiscard]] bool check_known_offset(
			const ResolvedOwner& a_owner,
			std::ptrdiff_t       a_offset,
			std::uint32_t        a_targetRVA,
			AutoCallsiteBranch   a_branch,
			std::string&         a_note)
		{
			const auto signedSite = static_cast<std::int64_t>(a_owner.ownerRVA) + static_cast<std::int64_t>(a_offset);
			const auto inside = signedSite >= 0 && std::ranges::any_of(a_owner.scopes, [&](const auto& a_scope) {
				return static_cast<std::uint64_t>(signedSite) >= a_scope.first &&
				       static_cast<std::uint64_t>(signedSite) + 5 <= a_scope.second;
			});
			if (!inside) {
				a_note = fmt::format("known offset {:+#x} is not inside the owner's code", a_offset);
				return false;
			}
			const auto site = a_owner.image.base + static_cast<std::uintptr_t>(signedSite);
			const auto opcode = *reinterpret_cast<const std::uint8_t*>(site);
			if (!branch_accepts(a_branch, opcode)) {
				a_note = fmt::format("known offset {:+#x} holds no {} (byte 0x{:02X})", a_offset, branch_text(a_branch), opcode);
				return false;
			}
			const auto destination = branch_destination(site);
			const auto imageBegin = a_owner.image.base;
			const auto imageEnd = imageBegin + a_owner.image.size;
			if (destination == imageBegin + a_targetRVA) {
				return true;
			}
			if (destination < imageBegin || destination >= imageEnd) {
				a_note = fmt::format("known offset {:+#x}: the call is already redirected by {}", a_offset, owning_module_name(destination));
				return true;
			}
			a_note = fmt::format("known offset {:+#x} calls another game function (offset 0x{:X})", a_offset, destination - imageBegin);
			return false;
		}

		[[nodiscard]] std::optional<std::vector<std::optional<std::uint8_t>>> parse_pattern(std::string_view a_pattern)
		{
			std::vector<std::optional<std::uint8_t>> bytes;
			std::size_t                              pos = 0;
			while (pos < a_pattern.size()) {
				if (a_pattern[pos] == ' ') {
					++pos;
					continue;
				}
				const auto end = (std::min)(a_pattern.find(' ', pos), a_pattern.size());
				const auto token = a_pattern.substr(pos, end - pos);
				pos = end;
				if (token == "?" || token == "??") {
					bytes.emplace_back(std::nullopt);
					continue;
				}
				if (token.size() != 2) {
					return std::nullopt;
				}
				std::uint8_t value = 0;
				for (const auto ch : token) {
					value = static_cast<std::uint8_t>(value << 4);
					if (ch >= '0' && ch <= '9') {
						value |= static_cast<std::uint8_t>(ch - '0');
					} else if (ch >= 'A' && ch <= 'F') {
						value |= static_cast<std::uint8_t>(ch - 'A' + 10);
					} else if (ch >= 'a' && ch <= 'f') {
						value |= static_cast<std::uint8_t>(ch - 'a' + 10);
					} else {
						return std::nullopt;
					}
				}
				bytes.emplace_back(value);
			}
			if (std::ranges::none_of(bytes, [](const auto& a_byte) { return a_byte.has_value(); })) {
				return std::nullopt;
			}
			return bytes;
		}
	}

	std::string_view callsite_status_text(CallsiteStatus a_status) noexcept
	{
		switch (a_status) {
		case CallsiteStatus::kResolved:
			return "resolved"sv;
		case CallsiteStatus::kResolvedKnownOffset:
			return "known_offset"sv;
		case CallsiteStatus::kNotFound:
			return "not_found"sv;
		case CallsiteStatus::kAmbiguous:
			return "ambiguous"sv;
		case CallsiteStatus::kUnknownID:
			return "unknown_id"sv;
		case CallsiteStatus::kNoFunctionRange:
			return "no_function_range"sv;
		case CallsiteStatus::kInvalidArgument:
		default:
			return "invalid_argument"sv;
		}
	}

	CallsiteResult resolve_callsites(CallsiteID a_owner, CallsiteID a_target, AutoCallsiteBranch a_branch)
	{
		CallsiteResult result;
		if (!valid_branch(a_branch)) {
			return result;
		}
		const auto owner = resolve_owner(a_owner, result.status);
		if (!owner) {
			return result;
		}
		const auto targetRVA = try_rva(a_target.id());
		if (!targetRVA) {
			result.status = CallsiteStatus::kUnknownID;
			return result;
		}
		for (const auto rva : scan_callsites(*owner, *targetRVA, a_branch)) {
			result.addresses.push_back(owner->image.base + rva);
			result.offsets.push_back(static_cast<std::ptrdiff_t>(rva) - static_cast<std::ptrdiff_t>(owner->ownerRVA));
		}
		result.status = result.addresses.empty() ? CallsiteStatus::kNotFound : CallsiteStatus::kResolved;
		return result;
	}

	CallsiteLookup try_resolve_callsite(CallsiteID a_owner, const AutoCallsite& a_callsite)
	{
		CallsiteLookup lookup;
		if (!valid_branch(a_callsite.branch())) {
			return lookup;
		}
		const auto owner = resolve_owner(a_owner, lookup.status);
		if (!owner) {
			return lookup;
		}
		const auto targetRVA = try_rva(a_callsite.target().id());
		if (!targetRVA) {
			lookup.status = CallsiteStatus::kUnknownID;
			return lookup;
		}

		if (const auto known = a_callsite.known_offset(Module::get().version())) {
			if (check_known_offset(*owner, *known, *targetRVA, a_callsite.branch(), lookup.note)) {
				lookup.offset = *known;
				lookup.address = owner->image.base + owner->ownerRVA + *known;
				lookup.status = CallsiteStatus::kResolvedKnownOffset;
				return lookup;
			}
		}

		const auto sites = scan_callsites(*owner, *targetRVA, a_callsite.branch());
		lookup.matches = sites.size();
		std::optional<std::uint32_t> selected;
		const auto                   occurrence = a_callsite.occurrence();
		if (occurrence == AutoCallsite::UNIQUE) {
			if (sites.size() == 1) {
				selected = sites.front();
			} else {
				lookup.status = sites.empty() ? CallsiteStatus::kNotFound : CallsiteStatus::kAmbiguous;
			}
		} else if (occurrence == AutoCallsite::LAST) {
			if (!sites.empty()) {
				selected = sites.back();
			} else {
				lookup.status = CallsiteStatus::kNotFound;
			}
		} else if (occurrence < sites.size()) {
			selected = sites[occurrence];
		} else {
			lookup.status = CallsiteStatus::kNotFound;
		}

		if (selected) {
			lookup.offset = static_cast<std::ptrdiff_t>(*selected) - static_cast<std::ptrdiff_t>(owner->ownerRVA);
			lookup.address = owner->image.base + *selected;
			lookup.status = CallsiteStatus::kResolved;
		} else if (lookup.note.empty()) {
			lookup.note = fmt::format("{} hit(s) for {} to the target inside the owner", sites.size(), branch_text(a_callsite.branch()));
		} else {
			lookup.note = fmt::format("{}; {} hit(s) for {} to the target inside the owner", lookup.note, sites.size(), branch_text(a_callsite.branch()));
		}
		return lookup;
	}

	CallsiteLookup try_resolve_pattern(CallsiteID a_owner, std::string_view a_pattern)
	{
		CallsiteLookup lookup;
		const auto     pattern = parse_pattern(a_pattern);
		if (!pattern) {
			lookup.note = "invalid pattern";
			return lookup;
		}
		const auto owner = resolve_owner(a_owner, lookup.status);
		if (!owner) {
			return lookup;
		}

		std::optional<std::uint32_t> first;
		for (const auto& [begin, end] : owner->scopes) {
			if (end - begin < pattern->size()) {
				continue;
			}
			for (auto rva = begin; rva <= end - pattern->size(); ++rva) {
				const auto* code = reinterpret_cast<const std::uint8_t*>(owner->image.base + rva);
				bool        match = true;
				for (std::size_t i = 0; i < pattern->size() && match; ++i) {
					match = !(*pattern)[i] || *(*pattern)[i] == code[i];
				}
				if (match) {
					if (!first) {
						first = rva;
					}
					++lookup.matches;
				}
			}
		}

		if (lookup.matches == 1) {
			lookup.offset = static_cast<std::ptrdiff_t>(*first) - static_cast<std::ptrdiff_t>(owner->ownerRVA);
			lookup.address = owner->image.base + *first;
			lookup.status = CallsiteStatus::kResolved;
		} else {
			lookup.status = lookup.matches == 0 ? CallsiteStatus::kNotFound : CallsiteStatus::kAmbiguous;
			lookup.note = fmt::format("{} pattern hit(s) inside the owner", lookup.matches);
		}
		return lookup;
	}
}
