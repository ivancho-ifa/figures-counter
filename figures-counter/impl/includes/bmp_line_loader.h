#pragma once

#include <bmp.h>

#include <error.h>

#include <filesystem>
#include <format>
#include <fstream>
#include <span>

namespace figures_counter {

class bmp_line_loader {
public:
	/**
	 * @throws error::bad_input If any of the input parameters is not in the described format
	 */

	bmp_line_loader(const std::filesystem::path& bmp_file);

	/**
	 * @returns The next line from the @p stream
	 *
	 * @warning The buffer to which this @p std::string_view points will change on next call to @p load_line
	 *
	 * @throws error::row_count_error If @p rows is bigger than the actual rows in @p data
	 * @throws error::row_length_error If one of the rows length differs from @p cols
	 */

	std::span<std::byte> load_line();

	/**
	 * @returns Number of lines already loaded from @p stream
	 */

	size_t lines_loaded() const noexcept;

	/**
	 * @returns Whether the expected number of lines are read
	 */

	bool not_finished() const;

	const bmp_info_header& info_header() const noexcept;

private:
	std::ifstream _bmp_in;
	bmp_file_header _file_header;
	bmp_info_header _info_header;
	std::vector<std::byte> _buffer;
	size_t _lines_loaded;
};

} // namespace figures_counter