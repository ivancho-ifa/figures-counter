#include <error.h>
#include <bmp_line_loader.h>

#include <boost/test/unit_test.hpp>

#include <sstream>

BOOST_AUTO_TEST_SUITE(BmpLineLoader)

BOOST_AUTO_TEST_CASE(Create) {
	const figures_counter::bmp_line_loader line_loader("");
}

BOOST_AUTO_TEST_SUITE_END()