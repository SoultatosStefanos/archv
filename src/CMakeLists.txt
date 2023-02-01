set(FILES)
set(SUBDIRECTORIES
	application
	architecture
	autocompletion
	clustering
	color_coding
	config
	degrees
	gui
	input
	layout
	misc
	presentation
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
