#pragma once

#include <error.h>

// Disable warnings on the external file
#pragma warning(push)
#pragma warning(disable : 4996)
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#pragma warning(pop)

#include <filesystem>
#include <format>
#include <span>

namespace figures_counter {

class bmp_file_header {
public:
	char id[2];
	uint32_t file_size; ///< In bytes
	uint32_t reserved;  ///< Reserved values, 0
	uint32_t pixel_array_offset;
};

class bmp_info_header {
public:
	uint32_t header_size;
	uint32_t width;
	uint32_t height;
	uint16_t color_panes;
	uint16_t bpp; ///< Bits per pixel
	uint32_t compression;
	uint32_t size;
	uint32_t horizontal_resolution;
	uint32_t vertical_resolution;
	uint32_t color_palette;
	uint32_t important_colors;
};

class bmp_line_loader {
public:
	bmp_line_loader(const std::filesystem::path& bmp) :
		_file(bmp.c_str(), boost::interprocess::read_only),
		_file_page(_file, boost::interprocess::read_only, 0, boost::interprocess::mapped_region::get_page_size()),
		_lines_loaded(0) {
		const auto* file_header = static_cast<bmp_file_header*>(_file_page.get_address());
		if (strncmp(file_header->id, "BM", 2) != 0) [[unlikely]] {
			throw error::bad_input(std::format("{} is of not supported type {}", bmp.string(), file_header->id));
		}

		const auto* info_header_begin = static_cast<std::byte*>(_file_page.get_address()) + sizeof(bmp_file_header);
		const auto* info_header = reinterpret_cast<const bmp_info_header*>(info_header_begin);
		if (info_header->bpp != 8) {
			throw error::bad_input(
				std::format("only 8 bpp is supported {} uses {} bpp", bmp.string(), info_header->bpp));
		}
		if (info_header->compression != 0) {
			throw error::bad_input(std::format(
				"{} compression method is unsupported, only uncompressed BMPs are supported", bmp.string()));
		}

		_lines_count = info_header->height;
		_line_length = info_header->width;
		_next_row = static_cast<std::byte*>(_file_page.get_address()) + file_header->pixel_array_offset;
	}

	std::span<const std::byte> load_line() {
		const auto* const next_row_end = _next_row + _line_length;
		const auto* const page_end = static_cast<std::byte*>(_file_page.get_address()) + _file_page.get_size();
		if (next_row_end < page_end) {
			const std::span<const std::byte> result(_next_row, next_row_end);
			_next_row = next_row_end;
			return result;
		}

		
	}

private:
	boost::interprocess::file_mapping _file;
	boost::interprocess::mapped_region _file_page;
	const std::byte* _next_row;
	[[maybe_unused]] size_t _lines_count;
	[[maybe_unused]] size_t _line_length;
	[[maybe_unused]] size_t _lines_loaded;
};

} // namespace figures_counter