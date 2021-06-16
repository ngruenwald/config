
add_library(
  lib-cache
  OBJECT
  i_cache.hpp
  xml_cache.cpp
  xml_cache.hpp
)

target_include_directories(
  lib-cache
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

add_dependencies(
  lib-cache
  pugixml::pugixml
)

install(
  FILES
    i_cache.hpp
    xml_cache.hpp
  DESTINATION
    ${HEADER_INSTALL_PATH}/cache
)
