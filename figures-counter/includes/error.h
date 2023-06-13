#pragma once

#include <stdexcept>
#include <string>

namespace figures_counter {
namespace error {

class figures_counter_exception : public std::invalid_argument {
public:
	figures_counter_exception(const std::string& what);
};

class bad_input : public figures_counter_exception {
public:
	bad_input(const std::string& what);
};

class row_length_error : public bad_input {
public:
	row_length_error(size_t row, size_t length, size_t expected_length);

	size_t row() const noexcept;
	size_t length() const noexcept;
	size_t expected_length() const noexcept;

private:
	size_t _row;
	size_t _length;
	size_t _expected_length;
};

class row_count_error : public bad_input {
public:
	row_count_error(size_t count, size_t expected_count);

	size_t count() const noexcept;
	size_t expected_count() const noexcept;

private:
	size_t _count;
	size_t _expected_count;
};

class unrecognized_symbol : public bad_input {
public:
	unrecognized_symbol(size_t row, size_t col, char symbol);

	size_t row() const noexcept;
	size_t col() const noexcept;
	char symbol() const noexcept;

private:
	size_t _row;
	size_t _col;
	char _symbol;
};

} // namespace error
} // namespace figures_counter