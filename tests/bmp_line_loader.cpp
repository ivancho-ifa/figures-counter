#include <bmp_line_loader.h>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(BmpLineLoader)

BOOST_AUTO_TEST_CASE(LoadBmpFile) {
	BOOST_REQUIRE_NO_THROW(figures_counter::bmp_line_loader("1080p.bmp"));
}

BOOST_AUTO_TEST_CASE(ReadWidthAndHeight) {
	figures_counter::bmp_line_loader bmp("1080p.bmp");

	BOOST_REQUIRE(bmp.not_finished());
	BOOST_REQUIRE_EQUAL(bmp.line_length(), 1920);
	BOOST_REQUIRE_EQUAL(bmp.lines_count(), 1080);
}

BOOST_AUTO_TEST_CASE(LoadLine) {
	figures_counter::bmp_line_loader bmp("1080p.bmp");

	const auto line = bmp.load_line();
	BOOST_REQUIRE_EQUAL(line.size(), bmp.line_length());
}

BOOST_AUTO_TEST_CASE(LoadAllLines) {
	figures_counter::bmp_line_loader bmp("1080p.bmp");

	while (bmp.not_finished()) {
		BOOST_REQUIRE_NE(bmp.lines_loaded(), bmp.lines_count());
		[[maybe_unused]] const std::span<std::byte> buffer = bmp.load_line();
	}

	BOOST_REQUIRE_EQUAL(bmp.lines_loaded(), bmp.lines_count());
}

BOOST_AUTO_TEST_CASE(Reset) {
	figures_counter::bmp_line_loader bmp("1080p.bmp");

	while (bmp.not_finished()) {
		[[maybe_unused]] const std::span<std::byte> buffer = bmp.load_line();
	}

	BOOST_CHECK_EQUAL(bmp.lines_loaded(), bmp.lines_count());

	const auto old_lines_count = bmp.lines_count();
	const auto old_line_length = bmp.line_length();

	BOOST_REQUIRE_NO_THROW(bmp.reset());
	BOOST_CHECK(bmp.not_finished());
	BOOST_CHECK_EQUAL(bmp.lines_count(), old_lines_count);
	BOOST_CHECK_EQUAL(bmp.line_length(), old_line_length);

	while (bmp.not_finished()) {
		BOOST_REQUIRE_NE(bmp.lines_loaded(), bmp.lines_count());
		[[maybe_unused]] const std::span<std::byte> buffer = bmp.load_line();
	}

	BOOST_REQUIRE_EQUAL(bmp.lines_loaded(), bmp.lines_count());
}

BOOST_AUTO_TEST_SUITE_END()