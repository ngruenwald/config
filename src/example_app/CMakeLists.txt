
add_executable(
  example_app
  main.cpp
  app_config.cpp
  app_config.hpp
)

target_link_libraries(
  example_app
  config
  pugixml::pugixml
)

target_link_libraries(
  example_app
  stdc++fs
)

target_include_directories(
  example_app
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../library
)

configure_file(
  example_app.fallback.xml
  example_app.fallback.xml
  COPYONLY
)
