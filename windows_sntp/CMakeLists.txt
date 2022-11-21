# CMAKE version of Ubuntu 20.04 is 3.16
cmake_minimum_required(VERSION 3.16)
project(exp_chrono)

set(SRC)
set(INCL)
set(LINKLIBS)
set(COMPILE_OPTION)

list(APPEND SRC
    test.cpp
    NTP_client.h
    NTP_client.cpp
  )
list(APPEND INCL
  ${CMAKE_CURRENT_LIST_DIR}/.
  )
list(APPEND LINKLIBS
  )
if(MSVC)
  list(APPEND COMPILE_OPTION
    )
else()
  list(APPEND COMPILE_OPTION
    )
endif()

add_executable(${PROJECT_NAME} ${SRC})
target_include_directories(${PROJECT_NAME}    PRIVATE    ${INCL}  )
target_link_libraries(${PROJECT_NAME} ${LINKLIBS})
target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILE_OPTION} )
set_target_properties(${PROJECT_NAME} PROPERTIES  CXX_STANDARD 14 )