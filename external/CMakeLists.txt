set(SUBDIRECTORIES
	infomap
)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

set(ICON_FONT_INCLUDE_DIR IconFontCppHeaders)
set(INFOMAP_INCLUDE_DIR infomap/src)

target_include_directories(${PROJECT_LIB} PUBLIC ${ICON_FONT_INCLUDE_DIR} ${INFOMAP_INCLUDE_DIR})

set(INFOMAP_LIB infomap_lib)
file(GLOB_RECURSE infomap_src CONFIGURE_DEPENDS "infomap/src/*.h" "infomap/src/*.cpp")
add_library(${INFOMAP_LIB} ${infomap_src})