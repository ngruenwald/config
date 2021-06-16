
add_library(
  lib-http
  OBJECT
  http.hpp
  http.cpp
)

target_include_directories(
  lib-http
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)
