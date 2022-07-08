set(FILES)
set(SUBDIRECTORIES communication layout)

foreach(VAR ${SUBDIRECTORIES})
  add_subdirectory(${VAR})
endforeach()

if(FILES)
  target_sources(${TESTS} PRIVATE ${FILES})
endif()