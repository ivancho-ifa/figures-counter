#include <bmp_line_loader.h>

#include <error.h>

#include <filesystem>
#include <format>
#include <fstream>
#include <span>

namespace figures_counter {

bmp_line_loader::bmp_line_loader(std::filesystem::path bmp_file) :
	_bmp_file(std::move(bmp_file)),
	_bmp_in(_bmp_file, std::ifstream::in | std::ifstream::binary) {
	reset();
}

std::span<std::byte> bmp_line_loader::load_line() {
	_bmp_in.read(reinterpret_cast<char*>(_buffer.data()), _buffer.size());
	if (!_bmp_in) [[unlikely]] {
		throw error::bad_input(std::format("error while reading pixel data from file {}", _bmp_file.string()));
	}
	++_lines_loaded;
	return _buffer;
}

size_t bmp_line_loader::lines_loaded() const noexcept {
	return _lines_loaded;
}

bool bmp_line_loader::not_finished() const noexcept {
	return _lines_loaded < _info_header.height;
}

size_t bmp_line_loader::lines_count() const noexcept {
	return _info_header.height;
}

size_t bmp_line_loader::line_length() const noexcept {
	return _info_header.width;
}

void bmp_line_loader::reset() {
	_bmp_in.clear();
	_bmp_in.seekg(0, std::ios::beg);

	_lines_loaded = 0;

	read_bmp_headers(_bmp_in, _file_header, _info_header);

	if (strncmp(_file_header.id, "BM", 2) != 0) [[unlikely]] {
		throw error::bad_input(std::format("{} is of not supported type {}", _bmp_file.string(), _file_header.id));
	}

	if (_info_header.bpp != 8) {
		throw error::bad_input(
			std::format("only 8 bpp is supported {} uses {} bpp", _bmp_file.string(), _info_header.bpp));
	}
	if (_info_header.compression != 0) {
		throw error::bad_input(std::format("{} compression method is unsupported, only uncompressed BMPs are supported",
		                                   _bmp_file.string()));
	}

	_bmp_in.seekg(_file_header.pixel_array_offset + _info_header.size);
	if (!_bmp_in || _bmp_in.tellg() == std::ifstream::traits_type::pos_type(-1)) {
		_bmp_in.seekg(0, std::ifstream::end);
		const auto end = _bmp_in.tellg();
		_bmp_in.seekg(0);
		const auto begin = _bmp_in.tellg();
		const auto size = end - begin;
		throw error::bad_input(std::format("{} with resolution {} * {} has insufficient pixels count {}",
		                                   _bmp_file.string(), _info_header.width, _info_header.height, size));
	}

	// Move to the start of the pixel data
	_bmp_in.seekg(_file_header.pixel_array_offset);
	_buffer.resize(_info_header.width);
}

} // namespace figures_counter