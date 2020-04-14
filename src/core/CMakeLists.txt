set(libs Ubpa::UTemplate_core)

if(TARGET xsimd)
	list(APPEND libs xsimd)
endif()

Ubpa_AddTarget(MODE "HEAD" INTERFACE_INC ON LIBS ${libs})

if(TARGET xsimd)
	message(STATUS "use xsimd")
	Ubpa_GetTargetName(targetname ${CMAKE_CURRENT_SOURCE_DIR})
	target_compile_definitions(${targetname} INTERFACE USE_XSIMD)
endif()
