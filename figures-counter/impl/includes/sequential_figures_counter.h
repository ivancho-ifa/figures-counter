#pragma once

#include <line_loader.h>

#include <istream>
#include <string_view>
#include <vector>

namespace figures_counter {

/**
 * @brief Utility to calculate the number of figures in @p data
 *
 * Counts figures by assigning IDs to the figures line by line and then identifying duplicate IDs
 */

class sequential_figures_counter final {
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

	sequential_figures_counter(size_t rows, size_t cols, std::istream& data);

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

	void assign_figure_ids_for_line(std::string_view buffer);
	unsigned get_figure_id_from_neighbors(unsigned left_cell, unsigned up_cell);
	size_t count_unique_figure_ids();

	line_loader lines_stream;
	std::vector<unsigned> prev_line;
	std::vector<unsigned> current_line;
	std::vector<unsigned> figure_ids;
};

} // namespace figures_counter