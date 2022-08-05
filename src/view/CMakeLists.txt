set(FILES
	paused_state.cpp
	state_event_dispatcher.cpp
	state_machine.cpp
	running_state.cpp
)
set(SUBDIRECTORIES)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_NAME} PUBLIC ${FILES})
endif()