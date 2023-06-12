#define BOOST_TEST_MODULE FiguresCounterTests

#include <figures-counter.h>

#include <boost/test/unit_test.hpp>

#include <sstream>

BOOST_AUTO_TEST_SUITE(CountFigures)

BOOST_AUTO_TEST_CASE(SingleCellFigure) {
	const std::string m = "      *\n";
	std::stringstream m_stream(m);

	BOOST_REQUIRE_EQUAL(figures_counter::count_figures(1, 7, m_stream), 1);
}

BOOST_AUTO_TEST_CASE(SimpleFigure) {
	const std::string m = "      *\n"
						  "      *\n";
	std::stringstream m_stream(m);

	BOOST_REQUIRE_EQUAL(figures_counter::count_figures(2, 7, m_stream), 1);
}

BOOST_AUTO_TEST_CASE(CircularFigure) {
	const std::string m = "      ***     \n"
						  "     ** **    \n"
						  "    **   **   \n"
						  "    *     *   \n"
						  "    **   **   \n"
						  "     ** **    \n"
						  "      ***     \n";
	std::stringstream m_stream(m);

	BOOST_REQUIRE_EQUAL(figures_counter::count_figures(7, 14, m_stream), 1);
}

BOOST_AUTO_TEST_CASE(CircularFigureWithRetractions) {
	const std::string m = "*     ***     \n"
						  "     ** **    \n"
						  "    **   **   \n"
						  " ****     *   \n"
						  " *  **   **   \n"
						  "     ** **    \n"
						  "  *   ***     \n";
	std::stringstream m_stream(m);

	BOOST_REQUIRE_EQUAL(figures_counter::count_figures(7, 14, m_stream), 3);
}

BOOST_AUTO_TEST_CASE(BigFigureSparsed) {
	const unsigned rows = 1000;
	const unsigned cols = 1000;
	const unsigned figures_count = rows * cols / 4;

	std::string m;
	for (auto row = 0; row < rows; ++row) {
		std::string m_row;
		if (row % 2 == 0) {
			for (auto col = 0; col < cols; ++col) {
				m_row +=
					static_cast<char>(col % 2 == 0 ? figures_counter::Cell::Figure : figures_counter::Cell::Background);
			}
		} else {
			m_row = std::string(cols, static_cast<char>(figures_counter::Cell::Background));
		}
		m_row += "\n";
		m += m_row;
	}

	std::stringstream m_stream(m);

	BOOST_REQUIRE_EQUAL(figures_counter::count_figures(rows, cols, m_stream), figures_count);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(BadInput)

BOOST_AUTO_TEST_CASE(EmptyInput) {
	const std::string m = "\n";
	std::stringstream m_stream(m);

	BOOST_REQUIRE_THROW(figures_counter::count_figures(0, 0, m_stream), figures_counter::bad_input);
}

BOOST_AUTO_TEST_CASE(InvalidRowCount) {
	const std::string m = "*\n";
	std::stringstream m_stream(m);

	BOOST_REQUIRE_THROW(figures_counter::count_figures(-1, 1, m_stream), figures_counter::bad_input);
}

BOOST_AUTO_TEST_CASE(InvalidColCount) {
	const std::string m = "*\n";
	std::stringstream m_stream(m);

	BOOST_REQUIRE_THROW(figures_counter::count_figures(1, -1, m_stream), figures_counter::bad_input);
}

BOOST_AUTO_TEST_CASE(InvalidData) {
	const std::string m = "*\n";
	std::stringstream m_stream(m);
	std::string m_stream_content;
	// Put the stream to bad state by reading all its contents
	std::getline(m_stream, m_stream_content);

	BOOST_REQUIRE_THROW(figures_counter::count_figures(1, 1, m_stream), figures_counter::bad_input);
}

BOOST_AUTO_TEST_CASE(BadRowLength) {
	const std::string m = " * \n";
	std::stringstream m_stream(m);

	BOOST_REQUIRE_THROW(figures_counter::count_figures(1, 2, m_stream), figures_counter::row_length_error);
}

BOOST_AUTO_TEST_CASE(BadRowCount) {
	const std::string m = " * \n"
						  " * \n";
	std::stringstream m_stream(m);

	BOOST_REQUIRE_THROW(figures_counter::count_figures(3, 3, m_stream), figures_counter::row_count_error);
}

BOOST_AUTO_TEST_CASE(UnrecognizedSymbol) {
	const std::string m = " 8 \n";
	std::stringstream m_stream(m);

	BOOST_REQUIRE_THROW(figures_counter::count_figures(1, 3, m_stream), figures_counter::unrecognized_symbol);
}

BOOST_AUTO_TEST_SUITE_END()