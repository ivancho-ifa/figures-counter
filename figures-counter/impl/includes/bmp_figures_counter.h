#pragma once

#include <bmp_line_loader.h>
#include <disjoint_set.h>

#include <istream>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace figures_counter {

/**
 * @brief Figures counter for BMP files
 *
 * A figure is consisting of one or more pixels with value different from the background that are in
 * 4-connectivity. In example 1 there is one figure and in example 2 there are 2 figures.
 *
 * Ex. 1          Ex. 2
 * | * |          |*  |
 * | * |          | * |
 * |   |          |   |
 *
 * @warning Currently this class works only with BMP files with 8 bit color depth and no compression.
 * @warning This is an internal class don't use directly but through @link figures_counter wrappers
 */
class bmp_figures_counter final {
public:
	/**
	 * @brief Initializes the counter
	 *
	 * @param[in] bmp The BMP file with the figures
	 * @param[in] background Which color to recognize as background
	 *
	 * @warning Modifying the file in @p bmp before this method finishes execution will cause undefined behavior.
	 *
	 * @throws error::bad_input If there's an error while reading the file. Check @link bmp_line_loader for more info on
	 * the errors.
	 */

	bmp_figures_counter(const std::filesystem::path& bmp, std::byte background);

	/**
	 * Counts the figures in @p bmp
	 *
	 * @warning This method should be called only once. Calling it multiple times on the same object will cause
	 * undefined behavior.
	 *
	 * @return The number of figures in the BMP file.
	 *
	 * @throws error::bad_input If there's an error while reading the file
	 */

	size_t count_figures();

private:
	static constexpr size_t EMPTY_FIGURE_ID =
		UINT_MAX; ///< The figure ID for background used when determining neighbor figure IDs.

	void assign_figure_ids_for_line(std::span<std::byte> buffer);
	size_t get_figure_id_from_neighbors(size_t left_cell, size_t up_cell);

	bmp_line_loader _bmp;              ///< Wrapper around the stream that reads lines of the image one by one
	std::vector<size_t> _prev_line;    ///< Holds the figure IDs of the pixels from the previous line
	std::vector<size_t> _current_line; ///< Holds the figure IDs of the pixels from the current line
	disjoint_set _figure_ids; ///< Holds the relations between figure IDs. Used to check which figure IDs identify the
	                          ///< same figure.
	std::byte _background;    ///< Color of the background
};

} // namespace figures_counter