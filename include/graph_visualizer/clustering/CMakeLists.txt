set(FILES 
	clustering.hpp
	highly_connected_components_clustering.hpp
	k_spanning_tree_clustering.hpp
	maximal_clique_enumeration_clustering.hpp
	maximal_clique_enumeration.hpp
	merge.hpp
	shared_nearest_neighbour_clustering.hpp
	shared_nearest_neighbour.hpp
)
set(SUBDIRECTORIES )

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_NAME} PUBLIC ${FILES})
endif()