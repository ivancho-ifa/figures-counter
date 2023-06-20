#include <figures-counter.h>

#include <bmp_figures_counter.h>

#include <limits.h>

namespace figures_counter {

size_t count_figures(const std::filesystem::path& bmp) {
	bmp_figures_counter counter(bmp);
	return counter.count_figures();
}

bool operator==(char lhs, Cell rhs) noexcept {
	return lhs == static_cast<char>(rhs);
}

} // namespace figures_counter