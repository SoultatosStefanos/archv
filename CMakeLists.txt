cmake_minimum_required(VERSION 3.13.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(archv CXX)

if(MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -pedantic)
endif()

add_library(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC src)

set(SUBDIRECTORIES src tests)

foreach(VAR ${SUBDIRECTORIES})
  add_subdirectory(${VAR})
endforeach()

set(JSONCPP_LIBS libjsoncpp.so)

find_package(OGRE REQUIRED COMPONENTS Bites CONFIG)

find_package(Boost COMPONENTS program_options log log_setup REQUIRED)

set(BOOST_LIBS ${Boost_LOG_LIBRARY} ${Boost_LOG_SETUP_LIBRARY})
set(MYGUI_LIBRARIES
  libMyGUIEngine.so
  libMyGUI.OgrePlatform.so
)

set(LIBS ${JSONCPP_LIBS} ${OGRE_LIBRARIES} ${BOOST_LIBS} ${MYGUI_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBS})

add_executable(archv_main src/main.cpp src/app.cpp)
target_link_libraries(archv_main PUBLIC ${PROJECT_NAME})