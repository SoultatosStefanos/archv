set(FILES)
set(SUBDIRECTORIES
	application
	architecture
	config
	degrees
	dependencies
	gui
	layout
	misc
	multithreading
	progress
	rendering
	scaling
	ui
	undo_redo
)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_LIB} PUBLIC ${FILES})
endif()
