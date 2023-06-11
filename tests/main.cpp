#define BOOST_TEST_MODULE FiguresCounterTests

#include <figures-counter.h>

#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test.hpp>

#include <sstream>

BOOST_AUTO_TEST_CASE(MatrixCreate) {
	const std::string m = 
		"*     ***     \n"
		"     ** **    \n"
		"    **   **   \n"
		"    *     *   \n"
		"    **   **   \n"
		"     ** **    \n"
		"      ***     \n"; 
	std::stringstream m_stream(m);
	
	BOOST_REQUIRE_EQUAL(figures_counter::count_figures(7, 14, m_stream), 2);
}