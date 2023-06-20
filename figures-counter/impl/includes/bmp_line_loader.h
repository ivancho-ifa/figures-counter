#pragma once

#include <bmp.h>

#include <error.h>

#include <filesystem>
#include <format>
#include <fstream>
#include <span>

namespace figures_counter {

/**
 * @brief Loads a BMP image line by line.
 */
class bmp_line_loader {
public:
	/**
	 * @brief A BMP file with no compression and 8-bit color depth.
	 * @throws error::bad_input If any of the input parameters is not in the described format
	 */
	bmp_line_loader(std::filesystem::path bmp_file);

	/**
	 * @returns The next line from the @p _bmp_in
	 *
	 * @warning The buffer to which this @p std::span points will change on next call to @p load_line
	 *
	 * @throws error::bad_input If @p _bmp_in is in bad state before end of data
	 */
	std::span<std::byte> load_line();

	/**
	 * @returns Number of lines already loaded from @p stream
	 */
	[[nodiscard]] size_t lines_loaded() const noexcept;

	/**
	 * @returns Whether the expected number of lines are read
	 */
	[[nodiscard]] bool not_finished() const noexcept;

	/**
	 * @return The number of lines expected to be read.
	 */
	[[nodiscard]] size_t lines_count() const noexcept;

	/**
	 * @return The length of each line.
	 */
	[[nodiscard]] size_t line_length() const noexcept;

	/**
	 * @brief Resets the state of this class to the initial state, reading can begin from start.
	 */
	void reset();

private:
	std::filesystem::path _bmp_file;
	std::ifstream _bmp_in;
	bmp_file_header _file_header;
	bmp_info_header _info_header;
	std::vector<std::byte> _buffer;
	size_t _lines_loaded;
};

} // namespace figures_counter