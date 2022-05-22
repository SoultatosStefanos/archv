set(EXTERNAL_TRGTS gtest nanobench)
set(EXTERNAL_SUBDIRECTORIES googletest nanobench)

foreach(TRGT DIRECTORY IN ZIP_LISTS EXTERNAL_TRGTS EXTERNAL_SUBDIRECTORIES)
    if(NOT TARGET ${TRGT})
		add_subdirectory(${DIRECTORY})
	endif()
endforeach()