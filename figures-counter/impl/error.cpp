#include <error.h>
#include <format>

namespace figures_counter {
namespace error {

figures_counter_exception::figures_counter_exception(const std::string& what) :
	invalid_argument(std::format("figures counter error: {}", what)) {
}

bad_input::bad_input(const std::string& what) :
	figures_counter_exception(std::format("bad input: {}", what)) {
}

row_length_error::row_length_error(size_t row, size_t length, size_t expected_length) :
	bad_input(std::format("row {} with length {}, expected length {}", row, length, expected_length)),
	_row(row),
	_length(length),
	_expected_length(expected_length) {
}

size_t row_length_error::row() const noexcept {
	return _row;
}

size_t row_length_error::length() const noexcept {
	return _length;
}

size_t row_length_error::expected_length() const noexcept {
	return _expected_length;
}

row_count_error::row_count_error(size_t count, size_t expected_count) :
	bad_input(std::format("row count is {}, expected count {}", count, expected_count)),
	_count(count),
	_expected_count(expected_count) {
}

size_t row_count_error::count() const noexcept {
	return _count;
}

size_t row_count_error::expected_count() const noexcept {
	return _expected_count;
}

unrecognized_symbol::unrecognized_symbol(size_t row, size_t col, char symbol) :
	bad_input(std::format("received unrecognized char \"{}\" at [row = {}, col = {}]", symbol, row, col)),
	_row(row),
	_col(col),
	_symbol(symbol) {
}

size_t unrecognized_symbol::row() const noexcept {
	return _row;
}

size_t unrecognized_symbol::col() const noexcept {
	return _col;
}

char unrecognized_symbol::symbol() const noexcept {
	return _symbol;
}

} // namespace error
} // namespace figures_counter