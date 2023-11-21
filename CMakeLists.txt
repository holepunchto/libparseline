cmake_minimum_required(VERSION 3.22)

project(parseline C)

add_library(parseline OBJECT)

set_target_properties(
  parseline
  PROPERTIES
  C_STANDARD 99
  POSITION_INDEPENDENT_CODE ON
)

target_sources(
  parseline
  INTERFACE
    include/parseline.h
  PRIVATE
    src/parseline.c
)

add_library(parseline_shared SHARED $<TARGET_OBJECTS:parseline>)

set_target_properties(
  parseline_shared
  PROPERTIES
  OUTPUT_NAME parseline
  WINDOWS_EXPORT_ALL_SYMBOLS ON
)

add_library(parseline_static STATIC $<TARGET_OBJECTS:parseline>)

set_target_properties(
  parseline_static
  PROPERTIES
  OUTPUT_NAME parseline
  PREFIX lib
)

install(TARGETS parseline_shared parseline_static)

install(FILES include/parseline.h DESTINATION include)

if(PROJECT_IS_TOP_LEVEL)
  enable_testing()
  add_subdirectory(test)
endif()
