set(FILES
	mygui_input.cpp
	mygui_manager.cpp
	overlay_manager.cpp
	pause_menu.cpp
	paused_state.cpp
)
set(SUBDIRECTORIES)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_NAME} PUBLIC ${FILES})
endif()