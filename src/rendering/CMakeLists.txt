set(FILES
	background_renderer.cpp
	config.cpp
	degrees_backend.cpp
	degrees_evaluation.cpp
	graph_renderer.cpp
)
set(SUBDIRECTORIES detail)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_LIB} PUBLIC ${FILES})
endif()