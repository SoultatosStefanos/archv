set(FILES)
set(SUBDIRECTORIES
	architecture
	config
	dependencies
	input
	layout
	rendering
	states
	utility
)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_NAME} PUBLIC ${FILES})
endif()
