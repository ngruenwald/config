
add_library(
  lib-common
  OBJECT
  file_error.hpp
  filter.cpp
  filter.hpp
  timestamp.hpp
  version.cpp
  version.hpp
)

target_include_directories(
  lib-common
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

install(
  FILES
    file_error.hpp
    filter.hpp
    timestamp.hpp
    version.hpp
  DESTINATION
    ${HEADER_INSTALL_PATH}/common
)
