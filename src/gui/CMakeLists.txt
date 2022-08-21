set(FILES
	menu_bar.cpp
	menu_window.cpp
	overlay_manager.cpp
	progress_bar.cpp
)
set(SUBDIRECTORIES)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_NAME} PUBLIC ${FILES})
endif()