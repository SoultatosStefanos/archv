set(FILES
	background_configurator.cpp
	clustering_editor.cpp
	color_coding_editor.cpp
	config.cpp
	controls_hud.cpp
	degrees_editor.cpp
	frames_hud.cpp
	graph_configurator.cpp
	gui_config.cpp
	gui_configurator.cpp
	layout_editor.cpp
	menu_bar.cpp
	minimap_configurator.cpp
	overlay_manager.cpp
	plugins.cpp
	popup_holder.cpp
	resources.cpp
	scaling_editor.cpp
	structure_popup.cpp
	ui_adaptor.cpp
	weights_editor.cpp
)
set(SUBDIRECTORIES detail)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_LIB} PUBLIC ${FILES})
endif()