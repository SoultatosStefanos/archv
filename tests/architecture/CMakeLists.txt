set(FILES
	config_tests.cpp
	metadata_tests.cpp
	symbol_table_tests.cpp
	vertex_marker_tests.cpp
)
set(SUBDIRECTORIES)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${TESTS} PRIVATE ${FILES})
endif()