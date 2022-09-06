cmake_minimum_required(VERSION 3.13.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(archv CXX)

set(PROJECT ${PROJECT_NAME})
set(PROJECT_LIB ${PROJECT}_lib)
set(PROJECT_APP ${PROJECT}_app)

# compiler flags
if(MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -pedantic)
endif()

add_library(${PROJECT_LIB})

set(SUBDIRECTORIES src tests)

foreach(VAR ${SUBDIRECTORIES})
  add_subdirectory(${VAR})
endforeach()

# provide config path
target_compile_definitions(${PROJECT_LIB} PUBLIC ARCHV_CONFIG_DIR="/home/stef/Documents/Projects/archv/data/config")

# make the src/ directory visible for include directives, as well as the tests/ (for testing utilites)
target_include_directories(${PROJECT_LIB} PUBLIC src tests)

# find cmake configs locally
find_package(Boost COMPONENTS program_options log log_setup REQUIRED)
find_package(OGRE REQUIRED COMPONENTS Bites Overlay CONFIG)
find_package(OpenGL REQUIRED)
find_package(FreeType REQUIRED)

# define 3rd party targets
set(BOOST_LIBRARIES ${Boost_LOG_LIBRARY} ${Boost_LOG_SETUP_LIBRARY})
set(JSONCPP_LIBRARIES libjsoncpp.so)
set(IMGUI_LIBRARIES libimgui.a)
set(STB_LIBRARIES libstb.so)
set(SDL2_LIBRARIES libSDL2.so)

# merge 3rd party targets
set(THIRD_PARTY_LIBRARIES
  ${JSONCPP_LIBRARIES}
  ${OGRE_LIBRARIES}
  ${BOOST_LIBRARIES}
  ${SDL2_LIBRARIES}
  ${IMGUI_LIBRARIES}
  ${OPENGL_LIBRARIES}
  ${FREETYPE_LIBRARIES}
  ${STB_LIBRARIES}
)

# define 3rd include directories
set(IMGUI_INCLUDE_DIR "/usr/include/imgui")

# include 3rd party directories globally
include_directories(${IMGUI_INCLUDE_DIR})

# link 3rd party libraries
target_link_libraries(${PROJECT_LIB} PUBLIC ${THIRD_PARTY_LIBRARIES})

# setup driver target
# set(APP_FILES src/application/main.cpp)
if(APP_FILES)
  add_executable(${PROJECT_APP} ${APP_FILES})

  # link driver with public library
  target_link_libraries(${PROJECT_APP} PUBLIC ${PROJECT_LIB})
endif()
