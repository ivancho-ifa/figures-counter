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
 * @warning Currently this class works only with BMP files with 8 bit color depth and no compression. Check 
 */
class bmp_figures_counter final {
public:
	/**
	 * @brief Initializes the counter
	 *
	 * @param[in] bmp The BMP file with the figures
	 *
	 * @warning Modifying the file in @p bmp before this method finishes execution will cause undefined behavior.
	 *
	 * @throws error::bad_input If there's an error while reading the file. Check @link bmp_line_loader for more info on the errors.
	 */

	bmp_figures_counter(const std::filesystem::path& bmp);

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
	/**
	 * @brief The figure ID for background used when determining neighbor figure IDs.
	 */
	static constexpr unsigned EMPTY_FIGURE_ID = UINT_MAX;
	static constexpr std::byte BACKGROUND{0xFF};

	void assign_figure_ids_for_line(std::span<std::byte> buffer);
	unsigned get_figure_id_from_neighbors(unsigned left_cell, unsigned up_cell);
	size_t count_unique_figure_ids();

	bmp_line_loader lines_stream;
	std::vector<unsigned> prev_line;
	std::vector<unsigned> current_line;
	disjoint_set figure_ids;
};

} // namespace figures_counter