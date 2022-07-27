set(FILES
	all.hpp
	core.cpp
	core.hpp
	factories.cpp
	factories.hpp
	layout.cpp
	layout.hpp
	services.cpp
	services.hpp
	topology.hpp
)
set(SUBDIRECTORIES)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_NAME} PUBLIC ${FILES})
endif()