find_library(IOKIT_LIBRARY IOKit)

list(APPEND SRC 
  ${CMAKE_CURRENT_LIST_DIR}/apple_uuid.h
  ${CMAKE_CURRENT_LIST_DIR}/apple_uuid.cpp
	)
list(APPEND INCL
	${CMAKE_CURRENT_LIST_DIR}
)