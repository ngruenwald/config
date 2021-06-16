
add_library(
  lib-discoverer
  OBJECT
  i_service_discoverer.hpp
  static_service_discoverer.cpp
  static_service_discoverer.hpp
)

target_include_directories(
  lib-discoverer
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

target_link_libraries(
  lib-discoverer
  PUBLIC
  pugixml::pugixml
)

install(
  FILES
    i_service_discoverer.hpp
    static_service_discoverer.hpp
  DESTINATION
    ${HEADER_INSTALL_PATH}/discoverer
)
