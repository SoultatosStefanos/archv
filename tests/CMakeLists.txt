set(FILES 
  strong_components_clustering_tests.cpp
  k_spanning_tree_clustering_tests.cpp
  maximal_clique_enumeration_clustering_tests.cpp
  shared_nearest_neighbour_clustering_tests.cpp
  tests.cpp
)

set(TESTS ${PROJECT_NAME}_tests)
set(TESTS_LIBS gtest gmock)

add_executable(${TESTS} ${FILES})
target_link_libraries(${TESTS} PRIVATE ${PROJECT_NAME} ${TESTS_LIBS})

set(SUBDIRECTORIES )

foreach(VAR ${SUBDIRECTORIES})
  add_subdirectory(${VAR})
endforeach()