#include <bmp_line_loader.h>

#include <error.h>

#include <filesystem>
#include <format>
#include <fstream>
#include <span>

namespace figures_counter {

bmp_line_loader::bmp_line_loader(const std::filesystem::path& bmp) :
	_bmp_in(bmp, std::ifstream::in | std::ifstream::binary),
	_lines_loaded(0) {
	read_bmp_headers(_bmp_in, _file_header, _info_header);

	if (strncmp(_file_header.id, "BM", 2) != 0) [[unlikely]] {
		throw error::bad_input(std::format("{} is of not supported type {}", bmp.string(), _file_header.id));
	}

	if (_info_header.bpp != 8) {
		throw error::bad_input(std::format("only 8 bpp is supported {} uses {} bpp", bmp.string(), _info_header.bpp));
	}
	if (_info_header.compression != 0) {
		throw error::bad_input(
			std::format("{} compression method is unsupported, only uncompressed BMPs are supported", bmp.string()));
	}

	_bmp_in.seekg(_file_header.pixel_array_offset + _info_header.size);
	if (!_bmp_in || _bmp_in.tellg() == std::ifstream::traits_type::pos_type(-1)) {
		_bmp_in.seekg(0, std::ifstream::end);
		const auto end = _bmp_in.tellg();
		_bmp_in.seekg(0);
		const auto begin = _bmp_in.tellg();
		const auto size = end - begin;
		throw error::bad_input(std::format("{} with resolution {} * {} has insufficient pixels count {}", bmp.string(),
		                                   _info_header.width, _info_header.height, size));
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

std::span<std::byte> bmp_line_loader::load_line() {
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

size_t bmp_line_loader::lines_loaded() const noexcept {
	return _lines_loaded;
}

/**
 * @returns whether the expected number of lines are read
 */

bool bmp_line_loader::not_finished() const {
	return _lines_loaded < _info_header.height;
}

const bmp_info_header& bmp_line_loader::info_header() const noexcept {
	return _info_header;
}

} // namespace figures_counter