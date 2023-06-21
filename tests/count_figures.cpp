#include <figures_counter.h>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(CountFigures)

BOOST_AUTO_TEST_SUITE(BmpFiguresCounter)

BOOST_AUTO_TEST_CASE(CountFigures1080p) {
	BOOST_REQUIRE_EQUAL(figures_counter::count_figures("1080p.bmp"), 6);
}

BOOST_AUTO_TEST_CASE(CountFigures4k) {
	BOOST_REQUIRE_EQUAL(figures_counter::count_figures("4k.bmp"), 1);
}

BOOST_AUTO_TEST_CASE(CountFigures8k) {
	BOOST_REQUIRE_EQUAL(figures_counter::count_figures("8k.bmp"), 5);
}

BOOST_AUTO_TEST_CASE(Assert4Connectivity) {
	BOOST_CHECK_EQUAL(figures_counter::count_figures("1-figure-4-connectivity.bmp"), 1);
	BOOST_CHECK_EQUAL(figures_counter::count_figures("2-figures-4-connectivity.bmp"), 2);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()