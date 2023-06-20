#pragma once

#include <error.h>

#include <filesystem>
#include <format>
#include <fstream>
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

static void read_bmp_headers(std::ifstream& file, bmp_file_header& file_header, bmp_info_header& info_header) {
	// Read the file header
	file.read(file_header.id, sizeof(file_header.id));
	file.read(reinterpret_cast<char*>(&file_header.file_size), sizeof(file_header.file_size));
	file.read(reinterpret_cast<char*>(&file_header.reserved), sizeof(file_header.reserved));
	file.read(reinterpret_cast<char*>(&file_header.pixel_array_offset), sizeof(file_header.pixel_array_offset));

	// Read the info header
	file.read(reinterpret_cast<char*>(&info_header.header_size), sizeof(info_header.header_size));
	file.read(reinterpret_cast<char*>(&info_header.width), sizeof(info_header.width));
	file.read(reinterpret_cast<char*>(&info_header.height), sizeof(info_header.height));
	file.read(reinterpret_cast<char*>(&info_header.color_panes), sizeof(info_header.color_panes));
	file.read(reinterpret_cast<char*>(&info_header.bpp), sizeof(info_header.bpp));
	file.read(reinterpret_cast<char*>(&info_header.compression), sizeof(info_header.compression));
	file.read(reinterpret_cast<char*>(&info_header.size), sizeof(info_header.size));
	file.read(reinterpret_cast<char*>(&info_header.horizontal_resolution), sizeof(info_header.horizontal_resolution));
	file.read(reinterpret_cast<char*>(&info_header.vertical_resolution), sizeof(info_header.vertical_resolution));
	file.read(reinterpret_cast<char*>(&info_header.color_palette), sizeof(info_header.color_palette));
	file.read(reinterpret_cast<char*>(&info_header.important_colors), sizeof(info_header.important_colors));
}

class bmp_line_loader {
public:
	/**
	 * @throws error::bad_input If any of the input parameters is not in the described format
	 */

	bmp_line_loader(const std::filesystem::path& bmp) :
		_bmp_in(bmp, std::ifstream::in | std::ifstream::binary),
		_lines_loaded(0) {
		read_bmp_headers(_bmp_in, _file_header, _info_header);

		if (strncmp(_file_header.id, "BM", 2) != 0) [[unlikely]] {
			throw error::bad_input(std::format("{} is of not supported type {}", bmp.string(), _file_header.id));
		}

		if (_info_header.bpp != 8) {
			throw error::bad_input(
				std::format("only 8 bpp is supported {} uses {} bpp", bmp.string(), _info_header.bpp));
		}
		if (_info_header.compression != 0) {
			throw error::bad_input(std::format(
				"{} compression method is unsupported, only uncompressed BMPs are supported", bmp.string()));
		}

		_bmp_in.seekg(_file_header.pixel_array_offset + _info_header.size);
		if (!_bmp_in || _bmp_in.tellg() == std::ifstream::traits_type::pos_type(-1)) {
			_bmp_in.seekg(0, std::ifstream::end);
			const auto end = _bmp_in.tellg();
			_bmp_in.seekg(0);
			const auto begin = _bmp_in.tellg();
			const auto size = end - begin;
			throw error::bad_input(std::format("{} with resolution {} * {} has insufficient pixels count {}",
			                                   bmp.string(), _info_header.width, _info_header.height, size));
		}

		// Move to the start of the pixel data
		_bmp_in.seekg(_file_header.pixel_array_offset);
		_buffer.resize(_info_header.width);
	}

	/**
	 * @returns the next line from the @p stream
	 *
	 * @warning the buffer to which this @p std::string_view points will change on next call to @p load_line
	 *
	 * @throws error::row_count_error If @p rows is bigger than the actual rows in @p data
	 * @throws error::row_length_error If one of the rows length differs from @p cols
	 */

	std::span<std::byte> load_line() {
		_bmp_in.read(reinterpret_cast<char*>(_buffer.data()), _buffer.size());
		if (!_bmp_in) [[unlikely]] {
			throw error::bad_input("error while reading pixel data from file");
		}
		++_lines_loaded;
		return _buffer;
	}

	/**
	 * @returns number of lines already loaded from @p stream
	 */

	size_t lines_loaded() const noexcept {
		return _lines_loaded;
	}

	/**
	 * @returns whether the expected number of lines are read
	 */

	bool not_finished() const {
		return _lines_loaded < _info_header.height;
	}

	const bmp_info_header& info_header() const noexcept {
		return _info_header;
	}

private:
	std::ifstream _bmp_in;
	bmp_file_header _file_header;
	bmp_info_header _info_header;
	std::vector<std::byte> _buffer;
	size_t _lines_loaded;
};

} // namespace figures_counter