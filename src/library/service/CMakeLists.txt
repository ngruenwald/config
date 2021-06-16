
add_subdirectory(http)
add_subdirectory(ssdp)

add_library(
  lib-service
  OBJECT
  i_service.hpp
  local_file_service.cpp
  local_file_service.hpp
  http_service.cpp
  http_service.hpp
)

target_include_directories(
  lib-service
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

install(
  FILES
    i_service.hpp
    local_file_service.hpp
    http_service.hpp
  DESTINATION
    ${HEADER_INSTALL_PATH}/service
)
