set(FILES
	backend_tests.cpp
	factor_repo_tests.cpp
	scale_map_tests.cpp
	scale_tests.cpp
)
set(SUBDIRECTORIES)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${TESTS} PRIVATE ${FILES})
endif()