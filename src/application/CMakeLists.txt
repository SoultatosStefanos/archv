set(FILES
	app_backends.cpp
	app_connections.cpp
	app_frontends.cpp
	app_input.cpp
	app.cpp
	background_presenter.cpp
	commands.cpp
	graph_interface.cpp
	graph_presenter.cpp
	shortcut_input_listener.cpp
)
set(SUBDIRECTORIES)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_LIB} PUBLIC ${FILES})
endif()