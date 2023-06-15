#include <line_loader.h>

#include <error.h>

namespace figures_counter {

line_loader::line_loader(std::istream& stream, size_t lines_count, size_t line_length) :
	_stream(stream),
	_lines_count(lines_count),
	_line_length(line_length),
	_lines_loaded(0) {
	if (_lines_count <= 0) [[unlikely]] {
		throw error::bad_input("lines count must be positive");
	}
	if (_line_length <= 0) [[unlikely]] {
		throw error::bad_input("line length must be positive");
	}
	if (!_stream) [[unlikely]] {
		throw error::bad_input("data stream can not be read");
	}

	_buffer.reserve(line_length);
}

std::string_view line_loader::load_line() {
	std::getline(_stream, _buffer);
	if (!_stream) [[unlikely]] {
		throw error::row_count_error(_lines_loaded, _lines_count);
	}
	if (_buffer.size() != _line_length) [[unlikely]] {
		throw error::row_length_error(_lines_loaded, _buffer.size() - 1, _line_length);
	}
	++_lines_loaded;
	return _buffer;
}

size_t line_loader::lines_loaded() const noexcept {
	return _lines_loaded;
}

bool line_loader::not_finished() const {
	return _lines_loaded < _lines_count;
}

} // namespace figures_counter