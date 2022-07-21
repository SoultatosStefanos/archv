set(FILES)
set(SUBDIRECTORIES
	clustering
	inspection
	layout
	shared
	volume
	weight
)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_NAME} PUBLIC ${FILES})
endif()