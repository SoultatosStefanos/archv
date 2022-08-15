set(FILES)
set(SUBDIRECTORIES
	architecture
	config
	dependencies
	gui
	json
	layout
	rendering
	undo_redo
	view
)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_NAME} PUBLIC ${FILES})
endif()
