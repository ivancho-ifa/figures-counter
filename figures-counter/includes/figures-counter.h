#pragma once

#include <istream>

namespace figures_counter {

/**
 * @brief The accepted input values for a cell of the input table
 */

enum class Cell : char {
	Background = ' ',
	Figure = '*'
};

/**
 * @return Whether @p rhs underlying implementation value is @p lhs
 */

bool operator==(char lhs, Cell rhs) noexcept;

/**
 * @brief Returns the number of figures in @p data
 *
 * The input is a table with the contents of @p data, formatted in size @p rows by @p cols. The table's contents @p data
 * consists of only two characters - @p EMPTY_CELL and @p FULL_CELL. A figure is consisting of one or more cells with
 * value @p FULL_CELL that share a side. In example 1 there is one figure and in example 2 there are 2 figures.
 *
 * Ex. 1          Ex. 2
 * | * |          |*  |
 * | * |          | * |
 * |   |          |   |
 *
 * @param rows Rows in the table. A positive integer.
 * @param cols Columns in the table. A positive integer.
 * @param data Contents of the table, formatted in @p rows rows with length @p cols. A readable data stream holding @p
 * rows new line separated rows with @p cols length each.
 *
 * @return The number of figures in the input.
 *
 * @throws error::row_count_error If @p rows is bigger than the actual rows in @p data
 * @throws error::row_length_error If one of the rows length differs from @p cols
 * @throws error::unrecognized_symbol If a symbol from @p data is different from the ones defined in @p Cell
 * @throws error::bad_input If any of the input parameters is not in the described format
 */

int count_figures(size_t rows, size_t cols, std::istream& data);

} // namespace figures_counter