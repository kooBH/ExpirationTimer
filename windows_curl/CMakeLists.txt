# CMAKE version of Ubuntu 20.04 is 3.16
cmake_minimum_required(VERSION 3.16)
project(exp_curl)

set(SRC)
set(INCL)
set(LINKLIBS)
set(COMPILE_OPTION)

list(APPEND SRC
    exp_curl.h
    test.cpp
  )
list(APPEND INCL
  ${CMAKE_CURRENT_LIST_DIR}/.
  ${CMAKE_CURRENT_LIST_DIR}/libcurl
  ${CMAKE_CURRENT_LIST_DIR}/libcurl/curl
  ${CMAKE_CURRENT_LIST_DIR}/libcurl/openssl
  )
list(APPEND LINKLIBS
${CMAKE_CURRENT_LIST_DIR}/build/libcurl.lib
  )
if(MSVC)
  list(APPEND COMPILE_OPTION
  -DCURL_STATICLIB
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