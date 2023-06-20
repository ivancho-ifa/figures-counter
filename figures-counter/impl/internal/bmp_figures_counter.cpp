#include <sequential_figures_counter.h>

#include <error.h>
#include <figures-counter.h>

#include <algorithm>

namespace figures_counter {

bmp_figures_counter::bmp_figures_counter(const std::filesystem::path& bmp) :
	lines_stream(bmp),
	current_line(lines_stream.info_header().width, EMPTY_FIGURE_ID) {
	prev_line.reserve(lines_stream.info_header().width);
}

size_t bmp_figures_counter::count_figures() {
	while (lines_stream.not_finished()) {
		std::swap(current_line, prev_line);
		current_line.clear();
		const std::span<std::byte> buffer = lines_stream.load_line();

		assign_figure_ids_for_line(buffer);
	}

	return count_unique_figure_ids();
}

void bmp_figures_counter::assign_figure_ids_for_line(std::span<std::byte> buffer) {
	// Cache the left cell in to skip check whether the current cell is 0
	unsigned left_cell = EMPTY_FIGURE_ID;

	for (size_t col = 0; col < buffer.size(); ++col) {
		const auto cell = buffer[col];

		if (cell == BACKGROUND) {
			left_cell = EMPTY_FIGURE_ID;
		} else {
  			const unsigned up_cell = prev_line[col];
			left_cell = get_figure_id_from_neighbors(left_cell, up_cell);
		}

		current_line.push_back(left_cell);
	}
}

unsigned bmp_figures_counter::get_figure_id_from_neighbors(unsigned left_cell, unsigned up_cell) {
	// Both are empty
	if (left_cell == EMPTY_FIGURE_ID && up_cell == EMPTY_FIGURE_ID) {
		const auto figure_id = figure_ids.AddNew();
		return figure_id;
	}

	// Both are part of the same figure
	if (figure_ids.Find(left_cell) == figure_ids.Find(up_cell)) {
		return figure_ids.Find(left_cell);
	}

	// Part of different figures
	if (left_cell != EMPTY_FIGURE_ID && up_cell != EMPTY_FIGURE_ID) {
		return figure_ids.Union(left_cell, up_cell);
	}

	// One is background 
	const auto sorted = std::minmax(left_cell, up_cell);
	return sorted.first;
}

size_t bmp_figures_counter::count_unique_figure_ids() {
	return figure_ids.CountUnique();
}

} // namespace figures_counter