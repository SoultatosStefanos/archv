set(FILES)
set(SUBDIRECTORIES
	architecture
	config
	gui
	layout
	misc
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
