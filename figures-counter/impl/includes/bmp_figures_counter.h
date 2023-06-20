#pragma once

#include <bmp_line_loader.h>
#include <disjoint_set.h>

#include <istream>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace figures_counter {

class bmp_figures_counter final {
public:
	/**
	 * @brief Initializes the state of the counter.
	 *
	 * The input is a table with the contents of @p data, formatted in size @p rows by @p cols. The table's contents
	 * @p data consists of only two characters - @p EMPTY_CELL and @p FULL_CELL. A figure is consisting of one or more
	 * cells with value @p FULL_CELL that share a side. In example 1 there is one figure and in example 2 there are 2
	 * figures.
	 *
	 * Ex. 1          Ex. 2
	 * | * |          |*  |
	 * | * |          | * |
	 * |   |          |   |
	 *
	 * @param rows Rows in the table. A positive integer.
	 * @param cols Columns in the table. A positive integer.
	 * @param data Contents of the table, formatted in @p rows rows with length @p cols. A readable data stream holding
	 * @p rows new line separated rows with @p cols length each.
	 *
	 * @warning Modifying the object referenced by @p data before @p count_figures finishes execution will cause
	 * undefined behavior
	 *
	 * @throws error::bad_input If any of the input parameters is not in the described format
	 */

	bmp_figures_counter(const std::filesystem::path& bmp);

	/**
	 * Counts the figures in @p data
	 *
	 * @warning This method should be called only once. Calling it multiple times on the same object will cause
	 * undefined behavior.
	 *
	 * @return The number of figures in the input.
	 *
	 * @throws error::row_count_error If @p rows is bigger than the actual rows in @p data
	 * @throws error::row_length_error If one of the rows length differs from @p cols
	 * @throws error::unrecognized_symbol If a symbol from @p data is different from the ones defined in @p Cell
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