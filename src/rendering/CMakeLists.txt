set(FILES
	background_config.cpp
	background_renderer.cpp
	config.cpp
	graph_config.cpp
	minimap_config.cpp
	minimap_renderer.cpp
)
set(SUBDIRECTORIES detail)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_LIB} PUBLIC ${FILES})
endif()