#include <bmp.h>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(Bmp)

BOOST_AUTO_TEST_CASE(ReadBmpHeaders) {
	figures_counter::bmp_file_header file_header;
	figures_counter::bmp_info_header info_header;
	std::ifstream bmp_file("1080p.bmp", std::ifstream::binary | std::ifstream::in);
	BOOST_REQUIRE_NO_THROW(figures_counter::read_bmp_headers(bmp_file, file_header, info_header));

	BOOST_REQUIRE_EQUAL(strncmp(file_header.id, "BM", 2), 0);
	BOOST_REQUIRE_EQUAL(file_header.file_size, 2074678);
	BOOST_REQUIRE_EQUAL(file_header.pixel_array_offset, 1078);
}

BOOST_AUTO_TEST_SUITE_END()