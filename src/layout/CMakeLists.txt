set(FILES
	all.hpp
	factories.cpp
	factories.hpp
	layout.cpp
	layout.hpp
	presenter.hpp
	services.cpp
	services.hpp
	topology.hpp
	view.cpp
	view.hpp
)
set(SUBDIRECTORIES)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_NAME} PUBLIC ${FILES})
endif()