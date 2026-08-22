#pragma once

#include <binary_io/file_stream.hpp>

#include <array>
#include <cstdint>
#include <functional>
#include <span>
#include <stdexcept>

namespace REL::detail
{
	class address_library_v5 final
	{
	public:
		static constexpr std::uint32_t format = 5;
		static constexpr std::size_t  executable_name_size = 64;
		static constexpr std::size_t  header_size =
			sizeof(std::uint32_t) + (4 * sizeof(std::uint32_t)) + executable_name_size +
			(3 * sizeof(std::uint32_t));

		struct header_t
		{
			std::array<std::uint32_t, 4> version{};
			std::array<char, executable_name_size> executable_name{};
			std::uint32_t pointer_size{ 0 };
			std::uint32_t data_format{ 0 };
			std::uint32_t address_count{ 0 };
		};

		[[nodiscard]] static constexpr std::uint64_t expected_size(std::uint32_t a_addressCount) noexcept
		{
			return header_size + (static_cast<std::uint64_t>(a_addressCount) * sizeof(std::uint32_t));
		}

		[[nodiscard]] static header_t read_header(
			binary_io::file_istream& a_in,
			std::uint32_t            a_observedFormat)
		{
			if (a_observedFormat != format) {
				throw std::invalid_argument("not an Address Library V5 file");
			}

			header_t header;
			a_in.read(
				header.version[0],
				header.version[1],
				header.version[2],
				header.version[3]);
			a_in.read_bytes(std::as_writable_bytes(std::span{ header.executable_name }));
			a_in.read(header.pointer_size, header.data_format, header.address_count);

			if (header.pointer_size != sizeof(std::uintptr_t) ||
				header.data_format != 0 ||
				header.address_count == 0) {
				throw std::runtime_error("invalid Address Library V5 header");
			}

			return header;
		}

		template <class Sink>
		static void read_entries(
			binary_io::file_istream& a_in,
			std::uint32_t            a_addressCount,
			Sink&&                   a_sink)
		{
			for (std::uint64_t id = 0; id < a_addressCount; ++id) {
				const auto [rva] = a_in.read<std::uint32_t>();
				std::invoke(a_sink, id, static_cast<std::uint64_t>(rva));
			}
		}
	};
}
