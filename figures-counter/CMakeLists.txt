project (figures-counter)

set (PROJECT_LIB ${PROJECT_NAME}-lib)
set (PROJECT_EXECUTABLE ${PROJECT_NAME})

# Compile to a static library
add_library (${PROJECT_LIB} STATIC 
	impl/figures-counter.cpp
	impl/error.cpp
	"impl/internal/line_loader.cpp"
	"impl/internal/sequential_figures_counter.cpp"
)
target_include_directories(${PROJECT_LIB} PUBLIC 
	${PROJECT_SOURCE_DIR}/includes
)
target_include_directories(${PROJECT_LIB} PRIVATE 
	${PROJECT_SOURCE_DIR}/impl/includes
)

# Compile to an executable
add_executable (${PROJECT_EXECUTABLE}
	main.cpp
)
target_link_libraries (${PROJECT_EXECUTABLE}
	${PROJECT_LIB}
)