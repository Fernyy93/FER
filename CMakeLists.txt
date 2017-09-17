cmake_minimum_required(VERSION 3.5)
set (CMAKE_CXX_STANDARD 11)
project( lbp )
find_package( OpenCV REQUIRED )
include_directories(include)
add_executable( lbp src/lbp.cpp src/lbp_main.cpp include/lbp.h)
target_link_libraries( lbp ${OpenCV_LIBS} ${include})