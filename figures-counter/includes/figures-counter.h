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

class figures_counter_exception : public std::invalid_argument {
public:
	figures_counter_exception(const std::string& what) :
		invalid_argument(std::format("figures counter error: {}", what)) {
	}
};

class bad_input : public figures_counter_exception {
public:
	bad_input(const std::string& what) :
		figures_counter_exception(std::format("bad input: {}", what)) {
	}
};

class row_length_error : public bad_input {
public:
	row_length_error(unsigned row, unsigned length, unsigned expected_length) :
		bad_input(std::format("row {} with length {}, expected length {}", row, length, expected_length)),
		_row(row),
		_length(length),
		_expected_length(expected_length) {
	}

	unsigned row() const noexcept {
		return _row;
	}

	unsigned length() const noexcept {
		return _length;
	}

	unsigned expected_length() const noexcept {
		return _expected_length;
	}

private:
	unsigned _row;
	unsigned _length;
	unsigned _expected_length;
};

class row_count_error : public bad_input {
public:
	row_count_error(unsigned count, unsigned expected_count) :
		bad_input(std::format("row count is {}, expected count {}", count, expected_count)),
		_count(count),
		_expected_count(expected_count) {
	}

	unsigned count() const noexcept {
		return _count;
	}

	unsigned expected_count() const noexcept {
		return _expected_count;
	}

private:
	unsigned _count;
	unsigned _expected_count;
};

class unrecognized_symbol : public bad_input {
public:
	unrecognized_symbol(unsigned row, unsigned col, char symbol) :
		bad_input(std::format("received unrecognized char \"{}\" at [row = {}, col = {}]", symbol, row, col)),
		_row(row),
		_col(col),
		_symbol(symbol) {
	}

	unsigned row() const noexcept {
		return _row;
	}

	unsigned col() const noexcept {
		return _col;
	}

	char symbol() const noexcept {
		return _symbol;
	}

private:
	unsigned _row;
	unsigned _col;
	char _symbol;
};

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
	if (rows <= 0) {
		throw bad_input("rows must be positive");
	}
	if (cols <= 0) {
		throw bad_input("cols must be positive");
	}
	if (!data) {
		throw bad_input("data stream can not be read");
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
			throw row_count_error(row, rows);
		}
		if (buffer.size() != cols) {
			throw row_length_error(row, buffer.size() - 1, cols);
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
				throw unrecognized_symbol(row, col, cell);
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