set(FILES
	background_config.cpp
	background_renderer.cpp
	cluster_color_pool.cpp
	config.cpp
	degrees_ranked_backend.cpp
	degrees_ranked_evaluation.cpp
	graph_config.cpp
)
set(SUBDIRECTORIES detail)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_LIB} PUBLIC ${FILES})
endif()