set(FILES
	visualization.hpp
)
set(SUBDIRECTORIES
	app
	layout
)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_NAME} PUBLIC ${FILES})
endif()