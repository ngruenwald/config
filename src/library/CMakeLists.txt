
#
# Setup install paths
#

if(WIN32)
  set(
    ORIGINAL_INSTALL_PREFIX
    binaries
  )

  mark_as_advanced(
    ORIGINAL_INSTALL_PREFIX
  )

  set(CMAKE_INSTALL_PREFIX "${ORIGINAL_INSTALL_PREFIX}" CACHE STRING "Install path" FORCE)

  set(LIBRARY_INSTALL_PATH ${PROJECT_NAME}/V_${PROJECT_VERSION_STRING}/lib)
  set(HEADER_INSTALL_ROOT  ${PROJECT_NAME}/V_${PROJECT_VERSION_STRING}/include)
  set(HEADER_INSTALL_PATH  ${HEADER_INSTALL_ROOT}/${PROJECT_NAME_PREFIX}${PROJECT_NAME})
  set(CONFIG_INSTALL_PATH  cmake/${PROJECT_NAME}-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION.PATCH})

  message(STATUS "Set install prefix to ${CMAKE_INSTALL_PREFIX}")
else()
  if (${CMAKE_SIZEOF_VOID_P} EQUAL 8)
    set(LIBSUFFIX 64)
  else()
    set(LIBSUFFIX "")
  endif()

  set(LIBRARY_INSTALL_PATH lib${LIBSUFFIX}/${PROJECT_NAME}-${PROJECT_VERSION})
  set(HEADER_INSTALL_ROOT include/${PROJECT_NAME}-${PROJECT_VERSION})
  set(HEADER_INSTALL_PATH ${HEADER_INSTALL_ROOT}/${PROJECT_NAME_PREFIX}${PROJECT_NAME})
  set(CONFIG_INSTALL_PATH ${LIBRARY_INSTALL_PATH}/cmake)
endif()

#
# Sub-projects
#

add_subdirectory(cache)
add_subdirectory(common)
add_subdirectory(config)
add_subdirectory(discoverer)
add_subdirectory(service)

#
# version
#

configure_file(
  library_version.cpp.in
  library_version.cpp
)

configure_file(
  library_version.h.in
  library_version.h
)

#
# Target
#

set(target config)

add_library(
  ${target}
  STATIC
  library.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/library_version.cpp
  $<TARGET_OBJECTS:lib-common>
  $<TARGET_OBJECTS:lib-cache>
  $<TARGET_OBJECTS:lib-discoverer>
  $<TARGET_OBJECTS:lib-service>
  $<TARGET_OBJECTS:lib-config>
)

add_library(
  ng::${target}
  ALIAS
  ${target}
)

target_include_directories(
  ${target}
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${HEADER_INSTALL_PATH}>
)

target_link_Libraries(
  ${target}
  curl::curl
)

if(WIN32)
  set_target_properties(
    ${target}
    PROPERTIES
      RELEASE_POSTFIX "_static_${COMPILER_POSTFIX}"
      RELWITHDEBINFO_POSTFIX "_static_${COMPILER_POSTFIX}"
      DEBUG_POSTFIX "_static_${COMPILER_POSTFIX}d"
  )
else()
  set_target_properties(
    ${target}
    PROPERTIES
      RELEASE_POSTFIX "-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
      RELWITHDEBINFO_POSTFIX "-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
      DEBUG_POSTFIX "-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}d"
  )
endif()

#
# Install files
#

install(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/library_version.h
  DESTINATION
    ${HEADER_INSTALL_PATH}/config
)

install(
  TARGETS ${target}
  EXPORT  ${PROJECT_NAME}Targets
  RUNTIME DESTINATION ${LIBRARY_INSTALL_PATH}
  LIBRARY DESTINATION ${LIBRARY_INSTALL_PATH}
  ARCHIVE DESTINATION ${LIBRARY_INSTALL_PATH}
)

#
# Export project information
#

install(
  EXPORT
    ${PROJECT_NAME}Targets
  FILE
    ${PROJECT_NAME}Targets.cmake
  NAMESPACE
    ng::
  DESTINATION
    ${CONFIG_INSTALL_PATH}
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${PROJECT_NAME}ConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)
install(
  FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  DESTINATION
    ${CONFIG_INSTALL_PATH}
)

