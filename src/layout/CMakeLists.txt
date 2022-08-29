set(FILES
	config.cpp
	core.cpp
	cube.cpp
	sphere.cpp
)
set(SUBDIRECTORIES)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_LIB} PUBLIC ${FILES})
endif()