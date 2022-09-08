set(FILES
	config.cpp
	cube.cpp
	sphere.cpp
	topology_factory.cpp
	topology_plugin.cpp
)
set(SUBDIRECTORIES)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_LIB} PUBLIC ${FILES})
endif()