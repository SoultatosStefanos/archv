set(FILES 
	graph_visualizer.hpp
)
set(SUBDIRECTORIES 
	clustering
)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_NAME} PUBLIC ${FILES})
endif()