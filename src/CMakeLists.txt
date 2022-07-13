set(FILES)
set(SUBDIRECTORIES
	architecture
	clustering
	generation
	layout
	utility
)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_NAME} PUBLIC ${FILES})
endif()

set(EXEC main)
set(EXEC_FILES main.cpp app.hpp app.cpp)

if(EXEC_FILES)
	add_executable(${EXEC} ${EXEC_FILES})
	target_link_libraries(${EXEC} PUBLIC ${PROJECT_NAME})
endif()