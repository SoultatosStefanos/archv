set(FILES
	controller_tests.cpp
	factories_tests.cpp
	presenter_tests.cpp
	services_tests.cpp
	view_tests.cpp
)
set(SUBDIRECTORIES)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${TESTS} PRIVATE ${FILES})
endif()