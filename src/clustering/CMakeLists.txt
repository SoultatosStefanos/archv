set(FILES
	all.hpp
	strong_components_clustering.hpp
	k_spanning_tree_clustering.hpp
	maximal_clique_enumeration_clustering.hpp
	shared_nearest_neighbour_clustering.hpp
)
set(SUBDIRECTORIES)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_NAME} PUBLIC ${FILES})
endif()