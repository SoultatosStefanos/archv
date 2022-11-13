set(FILES
	background_configurator.cpp
	clustering_editor.cpp
	config.cpp
	controls_hud.cpp
	degrees_editor.cpp
	frames_hud.cpp
	graph_configurator.cpp
	gui_config.cpp
	gui_configurator.cpp
	gui.cpp
	layout_editor.cpp
	overlay_manager.cpp
	plugins.cpp
	resources.cpp
	scaling_editor.cpp
	weights_editor.cpp
)
set(SUBDIRECTORIES detail)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_LIB} PUBLIC ${FILES})
endif()