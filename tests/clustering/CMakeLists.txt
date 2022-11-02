set(FILES
	backend_config_tests.cpp
	backend_tests.cpp
	cluster_map_tests.cpp
	clusterer_builder_tests.cpp
	config_tests.cpp
	k_spanning_tree_clustering_tests.cpp
	maximal_clique_enumeration_clustering_tests.cpp
	min_spanning_tree_finder_factory_tests.cpp
	plugin_tests.cpp
	shared_nearest_neighbour_clustering_tests.cpp
)
set(SUBDIRECTORIES)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${TESTS} PRIVATE ${FILES})
endif()