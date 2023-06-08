project (figures-counter-tests)

set (TESTS_EXECUTABLE ${PROJECT_NAME})
set (PROJECT_LIB figures-counter-lib)

find_package(Boost REQUIRED COMPONENTS 
	unit_test_framework
)

# Compile a test executable
add_executable(${TESTS_EXECUTABLE} 
	main.cpp
)
target_link_libraries(${TESTS_EXECUTABLE} PRIVATE 
	${PROJECT_LIB}
	Boost::unit_test_framework
)

# Run the test executable
enable_testing()
add_test(NAME ${PROJECT_NAME} COMMAND ${TESTS_EXECUTABLE})