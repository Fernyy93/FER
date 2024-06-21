cmake_minimum_required(VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_CXX_STANDARD 17)
project("FER2.0")

find_package(OpenCV REQUIRED)
include_directories(include ${OpenCV_INCLUDE_DIRS})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(writeyaml src/writeyaml.cpp src/Image.cpp include/image.h)
add_executable(feature_extraction src/feature_extraction.cpp src/Image.cpp include/image.h)
# add_executable(classification src/classification.cpp src/Image.cpp include/image.h)

target_link_libraries(writeyaml ${OpenCV_LIBS})
target_link_libraries(feature_extraction ${OpenCV_LIBS})
# target_link_libraries(classification ${OpenCV_LIBS})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET writeyaml PROPERTY CXX_STANDARD 20)
  
  set_property(TARGET feature_extraction PROPERTY CXX_STANDARD 20)
  # set_property(TARGET classification PROPERTY CXX_STANDARD 20)
endif()
