set(FILES 
	link.cpp
)
set(SUBDIRECTORIES )

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_NAME} PUBLIC ${FILES})
endif()

add_executable(main main.cpp)
target_link_libraries(main PUBLIC ${PROJECT_NAME})