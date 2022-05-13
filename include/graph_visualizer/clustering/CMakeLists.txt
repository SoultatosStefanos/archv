set(FILES 
	clustering.hpp
	k_spanning_tree_clustering.hpp
	minimum_spanning_tree.hpp
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