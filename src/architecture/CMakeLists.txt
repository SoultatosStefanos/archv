set(FILES
	architecture.hpp
	graph_generation.hpp
	graph_generation.cpp
	graph_output.cpp
	graph_output.hpp
	graph.hpp
)
set(SUBDIRECTORIES)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_NAME} PUBLIC ${FILES})
endif()