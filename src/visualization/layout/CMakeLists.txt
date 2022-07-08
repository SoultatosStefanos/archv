set(FILES
	all.hpp
	controller.hpp
	core.cpp
	core.hpp
	layout_factory.cpp
	layout_factory.hpp
	layout.cpp
	layout.hpp
	presenter.hpp
	services.cpp
	services.hpp
	topology_factory.cpp
	topology_factory.hpp
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