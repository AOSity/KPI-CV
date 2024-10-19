cmake_minimum_required(VERSION 3.10)
project(Lab1)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find OpenCV package
set(OpenCV_DIR third-party/opencv/build/install/lib/cmake/opencv4)
find_package(OpenCV REQUIRED)

# Define the executable for project
add_executable(ImageOpen source/image_open.cpp)
add_executable(ImageEqualizeHistogram source/image_histogram_equalization.cpp)
add_executable(ImageScaleConditional source/image_conditional_scaling.cpp)
add_executable(ImageApplyKernel source/image_apply_kernel.cpp)
add_executable(ImageSmooth source/image_smooth.cpp)
add_executable(ImageSharpen source/image_sharpen.cpp)
add_executable(ImageDetectEdges source/image_edge_detect.cpp)
add_executable(ImageDetectEdges2 source/image_edge_detect_2.cpp)

# Include OpenCV headers and link libraries
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(ImageOpen ${OpenCV_LIBS})
target_link_libraries(ImageEqualizeHistogram ${OpenCV_LIBS})
target_link_libraries(ImageScaleConditional ${OpenCV_LIBS})
target_link_libraries(ImageApplyKernel ${OpenCV_LIBS})
target_link_libraries(ImageSmooth ${OpenCV_LIBS})
target_link_libraries(ImageSharpen ${OpenCV_LIBS})
target_link_libraries(ImageDetectEdges ${OpenCV_LIBS})
target_link_libraries(ImageDetectEdges2 ${OpenCV_LIBS})
