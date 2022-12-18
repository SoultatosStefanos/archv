set(FILES)
set(SUBDIRECTORIES
	application
	architecture
	clustering
	color_coding
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
