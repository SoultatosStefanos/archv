set(FILES
  tests.cpp
)

find_package(GTest REQUIRED)

set(TESTS ${PROJECT_NAME}_tests)
set(TESTS_LIBS gtest gmock)

add_executable(${TESTS} ${FILES})

target_link_libraries(${TESTS} PRIVATE ${PROJECT_NAME} ${TESTS_LIBS})

set(SUBDIRECTORIES
  features
  utility
)

foreach(VAR ${SUBDIRECTORIES})
  add_subdirectory(${VAR})
endforeach()