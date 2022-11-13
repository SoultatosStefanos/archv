set(FILES
	camera_handler.cpp
	hud_handler.cpp
	inspection_handler.cpp
	pause_resume_handler.cpp
	quit_handler.cpp
	shortcut_handler.cpp
)
set(SUBDIRECTORIES)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_LIB} PUBLIC ${FILES})
endif()