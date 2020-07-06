set(core_defines "")
set(core_c_options "")
set(sources "")

if(MSVC)
  list(APPEND core_c_options "/fp:fast;/permissive-")
  list(APPEND sources "${PROJECT_SOURCE_DIR}/vs/UGM.natvis")
endif()

if(Ubpa_USE_XSIMD)
  list(APPEND core_defines UBPA_USE_SIMD)
  if(Ubpa_USE_SSE_4_1)
    list(APPEND core_defines UBPA_USE_SSE_4_1)
  endif()
endif()

Ubpa_AddTarget(
  MODE INTERFACE
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
