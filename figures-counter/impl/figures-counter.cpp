#include <figures-counter.h>

#include <bmp_figures_counter.h>

#include <limits.h>

namespace figures_counter {

size_t count_figures(const std::filesystem::path& bmp) {
	bmp_figures_counter counter(bmp);
	return counter.count_figures();
}

} // namespace figures_counter