cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(cpp)

set(BUILD_SHARED_LIBS ON)

if(NOT DEFINED _DISABLE_INSTALLATION)
  # This variable is responsible for installation disabling.
  set(_DISABLE_INSTALLATION FALSE)

  # Replace install() with conditional installation.
  macro(install)
    if (NOT _DISABLE_INSTALLATION)
      _install(${ARGN})
    endif()
  endmacro()
endif()

set(INSTALL_GTEST OFF)

include(FetchContent)
include(GenerateExportHeader)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

enable_testing()
include(GoogleTest)

add_subdirectory(gol)

add_executable(app
    main.cpp
)

target_link_libraries(app gol)
