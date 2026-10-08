#include "REL/Relocation.h"

#include "REX/W32/KERNEL32.h"

#include <map>
#include <memory>
#include <mutex>

namespace REL
{
	void safe_write(std::uintptr_t a_dst, const void* a_src, std::size_t a_count)
	{
		std::uint32_t old{ 0 };
		bool          success = REX::W32::VirtualProtect(
					 reinterpret_cast<void*>(a_dst), a_count, REX::W32::PAGE_EXECUTE_READWRITE, std::addressof(old));
		if (success) {
			std::memcpy(reinterpret_cast<void*>(a_dst), a_src, a_count);
			success = REX::W32::VirtualProtect(
				reinterpret_cast<void*>(a_dst), a_count, old, std::addressof(old));
		}

		assert(success);
	}

	void safe_fill(std::uintptr_t a_dst, std::uint8_t a_value, std::size_t a_count)
	{
		std::uint32_t old{ 0 };
		bool          success = REX::W32::VirtualProtect(
					 reinterpret_cast<void*>(a_dst), a_count, REX::W32::PAGE_EXECUTE_READWRITE, std::addressof(old));
		if (success) {
			std::fill_n(reinterpret_cast<std::uint8_t*>(a_dst), a_count, a_value);
			success = REX::W32::VirtualProtect(
				reinterpret_cast<void*>(a_dst), a_count, old, std::addressof(old));
		}

		assert(success);
	}

	const std::uintptr_t* InsertVTableSlots(const std::uintptr_t* a_cppVtable, std::size_t a_cppSlotCount,
		std::size_t a_insertAt, std::size_t a_insertCount, const std::uintptr_t* a_gameBaseVtable)
	{
		static std::mutex                                                     lock;
		static std::map<const std::uintptr_t*, std::unique_ptr<std::uintptr_t[]>> tables;

		std::scoped_lock guard{ lock };
		auto&            table = tables[a_cppVtable];
		if (!table) {
			// Slot -1 holds the RTTI locator (MSVC), so dynamic_cast and typeid keep working on the object.
			table = std::make_unique<std::uintptr_t[]>(1 + a_cppSlotCount + a_insertCount);
			table[0] = a_cppVtable[-1];
			for (std::size_t i = 0, out = 1; i < a_cppSlotCount; ++i, ++out) {
				if (i == a_insertAt) {
					for (std::size_t k = 0; k < a_insertCount; ++k) {
						table[out++] = a_gameBaseVtable[a_insertAt + k];
					}
				}
				table[out] = a_cppVtable[i];
			}
		}
		return table.get() + 1;
	}
}
