set(FILES
	config_tests.cpp
	gui_tests.cpp
	overlay_manager_tests.cpp
	structure_dialog_manager_tests.cpp
)
set(SUBDIRECTORIES)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${TESTS} PRIVATE ${FILES})
endif()