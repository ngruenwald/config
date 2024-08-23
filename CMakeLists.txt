cmake_minimum_required(VERSION 3.20)

project(
  config
  VERSION 1.0.0
  DESCRIPTION "config library"
  LANGUAGES CXX
)

option(CONFIG_UNIT_TESTS "Build the unit tests" ON)
option(CONFIG_EXTRAS "Build some extra stuff" ON)

add_subdirectory(contrib)
add_subdirectory(src)
