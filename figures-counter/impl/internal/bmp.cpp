#include <bmp.h>

namespace figures_counter {

void read_bmp_headers(std::ifstream& file, bmp_file_header& file_header, bmp_info_header& info_header) {
	// Read the file header
	file.read(file_header.id, sizeof(file_header.id));
	file.read(reinterpret_cast<char*>(&file_header.file_size), sizeof(file_header.file_size));
	file.read(reinterpret_cast<char*>(&file_header.reserved), sizeof(file_header.reserved));
	file.read(reinterpret_cast<char*>(&file_header.pixel_array_offset), sizeof(file_header.pixel_array_offset));

	// Read the info header
	file.read(reinterpret_cast<char*>(&info_header.header_size), sizeof(info_header.header_size));
	file.read(reinterpret_cast<char*>(&info_header.width), sizeof(info_header.width));
	file.read(reinterpret_cast<char*>(&info_header.height), sizeof(info_header.height));
	file.read(reinterpret_cast<char*>(&info_header.color_panes), sizeof(info_header.color_panes));
	file.read(reinterpret_cast<char*>(&info_header.bpp), sizeof(info_header.bpp));
	file.read(reinterpret_cast<char*>(&info_header.compression), sizeof(info_header.compression));
	file.read(reinterpret_cast<char*>(&info_header.size), sizeof(info_header.size));
	file.read(reinterpret_cast<char*>(&info_header.horizontal_resolution), sizeof(info_header.horizontal_resolution));
	file.read(reinterpret_cast<char*>(&info_header.vertical_resolution), sizeof(info_header.vertical_resolution));
	file.read(reinterpret_cast<char*>(&info_header.color_palette), sizeof(info_header.color_palette));
	file.read(reinterpret_cast<char*>(&info_header.important_colors), sizeof(info_header.important_colors));
}

} // namespace figures_counter