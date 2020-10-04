cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(UGM VERSION 0.6.12)
message(STATUS "[Project] ${PROJECT_NAME}")

include(cmake/InitUCMake.cmake)
Ubpa_InitUCMake()

Ubpa_InitProject()

set(Ubpa_USE_SIMD TRUE CACHE BOOL "use simd")
set(Ubpa_USE_SSE_4_1 TRUE CACHE BOOL "use sse 4.1")

Ubpa_AddDep(UTemplate 0.4.8)

Ubpa_AddSubDirsRec(include)
Ubpa_AddSubDirsRec(src)

Ubpa_Export(
  TARGET
  DIRECTORIES
    "include"
    "vs"
)
