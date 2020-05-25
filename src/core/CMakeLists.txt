set(xsimd_define "")

if(Ubpa_USE_XSIMD)
  list(APPEND xsimd_define UBPA_USE_XSIMD)
endif()

if(Ubpa_USE_SSE_4_1)
  list(APPEND xsimd_define UBPA_USE_SSE_4_1)
endif()

Ubpa_AddTarget(
  MODE HEAD
  LIB Ubpa::UTemplate_core
  DEFINE ${xsimd_define}
  INC include
)
