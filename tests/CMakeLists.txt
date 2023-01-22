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
  autocompletion
  clustering
  color_coding
  gui
  layout
  misc
  presentation
  rendering
  scaling
  ui
  undo_redo
  utility
  weights
)

foreach(VAR ${SUBDIRECTORIES})
  add_subdirectory(${VAR})
endforeach()