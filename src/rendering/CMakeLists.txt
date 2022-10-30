set(FILES
	background_config.cpp
	background_renderer.cpp
	cluster_color_coder.cpp
	config.cpp
	degrees_backend.cpp
	degrees_evaluation.cpp
	graph_config.cpp
)
set(SUBDIRECTORIES detail)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_LIB} PUBLIC ${FILES})
endif()