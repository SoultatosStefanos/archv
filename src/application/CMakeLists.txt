set(FILES
	app.cpp
	config.cpp
	paused_state.cpp
	state_machine.cpp
	visualization_state.cpp
)
set(SUBDIRECTORIES)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_NAME} PUBLIC ${FILES})
endif()