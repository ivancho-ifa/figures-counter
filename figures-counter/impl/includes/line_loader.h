#pragma once

#include <istream>
#include <string>
#include <string_view>

namespace figures_counter {

/**
 * @brief Wraps the logic of reading an input stream line by line
 */

class line_loader {
public:
	/**
	 * @throws error::bad_input If any of the input parameters is not in the described format
	 */

	line_loader(std::istream& stream, size_t lines_count, size_t line_length);

	/**
	 * @returns the next line from the @p stream
	 *
	 * @warning the buffer to which this @p std::string_view points will change on next call to @p load_line
	 *
	 * @throws error::row_count_error If @p rows is bigger than the actual rows in @p data
	 * @throws error::row_length_error If one of the rows length differs from @p cols
	 */

	std::string_view load_line();

	/**
	 * @returns number of lines already loaded from @p stream
	 */

	size_t lines_loaded() const noexcept;

	/**
	 * @returns whether the expected number of lines are read
	 */

	bool not_finished() const;

private:
	std::istream& _stream;
	std::string _buffer;
	size_t _lines_count;
	size_t _line_length;
	size_t _lines_loaded;
};

} // namespace figures_counter