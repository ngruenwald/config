
set(CMAKE_CXX_STANDARD 17)

#
# Common dependencies
#

find_package(CURL REQUIRED)
add_library(curl::curl SHARED IMPORTED)
set_target_properties(
  curl::curl
  PROPERTIES
  IMPORTED_LOCATION ${CURL_LIBRARIES}
  IMPORTED_IMPLIB ${CURL_LIBRARIES}
  INTERFACE_INCLUDE_DIRECTORIES ${CURL_INCLUDE_DIRS}
  INTERFACE_LINK_LIBRARIES ${CURL_LIBRARIES}
)

#
# Sub projects
#

add_subdirectory(library)

if(CONFIG_UNIT_TESTS)
  add_subdirectory(unit_tests)
endif()

if(CONFIG_EXTRAS)
  add_subdirectory(example_app)
endif()
