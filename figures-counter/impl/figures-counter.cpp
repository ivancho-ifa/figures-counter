#include <figures-counter.h>

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
 * @brief The figure ID for background used when determining neighbor figure IDs.
 */

constexpr unsigned EMPTY_FIGURE_ID = UINT_MAX;

bool operator==(char lhs, Cell rhs) noexcept {
	return lhs == static_cast<char>(rhs);
}

int count_figures(size_t rows, size_t cols, std::istream& data) {
	if (rows <= 0) {
		throw error::bad_input("rows must be positive");
	}
	if (cols <= 0) {
		throw error::bad_input("cols must be positive");
	}
	if (!data) {
		throw error::bad_input("data stream can not be read");
	}

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
		return data;
	};

	for (size_t row = 0; row < rows; ++row) {
		load_next_row();
		if (!data) {
			throw error::row_count_error(row, rows);
		}
		if (buffer.size() != cols) {
			throw error::row_length_error(row, buffer.size() - 1, cols);
		}

		// Cache the left cell in to skip check whether the current cell is 0
		unsigned left_cell_figure = EMPTY_FIGURE_ID;

		for (size_t col = 0; col < buffer.size(); ++col) {
			const auto cell = buffer[col];

			if (cell == static_cast<char>(Cell::Background)) {
				current_row.push_back(left_cell_figure = EMPTY_FIGURE_ID);
			} else if (cell == Cell::Figure) {
				const unsigned up_cell_figure = prev_row[col];

				if (left_cell_figure == EMPTY_FIGURE_ID && up_cell_figure == EMPTY_FIGURE_ID) { ///< Both are empty
					const unsigned figure_id = static_cast<unsigned>(figure_ids.size());
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
				throw error::unrecognized_symbol(row, col, cell);
			}
		}
	}

	std::sort(std::begin(figure_ids), std::end(figure_ids));
	const auto unique_ids_end = std::unique(std::begin(figure_ids), std::end(figure_ids));

	const int figures_count = static_cast<int>(unique_ids_end - std::begin(figure_ids));
	assert(figures_count > 0);
	return figures_count;
}

} // namespace figures_counter