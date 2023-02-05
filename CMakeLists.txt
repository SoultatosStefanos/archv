# #####################################################################
# CMake misc variables
# #####################################################################
cmake_minimum_required(VERSION 3.13.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# #####################################################################
# Project name
# #####################################################################
project(archv CXX)

# #####################################################################
# Targets
# #####################################################################
set(PROJECT ${PROJECT_NAME})
set(PROJECT_LIB ${PROJECT}_lib)
set(PROJECT_APP ${PROJECT}_app)

# #####################################################################
# Compiler flags
# #####################################################################
if(MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -pedantic)
endif()

# #####################################################################
# Library
# #####################################################################
add_library(${PROJECT_LIB})

# #####################################################################
# Subdirectories
# #####################################################################
set(SUBDIRECTORIES external src tests)

foreach(VAR ${SUBDIRECTORIES})
  add_subdirectory(${VAR})
endforeach()

# #####################################################################
# Setup options
# #####################################################################
set(ARCHV_CONFIG_DIR "${CMAKE_SOURCE_DIR}/data/config")
set(ARCHV_RENDERING_CONFIG_PATH "${ARCHV_CONFIG_DIR}/rendering.json" CACHE STRING "The file path to the .json runtime configuration file for the rendering subsystem e.g. '/home/Descktop/archv/data/config/rendering.json'.")
set(ARCHV_GUI_CONFIG_PATH "${ARCHV_CONFIG_DIR}/gui.json" CACHE STRING "The file path to the .json runtime configuration file for the gui subsystem e.g. '/home/Descktop/archv/data/config/gui.json'.")
set(ARCHV_WEIGHTS_CONFIG_PATH "${ARCHV_CONFIG_DIR}/weights.json" CACHE STRING "The file path to the .json runtime configuration file for the edge weight management subsystem e.g. '/home/Descktop/archv/data/config/layout.json'.")
set(ARCHV_LAYOUT_CONFIG_PATH "${ARCHV_CONFIG_DIR}/layout.json" CACHE STRING "The file path to the .json runtime configuration file for the graph layout management subsystem e.g. '/home/Descktop/archv/data/config/layout.json'.")
set(ARCHV_SCALING_CONFIG_PATH "${ARCHV_CONFIG_DIR}/scaling.json" CACHE STRING "The file path to the .json runtime configuration file for the vertex property scaling management subsystem e.g. '/home/Descktop/archv/data/config/scaling.json'.")
set(ARCHV_CLUSTERING_CONFIG_PATH "${ARCHV_CONFIG_DIR}/clustering.json" CACHE STRING "The file path to the .json runtime configuration file for the graph clustering subsystem e.g. '/home/Descktop/archv/data/config/clustering.json'.")
set(ARCHV_COLOR_CODING_CONFIG_PATH "${ARCHV_CONFIG_DIR}/color_coding.json" CACHE STRING "The file path to the .json runtime configuration file for the graph color coding subsystem e.g. '/home/Descktop/archv/data/config/color_coding.json'.")
set(ARCHV_DEGREES_CONFIG_PATH "${ARCHV_CONFIG_DIR}/degrees.json" CACHE STRING "The file path to the .json runtime configuration file for the graph degrees particles subsystem e.g. '/home/Descktop/archv/data/config/degrees.json'.")
set(ARCHV_RESOURCE_GROUP "Archv" CACHE STRING "The media resource group used, e.g. 'Archv'.")

set(OGRE_PROCEDURAL_DIR "/usr/local/lib/OgreProcedural/cmake" CACHE STRING "The directory containing a CMake configuration file for the Ogre Procedural component.")

# #####################################################################
# Find 3rd party packages
# #####################################################################
find_package(Boost COMPONENTS program_options log log_setup REQUIRED)
find_package(OGRE REQUIRED COMPONENTS Bites Overlay CONFIG)
find_package(OpenGL REQUIRED)
find_package(FreeType REQUIRED)
set(OgreProcedural_DIR ${OGRE_PROCEDURAL_DIR})
list(APPEND CMAKE_MODULE_PATH ${OgreProcedural_DIR})
find_package(OgreProcedural REQUIRED)

# #####################################################################
# Compile definitions
# #####################################################################
message("Archv definitions:")
message("Found ARCHV_RENDERING_CONFIG_PATH: " ${ARCHV_RENDERING_CONFIG_PATH})
message("Found ARCHV_GUI_CONFIG_PATH: " ${ARCHV_GUI_CONFIG_PATH})
message("Found ARCHV_WEIGHTS_CONFIG_PATH: " ${ARCHV_WEIGHTS_CONFIG_PATH})
message("Found ARCHV_LAYOUT_CONFIG_PATH: " ${ARCHV_LAYOUT_CONFIG_PATH})
message("Found ARCHV_SCALING_CONFIG_PATH: " ${ARCHV_SCALING_CONFIG_PATH})
message("Found ARCHV_CLUSTERING_CONFIG_PATH: " ${ARCHV_CLUSTERING_CONFIG_PATH})
message("Found ARCHV_COLOR_CODING_CONFIG_PATH: " ${ARCHV_COLOR_CODING_CONFIG_PATH})
message("Found ARCHV_DEGREES_CONFIG_PATH: " ${ARCHV_DEGREES_CONFIG_PATH})
message("Found ARCHV_RESOURCE_GROUP: " ${ARCHV_RESOURCE_GROUP})
message("Found OGRE_PROCEDURAL_DIR: " ${OGRE_PROCEDURAL_DIR})
message(" ")

target_compile_definitions(${PROJECT_LIB} PUBLIC ARCHV_CONFIG_DIR="${ARCHV_CONFIG_DIR}")
target_compile_definitions(${PROJECT_LIB} PUBLIC ARCHV_RENDERING_CONFIG_PATH="${ARCHV_RENDERING_CONFIG_PATH}")
target_compile_definitions(${PROJECT_LIB} PUBLIC ARCHV_GUI_CONFIG_PATH="${ARCHV_GUI_CONFIG_PATH}")
target_compile_definitions(${PROJECT_LIB} PUBLIC ARCHV_WEIGHTS_CONFIG_PATH="${ARCHV_WEIGHTS_CONFIG_PATH}")
target_compile_definitions(${PROJECT_LIB} PUBLIC ARCHV_LAYOUT_CONFIG_PATH="${ARCHV_LAYOUT_CONFIG_PATH}")
target_compile_definitions(${PROJECT_LIB} PUBLIC ARCHV_SCALING_CONFIG_PATH="${ARCHV_SCALING_CONFIG_PATH}")
target_compile_definitions(${PROJECT_LIB} PUBLIC ARCHV_CLUSTERING_CONFIG_PATH="${ARCHV_CLUSTERING_CONFIG_PATH}")
target_compile_definitions(${PROJECT_LIB} PUBLIC ARCHV_COLOR_CODING_CONFIG_PATH="${ARCHV_COLOR_CODING_CONFIG_PATH}")
target_compile_definitions(${PROJECT_LIB} PUBLIC ARCHV_DEGREES_CONFIG_PATH="${ARCHV_DEGREES_CONFIG_PATH}")
target_compile_definitions(${PROJECT_LIB} PUBLIC ARCHV_RESOURCE_GROUP="${ARCHV_RESOURCE_GROUP}")

# #####################################################################
# Include directories
# #####################################################################
set(OGRE_PROCEDURAL_INCLUDE_DIR ${OgreProcedural_INCLUDE_DIRS})
set(THIRD_PARTY_INCLUDE_DIRS ${OGRE_PROCEDURAL_INCLUDE_DIR})

include_directories(${THIRD_PARTY_INCLUDE_DIRS})
target_include_directories(${PROJECT_LIB} PUBLIC src)

# #####################################################################
# Linking
# #####################################################################
set(BOOST_LIBRARIES ${Boost_LOG_LIBRARY} ${Boost_LOG_SETUP_LIBRARY})
set(JSONCPP_LIBRARIES libjsoncpp.so)
set(STB_LIBRARIES libstb.so)
set(SDL2_LIBRARIES libSDL2.so)
set(OGRE_PROCEDURAL_LIBRARIES ${OgreProcedural_LIBRARIES})
set(INFOMAP_LIBRARIES infomap_lib)
set(THIRD_PARTY_LIBRARIES
  ${JSONCPP_LIBRARIES}
  ${OGRE_LIBRARIES}
  ${BOOST_LIBRARIES}
  ${SDL2_LIBRARIES}
  ${OPENGL_LIBRARIES}
  ${FREETYPE_LIBRARIES}
  ${STB_LIBRARIES}
  ${OGRE_PROCEDURAL_LIBRARIES}
  ${INFOMAP_LIBRARIES}
)

target_link_libraries(${PROJECT_LIB} PUBLIC ${THIRD_PARTY_LIBRARIES})

# #####################################################################
# App
# #####################################################################
set(APP_FILES src/main.cpp)
add_executable(${PROJECT_APP} ${APP_FILES})
target_link_libraries(${PROJECT_APP} PUBLIC ${PROJECT_LIB})
