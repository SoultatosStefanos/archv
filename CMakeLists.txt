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

# make the src/ directory visible for include directives
target_include_directories(${PROJECT_LIB} PUBLIC src)

# find cmake configs locally
find_package(Boost COMPONENTS program_options log log_setup REQUIRED)
find_package(OGRE REQUIRED COMPONENTS Bites Overlay CONFIG)
find_package(OpenGL REQUIRED)
find_package(FreeType REQUIRED)

set(OgreProcedural_DIR "/usr/local/lib/OgreProcedural/cmake")
list(APPEND CMAKE_MODULE_PATH "/usr/local/lib/OgreProcedural/cmake")
find_package(OgreProcedural REQUIRED)

# define 3rd party targets
set(BOOST_LIBRARIES ${Boost_LOG_LIBRARY} ${Boost_LOG_SETUP_LIBRARY})
set(JSONCPP_LIBRARIES libjsoncpp.so)
set(IMGUI_LIBRARIES libimgui.a)
set(STB_LIBRARIES libstb.so)
set(SDL2_LIBRARIES libSDL2.so)
set(OGRE_PROCEDURAL_LIBRARIES ${OgreProcedural_LIBRARIES})

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
  ${OGRE_PROCEDURAL_LIBRARIES}
)

# link 3rd party libraries
target_link_libraries(${PROJECT_LIB} PUBLIC ${THIRD_PARTY_LIBRARIES})

set(IMGUI_INCLUDE_DIR "/usr/include/imgui")
set(OGRE_PROCEDURAL_INCLUDE_DIR ${OgreProcedural_INCLUDE_DIRS})

# define 3rd include directories
set(THIRD_PARTY_INCLUDE_DIRS
  ${IMGUI_INCLUDE_DIR}
  ${OGRE_PROCEDURAL_INCLUDE_DIR}
)

# include 3rd party directories globally
include_directories(${THIRD_PARTY_INCLUDE_DIRS})

# setup driver target
set(APP_FILES src/main.cpp)

if(APP_FILES)
  add_executable(${PROJECT_APP} ${APP_FILES})

  # link driver with public library
  target_link_libraries(${PROJECT_APP} PUBLIC ${PROJECT_LIB})
endif()
