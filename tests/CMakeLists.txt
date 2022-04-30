set(FILES tests.cpp)

set(TESTS ${PROJECT_NAME}_tests)
set(TESTS_LIBS gtest)

add_executable(${TESTS} ${FILES})
target_link_libraries(${TESTS} PRIVATE ${PROJECT_NAME} ${TESTS_LIBS})

set(SUBDIRECTORIES )

foreach(VAR ${SUBDIRECTORIES})
  add_subdirectory(${VAR})
endforeach()