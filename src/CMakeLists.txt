set(FILES)
set(SUBDIRECTORIES
	application
	config
	dependencies
	gui
	layout
	metadata
	rendering
)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_LIB} PUBLIC ${FILES})
endif()
