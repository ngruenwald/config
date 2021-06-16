
add_library(
  lib-ssdp
  OBJECT
  ssdp.hpp
  ssdp.cpp
)

target_include_directories(
  lib-ssdp
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

