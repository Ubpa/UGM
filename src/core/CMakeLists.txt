Ubpa_AddTarget(MODE "HEAD" INTERFACE_INC ON LIBS "Ubpa::UTemplate_core" INC_DIRS "_deps")

if("${Ubpa_USE_XSIMD}" STREQUAL "ON")
	Ubpa_GetTargetName(targetname ${CMAKE_CURRENT_SOURCE_DIR})
	target_compile_definitions(${targetname} INTERFACE UBPA_USE_XSIMD)
endif()
