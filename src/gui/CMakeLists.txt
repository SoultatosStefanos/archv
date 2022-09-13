set(FILES
	core.cpp
	background_configurator.cpp
	graph_configurator.cpp
	gui_configurator.cpp
	menu_bar.cpp
	menu_window.cpp
	overlay_manager.cpp
	plugins.cpp
	resources.cpp
)
set(SUBDIRECTORIES)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_LIB} PUBLIC ${FILES})
endif()