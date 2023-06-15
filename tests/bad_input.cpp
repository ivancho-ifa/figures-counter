#include <error.h>
#include <figures-counter.h>

#include <boost/test/unit_test.hpp>

#include <sstream>

BOOST_AUTO_TEST_SUITE(BadInput)

BOOST_AUTO_TEST_CASE(EmptyInput) {
	const std::string m = "\n";
	std::stringstream m_stream(m);

	BOOST_REQUIRE_THROW(figures_counter::count_figures(0, 0, m_stream), figures_counter::error::bad_input);
}

BOOST_AUTO_TEST_CASE(InvalidRowCount) {
	const std::string m = "*\n";
	std::stringstream m_stream(m);

	BOOST_REQUIRE_THROW(figures_counter::count_figures(0, 1, m_stream), figures_counter::error::bad_input);
}

BOOST_AUTO_TEST_CASE(InvalidColCount) {
	const std::string m = "*\n";
	std::stringstream m_stream(m);

	BOOST_REQUIRE_THROW(figures_counter::count_figures(1, 0, m_stream), figures_counter::error::bad_input);
}

BOOST_AUTO_TEST_CASE(InvalidData) {
	const std::string m = "*\n";
	std::stringstream m_stream(m);
	std::string m_stream_content;
	// Put the stream to bad state by reading all its contents
	std::getline(m_stream, m_stream_content);

	BOOST_REQUIRE_THROW(figures_counter::count_figures(1, 1, m_stream), figures_counter::error::bad_input);
}

BOOST_AUTO_TEST_CASE(BadRowLength) {
	const std::string m = " * \n";
	std::stringstream m_stream(m);

	BOOST_REQUIRE_THROW(figures_counter::count_figures(1, 2, m_stream), figures_counter::error::row_length_error);
}

BOOST_AUTO_TEST_CASE(BadRowCount) {
	const std::string m = " * \n"
						  " * \n";
	std::stringstream m_stream(m);

	BOOST_REQUIRE_THROW(figures_counter::count_figures(3, 3, m_stream), figures_counter::error::row_count_error);
}

BOOST_AUTO_TEST_CASE(UnrecognizedSymbol) {
	const std::string m = " 8 \n";
	std::stringstream m_stream(m);

	BOOST_REQUIRE_THROW(figures_counter::count_figures(1, 3, m_stream), figures_counter::error::unrecognized_symbol);
}

BOOST_AUTO_TEST_SUITE_END()