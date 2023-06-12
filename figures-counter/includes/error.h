#pragma once

#include <format>
#include <stdexcept>
#include <string>

namespace figures_counter {
namespace error {

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

} // namespace error
} // namespace figures_counter