set(FILES
	cluster_color_pool_tests.cpp
	config_tests.cpp
	degrees_ranked_backend_tests.cpp
	degrees_ranked_evaluation_tests.cpp
)
set(SUBDIRECTORIES)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${TESTS} PRIVATE ${FILES})
endif()