cmake_minimum_required(VERSION 3.13.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(archv CXX)

# compiler flags
if(MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -pedantic)
endif()

add_library(${PROJECT_NAME})

set(SUBDIRECTORIES src tests)

foreach(VAR ${SUBDIRECTORIES})
  add_subdirectory(${VAR})
endforeach()

# make the src/ directory visible for include directives, as well as the tests/ (for testing utilites)
target_include_directories(${PROJECT_NAME} PUBLIC src tests)

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

# link 3rd party libraries
target_link_libraries(${PROJECT_NAME} PUBLIC ${THIRD_PARTY_LIBRARIES})

# setup driver target
set(DRIVER archv_main)
#set(DRIVER_FILES src/main.cpp src/app.cpp)

if(DRIVER_FILES)
  add_executable(${DRIVER} ${DRIVER_FILES})

  # link driver with public library
  target_link_libraries(${DRIVER} PUBLIC ${PROJECT_NAME})
endif()
