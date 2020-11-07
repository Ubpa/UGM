if(NOT Ubpa_UGM_UseSIMD)
  return()
endif()

Ubpa_GetTargetName(core "${PROJECT_SOURCE_DIR}/src/core")
Ubpa_AddTarget(
  TEST
  MODE EXE
  LIB
    ${core}
)
