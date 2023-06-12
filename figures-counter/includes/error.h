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
	row_length_error(unsigned row, unsigned length, unsigned expected_length);

	unsigned row() const noexcept;
	unsigned length() const noexcept;
	unsigned expected_length() const noexcept;

private:
	unsigned _row;
	unsigned _length;
	unsigned _expected_length;
};

class row_count_error : public bad_input {
public:
	row_count_error(unsigned count, unsigned expected_count);

	unsigned count() const noexcept;
	unsigned expected_count() const noexcept;

private:
	unsigned _count;
	unsigned _expected_count;
};

class unrecognized_symbol : public bad_input {
public:
	unrecognized_symbol(unsigned row, unsigned col, char symbol);

	unsigned row() const noexcept;
	unsigned col() const noexcept;
	char symbol() const noexcept;

private:
	unsigned _row;
	unsigned _col;
	char _symbol;
};

} // namespace error
} // namespace figures_counter