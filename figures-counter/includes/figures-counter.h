#pragma once

#include <filesystem>
#include <istream>

namespace figures_counter {

/**
 * @brief Returns the number of figures in the BMP file @p bmp
 *
 * A figure is consisting of one or more pixels with value different from the background @p ? that are in
 * 4-connectivity. In example 1 there is one figure and in example 2 there are 2 figures.
 *
 * Ex. 1          Ex. 2
 * | * |          |*  |
 * | * |          | * |
 * |   |          |   |
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