#pragma once

#include <algorithm>
#include <cassert>
#include <format>
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

bool operator==(char lhs, Cell rhs) noexcept {
	return lhs == static_cast<char>(rhs);
}

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

int count_figures(int rows, int cols, std::istream& data) {
	std::string buffer;
	buffer.reserve(cols);

	std::vector<unsigned> prev_row;
	prev_row.reserve(cols);

	std::vector<unsigned> current_row(cols, EMPTY_FIGURE_ID);

	std::vector<unsigned> figure_ids;

	const auto load_next_row = [&]() -> std::istream& {
		std::swap(current_row, prev_row);
		current_row.clear();

		std::getline(data, buffer);
		if (buffer.size() != cols) {
			throw std::invalid_argument("shorter than expected buffer");
		}
		return data;
	};

	for (size_t row = 0; row < rows && load_next_row(); ++row) {
		// Cache the left cell in to skip check whether the current cell is 0
		unsigned left_cell_figure = EMPTY_FIGURE_ID;

		for (size_t col = 0; col < buffer.size(); ++col) {
			const auto cell = buffer[col];

			if (cell == static_cast<char>(Cell::Background)) {
				current_row.push_back(left_cell_figure = EMPTY_FIGURE_ID);
			} else if (cell == Cell::Figure) {
				const unsigned up_cell_figure = prev_row[col];

				if (left_cell_figure == EMPTY_FIGURE_ID && up_cell_figure == EMPTY_FIGURE_ID) { ///< Both are empty
					const unsigned figure_id = figure_ids.size();
					figure_ids.push_back(figure_id);
					current_row.push_back(left_cell_figure = figure_id);
				} else if (left_cell_figure == up_cell_figure) { ///< Both are part of the same figure
					current_row.push_back(left_cell_figure);
				} else { ///< They differ
					const auto sorted = std::minmax(left_cell_figure, up_cell_figure);
					if (left_cell_figure != EMPTY_FIGURE_ID && up_cell_figure != EMPTY_FIGURE_ID) {
						figure_ids[sorted.second] = figure_ids[sorted.first];
					}

					current_row.push_back(left_cell_figure = sorted.first);
				}
			} else [[unlikely]] {
				throw std::invalid_argument("unrecognized symbol");
			}
		}

		if (current_row.size() != prev_row.size()) {
			throw std::logic_error(
				std::format("current_row.size() is {}, prev_row.size() is {}", current_row.size(), prev_row.size()));
		}
	}

	std::sort(std::begin(figure_ids), std::end(figure_ids));
	const int figures_count = std::unique(std::begin(figure_ids), std::end(figure_ids)) - std::begin(figure_ids);
	return figures_count;
}

} // namespace figures_counter