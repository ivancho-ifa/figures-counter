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
	const size_t rows = 1000;
	const size_t cols = 1000;
	const size_t figures_count = rows * cols / 4;

	std::string m;
	for (size_t row = 0; row < rows; ++row) {
		std::string m_row;
		if (row % 2 == 0) {
			for (size_t col = 0; col < cols; ++col) {
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