#pragma once

#include <istream>
#include <string>
#include <string_view>

namespace figures_counter {

/**
 * @brief
 */

class line_loader {
public:
	line_loader(std::istream& stream, size_t lines_count, size_t line_length);

	std::string_view load_line();
	size_t lines_loaded() const noexcept;
	bool not_finished() const;

private:
	std::istream& _stream;
	std::string _buffer;
	size_t _lines_count;
	size_t _line_length;
	size_t _lines_loaded;
};

} // namespace figures_counter