set(FILES
	camera_handler.cpp
	event_dispatcher.cpp
	hud_handler.cpp
	inspection_handler.cpp
	quit_handler.cpp
	shortcut_handler.cpp
	stiff_camera_handler.cpp
)
set(SUBDIRECTORIES)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_LIB} PUBLIC ${FILES})
endif()