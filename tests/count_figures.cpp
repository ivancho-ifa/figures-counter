#include <figures_counter.h>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(CountFigures)

BOOST_AUTO_TEST_SUITE(BmpFiguresCounter)

BOOST_AUTO_TEST_CASE(CircularFigureWithRetractions) {
	BOOST_REQUIRE_EQUAL(figures_counter::count_figures("1080p.bmp"), 6);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()