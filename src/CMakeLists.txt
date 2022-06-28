set(FILES
	json_manager.cpp
	json_manager.hpp
)
set(SUBDIRECTORIES
	architecture
	clustering
	generation
	visualization
)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_NAME} PUBLIC ${FILES})
endif()

set(EXEC main)

if(EXEC)
	add_executable(${EXEC} ${EXEC}.cpp)
	target_link_libraries(${EXEC} PUBLIC ${PROJECT_NAME})
endif()