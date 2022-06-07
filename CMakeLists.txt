cmake_minimum_required(VERSION 3.13.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(graph_visualizer CXX)

if (MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -pedantic )
endif()

add_library(${PROJECT_NAME} )

target_include_directories(${PROJECT_NAME} PUBLIC src tests)

set(SUBDIRECTORIES external src tests)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()