set(FILES
	backend_tests.cpp
	config_tests.cpp
	weight_map_tests.cpp
	weight_repo_tests.cpp
)
set(SUBDIRECTORIES)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${TESTS} PRIVATE ${FILES})
endif()