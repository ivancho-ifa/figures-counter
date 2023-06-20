#include <bmp_figures_counter.h>

#include <error.h>
#include <figures-counter.h>

#include <algorithm>
#include <cassert>

namespace figures_counter {

bmp_figures_counter::bmp_figures_counter(const std::filesystem::path& bmp, std::byte background) :
	_bmp(bmp),
	_current_line(_bmp.line_length(), EMPTY_FIGURE_ID),
	_background(background) {
	_prev_line.reserve(_bmp.line_length());
}

size_t bmp_figures_counter::count_figures() {
	while (_bmp.not_finished()) {
		std::swap(_current_line, _prev_line);
		_current_line.clear();
		const std::span<std::byte> buffer = _bmp.load_line();

		assign_figure_ids_for_line(buffer);
	}

	return _figure_ids.count_unique();
}

void bmp_figures_counter::assign_figure_ids_for_line(std::span<std::byte> buffer) {
	// Cache the left cell in to skip check whether the current cell is 0
	size_t left_cell = EMPTY_FIGURE_ID;

	for (size_t col = 0; col < buffer.size(); ++col) {
		const auto cell = buffer[col];

		if (cell == _background) {
			left_cell = EMPTY_FIGURE_ID;
		} else {
			const auto up_cell = _prev_line[col];
			left_cell = get_figure_id_from_neighbors(left_cell, up_cell);
		}

		_current_line.push_back(left_cell);
	}
}

size_t bmp_figures_counter::get_figure_id_from_neighbors(size_t left_cell, size_t up_cell) {
	// Both are empty
	if (left_cell == EMPTY_FIGURE_ID && up_cell == EMPTY_FIGURE_ID) {
		const auto figure_id = _figure_ids.add_new();
		return figure_id;
	}
	assert(left_cell != EMPTY_FIGURE_ID || up_cell != EMPTY_FIGURE_ID);

	if (left_cell == EMPTY_FIGURE_ID || up_cell == EMPTY_FIGURE_ID) {
		const auto& [cell, background] = std::minmax(left_cell, up_cell);
		return cell;
	}
	assert(left_cell != EMPTY_FIGURE_ID && up_cell != EMPTY_FIGURE_ID);

	// Both are part of the same figure
	const auto left_parent = _figure_ids.find_parent(left_cell);
	const auto up_parent = _figure_ids.find_parent(up_cell);
	if (left_parent == up_parent) {
		return left_parent;
	}
	assert(left_parent != up_parent);

	// Part of different figures
	return _figure_ids.unite(left_cell, up_cell);
}

} // namespace figures_counter