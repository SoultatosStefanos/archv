set(FILES a.cpp)
set(SUBDIRECTORIES )

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_NAME} PUBLIC ${FILES})
endif()

set(EXECUTABLES main)

foreach(VAR ${EXECUTABLES})
	add_executable(${VAR} ${VAR}.cpp)
	target_link_libraries(${VAR} PUBLIC ${PROJECT_NAME})
endforeach()
