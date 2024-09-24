cmake_minimum_required(VERSION 3.10)
project(Lab1)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find OpenCV package
set(OpenCV_DIR third-party/opencv/build/install/lib/cmake/opencv4)
find_package(OpenCV REQUIRED)

# Define the executable for your project
add_executable(Lab1 source/main.cpp)

# Include OpenCV headers and link libraries
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(Lab1 ${OpenCV_LIBS})
