#include <figures-counter.h>

#include <cstdlib>
#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {

	std::cout << figures_counter::count_figures(argv[1]) << std::endl;

	return EXIT_SUCCESS;
}
