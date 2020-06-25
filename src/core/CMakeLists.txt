set(core_define "")
set(core_c_option "")

if(MSVC)
  list(APPEND core_c_option "/fp:fast;/permissive-")
endif()

if(Ubpa_USE_XSIMD)
  list(APPEND core_define UBPA_USE_SIMD)
endif()

if(Ubpa_USE_SSE_4_1)
  list(APPEND core_define UBPA_USE_SSE_4_1)
endif()

Ubpa_AddTarget(
  MODE HEAD
  LIB Ubpa::UTemplate_core
  DEFINE ${core_define}
  C_OPTION ${core_c_option}
  INC include
)
