#pragma once

#include <binary_io/file_stream.hpp>

#include <array>
#include <cstdint>
#include <functional>
#include <stdexcept>

namespace REL::detail
{
	class address_library_v5 final
	{
	public:
		static constexpr std::int32_t format = 5;
		static constexpr std::size_t  executable_name_size = 64;
		static constexpr std::size_t  header_size =
			sizeof(std::int32_t) + (4 * sizeof(std::int32_t)) + executable_name_size +
			sizeof(std::uint64_t) + sizeof(std::uint32_t);

		struct header_t
		{
			std::array<std::int32_t, 4> version{};
			std::uint64_t               pointer_size{ 0 };
			std::uint32_t               address_count{ 0 };
		};

		[[nodiscard]] static header_t read_header(
			binary_io::file_istream& a_in,
			std::int32_t             a_observedFormat)
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
			a_in.seek_relative(executable_name_size);
			a_in.read(header.pointer_size, header.address_count);

			if (header.pointer_size == 0 || header.address_count == 0) {
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
