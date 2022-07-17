set(FILES
	all.hpp
	flyweight.cpp
	flyweight.hpp
	path.hpp
	random.hpp
	undo_redo.cpp
	undo_redo.hpp
)
set(SUBDIRECTORIES)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_NAME} PUBLIC ${FILES})
endif()