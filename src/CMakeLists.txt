set(FILES)
set(SUBDIRECTORIES
	architecture
	config
	degrees
	gui
	layout
	misc
	multithreading
	progress
	rendering
	scaling
	ui
	undo_redo
	weights
)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_LIB} PUBLIC ${FILES})
endif()
