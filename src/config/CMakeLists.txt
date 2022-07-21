set(FILES
	all.hpp
	arch_deserialization.cpp
	arch_deserialization.hpp
	archives.cpp
	archives.hpp
	error.hpp
)
set(SUBDIRECTORIES)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_NAME} PUBLIC ${FILES})
endif()