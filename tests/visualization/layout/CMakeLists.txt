set(FILES
	controller_tests.cpp
	layout_factory_tests.cpp
	presenter_tests.cpp
	topology_factory_tests.cpp
)
set(SUBDIRECTORIES)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${TESTS} PRIVATE ${FILES})
endif()