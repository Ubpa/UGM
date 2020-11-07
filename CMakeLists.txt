cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(UGM VERSION 0.6.13)
message(STATUS "[Project] ${PROJECT_NAME}")

include(cmake/InitUCMake.cmake)
Ubpa_InitUCMake()

Ubpa_InitProject()

option(Ubpa_UGM_UseSIMD "use SIMD" TRUE)
option(Ubpa_UGM_UseSSE_4_1 "use SSE 4.1" TRUE)

Ubpa_AddDep(UTemplate 0.4.9)

Ubpa_AddSubDirsRec(include)
Ubpa_AddSubDirsRec(src)

Ubpa_Export(
  TARGET
  DIRECTORIES
    "include"
    "vs"
)
