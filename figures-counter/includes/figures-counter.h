#pragma once

#include <filesystem>
#include <istream>

namespace figures_counter {

/**
 * @brief Returns the number of figures in the BMP file @p bmp
 *
 * For more information check @link bmp_figures_counter
 *
 * @param[in] bmp The BMP file with the figures
 *
 * @warning Modifying the file in @p bmp before this method finishes execution will cause undefined behavior.
 *
 * @return The number of figures in the input.
 *
 * @throws error::bad_input If there's an error while reading the file
 */

size_t count_figures(const std::filesystem::path& bmp);

} // namespace figures_counter