#include <sequential_figures_counter.h>

#include <error.h>
#include <figures-counter.h>

#include <algorithm>

namespace figures_counter {

sequential_figures_counter::sequential_figures_counter(size_t rows, size_t cols, std::istream& data) :
	lines_stream(data, rows, cols),
	current_line(cols, EMPTY_FIGURE_ID) {
	prev_line.reserve(cols);
}

size_t sequential_figures_counter::count_figures() {
	while (lines_stream.not_finished()) {
		std::swap(current_line, prev_line);
		current_line.clear();
		std::string_view buffer = lines_stream.load_line();

		assign_figure_ids_for_line(buffer);
	}

	return count_unique_figure_ids();
}

void sequential_figures_counter::assign_figure_ids_for_line(std::string_view buffer) {
	// Cache the left cell in to skip check whether the current cell is 0
	unsigned left_cell = EMPTY_FIGURE_ID;

	for (size_t col = 0; col < buffer.size(); ++col) {
		const auto cell = buffer[col];

		if (cell == static_cast<char>(Cell::Background)) {
			left_cell = EMPTY_FIGURE_ID;
		} else if (cell == Cell::Figure) {
			const unsigned up_cell = prev_line[col];
			left_cell = get_figure_id_from_neighbors(left_cell, up_cell);
		} else [[unlikely]] {
			throw error::unrecognized_symbol(lines_stream.lines_loaded(), col, cell);
		}

		current_line.push_back(left_cell);
	}
}

unsigned sequential_figures_counter::get_figure_id_from_neighbors(unsigned left_cell, unsigned up_cell) {
	if (left_cell == EMPTY_FIGURE_ID && up_cell == EMPTY_FIGURE_ID) { ///< Both are empty
		const unsigned figure_id = static_cast<unsigned>(figure_ids.size());
		figure_ids.push_back(figure_id);

		return figure_id;
	} else if (left_cell == up_cell) { ///< Both are part of the same figure
		return left_cell;
	} else { ///< They differ
		const auto sorted = std::minmax(left_cell, up_cell);
		if (left_cell != EMPTY_FIGURE_ID && up_cell != EMPTY_FIGURE_ID) {
			figure_ids[sorted.second] = figure_ids[sorted.first];
		}

		return sorted.first;
	}
}

size_t sequential_figures_counter::count_unique_figure_ids() {
	std::sort(std::begin(figure_ids), std::end(figure_ids));
	const auto unique_ids_end = std::unique(std::begin(figure_ids), std::end(figure_ids));

	const size_t figures_count = unique_ids_end - std::begin(figure_ids);
	return figures_count;
}

} // namespace figures_counter