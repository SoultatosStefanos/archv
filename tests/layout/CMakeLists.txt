set(FILES
	backend_config_tests.cpp
	backend_tests.cpp
	config_tests.cpp
	layout_factory_tests.cpp
	plugin_tests.cpp
	position_map_tests.cpp
	topology_factory_tests.cpp
)
set(SUBDIRECTORIES)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${TESTS} PRIVATE ${FILES})
endif()