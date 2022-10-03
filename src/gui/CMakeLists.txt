set(FILES
	background_configurator.cpp
	config.cpp
	configurator.cpp
	dependencies_editor.cpp
	editor.cpp
	file_browser.cpp
	graph_configurator.cpp
	gui_configurator.cpp
	gui.cpp
	layout_editor.cpp
	plugins.cpp
	resources.cpp
	scaling_editor.cpp
)
set(SUBDIRECTORIES)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_LIB} PUBLIC ${FILES})
endif()