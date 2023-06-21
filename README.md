# Figures Counter

A library and command line utility to count figures in BMP images. Whether two pixels are connected is determined by their relative position - if we look at pixels like little squares, the connected ones are those that share a side. If they are diagonally positioned the pixels are not connected.

## Overview

### Structure of the project

The project consists of three parts - static library for counting figures, command line utility that uses the library to count figures and tests.

#### The library

Keeping simple the public interface the library consists of a single method that accepts a file path and optionally a background color and returns the count of the figures in it or throws error. The figure counting is done in a single iteration through the pixels data of the image, assigning an ID to every pixel based on checking its left and up neighbors:

- if both are part of the background a new ID is assigned to the pixel
- if only one of the neighbors is set, the pixel is assigned the ID of its non-background neighbor
- if both have the same ID, the pixel is assigned the ID of its neighbors
- if both have IDs (they are not part of the background) but they are different, we mark both IDs as equivalent and assign the pixel the ID of the pixel that's part of the bigger figure

In order to efficiently manage the equivalent IDs [disjoint set](https://en.wikipedia.org/wiki/Disjoint-set_data_structure) is used. The space complexity of this particular implemnetation of disjoint set is linear and the time complexity is <em>O(α(n))</em> which is the extremely slow-growing inverse Ackerman function.

To further optimize the space complexity, while iterating through the image only two lines of the image are being kept in memory.

The source code is logically split in line loader module and figure counter. It's taken care that the public interface remain simple and implementation headers are not available to the consumers.

#### The command line utility

Using Boost.Program_options the utility can provide help message or count the figures in an BMP image. User can also specify verbosity of the output.

#### The tests

Boost.Test unittests that test each internal component of the library and the public interface.

### Technologies used

The library and utility are built with C++ and CMake. Library dependecies are handled by vcpkg. To ensure quality unit tests are written in the Boost.Test framework. Every component of the library is tested. For handling command line parameters Boost.Program_options are used.

### Continuous integration and automatic testing

Several workflows are running on every pull request using GitHub Actions

- clang-tidy and MSVC Code Analysis statically check for errors
- clang-format checks for following unified style throughout the codebase
- doxygen documentation is being generated from the comments in code and is uploaded to GitHub Pages
- vcpkg library dependencies are cached so they are not downloaded on every run
- builds of all supported compilers, platforms and architectures
- all unit tests are run

## Future perspectives

I've tracked the remaining work in the issues of the project.

The most important improvement of the performance of the project would be efficient reading of the file in memory. After that we can split image in several stripes of memory and iterate each stripe in a new thread and then find which figure IDs are equivalent between neighboring stripes.

In terms of usability probably the best thing would be to include multiple image types. Another great feature for usability would be translation of the messages that reach the user.

To ensure the quality of the application a continuos benchmark should be provided. Profiling the code can point to the bottlenecks which can guide to what part of the code should be improved in next iterations. I ran MSVC's profiler locally and found no serious drops in performance. I learned about an interesting testing technique that I'm eager to try - fuzzy testing - providing unexpected random inputs at the software to test how stable it is. Memory checker should ensure what might have been missed in terms of memory management.

## Running the project

This project uses CMake to generate build files and vcpkg for dependencies. It is tested only on Windows 10 using Visual Studio 2022 and the vcpkg and CMake distributions coming with it. You can learn how to work with a CMake project in Visual Studio from [this article](https://learn.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=msvc-170)

### Prerequisites

- Visual Studio 2022 with "vcpkg package manager" and "C++ CMake tools for Windows" installed as individual components. You can learn how to install individual components from [this article](https://learn.microsoft.com/en-us/visualstudio/install/modify-visual-studio?view=vs-2022)

### Building the project

1. Open the folder in Visual Studio 2022.
2. Build one of the targets.
   - figures-counter
   - figures-counter-tests

### Running the project

After the builds are done you can run the executables.

- out/build/x64-debug/figures-counter/figures-counter.exe is a command line application that you can use to count the figures in a file or from the input
- out/build/x64-debug/tests/figures-counter-tests.exe runs some predefined tests
