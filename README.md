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