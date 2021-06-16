
add_executable(
  unit_tests
  main.cpp
  tests-static_service_discoverer.cpp
#  tests-cache_backend_xml.cpp
  tests-config.cpp
  tests-common.cpp
  tests-http_service.cpp
)

target_link_libraries(
  unit_tests
  config
  Catch2::Catch2
  pugixml::pugixml
)

target_link_libraries(
  unit_tests
  stdc++fs
)

target_include_directories(
  unit_tests
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../library
)

configure_file(
  test_config.hpp.in
  test_config.hpp
)

configure_file(
  data/index.xml.in
  index.xml
)

configure_file(
  data/config.xml.in
  config.xml
)

file(
  MAKE_DIRECTORY
  "${CMAKE_CURRENT_BINARY_DIR}/configs/dumpy/E000/1.1"
)

configure_file(
  data/config.xml.in
  "${CMAKE_CURRENT_BINARY_DIR}/configs/dumpy/E000/1.1/dumpy.xml"
)
