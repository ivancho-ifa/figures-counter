#pragma once

#include <fstream>

namespace figures_counter {

class bmp_file_header {
public:
	char id[2];
	uint32_t file_size; ///< In bytes
	uint32_t reserved;  ///< Reserved values, 0
	uint32_t pixel_array_offset;
};

class bmp_info_header {
public:
	uint32_t header_size;
	uint32_t width;
	uint32_t height;
	uint16_t color_panes;
	uint16_t bpp; ///< Bits per pixel
	uint32_t compression;
	uint32_t size;
	uint32_t horizontal_resolution;
	uint32_t vertical_resolution;
	uint32_t color_palette;
	uint32_t important_colors;
};

void read_bmp_headers(std::ifstream& file, bmp_file_header& file_header, bmp_info_header& info_header);

} // namespace figures_counter