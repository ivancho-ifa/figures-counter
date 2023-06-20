#include <error.h>
#include <figures_counter.h>

#include <boost/program_options.hpp>

#include <cstdlib>
#include <iostream>

int main(int argc, char* argv[]) {
	try {
		constexpr auto HELP = "help";
		constexpr auto BMP = "bmp";
		constexpr auto COUNT_ONLY = "count-only";

		boost::program_options::options_description options("Allowed options");
		// clang-format off
		options.add_options()
			(HELP, "Display this message")
			(BMP, boost::program_options::value<std::string>(), "Path to an uncompressed BMP file with 8 bits color depth")
			(COUNT_ONLY, "Output only the figures count, requires --bmp");
		// clang-format on

		boost::program_options::positional_options_description positional_options;
		positional_options.add(BMP, 1);

		boost::program_options::variables_map vars;
		boost::program_options::store(boost::program_options::command_line_parser(argc, argv)
		                                  .options(options)
		                                  .positional(positional_options)
		                                  .run(),
		                              vars);
		boost::program_options::notify(vars);

		// If there is --help passed anywhere on the args print the help message and do not proceed
		if (vars.count(HELP)) {
			std::cout << "Command line utility that counts figures in the specified input file"
					  << "\n"
					  << "\n";

			std::cout << options << "\n";
			return EXIT_SUCCESS;
		}

		// If there is BMP file specified attempt to count figures
		if (vars.count(BMP)) {
			const std::filesystem::path bmp(vars[BMP].as<std::string>());
			const size_t figures_count = figures_counter::count_figures(bmp);

			if (!vars.count(COUNT_ONLY)) {
				std::cout << bmp << " contains " << figures_count << " figures" << "\n";
			} else {
				std::cout << figures_count << "\n";
			}
			return EXIT_SUCCESS;
		}

		if (!vars.count(COUNT_ONLY) || !vars.empty()) {
			std::cout << options << "\n";
			return EXIT_FAILURE;
		} 
	} catch (const boost::program_options::error& e) {
		std::cerr << "Failed to parse command line arguments: " << e.what() << "\n";
	} catch (const figures_counter::error::bad_input& e) {
		std::cerr << "Failed to count figures, error occurred while reading input: " << e.what() << "\n";
	} catch (const std::exception& e) {
		std::cerr << "Failed to count figures, generic error occurred: " << e.what() << "\n";
	} catch (...) {
		std::cerr << "Failed to count figures, unexpected error occurred!\n";
	}

	return EXIT_FAILURE;
}
