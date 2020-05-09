cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(UGM VERSION 0.5.8)
message(STATUS "[Project] ${PROJECT_NAME}")

include(FetchContent)
message(STATUS "find package: UCMake 0.5.2")
find_package(UCMake 0.5.2 QUIET)
if(NOT UCMake_FOUND)
  message(STATUS "UCMake 0.5.2 not found")
  set(_address "https://github.com/Ubpa/UCMake")
  message(STATUS "fetch: ${_address} with tag 0.5.2")
  FetchContent_Declare(
    UCMake
    GIT_REPOSITORY ${_address}
    GIT_TAG "0.5.2"
  )
  FetchContent_MakeAvailable(UCMake)
  message(STATUS "UCMake 0.5.2 build done")
endif()

Ubpa_InitProject()

set(Ubpa_USE_XSIMD TRUE CACHE BOOL "use xsimd")

Ubpa_AddDep(UTemplate 0.4.2)

Ubpa_AddSubDirsRec(include)
Ubpa_AddSubDirsRec(src)

Ubpa_Export(DIRECTORIES "include")
