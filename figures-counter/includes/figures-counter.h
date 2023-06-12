#pragma once

#include "error.h"

#include <algorithm>
#include <cassert>
#include <istream>
#include <limits.h>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace figures_counter {

/**
 * @brief The accepted input values for a cell of the input table
 */

enum class Cell : char {
	Background = ' ',
	Figure = '*'
};

bool operator==(char lhs, Cell rhs) noexcept;

constexpr unsigned EMPTY_FIGURE_ID = UINT_MAX;

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
 * @param rows Rows in the table
 * @param cols Columns in the table
 * @param data Contents of the table, formatted in @p rows rows with length @p cols
 *
 * @return The number of figures in the input.
 */

int count_figures(int rows, int cols, std::istream& data);

} // namespace figures_counter