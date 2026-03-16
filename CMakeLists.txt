cmake_minimum_required(VERSION 3.10)
project(StockDataPlot)

# Use brew-installed GCC
set(CMAKE_C_COMPILER "/home/linuxbrew/.linuxbrew/opt/gcc/bin/gcc-15")
set(CMAKE_CXX_COMPILER "/home/linuxbrew/.linuxbrew/opt/gcc/bin/g++-15")

# Set C++ standard to 23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set matplot++ paths
set(MATPLOT_INCLUDE_DIR "/home/linuxbrew/.linuxbrew/Cellar/matplotplusplus/1.2.2/include")
set(MATPLOT_LIBRARY_DIR "/home/linuxbrew/.linuxbrew/Cellar/matplotplusplus/1.2.2/lib")

# Add executable
add_executable(matplot_lib
    src/main.cpp
    src/plotter.cpp
    src/parser.cpp
    src/stock.cpp
    src/analysis.cpp
)

# Include directories and link libraries
target_include_directories(matplot_lib PRIVATE ${MATPLOT_INCLUDE_DIR} include)
target_link_directories(matplot_lib PRIVATE ${MATPLOT_LIBRARY_DIR})
target_link_libraries(matplot_lib
    PRIVATE
    matplot
    m  # Link the math library
    stdc++  # Link the C++ standard library
)

# Set rpath to find the library at runtime
set_target_properties(matplot_lib PROPERTIES
    INSTALL_RPATH "${MATPLOT_LIBRARY_DIR}"
    BUILD_RPATH "${MATPLOT_LIBRARY_DIR}"
)

# Add linker flags for brew-installed GCC
set(CMAKE_EXE_LINKER_FLAGS "-L/home/linuxbrew/.linuxbrew/lib/gcc/current")

