
add_library(
  lib-config
  OBJECT
  i_config.hpp
  i_config_reader.hpp
  config_provider.cpp
  config_provider.hpp
)

target_include_directories(
  lib-config
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

target_link_libraries(
  lib-config
  PUBLIC
  pugixml::pugixml
)

install(
  FILES
    i_config.hpp
    i_config_reader.hpp
    config_provider.hpp
  DESTINATION
    ${HEADER_INSTALL_PATH}/config
)
