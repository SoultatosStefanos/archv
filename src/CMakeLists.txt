set(FILES)
set(SUBDIRECTORIES

	# application
	architecture
	config
	dependencies
	gui
	layout
	multithreading
	progress
	rendering
	ui
	undo_redo
)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_LIB} PUBLIC ${FILES})
endif()
