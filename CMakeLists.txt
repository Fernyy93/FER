cmake_minimum_required(VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set (CMAKE_CXX_STANDARD 17)
project( "FER2.0" )
find_package( OpenCV REQUIRED )
include_directories(include ${OpenCV_INCLUDE_DIRS} "C:/Users/ferni/Documents/dirent-1.24/include")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_executable( FER2.0 src/lbp.cpp src/lbp_main.cpp src/image.cpp include/image.h include/lbp.h)
#add_executable( writejson src/writejson.cpp src/image.cpp include/image.h)

target_link_libraries( FER2.0 ${OpenCV_LIBS})
#target_link_libraries( writejson ${OpenCV_LIBS})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET FER2.0 PROPERTY CXX_STANDARD 20)
endif()