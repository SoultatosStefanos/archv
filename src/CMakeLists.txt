set(FILES)
set(SUBDIRECTORIES
	config
	dependencies
	features
	symbols
	utility
)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_NAME} PUBLIC ${FILES})
endif()
