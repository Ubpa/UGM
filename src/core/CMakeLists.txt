set(core_defines "")
set(core_c_options "")
set(sources "")

if(MSVC)
  list(APPEND core_c_options "/fp:fast;/permissive-")
  list(APPEND sources "${PROJECT_SOURCE_DIR}/vs/UGM.natvis")
endif()

if(NOT Ubpa_UGM_UseSIMD)
  list(APPEND core_defines UBPA_UGM_NOT_USE_SIMD)
elseif(NOT Ubpa_UGM_UseSSE_4_1)
  list(APPEND core_defines UBPA_UGM_NOT_USE_SSE_4_1)
endif()

Ubpa_AddTarget(
  MODE INTERFACE
  RET_TARGET_NAME targetName
  SOURCE
    ${sources}
  DEFINE
    ${core_defines}
  C_OPTION
    ${core_c_options}
  INC
    "${PROJECT_SOURCE_DIR}/include"
  LIB
    Ubpa::UTemplate_core
)