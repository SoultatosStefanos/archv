set(FILES
	background_configurator.cpp
	clustering_editor.cpp
	config.cpp
	degrees_editor.cpp
	graph_configurator.cpp
	gui_config.cpp
	gui_configurator.cpp
	gui.cpp
	layout_editor.cpp
	plugins.cpp
	resources.cpp
	scaling_editor.cpp
	structure_dialog_manager.cpp
	weights_editor.cpp
)
set(SUBDIRECTORIES detail)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_LIB} PUBLIC ${FILES})
endif()