set(FILES
	strong_components_clustering_tests.cpp
	k_spanning_tree_clustering_tests.cpp
	maximal_clique_enumeration_clustering_tests.cpp
	shared_nearest_neighbour_clustering_tests.cpp
)
set(SUBDIRECTORIES )

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${TESTS} PRIVATE ${FILES})
endif()