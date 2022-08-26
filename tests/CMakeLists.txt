set(FILES
  tests.cpp
)

find_package(GTest REQUIRED)

set(TESTS ${PROJECT}_tests)
set(TESTS_LIBS gtest gmock)

add_executable(${TESTS} ${FILES})

target_link_libraries(${TESTS} PRIVATE ${PROJECT_LIB} ${TESTS_LIBS})

set(SUBDIRECTORIES
  architecture
  dependencies
  layout
  progress
  undo_redo
  utility
)

foreach(VAR ${SUBDIRECTORIES})
  add_subdirectory(${VAR})
endforeach()