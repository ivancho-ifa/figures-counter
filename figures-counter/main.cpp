#include "figures-counter.h"

#include <cstdlib>
#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
	int rows;
	std::cin >> rows;

	int cols;
	std::cin >> cols;

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	std::cout << figures_counter::count_figures(rows, cols, std::cin) << std::endl;

	return EXIT_SUCCESS;
}
