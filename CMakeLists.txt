cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(UGM VERSION 0.6.2)
message(STATUS "[Project] ${PROJECT_NAME}")

include(FetchContent)
set(UCMake_VERSION 0.5.5)
message(STATUS "find package: UCMake ${UCMake_VERSION}")
find_package(UCMake ${UCMake_VERSION} QUIET)
if(NOT UCMake_FOUND)
  message(STATUS "UCMake ${UCMake_VERSION} not found")
  set(_address "https://github.com/Ubpa/UCMake")
  message(STATUS "fetch: ${_address} with tag ${UCMake_VERSION}")
  FetchContent_Declare(
    UCMake
    GIT_REPOSITORY ${_address}
    GIT_TAG ${UCMake_VERSION}
  )
  FetchContent_MakeAvailable(UCMake)
  message(STATUS "UCMake ${UCMake_VERSION} build done")
endif()

Ubpa_InitProject()

set(Ubpa_USE_SIMD TRUE CACHE BOOL "use simd")
set(Ubpa_USE_SSE_4_1 TRUE CACHE BOOL "use sse 4.1")

Ubpa_AddDep(UTemplate 0.4.5)

Ubpa_AddSubDirsRec(include)
Ubpa_AddSubDirsRec(src)

Ubpa_Export(DIRECTORIES "include")
