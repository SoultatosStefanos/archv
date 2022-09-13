set(FILES
	background_configurator.cpp
	configurator.cpp
	dependencies_editor.cpp
	editor.cpp
	graph_configurator.cpp
	gui_configurator.cpp
	gui.cpp
	layout_editor.cpp
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