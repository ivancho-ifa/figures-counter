#include <figures-counter.h>

#include <sequential_figures_counter.h>

#include <limits.h>

namespace figures_counter {

size_t count_figures(size_t rows, size_t cols, std::istream& data) {
	sequential_figures_counter counter(rows, cols, data);
	return counter.count_figures();
}

bool operator==(char lhs, Cell rhs) noexcept {
	return lhs == static_cast<char>(rhs);
}

} // namespace figures_counter