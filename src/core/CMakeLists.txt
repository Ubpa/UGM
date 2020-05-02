if(Ubpa_USE_XSIMD)
  set(xsimd_define UBPA_USE_XSIMD)
else()
  set(xsimd_define "")
endif()

Ubpa_AddTarget(
  MODE HEAD
  LIB Ubpa::UTemplate_core
  DEFINE ${xsimd_define}
  INC include
)
