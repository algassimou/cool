cmake_minimum_required(VERSION 2.6)

find_package(FLEX)
find_package(BISON)

BISON_TARGET(
  MyParser 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.y 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.c
)

file(
  GLOB_RECURSE
  SRC_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*
)

include_directories(include src)

set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE}/)

add_executable(
  compile
  ${SRC_FILES}
  ${BISON_MyParser_OUTPUTS}
)

target_link_libraries(compile ${BISON_LIBRARIES})

