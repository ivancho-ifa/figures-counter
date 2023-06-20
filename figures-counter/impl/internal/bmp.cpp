#include <bmp.h>

#include <error.h>

#include <format>

namespace figures_counter {

void read_bmp_headers(std::ifstream& file, bmp_file_header& file_header, bmp_info_header& info_header) {
	// Read the headers field by field in order to work around structure packing in different architectures.

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

void validate_bmp_headers(const std::filesystem::path& file_path, std::ifstream& file, bmp_file_header& file_header,
                          bmp_info_header& info_header) {
	if (strncmp(file_header.id, "BM", 2) != 0) [[unlikely]] {
		throw error::bad_input(std::format("{} is of not supported type {}", file_path.string(), file_header.id));
	}

	if (info_header.bpp != 8) {
		throw error::bad_input(
			std::format("only 8 bpp is supported {} uses {} bpp", file_path.string(), info_header.bpp));
	}
	if (info_header.compression != 0) {
		throw error::bad_input(std::format("{} compression method is unsupported, only uncompressed BMPs are supported",
		                                   file_path.string()));
	}

	file.seekg(static_cast<std::streampos>(file_header.pixel_array_offset) + info_header.size);
	if (!file || file.tellg() == std::ifstream::traits_type::pos_type(-1)) {
		file.seekg(0, std::ifstream::end);
		const auto end = file.tellg();
		file.seekg(0, std::ifstream::beg);
		const auto begin = file.tellg();
		const auto size = end - begin;
		throw error::bad_input(std::format("{} with resolution {} * {} has insufficient pixels count {}",
		                                   file_path.string(), info_header.width, info_header.height, size));
	}
}

} // namespace figures_counter