cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(UGM VERSION 0.7.5)
message(STATUS "[Project] ${PROJECT_NAME}")

include(cmake/InitUCMake.cmake)
Ubpa_InitUCMake(VERSION 0.6.4)

Ubpa_InitProject()

option(Ubpa_UGM_UseSIMD "use SIMD" ON)
option(Ubpa_UGM_UseSSE_4_1 "use SSE 4.1" ON)
message(STATUS "Ubpa_UGM_UseSIMD    : ${Ubpa_UGM_UseSIMD}")
message(STATUS "Ubpa_UGM_UseSSE_4_1 : ${Ubpa_UGM_UseSSE_4_1}")

if(Ubpa_BuildTest_UGM)
  find_package(GTest QUIET)
  if(GTest_FOUND)
    message(NOTICE "GTest Found")
  else()
    message(NOTICE "GTest not Found, so we ignore some targets depending on GTest")
  endif()
endif()

Ubpa_AddDep(UTemplate 0.7.2)

Ubpa_AddSubDirsRec(include)
Ubpa_AddSubDirsRec(src)

Ubpa_Export(
  TARGET
  DIRECTORIES
    "include"
    "vs"
)
